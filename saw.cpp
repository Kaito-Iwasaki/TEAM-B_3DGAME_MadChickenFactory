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
#include "player.h"
#include "fade.h"

//==================================================
//
//	マクロ定義
//
//==================================================
#define MAX_SAW		(128)					//モデルの最大数
#define SAW_MODEL_PATH	"data\\MODEL\\saw000.x"	//saw000.xへのパス
#define MAX_SAW_SPEED	(0.15f)				//ノコギリの回転速度のMAX

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
Saw g_aSaw[MAX_SAW];

//==================================================
//
//	回転ノコギリの初期化
//
//==================================================
void InitSaw(void)
{
	for (int nCntSaw = 0; nCntSaw < MAX_SAW; nCntSaw++)
	{
		g_aSaw[nCntSaw].bStartup = false;
		g_aSaw[nCntSaw].bUse = false;
		g_aSaw[nCntSaw].pos = D3DXVECTOR3_ZERO;
		g_aSaw[nCntSaw].rot = D3DXVECTOR3_ZERO;
		g_aSaw[nCntSaw].turnSpeed = 0;
	}

	LoadModel(SAW_MODEL_PATH, &g_aSawModelData);

	SetSaw(D3DXVECTOR3(200.0f, 30.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), true);
}

//==================================================
//
//	回転ノコギリの終了処理
//
//==================================================
void UninitSaw(void)
{
	ReleaseMesh(&g_aSawModelData);
}

//==================================================
//
//	回転ノコギリの更新処理
//
//==================================================
void UpdateSaw(void)
{
	if (GetKeyboardTrigger(DIK_F3))
	{
		SwitchSaw(0);
	}

	for (int nCntSaw = 0; nCntSaw < MAX_SAW; nCntSaw++)
	{
		if (g_aSaw[nCntSaw].bUse == true)
		{
			if (g_aSaw[nCntSaw].bStartup == true)
			{//起動スイッチがON
				//MAX_SAW_SPEEDまで速度をあげながら回転
				g_aSaw[nCntSaw].turnSpeed += (MAX_SAW_SPEED - g_aSaw[nCntSaw].turnSpeed) * 0.005f;
				g_aSaw[nCntSaw].rot.z += g_aSaw[nCntSaw].turnSpeed;
				
			}
			else
			{//OFF
				//0.0まで速度を徐々に落とす
				g_aSaw[nCntSaw].turnSpeed += (0.0f - g_aSaw[nCntSaw].turnSpeed) * 0.02f;
				g_aSaw[nCntSaw].rot.z += g_aSaw[nCntSaw].turnSpeed;

			}
		}
	}

	CollisionSaw();
}

//==================================================
//
//	回転ノコギリの描画処理
//
//==================================================
void DrawSaw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL* pMat;				//マテリアルデータへのポインタ

	for (int nCnt = 0; nCnt < MAX_SAW; nCnt++)
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
//	回転ノコギリ設置処理
//
//==================================================
void SetSaw(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool startup)
{
	for (int nCnt = 0; nCnt < MAX_SAW; nCnt++)
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
//==================================================
//
//	回転ノコギリの当たり判定
//
//==================================================
void CollisionSaw(void)
{
	Player* pPlayer = GetPlayer();

	for (int nCntSaw = 0; nCntSaw < MAX_SAW; nCntSaw++)
	{
		if (g_aSaw[nCntSaw].bUse == true)
		{
			
		}
	}
}
//==================================================
//
//	回転ノコギリのスイッチを切り替え
//
//==================================================
void SwitchSaw(int nIdx)
{
	g_aSaw[nIdx].bStartup ^= true;
}
