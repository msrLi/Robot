
#include"Bsp_LCD2_4_SPI.h"
#include"Font.h"

/*reset A8
* DC A11
* 
*/
#define HARD 1
#define CS_LOW   GPIO_ResetBits(GPIOB, GPIO_Pin_12);//不选中	
#define CS_HIGH  GPIO_SetBits(GPIOB, GPIO_Pin_12);//不选中	
#define DC_LOW   GPIO_ResetBits(GPIOA, GPIO_Pin_11);//不选中	
#define DC_HIGH  GPIO_SetBits(GPIOA, GPIO_Pin_11);//不选中	

#define RESET_LOW   GPIO_ResetBits(GPIOA, GPIO_Pin_8);//不选中	
#define RESET_HIGH  GPIO_SetBits(GPIOA, GPIO_Pin_8);//不选中	

#define MISO_LOW    GPIO_ResetBits(GPIOB, GPIO_Pin_14);//不选中	
#define MISO_HIGH   GPIO_SetBits(GPIOB, GPIO_Pin_14);

#define MOSI_LOW   GPIO_ResetBits(GPIOB, GPIO_Pin_15);//不选中	
#define MOSI_HIGH  GPIO_SetBits(GPIOB, GPIO_Pin_15);

#define SCK_LOW    GPIO_ResetBits(GPIOB, GPIO_Pin_13);//不选中	
#define SCK_HIGH   GPIO_SetBits(GPIOB, GPIO_Pin_13);

#define LCD_W 321
#define LCD_H 254
uint16_t POINT_COLOR=0xFF33;
uint16_t BACK_COLOR=0x20FF;
//画点
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x,u16 y)
{
	Address_set(x,y,x,y);//设置光标位置 
	LCD_WR_DATA(POINT_COLOR); 	    
} 
//在指定位置显示一个字符

//num:要显示的字符:" "--->"~"
//mode:叠加方式(1)还是非叠加方式(0)
//在指定位置显示一个字符

//num:要显示的字符:" "--->"~"

//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode)
{
  u8 temp;
  u8 pos,t;
	u16 x0=x;
	u16 colortemp=POINT_COLOR;      
    if(x>LCD_W-16||y>LCD_H-16)return;	    
	//设置窗口		   
	num=num-' ';//得到偏移后的值
	Address_set(x,y,x+8-1,y+16-1);      //设置光标位置 
	if(!mode) //非叠加方式
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=asc2_1608[(u16)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_WR_DATA(POINT_COLOR);	
				temp>>=1; 
				x++;
		    }
			x=x0;
			y++;
		}	
	}else//叠加方式
	{
		for(pos=0;pos<16;pos++)
		{
		    temp=asc2_1608[(u16)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//画一个点     
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	    	   	 	  
}   

//显示字符串
//x,y:起点坐标  
//*p:字符串起始地址
//用16字体
void LCD_ShowString(u16 x,u16 y,const u8 *p)
{         
    while(*p!='\0')
    {       
        if(x>LCD_W-16){x=0;y+=16;}
        if(y>LCD_H-16){y=x=0;}
        LCD_ShowChar(x,y,*p,0);
        x+=8;
        p++;
    }  
}
/*
*
**/
static void DelayUs(uint16_t delTime)
{
	uint8_t i;
	while(delTime--)
	{
		for(i=0;i<6;i++)
		{
			i=i;
		}
	}
}
/*
*    延时函数 
**/
static void DelayMs(uint16_t deTime)
{
	uint8_t i,j;
	do{
		for(i=0;i<0xfE;i++)
		{
			for(j=0;j<10;j++)
			{
				j=j;
			}		
		}
	}while(deTime--);
}
/*
*  
**/
static void CSandDS_init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;

  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//TOUCH-CS:PB12
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
  	GPIO_SetBits(GPIOB, GPIO_Pin_12);//不选中	
	
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;//TOUCH-CS:PB12
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_8);//不选中	
}

/*
*
**/
static void _SPI_init()
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;
	
