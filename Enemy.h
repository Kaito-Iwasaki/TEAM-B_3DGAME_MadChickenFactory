//=====================================================================
//
// Enemy.cppのヘッダファイル [Enemy.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "main.h"
#include "motion_loader.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_ENEMY	(4)		// 敵最大数
#define MAX_ROUTINE	(16)

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	ENEMYSTATE_WAIT = 0,
	ENEMYSTATE_MOVE,
	ENEMYSTATE_TURNING,
	ENEMYSTATE_FOUND,
	ENEMYSTATE_MAX
}ENEMYSTATE;

typedef enum
{
	ENEMY_MOTIONTYPE_IDLE = 0,
	ENEMY_MOTIONTYPE_MOVE,
	ENEMY_MOTIONTYPE_ACTION,
	ENEMY_MOTIONTYPE_JUMP,
	ENEMY_MOTIONTYPE_LANDING,
	ENEMY_MOTIONTYPE_MAX
}ENEMY_MOTIONTYPE;

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct
{
	int nWait;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
}ENEMY_ROUTINE;

typedef struct
{
	bool bUse;					// 使用中か
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 角度
	D3DXVECTOR3 rotMove;
	D3DXVECTOR3 destination;
	D3DXMATRIX mtxWorld;		// ワールドマトリックス
	MOTION motion;				// モーション
	int nIdxShadow;				// 影のインデックス
	int nCounterState;
	ENEMYSTATE currentState;
	ENEMYSTATE previousState;
	int nTarget;
	float fSpeed;

	int nMaxRoutine;
	int nCurrentRoutine;
	ENEMY_ROUTINE routine[MAX_ROUTINE];

	D3DXVECTOR3 watchPoint;		// 監視位置
	D3DXVECTOR3 TargetPoint;	// 追跡位置

	int nSoundIdx;				// 使用するサウンドスポットのインデックス
}ENEMY;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSpeed, ENEMY_ROUTINE* pRoutine);

#endif