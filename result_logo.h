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
	RESULTTYPE_BACKGROUND = 0,
	RESULTTYPE_CLEAR,
	RESULTTYPE_CLEARTIME,
	RESULTTYPE_MAX,
}RESULT;
//=====================
//リザルト画面の状態
//=====================
typedef struct
{

	D3DXVECTOR3 pos;			//位置
	D3DXCOLOR col;			//色
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
