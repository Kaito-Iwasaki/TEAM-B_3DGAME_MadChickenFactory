#ifndef _RESULT_H_
#define _RESULT_H_	
#include"main.h"
//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
typedef enum
{
	RESULTTYPE_CLEAR = 0,
	RESULTTYPE_CLEARTIME,
	RESULTTYPE_MAX,
}RESULT;
//=====================
//リザルト画面の状態
//=====================
typedef struct
{

	D3DXVECTOR3 g_posResult;			//位置
	D3DXCOLOR g_colerResult;			//色
	int g_nCounterAnimResult;			//アニメーションカウンター
	int g_nPatternAnimResult;			//アニメーションパターンNo
	RESULT nType;							//リザルトのの種類
}Result;
//================
//プロトタイプ宣言
//================
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);		
void DrawResult(void);
#endif 
