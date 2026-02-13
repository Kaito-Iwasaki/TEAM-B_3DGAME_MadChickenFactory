//=====================================================================
//
// Enemy [Enemy.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "Enemy.h"
#include "motion.h"
#include "util.h"
#include "player.h"
#include "DebugProc.h"
#include "shadow.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define ENEMY_MOTION_FILENAME			"data\\motion_donald.txt"
#define ENEMY_MAX_SIGHT_DISTANCE		(400)								// 敵の視界距離
#define ENEMY_MAX_SIGHT_DISTANCE_Y		(150)								// 敵の視界距離（縦）
#define ENEMY_MAX_SIGHT_ANGLE			(0.3f)								// 敵の視野角

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
ENEMY g_aEnemy[MAX_ENEMY];

//=====================================================================
// 初期化処理
//=====================================================================
void InitEnemy(void)
{
	ENEMY* pEnemy = &g_aEnemy[0];

	for (int nCountEnemy = 0; nCountEnemy < MAX_ENEMY; nCountEnemy++, pEnemy++)
	{
		ZeroMemory(pEnemy, sizeof(ENEMY));
		InitMotion(&pEnemy->motion, ENEMY_MOTION_FILENAME);
	}
	
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitEnemy(void)
{
	ENEMY* pEnemy = &g_aEnemy[0];

	for (int nCountEnemy = 0; nCountEnemy < MAX_ENEMY; nCountEnemy++, pEnemy++)
	{
		UninitMotion(&pEnemy->motion);
		pEnemy->nIdxShadow = -1;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateEnemy(void)
{
	ENEMY* pEnemy = &g_aEnemy[0];

	for (int nCountEnemy = 0; nCountEnemy < MAX_ENEMY; nCountEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false) continue;

		Player* pPlayer = GetPlayer();

		for (int nCountPlayer = 0; nCountPlayer < MAX_PLAYER; nCountPlayer++, pPlayer++)
		{
			D3DXVECTOR3 vSight = D3DXVECTOR3(sinf(pEnemy->rot.y + D3DX_PI), 0, cosf(pEnemy->rot.y + D3DX_PI));
			D3DXVECTOR3 vToPlr = pPlayer->pos - pEnemy->pos;

			if (
				Magnitude(vToPlr) < ENEMY_MAX_SIGHT_DISTANCE		// プレイヤーとの距離が範囲内かつ
				&& fabsf(vToPlr.y) < ENEMY_MAX_SIGHT_DISTANCE_Y		// 視野角の内側にいる
				&& DotProduct(vSight, Normalize(vToPlr)) >= 1.0f - ENEMY_MAX_SIGHT_ANGLE
				)
			{
				PrintDebugProc("プレイヤー[%d]がサンダーズ(%d)の視界内に入っています！\n", nCountPlayer, nCountEnemy	);
			}
		}

		UpdateMotion(&pEnemy->motion);
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY* pEnemy = &g_aEnemy[0];
	D3DXMATRIX mtxRot, mtxTrans;
	D3DMATERIAL9 matDef;

	// 現在のマテリアルデータを取得
	pDevice->GetMaterial(&matDef);

	for (int nCountEnemy = 0; nCountEnemy < MAX_ENEMY; nCountEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false) continue;

		// プレイヤーのマトリックスを設定
		MatrixRotationPosition(&pEnemy->mtxWorld, pEnemy->rot, pEnemy->pos);

		for (int nCntPart = 0; nCntPart < pEnemy->motion.nNumPart; nCntPart++)
		{
			D3DXMATRIX mtxRotModel, mtxTransModel;		// 計算用マトリックス
			D3DXMATRIX mtxParent;						// 親のマトリックス
			D3DXMATERIAL* pMat;

			// パーツへのポインタを取得
			PART* pPart = &pEnemy->motion.aPart[nCntPart];

			// パーツのワールドマトリックスの初期化
			MatrixRotationPosition(&pPart->mtxWorld, pPart->rot, pPart->pos);

			// パーツの「親マトリックス」を設定
			if (pPart->nIdxModelParent != -1)
			{// 親モデルがある場合
				// 親のインデックスを使用して、同じプレイヤーのパーツ配列から取得
				mtxParent = pEnemy->motion.aPart[pPart->nIdxModelParent].mtxWorld;
			}
			else
			{// 親モデルが無い場合
				mtxParent = pEnemy->mtxWorld;
			}

			// 算出した「パーツのワールドマトリックス」と「親のマトリックス」をかけ合わせる
			D3DXMatrixMultiply(&pPart->mtxWorld, &pPart->mtxWorld, &mtxParent);

			// パーツのワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pPart->mtxWorld);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)pPart->pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)pPart->dwNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャ設定
				pDevice->SetTexture(0, pPart->apTexture[nCntMat]);

				// パーツの描画
				pPart->pMesh->DrawSubset(nCntMat);
			}
		}
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=====================================================================
// 敵設定処理
//=====================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	ENEMY* pEnemy = &g_aEnemy[0];

	for (int nCountEnemy = 0; nCountEnemy < MAX_ENEMY; nCountEnemy++, pEnemy++)
	{
		if (pEnemy->bUse) continue;

		pEnemy->bUse = true;
		pEnemy->pos = pos;
		pEnemy->rot = rot;
		pEnemy->nIdxShadow = SetShadow(pEnemy->pos, 50);
		break;
	}
}