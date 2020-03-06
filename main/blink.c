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
#include "connectNetwork.h"
#include "driver/adc.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"

//** ADC Includes **//
#include "driver/adc.h"
#include "esp_adc_cal.h"
//** ADC Includes **//

#include "tcpClient.h"
#define HOST_IP "192.168.43.1"
#define PORT 3000
TCP_CLIENT_CREATE(test, HOST_IP, PORT)

#define TASK_DELAY_TCP_TASK     2000 // ms
#define TASK_DELAY_ADC          100 // ms
#define NUMBER_OF_SAMPLES       20
// #if CONFIG_IDF_TARGET32
// static esp_adc_cal_characteristics_t *adc_characs;
// const static adc_channel_t channel = ADC_CHANNEL_6
// #elif CONFIG_IDF_TARGET_ESP32S2
// const static adc_channel_t channel = ADC_CHANNEL_6
// #endif
// static  ad

#define BLINK_GPIO      CONFIG_BLINK_GPIO
#define BLINK_LED_13    13

const char *ssid = "EvrenKenanoglu";
const char *pass = "12345678";

void initialize();
void sendAdcValue(uint32_t adcValue);
static void tcpClientTask(void *pvParameters);
// static void AdcTask(void *pvParameters);

void app_main(void)
{
    initialize();
    tcpClientConnect(&test);
    xTaskCreate(tcpClientTask, "tcp_client", 4096, NULL, 5, NULL);
    fflush(stdout);
}

static void tcpClientTask(void *pvParameters)
{
    while(1)
    {
        sendAdcValue(50);
        vTaskDelay(TASK_DELAY_TCP_TASK / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
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

    for (int i = 3; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    connectNet(ssid,pass); 
    tcpClientInit(&test);
}
