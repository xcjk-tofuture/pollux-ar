# Hi3861 平台边界

board_io 封装板级 GPIO，UI 任务通过显示设备接口操作 ST7789。保留海思 LiteOS-M、CMSIS-RTOS2、GN/hb 工具链与原 SDK，网络库继续使用现有 lwIP 与 Paho。TCP 建连通过非阻塞 connect/select 设 5 秒截止，收发套接字 1 秒超时。lwIP 版本和编译选项依赖外部 SDK，必须用实际 SDK 完成编译验证；主机模型测试不能替代这一步。
