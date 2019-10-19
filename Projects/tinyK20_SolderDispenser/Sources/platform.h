#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "PE_Types.h"

#define PL_MOTOR_EN			(1)
#define PL_COMM_EN			(1)
#define PL_RTT_EN			  (0)
#define PL_CDC_EN			  (1)
#define PL_RTOS_EN			(1)

#define PL_CUSTOM_DEBOUNCE	(0)

void PL_Init(void);

#endif /* PLATFORM_H_ */
