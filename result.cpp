#include"result.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
//#include"score.h"
#include <stdio.h>
//#include"ranking.h"
#include <stdlib.h>
//マクロ定義
#define MAX_RESULT (1)						//リザルトの最大数
#define ZYUNI (80.0f)						//順位の大きさ
//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_RESULT] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;			//頂点バッファのポインタ
Result g_aResult[MAX_RESULT];

//======================
//リザルトの初期化処理
//======================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();
	
	//リザルトの背景
	g_aResult[0].g_posResult = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aResult[0].g_moveResult = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aResult[0].g_colerResult = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aResult[0].g_nCounterAnimResult = 0;
	g_aResult[0].g_nPatternAnimResult = 0;
	g_aResult[0].nType = 0;
	g_aResult[0].bEnter = false;
#if 0
	g_aResult[1].g_posResult = D3DXVECTOR3(500.0f, 150.0f, 0.0f);
	g_aResult[1].g_moveResult = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aResult[1].g_colerResult = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aResult[1].g_nCounterAnimResult = 0;
	g_aResult[1].g_nPatternAnimResult = 0;
	g_aResult[1].nType = 1;
	g_aResult[1].bEnter = false;

	g_aResult[2].g_posResult = D3DXVECTOR3(500.0f, 250.0f, 0.0f);
	g_aResult[2].g_moveResult = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aResult[2].g_colerResult = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aResult[2].g_nCounterAnimResult = 0;
	g_aResult[2].g_nPatternAnimResult = 0;
	g_aResult[2].nType = 2;
	g_aResult[2].bEnter = false;

	g_aResult[3].g_posResult = D3DXVECTOR3(500.0f, 350.0f, 0.0f);
	g_aResult[3].g_moveResult = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aResult[3].g_colerResult = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aResult[3].g_nCounterAnimResult = 0;
	g_aResult[3].g_nPatternAnimResult = 0;
	g_aResult[3].nType = 3;
	g_aResult[3].bEnter = false;

	g_aResult[4].g_posResult = D3DXVECTOR3(500.0f, 450.0f, 0.0f);
	g_aResult[4].g_moveResult = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aResult[4].g_colerResult = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aResult[4].g_nCounterAnimResult = 0;
	g_aResult[4].g_nPatternAnimResult = 0;
	g_aResult[4].nType = 4;
	g_aResult[4].bEnter = false;

	g_aResult[5].g_posResult = D3DXVECTOR3(500.0f, 550.0f, 0.0f);
	g_aResult[5].g_moveResult = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aResult[5].g_colerResult = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aResult[5].g_nCounterAnimResult = 0;
	g_aResult[5].g_nPatternAnimResult = 0;
	g_aResult[5].nType = 5;
	g_aResult[5].bEnter = false;
#endif
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\last.png",&g_pTextureResult[0]);		//背景
#if 0
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\1i.png", &g_pTextureResult[1]);		//1位
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\2i.png", &g_pTextureResult[2]);		//2位
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3i.png", &g_pTextureResult[3]);		//3位
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\4i.png", &g_pTextureResult[4]);		//4位
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\5i.png", &g_pTextureResult[5]);		//5位
#endif
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*4 * MAX_RESULT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED,&g_pVtxBuffResult, NULL);		//sizeof(VERTEX)の後に*と頂点数を書く
	VERTEX_2D* pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aResult[0].g_posResult.x, g_aResult[0].g_posResult.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aResult[0].g_posResult.x + 1280.0f, g_aResult[0].g_posResult.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aResult[0].g_posResult.x, g_aResult[0].g_posResult.y + 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aResult[0].g_posResult.x +1280.0f, g_aResult[0].g_posResult.y + 720.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//色の設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
