//===========================================================
//
// ポーズ画面[Pause.cpp]
// Author : 長手　桂太朗
//
//===========================================================
#include "main.h"
#include "Pause.h"
#include "input.h"
#include "title.h"
#include "sound.h"
#include "game.h"
#include "sound.h"

//マクロ定義
#define PA_BG_WIDTH (400)
#define PA_BG_HEIGHT (420)

#define PA_SE_WIDTH (400)
#define PA_SE_HEIGHT (140)

typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 rot;
	float nDiagonalHalf;				//対角線の長さ
	float fAngle;
	int PauseType;				//背景、選択肢の番号保存
	D3DXCOLOR col;				//色
	bool bUse;					//使用、不使用
}PAUSE;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePause[4] = {};		//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;		//頂点バッファへのポインタ
PAUSE_MENU g_PauseMenu;								//ポーズメニュ-
PAUSE g_aPause[NUM_PAUSE];
int g_SelectModeP = 0;								//現在選択しているメニュー

//==============================================================
//ポーズ画面の初期化処理
//==============================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();
	
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\block_tex_000.jpg", &g_pTexturePause[PAUSE_MENU_BG]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pause000.png", &g_pTexturePause[PAUSE_MENU_CONTINUE]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pause001.png", &g_pTexturePause[PAUSE_MENU_RETRY]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pause002.png", &g_pTexturePause[PAUSE_MENU_QUIT]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPause = 0; nCntPause < 4; nCntPause++)
	{
		if (nCntPause == 0)
		{
			g_aPause[nCntPause].pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);				//ポーズ画面背景の座標
			g_aPause[nCntPause].nDiagonalHalf = 
				sqrtf(PA_BG_WIDTH * PA_BG_WIDTH + PA_BG_HEIGHT * PA_BG_HEIGHT) * 0.5;	//対角線の半分の長さ4
			g_aPause[nCntPause].fAngle = atan2f(PA_BG_WIDTH, PA_BG_HEIGHT);
			g_aPause[nCntPause].PauseType = nCntPause;									//ポーズ画面のパーツの種類
			g_aPause[nCntPause].bUse = true;

			//頂点カラーの設定
			pVtx[0].col =
				pVtx[1].col =
				pVtx[2].col =
				pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.5f);
		}
		else
		{
			g_aPause[nCntPause].pos = D3DXVECTOR3(640.0f, 220.0f + ((nCntPause - 1) * 140), 0.0f);
			g_aPause[nCntPause].nDiagonalHalf = sqrtf((PA_SE_WIDTH * PA_SE_HEIGHT) + (PA_SE_HEIGHT * PA_SE_HEIGHT)) * 0.5;
			g_aPause[nCntPause].fAngle = atan2f(PA_SE_WIDTH, PA_SE_HEIGHT);
			g_aPause[nCntPause].PauseType = nCntPause;
			g_aPause[nCntPause].bUse = false;	//すべて使用していない
			
			//頂点カラーの設定
			pVtx[0].col =
				pVtx[1].col =
				pVtx[2].col =
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		g_SelectModeP = 1;
		
		g_aPause[nCntPause].rot.z = 0.0f;

		pVtx[0].pos.x = g_aPause[nCntPause].pos.x - sinf(g_aPause[nCntPause].rot.z + g_aPause[nCntPause].fAngle) * g_aPause[nCntPause].nDiagonalHalf;
		pVtx[0].pos.y = g_aPause[nCntPause].pos.y - cosf(g_aPause[nCntPause].rot.z + g_aPause[nCntPause].fAngle) * g_aPause[nCntPause].nDiagonalHalf;
		pVtx[0].pos.z =  0.0f;				
					    					
		pVtx[1].pos.x = g_aPause[nCntPause].pos.x + sinf(g_aPause[nCntPause].rot.z + g_aPause[nCntPause].fAngle) * g_aPause[nCntPause].nDiagonalHalf;
		pVtx[1].pos.y = g_aPause[nCntPause].pos.y - cosf(g_aPause[nCntPause].rot.z + g_aPause[nCntPause].fAngle) * g_aPause[nCntPause].nDiagonalHalf;
		pVtx[1].pos.z =  0.0f;				
					    					
		pVtx[2].pos.x = g_aPause[nCntPause].pos.x - sinf(g_aPause[nCntPause].rot.z + g_aPause[nCntPause].fAngle) * g_aPause[nCntPause].nDiagonalHalf;
		pVtx[2].pos.y = g_aPause[nCntPause].pos.y + cosf(g_aPause[nCntPause].rot.z + g_aPause[nCntPause].fAngle) * g_aPause[nCntPause].nDiagonalHalf;
		pVtx[2].pos.z =  0.0f;				
					    					
		pVtx[3].pos.x = g_aPause[nCntPause].pos.x + sinf(g_aPause[nCntPause].rot.z + g_aPause[nCntPause].fAngle) * g_aPause[nCntPause].nDiagonalHalf;
		pVtx[3].pos.y = g_aPause[nCntPause].pos.y + cosf(g_aPause[nCntPause].rot.z + g_aPause[nCntPause].fAngle) * g_aPause[nCntPause].nDiagonalHalf;
		pVtx[3].pos.z =  0.0f;
		

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

}
//==============================================================
//ポーズ画面の終了処理
//==============================================================
void UninitPause(void)
{
	int nCnt;

	//テクスチャの破棄
	for (nCnt = 0; nCnt < PAUSE_MENU_MAX; nCnt++)
	{
		if (g_pTexturePause[nCnt] != NULL)
		{
			g_pTexturePause[nCnt]->Release();
			g_pTexturePause[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}
//==============================================================
//ポーズ画面の更新処理
//==============================================================
void UpdatePause(void)
{
	int nCntPause;

	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		if (g_aPause[nCntPause].PauseType == g_SelectModeP || g_aPause[nCntPause].PauseType == PAUSE_MENU_BG)
		{
			g_aPause[nCntPause].bUse = true;
		}
		else
		{
			g_aPause[nCntPause].bUse = false;
		}

		if (g_aPause[nCntPause].bUse == true)
		{
			//頂点カラーの設定
			pVtx[0].col =
				pVtx[1].col =
				pVtx[2].col =
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			//頂点カラーの設定
			pVtx[0].col =
				pVtx[1].col =
				pVtx[2].col =
				pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

	if (GetKeyboardTrigger(DIK_S) == true)
	{
		PlaySound(SOUND_LABEL_SE_ENTER);
		if (g_SelectModeP < 3)
		{
			g_SelectModeP += 1;
		}
		else if(g_SelectModeP == 3)
		{
			g_SelectModeP = 1;
		}
	}

	if (GetKeyboardTrigger(DIK_W) == true)
	{
		PlaySound(SOUND_LABEL_SE_ENTER);
		if (g_SelectModeP > 1)
		{
			g_SelectModeP -= 1;
		}
		else if (g_SelectModeP == 1)
		{
			g_SelectModeP = 3;
		}
	}

	if ((GetKeyboardTrigger(DIK_RETURN) == true) || (GetJoypadTrigger(JOYKEY_B) == true))
	{
		PlaySound(SOUND_LABEL_SE_ENTER);
		switch (g_SelectModeP)
		{
		case PAUSE_MENU_CONTINUE:
			SetEnablePause();
			break;
		case PAUSE_MENU_RETRY:	//ゲームやり直し
		
			SetMODE(MODE_GAME);
			break;

		case PAUSE_MENU_QUIT:	//タイトルへ戻る

			SetMODE(MODE_TITLE);
			break;
		}
	}

}
//==============================================================
//ポーズ画面の描画処理
//==============================================================
void DrawPause(void)
{
	int nCnt;

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	for (nCnt = 0; nCnt < PAUSE_MENU_MAX; nCnt++)
	{
		pDevice->SetTexture(0, g_pTexturePause[nCnt]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);
	}
}
//==============================================================
// ポーズ選択肢リセット
//==============================================================
void ResetPause(void)
{
	g_SelectModeP = PAUSE_MENU_CONTINUE;
}