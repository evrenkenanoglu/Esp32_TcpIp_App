/*
 * tcpClient.c
 *
 *  Created on: Feb 29, 2020
 *      Author: 26026897
 */

#include "tcpClient.h"

#define SOCKET_NOT_CONNECTED    0

static const char *clientTag = "TcpClient";


int8_t tcpClientInit(tsTcpClient *client)
{
    client->socketStatus = eSocketNotCreated;
    client->destinationAddress.sin_addr.s_addr = inet_addr(client->ipAddress);
    client->destinationAddress.sin_family = AF_INET;
    client->destinationAddress.sin_port = htons(client->port);
    client->addressFamily = AF_INET;
    client->ipProtocol = IPPROTO_IP; 
    inet_ntoa_r(client->destinationAddress.sin_addr, client->AddressString, sizeof(client->AddressString - 1));
    ESP_LOGI(clientTag, "Configuration Done!");
    
    client->socket = socket(client->addressFamily, SOCK_STREAM, client->ipProtocol); // Socket Creating
    if(client->socket <0)
    {
        ESP_LOGE(clientTag, "Unable to create socket: errno %d", errno);
        client->socketStatus = eSocketDisconnected;     // To create new healty socket, close first!
        return ESP_FAIL;
    }
    ESP_LOGI(clientTag, "Socket created, connecting to %s:%d", client->ipAddress, client->port);
    client->socketStatus = eSocketNotConnected;         // Socket created but not connected

    return ESP_OK;
}

int8_t tcpClientConnect(tsTcpClient *client)
{
    if (!(client->socket < 0))
    {
        int err = connect(client->socket, (struct sockaddr *)&client->destinationAddress, sizeof(client->destinationAddress));
        if (err != 0) // Connection Error 
        {
            ESP_LOGE(clientTag, "Socket unable to connect: errno %d", errno);
            client->socketStatus = eSocketDisconnected; // To create new Socket, close socket first
            return ESP_FAIL;
        }
        client->socketStatus = eSocketConnected;
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
        client->socketStatus = eSocketDisconnected; // To create new Socket, close socket first
        return ESP_FAIL;
    }
    else
    {
        ESP_LOGI(clientTag, "Message sending successful: %s", message);
        client->socketStatus = eSocketConnected;
        return ESP_OK;
    }
}


int8_t tcpClientReceive(tsTcpClient *client)
{
    int lenght = recv(client->socket, client->rx_buffer, sizeof(client->rx_buffer) - 1, 0);
    // Error during data Receive
    if (lenght < 0)
    {
        ESP_LOGE(clientTag, "recv failed: errno %d", errno);
        client->socketStatus = eSocketDisconnected; // To create new Socket, close socket first
        return ESP_FAIL;
    }
    // Data received
    else
    {
        client->rx_buffer[lenght] = 0; // Null-terminate whatever we received and treat like a string
        ESP_LOGI(clientTag, "Received %d bytes from %s:", lenght, client->AddressString);
        ESP_LOGI(clientTag, "%s", client->rx_buffer);
        client->socketStatus = eSocketConnected;
        return ESP_OK;
    }
}

int8_t tcpClientCheckConnectionStatus(tsTcpClient *client)
{
    return client->socketStatus;
}

int8_t tcpClientClose(tsTcpClient *client)
{
    ESP_LOGE(clientTag, "Shutting down socket");
    shutdown(client->socket, 0);
    close(client->socket); 
    client->socketStatus = eSocketNotCreated;
    return ESP_OK;   
}
