/*
 * device_adc.h
 *
 *  Created on: Mar 6, 2020
 *      Author: 26026897
 */

#ifndef DEVICE_ADC_H_
#define DEVICE_ADC_H_

#include <stdint.h>

#include "driver/adc.h"
#if CONFIG_IDF_TARGET_ESP32
#include "esp_adc_cal.h"
#endif

typedef struct 
{
    adc_channel_t channel;
    adc_atten_t atten;
    uint8_t resolution;
    uint8_t numberOfSamples;
    uint32_t sensorResult;
}tsAdc;


extern uint8_t  adcInit(tsAdc *deviceAdc);
extern uint32_t adcGetRaw(tsAdc *deviceAdc);
extern uint32_t adcGetRawSampled(tsAdc *deviceAdc);

/**
 * @brief 
 * @param   _name           Device name
 * @param   _channel        ADC channel
 * @param   _atten          Attentuation
 *                          ADC_ATTEN_DB_0   = 0,
                            ADC_ATTEN_DB_2_5 = 1,
                            ADC_ATTEN_DB_6   = 2,
                            ADC_ATTEN_DB_11  = 3, 
 * @param   _resoluiton     Resolution of ADC in bits (9, 10, 11, 12)
 *                          ADC_WIDTH_BIT_9 
                            ADC_WIDTH_BIT_10
                            ADC_WIDTH_BIT_11
                            ADC_WIDTH_BIT_12
 * @param   _noOfSamples    Number of Samples
 */
#define ADC_DEVICE_CREATE(_name, _channel, _atten, _resolution, _noOfSamples) \
    tsAdc _name = {                                                           \
        ADC_CHANNEL_##_channel,                                                             \
        _atten,                                                               \
        _resolution,                                                          \
        (_noOfSamples == 0) ? 1 : _noSamples,                                 \
        0,                                                                    \
    };

#endif /* DEVICE_ADC_H_ */
