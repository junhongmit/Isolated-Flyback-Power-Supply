#include "exti.h"
#include "delay.h" 
#include "led.h" 
#include "adc.h"
#include "control.h"
#include "lcd.h"
#include "pid.h"
#include <string.h>
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//外部中断 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
extern u8 choose,Update,SetFreq;	
extern HEDIT *edit1,*edit2,*edit3,*edit4,*edit5,*edit6;
extern float pwm;
extern float SetVol;
void EXTI_GPIO_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//??GPIOA,GPIOE??
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //KEY0 KEY1 KEY2????
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//??????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//??
	GPIO_Init(GPIOE, &GPIO_InitStructure);//???GPIOE4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //KEY0 KEY1 KEY2????
	GPIO_Init(GPIOE, &GPIO_InitStructure);//???GPIOE4
}
void EXTI3_IRQHandler(void)//过零中断，检测电流，并清零
{
	u16 adc;
	double Current;
	adc=Get_Adc_Average(ADC1,ADC_Channel_2,30);
	Current=(adc/4096.0*2.5)/0.5;
	strcpy((char*)edit2->text,(char*)RealToStr(Current));
	Update=1;
	EXTI_ClearITPendingBit(EXTI_Line3);//??LINE2??????? 
}
void EXTI2_IRQHandler(void)//过零中断，检测电压，并清零
{
	/*
	adc=Get_Adc_Average(ADC1,ADC_Channel_2,30);
	Voltage=(adc/4096.0*2.5)*30;
	if (choose==1) PID(Voltage,36,&pwm);
	update_time++;-
	if (update_time>SetFreq/2) {
		update_time=0;
		strcpy((char*)edit2->text,(char*)RealToStr(Voltage));
		Update=1;
	}*/
	LED0=!LED0;
	EXTI_ClearITPendingBit(EXTI_Line2);  //??LINE3???????  
}
	   
//外部中断初始化程序
//初始化PE2~4,PA0为中断输入.
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	EXTI_GPIO_Init(); //按键对应的IO口初始化
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
	
	
	/* 配置EXTI_Line2,3,4 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
	EXTI_Init(&EXTI_InitStructure);//配置
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;//中断线使能
	EXTI_Init(&EXTI_InitStructure);//配置
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//外部中断2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
	   
}












