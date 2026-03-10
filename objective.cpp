//=====================================================================
//
// 目標表示 [objective.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "objective.h"
#include "Checkpoint.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define OBJECTIVE_TEXTURE_FILENAME	"data\\TEXTURE\\objective000.png"
#define OBJECTIVE_SIZE_X	(1000)
#define OBJECTIVE_SIZE_Y	(200)
#define OBJECTIVE_POS_X		(SCREEN_CENTER)
#define OBJECTIVE_POS_Y		(SCREEN_VCENTER)
#define OBJECTIVE_FADE_START	(60 * 5)

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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffObjective = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffObjective = NULL;
OBJECTIVE g_objective;

//=====================================================================
// 初期化処理
//=====================================================================
void InitObjective(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffObjective,
		NULL
	);

	// テクスチャの生成
	D3DXCreateTextureFromFile(
		pDevice,
		OBJECTIVE_TEXTURE_FILENAME,
		&g_pTexBuffObjective
	);

	// 構造体の初期化
	ZeroMemory(&g_objective, sizeof(OBJECTIVE));
	g_objective.transform.pos = D3DXVECTOR3(OBJECTIVE_POS_X, OBJECTIVE_POS_Y, 0);
	g_objective.transform.size = D3DXVECTOR3(OBJECTIVE_SIZE_X, OBJECTIVE_SIZE_Y, 0);
	g_objective.color = D3DXCOLOR_WHITE;
	g_objective.nCounterState = 0;

	if (GetCurrentCheckpoint() == 0)
	{
		g_objective.color = D3DXCOLOR_WHITE;
	}
	else
	{
		g_objective.color = D3DXCOLOR_ZERO;
	}
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitObjective(void)
{
	// 頂点バッファの破棄
	RELEASE(g_pVtxBuffObjective);

	// テクスチャの破棄
	RELEASE(g_pTexBuffObjective);
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateObjective(void)
{
	if (g_objective.color.a > 0.0f)
	{
		if (g_objective.nCounterState > OBJECTIVE_FADE_START)
		{
			g_objective.color.a -= 0.01f;
		}

		g_objective.nCounterState++;
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawObjective(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;

	g_pVtxBuffObjective->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = g_objective.transform.pos.x - g_objective.transform.size.x * 0.5f;
	pVtx[1].pos.x = g_objective.transform.pos.x + g_objective.transform.size.x * 0.5f;
	pVtx[2].pos.x = g_objective.transform.pos.x - g_objective.transform.size.x * 0.5f;
	pVtx[3].pos.x = g_objective.transform.pos.x + g_objective.transform.size.x * 0.5f;

	pVtx[0].pos.y = g_objective.transform.pos.y - g_objective.transform.size.y * 0.5f;
	pVtx[1].pos.y = g_objective.transform.pos.y - g_objective.transform.size.y * 0.5f;
	pVtx[2].pos.y = g_objective.transform.pos.y + g_objective.transform.size.y * 0.5f;
	pVtx[3].pos.y = g_objective.transform.pos.y + g_objective.transform.size.y * 0.5f;

	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.z = 0.0f;

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = g_objective.color;
	pVtx[1].col = g_objective.color;
	pVtx[2].col = g_objective.color;
	pVtx[3].col = g_objective.color;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	g_pVtxBuffObjective->Unlock();

	pDevice->SetStreamSource(0, g_pVtxBuffObjective, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_pTexBuffObjective);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}