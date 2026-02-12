//=====================================================================
//
// 門処理 [gate.cpp]
// Author : 土田青空
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "gate.h"
#include "DebugProc.h"
#include"fade.h"
#include "model.h"
#include "prompt.h"
#include "collision.h"
#include"player.h"
//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define GATE_MODELPATH	"data\\MODEL\\Factory\\gate.x"	// モデルパス
#define GATE_UP_SPEED (5.0f)	//上がる速度
#define GATE_UP_MAX (300.0f)	//上がるまでの上限
//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** 構造体 *****
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
MESHDATA g_aGateModelData;					// 火炎放射器のモデル情報
Gate g_aGate[MAX_GATE];
//=====================================================================
// 初期化処理
//=====================================================================
void InitGate(void)
{

	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{
		g_aGate[nCntGate].bGoup = false;
		g_aGate[nCntGate].bUse = false;
		g_aGate[nCntGate].pos = D3DXVECTOR3_ZERO;
		g_aGate[nCntGate].rot = D3DXVECTOR3_ZERO;
		
	}
	

	LoadModel(GATE_MODELPATH, &g_aGateModelData);
	//SetGate(0, D3DXVECTOR3(300.0f, -1.0f, 0.0f), D3DXVECTOR3_ZERO, 300.0f, false ,GATESTATE_UP);

}

