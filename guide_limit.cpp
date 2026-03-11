//=====================================================================
//
// 距離制限ガイド表示 [guide_limit.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "guide_limit.h"
#include "util.h"
#include "player.h"
#include "goal.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	"data\\TEXTURE\\guide_limit.png"
#define GUIDELIMIT_SIZE_X	(550)
#define GUIDELIMIT_SIZE_Y	(250)
#define GUIDELIMIT_POS_X	SCREEN_CENTER
#define GUIDELIMIT_POS_Y	SCREEN_VCENTER + 200

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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGuideLimit = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffGuideLimit = NULL;

//=====================================================================
// 初期化処理
//=====================================================================
void InitGuide_Limit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGuideLimit,
		NULL
	);

	// テクスチャの生成
	D3DXCreateTextureFromFile(
		pDevice,
		TEXTURE_FILENAME,
		&g_pTexBuffGuideLimit
	);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitGuide_Limit(void)
{
	// 頂点バッファの破棄
	RELEASE(g_pVtxBuffGuideLimit);

	// テクスチャの破棄
	RELEASE(g_pTexBuffGuideLimit);
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateGuide_Limit(void)
{

}

//=====================================================================
// 描画処理
//=====================================================================
void DrawGuide_Limit(void)
{
	Player* pPlayer = GetPlayer();

	// プレイヤーが離れていなかったら表示しない
	// プレイヤーの方でもう位置を補正しちゃってて
	// そっちで使ってた条件式使えないので
	// ヤケクソ気味に判定
	if (fabsf(pPlayer[0].pos.x - pPlayer[1].pos.x) < PLAYER_BETWEEN - 20.0f && GetNumPlayersTouchingGoal() != 1)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	VERTEX_2D* pVtx;

	g_pVtxBuffGuideLimit->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = GUIDELIMIT_POS_X - GUIDELIMIT_SIZE_X * 0.5f;
	pVtx[0].pos.y = GUIDELIMIT_POS_Y - GUIDELIMIT_SIZE_Y * 0.5f;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = GUIDELIMIT_POS_X + GUIDELIMIT_SIZE_X * 0.5f;
	pVtx[1].pos.y = GUIDELIMIT_POS_Y - GUIDELIMIT_SIZE_Y * 0.5f;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = GUIDELIMIT_POS_X - GUIDELIMIT_SIZE_X * 0.5f;
	pVtx[2].pos.y = GUIDELIMIT_POS_Y + GUIDELIMIT_SIZE_Y * 0.5f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = GUIDELIMIT_POS_X + GUIDELIMIT_SIZE_X * 0.5f;
	pVtx[3].pos.y = GUIDELIMIT_POS_Y + GUIDELIMIT_SIZE_Y * 0.5f;
	pVtx[3].pos.z = 0.0f;

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DXCOLOR_WHITE;
	pVtx[1].col = D3DXCOLOR_WHITE;
	pVtx[2].col = D3DXCOLOR_WHITE;
	pVtx[3].col = D3DXCOLOR_WHITE;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	g_pVtxBuffGuideLimit->Unlock();

	pDevice->SetStreamSource(0, g_pVtxBuffGuideLimit, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_pTexBuffGuideLimit);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}