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
#include "collision.h"

//==================================================
//
//	マクロ定義
//
//==================================================
#define MOVEBOX_MODEL_PATH	"data\\MODEL\\movebox.x"	//movebox000.xへのパス
#define ENABLE_MOVE		(5.0f)					//箱を押せる強さ
#define MOVE_SPEED		(3.5f)					//箱を押す際の移動量

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
//	可動箱の初期化
//
//==================================================
void InitMoveBox(void)
{
	for (int nCntMoveBox = 0; nCntMoveBox < MAX_MOVEBOX; nCntMoveBox++)
	{
		g_aMoveBox[nCntMoveBox].bUse = false;
		g_aMoveBox[nCntMoveBox].pos = D3DXVECTOR3_ZERO;
		g_aMoveBox[nCntMoveBox].rot = D3DXVECTOR3_ZERO;
		g_aMoveBox[nCntMoveBox].state = STATE_NORMAL;

	}

	LoadModel(MOVEBOX_MODEL_PATH, &g_aMoveBoxModelData);

	SetMoveBox(D3DXVECTOR3(4900.0f, 0.0f, 800.0f), D3DXVECTOR3_ZERO, D3DXVECTOR3(-600.0f, 0.0f, 0.0f));
}

//==================================================
//
//	可動箱の終了処理
//
//==================================================
void UninitMoveBox(void)
{
	ReleaseMesh(&g_aMoveBoxModelData);
}

//==================================================
//
//	可動箱の更新処理
//
//==================================================
void UpdateMoveBox(void)
{
	Player* pPlayer = GetPlayer();

	CollisionMoveBox();

	PrintDebugProc("%f\n", pPlayer->move.x);

	for (int nCntMBox = 0; nCntMBox < MAX_MOVEBOX; nCntMBox++)
	{
		if (g_aMoveBox[nCntMBox].bUse == true)
		{
			if (g_aMoveBox[nCntMBox].state == STATE_RIGHT)
			{//右から
				if (pPlayer->move.x <= -ENABLE_MOVE)
				{//ENABLE_MOVE以上の力で押されている
					if (g_aMoveBox[nCntMBox].pos.x > g_aMoveBox[nCntMBox].SavePos.x || 
						g_aMoveBox[nCntMBox].pos.x > g_aMoveBox[nCntMBox].MotionRange.x)
					{//有効範囲内
						g_aMoveBox[nCntMBox].pos.x -= MOVE_SPEED;
					}
				}
				
			}
			else if (g_aMoveBox[nCntMBox].state == STATE_LEFT)
			{//左から
				if (pPlayer->move.x >= ENABLE_MOVE)
				{
					if (g_aMoveBox[nCntMBox].pos.x < g_aMoveBox[nCntMBox].SavePos.x ||
						g_aMoveBox[nCntMBox].pos.x < g_aMoveBox[nCntMBox].MotionRange.x)
					{
						g_aMoveBox[nCntMBox].pos.x += MOVE_SPEED;
					}
				}
			}
			else if (g_aMoveBox[nCntMBox].state == STATE_REAR)
			{//奥から
				if (pPlayer->move.z <= -ENABLE_MOVE)
				{
					if (g_aMoveBox[nCntMBox].pos.z > g_aMoveBox[nCntMBox].SavePos.z ||
						g_aMoveBox[nCntMBox].pos.z > g_aMoveBox[nCntMBox].MotionRange.z)
					{
						g_aMoveBox[nCntMBox].pos.z -= MOVE_SPEED;
					}
				}
			}
			else if (g_aMoveBox[nCntMBox].state == STATE_FRONT)
			{//手前から
				if (pPlayer->move.z >= ENABLE_MOVE)
				{
					if (g_aMoveBox[nCntMBox].pos.z < g_aMoveBox[nCntMBox].SavePos.z ||
						g_aMoveBox[nCntMBox].pos.z < g_aMoveBox[nCntMBox].MotionRange.z)
					{
						g_aMoveBox[nCntMBox].pos.z += MOVE_SPEED;
					}
				}
			}

			for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
			{
				// オブジェクトの当たり判定処理
				pPlayer[nCntPlayer].fStandPos = CollisionPointBoxObject(pPlayer[nCntPlayer].pos, 
												g_aMoveBox[nCntMBox].pos, 
												g_aMoveBoxModelData.vtxMin, 
												g_aMoveBoxModelData.vtxMax, 
												pPlayer[nCntPlayer].fStandPos);
			}
		}

	}
}

