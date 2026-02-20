//=====================================================================
//
// 画面処理 [mode.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "mode.h"
#include "sound.h"
#include "input.h"
#include "fade.h"
#include "DebugProc.h"
// シーンファイルのヘッダ
#include "title.h"
#include "Game.h"
#include "result.h"
#include "Editor.h"
#include"Logo.h"
#include"Gameover.h"
//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
MODE g_currentMode = MODE_GAMEOVER;			// 現在のシーン
MODE g_previousMode = g_currentMode;		// 直前のシーン

//*********************************************************************
// 各シーンの処理関数
//*********************************************************************
MODEDATA g_modes[MODE_MAX] = {
	{ InitLogo, UninitLogo, UpdateLogo, DrawLogo },
	{ InitTitle, UninitTitle, UpdateTitle, DrawTitle },
	{ InitGame, UninitGame, UpdateGame, DrawGame },
	{ InitGameover, UninitGameover, UpdateGameover, DrawGameover },
	{ InitResult, UninitResult, UpdateResult, DrawResult },
	{ InitEditor, UninitEditor, UpdateEditor, DrawEditor },
};

//=====================================================================
// 初期化処理
//=====================================================================
void InitMode(void)
{
	// フェード＆シーンの初期化処理
	InitFade(g_currentMode);

	// デバッグ表示の初期化処理
	InitDebugProc();
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitMode(void)
{
	// 各シーンの終了処理
	for (int nCount = 0; nCount < MODE_MAX; nCount++)
	{
		g_modes[nCount].Uninit();
	}

	// フェードのの終了処理
	UninitFade();

	// デバッグ表示の初期化処理
	UninitDebugProc();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateMode(void)
{
	// 現在のシーンの更新処理
	g_modes[g_currentMode].Update();

	// フェードの更新処理
	UpdateFade();

	// デバッグ表示の更新処理
	UpdateDebugProc();
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawMode(void)
{
	// 現在のシーンの描画処理
	g_modes[g_currentMode].Draw();

	// フェードの描画処理
	DrawFade();

	// デバッグ表示の描画処理
	DrawDebugProc();
}

//=====================================================================
// シーン設定処理
//=====================================================================
MODE SetMode(MODE nextMode, bool bStopSound)
{
	// 直前のシーンを記録
	g_previousMode = g_currentMode;

	// 現在のシーンを終了
	g_modes[g_currentMode].Uninit();

	// 新規シーンを現在のシーンに設定
	g_currentMode = nextMode;

	// 新規シーンを初期化
	g_modes[nextMode].Init();

	return g_currentMode;
}

//=====================================================================
// 現在のシーンを取得する処理
//=====================================================================
MODE GetCurrentMode(void)
{
	return g_currentMode;
}

//=====================================================================
// 直前のシーンを取得する処理
//=====================================================================
MODE GetPreviousMode(void)
{
	return g_previousMode;
}
