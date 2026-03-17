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
#include"field.h"
#include"wall.h"
#include"model.h"
#include"meshcylinder.h"
#include "Game.h"
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
MODELDATA g_modelDataResult;
//======================
//リザルトの初期化処理
//======================
void InitResult(void)
{
	InitResult_Logo();
	InitCamera();					//カメラ
	InitField();					//フィールド
	InitWall();						//壁
	InitModel();					//モデル
	InitMeshCylinder();				//メッシュシリンダー
	InitLight();					//ライト
	InitTimer();

	SetTimerCount(GAME_TIMER_COUNT - GetTimer());

	SetTimer(D3DXVECTOR3(500.0f, -330.0f, 0.0f), D3DXVECTOR2(400.0f, 250.0f));
	CAMERA* pCamera = GetCamera(0);

	// スクリプトの読み込み
	LoadScript("data\\model_Land.txt", &g_modelDataResult);
	// テクスチャの読み込み

	// モデルの読み込み・配置
	LoadAndSetModelFromData(&g_modelDataResult);
	for (int nCountTex = 0; nCountTex < MAX_LOADABLE_TEXTURE; nCountTex++)
	{
		LoadTexture(&g_modelDataResult.aFilenameTexture[nCountTex][0], nCountTex);
	}

	// フィールドの設定
	for (int nCountField = 0; nCountField < g_modelDataResult.nCountFieldSet; nCountField++)
	{
		FIELDSETDATA* pFieldData = &g_modelDataResult.aInfoFieldSet[nCountField];

		SetField(
			pFieldData->pos,
			D3DXVECTOR3(pFieldData->size.x * pFieldData->nBlockX, 0, pFieldData->size.z * pFieldData->nBlockZ),
			pFieldData->rot,
			pFieldData->nType
		);
	}

	// ウォールの設定
	for (int nCountWALL = 0; nCountWALL < g_modelDataResult.nCountWallSet; nCountWALL++)
	{
		WALLSETDATA* pWallData = &g_modelDataResult.aInfoWallSet[nCountWALL];

		SetWall(
			pWallData->nType,
			pWallData->pos,
			D3DXVECTOR3(pWallData->size.x * pWallData->nBlockX, pWallData->size.y * pWallData->nBlockY, 0),
			pWallData->rot
		);
	}	
	SetCameraPosVFromAngle(0);
	pCamera->mode = CAMERAMODE_NONE;

	pCamera->posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pCamera->posV = D3DXVECTOR3(0.0f, 50.0f, -1450.0f);

	PlaySound(SOUND_LABEL_BGM_RESULT);
}
//=======================
//リザルトの終了処理
//=======================
void UninitResult(void)
{
	
	UninitResult_Logo();
	UninitCamera();					//カメラ
	UninitField();					//フィールド
	UninitWall();					//壁
	UninitModel();					//モデル
	UninitMeshCylinder();			//メッシュシリンダー
	UninitLight();					//ライト
	UninitTimer();

	// テクスチャの解放
	ReleaseLoadedTexture();

	GetDevice()->SetRenderState(D3DRS_FOGENABLE, FALSE);

	// サウンドの停止
	StopSound();
	
}	
//==================
//リザルトの更新処理
//==================
void UpdateResult(void)
{
	int time = GetTimer();
	SetTimerCount(time);
	PrintDebugProc("%d\n",time);
	PrintDebugProc("リザルト画面\n");
	UpdateResult_Logo();
	CAMERA* pCamera = GetCamera(0);
	UpdateCamera();					//カメラ
	UpdateField();					//フィールド
	UpdateModel();					//モデル
	UpdateMeshCylinder();			//メッシュシリンダー
	UpdateLight();					//ライト

	
}
//===================
//リザルトの描画処理
//===================
void DrawResult(void)
{	
	// ゲームカメラの設定
	SetCamera(CAMERATYPE_GAME);

	DrawField();					//フィールド
	DrawWall();						//壁
	DrawModel();					//モデル
	DrawMeshCylinder();				//メッシュシリンダー
	DrawTimer();
	DrawResult_Logo();


}
