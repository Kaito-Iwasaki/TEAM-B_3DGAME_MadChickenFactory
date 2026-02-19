//=====================================================================
//
// Logo [Logo.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "Logo.h"
#include"sound.h"
#include"fade.h"
#include"input.h"
//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_LOGO (2)		//ロゴの最大数
#define LOGO_WIDTH		(300.0f)	//チームロゴの幅
#define LOGO_HEIGHT	(200.0f)	//高さ
#define LOGO_FREAM (60)
//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DTEXTURE9 g_pTextureLogo[LOGO_MAX] = {};		//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLogo;		//頂点バッファへのポインタ
Logo g_aLogo[MAX_LOGO];
int LogoFream;
//=====================================================================
// 初期化処理
//=====================================================================
void InitLogo(void)
{
	
	  
	g_aLogo[0].pos = D3DXVECTOR3(620.0f, 380.0f, 0.0f);
	  		
	g_aLogo[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	  		
	g_aLogo[0].type = LOGO_BLOOD;
			
	g_aLogo[0].nCountLogo = 200;

	g_aLogo[1].pos = D3DXVECTOR3(620.0f, 380.0f, 0.0f);
	  		
	g_aLogo[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	  		
	g_aLogo[1].type = LOGO_TEAMLOGO;
			
	g_aLogo[1].nCountLogo = 300;
	  

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\blood.png", &g_pTextureLogo[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\teamlogo.png", &g_pTextureLogo[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LOGO,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLogo,
		NULL);

	VERTEX_2D* pVtx;	//頂点情報へのポインタ


	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffLogo->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntLogo = 0; nCntLogo < MAX_LOGO; nCntLogo++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aLogo[nCntLogo].pos.x - LOGO_WIDTH, g_aLogo[nCntLogo].pos.y -LOGO_HEIGHT, 0.0f);//右回りで！
		pVtx[1].pos = D3DXVECTOR3(g_aLogo[nCntLogo].pos.x + LOGO_WIDTH, g_aLogo[nCntLogo].pos.y -LOGO_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aLogo[nCntLogo].pos.x - LOGO_WIDTH, g_aLogo[nCntLogo].pos.y +LOGO_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aLogo[nCntLogo].pos.x + LOGO_WIDTH, g_aLogo[nCntLogo].pos.y +LOGO_HEIGHT, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = g_aLogo[nCntLogo].col;
		pVtx[1].col = g_aLogo[nCntLogo].col;
		pVtx[2].col = g_aLogo[nCntLogo].col;
		pVtx[3].col = g_aLogo[nCntLogo].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffLogo->Unlock();
	LogoFream = LOGO_FREAM;
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitLogo(void)
{
	for (int nCntLogo = 0; nCntLogo < MAX_LOGO; nCntLogo++)
	{

		//テクスチャの破棄
		if (g_pTextureLogo[nCntLogo] != NULL)
		{
			g_pTextureLogo[nCntLogo]->Release();
			g_pTextureLogo[nCntLogo] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffLogo != NULL)
	{
		g_pVtxBuffLogo->Release();
		g_pVtxBuffLogo = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateLogo(void)
{
	
	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffLogo->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntLogo = 0; nCntLogo < MAX_LOGO; nCntLogo++)
	{
		switch (g_aLogo[nCntLogo].type)
		{
		
		case LOGO_BLOOD:
			g_aLogo[nCntLogo].nCountLogo--;
			if (g_aLogo[nCntLogo].nCountLogo == 0)
			{
				g_aLogo[nCntLogo].col.a = 1.0f;
				PlaySound(SOUND_LABEL_SE_BLOOD);
			}
			break;
		case LOGO_TEAMLOGO:
			g_aLogo[nCntLogo].nCountLogo--;
			if (g_aLogo[nCntLogo].col.a != 1.0f)
			{
				g_aLogo[nCntLogo].col.a += 0.01f;
			}
			if (g_aLogo[nCntLogo].nCountLogo == 0 || GetKeyboardTrigger(DIK_P) || GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_A))
			{
				SetFade(MODE_TITLE);
			}
			break;
		default:
			break;
		}
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aLogo[nCntLogo].pos.x - LOGO_WIDTH, g_aLogo[nCntLogo].pos.y - LOGO_HEIGHT, 0.0f);//右回りで！
		pVtx[1].pos = D3DXVECTOR3(g_aLogo[nCntLogo].pos.x + LOGO_WIDTH, g_aLogo[nCntLogo].pos.y - LOGO_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aLogo[nCntLogo].pos.x - LOGO_WIDTH, g_aLogo[nCntLogo].pos.y + LOGO_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aLogo[nCntLogo].pos.x + LOGO_WIDTH, g_aLogo[nCntLogo].pos.y + LOGO_HEIGHT, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = g_aLogo[nCntLogo].col;
		pVtx[1].col = g_aLogo[nCntLogo].col;
		pVtx[2].col = g_aLogo[nCntLogo].col;
		pVtx[3].col = g_aLogo[nCntLogo].col;

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffLogo->Unlock();
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawLogo(void)
{

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLogo, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int nCntLogo = 0; nCntLogo < MAX_LOGO; nCntLogo++)
	{

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureLogo[nCntLogo]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLogo * 4, 2);
	}
}