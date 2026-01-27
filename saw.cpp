//==================================================
//
//	舞台照明[model.cpp]
//
//==================================================
//==================================================
//
//	ファイルインクルード
//
//==================================================
#include "StageLight.h"
#include "main.h"
#include "input.h"
#include "debugproc.h"

//==================================================
//
//	マクロ定義
//
//==================================================
#define MAX_MODEL		(32)		//モデルの最大数
#define DISTANCE_SLIGHT	(600.0f)	//中心との距離
#define MISALIGNMENT	(300.0f)	//横ずれ

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
//LPD3DXMESH g_aStageLight[nCnt].Mesh = NULL;			//メッシュ（頂点情報）へのポインタ
//LPD3DXBUFFER g_aStageLight[nCnt].BuffMat = NULL;	//マテリアルへのポインタ
//DWORD g_aStageLight[nCnt].dwNumMatStageLight = 0;				//マテリアルの数
//D3DXMATRIX g_aStageLight[nCnt].mtxWorld;				//ワールドマトリックス
//LPDIRECT3DTEXTURE9 g_aStageLight[nCnt].Texture[MAX_TEXTURE] = {};	//テクスチャへのポインタ
StageLight g_aStageLight[MAX_MODEL];

//==================================================
//
//	舞台照明の初期化
//
//==================================================
void InitStageLight(void)
{
	int nNumVtx;	//頂点数
	DWORD dwSizeFVF;//頂点フォーマットのサイズ
	BYTE* pVtxBuff;	//頂点バッファへのポインタ

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		g_aStageLight[nCnt].pos = INIT_D3DXVECTOR3;
		g_aStageLight[nCnt].rot = INIT_D3DXVECTOR3;
		g_aStageLight[nCnt].bUse = false;

		// Xファイル読み込み
		D3DXLoadMeshFromX("data\\model\\light.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aStageLight[nCnt].BuffMat,
			NULL,
			&g_aStageLight[nCnt].dwNumMat,
			&g_aStageLight[nCnt].Mesh);

		D3DXMATERIAL* pMat;

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_aStageLight[nCnt].BuffMat->GetBufferPointer();

		for (int nCntMdl = 0; nCntMdl < g_aStageLight[nCnt].dwNumMat; nCntMdl++)
		{//テクスチャファイルを読み込む
			if (pMat[nCntMdl].pTextureFilename != NULL)
			{//テクスチャファイルが存在する
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMdl].pTextureFilename, &g_aStageLight[nCnt].Texture[nCntMdl]);
			}

		}
	}

	SetStageLight(D3DXVECTOR3(MISALIGNMENT,0.0f, DISTANCE_SLIGHT), D3DXVECTOR3(0.0f, D3DX_PI * 0.2f, 0.0f));
	SetStageLight(D3DXVECTOR3(DISTANCE_SLIGHT, 0.0f, MISALIGNMENT), D3DXVECTOR3(0.0f, D3DX_PI * 0.35f, 0.0f));
	SetStageLight(D3DXVECTOR3(DISTANCE_SLIGHT, 0.0f, -MISALIGNMENT), D3DXVECTOR3(0.0f, D3DX_PI * 0.7f, 0.0f));
	SetStageLight(D3DXVECTOR3(MISALIGNMENT, 0.0f, -DISTANCE_SLIGHT), D3DXVECTOR3(0.0f, D3DX_PI * 0.9f, 0.0f));
	SetStageLight(D3DXVECTOR3(-DISTANCE_SLIGHT, 0.0f, -MISALIGNMENT), D3DXVECTOR3(0.0f, -D3DX_PI * 0.7f, 0.0f));
	SetStageLight(D3DXVECTOR3(-MISALIGNMENT, 0.0f, -DISTANCE_SLIGHT), D3DXVECTOR3(0.0f, -D3DX_PI * 0.9f, 0.0f));
	SetStageLight(D3DXVECTOR3(-DISTANCE_SLIGHT, 0.0f, MISALIGNMENT), D3DXVECTOR3(0.0f, -D3DX_PI * 0.35f, 0.0f));
	SetStageLight(D3DXVECTOR3(-MISALIGNMENT, 0.0f, DISTANCE_SLIGHT), D3DXVECTOR3(0.0f, -D3DX_PI * 0.2f, 0.0f));

}

//==================================================
//
//	舞台照明の終了処理
//
//==================================================
void UninitStageLight(void)
{
	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		//メッシュ破棄
		if (g_aStageLight[nCnt].Mesh != NULL)
		{
			g_aStageLight[nCnt].Mesh->Release();
			g_aStageLight[nCnt].Mesh = NULL;
		}

		//マテリアルの破棄
		if (g_aStageLight[nCnt].BuffMat != NULL)
		{
			g_aStageLight[nCnt].BuffMat->Release();
			g_aStageLight[nCnt].BuffMat = NULL;
		}
	}
}

//==================================================
//
//	舞台照明の更新処理
//
//==================================================
void UpdateStageLight(void)
{
	
}

//==================================================
//
//	舞台照明の描画処理
//
//==================================================
void DrawStageLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL* pMat;				//マテリアルデータへのポインタ

	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		if (g_aStageLight[nCnt].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aStageLight[nCnt].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aStageLight[nCnt].rot.y, g_aStageLight[nCnt].rot.x, g_aStageLight[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aStageLight[nCnt].mtxWorld, &g_aStageLight[nCnt].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aStageLight[nCnt].pos.x, g_aStageLight[nCnt].pos.y, g_aStageLight[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aStageLight[nCnt].mtxWorld, &g_aStageLight[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aStageLight[nCnt].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aStageLight[nCnt].BuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aStageLight[nCnt].dwNumMat; nCntMat++)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_aStageLight[nCnt].Texture[nCntMat]);

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//モデル(パーツ)の描画
				g_aStageLight[nCnt].Mesh->DrawSubset(nCntMat);
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
void SetStageLight(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		if (g_aStageLight[nCnt].bUse == false)
		{
			g_aStageLight[nCnt].bUse = true;
			g_aStageLight[nCnt].pos = pos;
			g_aStageLight[nCnt].rot = rot;
			break;
		}
	}
}
