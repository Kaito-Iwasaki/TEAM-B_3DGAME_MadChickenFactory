//=======================================================
//
// コンベアの処理[conveyer.cpp]
// Author : Airi Takahashi
//
//=======================================================
#include"conveyer.h"
#include"input.h"
#include"prompt.h"

// マクロ定義
#define MAX_TEXTURE			(1)				// テクスチャ数
#define MAX_PATTERN			(1)				// 全パターン
#define MAX_PATTERN_X		(1)				// X軸パターン数
#define MAX_PATTERN_Y		(1)				// Y軸パターン数

// グローバル変数
LPDIRECT3DTEXTURE9	g_pTextureConveyer[MAX_TEXTURE] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffConveyer = NULL;				// 頂点バッファへのポインタ
Conveyer g_aConveyer[MAX_CONVEYER];								// コンベア情報
const char* c_apFilenameConveyer[MAX_TEXTURE] =					// テクスチャ読み込み
{
	"data\\TEXTURE\\beltconveyor.png",
};

//=======================================================
// コンベアの初期化処理
//=======================================================
void InitConveyer(void)
{
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ
	VERTEX_3D* pVtx;				// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	for (int nCntConveyer = 0; nCntConveyer < MAX_TEXTURE; nCntConveyer++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
									c_apFilenameConveyer[nCntConveyer],
									&g_pTextureConveyer[nCntConveyer]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_CONVEYER,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffConveyer,
								NULL);

	for (int nCntConveyer = 0; nCntConveyer < MAX_CONVEYER; nCntConveyer++)
	{
		g_aConveyer[nCntConveyer].nIdx = -1;								// インデックス初期化
		g_aConveyer[nCntConveyer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置初期化
		g_aConveyer[nCntConveyer].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 角度初期化
		g_aConveyer[nCntConveyer].Onmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量初期化
		g_aConveyer[nCntConveyer].Offmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量初期化
		g_aConveyer[nCntConveyer].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズ初期化
		g_aConveyer[nCntConveyer].movetex = 0.0f;							// テクスチャ移動量初期化
		g_aConveyer[nCntConveyer].nIdx = -1;								// インデックス初期化
		g_aConveyer[nCntConveyer].bUse = false;								// 使用していない状態にする
	}


	// 頂点バッファをロックし、頂点情報へのポインタ取得
	g_pVtxBuffConveyer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntConveyer = 0; nCntConveyer < MAX_CONVEYER; nCntConveyer++)
	{

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aConveyer[nCntConveyer].size.x * 0.5f, 0.0f, g_aConveyer[nCntConveyer].size.z * 0.5f);
		pVtx[1].pos = D3DXVECTOR3(g_aConveyer[nCntConveyer].size.x * 0.5f, 0.0f, g_aConveyer[nCntConveyer].size.z * 0.5f);
		pVtx[2].pos = D3DXVECTOR3(-g_aConveyer[nCntConveyer].size.x * 0.5f, 0.0f, -g_aConveyer[nCntConveyer].size.z * 0.5f);
		pVtx[3].pos = D3DXVECTOR3(g_aConveyer[nCntConveyer].size.x * 0.5f, 0.0f, -g_aConveyer[nCntConveyer].size.z * 0.5f);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の指定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;		// 頂点データのポインタを4つ進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffConveyer->Unlock();
}

//=======================================================
// コンベアの終了処理
//=======================================================
void UninitConveyer(void)
{
	for (int nCntConveyer = 0; nCntConveyer < MAX_TEXTURE; nCntConveyer++)
	{
		// テクスチャの破棄
		if (g_pTextureConveyer[nCntConveyer] != NULL)
		{
			g_pTextureConveyer[nCntConveyer]->Release();
			g_pTextureConveyer[nCntConveyer] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffConveyer != NULL)
	{
		g_pVtxBuffConveyer->Release();
		g_pVtxBuffConveyer = NULL;
	}
}

//=======================================================
// コンベアの更新処理
//=======================================================
void UpdateConveyer(void)
{
	VERTEX_3D* pVtx;				// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタ取得
	g_pVtxBuffConveyer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntConveyer = 0; nCntConveyer < MAX_CONVEYER; nCntConveyer++)
	{
		if (g_aConveyer[nCntConveyer].bUse == true)
		{// 使用している

			switch (g_aConveyer[nCntConveyer].state)
			{
			case CONVEYERSTATE_MOVE:		// 稼働状態

				// テクスチャを移動させる
				g_aConveyer[nCntConveyer].movetex += (g_aConveyer[nCntConveyer].Onmove.x + g_aConveyer[nCntConveyer].Onmove.z) * 0.002f;

				if (GetPromptTrigger(g_aConveyer[nCntConveyer].nIdx) == true)
				{// 指定のインデックスのプロンプトに反応があった

					// 稼働停止
					g_aConveyer[nCntConveyer].state = CONVEYERSTATE_STOP;
				}

				break;

			case CONVEYERSTATE_STOP:		// 停止状態

				// テクスチャを移動させる
				g_aConveyer[nCntConveyer].movetex += (g_aConveyer[nCntConveyer].Offmove.x + g_aConveyer[nCntConveyer].Offmove.z) * 0.002f;

				if (GetPromptTrigger(g_aConveyer[nCntConveyer].nIdx) == true)
				{// 指定のインデックスのプロンプトに反応があった

					// 稼働再開
					g_aConveyer[nCntConveyer].state = CONVEYERSTATE_MOVE;
				}

				break;
			}

			// テクスチャ座標の指定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f + g_aConveyer[nCntConveyer].movetex);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f + g_aConveyer[nCntConveyer].movetex);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f + g_aConveyer[nCntConveyer].movetex);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f + g_aConveyer[nCntConveyer].movetex);

		}

		pVtx += 4;		// 頂点データのポインタを4つ進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffConveyer->Unlock();

}

//=======================================================
// コンベアの描画処理
//=======================================================
void DrawConveyer(void)
{
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffConveyer, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureConveyer[0]);

	for (int nCntConveyer = 0; nCntConveyer < MAX_CONVEYER; nCntConveyer++)
	{
		if (g_aConveyer[nCntConveyer].bUse == true)
		{// 使用している

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aConveyer[nCntConveyer].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_aConveyer[nCntConveyer].rot.y, g_aConveyer[nCntConveyer].rot.x, g_aConveyer[nCntConveyer].rot.z);

			D3DXMatrixMultiply(&g_aConveyer[nCntConveyer].mtxWorld, &g_aConveyer[nCntConveyer].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_aConveyer[nCntConveyer].pos.x, g_aConveyer[nCntConveyer].pos.y, g_aConveyer[nCntConveyer].pos.z);

			D3DXMatrixMultiply(&g_aConveyer[nCntConveyer].mtxWorld, &g_aConveyer[nCntConveyer].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aConveyer[nCntConveyer].mtxWorld);

			// コンベアの描写
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntConveyer, 2);
		}
	}
}

