//==================================================
//
//	可動箱[box_movable.cpp]
//
//==================================================
//==================================================
//
//	ファイルインクルード
//
//==================================================
#include "lift.h"
#include "main.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"
#include "fade.h"

//==================================================
//
//	マクロ定義
//
//==================================================
#define MAX_LIFT		(128)					//モデルの最大数
#define LIFT_MODEL_PATH "q"

//==================================================
//
//	構造体
//
//==================================================

//==================================================
//
//	グローバル変数宣言
//
//==================================================
MESHDATA g_aLiftModelData;
Lift g_aLift[MAX_LIFT];

//==================================================
//
//	可動箱の初期化
//
//==================================================
void InitLift(void)
{
	for (int nCntLift = 0; nCntLift < MAX_LIFT; nCntLift++)
	{
		g_aLift[nCntLift].bUse = false;
		g_aLift[nCntLift].pos = D3DXVECTOR3_ZERO;
		g_aLift[nCntLift].rot = D3DXVECTOR3_ZERO;

	}

	LoadModel(LIFT_MODEL_PATH, &g_aLiftModelData);

	SetLift(D3DXVECTOR3(-300.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(100.0f, 0.0f, 1000.0f));
}

//==================================================
//
//	可動箱の終了処理
//
//==================================================
void UninitLift(void)
{
	ReleaseMesh(&g_aLiftModelData);
}

//==================================================
//
//	可動箱の更新処理
//
//==================================================
void UpdateLift(void)
{
	Player* pPlayer = GetPlayer();

	CollisionLift();

	PrintDebugProc("%f\n", pPlayer->move.x);

	for (int nCntMBox = 0; nCntMBox < MAX_LIFT; nCntMBox++)
	{
	}
}

//==================================================
//
//	可動箱の描画処理
//
//==================================================
void DrawLift(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL* pMat;				//マテリアルデータへのポインタ

	for (int nCnt = 0; nCnt < MAX_LIFT; nCnt++)
	{
		if (g_aLift[nCnt].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aLift[nCnt].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aLift[nCnt].rot.y, g_aLift[nCnt].rot.x, g_aLift[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aLift[nCnt].mtxWorld, &g_aLift[nCnt].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aLift[nCnt].pos.x, g_aLift[nCnt].pos.y, g_aLift[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aLift[nCnt].mtxWorld, &g_aLift[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aLift[nCnt].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aLiftModelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aLiftModelData.dwNumMat; nCntMat++)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_aLiftModelData.apTexture[nCntMat]);

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//モデル(パーツ)の描画
				g_aLiftModelData.pMesh->DrawSubset(nCntMat);
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}
//==================================================
//
//	可動箱設置処理
//
//==================================================
void SetLift(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 range)
{
	for (int nCnt = 0; nCnt < MAX_LIFT; nCnt++)
	{
		if (g_aLift[nCnt].bUse == false)
		{
			g_aLift[nCnt].bUse = true;
			g_aLift[nCnt].pos = pos;
			g_aLift[nCnt].SavePos = pos;
			g_aLift[nCnt].MotionRange = pos + range;
			g_aLift[nCnt].rot = rot;
			break;
		}
	}
}
//==================================================
//
//	可動箱の当たり判定
//
//==================================================
bool CollisionLift(void)
{
	Player* pPlayer = GetPlayer();
	bool bHitCheck = false;

	for (int nCntLift = 0; nCntLift < MAX_LIFT; nCntLift++)
	{
		if (g_aLift[nCntLift].bUse == true)
		{
			if ((pPlayer->pos.x <= g_aLift[nCntLift].pos.x + g_aLiftModelData.vtxMax.x) &&
				(pPlayer->pos.x >= g_aLift[nCntLift].pos.x + g_aLiftModelData.vtxMin.x) &&
				(pPlayer->pos.y <= g_aLift[nCntLift].pos.y + g_aLiftModelData.vtxMax.y) &&
				(pPlayer->pos.y >= g_aLift[nCntLift].pos.y + g_aLiftModelData.vtxMin.y - 10.0f) &&
				(pPlayer->pos.z <= g_aLift[nCntLift].pos.z + g_aLiftModelData.vtxMax.z) &&
				(pPlayer->pos.z >= g_aLift[nCntLift].pos.z + g_aLiftModelData.vtxMin.z))
			{
				if (pPlayer->posOld.x >= g_aLift[nCntLift].pos.x + g_aLiftModelData.vtxMax.x)
				{//右から
					pPlayer->pos.x = g_aLift[nCntLift].pos.x + g_aLiftModelData.vtxMax.x;
					
					bHitCheck = true;
				}
				else if (pPlayer->posOld.x <= g_aLift[nCntLift].pos.x + g_aLiftModelData.vtxMin.x)
				{//左から
					pPlayer->pos.x = g_aLift[nCntLift].pos.x + g_aLiftModelData.vtxMin.x;
				
					bHitCheck = true;
				}

				if (pPlayer->posOld.y >= g_aLift[nCntLift].pos.y + g_aLiftModelData.vtxMax.y)
				{//上から
					pPlayer->pos.y = g_aLift[nCntLift].pos.y + g_aLiftModelData.vtxMax.y;
					bHitCheck = true;
					pPlayer->move.y = 0;
					pPlayer->bJump = false;
				}
				else if (pPlayer->posOld.y <= g_aLift[nCntLift].pos.y + g_aLiftModelData.vtxMin.y)
				{//下から
					pPlayer->pos.y = g_aLift[nCntLift].pos.y + g_aLiftModelData.vtxMin.y;
					bHitCheck = true;
				}

				if (pPlayer->posOld.z >= g_aLift[nCntLift].pos.z + g_aLiftModelData.vtxMax.z)
				{//奥から
					pPlayer->pos.z = g_aLift[nCntLift].pos.z + g_aLiftModelData.vtxMax.z;
					
					bHitCheck = true;
				}
				else if (pPlayer->posOld.z <= g_aLift[nCntLift].pos.z + g_aLiftModelData.vtxMin.z)
				{//手前から
					pPlayer->pos.z = g_aLift[nCntLift].pos.z + g_aLiftModelData.vtxMin.z;
					
					bHitCheck = true;
				}
			}

			if (bHitCheck == false)
			{//触られていないので、修正
				
			}

		}
	}

	return bHitCheck;
}
