//===========================================================
//
// ゲームオーバー[Gameover.cpp]
// Author : 土田　青空
//
//===========================================================
#include "main.h"
#include "Gameover.h"
#include"input.h"
#include"fade.h"
#include"sound.h"
//===========================================================
// 
// マクロ定義
// 
//===========================================================
#define TITLELOGO_WIDTH		(200.0f)	//チームロゴの幅
#define TITLEROGO_HEIGHT	(50.0f)		//高さ
#define MAX_GAMEOVER (7)				//ロゴの最大数
#define GAMEOVER_TIMER (11)				//ゲームオーバーの時間
#define GAMEOVER_FREAM (90)				//ゲームオーバーのフレーム
//===========================================================
// 
//グローバル変数
// 
//===========================================================
LPDIRECT3DTEXTURE9 g_pTextureGameover[GAMEOVER_MAX] = {};		//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameover;		//頂点バッファへのポインタ
TeamLogo g_Gameover[MAX_GAMEOVER];
int g_GameoverSelect;
//===========================================================
// ゲームオーバーの初期化処理
//===========================================================
void InitGameover(void)
{
	
	g_Gameover[0].pos = D3DXVECTOR3(620.0f, 480.0f, 0.0f);

	g_Gameover[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	  
	g_Gameover[0].type = GAMEOVER_RETRY;
	  
	g_Gameover[0].bUse = true;
	  
	g_Gameover[1].pos = D3DXVECTOR3(620.0f, 580.0f, 0.0f);
	  
	g_Gameover[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	  
	g_Gameover[1].type = GAMEOVER_QUIT;
	 
	g_Gameover[1].bUse = true;

	g_Gameover[2].pos = D3DXVECTOR3(120.0f, 80.0f, 0.0f);

	g_Gameover[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);

	g_Gameover[2].type = GAMEOVER_BLOOD1;

	g_Gameover[2].bUse = false;

	g_Gameover[3].pos = D3DXVECTOR3(820.0f, -80.0f, 0.0f);

	g_Gameover[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);

	g_Gameover[3].type = GAMEOVER_BLOOD2;

	g_Gameover[3].bUse = false;

	g_Gameover[4].pos = D3DXVECTOR3(470.0f, 100.0f, 0.0f);

	g_Gameover[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	g_Gameover[4].type = GAMEOVER_GAMEOVER;

	g_Gameover[4].bUse = true;

	g_Gameover[5].pos = D3DXVECTOR3(600.0f, 350.0f, 0.0f);

	g_Gameover[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	g_Gameover[5].type = GAMEOVER_TIMER1;

	g_Gameover[5].bUse = false;

	g_Gameover[5].Fream = 60;

	g_Gameover[5].Timer = 1;

	g_Gameover[6].pos = D3DXVECTOR3(650.0f, 350.0f, 0.0f);

	g_Gameover[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	g_Gameover[6].type = GAMEOVER_TIMER2;

	g_Gameover[6].bUse = false;

	g_Gameover[6].Fream = 60;

	g_Gameover[6].Timer = 10;

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\retry.png", &g_pTextureGameover[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pause002.png", &g_pTextureGameover[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\blood.png", &g_pTextureGameover[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\blood.png", &g_pTextureGameover[3]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\gameover.png", &g_pTextureGameover[4]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_pTextureGameover[5]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_pTextureGameover[6]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GAMEOVER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameover,
		NULL);
	
	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	
	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameover->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntGameover = 0; nCntGameover < MAX_GAMEOVER; nCntGameover++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x - TITLELOGO_WIDTH, g_Gameover[nCntGameover].pos.y - TITLEROGO_HEIGHT, 0.0f);//右回りで！
		pVtx[1].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x + TITLELOGO_WIDTH, g_Gameover[nCntGameover].pos.y - TITLEROGO_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x - TITLELOGO_WIDTH, g_Gameover[nCntGameover].pos.y + TITLEROGO_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x + TITLELOGO_WIDTH, g_Gameover[nCntGameover].pos.y + TITLEROGO_HEIGHT, 0.0f);
																							 
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = g_Gameover[nCntGameover].col;
		pVtx[1].col = g_Gameover[nCntGameover].col;
		pVtx[2].col = g_Gameover[nCntGameover].col;
		pVtx[3].col = g_Gameover[nCntGameover].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffGameover->Unlock();
	g_GameoverSelect = 0;
}

//===========================================================
//ゲームオーバーの終了処理
//===========================================================
void UninitGameover(void)
{
	for (int nCntGameover = 0; nCntGameover < MAX_GAMEOVER; nCntGameover++)
	{

		//テクスチャの破棄
		if (g_pTextureGameover[nCntGameover] != NULL)
		{
			g_pTextureGameover[nCntGameover]->Release();
			g_pTextureGameover[nCntGameover] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffGameover != NULL)
	{			   
		g_pVtxBuffGameover->Release();
		g_pVtxBuffGameover = NULL;
	}
}

//===========================================================
// ゲームオーバーの更新処理
//===========================================================
void UpdateGameover(void)
{
	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameover->Lock(0, 0, (void**)&pVtx, 0);

	if (GetKeyboardTrigger(DIK_S) || GetKeyboardTrigger(DIK_DOWN) || GetJoystickTrigger(JOYSTICK_L_DOWN))
	{//選択フレームを下に
		g_GameoverSelect++;

		if (g_GameoverSelect >= GAMEOVER_BLOOD1)
		{//選択フレームが下限
			g_GameoverSelect = GAMEOVER_RETRY;
		}
	}
	else if (GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_UP) || GetJoystickTrigger(JOYSTICK_L_UP))
	{//選択フレームを上に
		g_GameoverSelect--;

		if (g_GameoverSelect <= -1)
		{//選択フレームが上限
			g_GameoverSelect = GAMEOVER_QUIT;
		}
		
	}
	if (GetKeyboardTrigger(DIK_P) || GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_A))
	{
		if (g_GameoverSelect == GAMEOVER_RETRY)
		{
			SetFade(MODE_GAME);
			
		}
		else if (g_GameoverSelect == GAMEOVER_QUIT)
		{
			SetFade(MODE_LOGO);
	
		}
	}
	
	for (int nCntGameover = 0;  nCntGameover < MAX_GAMEOVER;  nCntGameover++)
	{
		if (g_GameoverSelect != nCntGameover && g_Gameover[nCntGameover].type <= 1)
		{//選択中で無いものをfalse
			g_Gameover[nCntGameover].bUse = false;
		}
		else
		{
			g_Gameover[nCntGameover].bUse = true;
		}

		if (g_Gameover[nCntGameover].bUse == true)
		{
			g_Gameover[nCntGameover].col.a = 1.0f;
		}
		else
		{
			g_Gameover[nCntGameover].col.a = 0.2f;
		}
		switch (g_Gameover[nCntGameover].type)
		{
		case GAMEOVER_RETRY:
			//頂点座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[0].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x - TITLELOGO_WIDTH,g_Gameover[nCntGameover].pos.y - TITLEROGO_HEIGHT * 2, 0.0f);//右回りで！
		pVtx[1].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x + TITLELOGO_WIDTH,g_Gameover[nCntGameover].pos.y - TITLEROGO_HEIGHT * 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x - TITLELOGO_WIDTH,g_Gameover[nCntGameover].pos.y + TITLEROGO_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x + TITLELOGO_WIDTH,g_Gameover[nCntGameover].pos.y + TITLEROGO_HEIGHT, 0.0f);
			break;
		case GAMEOVER_QUIT:
			//頂点座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[0].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x - TITLELOGO_WIDTH, g_Gameover[nCntGameover].pos.y - TITLEROGO_HEIGHT * 2, 0.0f);//右回りで！
			pVtx[1].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x + TITLELOGO_WIDTH, g_Gameover[nCntGameover].pos.y - TITLEROGO_HEIGHT * 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x - TITLELOGO_WIDTH, g_Gameover[nCntGameover].pos.y + TITLEROGO_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x + TITLELOGO_WIDTH, g_Gameover[nCntGameover].pos.y + TITLEROGO_HEIGHT, 0.0f);
			break;
		case GAMEOVER_BLOOD1:
			//頂点座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[0].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x - TITLELOGO_WIDTH, g_Gameover[nCntGameover].pos.y - TITLEROGO_HEIGHT * 2, 0.0f);//右回りで！
			pVtx[1].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x + TITLELOGO_WIDTH + 100.0f, g_Gameover[nCntGameover].pos.y - TITLEROGO_HEIGHT * 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x - TITLELOGO_WIDTH, g_Gameover[nCntGameover].pos.y + TITLEROGO_HEIGHT * 4, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x + TITLELOGO_WIDTH + 100.0f, g_Gameover[nCntGameover].pos.y + TITLEROGO_HEIGHT * 4, 0.0f);

			g_Gameover[nCntGameover].col.a = 0.5f;
			break;
		case GAMEOVER_BLOOD2:
			//頂点座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[0].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x - TITLELOGO_WIDTH, g_Gameover[nCntGameover].pos.y - TITLEROGO_HEIGHT * 3, 0.0f);//右回りで！
			pVtx[1].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x + TITLELOGO_WIDTH + 500.0f, g_Gameover[nCntGameover].pos.y - TITLEROGO_HEIGHT * 3, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x - TITLELOGO_WIDTH, g_Gameover[nCntGameover].pos.y + TITLEROGO_HEIGHT + 400.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x + TITLELOGO_WIDTH + 500.0f, g_Gameover[nCntGameover].pos.y + TITLEROGO_HEIGHT + 400.0f, 0.0f);
			g_Gameover[nCntGameover].col.a = 0.5f;
			break;
		case GAMEOVER_GAMEOVER:
			//頂点座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx[0].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x - TITLELOGO_WIDTH, g_Gameover[nCntGameover].pos.y - TITLEROGO_HEIGHT, 0.0f);//右回りで！
			pVtx[1].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x + TITLELOGO_WIDTH + 350.0f, g_Gameover[nCntGameover].pos.y - TITLEROGO_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x - TITLELOGO_WIDTH, g_Gameover[nCntGameover].pos.y + TITLEROGO_HEIGHT + 200.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x + TITLELOGO_WIDTH + 350.0f,  g_Gameover[nCntGameover].pos.y + TITLEROGO_HEIGHT + 200.0f, 0.0f);
			break;
		case GAMEOVER_TIMER1:
			//頂点座標の設定
			
			
			pVtx[0].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.2f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.2f, 1.0f);

			pVtx[0].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x - TITLELOGO_WIDTH + 175.0f, g_Gameover[nCntGameover].pos.y - TITLEROGO_HEIGHT + 25.0f, 0.0f);//右回りで！
			pVtx[1].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x + TITLELOGO_WIDTH - 175.0f, g_Gameover[nCntGameover].pos.y - TITLEROGO_HEIGHT + 25.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x - TITLELOGO_WIDTH + 175.0f, g_Gameover[nCntGameover].pos.y + TITLEROGO_HEIGHT - 25.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x + TITLELOGO_WIDTH - 175.0f, g_Gameover[nCntGameover].pos.y + TITLEROGO_HEIGHT - 25.0f, 0.0f);
			
			if (g_Gameover[nCntGameover].Timer >= 1)
			{
				if (g_Gameover[nCntGameover].Fream >= 1)
				{
					g_Gameover[nCntGameover].Fream--;
				}
				else
				{
					g_Gameover[nCntGameover].Timer--;
					g_Gameover[nCntGameover].Fream = GAMEOVER_FREAM;
				}
			}
			else
			{
				g_Gameover[nCntGameover].col.a = 0.0f;
			}
			

			break;
		case GAMEOVER_TIMER2:
			//頂点座標の設定
			if (g_Gameover[nCntGameover].Timer == 10)
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
			}
			if (g_Gameover[nCntGameover].Timer == 9)
			{
				pVtx[0].tex = D3DXVECTOR2(0.9f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.9f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
			pVtx[0].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x - TITLELOGO_WIDTH + 175.0f, g_Gameover[nCntGameover].pos.y - TITLEROGO_HEIGHT + 25.0f, 0.0f);//右回りで！
			pVtx[1].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x + TITLELOGO_WIDTH - 175.0f, g_Gameover[nCntGameover].pos.y - TITLEROGO_HEIGHT + 25.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x - TITLELOGO_WIDTH + 175.0f, g_Gameover[nCntGameover].pos.y + TITLEROGO_HEIGHT - 25.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Gameover[nCntGameover].pos.x + TITLELOGO_WIDTH - 175.0f, g_Gameover[nCntGameover].pos.y + TITLEROGO_HEIGHT - 25.0f, 0.0f);
			if (g_Gameover[nCntGameover].Timer >= 1)
			{
				if (g_Gameover[nCntGameover].Fream >= 1)
				{
					g_Gameover[nCntGameover].Fream--;
				}
				else
				{
					if (nCntGameover <= 8)
					{
						pVtx[0].tex.x -= 0.1f;
						pVtx[1].tex.x -= 0.1f;
						pVtx[2].tex.x -= 0.1f;
						pVtx[3].tex.x -= 0.1f;
					}
					
					g_Gameover[nCntGameover].Timer--;
					g_Gameover[nCntGameover].Fream = GAMEOVER_FREAM;
				}
				
			}
			else
			{
				g_Gameover[nCntGameover].col.a = 0.0f;
				SetFade(MODE_LOGO);
			}
			break;
		default:
			break;
		}
		

		pVtx[0].col = g_Gameover[nCntGameover].col;
		pVtx[1].col = g_Gameover[nCntGameover].col;
		pVtx[2].col = g_Gameover[nCntGameover].col;
		pVtx[3].col = g_Gameover[nCntGameover].col;

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffGameover->Unlock();
}

//===========================================================
// ゲームオーバーの描画処理
//===========================================================
void DrawGameover(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGameover, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int nCntGameover = 0; nCntGameover < MAX_GAMEOVER; nCntGameover++)
	{

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureGameover[nCntGameover]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntGameover * 4, 2);
	}
}

