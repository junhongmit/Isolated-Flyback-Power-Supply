#include "control.h"
#include "lcd.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pwm.h"

extern HTRACK *Dutytrack;
extern float pwm;
extern u8 nowpwm;
extern u8 choose,choosedis;
extern HBUTTON *but[3][5],*button1,*button2,*button3;
extern HEDIT *edit1,*edit2,*edit3,*edit4,*edit5,*edit6;
extern double setValue;
extern double integral;
extern float SetVol;
extern u16 delay,delay_left;
const unsigned char gImage_window[2528] = {  0X10,0X10,0X00,0X1E,0X00,0X2A,0X01,0X1B, 
0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,
0XF7,0X9E,0XF7,0X9E,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,
0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,
0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,0XF7,0X9E,0XF7,0X9E,
0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,
0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,
0XE7,0XBF,0XE7,0XBF,0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,
0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,
0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XE7,0X9F,0XE7,0X9F,
0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,
0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,
0XE7,0XBF,0XE7,0XBF,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,
0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,
0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0XBF,0XE7,0XBF,
0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,
0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,
0XF7,0X9E,0XF7,0X9E,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,
0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,
0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XF7,0X9E,0XF7,0X9E,
0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,
0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,
0XDF,0X9F,0XDF,0X9F,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,
0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XE7,0XBF,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,
0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XDF,0X7F,0XDF,0X7F,
0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,
0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,
0XE7,0X9F,0XE7,0X9F,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,
0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,
0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,0XDF,0X7F,0XE7,0X9F,0XE7,0X9F,
0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,0XE7,0X9F,
0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,
0XEF,0X7D,0XEF,0X7D,0XD7,0X7F,0XD7,0X7F,0XD7,0X7F,0XD7,0X7F,0XD7,0X7F,0XD7,0X7F,
0XD7,0X7F,0XD7,0X7F,0XD7,0X7F,0XD7,0X7F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,
0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XEF,0X7D,0XEF,0X7D,
0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,
0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,
0XCF,0X5F,0XCF,0X5F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,
0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,
0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XCF,0X5F,0XCF,0X5F,
0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,0XCF,0X5F,
0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,
0XDF,0X9F,0XDF,0X9F,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,
0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XEF,0X7D,0XCF,0X3E,0XCF,0X3E,0XCF,0X3E,0XCF,0X3E,
0XCF,0X3E,0XCF,0X3E,0XCF,0X3E,0XCF,0X3E,0XCF,0X3E,0XCF,0X3E,0XDF,0X9F,0XDF,0X9F,
0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,
0XEF,0X5D,0XEF,0X5D,0XEF,0X5D,0XEF,0X5D,0XEF,0X5D,0XEF,0X5D,0XEF,0X5D,0XEF,0X5D,
0XEF,0X5D,0XEF,0X5D,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,
0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,
0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDF,0X9F,0XDE,0XFB,0XDE,0XFB,
0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,
0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,
0XC7,0X3E,0XC7,0X3E,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,
0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,
0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,0XDE,0XFB,0XC7,0X3E,0XC7,0X3E,
0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,0XC7,0X3E,
0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,
0XBF,0X3F,0XBF,0X3F,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,
0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0X9E,0X9D,0X9E,0X9D,0X9E,0X9D,0X9E,0X9D,
0X9E,0X9D,0X9E,0X9D,0X9E,0X9D,0X9E,0X9D,0X9E,0X9D,0X9E,0X9D,0XBF,0X3F,0XBF,0X3F,
0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,0XBF,0X3F,
0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,
0XDE,0XDB,0XDE,0XDB,0X96,0X7D,0X96,0X7D,0X96,0X7D,0X96,0X7D,0X96,0X7D,0X96,0X7D,
0X96,0X7D,0X96,0X7D,0X96,0X7D,0X96,0X7D,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,
0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XDE,0XDB,0XDE,0XDB,
0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,
0X96,0X7D,0X96,0X7D,0X96,0X7D,0X96,0X7D,0X96,0X7D,0X96,0X7D,0X96,0X7D,0X96,0X7D,
0X96,0X7D,0X96,0X7D,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,
0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,
0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0X8E,0X5D,0X8E,0X5D,
0X8E,0X5D,0X8E,0X5D,0X8E,0X5D,0X8E,0X5D,0X8E,0X5D,0X8E,0X5D,0X8E,0X5D,0X8E,0X5D,
0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,0XBF,0X1F,
0XBF,0X1F,0XBF,0X1F,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,
0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0XDE,0XDB,0X86,0X3D,0X86,0X3D,0X86,0X3D,0X86,0X3D,
0X86,0X3D,0X86,0X3D,0X86,0X3D,0X86,0X3D,0X86,0X3D,0X86,0X3D,0XB7,0X1F,0XB7,0X1F,
0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,
0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,
0XD6,0XBA,0XD6,0XBA,0X86,0X3D,0X86,0X3D,0X86,0X3D,0X86,0X3D,0X86,0X3D,0X86,0X3D,
0X86,0X3D,0X86,0X3D,0X86,0X3D,0X86,0X3D,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,
0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XD6,0XBA,0XD6,0XBA,
0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,
0X7E,0X1C,0X7E,0X1C,0X7E,0X1C,0X7E,0X1C,0X7E,0X1C,0X7E,0X1C,0X7E,0X1C,0X7E,0X1C,
0X7E,0X1C,0X7E,0X1C,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,
0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XB7,0X1F,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,
0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0X7D,0XFC,0X7D,0XFC,
0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,
0XAE,0XFF,0XAE,0XFF,0XAE,0XFF,0XAE,0XFF,0XAE,0XFF,0XAE,0XFF,0XAE,0XFF,0XAE,0XFF,
0XAE,0XFF,0XAE,0XFF,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,
0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,
0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0XAE,0XFE,0XAE,0XFE,
0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,
0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,
0XD6,0X9A,0XD6,0X9A,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,
0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0X7D,0XFC,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,
0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XD6,0X9A,0XD6,0X9A,
0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,
0X75,0XDB,0X75,0XDB,0X75,0XDB,0X75,0XDB,0X75,0XDB,0X75,0XDB,0X75,0XDB,0X75,0XDB,
0X75,0XDB,0X75,0XDB,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,
0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XAE,0XFE,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,
0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0XD6,0X9A,0X6D,0XBB,0X6D,0XBB,
0X6D,0XBB,0X6D,0XBB,0X6D,0XBB,0X6D,0XBB,0X6D,0XBB,0X6D,0XBB,0X6D,0XBB,0X6D,0XBB,
0XAE,0XDE,0XAE,0XDE,0XAE,0XDE,0XAE,0XDE,0XAE,0XDE,0XAE,0XDE,0XAE,0XDE,0XAE,0XDE,
0XAE,0XDE,0XAE,0XDE,0XCE,0X79,0XCE,0X79,0XCE,0X79,0XCE,0X79,0XCE,0X79,0XCE,0X79,
0XCE,0X79,0XCE,0X79,0XCE,0X79,0XCE,0X79,0X6D,0X9B,0X6D,0X9B,0X6D,0X9B,0X6D,0X9B,
0X6D,0X9B,0X6D,0X9B,0X6D,0X9B,0X6D,0X9B,0X6D,0X9B,0X6D,0X9B,0XA6,0XDE,0XA6,0XDE,
0XA6,0XDE,0XA6,0XDE,0XA6,0XDE,0XA6,0XDE,0XA6,0XDE,0XA6,0XDE,0XA6,0XDE,0XA6,0XDE,
0XF7,0X9E,0X94,0X92,0X73,0XAE,0X73,0X8E,0X73,0X8E,0X73,0X8E,0X73,0X8E,0X73,0XAE,
0X94,0X92,0XF7,0X9E,0XFF,0XFF,0X3A,0XCE,0X2B,0X10,0X2B,0X11,0X2B,0X11,0X2B,0X11,
0X2B,0X11,0X2B,0X10,0X3A,0XCE,0XCE,0X59,0XF7,0X9E,0X64,0X96,0X44,0X16,0X3B,0XF6,
0X3B,0XF6,0X3B,0XF6,0X3B,0XF6,0X44,0X16,0X64,0X96,0XFF,0XFF,0X84,0X51,0X94,0X92,
0XEF,0X5D,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0X5D,0X94,0X92,0X84,0X51,
0X32,0XAE,0X3B,0X51,0X84,0XF5,0X9D,0X97,0X9D,0X97,0X9D,0X97,0X9D,0X97,0X95,0X57,
0X3B,0X51,0X32,0XAE,0X5C,0X55,0X6C,0XF8,0XDF,0X5E,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XDF,0X5E,0X6C,0XF8,0X5C,0X55,0X73,0XAE,0XEF,0X5D,0XFF,0XDF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XEF,0X5D,0X73,0XAE,0X2A,0XF0,0X84,0XF5,
0XBE,0X5A,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBE,0X5A,0X95,0X57,0X2A,0XF0,
0X3C,0X16,0XE7,0X7E,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XE7,0X7E,0X3C,0X16,0X73,0X8E,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0X73,0X8E,0X2B,0X11,0XB6,0X19,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XB6,0X19,0X2B,0X11,0X3B,0XF6,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X3B,0XF6,
0X73,0X8E,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XDF,0X73,0X8E,0X2B,0X11,0XAE,0X19,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XAE,0X19,0X2B,0X11,0X3B,0XF6,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XFF,0X3B,0XF6,0X73,0X8E,0XF7,0XBE,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XBE,0X73,0X8E,
0X2B,0X11,0XA5,0XF9,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XA5,0XF9,0X2B,0X11,0X3B,0XF6,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF7,0XFF,0X3B,0XF6,0X73,0X8E,0XF7,0XBE,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XBE,0X73,0X8E,0X2B,0X11,0X9D,0XD9,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X9D,0XD9,0X2B,0X11,
0X3B,0XF6,0XF7,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF7,0XDF,0X3B,0XF6,0X73,0X8E,0XF7,0XBE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF7,0XBE,0X73,0X8E,0X2B,0X11,0X7D,0X58,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X7D,0X58,0X2B,0X11,0X3B,0XF6,0XEF,0XDF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0XDF,0X3B,0XF6,
0X73,0X8E,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XDF,0X73,0X8E,0X2B,0X11,0X6D,0X38,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X6D,0X38,0X2B,0X11,0X3B,0XF6,0XEF,0XDF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0XDF,0X3B,0XF6,0X73,0X8E,0XFF,0XDF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0X73,0X8E,
0X2B,0X11,0X6D,0X17,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X6D,0X17,0X2B,0X11,0X3B,0XF6,0XEF,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XEF,0XBF,0X3B,0XF6,0X73,0X8E,0XF7,0XBE,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XBE,0X73,0X8E,0X2B,0X11,0X5C,0XD7,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5C,0XD7,0X2B,0X11,
0X3B,0XF6,0XEF,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XEF,0XBF,0X3B,0XF6,0X73,0XAE,0XE7,0X1C,0XEF,0X7D,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XEF,0X7D,0XE7,0X1C,0X73,0XAE,0X2B,0X11,0X54,0X96,0X6D,0XBB,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X65,0X7A,0X54,0X96,0X2B,0X11,0X3C,0X16,0XD7,0X3E,
0XDF,0X9F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0X9F,0XD7,0X3E,0X3C,0X16,
0X8C,0X51,0X8C,0X71,0XE7,0X3C,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XE7,0X3C,
0X8C,0X71,0X8C,0X51,0X43,0X71,0X2B,0X31,0X65,0X7A,0X6D,0X9B,0X6D,0X9B,0X6D,0X9B,
0X6D,0X9B,0X65,0X7A,0X2B,0X31,0X43,0X71,0X5C,0X56,0X64,0XD8,0XD7,0X3E,0XEF,0XBF,
0XEF,0XBF,0XEF,0XBF,0XEF,0XBF,0XD7,0X3E,0X64,0XD8,0X5C,0X56,0XF7,0X9E,0X84,0X30,
0X73,0XAE,0X73,0X8E,0X73,0X8E,0X73,0X8E,0X73,0X8E,0X73,0XAE,0X84,0X30,0XF7,0X9E,
0XF7,0X9E,0X4B,0XB2,0X2B,0X31,0X2B,0X11,0X2B,0X11,0X2B,0X11,0X2B,0X11,0X2B,0X31,
0X4B,0XB2,0XF7,0X9E,0XF7,0X9E,0X54,0X55,0X3C,0X16,0X3B,0XF6,0X3B,0XF6,0X3B,0XF6,
0X3B,0XF6,0X3C,0X16,0X54,0X55,0XF7,0X9E,};


