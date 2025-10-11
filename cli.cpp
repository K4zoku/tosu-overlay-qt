#include "cli.h"
#include <QApplication>
#include <QUrl>
#include <qapplication.h>

CLI::CLI() : QCommandLineParser(),
    mTosuUrlOption(
        QStringList() << "t" << "tosu-url",
        QApplication::translate("main", "Base url to tosu instance."),
        "tosu-url",
        "http://127.0.0.1:24050"
    ),
    mMonitorOption(
        QStringList() << "m" << "monitor",
        QApplication::translate("main", "Monitor to display overlay on, run with empty flag to show list of monitor"),
        "monitor"
    ),
    mToggleEditOption(
        QStringList() << "e" << "ipc-toggle-edit",
        QApplication::translate("main", "Send 'toggle-edit' command to the running overlay")
    ),
    mToggleOverlayOption(
        QStringList() << "H" << "ipc-toggle-overlay",
        QApplication::translate("main", "Send 'toggle-visible' command to the running overlay")
    ),
    mQuitOption(
        QStringList() << "q" << "ipc-quit",
        QApplication::translate("main", "Send 'quit' command to the running overlay")
    )
{
    this->mHelpOption = this->addHelpOption();
    this->mVersionOption = this->addVersionOption();
    this->addOption(this->mTosuUrlOption);
    this->addOption(this->mMonitorOption);
    this->addOption(this->mToggleEditOption);
    this->addOption(this->mToggleOverlayOption);
    this->addOption(this->mQuitOption);
}

bool CLI::parseCommonOptions(CommandLineParseResult *result) {
    if (this->isSet(*this->mHelpOption)) {
        result->statusCode = CommandLineParseResult::Status::HelpRequested;
        return false;
    }
    if (this->isSet(*this->mVersionOption)) {
        result->statusCode = CommandLineParseResult::Status::VersionRequested;
        return false;
    }
    return true;
}

bool CLI::parseIpcOption(CommandLineParseResult *result) {
    result->statusCode = CommandLineParseResult::Status::IpcRequested;
    if (this->isSet(this->mToggleEditOption)) {
        result->ipcCommand = IpcCommand::ToggleEditing;
        return false;
    }
    if (this->isSet(this->mToggleOverlayOption)) {
        result->ipcCommand = IpcCommand::ToggleOverlay;
        return false;
    }

    if (this->isSet(this->mQuitOption)) {
        result->ipcCommand = IpcCommand::QuitOverlay;
        return false;
    }
    result->statusCode = CommandLineParseResult::Status::Ok;
    return true;
}

static inline QString availableMonitorMessage(QList<QScreen*> screens) {
    QString result = QApplication::translate("main", "Available monitors: (%1)").arg(screens.size());
    result.append("\n");
    for (int i = 0; i < screens.size(); ++i) {
        result.append("  "); // indent
        result.append(QApplication::translate("main", "Monitor %1: %2").arg(QString::number(i), screens[i]->name()));
        result.append("\n");
    }
    return result;
}

bool CLI::parseMonitorOption(CommandLineParseResult *result) {
    if (!this->isSet(this->mMonitorOption)) {
        result->screen = QApplication::primaryScreen();
        return true;
    }
    auto value = this->value(this->mMonitorOption);
    auto screens = QApplication::screens();
    if (value.isEmpty()) {
        result->statusCode = CommandLineParseResult::Status::Error;
        result->errorString = availableMonitorMessage(screens);
        return false;
    }
    bool parseOk;
    int screenId = value.toInt(&parseOk);
    if (parseOk) {
        if (screenId < screens.size()) {
            result->screen = screens[screenId];
            return true;
        }
    } else {
        foreach (auto screen, screens) {
            if (screen->name() == value) {
                result->screen = screen;
                return true;
            }
        }
        if (!result->screen) {
            result->statusCode = CommandLineParseResult::Status::Error;
            auto errorString = tr("main", "Invalid monitor %1").arg(value);
            errorString.append("\n");
            errorString.append(availableMonitorMessage(screens));
            result->errorString = errorString;
            return false;
        }
    }
    result->statusCode = CommandLineParseResult::Status::Error;
    auto errorString = tr("main", "Invalid monitor %1").arg(QString::number(screenId));
    errorString.append("\n");
    errorString.append(availableMonitorMessage(screens));
    result->errorString = errorString;
    return false;
}

bool CLI::parseTosuUrlOption(CommandLineParseResult *result) {
    QUrl url(this->value(this->mTosuUrlOption));
    result->tosuBaseUrl = url;
    return true;
}

CommandLineParseResult CLI::parseCommandLine() {
    CommandLineParseResult result;
    CLIParserFn parsers[] = {
        &CLI::parseCommonOptions,
        &CLI::parseIpcOption,
        &CLI::parseMonitorOption,
        &CLI::parseTosuUrlOption
    };
    for (auto &parser : parsers) {
        if (!(this->*parser)(&result)) {
            break;
        }
    }
    return result;
}

