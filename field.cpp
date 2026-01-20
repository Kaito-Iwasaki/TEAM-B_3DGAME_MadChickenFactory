//================================
// 
//  フィールド処理　[field.cpp]
//  Author shuuhei Ida
//
//================================

#include "input.h"
#include "field.h"

// マクロ定義
#define FIELD_TXT_PASS "data\\TEXTURE\\field000.jpg"
#define MAX_FIELD (255)
#define TEXTURE_SIZE_X (100)
#define TEXTURE_SIZE_Y (100)

// グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureField = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL;						// 頂点バッファへのポインタ
Field g_aField[MAX_FIELD];

//========================
// フィールドの初期化処理
//========================
void InitField(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, FIELD_TXT_PASS, &g_pTextureField);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_FIELD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffField,
		NULL);

	VERTEX_3D* pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCountField = 0; nCountField < MAX_FIELD; nCountField++)
	{

		g_aField[nCountField].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aField[nCountField].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aField[nCountField].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aField[nCountField].bUse = false;

		// 頂点座標の設定(x,y,z,の順番になる、zの値は2Dの場合は必ず0にする)
		pVtx[0].pos = D3DXVECTOR3(g_aField[nCountField].pos.x - g_aField[nCountField].size.x, g_aField[nCountField].pos.y, g_aField[nCountField].pos.z + g_aField[nCountField].size.z);
		pVtx[1].pos = D3DXVECTOR3(g_aField[nCountField].pos.x + g_aField[nCountField].size.x, g_aField[nCountField].pos.y, g_aField[nCountField].pos.z + g_aField[nCountField].size.z);
		pVtx[2].pos = D3DXVECTOR3(g_aField[nCountField].pos.x - g_aField[nCountField].size.x, g_aField[nCountField].pos.y, g_aField[nCountField].pos.z - g_aField[nCountField].size.z);
		pVtx[3].pos = D3DXVECTOR3(g_aField[nCountField].pos.x + g_aField[nCountField].size.x, g_aField[nCountField].pos.y, g_aField[nCountField].pos.z - g_aField[nCountField].size.z);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

	// 頂点バッファをアンロックする
	g_pVtxBuffField->Unlock();

	SetField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	SetField(D3DXVECTOR3(100.0f, 0.0f, 0.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//======================
// フィールドの終了処理
//======================
void UninitField(void)
{
	// テクスチャの破棄
	if (g_pTextureField != NULL)
	{
		g_pTextureField->Release();
		g_pTextureField = NULL;
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffField != NULL)
	{
		g_pVtxBuffField->Release();

		g_pVtxBuffField = NULL;
	}
}

//======================
// フィールドの更新処理
//======================
void UpdateField(void)
{
	VERTEX_3D* pVtx;		// 頂点情報へのポインタ

	float fTexsizeX;
	float fTexsizeY;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCountField = 0; nCountField < MAX_FIELD; nCountField++)
	{
		// 頂点座標の設定(x,y,z,の順番になる、zの値は2Dの場合は必ず0にする)
		pVtx[0].pos = D3DXVECTOR3(g_aField[nCountField].pos.x - g_aField[nCountField].size.x, g_aField[nCountField].pos.y, g_aField[nCountField].pos.z + g_aField[nCountField].size.z);
		pVtx[1].pos = D3DXVECTOR3(g_aField[nCountField].pos.x + g_aField[nCountField].size.x, g_aField[nCountField].pos.y, g_aField[nCountField].pos.z + g_aField[nCountField].size.z);
		pVtx[2].pos = D3DXVECTOR3(g_aField[nCountField].pos.x - g_aField[nCountField].size.x, g_aField[nCountField].pos.y, g_aField[nCountField].pos.z - g_aField[nCountField].size.z);
		pVtx[3].pos = D3DXVECTOR3(g_aField[nCountField].pos.x + g_aField[nCountField].size.x, g_aField[nCountField].pos.y, g_aField[nCountField].pos.z - g_aField[nCountField].size.z);

		fTexsizeX = g_aField[nCountField].size.x / TEXTURE_SIZE_X;
		fTexsizeY = g_aField[nCountField].size.z / TEXTURE_SIZE_Y;

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(fTexsizeX * 1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, fTexsizeY * 1.0f);
		pVtx[3].tex = D3DXVECTOR2(fTexsizeX * 1.0f, fTexsizeY * 1.0f);
	
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffField->Unlock();

}

//======================
// フィールドの描画処理
//======================
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();				// デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCountField = 0; nCountField < MAX_FIELD; nCountField++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aField[nCountField].mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aField[nCountField].rot.y, g_aField[nCountField].rot.x, g_aField[nCountField].rot.z);

		D3DXMatrixMultiply(&g_aField[nCountField].mtxWorld, &g_aField[nCountField].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aField[nCountField].pos.x, g_aField[nCountField].pos.y, g_aField[nCountField].pos.z);

		D3DXMatrixMultiply(&g_aField[nCountField].mtxWorld, &g_aField[nCountField].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aField[nCountField].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureField);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCountField * 4,
			2);
	}

}

//===================
// フィールドの設置
//===================
void SetField(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot)
{
	for (int nCountField = 0; nCountField < MAX_FIELD; nCountField++)
	{
		if (g_aField[nCountField].bUse == false)
		{
			g_aField[nCountField].pos = pos;
			g_aField[nCountField].size = size;
			g_aField[nCountField].rot = rot;
			g_aField[nCountField].bUse = true;
			break;
		}
	}
}
