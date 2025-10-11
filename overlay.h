#ifndef OVERLAY_H
#define OVERLAY_H

#include "systemtray.h"
#include "tosuwebview.h"

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
    void onLoaded(bool ok);
    void onEditingStarted();
    void onEditingEnded();
    void onEditingToggled();
    void onVisibilityToggled();
    void onQuitRequested();

private:
    bool mEditing = false;
    TosuWebView *mTosuWebView = nullptr;
    SystemTray *mSystemTray = nullptr;
};

#endif // OVERLAY_H
