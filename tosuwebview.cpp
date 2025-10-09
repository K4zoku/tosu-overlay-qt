#include "tosuwebview.h"

TosuWebView::TosuWebView(QWidget *parent, const QUrl &baseUrl) : QWebEngineView{parent} {
    this->setTosuBaseUrl(baseUrl);
    this->page()->setBackgroundColor(Qt::transparent);
}

TosuWebView::~TosuWebView() {
    delete this->page();
}

void TosuWebView::setTosuBaseUrl(const QUrl &baseUrl) {
    this->m_baseUrl = baseUrl;
    QUrl overlayUrl = baseUrl.adjusted(QFlag(QUrl::RemovePath));
    overlayUrl.setPath("/api/ingame");
    this->setUrl(overlayUrl);
}

void TosuWebView::onEditingStarted() {
    this->page()->runJavaScript("window.postMessage('editingStarted')");
}

void TosuWebView::onEditingEnded() {
    this->page()->runJavaScript("window.postMessage('editingEnded')");
}
