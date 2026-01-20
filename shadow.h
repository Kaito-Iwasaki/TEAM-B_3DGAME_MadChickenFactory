//=======================================================
//
// 影の描画処理[shadow.h]
// Author : Airi Takahashi
//
//=======================================================
#ifndef _SHADOW_H_		// このマクロ定義がされなかったら
#define _SHADOW_H_		// 2重インクルード防止のマクロを定義する

#include"main.h"

// 影の構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 角度
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	float fWidth;			// 幅
	bool bUse;				// 使用しているかどうか

}Shadow;

// プロトタイプ宣言
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(D3DXVECTOR3 pos, float fWidth);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, float fAddWidth);
Shadow* GetShadow(void);

#endif
