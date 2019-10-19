#include "platform.h"
#include "UTIL1.h"
#include "motor.h"
#include "wait1.h"
#include "step.h"
#include "led1.h"
#include "comm.h"
#include "cls1.h"
#include "MotEn.h"
#include "MotSleep.h"
#include "FRTOS1.h"
#include "MS1.h"
#include "MS2.h"
#include "Dir.h"
#include "WAIT1.h"
#include "platform.h"
#include "stdlib.h"

static MOT_state_t MOTstate;
static TaskHandle_t MotTaskHandle;

/* ===================================================================
 * - TASK NOTIFICATION
 * -------------------------------------------------------------------
 * the motor task can be notified as followed:
 *
 * MOT_START_CONTINUOUS:	starts the continuous dispensing mode
 * MOT_STOP_CONTINUOUS:		stops the continuous dispensing mode
 * MOT_SINGLE_DISP:			dispense a predefined amount
 *
 */

#define MOT_START_CONTINUOUS (1<<0)
#define MOT_STOP_CONTINUOUS (1<<1)
#define MOT_SINGLE_DISP (1<<2)

void MOT_StartContinuous(void) {xTaskNotify(MotTaskHandle, MOT_START_CONTINUOUS, eSetBits);}
void MOT_StopContinuous(void) {xTaskNotify(MotTaskHandle, MOT_STOP_CONTINUOUS, eSetBits);}
void MOT_SingleDisp(void) {xTaskNotify(MotTaskHandle, MOT_SINGLE_DISP, eSetBits);}

/* ===================================================================
 * - MOTOR DRIVER
 * -------------------------------------------------------------------
 * the motor can be controlled with the following funtions
 *
 * MOT_enable:		enables the motor driver
 * MOT_disable:		disables the motor driver
 * MOT_sleep:		disables the motor drivers output
 * MOT_wakeup:		ennables the motor drivers output
 * MOT_setMS:		the microstepping gets set by parameter
 * MOT_setDir:		the motor direction gets set by parameter
 *
 */

static void MOT_enable(void){
	EnterCritical();
	MotEn_ClrVal();
	ExitCritical();
	//SHELL_SendString("motor enabled\r\n");
}

static void MOT_disable(void){
	EnterCritical();
	MotEn_SetVal();
	ExitCritical();
	//SHELL_SendString("motor disabled\r\n");
}

static void MOT_sleep(void){
	EnterCritical();
	MotSleep_ClrVal();
	ExitCritical();
	//SHELL_SendString("motor sleeping\r\n");
}

static void MOT_wakeup(void){
	EnterCritical();
	MotSleep_SetVal();
	ExitCritical();
	//SHELL_SendString("motor woke up\r\n");
}

static void MOT_setMS(MicroStep_t ms){
	switch (ms) {
		case FULLSTEP:
			EnterCritical();
			MS1_ClrVal();
			MS2_ClrVal();
			MOTstate.MS = FULLSTEP;
			ExitCritical();
			//SHELL_SendString("fullstep\r\n");	// for debugging
			break;
		case HALFSTEP:
			EnterCritical();
			MS1_SetVal();
			MS2_ClrVal();
			MOTstate.MS = HALFSTEP;
			ExitCritical();
			//SHELL_SendString("halfstep\r\n");	// for debugging
			break;
		case STEP_1_4:
			EnterCritical();
			MS1_ClrVal();
			MS2_SetVal();
			MOTstate.MS = STEP_1_4;
			ExitCritical();
			//SHELL_SendString("1/4 step\r\n");	// for debugging
			break;
		case STEP_1_8:
			EnterCritical();
			MS1_SetVal();
			MS2_SetVal();
			MOTstate.MS = STEP_1_8;
			ExitCritical();
			//SHELL_SendString("1/8 step\r\n");	// for debugging
			break;
		default:
			break;
	}
}

