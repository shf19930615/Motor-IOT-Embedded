/*
 * Copyright (c) 2016-xxxx, Song HaiFeng(304920508@qq.com)
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date        	Author   		Notes
 * 2019-09-01	Song HaiFeng	the first version
 */
#include "Motor.h"

/* 基本 */
void MotorInit(Motor *motor, MotorHard *hard, void *usrData)
{
	motor->a = 0;
	motor->v = 0;
	motor->s = 0;
	motor->usrData = usrData;
	motor->hard = hard;

	motor->queue = NULL;
	motor->head = 0;
	motor->tail = 0;
	motor->size = 0;
	motor->maxSize = 0;

	motor->hard->Init(motor->hard);
}

void MotorSetSpeed(Motor *motor, int32_t v)
{
	motor->v = v;
	motor->hard->SetSpeed(motor->hard, motor->v);
}

void MotorSetPos(Motor *motor, int32_t s)
{
	motor->s = s;
}

/* 执行动作队列 */
void MotorRunAction(Motor *motor, uint32_t dt)
{
	bool complete = false;

	MotorAction* action = MotorActionFront(motor);
	if (!action)
	{
		return;
	}
	/* 更新位置 */
	// motor->s += motor->v * dt;
	/* 加速 */
	if (motor->v != action->v)
	{
		motor->v += action->a * dt;
		motor->hard->SetSpeed(motor->hard, motor->v);
	}

	if (action->type == ACTION_ACCELERATE && motor->v == action->v)
	{
		MotorPopAction(motor);
	}
	else if (action->type == ACTION_MOVE_TO || action->type == ACTION_MOVE_BY)
	{
		if (action->tick == 0)
		{
			if (action->type == ACTION_MOVE_BY)
			{
				action->s += motor->s;
			}
			/* 需要前进 */
			if (motor->s < action->s)
			{
				action->tick = 1;
			}
			/* 需要前进 */
			else
			{
				action->tick = 2;
			}
		}

		if ((action->tick == 1 && motor->s >= action->s)
			|| (action->tick == 2 && motor->s <= action->s))
		{
			MotorPopAction(motor);
			if (MotorActionEmpty(motor))
			{
				motor->hard->SetSpeed(motor->hard, 0);
			}
		}
	}
	else if(action->type == ACTION_RUN_TIME)
	{
		if (action->tick > dt)
		{
			action->tick -= dt;
		}
		else
		{
			MotorPopAction(motor);
			if (MotorActionEmpty(motor))
			{
				motor->hard->SetSpeed(motor->hard, 0);
			}
		}
	}
}

/* 动作队列 */
void MotorAddActionQueue(Motor *motor, MotorAction *action, uint32_t maxSize)
{
	motor->queue = action;
	motor->head = 0;
	motor->tail = 0;
	motor->size = 0;
	motor->maxSize = maxSize;
}

void MotorPushAction(Motor* motor, MotorAction action)
{
	if (motor->size >= motor->maxSize)
	{
		return;
	}
	motor->queue[motor->tail] = action;
	motor->size++;
	motor->tail++;
	if (motor->tail >= motor->maxSize)
	{
		motor->tail = 0;
	}
}

void MotorPopAction(Motor *motor)
{
	if (motor->size <= 0)
	{
		return;
	}
	motor->size--;
	motor->head++;
	if (motor->head >= motor->maxSize)
	{
		motor->head = 0;
	}
}

MotorAction* MotorActionFront(Motor* motor)
{
	if (motor->size <= 0)
	{
		return NULL;
	}
	return &(motor->queue[motor->head]);
}

bool MotorActionEmpty(Motor *motor)
{
	return motor->size;
}

void MotorActionClear(Motor *motor)
{
	motor->head = 0;
	motor->tail = 0;
	motor->size = 0;

	motor->hard->SetSpeed(motor->hard, 0);
}
