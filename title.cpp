//=====================================================================
//
// タイトル画面処理 [Title.h]
// Author 土田青空: 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include"camera.h"
#include "title.h"
#include "input.h"
#include"game.h"
#include"sound.h"
#include "fade.h"
#include"title_logo.h"
#include "field.h"
#include "light.h"
#include"wall.h"
#include"model.h"
#include"meshcylinder.h"
#include"team_logo.h"
#include"DebugProc.h"
//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_TITLE (3)				//タイトルの種類
//===============
//グローバル変数
//===============
//LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE] = {};		//テクスチャへのポインタ
//LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;	//頂点バッファのポインタ
TITLE g_aTitle[MAX_TITLE];
//===========
//初期化処理
//===========
void InitTitle(void)
{
	InitCamera();					//カメラ
	InitTitleLogo();				//タイトルロゴ
	InitField();					//フィールド
	InitWall();						//壁
	InitModel();					//モデル
	InitMeshCylinder();				//メッシュシリンダー
	InitLight();					//ライト
	InitTeamLogo();					//チームロゴ
	
}

//=======================
//タイトル画面の終了処理
//=======================
void UninitTitle(void)
{
	UninitCamera();					//カメラ
	UninitTitleLogo();				//タイトルロゴ
	UninitField();					//フィールド
	UninitWall();					//壁
	UninitModel();					//モデル
	UninitMeshCylinder();			//メッシュシリンダー
	UninitLight();					//ライト
	UninitTeamLogo();				//チームロゴ

}
//==================
//タイトル画面の更新
//==================
void UpdateTitle(void)
{
	PrintDebugProc("タイトル画面\n");
	UpdateCamera();					//カメラ
	UpdateTitleLogo();				//タイトルロゴ
	UpdateField();					//フィールド
	UpdateWall();					//壁
	UpdateModel();					//モデル
	UpdateMeshCylinder();			//メッシュシリンダー
	UpdateLight();					//ライト
	UpdateTeamLogo();				//チームロゴ

	if (GetKeyboardTrigger(DIK_RETURN) == true && g_aTitle->bEnter == false)
	{
		g_aTitle->bEnter = true;
		//ENTERが押された
		SetFade(MODE_GAME);
		//ゲーム画面に移行
		g_aTitle->bEnter = false;
	}
	if (GetJoypadTrigger(JOYKEY_START) == true && g_aTitle->bEnter == false)
	{
		g_aTitle->bEnter = true;
		//STARTが押された
		SetFade(MODE_GAME);
		//ゲーム画面に移行
		g_aTitle->bEnter = false;
	}
}
//=========================
//タイトル画面の描画処理
//=========================
void DrawTitle(void)
{
	// ゲームカメラの設定
	SetCamera(CAMERATYPE_GAME);

	DrawTitleLogo();				//タイトルロゴ
	DrawField();					//フィールド
	DrawWall();						//壁
	DrawModel();					//モデル
	DrawMeshCylinder();				//メッシュシリンダー
	DrawTeamLogo();					//チームロゴ
}
