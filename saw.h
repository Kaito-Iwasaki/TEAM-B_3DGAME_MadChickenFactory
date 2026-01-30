//==================================================
//
// 回転ノコギリ[saw.h]
// Author : Keitaro Nagate
//
//==================================================
#ifndef _SAW_H_
#define _SAW_H_

//==================================================
//
//	ファイルインクルード
//
//==================================================
#include "main.h"
#include "saw.h"

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
}Saw;

//==================================================
//
//	プロトタイプ宣言
//
//==================================================
void InitSaw(void);
void UninitSaw(void);
void UpdateSaw(void);
void DrawSaw(void);
void SetSaw(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool startup);
void SwitchSaw(int nIdx);
bool CollisionSaw(void);

#endif // !_STAGELIGHT_H_

