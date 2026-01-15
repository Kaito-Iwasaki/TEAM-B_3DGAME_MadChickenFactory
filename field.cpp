//================================
// 
//  フィールド処理　[field.cpp]
//  Author shuuhei Ida
//
//================================

#include "input.h"
#include "field.h"

LPDIRECT3DTEXTURE9 g_pTextureField = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL;			// 頂点バッファへのポインタ
D3DXVECTOR3 g_posField;
D3DXVECTOR3 g_rotField;
D3DXMATRIX g_mtxWorldField;								//	ワールドマトリックス

//========================
// フィールドの初期化処理
//========================
void InitField(void)
{
	LPDIRECT3DDEVICE9 pDevice ;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\field000.jpg", &g_pTextureField);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffField,
		NULL);

	VERTEX_3D* pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	g_posField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 頂点座標の設定(x,y,z,の順番になる、zの値は2Dの場合は必ず0にする)
	pVtx[0].pos = D3DXVECTOR3(-100.0f, 0.0f, 100.0f);
	pVtx[1].pos = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
	pVtx[2].pos = D3DXVECTOR3(-100.0f, 0.0f, -100.0f);
	pVtx[3].pos = D3DXVECTOR3(100.0f, 0.0f, -100.0f);

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	// 頂点カラーの設定

	pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffField->Unlock();
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

}

//======================
// フィールドの描画処理
//======================
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();				// デバイスへのポインタ
	D3DXMATRIX mtxRot,mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldField);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotField.y, g_rotField.x, g_rotField.z);

	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posField.x, g_posField.y, g_posField.z);
	
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldField);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureField);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		2);
}
