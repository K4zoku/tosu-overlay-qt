#ifndef IPC_H
#define IPC_H

#include <QDBusConnection>


enum class IpcCommand {
  None,
  ToggleEditing,
  ToggleOverlay,
  QuitOverlay
};

class IPC : public QObject
{
Q_OBJECT
public:

    IPC();
    bool connect();
    bool registerService();
    bool send(IpcCommand command);

signals:
    void ipcToggleEditing();
    void ipcToggleOverlay();
    void ipcQuit();

public slots:
    bool call(int command);
};

#endif // IPC_H
