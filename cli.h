#ifndef CLI_H
#define CLI_H

#include "ipc.h"

#include <QCommandLineParser>
#include <QScreen>
#include <QUrl>
#include <optional>

struct CommandLineParseResult
{
    enum class Status {
        Ok,
        Error,
        IpcRequested,
        VersionRequested,
        HelpRequested
    };
    IpcCommand ipcCommand = IpcCommand::None;
    Status statusCode = Status::Ok;
    std::optional<QString> errorString = std::nullopt;
    std::optional<QString> messageString = std::nullopt;
    std::optional<QScreen*> screen = std::nullopt;
    std::optional<QUrl> tosuBaseUrl = std::nullopt;
};

class CLI : public QCommandLineParser
{
public:
    CLI();
    CommandLineParseResult parseCommandLine();
private:
    QCommandLineOption mTosuUrlOption;
    QCommandLineOption mMonitorOption;
    QCommandLineOption mToggleEditOption;
    QCommandLineOption mToggleOverlayOption;
    QCommandLineOption mQuitOption;
    std::optional<QCommandLineOption> mHelpOption = std::nullopt;
    std::optional<QCommandLineOption> mVersionOption = std::nullopt;

    bool parseCommonOptions(CommandLineParseResult *result);
    bool parseIpcOption(CommandLineParseResult *result);
    bool parseMonitorOption(CommandLineParseResult *result);
    bool parseTosuUrlOption(CommandLineParseResult *result);
};

typedef bool (CLI::*CLIParserFn)(CommandLineParseResult *);

#endif // CLI_H
