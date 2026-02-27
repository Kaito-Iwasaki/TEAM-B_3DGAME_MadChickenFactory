//=====================================================================
//
// タイマー処理 [timer.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "timer.h"
#include "DebugProc.h"
#include"fade.h"
#include "util.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TIMER_WIDTH		(100.0f)
#define TIMER_HEIGHT	(100.0f)

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** 構造体 *****
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
LPDIRECT3DTEXTURE9 g_pTextureTimer = NULL;			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;		// 頂点バッファへのポインタ
Timer g_aTimer[TIMER_SET_MAX];						// タイマーオブジェクト
int g_nTimerCount = 0;								// 全タイマーのカウント
int g_nCounterStateTimer = 0;

//=====================================================================
// 初期化処理
//=====================================================================
void InitTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();	
	
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_pTextureTimer);
	
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_PLACE * TIMER_SET_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL);

	VERTEX_3D* pVtx;		// 頂点情報へのポインタ
	//テクスチャ4枚分

	g_nTimerCount = 0;
	g_nCounterStateTimer = 0;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nSetTimer = 0; nSetTimer < TIMER_SET_MAX; nSetTimer++)
	{
		g_aTimer[nSetTimer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aTimer[nSetTimer].size = D3DXVECTOR2(TIMER_WIDTH, TIMER_HEIGHT);
		g_aTimer[nSetTimer].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		g_aTimer[nSetTimer].bUse = false;

		for (int nCntTimer = 0; nCntTimer < MAX_PLACE; nCntTimer++)
		{
			//頂点座標の設定
			//タイマーテクスチャ位置更新
			pVtx[0].pos = D3DXVECTOR3_ZERO;
			pVtx[1].pos = D3DXVECTOR3_ZERO;
			pVtx[2].pos = D3DXVECTOR3_ZERO;
			pVtx[3].pos = D3DXVECTOR3_ZERO;

			//法線ベクトル
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 1.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 0.0f);

			pVtx += 4;
		}
	}
	g_pVtxBuffTimer->Unlock();

	SetTimerCount(300);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitTimer(void)
{
	// テクスチャの破棄
	if (g_pTextureTimer != NULL)
	{
		g_pTextureTimer->Release();
		g_pTextureTimer = NULL;
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();

		g_pVtxBuffTimer = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateTimer(void)
{
	VERTEX_3D* pVtx;

	if (g_nTimerCount < 0)
	{
		SetFade(MODE_GAME);
		return;
	}

	if (g_nCounterStateTimer % 60 == 0)
	{
		g_nTimerCount--;
		g_nCounterStateTimer = 0;
	}
	g_nCounterStateTimer++;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	int aTexU[MAX_PLACE] = {};

	for (int nCntTimer = 0; nCntTimer < MAX_PLACE; nCntTimer++)
	{
		float fNum = powf(10.0f, nCntTimer+1);

		aTexU[MAX_PLACE - nCntTimer - 1] = g_nTimerCount % (int)fNum / (int)(fNum / 10.0f);
	}

	for (int nSetTimer = 0; nSetTimer < TIMER_SET_MAX; nSetTimer++)
	{
		for (int nCntTimer = 0; nCntTimer < MAX_PLACE; nCntTimer++)
		{
			if (g_aTimer[nSetTimer].bUse == true)
			{
				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(aTexU[nCntTimer] * 0.1f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(aTexU[nCntTimer] * 0.1f + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(aTexU[nCntTimer] * 0.1f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(aTexU[nCntTimer] * 0.1f + 0.1f, 1.0f);
			}

			pVtx += 4;
		}
	}
	g_pVtxBuffTimer->Unlock();
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();				// デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTimer, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nSetTimer = 0; nSetTimer < TIMER_SET_MAX; nSetTimer++)
	{
		if (g_aTimer[nSetTimer].bUse == false) continue;

		for (int nCntTimer = 0; nCntTimer < MAX_PLACE; nCntTimer++)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aTimer[nSetTimer].mtxWorld);

			MatrixRotationPosition(&g_aTimer[nSetTimer].mtxWorld, D3DXVECTOR3_ZERO, g_aTimer[nSetTimer].pos);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aTimer[nSetTimer].mtxWorld);

			//アルファテスト無効
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureTimer);
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				(nSetTimer * MAX_PLACE + nCntTimer) * 4,
				2);

			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
			//アルファテスト有効
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		}
	}
}
//======================
//タイマーセット処理
//======================
void SetTimer(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	VERTEX_3D* pVtx;
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	int aTexU[MAX_PLACE] = {};

	for (int nCntTimer = 0; nCntTimer < MAX_PLACE; nCntTimer++)
	{
		float fNum = powf(10.0f, nCntTimer + 1);

		aTexU[MAX_PLACE - nCntTimer - 1] = g_nTimerCount % (int)fNum / (int)(fNum / 10.0f);
	}

	for (int nSetTimer = 0; nSetTimer < TIMER_SET_MAX; nSetTimer++)
	{
		if (g_aTimer[nSetTimer].bUse == false)
		{
			g_aTimer[nSetTimer].bUse = true;
			g_aTimer[nSetTimer].pos = pos;
			g_aTimer[nSetTimer].size = size;

			for (int nCntTimer = 0; nCntTimer < MAX_PLACE; nCntTimer++)
			{
				float fStartPosX = -g_aTimer[nSetTimer].size.x * 0.5f;
				float fOffsetX = g_aTimer[nSetTimer].size.x / (float)MAX_PLACE;

				//頂点座標の設定
				//タイマーテクスチャ位置更新
				pVtx[0].pos = D3DXVECTOR3(fStartPosX + fOffsetX * nCntTimer, g_aTimer[nSetTimer].size.y * 0.5f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(fStartPosX + fOffsetX * (nCntTimer + 1), g_aTimer[nSetTimer].size.y * 0.5f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(fStartPosX + fOffsetX * nCntTimer, -g_aTimer[nSetTimer].size.y * 0.5f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(fStartPosX + fOffsetX * (nCntTimer + 1), -g_aTimer[nSetTimer].size.y * 0.5f, 0.0f);

				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(aTexU[nCntTimer] * 0.1f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(aTexU[nCntTimer] * 0.1f + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(aTexU[nCntTimer] * 0.1f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(aTexU[nCntTimer] * 0.1f + 0.1f, 1.0f);

				pVtx += 4;
			}

			break;
		}

		pVtx += 4 * MAX_PLACE;
	}
	g_pVtxBuffTimer->Unlock();
}

//===========
//時間増加
//===========
void AddTimer(int nValue)
{
	//int aTexU[TIMER_SET_MAX][MAX_TIMER];
	//
	//VERTEX_3D* pVtx;
	//g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);
	//for (int nSetTimer = 0; nSetTimer < TIMER_SET_MAX; nSetTimer++)
	//{
	//	g_nTimer[nSetTimer][0].CountTimer += nValue;
	//	g_nTimer[nSetTimer][1].CountTimer += nValue;
	//	g_nTimer[nSetTimer][2].CountTimer += nValue;
	//	aTexU[nSetTimer][0] = g_nTimer[nSetTimer][0].CountTimer % 1000 / 100;
	//	aTexU[nSetTimer][1] = g_nTimer[nSetTimer][1].CountTimer % 100 / 10;
	//	aTexU[nSetTimer][2] = g_nTimer[nSetTimer][2].CountTimer % 10 / 1;
	//	//テクスチャの更新処理


	//	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	//	{

	//		pVtx[0].tex.x = aTexU[nSetTimer][nCntTimer] * 0.1f;
	//		pVtx[1].tex.x = aTexU[nSetTimer][nCntTimer] * 0.1f + 0.1f;
	//		pVtx[2].tex.x = aTexU[nSetTimer][nCntTimer] * 0.1f;
	//		pVtx[3].tex.x = aTexU[nSetTimer][nCntTimer] * 0.1f + 0.1f;

	//		pVtx += 4;
	//	}
	//}
	//g_pVtxBuffTimer->Unlock();
}
//===========
//時間減少
//===========
void DownTimer(int nDown)
{
	//int aTexU[TIMER_SET_MAX][MAX_TIMER];

	//VERTEX_3D* pVtx;
	//g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);
	//for (int nSetTimer = 0; nSetTimer < TIMER_SET_MAX; nSetTimer++)
	//{
	//	g_nTimer[nSetTimer][0].CountTimer -= nDown;
	//	g_nTimer[nSetTimer][1].CountTimer -= nDown;
	//	g_nTimer[nSetTimer][2].CountTimer -= nDown;
	//	aTexU[nSetTimer][0] = g_nTimer[nSetTimer][0].CountTimer % 1000 / 100;
	//	aTexU[nSetTimer][1] = g_nTimer[nSetTimer][1].CountTimer % 100 / 10;
	//	aTexU[nSetTimer][2] = g_nTimer[nSetTimer][2].CountTimer % 10 / 1;
	//	//テクスチャの更新処理


	//	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	//	{
	//		pVtx[0].tex.x = aTexU[nSetTimer][nCntTimer] * 0.1f;
	//		pVtx[1].tex.x = aTexU[nSetTimer][nCntTimer] * 0.1f + 0.1f;
	//		pVtx[2].tex.x = aTexU[nSetTimer][nCntTimer] * 0.1f;
	//		pVtx[3].tex.x = aTexU[nSetTimer][nCntTimer] * 0.1f + 0.1f;

	//		pVtx += 4;
	//	}
	//}
	//g_pVtxBuffTimer->Unlock();
}
int GetTimer(void)
{
	return  g_nTimerCount;
}
//===========
//時間設定
//===========
void SetTimerCount(int nTimer)
{
	g_nTimerCount = nTimer;
}
