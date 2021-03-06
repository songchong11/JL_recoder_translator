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
#include "../../../../soundbox/include/task_manager/record/record.h"

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



void send_hand_shake_to_app(u8 mode)
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

	if (mode == BLE_MODE)
		app_send_user_data_do(NULL, data, sizeof(data));
	else
	 	spp_send_data_to_app(data, sizeof(data));
}

void send_dev_mac_to_app(u8 mode)
{
	u8 data[12];
	u8 crc = 0;

	data[0] = HEARD_H;
	data[1] = HEARD_L;
	data[2] = DEVICE_SEND_GET_MAC_ADDR;
	/*len == cmd + data*/
	data[3] = 0x07;
	/*data*/
	data[4] = 0x11;
	data[5] = 0x22;
	data[6] = 0x33;
	data[7] = 0x44;
	data[8] = 0x55;
	data[9] = 0x66;
	/*crc = cmd + len + data*/
	for (int i = 2; i < 10; i++)
		crc += data[i];

	data[10] = crc;
	data[11] = TAIL;
	//收发测试，自动发送收到的数据;for test
	if (mode == BLE_MODE)
		app_send_user_data_do(NULL, data, sizeof(data));
	else
	 	spp_send_data_to_app(data, sizeof(data));
}


void send_left_key_press_to_app(u8 mode)
{
	u8 data[7];

	data[0] = HEARD_H;
	data[1] = HEARD_L;
	data[2] = DEVICE_SEND_KEY_LEFT_LONG_DOWN;
	data[3] = 0x02;/*len == cmd + data*/
	data[4] = 0x00;
	data[5] = data[2] + data[3] + data[4];/*crc = cmd + len + data*/
	data[6] = TAIL;
	printf("send_left_key_press_to_app \n");
	//收发测试，自动发送收到的数据;for test
	if (mode == BLE_MODE)
		app_send_user_data_do(NULL, data, sizeof(data));
	else
	 	spp_send_data_to_app(data, sizeof(data));
}

void send_left_key_up_to_app(u8 mode)
{
	u8 data[7];

	data[0] = HEARD_H;
	data[1] = HEARD_L;
	data[2] = DEVICE_SEND_KEY_LEFT_LONG_UP;
	data[3] = 0x02;/*len == cmd + data*/
	data[4] = 0x00;
	data[5] = data[2] + data[3] + data[4];/*crc = cmd + len + data*/
	data[6] = TAIL;
	printf("send_left_key_up_to_app \n");
	//收发测试，自动发送收到的数据;for test
	if (mode == BLE_MODE)
		app_send_user_data_do(NULL, data, sizeof(data));
	else
	 	spp_send_data_to_app(data, sizeof(data));
}


void send_right_key_press_to_app(u8 mode)
{
	u8 data[7];

	data[0] = HEARD_H;
	data[1] = HEARD_L;
	data[2] = DEVICE_SEND_KEY_RIGHT_LONG_DOWN;
	data[3] = 0x02;/*len == cmd + data*/
	data[4] = 0x00;
	data[5] = data[2] + data[3] + data[4];/*crc = cmd + len + data*/
	data[6] = TAIL;
	printf("send_right_key_press_to_app \n");
	//收发测试，自动发送收到的数据;for test
	if (mode == BLE_MODE)
		app_send_user_data_do(NULL, data, sizeof(data));
	else
	 	spp_send_data_to_app(data, sizeof(data));
}

void send_right_key_up_to_app(u8 mode)
{
	u8 data[7];

	data[0] = HEARD_H;
	data[1] = HEARD_L;
	data[2] = DEVICE_SEND_KEY_RIGHT_LONG_UP;
	data[3] = 0x02;/*len == cmd + data*/
	data[4] = 0x00;
	data[5] = data[2] + data[3] + data[4];/*crc = cmd + len + data*/
	data[6] = TAIL;
	printf("send_right_key_up_to_app \n");
	//收发测试，自动发送收到的数据;for test
	if (mode == BLE_MODE)
		app_send_user_data_do(NULL, data, sizeof(data));
	else
	 	spp_send_data_to_app(data, sizeof(data));
}


