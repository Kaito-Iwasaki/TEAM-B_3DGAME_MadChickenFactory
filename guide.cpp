//=====================================================================
//
// guide [guide.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "Guide.h"
#include "team_logo.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define GUIDE_SIZE			(150)
#define GUIDE_POS_START		D3DXVECTOR3(80, SCREEN_HEIGHT - (GUIDE_SIZE * 0.5f + 10), 0)
#define GUIDE_OFFSET_X		(GUIDE_SIZE * 0.5f + 20)

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
typedef enum
{
	GUIDETYPE_SWITCH = 0,
	GUIDETYPE_FOLLOW,
	GUIDETYPE_MAX
}GUIDETYPE;

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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGuide = NULL;
LPDIRECT3DTEXTURE9 g_apTexBuffGuide[GUIDETYPE_MAX] = {};
GUIDE g_aGuide[GUIDETYPE_MAX] = {};

const char* g_aFilenameGuideTexture[GUIDETYPE_MAX] = {
	"data\\TEXTURE\\guide000.png",
	"data\\TEXTURE\\guide001.png",
};

//=====================================================================
// 初期化処理
//=====================================================================
void InitGuide(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GUIDE* pGuide = &g_aGuide[0];

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * GUIDETYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGuide,
		NULL
	);

	// テクスチャの生成
	for (int nTexture = 0; nTexture < GUIDETYPE_MAX; nTexture++)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			&g_aFilenameGuideTexture[nTexture][0],
			&g_apTexBuffGuide[nTexture]
		);
	}

	// 構造体の初期化
	ZeroMemory(&g_aGuide[0], sizeof g_aGuide);
	for (int nGuide = 0; nGuide < GUIDETYPE_MAX; nGuide++, pGuide++)
	{
		pGuide->transform.pos = GUIDE_POS_START + D3DXVECTOR3(GUIDE_OFFSET_X * nGuide, 0, 0);
		pGuide->transform.size = D3DXVECTOR3(GUIDE_SIZE, GUIDE_SIZE, 0);
		pGuide->color = D3DXCOLOR_WHITE;
	}
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitGuide(void)
{
	// 頂点バッファの破棄
	RELEASE(g_pVtxBuffGuide);

	// テクスチャの破棄
	for (int nTexture = 0; nTexture < GUIDETYPE_MAX; nTexture++)
	{
		RELEASE(g_apTexBuffGuide[nTexture]);
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateGuide(void)
{

}

//=====================================================================
// 描画処理
//=====================================================================
void DrawGuide(void)
{
	// 1Pモード以外では必要ないので表示しない
	if (GetTitle() != 0)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	VERTEX_2D* pVtx;
	GUIDE* pGuide = &g_aGuide[0];

	g_pVtxBuffGuide->Lock(0, 0, (void**)&pVtx, 0);

	for (int nGuide = 0; nGuide < GUIDETYPE_MAX; nGuide++, pGuide++)
	{
		pVtx[0].pos.x = pGuide->transform.pos.x - pGuide->transform.size.x * 0.5f;
		pVtx[0].pos.y = pGuide->transform.pos.y - pGuide->transform.size.y * 0.5f;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pGuide->transform.pos.x + pGuide->transform.size.x * 0.5f;
		pVtx[1].pos.y = pGuide->transform.pos.y - pGuide->transform.size.y * 0.5f;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pGuide->transform.pos.x - pGuide->transform.size.x * 0.5f;
		pVtx[2].pos.y = pGuide->transform.pos.y + pGuide->transform.size.y * 0.5f;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pGuide->transform.pos.x + pGuide->transform.size.x * 0.5f;
		pVtx[3].pos.y = pGuide->transform.pos.y + pGuide->transform.size.y * 0.5f;
		pVtx[3].pos.z = 0.0f;

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = pGuide->color;
		pVtx[1].col = pGuide->color;
		pVtx[2].col = pGuide->color;
		pVtx[3].col = pGuide->color;

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffGuide->Unlock();

	pDevice->SetStreamSource(0, g_pVtxBuffGuide, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nGuide = 0; nGuide < GUIDETYPE_MAX; nGuide++)
	{
		pDevice->SetTexture(0, g_apTexBuffGuide[nGuide]);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nGuide * 4, 2);
	}
}