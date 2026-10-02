# Pollux AR 架构

## 应用入口

`app/main.c` 通过 `SYS_RUN(beihe_start)` 注册到原海思/OpenHarmony 启动框架。
`beihe_start` 创建深度 4 的事件邮箱，再创建 UI 和 Network 两个 CMSIS-RTOS2 线程。两线程栈各 4096 字节，UI 为 AboveNormal，Network 为 Normal。

## 模块与数据流

| 目录 | 职责 |
|---|---|
| `app` | 连接编排、消息回调与 UI 事件消费 |
| `services/messages.c` | cJSON 检查 `center` 字符串 |
| `services/ui_model.c` | 连接状态、文本和解析错误计数 |
| `os/beihe_mailbox.c` | 有界事件副本传递，非阻塞投递 |
| `drivers/wifi`、`drivers/mqtt` | 原网络设备接口 |
| `drivers/spi_st7789` | 显示初始化及基线绘制 |
| `platform/hi3861` | 板载 IO 适配 |
| `boards/hi3861/beihe_board.h` | 部署配置、节拍和接收开关 |

Network：Wi-Fi → TCP → MQTT → 订阅 → Ready/故障。
`on_payload` 校验 topic 与长度后复制消息到邮箱；UI 线程取事件，解析 JSON 并更新自己拥有的模型。回调不修改显示业务。

`BEIHE_ENABLE_MQTT_RECEIVE=0`，订阅后网络线程退出，不运行持续接收或重连。
消息模型更新后只打印文本；等待超时时调用 `beihe_display_baseline` 绘制原测试图案。模型文本尚未接入完整消息页面。

没有独立业务算法需求的目录不强行填充。该工程使用 Wi-Fi/MQTT，不采用底盘/无人机的串口协议。
