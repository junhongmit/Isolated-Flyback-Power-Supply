#include "led.h"
#include "control.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "24cxx.h"
#include "touch.h"
#include "pwm.h"
#include "adc.h"
#include "pid.h"
#include "exti.h"
#include "timer.h"
#include <string.h>
#include <math.h>
const double pi=3.1415926;
u8 *name1[15]={
"7","8","9","Volt","Duty",
"4","5","6","Del","Enter",
"1","2","3","0",".",
};
// ADC1转换的电压值通过DMA方式传到flash
extern __IO u16 ADC_ConvertedValue;

// 局部变量，用于存从flash读到的电压值			 
__IO u16 ADC_ConvertedValueLocal; 
HBUTTON *but[3][5],*button1,*button2,*button3;
HTRACK *Dutytrack;
HEDIT *edit1,*edit2,*edit3,*edit4,*edit5,*edit6;
u8 choose,choosedis,Update=0;
u16 chart[200];
u8 nowpwm,POWER;
float pwm=0.5,setValue=1.0,SetVol;
u16 delay,delay_left;
float Period;
//ALIENTEK 探索者STM32F407开发板 实验13
//LCD显示实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK
void GetSin()
{
	u8 i;
	double f;
	for (i=0; i<200; i++) {
		chart[i]=Round(sin(2*pi*(i+1)/200.0)*840+840);
	}
}
void Load_Draw_Dialog(void)
{
	int i,j;
	LCD_Clear(WHITE);//清屏   
 	POINT_COLOR=BLACK;//设置字体为黑色 
	//LCD_ShowString(40,5,64,16,16,"Duty:");
	//LCD_ShowBigNum(80,0,(double)500/1000*100);
	//LCD_ShowString(200,8,8,16,16,"%");
	//CreateButton(0,0,120,160,"123",NULL,0);
	//dutytrack=CreateTrackBar(20,40,200,30,6000,201);
	//SetTrackPos(dutytrack,3000);
	//CreateButton(0,40,20,30,"-",0,000);
	//CreateButton(220,40,20,30,"+",0,001);
	//periodtrack=CreateTrackBar(20,100,200,30,65535,202);
	//SetTrackPos(periodtrack,6000);
	//CreateButton(0,100,20,30,"-",0,002);
	//CreateButton(220,100,20,30,"+",0,003);
	POINT_COLOR=BLACK;
	//LCD_ShowString(38,80,100,20,16,"Period:");
	//LCD_ShowBigNum(78,50+16+4,6000);
	edit1=CreateEdit(10,20,110,30,"0");//V
	edit2=CreateEdit(10,55,110,30,"0");//A
	edit3=CreateEdit(10,105,110,30,"0");//V
	//edit4=CreateEdit(10,140,110,30,"0");//A
	edit4=CreateEdit(138,105,85,30,"0");//%
	Dutytrack=CreateTrackBar(20,140,200,30,80,201);
	//LCD_ShowString(38,125,100,20,16,"Freq:");
	//LCD_ShowBigNum(78,125,50);
	//SetTrackPos(Freqtrack,30);
	edit5=CreateEdit(10,175,220,40,"30");
	LCD_ShowString(5,2,60,16,16,"Output:");
	LCD_ShowString(125,26,8,16,16,"V");
	LCD_ShowString(125,61,8,16,16,"A");
	LCD_ShowString(5,87,60,16,16,"Input:");
	LCD_ShowString(135,87,60,16,16,"Duty:");
	LCD_ShowString(227,113,8,16,16,"%");
	LCD_ShowString(125,115,8,16,16,"V");
	
	button1=CreateButton(140,5,90,35,"Open Loop",0,000);
	button2=CreateButton(140,50,90,35,"Close Loop",0,001);
	for (i=0; i<3; i++)
		for (j=0; j<6; j++) {
				//LCD_Color_Fill(10+38*j,90+26*i,30,20,gImage_button3);
				//LCD_ShowString(10+38*j+15-strlen(name2[i*6+j])*3,90+26*i+10-6,30,20,12,name2[i*6+j]);
		}
	for (i=0; i<3; i++)
		for (j=0; j<5; j++) {
				//LCD_Color_Fill(9+46*j,172+36*i,40,30,gImage_button1);
				//LCD_ShowString(9+46*j+20-strlen(name1[i*5+j])*3,172+36*i+15-6,40,30,12,name1[i*5+j]);
			but[i][j]=CreateButton(9+46*j,223+31*i,40,25,(char *)name1[i*5+j],0,100+i*5+j);
		}
	SetButtonState(button1,1);
	SetButtonState(but[0][3],1);
	choose=0; choosedis=0;
  	POINT_COLOR=RED;//设置画笔蓝色 
}
////////////////////////////////////////////////////////////////////////////////
//电容触摸屏专有部分
//画水平线
//x0,y0:坐标
//len:线长度
//color:颜色
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color)
{
	if(len==0)return;
	LCD_Fill(x0,y0,x0+len-1,y0,color);	
}
//画实心圆
//x0,y0:坐标
//r:半径
//color:颜色
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color)
{											  
	u32 i;
	u32 imax = ((u32)r*707)/1000+1;
	u32 sqmax = (u32)r*(u32)r+(u32)r/2;
	u32 x=r;
	gui_draw_hline(x0-r,y0,2*r,color);
	for (i=1;i<=imax;i++) 
	{
		if ((i*i+x*x)>sqmax)// draw lines from outside  
		{
 			if (x>imax) 
			{
				gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
				gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
			}
			x--;
		}
		// draw lines from inside (center)  
		gui_draw_hline(x0-x,y0+i,2*x,color);
		gui_draw_hline(x0-x,y0-i,2*x,color);
	}
}  
//两个数之差的绝对值 
//x1,x2：需取差值的两个数
//返回值：|x1-x2|
u16 my_abs(u16 x1,u16 x2)
{			 
	if(x1>x2)return x1-x2;
	else return x2-x1;
}  
//画一条粗线
//(x1,y1),(x2,y2):线条的起始坐标
//size：线条的粗细程度
//color：线条的颜色
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	if(x1<size|| x2<size||y1<size|| y2<size)return; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		gui_fill_circle(uRow,uCol,size,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}   
////////////////////////////////////////////////////////////////////////////////
 //5个触控点的颜色												 
