//==================================================
//
// リフト[lift.h]
// Author : Keitaro Nagate
//
//==================================================
#ifndef _LIFT_H_
#define _LIFT_H_

//==================================================
//
//	ファイルインクルード
//
//==================================================
#include "main.h"

//==================================================
//
//	構造体定義
//
//==================================================
#define MAX_TEXTURE		(8)	//テクスチャの最大数

//==================================================
//
//	列挙型
//
//==================================================
typedef enum
{// リフトの状態
	LIFTSTATE_SV_POINT = 0,		// SavePointへ向かっている
	LIFTSTATE_GO_POINT,			// GoPointへ向かっている
	LIFTSTATE_STAY,				// インターバル中
	LIFTSTATTE_MAX
}LiftState;
//==================================================
//
//	構造体定義
//
//==================================================
typedef struct
{
	int nIdx;					// プロンプトとの同機用
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 move;			// 移動量
	D3DXVECTOR3 rot;			// 角度
	D3DXVECTOR3 SavePos;		// 初期配置
	D3DXVECTOR3 GoPoint;		// 移動先の座標
	D3DXVECTOR3 vec;			// SaveとGo間のベクトル
	float speed;				// 移動速度
	bool bswitch;				// 起動のオンオフ
	LiftState NowState;			// 現在の状態
	LiftState PreviousState;	// 前回の状態（STAY中に仕様）
	bool bUse;					// 使用 /不使用
	int interval;				// インターバル(-1で手動)
	int nCounter;				// インターバルのカウンタ
	D3DXMATRIX mtxWorld;
}Lift;

//==================================================
//
//	プロトタイプ宣言
//
//==================================================
void InitLift(void);
void UninitLift(void);
void UpdateLift(void);
void DrawLift(void);
void SetLift(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 range);
bool CollisionLift(void);

#endif // !_STAGELIGHT_H_

