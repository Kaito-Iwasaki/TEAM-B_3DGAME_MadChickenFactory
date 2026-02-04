//================================
// 
//  フィールド処理　[field.h]
//  Author shuuhei Ida
//
//================================

#ifndef _FIELD_H_
#define _FIELD_H_
#include "main.h"

#define MAX_FIELD (256)

// フィールドの構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 poslocal;	// 位置(保存用)
	D3DXVECTOR3 rot;		// 向き
	D3DXVECTOR3 speed;		// 速さ
	D3DXVECTOR3 range;		// 可動域
	D3DXVECTOR3 size;		// 大きさ
	int nTxtype;			// テクスチャの種類
	D3DXMATRIX mtxWorld;	//	ワールドマトリックス
	bool bUse;
}Field;

//プロトタイプ宣言
void InitField(void);
void UninitField(void);
void UpdateField(void);
void DrawField(void);

void SetField(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot,int ntxtype
	,D3DXVECTOR3 speed = D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3 range = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

bool CollisionField(D3DXVECTOR3* pos, D3DXVECTOR3 posold);

#endif // !_FIELD_H_