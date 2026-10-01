# 原海思环境（存档）

保留 GN/hb、原 LiteOS-M/CMSIS-RTOS2 和 Hi3861 SDK。把目录按原 SDK 的 applications 路径接入 GN 依赖，使用其既有 hb 构建配置。当前未取得 SDK 根目录，用户已明确无需 SDK 完整编译；未把 AR 换成 ARM GCC/CMake/OpenOCD。SDK版本不能从仓库唯一确认。主机测试使用测试专用 cJSON 1.7.18，不替换目标SDK的cJSON。

Wi-Fi/MQTT 参数位于 boards/hi3861/beihe_board.h；若日后恢复使用，应在原环境配置本机部署值。
