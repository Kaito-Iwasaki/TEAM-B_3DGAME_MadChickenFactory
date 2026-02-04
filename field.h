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
	D3DXVECTOR3 rot;		// 向き
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

void SetField(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int ntxtype);

bool CollisionField(D3DXVECTOR3* pos, D3DXVECTOR3 posold);

#endif // !_FIELD_H_