const u16 POINT_COLOR_TBL[CT_MAX_TOUCH]={RED,GREEN,BLUE,BROWN,GRED};  
void Check_Touch(void)
{ 
	u8 key;
	u8 i,j,choosex,choosey;
	static u8 down=0;
	char c;
	char *temp;
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{	
		 	if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			{	
				//if(tp_dev.x[0]>(lcddev.width-24)&&tp_dev.y[0]<16)Load_Draw_Dialog();//清除
				//else TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],RED);		//画图	  
					/*if (flag) continue;
					for (i=0; i<4; i++)
						for (j=0; j<5; j++)
							if (tp_dev.x[0]>=9+46*j&&tp_dev.x[0]<=9+46*j+40&&tp_dev.y[0]>=172+36*i&&tp_dev.y[0]<=172+36*i+30) {
								choosey=i; choosex=j; flag=1;
								//LCD_Color_Fill(9+46*j,172+36*i,40,30,gImage_button2);
								POINT_COLOR=RED;
								LCD_ShowString(9+46*j+20-strlen(name1[i*5+j])*3,172+36*i+15-6,40,30,12,name1[i*5+j]);
							}
					for (i=0; i<3; i++)
						for (j=0; j<6; j++)
							if (tp_dev.x[0]>=10+38*j&&tp_dev.x[0]<=10+38*j+30&&tp_dev.y[0]>=90+26*i&&tp_dev.y[0]<=90+26*i+20) {
								choosey=i; choosex=j; flag=2;
								//LCD_Color_Fill(10+38*j,90+26*i,30,20,gImage_button4);
								POINT_COLOR=RED;
								LCD_ShowString(10+38*j+15-strlen(name2[i*6+j])*3,90+26*i+10-6,30,20,12,name2[i*6+j]);
							}*/
				//TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],RED);	
				if (!down) {
					SendMessage(0,WM_TOUCHDOWN,tp_dev.x[0],tp_dev.y[0]);
					down=1;
				} else {
					SendMessage(0,WM_TOUCHMOVE,tp_dev.x[0],tp_dev.y[0]);
				}
			}
		}else {
			if (down) {
				SendMessage(0,WM_TOUCHUP,tp_dev.x[0],tp_dev.y[0]);
				down=0;
			}
		}
}

 int main(void)
 {	 
	delay_init(168);	    	 //延时函数初始化	  
	//NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	uart_init(9600);	 	//串口初始化为9600
 	LED_Init();			     //LED端口初始化
	POWER=1;
	LCD_Init();		
	EXTI_GPIO_Init();
	//EXTIX_Init();
	//TP_PreAdjust();
 	tp_dev.init();
	Adc_Init();
	GetSin();
 	POINT_COLOR=RED;//设置字体为红色 
	/*LCD_ShowString(60,50,200,16,16,"WarShip STM32");	
	LCD_ShowString(60,70,200,16,16,"TOUCH TEST");	
	LCD_ShowString(60,90,200,16,16,"mucdev.taobao");
	LCD_ShowString(60,110,200,16,16,"2014/9/11");
   	LCD_ShowString(60,130,200,16,16,"Press KEY0 to Adjust");	
   	if(tp_dev.touchtype!=0XFF)LCD_ShowString(60,130,200,16,16,"Press KEY0 to Adjust");//电阻屏才显示*/
	Load_Draw_Dialog();
	 
	 
	delay=1; delay_left=10000;
	TIM3_Int_Init(4200);
	TIM1_PWM_Init();
	SetVol=30;
	//LCD_Color_Fill(100,100,40,gImage_button1);
	//LCD_Color_Fill(150,100,40,gImage_button2);
	 
	//rtp_test(); 						//电阻屏测试
	while (1) {
		//PID((float)ADC_ConvertedValueLocal/4096*3.3,1.5,&dutyCycle); //开窗到30.5V，目标在1.5V
		//TIM_SetCompare1(TIM1,(u16)dutyCycle*720);
		//TIM_SetCompare1(TIM1,420);
		Check_Touch();
		if (Update) {
			Update=0;
			UpdateWindow(edit1);
			UpdateWindow(edit2);
			UpdateWindow(edit3);
			UpdateWindow(edit4);
		}
	}
}
