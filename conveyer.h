//=======================================================
//
// ベルトコンベアの処理[conveyer.h]
// Author : Airi Takahashi
//
//=======================================================
#ifndef _CONVEYER_H_		// このマクロ定義がされなかったら
#define _CONVEYER_H_		// 2重インクルード防止のマクロを定義する

#include"main.h"

// コンベアの構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	D3DXVECTOR3 move;		// 移動量
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	float fWidth;			// 幅
	float fDepth;			// 奥行
	int nIdx;				// インデックス
	bool bUse;				// 使用しているかどうか

}Conveyer;

// プロトタイプ宣言
void InitConveyer(void);
void UninitConveyer(void);
void UpdateConveyer(void);
void DrawConveyer(void);
void SetConveyer(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, float fWidth, float fDepth);


#endif
