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
#define MAX_EFFECT (2048)			//エフェクトの最大数
#define EFFECTSIZE_DASH (60)		//ダッシュエフェクトのサイズ
#define EFFECTSIZE_LANDINGE	(80)		//着地エフェクトのサイズ
#define MAX_FREAM (60.0f)			//最大フレーム
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
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\smokeeffect.png", &g_pTextureEffect[3]);			//着地エフェクト

	for (int nCntEffect = 0;  nCntEffect < MAX_EFFECT;  nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//エフェクトの位置
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//エフェクトの移動
		g_aEffect[nCntEffect].type = EFFECTTYPE_NOMALE;							//エフェクトの種類
		g_aEffect[nCntEffect].bUse = false;										//使用しているかどうか
		g_aEffect[nCntEffect].nLife = 100;										//エフェクトの描画時間
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);					//エフェクトのカラー

		g_aEffect[nCntEffect].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//エフェクトのサイズ

	}
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4*MAX_EFFECT , D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL);		//sizeof(VERTEX)の後に*と頂点数を書く
	VERTEX_3D* pVtx;
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
	for (int  nCntEffect = 0;  nCntEffect < MAX_EFFECT;  nCntEffect++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].size.x * 0.5f, g_aEffect[nCntEffect].size.y, g_aEffect[nCntEffect].size.z);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].size.x * 0.5f, g_aEffect[nCntEffect].size.y, g_aEffect[nCntEffect].size.z);
		pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].size.x * 0.5f, 0.0f, g_aEffect[nCntEffect].size.z);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].size.x * 0.5f, 0.0f, g_aEffect[nCntEffect].size.z);

		//法線ベクトル
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

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

	for (int nCntEffect = 0; nCntEffect < EFFECTTYPE_MAX; nCntEffect++)
	{
		if (g_pTextureEffect[nCntEffect] != NULL)
		{
			g_pTextureEffect[nCntEffect]->Release();
			g_pTextureEffect[nCntEffect] = NULL;
		}
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
		
				// 色の反映
				pVtx[0].col = g_aEffect[nCntEffect].col;
				pVtx[1].col = g_aEffect[nCntEffect].col;
				pVtx[2].col = g_aEffect[nCntEffect].col;
				pVtx[3].col = g_aEffect[nCntEffect].col;
				//エフェクトの移動
				g_aEffect[nCntEffect].pos.x += g_aEffect[nCntEffect].move.x;
				g_aEffect[nCntEffect].pos.y += g_aEffect[nCntEffect].move.y;
				g_aEffect[nCntEffect].pos.z += g_aEffect[nCntEffect].move.z;

				//透明度減少
				if (g_aEffect[nCntEffect].col.a > 0.0f)
				{
					float lifeNorm = (float)g_aEffect[nCntEffect].nLife / MAX_FREAM;
					if (lifeNorm < 0.0f) lifeNorm = 0.0f;
					if (lifeNorm > 1.0f) lifeNorm = 1.0f;

					// 透明度（アルファ）を線形に減少
					g_aEffect[nCntEffect].col.a = lifeNorm;

					//寿命減少
					g_aEffect[nCntEffect].nLife--;
					if (g_aEffect[nCntEffect].nLife <= 0)
					{
				
					g_aEffect[nCntEffect].bUse = false;

					}
				}
	
					switch (g_aEffect[nCntEffect].type)
					{
					case EFFECTTYPE_NOMALE:

						break;
					case EFFECTTYPE_DASH:				//走った後の煙
							
						break;
					case EFFECTTYPE_LANDINGE:			//着地の煙
												
						break;
					case EFFECTTYPE_SMOKE:
						break;
					default:
						break;
					}
				
				//static int dashCool = 0;

				//if (dashCool > 0) dashCool--;
				//if (dashCool == 0)
				//{
				//	Player* pPlayer = GetPlayer();
				//	SetEffect(D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), EFFECTTYPE_DASH, 100, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(80.0f, 80.0f, 0.0f));
				//	dashCool = 20; // 10フレーム間隔
				//}
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
		//アルファテスト無効
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 128);
		//aプレンディングを加算合成
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		if (g_aEffect[nCntEffect].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect[g_aEffect[nCntEffect].type]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);		


		}

		//zテスト有効
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		//アルファテスト有効
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 256);

		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}
//===================
//エフェクトのセット
//===================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, EFFECTTYPE type, int nLife , D3DXCOLOR col , D3DXVECTOR3 size)
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

				g_aEffect[nCntEffect].size = size;
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].size.x * 0.5f, g_aEffect[nCntEffect].size.y, g_aEffect[nCntEffect].size.z);
				pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].size.x * 0.5f, g_aEffect[nCntEffect].size.y, g_aEffect[nCntEffect].size.z);
				pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].size.x * 0.5f, 0.0f, g_aEffect[nCntEffect].size.z);
				pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].size.x * 0.5f, 0.0f, g_aEffect[nCntEffect].size.z);

				

				g_aEffect[nCntEffect].bUse = true;

				break;
			
		}
		pVtx += 4;
	}
	
	g_pVtxBuffEffect->Unlock();
}



