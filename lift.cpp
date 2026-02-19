//==================================================
//
//	リフト[box_movable.cpp]
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
#include "prompt.h"
#include "collision.h"

//==================================================
//
//	マクロ定義
//
//==================================================
#define MAX_LIFT		(128)					// モデルの最大数
#define LIFT_MODEL_PATH "data\\MODEL\\Factory\\lift.x"	// モデルファイルパス
#define EFFECTIVE_RANGE_LIFT	(10.0f)			// 有効範囲

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
//	リフトの初期化
//
//==================================================
void InitLift(void)
{
	for (int nCntLift = 0; nCntLift < MAX_LIFT; nCntLift++)
	{
		g_aLift[nCntLift].nIdx = -1;
		g_aLift[nCntLift].bUse = false;
		g_aLift[nCntLift].pos = D3DXVECTOR3_ZERO;
		g_aLift[nCntLift].rot = D3DXVECTOR3_ZERO;
		g_aLift[nCntLift].move = D3DXVECTOR3_ZERO;
		g_aLift[nCntLift].SavePos = D3DXVECTOR3_ZERO;
		g_aLift[nCntLift].GoPoint = D3DXVECTOR3_ZERO;
		g_aLift[nCntLift].bswitch = false;
		g_aLift[nCntLift].interval = 0;
		g_aLift[nCntLift].speed = 0.0f;
		g_aLift[nCntLift].nCounter = 0;
	}

	LoadModel(LIFT_MODEL_PATH, &g_aLiftModelData);

	SetLift(1, D3DXVECTOR3(1803.0f, 0.0f, -400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 200.0f, 0.0f), 1.0f);
	SetLift(1, D3DXVECTOR3(1430.0f, 0.0f, 2000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 200.0f, 0.0f), 1.0f);
	SetLift(1, D3DXVECTOR3(5250.0f, 0.0f, 2000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 200.0f, 0.0f), 1.0f);
}

//==================================================
//
//	リフトの終了処理
//
//==================================================
void UninitLift(void)
{
	ReleaseMesh(&g_aLiftModelData);
}

//==================================================
//
//	リフトの更新処理
//
//==================================================
void UpdateLift(void)
{
	/*if (GetKeyboardTrigger(DIK_9))
	{
		GetPromptTrigger(0);
	}*/

	Player* pPlayer = GetPlayer();

	for (int nCntLift = 0; nCntLift < MAX_LIFT; nCntLift++)
	{
		if (g_aLift[nCntLift].bUse == true)
		{
			if (g_aLift[nCntLift].NowState == LIFTSTATE_GO_POINT)
			{
				//==============================
				// --- 初期位置から移動中 ---
				//==============================
				g_aLift[nCntLift].move = g_aLift[nCntLift].vec * g_aLift[nCntLift].speed;

				if (CollisionPointBoxDirection(pPlayer->pos, pPlayer->posOld, g_aLift[nCntLift].pos, g_aLiftModelData.vtxMin, g_aLiftModelData.vtxMax))
				{
					pPlayer->pos += g_aLift[nCntLift].move;
				}

				g_aLift[nCntLift].pos += g_aLift[nCntLift].move;
				if (g_aLift[nCntLift].pos.x <= g_aLift[nCntLift].GoPoint.x + EFFECTIVE_RANGE_LIFT &&
					g_aLift[nCntLift].pos.x >= g_aLift[nCntLift].GoPoint.x - EFFECTIVE_RANGE_LIFT &&
					g_aLift[nCntLift].pos.y <= g_aLift[nCntLift].GoPoint.y + EFFECTIVE_RANGE_LIFT &&
					g_aLift[nCntLift].pos.y >= g_aLift[nCntLift].GoPoint.y - EFFECTIVE_RANGE_LIFT &&
					g_aLift[nCntLift].pos.z <= g_aLift[nCntLift].GoPoint.z + EFFECTIVE_RANGE_LIFT &&
					g_aLift[nCntLift].pos.z >= g_aLift[nCntLift].GoPoint.z - EFFECTIVE_RANGE_LIFT)
				{
					g_aLift[nCntLift].NowState = LIFTSTATE_STAY;
				}

			}
			else if (g_aLift[nCntLift].NowState == LIFTSTATE_SV_POINT)
			{
				//==============================
				// --- 設定位置から移動中 ---
				//==============================
				g_aLift[nCntLift].move = g_aLift[nCntLift].vec * g_aLift[nCntLift].speed;
				
				if (CollisionPointBoxDirection(pPlayer->pos, pPlayer->posOld, g_aLift[nCntLift].pos, g_aLiftModelData.vtxMin, g_aLiftModelData.vtxMax))
				{
					pPlayer->pos += g_aLift[nCntLift].move;
				}

				g_aLift[nCntLift].pos -= g_aLift[nCntLift].move;
				if (g_aLift[nCntLift].pos.x <= g_aLift[nCntLift].SavePos.x + EFFECTIVE_RANGE_LIFT &&
					g_aLift[nCntLift].pos.x >= g_aLift[nCntLift].SavePos.x - EFFECTIVE_RANGE_LIFT &&
					g_aLift[nCntLift].pos.y <= g_aLift[nCntLift].SavePos.y + EFFECTIVE_RANGE_LIFT &&
					g_aLift[nCntLift].pos.y >= g_aLift[nCntLift].SavePos.y - EFFECTIVE_RANGE_LIFT &&
					g_aLift[nCntLift].pos.z <= g_aLift[nCntLift].SavePos.z + EFFECTIVE_RANGE_LIFT &&
					g_aLift[nCntLift].pos.z >= g_aLift[nCntLift].SavePos.z - EFFECTIVE_RANGE_LIFT)
				{
					g_aLift[nCntLift].NowState = LIFTSTATE_STAY;
				}
			}
			else if (g_aLift[nCntLift].NowState == LIFTSTATE_STAY)
			{
				PrintDebugProc("%d\n", GetPromptTrigger(g_aLift[nCntLift].nIdx));
				g_aLift[nCntLift].move = D3DXVECTOR3_ZERO;

				if (GetPromptTrigger(g_aLift[nCntLift].nIdx))
				{
					if (g_aLift[nCntLift].PreviousState == LIFTSTATE_SV_POINT)
					{
						g_aLift[nCntLift].NowState = LIFTSTATE_GO_POINT;
						g_aLift[nCntLift].PreviousState = LIFTSTATE_GO_POINT;
					}
					else if (g_aLift[nCntLift].PreviousState == LIFTSTATE_GO_POINT)
					{
						g_aLift[nCntLift].NowState = LIFTSTATE_SV_POINT;
						g_aLift[nCntLift].PreviousState = LIFTSTATE_SV_POINT;
					}
				}
			}

			for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
			{
				// オブジェクトの当たり判定処理
				pPlayer[nCntPlayer].fStandPos = CollisionPointBoxObject(pPlayer[nCntPlayer].pos,
												g_aLift[nCntLift].pos,
												g_aLiftModelData.vtxMin,
												g_aLiftModelData.vtxMax,
												pPlayer[nCntPlayer].fStandPos);
			}
		}
	}
}

