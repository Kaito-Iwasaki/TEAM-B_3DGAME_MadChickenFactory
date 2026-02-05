//=====================================================================
//
// model_loader.cppのヘッダファイル [model_loader.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _MODEL_LOADER_H_
#define _MODEL_LOADER_H_

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "main.h"
#include "field.h"
#include "wall.h"
#include "saw.h"
#include "press.h"
#include "fire.h"
#include "prompt.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_LOADABLE_TEXTURE	(256)
#define MAX_LOADABLE_MODEL		(256)
#define MAX_MODEL				(1024)

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
//*********************************************************************
// モデル設定情報構造体
//*********************************************************************
typedef struct
{
	int nType;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	bool bCollision;
	bool bShadow;
}MODELSETDATA;

//*********************************************************************
// 床配置情報構造体
//*********************************************************************
typedef struct
{
	int nType;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 size;
	int nBlockX, nBlockZ;
}FIELDSETDATA;

//*********************************************************************
// 壁配置情報構造体
//*********************************************************************
typedef struct
{
	int nType;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 size;
	int nBlockX, nBlockY;
}WALLSETDATA;

//*********************************************************************
// ギミック配置情報構造体（ノコギリ）
//*********************************************************************
typedef struct
{
	int nIdx;				// インデックス（番号）
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	D3DXVECTOR3 moveRange;	//可動域
	int nTimeMove;			// 移動時間
	bool bStartup;			// 起動のON/OFF
	bool bShadow;			// 影の有無
}SAWSETDATA;

//*********************************************************************
// ギミック配置情報構造体（プレス機）
//*********************************************************************
typedef struct
{
	int nIdx;			// インデックス（番号）
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//角度
	float width;		//可動幅
	int interval;		//下降のインターバル
	bool bShadow;		// 影の有無
	PressState state;	// 上下どちら配置するか
}PRESSSETDATA;

//*********************************************************************
// ギミック配置情報構造体（火炎放射器）
//*********************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	OPERATIONSTATE state;	// 状態
	FIRESTATE firestate;	// 炎の状態
	int nIdx;				// インデックス
	bool bShadow;			// 影の有無
}FIRESETDATA;

//*********************************************************************
// ギミック配置情報構造体（プロンプト）
//*********************************************************************
typedef struct
{
	int nIdx;				// インデックス（番号）
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 size;		// サイズ
}PROMPTSETDATA;

//*********************************************************************
// ギミック配置情報構造体（ゴール）
//*********************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	bool bShadow;			// 影の有無
}GOALSETDATA;

//*********************************************************************
// モデル配置情報構造体
//*********************************************************************
typedef struct
{
	int nNumTexture;
	char aFilenameTexture[MAX_LOADABLE_TEXTURE][MAX_PATH];

	int nNumModel;
	char aFilenameModel[MAX_LOADABLE_MODEL][MAX_PATH];

	int nCountModelSet;
	MODELSETDATA aInfoModelSet[MAX_MODEL];

	int nCountFieldSet;
	FIELDSETDATA aInfoFieldSet[MAX_FIELD];

	int nCountWallSet;
	WALLSETDATA aInfoWallSet[MAX_WALL];

	int nCountSawSet;
	SAWSETDATA aInfoSawSet[MAX_SAW];

	int nCountPressSet;
	PRESSSETDATA aInfoPressSet[MAX_PRESS];

	int nCountFireSet;
	FIRESETDATA aInfoFireSet[MAX_FIRE];

	int nCountPromptSet;
	PROMPTSETDATA aInfoPromptSet[MAX_PROMPT];

	GOALSETDATA InfoGoalSet;
}MODELDATA;

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
void LoadScript(const char* pFileName, MODELDATA* pBuffer);

#endif