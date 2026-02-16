//=====================================================================
//
// model_loader [model_loader.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "model_loader.h"
#include "script.h"
#include <cassert>
#include "model.h"
#include "texture.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_READABLE_CHAR	(256)

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
void _Read_SCRIPT(FILE* pFile, MODELDATA* pBuffer);
void _Read_MODELSET(FILE* pFile, MODELSETDATA* pBuffer);
void _Read_FIELDSET(FILE* pFile, FIELDSETDATA* pBuffer);
void _Read_WALLSET(FILE* pFile, WALLSETDATA* pBuffer);
void _Read_SAWSET(FILE* pFile, SAWSETDATA* pBuffer);
void _Read_PRESSSET(FILE* pFile, PRESSSETDATA* pBuffer);
void _Read_FIRESET(FILE* pFile, FIRESETDATA* pBuffer);
void _Read_PROMPTSET(FILE* pFile, PROMPTSETDATA* pBuffer);
void _Read_GOALSET(FILE* pFile, GOALSETDATA* pBuffer);
void _Read_CONVEYERSET(FILE* pFile, CONVEYERSETDATA* pBuffer);
void _Read_MOVEBOXSET(FILE* pFile, MOVEBOXSETDATA* pBuffer);

//=====================================================================
// スクリプト読み込み処理
//=====================================================================
void LoadScript(const char* pFileName, MODELDATA* pBuffer)
{
	ZeroMemory(pBuffer, sizeof(MODELDATA));

	FILE* pFile = fopen(pFileName, "r");

	if (pFile != NULL)
	{
		char aStrLine[MAX_READABLE_CHAR] = {};

		while (true)
		{
			// 一行読み込む
			if (ReadWord(pFile, &aStrLine[0]) == EOF)
			{// ファイルの最後まで読み込んだら終了する
				break;
			}

			if (strcmp(&aStrLine[0], "SCRIPT") == 0)
			{
				OutputDebugString("[model_loader.cpp] Script Start\n");
				_Read_SCRIPT(pFile, pBuffer);
			}
			
		}

		OutputDebugString("[model_loader.cpp] Script End\n");
		
	}
	else
	{
		OutputDebugString("[model_loader.cpp] Script Failed\n");
	}
}

