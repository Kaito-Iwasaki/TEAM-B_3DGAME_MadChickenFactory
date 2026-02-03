//===================================
// 
//  プロンプト処理　[prompt.h]
//  Author shuuhei Ida
//
//===================================

#ifndef _BILLBOAD_H_
#define _BILLBOAD_H_

// インクルードファイル
#include "main.h"

// マクロ定義
#define MAX_PROMPT (256)									// 最大数

//プロトタイプ宣言

void InitPrompt(void);

void UninitPrompt(void);

void UpdatePrompt(void);

void DrawPrompt(void);

void SetPrompt(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nIdx);

void SetPromptUse(int nIdx, bool bUse);

bool GetPromptTrigger(int nIdx);

#endif // !_BILLBOAD_H_