#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>

class SystemTray : public QSystemTrayIcon {
  Q_OBJECT

public:
  SystemTray(QObject *parent = nullptr);

signals:
  void toggleEditing();
  void toggleVisibility();
  void requestQuit();

public slots:
  void onVisibleChange(bool visible);
  void onEditingStarted();
  void onEditingEnded();

private slots:
  void onActivated(QSystemTrayIcon::ActivationReason reason);

private:
  QAction *actionShowHide;
  QAction *actionEdit;
  QAction *actionQuit;
  QMenu *menu;
};

#endif // SYSTEMTRAY_H
