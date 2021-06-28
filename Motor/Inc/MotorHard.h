/*
 * Copyright (c) 2016-xxxx, Song HaiFeng(304920508@qq.com)
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date        	Author   		Notes
 * 2019-09-01	Song HaiFeng	the first version
 */
#ifndef MOTOR_HARD_H
#define MOTOR_HARD_H

#include "stdbool.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* the abstract interface of motor hardware */
/* 电机硬件抽象 */
typedef struct MotorHard
{
	/* 电机硬件实现 */
	void (*Init)(struct MotorHard *hard);
	void (*SetSpeed)(struct MotorHard *hard, int32_t speed);
	/* 用户数据 */
	void *usrData;

} MotorHard;

/* 电机硬件初始化 */
void MotorHardInit(MotorHard *hard,
					void (*Init)(MotorHard *hard),
					void (*SetSpeed)(MotorHard *hard, int32_t speed),
					void *usrData);

#ifdef __cplusplus
}
#endif

#endif
