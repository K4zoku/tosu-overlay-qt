#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QSystemTrayIcon>

class SystemTray : public QSystemTrayIcon
{
Q_OBJECT

public:
    SystemTray(QObject *parent = nullptr);

private slots:
    void onActivated(QSystemTrayIcon::ActivationReason reason);

signals:
    void toggleEditing();
    void toggleVisibility();
    void requestQuit();
};

#endif // SYSTEMTRAY_H
