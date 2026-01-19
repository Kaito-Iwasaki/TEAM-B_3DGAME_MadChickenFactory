//==================================================
//
//	照明[light.cpp]
// Author : Keitaro Nagate
//
//==================================================
//==================================================
//
//	ファイルインクルード
//
//==================================================
#include "light.h"

//==================================================
//
//	マクロ定義
//
//==================================================
#define MAX_LIGHT (4)			//照明の最大数
#define LIGHT_POS (600.0f)		//照明の位置

//==================================================
//
//	グローバル変数宣言
//
//==================================================
D3DLIGHT9 g_aLight[MAX_LIGHT];	//ライトの情報

//==================================================
//
//	ライトの初期化処理
//
//==================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//ライトの情報
	D3DXVECTOR3 vecDir;

	for (int nCntlight = 0; nCntlight < MAX_LIGHT; nCntlight++)
	{
		//ライトの情報をクリアにする
		ZeroMemory(&g_aLight[nCntlight], sizeof(D3DLIGHT9));

		//ライトの種類を設定
		g_aLight[nCntlight].Type = D3DLIGHT_DIRECTIONAL;

		//ライトの拡散光を設定
		g_aLight[nCntlight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//ライトのベクトルを設定
		g_aLight[nCntlight].Direction = vecDir;

		//ライトの方向を設定
		SetLight(0, D3DXVECTOR3(0.2f, -0.8f, -0.4f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

		//ライトを設定する
		pDevice->SetLight(nCntlight, &g_aLight[nCntlight]);

		//ライトを有効にする
		pDevice->LightEnable(nCntlight, TRUE);
	}
	
}
//==================================================
//
//	ライトの終了処理
//
//==================================================
void UninitLight(void)
{

}
//==================================================
//
//	ライトの更新処理
//
//==================================================
void UpdateLight(void)
{

}
//==================================================
//
//	ライトのベクトル設定処理
//
//==================================================
void SetLight(int Number, D3DXVECTOR3 vec, D3DXCOLOR col)
{
	D3DXVECTOR3 vecDir;

	//ライトの方向を設定
	vecDir = vec;
	D3DXVec3Normalize(&vecDir, &vecDir);	//ベクトルを正規化する

	//ライトの色を設定
	g_aLight[Number].Diffuse = col;

	//ライトの方向を設定
	g_aLight[Number].Direction = vecDir;
}