#include "systemtray.h"

#include <QAction>
#include <QMenu>
#include <QDebug>

SystemTray::SystemTray(QObject *parent)
    : QSystemTrayIcon{parent}
{
    QIcon icon = QIcon(QPixmap(":/images/logo.svg"));
    this->setIcon(icon);
    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onActivated(QSystemTrayIcon::ActivationReason)));
    QAction *toggleVisibilityAction = new QAction(tr("Hide/Show overlay"), this);
    connect(toggleVisibilityAction, SIGNAL(triggered()), this, SIGNAL(toggleVisibility()));
    QAction *toggleEditAction = new QAction(tr("Toggle overlay editing"), this);
    connect(toggleEditAction, SIGNAL(triggered()), this, SIGNAL(toggleEditing()));
    QAction *quitAction = new QAction(tr("Quit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SIGNAL(requestQuit()));

    QMenu *menu = new QMenu();
    menu->addAction(toggleVisibilityAction);
    menu->addAction(toggleEditAction);
    menu->addSeparator();
    menu->addAction(quitAction);
    this->setContextMenu(menu);
}

void SystemTray::onActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
    case QSystemTrayIcon::ActivationReason::Trigger:
        emit toggleVisibility();
        break;
    case QSystemTrayIcon::ActivationReason::DoubleClick:
        emit toggleEditing();
        break;
    case QSystemTrayIcon::ActivationReason::MiddleClick:
        emit requestQuit();
        break;
    default:
        break;
    }
}
