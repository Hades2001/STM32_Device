/**************************************************/
#include "Hardware_System.h"

/*************Statement system Function************/

void HardWare_ALL_Init(void);


/**************Statement User Function*************/


unsigned char SystemID[12]={0,0,0,0,0,0,0,0,0,0,0,0};

/****************Function Code*********************/
int USART1_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//??????APB2????

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
	GPIO_Init(GPIOA, &GPIO_InitStructure);			/* TXIO */			
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 			/* RXIO */	

	USART_InitStructure.USART_BaudRate = 115200;//???
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8???
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//??????1????
	USART_InitStructure.USART_Parity = USART_Parity_No;//???
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//????????
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//???????
	USART_Init(USART1,&USART_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;       //低优先级别的中断
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;							//响应中断等级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);  
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1, ENABLE);//??????USART??

	return 0;

}INIT_CPU_EXPORT(USART1_Init);
 
int USART2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//??????APB1????

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
	GPIO_Init(GPIOA, &GPIO_InitStructure);			/* TXIO */			
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;			 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 			/* RXIO */	

	USART_InitStructure.USART_BaudRate = 115200;//???
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8???
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//??????1????
	USART_InitStructure.USART_Parity = USART_Parity_No;//???
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//????????
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//???????
	USART_Init(USART2,&USART_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;       //低优先级别的中断
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//响应中断等级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);   		
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART2, ENABLE);//??????USART??

	return 0;
}INIT_CPU_EXPORT(USART2_Init);

int USART3_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//??????APB1????
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
	GPIO_Init(GPIOB, &GPIO_InitStructure);			/* TXIO */			
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;			 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 			/* RXIO */	

	USART_InitStructure.USART_BaudRate = 19200;//???
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;//8???
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//??????1????
	USART_InitStructure.USART_Parity = USART_Parity_Even;//偶校验
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//????????
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//???????
	USART_Init(USART3,&USART_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;       
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;							
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);  		
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART3, ENABLE);//??????USART??

	return 0;

}INIT_CPU_EXPORT(USART3_Init);

int InitLED_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOA,GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15);
	return 0;

}INIT_CPU_EXPORT(InitLED_GPIO);

int InitLCDGPIO( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_SetBits(GPIOC , GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 );

	return 0;
}INIT_CPU_EXPORT(InitLCDGPIO);

int SPI2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);//使能或者失能APB1外设时钟

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//内部NSS信号由SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;//预分频值为16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;//用于CRC值计算的多项式
	SPI_Init(SPI2,&SPI_InitStructure);//初始化配置

	SPI_Cmd(SPI2,ENABLE);//使能或者失能SPI外设

	return 0;
}INIT_CPU_EXPORT(SPI2_Init);

int InitBellGPIO(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	
	return 0;

}//INIT_CPU_EXPORT(InitBellGPIO);

int SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
  	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	
	SPI_InitStructure.SPI_CRCPolynomial = 7;	
	SPI_Init(SPI1, &SPI_InitStructure);  
 
	SPI_Cmd(SPI1, ENABLE); 

	return 0;
}   
INIT_CPU_EXPORT(SPI1_Init);

int InitSC6803GPIO( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 ; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	return 0;
}
INIT_CPU_EXPORT(InitSC6803GPIO);

int InitKeyGPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_6 | \
									GPIO_Pin_7 | \
									GPIO_Pin_8 | \
									GPIO_Pin_9 | \
									GPIO_Pin_10 | \
									GPIO_Pin_11 | \
									GPIO_Pin_12 ;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速率50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化外设GPIOx寄存器

	return 0;
}
INIT_CPU_EXPORT(InitKeyGPIO);

int Time1_Init(void)
{
	return 0;
}
int Time2_Init(void)
{
	return 0;
}

int Time3_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//开启定时器外设时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	//配置定时器参数
	TIM_DeInit(TIM3); 
	TIM_TimeBaseStructure.TIM_Period = 1000; 								 	//10ms定时			 
	TIM_TimeBaseStructure.TIM_Prescaler = (72000000/1000000 - 1);              
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	
	//中断配置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //抢占优先级1 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  	 //响应优先级0 高级别的响应中断
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);	  
	//开中断
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);					  
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); 
	//开启定时器			 
	TIM_Cmd(TIM3, ENABLE); 
	//extern void Time3_IntHandle(void);
	return 0;
}
INIT_CPU_EXPORT(Time3_Init);


int HardWare_RCC_Init(void)
{
		//--------------------------- CLK INIT, HSE PLL ----------------------------
		ErrorStatus HSEStartUpStatus;
		//RCC reset
		RCC_DeInit();
		//开启外部时钟 并执行初始化
		RCC_HSEConfig(RCC_HSE_ON); 
		//等待外部时钟准备好
		HSEStartUpStatus = RCC_WaitForHSEStartUp();
		//启动失败 在这里等待
		while(HSEStartUpStatus == ERROR);
		//设置内部总线时钟
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div1);
		RCC_PCLK2Config(RCC_HCLK_Div1);
		//外部时钟为8M 这里倍频到72M
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);
		RCC_PLLCmd(ENABLE); 
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		while(RCC_GetSYSCLKSource() != 0x08);
		//----------------------------- CLOSE HSI ---------------------------
		//关闭内部时钟HSI
		RCC_HSICmd(DISABLE);	
		//--------------------------- OPEN GPIO CLK -------------------------
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//使能或者失能APB2外设时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);//使能或者失能APB2外设时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);//使能或者失能APB2外设时钟

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

		return 0;
}
INIT_BOARD_EXPORT(HardWare_RCC_Init);

int SerialNum(void)
{
  u32 Device_Serial0, Device_Serial1, Device_Serial2;	  
  Device_Serial0 = *(vu32*)(0x1FFFF7E8); 
  Device_Serial1 = *(vu32*)(0x1FFFF7EC); 
  Device_Serial2 = *(vu32*)(0x1FFFF7F0); 
 
	SystemID[0]=Device_Serial0;
	Device_Serial0>>=8;
	SystemID[1]=Device_Serial0;
	Device_Serial0>>=8;
	SystemID[2]=Device_Serial0;
	Device_Serial0>>=8;
	SystemID[3]=Device_Serial0;
	
	SystemID[4]=Device_Serial1;
	Device_Serial1>>=8;
	SystemID[5]=Device_Serial1;
	Device_Serial1>>=8;
	SystemID[6]=Device_Serial1;
	Device_Serial1>>=8;
	SystemID[7]=Device_Serial1;

	
	SystemID[8]=Device_Serial2;
	Device_Serial2>>=8;
	SystemID[9]=Device_Serial2;
	Device_Serial2>>=8;
	SystemID[10]=Device_Serial2;
	Device_Serial2>>=8;
	SystemID[11]=Device_Serial2;

	return 0;
}INIT_CPU_EXPORT(SerialNum);

