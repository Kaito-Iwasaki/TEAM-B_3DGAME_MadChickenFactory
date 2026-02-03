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
#include "input.h"
#include "model.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define FIRE_MODELPATH	"data\\MODEL\\Factory\\gasburner00.x"	// モデルパス
#define FIRE_INTERVAL	(180)									// 炎の切り替え間隔

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
MESHDATA g_aFireModelData;					// 火炎放射器のモデル情報
FLAMETHROWER g_aflamethrower[MAX_FIRE];		// 火炎放射器情報
FIRE g_aFire[MAX_FIRE];						// 炎情報

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

	for (int nCntFire = 0; nCntFire < MAX_FIRE; nCntFire++)
	{
		g_aflamethrower[nCntFire].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置の初期化
		g_aflamethrower[nCntFire].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向きの初期化
		g_aflamethrower[nCntFire].state = OPERATIONSTATE_NONE;				// 状態の初期化
		g_aflamethrower[nCntFire].fireCounter = 0;							// 炎カウンター初期化
		g_aflamethrower[nCntFire].bUse = false;								// 使用していない状態にする
		g_aFire[nCntFire].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置初期化
		g_aFire[nCntFire].nSwitching = -1;									// 炎の切り替え順番初期化
		g_aFire[nCntFire].nIdx = -1;										// インデックス初期化
		g_aFire[nCntFire].state = FIRESTATE_OFF;							// 炎様態初期化
		g_aFire[nCntFire].bUse = false;										// 使用していない状態にする
	}

	LoadModel(FIRE_MODELPATH, &g_aFireModelData);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitFire(void)
{
	ReleaseMesh(&g_aFireModelData);
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateFire(void)
{
	FLAMETHROWER* pFlamethrower = &g_aflamethrower[0];		// 火炎放射器情報へのポインタ
	FIRE* pFire = &g_aFire[0];								// 炎情報へのポインタ
	Player* pPlayer = GetPlayer();							// プレイヤーへのポインタ
	FADE pFade = GetFade();									// フェード情報取得
	D3DXVECTOR3 move;										// 移動量

	for (int nCntFire = 0; nCntFire < MAX_FIRE; nCntFire++, pFlamethrower++, pFire++)
	{
		if (pFire->bUse == true)
		{// 使用している

			// 炎の状態事の処理
			switch (pFire->state)
			{
			case FIRESTATE_OFF:		// 炎OFF状態

				break;

			case FIRESTATE_READY:	// 炎準備状態


				move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 500) / 495 + 0.1f;
				move.y = 0.1f;
				move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 500) / 495 + 0.1f;

				// エフェクト設定
				SetEffect(pFire->pos, move, EFFECTTYPE_NOMALE, 60, D3DXCOLOR(0.8f, 0.3f, 0.1f, 1.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f));

				break;

			case FIRESTATE_ON:		// 炎ON状態

				move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 500) / 490 + 0.1f;
				move.y = (float)(rand() % 200) / 100 + 3.0f;
				move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 500) / 490 + 0.1f;

				// エフェクト設定
				SetEffect(pFire->pos, move, EFFECTTYPE_NOMALE, 60, D3DXCOLOR(0.8f, 0.3f, 0.1f, 1.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f));

				// 炎との当たり判定
				for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
				{
					if (CollisionPointBox(pPlayer[nCntPlayer].pos,
						D3DXVECTOR3(pFlamethrower->pos.x, pFlamethrower->pos.y + g_aFireModelData.vtxMax.y, pFlamethrower->pos.z),
						D3DXVECTOR3(pFlamethrower->fWidMax + pFlamethrower->fWidMin, 300.0f, pFlamethrower->fDepMax + pFlamethrower->fDepMin)) == true
						&& pFade.state == FADESTATE_NONE)
					{// 炎に当たった

						// 画面遷移する(GAME)
						SetFade(MODE_GAME);
					}
				}

				break;
			}
			
			// 炎の操作状態毎の処理
			switch (pFlamethrower->state)
			{
			case OPERATIONSTATE_AUTMATIC:		// 自動操作

				pFlamethrower->fireCounter++;		// 炎カウンター加算

				if (pFlamethrower->fireCounter > FIRE_INTERVAL)
				{// 炎をONOFFの切り替え

					pFire->state = (FIRESTATE)(int)(pFire->state + pFire->nSwitching);
					pFlamethrower->fireCounter = 0;	// カウント初期化

					if (pFire->state != FIRESTATE_READY)
					{// 炎の切り替え順番反転

						pFire->nSwitching *= -1;
					}
				}

				break;

			case OPERATIONSTATE_MANUAL:		// 自動操作

				if (GetKeyboardTrigger(DIK_F3) == true)
				{// 炎のONOFF切り替え

					if (pFire->state == FIRESTATE_ON)
					{// 炎をOFFにする

						pFire->state = FIRESTATE_OFF;
					}
					else
					{// 炎をONにする

						pFire->state = FIRESTATE_ON;
					}
				}

				break;
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

	for (int nCntFlaethrower = 0; nCntFlaethrower < MAX_FIRE; nCntFlaethrower++, pFlamethrower++)
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
			pMat = (D3DXMATERIAL*)g_aFireModelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aFireModelData.dwNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャ設定
				pDevice->SetTexture(0, g_aFireModelData.apTexture[nCntMat]);

				// ブロック(パーツの描画)
				g_aFireModelData.pMesh->DrawSubset(nCntMat);
			}

			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=====================================================================
