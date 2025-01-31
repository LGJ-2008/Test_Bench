#ifndef PRE_SEN_H
#define PRE_SEN_H

#include <stdint.h>
#include "port_define.h"

#endif //PRE_SEN_H

typedef enum
{
    Pre_OK,
    Pre_Fail,

}Pre_receive;



Pre_receive pre_send_reback(uint8_t* data);

void pre_init(void);
void pre_get_value(void);