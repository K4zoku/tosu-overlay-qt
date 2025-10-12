#include "commandline.h"
#include <QApplication>
#include <QUrl>
#include <qapplication.h>

CommandLineParser::CommandLineParser() : QCommandLineParser(),
    optionUrl(
        QStringList() << "t" << "tosu-url",
        QApplication::translate("main", "Base url to tosu instance."),
        "tosu-url",
        "http://127.0.0.1:24050"
    ),
    optionMonitor(
        QStringList() << "m" << "monitor",
        QApplication::translate("main", "Monitor to display overlay on, run with empty flag to show list of monitor"),
        "monitor"
    ),
    optionIpcToggleEdit(
        QStringList() << "e" << "ipc-toggle-edit",
        QApplication::translate("main", "Send 'toggle-edit' command to the running overlay")
    ),
    optionIpcToggleOverlay(
        QStringList() << "H" << "ipc-toggle-overlay",
        QApplication::translate("main", "Send 'toggle-visible' command to the running overlay")
    ),
    optionQuit(
        QStringList() << "q" << "ipc-quit",
        QApplication::translate("main", "Send 'quit' command to the running overlay")
    )
{
    this->optionHelp = this->addHelpOption();
    this->optionVersion = this->addVersionOption();
    this->addOption(this->optionUrl);
    this->addOption(this->optionMonitor);
    this->addOption(this->optionIpcToggleEdit);
    this->addOption(this->optionIpcToggleOverlay);
    this->addOption(this->optionQuit);
}

bool CommandLineParser::parseCommonOptions(CommandLineParseResult *result) {
    if (this->isSet(*this->optionHelp)) {
        result->status = CommandLineParseResult::Status::HelpRequested;
        return false;
    }
    if (this->isSet(*this->optionVersion)) {
        result->status = CommandLineParseResult::Status::VersionRequested;
        return false;
    }
    return true;
}

bool CommandLineParser::parseIpcOption(CommandLineParseResult *result) {
    result->status = CommandLineParseResult::Status::IpcRequested;
    if (this->isSet(this->optionIpcToggleEdit)) {
        result->command = IpcCommand::ToggleEditing;
        return false;
    }
    if (this->isSet(this->optionIpcToggleOverlay)) {
        result->command = IpcCommand::ToggleOverlay;
        return false;
    }

    if (this->isSet(this->optionQuit)) {
        result->command = IpcCommand::QuitOverlay;
        return false;
    }
    result->status = CommandLineParseResult::Status::Ok;
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

bool CommandLineParser::parseMonitorOption(CommandLineParseResult *result) {
    if (!this->isSet(this->optionMonitor)) {
        result->screen = QApplication::primaryScreen();
        return true;
    }
    auto value = this->value(this->optionMonitor);
    auto screens = QApplication::screens();
    if (value.isEmpty()) {
        result->status = CommandLineParseResult::Status::Error;
        result->error = availableMonitorMessage(screens);
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
            result->status = CommandLineParseResult::Status::Error;
            auto errorString = tr("main", "Invalid monitor %1").arg(value);
            errorString.append("\n");
            errorString.append(availableMonitorMessage(screens));
            result->error = errorString;
            return false;
        }
    }
    result->status = CommandLineParseResult::Status::Error;
    auto errorString = tr("main", "Invalid monitor %1").arg(QString::number(screenId));
    errorString.append("\n");
    errorString.append(availableMonitorMessage(screens));
    result->error = errorString;
    return false;
}

bool CommandLineParser::parseTosuUrlOption(CommandLineParseResult *result) {
    QUrl url(this->value(this->optionUrl));
    result->url = url;
    return true;
}

CommandLineParseResult CommandLineParser::parseCommandLine() {
    CommandLineParseResult result;
    CommandLineParserFunction parsers[] = {
        &CommandLineParser::parseCommonOptions,
        &CommandLineParser::parseIpcOption,
        &CommandLineParser::parseMonitorOption,
        &CommandLineParser::parseTosuUrlOption
    };
    for (auto &parser : parsers) {
        if (!(this->*parser)(&result)) {
            break;
        }
    }
    return result;
}

