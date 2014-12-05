
#ifndef actuatorFunction_H
#define actuatorFunction_H
#include"stm32f4xx.h"
#include"Bsp_PWM.h"
#define MOTOR_1 1
#define MOTOR_2 2
#define MOTOR_3 3
#define MOTOR_4 4

#define SERVI_1 1
#define SERVI_2 1
#define SERVI_3 1
#define SERVI_4 1
#define SERVI_5 2
#define SERVI_6 2
#define SERVI_7 2
#define SERVI_8 2

/* 
*  用于初始化 电机模块 
**/
void motorInit(uint8_t moterNumber);

/*
*   初始化舵机
**/
void servoInit(uint8_t moterNumber);
#endif 
