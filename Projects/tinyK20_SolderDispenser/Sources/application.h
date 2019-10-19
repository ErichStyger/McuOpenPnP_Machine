#ifndef app_h
#define app_h

#include "platform.h"

typedef enum KEY_Event {
	EVT_PRESS,
	EVT_LONG_PRESS,
	EVT_RELEASE,
	EVT_LONG_RELEASE,
	EVT_HANDLED
} KEY_Event;

typedef enum KEY_StateKinds {
	KEY_IDLE, 	/*<! Initial idle state */
	KEY_PRESSED,  /*<! Key pressing detected, see if it is a long key */
} KEY_StateKinds;

typedef struct KEY_FSMData {
	KEY_StateKinds state;  /*!< status of the state machine to detect long and short keys */
	uint8_t keyValue;  	/*!< value of keys scanned in */
	uint16_t longKeyCnt; /*!< counting how long we press a key */
	uint16_t longKeyTicks; /*!< number of trigger ticks needed for long key press */
	uint16_t debounceTicks; /*!< number of trigger ticks needed for debouncing */
} KEY_FSMData;

void APP_Start(void);

void APP_OnKeyPressed(void);
void APP_OnKeyReleased(void);
void APP_OnKeyPressedLong(void);
void APP_OnKeyReleasedLong(void);

void APP_Init(void);

void APP_Deinit(void);

#endif // app_h
