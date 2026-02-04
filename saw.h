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
#define MAX_SAW			(128)		//モデルの最大数
#define MAX_TEXTURE		(8)			//テクスチャの最大数

//==================================================
//
//	構造体定義
//
//==================================================
typedef struct
{
	int nIdx;
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 poslocal;	//設定位置
	D3DXVECTOR3 rot;		//角度
	D3DXVECTOR3 moveRange;	//可動域
	bool bStartup;			//起動のON/OFF
	bool bUse;				// 使用 / 不使用
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
void SetSaw(int nIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 moveRange, bool startup);
void SwitchSaw(int nIdx);
bool CollisionSaw(void);

#endif // !_STAGELIGHT_H_

