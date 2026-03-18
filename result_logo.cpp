//=====================================================================
//
// リザルト画面処理 [Result.h]
// Author 土田青空: 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "result_logo.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include"DebugProc.h"
#include"Timer.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_RESULT (3)						//リザルトの最大数
#define BACKGROUND_TEXTURE_FILENAME0			"data\\TEXTURE\\Vresult_bacuground.png"		//文字を見やすくする背景
#define BACKGROUND_TEXTURE_FILENAME1			"data\\TEXTURE\\gameclear.png"		//ゲームクリアの文字
#define BACKGROUND_TEXTURE_FILENAME2			"data\\TEXTURE\\cleartime.png"		//クリアタイムの文字
#define RESULTBACKGROUND_WIDTH		(700.0f)		//背景幅
#define RESULTBACKGROUND_HEIGHT		(320.0f)		//背景高さ
#define RESULTLOGO_WIDTH		(300.0f)		//幅
#define RESULTLOGO_HEIGHT		(100.0f)		//高さ
//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DTEXTURE9 g_pTextureResult[RESULTTYPE_MAX] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;			//頂点バッファのポインタ
Result g_aResult[MAX_RESULT];
int resultbgm;
//======================
//リザルトの初期化処理
//======================
void InitResult_Logo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	//リザルトの背景
	g_aResult[0].pos = D3DXVECTOR3(600.0f, 160.0f, 0.0f);
	g_aResult[0].col = D3DXCOLOR(0.0f, 0.0f,0.0f, 0.4f);
	g_aResult[0].nType = RESULTTYPE_BACKGROUND;

	g_aResult[1].pos = D3DXVECTOR3(600.0f, 200.0f, 0.0f);
	g_aResult[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aResult[1].nType = RESULTTYPE_CLEAR;

	g_aResult[2].pos = D3DXVECTOR3(450.0f, 560.0f, 0.0f);
	g_aResult[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aResult[2].nType = RESULTTYPE_CLEARTIME;
	
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BACKGROUND_TEXTURE_FILENAME0, &g_pTextureResult[0]);		
	D3DXCreateTextureFromFile(pDevice,BACKGROUND_TEXTURE_FILENAME1, &g_pTextureResult[1]);		
	D3DXCreateTextureFromFile(pDevice, BACKGROUND_TEXTURE_FILENAME2, &g_pTextureResult[2]);		

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RESULT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult, NULL
	);	
	
	
	VERTEX_2D* pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);
	for (int nResult = 0; nResult < MAX_RESULT; nResult++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aResult[nResult].pos.x - RESULTLOGO_WIDTH, g_aResult[nResult].pos.y - RESULTLOGO_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aResult[nResult].pos.x + RESULTLOGO_WIDTH, g_aResult[nResult].pos.y - RESULTLOGO_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aResult[nResult].pos.x - RESULTLOGO_WIDTH, g_aResult[nResult].pos.y + RESULTLOGO_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aResult[nResult].pos.x + RESULTLOGO_WIDTH, g_aResult[nResult].pos.y + RESULTLOGO_HEIGHT, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = g_aResult[nResult].col;
		pVtx[1].col = g_aResult[nResult].col;
		pVtx[2].col = g_aResult[nResult].col;
		pVtx[3].col = g_aResult[nResult].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();

	resultbgm = 0;
}
//=======================
//リザルトの終了処理
//=======================
void UninitResult_Logo(void)
{
	
	for (int nResult = 0; nResult < MAX_RESULT; nResult++)
	{
		if (g_pTextureResult[nResult] != NULL)
		{//テクスチャの破棄
			g_pTextureResult[nResult]->Release();
			g_pTextureResult[nResult] = NULL;
		}
	}

	if (g_pVtxBuffResult != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}	
//==================
//リザルトの更新処理
//==================
void UpdateResult_Logo(void)
{

	
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true)
	{
		if (resultbgm == 0)
		{
			PlaySound(SOUND_LABEL_SE_DECISION);
		}
		SetFade(MODE_LOGO);
	}
	VERTEX_2D* pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);
	for (int nResult = 0; nResult < MAX_RESULT; nResult++)
	{
		switch (g_aResult[nResult].nType)
		{
		case RESULTTYPE_BACKGROUND:
			pVtx[0].pos = D3DXVECTOR3(g_aResult[nResult].pos.x - RESULTBACKGROUND_WIDTH , g_aResult[nResult].pos.y - RESULTBACKGROUND_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aResult[nResult].pos.x + RESULTBACKGROUND_WIDTH , g_aResult[nResult].pos.y - RESULTBACKGROUND_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aResult[nResult].pos.x - RESULTBACKGROUND_WIDTH , g_aResult[nResult].pos.y + RESULTBACKGROUND_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aResult[nResult].pos.x + RESULTBACKGROUND_WIDTH , g_aResult[nResult].pos.y + RESULTBACKGROUND_HEIGHT, 0.0f);

			
			break;																									
		case RESULTTYPE_CLEAR:
			pVtx[0].pos = D3DXVECTOR3(g_aResult[nResult].pos.x - RESULTLOGO_WIDTH - 200.0f, g_aResult[nResult].pos.y - RESULTLOGO_HEIGHT - 200.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aResult[nResult].pos.x + RESULTLOGO_WIDTH + 200.0f, g_aResult[nResult].pos.y - RESULTLOGO_HEIGHT - 200.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aResult[nResult].pos.x - RESULTLOGO_WIDTH - 200.0f, g_aResult[nResult].pos.y + RESULTLOGO_HEIGHT + 200.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aResult[nResult].pos.x + RESULTLOGO_WIDTH + 200.0f, g_aResult[nResult].pos.y + RESULTLOGO_HEIGHT + 200.0f, 0.0f);
			break;
		case RESULTTYPE_CLEARTIME:
			pVtx[0].pos = D3DXVECTOR3(g_aResult[nResult].pos.x - RESULTLOGO_WIDTH - 100.0f, g_aResult[nResult].pos.y - RESULTLOGO_HEIGHT - 100.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aResult[nResult].pos.x + RESULTLOGO_WIDTH + 100.0f, g_aResult[nResult].pos.y - RESULTLOGO_HEIGHT - 100.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aResult[nResult].pos.x - RESULTLOGO_WIDTH - 100.0f, g_aResult[nResult].pos.y + RESULTLOGO_HEIGHT + 100.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aResult[nResult].pos.x + RESULTLOGO_WIDTH + 100.0f, g_aResult[nResult].pos.y + RESULTLOGO_HEIGHT + 100.0f, 0.0f);
			break;
		default:
			break;
		}
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();
}
//===================
//リザルトの描画処理
//===================
void DrawResult_Logo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,g_pVtxBuffResult, 0, sizeof(VERTEX_2D));		//ここのsizeof(VERTEX)には*がいらない頂点バッファの時だけ
	
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//アルファテスト無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 128);
	//アルファテスト有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 256);
	for (int nResult = 0; nResult < MAX_RESULT; nResult++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult[g_aResult[nResult].nType]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nResult * 4, 2);		//TRIANGLELIST,1 TRIANGLESTRIP,2は四角形
	}
}
