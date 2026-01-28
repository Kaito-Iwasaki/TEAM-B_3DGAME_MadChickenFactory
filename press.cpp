//=====================================================================
//
// press [press.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "press.h"
#include "main.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"
#include "fade.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_PRESS (128)		//プレス機の最大数
#define PRESS_MODEL_PATH	"data\\MODEL\\chicken.x"	//プレス機のモデルへのパス

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
MESHDATA g_aPressModelData;
Press g_aPress[MAX_PRESS];

//=====================================================================
// 初期化処理
//=====================================================================
void InitPress(void)
{
	for (int nCntPress = 0; nCntPress < MAX_PRESS; nCntPress++)
	{
		g_aPress[nCntPress].bStartup = false;
		g_aPress[nCntPress].bUse = false;
		g_aPress[nCntPress].pos = D3DXVECTOR3_ZERO;
		g_aPress[nCntPress].rot = D3DXVECTOR3_ZERO;
		g_aPress[nCntPress].fInterval = 0;
	}

	LoadModel(PRESS_MODEL_PATH, &g_aPressModelData);
	SetPress(D3DXVECTOR3(200.0f, 0.0f, 0.0f), D3DXVECTOR3_ZERO, 0);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitPress(void)
{
	ReleaseMesh(&g_aPressModelData);
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdatePress(void)
{

}

//=====================================================================
// 描画処理
//=====================================================================
void DrawPress(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL* pMat;				//マテリアルデータへのポインタ

	for (int nCnt = 0; nCnt < MAX_PRESS; nCnt++)
	{
		if (g_aPress[nCnt].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aPress[nCnt].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPress[nCnt].rot.y, g_aPress[nCnt].rot.x, g_aPress[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aPress[nCnt].mtxWorld, &g_aPress[nCnt].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aPress[nCnt].pos.x, g_aPress[nCnt].pos.y, g_aPress[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aPress[nCnt].mtxWorld, &g_aPress[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aPress[nCnt].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aPressModelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aPressModelData.dwNumMat; nCntMat++)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_aPressModelData.apTexture[nCntMat]);

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//モデル(パーツ)の描画
				g_aPressModelData.pMesh->DrawSubset(nCntMat);
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}
//==================================================
//
//	プレス機設置処理
//
//==================================================
void SetPress(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool Interval)
{
	for (int nCnt = 0; nCnt < MAX_PRESS; nCnt++)
	{
		if (g_aPress[nCnt].bUse == false)
		{
			g_aPress[nCnt].bStartup = Interval;
			g_aPress[nCnt].bUse = true;
			g_aPress[nCnt].pos = pos;
			g_aPress[nCnt].rot = rot;
			break;
		}
	}
}