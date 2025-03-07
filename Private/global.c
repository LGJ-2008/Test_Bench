#include "global.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include "pre_sen.h"
#include "temprature.h"
#include "file_conf.h"
#include "usbd_cdc_if.h"

extern int ms_counter;
extern int64_t time_ms;
extern char time_c[15];



uint8_t pressure_value[2]={0x00};
char file_Details[30];
char send_Details[30];

int pre_value;
int temp_value;

float pre_value2;

bool Read_States = false;


void convert_unix_to_beijing_time(time_t timestamp, uint8_t* time_c, size_t buffer_size) {
	timestamp += 8 * 3600;  // 手动添加 8 小时偏移
	struct tm *timeinfo = gmtime(&timestamp);  // 解析为 UTC+8 时间

	// 使用 sprintf 格式化字符串，并存入 uint8_t 数组
	snprintf((char *)time_c, buffer_size, "%d%02d%02d%02d%02d%02d",
			 timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
			 timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}


void Start_Read() {
	char file_name[20];
	convert_unix_to_beijing_time(time_ms/1000, time_c, sizeof(time_c));
	sprintf(file_name, "%s.csv",time_c);
	SD_files_New(file_name);
	char tittle[] = "time,ms,pre,temp";
	SD_files_Write(tittle, strlen(tittle));
}


void Read_value() {

		pre_read_value(pressure_value);
		pre_value = pressure_value[0]<<8 | pressure_value[1];
		temp_value = temp_receive();
		if (pre_value > 60000){
			pre_value = 0;
		}
		pre_value2 = (float)pre_value/10;

		convert_unix_to_beijing_time(time_ms/1000, time_c, sizeof(time_c));
		sprintf(file_Details, "%s,%d,%.1f,%d", time_c, ms_counter, pre_value2, temp_value);
		SD_files_Write(file_Details, strlen(file_Details));
		Read_States = true;
}


void Send_Value() {
	if (Read_States == true) {
		sprintf(send_Details, "%s,%d,%.1f,%d\n", time_c, ms_counter, pre_value2, temp_value);
		CDC_Transmit_FS(send_Details, strlen(send_Details));
		Read_States = false;
		return;
	}

	pre_read_value(pressure_value);
	pre_value = pressure_value[0]<<8 | pressure_value[1];
	temp_value = temp_receive();
	if (pre_value > 60000){
		pre_value = 0;
	}
	pre_value2 = (float)pre_value/10;

	convert_unix_to_beijing_time(time_ms/1000, time_c, sizeof(time_c));

	sprintf(send_Details, "%s,%d,%.1f,%d\n", time_c, ms_counter, pre_value2, temp_value);
	CDC_Transmit_FS(send_Details, strlen(send_Details));
	Read_States = false;
}


void finishing_Read() {
	SD_files_Close();
}