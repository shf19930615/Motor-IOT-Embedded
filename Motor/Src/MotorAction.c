/*
 * Copyright (c) 2016-xxxx, Song HaiFeng(304920508@qq.com)
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date        	Author   		Notes
 * 2019-09-01	Song HaiFeng	the first version
 */
#include "MotorAction.h"

MotorAction ActionAccelerate(int32_t a, int32_t v)
{
	return ActionAccelerateCB(a, v, NULL);
}

MotorAction ActionMoveTo(int32_t a, int32_t v, int32_t s)
{
	return ActionMoveToCB(a, v, s, NULL);
}

MotorAction ActionMoveBy(int32_t a, int32_t v, int32_t s)
{
	return ActionMoveByCB(a, v, s, NULL);
}

MotorAction ActionRunTime(int32_t a, int32_t v, uint32_t tick)
{
	return ActionRunTimeCB(a, v, tick, NULL);
}

MotorAction ActionAccelerateCB(int32_t a, int32_t v, void (*CallBack)(struct MotorAction *action, void *other))
{
	MotorAction action;

	action.type = ACTION_ACCELERATE;
	action.a = a;
	action.v = v;
	action.s = 0;
	action.tick = 0;
	action.CallBack = CallBack;

	return action;
}

MotorAction ActionMoveToCB(int32_t a, int32_t v, int32_t s, void (*CallBack)(struct MotorAction *action, void *other))
{
	MotorAction action;

	action.type = ACTION_MOVE_TO;
	action.a = a;
	action.v = v;
	action.s = s;
	action.tick = 0;
	action.CallBack = CallBack;

	return action;
}

MotorAction ActionMoveByCB(int32_t a, int32_t v, int32_t s,
						   void (*CallBack)(struct MotorAction *action, void *other))
{
	MotorAction action;

	action.type = ACTION_MOVE_BY;
	action.a = a;
	action.v = v;
	action.s = s;
	action.tick = 0;
	action.CallBack = CallBack;

	return action;
}

MotorAction ActionRunTimeCB(int32_t a, int32_t v, uint32_t tick,
							void (*CallBack)(struct MotorAction *action, void *other))
{
	MotorAction action;

	action.type = ACTION_RUN_TIME;
	action.a = a;
	action.v = v;
	action.s = 0;
	action.tick = tick;
	action.CallBack = CallBack;

	return action;
}
