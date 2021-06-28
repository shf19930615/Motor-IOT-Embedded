/*
 * Copyright (c) 2016-xxxx, Song HaiFeng(304920508@qq.com)
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date        	Author   		Notes
 * 2019-09-01	Song HaiFeng	the first version
 */
#include <stdio.h>
#include "Motor.h"

/* 电机硬件初始化函数 */
static void Init(struct MotorHard* hard);
/* 电机硬件设置速度函数 */
static void SetSpeed(struct MotorHard* hard, int32_t speed);

static MotorHard motorHard;
static Motor motor;

void MotorNormalTest(void)
{
	/* 电机硬件实现 */
	MotorHardInit(&motorHard, Init, SetSpeed, NULL);
	/* 电机抽象绑定电机硬件实现 */
	MotorInit(&motor, &motorHard, NULL);

	/* 设置电机速度(单位由开发者自己定义) */
	MotorSetSpeed(&motor, 100);
}

static MotorAction motorAction[4];

void MotorActionTest(void)
{
	/* 电机硬件实现 */
	MotorHardInit(&motorHard, Init, SetSpeed, NULL);
	/* 电机抽象绑定电机硬件实现 */
	MotorInit(&motor, &motorHard, NULL);
	/* 增加动作队列 */
	MotorAddActionQueue(&motor, motorAction, sizeof(motorAction)/ sizeof(motorAction[0]));
	/* 加速度为50加速到100 以100的速度运行4000(单位由开发者自己定义) 加速度为-50减速到0 */
	MotorPushAction(&motor, ActionAccelerate(50, 100));
	MotorPushAction(&motor, ActionRunTime(50, 100, 4000));
	MotorPushAction(&motor, ActionAccelerate(-50, 0));

	for (;;)
	{
		uint32_t dt = 0;
		/* 计算运行间隔(单位由开发者自己定义) */
		// dt = xx - xy;
		MotorRunAction(&motor, dt);

		uint32_t pos = 0;
		/* 由其他反馈量获取位置(单位由开发者自己定义) */
		/* ACTION_MOVE_TO | ACTION_MOVE_BY 需要用到位置 */
		// pos = xx;
		MotorSetPos(&motor, pos);
	}
}
/* 电机硬件初始化函数 */
void Init(struct MotorHard* hard)
{
	/* 电机硬件初始化 */
}
/* 电机硬件设置速度函数 */
void SetSpeed(struct MotorHard* hard, int32_t speed)
{
	/* 停止 */
	if (speed == 0)
	{
		
	}
	/* 正转 */
	else if (speed > 0)
	{
		
	}
	/* 反转 */
	else if (speed < 0)
	{

	}
}