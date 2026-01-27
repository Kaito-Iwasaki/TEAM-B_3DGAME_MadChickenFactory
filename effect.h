#ifndef _EFFECT_H_
#define _EFFECT_H_
//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	EFFECTTYPE_SHADOW = 0,
	EFFECTTYPE_DASH,				//走った後の煙
	EFFECTTYPE_LANDINGE,			//着地の煙
	EFFECTTYPE_MAX,
}EFFECTTYPE;
//=====================
//エフェクトの構造体
//=====================
typedef struct
{
	D3DXVECTOR3 pos;					//エフェクトの位置
	D3DXVECTOR3 move;					//エフェクトの移動
	D3DXMATRIX g_mtxWorldEffect;		//ワールドマトリックス
	EFFECTTYPE type;					//エフェクトの種類
	int nLife;							//エフェクトの描画時間
	bool bUse;							//エフェクトを使用しているかどうか
	D3DCOLOR col;						//エフェクトのカラー
}Effect;
//=================
//プロトタイプ宣言
//=================
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);		
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, EFFECTTYPE type, int nLife , D3DCOLOR col);

#endif
