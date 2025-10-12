#ifndef IPC_H
#define IPC_H

#include <QDBusConnection>
#include <QRect>


enum class IpcCommand {
  None,
  ToggleEditing,
  ToggleOverlay,
  QuitOverlay
};

class Ipc : public QObject
{
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "app.tosu.Overlay.Ipc")
public:
    bool run();
    static bool send(const IpcCommand command);

signals:
    void ipcToggleEditing();
    void ipcToggleOverlay();
    void ipcQuit();
    void ipcGeometryChanged(QRect rect);

public slots:
    bool call(const int command);
    void geometryChanged(const int x, const int y, const int width, const int height);
};

#endif // IPC_H
