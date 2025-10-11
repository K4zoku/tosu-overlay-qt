#include "ipc.h"

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>

#define SERVICE_NAME "app.tosu.Overlay"

IPC::IPC() {}

bool IPC::connect() {
    auto connection = QDBusConnection::sessionBus();
    if (!connection.isConnected()) {
        qWarning("Cannot connect to the D-Bus session bus.\n"
                 "To start it, run:\n"
                 "\teval `dbus-launch --auto-syntax`\n");
        return false;
    }

    return true;
}

bool IPC::registerService() {
    auto connection = QDBusConnection::sessionBus();
    if (!connection.registerService(SERVICE_NAME)) {
        qWarning("%s\n", qPrintable(connection.lastError().message()));
        return false;
    }
    connection.registerObject("/", this, QDBusConnection::ExportAllSlots);
    return true;
}

bool IPC::send(IpcCommand command) {
    QDBusInterface iface(SERVICE_NAME, "/");
    if (iface.isValid()) {
        QDBusReply<int> reply = iface.call("call", (int)command);
        if (reply.isValid()) {
            bool value = reply.value();
            if (value) {
                qWarning("IPC command execute failed");
            }
            return value;
        }
        qWarning("Call failed: %s\n", qPrintable(reply.error().message()));
        return false;
    }
    return true;
}

bool IPC::call(int command) {
    IpcCommand ipcCommand = (IpcCommand) command;
    switch (ipcCommand) {
    case IpcCommand::ToggleOverlay:
        emit ipcToggleOverlay();
        break;
    case IpcCommand::ToggleEditing:
        emit ipcToggleEditing();
        break;
    case IpcCommand::QuitOverlay:
        emit ipcQuit();
        break;
    default:
        return false;
    }
    return true;
}
