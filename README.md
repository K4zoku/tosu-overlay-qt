# Tosu Overlay Linux Qt

<p align="center">
  <img src="./src/logo.svg" width="64" height="64">
</p>

> An overlay for osu!. Powered by Tosu, Qt 6, and Qt 6 WebEngine.
> Only support Linux, for Windows, please use the official [tosu overlay](https://github.com/tosuapp/tosu).

## ✨ Features

- [x] Transparent window.
- [x] Clickthrough window.
- [x] Overlay edit mode.
- [x] Global hotkey.
- [x] Wayland support.
- [x] X11 support(?).
- [x] Multi-monitor.
- [x] System tray.
- [ ] Attach overlay to osu! window (didn't find a way).

### 📚 Dependencies

- [Qt 6](https://www.qt.io/product/qt6)
- Qt 6 WebEngine
- [LayerShellQT](https://github.com/KDE/layer-shell-qt)

### 🏗️ Building

```sh
./build.sh
```

### 📜 Usage

1. Run [tosu](https://tosu.app) first
2. Run `tosu-overlay-qt` (you can choose monitor with `--monitor` option)
3. Run `tosu-overlay-qt --ipc-*` commands or right click the system tray icon to send commands

```sh
Usage: tosu-overlay-qt [options]

Options:
  -h, --help                 Displays help on commandline options.
  --help-all                 Displays help, including generic Qt options.
  -v, --version              Displays version information.
  -t, --tosu-url <tosu-url>  Base url to tosu instance.
  -m, --monitor <monitor>    Monitor to display overlay on, run with empty flag to show list of monitor
  -e, --ipc-toggle-edit      Send 'toggle-edit' command to the running overlay
  -H, --ipc-toggle-overlay   Send 'toggle-visible' command to the running overlay
  -q, --ipc-quit             Send 'quit' command to the running overla
```
