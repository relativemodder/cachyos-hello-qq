# CachyOS Hello

Welcome screen for CachyOS written in QML/C++.

<img src="assets/welcome.png" />

<img src="assets/tweaks.png" />

## Dependencies (Arch Linux)
### Runtime
- qt6-base
- qt6-declarative
- kirigami
- qqc2-desktop-style
- qqc2-breeze-style
### Build deps
- base-devel
- cmake
- qt6-base
- qt6-declarative
- qt6-tools
- kirigami
- qqc2-desktop-style
- qqc2-breeze-style
- rust

```bash
sudo pacman -S base-devel cmake qt6-base qt6-declarative qt6-tools kirigami qqc2-desktop-style qqc2-breeze-style rust
```

## Qt Quick animations

Unline other Qt apps, this app is uncapped from 60 FPS. 

If it creates problems for you, pass `DONT_FIX_60FPS=1` environment variable to disable that workaround.


## Theming

If you want something different from Breeze, you can specify controls style by passing `QT_QUICK_CONTROLS_STYLE` environment variable.


## Translations

If you want to generate new strings in translation files, run `update_translations` build target:
```bash
cd build
cmake --build . --target update_translations
```