// 火炎放射器の設定
//=====================================================================
void SetFlamethrower(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OPERATIONSTATE state)
{
	for (int nCntFlamethrower = 0; nCntFlamethrower < MAX_FIRE; nCntFlamethrower++)
	{
		if (g_aflamethrower[nCntFlamethrower].bUse == false)
		{
			g_aflamethrower[nCntFlamethrower].pos = pos;		// 位置
			g_aflamethrower[nCntFlamethrower].rot = rot;		// 向き
			g_aflamethrower[nCntFlamethrower].state = state;	// 状態
			g_aflamethrower[nCntFlamethrower].bUse = true;		// 使用している状態にする

			// ブロックの幅と奥行の設定
			SetFlamethrowerWidthAndDepth(nCntFlamethrower);

			// 炎の設定
			SetFire(nCntFlamethrower, D3DXVECTOR3(g_aflamethrower[nCntFlamethrower].pos.x, g_aflamethrower[nCntFlamethrower].pos.y + g_aFireModelData.vtxMax.y, g_aflamethrower[nCntFlamethrower].pos.z));
			
			break;
		}
	}
}
//=====================================================================
// 炎の設定
//=====================================================================
void SetFire(int nIdx, D3DXVECTOR3 pos)
{
	g_aFire[nIdx].pos = pos;				// 位置設定
	g_aFire[nIdx].nIdx = nIdx;				// インデックス設定
	g_aFire[nIdx].state = FIRESTATE_ON;		// 炎状態設定
	g_aFire[nIdx].bUse = true;				// 使用している状態にする
}