static void MOT_setDir(MotDirection_t dir){
	switch (dir) {
		case UP:
			EnterCritical();
			MOTstate.MOT_DIR = UP;
			Dir_ClrVal();
			ExitCritical();
			//SHELL_SendString("direction up\r\n");	// for debugging
			break;
		case DOWN:
			EnterCritical();
			MOTstate.MOT_DIR = DOWN;
			Dir_SetVal();
			ExitCritical();
			//SHELL_SendString("direction down\r\n");	// for debugging
			break;
		default:
			break;
	}
}


/* ===================================================================
 * - MOTOR STATE METHODES
 * -------------------------------------------------------------------
 * the states of the motor can be modified as followed:
 *
 * MOT_setSteps:		saves the given amount of steps
 * MOT_getSteps:		returns the remaining steps
 * MOT_stepDone:		subtracts one step from the remaining steps
 * MOT_setContinuous:	sets the continuous dispensing mode flag
 * MOT_clearContinuous:	cleares the continuous dispensing mode flag
 * MOT_startDisp:		wakes the motor driver up and sets dispensing
 * 						flag to start the dispensing process
 * MOT_stopDisp:		clears the dispensing flag and puts the
 * 						motor driver to sleep
 *
 */

static void MOT_setSteps(int32_t steps){
	EnterCritical();
	MOTstate.STEPS = steps;
	ExitCritical();
	//SHELL_SendString("steps set\r\n");	// for debugging
}

static int32_t MOT_getSteps(void){
	EnterCritical();
	uint32_t steps =  MOTstate.STEPS;
	ExitCritical();
	return steps;
}

static void MOT_stepDone(void){
	EnterCritical();
	MOTstate.STEPS --;
	ExitCritical();
}

static void MOT_setContinuous(void){
	EnterCritical();
	MOTstate.CONTINUOUS = TRUE;
	ExitCritical();
	// SHELL_SendString("continuous set\r\n");	// for debugging
	SHELL_SendString("ok\r\n");
}

static void MOT_clearContinuous(void){
	EnterCritical();
	MOTstate.CONTINUOUS = FALSE;
	ExitCritical();
	// SHELL_SendString("continuous clear\r\n");	// for debugging
}

static void MOT_startDisp(void){
	MOT_wakeup();
	EnterCritical();
	MOTstate.DISPENSING = TRUE;
	ExitCritical();
	//SHELL_SendString("dispensing started\r\n");	// for debugging
}

static void MOT_stopDisp(void){
	EnterCritical();
	MOTstate.DISPENSING = FALSE;
	ExitCritical();
	//SHELL_SendString("dispensing stopped\r\n");	// for debugging
	SHELL_SendString("ok\r\n");
	MOT_sleep();
}

/* ===================================================================
 * - SHELL
 * -------------------------------------------------------------------
 * Communication and Commands:
 *
 * MOT_PrintHelp		prints defined commands for communication
 * MOT_PrintStatus		prints the current status of the system
 * MOT_ParseCommand		comparse and executes commands
 *
 */

