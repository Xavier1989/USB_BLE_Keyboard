#ifndef __USER_KEY_H
#define __USER_KEY_H
#include "bsp.h"
void Key_Scan_Init(void);					//按键扫描初始化
void Key_Scan(void);						//按键扫描
uint16_t Key_Read_Col(void);				//读取列数据
uint16_t Key_Matrix_Get_Row(uint8_t row);	//	获取按键扫描值

#endif
