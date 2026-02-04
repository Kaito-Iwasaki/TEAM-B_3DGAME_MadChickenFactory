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
#define	MAX_FIRE			(256)		// 最大数
#define MAX_FIRETEXTURE		(128)		// テクスチャ数

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
// 操作状態列挙型
typedef enum
{
	OPERATIONSTATE_NONE = 0,	// 操作未設定
	OPERATIONSTATE_MANUAL,		// 手動操作
	OPERATIONSTATE_AUTMATIC,	// 自動操作
	OPERATIONSTATE_MAX
}OPERATIONSTATE;

// 炎状態列挙型
typedef enum
{
	FIRESTATE_OFF = 0,			// 炎OFF状態
	FIRESTATE_READY,			// 炎準備状態
	FIRESTATE_ON,				// 炎ON状態
	FIRESTATE_MAX
}FIRESTATE;

typedef enum
{
	SETFIREMODE_SETTING = 0,	// 炎設定
	SETFIREMODE_SWICHING,		// 炎状態変更
	SETFIREMODE_MAX

}SETFIREMODE;

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
	OPERATIONSTATE state;							// 状態
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
	int nIdx;				// インデックス
	int nSwitching;			// 炎の切り替え値
	FIRESTATE state;		// 炎状態
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
void SetFlamethrower(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OPERATIONSTATE state);
void SetFire(SETFIREMODE setfiremode, int nIdx, D3DXVECTOR3 pos, FIRESTATE state);
void SetFlamethrowerWidthAndDepth(int nIdx);
void CollisionFlamethrower(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fRadius);

#endif