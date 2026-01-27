//=======================================================
//
// ゴール処理[goal.cpp]
// Author : Airi Takahashi
//
//=======================================================
#include"goal.h"

// マクロ定義
#define MAX_GOAL		(1)								// ゴール数
#define GOAL_MODELPAS	"data\\MODEL\\player000.x"		// ゴールモデルのパス情報

// グローバル変数
GOAL g_Goal;									// ゴール情報

//=======================================================
// ゴールの初期化処理
//=======================================================
void InitGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得
	D3DXMATERIAL* pMat;								// マテリアルデータへのポインタ
	int nNumVtx;									// 頂点数
	DWORD dwSizeFVF;								// 頂点フォーマットのサイズ
	BYTE* pVtxBuff;									// 頂点フォーマットへのポインタ

	g_Goal.pos = D3DXVECTOR3(500.0f, 0.0f, 0.0f);	// 位置の初期化
	g_Goal.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向きの初期化

	// Xファイルの読み込み
	D3DXLoadMeshFromX(GOAL_MODELPAS,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Goal.pBuffMat,
		NULL,
		&g_Goal.dwNumMat,
		&g_Goal.pMesh);

	// 頂点数を取得
	nNumVtx = g_Goal.pMesh->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(g_Goal.pMesh->GetFVF());

	// 頂点バッファをロック
	g_Goal.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		// 頂点座標の代入

		// X座標の最小値の比較
		if (vtx.x <= g_Goal.vtxMin.x)
		{
			// X座標の最小値の設定
			g_Goal.vtxMin.x = vtx.x;
		}

		// Y座標の最小値の比較
		if (vtx.y <= g_Goal.vtxMin.y)
		{
			// Y座標の最小値の設定
			g_Goal.vtxMin.y = vtx.y;
		}

		// Z座標の最小値の比較
		if (vtx.z <= g_Goal.vtxMin.z)
		{
			// X座標の最小値の設定
			g_Goal.vtxMin.z = vtx.z;
		}

		// X座標の最大値の比較
		if (vtx.x >= g_Goal.vtxMax.x)
		{
			// X座標の最大値の設定
			g_Goal.vtxMax.x = vtx.x;
		}

		// Y座標の最大値の比較
		if (vtx.y >= g_Goal.vtxMax.y)
		{
			// Y座標の最大値の設定
			g_Goal.vtxMax.y = vtx.y;
		}

		// Z座標の最大値の比較
		if (vtx.z >= g_Goal.vtxMax.z)
		{
			// X座標の最大値の設定
			g_Goal.vtxMax.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;		// 頂点フォーマットのサイズ分ポインタを進める
	}

	// 頂点バッファをアンロック
	g_Goal.pMesh->UnlockVertexBuffer();

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_Goal.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_Goal.dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_Goal.apTexture[nCntMat]);
		}
	}
}

//=======================================================
// ゴールの終了処理
//=======================================================
void UninitGoal(void)
{
	// メッシュの破棄
	if (g_Goal.pMesh != NULL)
	{
		g_Goal.pMesh->Release();
		g_Goal.pMesh = NULL;
	}

	// マテリアルの破棄
	if (g_Goal.pBuffMat != NULL)
	{
		g_Goal.pBuffMat->Release();
		g_Goal.pBuffMat = NULL;
	}

	// テクスチャの破棄
	for (int nCntMat = 0; nCntMat < (int)g_Goal.dwNumMat; nCntMat++)
	{
		if (g_Goal.apTexture[nCntMat] != NULL)
		{
			g_Goal.apTexture[nCntMat]->Release();
			g_Goal.apTexture[nCntMat] = NULL;
		}
	}

}

//=======================================================
// ゴールの更新処理
//=======================================================
void UpdateGoal(void)
{


}

//=======================================================
// ゴールの描画処理
//=======================================================
void DrawGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;					// 計算用マトリックス
	D3DMATERIAL9 matDef;							// 現在のマテリアル保存
	D3DXMATERIAL* pMat;								// マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Goal.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_Goal.rot.y, g_Goal.rot.x, g_Goal.rot.z);

	D3DXMatrixMultiply(&g_Goal.mtxWorld, &g_Goal.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		g_Goal.pos.x, g_Goal.pos.y, g_Goal.pos.z);

	D3DXMatrixMultiply(&g_Goal.mtxWorld, &g_Goal.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Goal.mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_Goal.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_Goal.dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャ設定
		pDevice->SetTexture(0, g_Goal.apTexture[nCntMat]);

		// モデル(パーツの描画)
		g_Goal.pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}