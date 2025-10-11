#ifndef TOSUWEBVIEW_H
#define TOSUWEBVIEW_H

#include <QWebEngineView>

#define TOSU_DEFAULT_BASE_URL "https://127.0.0.1:24050"

class TosuWebView : public QWebEngineView
{
Q_OBJECT

public:
    TosuWebView(QWidget *parent = nullptr, const QUrl &baseUrl = QUrl(QString(TOSU_DEFAULT_BASE_URL)));
    ~TosuWebView();
    void setTosuBaseUrl(const QUrl &baseUrl);

signals:
    void editingEnd();

public slots:
    void onEditingStarted();
    void onEditingEnded();
    void loaded(bool ok);
    void onKeyDown(QString key);

private:
    QUrl m_baseUrl;
    void javaScriptConsoleMessage(QWebEnginePage::JavaScriptConsoleMessageLevel level, const QString &message, int lineNumber, const QString &sourceID);
};

class WebChannelObject : public QObject
{
    Q_OBJECT
public:
    WebChannelObject(QObject *parent = nullptr);

public slots:
    void onKeyDown(QString key);

signals:
    void keyDown(QString key);
};

#endif // TOSUWEBVIEW_H
