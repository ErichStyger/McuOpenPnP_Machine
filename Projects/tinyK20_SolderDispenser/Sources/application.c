#include "platform.h"
#include "motor.h"
#include "comm.h"
#include "application.h"
#include "LED1.h"
#include "CLS1.h"
//#include "SW.h"
#include "CDC1.h"
#include "FRTOS1.h"
#include "platform.h"
#include "TI1.h"
#include "PORT_PDD.h"
#include "KEY1.h"


void APP_Start(void){
	PL_Init();
	TI1_Enable();
	vTaskStartScheduler();
}

static KEY_Event keyEvent;

static void keyEventHandler(void){
	 switch (keyEvent) {
		case EVT_PRESS:
			SHELL_SendString("EVT_PRESS\r\n");
			keyEvent = EVT_HANDLED;
			break;
		case EVT_LONG_PRESS:
			SHELL_SendString("EVT_LONG_PRESS\r\n");
			MOT_StartContinuous();
			keyEvent = EVT_HANDLED;
			break;
		case EVT_RELEASE:
			SHELL_SendString("EVT_SHORT_RELEASE\r\n");
			MOT_SingleDisp();
			keyEvent = EVT_HANDLED;
			break;
		case EVT_LONG_RELEASE:
			SHELL_SendString("EVT_LONG_RELEASE\r\n");
			MOT_StopContinuous();
			keyEvent = EVT_HANDLED;
			break;
		default:
			break;
	}
}

static void AppTask(void *pv) {
	SHELL_SendString("App Task started!\r\n");
	for(;;){
		if(keyEvent != EVT_HANDLED){
			keyEventHandler();
		}
		KEY1_ScanKeys();
		LED1_Neg();
		vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void APP_OnKeyPressed(void){keyEvent = EVT_PRESS;}
void APP_OnKeyReleased(void){keyEvent = EVT_RELEASE;}
void APP_OnKeyPressedLong(void){keyEvent = EVT_LONG_PRESS;}
void APP_OnKeyReleasedLong(void){keyEvent = EVT_LONG_RELEASE;}

void APP_Init(void){
  /* enable and turn on pull-up resistor for PTA14 */
  PORT_PDD_SetPinPullSelect(PORTD_BASE_PTR, 5, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTD_BASE_PTR, 5, PORT_PDD_PULL_ENABLE);
  keyEvent = EVT_HANDLED;
  FRTOS1_xTaskCreate(AppTask, "App", 900/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+2, NULL);
}

void APP_Deinit(void){

}

