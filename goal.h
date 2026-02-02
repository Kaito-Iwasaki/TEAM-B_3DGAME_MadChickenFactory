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
	float fWidMax;									// 幅の最大値
	float fWidMin;									// 幅の最小値
	float fDepMax;									// 奥行の最大値
	float fDepMin;									// 奥行の最小値
	bool bUse;										// 使用しているかどうか

}GOAL;

// プロトタイプ宣言
void InitGoal(void);
void UninitGoal(void);
void UpdateGoal(void);
void DrawGoal(void);
void CollisionGoal(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fRadius);
void SetGoalWidthAndDepth(GOAL* pGoal, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);
void SetGoal(D3DXVECTOR3 pos, D3DXVECTOR3 rot);


#endif
