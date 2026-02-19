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
#include"player.h"
#include"model_loader.h"
#include"texture.h"
#include"meshcylinder.h"
//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_TITLE (3)				//タイトルの種類
//===============
//グローバル変数
//===============
TITLE g_aTitle[MAX_TITLE];
MODELDATA g_modelDataTitle;
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
	CAMERA* pCamera = GetCamera(0);
	// スクリプトの読み込み
	LoadScript("data\\model_Land.txt", &g_modelDataTitle);
	// テクスチャの読み込み
	
	// モデルの読み込み・配置
	LoadAndSetModelFromData(&g_modelDataTitle);
	for (int nCountTex = 0; nCountTex < MAX_LOADABLE_TEXTURE; nCountTex++)
	{
		LoadTexture(&g_modelDataTitle.aFilenameTexture[nCountTex][0], nCountTex);
	}

	// フィールドの設定
	for (int nCountField = 0; nCountField < g_modelDataTitle.nCountFieldSet; nCountField++)
	{
		FIELDSETDATA* pFieldData = &g_modelDataTitle.aInfoFieldSet[nCountField];

		SetField(
			pFieldData->pos,
			D3DXVECTOR3(pFieldData->size.x * pFieldData->nBlockX, 0, pFieldData->size.z * pFieldData->nBlockZ),
			pFieldData->rot,
			pFieldData->nType
		);
	}

	// ウォールの設定
	for (int nCountWALL = 0; nCountWALL < g_modelDataTitle.nCountWallSet; nCountWALL++)
	{
		WALLSETDATA* pWallData = &g_modelDataTitle.aInfoWallSet[nCountWALL];

		SetWall(
			pWallData->nType,
			pWallData->pos,
			D3DXVECTOR3(pWallData->size.x * pWallData->nBlockX, pWallData->size.y * pWallData->nBlockY, 0),
			pWallData->rot
		);
	}
	// カメラの初期設定
	SetCameraPosVFromAngle(0);
	pCamera->mode = CAMERAMODE_NONE;

	pCamera->posR = D3DXVECTOR3(0.0f, 50.0f, 1100.0f);
	pCamera->posV = D3DXVECTOR3(200.0f, 20.0f, 900.0f);
	
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
	
	// テクスチャの解放
	ReleaseLoadedTexture();

	GetDevice()->SetRenderState(D3DRS_FOGENABLE, FALSE);
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
	UpdateModel();					//モデル
	UpdateMeshCylinder();			//メッシュシリンダー
	UpdateLight();					//ライト
	UpdateTeamLogo();				//チームロゴ

	
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
