//=====================================================================
//
// Camera [Camera.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "Camera.h"
#include "input.h"
#include "DebugProc.h"
#include "util.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** 構造体 *****
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
CAMERA g_camera[CAMERATYPE_MAX];

//=====================================================================
// 初期化処理
//=====================================================================
void InitCamera(void)
{
	ZeroMemory(&g_camera[0], sizeof(g_camera));
	g_camera[CAMERATYPE_GAME].posV = D3DXVECTOR3(0, 500, -400);
	g_camera[CAMERATYPE_GAME].posR = D3DXVECTOR3(0, 0, 0);
	g_camera[CAMERATYPE_GAME].vecU = D3DXVECTOR3(0, 1, 0);
	g_camera[CAMERATYPE_GAME].rot = D3DXVECTOR3(D3DXToRadian(-55), D3DXToRadian(0), 0);
	g_camera[CAMERATYPE_GAME].fDistance = 100.0f;
	g_camera[CAMERATYPE_GAME].viewport.X = 0;
	g_camera[CAMERATYPE_GAME].viewport.Y = 0;
	g_camera[CAMERATYPE_GAME].viewport.Width = SCREEN_WIDTH ;
	g_camera[CAMERATYPE_GAME].viewport.Height = SCREEN_HEIGHT;
	g_camera[CAMERATYPE_GAME].viewport.MinZ = 0.0f;
	g_camera[CAMERATYPE_GAME].viewport.MaxZ = 1.0f;
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitCamera(void)
{

}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateCamera(void)
{
	CAMERA* pCamera = &g_camera[0];

	// カメラ位置の設定
	pCamera->posR.x = pCamera->posV.x + sinf(pCamera->rot.y) * cosf(pCamera->rot.x) * pCamera->fDistance;
	pCamera->posR.y = pCamera->posV.y + sinf(pCamera->rot.x) * pCamera->fDistance;
	pCamera->posR.z = pCamera->posV.z + cosf(pCamera->rot.y) * cosf(pCamera->rot.x) * pCamera->fDistance;
}

//=====================================================================
// カメラ設定処理
//=====================================================================
void SetCamera(int nIdx)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CAMERA* pCamera = &g_camera[nIdx];

	// ビューポートを設定
	pDevice->SetViewport(&pCamera->viewport);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&pCamera->mtxProjection);

	// プロジェクションマトリックスを作成（透視投影行列）
	D3DXMatrixPerspectiveFovLH(
		&pCamera->mtxProjection,
		D3DXToRadian(45.0f),							// 視野角
		(float)pCamera->viewport.Width / (float)pCamera->viewport.Height,		// アスペクト比
		10.0f,											// 最小Z値
		5000.0f											// 最大Z値
	);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &pCamera->mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&pCamera->mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(
		&pCamera->mtxView,
		&pCamera->posV,
		&pCamera->posR,
		&pCamera->vecU
	);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &pCamera->mtxView);
}

//=====================================================================
// カメラ処理
//=====================================================================
CAMERA* GetCamera(int nIdx)
{
	return &g_camera[nIdx];
}