void send_request_send_voice_data_to_app(u8 mode)
{
	u8 data[7];

	data[0] = HEARD_H;
	data[1] = HEARD_L;
	data[2] = DEVICE_SEND_REQUEST_SEND_VOICE_DATA;
	data[3] = 0x02;/*len == cmd + data*/
	data[4] = 0x00;
	data[5] = data[2] + data[3] + data[4];/*crc = cmd + len + data*/
	data[6] = TAIL;
	printf("send_request_send_voice_data_to_app \n");
	//收发测试，自动发送收到的数据;for test
	if (mode == BLE_MODE)
		app_send_user_data_do(NULL, data, sizeof(data));
	else
	 	spp_send_data_to_app(data, sizeof(data));
}


void device_send_ack_stop_voice_data_to_app(u8 mode)
{
	u8 data[7];

	data[0] = HEARD_H;
	data[1] = HEARD_L;
	data[2] = DEVICE_SEND_STOP_RECODER;
	data[3] = 0x02;/*len == cmd + data*/
	data[4] = 0x00;
	data[5] = data[2] + data[3] + data[4];/*crc = cmd + len + data*/
	data[6] = TAIL;
	printf("device_send_ack_stop_voice_data_to_app \n");
	//收发测试，自动发送收到的数据;for test
	if (mode == BLE_MODE)
		app_send_user_data_do(NULL, data, sizeof(data));
	else
	 	spp_send_data_to_app(data, sizeof(data));
}

void send_play_prev_to_app(u8 mode)
{
	u8 data[7];

	data[0] = HEARD_H;
	data[1] = HEARD_L;
	data[2] = DEVICE_SEND_PLAY_PREVIOUS;
	data[3] = 0x02;/*len == cmd + data*/
	data[4] = 0x00;
	data[5] = data[2] + data[3] + data[4];/*crc = cmd + len + data*/
	data[6] = TAIL;
	printf("send_play_prev_to_app \n");
	//收发测试，自动发送收到的数据;for test
	if (mode == BLE_MODE)
		app_send_user_data_do(NULL, data, sizeof(data));
	else
	 	spp_send_data_to_app(data, sizeof(data));
}

void send_play_next_to_app(u8 mode)
{
	u8 data[7];

	data[0] = HEARD_H;
	data[1] = HEARD_L;
	data[2] = DEVICE_SEND_PLAY_NEXT;
	data[3] = 0x02;/*len == cmd + data*/
	data[4] = 0x00;
	data[5] = data[2] + data[3] + data[4];/*crc = cmd + len + data*/
	data[6] = TAIL;
	printf("send_play_next_to_app \n");
	//收发测试，自动发送收到的数据;for test
	if (mode == BLE_MODE)
		app_send_user_data_do(NULL, data, sizeof(data));
	else
	 	spp_send_data_to_app(data, sizeof(data));
}

void send_battery_precent_to_app(u8 mode)
{
	u8 data[7];

	u8 precent = get_vbat_percent();

	data[0] = HEARD_H;
	data[1] = HEARD_L;
	data[2] = DEVICE_SEND_GET_BAT_LEVEL;
	data[3] = 0x02;/*len == cmd + data*/
	data[4] = precent;
	data[5] = data[2] + data[3] + data[4];/*crc = cmd + len + data*/
	data[6] = TAIL;
	printf("send_battery_precent_to_app %d \n", precent);
	//收发测试，自动发送收到的数据;for test
	if (mode == BLE_MODE)
		app_send_user_data_do(NULL, data, sizeof(data));
	else
	 	spp_send_data_to_app(data, sizeof(data));
}


void send_dev_info_to_app(u8         mode)
{
	u8 data[23];
	u8 crc = 0;

	data[0] = HEARD_H;
	data[1] = HEARD_L;
	data[2] = DEVICE_SEND_GET_DEV_INFO;
	/*len == cmd + data*/
	data[3] = 19;
	/*data*/
	data[4] = 'h';
	data[5] = 'a';
	data[6] = 'r';
	data[7] = 'd';
	data[8] = ':';
	data[9] = '1';
	data[10] = '.';
	data[11] = '0';
	data[12] = 's';
	data[13] = 'o';
	data[14] = 'f';
	data[15] = 't';
	data[16] = ':';
	data[17] = '1';
	data[18] = '.';
	data[19] = '0';
	data[20] = '.';
	data[21] = '1';
	/*crc = cmd + len + data*/
	for (int i = 2; i < 22; i++)
		crc += data[i];

	data[22] = crc;
	data[23] = TAIL;
	//收发测试，自动发送收到的数据;for test
	if (mode == BLE_MODE)
		app_send_user_data_do(NULL, data, sizeof(data));
	else
	 	spp_send_data_to_app(data, sizeof(data));
}


