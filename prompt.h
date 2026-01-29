//===================================
// 
//  プロンプト処理　[prompt.h]
//  Author shuuhei Ida
//
//===================================

#ifndef _BILLBOAD_H_
#define _BILLBOAD_H_
#include "main.h"

//プロトタイプ宣言

void InitPrompt(void);

void UninitPrompt(void);

void UpdatePrompt(void);

void DrawPrompt(void);

void SetPrompt(D3DXVECTOR3 pos, D3DXVECTOR3 size);

#endif // !_BILLBOAD_H_