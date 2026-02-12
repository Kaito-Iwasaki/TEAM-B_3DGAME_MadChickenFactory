//=====================================================================
//
// モーション処理 [motion.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "motion.h"
#include "util.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************


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

//=====================================================================
// モーション初期化処理
// [pFilename].txtからモーション情報を読み込み、
// パーツ（モデル）をpMotionにセットします。
//=====================================================================
void InitMotion(MOTION* pMotion, const char* pFilename)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL* pMat;

	// 構造体を初期化
	ZeroMemory(pMotion, sizeof(MOTION));

	// モーションスクリプトの読み込み
	LoadMotionScript(pFilename, pMotion);

	// パーツ（モデル）読み込み
	for (int nCountPart = 0; nCountPart < pMotion->nNumPart; nCountPart++)
	{
		PART* pPart = &pMotion->aPart[nCountPart];	// パーツ

		// Xファイルの読み込み
		D3DXLoadMeshFromX(
			&pMotion->aPartFilename[nCountPart][0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&pPart->pBuffMat,
			NULL,
			&pPart->dwNumMat,
			&pPart->pMesh
		);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)pPart->pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)pPart->dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				// テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&pPart->apTexture[nCntMat]);
			}
		}
	}

	// パーツのオフセット値を適用
	SetPartOffset(pMotion);
}

//=====================================================================
// モーション解放処理
// pMotionのパーツ（モデル）を解放します。
//=====================================================================
void UninitMotion(MOTION* pMotion)
{
	for (int nCountPart = 0; nCountPart < pMotion->nNumPart; nCountPart++)
	{
		PART* pPart = &pMotion->aPart[nCountPart];		// パーツ

		// メッシュの破棄
		RELEASE(pPart->pMesh);

		// マテリアルの破棄
		RELEASE(pPart->pBuffMat);

		for (int nCntTex = 0; nCntTex < (int)pPart->dwNumMat; nCntTex++)
		{// テクスチャの破棄
			RELEASE(pPart->apTexture[nCntTex]);
		}
	}
}