//=====================================================================
// 火炎放射器の幅と奥行の設定
//=====================================================================
void SetFlamethrowerWidthAndDepth(int nIdx)
{

	if (g_aflamethrower[nIdx].rot.y == 0.0f)
	{// 回転無し

		g_aflamethrower[nIdx].fWidMax = g_aFireModelData.vtxMax.x;		// 最大幅設定
		g_aflamethrower[nIdx].fWidMin = g_aFireModelData.vtxMin.x;		// 最小幅設定
		g_aflamethrower[nIdx].fDepMax = g_aFireModelData.vtxMax.z;		// 最大奥行設定
		g_aflamethrower[nIdx].fDepMin = g_aFireModelData.vtxMin.z;		// 最少奥行設定
	}
	else if (g_aflamethrower[nIdx].rot.y <= D3DX_PI / 2.0f && g_aflamethrower[nIdx].rot.y > 0)
	{// 90度回転

		g_aflamethrower[nIdx].fWidMax = g_aFireModelData.vtxMax.z;		// 最大幅設定
		g_aflamethrower[nIdx].fWidMin = g_aFireModelData.vtxMin.z;		// 最小幅設定
		g_aflamethrower[nIdx].fDepMax = g_aFireModelData.vtxMin.x;		// 最大奥行設定
		g_aflamethrower[nIdx].fDepMin = g_aFireModelData.vtxMax.x;		// 最少奥行設定
	}
	else if (g_aflamethrower[nIdx].rot.y >= D3DX_PI / -2.0f && g_aflamethrower[nIdx].rot.y < 0)
	{// -90度回転

		g_aflamethrower[nIdx].fWidMax = g_aFireModelData.vtxMin.z;		// 最大幅設定
		g_aflamethrower[nIdx].fWidMin = g_aFireModelData.vtxMax.z;		// 最小幅設定
		g_aflamethrower[nIdx].fDepMax = g_aFireModelData.vtxMax.x;		// 最大奥行設定
		g_aflamethrower[nIdx].fDepMin = g_aFireModelData.vtxMin.x;		// 最少奥行設定
	}
	else if (g_aflamethrower[nIdx].rot.y <= D3DX_PI && g_aflamethrower[nIdx].rot.y > D3DX_PI / 2.0f)
	{// 180度回転

		g_aflamethrower[nIdx].fWidMax = g_aFireModelData.vtxMin.x;		// 最大幅設定
		g_aflamethrower[nIdx].fWidMin = g_aFireModelData.vtxMax.x;		// 最小幅設定
		g_aflamethrower[nIdx].fDepMax = g_aFireModelData.vtxMin.z;		// 最大奥行設定
		g_aflamethrower[nIdx].fDepMin = g_aFireModelData.vtxMax.z;		// 最少奥行設定
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

	for(int nCntFlaethrower = 0; nCntFlaethrower < MAX_FIRE; nCntFlaethrower++, pFlamethrower++)
	{ 
		if (pFlamethrower->bUse == true)
		{// 使用している

			if (pPos->x + fRadius >= pFlamethrower->pos.x - pFlamethrower->fWidMin
				&& pPos->x - fRadius <= pFlamethrower->pos.x + pFlamethrower->fWidMax
				&& pPos->y + fRadius > pFlamethrower->pos.y + g_aFireModelData.vtxMin.y
				&& pPos->y - fRadius < pFlamethrower->pos.y + g_aFireModelData.vtxMax.y
				&& pPos->z + fRadius >= pFlamethrower->pos.z - pFlamethrower->fDepMin
				&& pPos->z - fRadius <= pFlamethrower->pos.z + pFlamethrower->fDepMax)
			{// ブロック範囲内

				if (pPosOld->y + fRadius <= pFlamethrower->pos.y + g_aFireModelData.vtxMin.y
					&& pPos->y + fRadius > pFlamethrower->pos.y + g_aFireModelData.vtxMin.y
					&& pPosOld->x + fRadius > pFlamethrower->pos.x - pFlamethrower->fWidMin
					&& pPosOld->x - fRadius < pFlamethrower->pos.x + pFlamethrower->fWidMax
					&& pPosOld->z + fRadius > pFlamethrower->pos.z - pFlamethrower->fDepMin
					&& pPosOld->z - fRadius < pFlamethrower->pos.z + pFlamethrower->fDepMax)
				{// 天井にめり込んだ

					bHitHead = true;
					nCount = nCntFlaethrower;
				}
				else if (pPosOld->y >= pFlamethrower->pos.y + g_aFireModelData.vtxMax.y
					&& pPos->y < pFlamethrower->pos.y + g_aFireModelData.vtxMax.y)
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

		pPos->y = g_aflamethrower[nCount].pos.y + g_aFireModelData.vtxMax.y;		// ブロックの上に立たせる
		pMove->y = 0.0f;															// 移動量を0にする

		pPlayer->bJump = false;			// ジャンプしていない状態にする
	}

	if (bHitHead == true)
	{// 頭が当たった

		pPos->y = g_aflamethrower[nCount].pos.y - fRadius + g_aFireModelData.vtxMin.y;	// ブロックの下に立たせる
		pMove->y = 0.0f;																	// 移動量を0にする
	}
}