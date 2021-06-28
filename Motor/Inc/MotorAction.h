/*
 * Copyright (c) 2016-xxxx, Song HaiFeng(304920508@qq.com)
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date        	Author   		Notes
 * 2019-09-01	Song HaiFeng	the first version
 */
#ifndef MOTOR_ACTION_H
#define MOTOR_ACTION_H

#include "stdbool.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* 动作类型 */
typedef enum
{

	ACTION_NONE = 0,

	ACTION_ACCELERATE,
	ACTION_MOVE_TO,
	ACTION_MOVE_BY,
	ACTION_RUN_TIME,

} ActionType;

/* the abstract interface of motor motor action */
/* 电机动作抽象 */
typedef struct MotorAction
{
	ActionType type;
	int32_t a;
	int32_t v;
	int32_t s;
	uint32_t tick;
	/* 电机动作回调函数 */
	void (*CallBack)(struct MotorAction *action, void *usrData);
	/* 用户数据 */
	void *usrData;

} MotorAction;

/*创建电机动作(执行完不回调)*/
MotorAction ActionAccelerate(int32_t a, int32_t v);
MotorAction ActionMoveTo(int32_t a, int32_t v, int32_t pos);
MotorAction ActionMoveBy(int32_t a, int32_t v, int32_t pos);
MotorAction ActionRunTime(int32_t a, int32_t v, uint32_t tick);

/*创建电机动作(执行完回调)*/
MotorAction ActionAccelerateCB(int32_t a, int32_t v,
								void (*CallBack)(struct MotorAction *action, void *other));
MotorAction ActionMoveToCB(int32_t a, int32_t v, int32_t s,
							void (*CallBack)(struct MotorAction *action, void *other));
MotorAction ActionMoveByCB(int32_t a, int32_t v, int32_t s,
							void (*CallBack)(struct MotorAction *action, void *other));
MotorAction ActionRunTimeCB(int32_t a, int32_t v, uint32_t tick,
							void (*CallBack)(struct MotorAction *action, void *other));

#ifdef __cplusplus
}
#endif

#endif
