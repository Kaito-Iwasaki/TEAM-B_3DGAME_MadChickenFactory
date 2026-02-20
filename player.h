//=======================================================
//
// プレイヤー処理[player.h]
// Author : Airi Takahashi
//
//=======================================================
#ifndef _PLAYER_H_		// このマクロ定義がされなかったら
#define _PLAYER_H_		// 2重インクルード防止のマクロを定義する

#include"main.h"
#include"motion_loader.h"

// プレイヤーモーション状態列挙型
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,					// 待機
	MOTIONTYPE_MOVE,						// 移動
	MOTIONTYPE_ACTION,						// アクション
	MOTIONTYPE_JUMP,						// ジャンプ
	MOTIONTYPE_LANDING,						// 着地
	MOTIONTYPE_SQUAT,						// しゃがみ
	MOTIONTYPE_PREPARATIONFOREXTRUSION,		// 押し出し準備
	MOTIONTYPE_EXTRUSION,					// 押し出し
	MOTIONTYPE_CROUCHINGWALKING,			// しゃがみ歩き
	MOTIONTYPE_WALK,						// 歩き
	MOTIONTYPE_DIE,							// 死亡状態
	MOTIONTYPE_MAX
}MOTIONTYPE;

// マクロ定義
#define MAX_PLAYERMODEL			(10)		// モデルの総数
#define MAX_PLAYERMOTION		(7)			// モーションの最大数
#define MAX_PLAYERTEXTURE		(16)		// テクスチャ数
#define MAX_PLAYER				(2)			// プレイヤー数

// プレイヤーの構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;										// 現在の位置
	D3DXVECTOR3 posOld;										// 前回の位置
	D3DXVECTOR3 move;										// 移動量
	D3DXVECTOR3 rot;										// 向き
	D3DXVECTOR3 rotmove;									// 向きの移動量
	D3DXMATRIX mtxWorld;									// ワールドマトリックス
	float fAngle;											// キャラの進む方向
	float fStandPos;										// 現在立っている高さ
	int nIdxShadow;											// 対象の影のインデックス(番号)
	bool bJump;												// ジャンプ状態
	byte ModelHit;											// モデルと当たった面
	LPD3DXMESH Mesh;										// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER BuffMat;									// マテリアルへのポインタ
	LPDIRECT3DTEXTURE9 Texture[MAX_PLAYERTEXTURE];			// テクスチャへのポインタ
	DWORD dwNumMat;											// マテリアルの数
	MOTION PlayerMotion;									// モーション
	float fRadius;											// 半径
	float fHeight;											// 高さ
	bool bDisableControl;									// 操作受け付け判定			
	bool bUse;												// 使用するかどうか
}Player;

// プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void DrawPlayerShadow(int nCnt);
Player* GetPlayer(void);
void SetMove(D3DXVECTOR3* move, byte HitModel, bool *pbjump);
void KillPlayer(Player* pPlayer);

#endif
