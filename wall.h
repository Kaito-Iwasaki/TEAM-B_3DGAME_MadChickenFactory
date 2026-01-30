//================================
// 
//  壁処理　[wall.h]
//  Author shuuhei Ida
//
//================================
#ifndef _WALL_H_
#define _WALL_H_

//===============================================
// 
// **** インクルードファイル ****
//
//===============================================
#include "main.h"

//===============================================
// 
// **** マクロ定義 ****
//
//===============================================
#define MAX_WALL (256)

//===============================================
// 
// **** 構造体定義 ****
//
//===============================================
typedef struct
{//壁構造体
	int nTexType;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 size;
	D3DXMATRIX mtxWorldWall;
	bool bUse;
} Wall;

//===============================================
// 
// **** プロトタイプ宣言 ****
//
//===============================================
void InitWall(void);

void UninitWall(void);

void UpdateWall(void);

void DrawWall(void);

void SetWall(int nTexType,  D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot);

bool CollisionWall(D3DXVECTOR3* pos, D3DXVECTOR3 posold, D3DXVECTOR3 size);

#endif // !_WALL_H_