//=====================================================================
// 終了処理
//=====================================================================
void UninitGate(void)
{
	ReleaseMesh(&g_aGateModelData);
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateGate(void)
{
	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{
		
			if (GetPromptTrigger(g_aGate[nCntGate].nIdx))
			{
				SwitchGate(nCntGate);
			}
			if (g_aGate[nCntGate].bGoup == true && g_aGate[nCntGate].bUse == true)
			{
				if (g_aGate[nCntGate].Gstate == GATESTATE_UP)
				{//上昇
					g_aGate[nCntGate].pos.y += GATE_UP_SPEED;
					if (g_aGate[nCntGate].pos.y >= g_aGate[nCntGate].memheight + g_aGate[nCntGate].movewidth)
					{//上昇前の座標よりも設定値分上
						//pos矯正
						g_aGate[nCntGate].pos.y = g_aGate[nCntGate].memheight + g_aGate[nCntGate].movewidth;

						g_aGate[nCntGate].Gstate = GATESTATE_STAY;
						g_aGate[nCntGate].bprevious = GATESTATE_UP;

						//現在の座標を保存
						g_aGate[nCntGate].memheight = g_aGate[nCntGate].pos.y;

						g_aGate[nCntGate].bGoup = false;					//起動初期化
					}
				}
				else if (g_aGate[nCntGate].Gstate == GATESTATE_DOWN)
				{//下降
					g_aGate[nCntGate].pos.y -= GATE_UP_SPEED;
					if (g_aGate[nCntGate].pos.y <= g_aGate[nCntGate].memheight - g_aGate[nCntGate].movewidth)
					{//下降前の座標よりも設定値分下
						//pos矯正
						g_aGate[nCntGate].pos.y = g_aGate[nCntGate].memheight - g_aGate[nCntGate].movewidth;

						//stateの書き換えと、保存
						g_aGate[nCntGate].Gstate = GATESTATE_STAY;			
						g_aGate[nCntGate].bprevious = GATESTATE_DOWN;		

						//現在の座標を保存
						g_aGate[nCntGate].memheight = g_aGate[nCntGate].pos.y;		

						g_aGate[nCntGate].bGoup = false;					//起動初期化
					}
				}
				else if (g_aGate[nCntGate].Gstate == GATESTATE_STAY)
				{//待機
					if (g_aGate[nCntGate].bprevious == GATESTATE_DOWN)
					{//前回下降していたら、上昇
						g_aGate[nCntGate].Gstate = GATESTATE_UP;
					}
					else if (g_aGate[nCntGate].bprevious == GATESTATE_UP)
					{//前回上昇していたら、下降
						g_aGate[nCntGate].Gstate = GATESTATE_DOWN;
					}
				}
				

			}
			

		
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawGate(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL* pMat;				//マテリアルデータへのポインタ

	for (int nCnt = 0; nCnt < MAX_GATE; nCnt++)
	{
		if (g_aGate[nCnt].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aGate[nCnt].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aGate[nCnt].rot.y, g_aGate[nCnt].rot.x, g_aGate[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aGate[nCnt].mtxWorld, &g_aGate[nCnt].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aGate[nCnt].pos.x, g_aGate[nCnt].pos.y, g_aGate[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aGate[nCnt].mtxWorld, &g_aGate[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aGate[nCnt].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aGateModelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aGateModelData.dwNumMat; nCntMat++)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_aGateModelData.apTexture[nCntMat]);

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//モデル(パーツ)の描画
				g_aGateModelData.pMesh->DrawSubset(nCntMat);
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}
//==================================================
//
//	ゲート設置処理
// 
//==================================================
void SetGate(int nIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float movewidth, bool Goup, GateState state )
{
	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{
		if (g_aGate[nCntGate].bUse == false)
		{
			g_aGate[nCntGate].nIdx = nIdx;
			g_aGate[nCntGate].pos = pos;
			g_aGate[nCntGate].poslocal = pos;
			g_aGate[nCntGate].rot = rot;
			g_aGate[nCntGate].memheight = pos.y;
			g_aGate[nCntGate].movewidth = movewidth;
			g_aGate[nCntGate].Gstate = state;
			g_aGate[nCntGate].bGoup = Goup;
			g_aGate[nCntGate].bUse = true;
			break;
		}
	}
}
//==================================================
//
//	ゲートの当たり判定処理
// 
//==================================================
void CollisionGate(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fRadius)
{

	Player* pPlayer = GetPlayer();
	bool bHitCheck = false;

	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{
		if (g_aGate[nCntGate].bUse == true)
		{
			if ((pPlayer->pos.x <= g_aGate[nCntGate].pos.x + g_aGateModelData.vtxMax.x) &&
				(pPlayer->pos.x >= g_aGate[nCntGate].pos.x + g_aGateModelData.vtxMin.x) &&
				(pPlayer->pos.y <= g_aGate[nCntGate].pos.y + g_aGateModelData.vtxMax.y) &&
				(pPlayer->pos.y >= g_aGate[nCntGate].pos.y + g_aGateModelData.vtxMin.y) &&
				(pPlayer->pos.z <= g_aGate[nCntGate].pos.z + g_aGateModelData.vtxMax.z) &&
				(pPlayer->pos.z >= g_aGate[nCntGate].pos.z + g_aGateModelData.vtxMin.z))
			{
				if (pPlayer->posOld.x >= g_aGate[nCntGate].pos.x + g_aGateModelData.vtxMax.x)
				{//右から
					pPlayer->pos.x = g_aGate[nCntGate].pos.x + g_aGateModelData.vtxMax.x;
					bHitCheck = true;
				}
				else if (pPlayer->posOld.x <= g_aGate[nCntGate].pos.x + g_aGateModelData.vtxMin.x)
				{//左から
					pPlayer->pos.x = g_aGate[nCntGate].pos.x + g_aGateModelData.vtxMin.x;
					bHitCheck = true;
				}

				if (pPlayer->posOld.y >= g_aGate[nCntGate].pos.y + g_aGateModelData.vtxMax.y)
				{//上から
					pPlayer->pos.y = g_aGate[nCntGate].pos.y + g_aGateModelData.vtxMax.y;
					bHitCheck = true;
				}
				else if (pPlayer->posOld.y <= g_aGate[nCntGate].pos.y + g_aGateModelData.vtxMin.y)
				{//下から
					pPlayer->pos.y = g_aGate[nCntGate].pos.y + g_aGateModelData.vtxMin.y;
					bHitCheck = true;
				}

				if (pPlayer->posOld.z >= g_aGate[nCntGate].pos.z + g_aGateModelData.vtxMax.z)
				{//奥から
					pPlayer->pos.z = g_aGate[nCntGate].pos.z + g_aGateModelData.vtxMax.z;
					bHitCheck = true;
				}
				else if (pPlayer->posOld.z <= g_aGate[nCntGate].pos.z + g_aGateModelData.vtxMin.z)
				{//手前から
					pPlayer->pos.z = g_aGate[nCntGate].pos.z + g_aGateModelData.vtxMin.z;
					bHitCheck = true;
				}

				
			}
		}
	}

}
//==================================================
//
// ゲートのスイッチ切り替え処理
// 
//==================================================
void SwitchGate(int nIdx)
{
	g_aGate[nIdx].bGoup ^= true;
}