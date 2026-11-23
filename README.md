# SHIN Rebuild

C++17 reconstruction of the classic SHIN platform game, targeting Windows and Nintendo Switch. Work in progress: implemented behavior is not a guarantee of frame-accurate equivalence with the original.

## 源码与素材

本仓库只发布重建源码、配置和测试输入。**不包含原版游戏、贴图、音效、音乐、角色图标、密钥或安装包。** 请自行取得可合法使用的原版资源，勿将其提交到仓库。原作名称和角色等权利归各自权利人所有。

## Windows 构建

需要 CMake 3.20+、Visual Studio 2022 C++ 工具与 Windows SDK。

```powershell
cmake -S . -B build
cmake --build build --config Release --target shin_progress
```

生成 `build/Release/SHIN_Level1.exe`。资源目录应包含 `RESOURCE.RES`、`RESOURCE/`、`bgm/` 和原版 `_inmm.ini`。将该目录复制到可执行文件旁的 `assets/`，或指定资源档案：

```powershell
.\build\Release\SHIN_Level1.exe --assets "你的资源目录/RESOURCE.RES" --width 1280 --height 720
```

方向键移动/跳跃，空格使用武器，Enter 暂停/确认，Esc 返回菜单，F11 切换全屏。

## 测试

不需要游戏素材的测试可由 CMake 构建。依赖素材的测试在配置时通过 `-DSHIN_ASSET_FILE="你的资源目录/RESOURCE.RES"` 启用。测试输入 `.keys` 文件是文本按键脚本，不是加密密钥。

## Nintendo Switch

`switch/` 包含基于 libnx、SDL2、SDL2_mixer 的移植。构建脚本使用固定版本 devkitPro Docker 镜像，需要 Docker，并显式传入本地素材目录：

```powershell
.\switch\build_switch.ps1 -Assets "你的资源目录"
```

需自行提供 `switch/icon.jpg`（256×256 baseline JPEG）。安装包工具 hacBrewPack 未随仓库分发；使用 `package_nsp.ps1` 前须自行准备经核对许可的工具及其依赖于 `switch/tools/hacBrewPack/`。个人密钥仅在本机使用，不要提交。Switch 构建与安装有独立环境要求，仓库不保证未经实机验证的设备兼容性。

## 状态

包含七关、小游戏、存档、宽屏与 Windows 图形界面启动支持。敌人行为、演出和部分场景仍有待与原版核对。开发工具中部分审计脚本需要另行准备的本地逆向分析资料，不属于无资源条件下可直接运行的测试。

## 许可证

本项目可授权的重建代码采用 **GPL-3.0-only**，详见 [LICENSE](LICENSE)。第三方组件遵循各自许可证，见 [THIRD_PARTY.md](THIRD_PARTY.md)。该许可证不授予原版游戏素材、商标或角色的使用和分发权。
