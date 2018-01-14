/************************************************************
    *   @file:      user_dispaly.c
    *   @brief:     颜色显示模块
    *   @author:    XIET
    *   @version:   V1.0
    *   @data:      201x-xx-xx
    *   @modify:    data            remarks
**************************************************************/


//使用BGR
/********************************************************
    *****            GRB             BGR          BRG            RGB
    *****红          101     5       110   6      101     5      011   3     110 100 101 000 001 010 011
    *****玫红        100     4       010   2      001     1      010   2
    *****黄          001     1       100   4      100     4      001   1
    *****白          000     0       000   0      000     0      000   0
    *****绿          011     3       101   5      110     6      101   5
    *****蓝绿        010     2       001   1      010     2      100   4
    *****蓝          110     6       011   3      011     3      110   6
*********************************************************/
#include "bsp.h"
WORKE_MODE g_Work_Mode = IDLE_MODE;
uint8_t snake_Color = RED;
/*============================================================
    *   @func:      Display_OneImage
    *   @brief:     通过SPI显示一张图片
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
void Display_Init(void)
{
    g_Work_Mode = INIT_MODE;
    Display_Color(BLACK);
    Display_ON();
    bsp_StartAutoTimer(TMR_LED_CTRL, TMR_PERIOD_3MS);
    bsp_StartAutoTimer(TMR_LED_INIT, TMR_PERIOD_500MS);
}
/*============================================================
    *   @func:      Display_LEDPwr_Ctrl
    *   @brief:     LED电源控制，扫描方式
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
void Display_LEDPwr_Ctrl(void)
{
    static uint8_t row = 1;
    if(bsp_CheckTimer(TMR_LED_CTRL))
    {
        switch (row)
        {
        case 1:
            LED_ROW1_ON();
            break;
        case 2:
            LED_ROW2_ON();
            break;
        case 3:
            LED_ROW3_ON();
            break;
        case 4:
            LED_ROW4_ON();
            break;
        case 5:
            LED_ROW5_ON();
            break;
        case 6:
            LED_ROW6_ON();
            break;
        default:
            break;
        }
        if(++row > 6)
        {
            row = 1;
        }
    }
}
/*============================================================
    *   @func:      Display_LED_Select
    *   @brief:     LED行选择
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
static void Display_LED_Select(uint8_t row)
{
    switch (row)
    {
    case 0:
        LED_ROW1_ON();
        break;
    case 1:
        LED_ROW2_ON();
        break;
    case 2:
        LED_ROW3_ON();
        break;
    case 3:
        LED_ROW4_ON();
        break;
    case 4:
        LED_ROW5_ON();
        break;
    case 5:
        LED_ROW6_ON();
        break;
    default:
        break;
    }
}
/*============================================================
    *   @func:      Display_OneImage
    *   @brief:     通过SPI显示一张图片
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
void Display_Handle(void)
{
    Display_LEDPwr_Ctrl();
    switch(g_Work_Mode)
    {
    case IDLE_MODE:

        break;
    case INIT_MODE:
        Display_InitMode();
        break;
    case FLOW_MODE:
        if(bsp_CheckTimer(TMR_FLOW))
        {
            Display_FlowColor();
        }
        break;
    case SNAKE_MODE:
        if(bsp_CheckTimer(TMR_FLOW))
        {
            Display_Snake_Mode();
        }
        break;


    case STATIC_MODE:

    break;
		
		case SLEEP_MODE:
			System_Enter_StopMode();	
			break;

    default:
        break;
    }

}
/*============================================================
    *   @func:      Display_InitMode
    *   @brief:     显示出时候模式，用于测试LED是否正常
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
void Display_InitMode(void)
{
    static uint8_t state = 0;
    if(!bsp_CheckTimer(TMR_LED_INIT))
    {
        return ;
    }
    switch(state)
    {
    case 0:
        Display_Color(RED);
        break;
    case 1:
        Display_Color(GREEN);
        break;
    case 2:
        Display_Color(BLUE);
        break;
    case 3:
        Display_Color(BLACK);
        bsp_StopTimer(TMR_LED_CTRL);
        bsp_StopTimer(TMR_LED_INIT);
        LED_ALL_ROW_OFF();
        g_Work_Mode = IDLE_MODE;
//        Display_SetWorkMode(STATIC_MODE);

        break;
    default:
        break;
    }
    state++;
}

/*============================================================
    *   @func:      Display_ON
    *   @brief:     通过SPI显示一张图片
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
void Display_ON(void)
{
    GPIO_LED_Power(ENABLE);
}
/*============================================================
    *   @func:      Display_OneImage
    *   @brief:     通过SPI显示一张图片
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
void Display_OFF(void)
{
    GPIO_LED_Power(DISABLE);
}
/*============================================================
    *   @func:      XX
    *   @brief:     XX
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
void Display_Start_Flow(void)
{
    if(g_Work_Mode == FLOW_MODE)
    {
        return;
    }
    g_Work_Mode = FLOW_MODE;
    bsp_StartAutoTimer(TMR_FLOW, TMR_PERIOD_50MS);
    Display_ON();
}
/*============================================================
    *   @func:      Display_OneImage
    *   @brief:     通过SPI显示一张图片
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
void Display_OneImage(uint8_t *data)
{
    HC595_LAT_LOW();
    HC595_SendData((uint16_t *)data, 3);
    bsp_DelayUS(10);
    HC595_LAT_HIGH();
}

/*============================================================
    *   @func:      Display_Color
    *   @brief:     一次只能显示一种颜色
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
void Display_Color(uint8_t color)
{
    uint8_t i = 0;
    uint32_t temp[2] = {0x00, 0x00};
    for (i = 0; i < 16; ++i)
    {
        temp[0] = (temp[0] >> 3) | ((temp[1] & 0x07) << 29);
        temp[1] = (temp[1] >> 3) | (color << 13);
    }
    Display_OneImage((uint8_t *)temp);
}
/*============================================================
    *   @func:      Display_LED_Blue
    *   @brief:     显示蓝色
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
void Display_FlowColor(void)
{
    static uint8_t i = 0;
    static uint8_t j = 0;
    // uint8_t color[12] = {3, 2, 1, 0, 5, 4, 6, 4, 5, 0, 1, 2};//RGB
    // uint8_t color[10] = {3,2,6,4,5,1,5,4,6,2};//RGB
    uint8_t color[6] = {3, 2, 6, 4, 5, 1}; //RGB
    static uint32_t temp[2] = {0xFFFFFFFF, 0xFFFF};
    Display_OneImage((uint8_t *)temp);
    temp[0] = (temp[0] >> 3) | ((temp[1] & 0x07) << 29);
    temp[1] = (temp[1] >> 3) | (color[j] << 13);
    if(++i == 4)
    {
        i = 0;
        if(++j == 6)
        {
            j = 0;
        }
    }
}

/*============================================================
    *   @func:      XX
    *   @brief:     XX
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
void Display_Indicate_LED(uint8_t led, uint8_t state)
{
    switch (led)
    {
    case LED_NUM_LOCK:
        break;
    case LED_CAPS_LOCK:
        if(state == ENABLE)
        {
            Display_OneLED(4, 1, RED);
            LED_ROW4_ON();
        }
        else
        {
            Display_OneLED(4, 1, BLACK);
            LED_ALL_ROW_OFF();
        }

        break;
    case LED_SCROLL_LOCK:

        break;
    case LED_COMPOSE:

        break;
    case LED_KANA:

        break;

    default:
        break;
    }

}
/*============================================================
    *   @func:      Display_Color
    *   @brief:     一次只能显示一种颜色
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
void Display_OneLED(uint8_t row, uint8_t col, uint8_t color)
{
    uint8_t i = 0;
    uint32_t temp[2] = {0xFFFFFFFF, 0xFFFF};
    temp[1] = temp[1] | (color << 16);
    for (i = 0; i < col; ++i)
    {
        temp[0] = (temp[0] >> 3) | ((temp[1] & 0x07) << 29);
        temp[1] = (temp[1] >> 3) | (0xFF << 16);
    }
    Display_OneImage((uint8_t *)temp);
}
/*============================================================
    *   @func:      Display_Snake_LED
    *   @brief:     贪吃蛇模式LED控制
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
static void Display_Snake_LED(uint8_t row_num)
{
    static uint8_t i = 0;
    static  uint32_t temp[2] = {0xFFFFFFFF, 0xFFFF};
    if(i == 0)
    {
        temp[1] = temp[1] | (snake_Color << 16);
    }
    temp[0] = (temp[0] >> 3) | ((temp[1] & 0x07) << 29);
    temp[1] = (temp[1] >> 3) | (0xFF << 16);
    Display_OneImage((uint8_t *)temp);
    if(++i == row_num)
    {
        i = 0;
        temp[0] = 0xFFFFFFFF;
        temp[1] = 0xFFFF;
    }
}
/*============================================================
    *   @func:      Display_Snake_Init
    *   @brief:     贪吃蛇模式初始化
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
static void Display_Snake_Init(void)
{
    Display_LED_Select(0);
}
/*============================================================
    *   @func:      Display_Snake_Mode
    *   @brief:     贪吃蛇模式
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
void Display_Snake_Mode(void)
{
    static uint8_t rows_num[6] = {16, 15, 15, 14, 14, 14};
    static uint8_t i = 0, j = 0;
    Display_LED_Select(j);
    Display_Snake_LED(rows_num[j]);
    if(++i == rows_num[j])
    {
        i = 0;
        if(++j==6)
        {
            j=0;
        }
    }

}

/*============================================================
    *   @func:      Display_SetWorkMode
    *   @brief:     设置显示工作模式
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
void Display_SetWorkMode(uint8_t mode)
{
    g_Work_Mode = (WORKE_MODE)mode;

    switch (mode)
    {
    case FLOW_MODE:
        bsp_StartAutoTimer(TMR_FLOW, TMR_PERIOD_50MS);
        bsp_StartAutoTimer(TMR_LED_CTRL, TMR_PERIOD_3MS);
        break;
    case SNAKE_MODE:
        Display_Snake_Init();
        bsp_StartAutoTimer(TMR_FLOW, TMR_PERIOD_80MS);
        break;

    case IDLE_MODE:
        bsp_StopTimer(TMR_LED_CTRL);
        bsp_StopTimer(TMR_FLOW);
        Display_Color(BLACK);
        LED_ALL_ROW_OFF();

        break;

    case STATIC_MODE:
        bsp_StartAutoTimer(TMR_LED_CTRL, TMR_PERIOD_3MS);
        Display_Color(RED);
    break;

    default:
        break;
    }

}
/*============================================================
    *   @func:      Display_ChangeSnakeColor
    *   @brief:     设置显示工作模式
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
void Display_ChangeSnakeColor(void)
{
    snake_Color = (snake_Color+1) % 7;
}
/*============================================================
    *   @func:      Display_ChangeStaticColor
    *   @brief:     设置显示工作模式
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
void Display_ChangeStaticColor(void)
{
    static uint8_t color = RED;
    color += 1; 
    Display_Color(color % 7);
}