//=====================================================================
// [SCRIPT]読み込み処理
//=====================================================================
void _Read_SCRIPT(FILE* pFile, MODELDATA* pBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};

	int nTextureCount = 0;		// テクスチャ読み込み数
	int nModelCount = 0;		// モデル読み込み数

	while (true)
	{
		// 一行読み込む
		if (ReadWord(pFile, &aStrLine[0]) == EOF)
		{// ファイルの最後まで読み込んだら終了する
			break;
		}
		else if (strcmp(&aStrLine[0], "END_SCRIPT") == 0)
		{// スクリプトの終了が宣言されたら終了する
			break;
		}

		if (strcmp(&aStrLine[0], "NUM_TEXTURE") == 0)
		{// テクスチャ数読み込み
			fscanf(pFile, " = %d", &pBuffer->nNumTexture);
		}
		else if (strcmp(&aStrLine[0], "TEXTURE_FILENAME") == 0)
		{// テクスチャファイル名読み込み
			if (nTextureCount < MAX_LOADABLE_TEXTURE)
			{// 最大読み込み数まで読み込む
				fscanf(pFile, " = %s", &pBuffer->aFilenameTexture[nTextureCount][0]);
				nTextureCount++;
			}
		}
		else if (strcmp(&aStrLine[0], "NUM_MODEL") == 0)
		{// モデル数読み込み
			fscanf(pFile, " = %d", &pBuffer->nNumModel);
		}
		else if (strcmp(&aStrLine[0], "MODEL_FILENAME") == 0)
		{// テクスチャファイル名読み込み
			if (nModelCount < MAX_LOADABLE_MODEL)
			{// 最大読み込み数まで読み込む
				fscanf(pFile, " = %s", &pBuffer->aFilenameModel[nModelCount][0]);
				nModelCount++;
			}
		}
		else if (strcmp(&aStrLine[0], "MODELSET") == 0)
		{// モデルセット情報読み込み
			MODELSETDATA* pData = &pBuffer->aInfoModelSet[pBuffer->nCountModelSet];

			_Read_MODELSET(pFile, pData);

			pBuffer->nCountModelSet++;
		}
		else if (strcmp(&aStrLine[0], "FIELDSET") == 0)
		{// フィールドセット情報読み込み
			FIELDSETDATA* pData = &pBuffer->aInfoFieldSet[pBuffer->nCountFieldSet];

			_Read_FIELDSET(pFile, pData);

			pBuffer->nCountFieldSet++;
		}
		else if (strcmp(&aStrLine[0], "WALLSET") == 0)
		{// ウォールセット情報読み込み
			WALLSETDATA* pData = &pBuffer->aInfoWallSet[pBuffer->nCountWallSet];

			_Read_WALLSET(pFile, pData);

			pBuffer->nCountWallSet++;
		}
		else if (strcmp(&aStrLine[0], "SAWSET") == 0)
		{// ソウセット情報読み込み
			SAWSETDATA* pData = &pBuffer->aInfoSawSet[pBuffer->nCountSawSet];

			_Read_SAWSET(pFile, pData);

			pBuffer->nCountSawSet++;
		}
		else if (strcmp(&aStrLine[0], "PRESSSET") == 0)
		{// プレスセット情報読み込み
			PRESSSETDATA* pData = &pBuffer->aInfoPressSet[pBuffer->nCountPressSet];

			_Read_PRESSSET(pFile, pData);

			pBuffer->nCountPressSet++;
		}
		else if (strcmp(&aStrLine[0], "FIRESET") == 0)
		{// ファイヤーセット情報読み込み
			FIRESETDATA* pData = &pBuffer->aInfoFireSet[pBuffer->nCountFireSet];

			_Read_FIRESET(pFile, pData);

			pBuffer->nCountFireSet++;
		}
		else if (strcmp(&aStrLine[0], "PROMPTSET") == 0)
		{
			PROMPTSETDATA* pData = &pBuffer->aInfoPromptSet[pBuffer->nCountPromptSet];

			_Read_PROMPTSET(pFile, pData);

			pBuffer->nCountPromptSet++;
		}
		else if (strcmp(&aStrLine[0], "GOALSET") == 0)
		{// ゴールセット情報読み取り

			GOALSETDATA* pData = &pBuffer->InfoGoalSet;

			_Read_GOALSET(pFile, pData);
		}
		else if (strcmp(&aStrLine[0], "CONVEYERSET") == 0)
		{// コンベアセット情報読み取り

			CONVEYERSETDATA* pData = &pBuffer->aInfoConveyerSet[pBuffer->nCountConveyerSet];

			_Read_CONVEYERSET(pFile, pData);

			pBuffer->nCountConveyerSet++;
		}
		else if (strcmp(&aStrLine[0], "MOVEBOXSET") == 0)
		{// 動く箱セット情報読み取り

			MOVEBOXSETDATA* pData = &pBuffer->aInfoMoveBoxSet[pBuffer->nCountMoveBoxSet];

			_Read_MOVEBOXSET(pFile, pData);

			pBuffer->nCountMoveBoxSet++;
		}
	}
}

//=====================================================================
// [MODELSET]読み込み処理
//=====================================================================
void _Read_MODELSET(FILE* pFile, MODELSETDATA* pBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};

	while (true)
	{
		// 一行読み込む
		if (ReadWord(pFile, &aStrLine[0]) == EOF)
		{// ファイルの最後まで読み込んだら終了する
			break;
		}

		if (strcmp(&aStrLine[0], "END_MODELSET") == 0)
		{
			break;
		}
		else if (strcmp(&aStrLine[0], "TYPE") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->nType);
		}
		else if (strcmp(&aStrLine[0], "POS") == 0)
		{
			fscanf(pFile, " = %f %f %f", &pBuffer->pos.x, &pBuffer->pos.y, &pBuffer->pos.z);
		}
		else if (strcmp(&aStrLine[0], "ROT") == 0)
		{
			float fRotX, fRotY, fRotZ;

			fscanf(pFile, " = %f %f %f", &fRotX, &fRotY, &fRotZ);

			pBuffer->rot.x = D3DXToRadian(fRotX);
			pBuffer->rot.y = D3DXToRadian(fRotY);
			pBuffer->rot.z = D3DXToRadian(fRotZ);
		}
		else if (strcmp(&aStrLine[0], "COLLISION") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->bCollision);
		}
		else if (strcmp(&aStrLine[0], "SHADOW") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->bShadow);
		}
	}
}

