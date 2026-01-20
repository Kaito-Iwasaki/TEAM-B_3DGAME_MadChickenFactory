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
#define MAX_LIGHT (256)			//照明の最大数
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
	//ライトの方向を設定
	SetLight(0, D3DXVECTOR3(0.2f, -0.8f, 0.4f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetSpotLight(1, D3DXVECTOR3(0.0f, 45.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));
	
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
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//ライトの情報
	D3DXVECTOR3 vecDir;

	//ライトの情報をクリアにする
	ZeroMemory(&g_aLight[Number], sizeof(D3DLIGHT9));

	//ライトの種類を設定
	g_aLight[Number].Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光を設定
	g_aLight[Number].Diffuse = col;
	/*g_aLight[Number].Specular = col;*/
	/*g_aLight[Number].Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);*/

	//ライトの方向を設定
	vecDir = vec;
	D3DXVec3Normalize(&vecDir, &vecDir);	//ベクトルを正規化する
	g_aLight[Number].Direction = vecDir;

	//ライトを設定する
	pDevice->SetLight(Number, &g_aLight[Number]);

	//ライトを有効にする
	pDevice->LightEnable(Number, TRUE);

}
//==================================================
//
//	スポットライトの設定
//
//==================================================
void SetSpotLight(int nIdx, D3DXVECTOR3 pos, D3DXCOLOR col)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//ライトの情報
	D3DXVECTOR3 vecDir;

	//ライトの情報をクリアにする
	ZeroMemory(&g_aLight[nIdx], sizeof(D3DLIGHT9));

	//ライトの種類
	g_aLight[nIdx].Type = D3DLIGHT_SPOT;

	//ライトの位置を設定
	g_aLight[nIdx].Position = pos;

	//ライトの色設定
	g_aLight[nIdx].Diffuse = col;
	g_aLight[nIdx].Specular = D3DXCOLOR(0.5f, 0.5f, 0.5f,1.0f);
	g_aLight[nIdx].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	//ライトのベクトル設定
	g_aLight[nIdx].Direction = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	//フォールオフ設定
	g_aLight[nIdx].Falloff = 1.0f;

	//定常減衰定数設定
	g_aLight[nIdx].Attenuation0 = 0.1f;
	g_aLight[nIdx].Attenuation1 = 0.0f;
	g_aLight[nIdx].Attenuation2 = 0.0f;

	//有効範囲設定
	g_aLight[nIdx].Range = 50.0f;

	//内側のコーンの角度設定
	g_aLight[nIdx].Theta = D3DX_PI;

	//外側のコーンの設定
	g_aLight[nIdx].Phi = D3DX_PI;

	//ライトを設定する
	pDevice->SetLight(nIdx, &g_aLight[nIdx]);

	//ライトを有効にする
	pDevice->LightEnable(nIdx, TRUE);
}