//=======================================================
//
// 影の描画処理[shadow.cpp]
// Author : Airi Takahashi
//
//=======================================================
#include"shadow.h"
#include"input.h"

// マクロ定義
#define MAX_SHADOW			(512)			// 影の最大数
#define MAX_TEXTURE			(1)				// テクスチャ数
#define MAX_PATTERN			(1)				// 全パターン
#define MAX_PATTERN_X		(1)				// X軸パターン数
#define MAX_PATTERN_Y		(1)				// Y軸パターン数
#define SHADOW_OPACITY		(0.5f)			// 影の不透明度

// グローバル変数
LPDIRECT3DTEXTURE9	g_pTextureShadow[MAX_TEXTURE] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;			// 頂点バッファへのポインタ
Shadow g_aShadow[MAX_SHADOW];								// 影の情報
const char* c_apFilenameShadow[MAX_TEXTURE] =				// テクスチャ読み込み
{
	"data\\TEXTURE\\shadow000.jpg",
};

//=======================================================
// 影の初期化処理
//=======================================================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ
	VERTEX_3D* pVtx;				// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	for (int nCntShadow = 0; nCntShadow < MAX_TEXTURE; nCntShadow++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
									c_apFilenameShadow[nCntShadow],
									&g_pTextureShadow[nCntShadow]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffShadow,
								NULL);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置初期化
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き初期化
		g_aShadow[nCntShadow].fWidth = 0.0f;							// 幅初期化
		g_aShadow[nCntShadow].bUse = false;								// 使用していない状態にする
	}

	// 頂点バッファをロックし、頂点情報へのポインタ取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fWidth / 2.0f, 0.0f, g_aShadow[nCntShadow].fWidth / 2.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].fWidth / 2.0f, 0.0f, g_aShadow[nCntShadow].fWidth / 2.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fWidth / 2.0f, 0.0f, -g_aShadow[nCntShadow].fWidth / 2.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].fWidth / 2.0f, 0.0f, -g_aShadow[nCntShadow].fWidth / 2.0f);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, SHADOW_OPACITY);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, SHADOW_OPACITY);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, SHADOW_OPACITY);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, SHADOW_OPACITY);

		// テクスチャ座標の指定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;		// 頂点データのポインタを4つ進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffShadow->Unlock();
}

//=======================================================
// 影の終了処理
//=======================================================
void UninitShadow(void)
{
	for (int nCntShadow = 0; nCntShadow < MAX_TEXTURE; nCntShadow++)
	{
		// テクスチャの破棄
		if (g_pTextureShadow[nCntShadow] != NULL)
		{
			g_pTextureShadow[nCntShadow]->Release();
			g_pTextureShadow[nCntShadow] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//=======================================================
// 影の更新処理
//=======================================================
void UpdateShadow(void)
{
	
}

//=======================================================
// 影の描画処理
//=======================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// デバイスの取得
	pDevice = GetDevice();

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureShadow[0]);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{// 使用している

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);

			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);

			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

			// 影の描写
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntShadow, 2);
		}
	}

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=======================================================
// 影の設定処理
//=======================================================
int SetShadow(D3DXVECTOR3 pos, float fWidth)
{
	VERTEX_3D* pVtx;				// 頂点情報へのポインタ
	int nShadowNumber = -1;			// 使用する影の番号

	// 頂点バッファをロックし、頂点情報へのポインタ取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == false)
		{// 使用していない

			g_aShadow[nCntShadow].pos = pos;		// 位置
			g_aShadow[nCntShadow].fWidth = fWidth;	// 幅
			nShadowNumber = nCntShadow;				// 番号取得
			g_aShadow[nCntShadow].bUse = true;		// 使用している状態にする
		
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aShadow[nShadowNumber].fWidth / 2.0f, 0.0f, g_aShadow[nShadowNumber].fWidth / 2.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aShadow[nShadowNumber].fWidth / 2.0f, 0.0f, g_aShadow[nShadowNumber].fWidth / 2.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aShadow[nShadowNumber].fWidth / 2.0f, 0.0f, -g_aShadow[nShadowNumber].fWidth / 2.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aShadow[nShadowNumber].fWidth / 2.0f, 0.0f, -g_aShadow[nShadowNumber].fWidth / 2.0f);

			break;
		}

		pVtx += 4;		// 頂点データのポインタを進める

	}

	// 頂点バッファをアンロック
	g_pVtxBuffShadow->Unlock();

	return nShadowNumber;
}

//=======================================================
// 影の位置設定処理
//=======================================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, float fAddWidth)
{
	VERTEX_3D* pVtx;				// 頂点情報へのポインタ

	// 位置更新
	g_aShadow[nIdxShadow].pos.x = pos.x;
	g_aShadow[nIdxShadow].pos.z = pos.z;

	// 影の大きさ更新
	g_aShadow[nIdxShadow].fWidth += fAddWidth;

	// 頂点バッファをロックし、頂点情報へのポインタ取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nIdxShadow;		// 頂点データのポインタを指定の位置に進める

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-g_aShadow[nIdxShadow].fWidth / 2.0f, 0.0f, g_aShadow[nIdxShadow].fWidth / 2.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aShadow[nIdxShadow].fWidth / 2.0f, 0.0f, g_aShadow[nIdxShadow].fWidth / 2.0f);
	pVtx[2].pos = D3DXVECTOR3(-g_aShadow[nIdxShadow].fWidth / 2.0f, 0.0f, -g_aShadow[nIdxShadow].fWidth / 2.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aShadow[nIdxShadow].fWidth / 2.0f, 0.0f, -g_aShadow[nIdxShadow].fWidth / 2.0f);

	// 頂点バッファをアンロック
	g_pVtxBuffShadow->Unlock();
}

//=======================================================
// 影の情報の取得処理
//=======================================================
Shadow *GetShadow(void)
{
	return &g_aShadow[0];
}