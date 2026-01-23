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
#include "camera.h"
#include "input.h"
#include "DebugProc.h"
#include "util.h"
#include "player.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define CAMERA_VECTOR_UP		D3DXVECTOR3(0, 1, 0)						// 上向きベクトル
#define INIT_CAMERA_ROT			D3DXVECTOR3(D3DXToRadian(-25), 0, 0)		// カメラ回転初期値
#define INIT_CAMERA_OFFSET		D3DXVECTOR3(0, 150, 0)						// カメラオフセット初期値
#define INIT_CAMERA_DISTANCE	(1000)										// カメラ距離初期値

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
CAMERA g_camera[CAMERATYPE_MAX];
bool g_bIs2PEnabled = true;

//=====================================================================
// 初期化処理
//=====================================================================
void InitCamera(void)
{
	ZeroMemory(&g_camera[0], sizeof(g_camera));
	g_camera[CAMERATYPE_GAME].vecU = CAMERA_VECTOR_UP;
	g_camera[CAMERATYPE_GAME].rot = INIT_CAMERA_ROT;
	g_camera[CAMERATYPE_GAME].offset = INIT_CAMERA_OFFSET;
	g_camera[CAMERATYPE_GAME].fDistance = INIT_CAMERA_DISTANCE;
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
	Player* pPlayer = GetPlayer();

	if (g_bIs2PEnabled)
	{// ２プレイヤーカメラ
		// カメラの注視点をプレイヤー１とプレイヤー２の中間に設定
		D3DXVECTOR3 vecP1ToP2 = pPlayer[1].pos - pPlayer[0].pos;
		SetCameraPosR(0, pPlayer->pos + (vecP1ToP2 / 2));

		// プレイヤーの距離に応じてカメラを離す
		pCamera->fDistance = max(fabsf(vecP1ToP2.x), INIT_CAMERA_DISTANCE);
	}
	else
	{// １プレイヤーカメラ
		// カメラの注視点をプレイヤー１とプレイヤー２の中間に設定
		SetCameraPosR(0, pPlayer->pos);
	}

	// カメラの視点を設定
	SetCameraPosVFromAngle(0);

	// カメラをオフセット分ずらす
	MoveCamera(0, pCamera->offset);
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
// カメラ取得処理
//=====================================================================
CAMERA* GetCamera(int nIdx)
{
	return &g_camera[nIdx];
}

//=====================================================================
// カメラ移動処理
//=====================================================================
void MoveCamera(int nIdx, D3DXVECTOR3 move)
{
	CAMERA* pCamera = &g_camera[nIdx];

	// カメラの視点と注視点を同量動かす（平行移動）
	pCamera->posV += move;
	pCamera->posR += move;
}

//=====================================================================
// 視点の位置設定処理
//=====================================================================
void SetCameraPosV(int nIdx, D3DXVECTOR3 pos)
{
	CAMERA* pCamera = &g_camera[nIdx];

	pCamera->posV = pos;
}

//=====================================================================
// 注視点の位置設定処理
//=====================================================================
void SetCameraPosR(int nIdx, D3DXVECTOR3 pos)
{
	CAMERA* pCamera = &g_camera[nIdx];

	pCamera->posR = pos;
}

//=====================================================================
// 角度から視点の位置を設定する処理
//=====================================================================
void SetCameraPosVFromAngle(int nIdx)
{
	CAMERA* pCamera = &g_camera[nIdx];

	// 角度から視点の位置を求める
	pCamera->posV.x = pCamera->posR.x - sinf(pCamera->rot.y) * cosf(pCamera->rot.x) * pCamera->fDistance;
	pCamera->posV.z = pCamera->posR.z - cosf(pCamera->rot.y) * cosf(pCamera->rot.x) * pCamera->fDistance;
	pCamera->posV.y = pCamera->posR.y - sinf(pCamera->rot.x) * pCamera->fDistance;
}

//=====================================================================
// 角度から注視点の位置を設定する処理
//=====================================================================
void SetCameraPosRFromAngle(int nIdx)
{
	CAMERA* pCamera = &g_camera[nIdx];

	// 角度から注視点の位置を求める
	pCamera->posR.x = pCamera->posV.x + sinf(pCamera->rot.y) * cosf(pCamera->rot.x) * pCamera->fDistance;
	pCamera->posR.z = pCamera->posV.z + cosf(pCamera->rot.y) * cosf(pCamera->rot.x) * pCamera->fDistance;
	pCamera->posR.y = pCamera->posV.y + sinf(pCamera->rot.x) * pCamera->fDistance;
}

//=====================================================================
// カメラ視点回転処理
//=====================================================================
void RotateCameraByV(int nIdx, D3DXVECTOR3 rotMove)
{
	CAMERA* pCamera = &g_camera[nIdx];

	// カメラの角度を変更
	pCamera->rot = GetFixedRotation(pCamera->rot + rotMove);

	// 角度から注視点の位置を設定
	SetCameraPosRFromAngle(nIdx);
}

//=====================================================================
// カメラ注視点回転処理
//=====================================================================
void RotateCameraByR(int nIdx, D3DXVECTOR3 rotMove)
{
	CAMERA* pCamera = &g_camera[nIdx];

	// カメラの角度を変更
	pCamera->rot = GetFixedRotation(pCamera->rot + rotMove);

	// 角度から視点の位置を設定
	SetCameraPosVFromAngle(nIdx);
}