static void MOT_PrintHelp(const CLS1_StdIOType *io) {
	CLS1_SendHelpStr((unsigned char*)"motor", (unsigned char*)"Group of motor commands\r\n", io->stdOut);
	CLS1_SendHelpStr((unsigned char*)" help|status", (unsigned char*)"Shows motor help or status\r\n", io->stdOut);

	CLS1_SendStr("\r\n > CONFIGURATION:\r\n", io->stdOut);
	CLS1_SendHelpStr((unsigned char*)" enable | disable", (unsigned char*)"Enable or disable motor\r\n", io->stdOut);
	CLS1_SendHelpStr((unsigned char*)" sleep | wakeup", (unsigned char*)"Put motor to sleep or wakeup\r\n", io->stdOut);
	CLS1_SendHelpStr((unsigned char*)" dir up | down", (unsigned char*)"Change direction of motor\r\n", io->stdOut);
	CLS1_SendHelpStr((unsigned char*)" ms full|half|1/4|1/8", (unsigned char*)"Change microsteps\r\n", io->stdOut);
	CLS1_SendHelpStr((unsigned char*)" ref amount x", (unsigned char*)"Set dispensing amount reference\r\n", io->stdOut);
	CLS1_SendHelpStr((unsigned char*)" speed div x", (unsigned char*)"Set speed divider\r\n", io->stdOut);

	CLS1_SendStr("\r\n > DISPENSING:\r\n", io->stdOut);
	CLS1_SendHelpStr((unsigned char*)" start", (unsigned char*)"Start continuous dispensing mode\r\n", io->stdOut);
	CLS1_SendHelpStr((unsigned char*)" disp x", (unsigned char*)"Dispense x times the defined ref amount \r\n", io->stdOut);
	CLS1_SendHelpStr((unsigned char*)" stop", (unsigned char*)"Stop dispensing and reset remaining steps\r\n", io->stdOut);

	CLS1_SendStr("\r\n > G-CODES:\r\n", io->stdOut);
	CLS1_SendHelpStr((unsigned char*)" M870 x", (unsigned char*)"Dispense x times the defined ref amount \r\n", io->stdOut);
	CLS1_SendHelpStr((unsigned char*)" M871 start|stop", (unsigned char*)"Start and stop dispensing \r\n", io->stdOut);
	CLS1_SendHelpStr((unsigned char*)" M872 x", (unsigned char*)"Set dispensing amount reference\r\n", io->stdOut);
	CLS1_SendHelpStr((unsigned char*)" M873 x", (unsigned char*)"Set speed divider\r\n", io->stdOut);
}

static void MOT_PrintStatus(const CLS1_StdIOType *io) {
	CLS1_SendStr("\r\n", io->stdOut);

	if(MotEn_GetVal()){CLS1_SendStr(" motor disabled\r\n", io->stdOut);}
	else{CLS1_SendStr(" motor enabled\r\n", io->stdOut);}

	if(MotSleep_GetVal()){CLS1_SendStr(" motor awake\r\n", io->stdOut);}
	else{CLS1_SendStr(" motor sleeping\r\n", io->stdOut);}

	unsigned char buf[32];
	buf[0] = '\0';
	UTIL1_strcat(buf, sizeof(buf),(unsigned char*)(MOTstate.MOT_DIR==DOWN?" down\r\n":" up\r\n"));
	CLS1_SendStr(buf, io->stdOut);

	switch(MOTstate.MS) {
			  case FULLSTEP:
				  CLS1_SendStr(" full steps", io->stdOut); break;
			  case HALFSTEP:
				  CLS1_SendStr(" half steps", io->stdOut);  break;
			  case STEP_1_4:
				  CLS1_SendStr(" 1/4 steps", io->stdOut); break;
			  case STEP_1_8:
				  CLS1_SendStr(" 1/8 steps", io->stdOut);  break;
			  default:
				  CLS1_SendStr(" unknown", io->stdOut);
	}
	CLS1_SendStr((unsigned char*)"\r\n", io->stdOut);

	buf[0] = '\0';
		UTIL1_strcat(buf, sizeof(buf), " reference amount: ");
		UTIL1_strcatNum32sFormatted(buf, sizeof(buf), MOTstate.STEPS_REF_AMOUNT, ' ', 4);
		UTIL1_strcat(buf, sizeof(buf), "\r\n");
		CLS1_SendStr(buf, io->stdOut);

	buf[0] = '\0';
		UTIL1_strcat(buf, sizeof(buf), " speed divider: ");
		UTIL1_strcatNum32sFormatted(buf, sizeof(buf), MOTstate.SPEED_DIV, ' ', 4);
		UTIL1_strcat(buf, sizeof(buf), "\r\n");
		CLS1_SendStr(buf, io->stdOut);

	buf[0] = '\0';
	UTIL1_strcat(buf, sizeof(buf), " remaining steps: ");
	UTIL1_strcatNum32sFormatted(buf, sizeof(buf), MOTstate.STEPS, ' ', 8);
	UTIL1_strcat(buf, sizeof(buf), "\r\n");
	CLS1_SendStr(buf, io->stdOut);
}