//==================================================
//
//	可動箱の描画処理
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
//	可動箱設置処理
//
//==================================================
void SetMoveBox(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 range)
{
	for (int nCnt = 0; nCnt < MAX_MOVEBOX; nCnt++)
	{
		if (g_aMoveBox[nCnt].bUse == false)
		{
			g_aMoveBox[nCnt].bUse = true;
			g_aMoveBox[nCnt].pos = pos;
			g_aMoveBox[nCnt].SavePos = pos;
			g_aMoveBox[nCnt].MotionRange = pos + range;
			g_aMoveBox[nCnt].rot = rot;
			break;
		}
	}
}
//==================================================
//
//	可動箱の当たり判定
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
				(pPlayer->pos.y >= g_aMoveBox[nCntMoveBox].pos.y + g_aMoveBoxModelData.vtxMin.y - 10.0f) &&
				(pPlayer->pos.z <= g_aMoveBox[nCntMoveBox].pos.z + g_aMoveBoxModelData.vtxMax.z) &&
				(pPlayer->pos.z >= g_aMoveBox[nCntMoveBox].pos.z + g_aMoveBoxModelData.vtxMin.z))
			{
				if (pPlayer->posOld.x >= g_aMoveBox[nCntMoveBox].pos.x + g_aMoveBoxModelData.vtxMax.x)
				{//右から
					pPlayer->pos.x = g_aMoveBox[nCntMoveBox].pos.x + g_aMoveBoxModelData.vtxMax.x;
					g_aMoveBox[nCntMoveBox].state = STATE_RIGHT;
					bHitCheck = true;
				}
				else if (pPlayer->posOld.x <= g_aMoveBox[nCntMoveBox].pos.x + g_aMoveBoxModelData.vtxMin.x)
				{//左から
					pPlayer->pos.x = g_aMoveBox[nCntMoveBox].pos.x + g_aMoveBoxModelData.vtxMin.x;
					g_aMoveBox[nCntMoveBox].state = STATE_LEFT;
					bHitCheck = true;
				}

				if (pPlayer->posOld.y >= g_aMoveBox[nCntMoveBox].pos.y + g_aMoveBoxModelData.vtxMax.y)
				{//上から
					pPlayer->pos.y = g_aMoveBox[nCntMoveBox].pos.y + g_aMoveBoxModelData.vtxMax.y;
					bHitCheck = true;
					pPlayer->move.y = 0;
					pPlayer->bJump = false;
				}
				else if (pPlayer->posOld.y <= g_aMoveBox[nCntMoveBox].pos.y + g_aMoveBoxModelData.vtxMin.y)
				{//下から
					pPlayer->pos.y = g_aMoveBox[nCntMoveBox].pos.y + g_aMoveBoxModelData.vtxMin.y;
					bHitCheck = true;
				}

				if (pPlayer->posOld.z >= g_aMoveBox[nCntMoveBox].pos.z + g_aMoveBoxModelData.vtxMax.z)
				{//奥から
					pPlayer->pos.z = g_aMoveBox[nCntMoveBox].pos.z + g_aMoveBoxModelData.vtxMax.z;
					g_aMoveBox[nCntMoveBox].state = STATE_REAR;
					bHitCheck = true;
				}
				else if (pPlayer->posOld.z <= g_aMoveBox[nCntMoveBox].pos.z + g_aMoveBoxModelData.vtxMin.z)
				{//手前から
					pPlayer->pos.z = g_aMoveBox[nCntMoveBox].pos.z + g_aMoveBoxModelData.vtxMin.z;
					g_aMoveBox[nCntMoveBox].state = STATE_FRONT;
					bHitCheck = true;
				}
			}

			if (bHitCheck == false)
			{//触られていないので、修正
				g_aMoveBox[nCntMoveBox].state = STATE_NORMAL;
			}

		}
	}

	return bHitCheck;
}
