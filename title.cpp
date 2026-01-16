#include "title.h"
#include "input.h"
#include"game.h"
#include"sound.h"
//#include "sound.h"
#include "fade.h"
#define MAX_TITLE (3)				//タイトルの種類
//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;	//頂点バッファのポインタ
TITLE g_aTitle[MAX_TITLE];
int nCnt;
//
//初期化処理
void InitTitle(void)
{
	
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

		g_aTitle[0].g_posTitle  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aTitle[0].g_moveTitle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aTitle[0].g_colerTitle = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aTitle[0].g_nCounterAnimTitle = 100;
		g_aTitle[0].g_nPatternAnimTitle = 0;
		g_aTitle[0].nType = 0;
		g_aTitle[0].bEnter = false;

		g_aTitle[1].g_posTitle = D3DXVECTOR3(150.0f, -50.0f, 0.0f);
		g_aTitle[1].g_moveTitle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aTitle[1].g_colerTitle = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aTitle[1].g_nCounterAnimTitle = 0;
		g_aTitle[1].g_nPatternAnimTitle = 100;
		g_aTitle[1].nType = 1;

		g_aTitle[2].g_posTitle = D3DXVECTOR3(400.0f, 300.0f, 0.0f);
		g_aTitle[2].g_moveTitle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aTitle[2].g_colerTitle = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aTitle[2].g_nCounterAnimTitle = 90;
		g_aTitle[2].g_nPatternAnimTitle = 0;
		g_aTitle[2].nType = 2;
	
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Reslt.jpg",&g_pTextureTitle[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3Dtte.png", &g_pTextureTitle[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sart.png", &g_pTextureTitle[2]);
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TITLE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED,&g_pVtxBuffTitle, NULL);		//sizeof(VERTEX)の後に*と頂点数を書く
	VERTEX_2D* pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);
	
		//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aTitle[0].g_posTitle.x, g_aTitle[0].g_posTitle.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aTitle[0].g_posTitle.x + 1280.0f, g_aTitle[0].g_posTitle.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aTitle[0].g_posTitle.x, g_aTitle[0].g_posTitle.y + 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aTitle[0].g_posTitle.x + 1280.0f, g_aTitle[0].g_posTitle.y + 720.0f, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//色の設定
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

	pVtx[0].pos = D3DXVECTOR3(g_aTitle[1].g_posTitle.x, g_aTitle[1].g_posTitle.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aTitle[1].g_posTitle.x + 900.0f, g_aTitle[1].g_posTitle.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aTitle[1].g_posTitle.x, g_aTitle[1].g_posTitle.y + 500.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aTitle[1].g_posTitle.x + 900.0f, g_aTitle[1].g_posTitle.y + 500.0f, 0.0f);
		
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//色の設定
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

		pVtx[0].pos = D3DXVECTOR3(g_aTitle[2].g_posTitle.x, g_aTitle[2].g_posTitle.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTitle[2].g_posTitle.x + 500.0f, g_aTitle[2].g_posTitle.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTitle[2].g_posTitle.x, g_aTitle[2].g_posTitle.y + 400.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTitle[2].g_posTitle.x + 500.0f, g_aTitle[2].g_posTitle.y + 400.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//色の設定
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
	
	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();
	//サウンドの再生
	/*PlaySound(SOUND_LABEL_BGM000);*/

}

//=======================
//タイトル画面の終了処理
//=======================
void UninitTitle(void)
{
	
/*	StopSound();*/	
	//テクスチャの破棄
	if (g_pTextureTitle[0] != NULL)
	{
		g_pTextureTitle[0]->Release();
		g_pTextureTitle[0] = NULL;
	}

	if (g_pTextureTitle[1] != NULL)
	{
		g_pTextureTitle[1]->Release();
		g_pTextureTitle[1] = NULL;
	}
	if (g_pTextureTitle[2] != NULL)
	{
		g_pTextureTitle[2]->Release();
		g_pTextureTitle[2] = NULL;
	}
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}
void UpdateTitle(void)
{

	VERTEX_2D* pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);
	g_aTitle[2].g_nCounterAnimTitle--;
	
	pVtx += 8;
	if (GetKeyboardTrigger(DIK_RETURN) == true && g_aTitle[0].bEnter == false)
	{
		g_aTitle[0].bEnter = true;		//trueにして連打されないようにする
		g_aTitle[2].g_nCounterAnimTitle = 20;
		if (g_aTitle[2].g_nCounterAnimTitle <= 19 && g_aTitle[2].g_nPatternAnimTitle >= 1)
		{
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		}
		else if (g_aTitle[2].g_nCounterAnimTitle == 0)
		{
			g_aTitle[2].g_nCounterAnimTitle = 20;
		}
		else
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		//決定キー(ENTERキー)が押された
		//モード設定(チュートリアル画面に移行)
		//PlaySound(SOUND_LABEL_SE_COIN1);
		/*SetjoypadVibration(30000, 30000, 3);*/
		SetFade(MODE_GAME);
	}
	else
	{
		if (g_aTitle[2].g_nCounterAnimTitle <= 45 && g_aTitle[2].g_nCounterAnimTitle >= 1)
		{
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		
		}
		else if(g_aTitle[2].g_nCounterAnimTitle == 0)
		{
				g_aTitle[2].g_nCounterAnimTitle = 90;
		}
		else
		{
			pVtx[0].col =  D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col =  D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col =  D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col =  D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}
	if (GetJoypadTrigger(JOYKEY_START) == true)
	{//stateが押された時
		
		
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			//PlaySound(SOUND_LABEL_SE_COIN2);
			SetFade(MODE_GAME);
	}

	g_pVtxBuffTitle->Unlock();
}
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nTitle;
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));		//ここのsizeof(VERTEX)には*がいらない頂点バッファの時だけ
	
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (nTitle = 0; nTitle < MAX_TITLE; nTitle++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[nTitle]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nTitle*4,2);		//TRIANGLELIST,1 TRIANGLESTRIP,2は四角形
	
	}

}
