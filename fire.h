//=====================================================================
//
// fire.cppのヘッダファイル [fire.h]
// Author : AIRI TAKAHASHI
// 
//=====================================================================
#ifndef _fire_H_
#define _fire_H_

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
#define MAX_FIRETEXTURE		(128)		// テクスチャ数

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	FIRESTATE_NONE = 0,		// 操作未設定
	FIRESTATE_MANUAL,		// 手動操作
	FIRESTATE_AUTMATIC		// 自動操作
}FIRESTATE;

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
// 火炎放射器モデル構造体
typedef struct
{
	LPDIRECT3DTEXTURE9 apTexture[MAX_FIRETEXTURE];	// テクスチャへのポインタ
	LPD3DXMESH pMesh;								// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat;							// マテリアルへのポインタ
	DWORD dwNumMat;									// マテリアルの数
	D3DXVECTOR3 vtxMax;								// 最大値
	D3DXVECTOR3 vtxMin;								// 最小値

}FLAMETHROWERMODEL;

// 火炎放射器構造体
typedef struct
{
	D3DXVECTOR3 pos;								// 位置
	D3DXVECTOR3 rot;								// 向き
	D3DXMATRIX mtxWorld;							// ワールドマトリックス
	FIRESTATE state;								// 状態
	float fWidMax;									// 幅の最大値
	float fWidMin;									// 幅の最小値
	float fDepMax;									// 奥行の最大値
	float fDepMin;									// 奥行の最小値
	int fireCounter;								// 炎の切り替え間隔カウンター
	bool bUse;										// 使用しているかどうか
}FLAMETHROWER;

// 炎構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	bool bUse;				// 使用しているかどうか
}FIRE;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitFire(void);
void UninitFire(void);
void UpdateFire(void);
void DrawFire(void);
void SetFlamethrower(D3DXVECTOR3 pos, D3DXVECTOR3 rot, FIRESTATE state);
void SetFire(int nIdx, D3DXVECTOR3 pos);
void SetFlamethrowerWidthAndDepth(int nIdx);
void CollisionFlamethrower(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fRadius);

#endif