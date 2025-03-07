#include <stdbool.h>
#include "EXTI.h"
#include "tim.h"

int64_t time_ms;
char time_c[15];
int ms_counter=0;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if (htim == &htim6){
        time_ms++;
        ms_counter++;
    }
}