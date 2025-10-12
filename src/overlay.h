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
    void setOverlayGeometry(QRect rect);
    bool editing = false;
    WebView *webView = nullptr;
    SystemTray *systemTray = nullptr;
    std::function<void(QRect)> throttledResize = nullptr;
};

#endif // OVERLAY_H
