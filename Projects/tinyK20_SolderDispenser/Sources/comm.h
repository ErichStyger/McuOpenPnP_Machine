#ifndef comm_h
#define comm_h

#include "platform.h"

/*
 * 	Send String to console
 */
void SHELL_SendString(unsigned char *msg);

void SHELL_ParseCmd(uint8_t *cmd);

void SHELL_Init(void);

void SHELL_Deinit(void);

//void ShellReadAndParseCmd(void);

#endif // comm_h
