//==================================================
//
// 可動箱[box_movable.h]
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

//==================================================
//
//	構造体定義
//
//==================================================
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//角度
	bool bStartup;		//起動のON/OFF
	bool bUse;			// 使用 / 不使用
	D3DXMATRIX mtxWorld;

	float turnSpeed;
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
void SetMoveBox(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool startup);
void SwitchMoveBox(int nIdx);
bool CollisionMoveBox(void);

#endif // !_STAGELIGHT_H_

