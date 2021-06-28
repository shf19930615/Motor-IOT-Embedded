/*
 * Copyright (c) 2016-xxxx, Song HaiFeng(304920508@qq.com)
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date        	Author   		Notes
 * 2019-09-01	Song HaiFeng	the first version
 */
#include "MotorHard.h"

/* 电机硬件初始化 */
void MotorHardInit(MotorHard *hard,
				   void (*Init)(MotorHard *hard),
				   void (*SetSpeed)(MotorHard *hard, int32_t speed),
				   void *usrData)
{
	hard->Init = Init;
	hard->SetSpeed = SetSpeed;
	hard->usrData = usrData;
}
