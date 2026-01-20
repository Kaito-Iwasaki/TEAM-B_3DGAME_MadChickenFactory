//================================
// 
//  壁処理　[wall.h]
//  Author shuuhei Ida
//
//================================


#ifndef _WALL_H_
#define _WALL_H_
#include "main.h"

//プロトタイプ宣言

void InitWall(void);

void UninitWall(void);

void UpdateWall(void);

void DrawWall(void);

void SetWall(D3DXVECTOR3 pos,D3DXVECTOR3 size, D3DXVECTOR3 rot);

#endif // !_WALL_H_