#if HARD 	
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);  //SCLK:PB13  MOSI:PB15  MISO:PB14
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB ,ENABLE); 
  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

  	SPI_I2S_DeInit(SPI2);
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;         //仅发送
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;									//8位数据模式
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;										//数据采样从第2个时间边沿开始
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;													//NSS软件管理
  	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//波特率
  	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;									//大端模式
  	SPI_InitStructure.SPI_CRCPolynomial = 7;														//CRC多项式
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;												//主机模式
  	SPI_Init(SPI2, &SPI_InitStructure);
  	SPI_Cmd(SPI2, ENABLE);
#else 
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 |GPIO_Pin_14 | GPIO_Pin_15;//TOUCH-CS:PB12
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_15);//不选中	
	  GPIO_SetBits(GPIOB, GPIO_Pin_13);//不选中	
	  GPIO_SetBits(GPIOB, GPIO_Pin_14);//不选中	
#endif 
}
/* 
*
**/
static uint8_t SPI2_RWByte(uint8_t byte)
{
#if HARD
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI2,byte);
  
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
	SPI_I2S_ReceiveData(SPI2);

#else
	uint8_t i;
	for(i=0;i<8;i++)
	{
		if(byte&(0x80>>i))
		{
			MOSI_HIGH;
		}else{
			MOSI_LOW;
		}
		SCK_LOW;
		DelayUs(2);

		SCK_HIGH;
	}
