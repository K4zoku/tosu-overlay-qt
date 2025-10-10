#include "tosuwebview.h"

#include <QWebChannel>

TosuWebView::TosuWebView(QWidget *parent, const QUrl &baseUrl) : QWebEngineView{parent} {
    this->setTosuBaseUrl(baseUrl);
    this->page()->setBackgroundColor(Qt::transparent);
    QWebChannel * webChannel= new QWebChannel(this);
    this->page()->setWebChannel(webChannel);
    webChannel->registerObject("tosu", this);
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
        "document.addEventListener('keydown', function(event) {"
            "event = event || window.event;"
            "let isEscape = false;"
            "if ('key' in event) {"
                "isEscape = (event.key === 'Escape' || event.key === 'Esc');"
            "} else {"
                "isEscape = (event.keyCode === 27);"
            "}"
            "if (isEscape) {"
                "tosu && tosu.escKeyPressed();"
            "}"
        "});"
    "};"
    "s.src = 'qrc:///qtwebchannel/qwebchannel.js';"
    "document.body.appendChild(s);"
"})();";

void TosuWebView::loaded(bool ok) {
    if (!ok) return;
    qDebug() << __FUNCTION__;
    this->page()->runJavaScript(INJECT_WEBCHANNEL_SCRIPT);
}

void TosuWebView::setTosuBaseUrl(const QUrl &baseUrl) {
    this->m_baseUrl = baseUrl;
    QUrl overlayUrl = baseUrl.adjusted(QFlag(QUrl::RemovePath));
    overlayUrl.setPath("/api/ingame");
    this->setUrl(overlayUrl);
}

void TosuWebView::escKeyPressed() {
    emit editingEnd();
}

void TosuWebView::onEditingStarted() {
    this->page()->runJavaScript("window.postMessage('editingStarted')");
}

void TosuWebView::onEditingEnded() {
    this->page()->runJavaScript("window.postMessage('editingEnded')");
}
