//==================================================
//
//	回転ノコギリ[saw.cpp]
//
//==================================================
//==================================================
//
//	ファイルインクルード
//
//==================================================
#include "Saw.h"
#include "main.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"

//==================================================
//
//	マクロ定義
//
//==================================================
#define MAX_MODEL		(128)					//モデルの最大数
#define SAW_MODEL_PATH	"data\\MODEL\\saw000.x"	//saw000.xへのパス

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
MESHDATA g_aSawModelData;
Saw g_aSaw[MAX_MODEL];

//==================================================
//
//	舞台照明の初期化
//
//==================================================
void InitSaw(void)
{
	int nNumVtx;	//頂点数
	DWORD dwSizeFVF;//頂点フォーマットのサイズ
	BYTE* pVtxBuff;	//頂点バッファへのポインタ

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	LoadModel(SAW_MODEL_PATH, &g_aSawModelData);
}

//==================================================
//
//	舞台照明の終了処理
//
//==================================================
void UninitSaw(void)
{
	ReleaseMesh(&g_aSawModelData);
}

//==================================================
//
//	舞台照明の更新処理
//
//==================================================
void UpdateSaw(void)
{
	
}

//==================================================
//
//	舞台照明の描画処理
//
//==================================================
void DrawSaw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL* pMat;				//マテリアルデータへのポインタ

	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		if (g_aSaw[nCnt].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aSaw[nCnt].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aSaw[nCnt].rot.y, g_aSaw[nCnt].rot.x, g_aSaw[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aSaw[nCnt].mtxWorld, &g_aSaw[nCnt].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aSaw[nCnt].pos.x, g_aSaw[nCnt].pos.y, g_aSaw[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aSaw[nCnt].mtxWorld, &g_aSaw[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aSaw[nCnt].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aSawModelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aSawModelData.dwNumMat; nCntMat++)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_aSawModelData.apTexture[nCntMat]);

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//モデル(パーツ)の描画
				g_aSawModelData.pMesh->DrawSubset(nCntMat);
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}
//==================================================
//
//	モデル設置処理
//
//==================================================
void SetSaw(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool startup)
{
	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		if (g_aSaw[nCnt].bUse == false)
		{
			g_aSaw[nCnt].bStartup = startup;
			g_aSaw[nCnt].bUse = true;
			g_aSaw[nCnt].pos = pos;
			g_aSaw[nCnt].rot = rot;
			break;
		}
	}
}
