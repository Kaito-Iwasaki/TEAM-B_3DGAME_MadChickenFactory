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