uint8_t MOT_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io) {
  uint8_t res = ERR_OK;
  const unsigned char *p;

  if (UTIL1_strcmp((char*)cmd, (char*)CLS1_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, (char*)"motor help")==0) {
	  MOT_PrintHelp(io);
	  *handled = TRUE;
  } else if (UTIL1_strcmp((char*)cmd, (char*)CLS1_CMD_STATUS)==0 || UTIL1_strcmp((char*)cmd, (char*)"motor status")==0) {
	  MOT_PrintStatus(io);
	  *handled = TRUE;
  }

  // CONFIGURATION
  else if (UTIL1_strncmp((char*)cmd, (char*)"enable", sizeof("enable")-1)==0) {
	  MOT_enable();
	  *handled = TRUE;
  }	else if (UTIL1_strncmp((char*)cmd, (char*)"disable", sizeof("disable")-1)==0) {
	  MOT_disable();
	  *handled = TRUE;
  }	else if (UTIL1_strncmp((char*)cmd, (char*)"sleep", sizeof("sleep")-1)==0) {
	  MOT_sleep();
	  *handled = TRUE;
  }	else if (UTIL1_strncmp((char*)cmd, (char*)"wakeup", sizeof("wakeup")-1)==0) {
	  MOT_wakeup();
  	  *handled = TRUE;
  }	else if (UTIL1_strncmp((char*)cmd, (char*)"dir up", sizeof("dir up")-1)==0) {
	MOT_setDir(UP);
	*handled = TRUE;
  }	else if (UTIL1_strncmp((char*)cmd, (char*)"dir down", sizeof("dir down")-1)==0) {
	MOT_setDir(DOWN);
	*handled = TRUE;
  }	else if (UTIL1_strncmp((char*)cmd, (char*)"ms ", sizeof("ms ")-1)==0) {
	  p = cmd+sizeof("ms ")-1;
	  if (UTIL1_strncmp((char*)p, (char*)"full", sizeof("full")-1)==0) {
		  MOT_setMS(FULLSTEP);
	  }
	  else if (UTIL1_strncmp((char*)p, (char*)"half", sizeof("half")-1)==0) {
		  MOT_setMS(HALFSTEP);
	  }
	  else if (UTIL1_strncmp((char*)p, (char*)"1/4", sizeof("1/4")-1)==0) {
		  MOT_setMS(STEP_1_4);
	  }
	  else if (UTIL1_strncmp((char*)p, (char*)"1/8", sizeof("1/8")-1)==0) {
		  MOT_setMS(STEP_1_8);
	  }
	  else {
		CLS1_SendStr((unsigned char*)"Wrong argument\r\n", io->stdErr);
		res = ERR_FAILED;
	  }
	  *handled = TRUE;
  } else if (UTIL1_strncmp((char*)cmd, "ref amount ", sizeof("ref amount ")-1)==0){
  	  int32_t steps;
  	  p = cmd + sizeof("ref amount ")-1;
  	  res = UTIL1_xatoi(&p, &steps);
  	  if (res==ERR_OK) {
  		  MOTstate.STEPS_REF_AMOUNT = steps;
  	  }
  	  else {
  	  		CLS1_SendStr((unsigned char*)"Wrong argument\r\n", io->stdErr);
  	  		res = ERR_FAILED;
  	  }
  	  *handled = TRUE;
    } else if (UTIL1_strncmp((char*)cmd, "speed div ", sizeof("speed div ")-1)==0) {
  	  uint32_t speed_div;
  	  p = cmd + sizeof("speed div ")-1;
  	  res = UTIL1_xatoi(&p, &speed_div);
  	  if (res==ERR_OK) {
  		  if(speed_div > 0){
  			  MOTstate.SPEED_DIV = speed_div;
  		  }
  	  }
  	  else {
  	  		CLS1_SendStr((unsigned char*)"Wrong argument\r\n", io->stdErr);
  	  		res = ERR_FAILED;
  	  }
  	  *handled = TRUE;
    }

  // DISPENSING
  else if (UTIL1_strncmp((char*)cmd, (char*)"disp ", sizeof("disp ")-1)==0) {
	  int32_t amount;
	  p = cmd+sizeof("disp ")-1;
	  if (UTIL1_ScanDecimal32sNumber(&p, &amount)==ERR_OK) {
		   if(MOTstate.DISPENSING == FALSE){		// wait for machine to be idle
			   MOT_setSteps(abs(amount * MOTstate.STEPS_REF_AMOUNT * MOTstate.MS));
			   MOT_startDisp();
		   }
		   else{
				CLS1_SendStr((unsigned char*)"Motor is busy!!\r\n", io->stdErr);
				res = ERR_FAILED;
		   }
	  } else {
		CLS1_SendStr((unsigned char*)"Wrong argument\r\n", io->stdErr);
		res = ERR_FAILED;
	  }
	  *handled = TRUE;
  }	else if (UTIL1_strncmp((char*)cmd, (char*)"start", sizeof("start")-1)==0) {
	  if(MOTstate.DISPENSING == FALSE){				// wait for machine to be idle
		  MOT_setContinuous();
		  MOT_startDisp();
	  }
	  else{
			CLS1_SendStr((unsigned char*)"Motor is busy!!\r\n", io->stdErr);
			res = ERR_FAILED;
	  }
	  *handled = TRUE;
  } else if (UTIL1_strncmp((char*)cmd, (char*)"stop", sizeof("stop")-1)==0) {
	  MOT_clearContinuous();
	  MOT_stopDisp();
	  MOT_setSteps(0);
	  *handled = TRUE;
  }

  // G-CODES
  else if (UTIL1_strncmp((char*)cmd, "M870 ", sizeof("M870 ")-1)==0) {
   int32_t amount;
   p = cmd + sizeof("M870 ")-1;
   res = UTIL1_xatoi(&p, &amount);
   if (res==ERR_OK) {
	   if(MOTstate.DISPENSING == FALSE){		// wait for machine to be idle
		  if(amount < 0){
			  MOT_setDir(UP);
		  }
		  else{
			  MOT_setDir(DOWN);
		  }
		  MOT_setSteps(abs(amount * MOTstate.STEPS_REF_AMOUNT * MOTstate.MS));
		  MOT_startDisp();
	   }
	   else{
			CLS1_SendStr((unsigned char*)"Motor is busy!!\r\n", io->stdErr);
			res = ERR_FAILED;
	   }
   }
   else{
   		CLS1_SendStr((unsigned char*)"Wrong argument\r\n", io->stdErr);
		res = ERR_FAILED;
   }
   *handled = TRUE;
  } else if (UTIL1_strncmp((char*)cmd, "M871 ", sizeof("M871 ")-1)==0){
	  p = cmd + sizeof("M871 ")-1;
	  if(UTIL1_strncmp((char*)p, (char*)"start", sizeof("start")-1)==0){
		  if(MOTstate.DISPENSING == FALSE){				// wait for machine to be idle
		  		  MOT_setContinuous();
		  		  MOT_startDisp();
		  	  }
		  	  else{
		  			CLS1_SendStr((unsigned char*)"Motor is busy!!\r\n", io->stdErr);
		  			res = ERR_FAILED;
		  	  }
	  }
	  else if(UTIL1_strncmp((char*)p, (char*)"stop", sizeof("start")-1)==0){
		 MOT_clearContinuous();
		 MOT_stopDisp();
		 MOT_setSteps(0);
	  }
	  else{
		  CLS1_SendStr((unsigned char*)"Wrong argument\r\n", io->stdErr);
		  res = ERR_FAILED;
	  }
	  *handled = TRUE;
  } else if (UTIL1_strncmp((char*)cmd, "M872 ", sizeof("M872 ")-1)==0){
	  int32_t steps;
	  p = cmd + sizeof("M872 ")-1;
	  res = UTIL1_xatoi(&p, &steps);
	  if (res==ERR_OK) {
		  MOTstate.STEPS_REF_AMOUNT = steps;
		  SHELL_SendString("ok\r\n");
	  }
	  else {
	  		CLS1_SendStr((unsigned char*)"Wrong argument\r\n", io->stdErr);
	  		res = ERR_FAILED;
	  }
	  *handled = TRUE;
  } else if (UTIL1_strncmp((char*)cmd, "M873 ", sizeof("M873 ")-1)==0) {
	  uint32_t speed_div;
	  p = cmd + sizeof("M873 ")-1;
	  res = UTIL1_xatoi(&p, &speed_div);
	  if (res==ERR_OK) {
		  if(speed_div > 0){
			  MOTstate.SPEED_DIV = speed_div;
		  }
		  SHELL_SendString("ok\r\n");
	  }
	  else {
	  		CLS1_SendStr((unsigned char*)"Wrong argument\r\n", io->stdErr);
	  		res = ERR_FAILED;
	  }
	  *handled = TRUE;
  }
  return res;
}


