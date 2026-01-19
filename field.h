//================================
// 
//  フィールド処理　[field.h]
//  Author shuuhei Ida
//
//================================

#ifndef _FIELD_H_
#define _FIELD_H_
#include "main.h"

// フィールドの構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 size;
	D3DXMATRIX mtxWorld;						//	ワールドマトリックス
	bool bUse;
}Field;

//プロトタイプ宣言

void InitField(void);

void UninitField(void);

void UpdateField(void);

void DrawField(void);

void SetField(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot);

#endif // !_FIELD_H_