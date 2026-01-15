//=======================================================
//
// プレイヤー処理[player.h]
// Author : Airi Takahashi
//
//=======================================================
#ifndef _PLAYER_H_		// このマクロ定義がされなかったら
#define _PLAYER_H_		// 2重インクルード防止のマクロを定義する

#include"main.h"
//#include"model.h"

// マクロ定義
#define MAX_MODEL		(10)		// モデルの総数
#define MAX_MOTION		(7)			// モーションの最大数
#define MAX_TEXTURE		(16)		// テクスチャ数

// プレイヤーの構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;							// 現在の位置
	D3DXVECTOR3 posOld;							// 前回の位置
	D3DXVECTOR3 move;							// 移動量
	D3DXVECTOR3 rot;							// 向き
	D3DXVECTOR3 rotmove;						// 向きの移動量
	D3DXMATRIX mtxWorld;						// ワールドマトリックス
	float fAngle;								// キャラの進む方向
	int nIdxShadow;								// 対象の影のインデックス(番号)
	bool bJump;									// ジャンプ状態
	LPD3DXMESH Mesh;							// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER BuffMat;						// マテリアルへのポインタ
	LPDIRECT3DTEXTURE9 Texture[MAX_TEXTURE];	// テクスチャへのポインタ
	DWORD dwNumMat;								// マテリアルの数
#if 0
	Model aModel[MAX_MODEL];					// モデル(パーツ)
	int nNumModelParts;							// モデル(パーツ)の総数
	int nNumModel;								// モデルの総数
	MOTION_INFO aMotionInfo[MAX_MOTION];		// モーション情報
	int nNumMotion;								// モーションの総数
	MOTIONTYPE motionType;						// 現在のモーションの種類
	bool bLoopMotion;							// 現在のモーションがループするかどうか
	int nNumKey;								// 現在のモーションのキーの総数
	int nKey;									// 現在のモーションの現在のキー
	int nCounterMotion;							// 現在のモーションのカウンター
	bool bFinishMotion;							// 現在のモーションが終了しているかどうか
	bool bBlendMotion;							// ブレンドモーションをするかどうか
	MOTIONTYPE motionTypeBlend;					// ブレンドモーションの種類
	bool bLoopMotionBlend;						// ブレンドモーションがループするかどうか
	int nNumKeyBlend;							// ブレンドモーションのキーの総数
	int nKeyBlend;								// ブレンドモーションの現在のキー
	int nCounterMotionBlend;					// ブレンドモーションのカウンター
	int nFrameBlend;							// ブレンドフレーム数
	int nCounterBlend;							// ブレンドカウンター
#endif
	float fRadius;								// 半径
	float fHeight;								// 高さ
}Player;

// プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player* GetPlayer(void);
//void UpdateMotion(void);
//void SetMosion(MOTIONTYPE motiontype, bool bBlendMotion, int nFrameBlend);

#endif
