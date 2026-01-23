//=======================================================
//
// メッシュシリンダーの描画処理[meshcylinder.cpp]
// Author : Airi Takahashi
//
//=======================================================
#include"meshcylinder.h"
#include"input.h"

// マクロ定義
#define MAX_MESHCYLINDER	(32)			// シリンダーの最大数
#define MAX_TEXTURE			(1)				// テクスチャ数
#define MAX_PATTERN			(1)				// 全パターン
#define MAX_PATTERN_X		(1)				// X軸パターン数
#define MAX_PATTERN_Y		(1)				// Y軸パターン数

// グローバル変数
LPDIRECT3DTEXTURE9	g_pTextureMeshCylinder[MAX_TEXTURE] = {};		// テクスチャへのポインタ
MeshCylinder g_aMeshCylinder[MAX_MESHCYLINDER];								// メッシュシリンダー情報
const char* c_apFilenameMeshCylinder[MAX_TEXTURE] =					// テクスチャ読み込み
{
	"data\\TEXTURE\\sky.jpg",
};

//=======================================================
// メッシュシリンダーの初期化処理
//=======================================================
void InitMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{
		g_aMeshCylinder[nCntMeshCylinder].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置初期化
		g_aMeshCylinder[nCntMeshCylinder].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 角度初期化
		g_aMeshCylinder[nCntMeshCylinder].fRadius = 0.0f;							// 半径初期化
		g_aMeshCylinder[nCntMeshCylinder].fHeight = 0.0f;							// 高さ初期化
		g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius = 0;						// 円の分割数
		g_aMeshCylinder[nCntMeshCylinder].nDivisionHeight = 0;						// 高さの分割数
		g_aMeshCylinder[nCntMeshCylinder].pVtxBuff = NULL;							// 頂点バッファの初期化
		g_aMeshCylinder[nCntMeshCylinder].pIdxBuff = NULL;							// インデックスバッファの初期化
		g_aMeshCylinder[nCntMeshCylinder].bUse = false;								// 使用していない状態にする
	}

	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_TEXTURE; nCntMeshCylinder++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
									c_apFilenameMeshCylinder[nCntMeshCylinder],
									&g_pTextureMeshCylinder[nCntMeshCylinder]);
	}

	// メッシュシリンダーの設定
	SetMeshCylinder(D3DXVECTOR3(0.0f, -200.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1500.0f, 400.0f, 32, 1);

}

//=======================================================
// メッシュシリンダーの終了処理
//=======================================================
void UninitMeshCylinder(void)
{
	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_TEXTURE; nCntMeshCylinder++)
	{
		// テクスチャの破棄
		if (g_pTextureMeshCylinder[nCntMeshCylinder] != NULL)
		{
			g_pTextureMeshCylinder[nCntMeshCylinder]->Release();
			g_pTextureMeshCylinder[nCntMeshCylinder] = NULL;
		}
	}

	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{
		// 頂点バッファの破棄
		if (g_aMeshCylinder[nCntMeshCylinder].pVtxBuff != NULL)
		{
			g_aMeshCylinder[nCntMeshCylinder].pVtxBuff->Release();
			g_aMeshCylinder[nCntMeshCylinder].pVtxBuff = NULL;
		}

		// インデックスバッファの破棄
		if (g_aMeshCylinder[nCntMeshCylinder].pIdxBuff != NULL)
		{
			g_aMeshCylinder[nCntMeshCylinder].pIdxBuff->Release();
			g_aMeshCylinder[nCntMeshCylinder].pIdxBuff = NULL;
		}
	}
}

//=======================================================
// メッシュシリンダーの更新処理
//=======================================================
void UpdateMeshCylinder(void)
{
	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{
		if (g_aMeshCylinder[nCntMeshCylinder].bUse == true)
		{
			g_aMeshCylinder[nCntMeshCylinder].rot.y += 0.0001f;

			// 角度の補正
			AngleCorrection(&g_aMeshCylinder[nCntMeshCylinder].rot.y);
		}
	}
}

//=======================================================
// メッシュシリンダーの描画処理
//=======================================================
void DrawMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshCylinder[0]);

	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{
		if (g_aMeshCylinder[nCntMeshCylinder].bUse == true)
		{
			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_aMeshCylinder[nCntMeshCylinder].pVtxBuff, 0, sizeof(VERTEX_3D));

			// インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_aMeshCylinder[nCntMeshCylinder].pIdxBuff);

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMeshCylinder[nCntMeshCylinder].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_aMeshCylinder[nCntMeshCylinder].rot.y, g_aMeshCylinder[nCntMeshCylinder].rot.x, g_aMeshCylinder[nCntMeshCylinder].rot.z);

			D3DXMatrixMultiply(&g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_aMeshCylinder[nCntMeshCylinder].pos.x, g_aMeshCylinder[nCntMeshCylinder].pos.y, g_aMeshCylinder[nCntMeshCylinder].pos.z);

			D3DXMatrixMultiply(&g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshCylinder[nCntMeshCylinder].mtxWorld);

			// ポリゴンの描写
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
				0,
				0,
				((g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius + 1) * (g_aMeshCylinder[nCntMeshCylinder].nDivisionHeight + 1)),
				0,
				(g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius * g_aMeshCylinder[nCntMeshCylinder].nDivisionHeight * 2) + ((g_aMeshCylinder[nCntMeshCylinder].nDivisionHeight - 1) * 4));
		}
	}
}

