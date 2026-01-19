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