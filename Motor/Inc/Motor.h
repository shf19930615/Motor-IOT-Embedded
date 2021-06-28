/*
 * Copyright (c) 2016-xxxx, Song HaiFeng(304920508@qq.com)
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date        	Author   		Notes
 * 2019-09-01	Song HaiFeng	the first version
 */
#ifndef MOTOR_H
#define MOTOR_H

#include "stdbool.h"
#include "stdint.h"
#include "MotorHard.h"
#include "MotorAction.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct Motor
{
	/* basic attributes */
	/* 基本属性 */
	int32_t a;
	int32_t v;
	int32_t s;
	/* 用户数据 */
	void *usrData;
	/* 硬件实现 */
	MotorHard *hard;
	/* 动作队列 */
	MotorAction *queue;
	uint8_t head;
	uint8_t tail;
	uint8_t size;
	uint8_t maxSize;
	/* 电机队列 */
	struct Motor* next;

} Motor;

/* 基本 */
void MotorInit(Motor *motor, MotorHard *hard, void *usrData);
void MotorSetSpeed(Motor *motor, int32_t v);
void MotorSetPos(Motor *motor, int32_t s);
/* 执行动作队列 */
void MotorRunAction(Motor *motor, uint32_t dt);
/* 动作队列 */
void MotorAddActionQueue(Motor *motor, MotorAction *action, uint32_t maxSize);
void MotorPushAction(Motor *motor, MotorAction action);
void MotorPopAction(Motor *motor);
MotorAction* MotorActionFront(Motor* motor);
bool MotorActionEmpty(Motor *motor);
void MotorActionClear(Motor *motor);

#ifdef __cplusplus
}
#endif

#endif