//=====================================================================
// モーション更新処理
// モーションの再生カウンタを進め、パーツの位置を設定します。
//=====================================================================
void UpdateMotion(MOTION* pMotion)
{
	MOTION_INFO* pCurrentMotion = &pMotion->aMotionInfo[pMotion->nIdxMotion];						// 現在のモーション情報
	KEY_INFO* pCurrentKeyInfo = &pCurrentMotion->aKeyInfo[pMotion->nKeyMotion];						// 現在のキー情報
	KEY_INFO* pNextKeyInfo = &pCurrentMotion->aKeyInfo[GetNextKeyIndex(pMotion)];					// 次のキー情報
	MOTION_INFO* pCurrentBlendMotion = &pMotion->aMotionInfo[pMotion->nIdxMotionBlend];				// 現在のブレンドモーション情報
	KEY_INFO* pCurrentBlendKeyInfo = &pCurrentBlendMotion->aKeyInfo[pMotion->nKeyMotionBlend];		// 現在のブレンドキー情報
	KEY_INFO* pNextBlendKeyInfo = &pCurrentBlendMotion->aKeyInfo[GetNextBlendKeyIndex(pMotion)];	// 次のブレンドキー情報

	for (int nCountPart = 0; nCountPart < pMotion->nNumPart; nCountPart++)
	{
		PART* pPart = &pMotion->aPart[nCountPart];								// パーツ情報
		KEY* pCurrentKey = &pCurrentKeyInfo->aKey[nCountPart];					// 現在のキー情報
		KEY* pNextKey = &pNextKeyInfo->aKey[nCountPart];						// 次のキー情報
		KEY* pCurrentBlendKey = &pCurrentBlendKeyInfo->aKey[nCountPart];		// 現在のブレンドキー情報
		KEY* pNextBlendKey = &pNextBlendKeyInfo->aKey[nCountPart];				// 次のブレンドキー情報

		// 現在のモーションフレームとモーション全体フレームとの割合を求める
		float fRateKey = (float)pMotion->nCounterMotion / (float)pCurrentKeyInfo->nFrame;

		// 現在のキー位置から次のキー位置までの差分を求める
		D3DXVECTOR3 vecDiffKeyPos = pNextKey->pos - pCurrentKey->pos;
		D3DXVECTOR3 vecDiffKeyRot = GetFixedRotation(pNextKey->rot - pCurrentKey->rot);

		if (pMotion->bBlendMotion)
		{// ブレンドモーションを再生
			// 現在のブレンドモーションフレームとブレンドモーションの全体フレームとの割合を求める
			float fRateBlendKey = (float)pMotion->nCounterMotionBlend / (float)pCurrentBlendKeyInfo->nFrame;

			// 現在のブレンド状態のフレームとブレンド状態の全体フレームとの割合を求める
			float fRateBlend = (float)pMotion->nCounterBlend / (float)pMotion->nFrameBlend;

			// 現在のキー位置からブレンドキー位置までの差分を求める
			D3DXVECTOR3 vecDiffBlendKeyPos = pNextBlendKey->pos - pCurrentBlendKey->pos;
			D3DXVECTOR3 vecDiffBlendKeyRot = GetFixedRotation(pNextBlendKey->rot - pCurrentBlendKey->rot);

			// 現在のフレーム時点でのブレンドキー位置を求める
			D3DXVECTOR3 vecBlendKeyPos = pCurrentBlendKey->pos + vecDiffBlendKeyPos * fRateBlendKey;
			D3DXVECTOR3 vecBlendKeyRot = GetFixedRotation(pCurrentBlendKey->rot + vecDiffBlendKeyRot * fRateBlendKey);

			// 現在のキー位置からモーションとブレンドモーションの中間位置までの差分を求める
			D3DXVECTOR3 vecDiffBlendPos = vecBlendKeyPos - pCurrentKey->pos;
			D3DXVECTOR3 vecDiffBlendRot = GetFixedRotation(vecBlendKeyRot - pCurrentKey->rot);

			// パーツの位置（二つのモーションがブレンドされた位置）を求める
			pPart->pos = pCurrentKey->pos + vecDiffBlendPos * fRateBlend;
			pPart->rot = GetFixedRotation(pCurrentKey->rot + vecDiffBlendRot * fRateBlend);
		}
		else
		{// モーションを再生
			// 現在のモーションフレーム分の差分を加算する
			pPart->pos = pCurrentKey->pos + vecDiffKeyPos * fRateKey;
			pPart->rot = GetFixedRotation(pCurrentKey->rot + vecDiffKeyRot * fRateKey);
		}
	}

	// パーツのオフセットを設定
	AddPartOffset(pMotion);

	if (pMotion->bFinishMotion == false)
	{// モーション再生中
		if (pMotion->bBlendMotion == true)
		{// ブレンドモーションを再生
			pMotion->nCounterMotionBlend++;

			if (pMotion->nCounterMotionBlend > pCurrentBlendKeyInfo->nFrame)
			{// フレームが現在のキーの最大フレームに達したら次のキーへ
				int nNextBlendKeyIndex = GetNextBlendKeyIndex(pMotion);

				if (nNextBlendKeyIndex == 0 && pMotion->bLoopMotionBlend == false)
				{// 次のキーが0かつループしないなら、ブレンドモーションを初期モーションに設定
					pMotion->nIdxMotion = 0;
					pMotion->nKeyMotionBlend = 0;
					pMotion->nCounterMotionBlend = 0;
					pMotion->nCounterBlend = 0;
				}
				else
				{// 最初のキーに戻る（ループ）
					pMotion->nKeyMotionBlend = GetNextBlendKeyIndex(pMotion);
					pMotion->nCounterMotionBlend = 0;
				}
			}

			// ブレンド状態のカウンタを進める
			pMotion->nCounterBlend++;

			if (pMotion->nCounterBlend > pMotion->nFrameBlend)
			{// ブレンドの終了→ブレンドモーションを現在のモーションに設定する
				pMotion->bBlendMotion = false;
				pMotion->nIdxMotion = pMotion->nIdxMotionBlend;
				pMotion->nCounterMotion = pMotion->nCounterMotionBlend;
				pMotion->nKeyMotion = pMotion->nKeyMotionBlend;
			}
		}
		else
		{// モーションを再生
			// モーションフレームを進める
			pMotion->nCounterMotion++;

			if (pMotion->nCounterMotion > pCurrentKeyInfo->nFrame)
			{// フレームが現在のキーの最大フレームに達したら次のキーへ
				int nNextKeyIndex = GetNextKeyIndex(pMotion);

				if (nNextKeyIndex == 0 && pMotion->bLoopMotion == false)
				{// 次のキーが0かつループしないなら、モーションの再生を終了する
					pMotion->bFinishMotion = true;
				}
				else
				{// 最初のキーに戻る（ループ）
					pMotion->nKeyMotion = GetNextKeyIndex(pMotion);
					pMotion->nCounterMotion = 0;
				}
			}
		}
	}
}

