#ifndef FILE_CONF_H
#define FILE_CONF_H

#endif //FILE_CONF_H

typedef enum
{
    FILE_CONF_OK = 0,
    FILE_CONF_ERROR = 1,
}File_Conf_Status_EN;

typedef enum
{
    FILE_Write_OK = 0,
    FILE_Write_ERROR = 1,
}File_Write_Status_EN;


void SD_files_Init(void);
void SD_files_mount(void);
void SD_files_New(char* file_name);
void SD_files_Write(char* file_Details, UINT Details_Size);
void SD_files_Close(void);
