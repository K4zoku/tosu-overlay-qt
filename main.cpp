#include "cli.h"
#include "overlay.h"
#include "version.h"

#include <QApplication>
#include <LayerShellQt/Shell>
#include <QDebug>

int main(int argc, char *argv[]) {
    LayerShellQt::Shell::useLayerShell();
    QApplication app(argc, argv);
    QApplication::setApplicationName(APPLICATION_NAME);
    QApplication::setApplicationDisplayName("Tosu Overlay QT");
    QApplication::setApplicationVersion(APPLICATION_VERSION);

    CLI parser;
    IPC ipc;

    parser.parse(QApplication::arguments());
    auto options = parser.parseCommandLine();

    switch (options.statusCode) {
    case CommandLineParseResult::Status::Error:
        qWarning() << *options.errorString;
        return 1;
    case CommandLineParseResult::Status::IpcRequested:
        if (!QDBusConnection::sessionBus().isConnected()) {
            qWarning() << QApplication::translate("main", "Cannot connect to the D-Bus session bus.");
            return 1;
        }
        if (ipc.send(options.ipcCommand)) {
            qInfo() << QApplication::translate("main", "IPC command sent");
        } else {
            qWarning() << QApplication::translate("main", "IPC command failed");
        }
        return 0;
    case CommandLineParseResult::Status::HelpRequested:
        parser.showHelp();
        return 0;
    case CommandLineParseResult::Status::VersionRequested:
        parser.showVersion();
        return 0;
    case CommandLineParseResult::Status::Ok:
        if (!QDBusConnection::sessionBus().isConnected()) {
            qWarning() << QApplication::translate("main", "Cannot connect to the D-Bus session bus.");
            return 1;
        }
        if (!ipc.registerService()) {
            qCritical() << QApplication::translate("main", "Register IPC failed");
            return 1;
        }

        Overlay overlay;
        QObject::connect(&ipc, SIGNAL(ipcToggleOverlay()), &overlay, SIGNAL(toggleVisibility()));
        QObject::connect(&ipc, SIGNAL(ipcToggleEditing()), &overlay, SIGNAL(toggleEditing()));
        QObject::connect(&ipc, SIGNAL(ipcQuit()), &overlay, SIGNAL(requestQuit()));
        overlay.setGeometry((*options.screen)->geometry());
        overlay.setTosuUrl(*options.tosuBaseUrl);
        overlay.showSysTray();
        overlay.initLayerShell();
        emit overlay.editingEnded();
        overlay.show();

        return app.exec();
    }
}
