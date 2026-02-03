//===================================
// 
//  プロンプト処理　[prompt.cpp]
//  Author shuuhei Ida
//
//===================================

#include "input.h"
#include "prompt.h"
#include "util.h"
#include "player.h"
#include "DebugProc.h"


// マクロ定義
#define PROMPT_TXT_PASS "data\\TEXTURE\\pressA.png"			// プロンプトのテクスチャパス
#define PROMPT_TEXTURE_SIZE_Y (100.0f)						// プロンプトのテクスチャサイズ(Y)
#define PROMPT_TEXTURE_SIZE_X (100.0f)						// プロンプトのテクスチャサイズ(X)
#define VIEW_PROMPT (300.0f)								// プロンプトを表示する範囲
#define MAX_COUNT_PLAYER (2)								// プレイヤーのポインタをずらす用のマクロ

// プロンプト構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 size;
	bool bUse;
	int nIdx;
	bool bDisp;
}Prompt;
// グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePrompt = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPrompt = NULL;			// 頂点バッファへのポインタ
D3DXMATRIX g_mtxWorldPrompt;								//	ワールドマトリックス
Prompt g_aPrompt[MAX_PROMPT];
bool g_aPromptTrigger[MAX_PROMPT];

//=========================
// ビルボードの初期化処理
//=========================
void InitPrompt(void)
{
	LPDIRECT3DDEVICE9 pDevice ;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, PROMPT_TXT_PASS, &g_pTexturePrompt);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_PROMPT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPrompt,
		NULL);

	VERTEX_3D* pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPrompt->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCountPrompt = 0; nCountPrompt < MAX_PROMPT; nCountPrompt++)
	{
		
		g_aPrompt[nCountPrompt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPrompt[nCountPrompt].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPrompt[nCountPrompt].bUse = false;
		g_aPrompt[nCountPrompt].bDisp = false;

		// 頂点座標の設定(x,y,z,の順番になる、zの値は2Dの場合は必ず0にする)
		pVtx[0].pos = D3DXVECTOR3(- g_aPrompt[nCountPrompt].size.x, + g_aPrompt[nCountPrompt].size.y, g_aPrompt[nCountPrompt].pos.z);
		pVtx[1].pos = D3DXVECTOR3(+ g_aPrompt[nCountPrompt].size.x, + g_aPrompt[nCountPrompt].size.y, g_aPrompt[nCountPrompt].pos.z);
		pVtx[2].pos = D3DXVECTOR3(- g_aPrompt[nCountPrompt].size.x, - g_aPrompt[nCountPrompt].size.y, g_aPrompt[nCountPrompt].pos.z);
		pVtx[3].pos = D3DXVECTOR3(+ g_aPrompt[nCountPrompt].size.x, - g_aPrompt[nCountPrompt].size.y, g_aPrompt[nCountPrompt].pos.z);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	
	// 頂点バッファをアンロックする
	g_pVtxBuffPrompt->Unlock();

	SetPrompt(D3DXVECTOR3(1000.0f, 0.0f, 0.0f), D3DXVECTOR3(100.0f, 100.0f, 0.0f), 0);
}

//======================
// ビルボードの終了処理
//======================
void UninitPrompt(void)
{
	// テクスチャの破棄
	if (g_pTexturePrompt != NULL)
	{
		g_pTexturePrompt->Release();
		g_pTexturePrompt = NULL;
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffPrompt != NULL)
	{
		g_pVtxBuffPrompt->Release();

		g_pVtxBuffPrompt = NULL;
	}
}

//======================
// ビルボードの更新処理
//======================
void UpdatePrompt(void)
{

	for (int nCountPrompt = 0; nCountPrompt < MAX_PROMPT; nCountPrompt++)
	{// １プロンプトずつ確認していく
		if (g_aPrompt[nCountPrompt].bUse == false) continue;

		Player* pPlayer = GetPlayer();
		int nPlayerCounter = 0;						// 初期化

		for (int nCountPlayer = 0; nCountPlayer < MAX_COUNT_PLAYER; nCountPlayer++, pPlayer++)
		{// １プレイヤーずつ確認していく
			bool bRengeX = VIEW_PROMPT >= fabsf(pPlayer->pos.x - g_aPrompt[nCountPrompt].pos.x);
			bool bRengeY = VIEW_PROMPT >= fabsf(pPlayer->pos.y - g_aPrompt[nCountPrompt].pos.y);
			bool bRengeZ = VIEW_PROMPT >= fabsf(pPlayer->pos.z - g_aPrompt[nCountPrompt].pos.z);
			bool bRenge = bRengeX && bRengeY && bRengeZ;

			if (bRenge)
			{
				nPlayerCounter++;					// プレイヤー数のカウントを＋１

				if (g_aPrompt[nCountPrompt].bDisp == true && (((nCountPlayer == 0) && (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_A)))|| ((nCountPlayer == 1) && (GetKeyboardTrigger(DIK_NUMPAD1) || GetJoypadTrigger(JOYKEY_A)))))
				{
					g_aPromptTrigger[nCountPrompt] = true;
				}
				else
				{
					g_aPromptTrigger[nCountPrompt] = false;
				}
			}	
		}

		PrintDebugProc("%d\n", nPlayerCounter);

		// プロンプトに近いプレイヤーの数のカウントが０じゃなかったら表示する
		if (nPlayerCounter >= 1)
		{
			g_aPrompt[nCountPrompt].bDisp = true;
		}
		else
		{
			g_aPrompt[nCountPrompt].bDisp = false;

		}

		
	}
}

