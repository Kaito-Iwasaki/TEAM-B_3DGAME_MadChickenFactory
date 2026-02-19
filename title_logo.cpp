//===========================================================
//
// タイトルロゴ[title_logo.cpp]
// Author : 長手　桂太朗
//
//===========================================================
#include "main.h"
#include "title_logo.h"

//===========================================================
// 
// マクロ定義
// 
//===========================================================
#define TITLELOGO_WIDTH		(270.0f)	//タイトルロゴの幅
#define TITLEROGO_HEIGHT	(170.0f)	//高さ

//===========================================================
// 
//グローバル変数
// 
//===========================================================
LPDIRECT3DTEXTURE9 g_pTextureTitleLogo = NULL;		//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleLogo;		//頂点バッファへのポインタ
D3DXVECTOR3 g_Titlepos;								//ロゴの位置
D3DXCOLOR g_TitleLogoCol;
int g_nCounter;



//===========================================================
// タイトルロゴの初期化処理
//===========================================================
void InitTitleLogo(void)
{
	g_Titlepos = D3DXVECTOR3(640.0f,250.0f,0.0f);
	g_nCounter = 1;
	g_TitleLogoCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\title_logo.png", &g_pTextureTitleLogo);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleLogo,
		NULL);

	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_Titlepos.x - TITLELOGO_WIDTH, g_Titlepos.y - TITLEROGO_HEIGHT, 0.0f);//右回りで！
	pVtx[1].pos = D3DXVECTOR3(g_Titlepos.x + TITLELOGO_WIDTH, g_Titlepos.y - TITLEROGO_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Titlepos.x - TITLELOGO_WIDTH, g_Titlepos.y + TITLEROGO_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Titlepos.x + TITLELOGO_WIDTH, g_Titlepos.y + TITLEROGO_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = g_TitleLogoCol;
	pVtx[1].col = g_TitleLogoCol;
	pVtx[2].col = g_TitleLogoCol;
	pVtx[3].col = g_TitleLogoCol;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffTitleLogo->Unlock();

}

//===========================================================
// タイトルロゴの終了処理
//===========================================================
void UninitTitleLogo(void)
{
	//テクスチャの破棄
	if (g_pTextureTitleLogo != NULL)
	{
		g_pTextureTitleLogo->Release();
		g_pTextureTitleLogo = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTitleLogo != NULL)
	{
		g_pVtxBuffTitleLogo->Release();
		g_pVtxBuffTitleLogo = NULL;
	}
}

//===========================================================
// タイトルロゴの更新処理
//===========================================================
void UpdateTitleLogo(void)
{
	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	if (g_Titlepos.y < 250.0f)
	{
		g_Titlepos.y += 5.0f;
	}
	else
	{
		g_nCounter++;
	}

	
	

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_Titlepos.x - TITLELOGO_WIDTH, g_Titlepos.y - TITLEROGO_HEIGHT, 0.0f);//右回りで！
	pVtx[1].pos = D3DXVECTOR3(g_Titlepos.x + TITLELOGO_WIDTH, g_Titlepos.y - TITLEROGO_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Titlepos.x - TITLELOGO_WIDTH, g_Titlepos.y + TITLEROGO_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Titlepos.x + TITLELOGO_WIDTH, g_Titlepos.y + TITLEROGO_HEIGHT, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = g_TitleLogoCol;
	pVtx[1].col = g_TitleLogoCol;
	pVtx[2].col = g_TitleLogoCol;
	pVtx[3].col = g_TitleLogoCol;

	//頂点バッファをアンロックする
	g_pVtxBuffTitleLogo->Unlock();
}

//===========================================================
// タイトルロゴの描画処理
//===========================================================
void DrawTitleLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//アルファテスト無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 128);
	//アルファテスト有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 256);
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTitleLogo);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	
}
