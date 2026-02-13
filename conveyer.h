//=======================================================
//
// ベルトコンベアの処理[conveyer.h]
// Author : Airi Takahashi
//
//=======================================================
#ifndef _CONVEYER_H_		// このマクロ定義がされなかったら
#define _CONVEYER_H_		// 2重インクルード防止のマクロを定義する

#include"main.h"

// マクロ定義
#define MAX_CONVEYER		(128)			// ベルトコンベアの最大数

// コンベア状態の列挙型
typedef enum
{
	CONVEYERSTATE_MOVE = 0,		// 稼働状態
	CONVEYERSTATE_STOP,			// 停止状態
	CONVEYERSTATE_MAX
}CONVEYERSTATE;

// コンベアの構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	D3DXVECTOR3 Onmove;		// ON状態の移動量
	D3DXVECTOR3 Offmove;	// OFF状態の移動量
	D3DXVECTOR3 size;		// サイズ
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	float movetex;			// コンベアの移動量
	int nIdx;				// インデックス
	CONVEYERSTATE state;	// 状態
	bool bUse;				// 使用しているかどうか

}Conveyer;

// プロトタイプ宣言
void InitConveyer(void);
void UninitConveyer(void);
void UpdateConveyer(void);
void DrawConveyer(void);
void SetConveyer(int nIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Onmove, D3DXVECTOR3 Offmove, D3DXVECTOR3 size, CONVEYERSTATE state);
bool CollisioncConveyer(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove);
void SetSizeConveyer(D3DXVECTOR3 rot, D3DXVECTOR3 *size);

#endif