#include "overlay.h"
#include "qfunctionutils.h"

#include <LayerShellQt/Window>
#include <QApplication>
#include <QBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <qmargins.h>
#include <qpoint.h>
#include <qsize.h>

using namespace LayerShellQt;

Overlay::Overlay(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    connect(this, SIGNAL(toggleEditing()),    this, SLOT(onEditingToggled()));
    connect(this, SIGNAL(editingStarted()),   this, SLOT(onEditingStarted()));
    connect(this, SIGNAL(editingEnded()),     this, SLOT(onEditingEnded()));
    connect(this, SIGNAL(toggleVisibility()), this, SLOT(onVisibilityToggled()));
    connect(this, SIGNAL(requestQuit()),      this, SLOT(onQuitRequested()));

    webView = new WebView(this);
    connect(webView, SIGNAL(editingEnd()),       this,    SIGNAL(editingEnded()));
    connect(this,    SIGNAL(editingStarted()),   webView, SLOT(onEditingStarted()));
    connect(this,    SIGNAL(editingEnded()),     webView, SLOT(onEditingEnded()));

    layout->addWidget(this->webView);

    systemTray = new SystemTray(this);
    connect(systemTray, SIGNAL(toggleEditing()),    this, SIGNAL(toggleEditing()));
    connect(systemTray, SIGNAL(toggleVisibility()), this, SIGNAL(toggleVisibility()));
    connect(systemTray, SIGNAL(requestQuit()),      this, SIGNAL(requestQuit()));

    show();
    hide();

    throttledResize = QFunctionUtils::Throttle(
        [this](QRect rect) {
            this->setOverlayGeometry(rect);
            emit this->toggleEditing();
            emit this->toggleEditing();
        },
    250);

    connect(windowHandle(), SIGNAL(visibleChanged(bool)), systemTray, SLOT(onVisibleChange(bool)));
    connect(this,           SIGNAL(editingStarted()),    systemTray,  SLOT(onEditingStarted()));
    connect(this,           SIGNAL(editingEnded()),      systemTray,  SLOT(onEditingEnded()));
    
    systemTray->show();
    emit editingEnded();
}

void Overlay::setOverlayGeometry(QRect rect) {
    if (auto layerShellWindow = Window::get(windowHandle())) {
        QScreen *screen = this->screen();
        int marginRight =
            screen->geometry().width() - (rect.x() + rect.width());
        int marginBottom =
            screen->geometry().height() - (rect.y() + rect.height());
        layerShellWindow->setMargins(
            QMargins(rect.x(), rect.y(), marginRight, marginBottom));
    }
}

void Overlay::onOsuGeometryChanged(QRect rect) {
    throttledResize(rect);
}

void Overlay::setTosuUrl(QUrl url) {
    this->webView->setTosuBaseUrl(url);
}

void Overlay::initLayerShell() {
    if (QApplication::platformName() != "wayland") return;
    if (auto layerShellWindow = Window::get(windowHandle())) {
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
    if (!isVisible()) show();
    editing = true;
    setMask(QRegion());
    if (auto layerShellWindow = Window::get(windowHandle())) {
        layerShellWindow->setKeyboardInteractivity(Window::KeyboardInteractivityExclusive);
    }
}

void Overlay::onEditingEnded() {
    editing = false;
    setMask(QRegion(geometry()));
    if (auto layerShellWindow = Window::get(windowHandle())) {
        layerShellWindow->setKeyboardInteractivity(Window::KeyboardInteractivityNone);
    }
}

void Overlay::onVisibilityToggled() {
    setVisible(!isVisible());
}

void Overlay::onQuitRequested() {
    QMetaObject::invokeMethod(QApplication::instance(), &QApplication::quit, Qt::QueuedConnection);
}
