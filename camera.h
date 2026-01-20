//=====================================================================
//
// Camera.cppのヘッダファイル [Camera.h]
// Author : 
// 
//=====================================================================
#ifndef _Camera_H_
#define _Camera_H_

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "main.h"

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
typedef enum
{
	CAMERATYPE_GAME = 0,
	CAMERATYPE_TIME,
	CAMERATYPE_MAX
}CAMERATYPE;

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct CAMERA
{
	D3DXVECTOR3 posV;
	D3DXVECTOR3 posR;
	D3DXVECTOR3 vecU;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 offset;
	float fDistance;
	D3DXMATRIX mtxProjection;	// プロジェクションマトリックス（投影行列）
	D3DXMATRIX mtxView;			// ビューマトリックス
	D3DVIEWPORT9 viewport;
};

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(int nIdx);
CAMERA* GetCamera(int nIdx);
void MoveCamera(int nIdx, D3DXVECTOR3 move);
void SetCameraPosV(int nIdx, D3DXVECTOR3 pos);
void SetCameraPosR(int nIdx, D3DXVECTOR3 pos);
void SetCameraPosVFromAngle(int nIdx);
void SetCameraPosRFromAngle(int nIdx);
void RotateCameraByV(int nIdx, D3DXVECTOR3 rotMove);
void RotateCameraByR(int nIdx, D3DXVECTOR3 rotMove);

#endif