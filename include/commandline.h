#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include "ipc.h"
#include "qtmetamacros.h"

#include <optional>

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QScreen>
#include <QUrl>

struct CommandLineParseResult
{
    enum class Status {
        Ok,
        Error,
        IpcRequested,
        VersionRequested,
        HelpRequested
    };
    IpcCommand command = IpcCommand::None;
    Status status = Status::Ok;
    bool attach = false;
    std::optional<QString> error = std::nullopt;
    std::optional<QScreen*> screen = std::nullopt;
    std::optional<QUrl> url = std::nullopt;
};

class CommandLineParser : public QCommandLineParser
{
public:
    CommandLineParser();
    CommandLineParseResult parseCommandLine();

private:
    QCommandLineOption optionUrl;
    QCommandLineOption optionMonitor;
    QCommandLineOption optionAttach;
    QCommandLineOption optionIpcToggleEdit;
    QCommandLineOption optionIpcToggleOverlay;
    QCommandLineOption optionQuit;
    std::optional<QCommandLineOption> optionHelp = std::nullopt;
    std::optional<QCommandLineOption> optionVersion = std::nullopt;

    bool parseCommonOptions(CommandLineParseResult *result);
    bool parseIpcOption(CommandLineParseResult *result);
    bool parseMonitorOption(CommandLineParseResult *result);
    bool parseTosuUrlOption(CommandLineParseResult *result);
};

typedef bool (CommandLineParser::*CommandLineParserFunction)(CommandLineParseResult *);

#endif // COMMANDLINE_H
