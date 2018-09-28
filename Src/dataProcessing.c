#include "dataProcessing.h"
#include "modbusSlave.h"
#include "adc.h"
#include "usart.h"
#include "dac.h"

static void gasCollect() {

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


void dataProcessing() {

	gasCollect();

}