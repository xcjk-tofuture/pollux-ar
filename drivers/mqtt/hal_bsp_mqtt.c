/*
 * Copyright (c) 2023 Beijing HuaQing YuanJian Education Technology Co., Ltd
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "lwip/netifapi.h"
#include "lwip/sockets.h"
#include "wifi_device.h"
#include "ohos_init.h"
#include "MQTTPacket.h"
#include "hal_bsp_mqtt.h"

#define MQTT_BUFF_MAX_SIZE 512

static int g_tcp_socket_fd = -1; // 网络套接字
static unsigned char mqttBuff[MQTT_BUFF_MAX_SIZE] = {0};

// 发送网络数据
static int transport_sendPacketBuffer(unsigned char *buf, int buflen) {
    int sent = 0, rc;
    while (sent < buflen) {
        rc = send(g_tcp_socket_fd, buf + sent, buflen - sent, 0);
        if (rc <= 0)
            return 0;
        sent += rc;
    }
    return 1;
}
// 接收网络数据
static int transport_getdata(unsigned char *buf, int count) {
    int received = 0, rc;
    while (received < count) {
        rc = recv(g_tcp_socket_fd, buf + received, count - received, 0);
        if (rc <= 0)
            return -1;
        received += rc;
    }
    return received;
}

// 连接服务器
int MQTTClient_connectServer(const char *ip_addr, int ip_port) {
    if (ip_addr == NULL || ip_port <= 0 || ip_port > 65535) {
        return -1;
    }

    int res = 0;                              // 函数返回值
    struct sockaddr_in tcpServerConfig = {0}; // tcp服务器信息

    // 创建TCP套接字
    g_tcp_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (g_tcp_socket_fd < 0) {
        printf("Failed to create Socket\r\n");
        return -1;
    }

    struct timeval timeout = {1, 0};
    if (setsockopt(g_tcp_socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0 ||
        setsockopt(g_tcp_socket_fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
        MQTTClient_unConnectServer();
        return -1;
    }
    // 连接TCP服务器
    tcpServerConfig.sin_family = AF_INET;                 // IPV4
    tcpServerConfig.sin_port = htons(ip_port);            // 填写服务器的IP端口号
    tcpServerConfig.sin_addr.s_addr = inet_addr(ip_addr); // 填写服务器的IP地址

    int nonblocking = 1;
    if (ioctl(g_tcp_socket_fd, FIONBIO, &nonblocking) < 0) {
        MQTTClient_unConnectServer();
        return -1;
    }
    res = connect(g_tcp_socket_fd, (struct sockaddr *)&tcpServerConfig, sizeof(tcpServerConfig));
    if (res < 0 && errno == EINPROGRESS) {
        fd_set writable;
        FD_ZERO(&writable);
        FD_SET(g_tcp_socket_fd, &writable);
        struct timeval connect_timeout = {5, 0};
        if (select(g_tcp_socket_fd + 1, NULL, &writable, NULL, &connect_timeout) > 0) {
            int socket_error = 0;
            socklen_t error_size = sizeof(socket_error);
            res = getsockopt(g_tcp_socket_fd, SOL_SOCKET, SO_ERROR, &socket_error, &error_size) ==
                              0 &&
                          socket_error == 0
                      ? 0
                      : -1;
        } else
            res = -1;
    }
    nonblocking = 0;
    if (ioctl(g_tcp_socket_fd, FIONBIO, &nonblocking) < 0)
        res = -1; // 连接服务器
    if (res == -1) {
        printf("Failed to connect to the server\r\n");
        MQTTClient_unConnectServer();
        return -1;
    }
    printf("Connection to server successful\r\n");

    return 0;
}

// 断开TCP服务器 0:成功, -1:失败
int MQTTClient_unConnectServer(void) {
    int ret = 0;
    printf("Server shut down successfully\r\n");
    ret = close(g_tcp_socket_fd);
    g_tcp_socket_fd = -1;
    return ret;
}

// mqtt客户端 订阅主题
int MQTTClient_subscribe(char *subTopic) {
    if (subTopic == NULL) {
        printf("Incorrect parameters\r\n");
        return -1;
    }

    int len = 0, res = 0;
    int subcount = 0, granted_qos = 0, req_qos = 0;
    unsigned short submsgid = 0;
    MQTTString topicString = MQTTString_initializer;

    /* subscribe */
    topicString.cstring = subTopic;
    len = MQTTSerialize_subscribe(mqttBuff, sizeof(mqttBuff), 0, 1, 1, &topicString, &req_qos);
    if (len <= 0) {
        printf("MQTTSerialize_subscribe Error %d\r\n", len);
        return -1;
    }
    res = transport_sendPacketBuffer(mqttBuff, len);
    if (res != 1) {
        printf("transport_sendPacketBuffer Error %d\r\n", res);
        return -1;
    }

    sleep(1);
    memset_s(mqttBuff, sizeof(mqttBuff), 0, sizeof(mqttBuff));

    /* wait for suback */
    if (MQTTPacket_read(mqttBuff, sizeof(mqttBuff), transport_getdata) != SUBACK) {
        printf("MQTTPacket_read Error\r\n");
        return -1;
    }

    if (MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, mqttBuff, sizeof(mqttBuff)) !=
        1) {
        printf("MQTTDeserialize_suback Error\r\n");
        return -1;
    }

    printf("MQTT subscribed to topics successfully\r\n");

    return 0;
}

