 /*
 * Brief:    Configures analogue inputs into the device, no uses for this yet
 *           but it works
 *
 * Uses:     
 *
 * Datasheet: 
 *
 * Copyright (C) 2015 Rajesh Nakarja
 * http://www.naklojik.com
 *
 * This software is licenced under the MIT Licence. For full terms visit:
 * http://opensource.org/licenses/MIT
 */

#include "stm32f4xx_hal.h"
#include "prototypes.h"
#include "config.h"

/* Public variables */
AnalogueOutput volatile AnalogueResult = {0,0,0,0};

/* Private variables */
volatile uint16_t rawADC[4];

/* Private structures */
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* Private function prototypes */


void initADC(void)
{
	/* Init ADC */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
	hadc1.Init.Resolution = ADC_RESOLUTION12b;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.NbrOfDiscConversion = 1;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 4;
	hadc1.Init.DMAContinuousRequests = ENABLE;
	hadc1.Init.EOCSelection = EOC_SEQ_CONV;
	HAL_ADC_Init(&hadc1);
	
	/* DMA settings in msp.c file */

	/* Configure Channels */
	ADC_ChannelConfTypeDef sConfig;
	
	/* Configure channel sample order*/
	sConfig.Channel = ADC_CHANNEL_13;
	sConfig.Rank = 4;
	sConfig.SamplingTime = ADCSAMPLES;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	sConfig.Channel = ADC_CHANNEL_10;
	sConfig.Rank = 1;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	sConfig.Channel = ADC_CHANNEL_11;
	sConfig.Rank = 2;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	sConfig.Channel = ADC_CHANNEL_12;
	sConfig.Rank = 3;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&rawADC, 4);
	print("\r\n[OK] ADC conversions started");
}

/* Put out values into a struct once complete */
/* This function cannot run currently as the DMA interrupts don't work */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc1)
{
	AnalogueResult.ch1 = rawADC[0];
	AnalogueResult.ch2 = rawADC[1];
	AnalogueResult.ch3 = rawADC[2];
	AnalogueResult.ch4 = rawADC[3];
}