HBUTTON button[40];
HFONT font[40];
u8 zpos[200];
HTRACK track[10];
HEDIT  edit[10];
u8 tempst[25];
void checknumber1()
{
	double temp;
	temp=StrToReal((char*)edit5->text);
	if (temp>38) {temp=38;strcpy((char*)edit5->text,(char *)RealToStr(temp));} 
	if (temp<29) {temp=29;strcpy((char*)edit5->text,(char *)RealToStr(temp));} 
}
void checknumber2()
{
	double temp;
	temp=StrToReal((char*)edit5->text);
	if (temp>80) {temp=80;strcpy((char*)edit5->text,(char *)RealToStr(temp));} 
	if (temp<0) {temp=0;strcpy((char*)edit5->text,(char *)RealToStr(temp));} 
}
u16 SendMessage(void *window,u16 Amessage,u16 Wparam,u16 Lparam)
{
	u16 i,x,y;
	double temp;
	if (Amessage&WM_TOUCHMESSAGE) {
		for (i=0; i<40; i++) if (button[i].flag) {
			switch(Amessage) {
				case WM_TOUCHDOWN:
					ButtonDown(&button[i],Wparam,Lparam);
				break;
				case WM_TOUCHMOVE:
					ButtonMouseMove(&button[i],Wparam,Lparam);
				break;
				case WM_TOUCHUP:
					ButtonUp(&button[i],Wparam,Lparam);
				break;
			}
		}
		for (i=0; i<10; i++) if (track[i].flag) {
			switch(Amessage) {
				case WM_TOUCHDOWN:
					TrackButtonDown(&track[i],Wparam,Lparam);
				break;
				case WM_TOUCHMOVE:
					TrackMouseMove(&track[i],Wparam,Lparam);
				break;
				case WM_TOUCHUP:
					TrackButtonUp(&track[i],Wparam,Lparam);
				break;
			}
		}
	} else if (Amessage==WM_COMMAND){
		switch (Wparam) {
			case 000:
				SetButtonState(button1,1);
				choose=0; delay=1; delay_left=10000; integral=0;
				UnSetButtonState(button2);
			break;
			case 001:
				SetButtonState(button2,1);
				choose=1; delay=1; delay_left=10000; integral=0;
				UnSetButtonState(button1);
			break;
			case 103://Set Voltage
				SetButtonState(but[0][3],1);
				choosedis=0; delay=1; delay_left=10000; integral=0;
				UnSetButtonState(but[0][4]);
				strcpy(edit5->text,RealToStr(SetVol)); 
				UpdateWindow(edit5);
			break;
			case 104://Set Duty
				SetButtonState(but[0][4],1);
				choosedis=1; delay=1; delay_left=10000; integral=0;
				UnSetButtonState(but[0][3]);
				strcpy(edit5->text,RealToStr(pwm*100)); 
				UpdateWindow(edit5);
			break;
			case 003:
			break;
			case 100:strcat((char*)edit5->text,"7"); UpdateWindow(edit5);break;
			case 101:strcat((char*)edit5->text,"8"); UpdateWindow(edit5);break;
			case 102:strcat((char*)edit5->text,"9"); UpdateWindow(edit5);break;
			case 105:strcat((char*)edit5->text,"4"); UpdateWindow(edit5);break;
			case 106:strcat((char*)edit5->text,"5"); UpdateWindow(edit5);break;
			case 107:strcat((char*)edit5->text,"6"); UpdateWindow(edit5);break;
			case 110:strcat((char*)edit5->text,"1"); UpdateWindow(edit5);break;
			case 111:strcat((char*)edit5->text,"2"); UpdateWindow(edit5);break;
			case 112:strcat((char*)edit5->text,"3"); UpdateWindow(edit5);break;
			case 113:strcat((char*)edit5->text,"0"); UpdateWindow(edit5);break;
			case 114:if (strchr((char*)edit5->text,'.')==NULL)strcat((char*)edit5->text,"."); 
				UpdateWindow(edit5);
			break;
			case 108:
				strcpy(tempst,edit5->text);
				if (strlen(tempst)!=0)	tempst[strlen(tempst)-1]='\0'; else strcpy(tempst,"0");
				strcpy(edit5->text,tempst); 
				UpdateWindow(edit5);
			break;
			case 109://Enter
				if (choosedis) {//Choose Set Duty
					checknumber2();
					UpdateWindow(edit5);
					pwm=StrToReal(edit5->text)/100.0;
					POINT_COLOR=BLACK;
					strcpy(edit4->text,edit5->text); 
					UpdateWindow(edit4);
					Dutytrack->pos=StrToReal(edit5->text);
					SetTrackPos(Dutytrack,Dutytrack->pos);
					//TIM3_Int_Init(42000000/200/(SetFreq));
					integral=0;
					delay=1; delay_left=10000;
				} else {//Choose Set Voltage
					checknumber1();
					UpdateWindow(edit5);
					SetVol=StrToReal(edit5->text);
					POINT_COLOR=BLACK;
					//TIM3_Int_Init(42000000/200/(SetFreq));
					integral=0;
					delay=1; delay_left=10000;
				}
			break;
		}
	} else if (Amessage==WM_TRACKPOSMOVE) {
		switch (Lparam) {
			case 201:
				if (!choose) {
					POINT_COLOR=BLACK;
					strcpy(edit4->text,RealToStr(Wparam)); 
					UpdateWindow(edit4);
					nowpwm=Wparam;
					pwm=nowpwm/100.0;
					//SetVol=Wparam;
					//TIM_SetCompare1(TIM1,Wparam);
					//TIM_SetCompare2(TIM1,Wparam);*/
				}
			break;	
		}
	} else if (Amessage==WM_TIMER) {
		for (i=0; i<10; i++) if (edit[i].flag) {
			edit[i].cursor=~edit[i].cursor;
			UpdateWindow(&edit[i]);
		}
	}
	return 1;
}
void SetWindowPos(void *window,u16 x,u16 y,u16 width,u16 height,u16 uFlag)
{
	u16 i;
	bool refresh;
	for (i=0; i<40; i++)
		if (&button[i]==window&&button[i].flag) {
			if (!(uFlag&SWP_NOPOS)) {
				if (button[i].x!=x||button[i].y!=y) refresh=true;
				button[i].x=x;
				button[i].y=y;
			}
			if (!(uFlag&SWP_NOSIZE)) {
				if (button[i].width!=width||button[i].height!=height) refresh=true;
				button[i].width=width;
				button[i].height=height;
			}
			if (refresh) UpdateWindow(window);
		}
}
HFONT *CreateFont(char *name,u16 size)
{
	
}
HBUTTON *CreateButton(u16 x,u16 y,u16 width,u16 height,char *name,HFONT *font,u16 cmd)
{
	u16 now=60,i;
	for (i=0; i<40; i++)
		if (!button[i].flag) {
			button[i].flag=true; now=i;
			break;
		}
	if (now==60) return NULL;
	button[now].font=font;
	button[now].x=x; button[now].y=y;
	button[now].width=width;
	button[now].height=height;
	button[now].edgewidth=2;
	button[now].edgeheight=2;
	button[now].cwidth=3;
	button[now].cheight=3;
	button[now].num=cmd;
	strcpy(button[now].text,name);
	
	UpdateWindow(&button[now]);
	return &button[now];
}
void ButtonDown(HBUTTON *now,int x,int y)
{
	if (x>=now->x&&y>=now->y&&x<=now->x+now->width-1&&y<=now->y+now->height-1) {
		if (!now->setstate) now->state=1;
		now->havepressed=true;
		UpdateWindow(now);
	}
}
void ButtonMouseMove(HBUTTON *now,int x,int y)
{
	u8 laststate=now->state;
	if (x>=now->x&&y>=now->y&&x<=now->x+now->width-1&&y<=now->y+now->height-1) {
		if (now->havepressed) {
			if (!now->setstate) now->state=1;
		}
	} else {
		if (now->state==1&&now->havepressed) {
			if (!now->setstate) now->state=2;
		}
	}
	if (laststate!=now->state) UpdateWindow(now);
}
void ButtonUp(HBUTTON *now,int x,int y)
{
	u8 laststate=now->state;
	if (x>=now->x&&y>=now->y&&x<=now->x+now->width-1&&y<=now->y+now->height-1) {
		if (now->havepressed) SendMessage(now,WM_COMMAND,now->num,0);
		now->havepressed=false;
	}
	if (!now->setstate) now->state=0;
	if (laststate) UpdateWindow(now);
}
void SetButtonState(HBUTTON *now,int state)
{
	now->setstate=1;
	now->state=state;
	UpdateWindow(now);
}
void UnSetButtonState(HBUTTON *now)
{
	now->setstate=0;
	now->state=0;
	UpdateWindow(now);
}