/* ===================================================================
 * - Motor Task
 * -------------------------------------------------------------------
 *
 *
 */

static void MotTask(void *pv) {
	SHELL_SendString("Mot Task started!\r\n");
	uint32_t notifcationValue;
	for(;;){
	xTaskNotifyWait(0UL, MOT_START_CONTINUOUS|MOT_STOP_CONTINUOUS|MOT_SINGLE_DISP, &notifcationValue, 0);
	 /* check flags */
	if (notifcationValue&MOT_START_CONTINUOUS) {
		MOT_setSteps(0);
		MOT_setDir(DOWN);
		MOT_setContinuous();
		MOT_startDisp();
	}
	if (notifcationValue&MOT_STOP_CONTINUOUS) {
		MOT_clearContinuous();
	}
	if (notifcationValue&MOT_SINGLE_DISP) {
		MOT_setSteps(abs(SINGLE_DISP_AMOUNT * MOTstate.STEPS_REF_AMOUNT * MOTstate.MS));
		MOT_startDisp();
	}
	vTaskDelay(pdMS_TO_TICKS(10));
	} // for
}

/*
* ===================================================================
* - Interrupt
* -------------------------------------------------------------------
*     Description :
*         Every 500us a timer interrupt occurs and this function is
*         called. It verifies the state of the system and is
*         generating steps for the motor to turn. If no more steps
*         are required the dispensing mode gets stopped.
*     Parameters  : None
*     Returns     : Nothing
* ===================================================================
*/
void MOT_OnInterrupt(void){
	static uint32_t speed_cnt;
	if(MOTstate.DISPENSING){					// is motor dispensing?
		if(speed_cnt == MOTstate.SPEED_DIV){	// is it time for a step?
			speed_cnt = 0;						// reset speed_cnt
			if(MOTstate.CONTINUOUS){			// is motor in continuous mode?
				 Step_NegVal();					// do step
			}
			else if(MOT_getSteps() > 0){		// are steps remaining?
				Step_NegVal();					// do step
				if(Step_GetVal()){				// rising edge?
					MOT_stepDone();				// subtract step
				}
			}
			else{
				MOT_stopDisp();					// exit dispensing mode
			}
		}
		else{
			speed_cnt ++;						// count skipped interrupts
		}
	}
}


/*
* ===================================================================
* - Motor Init
* -------------------------------------------------------------------
*
* ===================================================================
*/

void MOT_Init(void){
	// Default Cofiguration Settings
	MOT_enable();			// enable motor driver
	MOT_sleep(); 			// set motor driver to sleep
	MOT_setMS(HALFSTEP);	// set halfstep mode
	MOT_setDir(DOWN);		// set direction down
	MOTstate.STEPS_REF_AMOUNT = 10;		// 1 disp = 10 Steps
	MOTstate.SPEED_DIV = 1;				// divide speed by 1 (Do not set zero)

	// Init Statemachine
	MOTstate.DISPENSING = FALSE;
	MOTstate.CONTINUOUS = FALSE;
	MOTstate.STEPS = 0;

	xTaskCreate(MotTask, "Motor", 900/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+2, &MotTaskHandle);
}

void MOT_Deinit(void){

}



