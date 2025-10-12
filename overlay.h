#ifndef OVERLAY_H
#define OVERLAY_H

#include "systemtray.h"
#include "webview.h"

#include <QWidget>

class Overlay : public QWidget
{
    Q_OBJECT
public:
    Overlay(QWidget *parent = nullptr);
    void setTosuUrl(QUrl url);
    void showSysTray();
    void initLayerShell();

signals:
    void toggleEditing();
    void editingStarted();
    void editingEnded();
    void requestQuit();
    void toggleVisibility();

private slots:
    void onEditingStarted();
    void onEditingEnded();
    void onEditingToggled();
    void onVisibilityToggled();
    void onQuitRequested();

private:
    bool editing = false;
    WebView *webView = nullptr;
    SystemTray *systemTray = nullptr;
};

#endif // OVERLAY_H
