#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWebEngineView>

static const QUrl TOSU_DEFAULT_BASE_URL = QUrl(QString("https://127.0.0.1:24050"));

class WebView : public QWebEngineView {
  Q_OBJECT

public:
  WebView(QWidget *parent = nullptr, const QUrl baseUrl = TOSU_DEFAULT_BASE_URL);
  ~WebView();
  void setTosuBaseUrl(const QUrl baseUrl);

signals:
  void editingEnd();

public slots:
  void onEditingStarted();
  void onEditingEnded();
  void onLoaded(bool ok);
  void onKeyDown(QString key);

private:
  QUrl baseUrl;
};

class WebPage : public QWebEnginePage {
  Q_OBJECT
public:
  WebPage(WebView *parent = nullptr);

private:
  void javaScriptConsoleMessage(QWebEnginePage::JavaScriptConsoleMessageLevel level, const QString &message,
                                int lineNumber, const QString &sourceID) override;
};

class WebChannelObject : public QObject {
  Q_OBJECT
public:
  WebChannelObject(QObject *parent = nullptr);

public slots:
  void onKeyDown(QString key);

signals:
  void keyDown(QString key);
};

#endif // WEBVIEW_H
