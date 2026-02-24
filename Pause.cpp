//===========================================================
//
// ポーズ画面[Pause.cpp]
// Author : 長手　桂太朗
//
//===========================================================
#include "main.h"
#include "Pause.h"
#include "input.h"
#include "sound.h"
#include "game.h"
#include "sound.h"
#include "util.h"
#include "fade.h"
#include "DebugProc.h"

//===========================================================
// 
//マクロ定義
// 
//===========================================================
#define PA_BG_WIDTH (400)
#define PA_BG_HEIGHT (420)

#define PA_SE_WIDTH (400)
#define PA_SE_HEIGHT (140)
#define MAX_FRAME	(8)		//必要な枠の数

#define PAUSE_BG_PATH			"data\\TEXTURE\\Face-in Panel.png"	//ポーズメニューのBGテクスチャへのパス
#define PAUSE_CONTINUE_PATH		"data\\TEXTURE\\pause000.png"		//CONTINUE
#define PAUSE_RETRY_PATH		"data\\TEXTURE\\pause001.png"		//PAUSE
#define PAUSE_QUIT_PATH			"data\\TEXTURE\\pause002.png"		//QUIT

//===========================================================
// 
//グローバル変数
// 
//===========================================================
LPDIRECT3DTEXTURE9 g_pTexturePause[MAX_FRAME] = {};		//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;		//頂点バッファへのポインタ
int g_SelectModeP = 0;		//現在選択しているメニュー
bool g_bPause;				//ポーズのON/OFF
bool g_bF1;					//F1による表示・非表示

PAUSE g_aPause[MAX_FRAME] =
{
	{D3DXVECTOR3(640.0f,320.0f,0.0f),300.0f,300.0f},
	{D3DXVECTOR3(640.0f,320.0f,0.0f),300.0f,100.0f},
	{D3DXVECTOR3(640.0f,420.0f,0.0f),300.0f,100.0f},
	{D3DXVECTOR3(640.0f,520.0f,0.0f),300.0f,100.0f}
};

const char g_pTextureName[MAX_FRAME][MAX_PATH] =
{
	{PAUSE_BG_PATH},
	{PAUSE_CONTINUE_PATH},
	{PAUSE_RETRY_PATH},
	{PAUSE_QUIT_PATH}
};


//==============================================================
//ポーズ画面の初期化処理
//==============================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_FRAME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	g_bF1 = false;
	g_bPause = false;
	g_SelectModeP = PAUSE_MENU_CONTINUE;	//選択番号

	for (int nCntPause = 0; nCntPause < MAX_FRAME; nCntPause++)
	{
		D3DXCreateTextureFromFile(pDevice, g_pTextureName[nCntPause], &g_pTexturePause[nCntPause]);

		g_aPause[nCntPause].col = D3DXCOLOR_WHITE;
		g_aPause[nCntPause].bUse = true;

		pVtx[0].pos = D3DXVECTOR3(g_aPause[nCntPause].pos.x - g_aPause[nCntPause].fWidth,
			g_aPause[nCntPause].pos.y - g_aPause[nCntPause].fHeight,
			g_aPause[nCntPause].pos.z);

		pVtx[1].pos = D3DXVECTOR3(g_aPause[nCntPause].pos.x + g_aPause[nCntPause].fWidth,
			g_aPause[nCntPause].pos.y - g_aPause[nCntPause].fHeight,
			g_aPause[nCntPause].pos.z);

		pVtx[2].pos = D3DXVECTOR3(g_aPause[nCntPause].pos.x - g_aPause[nCntPause].fWidth,
			g_aPause[nCntPause].pos.y + g_aPause[nCntPause].fHeight,
			g_aPause[nCntPause].pos.z);

		pVtx[3].pos = D3DXVECTOR3(g_aPause[nCntPause].pos.x + g_aPause[nCntPause].fWidth,
			g_aPause[nCntPause].pos.y + g_aPause[nCntPause].fHeight,
			g_aPause[nCntPause].pos.z);
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = g_aPause[nCntPause].col;
		pVtx[1].col = g_aPause[nCntPause].col;
		pVtx[2].col = g_aPause[nCntPause].col;
		pVtx[3].col = g_aPause[nCntPause].col;

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
	for (nCnt = 0; nCnt < MAX_FRAME; nCnt++)
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
	PrintDebugProc("F1 : ポーズメニュー非表示\n");

	if (GetFade().state != FADESTATE_NONE) return;

	if (GetKeyboardTrigger(DIK_F1))
	{
		g_bF1 ^= true;
	}

	if (GetKeyboardTrigger(DIK_P) || GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_A))
	{//セレクト
		if (g_SelectModeP == PAUSE_MENU_CONTINUE)
		{
			SwitchPause();
		}
		else if (g_SelectModeP == PAUSE_MENU_RETRY)
		{
			SetFade(MODE_GAME);
		}
		else if (g_SelectModeP == PAUSE_MENU_QUIT)
		{
			SetFade(MODE_LOGO);
		}
		g_bF1 = false;
	}
	else if (GetKeyboardTrigger(DIK_S) || GetKeyboardTrigger(DIK_DOWN) || GetJoystickTrigger(JOYSTICK_L_DOWN))
	{//選択フレームを下に
		g_SelectModeP++;

		if (g_SelectModeP >= PAUSE_MENU_MAX)
		{//選択フレームが下限
			g_SelectModeP = PAUSE_MENU_CONTINUE;
		}
	}
	else if (GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_UP) || GetJoystickTrigger(JOYSTICK_L_UP))
	{//選択フレームを上に
		g_SelectModeP--;

		if (g_SelectModeP <= PAUSE_MENU_BG)
		{//選択フレームが上限
			g_SelectModeP = PAUSE_MENU_QUIT;
		}
	}

	//=============================================
	// フレームの透明度設定
	//=============================================
	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4;

	for (int nCntPause = 1; nCntPause < MAX_FRAME; nCntPause++)
	{
		if (g_SelectModeP != nCntPause)
		{//選択中で無いものをfalse
			g_aPause[nCntPause].bUse = false;
		}
		else
		{
			g_aPause[nCntPause].bUse = true;
		}

		if (g_aPause[nCntPause].bUse == true)
		{
			g_aPause[nCntPause].col.a = 1.0f;
		}
		else
		{
			g_aPause[nCntPause].col.a = 0.4f;
		}

		//頂点カラーの設定
		pVtx[0].col = g_aPause[nCntPause].col;
		pVtx[1].col = g_aPause[nCntPause].col;
		pVtx[2].col = g_aPause[nCntPause].col;
		pVtx[3].col = g_aPause[nCntPause].col;

		pVtx += 4;

	}
	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

}
//==============================================================
//ポーズ画面の描画処理
//==============================================================
void DrawPause(void)
{
	if (g_bPause == true && g_bF1 == false)
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
}
//==============================================================
// ポーズ情報取得
//==============================================================
bool GetPause(void)
{
	return g_bPause;
}
//==============================================================
// ポーズ切替
//==============================================================
void SwitchPause(void)
{
	g_bPause ^= true;
}