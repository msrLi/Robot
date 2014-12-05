#include"Task_dateConnected.h"
#include"Bsp_Led.h"
#include"dataStory.h"
#define Base  2
#define SAVEDATATIME    360000
#define  time_out_dely  300
#define A_154Pin  GPIO_Pin_10
#define B_154Pin  GPIO_Pin_11
#define C_154Pin  GPIO_Pin_12
#define D_154Pin  GPIO_Pin_13
#define U1_A_405Pin  GPIO_Pin_0
#define U1_B_405Pin  GPIO_Pin_1
#define U1_C_405Pin  GPIO_Pin_2
#define U2_A_405Pin  GPIO_Pin_3
#define U2_B_405Pin  GPIO_Pin_4
#define U2_C_405Pin  GPIO_Pin_5
uint32_t cGg=4000;
OS_SEM  Usart1RecerveSem;       // 串口1  信号量 
OS_SEM * semTemp;
//OS_SEM  Usart3RecerveSem;       //  串口3 同步信号量
uint8_t rece1_buff[Max_date1];  // 串口数据缓冲 
uint16_t USART1_RX_STA=0;     //串口接收结束标志和 接收数量存放位置

OS_SEM  Usart2RecerveSem;
uint8_t  rece2_buff[Max_date2];
uint16_t USART2_RX_STA;

uint16_t ModeOkFlage=0;     // 外围设备 准备好数据 标志位 
/* 分类存储*/
void Deal_Recerve(uint8_t buff,uint8_t * adds)
{
	uint16_t buffs;
	comPortData[buff].DeviceID=adds[0];   // 设备ID
	comPortData[buff].Online=1;	
	buffs=0;
	if(comPortData[buff].DeviceID==0x08)
	{
		buffs+=adds[1];
		buffs+=(adds[2]<<8);
		comPortData[buff].dataPoint[0]=buffs;
//		buffs=GetValue2(15);
//		buffs=buffs;
	}
//	comPortData[buff].dataPoint[0]=adds[1];
//	comPortData[buff].dataPoint[1]=adds[2];	
}
/* 
*   选中输入的信号
*   sle  0--15
**/
void Slect_CS(uint8_t sle)
{
	uint8_t i;
	const BitAction BitVals[2]={Bit_RESET,Bit_SET};
	uint8_t buff;
	
	if(sle>=16) return ;
	buff=sle;
	i=buff/8;
	GPIO_WriteBit(GPIOC,D_154Pin,BitVals[i]);
	buff-=(i*8);
	i=buff/4;
	GPIO_WriteBit(GPIOC,C_154Pin,BitVals[i]);	
	buff-=(i*4);
	i=buff/2;
	GPIO_WriteBit(GPIOC,B_154Pin,BitVals[i]);		
	buff-=(i*2);
	i=buff;
	GPIO_WriteBit(GPIOC,A_154Pin,BitVals[i]);	
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);	
}
void OpenCharge(uint16_t data)
{
	uint16_t IO_data;
	IO_data=GPIOC->ODR;   // GPIOC IO Out Put Pin numbers 
	IO_data &= 0xFFC0;
	if(data<8)
	{
		IO_data |=data;

	}else{
		data-=8;
		IO_data |=(data<<3);
	}
	GPIOC->ODR = IO_data;
}
/* 
*   HC4054_Init
*   选通器初始化 
**/ 
void HC4054_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	
	
	GPIO_InitStructure.GPIO_Pin = U1_A_405Pin | U1_B_405Pin | U1_C_405Pin | \
	U2_A_405Pin | U2_B_405Pin | U2_C_405Pin ;  // Init 
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
}
/*
*    初始化 4--16 译码器
**/
void HC154_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	
	GPIO_InitStructure.GPIO_Pin = A_154Pin | B_154Pin | C_154Pin | D_154Pin ;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
}
/* 
*    设置输入读取 IO状态  初始化 
**/ 
void ExamFlagInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	
	GPIO_DeInit(GPIOE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
}

