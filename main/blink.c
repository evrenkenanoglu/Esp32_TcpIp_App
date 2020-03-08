/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/timers.h"
#include "driver/adc.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"

#define NUMBER_OF_SAMPLES       50 
#include "device_adc.h"
ADC_DEVICE_CREATE(adcNTC, 6, ADC_ATTEN_0db, ADC_WIDTH_10Bit, NUMBER_OF_SAMPLES)


#include "connectNetwork.h"
#include "tcpClient.h"
#define HOST_IP "192.168.43.1"
#define PORT 3000
TCP_CLIENT_CREATE(test, HOST_IP, PORT)

#define TASK_DELAY_TCP_TASK     2000 // ms
#define TASK_DELAY_ADC          1000 // ms

#define BLINK_GPIO      CONFIG_BLINK_GPIO
#define BLINK_LED_13    13

const char *ssid = "EvrenKenanoglu";
const char *pass = "12345678";



/****************************************************************************************************
* 											    FUNCTIONS PROTOTYPE
*****************************************************************************************************/
void initialize();
void sendAdcValue(uint32_t adcValue);
static void startupDelay(); 

static void adcReadingTask(void *pvParameters);
static void tcpClientTask(void *pvParameters);
/****************************************************************************************************
* 											    FUNCTIONS PROTOTYPE END
*****************************************************************************************************/

void app_main(void)
{
    initialize();
    tcpClientConnect(&test);
    xTaskCreate(adcReadingTask, "adc_reading", 1024, NULL, 1, NULL );
    xTaskCreate(tcpClientTask, "tcp_client", 4096, NULL, 5, NULL);
    fflush(stdout);
}

static void tcpClientTask(void *pvParameters)
{
    while(1)
    {
        sendAdcValue(adcGetRawSampled(&adcNTC));
        vTaskDelay(TASK_DELAY_TCP_TASK / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

static void adcReadingTask(void *pvParameters)
{
    
}
// static void AdcSamplingTask(void *pvParameters)
// {
//     for(int i=0; )
//     vTaskDelay();
// }
void sendAdcValue(uint32_t adcValue)
{ 
    char message[128] = ""; 
    sprintf(message, "ADC Value : %d\n", adcValue);
    tcpClientSend(&test, message);
}

void initialize()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    gpio_pad_select_gpio(BLINK_LED_13);
    gpio_set_direction(BLINK_LED_13, GPIO_MODE_OUTPUT);

    startupDelay();
    connectNet(ssid, pass);
    adcInit(&adcNTC);
    tcpClientInit(&test);
}

void startupDelay()
{
    for (int i = 3; i >= 0; i--)
    {
        printf("Starting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
