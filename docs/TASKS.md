# 任务与资源所有权

| 任务 | 优先级 | 配置栈 | 周期/等待 | 资源与失败策略 |
|---|---|---|---|---|
| UI | AboveNormal | 4096 bytes | 事件/300ms显示周期 | 唯一UI写入者，LCD与UI模型 |
| Network | Normal | 4096 bytes | Wi-Fi→TCP→MQTT→Ready/Fault | 消息回调只投递副本，队列4×200；原接收功能关闭 |

STM32 CMSIS-RTOS v1 适配器直接把 stacksize 传给 FreeRTOS xTaskCreate，因此这里是32位 words；AR CMSIS-RTOS2 是 bytes。配置值不是实测高水位。STM32堆24KiB，TM4C堆16KiB；MSP与newlib堆另由链接脚本预留。具体余量须测 uxTaskGetStackHighWaterMark 和空闲堆。
