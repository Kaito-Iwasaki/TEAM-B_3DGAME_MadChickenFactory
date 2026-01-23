//===========================================================
//
// チームロゴ[team_logo.cpp]
// Author : 土田　青空
//
//===========================================================
#include "main.h"
#include "team_logo.h"

//===========================================================
// 
// マクロ定義
// 
//===========================================================
#define TITLELOGO_WIDTH		(100.0f)	//タイトルロゴの幅
#define TITLEROGO_HEIGHT	(50.0f)	//高さ

//===========================================================
// 
//グローバル変数
// 
//===========================================================
LPDIRECT3DTEXTURE9 g_pTextureTeamLogo = NULL;		//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTeamLogo;		//頂点バッファへのポインタ
D3DXVECTOR3 g_TeamLogopos;								//ロゴの位置
D3DXCOLOR g_TeamLogoCol;


//===========================================================
// チームロゴの初期化処理
//===========================================================
void InitTeamLogo(void)
{
	g_TeamLogopos = D3DXVECTOR3(1000.0f,-600.0f,0.0f);
	
	g_TeamLogoCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\StarInACage.png", &g_pTextureTeamLogo);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTeamLogo,
		NULL);

	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffTeamLogo->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3( g_TeamLogopos.x - TITLELOGO_WIDTH,  g_TeamLogopos.y - TITLEROGO_HEIGHT, 0.0f);//右回りで！
	pVtx[1].pos = D3DXVECTOR3( g_TeamLogopos.x + TITLELOGO_WIDTH,  g_TeamLogopos.y - TITLEROGO_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3( g_TeamLogopos.x - TITLELOGO_WIDTH,  g_TeamLogopos.y + TITLEROGO_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3( g_TeamLogopos.x + TITLELOGO_WIDTH,  g_TeamLogopos.y + TITLEROGO_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = g_TeamLogoCol;
	pVtx[1].col = g_TeamLogoCol;
	pVtx[2].col = g_TeamLogoCol;
	pVtx[3].col = g_TeamLogoCol;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffTeamLogo->Unlock();

}

//===========================================================
// チームロゴの終了処理
//===========================================================
void UninitTeamLogo(void)
{
	//テクスチャの破棄
	if (g_pTextureTeamLogo != NULL)
	{			   
		g_pTextureTeamLogo->Release();
		g_pTextureTeamLogo = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTeamLogo != NULL)
	{			   
		g_pVtxBuffTeamLogo->Release();
		g_pVtxBuffTeamLogo = NULL;
	}
}

//===========================================================
// チームロゴの更新処理
//===========================================================
void UpdateTeamLogo(void)
{
	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffTeamLogo->Lock(0, 0, (void**)&pVtx, 0);

	if (g_TeamLogopos.y < 300.0f)
	{
		g_TeamLogopos.y += 5.0f;
	}
	

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3( g_TeamLogopos.x - TITLELOGO_WIDTH,  g_TeamLogopos.y - TITLEROGO_HEIGHT, 0.0f);//右回りで！
	pVtx[1].pos = D3DXVECTOR3( g_TeamLogopos.x + TITLELOGO_WIDTH,  g_TeamLogopos.y - TITLEROGO_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3( g_TeamLogopos.x - TITLELOGO_WIDTH,  g_TeamLogopos.y + TITLEROGO_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3( g_TeamLogopos.x + TITLELOGO_WIDTH,  g_TeamLogopos.y + TITLEROGO_HEIGHT, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = g_TeamLogoCol;
	pVtx[1].col = g_TeamLogoCol;
	pVtx[2].col = g_TeamLogoCol;
	pVtx[3].col = g_TeamLogoCol;

	//頂点バッファをアンロックする
	g_pVtxBuffTeamLogo->Unlock();
}

//===========================================================
// チームロゴの描画処理
//===========================================================
void DrawTeamLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTeamLogo, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTeamLogo);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
