//==================================================
//
// 舞台照明モデル[StageLight.h]
//
//==================================================
#ifndef _STAGELIGHT_H_
#define _STAGELIGHT_H_

//==================================================
//
//	ファイルインクルード
//
//==================================================
#include "main.h"

//==================================================
//
//	構造体定義
//
//==================================================
#define MAX_TEXTURE		(16)	//テクスチャの最大数

//==================================================
//
//	構造体定義
//
//==================================================
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//角度
	bool bUse;

	LPD3DXMESH Mesh;				//メッシュ（頂点情報）へのポインタ
	LPD3DXBUFFER BuffMat;			//マテリアルへのポインタ
	DWORD dwNumMat;					//マテリアルの数
	D3DXMATRIX mtxWorld;			//ワールドマトリックス
	LPDIRECT3DTEXTURE9 Texture[MAX_TEXTURE];	//テクスチャへのポインタ
}StageLight;

//==================================================
//
//	プロトタイプ宣言
//
//==================================================
void InitStageLight(void);
void UninitStageLight(void);
void UpdateStageLight(void);
void DrawStageLight(void);
void SetStageLight(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif // !_STAGELIGHT_H_

