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


static void dacOutput() {

	switch (localArray[17])
	{
	case 0: HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0);//0V
		break;
	case 1: HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 550);//1V
		break;
	case 2: HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 1000);//2V
		break;
	case 3: HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 1400);//3V
		break;
	case 4: HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 1750);//4V
		break;
	case 5: HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 2100);//5V
		break;
	case 6: HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 2500);//6V
		break;
	case 7: HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 2900);//7V
		break;
	case 8: HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 3260);//8V
		break;
	case 9: HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 3655);//9V
		break;
	case 10: HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 4095);//10V
		break;

	default:
		break;
	}

	switch (localArray[18])
	{
	case 0: HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0);//0V
		break;
	case 1: HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 550);//1V
		break;
	case 2: HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 1000);//2V
		break;
	case 3: HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 1400);//3V
		break;
	case 4: HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 1750);//4V
		break;
	case 5: HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 2100);//5V
		break;
	case 6: HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 2500);//6V
		break;
	case 7: HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 2900);//7V
		break;
	case 8: HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 3260);//8V
		break;
	case 9: HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 3655);//9V
		break;
	case 10: HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 4095);//10V
		break;

	default:
		break;
	}

}


void dataProcessing() {

	adcCollect();
	switchInCollect();
	relayControl();
	dacOutput();
}