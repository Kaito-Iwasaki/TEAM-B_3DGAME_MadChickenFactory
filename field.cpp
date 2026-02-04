//================================
// 
//  フィールド処理　[field.cpp]
//  Author shuuhei Ida
//		  : Keitaro Nagate
//
//================================
//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "input.h"
#include "field.h"
#include "texture.h"
#include "player.h"
#include "util.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define FIELD_TXT_PASS "data\\TEXTURE\\field000.jpg"
#define FIELD_TEXTURE_SIZE_X (300.0f)
#define FIELD_TEXTURE_SIZE_Y (300.0f)

//*********************************************************************
// 
// ***** グローバル変数宣言 *****
// 
//*********************************************************************
LPDIRECT3DTEXTURE9 g_pTextureField = NULL;							// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL;						// 頂点バッファへのポインタ
Field g_aField[MAX_FIELD];

//========================
// フィールドの初期化処理
//========================
void InitField(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_FIELD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffField,
		NULL);

	VERTEX_3D* pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCountField = 0; nCountField < MAX_FIELD; nCountField++)
	{

		g_aField[nCountField].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置初期化
		g_aField[nCountField].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き初期化
		g_aField[nCountField].speed = D3DXVECTOR3_ZERO;					// 移動量初期化
		g_aField[nCountField].range = D3DXVECTOR3_ZERO;
		g_aField[nCountField].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 大きさ初期化
		g_aField[nCountField].nTxtype = -1;								// 種類初期化
		g_aField[nCountField].bUse = false;								// 使用していない状態にする

		// 頂点座標の設定(x,y,z,の順番になる、zの値は2Dの場合は必ず0にする)
		pVtx[0].pos = D3DXVECTOR3(-g_aField[nCountField].size.x / 2, 0, +g_aField[nCountField].size.z / 2);
		pVtx[1].pos = D3DXVECTOR3(+g_aField[nCountField].size.x / 2, 0, +g_aField[nCountField].size.z / 2);
		pVtx[2].pos = D3DXVECTOR3(-g_aField[nCountField].size.x / 2, 0, -g_aField[nCountField].size.z / 2);
		pVtx[3].pos = D3DXVECTOR3(+g_aField[nCountField].size.x / 2, 0, -g_aField[nCountField].size.z / 2);

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
	SetField(D3DXVECTOR3_ZERO, D3DXVECTOR3(200.0f, 0.0f, 200.0f), D3DXVECTOR3_ZERO,
		2, D3DXVECTOR3(2.0f, 2.0f, 0.0f), D3DXVECTOR3(200.0f, 200.0f, 200.0f));

	// 頂点バッファをアンロックする
	g_pVtxBuffField->Unlock();
}

//======================
// フィールドの終了処理
//======================
void UninitField(void)
{
	// テクスチャの破棄
	if (g_pTextureField != NULL)
	{
		g_pTextureField->Release();
		g_pTextureField = NULL;
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffField != NULL)
	{
		g_pVtxBuffField->Release();

		g_pVtxBuffField = NULL;
	}
}

//======================
// フィールドの更新処理
//======================
void UpdateField(void)
{
	VERTEX_3D* pVtx;		// 頂点情報へのポインタ

	float fTexsizeX;
	float fTexsizeY;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (g_aField[nCntField].bUse == true)
		{
			g_aField[nCntField].pos += g_aField[nCntField].speed;

			if (g_aField[nCntField].pos.x <= g_aField[nCntField].poslocal.x - g_aField[nCntField].range.x ||
				g_aField[nCntField].pos.x >= g_aField[nCntField].poslocal.x + g_aField[nCntField].range.x)
			{//移動して、範囲外へ出たら
				g_aField[nCntField].speed.x *= -1;
			}

			if (g_aField[nCntField].pos.y <= g_aField[nCntField].poslocal.y - g_aField[nCntField].range.y ||
				g_aField[nCntField].pos.y >= g_aField[nCntField].poslocal.y + g_aField[nCntField].range.y)
			{//移動して、範囲外へ出たら
				g_aField[nCntField].speed.y *= -1;
			}

			if (g_aField[nCntField].pos.z <= g_aField[nCntField].poslocal.z - g_aField[nCntField].range.z ||
				g_aField[nCntField].pos.z >= g_aField[nCntField].poslocal.z + g_aField[nCntField].range.z)
			{//移動して、範囲外へ出たら
				g_aField[nCntField].speed.z *= -1;
			}
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffField->Unlock();

}

//======================
// フィールドの描画処理
//======================
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();				// デバイスへのポインタ
	LPDIRECT3DTEXTURE9 *pTexture = GetLoadedTexture();		// テクスチャ情報読み取り
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCountField = 0; nCountField < MAX_FIELD; nCountField++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aField[nCountField].mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aField[nCountField].rot.y, g_aField[nCountField].rot.x, g_aField[nCountField].rot.z);

		D3DXMatrixMultiply(&g_aField[nCountField].mtxWorld, &g_aField[nCountField].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aField[nCountField].pos.x, g_aField[nCountField].pos.y, g_aField[nCountField].pos.z);

		D3DXMatrixMultiply(&g_aField[nCountField].mtxWorld, &g_aField[nCountField].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aField[nCountField].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, pTexture[g_aField->nTxtype]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCountField * 4,
			2);
	}

}