// 	asm("nop");
#endif
}
/*
*
**/
static void LCD_WR_DATA8(char da)
{

	DC_HIGH;
//	i=8;
//	while(i--);
	SPI2_RWByte(da);
}
/*
*
**/
static void LCD_WR_DATA(int da)
{

  DC_HIGH;
//	i=3;
//	while(i--);
	SPI2_RWByte(da>>8);
	SPI2_RWByte(da);
}	 
/*
*
**/
void LCD_WR_REG(char da)	 
{

  DC_LOW;
//	i=3;
//	while(i--);
	SPI2_RWByte(da);
}
static  void LCD_WR_REG_DATA(int reg,int da)
{
    LCD_WR_REG(reg);
	  LCD_WR_DATA(da);
}
/*
*
**/
static void InitLCDsReages(void)
{
	LCD_WR_REG(0xCB);  
    LCD_WR_DATA8(0x39); 
    LCD_WR_DATA8(0x2C); 
    LCD_WR_DATA8(0x00); 
    LCD_WR_DATA8(0x34); 
    LCD_WR_DATA8(0x02); 

    LCD_WR_REG(0xCF);  
    LCD_WR_DATA8(0x00); 
    LCD_WR_DATA8(0XC1); 
    LCD_WR_DATA8(0X30); 
 	
	LCD_WR_REG(0xE8);  
    LCD_WR_DATA8(0x85); 
    LCD_WR_DATA8(0x00); 
    LCD_WR_DATA8(0x78); 
 	
	LCD_WR_REG(0xEA);  
    LCD_WR_DATA8(0x00); 
    LCD_WR_DATA8(0x00); 
 	
	LCD_WR_REG(0xED);  
    LCD_WR_DATA8(0x64); 
    LCD_WR_DATA8(0x03); 
    LCD_WR_DATA8(0X12); 
    LCD_WR_DATA8(0X81); 

    LCD_WR_REG(0xF7);  
    LCD_WR_DATA8(0x20); 
   
    LCD_WR_REG(0xC0);    //Power control 
    LCD_WR_DATA8(0x23);   //VRH[5:0] 
   
    LCD_WR_REG(0xC1);    //Power control 
    LCD_WR_DATA8(0x10);   //SAP[2:0];BT[3:0] 
 
    LCD_WR_REG(0xC5);    //VCM control 
    LCD_WR_DATA8(0x3e); //对比度调节
    LCD_WR_DATA8(0x28); 
 
    LCD_WR_REG(0xC7);    //VCM control2 
    LCD_WR_DATA8(0x86);  //--
 
    LCD_WR_REG(0x36);    // Memory Access Control 
    LCD_WR_DATA8(0x28); //	   //48 68竖屏//28 E8 横屏

    LCD_WR_REG(0x3A);    
    LCD_WR_DATA8(0x55); 

    LCD_WR_REG(0xB1);    
    LCD_WR_DATA8(0x00);  
    LCD_WR_DATA8(0x18); 
 
    LCD_WR_REG(0xB6);    // Display Function Control 
    LCD_WR_DATA8(0x08); 
    LCD_WR_DATA8(0x82);
    LCD_WR_DATA8(0x27);  
 	
	LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
    LCD_WR_DATA8(0x00); 
 	
	LCD_WR_REG(0x26);    //Gamma curve selected 
    LCD_WR_DATA8(0x01); 
 
 	LCD_WR_REG(0xE0);    //Set Gamma 
    LCD_WR_DATA8(0x0F); 
    LCD_WR_DATA8(0x31); 
    LCD_WR_DATA8(0x2B); 
    LCD_WR_DATA8(0x0C); 
    LCD_WR_DATA8(0x0E); 
    LCD_WR_DATA8(0x08); 
    LCD_WR_DATA8(0x4E); 
    LCD_WR_DATA8(0xF1); 
    LCD_WR_DATA8(0x37); 
    LCD_WR_DATA8(0x07); 
    LCD_WR_DATA8(0x10); 
    LCD_WR_DATA8(0x03); 
    LCD_WR_DATA8(0x0E); 
    LCD_WR_DATA8(0x09); 
    LCD_WR_DATA8(0x00); 

    LCD_WR_REG(0XE1);    //Set Gamma 
    LCD_WR_DATA8(0x00); 
    LCD_WR_DATA8(0x0E); 
    LCD_WR_DATA8(0x14); 
    LCD_WR_DATA8(0x03); 
    LCD_WR_DATA8(0x11); 
    LCD_WR_DATA8(0x07); 
    LCD_WR_DATA8(0x31); 
    LCD_WR_DATA8(0xC1); 
    LCD_WR_DATA8(0x48); 
    LCD_WR_DATA8(0x08); 
    LCD_WR_DATA8(0x0F); 
    LCD_WR_DATA8(0x0C); 
    LCD_WR_DATA8(0x31); 
    LCD_WR_DATA8(0x36); 
    LCD_WR_DATA8(0x0F); 

    LCD_WR_REG(0x11);    //Exit Sleep 
    DelayMs(100); 
			
    LCD_WR_REG(0x29);    //Display on 
    LCD_WR_REG(0x2c); 	
}
/* 
*  name : _InitLCD 
* 			初始化LCD显示屏
* 
**/
void _InitLCD(void)
{
	CSandDS_init();
	_SPI_init();
	DelayMs(120);
	RESET_LOW;
	DelayMs(120);
	RESET_HIGH;
	DelayMs(230);
	CS_LOW;   // CS enable
	InitLCDsReages();
	
}
/*
*   定点函数 
**/
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{  
   LCD_WR_REG(0x2a);
   LCD_WR_DATA8(x1>>8);
   LCD_WR_DATA8(x1);
   LCD_WR_DATA8(x2>>8);
   LCD_WR_DATA8(x2);
  
   LCD_WR_REG(0x2b);
   LCD_WR_DATA8(y1>>8);
   LCD_WR_DATA8(y1);
   LCD_WR_DATA8(y2>>8);
   LCD_WR_DATA8(y2);

   LCD_WR_REG(0x2C);

}
void LCD_Clear(u16 Color)
{
	u8 VH,VL;
	u16 i,j;
	VH=Color>>8;
	VL=Color;	
	Address_set(0,0,LCD_W-1,LCD_H-1);
   for(i=0;i<LCD_W;i++)
	 {
	  for (j=0;j<LCD_H;j++)
	   	{
        	 LCD_WR_DATA8(VH);
			     LCD_WR_DATA8(VL);	

	    }

	  }
}

