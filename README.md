# Tosu Overlay Linux

<p align="center">
  <img src="./res/logo.svg" width="64" height="64">
</p>

> An overlay for osu!. Powered by Tosu, Qt 6, and Qt 6 WebEngine.
> Only support Linux, for Windows, please use the official [tosu overlay](https://github.com/tosuapp/tosu).

## ✨ Features

- [x] Transparent window.
- [x] Clickthrough window.
- [x] Overlay edit mode.
  - [x] Press Esc to exit editing.
- [x] Global hotkey support. (need manual configuration per compositor/DE)
- [x] Wayland support.
- [ ] X11 support. (not tested yet)
- [x] Multi-monitor.
- [x] System tray.
- [x] Attach overlay to osu! window (Currently supported KDE by kwin script in [extra/kwin-script](extra/kwin-script). You can use external script to send osu window geometry via D-Bus, contributions are welcome).

## 📚 Dependencies

- [Qt 6](https://www.qt.io/product/qt6)
- Qt 6 WebEngine
- [LayerShellQT](https://github.com/KDE/layer-shell-qt)

## 🏗️ Building

```sh
./build.sh
```

## 📜 Usage

1. Run [tosu](https://tosu.app) first
2. Run `tosu-overlay` (you can choose monitor with `--monitor` option)
3. Run `tosu-overlay --ipc-*` commands or right click the system tray icon to send commands

```sh
Usage: tosu-overlay-qt [options]

Options:
  -h, --help                 Displays help on commandline options.
  --help-all                 Displays help, including generic Qt options.
  -v, --version              Displays version information.
  -t, --tosu-url <tosu-url>  Base url to tosu instance.
  -m, --monitor <monitor>    Monitor to display overlay on, run with empty flag
                             to show list of monitor
  -a, --attach               Attach overlay to osu! window, require external
                             script to send geometry. Using this option will
                             ignore --monitor option
  -e, --ipc-toggle-edit      Send 'toggle-edit' command to the running overlay
  -H, --ipc-toggle-overlay   Send 'toggle-visible' command to the running
                             overlay
  -q, --ipc-quit             Send 'quit' command to the running overlay
```

## 🚌 D-Bus Interface

**Service**: `app.tosu.Overlay`

**Path**: `/`

**Interface**: `app.tosu.Overlay.Ipc`

**Methods**:
- `call(int command) -> bool`
  - Details: Method to call IPC command 
  - Commands: 
    - `1`: toggle-edit, 
    - `2`: toggle-visible, 
    - `3`: quit
- `toggleEditing() -> void`
  - Details: Method to toggle editing, equivalent to `call(1)`
- `toggleOverlay() -> void`
  - Details: Method to toggle overlay, equivalent to `call(2)`
- `quitOverlay() -> void`
  - Details: Method to quit overlay, equivalent to `call(3)`
- `geometryChanged(int x, int y, int width, int height) -> void`
  - Details: Signal to notify geometry change, used to attach overlay to osu! window by external script