//===================
// フィールドの設置
//===================
void SetField(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot,int ntxtype, D3DXVECTOR3 speed, D3DXVECTOR3 range)
{
	VERTEX_3D* pVtx;		// 頂点情報へのポインタ

	float fTexsizeX;
	float fTexsizeY;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCountField = 0; nCountField < MAX_FIELD; nCountField++)
	{
		if (g_aField[nCountField].bUse == false)
		{
			g_aField[nCountField].pos = pos;			// 位置設定
			g_aField[nCountField].poslocal = pos;		// 位置保存
			g_aField[nCountField].size = size;			// 大きさ設定
			g_aField[nCountField].rot = rot;			// 向き設定
			g_aField[nCountField].nTxtype = ntxtype;	// テクスチャ設定
			g_aField[nCountField].speed = speed;		// 移動量設定
			g_aField[nCountField].range = range;		// 可動域
			g_aField[nCountField].bUse = true;			// 使用している状態にする

			// 頂点座標の設定(x,y,z,の順番になる、zの値は2Dの場合は必ず0にする)
			pVtx[0].pos = D3DXVECTOR3(-g_aField[nCountField].size.x / 2, 0, +g_aField[nCountField].size.z / 2);
			pVtx[1].pos = D3DXVECTOR3(+g_aField[nCountField].size.x / 2, 0, +g_aField[nCountField].size.z / 2);
			pVtx[2].pos = D3DXVECTOR3(-g_aField[nCountField].size.x / 2, 0, -g_aField[nCountField].size.z / 2);
			pVtx[3].pos = D3DXVECTOR3(+g_aField[nCountField].size.x / 2, 0, -g_aField[nCountField].size.z / 2);

			fTexsizeX = g_aField[nCountField].size.x / FIELD_TEXTURE_SIZE_X;
			fTexsizeY = g_aField[nCountField].size.z / FIELD_TEXTURE_SIZE_Y;

			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(fTexsizeX * 1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, fTexsizeY * 1.0f);
			pVtx[3].tex = D3DXVECTOR2(fTexsizeX * 1.0f, fTexsizeY * 1.0f);

			break;
		}

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffField->Unlock();

}
//*********************************************************************
// 
// ***** 当たり判定 *****
// 
//*********************************************************************
bool CollisionField(D3DXVECTOR3 *pos, D3DXVECTOR3 posold)
{
	bool bGround = false;

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (g_aField[nCntField].bUse == true)
		{
			if (pos->x >= g_aField[nCntField].pos.x - g_aField[nCntField].size.x &&
				pos->x <= g_aField[nCntField].pos.x + g_aField[nCntField].size.x &&
				pos->z >= g_aField[nCntField].pos.z - g_aField[nCntField].size.z &&
				pos->z <= g_aField[nCntField].pos.z + g_aField[nCntField].size.z &&
				pos->y <= g_aField[nCntField].pos.y)
			{
				if (posold.y >= g_aField[nCntField].pos.y)
				{
					pos->y = g_aField[nCntField].pos.y;
					bGround = true;
				}
			}

			if (bGround == true)
			{
				*pos += g_aField[nCntField].speed;
			}

		}
	}

	return bGround;
}