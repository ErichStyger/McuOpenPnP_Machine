#include "platform.h"
#include "comm.h"
#include "motor.h"
#include "application.h"
#include "FRTOS1.h"
#include "CLS1.h"
#include "CDC1.h"
#include "RTT1.h"
#include "UTIL1.h"

/* ******************************************************************
 * SHELL Standard I/O
 * ******************************************************************/
static void SHELL_SendChar(uint8_t ch) {
#if PL_RTT_EN
  CLS1_SendCharFct(ch, RTT1_SendChar); /* blocking version with timeout */
#endif
#if PL_CDC_EN
  CDC1_SendChar(ch);
#endif
}

static void SHELL_ReadChar(uint8_t *p) {
  *p = '\0'; /* default, nothing available */
#if PL_RTT_EN
  if (RTT1_stdio.keyPressed()) {
    RTT1_stdio.stdIn(p);
    return;
  }
#endif
#if PL_CDC_EN
  if (CDC1_stdio.keyPressed()) {
    CDC1_stdio.stdIn(p);
    return;
  }
#endif
}

static bool SHELL_KeyPressed(void) {
#if PL_RTT_EN
  if (RTT1_stdio.keyPressed()) {
    return TRUE;
  }
#endif
#if PL_CDC_EN
  if (CDC1_stdio.keyPressed()) {
    return TRUE;
  }
#endif
  return FALSE;
}

CLS1_ConstStdIOType SHELL_stdio =
{
  (CLS1_StdIO_In_FctType)SHELL_ReadChar, /* stdin */
  (CLS1_StdIO_OutErr_FctType)SHELL_SendChar, /* stdout */
  (CLS1_StdIO_OutErr_FctType)SHELL_SendChar, /* stderr */
  SHELL_KeyPressed /* if input is not empty */
};

static uint8_t SHELL_DefaultShellBuffer[CLS1_DEFAULT_SHELL_BUFFER_SIZE]; /* default buffer which can be used by the application */

CLS1_ConstStdIOType *SHELL_GetStdio(void) {
  return &SHELL_stdio;
}

typedef struct {
  CLS1_ConstStdIOType *stdio;
  unsigned char *buf;
  size_t bufSize;
} SHELL_IODesc;

static const SHELL_IODesc ios[] =
{
  {&SHELL_stdio, SHELL_DefaultShellBuffer, sizeof(SHELL_DefaultShellBuffer)},
#if PL_RTT_EN
  {&RTT1_stdio, RTT1_DefaultShellBuffer, sizeof(RTT1_DefaultShellBuffer)},
#endif
#if PL_CDC_EN
  {&CDC1_stdio, CDC1_DefaultShellBuffer, sizeof(CDC1_DefaultShellBuffer)},
#endif
};



static uint8_t SHELL_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io);

static const CLS1_ParseCommandCallback CmdParserTable[] =
{
  CLS1_ParseCommand, /* Processor Expert Shell component, is first in list */
  SHELL_ParseCommand, /* our own module parser */
  MOT_ParseCommand,
  NULL /* Sentinel */
};

static uint8_t localConsole_buf[48];
CLS1_ConstStdIOTypePtr ioLocal;

void SHELL_SendString(unsigned char *msg){
	CLS1_SendStr(msg, CLS1_GetStdio()->stdOut);
}


static uint8_t SHELL_PrintHelp(const CLS1_StdIOType *io) {
	return ERR_OK;
}

static uint8_t SHELL_PrintStatus(const CLS1_StdIOType *io) {
	return ERR_OK;
}

static uint8_t SHELL_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io) {
	if (UTIL1_strcmp((char*)cmd, CLS1_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, "Shell help")==0) {
    *handled = TRUE;
    return SHELL_PrintHelp(io);
  } else if (UTIL1_strcmp((char*)cmd, CLS1_CMD_STATUS)==0 || UTIL1_strcmp((char*)cmd, "Shell status")==0) {
    *handled = TRUE;
    return SHELL_PrintStatus(io);
  }
  return ERR_OK;
}

void SHELL_ParseCmd(uint8_t *cmd) {
  (void)CLS1_ParseWithCommandTable(cmd, ios[0].stdio, CmdParserTable);
}

#if PL_RTOS_EN
static void ShellTask(void *pvParameters) {
  int i;

  /* initialize buffers */
  for(i=0;i<sizeof(ios)/sizeof(ios[0]);i++) {
    ios[i].buf[0] = '\0';
  }
  SHELL_SendString("Shell task started!\r\n");
  for(;;) {
    /* process all I/Os */
    for(i=0;i<sizeof(ios)/sizeof(ios[0]);i++) {
      (void)CLS1_ReadAndParseWithCommandTable(ios[i].buf, ios[i].bufSize, ios[i].stdio, CmdParserTable);
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  } /* for */
}
#endif /* PL_CONFIG_HAS_RTOS */


TaskHandle_t ShellTaskHandle = NULL;

void SHELL_Init(void) {
	CLS1_SetStdio(SHELL_GetStdio()); /* set default standard I/O to RTT */
	#if PL_RTOS_EN
	if (xTaskCreate(ShellTask, "Shell", 900/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+1, &ShellTaskHandle) != pdPASS) {
		for(;;){} /* error */
	}
	#endif
}

void SHELL_Deinit(void){

}
