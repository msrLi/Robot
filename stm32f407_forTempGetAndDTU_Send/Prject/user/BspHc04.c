
#include"BspHc04.h"
void initHCf4051(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  // opne 
	// GPIO_InitStructure.GPIO_Pin = INH_PIN ;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	// GPIO_Init(INH_PORT, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = INH_PIN ;
//	GPIO_Init(INH_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = A1_PIN ;
	GPIO_Init(A1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = B1_PIN ;
	GPIO_Init(B1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = C1_PIN ;
	GPIO_Init(C1_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = A2_PIN ;
	GPIO_Init(A2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = B2_PIN ;
	GPIO_Init(B2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = C2_PIN ;
	GPIO_Init(C2_PORT, &GPIO_InitStructure);
	
	GPIO_SetBits(A1_PORT,A1_PIN);	
	GPIO_SetBits(B1_PORT,B1_PIN);	
	GPIO_SetBits(C1_PORT,C1_PIN);
	GPIO_SetBits(A2_PORT,A2_PIN);	
	GPIO_SetBits(B2_PORT,B2_PIN);	
	GPIO_SetBits(C2_PORT,C2_PIN);		
}

void selectPoint(uint8_t port)
{
	uint8_t i,j,port_v;
	const BitAction BitVals[2]={Bit_RESET,Bit_SET};
	// GPIO_ResetBits(INH_PORT,INH_PIN);	
 if(port<8)
 {
		port_v=port;
		i=(uint8_t) port_v/4;        //写高位 
		port_v=port_v-i*4;
		GPIO_WriteBit(C1_PORT,C1_PIN,BitVals[i]);
		i=(uint8_t) port_v/2;
		port_v=port_v-i*2;	
		GPIO_WriteBit(B1_PORT,B1_PIN,BitVals[i]);
		i=port_v;
		GPIO_WriteBit(A1_PORT,A1_PIN,BitVals[i]);
 }else if(port<16)
 {
	 port_v=port-8;
		port_v=port;
		i=(uint8_t) port_v/4;        //写高位 
		port_v=port_v-i*4;
		GPIO_WriteBit(C2_PORT,C2_PIN,BitVals[i]);
		i=(uint8_t) port_v/2;
		port_v=port_v-i*2;	
		GPIO_WriteBit(B2_PORT,B2_PIN,BitVals[i]);
		i=port_v;
		GPIO_WriteBit(A2_PORT,A2_PIN,BitVals[i]);
 }

}
