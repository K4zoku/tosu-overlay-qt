#include "webview.h"

#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QPushButton>
#include <QTextStream>
#include <QWebChannel>

WebView::WebView(QWidget *parent, const QUrl baseUrl) : QWebEngineView{parent} {
  this->setPage(new WebPage(this));
  this->setTosuBaseUrl(baseUrl);
  connect(this->page(), SIGNAL(loadFinished(bool)), this, SLOT(onLoaded(bool)));
}

WebView::~WebView() { delete this->page(); }

static QString script;

static inline void inject(QWebEnginePage *page) {
  if (script.isEmpty()) {
    QFile file(":/scripts/tosu-overlay-injector.js");
    if (!file.open(QFile::ReadOnly)) {
      qWarning("Failed to open tosu-overlay-injector.js");
      return;
    }
    QTextStream in(&file);
    script = in.readAll();
    file.close();
  }
  page->runJavaScript(script);
}

void WebView::onLoaded(bool ok) {
  if (ok) {
    inject(page());
  } else {
    parentWidget()->hide();
    auto *msgBox = new QMessageBox(QMessageBox::Critical, tr("Error"), tr("Error connecting with tosu, is it running?"),
                                   QMessageBox::Yes | QMessageBox::No, this);
    msgBox->button(QMessageBox::Yes)->setText(tr("Yes, reload the overlay"));
    msgBox->button(QMessageBox::No)->setText(tr("No, close the overlay"));
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

void WebView::onKeyDown(QString key) {
  if (key == "Esc" || key == "Escape") {
    emit editingEnd();
  }
}

void WebView::onEditingStarted() { this->page()->runJavaScript("window.postMessage('editingStarted')"); }

void WebView::onEditingEnded() { this->page()->runJavaScript("window.postMessage('editingEnded')"); }

WebPage::WebPage(WebView *parent) : QWebEnginePage{parent} {
  this->setBackgroundColor(Qt::transparent);
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
