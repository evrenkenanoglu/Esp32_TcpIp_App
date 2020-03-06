/*
 * tcpClient.c
 *
 *  Created on: Feb 29, 2020
 *      Author: 26026897
 */

#include "tcpClient.h"

static const char *clientTag = "TcpClient";

int8_t tcpClientInit(tsTcpClient *client)
{
    client->destinationAddress.sin_addr.s_addr = inet_addr(client->ipAddress);
    client->destinationAddress.sin_family = AF_INET;
    client->destinationAddress.sin_port = htons(client->port);
    client->addressFamily = AF_INET;
    client->ipProtocol = IPPROTO_IP; 
    inet_ntoa_r(client->destinationAddress.sin_addr, client->AddressString, sizeof(client->AddressString - 1));
    ESP_LOGI(clientTag, "Configuration Done!");
    
    client->socket = socket(client->addressFamily, SOCK_STREAM, client->ipProtocol);
    if(client->socket <0)
    {
        ESP_LOGE(clientTag, "Unable to create socket: errno %d", errno);
    }
    ESP_LOGI(clientTag, "Socket created, connecting to %s:%d", client->ipAddress, client->port);

    return ESP_OK;
}

int8_t tcpClientConnect(tsTcpClient *client)
{
    
    if(!(client->socket < 0))
    {
        int err = connect(client->socket, (struct sockaddr *)&client->destinationAddress, sizeof(client->destinationAddress));
        if(err != 0)
        {
            ESP_LOGE(clientTag, "Socket unable to connect: errno %d", errno);
            return ESP_FAIL;
        }
        ESP_LOGI(clientTag, "Successfully connected");
        
    }
    else
    {
        ESP_LOGE(clientTag, "Socket has not been created!");
        return ESP_FAIL;
    }
    return ESP_OK;
}

int8_t tcpClientSend(tsTcpClient *client, const char *message)
{

    int err = send(client->socket, message, strlen(message), 0);
    if (err < 0)
    {
        ESP_LOGE(clientTag, "Error occurred during sending: errno %d", errno);
        return ESP_FAIL;
    }
    else
    {
        ESP_LOGI(clientTag, "Message sending successful: %s", message);
        return ESP_OK;
    }
}
