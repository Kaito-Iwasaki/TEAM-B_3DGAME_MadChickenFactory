//=======================================================
//
// フィールド処理[field.cpp]
// Author : Airi Takahashi
//
//=======================================================
#include"field.h"
#include"input.h"

// マクロ定義
#define MAX_FIELD			(64)			// 床の最大数
#define MAX_TEXTURE			(1)				// テクスチャ数
#define MAX_PATTERN			(1)				// 全パターン
#define MAX_PATTERN_X		(1)				// X軸パターン数
#define MAX_PATTERN_Y		(1)				// Y軸パターン数
#define FIELD_WIDTH			(300.0f)		// 幅
#define FIELD_DEPTH			(300.0f)		// 奥行

// グローバル変数
LPDIRECT3DTEXTURE9	g_pTextureField[MAX_TEXTURE] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL;				// 頂点バッファへのポインタ
Field g_aFiled[MAX_FIELD];									// フィールド情報
const char* c_apFilenameField[MAX_TEXTURE] =				// テクスチャ読み込み
{
	"data\\TEXTURE\\bg001.jpg",
};

//=======================================================
// フィールドの初期化処理
//=======================================================
void InitField(void)
{
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ
	VERTEX_3D* pVtx;				// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	for (int nCntField = 0; nCntField < MAX_TEXTURE; nCntField++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
									c_apFilenameField[nCntField],
									&g_pTextureField[nCntField]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_FIELD,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffField,
								NULL);

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		g_aFiled[nCntField].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置初期化
		g_aFiled[nCntField].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 角度初期化
		g_aFiled[nCntField].fWidth = 0.0f;								// 幅初期化
		g_aFiled[nCntField].fDepth = 0.0f;								// 奥行初期化
		g_aFiled[nCntField].bUse = false;								// 使用していない状態にする
	}


	// 頂点バッファをロックし、頂点情報へのポインタ取得
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aFiled[nCntField].fWidth / 2.0f, 0.0f, g_aFiled[nCntField].fDepth / 2.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aFiled[nCntField].fWidth / 2.0f, 0.0f, g_aFiled[nCntField].fDepth / 2.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aFiled[nCntField].fWidth / 2.0f, 0.0f, -g_aFiled[nCntField].fDepth / 2.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aFiled[nCntField].fWidth / 2.0f, 0.0f, -g_aFiled[nCntField].fDepth / 2.0f);

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

		// テクスチャ座標の指定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;		// 頂点データのポインタを4つ進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffField->Unlock();

	SetField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 300.0f, 300.0f);
}

//=======================================================
// フィールドの終了処理
//=======================================================
void UninitField(void)
{
	for (int nCntField = 0; nCntField < MAX_TEXTURE; nCntField++)
	{
		// テクスチャの破棄
		if (g_pTextureField[nCntField] != NULL)
		{
			g_pTextureField[nCntField]->Release();
			g_pTextureField[nCntField] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffField != NULL)
	{
		g_pVtxBuffField->Release();
		g_pVtxBuffField = NULL;
	}
}

//=======================================================
// フィールドの更新処理
//=======================================================
void UpdateField(void)
{
	//if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true || GetJoypadLeftStick(JOYKEY_LSTICKUP) == true)
	//{// Wキー入力(Z軸+の方向に移動)

	//	g_posField.z += 1.0f;
	//}
	//else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true || GetJoypadLeftStick(JOYKEY_LSTICKDOWN) == true)
	//{/// Sキー入力(Z軸-の方向に移動)

	//	g_posField.z += -1.0f;
	//}

	//if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true || GetJoypadLeftStick(JOYKEY_LSTICKLEFT) == true)
	//{// Aキー入力(X軸-の方向に移動)

	//	g_posField.x += -1.0f;
	//}
	//else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true || GetJoypadLeftStick(JOYKEY_LSTICKRIGHT) == true)
	//{// Dキー入力(X軸+の方向に移動)

	//	g_posField.x += 1.0f;
	//}

	//if (GetKeyboardPress(DIK_UP) == true)
	//{// ↑キー入力(X軸+の方向に回転)

	//	g_rotField.x += 0.05f;
	//}
	//else if (GetKeyboardPress(DIK_DOWN) == true)
	//{// ↑キー入力(X軸-の方向に回転)

	//	g_rotField.x += -0.05f;
	//}

	//if (GetKeyboardPress(DIK_RIGHT) == true)
	//{// ↑キー入力(Z軸-の方向に回転)

	//	g_rotField.z += -0.05f;
	//}
	//else if (GetKeyboardPress(DIK_LEFT) == true)
	//{// ↑キー入力(Z軸+の方向に回転)

	//	g_rotField.z += 0.05f;
	//}

	//if (GetKeyboardPress(DIK_RSHIFT) == true)
	//{// ↑キー入力(Y軸+の方向に回転)

	//	g_rotField.y += 0.05f;
	//}
	//else if (GetKeyboardPress(DIK_LSHIFT) == true)
	//{// ↑キー入力(Y軸-の方向に回転)

	//	g_rotField.y += -0.05f;
	//}

}

//=======================================================
// フィールドの描画処理
//=======================================================
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureField[0]);

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (g_aFiled[nCntField].bUse == true)
		{// 使用している

				// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aFiled[nCntField].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_aFiled[nCntField].rot.y, g_aFiled[nCntField].rot.x, g_aFiled[nCntField].rot.z);

			D3DXMatrixMultiply(&g_aFiled[nCntField].mtxWorld, &g_aFiled[nCntField].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_aFiled[nCntField].pos.x, g_aFiled[nCntField].pos.y, g_aFiled[nCntField].pos.z);

			D3DXMatrixMultiply(&g_aFiled[nCntField].mtxWorld, &g_aFiled[nCntField].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aFiled[nCntField].mtxWorld);

			// フィールドの描写
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}
}

//=======================================================
// フィールドの設定処理
//=======================================================
void SetField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fDepth)
{
	VERTEX_3D* pVtx;				// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタ取得
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (g_aFiled[nCntField].bUse == false)
		{// 壁の設置

			g_aFiled[nCntField].pos = pos;			// 位置
			g_aFiled[nCntField].rot = rot;			// 向き
			g_aFiled[nCntField].fWidth = fWidth;		// 幅
			g_aFiled[nCntField].fDepth = fDepth;		// 奥行
			g_aFiled[nCntField].bUse = true;			// 使用している状態にする

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aFiled[nCntField].fWidth / 2.0f, 0.0f, g_aFiled[nCntField].fDepth / 2.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aFiled[nCntField].fWidth / 2.0f, 0.0f, g_aFiled[nCntField].fDepth / 2.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aFiled[nCntField].fWidth / 2.0f, 0.0f, -g_aFiled[nCntField].fDepth / 2.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aFiled[nCntField].fWidth / 2.0f, 0.0f, -g_aFiled[nCntField].fDepth / 2.0f);

			break;
		}

		pVtx += 4;		// 頂点データのポインタを4つ進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffField->Unlock();
}