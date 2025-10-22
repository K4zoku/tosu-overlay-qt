#include "overlay.h"

#include <LayerShellQt/Window>
#include <QApplication>
#include <QBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QRegion>

using namespace LayerShellQt;

Overlay::Overlay(QWidget *parent) : QWidget(parent) {
  setAttribute(Qt::WA_TranslucentBackground);
  setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);

  auto *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

  connect(this, SIGNAL(toggleEditing()), this, SLOT(onEditingToggled()));
  connect(this, SIGNAL(editingStarted()), this, SLOT(onEditingStarted()));
  connect(this, SIGNAL(editingEnded()), this, SLOT(onEditingEnded()));
  connect(this, SIGNAL(toggleVisibility()), this, SLOT(onVisibilityToggled()));
  connect(this, SIGNAL(requestQuit()), this, SLOT(onQuitRequested()));

  webView = new WebView(this);
  connect(webView, SIGNAL(editingEnd()), this, SIGNAL(editingEnded()));
  connect(this, SIGNAL(editingStarted()), webView, SLOT(onEditingStarted()));
  connect(this, SIGNAL(editingEnded()), webView, SLOT(onEditingEnded()));

  layout->addWidget(this->webView);

  systemTray = new SystemTray(this);
  connect(systemTray, SIGNAL(toggleEditing()), this, SIGNAL(toggleEditing()));
  connect(systemTray, SIGNAL(toggleVisibility()), this, SIGNAL(toggleVisibility()));
  connect(systemTray, SIGNAL(requestQuit()), this, SIGNAL(requestQuit()));

  createWinId();

  connect(windowHandle(), SIGNAL(visibleChanged(bool)), systemTray, SLOT(onVisibleChange(bool)));
  connect(this, SIGNAL(editingStarted()), systemTray, SLOT(onEditingStarted()));
  connect(this, SIGNAL(editingEnded()), systemTray, SLOT(onEditingEnded()));

  systemTray->show();
  emit editingEnded();
}

void Overlay::setOverlayGeometry(QRect rect) {
  if (auto layerShellWindow = QApplication::platformName() == "wayland" ? Window::get(windowHandle()) : nullptr) {
    QScreen *screen = this->screen();
    int marginRight = screen->geometry().width() - (rect.x() + rect.width());
    int marginBottom = screen->geometry().height() - (rect.y() + rect.height());
    layerShellWindow->setMargins(QMargins(rect.x(), rect.y(), marginRight, marginBottom));
  }
  setGeometry(rect);
  update();
}

void Overlay::onOsuGeometryChanged(QRect rect) { setOverlayGeometry(rect); }

void Overlay::setTosuUrl(QUrl url) { webView->setTosuBaseUrl(url); }

void Overlay::initLayerShell() {
  if (auto layerShellWindow = QApplication::platformName() == "wayland" ? Window::get(windowHandle()) : nullptr) {
    layerShellWindow->setExclusiveZone(-1);
    layerShellWindow->setScope("tosu-overlay");
    layerShellWindow->setLayer(Window::LayerOverlay);
  }
}

void Overlay::onEditingToggled() {
  if (editing) {
    emit editingEnded();
  } else {
    emit editingStarted();
  }
}

void Overlay::onEditingStarted() {
  editing = true;
  auto window = windowHandle();
  if (QApplication::platformName() == "xcb") {
    window->setFlag(Qt::WindowTransparentForInput, false);
  } else {
    window->setMask(QRegion());
  }
  if (auto layerShellWindow = QApplication::platformName() == "wayland" ? Window::get(windowHandle()) : nullptr) {
    layerShellWindow->setKeyboardInteractivity(Window::KeyboardInteractivityExclusive);
  }
}

void Overlay::onEditingEnded() {
  editing = false;
  auto window = windowHandle();
  if (QApplication::platformName() == "xcb") {
    window->setFlag(Qt::WindowTransparentForInput, true);
  } else {
    window->setMask(QRegion(-1, -1, 1, 1));
  }
  if (auto layerShellWindow = QApplication::platformName() == "wayland" ? Window::get(windowHandle()) : nullptr) {
    layerShellWindow->setKeyboardInteractivity(Window::KeyboardInteractivityNone);
  }
}

void Overlay::onVisibilityToggled() { setVisible(!isVisible()); }

void Overlay::onQuitRequested() {
  QMetaObject::invokeMethod(QApplication::instance(), &QApplication::quit, Qt::QueuedConnection);
}
