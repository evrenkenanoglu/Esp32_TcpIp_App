/*
 * tcpClient.h
 *
 *  Created on: Feb 29, 2020
 *      Author: 26026897
 * 
 * This code is configuration for IPV4
 *   
 */

#ifndef MAIN_TCPCLIENT_H_
#define MAIN_TCPCLIENT_H_

#include <string.h>
#include <stdint.h>

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include <sys/param.h>
/**
 * @brief params
 * 
 */

typedef enum teSocketStatus
{
    eSocketNotCreated=1,
    eSocketNotConnected,
    eSocketConnected,
    eSocketDisconnected,
}teSocketStatus;
typedef struct 
{
    const char *ipAddress;
    const uint32_t port;
    // const char *tag;
    int32_t socket;
    uint8_t socketStatus; 
    char rx_buffer[128];
    char AddressString[128];
    struct sockaddr_in destinationAddress;
    int32_t ipProtocol;
    int32_t addressFamily;
    uint32_t tryCountConnection;
}tsTcpClient;

extern int8_t tcpClientInit(tsTcpClient *client);
extern int8_t tcpClientConnect(tsTcpClient *client);
extern int8_t tcpClientClose(tsTcpClient *client);
extern int8_t tcpClientSend(tsTcpClient *client, const char *message);
extern int8_t tcpClientConnected(tsTcpClient *client);
extern int8_t tcpClientCheckConnectionStatus(tsTcpClient *client);

#define TCP_CLIENT_CREATE(_name, _ip, _port) \
    tsTcpClient _name = {                    \
        _ip,                                 \
        _port,                               \
        0,                                   \
        eSocketNotCreated,                   \
        "",                                  \
        "",                                  \
    };

#endif /* MAIN_TCPCLIENT_H_ */
