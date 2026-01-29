//==================================================
//
//	可動箱[box_movable.cpp]
//
//==================================================
//==================================================
//
//	ファイルインクルード
//
//==================================================
#include "box_movable.h"
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
#define MAX_MOVEBOX		(128)					//モデルの最大数
#define MOVEBOX_MODEL_PATH	"data\\MODEL\\movebox000.x"	//movebox000.xへのパス
#define MAX_MOVEBOX_SPEED	(0.15f)				//ノコギリの回転速度のMAX

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
MESHDATA g_aMoveBoxModelData;
MoveBox g_aMoveBox[MAX_MOVEBOX];

//==================================================
//
//	回転ノコギリの初期化
//
//==================================================
void InitMoveBox(void)
{
	for (int nCntMoveBox = 0; nCntMoveBox < MAX_MOVEBOX; nCntMoveBox++)
	{
		g_aMoveBox[nCntMoveBox].bStartup = false;
		g_aMoveBox[nCntMoveBox].bUse = false;
		g_aMoveBox[nCntMoveBox].pos = D3DXVECTOR3_ZERO;
		g_aMoveBox[nCntMoveBox].rot = D3DXVECTOR3_ZERO;
		g_aMoveBox[nCntMoveBox].turnSpeed = 0;
	}

	LoadModel(MOVEBOX_MODEL_PATH, &g_aMoveBoxModelData);

	SetMoveBox(D3DXVECTOR3(200.0f, 30.0f, 500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), true);
}

//==================================================
//
//	回転ノコギリの終了処理
//
//==================================================
void UninitMoveBox(void)
{
	ReleaseMesh(&g_aMoveBoxModelData);
}

//==================================================
//
//	回転ノコギリの更新処理
//
//==================================================
void UpdateMoveBox(void)
{
	if (GetKeyboardTrigger(DIK_F3))
	{
		SwitchMoveBox(0);
	}

	for (int nCntMoveBox = 0; nCntMoveBox < MAX_MOVEBOX; nCntMoveBox++)
	{
		if (g_aMoveBox[nCntMoveBox].bUse == true)
		{
			if (g_aMoveBox[nCntMoveBox].bStartup == true)
			{//起動スイッチがON
				//MAX_MOVEBOX_SPEEDまで速度をあげながら回転
				g_aMoveBox[nCntMoveBox].turnSpeed += (MAX_MOVEBOX_SPEED - g_aMoveBox[nCntMoveBox].turnSpeed) * 0.005f;
				g_aMoveBox[nCntMoveBox].rot.z += g_aMoveBox[nCntMoveBox].turnSpeed;
				
			}
			else
			{//OFF
				//0.0まで速度を徐々に落とす
				g_aMoveBox[nCntMoveBox].turnSpeed += (0.0f - g_aMoveBox[nCntMoveBox].turnSpeed) * 0.02f;
				g_aMoveBox[nCntMoveBox].rot.z += g_aMoveBox[nCntMoveBox].turnSpeed;

			}
		}
	}

	CollisionMoveBox();
}

