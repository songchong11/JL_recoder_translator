#ifndef __RECORD_H__
#define __RECORD_H__

#include "generic/typedef.h"
#include "app_config.h"


extern void record_file_replay_lev2();
extern void record_file_replay_lev1();
extern void record_file_start(void);
extern void rec_userinput_del();

extern void start_record_by_app_cmd(void);
extern void stop_record_by_app_cmd(void);

#endif//__RECORD_H__
