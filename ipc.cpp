#include "ipc.h"

#include <QApplication>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>

#define SERVICE_NAME "app.tosu.Overlay"

bool Ipc::run() {
    auto connection = QDBusConnection::sessionBus();
    if (!connection.isConnected()) {
        qWarning("%s\n", qPrintable(tr("Cannot connect to D-Bus")));
        return false;
    }
    if (!connection.registerService(SERVICE_NAME)) {
        qWarning("%s\n", qPrintable(connection.lastError().message()));
        return false;
    }
    if (!connection.registerObject("/", this, QDBusConnection::ExportAllSlots)) {
        qWarning("%s\n", qPrintable(tr("Cannot register object")));
        return false;
    }
    return true;
}

bool Ipc::send(IpcCommand command) {
    auto connection = QDBusConnection::sessionBus();
    if (!connection.isConnected()) {
        qWarning("%s\n", qPrintable(QApplication::translate("main", "Cannot connect to D-Bus")));
        return false;
    }
    QDBusInterface iface(SERVICE_NAME, "/");
    if (iface.isValid()) {
        QDBusReply<bool> reply = iface.call("call", (int)command);
        if (reply.isValid()) {
            bool value = reply.value();
            if (value) {
                qInfo("%s\n", qPrintable(QApplication::translate("main", "IPC command executed")));
            } else {
                qWarning("%s\n", qPrintable(QApplication::translate("main", "IPC command execute failed")));
            }
            return value;
        }
        qWarning("%s %s\n", qPrintable(QApplication::translate("main", "D-Bus call failed")), qPrintable(reply.error().message()));
        return false;
    }
    return true;
}

bool Ipc::call(int command) {
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
