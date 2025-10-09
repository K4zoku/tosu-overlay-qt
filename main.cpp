#include "cli.h"
#include "overlay.h"
#include "version.h"

#include <QApplication>
#include <LayerShellQt/Shell>

int main(int argc, char *argv[]) {
    LayerShellQt::Shell::useLayerShell();
    QApplication app(argc, argv);
    QApplication::setApplicationName(APPLICATION_NAME);
    QApplication::setApplicationDisplayName("Tosu Overlay QT");
    QApplication::setApplicationVersion(APPLICATION_VERSION);

    CLI parser;
    IPC ipc;
    if (!ipc.connect()) {
        parser.showMessageAndExit(QCommandLineParser::MessageType::Error, QApplication::translate("main", "Could not connect to D-Bus"));
    }
    parser.parse(QApplication::arguments());
    auto options = parser.parseCommandLine();
    switch (options.statusCode) {
    case CommandLineParseResult::Status::Error:
        parser.showMessageAndExit(QCommandLineParser::MessageType::Error, *options.errorString);
        break;
    case CommandLineParseResult::Status::IpcRequested:
        if (ipc.send(options.ipcCommand)) {
            parser.showMessageAndExit(QCommandLineParser::MessageType::Information, QApplication::translate("main", "IPC command sent"));
        } else {
            parser.showMessageAndExit(QCommandLineParser::MessageType::Error, QApplication::translate("main", "IPC command failed"));
        }
        break;
    case CommandLineParseResult::Status::HelpRequested:
        parser.showHelp();
        break;
    case CommandLineParseResult::Status::VersionRequested:
        parser.showVersion();
        break;
    case CommandLineParseResult::Status::Ok:
        break;
    }

    if (!ipc.registerService()) {
        parser.showMessageAndExit(QCommandLineParser::MessageType::Error, QApplication::translate("main", "Register IPC failed"));
    }

    Overlay overlay;
    QObject::connect(&ipc, SIGNAL(ipcToggleOverlay()), &overlay, SIGNAL(toggleVisibility()));
    QObject::connect(&ipc, SIGNAL(ipcToggleEditing()), &overlay, SIGNAL(toggleEditing()));
    QObject::connect(&ipc, SIGNAL(ipcQuit()), &overlay, SIGNAL(requestQuit()));
    overlay.setTosuUrl(*options.tosuBaseUrl);
    overlay.setScreen(*options.screen);
    overlay.showSysTray();
    overlay.show();

    return app.exec();
}
