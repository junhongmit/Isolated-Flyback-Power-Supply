/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名  ：adc.c
 * 描述    ：DMA方式读取ADC值应用函数库         
 * 实验平台：野火STM32开发板
 * 硬件连接：----------------------------------------------------
 *          |                                                    |
 *          | PC1 - ADC1 连接外部电压(通过一个滑动变阻器分压而来)|
 *          |                                                    |
 *           ----------------------------------------------------
 * 库版本  ：ST3.0.0
 *
 * 作者    ：fire  QQ: 313303034 
 * 博客    ：firestm32.blog.chinaunix.net
**********************************************************************************/
#include "adc.h"
#include "delay.h"

#define ADC1_DR_Address    ((u32)0x4001244C)

/*
 * 函数名：ADC1_Init
 * 描述  ：无
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void  Adc_Init(void)
{    
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//??GPIOA??
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //??ADC1??

	//????ADC1??5 IO?
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PA0 ??5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//????
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//?????
	GPIO_Init(GPIOA, &GPIO_InitStructure);//???  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1??
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//????	 
 
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//????
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//???????????5???
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA??
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//???4???ADCCLK=PCLK2/4=84/4=21Mhz,ADC????????36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);//???
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12???
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//?????	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//??????
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//??????,??????
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//???	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1????????? ??????????1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC???
	ADC_Init(ADC2, &ADC_InitStructure);
		
 
	ADC_Cmd(ADC1, ENABLE);//??AD???	
	ADC_Cmd(ADC2, ENABLE);
}	

u16 Get_Adc(ADC_TypeDef * ADCx,u8 ch)   
{
	  	//????ADC??????,????,????
	ADC_RegularChannelConfig(ADCx, ch, 1, ADC_SampleTime_3Cycles );	//ADC1,ADC??,480???,?????????????			    
  
	ADC_SoftwareStartConv(ADCx);		//?????ADC1?????????	
	 
	while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC ));//??????

	return ADC_GetConversionValue(ADCx);	//??????ADC1????????
}
//????ch????,?times?,???? 
//ch:????
//times:????
//???:??ch?times????????
u16 Get_Adc_Average(ADC_TypeDef * ADCx,u8 ch,u8 times)
{
	u32 sum=0;
	u8 t;
	u16 min,max,temp;
	max=0; min=0xFFFF;
	for(t=0;t<times;t++)
	{
		temp=Get_Adc(ADCx,ch);
		if (temp<min) min=temp;
		if (temp>max) max=temp;
		sum+=temp;
	}
	if (times>=3) {
		sum-=min+max;
		return sum/(times-2);
	} else return sum/times;
} 
	 


/******************* (C) COPYRIGHT 2011 野火嵌入式开发工作室 *****END OF FILE****/