HTRACK *CreateTrackBar(u16 x,u16 y,u16 width,u16 height,u16 range,u16 cmd)
{
	u16 now=60,i;
	for (i=0; i<10; i++)
		if (!track[i].flag) {
			track[i].flag=true; now=i;
			break;
		}
	if (now==60) return NULL;
	track[now].x=x; track[now].y=y;
	track[now].width=width;
	track[now].height=height;
	track[now].pos=0;
	track[now].range=range;
	track[now].num=cmd;
	track[now].button=CreateButton(x+10,y+2,12,height-4,"",NULL,0);
	
	UpdateWindow(&track[now]);
	UpdateWindow(track[now].button);
	return &track[now];
}
void TrackButtonDown(HTRACK *now,int x,int y)
{
	if (x>=now->x&&y>=now->y&&x<=now->x+now->width-1&&y<=now->y+now->height-1) {
		if (now->button->state==1) {
			now->x0=x-now->button->x;
			now->y0=y-now->button->y;
			now->move=true;
		}
	}
}
void TrackMouseMove(HTRACK *now,int x,int y)
{
	if (now->move) {
		if (x-now->x0<now->x+10) x=now->x+10+now->x0;
		if (x-now->x0>now->x+now->width-22) x=now->x+now->width-22+now->x0;
		if (x-now->x0!=now->button->x) UpdateWindow(now);
		SetWindowPos(now->button,x-now->x0,now->y+2,0,0,SWP_NOSIZE);
		now->pos=(x-now->x0-now->x-10)*now->range/(now->width-32);
		SendMessage(now,WM_TRACKPOSMOVE,now->pos,now->num);
	}
}
void TrackButtonUp(HTRACK *now,int x,int y)
{
	now->x0=0;
	now->y0=0;
	now->move=false;
}
void SetTrackPos(HTRACK *now,u16 x)
{
	now->pos=x;
	UpdateWindow(now);
	SetWindowPos(now->button,(u32)(now->width-32)*now->pos/now->range+now->x+10,now->y+2,0,0,SWP_NOSIZE);
	UpdateWindow(now->button);
	SendMessage(now,WM_TRACKPOSMOVE,now->pos,now->num);
}
HEDIT *CreateEdit(u16 x,u16 y,u16 width,u16 height,u8 *text)
{
	u16 now=60,i;
	for (i=0; i<10; i++)
		if (!edit[i].flag) {
			edit[i].flag=true; now=i;
			break;
		}
	if (now==60) return NULL;
	edit[now].x=x; edit[now].y=y;
	edit[now].width=width;
	edit[now].height=height;
	strcpy(edit[now].text,text);
	
	UpdateWindow(&edit[now]);
	return &edit[now];
}

