 /**
 ****************************************************************************************
 *
 * @file app_stream.c
 *
 * @brief Stream application.
 *
 * Copyright (C) 2012. Dialog Semiconductor Ltd, unpublished work. This computer 
 * program includes Confidential, Proprietary Information and is a Trade Secret of 
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited 
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"               // SW configuration
#if (BLE_APP_PRESENT)

#include "app_stream.h"

#include <string.h>                  // string manipulation and functions

#include "app.h"                     // application definitions
#include "app_task.h"                // application task definitions
#include "streamdatad_task.h"              // streamd functions
#include "l2cc_task.h"
#include "streamdatad.h"
#include "co_bt.h"

#include "arch.h"                      // platform definitions
#include "datasheet.h"
#include "gpio.h"

#if PLF_DISPLAY
#include "display.h"
#endif //PLF_DISPLAY



int mybuttoncounter = 0;
int fw_version = 0x08130003;

void app_stream_send_enable(short value);

int app_stream_stop_when_the_buffer_is_empty;

static int app_stream_streamon; 

int app_stream_check_for_button_release;
int app_stream_direct_streaming;


void app_stream_create_db(void)
{
    // Add STREAMDATA in the database
    struct streamdatad_create_db_req * req = KE_MSG_ALLOC(STREAMDATAD_CREATE_DB_REQ, TASK_STREAMDATAD,
                                                   TASK_APP, streamdatad_create_db_req);

    req->start_hdl = 0;

    // Send the message
    ke_msg_send(req);
}


void stream_start_button_init(void)
{      
    NVIC_DisableIRQ(GPIO3_IRQn);

    //Push Button input	
    GPIO_ConfigurePin( GPIO_PORT_0, GPIO_PIN_6, INPUT_PULLUP, PID_GPIO, false );

    SetBits16(GPIO_INT_LEVEL_CTRL_REG, EDGE_LEVELn3, 1); //select falling edge P1.1
    SetBits16(GPIO_INT_LEVEL_CTRL_REG, INPUT_LEVEL3, 1); //select falling edge P1.1
    SetWord16(GPIO_IRQ3_IN_SEL_REG, 7); //P1.1 is selected, GPIO Input push buttton
    //SetWord16(GPIO_RESET_IRQ_REG, 0x8);

    NVIC_SetPriority(GPIO3_IRQn,1); 
    NVIC_EnableIRQ(GPIO3_IRQn);
}

void app_stream_idle_handler(void)
{
   if (1 == app_stream_streamon)
	{
        if (ke_state_get(TASK_APP) != APP_CONNECTED){ 
            return;
        }
		app_stream_streamon = 0;
        app_stream_check_for_button_release = 1;
        app_stream_direct_streaming = 0;
		app_stream_send_enable(1); // will trigger an stream_start()
	}
}

void GPIO3_Handler(void)
{
   // NVIC_DisableIRQ(GPIO3_IRQn); /* in case we want to block more presses a bit */

   mybuttoncounter++;

   SetWord16(GPIO_RESET_IRQ_REG,1<<3); // reset GPIO3 IRQ 

	//	NVIC_EnableIRQ(GPIO3_IRQn);
   app_stream_stop_when_the_buffer_is_empty = 0; // while the button is pressed.
   app_stream_streamon = 1;
    
   //set_pxact_gpio();
}

void app_stream_button_released(void)
{
    if(app_stream_check_for_button_release)
    {
       app_stream_check_for_button_release = 0;
        
       app_stream_stop_when_the_buffer_is_empty = 1;
       stream_stop();
       app_stream_direct_streaming = 0;
       
       // We're not doing app_stream_send_enable(0); here
       // because that's done in app_stream.c::stream_queue_more_data()
       // after sending everything from the buffer.
       
       // We're sending this extra streamon notification to sell the gui that the 
       // button is relased.
       // The gui can then use that to immediately start playback, because at least
       // half of the stream command has already been transmitted, guaranteeing
       // uninterrupted playback of the entire command if the datarate doesn't go down 
       // (which is the case)
       //app_stream_send_enable(1);
    }
}

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
// Called when the streamdatad atts database has been created
void app_stream_init(void)
{
   app_stream_stop_when_the_buffer_is_empty = 0;
   app_stream_streamon = 0;
   app_stream_check_for_button_release = 0;
   app_stream_direct_streaming = 1;

   stream_start_button_init();

   if(fw_version)
      mybuttoncounter++;
}


