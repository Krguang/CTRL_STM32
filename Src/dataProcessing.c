#include "dataProcessing.h"
#include "modbusSlave.h"
#include "adc.h"
#include "usart.h"
#include "dac.h"

static void adcCollect() {

	uint16_t gasTemp[11];

	for (uint8_t i = 0; i < 100; i++)
	{
		for (uint8_t j = 0; j < 11; j++) {
			ADC_Average[j] += ADC_ConvertedValue[j];
		}
	}

	for (uint8_t i = 0; i < 11; i++)
	{

		if (i<7)
		{
			if (ADC_Average[i]<84000)
			{
				ADC_Average[i] = 84000;
			}
			gasTemp[i] = (uint16_t)((ADC_Average[i] / 100 - 840) * 999 / 3255);

			localArray[i + 5] = gasTemp[i];
		}
		else
		{
			gasTemp[i] = (uint16_t)(ADC_Average[i] / 100 * 1000 / 4096);
			localArray[i - 6] = gasTemp[i];
		}

		ADC_Average[i] = 0;
	}
}


static void switchInCollect() {

	if (1 == HAL_GPIO_ReadPin(io_in_geli_GPIO_Port,io_in_geli_Pin))
	{
		localArray[0] |= 1;
	}
	else
	{
		localArray[0] &= ~1;
	}

	if (1 == HAL_GPIO_ReadPin(io_in_huozai_GPIO_Port, io_in_huozai_Pin))
	{
		localArray[0] |= 1<<1;
	}
	else
	{
		localArray[0] &= ~(1<<1);
	}
	 
	if (1 == HAL_GPIO_ReadPin(io_in_beiyong1_GPIO_Port, io_in_beiyong1_Pin))
	{
		localArray[0] |= 1 << 2;
	}
	else
	{
		localArray[0] &= ~(1 << 2);
	}

	if (1 == HAL_GPIO_ReadPin(io_in_beiyong2_GPIO_Port, io_in_beiyong2_Pin))
	{
		localArray[0] |= 1 << 3;
	}
	else
	{
		localArray[0] &= ~(1 << 3);
	}

}

static void relayControl() {

	int relayTemp = localArray[16];

	if (1 == (relayTemp & 1))
	{
		HAL_GPIO_WritePin(relay_wuying_GPIO_Port, relay_wuying_Pin,GPIO_PIN_SET);
	}
	else {
		HAL_GPIO_WritePin(relay_wuying_GPIO_Port, relay_wuying_Pin, GPIO_PIN_RESET);
	}

	if (1 == ((relayTemp >>1) & 1))
	{
		HAL_GPIO_WritePin(relay_guanpian_GPIO_Port, relay_guanpian_Pin, GPIO_PIN_SET);
	}
	else {
		HAL_GPIO_WritePin(relay_guanpian_GPIO_Port, relay_guanpian_Pin, GPIO_PIN_RESET);
	}

	if (1 == ((relayTemp >> 2) & 1))
	{
		HAL_GPIO_WritePin(relay_shuzhong_GPIO_Port, relay_shuzhong_Pin, GPIO_PIN_SET);
	}
	else {
		HAL_GPIO_WritePin(relay_shuzhong_GPIO_Port, relay_shuzhong_Pin, GPIO_PIN_RESET);
	}

	if (1 == ((relayTemp >> 3) & 1))
	{
		HAL_GPIO_WritePin(relay_chouyang_GPIO_Port, relay_chouyang_Pin, GPIO_PIN_SET);
	}
	else {
		HAL_GPIO_WritePin(relay_chouyang_GPIO_Port, relay_chouyang_Pin, GPIO_PIN_RESET);
	}

	if (1 == ((relayTemp >> 4) & 1))
	{
		HAL_GPIO_WritePin(relay_beiyong_GPIO_Port, relay_beiyong_Pin, GPIO_PIN_SET);
	}
	else {
		HAL_GPIO_WritePin(relay_beiyong_GPIO_Port, relay_beiyong_Pin, GPIO_PIN_RESET);
	}
}


void dataProcessing() {

	adcCollect();
	switchInCollect();
	relayControl();
}