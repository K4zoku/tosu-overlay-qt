#ifndef IPC_H
#define IPC_H

#include <QDBusConnection>


enum class IpcCommand {
  None,
  ToggleEditing,
  ToggleOverlay,
  QuitOverlay
};

class Ipc : public QObject
{
Q_OBJECT
public:
    bool run();
    static bool send(IpcCommand command);

signals:
    void ipcToggleEditing();
    void ipcToggleOverlay();
    void ipcQuit();

public slots:
    bool call(int command);
};

#endif // IPC_H
