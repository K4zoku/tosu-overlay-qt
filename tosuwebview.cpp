#include "tosuwebview.h"

#include <QWebChannel>

TosuWebView::TosuWebView(QWidget *parent, const QUrl &baseUrl) : QWebEngineView{parent} {
    this->setTosuBaseUrl(baseUrl);
    this->page()->setBackgroundColor(Qt::transparent);
    QWebChannel * webChannel= new QWebChannel(this);
    this->page()->setWebChannel(webChannel);
    WebChannelObject * object = new WebChannelObject(this);
    webChannel->registerObject("tosu", object);
    connect(object, SIGNAL(keyDown(QString)), this, SLOT(onKeyDown(QString)));
    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(loaded(bool)));
}

TosuWebView::~TosuWebView() {
    delete this->page();
}

static const char *INJECT_WEBCHANNEL_SCRIPT =
"(() => {"
    "let s = document.createElement('script');"
    "s.onload = function() {"
        "let tosu = null;"
        "new QWebChannel(qt.webChannelTransport, (channel) => tosu = channel.objects.tosu);"
        "document.addEventListener('keydown', function (e) { tosu && tosu.onKeyDown(event.key) });"
    "};"
    "s.src = 'qrc:///qtwebchannel/qwebchannel.js';"
    "document.body.appendChild(s);"
"})();";

void TosuWebView::loaded(bool ok) {
    if (!ok) return;
    this->page()->runJavaScript(INJECT_WEBCHANNEL_SCRIPT);
}

void TosuWebView::setTosuBaseUrl(const QUrl &baseUrl) {
    this->m_baseUrl = baseUrl;
    QUrl overlayUrl = baseUrl.adjusted(QFlag(QUrl::RemovePath));
    overlayUrl.setPath("/api/ingame");
    this->setUrl(overlayUrl);
}

void TosuWebView::onKeyDown(QString key) {
    if (key == "Esc" || key == "Escape") {
        emit editingEnd();
    }
}

void TosuWebView::onEditingStarted() {
    this->page()->runJavaScript("window.postMessage('editingStarted')");
}

void TosuWebView::onEditingEnded() {
    this->page()->runJavaScript("window.postMessage('editingEnded')");
}


void TosuWebView::javaScriptConsoleMessage(QWebEnginePage::JavaScriptConsoleMessageLevel level, const QString &message, int lineNumber, const QString &sourceID)
{
    Q_UNUSED(level)
    Q_UNUSED(message)
    Q_UNUSED(lineNumber)
    Q_UNUSED(sourceID)
}

WebChannelObject::WebChannelObject(QObject *parent) : QObject{parent} {}

void WebChannelObject::onKeyDown(QString key) {
    emit this->keyDown(key);
}
