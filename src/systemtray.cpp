#include "systemtray.h"

SystemTray::SystemTray(QObject *parent) : QSystemTrayIcon{parent} {
  QIcon icon = QIcon(QPixmap(":/logo.svg"));
  setIcon(icon);
  connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
          SLOT(onActivated(QSystemTrayIcon::ActivationReason)));

  actionShowHide = new QAction(tr("Hide/Show overlay"), this);
  actionEdit = new QAction(tr("Toggle overlay editing"), this);
  actionReload = new QAction(tr("Reload overlay"), this);
  actionReload->setIcon(QIcon(QPixmap(":/icons/refresh-cw.svg")));
  actionQuit = new QAction(tr("Quit"), this);
  actionQuit->setIcon(QIcon(QPixmap(":/icons/log-out.svg")));

  connect(actionShowHide, SIGNAL(triggered()), this, SIGNAL(toggleVisibility()));
  connect(actionEdit, SIGNAL(triggered()), this, SIGNAL(toggleEditing()));
  connect(actionReload, SIGNAL(triggered()), this, SIGNAL(reloadOverlay()));
  connect(actionQuit, SIGNAL(triggered()), this, SIGNAL(requestQuit()));

  menu = new QMenu();
  menu->addAction(actionShowHide);
  menu->addAction(actionEdit);
  menu->addAction(actionReload);
  menu->addSeparator();
  menu->addAction(actionQuit);
  this->setContextMenu(menu);
}

void SystemTray::onVisibleChange(bool visible) {
  if (visible) {
    actionShowHide->setText(tr("Hide overlay"));
    QIcon icon = QIcon(QPixmap(":/icons/eye-off.svg"));
    actionShowHide->setIcon(icon);
  } else {
    actionShowHide->setText(tr("Show overlay"));
    QIcon icon = QIcon(QPixmap(":/icons/eye.svg"));
    actionShowHide->setIcon(icon);
  }
}

void SystemTray::onEditingStarted() {
  actionEdit->setText(tr("Disable editing"));
  QIcon icon = QIcon(QPixmap(":/icons/save.svg"));
  actionEdit->setIcon(icon);
}

void SystemTray::onEditingEnded() {
  actionEdit->setText("Enable editing");
  QIcon icon = QIcon(QPixmap(":/icons/edit.svg"));
  actionEdit->setIcon(icon);
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