void AppTaskDateConnected(void *p_arg)
{
	OS_ERR     err;
//	uint8_t ubts;
	uint8_t i,j,count;
	uint8_t adds=0;
  uint8_t *rece;
	ExamFlagInit();       // 初始化tick时钟
	HC154_Init();
	HC4054_Init();
	ModeOkFlage=GPIO_ReadInputData(GPIOE);  		// 获取 状态标志位
	/* 创建 信号量 用与同步两个进程 */
	// 用于同步  cn=0;
	OSSemCreate((OS_SEM    *)&Usart2RecerveSem,
							(CPU_CHAR  *)"Usart2RecerveSem",
							(OS_SEM_CTR )0,
							(OS_ERR    *)&err);	

	OSSemCreate((OS_SEM    *)&Usart1RecerveSem,
							(CPU_CHAR  *)"Usart1RecerveSem",
							(OS_SEM_CTR )0,
							(OS_ERR    *)&err);		

  USART2_RX_STA=0;
  USART1_RX_STA=0;
	GPIO_SetBits(GPIOB,GPIO_Pin_0);

	cGg=90;
	while(1)
	{
		// cGg++;
//		ChangeAngle(1,cGg);
//		ChangeAngle(1,cGg);
//		ChangeAngle(2,cGg);
//		ChangeAngle(3,cGg);
//		ChangeAngle(4,cGg);
//		ChangeAngle(5,cGg);
//		ChangeAngle(6,cGg);
//		ChangeAngle(7,cGg);
//		ChangeAngle(8,cGg);
		// Usart1_sendChar(0xFA);
		ModeOkFlage=GPIO_ReadInputData(GPIOE);  		// 获取 状态标志位
		
		for(i=8;i<16;i++)
		{
			count=(uint8_t)(i/8);     // 将 16 分为 前8 和后 8 两个数据段
			count>=1? USART_ITConfig(USART1, USART_IT_RXNE, DISABLE):USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
			count>=1? USART_ITConfig(USART2, USART_IT_RXNE, ENABLE):USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); 
 //			count>=1? USART_ITConfig(USART1, USART_IT_RXNE, ENABLE):USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
 //			count>=1? USART_ITConfig(USART2, USART_IT_RXNE, ENABLE):USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
			OSSemSet(&Usart2RecerveSem,0,&err);
			OSSemSet(&Usart1RecerveSem,0,&err);
			if(ModeOkFlage & (0x01<<i))
			{
				OpenCharge(i);  //选通器打开
				// 状态有效 
//				if(i==0)
//				{
//				  Slect_CS(15);     // CS 输出有效 
//				}else 
//				{ 
//					Slect_CS(i-1);     // CS 输出有效 
//				}
				USART1_RX_STA=0;
				USART2_RX_STA=0;
				semTemp= count>=1? &Usart2RecerveSem:&Usart1RecerveSem;
				Slect_CS(i);     // CS 输出有效 
//				OSTimeDlyHMSM((CPU_INT16U) 0u,
//											(CPU_INT16U) 0u,
//											(CPU_INT16U) 0,
//											(CPU_INT32U) 1,
//											(OS_OPT    ) OS_OPT_TIME_HMSM_STRICT,
//											(OS_ERR   *)&err);					

				/* 等待 收到 串口 数据 */
			 //  count>=1? Usart2_sendChar(0xA1):Usart1_sendChar(0xA8);
				
				
				OSSemPend((OS_SEM *) semTemp,
								(OS_TICK )50,
								(OS_OPT  )OS_OPT_PEND_BLOCKING,
								(CPU_TS  )NULL,
								(OS_ERR *)&err); 	
        GPIO_SetBits(GPIOB,GPIO_Pin_0);
				rece= count>=1? rece2_buff:rece1_buff;
				if(err==0)       //成功返回
				{
					
					adds=rece[0];
					for(j=1;j<5;j++)
					{
						adds+=rece[j];
						// rece[j]=0;						
					}
					if(adds==rece[j])   // 收取正确
					{
						Trage_Led();
						Deal_Recerve(i,rece);
//						comPortData[i].DeviceID=rece[0];   // 设备ID
//						comPortData[i].Online=1;
//						comPortData[i].dataPoint[0]=rece[1];
//						comPortData[i].dataPoint[1]=rece[2];
					}else{
						USART1_RX_STA=0;
						USART2_RX_STA=0;
						continue;
					}
						USART1_RX_STA=0;
					  USART2_RX_STA=0;
				}
				if(err==OS_ERR_TIMEOUT)   // 超时 进行下一个循环 
				{
						continue;   
				}
				if(err==OS_ERR_OBJ_TYPE)
				{
					OSSemCreate((OS_SEM    *) semTemp,
											(CPU_CHAR  *)"UsartRecerveSem",
											(OS_SEM_CTR )0,
											(OS_ERR    *)&err);			
				}	
				OSTimeDlyHMSM((CPU_INT16U) 0u,
											(CPU_INT16U) 0u,
											(CPU_INT16U) 0,
											(CPU_INT32U) 5,
											(OS_OPT    ) OS_OPT_TIME_HMSM_STRICT,
											(OS_ERR   *)&err);					
			}

		}
		 // GPIO_SetBits(GPIOB,GPIO_Pin_0);    // 不选中 4--16 译码器 
		/*  查询频率 设置 */
		OSTimeDlyHMSM((CPU_INT16U) 0u,
                  (CPU_INT16U) 0u,
                  (CPU_INT16U) 0,
                  (CPU_INT32U) 50,
                  (OS_OPT    ) OS_OPT_TIME_HMSM_STRICT,
                  (OS_ERR   *)&err);	
	 }	  
}

