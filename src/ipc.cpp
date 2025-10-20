#include "ipc.h"

#include <QApplication>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>

#define SERVICE_NAME "app.tosu.Overlay"

bool Ipc::run() {
  auto connection = QDBusConnection::sessionBus();
  if (!connection.isConnected()) {
    qWarning("%s", qPrintable(tr("Cannot connect to D-Bus")));
    return false;
  }
  if (!connection.registerService(SERVICE_NAME)) {
    qWarning("%s", qPrintable(connection.lastError().message()));
    return false;
  }
  if (!connection.registerObject("/", this, QDBusConnection::ExportAllSlots)) {
    qWarning("%s", qPrintable(tr("Cannot register object")));
    return false;
  }
  return true;
}

bool Ipc::send(const IpcCommand command) {
  auto connection = QDBusConnection::sessionBus();
  if (!connection.isConnected()) {
    qWarning("%s", qPrintable(QApplication::translate("main", "Cannot connect to D-Bus")));
    return false;
  }
  QDBusInterface iface(SERVICE_NAME, "/");
  if (iface.isValid()) {
    QDBusReply<bool> reply = iface.call("call", (int)command);
    if (reply.isValid()) {
      bool value = reply.value();
      if (value) {
        qInfo("%s", qPrintable(QApplication::translate("main", "IPC command executed")));
      } else {
        qWarning("%s", qPrintable(QApplication::translate("main", "IPC command execute failed")));
      }
      return value;
    }
    qWarning("%s\n%s", qPrintable(QApplication::translate("main", "D-Bus call failed")),
             qPrintable(reply.error().message()));
    return false;
  }
  return true;
}

bool Ipc::call(const int command) {
  switch ((IpcCommand)command) {
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

void Ipc::toggleEditing() { emit ipcToggleEditing(); }

void Ipc::toggleOverlay() { emit ipcToggleOverlay(); }

void Ipc::quitOverlay() { emit ipcQuit(); }

void Ipc::geometryChanged(const int x, const int y, const int width, const int height) {
  emit ipcGeometryChanged(QRect(x, y, width, height));
}
