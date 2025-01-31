
#ifndef MESSAGE_DEFINE_H
#define MESSAGE_DEFINE_H
#include <stdint.h>

#endif //MESSAGE_DEFINE_H



typedef enum
{
    FatFs = 0x01,
    Pressure = 0x02,
    Temperature = 0x03
}SourceType_EN;


typedef enum
{
    Error = 0x01,
    Init = 0x02,
    Message = 0x03,
    Value = 0x04
}Type_EN;


typedef struct MESSAGE_ST
{
    SourceType_EN SourceType;
    Type_EN Type;
}MESSAGE_Packet;






void Send_Error_to_PC(MESSAGE_Packet message_Packet, uint8_t ErrorCode);
void Send_Warning_to_PC(MESSAGE_Packet message_Packet, uint8_t WarningCode);
void Send_Init_Success_to_PC(MESSAGE_Packet message_Packet);
void Send_Value_to_PC(MESSAGE_Packet message_Packet, uint8_t message_Value);
