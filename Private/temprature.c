#include "temprature.h"

#include "spi.h"
#include "stm32f4xx_hal.h"


void temp_receive() {
	uint8_t temp_Full[4];
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_SPI_Receive(&hspi2, temp_Full, 4, 100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	int temp_out = 0;
	temp_out = (temp_Full[1] << 4) | (temp_Full[2] >> 4);
	if (temp_out & (1<<11)) {
		//符号位为1
		temp_out = ~temp_out;
		for (int i = 12; i < 32; i++) {
			temp_out = temp_out ^ (1<<i);
		}
		temp_out++;
		temp_out = -temp_out;
	}
}
