# 智能家居系统

基于嵌入式 Linux（GEC 开发板）的触摸屏智能家居控制系统，使用 C 语言开发，支持 LCD 显示、触摸交互、音乐播放、传感器采集与 LED 控制等功能。

## 功能模块

系统主界面（`smarthome.bmp`）提供五个功能入口：

| 模块 | 说明 |
|------|------|
| DVD 播放器 | 音乐播放，支持上/下一首、暂停/继续、音量调节、自定义播放列表 |
| 环境检测 | 读取 DHT11 温湿度、水位传感器、MQ2 烟雾传感器数据 |
| 跑马灯 | 独立或全体控制 D7/D8/D9 三路 LED 灯 |
| 相册 | BMP 图片浏览，支持自动轮播（2s）和手动滑动切换 |
| 钢琴 | 虚拟 7 键钢琴，触摸按键播放对应音阶音效 |

## 目录结构

```
last_project/
├── main2.c          # 主程序（完整版，包含全部五个功能模块）
├── main.c           # 早期版本（仅含相册和音乐选择功能）
├── touch.c/h        # 触摸屏驱动（独立线程，读取 /dev/input/event0）
├── lcd.c/h          # LCD 显示驱动（800×480，mmap 帧缓冲）
├── bmp.c/h          # BMP 图片解析与显示
├── dev.c/h          # 硬件设备控制（LED、蜂鸣器、DHT11、MQ2）
├── thread.c         # 多线程功能测试示例
├── *.bmp            # UI 界面图片资源
├── 1.mp3 ~ 5.mp3   # 背景音乐
└── piano_1.mp3 ~ piano_7.mp3  # 钢琴音阶音效
```

## 硬件依赖

- **LCD**：800×480 分辨率，通过 `/dev/fb0` framebuffer 驱动
- **触摸屏**：电阻屏，通过 `/dev/input/event0` 读取原始事件
- **LED**：D7 / D8 / D9 / D10，通过 `/sys/kernel/gec_ctrl/led_dX` 控制
- **蜂鸣器**：通过 `/sys/kernel/gec_ctrl/beep` 控制
- **DHT11**：温湿度传感器
- **MQ2**：烟雾传感器
- **音频**：使用 `madplay` 命令行播放 MP3

## 触摸交互逻辑

触摸线程（`get_touch`）在独立 pthread 中持续运行，识别两类手势：

- **点击**：起点到终点偏移 < 50px，设置 `flag=0`，记录坐标 `(endx, endy)`
- **滑动**：偏移 ≥ 50px，设置 `flag=1`，并判断 `move_flag`（Up/Down/Left/Right）

主循环通过检测 `flag`、`endx`、`endy` 坐标范围来响应触摸区域。

全局返回按钮固定在屏幕右上角（x: 700~800，y: 0~60），点击后退回主菜单并释放资源。

## 编译与运行

```bash
# 交叉编译（针对 ARM 目标板）
arm-linux-gnueabihf-gcc main2.c touch.c lcd.c bmp.c dev.c -o smart_home -lpthread

# 在目标板上运行（需 root 权限访问硬件）
./smart_home
```

> 依赖：`madplay`（需预装于目标板），`pthread` 库

## 主要全局状态变量

| 变量 | 说明 |
|------|------|
| `begin` | 0=主界面，1=子功能界面 |
| `smarthome_order` | 当前功能模块（1~5） |
| `flag` | 触摸模式：0=点击，1=滑动，-1=默认 |
| `endx / endy` | 当前触摸终点坐标 |
| `move_flag` | 滑动方向：1=Up，2=Down，3=Left，4=Right |
| `exti_flag` | 退出标志：1=退出当前子模块 |
| `music_logo` | 当前播放音乐索引 |
| `environment_flag` | 环境检测子模式（1=温湿度，2=水位，3=烟雾） |
| `photo_flag` | 相册模式（1=自动，2=手动） |