//==================================================
//
//	回転ノコギリの描画処理
//
//==================================================
void DrawMoveBox(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL* pMat;				//マテリアルデータへのポインタ

	for (int nCnt = 0; nCnt < MAX_MOVEBOX; nCnt++)
	{
		if (g_aMoveBox[nCnt].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMoveBox[nCnt].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMoveBox[nCnt].rot.y, g_aMoveBox[nCnt].rot.x, g_aMoveBox[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aMoveBox[nCnt].mtxWorld, &g_aMoveBox[nCnt].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aMoveBox[nCnt].pos.x, g_aMoveBox[nCnt].pos.y, g_aMoveBox[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aMoveBox[nCnt].mtxWorld, &g_aMoveBox[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMoveBox[nCnt].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aMoveBoxModelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aMoveBoxModelData.dwNumMat; nCntMat++)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_aMoveBoxModelData.apTexture[nCntMat]);

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//モデル(パーツ)の描画
				g_aMoveBoxModelData.pMesh->DrawSubset(nCntMat);
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
void SetMoveBox(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool startup)
{
	for (int nCnt = 0; nCnt < MAX_MOVEBOX; nCnt++)
	{
		if (g_aMoveBox[nCnt].bUse == false)
		{
			g_aMoveBox[nCnt].bStartup = startup;
			g_aMoveBox[nCnt].bUse = true;
			g_aMoveBox[nCnt].pos = pos;
			g_aMoveBox[nCnt].rot = rot;
			break;
		}
	}
}
//==================================================
//
//	回転ノコギリの当たり判定
//
//==================================================
bool CollisionMoveBox(void)
{
	Player* pPlayer = GetPlayer();
	bool bHitCheck = false;

	for (int nCntMoveBox = 0; nCntMoveBox < MAX_MOVEBOX; nCntMoveBox++)
	{
		if (g_aMoveBox[nCntMoveBox].bUse == true)
		{
			if ((pPlayer->pos.x <= g_aMoveBox[nCntMoveBox].pos.x + g_aMoveBoxModelData.vtxMax.x) &&
				(pPlayer->pos.x >= g_aMoveBox[nCntMoveBox].pos.x + g_aMoveBoxModelData.vtxMin.x) &&
				(pPlayer->pos.y <= g_aMoveBox[nCntMoveBox].pos.y + g_aMoveBoxModelData.vtxMax.y) &&
				(pPlayer->pos.y >= g_aMoveBox[nCntMoveBox].pos.y + g_aMoveBoxModelData.vtxMin.y) &&
				(pPlayer->pos.z <= g_aMoveBox[nCntMoveBox].pos.z + g_aMoveBoxModelData.vtxMax.z) &&
				(pPlayer->pos.z >= g_aMoveBox[nCntMoveBox].pos.z + g_aMoveBoxModelData.vtxMin.z))
			{
				if (pPlayer->posOld.x >= g_aMoveBox[nCntMoveBox].pos.x + g_aMoveBoxModelData.vtxMax.x)
				{//右から
					pPlayer->pos.x = g_aMoveBox[nCntMoveBox].pos.x + g_aMoveBoxModelData.vtxMax.x;
					bHitCheck = true;
				}
				else if (pPlayer->posOld.x <= g_aMoveBox[nCntMoveBox].pos.x + g_aMoveBoxModelData.vtxMin.x)
				{//左から
					pPlayer->pos.x = g_aMoveBox[nCntMoveBox].pos.x + g_aMoveBoxModelData.vtxMin.x;
					bHitCheck = true;
				}

				if (pPlayer->posOld.y >= g_aMoveBox[nCntMoveBox].pos.y + g_aMoveBoxModelData.vtxMax.y)
				{//上から
					pPlayer->pos.y = g_aMoveBox[nCntMoveBox].pos.y + g_aMoveBoxModelData.vtxMax.y;
					bHitCheck = true;
				}
				else if (pPlayer->posOld.y <= g_aMoveBox[nCntMoveBox].pos.y + g_aMoveBoxModelData.vtxMin.y)
				{//下から
					pPlayer->pos.y = g_aMoveBox[nCntMoveBox].pos.y + g_aMoveBoxModelData.vtxMin.y;
					bHitCheck = true;
				}

				if (pPlayer->posOld.z >= g_aMoveBox[nCntMoveBox].pos.z + g_aMoveBoxModelData.vtxMax.z)
				{//奥から
					pPlayer->pos.z = g_aMoveBox[nCntMoveBox].pos.z + g_aMoveBoxModelData.vtxMax.z;
					bHitCheck = true;
				}
				else if (pPlayer->posOld.z <= g_aMoveBox[nCntMoveBox].pos.z + g_aMoveBoxModelData.vtxMin.z)
				{//手前から
					pPlayer->pos.z = g_aMoveBox[nCntMoveBox].pos.z + g_aMoveBoxModelData.vtxMin.z;
					bHitCheck = true;
				}
			}
		}
	}

	if (bHitCheck == true)
	{
		SetFade(MODE_GAME);
	}

	return bHitCheck;
}
//==================================================
//
//	回転ノコギリのスイッチを切り替え
//
//==================================================
void SwitchMoveBox(int nIdx)
{
	g_aMoveBox[nIdx].bStartup ^= true;
}
