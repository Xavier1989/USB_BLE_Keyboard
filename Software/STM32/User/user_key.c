/************************************************************
    *   @file:      XX.c
    *   @brief:     XX
    *   @author:    XIET
    *   @version:   V1.0
    *   @data:      201x-xx-xx
    *   @modify:    data            remarks
**************************************************************/
#include "bsp.h"
#define DEBOUNCE 5
uint16_t matrix[MATRIX_ROWS];
uint16_t matrix_Debouncing[MATRIX_ROWS];
static uint8_t debouncing = DEBOUNCE;

static void Key_Select_Row(uint8_t row);
/*============================================================
    *   @func:      Key_Scan_Init
    *   @brief:     按键扫描初始化
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
void Key_Scan_Init(void)
{
    KEY_ALL_ROW_UNSELECT();     //释放行
    for (uint8_t i = 0; i < MATRIX_ROWS; ++i)
    {
        matrix[i] = 0;
        matrix_Debouncing[i] = 0;
    }
    bsp_StartAutoTimer(TMR_KEY_SCAN, TMR_PERIOD_2MS);
    bsp_StartTimer(TMR_SLEEP, TMR_PERIOD_10MIN);
}
/*============================================================
    *   @func:      XX
    *   @brief:     XX
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
void Key_Scan(void)
{
    uint8_t i;
    uint16_t col_Value;

    for (i = 0; i < MATRIX_ROWS; ++i)
    {
        Key_Select_Row(i);
        bsp_DelayUS(10);
        col_Value = Key_Read_Col();
        if(col_Value != matrix_Debouncing[i])
        {
            matrix_Debouncing[i] = col_Value;
            debouncing = DEBOUNCE;
        }
    }
    if(debouncing)
    {
        if(--debouncing)
        {
        }
        else
        {
            for ( i = 0; i < MATRIX_ROWS; ++i)
            {
                matrix[i] = matrix_Debouncing[i];
            }
        }
    }
}

/*============================================================
    *   @func:      Key_Select_Row
    *   @brief:     选择需要扫描的按键行
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
static void Key_Select_Row(uint8_t row)
{
    switch(row)
    {
    case 0:
        KEY_ROW1_SELECT();
        break;
    case 1:
        KEY_ROW2_SELECT();
        break;
    case 2:
        KEY_ROW3_SELECT();
        break;
    case 3:
        KEY_ROW4_SELECT();
        break;
    case 4:
        KEY_ROW5_SELECT();
        break;
    case 5:
        KEY_ROW6_SELECT();
        break;
    default:
        break;
    }
}
/*============================================================
    *   @func:      Key_Read_Col
    *   @brief:     读取按键扫描列数据
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
uint16_t Key_Read_Col(void)
{
    return (uint16_t)(LL_GPIO_ReadInputPort(KEY_COL_PORT));
}
/*============================================================
    *   @func:      Key_Read_Col
    *   @brief:     读取按键扫描列数据
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
uint16_t Key_Matrix_Get_Row(uint8_t row)
{
    return matrix[row];
}
