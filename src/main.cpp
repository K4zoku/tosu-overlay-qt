#include "commandline.h"
#include "overlay.h"
#include "version.h"

#include <LayerShellQt/Shell>
#include <QApplication>

int main(int argc, char *argv[]) {
  LayerShellQt::Shell::useLayerShell();
  QApplication app(argc, argv);
  QApplication::setApplicationName(APPLICATION_NAME);
  QApplication::setApplicationDisplayName("Tosu Overlay");
  QApplication::setApplicationVersion(APPLICATION_VERSION);

  CommandLineParser parser;
  Ipc ipc;

  parser.parse(QApplication::arguments());
  auto options = parser.parseCommandLine();

  switch (options.status) {
  case CommandLineParseResult::Status::Error:
    qWarning("%s", qPrintable(*options.error));
    return 1;
  case CommandLineParseResult::Status::IpcRequested:
    if (Ipc::send(options.command)) {
      qInfo("%s", qPrintable(QApplication::translate("main", "IPC command sent")));
    } else {
      qWarning("%s", qPrintable(QApplication::translate("main", "IPC command failed")));
    }
    return 0;
  case CommandLineParseResult::Status::HelpRequested:
    qInfo("%s", qPrintable(parser.helpText()));
    return 0;
  case CommandLineParseResult::Status::VersionRequested:
    qInfo("%s %s", APPLICATION_NAME, APPLICATION_VERSION);
    return 0;
  case CommandLineParseResult::Status::Ok:
    if (!ipc.run()) {
      qCritical("%s", qPrintable(QApplication::translate("main", "Cannot run IPC server")));
      return 1;
    }

    Overlay overlay;
    QObject::connect(&ipc, SIGNAL(ipcToggleOverlay()), &overlay, SIGNAL(toggleVisibility()));
    QObject::connect(&ipc, SIGNAL(ipcToggleEditing()), &overlay, SIGNAL(toggleEditing()));
    QObject::connect(&ipc, SIGNAL(ipcQuit()), &overlay, SIGNAL(requestQuit()));
    if (options.attach) {
      QObject::connect(&ipc, SIGNAL(ipcGeometryChanged(QRect)), &overlay, SLOT(onOsuGeometryChanged(QRect)));
    } else {
      overlay.setGeometry((*options.screen)->geometry());
    }
    overlay.setTosuUrl(*options.url);
    overlay.initLayerShell();
    overlay.showFullScreen();

    return app.exec();
  }
}
