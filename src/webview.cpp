#include "webview.h"
#include "beveledbutton.h"

#include <LayerShellQt/Window>
#include <QApplication>
#include <QDesktopServices>
#include <QFile>
#include <QMargins>
#include <QMessageBox>
#include <QPushButton>
#include <QScreen>
#include <QStyle>
#include <QTextStream>
#include <QVBoxLayout>
#include <QWebChannel>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>
#include <QtGlobal>

WebView::WebView(QWidget *parent, const QUrl baseUrl) : QWebEngineView{parent} {
  setPage(new WebPage(this));
  setTosuBaseUrl(baseUrl);
  setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
  connect(this->page(), SIGNAL(loadFinished(bool)), this, SLOT(onLoaded(bool)));
}

WebView::~WebView() { delete this->page(); }

static QString loadInjectorTemplate() {
  static QString injectorTemplate;
  if (injectorTemplate.isEmpty()) {
    QFile file(":/scripts/tosu-overlay-injector.js");
    if (!file.open(QFile::ReadOnly)) {
      qWarning("Failed to open tosu-overlay-injector.js");
      return {};
    }
    QTextStream in(&file);
    injectorTemplate = in.readAll();
    file.close();
  }
  return injectorTemplate;
}

static double detectRefreshRate(const QWidget *widget) {
  constexpr double defaultRefreshRate = 60.0;
  auto *screen = widget != nullptr ? widget->screen() : QApplication::primaryScreen();
  if (screen == nullptr) {
    return defaultRefreshRate;
  }

  const auto refreshRate = screen->refreshRate();
  if (!qIsFinite(refreshRate) || refreshRate < 1.0) {
    return defaultRefreshRate;
  }

  return qBound(30.0, refreshRate, 360.0);
}

static QString animationFrameRateScript(double refreshRate) {
  return QStringLiteral(
             "if (typeof window.__tosuOverlaySetAnimationFrameRate === 'function') { "
             "window.__tosuOverlaySetAnimationFrameRate(%1); }")
      .arg(QString::number(refreshRate, 'f', 3));
}

static void installPageScript(QWebEnginePage *page, const QWidget *widget) {
  const auto injectorTemplate = loadInjectorTemplate();
  if (injectorTemplate.isEmpty()) {
    return;
  }

  auto injectorSource = injectorTemplate;
  injectorSource.replace("__TOSU_OVERLAY_MAX_FPS__", QString::number(detectRefreshRate(widget), 'f', 3));

  QWebEngineScript injector;
  injector.setName("tosu-overlay-injector");
  injector.setInjectionPoint(QWebEngineScript::DocumentCreation);
  injector.setRunsOnSubFrames(false);
  injector.setWorldId(QWebEngineScript::MainWorld);
  injector.setSourceCode(injectorSource);
  page->scripts().insert(injector);
}

void WebView::onLoaded(bool ok) {
  setVisible(ok);
  if (ok) {
    syncAnimationFrameRate();
    return;
  }
  if (!ok) {
    auto *msgBox = new QMessageBox(parentWidget());
    msgBox->setFixedSize(462, 125);
    msgBox->createWinId();
    if (auto layerShellWindow = QApplication::platformName() == "wayland" ? LayerShellQt::Window::get(msgBox->windowHandle()) : nullptr) {
      auto screen = msgBox->screen();
      int marginX = (screen->geometry().width() - msgBox->width()) / 2;
      int marginY = (screen->geometry().height() - msgBox->height()) / 2;
      layerShellWindow->setMargins(QMargins(marginX, marginY, marginX, marginY));
    }
    msgBox->setAutoFillBackground(true);
    msgBox->setIcon(QMessageBox::Icon::Critical);
    msgBox->setText(tr("Error connecting to tosu, is it running?"));
    msgBox->setTextInteractionFlags(Qt::NoTextInteraction);
    msgBox->update();

    auto yes = new BeveledButton(msgBox);
    auto no = new BeveledButton(msgBox);
    yes->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    no->setIcon(style()->standardIcon(QStyle::SP_BrowserStop));
    yes->setText(tr("Yes, reload the overlay"));
    no->setText(tr("No, close the overlay"));
    msgBox->addButton(yes, QMessageBox::YesRole);
    msgBox->addButton(no, QMessageBox::NoRole);

    connect(msgBox, SIGNAL(rejected()), parentWidget(), SLOT(onQuitRequested()));
    connect(msgBox, &QMessageBox::accepted, this, [this]() { reload(); });

    msgBox->show();
  }
}

void WebView::setTosuBaseUrl(const QUrl baseUrl) {
  this->baseUrl = baseUrl;
  QUrl overlayUrl = baseUrl.adjusted(QFlag(QUrl::RemovePath));
  overlayUrl.setPath("/api/ingame");
  this->setUrl(overlayUrl);
}

void WebView::syncAnimationFrameRate() {
  page()->runJavaScript(animationFrameRateScript(detectRefreshRate(this)));
}

void WebView::onKeyDown(QString key) {
  if (key == "Esc" || key == "Escape") {
    emit editingEnd();
  }
}

void WebView::onEditingStarted() { this->page()->runJavaScript("window.postMessage('editingStarted')"); }

void WebView::onEditingEnded() { this->page()->runJavaScript("window.postMessage('editingEnded')"); }

WebPage::WebPage(WebView *parent) : QWebEnginePage{parent} {
  this->setBackgroundColor(Qt::transparent);
  installPageScript(this, parent);
  QWebChannel *channel = new QWebChannel(parent);
  this->setWebChannel(channel);
  WebChannelObject *object = new WebChannelObject(parent);
  channel->registerObject("object", object);
  connect(object, SIGNAL(keyDown(QString)), parent, SLOT(onKeyDown(QString)));
}

void WebPage::javaScriptConsoleMessage(QWebEnginePage::JavaScriptConsoleMessageLevel level, const QString &message,
                                       int lineNumber, const QString &sourceID) {
  Q_UNUSED(level)
  Q_UNUSED(message);
  Q_UNUSED(lineNumber)
  Q_UNUSED(sourceID)
}

WebChannelObject::WebChannelObject(QObject *parent) : QObject{parent} {}

void WebChannelObject::onKeyDown(QString key) { emit this->keyDown(key); }