void UpdateWindow(void *window)
{
	u16 i;
	u16 x,y,width,height,cwidth,cheight,edgewidth,edgeheight;
	u8 state;
	u8 *p;
	for (i=0; i<40; i++)
		if (window==&button[i]&&button[i].flag) {
			x=button[i].x; y=button[i].y;
			width=button[i].width;
			height=button[i].height;
			cwidth=button[i].cwidth;
			cheight=button[i].cheight;
			edgewidth=button[i].edgewidth;
			edgeheight=button[i].edgeheight;
			state=button[i].state;
			StretchBlt(x,y,width,height,gImage_window,0+10*state,0,10,28);
			BitBlt(x,y,cwidth,cheight,gImage_window,0+10*state,28);
			BitBlt(x+width-cwidth,y,cwidth,cheight,gImage_window,7+10*state,28);
			BitBlt(x,y+height-cheight,cwidth,cheight,gImage_window,0+10*state,39);
			BitBlt(x+width-cwidth,y+height-cheight,cwidth,cheight,gImage_window,7+10*state,39);
			StretchBlt(x+cwidth,y,width-2*cwidth,edgeheight,gImage_window,3+10*state,28,4,2);
			StretchBlt(x,y+cheight,edgewidth,height-2*cheight,gImage_window,0+10*state,31,2,8);
			StretchBlt(x+width-edgewidth,y+cheight,edgewidth,height-2*cheight,gImage_window,8+10*state,31,2,8);
			StretchBlt(x+cwidth,y+height-edgeheight,width-2*cwidth,edgeheight,gImage_window,3+10*state,40,4,2);
			if (button[i].state==0) POINT_COLOR=BLACK; else POINT_COLOR=RED;
			LCD_ShowString(x+width/2-strlen(button[i].text)*3,y+height/2-6,strlen(button[i].text)*5+20,30,12,button[i].text);
			return;
		}
	for (i=0; i<10; i++) 
		if (window==&track[i]&&track[i].flag) {
			x=track[i].x; y=track[i].y;
			width=track[i].width;
			height=track[i].height;
			LCD_Fill(x,y,width,height,WHITE);
			LCD_Fast_DrawPoint(x+2,y+(height-4)/2,RGB(156,156,156));
			POINT_COLOR=RGB(176,176,176); LCD_DrawLine(x+3,y+(height-4)/2,x+width-3,y+(height-4)/2);
			LCD_Fast_DrawPoint(x+2,y+(height-4)/2+1,RGB(176,176,176));
			LCD_Fast_DrawPoint(x+2,y+(height-4)/2+2,RGB(176,176,176));
			POINT_COLOR=RGB(252,252,252); LCD_DrawLine(x+3,y+(height-4)/2+3,x+width-3,y+(height-4)/2+3);
			LCD_Fast_DrawPoint(x+width-3,y+(height-4)/2+1,RGB(231,234,234));
			LCD_Fast_DrawPoint(x+width-3,y+(height-4)/2+2,RGB(231,234,234));
			LCD_Fill(x+3,y+(height-4)/2+1,width-6,2,RGB(231,234,234));
			return;
		}
	for (i=0; i<10; i++) 
		if (window==&edit[i]&&edit[i].flag) {
			POINT_COLOR=0x738E;
			x=edit[i].x; y=edit[i].y;
			width=edit[i].width;
			height=edit[i].height;
			LCD_Fill(x,y,width,height,WHITE);
			LCD_DrawLine(x,y,x+width-1,y);
			LCD_DrawLine(x,y+height-1,x+width-1,y+height-1);
			LCD_DrawLine(x,y,x,y+height-1);
			LCD_DrawLine(x+width-1,y,x+width-1,y+height-1);
			LCD_ShowBigNum(x,y+(height-30)/2,StrToReal((char *)edit[i].text));
			if (edit[i].cursor) {
				POINT_COLOR=BLACK;
				LCD_DrawLine(x+7+15*strlen((char *)edit[i].text),y+5,x+7+15*strlen((char *)edit[i].text),y+35);
			}
			/*width+=x;
			height+=y;
			p=edit[i].text;
			while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
			{       
				if(x>=width){x=x0;y+=30;}
				if(y>=height)break;//退出
				LCD_ShowBigNum(x,y,*p,size,0);
				x+=15;
				p++;
			} */
			
		}
}