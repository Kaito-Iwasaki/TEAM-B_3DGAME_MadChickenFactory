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
#include "team_logo.h"
#include "sound.h"


// マクロ定義
#define PROMPT_TXT_PASS "data\\TEXTURE\\pressB.png"			// プロンプトのテクスチャパス
#define VIEW_PROMPT (200.0f)								// プロンプトを表示する範囲
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

// プロトタイプ宣言
bool IsPromptKeyTriggered(int nCountPlayer);

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
		pVtx[0].pos = D3DXVECTOR3(- g_aPrompt[nCountPrompt].size.x, + g_aPrompt[nCountPrompt].size.y, 0);
		pVtx[1].pos = D3DXVECTOR3(+ g_aPrompt[nCountPrompt].size.x, + g_aPrompt[nCountPrompt].size.y, 0);
		pVtx[2].pos = D3DXVECTOR3(- g_aPrompt[nCountPrompt].size.x, - g_aPrompt[nCountPrompt].size.y, 0);
		pVtx[3].pos = D3DXVECTOR3(+ g_aPrompt[nCountPrompt].size.x, - g_aPrompt[nCountPrompt].size.y, 0);

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
	memset(&g_aPromptTrigger[0], 0, sizeof g_aPromptTrigger);

	for (int nCountPrompt = 0; nCountPrompt < MAX_PROMPT; nCountPrompt++)
	{// １プロンプトずつ確認していく
		if (g_aPrompt[nCountPrompt].bUse == false) continue;
		if (GetPromptTrigger(g_aPrompt[nCountPrompt].nIdx)) continue;

		Player* pPlayer = GetPlayer();
		int nPlayerCounter = 0;						// 初期化

		for (int nCountPlayer = 0; nCountPlayer < MAX_COUNT_PLAYER; nCountPlayer++, pPlayer++)
		{// １プレイヤーずつ確認していく
			if (pPlayer->bUse == false) continue;

			float fMag = Magnitude(g_aPrompt[nCountPrompt].pos, pPlayer->pos);

			if (fMag > VIEW_PROMPT)
			{
				g_aPromptTrigger[g_aPrompt[nCountPrompt].nIdx] = false;
			}
			else
			{
				nPlayerCounter++;	// プレイヤー数のカウントを＋１

				if (IsPromptKeyTriggered(nCountPlayer))
				{
					PlaySound(SOUND_LABEL_SE_SWITCH);
					g_aPromptTrigger[g_aPrompt[nCountPrompt].nIdx] = true;
					break;
				}
			}

		}

		// プロンプトに近いプレイヤーの数のカウントが０じゃなかったら表示する
		if (nPlayerCounter >= 1)
		{
			g_aPrompt[nCountPrompt].bDisp = true;
		}
		else
		{
			g_aPrompt[nCountPrompt].bDisp = false;
		}

		pPlayer = GetPlayer();

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
			DWORD dwLastLightState;
			pDevice->GetRenderState(D3DRS_LIGHTING, &dwLastLightState);
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

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
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,
				nCountPrompt * 4,
				2
			);
			// ライティングを有効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, dwLastLightState);

			// Zテストを有効にする
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			// アルファテストを無効にする
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
			pVtx[0].pos = D3DXVECTOR3( - g_aPrompt[nCountPrompt].size.x,  + g_aPrompt[nCountPrompt].size.y, 0);
			pVtx[1].pos = D3DXVECTOR3( + g_aPrompt[nCountPrompt].size.x,  + g_aPrompt[nCountPrompt].size.y, 0);
			pVtx[2].pos = D3DXVECTOR3( - g_aPrompt[nCountPrompt].size.x,  - g_aPrompt[nCountPrompt].size.y, 0);
			pVtx[3].pos = D3DXVECTOR3( + g_aPrompt[nCountPrompt].size.x,  - g_aPrompt[nCountPrompt].size.y, 0);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

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
	if (nIdx < 0 || nIdx >= MAX_PROMPT)
	{// 範囲外ならfalseを返す
		return false;
	}

	return g_aPromptTrigger[nIdx];
}

bool IsPromptKeyTriggered(int nCountPlayer)
{
	Player* pPlayer = GetPlayer();

	if (GetTitle() == 0)
	{
		return GetPlayerOperation() == nCountPlayer && (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_B, 0));
	}
	else
	{
		if (nCountPlayer == 0 && (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_B, 0)))
		{
			return true;
		}
		else if (nCountPlayer == 1 && (GetKeyboardTrigger(DIK_NUMPAD1) || GetJoypadTrigger(JOYKEY_B, 1)))
		{
			return true;
		}
	}
	
	return false;
}
