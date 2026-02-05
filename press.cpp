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
#include "collision.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define PRESS_MODEL_PATH	"data\\MODEL\\chicken.x"	//プレス機のモデルへのパス
#define PRESS_UP_SPEED		(7.0f)			//プレス機の上昇速度
#define PRESS_DOWN_SPEED	(30.0f)			//プレス機の下降速度

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
		g_aPress[nCntPress].movewidth = 0.0f;
		g_aPress[nCntPress].interval = 0;
		g_aPress[nCntPress].rot = D3DXVECTOR3_ZERO;
		g_aPress[nCntPress].PState = PRESSSTATE_STAY;
		g_aPress[nCntPress].bprevious = PRESSSTATE_STAY;
	}

	LoadModel(PRESS_MODEL_PATH, &g_aPressModelData);
	SetPress(0, D3DXVECTOR3(-200.0f,0.0f,0.0f), D3DXVECTOR3_ZERO, 500.0f, 60, PRESSSTATE_UP);
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
	Player* pPlayer = GetPlayer();

	for (int nCntPress = 0; nCntPress < MAX_PRESS; nCntPress++)
	{
		if (g_aPress[nCntPress].bUse == true && g_aPress[nCntPress].bStartup == true)
		{//使用時&&起動時

			if (g_aPress[nCntPress].PState == PRESSSTATE_DOWN)
			{//下降
				g_aPress[nCntPress].pos.y -= PRESS_DOWN_SPEED;
				if (g_aPress[nCntPress].pos.y <= g_aPress[nCntPress].memheight - g_aPress[nCntPress].movewidth)
				{//下降前の座標よりも設定値分下
					//pos矯正
					g_aPress[nCntPress].pos.y = g_aPress[nCntPress].memheight - g_aPress[nCntPress].movewidth;

					//stateの書き換えと、保存
					g_aPress[nCntPress].PState = PRESSSTATE_STAY;
					g_aPress[nCntPress].bprevious = PRESSSTATE_DOWN;

					//現在の座標を保存
					g_aPress[nCntPress].memheight = g_aPress[nCntPress].pos.y;

				}
			}
			else if (g_aPress[nCntPress].PState == PRESSSTATE_UP)
			{//上昇
				g_aPress[nCntPress].pos.y += PRESS_UP_SPEED;
				if (g_aPress[nCntPress].pos.y >= g_aPress[nCntPress].memheight + g_aPress[nCntPress].movewidth)
				{//上昇前の座標よりも上
					//pos矯正
					g_aPress[nCntPress].pos.y = g_aPress[nCntPress].memheight + g_aPress[nCntPress].movewidth;

					//stateの書き換えと、保存
					g_aPress[nCntPress].PState = PRESSSTATE_STAY;
					g_aPress[nCntPress].bprevious = PRESSSTATE_UP;

					//現在の座標を保存
					g_aPress[nCntPress].memheight = g_aPress[nCntPress].pos.y;

				}
			}
			else if (g_aPress[nCntPress].PState == PRESSSTATE_STAY)
			{//待機

				if (g_aPress[nCntPress].interval >= 0)
				{//自動操縦
					//インターバルカウンタ進
					g_aPress[nCntPress].intervalCnt++;

					if (g_aPress[nCntPress].interval <= g_aPress[nCntPress].intervalCnt)
					{
						if (g_aPress[nCntPress].bprevious == PRESSSTATE_DOWN)
						{//前回下降していたら、上昇
							g_aPress[nCntPress].PState = PRESSSTATE_UP;
						}
						else if (g_aPress[nCntPress].bprevious == PRESSSTATE_UP)
						{//前回上昇していたら、下降
							g_aPress[nCntPress].PState = PRESSSTATE_DOWN;
						}
						//インターバルカウンタを０に
						g_aPress[nCntPress].intervalCnt = 0;
					}
				}
				else if(g_aPress[nCntPress].interval < 0)
				{//手動操縦
					if (GetKeyboardTrigger(DIK_F3) || GetPromptTrigger(nCntPress))
					{
						PressMachineSwitch(0);
					}
				}
			}

			//************************************
			// プレイヤーとの当たり判定
			//************************************
			if (g_aPress[nCntPress].PState == PRESSSTATE_DOWN)
			{
				if (CollisionPointBox(pPlayer->pos, g_aPress[nCntPress].pos, g_aPressModelData.vtxMin, g_aPressModelData.vtxMax))
				{
					if (g_aPress[nCntPress].pos.y >= pPlayer->pos.y)
					{
						SetFade(MODE_GAME);
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
void SetPress(int nIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float movewidth, int interval, PressState state)
{//IDX,位置、角度、インターバルの設定
	for (int nCntPress = 0; nCntPress < MAX_PRESS; nCntPress++)
	{
		if (g_aPress[nCntPress].bUse == false)
		{
			g_aPress[nCntPress].nIdx = nIdx;
			g_aPress[nCntPress].bUse = true;
			g_aPress[nCntPress].bStartup = true;
			g_aPress[nCntPress].pos = pos;
			g_aPress[nCntPress].memheight = pos.y;
			g_aPress[nCntPress].movewidth = movewidth;
			g_aPress[nCntPress].interval = interval;
			g_aPress[nCntPress].rot = rot;
			g_aPress[nCntPress].PState = state;
			break;
		}
	}

}

//==================================================
//
//	スイッチ切り替え処理
//
//==================================================
void PressMachineSwitch(int nIdx)
{
	for (int nCntPress = 0; nCntPress < MAX_PRESS; nCntPress++)
	{
		if ((g_aPress[nCntPress].bUse == true) && (g_aPress[nCntPress].nIdx == nIdx))
		{
			if (g_aPress[nCntPress].bprevious == PRESSSTATE_DOWN)
			{//前回下降していたら、上昇
				g_aPress[nCntPress].PState = PRESSSTATE_UP;
			}
			else if (g_aPress[nCntPress].PState == PRESSSTATE_UP)
			{//前回上昇していたら、下降
				g_aPress[nCntPress].PState = PRESSSTATE_DOWN;
			}
		}
	}
}