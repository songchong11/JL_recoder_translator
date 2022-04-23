/*********************************************************************************************
    *   Filename        : le_server_module.c

    *   Description     :

    *   Author          :

    *   Email           : zh-jieli.com

    *   Last modifiled  : 2017-01-17 11:14

    *   Copyright:(c)JIELI  2011-2016  @ , All Rights Reserved.
*********************************************************************************************/

// *****************************************************************************
/* EXAMPLE_START(le_counter): LE Peripheral - Heartbeat Counter over GATT
 *
 * @text All newer operating systems provide GATT Client functionality.
 * The LE Counter examples demonstrates how to specify a minimal GATT Database
 * with a custom GATT Service and a custom Characteristic that sends periodic
 * notifications.
 */
// *****************************************************************************
#include "system/includes.h"

#include "translator_procotol.h"

unsigned char check_crc(u8 *data, u8 len, u8 crc)
{
	u8 sum;

	for (int i = 0; i < len; i++) {
		sum += data[i];
	}

	if (sum == crc)
		return 1;
	else
		return 0;
}

//Parse the data received from the app
void receive_data_form_app(u8 *data, u8 len)
{

	if (data[0] != HEARD_H || data[1] != HEARD_L || data[len - 1] != TAIL)
		return;

	if (!check_crc(&data[2], data[3] + 1, data[len - 2])) {
		printf("crc check err\n");
		return;
	}

	u8 cmd = data[2];

	switch(cmd) {

	case DEVICE_RECEIVE_HAND_SHAKE:
		printf("receive app hand shake\n");
		break;

	}

}


void send_hand_shake_to_app(void)
{
	u8 data[7];

	data[0] = HEARD_H;
	data[1] = HEARD_L;
	data[2] = DEVICE_SEND_HAND_SHAKE;
	data[3] = 0x02;/*len == cmd + data*/
	data[4] = 0x00;
	data[5] = data[2] + data[3] + data[4];/*crc = cmd + len + data*/
	data[6] = TAIL;
	printf("app_send_user_data_do \n");
	//收发测试，自动发送收到的数据;for test
	app_send_user_data_do(NULL, data, sizeof(data));
}

