
#ifndef MESSAGE_DEFINE_H
#define MESSAGE_DEFINE_H
#include <stdint.h>

#endif //MESSAGE_DEFINE_H



typedef enum
{
    FatFs = 0x01,
    Pressure = 0x02,
    Temperature = 0x03,
    System = 0x04
}SourceType_EN;


typedef enum
{
    Error = 0x01,
    Warning = 0x02,
    Init = 0x03,
    Message = 0x04,
    Value = 0x05
}Type_EN;








void Send_Error_to_PC(SourceType_EN SourceType, uint8_t ErrorCode);
void Send_Warning_to_PC(SourceType_EN SourceType, uint8_t WarningCode);
void Send_Init_Success_to_PC(SourceType_EN SourceType);
void Send_Init_Success_to_PC_Value(SourceType_EN SourceType, uint8_t message_Value);
void Send_Message_to_PC(SourceType_EN SourceType, uint8_t message_Value);
void Send_Value_to_PC(SourceType_EN SourceType, uint8_t* message_Value);
