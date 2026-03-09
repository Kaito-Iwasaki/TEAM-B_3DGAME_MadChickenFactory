//=====================================================================
//
// リザルト画面処理 [Result.h]
// Author 土田青空: 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "result.h"
#include"result_logo.h"
#include "input.h"
#include"camera.h"
#include "sound.h"
#include "fade.h"
#include"DebugProc.h"
#include"Timer.h"
#include"light.h"
#include"texture.h"
//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************

//======================
//リザルトの初期化処理
//======================
void InitResult(void)
{
	InitResult_Logo();
	InitCamera();					//カメラ
	InitLight();					//ライト
	InitTimer();
	SetTimer(D3DXVECTOR3(500.0f, -500.0f, 0.0f), D3DXVECTOR2(200.0f, 200.0f));

}
//=======================
//リザルトの終了処理
//=======================
void UninitResult(void)
{
	// サウンドの停止
	StopSound();
	UninitResult_Logo();
	UninitCamera();					//カメラ
	UninitLight();					//ライト
	UninitTimer();
	int time = GetTimer();
	// テクスチャの解放
	ReleaseLoadedTexture();

	GetDevice()->SetRenderState(D3DRS_FOGENABLE, FALSE);
	
}	
//==================
//リザルトの更新処理
//==================
void UpdateResult(void)
{

	PrintDebugProc("リザルト画面\n");
	UpdateResult_Logo();
	CAMERA* pCamera = GetCamera(0);

	PrintDebugProc("タイトル画面\n");
	UpdateCamera();					//カメラ
	UpdateLight();					//ライト
	UpdateTimer();
	
}
//===================
//リザルトの描画処理
//===================
void DrawResult(void)
{
	DrawResult_Logo();
	DrawTimer();
	// ゲームカメラの設定
	SetCamera(CAMERATYPE_GAME);

}