//==================================================
//
//	リフトの描画処理
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
//	リフト設置処理
//
//==================================================
void SetLift(int nIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 range,float speed)
{
	D3DXVECTOR3 Compornent;	// 成分
	float Size;				// 大きさ

	for (int nCnt = 0; nCnt < MAX_LIFT; nCnt++)
	{
		if (g_aLift[nCnt].bUse == false)
		{
			g_aLift[nCnt].nIdx = nIdx;
			g_aLift[nCnt].bUse = true;
			g_aLift[nCnt].pos = pos;
			g_aLift[nCnt].SavePos = pos;
			g_aLift[nCnt].GoPoint = pos + range;
			// ベクトル計算
			Compornent = g_aLift[nCnt].GoPoint - g_aLift[nCnt].SavePos;
			Size = sqrtf(powf(g_aLift[nCnt].GoPoint.x - g_aLift[nCnt].SavePos.x, 2.0f)
				+ powf(g_aLift[nCnt].GoPoint.y - g_aLift[nCnt].SavePos.y, 2.0f)
				+ powf(g_aLift[nCnt].GoPoint.z - g_aLift[nCnt].SavePos.z, 2.0f));
			g_aLift[nCnt].vec = Compornent / Size;

			g_aLift[nCnt].rot = rot;
			g_aLift[nCnt].speed = speed;
			g_aLift[nCnt].NowState = LIFTSTATE_STAY;
			g_aLift[nCnt].PreviousState = LIFTSTATE_SV_POINT;
			break;
		}
	}
}
//==================================================
//
//	リフトの当たり判定
//
//==================================================
bool CollisionLift(void)
{
	Player* pPlayer = GetPlayer();
	bool bHitCheck = false;
	Lift* pLift = &g_aLift[0];

	for (int nPlayer = 0; nPlayer < MAX_PLAYER; nPlayer++, pPlayer++)
	{
		for (int nCntLift = 0; nCntLift < MAX_LIFT; nCntLift++, pLift++)
		{
			D3DXVECTOR3 vecHeight = D3DXVECTOR3(0, pPlayer->fHeight, 0);
			BYTE byHit = CollisionBoxBoxDirection(
				pPlayer->pos,
				pPlayer->posOld,
				D3DXVECTOR3(0, 0, 0),
				D3DXVECTOR3(0, pPlayer->fHeight, 0),
				pLift->pos,
				g_aLiftModelData.vtxMin,
				g_aLiftModelData.vtxMax
			);

			if (byHit & COLLISION_UP)
			{
				pPlayer->pos.y = pLift->pos.y + g_aLiftModelData.vtxMax.y;
				pPlayer->move.y = 0;
				pPlayer->bJump = false;
			}
			else if (byHit & COLLISION_DOWN)
			{
				pPlayer->pos.y = pLift->pos.y - pPlayer->fHeight;
				pPlayer->move.y = 0;
			}

			if (byHit & COLLISION_LEFT)
			{
				pPlayer->pos.x = pLift->pos.x + g_aLiftModelData.vtxMin.x;
			}
			if (byHit & COLLISION_RIGHT)
			{
				pPlayer->pos.x = pLift->pos.x + g_aLiftModelData.vtxMax.x;
			}
			if (byHit & COLLISION_FRONT)
			{
				pPlayer->pos.z = pLift->pos.z + g_aLiftModelData.vtxMin.z;
			}
			if (byHit & COLLISION_BACK)
			{
				pPlayer->pos.z = pLift->pos.z + g_aLiftModelData.vtxMax.z;
			}
		}
	}

	return bHitCheck;
}
