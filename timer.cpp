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

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define INIT_TIMER		(300)
#define FRAME_TIMER		(60)		//時間が減るフレーム数
#define MAX_TIMER		(3)			//表示される時間の数

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
int g_aTimer = 0;
int g_nFrame = 0;
LPDIRECT3DTEXTURE9 g_pTextureTimer = NULL;							// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;						// 頂点バッファへのポインタ
Timer g_nTimer[MAX_TIMER];

//=====================================================================
// 初期化処理
//=====================================================================
void InitTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_TIMER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL);
	VERTEX_3D* pVtx;		// 頂点情報へのポインタ
		//テクスチャ4枚分
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_pTextureTimer);
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
		g_nTimer[nCntTimer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_nTimer[nCntTimer].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		g_nTimer[nCntTimer].CountTimer = 300;
		g_nTimer[nCntTimer].Fream = 0;
		g_nTimer[nCntTimer].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_nTimer[nCntTimer].size.x * 0.5f,g_nTimer[nCntTimer].size.y, g_nTimer[nCntTimer].size.z);
		pVtx[1].pos = D3DXVECTOR3( g_nTimer[nCntTimer].size.x * 0.5f,g_nTimer[nCntTimer].size.y, g_nTimer[nCntTimer].size.z);
		pVtx[2].pos = D3DXVECTOR3(-g_nTimer[nCntTimer].size.x * 0.5f, 0.0f,g_nTimer[nCntTimer].size.z);
		pVtx[3].pos = D3DXVECTOR3( g_nTimer[nCntTimer].size.x * 0.5f, 0.0f,g_nTimer[nCntTimer].size.z);

		//法線ベクトル
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);


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
	g_pVtxBuffTimer->Unlock();
	g_aTimer = INIT_TIMER;
	g_nFrame = FRAME_TIMER;
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
	if (g_aTimer > 0)
	{
		
		if (g_nFrame >= 1)
		{//フレームが1以上の時減る

			g_nFrame--;

		}
		else if (g_nFrame <= 0)
		{//フレームが０以下になった時タイマーが減り,フレームが戻る

			g_aTimer--;
			g_nFrame = FRAME_TIMER;

		}
		
	}
	else
	{
		/*SetFade(MODE_RESULT);*/
	}
	VERTEX_3D* pVtx;


	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
		if (g_nTimer[nCntTimer].bUse == true)
		{

			if (g_nTimer[1].Fream <= 179 && g_nTimer->CountTimer >= 1)
			{
				g_nTimer[1].Fream++;
			}
			if (g_nTimer[1].Fream == 180)
			{
				DownTimer(1);

				g_nTimer[1].Fream = 0;
			}

			if (g_nTimer->CountTimer <= 0)
			{
				SetFade(MODE_GAME);
			}

		}
		pVtx += 4;			//頂点データのポインタを4つ進める(忘れない)
	}
	g_pVtxBuffTimer->Unlock();
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawTimer(void)
{
	// タイマーのデバッグ表示
	PrintDebugProc("タイマー:%d\n", g_aTimer);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();				// デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_nTimer[nCntTimer].mtxWorld);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_nTimer[nCntTimer].pos.x, g_nTimer[nCntTimer].pos.y, g_nTimer[nCntTimer].pos.z);

		D3DXMatrixMultiply(&g_nTimer[nCntTimer].mtxWorld, &g_nTimer[nCntTimer].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_nTimer[nCntTimer].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffTimer, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);
		//zテスト無効
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// 描画
		
		// テクスチャの設定
		pDevice->SetTexture(0,g_pTextureTimer);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntTimer * 4,
			2);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // 元に戻す
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	}
}
//======================
//タイマーセット処理
//======================
void SetTimer(D3DXVECTOR3 pos, int CountTimer, D3DXCOLOR col)
{
	int aTexU[MAX_TIMER];
	g_nTimer->CountTimer = g_nTimer->CountTimer;
	VERTEX_3D* pVtx;
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);
	aTexU[0] = g_nTimer[0].CountTimer % 1000 / 100;
	aTexU[1] = g_nTimer[1].CountTimer % 100 / 10;
	aTexU[2] = g_nTimer[2].CountTimer % 10 / 1;
	//テクスチャの更新処理

	for (int  nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
		if (g_nTimer[nCntTimer].bUse == false)
		{
			pVtx[0].tex.x = aTexU[nCntTimer] * 0.1f;
			pVtx[1].tex.x = aTexU[nCntTimer] * 0.1f + 0.1f;
			pVtx[2].tex.x = aTexU[nCntTimer] * 0.1f;
			pVtx[3].tex.x = aTexU[nCntTimer] * 0.1f + 0.1f;

		
			g_nTimer[nCntTimer].pos = pos;

			g_nTimer[nCntTimer].CountTimer = CountTimer;			

			g_nTimer[nCntTimer].col = col;

			pVtx[0].pos = D3DXVECTOR3(g_nTimer[nCntTimer].pos.x + (nCntTimer * 100.0f),         g_nTimer[nCntTimer].pos.y,  g_nTimer[nCntTimer].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_nTimer[nCntTimer].pos.x + (nCntTimer * 100.0f + 100.0f), g_nTimer[nCntTimer].pos.y,  g_nTimer[nCntTimer].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_nTimer[nCntTimer].pos.x + (nCntTimer * 100.0f),         g_nTimer[nCntTimer].pos.y + 200.0f, g_nTimer[nCntTimer].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_nTimer[nCntTimer].pos.x + (nCntTimer * 100.0f + 100.0f), g_nTimer[nCntTimer].pos.y + 200.0f, g_nTimer[nCntTimer].pos.z);

			pVtx[0].col = g_nTimer[nCntTimer].col;
			pVtx[1].col = g_nTimer[nCntTimer].col;
			pVtx[2].col = g_nTimer[nCntTimer].col;
			pVtx[3].col = g_nTimer[nCntTimer].col;

			g_nTimer[nCntTimer].bUse = true;

			break;

		}

		pVtx += 4;
	}

	g_pVtxBuffTimer->Unlock();

}
//===========
//時間増加
//===========
void AddTimer(int nValue)
{
	int aTexU[MAX_TIMER];
	g_nTimer->CountTimer += nValue;
	VERTEX_3D* pVtx;
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);
	aTexU[0] = g_nTimer->CountTimer % 1000 / 100;
	aTexU[1] = g_nTimer->CountTimer % 100 / 10;
	aTexU[2] = g_nTimer->CountTimer % 10 / 1;

	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{

		pVtx[0].tex.x = aTexU[nCntTimer] * 0.1f;
		pVtx[1].tex.x = aTexU[nCntTimer] * 0.1f + 0.1f;
		pVtx[2].tex.x = aTexU[nCntTimer] * 0.1f;
		pVtx[3].tex.x = aTexU[nCntTimer] * 0.1f + 0.1f;

		pVtx += 4;
	}
	g_pVtxBuffTimer->Unlock();
}
//===========
//時間減少
//===========
void DownTimer(int nDown)
{
	int aTexU[MAX_TIMER];
	g_nTimer->CountTimer -= nDown;
	VERTEX_3D* pVtx;
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);
	aTexU[0] = g_nTimer->CountTimer % 1000 / 100;
	aTexU[1] = g_nTimer->CountTimer % 100 / 10;
	aTexU[2] = g_nTimer->CountTimer % 10 / 1;

	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{

		pVtx[0].tex.x = aTexU[nCntTimer] * 0.1f;
		pVtx[1].tex.x = aTexU[nCntTimer] * 0.1f + 0.1f;
		pVtx[2].tex.x = aTexU[nCntTimer] * 0.1f;
		pVtx[3].tex.x = aTexU[nCntTimer] * 0.1f + 0.1f;

		pVtx += 4;
	}
	g_pVtxBuffTimer->Unlock();
}
int GetTimer(void)
{
	return  g_aTimer;
}