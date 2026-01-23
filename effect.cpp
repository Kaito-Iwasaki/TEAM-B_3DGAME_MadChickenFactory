//=====================================================================
// 
// effect.cppのヘッダファイル [effect.h]
// Author : 土田　青空
//
//=====================================================================
//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include"main.h"
#include"effect.h"
#include"input.h"
//==========
//マクロ定義 
//==========
#define MAX_EFFECT (60)			//エフェクトの最大数
//==============
//グローバル変数
//==============
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	//頂点バッファのポインタ
Effect g_aEffect[MAX_EFFECT];
//==========
//初期化処理
//==========
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\explosion000.png", &g_pTextureEffect);
	for (int nCntEffect = 0;  nCntEffect < MAX_EFFECT;  nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//エフェクトの位置
		g_aEffect[nCntEffect].type = EFFECTTYPE_BULLET;							//エフェクトの種類
		g_aEffect[nCntEffect].bUse = false;										//使用しているかどうか
		g_aEffect[nCntEffect].nLife = 100;										//エフェクトの描画時間

	}
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4*MAX_EFFECT , D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL);		//sizeof(VERTEX)の後に*と頂点数を書く
	VERTEX_3D* pVtx;
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
	for (int  nCntEffect = 0;  nCntEffect < MAX_EFFECT;  nCntEffect++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x -30.0f, g_aEffect[nCntEffect].pos.y +60.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + 30.0f, g_aEffect[nCntEffect].pos.y+ 60.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x-30.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x +30.0f, 0.0f, 0.0f);

		//法線ベクトル
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


		//色の設定
		pVtx[0].col = D3DCOLOR_RGBA(155, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(155, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(155, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(155, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();

}

//=====================
//エフェクトの終了処理
//=====================
void UninitEffect(void)
{
	//テクスチャの破棄
	if (g_pTextureEffect != NULL)
	{			
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}		
	if (g_pVtxBuffEffect != NULL)
	{			
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}
//=====================
//エフェクトの更新処理
//=====================
void UpdateEffect(void)
{
	VERTEX_3D* pVtx;
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);


	g_pVtxBuffEffect->Unlock();
}
//=====================
//エフェクトの描画処理
//=====================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();
	D3DXMATRIX  mtxTrans;		//計算用マトリックス
	D3DXMATRIX mtxView;			//ビューマトリックス取得
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//ワールドマトリックス初期化
		D3DXMatrixIdentity(&g_aEffect[nCntEffect].g_mtxWorldEffect);
		//ビューマトリックス初期化
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);
		D3DXMatrixInverse(&g_aEffect[nCntEffect].g_mtxWorldEffect, NULL, &mtxView);			//逆行列を求める
		g_aEffect[nCntEffect].g_mtxWorldEffect._41 = 0.0f;
		g_aEffect[nCntEffect].g_mtxWorldEffect._42 = 0.0f;
		g_aEffect[nCntEffect].g_mtxWorldEffect._43 = 0.0f;


		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);

		D3DXMatrixMultiply(&g_aEffect[nCntEffect].g_mtxWorldEffect, &g_aEffect[nCntEffect].g_mtxWorldEffect, &mtxTrans);				//後ろの二人を掛け合わせるのが前のになる
		pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].g_mtxWorldEffect);
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));		//ここのsizeof(VERTEX)には*がいらない頂点バッファの時だけ

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);
		//Zテスト有効
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		//Zテストを無効
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		//アルファテスト有効
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 201);
		//アルファテスト無効
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 11);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureEffect);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);		//TRIANGLELIST,1 TRIANGLESTRIP,2は四角形

	}
}



