#include "ff.h"
#include "message_define.h"
#include "file_conf.h"

#include <stdio.h>
#include <string.h>

/*声明文件状态*/
File_Conf_Status_EN File_Conf_Status = FILE_CONF_OK;
File_Write_Status_EN File_Write_Status = FILE_Write_OK;

/*新建FatFs对象*/
FATFS *fs;//新建文件系统对象
FIL *file;  //新建文件对象


/*初始化文件系统*/
void SD_files_Init(void)
{
    /*分配内存*/
    fs = malloc(sizeof (FATFS));
    file = malloc (sizeof (FIL));
}

/*挂载驱动器*/
void SD_files_mount(void)
{
    /*再次声明文件状态*/
    File_Conf_Status = FILE_CONF_OK;
    File_Write_Status = FILE_Write_OK;

    /*挂载驱动器*/
    if (f_mount(fs, "", 1)!=FR_OK)
    {
        Send_Warning_to_PC(FatFs, 0x01);
        File_Conf_Status = FILE_CONF_ERROR;
    } else {
        Send_Init_Success_to_PC(FatFs);
    }
}

/*新建文件*/
void SD_files_New(char* file_name)
{
    /*判断文件状态*/
    if (File_Conf_Status == FILE_CONF_ERROR)
    {
        return;
    }

    /*新建或覆盖文件并打开*/
    if (f_open(file, file_name, FA_CREATE_ALWAYS | FA_WRITE)!=FR_OK)
    {
        Send_Warning_to_PC(FatFs, 0x02);
        File_Conf_Status = FILE_CONF_ERROR;
    }
}

/*写入信息*/
void SD_files_Write(char* file_Details, unsigned int Details_Size)
{
    /*判断文件状态*/
    if (File_Conf_Status == FILE_CONF_ERROR)
    {
        return;
    }

    /*移至文件末尾*/
    if (f_lseek(file, f_size(file))!=FR_OK)
    {
        if (File_Write_Status == FILE_Write_OK)
        {
            Send_Warning_to_PC(FatFs, 0x03);
            File_Write_Status = FILE_Write_ERROR;
            return;
        }
    };

    /*添加换行符*/
    char Details[Details_Size+3] = {};
    sprintf(Details, "%s\r\n", file_Details);

    /*写入信息*/
    if (f_write(file, Details, strlen(Details), NULL)!=FR_OK)
    {
        if (File_Write_Status == FILE_Write_OK)
        {
            Send_Warning_to_PC(FatFs, 0x04);
            File_Write_Status = FILE_Write_ERROR;
            return;
        }
    }

    /*立即写入*/
    if (f_sync(file)!=FR_OK)
    {
        if (File_Write_Status == FILE_Write_OK)
        {
            Send_Warning_to_PC(FatFs, 0x05);
            File_Write_Status = FILE_Write_ERROR;
        }
    }
}

/*卸载驱动器*/
void SD_files_Close()
{
    if (f_close(file)!=FR_OK)
    {
        Send_Warning_to_PC(FatFs, 0x06);
        File_Conf_Status = FILE_CONF_OK;
        File_Write_Status = FILE_Write_OK;
    }
}