//==================================================
//
// プレス機[press.h]
//
//==================================================
#ifndef _PRESS_H_
#define _PRESS_H_

//==================================================
//
//	ファイルインクルード
//
//==================================================
#include "main.h"

//==================================================
//
//	マクロ定義
//
//==================================================
#define MAX_PRESS				(128)		//プレス機の最大数
#define MAX_TEXTURE_PRESS		(8)			//テクスチャの最大数

//==================================================
//
//	列挙型
//
//==================================================
typedef enum
{//プレス機の状態
	PRESSSTATE_UP = 0,
	PRESSSTATE_DOWN,
	PRESSSTATE_STAY,
	PRESSSTATE_MAX
}PressState;

//==================================================
//
//	構造体定義
//
//==================================================
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 角度
	float memheight;		// 動作前高度記憶用
	float movewidth;		// 動作幅
	int interval;			// 下降のインターバル
	int intervalCnt;		// インターバルのカウント
	bool bStartup;			// 起動のON/OFF
	PressState PState;		// プレス機の状態
	PressState bprevious;	// ひとつ前の状態
	bool bUse;				// 使用 / 不使用
	int nIdx;				// インデックス
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
}Press;


//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitPress(void);
void UninitPress(void);
void UpdatePress(void);
void DrawPress(void);
void SetPress(int nIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float movewidth, int interval, PressState state = PRESSSTATE_STAY);
void PressMachineSwitch(int nIdx);

#endif