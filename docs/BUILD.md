# Pollux AR 构建

保留原 Hi3861 SDK、GN/hb、LiteOS-M 与 CMSIS-RTOS2。`app/BUILD.gn` 列出应用源文件和原 SDK 依赖，各驱动保留各自 GN 配置。
在匹配的原 SDK 中接入 applications 路径及构建依赖，使用该 SDK 的 hb 配置；仓库不能独立替代完整 SDK，精确 SDK 版本未能唯一确认。

Wi-Fi/MQTT 部署参数在 `boards/hi3861/beihe_board.h`。原接收开关默认关闭，模型与消息接口不等于持续联网/UI 页面已完成。

主机检查：

```sh
python tests/run_host.py --cc gcc --cjson-directory "<cJSON源码目录>"
```

将示例中的路径替换为包含 cJSON.c/cJSON.h 的实际目录。消息/模型测试使用测试专用 cJSON 1.7.18，不替换目标 SDK。完整 SDK 构建和上板行为未验证；恢复使用需核对 GN、Wi-Fi/lwIP/CMSIS 接口、网络故障、显示和栈水位。
原源码提交 `b1c5644df9f17a4b0873ca657f8b0874779f9cde` 可在独立 checkout 查看。
