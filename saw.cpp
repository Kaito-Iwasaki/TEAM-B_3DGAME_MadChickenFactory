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
#define SAW_MODEL_PATH	"data\\MODEL\\saw000.x"	//saw000.xへのパス
#define MAX_SAW_SPEED		(0.15f)					//ノコギリの回転速度のMAX
#define SAW_FACE			(4)						//ノコギリの判定する面の数
#define MARGIN_RANGE_SAW	(0.5f)					//当たり判定有効範囲のゆとり

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
	for (int nCntSaw = 0; nCntSaw < MAX_SAW; nCntSaw++)
	{
		if (g_aSaw[nCntSaw].bUse == true)
		{
			if (GetPromptTrigger(g_aSaw[nCntSaw].nIdx))
			{
				SwitchSaw(nCntSaw);
			}

			if (g_aSaw[nCntSaw].bStartup == true)
			{//起動スイッチがON
				//MAX_SAW_SPEEDまで速度をあげながら回転
				g_aSaw[nCntSaw].turnSpeed += (MAX_SAW_SPEED - g_aSaw[nCntSaw].turnSpeed) * 0.005f;
				g_aSaw[nCntSaw].rot.z += g_aSaw[nCntSaw].turnSpeed;

				if (g_aSaw[nCntSaw].nCounterState > g_aSaw[nCntSaw].nMoveTime)
				{
					g_aSaw[nCntSaw].move *= -1;
					g_aSaw[nCntSaw].nCounterState = 0;
				}
				
				g_aSaw[nCntSaw].pos += g_aSaw[nCntSaw].move;

				g_aSaw[nCntSaw].nCounterState++;
			}
			else
			{//OFF
				//0.0まで速度を徐々に落とす
				g_aSaw[nCntSaw].turnSpeed += (0.0f - g_aSaw[nCntSaw].turnSpeed) * 0.02f;
				g_aSaw[nCntSaw].rot.z += g_aSaw[nCntSaw].turnSpeed;

			}

			//X軸を回していたら矩形
			if (g_aSaw[nCntSaw].rot.x > 0.0f)
			{
				CollisionSawRotX();
			}
			else
			{
				CollisionSawRotY();
			}
		}
	}
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
//	moveRange = posを中心に+-で動作
// 
//==================================================
void SetSaw(int nIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 moveRange, int nMoveTime, bool startup)
{
	for (int nCnt = 0; nCnt < MAX_SAW; nCnt++)
	{
		if (g_aSaw[nCnt].bUse == false)
		{
			g_aSaw[nCnt].nIdx = nIdx;
			g_aSaw[nCnt].pos = pos;
			g_aSaw[nCnt].poslocal = pos;
			g_aSaw[nCnt].rot = rot;
			g_aSaw[nCnt].moveRange = moveRange;
			g_aSaw[nCnt].nMoveTime = nMoveTime;
			g_aSaw[nCnt].nCounterState = 0;
			g_aSaw[nCnt].move = ((pos + moveRange) - pos) / (float)nMoveTime;
			g_aSaw[nCnt].bStartup = startup;
			g_aSaw[nCnt].bUse = true;
			break;
		}
	}
}
//==================================================
//
//	回転ノコギリの当たり判定
//
//==================================================
#if 1
bool CollisionSawRotX(void)
{
	Player* pPlayer = GetPlayer();
	bool bHitCheck = false;

	for (int nCntSaw = 0; nCntSaw < MAX_SAW; nCntSaw++)
	{
		if (g_aSaw[nCntSaw].bUse == true)
		{
			if ((pPlayer->pos.x <= g_aSaw[nCntSaw].pos.x + g_aSawModelData.vtxMax.x) &&
				(pPlayer->pos.x >= g_aSaw[nCntSaw].pos.x + g_aSawModelData.vtxMin.x) &&
				(pPlayer->pos.y <= g_aSaw[nCntSaw].pos.y + g_aSawModelData.vtxMax.y) &&
				(pPlayer->pos.y >= g_aSaw[nCntSaw].pos.y + g_aSawModelData.vtxMin.y) &&
				(pPlayer->pos.z <= g_aSaw[nCntSaw].pos.z + g_aSawModelData.vtxMax.x) &&
				(pPlayer->pos.z >= g_aSaw[nCntSaw].pos.z + g_aSawModelData.vtxMin.x))
			{
				if (pPlayer->posOld.x >= g_aSaw[nCntSaw].pos.x + g_aSawModelData.vtxMax.x)
				{//右から
					/*pPlayer->pos.x = g_aSaw[nCntSaw].pos.x + g_aSawModelData.vtxMax.x;*/
					bHitCheck = true;
				}
				else if (pPlayer->posOld.x <= g_aSaw[nCntSaw].pos.x + g_aSawModelData.vtxMin.x)
				{//左から
					/*pPlayer->pos.x = g_aSaw[nCntSaw].pos.x + g_aSawModelData.vtxMin.x;*/
					bHitCheck = true;
				}

				if (pPlayer->posOld.y >= g_aSaw[nCntSaw].pos.y + g_aSawModelData.vtxMax.y)
				{//上から
					/*pPlayer->pos.y = g_aSaw[nCntSaw].pos.y + g_aSawModelData.vtxMax.y;*/
					bHitCheck = true;
				}
				else if (pPlayer->posOld.y <= g_aSaw[nCntSaw].pos.y + g_aSawModelData.vtxMin.y)
				{//下から
					/*pPlayer->pos.y = g_aSaw[nCntSaw].pos.y + g_aSawModelData.vtxMin.y;*/
					bHitCheck = true;
				}

				if (pPlayer->posOld.z >= g_aSaw[nCntSaw].pos.z + g_aSawModelData.vtxMax.z)
				{//奥から
					/*pPlayer->pos.z = g_aSaw[nCntSaw].pos.z + g_aSawModelData.vtxMax.z;*/
					bHitCheck = true;
				}
				else if (pPlayer->posOld.z <= g_aSaw[nCntSaw].pos.z + g_aSawModelData.vtxMin.z)
				{//手前から
					/*pPlayer->pos.z = g_aSaw[nCntSaw].pos.z + g_aSawModelData.vtxMin.z;*/
					bHitCheck = true;
				}

				if (bHitCheck == true && g_aSaw[nCntSaw].bStartup == false)
				{//当たっても、スイッチが止まっていいれば死なない
					bHitCheck = false;
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
#endif
#if 1
bool CollisionSawRotY(void)
{
	Player* pPlayer = GetPlayer();
	bool bHitCheck = false;

	bool check = false, checkold = false;
	bool bChek = false, bcheckold = false;


	D3DXVECTOR3 VecLine, VecToPos, VecMove, VecToPosOld;
	D3DXVECTOR3 v[SAW_FACE];		//壁の端to端ベクトル
	D3DXVECTOR3 WallMove, Reflection;	//壁ずりベクトル,反射ベクトル

	float fNormal;	//正規化法線ベクトル
	float fRate, fAll, fIntersect;

	VecMove = pPlayer->pos - pPlayer->posOld;

	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		if (g_aSaw[nCnt].bUse == true)
		{
			//Y軸回転時
			v[0].x = g_aSaw[nCnt].pos.x 
				+ (g_aSawModelData.vtxMax.z * sinf(g_aSaw[nCnt].rot.y))		//厚み
				+ (g_aSawModelData.vtxMin.x * cosf(g_aSaw[nCnt].rot.y));	//幅
			v[0].y = 0.0f;
			v[0].z = g_aSaw[nCnt].pos.z 
				+ (g_aSawModelData.vtxMax.z * cosf(g_aSaw[nCnt].rot.y))		//厚み
				+ (g_aSawModelData.vtxMax.x * sinf(g_aSaw[nCnt].rot.y));	//幅

			v[1].x = g_aSaw[nCnt].pos.x 
				+ (g_aSawModelData.vtxMax.z * sinf(g_aSaw[nCnt].rot.y))		//厚み
				+ (g_aSawModelData.vtxMax.x * cosf(g_aSaw[nCnt].rot.y));	//幅
			v[1].y = 0.0f;
			v[1].z = g_aSaw[nCnt].pos.z 
				+ (g_aSawModelData.vtxMax.z * cosf(g_aSaw[nCnt].rot.y))		//厚み
				+ (g_aSawModelData.vtxMax.x * -sinf(g_aSaw[nCnt].rot.y));	//幅

			v[2].x = g_aSaw[nCnt].pos.x
				+ (g_aSawModelData.vtxMin.z * sinf(g_aSaw[nCnt].rot.y))	//厚み
				+ (g_aSawModelData.vtxMax.x * cosf(g_aSaw[nCnt].rot.y));	//幅
			v[2].y = 0.0f;
			v[2].z = g_aSaw[nCnt].pos.z 
				+ (g_aSawModelData.vtxMin.z * cosf(g_aSaw[nCnt].rot.y))	//厚み
				+ (g_aSawModelData.vtxMax.x * -sinf(g_aSaw[nCnt].rot.y));	//幅

			v[3].x = g_aSaw[nCnt].pos.x 
				+ (g_aSawModelData.vtxMin.z * sinf(g_aSaw[nCnt].rot.y))	//厚み
				+ (g_aSawModelData.vtxMin.x * cosf(g_aSaw[nCnt].rot.y));	//幅
			v[3].y = 0.0f;
			v[3].z = g_aSaw[nCnt].pos.z 
				+ (g_aSawModelData.vtxMin.z * cosf(g_aSaw[nCnt].rot.y))	//厚み
				+ (g_aSawModelData.vtxMax.x * sinf(g_aSaw[nCnt].rot.y));	//幅
			//0┌--┐1
			//	 ・
			//3└--┘2
			//x -150
			//z 250
			for (int nCntColl = 0; nCntColl < SAW_FACE; nCntColl++)
			{//各面の外積判定を行う
				if (nCntColl == 3)
				{
					VecLine = v[0] - v[nCntColl];
					VecToPos = pPlayer->pos - v[nCntColl];
					VecToPosOld = pPlayer->posOld - v[nCntColl];
				}
				else
				{
					VecLine = v[nCntColl + 1] - v[nCntColl];
					VecToPos = pPlayer->pos - v[nCntColl];
					VecToPosOld = pPlayer->posOld - v[nCntColl];
				}

				if ((VecLine.z * VecToPos.x) - (VecLine.x * VecToPos.z) > MARGIN_RANGE_SAW)
				{//posが右にいる
					check = true;
				}
				else
				{//posが左にいる
					check = false;
					break;
				}

				if ((VecLine.z * VecToPosOld.x) - (VecLine.x * VecToPosOld.z) < -MARGIN_RANGE_SAW)
				{//posoldが左にいる
					checkold = true;
				}
				else
				{//posoldが右にいる
					checkold = false;
				}

				if (check == true)
				{
					PrintDebugProc("%d : HIT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n",nCntColl);
				}
			}

			if (check == true)
			{
				SetFade(MODE_GAME);
			}
		}

	}

	return bChek;
}
#endif
//==================================================
//
//	回転ノコギリのスイッチを切り替え
//
//==================================================
void SwitchSaw(int nIdx)
{
	g_aSaw[nIdx].bStartup ^= true;
}
