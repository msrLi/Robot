
#ifndef dataStory_H_
#define dataStory_H_
#include "stm32f4xx.h"
#define MAXCOM     16
typedef enum
{
	LED=0,
	LCD=1
}MODE;
typedef struct
{
	char name[10];   // 模块名称
	MODE modeType;   // 模块类型
	uint8_t Online;  // 数据获取是成功 
									 // 1 --成功 
									 // 0 ---失败
	uint16_t * dataPoint;  // 数据指针 
	uint8_t DeviceID;      // 收到的设备ID
}COMDATA;            		 // 设备描述 
extern COMDATA comPortData[MAXCOM];   // 设备描述变量 
extern uint16_t Values[MAXCOM][2];
/* 
* name : PortConfig
* MODDE:
**/
void PortConfig(MODE modes,uint8_t comPort,char buff[]);
/*
* name : GiveValue 
*     :: 用于复制给相应的变量 
**/
void GiveValue(void);
/*
*  name : DevieceConfig
*  canshu: com 端口号 0--15  
*  buff  : 名称号
**/
void DevieceConfig(uint8_t com,char *buff);
/*
* name : GetValue
* buff : 返回名称
**/
uint16_t GetValue(char buff[]);
/*
* name : GetValue2
*      comPort 端口号
*   按端口号返回端口读取到的数据
**/
uint16_t GetValue2(uint8_t comPort);

#endif
