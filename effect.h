#ifndef _EFFECT_H_
#define _EFFECT_H_
//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	EFFECTTYPE_BULLET = 0,
	EFFECTTYPE_MAX,
}EFFECTTYPE;
//=====================
//エフェクトの構造体
//=====================
typedef struct
{
	D3DXVECTOR3 pos;					//エフェクトの位置
	D3DXMATRIX g_mtxWorldEffect;		//ワールドマトリックス
	EFFECTTYPE type;					//エフェクトの種類
	int nLife;							//エフェクトの描画時間
	bool bUse;							//エフェクトを使用しているかどうか
}Effect;
//=================
//プロトタイプ宣言
//=================
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);		
void DrawEffect(void);


#endif
