/*
 * connectNetwork.h
 *
 *  Created on: Feb 23, 2020
 *      Author: 26026897
 */

#ifndef NETWORK_CONNECTNETWORK_H_
#define NETWORK_CONNECTNETWORK_H_



#include <stdint.h>

#include "esp_err.h"
#include "esp_netif.h"

#define CONFIG_EXAMPLE_CONNECT_WIFI 1


#ifdef CONFIG_EXAMPLE_CONNECT_ETHERNET
#define EXAMPLE_INTERFACE get_example_netif()
#endif

#ifdef CONFIG_EXAMPLE_CONNECT_WIFI
#define EXAMPLE_INTERFACE get_example_netif()
#endif


typedef struct
{
    char ssid[32];  
    char password[64];
    wifi_config_t wifi_config;

}connectNetwork;

connectNetwork params;

/**
 * @brief Construct a new connect Network object
 * 
 */
extern uint8_t connectNet(const char *ssid ,const char *password);


/**
 * @brief Configure Wi-Fi or Ethernet, connect, wait for IP
 *
 * This all-in-one helper function is used in protocols examples to
 * reduce the amount of boilerplate in the example.
 *
 * It is not intended to be used in real world applications.
 * See examples under examples/wifi/getting_started/ and examples/ethernet/
 * for more complete Wi-Fi or Ethernet initialization code.
 *
 * Read "Establishing Wi-Fi or Ethernet Connection" section in
 * examples/protocols/README.md for more information about this function.
 *
 * @return ESP_OK on successful connection
 */
esp_err_t connectFunc(void);

/**
 * Counterpart to connect, de-initializes Wi-Fi or Ethernet
 */
esp_err_t example_disconnect(void);

/**
 * @brief Configure stdin and stdout to use blocking I/O
 *
 * This helper function is used in ASIO examples. It wraps installing the
 * UART driver and configuring VFS layer to use UART driver for console I/O.
 */
esp_err_t example_configure_stdin_stdout(void);

/**
 * @brief Returns esp-netif pointer created by connect()
 *
 */
esp_netif_t *get_example_netif(void);

#endif /* NETWORK_CONNECTNETWORK_H_ */
