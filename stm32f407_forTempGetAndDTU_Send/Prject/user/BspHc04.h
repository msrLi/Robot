
#ifndef BspHc04_h_
#define BspHc04_h_

#include "stm32f4xx.h"

#define A1_PORT    GPIOC
#define A1_PIN     GPIO_Pin_0
#define B1_PORT    GPIOC
#define B1_PIN     GPIO_Pin_1
#define C1_PORT    GPIOC
#define C1_PIN     GPIO_Pin_2

#define A2_PORT    GPIOC
#define A2_PIN     GPIO_Pin_3
#define B2_PORT    GPIOC
#define B2_PIN     GPIO_Pin_4
#define C2_PORT    GPIOC
#define C2_PIN     GPIO_Pin_5
void initHCf4051(void);
#endif 
