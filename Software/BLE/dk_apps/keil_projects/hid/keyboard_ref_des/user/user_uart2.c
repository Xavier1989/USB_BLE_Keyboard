/************************************************************
    *   @file:      XX.c
    *   @brief:     XX
    *   @author:    XIET
    *   @version:   V1.0
    *   @data:      201x-xx-xx
    *   @modify:    data            remarks
**************************************************************/
#include "bsp.h"
#define UART2_BUF_SIZES     8
uint8_t uart_rxbuf[UART2_BUF_SIZES];
uint8_t uart_txbuf[UART2_BUF_SIZES];
uint8_t buf_sizes = UART2_BUF_SIZES;
volatile uint32_t pass_code __attribute__((section("retention_mem_area0"), zero_init));
extern enum REPORT_MODE kbd_reports_en;

/*============================================================
    *   @func:      XX
    *   @brief:     XX
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
static void user_Send_Passcode(void)
{
    uint8_t keycode;
    int num = -1;

    for (uint8_t i = 0; i < 6; ++i)
    {
        keycode = uart_rxbuf[i+2];
				//首先判断键值是否存在
        if(keycode == 0x00)         
        {
            break;
        }
        /***********number key 判断键值是否有效**************/
        if((keycode >= 0x1E) && (keycode <= 0x27))
        {
            num = keycode - 0x1D;
        }
        if(num == 10)
        {
            num = 0;
        }
        if(num != -1)
        {
            pass_code *= 10;
            pass_code += num;
        }
				if(keycode == 0x28)            //回车
				{
					app_mitm_passcode_report(pass_code);
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
static void User_Send_HID_Report(void)
{
    struct hogpd_report_info *req;
    req = KE_MSG_ALLOC_DYN(HOGPD_REPORT_UPD_REQ, TASK_HOGPD, TASK_APP, hogpd_report_info, 8);

    req->conhdl = app_env.conhdl;
    req->hids_nb = 0;
    req->report_nb = 0;
    req->report_length = 8;
    memcpy(req->report, uart_rxbuf, 8);

    dbg_printf(DBG_SCAN_LVL, "Sending HOGPD_REPORT_UPD_REQ %02x:[%02x:%02x:%02x:%02x:%02x:%02x]\r\n",
               (int)uart_rxbuf[0], (int)uart_rxbuf[2], (int)uart_rxbuf[3], (int)uart_rxbuf[4], (int)uart_rxbuf[5], (int)uart_rxbuf[6], (int)uart_rxbuf[7]);

    ke_msg_send(req);
}

/*============================================================
    *   @func:      XX
    *   @brief:     XX
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
static void User_Uart2_Callback(uint8_t state)
{
    if(state == UART_STATUS_OK)
    {
        if (kbd_reports_en == REPORTS_ENABLED)
        {
            User_Send_HID_Report();
        }
        else
        {
            user_Send_Passcode();
        }
        uart2_write(uart_rxbuf, buf_sizes, NULL);    //串口2打印接收到的数据
        User_Uart2_RX_Pre();                         //串口2重新进入接收状态
    }
}
/*============================================================
    *   @func:      XX
    *   @brief:     XX
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
void User_Uart2_RX_Pre(void)
{
    uart2_read_func(uart_rxbuf, buf_sizes, &User_Uart2_Callback);
}



