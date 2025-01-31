#include "message_define.h"

#include <stdint.h>

#include "usbd_cdc_if.h"







/*传输错误到PC*/
void Send_Error_to_PC(MESSAGE_Packet message_Packet, uint8_t ErrorCode)
{
    uint8_t message[3]={0x00};

    message_Packet.Type = Error;

    message[0] = message_Packet.SourceType;
    message[1]=message_Packet.Type;
    message[2] = ErrorCode;


    CDC_Transmit_FS(message, 3);

    while (1);
}

/*传输警告到PC*/
void Send_Warning_to_PC(MESSAGE_Packet message_Packet, uint8_t WarningCode)
{
    uint8_t message[3]={0x00};

    message_Packet.Type = Error;

    message[0] = message_Packet.SourceType;
    message[1]=message_Packet.Type;
    message[2] = WarningCode;


    CDC_Transmit_FS(message, 3);

}

/*传输初始化成功到PC*/
void Send_Init_Success_to_PC(MESSAGE_Packet message_Packet)
{
    uint8_t message[2]={0x00};

    message_Packet.Type = Init;

    message[0] = message_Packet.SourceType;
    message[1]=message_Packet.Type;



    CDC_Transmit_FS(message, 2);
}

/*传输值到PC*/
void Send_Value_to_PC(MESSAGE_Packet message_Packet, uint8_t* message_Value)
{
    uint8_t message[10]={0x00};

    message_Packet.Type = Value;

    message[0] = message_Packet.SourceType;
    message[1]=message_Packet.Type;

    memcpy(&message[2], message_Value, sizeof(message_Value));

    CDC_Transmit_FS(message, sizeof(message));
}
