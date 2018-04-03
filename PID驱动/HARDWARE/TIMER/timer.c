#include "timer.h"
#include "pwm.h"
#include "sys.h"	
#include "lcd.h"
#include "pid.h"
#include <string.h>
#include "control.h"
#include "adc.h"
#include "math.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


extern u16 chart[200];
extern float pwm;
extern u8 nowpwm,POWER;
extern float setValue;
extern u8 choose,Update;
extern float SetFreq;	
extern u16 delay,delay_left;
extern float SetVol,integral;
//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
void TIM3_Int_Init(u16 period)//TIM3 定时器主频42MHz
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = period-1; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=0;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0; 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

u16 adc0,adc1,adc2,adc3,POWERTimer;
float volt1,volt2,amp1,amp2;
extern HEDIT *edit1,*edit2,*edit3,*edit4,*edit5,*edit6;
u8 nowEXTI2,lastEXTI2,nowEXTI3,lastEXTI3;
static u16 update_time=0;
double Voltage,VoltageIn,Current;
//定时器3中断服务函数 10kHz
void TIM3_IRQHandler(void)
{
	u16 adc;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		if (POWER) {
			adc=Get_Adc_Average(ADC1,ADC_Channel_1,30);
			Voltage=(adc/4096.0*3.269)*3.3386+28.314;
			if (choose==1) PID(Voltage,SetVol,&pwm); //CloseLoop
		
			adc=Get_Adc_Average(ADC1,ADC_Channel_0,30);
			VoltageIn=(adc/4096.0*3.269)*3.3806+16.054;
	
			adc=Get_Adc_Average(ADC1,ADC_Channel_3,30);
			Current=(adc/4096.0*3.269)*1.7657-2.2301;
			if (Current>1.539) {  
				POWERTimer=0;
				POWER=0;
				pwm=0;
				//过流保护
			}
		} else {
			POWERTimer++;
			Voltage=0; VoltageIn=0; Current=0;
			if (POWERTimer>10000) {
				POWER=1; integral=0;
			}
		}
		
		update_time++;
		if (update_time>2000) {	//Update Frequency 5Hz
			update_time=0;
			strcpy(edit1->text,RealToStr(Voltage));
			strcpy(edit2->text,RealToStr(Current));
			strcpy(edit3->text,RealToStr(VoltageIn));
			strcpy(edit4->text,RealToStr(Round(pwm*100.0))); 
			Update=1;
		}
		TIM_SetCompare1(TIM1,Round(1680*pwm));
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}
