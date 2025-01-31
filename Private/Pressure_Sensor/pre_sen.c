#include "pre_sen.h"

#include <string.h>


#include "message_define.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "usbd_cdc_if.h"


static const uint8_t Pre_Set_Decimal_digits[]={0x01, 0x06, 0x00, 0x01, 0x00, 0x01, 0x19, 0xCA};//设置小数点后精确到一位
static const uint8_t Pre_Set_Unit[]={0x01, 0x06, 0x00, 0x02, 0x00, 0x05, 0xE8, 0x09};//设置单位：N
static const uint8_t Pre_read_Value[]={0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};//读取传感器数值
static const uint8_t Pre_clear[] ={0x01, 0x06, 0x00, 0x11, 0x00, 0x01, 0x18, 0x0F};//清零数值

/*声明信息包*/
static MESSAGE_Packet message_Packet_Pre;


Pre_receive pre_send_reback(uint8_t* data)
{
    uint8_t receive_data[8]={0x00};

    HAL_UART_Transmit(Pressure_UART_handler, data, 8, 10);

    HAL_UART_Receive(Pressure_UART_handler, receive_data, 8, 100);

    if (memcmp(data, receive_data, 8)==0)
    {
        return Pre_OK;
    }
    return Pre_Fail;
}


/*初始化*/
void pre_init(void)
{
    /*初始化信息包*/
    message_Packet_Pre.SourceType = Pressure;

    /*设置小数位 : 一位小数*/
    if (pre_send_reback(Pre_Set_Decimal_digits)!=Pre_OK)
    {
        Send_Error_to_PC(message_Packet_Pre, 0x01);
    }

    /*设置单位 : N*/
    if (pre_send_reback(Pre_Set_Unit)!=Pre_OK)
    {
        Send_Error_to_PC(message_Packet_Pre, 0x02);
    }

    /*清零数值*/
    if (pre_send_reback(Pre_clear)!=Pre_OK)
    {
        Send_Error_to_PC(message_Packet_Pre, 0x03);
    }

    /*发送初始化成功*/
    Send_Init_Success_to_PC(message_Packet_Pre);
}


void pre_read_value(uint8_t* pressure_value)
{

    uint8_t data_buffer[7]={0x00};
    HAL_UART_Transmit(Pressure_UART_handler, Pre_read_Value, 8, 10);
    HAL_UART_Receive(Pressure_UART_handler, data_buffer, 7, 100);
    memcpy(pressure_value, &data_buffer[3], 2);

}

/*清零数值*/
void pre_clear(void)
{


    if (pre_send_reback(Pre_clear)!=Pre_OK)
    {
        Send_Error_to_PC(message_Packet_Pre, 0x03);
    }
}