int connect_timer = 0;

//Parse the data received from the app
void receive_data_form_app(u8 *data, u8 len, u8 mode)
{

	printf("**************** mode ****************** %x\n", mode);

	if (data[0] != HEARD_H || data[1] != HEARD_L || data[len - 1] != TAIL)
		return;

	if (!check_crc(&data[2], data[3] + 1, data[len - 2])) {
		printf("crc check err\n");
		return;
	}

	u8 cmd = data[2];

	switch(cmd) {

	case DEVICE_RECEIVE_HAND_SHAKE:
		// TODO: del the timer
		printf("--------------receive app hand shake\n");
		if (connect_timer) {
			sys_timeout_del(connect_timer);
			connect_timer = 0;
		}
		break;

	case DEVICE_RECEIVE_GET_DEV_INFO:
		// TODO:send dev info to app
		printf("--------------app get dev info\n");
		send_dev_info_to_app(mode);
		break;

	case DEVICE_RECEIVE_GET_MAC_ADDR:
		// TODO:send dev mac to app
		printf("--------------app get dev mac addr\n");
		send_dev_mac_to_app(mode);
		break;

	case DEVICE_RECEIVE_SET_BLE_NAME:
		// TODO:set ble name
		printf("--------------app set ble name\n");
		//int syscfg_read(CFG_USER_DEFINE_APP_SET_NAME, void *buf, u16 len);
		//int syscfg_write(CFG_USER_DEFINE_APP_SET_NAME, void *buf, u16 len);
		break;

	case DEVICE_RECEIVE_GET_BAT_LEVEL:
		// TODO:get   bat level 
		printf("--------------app get bat level\n");
		send_battery_precent_to_app(mode);
		break;

	case DEVICE_RECEIVE_SET_COUNTRY_INFO:
		// TODO:set country info
		printf("--------------app set country info\n");
		//int syscfg_read(CFG_USER_DEFINE_APP_SET_COUNTRY_INFO, void *buf, u16 len);
		//int syscfg_write(CFG_USER_DEFINE_APP_SET_COUNTRY_INFO, void *buf, u16 len);
		break;

	case DEVICE_RECEIVE_GET_COUNTRY_INFO:
		// TODO:get country info
		printf("--------------app get country info\n");
		break;

	case DEVICE_RECEIVE_SET_QUDAO_INFO:
		// TODO:set qudao    info
		printf("--------------app set qudao info\n");
		//int syscfg_read(CFG_USER_DEFINE_APP_SET_QUDAO_INFO, void *buf, u16 len);
		//int syscfg_write(CFG_USER_DEFINE_APP_SET_QUDAO_INFO, void *buf, u16 len);
		break;

	case DEVICE_RECEIVE_GET_QUDAO_INFO:
		// TODO:set qudao info
		printf("--------------app get qudao info\n");
		break;

	case DEVICE_RECEIVE_KEY_LEFT_LONG_DOWN:
		printf("--------------app ack key left long down\n");
		printf("--------------request send voice\n");
		send_request_send_voice_data_to_app(mode);
		break;

	case DEVICE_RECEIVE_KEY_LEFT_LONG_UP:
		printf("--------------app ack key left long up\n");
		break;

	case DEVICE_RECEIVE_KEY_RIGHT_LONG_DOWN:
		printf("--------------app ack key right long down\n");
		printf("--------------request send voice\n");
		send_request_send_voice_data_to_app(mode);
		break;

	case DEVICE_RECEIVE_KEY_RIGHT_LONG_UP:
		printf("app ack key right long up\n");
		break;

	case DEVICE_RECEIVE_REQUEST_SEND_VOICE_DATA:
		printf("****************************************\n");
		printf("--------------app ack request send voice\n");
		extern void start_record_by_app_cmd(void);
		start_record_by_app_cmd();
		break;

	case DEVICE_RECEIVE_STOP_RECODER:
		printf("--------------app stop recoder and stop send voice\n");
		extern void stop_record_by_app_cmd(void);
		stop_record_by_app_cmd();
		device_send_ack_stop_voice_data_to_app(mode);
		break;

	case DEVICE_RECEIVE_PLAY_PREVIOUS:
		printf("--------------app ack play previous\n");
		break;

	case DEVICE_RECEIVE_PLAY_NEXT:
		printf("--------------app ack play next\n");
		break;

	}

}


