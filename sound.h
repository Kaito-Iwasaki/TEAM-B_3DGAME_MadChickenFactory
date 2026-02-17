//=============================================================================
//
// サウンド処理 [sound.h]
// Author : KAITO IWASAKI
// Special Thanks : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_SE_SAW = 0,
	SOUND_LABEL_SE_PRESS,
	SOUND_LABEL_SE_FIRE,
	SOUND_LABEL_SE_ENEMY,
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

void SetVolume(SOUND_LABEL sound, float Volume);
XAUDIO2_VOICE_STATE* GetSoundState(SOUND_LABEL label);

#endif
