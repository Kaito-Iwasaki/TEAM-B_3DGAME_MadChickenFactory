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
#define MAX_TEXTURE_PRESS		(8)	//テクスチャの最大数

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
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 Setpos;	//移動量
	D3DXVECTOR3 rot;	//角度
	int interval;		//下降のインターバル
	int intervalCnt;	//インターバルのカウント
	bool bStartup;		//起動のON/OFF
	PressState PState;	//プレス機の状態
	bool bUse;			// 使用 / 不使用
	int nIdx;			//インデックス
	D3DXMATRIX mtxWorld;
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
void SetPress(int nIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, int interval);
void PressMachineSwitch(int nIdx);

#endif