//=====================================================================
// [FIELDSET]読み込み処理
//=====================================================================
void _Read_FIELDSET(FILE* pFile, FIELDSETDATA* pBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};

	while (true)
	{
		// 一行読み込む
		if (ReadWord(pFile, &aStrLine[0]) == EOF)
		{// ファイルの最後まで読み込んだら終了する
			break;
		}

		if (strcmp(&aStrLine[0], "END_FIELDSET") == 0)
		{
			break;
		}
		else if (strcmp(&aStrLine[0], "TEXTYPE") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->nType);
		}
		else if (strcmp(&aStrLine[0], "POS") == 0)
		{
			fscanf(pFile, " = %f %f %f", &pBuffer->pos.x, &pBuffer->pos.y, &pBuffer->pos.z);
		}
		else if (strcmp(&aStrLine[0], "ROT") == 0)
		{
			float fRotX, fRotY, fRotZ;

			fscanf(pFile, " = %f %f %f", &fRotX, &fRotY, &fRotZ);

			pBuffer->rot.x = D3DXToRadian(fRotX);
			pBuffer->rot.y = D3DXToRadian(fRotY);
			pBuffer->rot.z = D3DXToRadian(fRotZ);
		}
		else if (strcmp(&aStrLine[0], "SIZE") == 0)
		{
			fscanf(pFile, " = %f %f", &pBuffer->size.x, &pBuffer->size.z);
		}
		else if (strcmp(&aStrLine[0], "BLOCK") == 0)
		{
			fscanf(pFile, " = %d %d", &pBuffer->nBlockX, &pBuffer->nBlockZ);
		}
		else if (strcmp(&aStrLine[0], "SPEED") == 0)
		{
			fscanf(pFile, " = %d %d", &pBuffer->nBlockX, &pBuffer->nBlockZ);
		}
		else if (strcmp(&aStrLine[0], "RANGE") == 0)
		{
			fscanf(pFile, " = %d %d", &pBuffer->nBlockX, &pBuffer->nBlockZ);
		}
	}
}

//=====================================================================
// [WALLSET]読み込み処理
//=====================================================================
void _Read_WALLSET(FILE* pFile, WALLSETDATA* pBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};

	while (true)
	{
		// 一行読み込む
		if (ReadWord(pFile, &aStrLine[0]) == EOF)
		{// ファイルの最後まで読み込んだら終了する
			break;
		}

		if (strcmp(&aStrLine[0], "END_WALLSET") == 0)
		{
			break;
		}
		else if (strcmp(&aStrLine[0], "TEXTYPE") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->nType);
		}
		else if (strcmp(&aStrLine[0], "POS") == 0)
		{
			fscanf(pFile, " = %f %f %f", &pBuffer->pos.x, &pBuffer->pos.y, &pBuffer->pos.z);
		}
		else if (strcmp(&aStrLine[0], "ROT") == 0)
		{
			float fRotX, fRotY, fRotZ;

			fscanf(pFile, " = %f %f %f", &fRotX, &fRotY, &fRotZ);

			pBuffer->rot.x = D3DXToRadian(fRotX);
			pBuffer->rot.y = D3DXToRadian(fRotY);
			pBuffer->rot.z = D3DXToRadian(fRotZ);
		}
		else if (strcmp(&aStrLine[0], "SIZE") == 0)
		{
			fscanf(pFile, " = %f %f", &pBuffer->size.x, &pBuffer->size.y);
		}
		else if (strcmp(&aStrLine[0], "BLOCK") == 0)
		{
			fscanf(pFile, " = %d %d", &pBuffer->nBlockX, &pBuffer->nBlockY);
		}
	}
}

//=====================================================================
// [SAWSET]読み込み処理
//=====================================================================
void _Read_SAWSET(FILE* pFile, SAWSETDATA* pBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};

	// 初期値代入
	pBuffer->nIdx = -1;

	while (true)
	{
		// 一行読み込む
		if (ReadWord(pFile, &aStrLine[0]) == EOF)
		{// ファイルの最後まで読み込んだら終了する
			break;
		}

		if (strcmp(&aStrLine[0], "END_SAWSET") == 0)
		{
			break;
		}
		else if (strcmp(&aStrLine[0], "ID") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->nIdx);
		}
		else if (strcmp(&aStrLine[0], "POS") == 0)
		{
			fscanf(pFile, " = %f %f %f", &pBuffer->pos.x, &pBuffer->pos.y, &pBuffer->pos.z);
		}
		else if (strcmp(&aStrLine[0], "ROT") == 0)
		{
			float fRotX, fRotY, fRotZ;

			fscanf(pFile, " = %f %f %f", &fRotX, &fRotY, &fRotZ);

			pBuffer->rot.x = D3DXToRadian(fRotX);
			pBuffer->rot.y = D3DXToRadian(fRotY);
			pBuffer->rot.z = D3DXToRadian(fRotZ);
		}
		else if (strcmp(&aStrLine[0], "STARTUP") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->bStartup);
		}
		else if (strcmp(&aStrLine[0], "TIME") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->nTimeMove);
		}
		else if (strcmp(&aStrLine[0], "RANGE") == 0)
		{
			fscanf(pFile, " = %f %f %f", &pBuffer->moveRange.x, &pBuffer->moveRange.y, &pBuffer->moveRange.z);
		}
		else if (strcmp(&aStrLine[0], "SHADOW") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->bShadow);
		}
	}
}

