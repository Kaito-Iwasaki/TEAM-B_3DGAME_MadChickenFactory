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
//==================
//マクロ定義 
//==================
#define MAX_EFFECT (512)			//エフェクトの最大数
//==============
//グローバル変数
//==============
LPDIRECT3DTEXTURE9 g_pTextureEffect[EFFECTTYPE_MAX] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	//頂点バッファのポインタ
Effect g_aEffect[MAX_EFFECT];
//====================
//初期化処理
//====================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\shadow000.jpg", &g_pTextureEffect[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\dasheffect.png", &g_pTextureEffect[1]);				//ダッシュエフェクト
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\landingeffect.png", &g_pTextureEffect[2]);			//着地エフェクト

	for (int nCntEffect = 0;  nCntEffect < MAX_EFFECT;  nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//エフェクトの位置
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//エフェクトの移動
		g_aEffect[nCntEffect].type = EFFECTTYPE_SHADOW;							//エフェクトの種類
		g_aEffect[nCntEffect].bUse = false;										//使用しているかどうか
		g_aEffect[nCntEffect].nLife = 100;										//エフェクトの描画時間
		g_aEffect[nCntEffect].col = (0.0f, 0.0f, 0.0f, 0.0f);					//エフェクトのカラー

	}
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4*MAX_EFFECT , D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL);		//sizeof(VERTEX)の後に*と頂点数を書く
	VERTEX_3D* pVtx;
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
	for (int  nCntEffect = 0;  nCntEffect < MAX_EFFECT;  nCntEffect++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3( -10.0f, 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3( + 10.0f, 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3( +10.0f, 0.0f, 0.0f);

		//法線ベクトル
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

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
	if (g_pTextureEffect[0] != NULL)
	{			
		g_pTextureEffect[0]->Release();
		g_pTextureEffect[0] = NULL;
	}	
	if (g_pTextureEffect[1] != NULL)
	{
		g_pTextureEffect[1]->Release();
		g_pTextureEffect[1] = NULL;
	}
	if (g_pTextureEffect[2] != NULL)
	{
		g_pTextureEffect[2]->Release();
		g_pTextureEffect[2] = NULL;
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
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//エフェクトの移動
			g_aEffect[nCntEffect].pos.x += g_aEffect[nCntEffect].move.x;
			g_aEffect[nCntEffect].pos.y += g_aEffect[nCntEffect].move.y;
			g_aEffect[nCntEffect].pos.z += g_aEffect[nCntEffect].move.z;

			//色の反映
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;


			g_aEffect[nCntEffect].nLife--;
			//寿命のカウントダウン
			if (g_aEffect[nCntEffect].nLife < 1)
			{
				g_aEffect[nCntEffect].bUse = false;

			}
			switch (g_aEffect[nCntEffect].type)
			{
			case EFFECTTYPE_SHADOW:				

				break;
			case EFFECTTYPE_DASH:				//走った後の煙
				pVtx[0].pos = D3DXVECTOR3( -15.0f,  30.0f, 0.0f);		//マトリックスでもらっているからpos指定しなくてもいい
				pVtx[1].pos = D3DXVECTOR3( 15.0f,  30.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3( -15.0f, 0.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3( 15.0f, 0.0f, 0.0f);

				break;
			case EFFECTTYPE_LANDINGE:			//着地の煙
				pVtx[0].pos = D3DXVECTOR3( - 15.0f, 30.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(  15.0f,  30.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3( - 15.0f, 0.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(  15.0f, 0.0f, 0.0f);
				//法線ベクトル
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.5f);
				pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.5f);
				pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.5f);
				pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.5f);
				break;
			default:
				break;
			}
		}
		pVtx += 4;
	}

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

		D3DXMatrixMultiply(&g_aEffect[nCntEffect].g_mtxWorldEffect, &g_aEffect[nCntEffect].g_mtxWorldEffect, &mtxTrans);//後ろの二人を掛け合わせるのが前のになる
		
		//ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].g_mtxWorldEffect);
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));		//ここのsizeof(VERTEX)には*がいらない頂点バッファの時だけ

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//zテスト無効
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		
		
		
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect[g_aEffect[nCntEffect].type]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);		//TRIANGLELIST,1 TRIANGLESTRIP,2は四角形


		}

		//zテスト有効
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		
	}
}
//===================
//エフェクトのセット
//===================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, EFFECTTYPE type, int nLife , D3DXCOLOR col)
{
	VERTEX_3D* pVtx;
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;

			g_aEffect[nCntEffect].move = move;

			g_aEffect[nCntEffect].type = type;

			g_aEffect[nCntEffect].nLife = nLife;

			g_aEffect[nCntEffect].col = col;

			g_aEffect[nCntEffect].bUse = true;

			break;
		}
		pVtx += 4;
	}
	
	g_pVtxBuffEffect->Unlock();
}



