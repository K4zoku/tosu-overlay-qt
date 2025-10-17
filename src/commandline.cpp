#include "commandline.h"
#include <QApplication>
#include <QUrl>

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
    optionAttach(
        QStringList() << "a" << "attach", 
        QApplication::translate("main", 
            "Attach overlay to osu! window, require external script to send geometry."
            "Using this option will ignore --monitor option"
        )
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
    optionHelp = addHelpOption();
    optionVersion = addVersionOption();
    addOption(optionUrl);
    addOption(optionMonitor);
    addOption(optionAttach);
    addOption(optionIpcToggleEdit);
    addOption(optionIpcToggleOverlay);
    addOption(optionQuit);
}

bool CommandLineParser::parseCommonOptions(CommandLineParseResult *result) {
    if (isSet(*optionHelp)) {
        result->status = CommandLineParseResult::Status::HelpRequested;
        return false;
    }
    if (isSet(*optionVersion)) {
        result->status = CommandLineParseResult::Status::VersionRequested;
        return false;
    }
    return true;
}

bool CommandLineParser::parseIpcOption(CommandLineParseResult *result) {
    result->status = CommandLineParseResult::Status::IpcRequested;
    if (isSet(optionIpcToggleEdit)) {
        result->command = IpcCommand::ToggleEditing;
        return false;
    }
    if (isSet(optionIpcToggleOverlay)) {
        result->command = IpcCommand::ToggleOverlay;
        return false;
    }

    if (isSet(optionQuit)) {
        result->command = IpcCommand::QuitOverlay;
        return false;
    }
    result->status = CommandLineParseResult::Status::Ok;
    return true;
}

static inline QString availableMonitorMessage(QList<QScreen*> screens) {
    QString result = QApplication::translate("main", "Available monitors: (%1)").arg(screens.size());
    for (int i = 0; i < screens.size(); ++i) {
        result.append("\n  ");
        result.append(QApplication::translate("main", "Monitor %1: %2").arg(QString::number(i), screens[i]->name()));
    }
    return result;
}

bool CommandLineParser::parseMonitorOption(CommandLineParseResult *result) {
    if (isSet(optionAttach)) {
        result->attach = true;
        return true;
    }
    if (!isSet(optionMonitor)) {
        result->screen = QApplication::primaryScreen();
        return true;
    }
    auto value = this->value(optionMonitor);
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
    QUrl url(value(optionUrl));
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

