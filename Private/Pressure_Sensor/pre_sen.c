#include "pre_sen.h"

#include <string.h>

#include "message_define.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "usbd_cdc_if.h"


static uint8_t Pre_Set_Decimal_digits[]={0x01, 0x06, 0x00, 0x01, 0x00, 0x01, 0x19, 0xCA};//设置小数点后精确到一位
static uint8_t Pre_Set_Unit[]={0x01, 0x06, 0x00, 0x02, 0x00, 0x05, 0xE8, 0x09};//设置单位：N
static uint8_t Pre_get_Value[]={0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};//读取传感器数值

uint8_t receive_data[8]={0x00};

Pre_receive pre_send_reback(uint8_t* data)
{


    HAL_UART_Transmit(Pressure_UART_handler, data, 8, 10);

    HAL_UART_Receive(Pressure_UART_handler, receive_data, 8, 100);

    if (memcmp(data, receive_data, 8)==0)
    {
        return Pre_OK;
    }
    return Pre_Fail;
}


void pre_init(void)
{
    MESSAGE_Packet message_Packet;
    message_Packet.SourceType = Pressure;

    // uint8_t Pre_Buf[16]={0x00};
    //
    //
    //
    //
    //
    //
    // uint8_t Pre_init_OK[] = {0x01, 0x01};
    //
    //
    //
    // char Pre_Decimal_digits_OK[]="Pressure Sensor：小数位初始化完成";
    // char Pre_Decimal_digits_Fail[]="Pressure Sensor：小数位初始化失败";
    //
    // char Pre_Set_Unit_OK[]="Pressure Sensor：单位初始化完成";
    // char Pre_Set_Unit_Fail[]="Pressure Sensor：单位初始化失败";
    //
    // HAL_UART_Transmit(&huart2, Pre_Set_Decimal_digits, sizeof(Pre_Set_Decimal_digits),10);
    // HAL_UART_Receive(&huart2,Pre_Buf,8,100);
    // if (memcmp(Pre_Buf,Pre_Set_Decimal_digits,8)==0) {
    //   CDC_Transmit_FS(Pre_Decimal_digits_OK, sizeof(Pre_Decimal_digits_OK));
    // } else {
    //   CDC_Transmit_FS(Pre_Decimal_digits_Fail, sizeof(Pre_Decimal_digits_Fail));
    // }



    /*设置小数位*/
    if (pre_send_reback(Pre_Set_Decimal_digits)!=Pre_OK)
    {
        Send_Error_to_PC(message_Packet);
    }

    if (pre_send_reback(Pre_Set_Unit)!=Pre_OK)
    {
        Send_Error_to_PC(message_Packet);
    }




    Send_Init_Success_to_PC(message_Packet);
}
