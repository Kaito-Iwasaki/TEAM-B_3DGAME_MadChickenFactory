//=====================================================================
//
// ゲーム画面 [Game.cpp]
// Author : Kaito Iwasaki
//			Shuhei Ida
//			Airi Takahashi
//			Sora Tutida
//			Ketaro Nagate
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
#include "texture.h"
#include "press.h"
#include "prompt.h"
#include "fire.h"

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
	InitModel();			// モデル
	InitSaw();				// 回転ノコギリ
	InitEffect();			// エフェクト
	InitGoal();				// ゴール
	InitPress();			// プレス機
	InitPrompt();			// プロンプト
	InitFire();				// 火炎放射器

	// スクリプトの読み込み
	LoadScript("data\\model_factory.txt", &modelData);

	// モデルの読み込み・配置
	LoadAndSetModelFromData(&modelData);

	// テクスチャの読み込み
	for (int nCountTex = 0; nCountTex < MAX_LOADABLE_TEXTURE; nCountTex++)
	{
		LoadTexture(&modelData.aFilenameTexture[nCountTex][0], nCountTex);
	}

	// フィールドの設定
	for (int nCountField = 0; nCountField < modelData.nCountFieldSet; nCountField++)
	{
		FIELDSETDATA* pFieldData = &modelData.aInfoFieldSet[nCountField];

		SetField(
			pFieldData->pos,
			D3DXVECTOR3(pFieldData->size.x * pFieldData->nBlockX, 0, pFieldData->size.z * pFieldData->nBlockZ),
			pFieldData->rot,
			pFieldData->nType
		);
	}

	// ウォールの設定
	for (int nCountWALL = 0; nCountWALL < modelData.nCountWallSet; nCountWALL++)
	{
		WALLSETDATA* pWallData = &modelData.aInfoWallSet[nCountWALL];

		SetWall(
			pWallData->nType,
			pWallData->pos,
			D3DXVECTOR3(pWallData->size.x * pWallData->nBlockX, pWallData->size.y * pWallData->nBlockY, 0),
			pWallData->rot
		);
	}

	// カメラの初期設定
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
	UninitModel();			// モデル
	UninitSaw();			// 回転ノコギリ
	UninitEffect();			// エフェクト
	UninitGoal();			// ゴール
	UninitPress();			// プレス機
	UninitPrompt();			// プロンプト
	UninitFire();			// 火炎放射器

	// テクスチャの解放
	ReleaseLoadedTexture();
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
		UpdateModel();			// モデル
		UpdateSaw();			// 回転ノコギリ
		UpdateEffect();			// エフェクト
		UpdateGoal();			// ゴール
		UpdatePress();			// プレス機
		UpdatePrompt();			// プロンプト
		UpdateFire();			// 火炎放射器
	}
	else
	{//ポーズ中
		UpdatePause();			// ポーズメニュー
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
	DrawModel();			// モデル
	DrawSaw();				// 回転ノコギリ
	DrawEffect();			// エフェクト
	DrawGoal();				// ゴール
	DrawPress();			// プレス機
	DrawPrompt();			// プロンプト
	DrawFire();				// 火炎放射器
}