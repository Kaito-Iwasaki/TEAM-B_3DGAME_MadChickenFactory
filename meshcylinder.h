//=======================================================
//
// メッシュシリンダーの描画処理[meshcylinder.h]
// Author : Airi Takahashi
//
//=======================================================
#ifndef _MESHCYLINDER_H_		// このマクロ定義がされなかったら
#define _MESHCYLINDER_H_		// 2重インクルード防止のマクロを定義する

#include"main.h"

// メッシュシリンダーの構造体
typedef struct
{
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 rot;					// 向き
	D3DXMATRIX mtxWorld;				// ワールドマトリックス
	float fRadius;						// 半径
	float fHeight;						// 高さ
	int nDivisionRadius;				// 円分割数
	int nDivisionHeight;				// 高さ分割数
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;	// インデックスバッファへのポインタ
	bool bUse;							// 使用しているかどう

}MeshCylinder;

// プロトタイプ宣言
void InitMeshCylinder(void);
void UninitMeshCylinder(void);
void UpdateMeshCylinder(void);
void DrawMeshCylinder(void);
void SetMeshCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fHeight, int nDivisionRadius, int nDivisionHeight);

#endif
