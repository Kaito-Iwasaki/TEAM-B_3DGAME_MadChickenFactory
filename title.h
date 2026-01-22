#ifndef _TITLE_H_
#define _TITLE_H_	
#include"main.h"
//=====================
//タイトル画面の構造体
//=====================
typedef struct
{

	D3DXVECTOR3 g_posTitle;				//位置
	D3DXVECTOR3 g_moveTitle;			//移動量
	D3DXCOLOR g_colerTitle;				//色
	int g_nCounterAnimTitle;			//アニメーションカウンター
	int g_nPatternAnimTitle;			//アニメーションパターンNo
	int nType;							//タイトルの種類
	bool bEnter;						//画面移行を連打されないようにするため
}TITLE;
//プロトタイプ宣言
void InitTitle(void);					
void UninitTitle(void);					
void UpdateTitle(void);		
void DrawTitle(void);
#endif 