//======================
// ビルボードの描画処理
//======================
void DrawPrompt(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();				// デバイスへのポインタ
	D3DXMATRIX mtxTrans;
	D3DXMATRIX mtxView;										// ビューマトリックス取得用

	for(int nCountPrompt = 0; nCountPrompt < MAX_PROMPT; nCountPrompt++)
	{
		if (g_aPrompt[nCountPrompt].bUse == true && g_aPrompt[nCountPrompt].bDisp == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldPrompt);

			// ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// ポリゴンをカメラに対して正面を向ける
			D3DXMatrixInverse(&g_mtxWorldPrompt, NULL, &mtxView); // 逆行列を求める
			g_mtxWorldPrompt._41 = 0.0f;
			g_mtxWorldPrompt._42 = 0.0f;
			g_mtxWorldPrompt._43 = 0.0f;

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aPrompt[nCountPrompt].pos.x, g_aPrompt[nCountPrompt].pos.y, g_aPrompt[nCountPrompt].pos.z);

			D3DXMatrixMultiply(&g_mtxWorldPrompt, &g_mtxWorldPrompt, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPrompt);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffPrompt, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// ライティングを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

			// アルファテストを有効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

			// Zテストを無効にする
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexturePrompt);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				0,
				2);
			// ライティングを有効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			// Zテストを有効にする
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			// アルファテストを有効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DPCMPCAPS_ALWAYS);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

		}

	}


}

//=====================================
// 設置処理
//=====================================
void SetPrompt(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nIdx)
{

	float fTexsizeX;
	float fTexsizeY;

	VERTEX_3D* pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPrompt->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCountPrompt = 0; nCountPrompt < MAX_PROMPT; nCountPrompt++)
	{
		if (g_aPrompt[nCountPrompt].bUse == false)
		{
			g_aPrompt[nCountPrompt].pos = pos;
			g_aPrompt[nCountPrompt].size = size;
			g_aPrompt[nCountPrompt].bUse = true;
			g_aPrompt[nCountPrompt].nIdx = nIdx;
			g_aPrompt[nCountPrompt].bDisp = false;

			// 頂点座標の設定(x,y,z,の順番になる、zの値は2Dの場合は必ず0にする)
			pVtx[0].pos = D3DXVECTOR3( - g_aPrompt[nCountPrompt].size.x,  + g_aPrompt[nCountPrompt].size.y, g_aPrompt[nCountPrompt].pos.z);
			pVtx[1].pos = D3DXVECTOR3( + g_aPrompt[nCountPrompt].size.x,  + g_aPrompt[nCountPrompt].size.y, g_aPrompt[nCountPrompt].pos.z);
			pVtx[2].pos = D3DXVECTOR3( - g_aPrompt[nCountPrompt].size.x,  - g_aPrompt[nCountPrompt].size.y, g_aPrompt[nCountPrompt].pos.z);
			pVtx[3].pos = D3DXVECTOR3( + g_aPrompt[nCountPrompt].size.x,  - g_aPrompt[nCountPrompt].size.y, g_aPrompt[nCountPrompt].pos.z);

			float fTexsizeX = g_aPrompt[nCountPrompt].size.x / PROMPT_TEXTURE_SIZE_X;
			float fTexsizeY = g_aPrompt[nCountPrompt].size.y / PROMPT_TEXTURE_SIZE_Y;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(fTexsizeX, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, fTexsizeY);
			pVtx[3].tex = D3DXVECTOR2(fTexsizeX, fTexsizeY);

			break;
		}

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffPrompt->Unlock();

}

void SetPromptUse(int nIdx, bool bUse)
{
	for (int nCountPrompt = 0; nCountPrompt < MAX_PROMPT; nCountPrompt++)
	{
		//このインデックス の bUseを変えたい
		g_aPrompt[nCountPrompt].nIdx;
		if (g_aPrompt[nCountPrompt].nIdx == nIdx)
		{
			g_aPrompt[nCountPrompt].bUse = bUse;
		}
		
	}
}

bool GetPromptTrigger(int nIdx)
{
	return g_aPromptTrigger[nIdx];
}
