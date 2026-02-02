//=====================================================================
//
// パーティクル処理 [particle.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "particle.h"
#include"effect.h"
//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_PARTICLE (256)				//パーティクルの最大数

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************

Particle g_aParticle[MAX_PARTICLE];
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


//=====================================================================
// 初期化処理
//=====================================================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//位置の初期化
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//移動量の初期化
		g_aParticle[nCntParticle].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);			//色の初期化
		g_aParticle[nCntParticle].nLife = 0;										//寿命の初期化
		g_aParticle[nCntParticle].bUse = false;										//使用しているかの初期化
	}
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitParticle(void)
{

}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateParticle(void)
{
		/*float fAngle;*/

		for (int nCountParticle = 0; nCountParticle < MAX_PARTICLE; nCountParticle++)
		{

			if (g_aParticle[nCountParticle].bUse == true)
			{
				
				/*fAngle = (rand() % 629 - 314) / 100.0f;

				g_aParticle[nCountParticle].move.x = cosf(fAngle) * (rand() % 100) / 10 + 0.5;
				g_aParticle[nCountParticle].move.z = sinf(fAngle) * (rand() % 100) / 10 + 0.5;*/

				float theta = ((float)rand() / RAND_MAX) * 2.0f * 3.14159f; // 水平
				float phi = ((float)rand() / RAND_MAX) * 3.14159f;       // 上下

				float speed = ((float)rand() / RAND_MAX) * 10.0f;

				//パーティクルの球体化
				g_aParticle[nCountParticle].move.x = sinf(phi) * cosf(theta) * speed;
				g_aParticle[nCountParticle].move.y = cosf(phi) * speed;
				g_aParticle[nCountParticle].move.z = sinf(phi) * sinf(theta) * speed;


				/*g_aParticle[nCountParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 500) / 490 + 0.1f;
				g_aParticle[nCountParticle].move.y = (float)(rand() % 200) / 100 + 3.0f;
				g_aParticle[nCountParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 500) / 490 + 0.1f;*/

				//パーティクルのテクスチャ
				SetEffect(g_aParticle[nCountParticle].pos, g_aParticle[nCountParticle].move,EFFECTTYPE_DASH, 100,g_aParticle[nCountParticle].col,D3DXVECTOR3(20.0f,20.0f,0.0f));
				
				//パーティクルの寿命減少
				g_aParticle[nCountParticle].nLife--;
				if (g_aParticle[nCountParticle].nLife < 1)
				{
					g_aParticle[nCountParticle].bUse = false;
				}
			}
		}
		
		//static int dashCool = 0;

		//if (dashCool > 0) dashCool--;
		//if (GetKeyboardPress(DIK_D) == true && dashCool == 0)
		//{
		//	Player* pPlayer = GetPlayer();
		//	SetParticle(pPlayer->pos, D3DXCOLOR(0.8f, 0.5f, 0.4f, 1.0f), pPlayer->move, 100);
		//	dashCool = 20; // 10フレーム間隔
		//}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawParticle(void)
{

}
//=====================================================================
// セット処理
//=====================================================================
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col , D3DXVECTOR3 move , int nLife)
{
	for (int nCountParticle = 0; nCountParticle < MAX_PARTICLE; nCountParticle++)
	{
		if (g_aParticle[nCountParticle].bUse == false)
		{
			g_aParticle[nCountParticle].pos = pos;

			g_aParticle[nCountParticle].col = col;

			g_aParticle[nCountParticle].move = move;

			g_aParticle[nCountParticle].nLife = nLife;

			g_aParticle[nCountParticle].bUse = true;

			break;
		}
	}
}