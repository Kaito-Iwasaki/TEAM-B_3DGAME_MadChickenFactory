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
#define TITLELOGO_WIDTH		(100.0f)	//チームロゴの幅
#define TITLEROGO_HEIGHT	(60.0f)	//高さ
#define MAX_TEAMLOGO (2)				//ロゴの最大数
//===========================================================
// 
//グローバル変数
// 
//===========================================================
LPDIRECT3DTEXTURE9 g_pTextureTeamLogo[TEAMLOGO_MAX] = {};		//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTeamLogo;		//頂点バッファへのポインタ
TeamLogo g_TeamLogo[MAX_TEAMLOGO];



//===========================================================
// チームロゴの初期化処理
//===========================================================
void InitTeamLogo(void)
{
	//g_TeamLogo[0].pos = D3DXVECTOR3(150.0f,-600.0f,0.0f);
	g_TeamLogo[0].pos = D3DXVECTOR3(150.0f, 620.0f, 0.0f);
	
	g_TeamLogo[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	g_TeamLogo[0].type = TEAMLOGO_LOGO;

	g_TeamLogo[1].pos = D3DXVECTOR3(540.0f, -600.0f, 0.0f);

	g_TeamLogo[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
	g_TeamLogo[1].type = TEAMLOGO_PUESS;


	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\teamlogo.png", &g_pTextureTeamLogo[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\startbutton.png", &g_pTextureTeamLogo[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TEAMLOGO,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTeamLogo,
		NULL);
	
	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffTeamLogo->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntTeamLogo = 0; nCntTeamLogo < MAX_TEAMLOGO; nCntTeamLogo++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_TeamLogo[nCntTeamLogo].pos.x - TITLELOGO_WIDTH, g_TeamLogo[nCntTeamLogo].pos.y - TITLEROGO_HEIGHT, 0.0f);//右回りで！
		pVtx[1].pos = D3DXVECTOR3(g_TeamLogo[nCntTeamLogo].pos.x + TITLELOGO_WIDTH, g_TeamLogo[nCntTeamLogo].pos.y - TITLEROGO_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_TeamLogo[nCntTeamLogo].pos.x - TITLELOGO_WIDTH, g_TeamLogo[nCntTeamLogo].pos.y + TITLEROGO_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_TeamLogo[nCntTeamLogo].pos.x + TITLELOGO_WIDTH, g_TeamLogo[nCntTeamLogo].pos.y + TITLEROGO_HEIGHT, 0.0f);
																							 
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = g_TeamLogo[nCntTeamLogo].col;
		pVtx[1].col = g_TeamLogo[nCntTeamLogo].col;
		pVtx[2].col = g_TeamLogo[nCntTeamLogo].col;
		pVtx[3].col = g_TeamLogo[nCntTeamLogo].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffTeamLogo->Unlock();

}

//===========================================================
// チームロゴの終了処理
//===========================================================
void UninitTeamLogo(void)
{
	for (int nCntTeamLogo = 0; nCntTeamLogo < MAX_TEAMLOGO; nCntTeamLogo++)
	{

		//テクスチャの破棄
		if (g_pTextureTeamLogo[nCntTeamLogo] != NULL)
		{
			g_pTextureTeamLogo[nCntTeamLogo]->Release();
			g_pTextureTeamLogo[nCntTeamLogo] = NULL;
		}
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
	for (int nCntTeamLogo = 0; nCntTeamLogo < MAX_TEAMLOGO; nCntTeamLogo++)
	{
		switch (g_TeamLogo[nCntTeamLogo].type)
		{
		case TEAMLOGO_LOGO:
			/*if (g_TeamLogo[nCntTeamLogo].pos.y < 620.0f)
			{
				g_TeamLogo[nCntTeamLogo].pos.y += 10.0f;
			}*/
			if (g_TeamLogo[nCntTeamLogo].col.a <= 1.0f)
			{
				g_TeamLogo[nCntTeamLogo].col.a += 0.01f;
			}
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_TeamLogo[nCntTeamLogo].pos.x - TITLELOGO_WIDTH, g_TeamLogo[nCntTeamLogo].pos.y - TITLEROGO_HEIGHT, 0.0f);//右回りで！
			pVtx[1].pos = D3DXVECTOR3(g_TeamLogo[nCntTeamLogo].pos.x + TITLELOGO_WIDTH, g_TeamLogo[nCntTeamLogo].pos.y - TITLEROGO_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_TeamLogo[nCntTeamLogo].pos.x - TITLELOGO_WIDTH, g_TeamLogo[nCntTeamLogo].pos.y + TITLEROGO_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_TeamLogo[nCntTeamLogo].pos.x + TITLELOGO_WIDTH, g_TeamLogo[nCntTeamLogo].pos.y + TITLEROGO_HEIGHT, 0.0f);
			break;
		case TEAMLOGO_PUESS:
			if (g_TeamLogo[nCntTeamLogo].pos.y < 550.0f)
			{
				g_TeamLogo[nCntTeamLogo].pos.y += 10.0f;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_TeamLogo[nCntTeamLogo].pos.x - TITLELOGO_WIDTH , g_TeamLogo[nCntTeamLogo].pos.y - TITLEROGO_HEIGHT, 0.0f);//右回りで！
			pVtx[1].pos = D3DXVECTOR3(g_TeamLogo[nCntTeamLogo].pos.x + TITLELOGO_WIDTH + 250.0f, g_TeamLogo[nCntTeamLogo].pos.y - TITLEROGO_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_TeamLogo[nCntTeamLogo].pos.x - TITLELOGO_WIDTH , g_TeamLogo[nCntTeamLogo].pos.y + TITLEROGO_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_TeamLogo[nCntTeamLogo].pos.x + TITLELOGO_WIDTH + 250.0f, g_TeamLogo[nCntTeamLogo].pos.y + TITLEROGO_HEIGHT, 0.0f);
			break;
		default:
			break;
		}
		


		//頂点カラーの設定
		pVtx[0].col = g_TeamLogo[nCntTeamLogo].col;
		pVtx[1].col = g_TeamLogo[nCntTeamLogo].col;
		pVtx[2].col = g_TeamLogo[nCntTeamLogo].col;
		pVtx[3].col = g_TeamLogo[nCntTeamLogo].col;

		pVtx += 4;
	}
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
	for (int nCntTeamLogo = 0; nCntTeamLogo < MAX_TEAMLOGO; nCntTeamLogo++)
	{

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTeamLogo[nCntTeamLogo]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntTeamLogo * 4, 2);
	}
}
