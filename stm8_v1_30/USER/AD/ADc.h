
#ifndef ADc_H_
#define ADc_H_
#include "stm8s.h"

/* 
*  name : InitADC
*       : 用于描述AD初始化
**/
void InitADC(void);

/*
*  name : readADCs
*       : 读取AD值得函数
**/
uint16_t readADCs(void);

#endif 


