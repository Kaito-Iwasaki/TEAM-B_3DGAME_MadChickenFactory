//==================================================
//
// 可動箱[box_movable.h]
// Author : Keitaro Nagate
//
//==================================================
#ifndef _BOX_MOVABLE_H_
#define _BOX_MOVABLE_H_

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
#define MAX_MOVEBOX		(128)					//モデルの最大数

//==================================================
//
//	列挙型
//
//==================================================
typedef enum
{
	STATE_NORMAL = 0,	// 触れられていない
	STATE_RIGHT,		// 右から
	STATE_LEFT,			// 左から
	STATE_REAR,			// 奥から
	STATE_FRONT,		// 前から
	STATE_MAX
}BoxState;

//==================================================
//
//	構造体定義
//
//==================================================
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 角度
	D3DXVECTOR3 MotionRange;	// 可動域
	bool bChatch;				// 掴まれているか
	bool bUse;					// 使用 /不使用
	BoxState state;				// どちらから触れられているか
	D3DXVECTOR3 SavePos;		// 初期配置
	D3DXMATRIX mtxWorld;
}MoveBox;

//==================================================
//
//	プロトタイプ宣言
//
//==================================================
void InitMoveBox(void);
void UninitMoveBox(void);
void UpdateMoveBox(void);
void DrawMoveBox(void);
void SetMoveBox(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 range);
bool CollisionMoveBox(void);

#endif // !_STAGELIGHT_H_

