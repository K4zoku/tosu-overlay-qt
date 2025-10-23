# Tosu Overlay Linux

<a href="https://github.com/K4zoku/tosu-overlay-qt/actions/workflows/build.yml">
  <img src="https://img.shields.io/github/actions/workflow/status/k4zoku/tosu-overlay-qt/build.yml?style=for-the-badge" alt="GitHub Actions Workflow Status">
</a>
<a href="github.com/K4zoku/tosu-overlay-qt/releases/latest">
  <img src="https://img.shields.io/github/v/release/k4zoku/tosu-overlay-qt?style=for-the-badge" alt="GitHub Release">
</a>
<a href="https://aur.archlinux.org/packages/tosu-overlay/">
  <img src="https://img.shields.io/aur/version/tosu-overlay?style=for-the-badge" alt="AUR Version">
</a>

## 💡 About
<p align="center">
  <img src="./res/logo.svg" width="64" height="64">
</p>

An overlay for osu!. Powered by Tosu, Qt, and Qt WebEngine.
Only support Linux, for Windows, please use the official [tosu overlay](https://github.com/tosuapp/tosu).

### ✨ Features

- [x] Transparent window.
- [x] Clickthrough window.
- [x] Overlay edit mode.
  - [x] Press Esc to exit editing.
- [x] Global hotkey support.
  - On Wayland, you can bind a hotkey/shortcuts in your compositor/DE to run command like `tosu-overlay --ipc-toggle-edit`.
  - X11 global hotkey is not supported yet.
- [x] Wayland support. 
  - Tested on KDE Plasma.
  - Tested on hyprland.
  - GNOME is not supported yet.
- [x] X11 support.
  - Tested on XFCE.
  - Tested on Cinnamon.
- [x] Multi-monitor.
- [x] System tray.
- [x] Attach overlay to osu! window (Currently supported KDE by kwin script in [extra/kwin-script](extra/kwin-script). You can use external script to send osu window geometry via D-Bus, contributions are welcome).

## 🧾 Dependencies

- [Qt](https://www.qt.io/product/qt6)
- Qt WebEngine
- [LayerShellQT](https://github.com/KDE/layer-shell-qt)

## 📥 Installation

### 📦 Package manager
#### AUR:
You can install it from [tosu-overlay](https://aur.archlinux.org/packages/tosu-overlay/) or [tosu-overlay-git](https://aur.archlinux.org/packages/tosu-overlay-git/)
```sh
yay -S tosu-overlay
```
or 
```sh
yay -S tosu-overlay-git
```

#### Others package manager:

If you can and want to port `tosu-overlay` to other package managers, feel free to do so. Contributions are welcome!

### 🏗️ Building from source

#### Build dependencies

- meson
- gcc
- qt6-base
- qt6-webengine
- layer-shell-qt

#### Build

##### Setup

```sh
meson setup build
```

##### Compile

```sh
meson compile -C build
```

##### Install

```sh
sudo meson install -C build
```

The binary will be installed to `/usr/local/bin/tosu-overlay`

## 📜 Usage

1. Run [`tosu`](https://tosu.app) first
2. Run `tosu-overlay` (you can choose monitor with `--monitor` option)
3. _Optional:_ Run `tosu-overlay --ipc-*` commands or right click the system tray icon to send commands

### 📝 Commandline options

```
Usage: tosu-overlay [options]

Options:
  -h, --help                 Displays help on commandline options.
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

### 🚌 D-Bus Interface

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
  
---
> <p align="center">Made with ❤️ by <a href="https://github.com/K4zoku">@K4zoku</a></p>
