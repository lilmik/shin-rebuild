# 开发工具与快速检查

本目录保存工具源码，不随游戏精简发行目录提供。资源、预览 PNG、报告和原版程序需在本机自行准备，不提交生成结果。以下命令均从源码仓库根目录执行，需要 Python 3；C++ 工具需要 CMake 和 Windows C++ 开发环境。

## 1. 素材浏览器与资源校验

```powershell
cmake -S . -B build
cmake --build build --config Release --target resource_viewer verify_resources
.\build\Release\resource_viewer.exe "你的素材目录/RESOURCE.RES"
.\build\Release\verify_resources.exe "你的素材目录/RESOURCE.RES"
```

`resource_browser/resource_viewer.cpp` 是此前原生 Windows 浏览器的源码。左右切换 APP 图片，上下切换 PAL 调色板；标题显示资源名、序号和尺寸。浏览器读取原始资源，不应用游戏中针对菜单的贴图修补，因此适合比对原始像素。调色板绑定需要手动选择，看到颜色异常不一定是解码错误。它与游戏共用 `src/assets/resource_archive.hpp`，修改解码器时应同时检查游戏和浏览器。

`verify_resources` 校验容器范围、APP 解压像素数、MAP 和 PAL 结构；通过不代表地图机关位置或游戏行为正确。

```powershell
python tools/resource_browser/analyze_resources.py "你的素材目录/RESOURCE.RES" build/resource-review --extract
python tools/resource_browser/decode_apps.py build/resource-review/extracted build/resource-preview --palette shin.pal
```

前者生成清单、哈希及可选原始 payload；后者生成 PNG、索引和前 120 项联系表。输出都是素材派生文件，仅留在本地。PNG 用指定调色板，不保证自动选择每张图正确的 PAL。批量导出会占用较多内存与磁盘空间。

## 2. 重建版关卡、Boss 和小游戏直达

```powershell
python tools/launch.py --stage 7 --exe "你的发行目录/SHIN.exe"
python tools/launch.py --boss 7 --exe "你的发行目录/SHIN.exe"
python tools/launch.py --bonus mole --exe "你的发行目录/SHIN.exe"
python tools/launch.py --menu --width 3440 --height 1440 --exe "你的发行目录/SHIN.exe"
```

默认使用 `build/Release/SHIN_Level1.exe`；素材由游戏查找可执行文件旁的 `assets/`，也可用 `--assets "完整路径/RESOURCE.RES"` 指定。默认传入 `--fresh-start`，不加载或写入正式进度存档；需要实际验证存档时显式传 `--use-save`。游戏仍可能生成配置和诊断日志。

`--stage 1..7` 使用玩家看到的关卡编号；旧的 `--scene stage3` 实际是第四关，第三关飞行射击对应 `shoot`。维护者应修改 launcher 的 SCENES 映射，不要再复制容易混淆的历史脚本。

`--boss 1..7` 使用游戏内 `debug_boss_entry(10)`，进入 Boss／结束触发点之前，并给现有库存武器各 10 件。飞行关卡操作和可用武器受其本身规则限制。此入口绕过部分正常流程，不能单独用来证明整关能通关。

小游戏参数：`car` 赛车、`card` 翻牌、`flag` 举旗、`mole` 打地鼠、`puzzle` 拼图。独立入口不用于验证关内解锁条件。

`shortcuts/` 提供 7 关入口、7 个 Boss 入口和 5 个小游戏的 CMD 包装，例如：

```powershell
.\tools\shortcuts\BOSS_WEAPONS10_07.cmd --exe "你的发行目录/SHIN.exe"
python tools/launch.py --boss 7 --dry-run
```

`--dry-run` 只输出最终参数，便于核对路径；`--headless --frames 10` 可做不显示窗口的载入检查。无窗口检查不代表画面、声音或完整战斗正确。

## 3. 快速检测入口

```powershell
python tools/quick_check.py --suite core
python tools/quick_check.py --suite resources --assets "你的素材目录"
python tools/quick_check.py --suite audio --assets "你的素材目录"
python tools/quick_check.py --test companion_hud_test --argument "你的素材目录/RESOURCE.RES"
```

默认构建所选目标后执行；已有构建可用 `--no-build`，自定义构建目录用 `--build-dir`。

- `core`：更新时钟、菜单流程、菜单 40～200 秒完整重复时序、存档恢复。时间戳模拟不需要真实等待 200 秒。
- `resources`：完整资源格式校验，需要本地 RESOURCE.RES。
- `audio`：菜单音频事件、音效队列和音乐交接，会调用本机音频设备；API 成功不等于已经人工确认音质。
- `--test`：现有任意 CMake 检测目标；参数用 `--argument` 传入，具体约定查看对应 `tests/*_test.cpp`。

此前散落在发行目录和 build 目录中的 verify CMD 大多硬编码 Visual Studio 路径和旧音频格式，本目录以 CMake 目标和统一入口替代，不再复制过时二进制或依赖特定电脑的编译命令。

## 4. 原版第七关 Boss 比对入口

```powershell
powershell -NoProfile -ExecutionPolicy RemoteSigned -File tools/original/Launch_Original_Boss.ps1 -OriginalGame "你自己的原版目录"
```

保留此前 PowerShell 和 C# 辅助源码。工具只启动指定原版，等待玩家手动进入第七关后，一次性调整该进程中的人物和镜头位置；不修改磁盘上的可执行文件。原版本身的正常存档行为仍然存在。

脚本只支持其内置 SHA-256 对应的已分析版本，地址表不能直接用于韩版、其他补丁或其他版本；不支持原版其余关卡的 Boss 直达。源代码中的 hash 校验不可移除后盲目重试。原版程序、数据和音轨不随仓库提供。不要对无关进程使用 MemoryAccess.cs。

## 5. 历史重建、审计和图标脚本

顶层原有 `audit_*` 是内容审计，`extract_*` 是结构或光盘音轨提取，`rebuild_*`、`restore_*` 是特定重建阶段的辅助生成脚本；`trace_to_keys.py`、`ppm_to_png.py` 用于记录转输入和图像转换。

这些历史脚本部分仍依赖 `shin_pseudocode` 中未公开的原版分析数据、特定本地绝对路径、旧报告或旧源码布局，不能视为开箱即用的构建步骤。运行前阅读脚本开头的路径及输出位置，先在工作副本中操作；不要直接对发布分支执行生成器。缺少分析输入时不能伪造通过结果。`make_switch_icon.py` 是旧素材拼接试验，当前图标来自作者提供的客厅图片，不应覆盖现有图标。

## 维护约定

运行工具产生的 build、提取素材、PNG、音轨、进程日志、个人密钥和游戏存档不得提交。新增跳转优先扩展 launch.py；新增检测优先建 CMake 目标并在 quick_check.py 登记。修改菜单声音时同时检查单次事件、完整重复周期、离开停止和重新进入后的计时重置。改进解码器时保留原始数据一致性验证，并在实际设备上试听。