/**
 ****************************************************************************************
 * Streamer Application Functions
 ****************************************************************************************
 */

// Called when STREAMON is received
void stream_start(void)
{
	//SetWord16(P0_SET_DATA_REG,   0x80); // Green LED on
    
    //SetBits16(SPI_CTRL_REG,SPI_ON,0);
    //SetWord16(SPI_CTRL_REG,0xC940);
    //SetBits16(SPI_CTRL_REG,SPI_ON,1);
    app_stream_stop_when_the_buffer_is_empty = 0;
}

// Called when a connection has been made
void app_stream_enable(void)
{
    //SetBits16(P07_MODE_REG,PID,PID_GPIO_PUPD);
    //SetBits16(P07_MODE_REG,PUPD,DIR_OUPUT);
    //SetWord16(P0_SET_DATA_REG,   0x80); // Green LED on

    // Allocate the message
    struct streamdatad_enable_req * req = KE_MSG_ALLOC(STREAMDATAD_ENABLE_REQ, TASK_STREAMDATAD, TASK_APP,
                                                 streamdatad_enable_req);

    // Fill in the parameter structure
    req->appid = TASK_APP;
    req->conhdl = app_env.conhdl;

    // Send the message
    ke_msg_send(req);
}

// Called when the connection bonding is completed.
void app_stream_start(void)
{
		// LED Output 
//    SetBits16(P07_MODE_REG,PID,PID_GPIO_PUPD);
//    SetBits16(P07_MODE_REG,PUPD,DIR_OUPUT);
//    SetWord16(P0_RESET_DATA_REG,   0x80);


	//SetBits16(P13_MODE_REG,PID,PID_GPIO_PUPD);
    //SetBits16(P13_MODE_REG,PUPD,DIR_OUPUT);
}

// Called when STREAMOFF is received
void stream_stop(void)
{
	//SetWord16(P0_RESET_DATA_REG, 0x80); // Green LED off
    app_stream_direct_streaming = 1;
#if BLE_SPI
    SetBits16(SPI_CTRL_REG,SPI_ON,0);         // spi disable
    pmesg_stream_off(&mesg111);
    pmesg_stream_off111(&mesg111);
#endif
}

uint8 num_of_pckets = 0;

void stream_queue_more_data(uint16_t nr_packets)
{
    uint8 dst[STREAMDATAD_PACKET_SIZE];
    
    if( num_of_pckets == STREAMDATAD_MAX || (l2cm_get_nb_buffer_available() == 0 ))
    {
        //set_pxact_gpio();
        num_of_pckets = 0;
        return;
    }
    
      //if( num_of_pckets == 0)
          //set_pxact_gpio();


    if (app_stream_stop_when_the_buffer_is_empty) // true on button release
    {
        app_stream_send_enable(0);
        app_stream_stop_when_the_buffer_is_empty = 0;
    }
        
    memset(dst, num_of_pckets, STREAMDATAD_PACKET_SIZE);
    dst[0] = (num_of_pckets >> 8);
    {
        struct l2cc_pdu_send_req *pkt = KE_MSG_ALLOC(L2CC_PDU_SEND_REQ,
                                                     KE_BUILD_ID(TASK_L2CC, app_env.conidx), TASK_APP,
                                                     l2cc_pdu_send_req);

        // Set attribute channel ID
        pkt->pdu.chan_id   = L2C_CID_ATTRIBUTE;
        // Set packet opcode.
        pkt->pdu.data.code = L2C_CODE_ATT_HDL_VAL_NTF;
        pkt->pdu.data.hdl_val_ntf.handle    = STREAMDATAD_DIR_VAL_HANDLE(num_of_pckets);
        pkt->pdu.data.hdl_val_ntf.value_len = STREAMDATAD_PACKET_SIZE;
        /* copy the content to value */
        memcpy(&(pkt->pdu.data.hdl_val_ntf.value[0]), dst, pkt->pdu.data.hdl_val_ntf.value_len);
        num_of_pckets++;
        ke_msg_send(pkt);
    }

}

#endif // (BLE_APP_PRESENT)

/// @} APP
