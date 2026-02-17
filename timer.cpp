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
#define TIMER_SET_MAX   (3)			// タイマーの個数
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
Timer g_nTimer[TIMER_SET_MAX][MAX_TIMER];

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
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_TIMER * TIMER_SET_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL);
	VERTEX_3D* pVtx;		// 頂点情報へのポインタ
		//テクスチャ4枚分

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);
	for (int nSetTimer = 0; nSetTimer < TIMER_SET_MAX; nSetTimer++)
	{
		for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
		{

			g_nTimer[ nSetTimer][nCntTimer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_nTimer[ nSetTimer][nCntTimer].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			g_nTimer[ nSetTimer][nCntTimer].CountTimer = 300;
			g_nTimer[ nSetTimer][nCntTimer].Fream = 0;
			g_nTimer[nSetTimer][nCntTimer].bUse = false;
			g_nTimer[nSetTimer][nCntTimer].bTimer = false;
			//頂点座標の設定
			//タイマーテクスチャ位置更新
			pVtx[0].pos = D3DXVECTOR3((nCntTimer * 100.0f), -100.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((nCntTimer * 100.0f + 100.0f), -100.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((nCntTimer * 100.0f), 100.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((nCntTimer * 100.0f + 100.0f), 100.0f, 0.0f);

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
			DownTimer(1);
			
		}
		
	}
	else
	{
		/*SetFade(MODE_RESULT);*/
	}
	VERTEX_3D* pVtx;


	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);
	for (int nSetTimer = 0; nSetTimer < TIMER_SET_MAX; nSetTimer++)
	{
		for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
		{
			if (g_nTimer[nSetTimer][nCntTimer].bUse == true)
			{
				
				//タイマーテクスチャ位置更新
				pVtx[0].pos = D3DXVECTOR3((nCntTimer * 100.0f), -100.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3((nCntTimer * 100.0f + 100.0f), -100.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3((nCntTimer * 100.0f), 100.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3((nCntTimer * 100.0f + 100.0f), 100.0f, 0.0f);
				
				//0秒でゲーム再び
				if (g_nTimer[nSetTimer][nCntTimer].CountTimer <= 0)
				{
					/*SetFade(MODE_GAME);*/
				}

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
	// タイマーのデバッグ表示
	PrintDebugProc("タイマー:%d\n", g_aTimer);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();				// デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nSetTimer = 0; nSetTimer < TIMER_SET_MAX; nSetTimer++)
	{
		for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
		{
			if (g_nTimer[nSetTimer][nCntTimer].bUse == true)
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_nTimer[nSetTimer][nCntTimer].mtxWorld);

				// 位置を反映
				D3DXMatrixTranslation(&mtxTrans, g_nTimer[nSetTimer][nCntTimer].pos.x, g_nTimer[nSetTimer][nCntTimer].pos.y, g_nTimer[nSetTimer][nCntTimer].pos.z);

				D3DXMatrixMultiply(&g_nTimer[nSetTimer][nCntTimer].mtxWorld, &g_nTimer[nSetTimer][nCntTimer].mtxWorld, &mtxTrans);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_nTimer[nSetTimer][nCntTimer].mtxWorld);

				// 頂点バッファをデータストリームに設定
				pDevice->SetStreamSource(0, g_pVtxBuffTimer, 0, sizeof(VERTEX_3D));

				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);

				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

				//アルファテスト無効
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
				pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
				pDevice->SetRenderState(D3DRS_ALPHAREF, 128);

				pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

				// テクスチャの設定
				pDevice->SetTexture(0, g_pTextureTimer);
				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
					(nSetTimer) * 4,
					2);

				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // 元に戻す

				pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
				//アルファテスト有効
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
				pDevice->SetRenderState(D3DRS_ALPHAREF, 256);
			}
		}
	}
}
//======================
//タイマーセット処理
//======================
void SetTimer(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	int aTexU[TIMER_SET_MAX][MAX_TIMER];
	
	VERTEX_3D* pVtx;
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);
	for (int nSetTimer = 0; nSetTimer < TIMER_SET_MAX; nSetTimer++)
	{
		g_nTimer[nSetTimer][0].CountTimer = g_nTimer[nSetTimer][0].CountTimer;
		g_nTimer[nSetTimer][1].CountTimer = g_nTimer[nSetTimer][1].CountTimer;
		g_nTimer[nSetTimer][2].CountTimer = g_nTimer[nSetTimer][2].CountTimer;
		aTexU[nSetTimer][0] = g_nTimer[nSetTimer][0].CountTimer % 1000 / 100;
		aTexU[nSetTimer][1] = g_nTimer[nSetTimer][1].CountTimer % 100 / 10;
		aTexU[nSetTimer][2] = g_nTimer[nSetTimer][2].CountTimer % 10 / 1;
		//テクスチャの更新処理

		for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
		{
			if (g_nTimer[nSetTimer][nCntTimer].bUse == false)
			{
				pVtx[0].tex.x = aTexU[nSetTimer][nCntTimer] * 0.1f;
				pVtx[1].tex.x = aTexU[nSetTimer][nCntTimer] * 0.1f + 0.1f;
				pVtx[2].tex.x = aTexU[nSetTimer][nCntTimer] * 0.1f;
				pVtx[3].tex.x = aTexU[nSetTimer][nCntTimer] * 0.1f + 0.1f;


				g_nTimer[nSetTimer][nCntTimer].pos = pos;

				g_nTimer[nSetTimer][nCntTimer].col = col;

				//タイマーテクスチャ位置更新
				pVtx[0].pos = D3DXVECTOR3((nCntTimer * 100.0f), - 100.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3((nCntTimer * 100.0f + 100.0f),- 100.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3((nCntTimer * 100.0f), 100.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3((nCntTimer * 100.0f + 100.0f),100.0f, 0.0f);
				//テクスチャ色
				pVtx[0].col = g_nTimer[nSetTimer][nCntTimer].col;
				pVtx[1].col = g_nTimer[nSetTimer][nCntTimer].col;
				pVtx[2].col = g_nTimer[nSetTimer][nCntTimer].col;
				pVtx[3].col = g_nTimer[nSetTimer][nCntTimer].col;

				g_nTimer[nSetTimer][nCntTimer].bUse = true;

				break;

			}

			
		}
	
	}

	g_pVtxBuffTimer->Unlock();

}
//===========
//時間増加
//===========
void AddTimer(int nValue)
{
	int aTexU[TIMER_SET_MAX][MAX_TIMER];
	
	VERTEX_3D* pVtx;
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);
	for (int nSetTimer = 0; nSetTimer < TIMER_SET_MAX; nSetTimer++)
	{
		g_nTimer[nSetTimer][0].CountTimer += nValue;
		g_nTimer[nSetTimer][1].CountTimer += nValue;
		g_nTimer[nSetTimer][2].CountTimer += nValue;
		aTexU[nSetTimer][0] = g_nTimer[nSetTimer][0].CountTimer % 1000 / 100;
		aTexU[nSetTimer][1] = g_nTimer[nSetTimer][1].CountTimer % 100 / 10;
		aTexU[nSetTimer][2] = g_nTimer[nSetTimer][2].CountTimer % 10 / 1;
		//テクスチャの更新処理


		for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
		{

			pVtx[0].tex.x = aTexU[nSetTimer][nCntTimer] * 0.1f;
			pVtx[1].tex.x = aTexU[nSetTimer][nCntTimer] * 0.1f + 0.1f;
			pVtx[2].tex.x = aTexU[nSetTimer][nCntTimer] * 0.1f;
			pVtx[3].tex.x = aTexU[nSetTimer][nCntTimer] * 0.1f + 0.1f;

			pVtx += 4;
		}
	}
	g_pVtxBuffTimer->Unlock();
}
//===========
//時間減少
//===========
void DownTimer(int nDown)
{
	int aTexU[TIMER_SET_MAX][MAX_TIMER];

	VERTEX_3D* pVtx;
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);
	for (int nSetTimer = 0; nSetTimer < TIMER_SET_MAX; nSetTimer++)
	{
		g_nTimer[nSetTimer][0].CountTimer -= nDown;
		g_nTimer[nSetTimer][1].CountTimer -= nDown;
		g_nTimer[nSetTimer][2].CountTimer -= nDown;
		aTexU[nSetTimer][0] = g_nTimer[nSetTimer][0].CountTimer % 1000 / 100;
		aTexU[nSetTimer][1] = g_nTimer[nSetTimer][1].CountTimer % 100 / 10;
		aTexU[nSetTimer][2] = g_nTimer[nSetTimer][2].CountTimer % 10 / 1;
		//テクスチャの更新処理


		for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
		{

			pVtx[0].tex.x = aTexU[nSetTimer][nCntTimer] * 0.1f;
			pVtx[1].tex.x = aTexU[nSetTimer][nCntTimer] * 0.1f + 0.1f;
			pVtx[2].tex.x = aTexU[nSetTimer][nCntTimer] * 0.1f;
			pVtx[3].tex.x = aTexU[nSetTimer][nCntTimer] * 0.1f + 0.1f;

			pVtx += 4;
		}
	}
	g_pVtxBuffTimer->Unlock();
}
int GetTimer(void)
{
	return  g_aTimer;
}
//===========
//時間設定
//===========
void SetTimerCount(int nTimer)
{

	for (int nSetTimer = 0; nSetTimer < TIMER_SET_MAX; nSetTimer++)
	{
		for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
		{
			if (g_nTimer[nSetTimer][nCntTimer].bTimer == false)
			{
					g_nTimer[nSetTimer][nCntTimer].CountTimer = nTimer;
					g_nTimer[nSetTimer][nCntTimer].bTimer = true;
			}
			
		}
	}
}