#include "webview.h"

#include <QApplication>
#include <QMessageBox>
#include <QPushButton>
#include <QWebChannel>

WebView::WebView(QWidget *parent, const QUrl baseUrl) : QWebEngineView{parent} {
    this->setPage(new WebPage(this));
    this->setTosuBaseUrl(baseUrl);
    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(onLoaded(bool)));
}

WebView::~WebView() {
    delete this->page();
}

static const char *INJECT_WEBCHANNEL_SCRIPT =
"(() => {"
    "let script = document.createElement('script');"
    "script.onload = function() {"
        "let object = null;"
        "new QWebChannel(qt.webChannelTransport, (channel) => object = channel.objects.object);"
        "document.addEventListener('keydown', (event) => object?.onKeyDown(event.key));"
    "};"
    "script.src = 'qrc:///qtwebchannel/qwebchannel.js';"
    "document.body.appendChild(script);"
"})();";

void WebView::onLoaded(bool ok) {
    if (ok) {
        this->page()->runJavaScript(INJECT_WEBCHANNEL_SCRIPT);
    } else {
        QMessageBox msgBox;
        msgBox.setText(tr("Error connecting with tosu, is it running?"));
        QPushButton *yes = msgBox.addButton(tr("Yes, reload the overlay"), QMessageBox::YesRole);
        QPushButton *no = msgBox.addButton(tr("No, close the overlay"), QMessageBox::NoRole);
        msgBox.setDefaultButton(yes);
        msgBox.setEscapeButton(no);
        msgBox.exec();
        if (msgBox.clickedButton() == yes) {
            reload();
        } else {
            QMetaObject::invokeMethod(QApplication::instance(), &QApplication::quit, Qt::QueuedConnection);
        }
    }
}

void WebView::setTosuBaseUrl(const QUrl baseUrl) {
    this->baseUrl = baseUrl;
    QUrl overlayUrl = baseUrl.adjusted(QFlag(QUrl::RemovePath));
    overlayUrl.setPath("/api/ingame");
    this->setUrl(overlayUrl);
}

void WebView::onKeyDown(QString key) {
    if (key == "Esc" || key == "Escape") {
        emit editingEnd();
    }
}

void WebView::onEditingStarted() {
    this->page()->runJavaScript("window.postMessage('editingStarted')");
}

void WebView::onEditingEnded() {
    this->page()->runJavaScript("window.postMessage('editingEnded')");
}

WebPage::WebPage(WebView *parent) : QWebEnginePage{parent} {
    this->setBackgroundColor(Qt::transparent);
    QWebChannel *channel = new QWebChannel(parent);
    this->setWebChannel(channel);
    WebChannelObject *object = new WebChannelObject(parent);
    channel->registerObject("object", object);
    connect(object, SIGNAL(keyDown(QString)), parent, SLOT(onKeyDown(QString)));
}

void WebPage::javaScriptConsoleMessage(QWebEnginePage::JavaScriptConsoleMessageLevel level, const QString &message, int lineNumber, const QString &sourceID)
{
    Q_UNUSED(level)
    Q_UNUSED(message);
    Q_UNUSED(lineNumber)
    Q_UNUSED(sourceID)
}

WebChannelObject::WebChannelObject(QObject *parent) : QObject{parent} {}

void WebChannelObject::onKeyDown(QString key) {
    emit this->keyDown(key);
}
