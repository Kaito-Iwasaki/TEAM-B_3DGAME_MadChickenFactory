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
#include"particle.h"
#include"gate.h"
#include "box_movable.h"
#include "conveyer.h"
#include "lift.h"
#include "vignette.h"
#include "Enemy.h"
#include "SE_controller.h"
#include "team_logo.h"

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
void _DebugDisplay();

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
MODELDATA g_modelDataGame;
bool g_bLightGame = false;
bool g_bFogGame = false;

//=====================================================================
// 初期化処理
//=====================================================================
void InitGame(void)
{
	// 各オブジェクトの初期化処理
	InitCamera();			// カメラ
	InitShadow();			// 影
	InitPlayer();			// プレイヤー
	InitField();			// フィールド
	InitLight();			// ライト
	InitWall();				// 壁
	InitPause();			// ポーズ
	InitTimer();			// タイマー
	InitModel();			// モデル
	InitSaw();				// 回転ノコギリ
	InitEffect();			// エフェクト
	InitGoal();				// ゴール
	InitPress();			// プレス機
	InitPrompt();			// プロンプト
	InitFire();				// 火炎放射器
	InitParticle();			// パーティクル
	InitGate();				// ゲート
	InitMoveBox();			// 動かせる箱
	InitConveyer();			// コンベア
	InitLift();				// リフト
	InitVignette();			// ビネット
	InitEnemy();			// 敵
	InitSEController();		// サウンドコントローラー

	// フォグの初期設定
	float FogStart = 2000.0f, FogEnd = 4000.0f;
	GetDevice()->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR); // バーテックスフォグ
	GetDevice()->SetRenderState(D3DRS_FOGSTART, *((DWORD*)(&FogStart))); // フォグ開始点
	GetDevice()->SetRenderState(D3DRS_FOGEND, *((DWORD*)(&FogEnd))); // フォグ終了点

	// スクリプトの読み込み
	LoadScript("data\\model_stage.txt", &g_modelDataGame);

	// モデルの読み込み・配置
	LoadAndSetModelFromData(&g_modelDataGame);

	// テクスチャの読み込み
	for (int nCountTex = 0; nCountTex < MAX_LOADABLE_TEXTURE; nCountTex++)
	{
		LoadTexture(&g_modelDataGame.aFilenameTexture[nCountTex][0], nCountTex);
	}

	// フィールドの設定
	for (int nCountField = 0; nCountField < g_modelDataGame.nCountFieldSet; nCountField++)
	{
		FIELDSETDATA* pFieldData = &g_modelDataGame.aInfoFieldSet[nCountField];

		SetField(
			pFieldData->pos,
			D3DXVECTOR3(pFieldData->size.x * pFieldData->nBlockX, 0, pFieldData->size.z * pFieldData->nBlockZ),
			pFieldData->rot,
			pFieldData->nType
		);
	}

	// ウォールの設定
	for (int nCountWALL = 0; nCountWALL < g_modelDataGame.nCountWallSet; nCountWALL++)
	{
		WALLSETDATA* pWallData = &g_modelDataGame.aInfoWallSet[nCountWALL];

		SetWall(
			pWallData->nType,
			pWallData->pos,
			D3DXVECTOR3(pWallData->size.x * pWallData->nBlockX, pWallData->size.y * pWallData->nBlockY, 0),
			pWallData->rot
		);
	}
	for (int nCountTIMER = 0; nCountTIMER < g_modelDataGame.nCountTimerSet; nCountTIMER++)
	{
		TIMERSETDATA* pTimerData = &g_modelDataGame.aInfoTimerSet[nCountTIMER];

		SetTimer(
			pTimerData->pos,
			pTimerData->size
		);
	}
	// 回転ノコギリの設定
	for (int nCntSaw = 0; nCntSaw < g_modelDataGame.nCountSawSet; nCntSaw++)
	{
		SAWSETDATA* pSawData = &g_modelDataGame.aInfoSawSet[nCntSaw];

		SetSaw(pSawData->nIdx, pSawData->pos, pSawData->rot, pSawData->moveRange, pSawData->nTimeMove, pSawData->bStartup);
	}

	// プレス機の設定
	for (int nCntPress = 0; nCntPress < g_modelDataGame.nCountPressSet; nCntPress++)
	{
		PRESSSETDATA* pPressData = &g_modelDataGame.aInfoPressSet[nCntPress];

		SetPress(pPressData->nIdx, pPressData->pos, pPressData->rot,pPressData->width, pPressData->interval, pPressData->state);
	}

	// 火炎放射器の設定
	for (int nCntFire = 0; nCntFire < g_modelDataGame.nCountFireSet; nCntFire++)
	{
		FIRESETDATA* pFireData = &g_modelDataGame.aInfoFireSet[nCntFire];

		SetFlamethrower(pFireData->pos, pFireData->rot, pFireData->state, pFireData->firestate, pFireData->nIdx);
	}

	// プロンプトの設定
	for (int nCntPrompt = 0; nCntPrompt < g_modelDataGame.nCountPromptSet; nCntPrompt++)
	{
		PROMPTSETDATA* pPromptData = &g_modelDataGame.aInfoPromptSet[nCntPrompt];

		SetPrompt(pPromptData->pos, pPromptData->size, pPromptData->nIdx);
	}

	// ゴールの設定
	GOALSETDATA* pGoalData = &g_modelDataGame.InfoGoalSet;

	SetGoal(pGoalData->pos, pGoalData->rot);

	// コンベアの設定
	for (int nCntConveyer = 0; nCntConveyer < g_modelDataGame.nCountConveyerSet; nCntConveyer++)
	{
		CONVEYERSETDATA* pConveyerData = &g_modelDataGame.aInfoConveyerSet[nCntConveyer];

		SetConveyer(pConveyerData->nIdx, pConveyerData->pos, pConveyerData->rot, pConveyerData->Onmove, pConveyerData->Offmove, pConveyerData->size, pConveyerData->state);
	}

	// 動く箱の設定
	for (int nCntMoveBox = 0; nCntMoveBox < g_modelDataGame.nCountMoveBoxSet; nCntMoveBox++)
	{
		MOVEBOXSETDATA* pMoveBoxData = &g_modelDataGame.aInfoMoveBoxSet[nCntMoveBox];

		SetMoveBox(pMoveBoxData->pos, pMoveBoxData->rot, pMoveBoxData->range);
	}

	// ゲートの設定
	for (int nCntGate = 0; nCntGate < g_modelDataGame.nCountGateSet; nCntGate++)
	{
		GATESETDATA* pGateData = &g_modelDataGame.aInfoGateSet[nCntGate];

		SetGate(pGateData->pos, pGateData->rot, pGateData->nIdx, pGateData->movewidth, pGateData->Goup, pGateData->state);
	}

	// 敵の設定
	for (int nCntEnemy = 0; nCntEnemy < g_modelDataGame.nCountEnemySet; nCntEnemy++)
	{
		ENEMYSETDATA* pEnemyData = &g_modelDataGame.aInfoEnemySet[nCntEnemy];

		SetEnemy(pEnemyData->routine[0].pos, pEnemyData->routine[0].rot, pEnemyData->fSpeed, &pEnemyData->routine[0]);
	}

	// リフトの設定
	for (int nCountLift = 0; nCountLift < g_modelDataGame.nCountLiftSet; nCountLift++)
	{
		LIFTSETDATA* pLiftData = &g_modelDataGame.aInfoLiftSet[nCountLift];

		SetLift(pLiftData->nIdx, pLiftData->pos, pLiftData->rot, pLiftData->range, pLiftData->fSpeed);
	}

	// タイマー設定
	SetTimerCount(GAME_TIMER_COUNT);

	int pStaet = GetTitle();		// プレイ人数情報取得

	// カメラの初期設定
	SetCameraPosVFromAngle(0);
	if (pStaet == 0)
	{// 1Pプレイ

		GetCamera(0)->mode = CAMERAMODE_FREE;
	}
	else
	{// 2Pプレイ

		GetCamera(0)->mode = CAMERAMODE_SIDEVIEW2P;
	}
	//GetCamera(0)->mode = CAMERAMODE_FREE;

	g_bLightGame = true;
	g_bFogGame = true;
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
	UninitTimer();			// タイマー
	UninitModel();			// モデル
	UninitSaw();			// 回転ノコギリ
	UninitEffect();			// エフェクト
	UninitGoal();			// ゴール
	UninitPress();			// プレス機
	UninitPrompt();			// プロンプト
	UninitFire();			// 火炎放射器
	UninitGate();			// ゲート
	UninitMoveBox();		// 動かせる箱
	UninitConveyer();		// コンベア
	UninitLift();			// リフト
	UninitVignette();		// ビネット
	UninitEnemy();			// 敵

	// テクスチャの解放
	ReleaseLoadedTexture();

	GetDevice()->SetRenderState(D3DRS_FOGENABLE, FALSE);
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateGame(void)
{
	// 各オブジェクトの更新処理
	if (GetPause() == false)
	{//ポーズでない
		//ポーズ切り替え
		if (GetKeyboardTrigger(DIK_P) || GetJoypadTrigger(JOYKEY_START))
		{
			SwitchPause();
		}

		UpdateLight();
		UpdateCamera();			// カメラ
		UpdateShadow();			// 影
		UpdatePlayer();			// プレイヤー
		UpdateField();			// フィールド
		UpdateLight();			// ライト
		UpdateTimer();			// タイマー
		UpdateModel();			// モデル
		UpdateSaw();			// 回転ノコギリ
		UpdateEffect();			// エフェクト
		UpdateGoal();			// ゴール
		UpdatePress();			// プレス機
		UpdatePrompt();			// プロンプト
		UpdateFire();			// 火炎放射器
		UpdateParticle();		// パーティクル
		UpdateGate();			// ゲート
		UpdateMoveBox();		// 動かせる箱
		UpdateConveyer();		// コンベア
		UpdateLift();			// リフト
		UpdateVignette();		// ビネット
		UpdateEnemy();			// 敵
		UpdateSEController();	// サウンドコントローラー

		// デバッグ表示
		_DebugDisplay();
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
	bool bLastFogState = g_bFogGame;

	// ゲームカメラの設定
	SetCamera(CAMERATYPE_GAME);

	// 各オブジェクトの描画処理
	// [3D]

	// フォグを有効化
	GetDevice()->SetRenderState(D3DRS_FOGENABLE, g_bFogGame);

	DrawPlayer();			// プレイヤー
	DrawField();			// フィールド
	DrawWall();				// 壁
	DrawTimer();			// タイマー
	DrawModel();			// モデル
	DrawSaw();				// 回転ノコギリ
	DrawGoal();				// ゴール
	DrawPress();			// プレス機
	DrawFire();				// 火炎放射器
	DrawGate();				// ゲート
	DrawMoveBox();			// 動かせる箱
	DrawConveyer();			// コンベア
	DrawLift();				// リフト
	DrawEnemy();			// 敵
	DrawEffect();			// エフェクト
	DrawShadow();			// 影

	// フォグを無効化
	GetDevice()->SetRenderState(D3DRS_FOGENABLE, FALSE);

	// [2D]
	DrawPrompt();			// プロンプト
	DrawVignette();			// ビネット
	DrawPause();			// ポーズ

	// フォグを元の状態に戻す
	GetDevice()->SetRenderState(D3DRS_FOGENABLE, bLastFogState);
}

void ReloadGame(void)
{
	UninitField();			// フィールド
	UninitWall();			// 壁
	UninitModel();			// モデル
	UninitSaw();			// 回転ノコギリ
	UninitGoal();			// ゴール
	UninitPress();			// プレス機
	UninitPrompt();			// プロンプト
	UninitFire();			// 火炎放射器
	UninitMoveBox();		// 可動箱
	UninitConveyer();		// コンベア
	UninitLift();			// リフト
	UninitEnemy();			// 敵
	UninitShadow();			// 影
	UninitTimer();
	ReleaseLoadedTexture();

	InitField();			// フィールド
	InitWall();				// 壁
	InitModel();			// モデル
	InitSaw();				// 回転ノコギリ
	InitGoal();				// ゴール
	InitPress();			// プレス機
	InitPrompt();			// プロンプト
	InitFire();				// 火炎放射器
	InitMoveBox();			// 動かせる箱
	InitConveyer();			// コンベア
	InitLift();				// リフト
	InitEnemy();			// 敵
	InitTimer();
	InitShadow();			// 影

	// スクリプトの読み込み
	LoadScript("data\\model_stage.txt", &g_modelDataGame);

	// モデルの読み込み・配置
	LoadAndSetModelFromData(&g_modelDataGame);

	// テクスチャの読み込み
	for (int nCountTex = 0; nCountTex < MAX_LOADABLE_TEXTURE; nCountTex++)
	{
		LoadTexture(&g_modelDataGame.aFilenameTexture[nCountTex][0], nCountTex);
	}

	// フィールドの設定
	for (int nCountField = 0; nCountField < g_modelDataGame.nCountFieldSet; nCountField++)
	{
		FIELDSETDATA* pFieldData = &g_modelDataGame.aInfoFieldSet[nCountField];

		SetField(
			pFieldData->pos,
			D3DXVECTOR3(pFieldData->size.x * pFieldData->nBlockX, 0, pFieldData->size.z * pFieldData->nBlockZ),
			pFieldData->rot,
			pFieldData->nType
		);
	}
	for (int nCountTIMER = 0; nCountTIMER < g_modelDataGame.nCountTimerSet; nCountTIMER++)
	{
		TIMERSETDATA* pTimerData = &g_modelDataGame.aInfoTimerSet[nCountTIMER];

		SetTimer(
			pTimerData->pos,
			pTimerData->size
		);
	}
	// ウォールの設定
	for (int nCountWALL = 0; nCountWALL < g_modelDataGame.nCountWallSet; nCountWALL++)
	{
		WALLSETDATA* pWallData = &g_modelDataGame.aInfoWallSet[nCountWALL];

		SetWall(
			pWallData->nType,
			pWallData->pos,
			D3DXVECTOR3(pWallData->size.x * pWallData->nBlockX, pWallData->size.y * pWallData->nBlockY, 0),
			pWallData->rot
		);
	}

	// 回転ノコギリの設定
	for (int nCntSaw = 0; nCntSaw < g_modelDataGame.nCountSawSet; nCntSaw++)
	{
		SAWSETDATA* pSawData = &g_modelDataGame.aInfoSawSet[nCntSaw];

		SetSaw(pSawData->nIdx, pSawData->pos, pSawData->rot, pSawData->moveRange, pSawData->nTimeMove, pSawData->bStartup);
	}

	// プレス機の設定
	for (int nCntPress = 0; nCntPress < g_modelDataGame.nCountPressSet; nCntPress++)
	{
		PRESSSETDATA* pPressData = &g_modelDataGame.aInfoPressSet[nCntPress];

		SetPress(pPressData->nIdx, pPressData->pos, pPressData->rot, pPressData->width, pPressData->interval, pPressData->state);
	}

	// 火炎放射器の設定
	for (int nCntFire = 0; nCntFire < g_modelDataGame.nCountFireSet; nCntFire++)
	{
		FIRESETDATA* pFireData = &g_modelDataGame.aInfoFireSet[nCntFire];

		SetFlamethrower(pFireData->pos, pFireData->rot, pFireData->state, pFireData->firestate, pFireData->nIdx);
	}

	// プロンプトの設定
	for (int nCntPrompt = 0; nCntPrompt < g_modelDataGame.nCountPromptSet; nCntPrompt++)
	{
		PROMPTSETDATA* pPromptData = &g_modelDataGame.aInfoPromptSet[nCntPrompt];

		SetPrompt(pPromptData->pos, pPromptData->size, pPromptData->nIdx);
	}

	// ゴールの設定
	GOALSETDATA* pGoalData = &g_modelDataGame.InfoGoalSet;

	SetGoal(pGoalData->pos, pGoalData->rot);

	// コンベアの設定
	for (int nCntConveyer = 0; nCntConveyer < g_modelDataGame.nCountConveyerSet; nCntConveyer++)
	{
		CONVEYERSETDATA* pConveyerData = &g_modelDataGame.aInfoConveyerSet[nCntConveyer];

		SetConveyer(pConveyerData->nIdx, pConveyerData->pos, pConveyerData->rot, pConveyerData->Onmove, pConveyerData->Offmove, pConveyerData->size, pConveyerData->state);
	}

	// 動く箱の設定
	for (int nCntMoveBox = 0; nCntMoveBox < g_modelDataGame.nCountMoveBoxSet; nCntMoveBox++)
	{
		MOVEBOXSETDATA* pMoveBoxData = &g_modelDataGame.aInfoMoveBoxSet[nCntMoveBox];

		SetMoveBox(pMoveBoxData->pos, pMoveBoxData->rot, pMoveBoxData->range);
	}

	// ゲート
	for (int nCntGate = 0; nCntGate < g_modelDataGame.nCountGateSet; nCntGate++)
	{
		GATESETDATA* pGateData = &g_modelDataGame.aInfoGateSet[nCntGate];

		SetGate(pGateData->pos, pGateData->rot, pGateData->nIdx, pGateData->movewidth, pGateData->Goup, pGateData->state);
	}

	// 敵の設定
	for (int nCntEnemy = 0; nCntEnemy < g_modelDataGame.nCountEnemySet; nCntEnemy++)
	{
		ENEMYSETDATA* pEnemyData = &g_modelDataGame.aInfoEnemySet[nCntEnemy];

		SetEnemy(pEnemyData->routine[0].pos, pEnemyData->routine[0].rot, pEnemyData->fSpeed, &pEnemyData->routine[0]);
	}

	// リフトの設定
	for (int nCountLift = 0; nCountLift < g_modelDataGame.nCountLiftSet; nCountLift++)
	{
		LIFTSETDATA* pLiftData = &g_modelDataGame.aInfoLiftSet[nCountLift];

		SetLift(pLiftData->nIdx, pLiftData->pos, pLiftData->rot, pLiftData->range, pLiftData->fSpeed);
	}
}

void _DebugDisplay(void)
{
#ifdef  _DEBUG
	// デバッグ表示
	Player* pPlayer = GetPlayer();

	PrintDebugProc("ゲーム画面\n");
	PrintDebugProc("ライト切り替え:[F1]\n");
	if (GetKeyboardTrigger(DIK_F1))
	{
		g_bLightGame = !g_bLightGame;
		GetDevice()->SetRenderState(D3DRS_LIGHTING, (DWORD)g_bLightGame);
	}

	PrintDebugProc("フォグ切り替え:[F2]\n");
	if (GetKeyboardTrigger(DIK_F2))
	{
		g_bFogGame = !g_bFogGame;
	}

	PrintDebugProc("位置[0]：%f, %f, %f\n", pPlayer[0].pos.x, pPlayer[0].pos.y, pPlayer[0].pos.z);
	PrintDebugProc("位置[1]：%f, %f, %f\n", pPlayer[1].pos.x, pPlayer[1].pos.y, pPlayer[1].pos.z);
#endif //  _DEBUG
}