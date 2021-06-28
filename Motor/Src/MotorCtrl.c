#include "Motor.h"
#include "Debug.h"

BOOL MotorRunAction(Motor* motor)
{
	// <! front
	MotorAction* act = MotorActionFront(motor);
	if (act == NULL) {
		//MotorStop(motor);
		return 1;
	}
	
	if (act->first) {
		act->first = FALSE;
		act->lastTick = GET_MS_TICK();
		//motor->hard->Start(motor->hard);
		MotorActionFirst(motor, act);
	}
	
	BOOL result = FALSE;
	// <! acceleration
	if (motor->speed != act->speed) {
		
		TICK now = GET_MS_TICK();
		TICK dis = now - act->lastTick;
		act->lastTick = now;
		motor->speed += act->acceleration * dis;
		
		if (act->speedDir == DIR_NEGATIVE) {
			if (motor->speed <= act->speed) motor->speed = act->speed;
		}
		else if (act->speedDir == DIR_POSITIVE){
			if (motor->speed >= act->speed) motor->speed = act->speed;
		}
		
		MotorSetSpeed(motor, motor->speed);
	}		
	else {
		if (act->type == ACTION_ACCELERATE) result = TRUE;
	}
	
	if (act->type == ACTION_MOVE_TO || act->type == ACTION_MOVE_BY)
	{ 
		if (act->posDir == DIR_NEGATIVE) {
			//LOG("DIR_NEGATIVE[%d, %d]", motor->pos, act->pos);
			if (motor->pos <= act->pos) {
				result = TRUE;
				//LOG("ACTION_ACCELERATE");
			}
			else
			{
				if (act->speed > 0)
				{
					act->speed = -abs(act->speed);
					if (motor->speed < act->speed) {
						act->speedDir = DIR_POSITIVE;
						act->acceleration = abs(act->acceleration);
					}
					else if (motor->speed > act->speed) {
						act->speedDir = DIR_NEGATIVE;
						act->acceleration = -abs(act->acceleration);
					}
					else act->acceleration = 0;
				}
			}
		} 
		else if (act->posDir == DIR_POSITIVE) {
			//LOG("DIR_POSITIVE[%d, %d]", motor->pos, act->pos);
			if (motor->pos >= act->pos) 
			{
				result = TRUE;
				// LOG("ACTION_ACCELERATE2");
			}
			else
			{
				if (act->speed < 0)
				{
					act->speed = abs(act->speed);
					if (motor->speed < act->speed) {
						act->speedDir = DIR_POSITIVE;
						act->acceleration = abs(act->acceleration);
					}
					else if (motor->speed > act->speed) {
						act->speedDir = DIR_NEGATIVE;
						act->acceleration = -abs(act->acceleration);
					}
					else act->acceleration = 0;
				}
			}
		}
		//LOG("%d, %d", motor->pos, act->pos);
	}
	else if(act->type == ACTION_RUN_TIME) {
		TICK now = GET_MS_TICK();
		TICK dis = now - act->lastTick;
		act->lastTick = now;
		if (dis >= act->runTick) result = TRUE;
		else act->runTick -= dis;
	}
	
	if (result) {
		motor->runCount++;
		act->CallBack(act, motor);
		// <! pop
		MotorPopAction(motor);
	}
	
	return FALSE;
}

void MotorActionFirst(Motor* motor, MotorAction* action)
{
	SPEED speed;
	POS pos;
	
	speed = motor->speed;
	pos = motor->pos;
	
	if (speed < action->speed) {
		action->speedDir = DIR_POSITIVE;
		action->acceleration = abs(action->acceleration);
	}
	else if (speed > action->speed) {
		action->speedDir = DIR_NEGATIVE;
		action->acceleration = -abs(action->acceleration);
	}
	else action->acceleration = 0;
	
	if (action->type == ACTION_ACCELERATE) {
		action->pos += pos + (action->speed * action->speed - speed * speed) / (2 * action->acceleration * 1000);
	}
	else if (action->type == ACTION_MOVE_TO) {
		// NONE
		;
	}
	else if (action->type == ACTION_MOVE_BY) {
		action->pos += pos;
	}
	else if (action->type == ACTION_RUN_TIME) {
		POS posn;
		TICK tn = (action->speed - speed) / action->acceleration;
		if (tn < action->runTick) {
			posn = action->speed * action->runTick - action->acceleration * tn * tn / 2000000;
		}
		else {
			posn = speed * tn / 1000  + action->acceleration * tn * tn / 2000000;
		}
		action->pos = pos + posn;
	}
	
	if (pos < action->pos) {
		action->posDir = DIR_POSITIVE;
	}
	else if (pos > action->pos) {
		action->posDir = DIR_NEGATIVE;
	}
	else {
		action->posDir = DIR_POSITIVE;
	}
	
	LOG("[Motor[%s] Action: (%d, %d, %d, %d, %d, %d, %d)]", motor->name,
		action->type, action->acceleration, action->speed, action->pos,
		action->runTick, action->speedDir, action->posDir);
}

// <! action queue
BOOL MotorPushAction(Motor* motor, MotorAction* action)
{
	if (motor->count >= MOTOR_ACTION_MAX_SIZE) {
		WARNNING("[MOTOR: Action queue overflow.(count:%d >= max:%d)]", motor->count, MOTOR_ACTION_MAX_SIZE);
		return FALSE;
	}
	
	DIS_IRQ();
	// <! --------------------------------------------------
	
	motor->queue[motor->tail].type = action->type;
	motor->queue[motor->tail].acceleration = action->acceleration;
	motor->queue[motor->tail].speed = action->speed;
	motor->queue[motor->tail].pos = action->pos;
	motor->queue[motor->tail].first = action->first;
	motor->queue[motor->tail].lastTick = action->lastTick;
	motor->queue[motor->tail].speedDir = action->speedDir;
	motor->queue[motor->tail].posDir = action->posDir;
	motor->queue[motor->tail].runTick = action->runTick;
	motor->queue[motor->tail].CallBack = action->CallBack;

	motor->tail++;
	motor->count++;
	
	if (motor->tail >= MOTOR_ACTION_MAX_SIZE) {
		motor->tail = 0;
	}
	
	EN_IRQ();
	
	return TRUE;
}

