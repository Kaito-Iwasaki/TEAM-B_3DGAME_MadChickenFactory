//=======================================================
//
// ゴール処理[goal.h]
// Author : Airi Takahashi
//
//=======================================================
#ifndef _GOAL_H_		// このマクロ定義がされなかったら
#define _GOAL_H_		// 2重インクルード防止のマクロを定義する

#include"main.h"

// マクロ定義
#define MAX_GOALTEXTURE		(16)		// ゴールのテクスチャ数

// ゴール情報
typedef struct
{
	D3DXVECTOR3 pos;								// 位置
	D3DXVECTOR3 rot;								// 向き
	D3DXMATRIX mtxWorld;							// ワールドマトリックス
	LPDIRECT3DTEXTURE9 apTexture[MAX_GOALTEXTURE];	// テクスチャへのポインタ
	LPD3DXMESH pMesh;								// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat;							// マテリアルへのポインタ
	DWORD dwNumMat;									// マテリアルの数
	D3DXVECTOR3 vtxMax;								// 最大値
	D3DXVECTOR3 vtxMin;								// 最小値

}GOAL;

// プロトタイプ宣言
void InitGoal(void);
void UninitGoal(void);
void UpdateGoal(void);
void DrawGoal(void);

#endif