//=====================================================================
// [PRESSSET]読み込み処理
//=====================================================================
void _Read_PRESSSET(FILE* pFile, PRESSSETDATA* pBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};

	while (true)
	{
		// 一行読み込む
		if (ReadWord(pFile, &aStrLine[0]) == EOF)
		{// ファイルの最後まで読み込んだら終了する
			break;
		}

		if (strcmp(&aStrLine[0], "END_PRESSSET") == 0)
		{
			break;
		}
		else if (strcmp(&aStrLine[0], "ID") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->nIdx);
		}
		else if (strcmp(&aStrLine[0], "POS") == 0)
		{
			fscanf(pFile, " = %f %f %f", &pBuffer->pos.x, &pBuffer->pos.y, &pBuffer->pos.z);
		}
		else if (strcmp(&aStrLine[0], "ROT") == 0)
		{
			float fRotX, fRotY, fRotZ;

			fscanf(pFile, " = %f %f %f", &fRotX, &fRotY, &fRotZ);

			pBuffer->rot.x = D3DXToRadian(fRotX);
			pBuffer->rot.y = D3DXToRadian(fRotY);
			pBuffer->rot.z = D3DXToRadian(fRotZ);
		}
		else if (strcmp(&aStrLine[0], "INTERVAL") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->interval);
		}
		else if (strcmp(&aStrLine[0], "WIDTH") == 0)
		{
			fscanf(pFile, " = %f", &pBuffer->width);
		}
		else if (strcmp(&aStrLine[0], "STATE") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->state);
		}
		else if (strcmp(&aStrLine[0], "SHADOW") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->bShadow);
		}
	}
}

//=====================================================================
// [FIRESET]読み込み処理
//=====================================================================
void _Read_FIRESET(FILE* pFile, FIRESETDATA* pBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};

	while (true)
	{
		// 一行読み込む
		if (ReadWord(pFile, &aStrLine[0]) == EOF)
		{// ファイルの最後まで読み込んだら終了する
			break;
		}

		if (strcmp(&aStrLine[0], "END_FIRESET") == 0)
		{
			break;
		}
		else if (strcmp(&aStrLine[0], "POS") == 0)
		{
			fscanf(pFile, " = %f %f %f", &pBuffer->pos.x, &pBuffer->pos.y, &pBuffer->pos.z);
		}
		else if (strcmp(&aStrLine[0], "ROT") == 0)
		{
			float fRotX, fRotY, fRotZ;

			fscanf(pFile, " = %f %f %f", &fRotX, &fRotY, &fRotZ);

			pBuffer->rot.x = D3DXToRadian(fRotX);
			pBuffer->rot.y = D3DXToRadian(fRotY);
			pBuffer->rot.z = D3DXToRadian(fRotZ);
		}
		else if (strcmp(&aStrLine[0], "STATE") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->state);
		}
		else if (strcmp(&aStrLine[0], "FIRESTATE") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->firestate);
		}
		else if (strcmp(&aStrLine[0], "SHADOW") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->bShadow);
		}
	}
}

//=====================================================================
// [PROMPTSET]読み込み処理
//=====================================================================
void _Read_PROMPTSET(FILE* pFile, PROMPTSETDATA* pBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};

	while (true)
	{
		// 一行読み込む
		if (ReadWord(pFile, &aStrLine[0]) == EOF)
		{// ファイルの最後まで読み込んだら終了する
			break;
		}

		if (strcmp(&aStrLine[0], "END_PROMPTSET") == 0)
		{
			break;
		}
		else if (strcmp(&aStrLine[0], "ID") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->nIdx);
		}
		else if (strcmp(&aStrLine[0], "POS") == 0)
		{
			fscanf(pFile, " = %f %f %f", &pBuffer->pos.x, &pBuffer->pos.y, &pBuffer->pos.z);
		}
		else if (strcmp(&aStrLine[0], "SIZE") == 0)
		{
			fscanf(pFile, " = %f %f %f", &pBuffer->size.x, &pBuffer->size.y, &pBuffer->size.z);
		}
	}
}

