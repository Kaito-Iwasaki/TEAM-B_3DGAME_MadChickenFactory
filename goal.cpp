//=======================================================
//
// ゴール処理[goal.cpp]
// Author : Airi Takahashi
//
//=======================================================
#include"goal.h"
#include"fade.h"

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

	// ブロックの幅と奥行の設定
	SetGoalWidthAndDepth(&g_Goal, g_Goal.vtxMax, g_Goal.vtxMin);

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

		// ブロック(パーツの描画)
		g_Goal.pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=======================================================
// ゴールとの当たり判定処理
//=======================================================
void CollisionGoal(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fRadius)
{
	FADE pFade = GetFade();			// フェード情報取得

	if (pPos->x + fRadius >= g_Goal.pos.x - g_Goal.fWidMin
		&& pPos->x - fRadius <= g_Goal.pos.x + g_Goal.fWidMax
		&& pPos->y + fRadius > g_Goal.pos.y + g_Goal.vtxMin.y
		&& pPos->y - fRadius < g_Goal.pos.y + g_Goal.vtxMax.y
		&& pPos->z + fRadius >= g_Goal.pos.z - g_Goal.fDepMin
		&& pPos->z - fRadius <= g_Goal.pos.z + g_Goal.fDepMax)
	{// ブロック範囲内

		if (pPosOld->x - fRadius >= g_Goal.pos.x + g_Goal.fWidMax
			&& pPos->x - fRadius < g_Goal.pos.x + g_Goal.fWidMax
			&& pPosOld->z + fRadius > g_Goal.pos.z - g_Goal.fDepMin
			&& pPosOld->z - fRadius < g_Goal.pos.z + g_Goal.fDepMax)
		{// 右側にめり込んだ

			pPos->x = g_Goal.pos.x + fRadius + g_Goal.fWidMax;	// ブロックの右側面に立たせる
		}
		else if (pPosOld->x + fRadius <= g_Goal.pos.x - g_Goal.fWidMin
			&& pPos->x + fRadius > g_Goal.pos.x - g_Goal.fWidMin
			&& pPosOld->z + fRadius > g_Goal.pos.z - g_Goal.fDepMin
			&& pPosOld->z - fRadius < g_Goal.pos.z + g_Goal.fDepMax)
		{//	左側にめり込んだ

			pPos->x = g_Goal.pos.x - fRadius - g_Goal.fWidMin;	// ブロックの左側面に立たせる
		}

		if (pPosOld->z + fRadius <= g_Goal.pos.z - g_Goal.fDepMin
			&& pPos->z + fRadius > g_Goal.pos.z - g_Goal.fDepMin
			&& pPosOld->x + fRadius > g_Goal.pos.x - g_Goal.fWidMin
			&& pPosOld->x - fRadius < g_Goal.pos.x + g_Goal.fWidMax)
		{// 正面側にめり込んだ

			pPos->z = g_Goal.pos.z - fRadius - g_Goal.fDepMin;	// ブロックの下側面に立たせる
		}
		else if (pPosOld->z - fRadius >= g_Goal.pos.z + g_Goal.fDepMax
			&& pPos->z - fRadius < g_Goal.pos.z + g_Goal.fDepMax
			&& pPosOld->x + fRadius > g_Goal.pos.x - g_Goal.fWidMin
			&& pPosOld->x - fRadius < g_Goal.pos.x + g_Goal.fWidMax)
		{// 後ろ側にめり込んだ

			pPos->z = g_Goal.pos.z + fRadius + g_Goal.fDepMax;	// ブロックの上側面に立たせる
		}

		if (pFade.state == FADESTATE_NONE)
		{// フェードの設定

			SetFade(MODE_RESULT);
		}
	}
}

//=======================================================
// ゴールの幅と奥行の設定
//=======================================================
void SetGoalWidthAndDepth(GOAL* pGoal, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	float rot = D3DX_PI / 2.0f;

	if (pGoal->rot.y == 0.0f)
	{// 回転無し

		pGoal->fWidMax = vtxMax.x;		// 最大幅設定
		pGoal->fWidMin = vtxMin.x;		// 最小幅設定
		pGoal->fDepMax = vtxMax.z;		// 最大奥行設定
		pGoal->fDepMin = vtxMin.z;		// 最少奥行設定
	}
	else if (pGoal->rot.y <= D3DX_PI / 2.0f && pGoal->rot.y > 0)
	{// 90度回転

		pGoal->fWidMax = vtxMax.z;		// 最大幅設定
		pGoal->fWidMin = vtxMin.z;		// 最小幅設定
		pGoal->fDepMax = vtxMin.x;		// 最大奥行設定
		pGoal->fDepMin = vtxMax.x;		// 最少奥行設定
	}
	else if (pGoal->rot.y >= D3DX_PI / -2.0f && pGoal->rot.y < 0)
	{// -90度回転

		pGoal->fWidMax = vtxMin.z;		// 最大幅設定
		pGoal->fWidMin = vtxMax.z;		// 最小幅設定
		pGoal->fDepMax = vtxMax.x;		// 最大奥行設定
		pGoal->fDepMin = vtxMin.x;		// 最少奥行設定
	}
	else if (pGoal->rot.y <= D3DX_PI && pGoal->rot.y > D3DX_PI / 2.0f)
	{// 180度回転

		pGoal->fWidMax = vtxMin.x;		// 最大幅設定
		pGoal->fWidMin = vtxMax.x;		// 最小幅設定
		pGoal->fDepMax = vtxMin.z;		// 最大奥行設定
		pGoal->fDepMin = vtxMax.z;		// 最少奥行設定
	}
	// 正の数にする
	if (pGoal->fWidMax < 0)
	{
		pGoal->fWidMax *= -1.0f;
	}

	if (pGoal->fWidMin < 0)
	{
		pGoal->fWidMin *= -1.0f;
	}

	if (pGoal->fDepMax < 0)
	{
		pGoal->fDepMax *= -1.0f;
	}

	if (pGoal->fDepMin < 0)
	{
		pGoal->fDepMin *= -1.0f;
	}
}