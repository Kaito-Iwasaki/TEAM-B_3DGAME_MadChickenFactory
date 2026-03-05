//=====================================================================
//
// Checkpoint [Checkpoint.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "Checkpoint.h"
#include "player.h"
#include "util.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************


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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCheckpoint;

CHECKPOINT g_aCheckpoints[MAX_CHECKPOINT] = {};
int g_nCurrentCheckpoint = 0;	// 現在のチェックポイント

//=====================================================================
// 初期化処理
//=====================================================================
void InitCheckpoint(void)
{
	ZeroMemory(&g_aCheckpoints, sizeof g_aCheckpoints);
	g_nCurrentCheckpoint = 0;

	GetDevice()->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_CHECKPOINT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCheckpoint,
		NULL
	);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitCheckpoint(void)
{
	RELEASE(g_pVtxBuffCheckpoint);
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateCheckpoint(void)
{
	CHECKPOINT* pCheckpoint = &g_aCheckpoints[MAX_CHECKPOINT];
	D3DXVECTOR3 vecCurrentCPPos = GetCurrentCheckpointPos();

	for (int nCheckpoint = 0; nCheckpoint < MAX_CHECKPOINT; nCheckpoint++, pCheckpoint++)
	{
		if (pCheckpoint->bUsed == false) continue;

		Player* pPlayer = GetPlayer();

		for (int nPlayer = 0; nPlayer < MAX_PLAYER; nPlayer++)
		{
			if (pPlayer->posOld.x <= vecCurrentCPPos.x && pPlayer->pos.x > vecCurrentCPPos.x)
			{
				g_nCurrentCheckpoint = nCheckpoint;
				break;
			}
		}
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawCheckpoint(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxWorld;
	CHECKPOINT* pCheckpoint = &g_aCheckpoints[MAX_CHECKPOINT];

	pDevice->SetStreamSource(0, g_pVtxBuffCheckpoint, 0, sizeof(VERTEX_3D));
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCheckpoint = 0; nCheckpoint < MAX_CHECKPOINT; nCheckpoint++, pCheckpoint++)
	{
		MatrixRotationPosition(&mtxWorld, D3DXVECTOR3_ZERO, pCheckpoint->pos);
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		pDevice->SetTexture(0, NULL);
		pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, nCheckpoint * 4, 2);
	}
}

// チェックポイント設定処理
// vec.xを超えるとその地点を現在のチェックポイントとし
// MoveToCurrentCheckpointPosが呼ばれた時にvecに設定します。
void SetCheckpoint(D3DXVECTOR3* pPos)
{
	CHECKPOINT* pCheckpoint = &g_aCheckpoints[MAX_CHECKPOINT];
	VERTEX_3D* pVtx;

	g_pVtxBuffCheckpoint->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCheckpoint = 0; nCheckpoint < MAX_CHECKPOINT; nCheckpoint++, pCheckpoint++)
	{
		if (pCheckpoint->bUsed == true)
		{
			pVtx += 4;
			continue;
		}
		
		pCheckpoint->bUsed = true;
		pCheckpoint->pos = *pPos;

		pVtx[0].pos = D3DXVECTOR3(0, +100000, +100000);
		pVtx[1].pos = D3DXVECTOR3(0, +100000, -100000);
		pVtx[2].pos = D3DXVECTOR3(0, -100000, +100000);
		pVtx[3].pos = D3DXVECTOR3(0, -100000, -100000);

		pVtx[0].nor = D3DXVECTOR3(-1, 0, 0);
		pVtx[1].nor = D3DXVECTOR3(-1, 0, 0);
		pVtx[2].nor = D3DXVECTOR3(-1, 0, 0);
		pVtx[3].nor = D3DXVECTOR3(-1, 0, 0);

		pVtx[0].col = D3DXCOLOR(1, 0, 1, 0.25f);
		pVtx[1].col = D3DXCOLOR(1, 0, 1, 0.25f);
		pVtx[2].col = D3DXCOLOR(1, 0, 1, 0.25f);
		pVtx[3].col = D3DXCOLOR(1, 0, 1, 0.25f);

		break;
	}

	g_pVtxBuffCheckpoint->Unlock();
}

// チェックポイント移動処理
// チェックポイント[nCheckpoint]の位置へ移動します。
void MoveToCheckpointPos(D3DXVECTOR3* pOut, int nCheckpoint)
{
	*pOut = g_aCheckpoints[g_nCurrentCheckpoint].pos;
}


// チェックポイント位置取得処理
// 現在のチェックポイントの位置を取得します。
D3DXVECTOR3 GetCurrentCheckpointPos(void)
{
	return g_aCheckpoints[g_nCurrentCheckpoint].pos;
}