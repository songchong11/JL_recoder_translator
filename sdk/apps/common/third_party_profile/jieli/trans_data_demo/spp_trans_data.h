#ifndef __SPP_TRANS_DATA_H__
#define __SPP_TRANS_DATA_H__

#include "typedef.h"
#include "bt_common.h"

void transport_spp_init(void);
extern void spp_send_data_to_app(u8 *data, u8 len);

#endif//__RCSP_SPP_USER_H__
