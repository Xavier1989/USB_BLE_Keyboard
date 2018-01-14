#ifndef __KEYBOARD_H
#define __KEYBOARD_H
#include "bsp.h"

typedef struct KeyPos_Type
{
	uint8_t row;
	uint8_t col;
}KeyPos;


typedef struct KeyEvent_Type
{
	KeyPos key;
	bool pressed;
}KeyEvent;

void Keyboard_Task(void);					//键盘任务
void Keyboard_Process(KeyEvent key_e);		//按键处理
void Keyboard_SendReport(uint8_t *buf);		//USB发送键值
void Keyboard_ReceiveReport(uint8_t data);	//USB输出处理
void Keyboard_FN_Combind(uint8_t keycode);	//FN组合键处理
#endif
