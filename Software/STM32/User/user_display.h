#ifndef __USER_DISPLAY_H
#define __USER_DISPLAY_H
#include "bsp.h"
typedef enum
 {
 	RED=3,
 	GREEN=5,
 	BLUE=6,
 	BLACK=7,
 	WHITE=0
 }COLOR_VALUE;

 typedef enum
{
	IDLE_MODE=0x11,
	INIT_MODE=0x30,
	FLOW_MODE=0x32,
	SNAKE_MODE=0x33,
	STATIC_MODE=0x34,
	OTA_MODE=0x55,
	SLEEP_MODE=0x60

}WORKE_MODE;
extern WORKE_MODE g_Work_Mode;
void Display_Init(void);						//显示初始化
void Display_ON(void);							//开启显示
void Display_OFF(void);							//关闭显示
void Display_OneImage(uint8_t *data);			//显示一张图片
void Display_FlowColor(void);					//流水灯控制
void Display_Color(uint8_t color);				//显示指定颜色
void Display_Image(void);						//刷图
void Display_Handle(void);						//显示处理
void Display_Start_Flow(void);					//启动流水灯
void Display_LEDPwr_Ctrl(void);					//LED电源控制切换
void Display_InitMode(void);					//初始化模式
void Display_Indicate_LED(uint8_t led, uint8_t state);	//指示灯状态
void Display_OneLED(uint8_t row, uint8_t col, uint8_t color);	//制定某行某列LED显示颜色	
void Display_SetWorkMode(uint8_t mode);			//设置显示模式
void Display_Snake_Mode(void);					//贪吃蛇模式
void Display_ChangeSnakeColor(void);			//改变贪吃蛇颜色
void Display_ChangeStaticColor(void);			//改变静态常亮颜色
#endif
