#include "overlay.h"

#include <LayerShellQt/Window>
#include <QApplication>
#include <QBoxLayout>
#include <QMessageBox>
#include <QPushButton>

Overlay::Overlay(QWidget *parent) : QWidget(parent) {
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->initLayerShell();

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);

    connect(this, SIGNAL(toggleEditing()),    this, SLOT(onEditingToggled()));
    connect(this, SIGNAL(editingStarted()),   this, SLOT(onEditingStarted()));
    connect(this, SIGNAL(editingEnded()),     this, SLOT(onEditingEnded()));
    connect(this, SIGNAL(toggleVisibility()), this, SLOT(onVisibilityToggled()));
    connect(this, SIGNAL(requestQuit()),      this, SLOT(onQuitRequested()));

    this->mTosuWebView = new TosuWebView(this);
    connect(this->mTosuWebView, SIGNAL(loadFinished(bool)), this, SLOT(onLoaded(bool)));
    connect(this->mTosuWebView, SIGNAL(editingEnd()),       this, SIGNAL(editingEnded()));
    connect(this, SIGNAL(editingStarted()), this->mTosuWebView, SLOT(onEditingStarted()));
    connect(this, SIGNAL(editingEnded()),   this->mTosuWebView, SLOT(onEditingEnded()));

    layout->addWidget(this->mTosuWebView);

    this->mSystemTray = new SystemTray(this);
    connect(this->mSystemTray, SIGNAL(toggleEditing()),    this, SIGNAL(toggleEditing()));
    connect(this->mSystemTray, SIGNAL(toggleVisibility()), this, SIGNAL(toggleVisibility()));
    connect(this->mSystemTray, SIGNAL(requestQuit()),      this, SIGNAL(requestQuit()));
    this->editingEnded();
}

void Overlay::showSysTray() {
    this->mSystemTray->show();
}

void Overlay::setTosuUrl(QUrl url) {
    this->mTosuWebView->setTosuBaseUrl(url);
}

void Overlay::initLayerShell() {
    this->show();
    this->hide();
    QWindow *window = this->windowHandle();
    QScreen *screen = this->screen();
    if (auto layerShellWindow = LayerShellQt::Window::get(window)) {
        layerShellWindow->setExclusiveZone(-1);
        layerShellWindow->setScope("tosu-overlay");
        layerShellWindow->setLayer(LayerShellQt::Window::LayerOverlay);
        layerShellWindow->setDesiredSize(screen->availableSize());
    }
}

void Overlay::onLoaded(bool ok) {
    this->ready = ok;
    if (ok) {

    } else {
        QMessageBox msgBox;
        msgBox.setText(tr("Error connecting with tosu, is it running?"));
        QPushButton * yes = msgBox.addButton(tr("Yes, reload the overlay"), QMessageBox::YesRole);
        QPushButton * no = msgBox.addButton(tr("No, close the overlay"), QMessageBox::NoRole);
        msgBox.setDefaultButton(yes);
        msgBox.setEscapeButton(no);
        msgBox.exec();
        if (msgBox.clickedButton() == yes) {
            this->mTosuWebView->reload();
        } else {
            requestQuit();
        }
    }
}

void Overlay::onEditingToggled() {
    if (!ready) {
        return;
    }
    if (this->mEditing ^= true) {
        emit editingStarted();
    } else {
        emit editingEnded();
    }
}

void Overlay::onEditingStarted() {
    if (!this->isVisible()) {
        this->show();
    }
    QWindow *window = this->windowHandle();
    QRegion mask = QRegion();
    window->setMask(mask);
    if (auto layerShellWindow = LayerShellQt::Window::get(window)) {
        layerShellWindow->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityExclusive);
    }
}

void Overlay::onEditingEnded() {
    QWindow *window = this->windowHandle();
    QRegion mask = QRegion(window->geometry());
    window->setMask(mask);
    if (auto layerShellWindow = LayerShellQt::Window::get(window)) {
        layerShellWindow->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityNone);
    }
}

void Overlay::onVisibilityToggled() {
    if (!ready) {
        return;
    }
    if (this->isVisible()) {
        this->hide();
    } else {
        this->show();
    }
}

void Overlay::onQuitRequested() {
    QMetaObject::invokeMethod(QApplication::instance(), &QApplication::quit, Qt::QueuedConnection);
}
