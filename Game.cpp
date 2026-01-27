//=====================================================================
//
// Game [Game.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "Game.h"
#include "input.h"
#include "mode.h"
#include "fade.h"
#include "camera.h"
#include "DebugProc.h"
#include "player.h"
#include "field.h"
#include "light.h"
#include "shadow.h"
#include "title_logo.h"
#include "wall.h"
#include "Pause.h"
#include "meshcylinder.h"
#include "Timer.h"
#include "model.h"
#include "model_loader.h"
#include "saw.h"
#include"effect.h"
#include"goal.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************


//=====================================================================
// 初期化処理
//=====================================================================
void InitGame(void)
{
	MODELDATA modelData;

	// 各オブジェクトの初期化処理
	InitCamera();			// カメラ
	InitShadow();			// 影
	InitPlayer();			// プレイヤー
	InitField();			// フィールド
	InitLight();			// ライト
	InitWall();				// 壁
	InitPause();			// ポーズ
	InitMeshCylinder();		// メッシュシリンダー
	InitTimer();			// タイマー
	InitModel();
	InitSaw();				//回転ノコギリ
	InitEffect();			//エフェクト
	InitGoal();				// ゴール

	LoadScript("data\\model.txt", &modelData);

	for (int i = 0; i < modelData.nNumModel; i++)
	{
		LoadModel(&modelData.aFilenameModel[i][0], i);
	}

	for (int i = 0; i < modelData.nCountModelSet; i++)
	{
		MODELSETDATA setData = modelData.aInfoModelSet[i];

		SetModel(
			setData.nType,
			setData.pos,
			setData.rot
		);
	}

	SetCameraPosVFromAngle(0);
	GetCamera(0)->mode = CAMERAMODE_SIDEVIEW2P;
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitGame(void)
{
	// 各オブジェクトの終了処理
	UninitCamera();			// カメラ
	UninitShadow();			// 影
	UninitPlayer();			// プレイヤー
	UninitField();			// フィールド
	UninitLight();			// ライト
	UninitWall();			// 壁
	UninitPause();			// ポーズ
	UninitMeshCylinder();	// メッシュシリンダー
	UninitTimer();			// タイマー
	UninitModel();
	UninitSaw();			//回転ノコギリ
	UninitEffect();			//エフェクト
	UninitGoal();			// ゴール
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateGame(void)
{
	// デバッグ表示
	PrintDebugProc("ゲーム画面\n");

	// 各オブジェクトの更新処理
	if (GetPause() == false)
	{//ポーズでない
		//ポーズ切り替え
		if (GetKeyboardTrigger(DIK_P) || GetJoypadTrigger(JOYKEY_START))
		{
			SwitchPause();
		}

		UpdateCamera();			// カメラ
		UpdateShadow();			// 影
		UpdatePlayer();			// プレイヤー
		UpdateField();			// フィールド
		UpdateLight();			// ライト
		UpdateMeshCylinder();	// メッシュシリンダー
		UpdateTimer();			// タイマー
		UpdateModel();
		UpdateSaw();			//回転ノコギリ
		UpdateEffect();			//エフェクト
		UpdateGoal();			// ゴール
	}
	else
	{//ポーズ中
		UpdatePause();		//ポーズメニュー
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawGame(void)
{
	// ゲームカメラの設定
	SetCamera(CAMERATYPE_GAME);

	// 各オブジェクトの描画処理
	DrawPlayer();			// プレイヤー
	DrawField();			// フィールド
	DrawShadow();			// 影
	DrawWall();				// 壁
	DrawPause();			// ポーズ
	DrawMeshCylinder();		// メッシュシリンダー
	DrawTimer();			// タイマー
	DrawModel();
	DrawSaw();				//回転ノコギリ
	DrawEffect();			//エフェクト
	DrawGoal();				// ゴール
}