// 保持在线时长 60s
#define MQTT_KEEP_ALIVE 60
#define MQTT_DELAY_TIME 3

// mqtt客户端 初始化
int MQTTClient_init(char *clientID, char *userName, char *password) {
    if (clientID == NULL || userName == NULL || password == NULL) {
        printf("Incorrect parameters\r\n");
        return -1;
    }

    int res = 0, len = 0, i = 0;
    int mqtt_read_len = 10;

    unsigned char sessionPresent = 0, connack_rc = 0;
    MQTTPacket_connectData mqttData = MQTTPacket_connectData_initializer;

    // 初始化MQTT客户端
    mqttData.clientID.cstring = clientID;
    mqttData.username.cstring = userName;
    mqttData.password.cstring = password;
    mqttData.cleansession = true; // 是否初始化的时候，清除上一次的对话
    mqttData.keepAliveInterval = MQTT_KEEP_ALIVE;

    // 组MQTT消息包
    len = MQTTSerialize_connect(mqttBuff, sizeof(mqttBuff), &mqttData);
    if (len <= 0) {
        printf("MQTTSerialize_connect Error %d\r\n", res);
        return -1;
    }
    res = transport_sendPacketBuffer(mqttBuff, len);
    if (res != 1) {
        printf("transport_sendPacketBuffer Error %d\r\n", res);
        return -1;
    }

    sleep(MQTT_DELAY_TIME);

    /* 打印发送出去的数据帧，调试用 */
    printf("MQTT_sendPacket:  \r\n");
    for (i = 0; i < len; i++) {
        printf("%x ", mqttBuff[i]);
    }
    printf("\r\n");

    memset_s(mqttBuff, sizeof(mqttBuff), 0, sizeof(mqttBuff));

    /* wait for connack */
    if (MQTTPacket_read(mqttBuff, sizeof(mqttBuff), transport_getdata) != CONNACK) {
        printf("MQTTPacket_read != CONNACK\r\n");
    }

    printf("MQTT_recvPacket:  \r\n");
    /* 打印服务器返回的消息，调试用 */
    for (i = 0; i < mqtt_read_len; i++) {
        printf("%x ", mqttBuff[i]);
    }
    printf("\r\n");

    if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, mqttBuff, sizeof(mqttBuff)) != 1 ||
        connack_rc != 0) {
        printf("Unable to connect, return code %d\r\n", connack_rc);
        memset_s(mqttBuff, sizeof(mqttBuff), 0, sizeof(mqttBuff));
        return -1;
    } else {
        printf("MQTT initialized successfully\r\n");
    }
    memset_s(mqttBuff, sizeof(mqttBuff), 0, sizeof(mqttBuff));
    return 0;
}

#define MQTT_PUB_DATA_TIME (100 * 1000)

int MQTTClient_pub(char *pub_Topic, unsigned char *payloadData, int payloadLen) {
    if (payloadData == NULL) {
        printf("Incorrect parameters\r\n");
        return -1;
    }

    printf("pubTopic: %s\n", pub_Topic);
    printf("pubData: %s\n", payloadData);

    int ret = 0, len = 0;
    unsigned short retry_count = 5; // 重发次数
    unsigned char sendBuff[MQTT_BUFF_MAX_SIZE] = {0};
    MQTTString topicString = MQTTString_initializer;

    topicString.cstring = pub_Topic;
    len = MQTTSerialize_publish(sendBuff, sizeof(sendBuff), 0, 0, 0, 0, topicString, payloadData,
                                payloadLen);
    while (--retry_count > 0) {
        ret = transport_sendPacketBuffer(sendBuff, len);
        if (ret == 1) {
            break;
        }

        printf("Send MQTT_Data Fail\r\n");
        usleep(MQTT_PUB_DATA_TIME);
    }

    if (!retry_count && ret != 1) {
        printf("transport_sendPacketBuffer Error %d\r\n", ret);
        return -1;
    }

    // printf("send==>%s", payloadData);
    return 0;
}

int8_t (*p_MQTTClient_sub_callback)(const unsigned char *, size_t, const unsigned char *, size_t);
int MQTTClient_sub(void) {
    int qos, length, type;
    unsigned char dup, retained, *payload;
    unsigned short message_id;
    MQTTString topic = MQTTString_initializer;
    memset(mqttBuff, 0, sizeof(mqttBuff));
    type = MQTTPacket_read(mqttBuff, sizeof(mqttBuff), transport_getdata);
    if (type <= 0)
        return -1;
    if (type != PUBLISH)
        return 0;
    if (MQTTDeserialize_publish(&dup, &qos, &retained, &message_id, &topic, &payload, &length,
                                mqttBuff, sizeof(mqttBuff)) != 1 ||
        length < 0 || topic.lenstring.len < 0 || !payload || !topic.lenstring.data)
        return -1;
    if (p_MQTTClient_sub_callback)
        return p_MQTTClient_sub_callback((const unsigned char *)topic.lenstring.data,
                                         (size_t)topic.lenstring.len, payload, (size_t)length);
    return 0;
}