//=======================================================
// メッシュシリンダーの設定処理
//=======================================================
void SetMeshCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fHeight, int nDivisionRadius, int nDivisionHeight)
{
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ
	VERTEX_3D* pVtx;				// 頂点情報へのポインタ
	WORD* pIdx;						// インデックス情報へのポインタ
	int nCntAdd = 0;				// 加算量
	D3DXVECTOR3 vecCylinder;		// シリンダーの方向ベクトル

	// デバイスの取得
	pDevice = GetDevice();

	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{
		if (g_aMeshCylinder[nCntMeshCylinder].bUse == false)
		{

			// 各値の設定
			g_aMeshCylinder[nCntMeshCylinder].pos = pos;								// 位置
			g_aMeshCylinder[nCntMeshCylinder].rot = rot;								// 向き
			g_aMeshCylinder[nCntMeshCylinder].fRadius = fRadius;						// 半径
			g_aMeshCylinder[nCntMeshCylinder].fHeight = fHeight;						// 高さ
			g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius = nDivisionRadius;		// 円分割数
			g_aMeshCylinder[nCntMeshCylinder].nDivisionHeight = nDivisionHeight;		// Y方向分割数
			g_aMeshCylinder[nCntMeshCylinder].bUse = true;							// 使用している状態にする

			// 頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius + 1) * (g_aMeshCylinder[nCntMeshCylinder].nDivisionHeight + 1)),
										D3DUSAGE_WRITEONLY,
										FVF_VERTEX_3D,
										D3DPOOL_MANAGED,
										&g_aMeshCylinder[nCntMeshCylinder].pVtxBuff,
										NULL);

			// 頂点バッファをロックし、頂点情報へのポインタ取得
			g_aMeshCylinder[nCntMeshCylinder].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			for (int nCntVtx = 0; nCntVtx < (g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius + 1) * (g_aMeshCylinder[nCntMeshCylinder].nDivisionHeight + 1); nCntVtx++)
			{
				// 頂点座標の設定
				pVtx[nCntVtx].pos = D3DXVECTOR3(sinf(-D3DX_PI + ((D3DX_PI / (g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius / 2)) * (nCntVtx % (g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius + 1)))) * g_aMeshCylinder[nCntMeshCylinder].fRadius,
					(g_aMeshCylinder[nCntMeshCylinder].fHeight * g_aMeshCylinder[nCntMeshCylinder].nDivisionHeight) + -g_aMeshCylinder[nCntMeshCylinder].fHeight * (nCntVtx / (g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius + 1)), 
					cosf(-D3DX_PI + ((D3DX_PI / (g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius / 2)) * (nCntVtx % (g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius + 1)))) * g_aMeshCylinder[nCntMeshCylinder].fRadius);

				// 法線ベクトルの設定
				vecCylinder = pVtx[nCntVtx].pos;
				D3DXVec3Normalize(&vecCylinder, &vecCylinder);		// ベクトルを正規化する
				pVtx[nCntVtx].nor = D3DXVECTOR3(-vecCylinder.x, vecCylinder.y, -vecCylinder.z);

				// 頂点カラーの設定
				pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の指定
				pVtx[nCntVtx].tex = D3DXVECTOR2(1.0f * (nCntVtx % (g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius + 1)), 1.0f * (nCntVtx / (g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius + 1)));
			}

			// 頂点バッファをアンロック
			g_aMeshCylinder[nCntMeshCylinder].pVtxBuff->Unlock();

			// インデックスバッファの生成
			pDevice->CreateIndexBuffer(sizeof(WORD) * ((g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius + 1) * 2 + ((g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius + 1) * (g_aMeshCylinder[nCntMeshCylinder].nDivisionHeight - 1) * 2) + 2 * (g_aMeshCylinder[nCntMeshCylinder].nDivisionHeight - 1)),
										D3DUSAGE_WRITEONLY,
										D3DFMT_INDEX16,
										D3DPOOL_MANAGED,
										&g_aMeshCylinder[nCntMeshCylinder].pIdxBuff,
										NULL);

			// インデックスバッファをロックし頂点番号データのポインタを取得
			g_aMeshCylinder[nCntMeshCylinder].pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

			for (int nCntVtx = 0; nCntVtx < ((g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius + 1) * 2
				+ ((g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius + 1) * (g_aMeshCylinder[nCntMeshCylinder].nDivisionHeight - 1) * 2)
				+ 2 * (g_aMeshCylinder[nCntMeshCylinder].nDivisionHeight - 1)); nCntVtx++)
			{
				
				if (nCntVtx % ((g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius + 1) * 2 * (1 + nCntVtx / ((g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius + 1) * 2 + 2)) + (2 * (nCntVtx / ((g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius + 1) * 2 + 2)))) == 0 && nCntVtx > 1
					|| nCntVtx % ((g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius + 1) * 2 * (1 + nCntVtx / ((g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius + 1) * 2 + 2)) + (2 * (nCntVtx / (((g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius + 1) * 2 + 2))))) == 1 && nCntVtx > 1)
				{
					pIdx[nCntVtx] = ((nCntVtx % 2) * (g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius + 1
									+ (1 + nCntVtx / ((g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius + 1) * 2 + 2))))
									+ (g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius * (1 + nCntVtx / ((g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius + 1) * 2 + 2)))
									+ ((nCntVtx % 2 ^ 1) * (nCntVtx / ((g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius + 1) * 2 + 2)));

				}
				else
				{
					// 2事に値を増加させる
					if (nCntVtx % 2 == 0 && nCntVtx > 0)
					{
						// 加算値増加
						nCntAdd++;

					}
					// 頂点番号データの設定
					pIdx[nCntVtx] = ((nCntVtx % 2 ^ 1) * (g_aMeshCylinder[nCntMeshCylinder].nDivisionRadius + 1)) + nCntAdd;
				}
			}

			// インデックスバッファをアンロックする
			g_aMeshCylinder[nCntMeshCylinder].pIdxBuff->Unlock();

			break;
		}
	}
}