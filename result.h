#ifndef _RESULT_H_
#define _RESULT_H_	
#include"main.h"
typedef struct
{

	D3DXVECTOR3 g_posResult;			//位置
	D3DXVECTOR3 g_moveResult;			//移動量
	D3DXCOLOR g_colerResult;			//色
	int g_nCounterAnimResult;			//アニメーションカウンター
	int g_nPatternAnimResult;			//アニメーションパターンNo
	int nType;							//リザルトのの種類
	bool bEnter;						//画面移行を連打されないようにするため
}Result;
//プロトタイプ宣言
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);		//main.cppから移動
void DrawResult(void);
#endif 
