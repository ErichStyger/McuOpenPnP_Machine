
#include "platform.h"
#include "application.h"
#if PL_MOTOR_EN
	#include "motor.h"
#endif
#if PL_COMM_EN
	#include "comm.h"
#endif

/*
 * starts Communication and Encoder tasks
 */
void PL_Init(void) {
#if PL_COMM_EN
	SHELL_Init();
#endif
#if PL_MOTOR_EN
	MOT_Init();
#endif
	APP_Init();
	Cpu_EnableInt();
}

