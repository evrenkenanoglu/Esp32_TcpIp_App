/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "driver/adc.h"
#include "driver/gpio.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_spi_flash.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "nvs_flash.h"
#include "sdkconfig.h"
#include <stdio.h>

#include "connectNetwork.h"
#include "device_adc.h"
#include "tcpClient.h"

/****************************************************************************************************
 * 											    MACROS
 *****************************************************************************************************/
#define NUMBER_OF_SAMPLES 50
ADC_DEVICE_CREATE(adcNTC, ADC_CHANNEL_6, ADC_ATTEN_DB_0, ADC_WIDTH_10Bit, NUMBER_OF_SAMPLES)

#define HOST_IP "192.168.43.1"
#define PORT    3000
TCP_CLIENT_CREATE(test, HOST_IP, PORT)

#define TASK_DELAY_TCP_TASK   1000 // ms
#define TASK_DELAY_ADC        1000 // ms
#define TASK_DELAY_ONE_SECOND 1000 // ms
#define TASK_DELAY_LED        250  // ms

#define BLINK_GPIO            CONFIG_BLINK_GPIO
#define BLINK_LED_BLUE        GPIO_NUM_2

/****************************************************************************************************
 * 											    MACROS END
 *****************************************************************************************************/

const char* ssid   = "EvrenKenanoglu";
const char* pass   = "12345678";
const char* tagAdc = "ADC";
/****************************************************************************************************
 * 											    FUNCTIONS PROTOTYPE
 *****************************************************************************************************/
void        initialize();
void        sendAdcValue(uint32_t adcValue);
static void startupDelay();

static void tcpClientTask(void* pvParameters);
static void ledTask(void* Parameters);
static void adcReadingTask(void* pvParameters);
/****************************************************************************************************
 * 											    FUNCTIONS PROTOTYPE END
 *****************************************************************************************************/

void app_main(void)
{
    initialize();
    xTaskCreate(tcpClientTask, "tcp_client", 4096, NULL, 5, NULL);
    // xTaskCreate(adcReadingTask, "adc_reading", 4096, NULL, 1, NULL );
    xTaskCreate(ledTask, "network", 4096, NULL, 5, NULL);

    fflush(stdout);
}

void tcpClientTask(void* pvParameters)
{
    while (1)
    {
        switch (tcpClientCheckConnectionStatus(&test))
        {
            case eSocketNotCreated:
            {
                tcpClientInit(&test);
                vTaskDelay(TASK_DELAY_TCP_TASK / portTICK_PERIOD_MS);
            } // 100ms
            break;

            case eSocketNotConnected:
            {
                tcpClientConnect(&test);

                vTaskDelay(TASK_DELAY_TCP_TASK / portTICK_PERIOD_MS); // 100ms
            }
            break;

            case eSocketConnected:
                sendAdcValue(adcGetRawSampled(&adcNTC));
                vTaskDelay(TASK_DELAY_TCP_TASK / portTICK_PERIOD_MS);
                break;

            case eSocketDisconnected:
                tcpClientClose(&test);
                vTaskDelay(TASK_DELAY_TCP_TASK / portTICK_PERIOD_MS); // 100ms
                break;
        }
    }
    vTaskDelete(NULL);
}

void ledTask(void* pvParameters)
{
    for (;;)
    {
        static uint8_t state = 0;
        if (state)
        {
            gpio_set_level(BLINK_LED_BLUE, 0);
            gpio_set_level(BLINK_GPIO, 0);
            printf("\nLED's are OFF!\n");
        }
        else
        {
            gpio_set_level(BLINK_LED_BLUE, 1);
            gpio_set_level(BLINK_GPIO, 1);
            printf("\nLED's are ON!\n");
        }
        state = !state;
        vTaskDelay(TASK_DELAY_LED / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

static void adcReadingTask(void* pvParameters)
{
    while (1)
    {

        ESP_LOGI(tagAdc, "Value Sampled     : %d\n", adcGetRawSampled(&adcNTC));
        ESP_LOGI(tagAdc, "Value Raw         : %d\n", adcGetRaw(&adcNTC));
        ESP_LOGI(tagAdc, "Value Raw Voltage : %d\n", adcGetVoltage(&adcNTC));

        vTaskDelay(TASK_DELAY_ONE_SECOND / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

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

    gpio_reset_pin(BLINK_LED_BLUE);
    gpio_pad_select_gpio(BLINK_LED_BLUE);
    gpio_set_direction(BLINK_LED_BLUE, GPIO_MODE_OUTPUT);

    gpio_reset_pin(BLINK_GPIO);
    gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    startupDelay();
    connectNet(ssid, pass);
    adcInit(&adcNTC);
}

void startupDelay()
{
    for (int i = 3; i >= 0; i--)
    {
        printf("Starting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