//=====================================================================
// [GOALSET]読み込み処理
//=====================================================================
void _Read_GOALSET(FILE* pFile, GOALSETDATA* pBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};

	while (true)
	{
		// 一行読み込む
		if (ReadWord(pFile, &aStrLine[0]) == EOF)
		{// ファイルの最後まで読み込んだら終了する
			break;
		}

		if (strcmp(&aStrLine[0], "END_GOALSET") == 0)
		{
			break;
		}
		else if (strcmp(&aStrLine[0], "POS") == 0)
		{
			fscanf(pFile, " = %f %f %f", &pBuffer->pos.x, &pBuffer->pos.y, &pBuffer->pos.z);
		}
		else if (strcmp(&aStrLine[0], "ROT") == 0)
		{
			float fRotX, fRotY, fRotZ;

			fscanf(pFile, " = %f %f %f", &fRotX, &fRotY, &fRotZ);

			pBuffer->rot.x = D3DXToRadian(fRotX);
			pBuffer->rot.y = D3DXToRadian(fRotY);
			pBuffer->rot.z = D3DXToRadian(fRotZ);
		}
		else if (strcmp(&aStrLine[0], "SHADOW") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->bShadow);
		}
	}
}

//=====================================================================
// [CONVEYERSET]読み込み処理
//=====================================================================
void _Read_CONVEYERSET(FILE* pFile, CONVEYERSETDATA* pBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};

	while (true)
	{
		// 一行読み込む
		if (ReadWord(pFile, &aStrLine[0]) == EOF)
		{// ファイルの最後まで読み込んだら終了する
			break;
		}

		if (strcmp(&aStrLine[0], "END_CONVEYERSET") == 0)
		{
			break;
		}
		else if (strcmp(&aStrLine[0], "POS") == 0)
		{
			fscanf(pFile, " = %f %f %f", &pBuffer->pos.x, &pBuffer->pos.y, &pBuffer->pos.z);
		}
		else if (strcmp(&aStrLine[0], "ROT") == 0)
		{
			float fRotX, fRotY, fRotZ;

			fscanf(pFile, " = %f %f %f", &fRotX, &fRotY, &fRotZ);

			pBuffer->rot.x = D3DXToRadian(fRotX);
			pBuffer->rot.y = D3DXToRadian(fRotY);
			pBuffer->rot.z = D3DXToRadian(fRotZ);
		}
		else if (strcmp(&aStrLine[0], "ONMOVE") == 0)
		{
			fscanf(pFile, " = %f %f %f", &pBuffer->Onmove.x, &pBuffer->Onmove.y, &pBuffer->Onmove.z);
		}
		else if (strcmp(&aStrLine[0], "OFFMOVE") == 0)
		{
			fscanf(pFile, " = %f %f %f", &pBuffer->Offmove.x, &pBuffer->Offmove.y, &pBuffer->Offmove.z);
		}
		else if (strcmp(&aStrLine[0], "SIZE") == 0)
		{
			fscanf(pFile, " = %f %f", &pBuffer->size.x, &pBuffer->size.z);
		}
		else if (strcmp(&aStrLine[0], "IDX") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->nIdx);
		}
		else if (strcmp(&aStrLine[0], "STATE") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->state);
		}
		else if (strcmp(&aStrLine[0], "SHADOW") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->bShadow);
		}
	}
}

//=====================================================================
// [MOVEBOXSET]読み込み処理
//=====================================================================
void _Read_MOVEBOXSET(FILE* pFile, MOVEBOXSETDATA* pBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};

	while (true)
	{
		// 一行読み込む
		if (ReadWord(pFile, &aStrLine[0]) == EOF)
		{// ファイルの最後まで読み込んだら終了する
			break;
		}

		if (strcmp(&aStrLine[0], "END_MOVEBOXSET") == 0)
		{
			break;
		}
		else if (strcmp(&aStrLine[0], "POS") == 0)
		{
			fscanf(pFile, " = %f %f %f", &pBuffer->pos.x, &pBuffer->pos.y, &pBuffer->pos.z);
		}
		else if (strcmp(&aStrLine[0], "ROT") == 0)
		{
			float fRotX, fRotY, fRotZ;

			fscanf(pFile, " = %f %f %f", &fRotX, &fRotY, &fRotZ);

			pBuffer->rot.x = D3DXToRadian(fRotX);
			pBuffer->rot.y = D3DXToRadian(fRotY);
			pBuffer->rot.z = D3DXToRadian(fRotZ);
		}
		else if (strcmp(&aStrLine[0], "RANGE") == 0)
		{
			fscanf(pFile, " = %f %f %f", &pBuffer->range.x, &pBuffer->range.y, &pBuffer->range.z);
		}
	}
}