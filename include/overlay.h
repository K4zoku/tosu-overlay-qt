#ifndef OVERLAY_H
#define OVERLAY_H

#include "systemtray.h"
#include "webview.h"

#include <QWidget>
#include <QRect>

class Overlay : public QWidget
{
    Q_OBJECT
public:
    Overlay(QWidget *parent = nullptr);
    void setTosuUrl(QUrl url);
    void initLayerShell();
    void setOverlayGeometry(QRect rect);

signals:
    void toggleEditing();
    void editingStarted();
    void editingEnded();
    void requestQuit();
    void toggleVisibility();
    
public slots:
    void onOsuGeometryChanged(QRect rect);

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
