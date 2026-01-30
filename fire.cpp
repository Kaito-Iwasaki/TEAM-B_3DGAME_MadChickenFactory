//=====================================================================
//
// fire [fire.cpp]
// Author : AIRI TAKAHASHI
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "fire.h"
#include "player.h"
#include "effect.h"
#include "collision.h"
#include "fade.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define	MAX_FIREMODEL	(256)									// 最大数
#define FIRE_MODELPAS	"data\\MODEL\\Factory\\gasburner00.x"	// モデルパス

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
FLAMETHROWERMODEL g_FlamethrowerModel;				// 火炎放射器のモデル情報
FLAMETHROWER g_aflamethrower[MAX_FIREMODEL];		// 火炎放射器情報
FIRE g_aFire[MAX_FIREMODEL];						// 炎情報

//=====================================================================
// 初期化処理
//=====================================================================
void InitFire(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得
	D3DXMATERIAL* pMat;								// マテリアルデータへのポインタ
	int nNumVtx;									// 頂点数
	DWORD dwSizeFVF;								// 頂点フォーマットのサイズ
	BYTE* pVtxBuff;									// 頂点フォーマットへのポインタ

	for (int nCntFire = 0; nCntFire < MAX_FIREMODEL; nCntFire++)
	{
		g_aflamethrower[nCntFire].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置の初期化
		g_aflamethrower[nCntFire].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向きの初期化
		g_aflamethrower[nCntFire].state = FIRESTATE_NONE;					// 状態の初期化
		g_aflamethrower[nCntFire].bUse = false;								// 使用していない状態にする
	}

	// Xファイルの読み込み
	D3DXLoadMeshFromX(FIRE_MODELPAS,
						D3DXMESH_SYSTEMMEM,
						pDevice,
						NULL,
						&g_FlamethrowerModel.pBuffMat,
						NULL,
						&g_FlamethrowerModel.dwNumMat,
						&g_FlamethrowerModel.pMesh);

	// 頂点数を取得
	nNumVtx = g_FlamethrowerModel.pMesh->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(g_FlamethrowerModel.pMesh->GetFVF());

	// 頂点バッファをロック
	g_FlamethrowerModel.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		// 頂点座標の代入

		// X座標の最小値の比較
		if (vtx.x <= g_FlamethrowerModel.vtxMin.x)
		{
			// X座標の最小値の設定
			g_FlamethrowerModel.vtxMin.x = vtx.x;
		}

		// Y座標の最小値の比較
		if (vtx.y <= g_FlamethrowerModel.vtxMin.y)
		{
			// Y座標の最小値の設定
			g_FlamethrowerModel.vtxMin.y = vtx.y;
		}

		// Z座標の最小値の比較
		if (vtx.z <= g_FlamethrowerModel.vtxMin.z)
		{
			// X座標の最小値の設定
			g_FlamethrowerModel.vtxMin.z = vtx.z;
		}

		// X座標の最大値の比較
		if (vtx.x >= g_FlamethrowerModel.vtxMax.x)
		{
			// X座標の最大値の設定
			g_FlamethrowerModel.vtxMax.x = vtx.x;
		}

		// Y座標の最大値の比較
		if (vtx.y >= g_FlamethrowerModel.vtxMax.y)
		{
			// Y座標の最大値の設定
			g_FlamethrowerModel.vtxMax.y = vtx.y;
		}

		// Z座標の最大値の比較
		if (vtx.z >= g_FlamethrowerModel.vtxMax.z)
		{
			// X座標の最大値の設定
			g_FlamethrowerModel.vtxMax.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;		// 頂点フォーマットのサイズ分ポインタを進める
	}

	// 頂点バッファをアンロック
	g_FlamethrowerModel.pMesh->UnlockVertexBuffer();

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_FlamethrowerModel.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_FlamethrowerModel.dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_FlamethrowerModel.apTexture[nCntMat]);
		}
	}

	// 火炎放射器の設置
	SetFlamethrower(D3DXVECTOR3(200.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), FIRESTATE_MANUAL);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitFire(void)
{
	// メッシュの破棄
	if (g_FlamethrowerModel.pMesh != NULL)
	{
		g_FlamethrowerModel.pMesh->Release();
		g_FlamethrowerModel.pMesh = NULL;
	}

	// マテリアルの破棄
	if (g_FlamethrowerModel.pBuffMat != NULL)
	{
		g_FlamethrowerModel.pBuffMat->Release();
		g_FlamethrowerModel.pBuffMat = NULL;
	}

	// テクスチャの破棄
	for (int nCntMat = 0; nCntMat < (int)g_FlamethrowerModel.dwNumMat; nCntMat++)
	{
		if (g_FlamethrowerModel.apTexture[nCntMat] != NULL)
		{
			g_FlamethrowerModel.apTexture[nCntMat]->Release();
			g_FlamethrowerModel.apTexture[nCntMat] = NULL;
		}
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateFire(void)
{
	FLAMETHROWER* pFlamethrower = &g_aflamethrower[0];		// 火炎放射器情報へのポインタ 
	Player* pPlayer = GetPlayer();							// プレイヤーへのポインタ
	FADE pFade = GetFade();									// フェード情報取得


	for (int nCntFire = 0; nCntFire < MAX_FIREMODEL; nCntFire++, pFlamethrower++)
	{
		if (pFlamethrower->bUse == true)
		{// 使用している


			// エフェクト設定
			SetEffect(D3DXVECTOR3(pFlamethrower->pos.x, pFlamethrower->pos.y + g_FlamethrowerModel.vtxMax.y, pFlamethrower->pos.z),
				D3DXVECTOR3(0.0f, 5.0f, 0.0f), EFFECTTYPE_DASH, 60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f));

			for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pPlayer++)
			{
				if (CollisionPointBox(pPlayer->pos,
					D3DXVECTOR3(pFlamethrower->pos.x, pFlamethrower->pos.y + g_FlamethrowerModel.vtxMax.y, pFlamethrower->pos.z),
					D3DXVECTOR3(pFlamethrower->fWidMax + pFlamethrower->fWidMin, 300.0f, pFlamethrower->fDepMax + pFlamethrower->fDepMin)) == true
					&& pFade.state == FADESTATE_NONE)
				{// 炎に当たった

					SetFade(MODE_GAME);
				}
			}
		}
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawFire(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();				// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;							// 計算用マトリックス
	D3DMATERIAL9 matDef;									// 現在のマテリアル保存
	D3DXMATERIAL* pMat;										// マテリアルデータへのポインタ
	FLAMETHROWER* pFlamethrower = &g_aflamethrower[0];		// 火炎放射器情報のポインタ


	for (int nCntFlaethrower = 0; nCntFlaethrower < MAX_FIREMODEL; nCntFlaethrower++, pFlamethrower++)
	{
		if (pFlamethrower->bUse == true)
		{// 使用している

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pFlamethrower->mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				pFlamethrower->rot.y, pFlamethrower->rot.x, pFlamethrower->rot.z);

			D3DXMatrixMultiply(&pFlamethrower->mtxWorld, &pFlamethrower->mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				pFlamethrower->pos.x, pFlamethrower->pos.y, pFlamethrower->pos.z);

			D3DXMatrixMultiply(&pFlamethrower->mtxWorld, &pFlamethrower->mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pFlamethrower->mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_FlamethrowerModel.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_FlamethrowerModel.dwNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャ設定
				pDevice->SetTexture(0, g_FlamethrowerModel.apTexture[nCntMat]);

				// ブロック(パーツの描画)
				g_FlamethrowerModel.pMesh->DrawSubset(nCntMat);
			}

			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=====================================================================
// 火炎放射器の設定
//=====================================================================
void SetFlamethrower(D3DXVECTOR3 pos, D3DXVECTOR3 rot, FIRESTATE state)
{
	for (int nCntFlamethrower = 0; nCntFlamethrower < MAX_FIREMODEL; nCntFlamethrower++)
	{
		if (g_aflamethrower[nCntFlamethrower].bUse == false)
		{
			g_aflamethrower[nCntFlamethrower].pos = pos;		// 位置
			g_aflamethrower[nCntFlamethrower].rot = rot;		// 向き
			g_aflamethrower[nCntFlamethrower].state = state;	// 状態
			g_aflamethrower[nCntFlamethrower].bUse = true;		// 使用している状態にする

			// ブロックの幅と奥行の設定
			SetFlamethrowerWidthAndDepth(nCntFlamethrower);
			
			break;
		}
	}
}


//=====================================================================
// 火炎放射器の幅と奥行の設定
//=====================================================================
void SetFlamethrowerWidthAndDepth(int nIdx)
{

	if (g_aflamethrower[nIdx].rot.y == 0.0f)
	{// 回転無し

		g_aflamethrower[nIdx].fWidMax = g_FlamethrowerModel.vtxMax.x;		// 最大幅設定
		g_aflamethrower[nIdx].fWidMin = g_FlamethrowerModel.vtxMin.x;		// 最小幅設定
		g_aflamethrower[nIdx].fDepMax = g_FlamethrowerModel.vtxMax.z;		// 最大奥行設定
		g_aflamethrower[nIdx].fDepMin = g_FlamethrowerModel.vtxMin.z;		// 最少奥行設定
	}
	else if (g_aflamethrower[nIdx].rot.y <= D3DX_PI / 2.0f && g_aflamethrower[nIdx].rot.y > 0)
	{// 90度回転

		g_aflamethrower[nIdx].fWidMax = g_FlamethrowerModel.vtxMax.z;		// 最大幅設定
		g_aflamethrower[nIdx].fWidMin = g_FlamethrowerModel.vtxMin.z;		// 最小幅設定
		g_aflamethrower[nIdx].fDepMax = g_FlamethrowerModel.vtxMin.x;		// 最大奥行設定
		g_aflamethrower[nIdx].fDepMin = g_FlamethrowerModel.vtxMax.x;		// 最少奥行設定
	}
	else if (g_aflamethrower[nIdx].rot.y >= D3DX_PI / -2.0f && g_aflamethrower[nIdx].rot.y < 0)
	{// -90度回転

		g_aflamethrower[nIdx].fWidMax = g_FlamethrowerModel.vtxMin.z;		// 最大幅設定
		g_aflamethrower[nIdx].fWidMin = g_FlamethrowerModel.vtxMax.z;		// 最小幅設定
		g_aflamethrower[nIdx].fDepMax = g_FlamethrowerModel.vtxMax.x;		// 最大奥行設定
		g_aflamethrower[nIdx].fDepMin = g_FlamethrowerModel.vtxMin.x;		// 最少奥行設定
	}
	else if (g_aflamethrower[nIdx].rot.y <= D3DX_PI && g_aflamethrower[nIdx].rot.y > D3DX_PI / 2.0f)
	{// 180度回転

		g_aflamethrower[nIdx].fWidMax = g_FlamethrowerModel.vtxMin.x;		// 最大幅設定
		g_aflamethrower[nIdx].fWidMin = g_FlamethrowerModel.vtxMax.x;		// 最小幅設定
		g_aflamethrower[nIdx].fDepMax = g_FlamethrowerModel.vtxMin.z;		// 最大奥行設定
		g_aflamethrower[nIdx].fDepMin = g_FlamethrowerModel.vtxMax.z;		// 最少奥行設定
	}

	// 正の数にする
	if (g_aflamethrower[nIdx].fWidMax < 0)
	{
		g_aflamethrower[nIdx].fWidMax *= -1.0f;
	}

	if (g_aflamethrower[nIdx].fWidMin < 0)
	{
		g_aflamethrower[nIdx].fWidMin *= -1.0f;
	}

	if (g_aflamethrower[nIdx].fDepMax < 0)
	{
		g_aflamethrower[nIdx].fDepMax *= -1.0f;
	}

	if (g_aflamethrower[nIdx].fDepMin < 0)
	{
		g_aflamethrower[nIdx].fDepMin *= -1.0f;
	}
}

//=======================================================
// 火炎放射器との当たり判定処理
//=======================================================
void CollisionFlamethrower(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fRadius)
{
	FLAMETHROWER* pFlamethrower = &g_aflamethrower[0];		// 火炎放射器情報のポインタ
	bool bLand = false;										// 着地したかどうか
	bool bHitHead = false;									// 頭が当たったかどうか
	int nCount = -1;										// ブロック番号代入

	for(int nCntFlaethrower = 0; nCntFlaethrower < MAX_FIREMODEL; nCntFlaethrower++, pFlamethrower++)
	{ 
		if (pFlamethrower->bUse == true)
		{// 使用している

			if (pPos->x + fRadius >= pFlamethrower->pos.x - pFlamethrower->fWidMin
				&& pPos->x - fRadius <= pFlamethrower->pos.x + pFlamethrower->fWidMax
				&& pPos->y + fRadius > pFlamethrower->pos.y + g_FlamethrowerModel.vtxMin.y
				&& pPos->y - fRadius < pFlamethrower->pos.y + g_FlamethrowerModel.vtxMax.y
				&& pPos->z + fRadius >= pFlamethrower->pos.z - pFlamethrower->fDepMin
				&& pPos->z - fRadius <= pFlamethrower->pos.z + pFlamethrower->fDepMax)
			{// ブロック範囲内

				if (pPosOld->y + fRadius <= pFlamethrower->pos.y + g_FlamethrowerModel.vtxMin.y
					&& pPos->y + fRadius > pFlamethrower->pos.y + g_FlamethrowerModel.vtxMin.y
					&& pPosOld->x + fRadius > pFlamethrower->pos.x - pFlamethrower->fWidMin
					&& pPosOld->x - fRadius < pFlamethrower->pos.x + pFlamethrower->fWidMax
					&& pPosOld->z + fRadius > pFlamethrower->pos.z - pFlamethrower->fDepMin
					&& pPosOld->z - fRadius < pFlamethrower->pos.z + pFlamethrower->fDepMax)
				{// 天井にめり込んだ

					bHitHead = true;
					nCount = nCntFlaethrower;
				}
				else if (pPosOld->y >= pFlamethrower->pos.y + g_FlamethrowerModel.vtxMax.y
					&& pPos->y < pFlamethrower->pos.y + g_FlamethrowerModel.vtxMax.y)
				{// 地面にめり込んだ

					bLand = true;
					nCount = nCntFlaethrower;
				}

				if (pPosOld->x - fRadius >= pFlamethrower->pos.x + pFlamethrower->fWidMax
					&& pPos->x - fRadius < pFlamethrower->pos.x + pFlamethrower->fWidMax
					&& pPosOld->z + fRadius > pFlamethrower->pos.z - pFlamethrower->fDepMin
					&& pPosOld->z - fRadius < pFlamethrower->pos.z + pFlamethrower->fDepMax)
				{// 右側にめり込んだ

					if (bLand != true)
					{
						pPos->x = pFlamethrower->pos.x + fRadius + pFlamethrower->fWidMax;	// ブロックの右側面に立たせる
					}

					if (bHitHead == true && nCount == nCntFlaethrower)
					{
						bHitHead = false;
					}
				}
				else if (pPosOld->x + fRadius <= pFlamethrower->pos.x - pFlamethrower->fWidMin
					&& pPos->x + fRadius > pFlamethrower->pos.x - pFlamethrower->fWidMin
					&& pPosOld->z + fRadius > pFlamethrower->pos.z - pFlamethrower->fDepMin
					&& pPosOld->z - fRadius < pFlamethrower->pos.z + pFlamethrower->fDepMax)
				{//	左側にめり込んだ

					if (bLand != true)
					{
						pPos->x = pFlamethrower->pos.x - fRadius - pFlamethrower->fWidMin;	// ブロックの左側面に立たせる
					}

					if (bHitHead == true && nCount == nCntFlaethrower)
					{
						bHitHead = false;
					}
				}

				if (pPosOld->z + fRadius <= pFlamethrower->pos.z - pFlamethrower->fDepMin
					&& pPos->z + fRadius > pFlamethrower->pos.z - pFlamethrower->fDepMin
					&& pPosOld->x + fRadius > pFlamethrower->pos.x - pFlamethrower->fWidMin
					&& pPosOld->x - fRadius < pFlamethrower->pos.x + pFlamethrower->fWidMax)
				{// 正面側にめり込んだ

					if (bLand != true)
					{
						pPos->z = pFlamethrower->pos.z - fRadius - pFlamethrower->fDepMin;	// ブロックの正面側に立たせる
					}

					if (bHitHead == true && nCount == nCntFlaethrower)
					{
						bHitHead = false;
					}
				}
				else if (pPosOld->z - fRadius >= pFlamethrower->pos.z + pFlamethrower->fDepMax
					&& pPos->z - fRadius < pFlamethrower->pos.z + pFlamethrower->fDepMax
					&& pPosOld->x + fRadius > pFlamethrower->pos.x - pFlamethrower->fWidMin
					&& pPosOld->x - fRadius < pFlamethrower->pos.x + pFlamethrower->fWidMax)
				{// 後ろ側にめり込んだ

					if (bLand != true)
					{
						pPos->z = pFlamethrower->pos.z + fRadius + pFlamethrower->fDepMax;	// ブロックの後ろ側に立たせる
					}

					if (bHitHead == true && nCount == nCntFlaethrower)
					{
						bHitHead = false;
					}
				}
			}
		}
	}

	// 位置更新
	if (bLand == true)
	{// 地面にめり込んだ

		Player* pPlayer = GetPlayer();

		pPos->y = g_aflamethrower[nCount].pos.y + g_FlamethrowerModel.vtxMax.y;		// ブロックの上に立たせる
		pMove->y = 0.0f;															// 移動量を0にする

		pPlayer->bJump = false;			// ジャンプしていない状態にする
	}

	if (bHitHead == true)
	{// 頭が当たった

		pPos->y = g_aflamethrower[nCount].pos.y - fRadius + g_FlamethrowerModel.vtxMin.y;	// ブロックの下に立たせる
		pMove->y = 0.0f;																	// 移動量を0にする
	}
}