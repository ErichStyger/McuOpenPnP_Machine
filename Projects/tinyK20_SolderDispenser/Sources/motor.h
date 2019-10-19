#ifndef motor_h
#define motor_h

#include "platform.h"
#include "CLS1.h"

#define SINGLE_DISP_AMOUNT (1) // define here the amount for a single disp command

typedef enum {
  UP,				// moving the solder paste backward -> avoid drool
  DOWN				// moving the solder paste forward -> dispensing
} MotDirection_t;

typedef enum {
  FULLSTEP = 1,		// 1 step equals the full stepangle
  HALFSTEP = 2,		// 1 step euqals the half of a stepangle
  STEP_1_4 = 4,		// 1 step equals the forth of a stepangle
  STEP_1_8 = 8		// 1 step equals the eighth of a stepangle
} MicroStep_t;

typedef struct {
	MicroStep_t MS;				// microstepping
	MotDirection_t MOT_DIR;		// direction
	bool DISPENSING;			// flag: is the motor busy with dispensing
	bool CONTINUOUS;			// flag: is the motor in continuous mode
	int32_t STEPS;				// amount of remaining steps
	int32_t STEPS_REF_AMOUNT;	// steps for specific amount of solder paste
	uint32_t SPEED_DIV;			// Speed divider to slow down dispensing
} MOT_state_t;

uint8_t MOT_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io);

// Task Notifications
void MOT_StartContinuous(void);
void MOT_StopContinuous(void);
void MOT_SingleDisp(void);

void MOT_OnInterrupt(void);

void MOT_Init(void);
void MOT_Deinit(void);

#endif // motor_h
