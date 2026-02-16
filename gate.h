//=====================================================================
//
// gate.cppのヘッダファイル [gate.h]
// Author : 
// 
//=====================================================================
#ifndef _GATE_H_
#define _GATE_H_

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "main.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_GATE			(256)		//モデルの最大数

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{//プレス機の状態
	GATESTATE_STAY = 0,
	GATESTATE_UP,
	GATESTATE_DOWN,
	GATESTATE_MAX
}GateState;

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
// ゲート構造体
typedef struct
{
	int nIdx;
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 poslocal;	// 設定位置
	D3DXVECTOR3 rot;		// 角度
	D3DXVECTOR3 moveRange;	// 可動域
	float memheight;		// 動作前高度記憶用
	float movewidth;		// 動作幅
	bool bGoup;				// 起動のON/OFF
	bool bUse;				// 使用 / 不使用
	GateState Gstate;		//ゲートの状態
	GateState bprevious;	//ゲートの前の状態
	D3DXMATRIX mtxWorld;

}Gate;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitGate(void);
void UninitGate(void);
void UpdateGate(void);
void DrawGate(void);
void SetGate(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int nIdx, float movewidth, bool Goup , GateState state = GATESTATE_STAY);
void SwitchGate(int nIdx);
int GetGate (void);
void CollisionGate(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fRadius);
#endif