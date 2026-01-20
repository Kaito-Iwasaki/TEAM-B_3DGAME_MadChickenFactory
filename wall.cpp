//================================
// 
//  壁処理　[wall.cpp]
//  Author shuuhei Ida
//
//================================
#include "input.h"
#include "wall.h"

#define MAX_WALL (4)

// 壁の構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorldWall;
	bool bUse;
} Wall;


LPDIRECT3DTEXTURE9 g_apTextureWall[MAX_WALL] = {};				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;			// 頂点バッファへのポインタ
Wall g_aWall[MAX_WALL];

//=========================
// 壁の初期化処理
//=========================
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice ;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\wall000.jpg", &g_apTextureWall[0]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	VERTEX_3D* pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
	
			g_aWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aWall[nCntWall].bUse = false;

		// 頂点座標の設定(x,y,z,の順番になる、zの値は2Dの場合は必ず0にする)
		pVtx[0].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x - 100.0f, g_aWall[nCntWall].pos.y + 150.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x + 100.0f, g_aWall[nCntWall].pos.y + 150.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x - 100.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x + 100.0f, 0.0f, 0.0f);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		// 頂点カラーの設定

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	
	SetWall(D3DXVECTOR3(0.0f, 0.0f, 100.0f),  D3DXVECTOR3(0.0f, (D3DX_PI * 0.0f),0.0f));
	SetWall(D3DXVECTOR3(-100.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, (D3DX_PI * -0.5f), 0.0f));
	SetWall(D3DXVECTOR3(100.0f, 0.0f, 0.0f),  D3DXVECTOR3(0.0f, (D3DX_PI * 0.5f), 0.0f));
	SetWall(D3DXVECTOR3(0.0f, 0.0f, -100.0f), D3DXVECTOR3(0.0f, (D3DX_PI * -1.0f), 0.0f));

	// 頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();
}

//==========================
// 壁の終了処理
//==========================
void UninitWall(void)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		// テクスチャの破棄
		if (g_apTextureWall[nCntWall] != NULL)
		{
			g_apTextureWall[nCntWall]->Release();
			g_apTextureWall[nCntWall] = NULL;
		}
	}
	
	// 頂点バッファの破棄
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();

		g_pVtxBuffWall = NULL;
	}
}

//====================
// 壁の更新処理
//====================
void UpdateWall(void)
{

}

//====================
// 壁の描画処理
//====================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();				// デバイスへのポインタ
	D3DXMATRIX mtxRot,mtxTrans;


	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aWall[nCntWall].mtxWorldWall);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);

			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorldWall, &g_aWall[nCntWall].mtxWorldWall, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);

			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorldWall, &g_aWall[nCntWall].mtxWorldWall, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCntWall].mtxWorldWall);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureWall[0]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntWall * 4,
				2);
		}
	}
	
}

//=================
// 壁の設置処理
//=================
void SetWall(D3DXVECTOR3 pos,D3DXVECTOR3 rot)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == false)
		{
			g_aWall[nCntWall].pos = pos;
			g_aWall[nCntWall].rot = rot;
			g_aWall[nCntWall].bUse = true;
			break;
		}

	}
}