#if 0
	pVtx += 4;
	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aResult[1].g_posResult.x, g_aResult[1].g_posResult.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aResult[1].g_posResult.x + ZYUNI, g_aResult[1].g_posResult.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aResult[1].g_posResult.x, g_aResult[1].g_posResult.y + ZYUNI, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aResult[1].g_posResult.x + ZYUNI, g_aResult[1].g_posResult.y + ZYUNI, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//色の設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;
	//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aResult[2].g_posResult.x, g_aResult[2].g_posResult.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aResult[2].g_posResult.x + ZYUNI, g_aResult[2].g_posResult.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aResult[2].g_posResult.x, g_aResult[2].g_posResult.y + ZYUNI, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aResult[2].g_posResult.x + ZYUNI, g_aResult[2].g_posResult.y + ZYUNI, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//色の設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;
	// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aResult[3].g_posResult.x, g_aResult[3].g_posResult.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aResult[3].g_posResult.x + ZYUNI, g_aResult[3].g_posResult.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aResult[3].g_posResult.x, g_aResult[3].g_posResult.y + ZYUNI, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aResult[3].g_posResult.x + ZYUNI, g_aResult[3].g_posResult.y + ZYUNI, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//色の設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;
	//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aResult[4].g_posResult.x, g_aResult[4].g_posResult.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aResult[4].g_posResult.x + ZYUNI, g_aResult[4].g_posResult.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aResult[4].g_posResult.x, g_aResult[4].g_posResult.y + ZYUNI, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aResult[4].g_posResult.x + ZYUNI, g_aResult[4].g_posResult.y + ZYUNI, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//色の設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;
	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aResult[5].g_posResult.x, g_aResult[5].g_posResult.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aResult[5].g_posResult.x + ZYUNI, g_aResult[5].g_posResult.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aResult[5].g_posResult.x, g_aResult[5].g_posResult.y + ZYUNI, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aResult[5].g_posResult.x + ZYUNI, g_aResult[5].g_posResult.y + ZYUNI, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//色の設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;
#endif
	//頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();
	
	//ランキング
	/*SetRanking(GetFileScore());

	ResetRanking();

	InitRanking();
	
	PlaySound(SOUND_LABEL_BGM002);*/
}
//=======================
//リザルトの終了処理
//=======================
void UninitResult(void)
{
	StopSound();
	
	//テクスチャの破棄
	if (g_pTextureResult[0] != NULL)
	{
		g_pTextureResult[0]->Release();
		g_pTextureResult[0] = NULL;
	}
#if 0
	if (g_pTextureResult[1] != NULL)
	{
		g_pTextureResult[1]->Release();
		g_pTextureResult[1] = NULL;
	}
	if (g_pTextureResult[2] != NULL)
	{
		g_pTextureResult[2]->Release();
		g_pTextureResult[2] = NULL;
	}
	if (g_pTextureResult[3] != NULL)
	{
		g_pTextureResult[3]->Release();
		g_pTextureResult[3] = NULL;
	}
	if (g_pTextureResult[4] != NULL)
	{
		g_pTextureResult[4]->Release();
		g_pTextureResult[4] = NULL;
	}
	if (g_pTextureResult[5] != NULL)
	{
		g_pTextureResult[5]->Release();
		g_pTextureResult[5] = NULL;
	}
#endif
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
	/*UninitRanking();*/
}	
//==================
//リザルトの更新処理
//==================
void UpdateResult(void)
{
	//エンターキーが押されて最初の時
	if (GetKeyboardTrigger(DIK_RETURN) == true && g_aResult[0].bEnter == false)
	{
		/*g_aResult[0].bEnter = true;*/
		//ENTERキーが押された
		SetFade(MODE_TITLE);
		//タイトル画面に移行
	}
	//スタートボタンが押されて再世の時
	if (GetJoypadTrigger(JOYKEY_START) == true && g_aResult[0].bEnter == false)
	{
		/*g_aResult[0].bEnter = true;*/
		//STARTが押された
		SetFade(MODE_TITLE);
		//タイトル画面に移行
	}
	/*UpdateRanking();*/
	
}
//===================
//リザルトの描画処理
//===================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,g_pVtxBuffResult, 0, sizeof(VERTEX_2D));		//ここのsizeof(VERTEX)には*がいらない頂点バッファの時だけ
	
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int nResult = 0; nResult < MAX_RESULT; nResult++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult[g_aResult[nResult].nType]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nResult * 4, 2);		//TRIANGLELIST,1 TRIANGLESTRIP,2は四角形
	}
	/*DrawRanking();*/
}
