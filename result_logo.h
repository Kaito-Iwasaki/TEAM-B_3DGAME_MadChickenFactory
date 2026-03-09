#ifndef _RESULT_LOGO_H_
#define _RESULT_LOGO_H_	
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
void InitResult_Logo(void);
void UninitResult_Logo(void);
void UpdateResult_Logo(void);		
void DrawResult_Logo(void);
#endif 