//=======================================================
// コンベアの設定処理
//=======================================================
void SetConveyer(int nIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Onmove, D3DXVECTOR3 Offmove, D3DXVECTOR3 size, CONVEYERSTATE state)
{
	VERTEX_3D* pVtx;				// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタ取得
	g_pVtxBuffConveyer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntConveyer = 0; nCntConveyer < MAX_CONVEYER; nCntConveyer++)
	{
		if (g_aConveyer[nCntConveyer].bUse == false)
		{// コンベアの設置

			g_aConveyer[nCntConveyer].nIdx = nIdx;			// インデックス
			g_aConveyer[nCntConveyer].pos = pos;			// 位置
			g_aConveyer[nCntConveyer].rot = rot;			// 向き
			g_aConveyer[nCntConveyer].Onmove = Onmove;		// On状態の移動量
			g_aConveyer[nCntConveyer].Offmove = Offmove;	// Off状態の移動量
			g_aConveyer[nCntConveyer].size = size;			// サイズ
			g_aConveyer[nCntConveyer].state = state;		// 状態
			g_aConveyer[nCntConveyer].bUse = true;			// 使用している状態にする

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aConveyer[nCntConveyer].size.x * 0.5f, 0.0f, g_aConveyer[nCntConveyer].size.z * 0.5f);
			pVtx[1].pos = D3DXVECTOR3(g_aConveyer[nCntConveyer].size.x * 0.5f, 0.0f, g_aConveyer[nCntConveyer].size.z * 0.5f);
			pVtx[2].pos = D3DXVECTOR3(-g_aConveyer[nCntConveyer].size.x * 0.5f, 0.0f, -g_aConveyer[nCntConveyer].size.z * 0.5f);
			pVtx[3].pos = D3DXVECTOR3(g_aConveyer[nCntConveyer].size.x * 0.5f, 0.0f, -g_aConveyer[nCntConveyer].size.z * 0.5f);

			break;
		}

		pVtx += 4;		// 頂点データのポインタを4つ進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffConveyer->Unlock();
}
//=======================================================
// コンベアとの当たり判定処理
//=======================================================
bool CollisioncConveyer(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove)
{
	Conveyer* pConveyer = &g_aConveyer[0];					// コンベア情報へのポインタ
	bool bLand = false;										// 着地したかどうか

	for (int nCntComveyer = 0; nCntComveyer < MAX_CONVEYER; nCntComveyer++, pConveyer++)
	{
		if (pConveyer->bUse == true)
		{// 使用している

			if (pPos->x >= pConveyer->pos.x - pConveyer->size.x * 0.5f
				&& pPos->x <= pConveyer->pos.x + pConveyer->size.x * 0.5f
				&& pPos->z >= pConveyer->pos.z - pConveyer->size.z * 0.5f
				&& pPos->z <= pConveyer->pos.z + pConveyer->size.z * 0.5f)
			{// 範囲内

				if (pPosOld->y >= pConveyer->pos.y && pPos->y < pConveyer->pos.y)
				{// 地面にめり込んだ

					bLand = true;
					pPos->y = pConveyer->pos.y;		// y軸の位置修正
					pMove->y = 0.0f;				// y軸の移動量初期化

					if (pConveyer->state == CONVEYERSTATE_MOVE)
					{// コンベア稼働中

						// 移動量分移動させる
						*pPos += pConveyer->Onmove;
					}
					else
					{// コンベア停止中

						// 移動量分移動させる
						*pPos += pConveyer->Offmove;
					}
				}
			}
		}
	}

	return bLand;
}