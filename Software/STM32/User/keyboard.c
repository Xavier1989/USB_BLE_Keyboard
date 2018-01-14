/************************************************************
	*	@file:		XX.c
	*	@brief:		XX
	*	@author:	XIET
	*	@version:	V1.0
	*	@data:		201x-xx-xx
	*	@modify:	data  			remarks
**************************************************************/
#include "bsp.h"
extern PCD_HandleTypeDef hpcd;
extern USBD_HandleTypeDef USBD_Device;
uint8_t report_release[8]={0,0,0,0,0,0,0,0};
uint8_t report_buf[8]={0,0,0,0,0,0,0,0};
uint8_t send_report_flag=0;				//USB 发送报告标志
static uint8_t FN0_Press_Flag=0;		//FN键按下标志 1：按下 0：释放

static uint8_t USB_BLE_Switch=1;
/*============================================================
	*	@func:		XX
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
void Keyboard_Task(void)
{
	if(!bsp_CheckTimer(TMR_KEY_SCAN))
	{
		return;
	}
	static uint16_t matrix_prev[MATRIX_ROWS];
	uint16_t matrix_now=0;
	uint16_t matrix_change=0;
	KeyEvent key_e;
	send_report_flag = 0;
	Key_Scan();
	for (uint8_t r = 0; r < MATRIX_ROWS; ++r)
	{
		matrix_now = Key_Matrix_Get_Row(r);
		matrix_change = matrix_now ^ matrix_prev[r];
		if(matrix_change)
		{
			bsp_StartTimer(TMR_SLEEP,TMR_PERIOD_10MIN);
			send_report_flag = 1;
			for (uint8_t c = 0; c < MATRIX_COLS; ++c)
			{
				if(matrix_change & ((uint16_t)1<<c))
				{
					key_e.key.row=r;
					key_e.key.col=c;
					key_e.pressed=matrix_now & ((uint16_t)1<<c);
					Keyboard_Process(key_e);
					matrix_prev[r] ^= (uint16_t)1<<c;
				}
			}
		}
	}
	if(send_report_flag)
	{
		Keyboard_SendReport(report_buf);
	}
}

/*============================================================
	*	@func:		XX
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
void Keyboard_Process(KeyEvent key_e)
{
	uint8_t key_code=0;
	static uint8_t modifier_key=0;
	uint8_t static key_buf[6]={0,0,0,0,0,0};
	if(key_e.pressed)
	{
		key_code = keymaps[0][key_e.key.row][key_e.key.col];
		if((key_code & 0xF0) == 0xE0 )		//功能键
		{
			modifier_key |= 1 << (key_code & 0x0F);
			report_buf[0] = modifier_key;
		}
		else							//常规按键
		{
			for (uint8_t i=0; i < 6; i++)
			{
				if(key_buf[i]==0)
				{
					key_buf[i] = key_code;
					break;
				}
			}
			Keyboard_FN_Combind(key_code);
		}
	}
	else
	{
		key_code = keymaps[0][key_e.key.row][key_e.key.col];
		if((key_code & 0xF0) == 0xE0 )		//功能键
		{
			modifier_key &= ~(1 << (key_code & 0x0F));
			report_buf[0] = modifier_key;
		}
		else						//常规按键
		{
			for (uint8_t i = 0; i < 6; ++i)
			{
				if(key_code == key_buf[i])
				{
					key_buf[i]=0;
				}
			}
		}
	}
	if(key_code == KC_FN0)		//Fn键
	{
		FN0_Press_Flag ^= 0x01;
	}
	memcpy(&report_buf[2],key_buf,6);	
}

/*============================================================
	*	@func:		XX
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
void Keyboard_SendReport(uint8_t *buf)
{
	if(USB_BLE_Switch)
	{
		USBD_HID_SendReport(&USBD_Device,buf,8);
	}
	else
	{
		comSendBuf(COM_BLE,buf,8);
	}
}
/*============================================================
	*	@func:		XX
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
void Keyboard_ReceiveReport(uint8_t data)
{
	if(g_Work_Mode != IDLE_MODE)
	{
		return;
	}
	if(data & (1<<LED_CAPS_LOCK))
	{
		Display_Indicate_LED(LED_CAPS_LOCK,ENABLE);
	}
	else
	{
		Display_Indicate_LED(LED_CAPS_LOCK,DISABLE);
	}
}
/*============================================================
	*	@func:		XX
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
void Keyboard_FN_Combind(uint8_t keycode)
{
	if(!FN0_Press_Flag)		//Fn键未按下，直接返回
	{
		return ;
	}
	send_report_flag = 0;//FN 键按下，USB不发送键值
	if(keycode == KC_W)		//snake mode
	{
		Display_SetWorkMode(SNAKE_MODE);
	}
	else if(keycode == KC_F)		//flow mode
	{
		Display_SetWorkMode(FLOW_MODE);
	}
	else if(keycode == KC_E)	//end 
	{
		Display_SetWorkMode(IDLE_MODE);
	}
	else if(keycode == KC_T)		//change snake color
	{
		Display_SetWorkMode(STATIC_MODE);
	}
	else if(keycode == KC_C)		//change snake color
	{
		if(g_Work_Mode ==SNAKE_MODE)
		{
			Display_ChangeSnakeColor();
		}
		else if(g_Work_Mode ==STATIC_MODE)
		{
			Display_ChangeStaticColor();
		}
	}
	else if(keycode == KC_U)		//USB
	{
		USB_BLE_Switch = 1;
	}
	else if(keycode == KC_B)		//BLE
	{
		USB_BLE_Switch =0;
	}
	else if(keycode == KC_S)
	{
		USB_BLE_Switch ^= 1;
	}
}


