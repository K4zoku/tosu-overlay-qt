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
    void escKeyPressed();

private:
    QUrl m_baseUrl;
};

#endif // TOSUWEBVIEW_H
