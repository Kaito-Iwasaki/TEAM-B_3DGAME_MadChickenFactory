//===========================================================
//
// チームロゴ[team_logo.cpp]
// Author : 土田　青空
//
//===========================================================
#include "main.h"
#include "team_logo.h"
#include"input.h"
#include"fade.h"
#include"sound.h"
#include"SE_controller.h"
//===========================================================
// 
// マクロ定義
// 
//===========================================================
#define TITLELOGO_WIDTH		(200.0f)	//チームロゴの幅
#define TITLEROGO_HEIGHT	(50.0f)	//高さ
#define MAX_TEAMLOGO (2)				//ロゴの最大数
//===========================================================
// 
//グローバル変数
// 
//===========================================================
LPDIRECT3DTEXTURE9 g_pTextureTeamLogo[TEAMLOGO_MAX] = {};		//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTeamLogo;		//頂点バッファへのポインタ
TeamLogo g_TeamLogo[MAX_TEAMLOGO];
int g_Start;

//===========================================================
// チームロゴの初期化処理
//===========================================================
void InitTeamLogo(void)
{
	
	g_TeamLogo[0].pos = D3DXVECTOR3(620.0f, 480.0f, 0.0f);
	
	g_TeamLogo[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	g_TeamLogo[0].type = TEAMLOGO_1PLAYER;

	g_TeamLogo[0].bUse = true;

	g_TeamLogo[1].pos = D3DXVECTOR3(620.0f, 580.0f, 0.0f);

	g_TeamLogo[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
	g_TeamLogo[1].type = TEAMLOGO_2PLAYER;

	g_TeamLogo[1].bUse = true;
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\1player.png", &g_pTextureTeamLogo[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\2player.png", &g_pTextureTeamLogo[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TEAMLOGO,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTeamLogo,
		NULL);
	
	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	
	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffTeamLogo->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntTeamLogo = 0; nCntTeamLogo < MAX_TEAMLOGO; nCntTeamLogo++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_TeamLogo[nCntTeamLogo].pos.x - TITLELOGO_WIDTH, g_TeamLogo[nCntTeamLogo].pos.y - TITLEROGO_HEIGHT, 0.0f);//右回りで！
		pVtx[1].pos = D3DXVECTOR3(g_TeamLogo[nCntTeamLogo].pos.x + TITLELOGO_WIDTH, g_TeamLogo[nCntTeamLogo].pos.y - TITLEROGO_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_TeamLogo[nCntTeamLogo].pos.x - TITLELOGO_WIDTH, g_TeamLogo[nCntTeamLogo].pos.y + TITLEROGO_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_TeamLogo[nCntTeamLogo].pos.x + TITLELOGO_WIDTH, g_TeamLogo[nCntTeamLogo].pos.y + TITLEROGO_HEIGHT, 0.0f);
																							 
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = g_TeamLogo[nCntTeamLogo].col;
		pVtx[1].col = g_TeamLogo[nCntTeamLogo].col;
		pVtx[2].col = g_TeamLogo[nCntTeamLogo].col;
		pVtx[3].col = g_TeamLogo[nCntTeamLogo].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffTeamLogo->Unlock();

}

//===========================================================
// チームロゴの終了処理
//===========================================================
void UninitTeamLogo(void)
{
	for (int nCntTeamLogo = 0; nCntTeamLogo < MAX_TEAMLOGO; nCntTeamLogo++)
	{

		//テクスチャの破棄
		if (g_pTextureTeamLogo[nCntTeamLogo] != NULL)
		{
			g_pTextureTeamLogo[nCntTeamLogo]->Release();
			g_pTextureTeamLogo[nCntTeamLogo] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffTeamLogo != NULL)
	{			   
		g_pVtxBuffTeamLogo->Release();
		g_pVtxBuffTeamLogo = NULL;
	}
	StopSound();
}

//===========================================================
// チームロゴの更新処理
//===========================================================
void UpdateTeamLogo(void)
{
	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffTeamLogo->Lock(0, 0, (void**)&pVtx, 0);
	if (GetKeyboardTrigger(DIK_S) || GetKeyboardTrigger(DIK_DOWN) || GetJoystickTrigger(JOYSTICK_L_DOWN))
	{//選択フレームを下に
		g_Start++;

		if (g_Start >= TEAMLOGO_MAX)
		{//選択フレームが下限
			g_Start = TEAMLOGO_1PLAYER;
		}
	}
	else if (GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_UP) || GetJoystickTrigger(JOYSTICK_L_UP))
	{//選択フレームを上に
		g_Start--;

		if (g_Start <= -1)
		{//選択フレームが上限
			g_Start = TEAMLOGO_2PLAYER;
		}
		
	}
	if (GetKeyboardTrigger(DIK_P) || GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_A))
	{

	
		if (g_Start == TEAMLOGO_1PLAYER)
		{
			
			SetFade(MODE_GAME);
			
		}
		else if (g_Start == TEAMLOGO_2PLAYER)
		{
			
			SetFade(MODE_GAME);
			
		}
	}
	for (int nCntTeamLogo = 0; nCntTeamLogo < MAX_TEAMLOGO; nCntTeamLogo++)
	{
		if (g_Start != nCntTeamLogo)
		{//選択中で無いものをfalse
			g_TeamLogo[nCntTeamLogo].bUse = false;
		}
		else
		{
			g_TeamLogo[nCntTeamLogo].bUse = true;
		}

		if (g_TeamLogo[nCntTeamLogo].bUse == true)
		{
			g_TeamLogo[nCntTeamLogo].col.a = 1.0f;
		}
		else
		{
			g_TeamLogo[nCntTeamLogo].col.a = 0.2f;
		}
		
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_TeamLogo[nCntTeamLogo].pos.x - TITLELOGO_WIDTH, g_TeamLogo[nCntTeamLogo].pos.y - TITLEROGO_HEIGHT, 0.0f);//右回りで！
		pVtx[1].pos = D3DXVECTOR3(g_TeamLogo[nCntTeamLogo].pos.x + TITLELOGO_WIDTH, g_TeamLogo[nCntTeamLogo].pos.y - TITLEROGO_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_TeamLogo[nCntTeamLogo].pos.x - TITLELOGO_WIDTH, g_TeamLogo[nCntTeamLogo].pos.y + TITLEROGO_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_TeamLogo[nCntTeamLogo].pos.x + TITLELOGO_WIDTH, g_TeamLogo[nCntTeamLogo].pos.y + TITLEROGO_HEIGHT, 0.0f);

		pVtx[0].col = g_TeamLogo[nCntTeamLogo].col;
		pVtx[1].col = g_TeamLogo[nCntTeamLogo].col;
		pVtx[2].col = g_TeamLogo[nCntTeamLogo].col;
		pVtx[3].col = g_TeamLogo[nCntTeamLogo].col;

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffTeamLogo->Unlock();
}

//===========================================================
// チームロゴの描画処理
//===========================================================
void DrawTeamLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTeamLogo, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int nCntTeamLogo = 0; nCntTeamLogo < MAX_TEAMLOGO; nCntTeamLogo++)
	{

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTeamLogo[nCntTeamLogo]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntTeamLogo * 4, 2);
	}
}
int GetTitle(void)
{
	return g_Start; // または nullptr
	
}
