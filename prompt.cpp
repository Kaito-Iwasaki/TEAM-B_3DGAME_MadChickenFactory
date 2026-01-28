//===================================
// 
//  ビルボード処理　[billboard.cpp]
//  Author shuuhei Ida
//
//===================================

#include "input.h"
#include "billboard.h"

LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;			// 頂点バッファへのポインタ
D3DXVECTOR3 g_posBillboard;
D3DXMATRIX g_mtxWorldBillboard;								//	ワールドマトリックス

//=========================
// ビルボードの初期化処理
//=========================
void InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice ;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ski001.jpg", &g_pTextureBillboard);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	VERTEX_3D* pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	g_posBillboard = D3DXVECTOR3(0.0f, 0.0f, -100.0f);

	// 頂点座標の設定(x,y,z,の順番になる、zの値は2Dの場合は必ず0にする)
	pVtx[0].pos = D3DXVECTOR3(g_posBillboard.x - 100.0f, g_posBillboard.y + 100.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posBillboard.x + 100.0f, g_posBillboard.y + 100.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posBillboard.x - 100.0f, g_posBillboard.y - 100.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posBillboard.x + 100.0f, g_posBillboard.y - 100.0f, 0.0f);

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
	g_pVtxBuffBillboard->Unlock();
}

//======================
// ビルボードの終了処理
//======================
void UninitBillboard(void)
{
	// テクスチャの破棄
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();

		g_pVtxBuffBillboard = NULL;
	}
}

//======================
// ビルボードの更新処理
//======================
void UpdateBillboard(void)
{

}

//======================
// ビルボードの描画処理
//======================
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();				// デバイスへのポインタ
	D3DXMATRIX mtxTrans;
	D3DXMATRIX mtxView;										// ビューマトリックス取得用


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldBillboard);

	// ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	
	// ポリゴンをカメラに対して正面を向ける
	D3DXMatrixInverse(&g_mtxWorldBillboard, NULL, &mtxView); // 逆行列を求める
	g_mtxWorldBillboard._41 = 0.0f;
	g_mtxWorldBillboard._42 = 0.0f;
	g_mtxWorldBillboard._43 = 0.0f;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posBillboard.x, g_posBillboard.y, g_posBillboard.z);
	
	D3DXMatrixMultiply(&g_mtxWorldBillboard, &g_mtxWorldBillboard, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBillboard);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//// アルファテストを有効にする
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBillboard);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		2);

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//// アルファテストを有効にする
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DPCMPCAPS_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

}
