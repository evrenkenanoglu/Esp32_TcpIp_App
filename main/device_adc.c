/*
 * device_adc.c
 *
 *  Created on: Mar 6, 2020
 *      Author: 26026897
 */

#include "device_adc.h"

static esp_adc_cal_characteristics_t *adc_chars;
#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate

uint8_t adcInit(tsAdc *adc)
{
    adc1_config_width(adc->resolution);
    adc1_config_channel_atten(adc->channel, adc->atten);
    
    #if CONFIG_IDF_TARGET_ESP32
    //Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, adc->atten, adc->resolution, DEFAULT_VREF, adc_chars);
    #endif

    return ESP_OK;
}

uint32_t adcGetRaw(tsAdc *adc)
{
    adc->sensorResult = adc1_get_raw(adc->channel);
    return adc->sensorResult;
}

uint32_t adcGetRawSampled(tsAdc *adc)
{
    for(int i=1; i<=adc->numberOfSamples; i++)
    {
        adc->sensorResult += adc1_get_raw(adc->channel);
    }
    return adc->sensorResult/adc->numberOfSamples;
}





