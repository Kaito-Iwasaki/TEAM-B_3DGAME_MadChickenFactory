#ifndef _PAUSE__H_
#define _PAUSE__H_

#include "main.h"

//ポーズメニュー
typedef enum
{
	PAUSE_MENU_BG = 0,
	PAUSE_MENU_CONTINUE,
	PAUSE_MENU_RETRY,
	PAUSE_MENU_QUIT,
	PAUSE_MENU_MAX,
}PAUSE_MENU;

//マクロ定義
#define NUM_PAUSE (4)

void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
void ResetPause(void);

#endif // !_PAUSE__H_

