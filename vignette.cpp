//===========================================================
//
// ビネット[vignette.cpp]
// Author : 長手　桂太朗
//
//===========================================================
#include "main.h"
#include "vignette.h"

//===========================================================
// 
// マクロ定義
// 
//===========================================================
#define VIGNETTE_WIDTH		(270.0f)	//ビネットの幅
#define VIGNETTE_HEIGHT	(170.0f)	//高さ

//===========================================================
// 
//グローバル変数
// 
//===========================================================
LPDIRECT3DTEXTURE9 g_pTextureVignette = NULL;		//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffVignette;		//頂点バッファへのポインタ
D3DXVECTOR3 g_Vignettepos;								//ビネットの位置
D3DXCOLOR g_VignetteCol;
int g_nCounter;
bool ba;											//アルファ値のON/OFF
bool bEnter;										//エンターを押したか

//===========================================================
// ビネットの初期化処理
//===========================================================
void InitVignette(void)
{
	g_Vignettepos = D3DXVECTOR3(640.0f,-600.0f,0.0f);
	g_nCounter = 1;
	g_VignetteCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	bEnter = false;
	ba = true;

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\vignette.png", &g_pTextureVignette);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffVignette,
		NULL);

	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffVignette->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_Vignettepos.x - VIGNETTE_WIDTH, g_Vignettepos.y - VIGNETTE_HEIGHT, 0.0f);//右回りで！
	pVtx[1].pos = D3DXVECTOR3(g_Vignettepos.x + VIGNETTE_WIDTH, g_Vignettepos.y - VIGNETTE_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Vignettepos.x - VIGNETTE_WIDTH, g_Vignettepos.y + VIGNETTE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Vignettepos.x + VIGNETTE_WIDTH, g_Vignettepos.y + VIGNETTE_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = g_VignetteCol;
	pVtx[1].col = g_VignetteCol;
	pVtx[2].col = g_VignetteCol;
	pVtx[3].col = g_VignetteCol;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffVignette->Unlock();

}

//===========================================================
// ビネットの終了処理
//===========================================================
void UninitVignette(void)
{
	//テクスチャの破棄
	if (g_pTextureVignette != NULL)
	{
		g_pTextureVignette->Release();
		g_pTextureVignette = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffVignette != NULL)
	{
		g_pVtxBuffVignette->Release();
		g_pVtxBuffVignette = NULL;
	}
}

//===========================================================
// ビネットの更新処理
//===========================================================
void UpdateVignette(void)
{
	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffVignette->Lock(0, 0, (void**)&pVtx, 0);

	if (g_Vignettepos.y < 250.0f)
	{
		g_Vignettepos.y += 5.0f;
	}
	else
	{
		g_nCounter++;
	}

	if (((g_nCounter % 60) == 0) && bEnter == false)
	{
		if (ba == true)
		{
			g_VignetteCol.a = 0.5f;
			ba = false;
		}
		else
		{
			g_VignetteCol.a = 1.0f;
			ba = true;
		}
	}
	else if (bEnter == true && (g_nCounter % 10) == 0)
	{
		if (ba == true)
		{
			g_VignetteCol.a = 0.5f;
			ba = false;
		}
		else
		{
			g_VignetteCol.a = 1.0f;
			ba = true;
		}
	}

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_Vignettepos.x - VIGNETTE_WIDTH, g_Vignettepos.y - VIGNETTE_HEIGHT, 0.0f);//右回りで！
	pVtx[1].pos = D3DXVECTOR3(g_Vignettepos.x + VIGNETTE_WIDTH, g_Vignettepos.y - VIGNETTE_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Vignettepos.x - VIGNETTE_WIDTH, g_Vignettepos.y + VIGNETTE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Vignettepos.x + VIGNETTE_WIDTH, g_Vignettepos.y + VIGNETTE_HEIGHT, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = g_VignetteCol;
	pVtx[1].col = g_VignetteCol;
	pVtx[2].col = g_VignetteCol;
	pVtx[3].col = g_VignetteCol;

	//頂点バッファをアンロックする
	g_pVtxBuffVignette->Unlock();
}

//===========================================================
// ビネットの描画処理
//===========================================================
void DrawVignette(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffVignette, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureVignette);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
