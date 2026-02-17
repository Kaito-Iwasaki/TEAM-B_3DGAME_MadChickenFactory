//=====================================================================
//
// Enemy [Enemy.cpp]
// Author : Kaito Iwasaki
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
#include "fade.h"
#include "SE_controller.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define ENEMY_MOTION_FILENAME			"data\\motion_donald.txt"
#define ENEMY_MAX_SIGHT_DISTANCE		(600)								// 敵の視界距離
#define ENEMY_MAX_SIGHT_DISTANCE_Y		(150)								// 敵の視界距離（縦）
#define ENEMY_MAX_SIGHT_ANGLE			(90.0f)								// 敵の視野角
#define ENEMY_SIGHT_NUM_SEGMENT			(16)								// 敵の視野角表示の詳細度（頂点の分割数）

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
void _SetEnemyState(int nIdx, ENEMYSTATE newState);
void _OnEnemyState(int nIdx);
void _OnEnemyStateChanged(int nIdx);

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
ENEMY g_aEnemy[MAX_ENEMY];
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemySight = NULL;

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

	GetDevice()->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_ENEMY * (ENEMY_SIGHT_NUM_SEGMENT + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemySight,
		NULL
	);
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

	RELEASE(g_pVtxBuffEnemySight);
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateEnemy(void)
{
	ENEMY* pEnemy = &g_aEnemy[0];
	int Enemy;

	for (int nCountEnemy = 0; nCountEnemy < MAX_ENEMY; nCountEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false) continue;

		Player* pPlayer = GetPlayer();

		pEnemy->previousState = pEnemy->currentState;


		for (int nCountPlayer = 0; nCountPlayer < MAX_PLAYER; nCountPlayer++, pPlayer++)
		{
			D3DXVECTOR3 vSight = D3DXVECTOR3(sinf(pEnemy->rot.y + D3DX_PI), 0, cosf(pEnemy->rot.y + D3DX_PI));
			D3DXVECTOR3 vToPlr = pPlayer->pos - pEnemy->pos;

			if (
				Magnitude(vToPlr) < ENEMY_MAX_SIGHT_DISTANCE		// プレイヤーとの距離が範囲内かつ
				&& fabsf(vToPlr.y) < ENEMY_MAX_SIGHT_DISTANCE_Y		// 視野角の内側にいる
				&& acosf(DotProduct(vSight, Normalize(vToPlr))) <= RAD(ENEMY_MAX_SIGHT_ANGLE) * 0.5f
				)
			{
				// ターゲットをプレイヤーに設定
				pEnemy->nTarget = nCountPlayer;

				// 発見状態に遷移
				_SetEnemyState(nCountEnemy, ENEMYSTATE_FOUND);

				// ゲームオーバー処理
				//SetFade(MODE_GAME);
			}
		}

		// 状態別処理
		_OnEnemyState(nCountEnemy);

		if (pEnemy->currentState != pEnemy->previousState)
		{// 状態遷移処理
			_OnEnemyStateChanged(nCountEnemy);
		}

		// モーション更新
		UpdateMotion(&pEnemy->motion);

		// 影位置の更新
		SetPositionShadow(pEnemy->nIdxShadow, pEnemy->pos, 0, true);
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
	D3DXMATRIX mtxWorldSight;

	// 現在のマテリアルデータを取得
	pDevice->GetMaterial(&matDef);

	VERTEX_3D* pVtx;

	D3DXMATERIAL pMatSight;
	
	g_pVtxBuffEnemySight->Lock(0, 0, (void**)&pVtx, 0);

	float fAngleStart = -RAD(ENEMY_MAX_SIGHT_ANGLE) * 0.5f;
	float fAngleGap = RAD(ENEMY_MAX_SIGHT_ANGLE) / (float)(ENEMY_SIGHT_NUM_SEGMENT - 1);

	for (int nCountVtx = 0; nCountVtx < ENEMY_SIGHT_NUM_SEGMENT + 1; nCountVtx++, pVtx++)
	{
		if (nCountVtx == 0)
		{
			pVtx->pos = D3DXVECTOR3_ZERO;
		}
		else
		{
			pVtx->pos = D3DXVECTOR3(-sinf(fAngleStart + fAngleGap * (nCountVtx - 1)), 0.001f, -cosf(fAngleStart + fAngleGap * (nCountVtx - 1))) * ENEMY_MAX_SIGHT_DISTANCE;
		}

		pVtx->nor = D3DXVECTOR3_UP;
		pVtx->col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.35f);
	}

	g_pVtxBuffEnemySight->Unlock();

	for (int nCountEnemy = 0; nCountEnemy < MAX_ENEMY; nCountEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false) continue;

		pDevice->SetStreamSource(0, g_pVtxBuffEnemySight, 0, sizeof(VERTEX_3D));
		pDevice->SetFVF(FVF_VERTEX_3D);

		MatrixRotationPosition(&mtxWorldSight, pEnemy->rot, pEnemy->pos);
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorldSight);

		//pDevice->SetMaterial(&pMatSight.MatD3D);
		pDevice->SetTexture(0, NULL);
		pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, nCountEnemy * (ENEMY_SIGHT_NUM_SEGMENT + 1), ENEMY_SIGHT_NUM_SEGMENT - 1);

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
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSpeed, ENEMY_ROUTINE* pRoutine)
{
	ENEMY* pEnemy = &g_aEnemy[0];

	for (int nCountEnemy = 0; nCountEnemy < MAX_ENEMY; nCountEnemy++, pEnemy++)
	{
		if (pEnemy->bUse) continue;

		pEnemy->bUse = true;
		pEnemy->pos = pos;
		pEnemy->rot = rot;
		pEnemy->fSpeed = fSpeed;
		pEnemy->nIdxShadow = SetShadow(pEnemy->pos, 100);
		SetSoundSpot(pos, SOUND_LABEL_SE_ENEMY);	// サウンドスポット設定

		if (pRoutine != NULL)
		{
			memcpy(&pEnemy->routine[0], pRoutine, sizeof(pEnemy->routine));

			for (int i = 0; i < MAX_ROUTINE; i++, pRoutine++)
			{
				if (pRoutine->nWait <= 0)
				{
					break;
				}
				pEnemy->nMaxRoutine++;
			}
		}

		break;
	}
}

