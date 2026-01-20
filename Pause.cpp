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

//===========================================================
// 
//マクロ定義
// 
//===========================================================
#define PA_BG_WIDTH (400)
#define PA_BG_HEIGHT (420)

#define PA_SE_WIDTH (400)
#define PA_SE_HEIGHT (140)

//===========================================================
// 
//グローバル変数
// 
//===========================================================
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

	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

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