//=====================================================================
// pMotionの現在のモーション番号をnIdxMotionに設定します。
// nFrameBlendが0より大きければ、現在のモーションをブレンドしながら
// 指定されたモーションに移行します。
//=====================================================================
void SetMotion(MOTION* pMotion, int nIdxMotion, int nFrameBlend)
{
	// モーションを再生状態に
	pMotion->bFinishMotion = false;

	if (nFrameBlend > 0)
	{// ブレンドでモーションを設定
		pMotion->bBlendMotion = true;
		pMotion->nIdxMotionBlend = nIdxMotion;
		pMotion->nCounterMotionBlend = 0;
		pMotion->nCounterBlend = 0;
		pMotion->nKeyMotionBlend = 0;
		pMotion->nFrameBlend = nFrameBlend;
	}
	else
	{// 通常通りにモーションを設定
		pMotion->bBlendMotion = false;
		pMotion->nIdxMotion = nIdxMotion;
		pMotion->nCounterMotion = 0;
		pMotion->nKeyMotion;
	}
}

//=====================================================================
// pMotionのパーツにそれぞれの位置・回転オフセット値を適用します。
//=====================================================================
void SetPartOffset(MOTION* pMotion)
{
	for (int nCountPart = 0; nCountPart < pMotion->nNumPart; nCountPart++)
	{
		PART* pPart = &pMotion->aPart[nCountPart];

		pPart->pos = pPart->posOffset;
		pPart->rot = pPart->rotOffset;
	}
}

//=====================================================================
// pMotionのパーツにそれぞれの位置・回転オフセット値を加算します。
//=====================================================================
void AddPartOffset(MOTION* pMotion)
{
	for (int nCountPart = 0; nCountPart < pMotion->nNumPart; nCountPart++)
	{
		PART* pPart = &pMotion->aPart[nCountPart];

		pPart->pos += pPart->posOffset;
		pPart->rot = GetFixedRotation(pPart->rot + pPart->rotOffset);
	}
}

//=====================================================================
// pMotionの次のモーションキーのインデックスを取得します。
//=====================================================================
int GetNextKeyIndex(MOTION* pMotion)
{
	int nIdxMotion = pMotion->nIdxMotion;
	int nIdxKey = pMotion->nKeyMotion;							
	int nMaxKey = pMotion->aMotionInfo[nIdxMotion].nNumKey;

	int nIdxNext = (nIdxKey + 1) % nMaxKey;

	return nIdxNext;
}

//=====================================================================
// pMotionの次のブレンドモーションキーのインデックスを取得します。
//=====================================================================
int GetNextBlendKeyIndex(MOTION* pMotion)
{
	int nIdxMotionBlend = pMotion->nIdxMotionBlend;
	int nIdxKeyBlend = pMotion->nKeyMotionBlend;
	int nMaxKeyBlend = pMotion->aMotionInfo[nIdxMotionBlend].nNumKey;

	int nIdxNext = (nIdxKeyBlend + 1) % nMaxKeyBlend;

	return nIdxNext;
}