void _SetEnemyState(int nIdx, ENEMYSTATE newState)
{
	ENEMY* pEnemy = &g_aEnemy[nIdx];

	pEnemy->currentState = newState;
	pEnemy->nCounterState = 0;
}

void _OnEnemyState(int nIdx)
{
	ENEMY* pEnemy = &g_aEnemy[nIdx];
	Player* pPlayer = GetPlayer();
	Player* pTarget = &pPlayer[pEnemy->nTarget];
	ENEMY_ROUTINE* pRoutine = &pEnemy->routine[pEnemy->nCurrentRoutine];

	// 状態別の処理
	switch (pEnemy->currentState)
	{
	case ENEMYSTATE_WAIT:		// 待機状態
	{
		// プレイヤーの向きを指定方向へ向ける
		float fRotDiff = GetFixedRotation(pEnemy->rotMove.y - pEnemy->rot.y);
		pEnemy->rot.y = GetFixedRotation(pEnemy->rot.y + fRotDiff * 0.035f);

		if (pEnemy->nCounterState > pRoutine->nWait)
		{// 待機時間終了→
			// 次のルーチンを設定
			pEnemy->nCurrentRoutine = (pEnemy->nCurrentRoutine + 1) % pEnemy->nMaxRoutine;
			pRoutine = &pEnemy->routine[pEnemy->nCurrentRoutine];

			// 移動先の設定
			pEnemy->destination = pRoutine->pos;

			// 移動状態へ遷移
			_SetEnemyState(nIdx, ENEMYSTATE_MOVE);
		}
		break;
	}

	case ENEMYSTATE_MOVE:		// 移動状態
	{
		// 敵を目的地に近づける
		pEnemy->pos += Direction(pEnemy->pos, pEnemy->destination) * pEnemy->fSpeed;

		// 敵の向きを移動方向へ向ける
		float fRotDiff = GetFixedRotation(pEnemy->rotMove.y - pEnemy->rot.y);
		pEnemy->rot.y = GetFixedRotation(pEnemy->rot.y + fRotDiff * 0.035f);

		if (Magnitude(pEnemy->pos, pEnemy->destination) < pEnemy->fSpeed)
		{// 目的地との距離が一定以下になったら
			// 敵の位置を目的地に設定
			pEnemy->pos = pEnemy->destination;

			// 待機状態へ遷移
			_SetEnemyState(nIdx, ENEMYSTATE_WAIT);
		}
		break;
	}

	case ENEMYSTATE_TURNING:	// 回転状態
		break;

	case ENEMYSTATE_FOUND:		// 発見状態
		// 敵をターゲット（発見したプレイヤー）の位置へ近づける
		D3DXVECTOR3 vToPlr = pTarget->pos - pEnemy->pos;
		pEnemy->pos += Normalize(vToPlr) * pEnemy->fSpeed;

		// 敵をターゲットの方向に向ける
		pEnemy->rotMove.y = GetFixedRotation(atan2f(vToPlr.x, vToPlr.z) + D3DX_PI);

		float fRotDiff = GetFixedRotation(pEnemy->rotMove.y - pEnemy->rot.y);
		pEnemy->rot.y = GetFixedRotation(pEnemy->rot.y + fRotDiff * 0.035f);

		break;
	}

	pEnemy->nCounterState++;
}

void _OnEnemyStateChanged(int nIdx)
{
	ENEMY* pEnemy = &g_aEnemy[nIdx];
	Player* pTarget = &GetPlayer()[pEnemy->nTarget];

	// 状態遷移時の処理
	switch (pEnemy->currentState)
	{
	case ENEMYSTATE_WAIT:
		// モーション設定（待機）
		SetMotion(&pEnemy->motion, ENEMY_MOTIONTYPE_IDLE, 10);

		// 方向を指定された向きに設定
  		pEnemy->rotMove.y = pEnemy->routine[pEnemy->nCurrentRoutine].rot.y;

		break;

	case ENEMYSTATE_MOVE:
	{
		// モーション設定（移動）
		SetMotion(&pEnemy->motion, ENEMY_MOTIONTYPE_MOVE, 10);

		// 方向を移動先に向ける
		D3DXVECTOR3 vecMove = pEnemy->destination - pEnemy->pos;
		pEnemy->rotMove.y = GetFixedRotation(atan2f(vecMove.x, vecMove.z) + D3DX_PI);
	}
		break;

	case ENEMYSTATE_TURNING:
		//SetMotion(&pEnemy->motion, ENEMY_MOTIONTYPE_IDLE, 10);
		//pEnemy->rotMove.y = GetFixedRotation(pEnemy->rotMove.y - pEnemy[pEnemy->nCurrentRoutine].rot.y);
		break;

	case ENEMYSTATE_FOUND:
		// モーション設定（移動）
		SetMotion(&pEnemy->motion, ENEMY_MOTIONTYPE_MOVE, 10);
		break;
	}
}