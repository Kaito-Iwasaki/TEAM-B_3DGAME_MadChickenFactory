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
#define MAX_PRESS (128)				//プレス機の最大数
#define PRESS_MODEL_PATH	"data\\MODEL\\chicken.x"	//プレス機のモデルへのパス
#define DESCENTSPEED	(10.0f)		//プレス機の下降速度
#define DESCENT_LIMIT	(150.0f)	//プレス機の下降限界

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
		g_aPress[nCntPress].Setpos = D3DXVECTOR3_ZERO;
		g_aPress[nCntPress].interval = 0;
		g_aPress[nCntPress].rot = D3DXVECTOR3_ZERO;
		g_aPress[nCntPress].PState = PRESSSTATE_STAY;
	}

	LoadModel(PRESS_MODEL_PATH, &g_aPressModelData);
	SetPress(0,D3DXVECTOR3(100.0f, 100.0f, 200.0f), D3DXVECTOR3_ZERO, -1);
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
	if (GetKeyboardTrigger(DIK_F3))
	{
		PressMachineSwitch(0);
	}

	for (int nCntPress = 0; nCntPress < MAX_PRESS; nCntPress++)
	{
		if (g_aPress[nCntPress].bUse == true)
		{//使用時
			if (g_aPress[nCntPress].interval > 0)
			{//自動操縦
				if (g_aPress[nCntPress].bStartup == true)
				{//起動時

					if (g_aPress[nCntPress].PState == PRESSSTATE_DOWN)
					{//下降
						g_aPress[nCntPress].pos.y += ((g_aPress[nCntPress].Setpos.y - DESCENT_LIMIT) - g_aPress[nCntPress].pos.y) * 0.1f;
						if (((g_aPress[nCntPress].Setpos.y - g_aPress[nCntPress].pos.y) > (DESCENT_LIMIT - 0.5f)) == true)
						{//一定範囲まで落下しきったら切り替え
							g_aPress[nCntPress].PState = PRESSSTATE_UP;
						}
					}
					else if (g_aPress[nCntPress].PState == PRESSSTATE_UP)
					{//上昇
						g_aPress[nCntPress].pos.y += (g_aPress[nCntPress].Setpos.y - g_aPress[nCntPress].pos.y) * 0.05f;
						if (((g_aPress[nCntPress].Setpos.y - g_aPress[nCntPress].pos.y) < 0.5f) == true)
						{//一定範囲まで上昇しきったら切り替え
							g_aPress[nCntPress].PState = PRESSSTATE_STAY;
						}
					}
					else if (g_aPress[nCntPress].PState == PRESSSTATE_STAY)
					{//待機
						g_aPress[nCntPress].intervalCnt++;
						if ((g_aPress[nCntPress].intervalCnt >= g_aPress[nCntPress].interval) == true)
						{//設定間隔が経過
							g_aPress[nCntPress].PState = PRESSSTATE_DOWN;
							g_aPress[nCntPress].intervalCnt = 0;
						}
					}
				}

			}
			else
			{//手動操縦
			if (g_aPress[nCntPress].PState == PRESSSTATE_DOWN)
			{//下降
				g_aPress[nCntPress].pos.y += ((g_aPress[nCntPress].Setpos.y - DESCENT_LIMIT) - g_aPress[nCntPress].pos.y) * 0.1f;
				if (((g_aPress[nCntPress].Setpos.y - g_aPress[nCntPress].pos.y) > (DESCENT_LIMIT - 0.5f)) == true)
				{//一定範囲まで落下しきったら切り替え
					g_aPress[nCntPress].PState = PRESSSTATE_STAY;
				}
			}
			else if (g_aPress[nCntPress].PState == PRESSSTATE_UP)
			{//上昇
				g_aPress[nCntPress].pos.y += (g_aPress[nCntPress].Setpos.y - g_aPress[nCntPress].pos.y) * 0.05f;
				if (((g_aPress[nCntPress].Setpos.y - g_aPress[nCntPress].pos.y) < 0.5f) == true)
				{//一定範囲まで上昇しきったら切り替え
					g_aPress[nCntPress].PState = PRESSSTATE_STAY;
				}
			}
			}
		}
	}
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
void SetPress(int nIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, int interval)
{//IDX,位置、角度、インターバルの設定
	g_aPress[nIdx].bUse = true;
	g_aPress[nIdx].bStartup = true;
	g_aPress[nIdx].pos = pos;
	g_aPress[nIdx].Setpos = pos;
	g_aPress[nIdx].interval = interval;
	g_aPress[nIdx].rot = rot;

}

//==================================================
//
//	スイッチ切り替え処理
//
//==================================================
void PressMachineSwitch(int nIdx)
{
	if ((g_aPress[nIdx].Setpos.y - g_aPress[nIdx].pos.y) >= (DESCENT_LIMIT - 0.5f) == true)
	{//落下しきっていたら
		g_aPress[nIdx].PState = PRESSSTATE_UP;
	}
	else if (((g_aPress[nIdx].Setpos.y - g_aPress[nIdx].pos.y) < 0.5f) == true)
	{//上昇しきっていたら切り替え
		g_aPress[nIdx].PState = PRESSSTATE_DOWN;
	}
}