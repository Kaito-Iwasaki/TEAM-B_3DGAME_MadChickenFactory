//=====================================================================
//
// ランキング画面 [ranking.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "ranking.h"
#include "util.h"
#include "DebugProc.h"
#include "Timer.h"
#include "input.h"
#include"camera.h"
#include "sound.h"
#include "fade.h"
#include"DebugProc.h"
#include"Timer.h"
#include"light.h"
#include"texture.h"
#include"field.h"
#include"wall.h"
#include"model.h"
#include"meshcylinder.h"
#include "team_logo.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define RANKING_FILENAME_TITLE			"data\\TEXTURE\\RANKING.png"
#define RANKING_FILENAME_LABEL_1P		"data\\TEXTURE\\1player.png"
#define RANKING_FILENAME_LABEL_2P		"data\\TEXTURE\\2player.png"
#define RANKING_FILENAME_TIME			"data\\TEXTURE\\number000.png"
#define RANKING_FILENAME_PLACE			"data\\TEXTURE\\number001.png"

#define EXPORT_FILENAME_1P				"data\\ranking_1pmode.bin"
#define EXPORT_FILENAME_2P				"data\\ranking_2pmode.bin"

#define MAX_RANKING						(5)
#define MAX_PLACE						(3)

#define RANKING_LABEL_TITLE_SIZE_X		(500)
#define RANKING_LABEL_TITLE_SIZE_Y		(150)
#define RANKING_LABEL_TITLE_POS_X		(SCREEN_CENTER - (RANKING_LABEL_TITLE_SIZE_X / 2))
#define RANKING_LABEL_TITLE_POS_Y		(10)

#define RANKING_LABEL_1P_SIZE_X			(330)
#define RANKING_LABEL_1P_SIZE_Y			(250)
#define RANKING_LABEL_1P_POS_X			(SCREEN_CENTER - 180 - (RANKING_LABEL_1P_SIZE_X / 2))
#define RANKING_LABEL_1P_POS_Y			(70)

#define RANKING_LABEL_2P_SIZE_X			(330)
#define RANKING_LABEL_2P_SIZE_Y			(250)
#define RANKING_LABEL_2P_POS_X			(SCREEN_CENTER + 180 - (RANKING_LABEL_2P_SIZE_X / 2))
#define RANKING_LABEL_2P_POS_Y			(70)

#define RANKING_PLACE_SIZE_X			(60)
#define RANKING_PLACE_SIZE_Y			(80)
#define RANKING_PLACE_POS_X				(250)
#define RANKING_PLACE_POS_Y				(250)

#define RANKING_SIZE_X					(180)
#define RANKING_SIZE_Y					(80)

#define RANKING_1P_POS_X				(SCREEN_CENTER - 170 - (RANKING_SIZE_X / 2))
#define RANKING_1P_POS_Y				(RANKING_PLACE_POS_Y)

#define RANKING_2P_POS_X				(SCREEN_CENTER + 170 - (RANKING_SIZE_X / 2))
#define RANKING_2P_POS_Y				(RANKING_PLACE_POS_Y)

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
int _Compare(const void* a, const void* b);

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingLabelTitle = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingLabel1p = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingLabel2p = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingBg = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingPlace = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking1p = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking2p = NULL;

LPDIRECT3DTEXTURE9 g_pTexBuffRankingLabelTitle = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffRankingLabel1p = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffRankingLabel2p = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffRanking = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffRankingPlace = NULL;

MODELDATA g_modelDataRanking;

int g_aRanking_1p[MAX_RANKING + 1] = { 200, 180, 140, 160, 180, 200 };
int g_aRanking_2p[MAX_RANKING + 1] = { 200, 180, 140, 160, 180, 200 };

int g_nCounterStateRanking = 0;

//=====================================================================
// 初期化処理
//=====================================================================
void InitRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	InitCamera();					//カメラ
	InitField();					//フィールド
	InitWall();						//壁
	InitModel();					//モデル
	InitMeshCylinder();				//メッシュシリンダー
	InitLight();					//ライト

	CAMERA* pCamera = GetCamera(0);
	g_nCounterStateRanking = 0;

	// -- ランキングの更新 --
	LoadBin(EXPORT_FILENAME_1P, &g_aRanking_1p[0], sizeof(int), MAX_RANKING);
	LoadBin(EXPORT_FILENAME_2P, &g_aRanking_2p[0], sizeof(int), MAX_RANKING);
	qsort(&g_aRanking_1p[0], MAX_RANKING + 1, sizeof(int), _Compare);
	qsort(&g_aRanking_2p[0], MAX_RANKING + 1, sizeof(int), _Compare);

	if (GetPreviousMode() == MODE_RESULT)
	{
		if (GetTitle() == 0)
		{// 1プレイヤーモードのランキング
			g_aRanking_1p[MAX_RANKING] = GetTimer();
			qsort(&g_aRanking_1p[0], MAX_RANKING + 1, sizeof(int), _Compare);
			SaveBin(EXPORT_FILENAME_1P, &g_aRanking_1p[0], sizeof(int), MAX_RANKING);
		}
		else
		{
			g_aRanking_2p[MAX_RANKING] = GetTimer();
			qsort(&g_aRanking_2p[0], MAX_RANKING + 1, sizeof(int), _Compare);
			SaveBin(EXPORT_FILENAME_2P, &g_aRanking_2p[0], sizeof(int), MAX_RANKING);
		}
	}

	// スクリプトの読み込み
	LoadScript("data\\model_Land.txt", &g_modelDataRanking);

	// モデルの読み込み・配置
	LoadAndSetModelFromData(&g_modelDataRanking);
	for (int nCountTex = 0; nCountTex < MAX_LOADABLE_TEXTURE; nCountTex++)
	{
		LoadTexture(&g_modelDataRanking.aFilenameTexture[nCountTex][0], nCountTex);
	}

	// フィールドの設定
	for (int nCountField = 0; nCountField < g_modelDataRanking.nCountFieldSet; nCountField++)
	{
		FIELDSETDATA* pFieldData = &g_modelDataRanking.aInfoFieldSet[nCountField];

		SetField(
			pFieldData->pos,
			D3DXVECTOR3(pFieldData->size.x * pFieldData->nBlockX, 0, pFieldData->size.z * pFieldData->nBlockZ),
			pFieldData->rot,
			pFieldData->nType
		);
	}

	// ウォールの設定
	for (int nCountWALL = 0; nCountWALL < g_modelDataRanking.nCountWallSet; nCountWALL++)
	{
		WALLSETDATA* pWallData = &g_modelDataRanking.aInfoWallSet[nCountWALL];

		SetWall(
			pWallData->nType,
			pWallData->pos,
			D3DXVECTOR3(pWallData->size.x * pWallData->nBlockX, pWallData->size.y * pWallData->nBlockY, 0),
			pWallData->rot
		);
	}
	SetCameraPosVFromAngle(0);
	pCamera->mode = CAMERAMODE_NONE;

	pCamera->posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pCamera->posV = D3DXVECTOR3(0.0f, 50.0f, -1450.0f);

	// ----- 頂点バッファの生成 -----

	// ランキングタイトルラベル
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingLabelTitle,
		NULL
	);

	// ランキング1pラベル
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingLabel1p,
		NULL
	);

	// ランキング2pラベル
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingLabel2p,
		NULL
	);

	// 背景
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingBg,
		NULL
	);

	// ランキング順位
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_RANKING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingPlace,
		NULL
	);

	// ランキング数字1P
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_RANKING * MAX_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking1p,
		NULL
	);

	// ランキング数字2P
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_RANKING * MAX_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking2p,
		NULL
	);

	// ----- テクスチャの生成 -----
	D3DXCreateTextureFromFile(pDevice, RANKING_FILENAME_TITLE, &g_pTexBuffRankingLabelTitle);
	D3DXCreateTextureFromFile(pDevice, RANKING_FILENAME_LABEL_1P, &g_pTexBuffRankingLabel1p);
	D3DXCreateTextureFromFile(pDevice, RANKING_FILENAME_LABEL_2P, &g_pTexBuffRankingLabel2p);
	D3DXCreateTextureFromFile(pDevice, RANKING_FILENAME_TIME, &g_pTexBuffRanking);
	D3DXCreateTextureFromFile(pDevice, RANKING_FILENAME_PLACE, &g_pTexBuffRankingPlace);

	VERTEX_2D* pVtx;

	// ----- 背景の頂点設定 -----
	g_pVtxBuffRankingBg->Lock(0, 0, (void**)&pVtx, 0);
	
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 1.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DXCOLOR_ALPHA(0.5f);
	pVtx[1].col = D3DXCOLOR_ALPHA(0.5f);
	pVtx[2].col = D3DXCOLOR_ALPHA(0.5f);
	pVtx[3].col = D3DXCOLOR_ALPHA(0.5f);

	g_pVtxBuffRankingBg->Unlock();

	// ----- ランキング（タイトルラベル）の頂点設定 -----
	g_pVtxBuffRankingLabelTitle->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(RANKING_LABEL_TITLE_POS_X, RANKING_LABEL_TITLE_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(RANKING_LABEL_TITLE_POS_X + RANKING_LABEL_TITLE_SIZE_X, RANKING_LABEL_TITLE_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(RANKING_LABEL_TITLE_POS_X, RANKING_LABEL_TITLE_POS_Y + RANKING_LABEL_TITLE_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(RANKING_LABEL_TITLE_POS_X + RANKING_LABEL_TITLE_SIZE_X, RANKING_LABEL_TITLE_POS_Y + RANKING_LABEL_TITLE_SIZE_Y, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DXCOLOR_WHITE;
	pVtx[1].col = D3DXCOLOR_WHITE;
	pVtx[2].col = D3DXCOLOR_WHITE;
	pVtx[3].col = D3DXCOLOR_WHITE;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	g_pVtxBuffRankingLabelTitle->Unlock();

	// ----- ランキング（1pラベル）の頂点設定 -----
	g_pVtxBuffRankingLabel1p->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(RANKING_LABEL_1P_POS_X, RANKING_LABEL_1P_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(RANKING_LABEL_1P_POS_X + RANKING_LABEL_1P_SIZE_X, RANKING_LABEL_1P_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(RANKING_LABEL_1P_POS_X, RANKING_LABEL_1P_POS_Y + RANKING_LABEL_1P_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(RANKING_LABEL_1P_POS_X + RANKING_LABEL_1P_SIZE_X, RANKING_LABEL_1P_POS_Y + RANKING_LABEL_1P_SIZE_Y, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DXCOLOR_WHITE;
	pVtx[1].col = D3DXCOLOR_WHITE;
	pVtx[2].col = D3DXCOLOR_WHITE;
	pVtx[3].col = D3DXCOLOR_WHITE;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	g_pVtxBuffRankingLabel1p->Unlock();

	// ----- ランキング（1pラベル）の頂点設定 -----
	g_pVtxBuffRankingLabel2p->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(RANKING_LABEL_2P_POS_X, RANKING_LABEL_2P_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(RANKING_LABEL_2P_POS_X + RANKING_LABEL_2P_SIZE_X, RANKING_LABEL_2P_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(RANKING_LABEL_2P_POS_X, RANKING_LABEL_2P_POS_Y + RANKING_LABEL_2P_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(RANKING_LABEL_2P_POS_X + RANKING_LABEL_2P_SIZE_X, RANKING_LABEL_2P_POS_Y + RANKING_LABEL_2P_SIZE_Y, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DXCOLOR_WHITE;
	pVtx[1].col = D3DXCOLOR_WHITE;
	pVtx[2].col = D3DXCOLOR_WHITE;
	pVtx[3].col = D3DXCOLOR_WHITE;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	g_pVtxBuffRankingLabel2p->Unlock();

	// ----- ランキング（順位）の頂点設定 -----
	g_pVtxBuffRankingPlace->Lock(0, 0, (void**)&pVtx, 0);

	for (int nRanking = 0; nRanking < MAX_RANKING; nRanking++)
	{
		float fTexOffset = (nRanking + 1) * 0.1f;

		pVtx[0].pos = D3DXVECTOR3(
			RANKING_PLACE_POS_X,
			RANKING_PLACE_POS_Y + RANKING_PLACE_SIZE_Y * nRanking,
			0
		);
		pVtx[1].pos = D3DXVECTOR3(
			RANKING_PLACE_POS_X + RANKING_PLACE_SIZE_X,
			RANKING_PLACE_POS_Y + RANKING_PLACE_SIZE_Y * nRanking,
			0
		);
		pVtx[2].pos = D3DXVECTOR3(
			RANKING_PLACE_POS_X,
			RANKING_PLACE_POS_Y + RANKING_PLACE_SIZE_Y * (nRanking + 1),
			0
		);
		pVtx[3].pos = D3DXVECTOR3(
			RANKING_PLACE_POS_X + RANKING_PLACE_SIZE_X,
			RANKING_PLACE_POS_Y + RANKING_PLACE_SIZE_Y * (nRanking + 1),
			0
		);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);

		pVtx[0].tex = D3DXVECTOR2(fTexOffset, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(fTexOffset + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(fTexOffset, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(fTexOffset + 0.1f, 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffRankingPlace->Unlock();

	// ----- ランキング（数字, 1P）の頂点設定 -----
	g_pVtxBuffRanking1p->Lock(0, 0, (void**)&pVtx, 0);

	for (int nRanking = 0; nRanking < MAX_RANKING; nRanking++)
	{
		float fYOffset = RANKING_SIZE_Y;
		float fYPosStart = fYOffset * nRanking;
		float fYPosEnd = fYOffset * (nRanking + 1);

		for (int nPlace = 0; nPlace < MAX_PLACE; nPlace++)
		{
			float fXOffset = ((float)RANKING_SIZE_X / (float)MAX_PLACE);
			float fXPosStart = fXOffset * nPlace;
			float fXPosEnd = fXOffset * (nPlace + 1);

			int nPow = (int)powf(10.0f, MAX_PLACE - nPlace);
			int nNumber = (g_aRanking_1p[nRanking] % nPow / (nPow / 10));
			float fTexOffset = nNumber * 0.1f;

			pVtx[0].pos = D3DXVECTOR3(
				RANKING_1P_POS_X + fXPosStart,
				RANKING_1P_POS_Y + fYPosStart,
				0
			);
			pVtx[1].pos = D3DXVECTOR3(
				RANKING_1P_POS_X + fXPosEnd,
				RANKING_1P_POS_Y + fYPosStart,
				0
			);
			pVtx[2].pos = D3DXVECTOR3(
				RANKING_1P_POS_X + fXPosStart,
				RANKING_1P_POS_Y + fYPosEnd,
				0
			);
			pVtx[3].pos = D3DXVECTOR3(
				RANKING_1P_POS_X + fXPosEnd,
				RANKING_1P_POS_Y + fYPosEnd,
				0
			);

			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			pVtx[0].tex = D3DXVECTOR2(fTexOffset, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(fTexOffset + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(fTexOffset, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(fTexOffset + 0.1f, 1.0f);

			pVtx += 4;
		}
	}
	g_pVtxBuffRanking1p->Unlock();

	// ----- ランキング（数字, 2P）の頂点設定 -----
	g_pVtxBuffRanking2p->Lock(0, 0, (void**)&pVtx, 0);

	for (int nRanking = 0; nRanking < MAX_RANKING; nRanking++)
	{
		float fYOffset = RANKING_SIZE_Y;
		float fYPosStart = fYOffset * nRanking;
		float fYPosEnd = fYOffset * (nRanking + 1);

		for (int nPlace = 0; nPlace < MAX_PLACE; nPlace++)
		{
			float fXOffset = ((float)RANKING_SIZE_X / (float)MAX_PLACE);
			float fXPosStart = fXOffset * nPlace;
			float fXPosEnd = fXOffset * (nPlace + 1);

			int nPow = (int)powf(10.0f, MAX_PLACE - nPlace);
			int nNumber = (g_aRanking_2p[nRanking] % nPow / (nPow / 10));
			float fTexOffset = nNumber * 0.1f;

			pVtx[0].pos = D3DXVECTOR3(
				RANKING_2P_POS_X + fXPosStart,
				RANKING_2P_POS_Y + fYPosStart,
				0
			);
			pVtx[1].pos = D3DXVECTOR3(
				RANKING_2P_POS_X + fXPosEnd,
				RANKING_2P_POS_Y + fYPosStart,
				0
			);
			pVtx[2].pos = D3DXVECTOR3(
				RANKING_2P_POS_X + fXPosStart,
				RANKING_2P_POS_Y + fYPosEnd,
				0
			);
			pVtx[3].pos = D3DXVECTOR3(
				RANKING_2P_POS_X + fXPosEnd,
				RANKING_2P_POS_Y + fYPosEnd,
				0
			);

			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			pVtx[0].tex = D3DXVECTOR2(fTexOffset, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(fTexOffset + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(fTexOffset, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(fTexOffset + 0.1f, 1.0f);

			pVtx += 4;
		}
	}
	g_pVtxBuffRanking2p->Unlock();
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitRanking(void)
{
	// 頂点バッファの解放
	RELEASE(g_pVtxBuffRankingLabelTitle);
	RELEASE(g_pVtxBuffRankingLabel1p);
	RELEASE(g_pVtxBuffRankingLabel2p);
	RELEASE(g_pVtxBuffRankingBg);
	RELEASE(g_pVtxBuffRankingPlace);
	RELEASE(g_pVtxBuffRanking1p);
	RELEASE(g_pVtxBuffRanking2p);
	
	// テクスチャの解放
	RELEASE(g_pTexBuffRankingLabelTitle);
	RELEASE(g_pTexBuffRankingLabel1p);
	RELEASE(g_pTexBuffRankingLabel2p);
	RELEASE(g_pTexBuffRankingPlace);
	RELEASE(g_pTexBuffRanking);

	UninitCamera();					//カメラ
	UninitField();					//フィールド
	UninitWall();					//壁
	UninitModel();					//モデル
	UninitMeshCylinder();			//メッシュシリンダー
	UninitLight();					//ライト

	// テクスチャの解放
	ReleaseLoadedTexture();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateRanking(void)
{
	PrintDebugProc("[ランキング画面]\n");

	UpdateCamera();					//カメラ
	UpdateField();					//フィールド
	UpdateModel();					//モデル
	UpdateMeshCylinder();			//メッシュシリンダー
	UpdateLight();					//ライト

	int nCountToChangeMode = 60 * 15;

	if (
		GetKeyboardTrigger(DIK_RETURN) ||
		GetJoypadTrigger(JOYKEY_A) ||
		g_nCounterStateRanking > nCountToChangeMode
		)
	{
		SetFade(MODE_LOGO);
	}

	int nClearTime = GetTimer();

	VERTEX_2D* pVtx;
	int* pRanking;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;

	if (GetTitle() == 0)
	{
		pRanking = &g_aRanking_1p[0];
		pVtxBuff = g_pVtxBuffRanking1p;
	}
	else
	{
		pRanking = &g_aRanking_2p[0];
		pVtxBuff = g_pVtxBuffRanking2p;
	}

	if (nClearTime != pRanking[MAX_RANKING])
	{
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nRanking = MAX_RANKING - 1; nRanking >= 0; nRanking--)
		{
			if (pRanking[nRanking] == nClearTime)
			{
				D3DXCOLOR color = pVtx->col;
				int VertexOffset = nRanking * MAX_PLACE * 4;

				if (g_nCounterStateRanking % 20 == 0)
				{
					color = D3DXCOLOR(1, 0, 0, 1);
				}
				else if (g_nCounterStateRanking % 10 == 0)
				{
					color = D3DXCOLOR(1, 1, 0, 1);
				}

				for (int i = 0; i < MAX_PLACE; i++)
				{
					pVtx[VertexOffset + 0 + (4 * i)].col = color;
					pVtx[VertexOffset + 1 + (4 * i)].col = color;
					pVtx[VertexOffset + 2 + (4 * i)].col = color;
					pVtx[VertexOffset + 3 + (4 * i)].col = color;
				}

				break;
			}
		}

		pVtxBuff->Unlock();
	}

	g_nCounterStateRanking++;
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ゲームカメラの設定
	SetCamera(CAMERATYPE_GAME);

	// ----- オブジェクトの描画 -----
	DrawField();					//フィールド
	DrawWall();						//壁
	DrawModel();					//モデル
	DrawMeshCylinder();				//メッシュシリンダー

	pDevice->SetFVF(FVF_VERTEX_2D);

	// ----- 背景の描画 -----
	pDevice->SetStreamSource(0, g_pVtxBuffRankingBg, 0, sizeof(VERTEX_2D));
	pDevice->SetTexture(0, NULL);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ----- タイトルラベルの描画 -----
	pDevice->SetStreamSource(0, g_pVtxBuffRankingLabelTitle, 0, sizeof(VERTEX_2D));
	pDevice->SetTexture(0, g_pTexBuffRankingLabelTitle);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ----- 1pラベルの描画 -----
	pDevice->SetStreamSource(0, g_pVtxBuffRankingLabel1p, 0, sizeof(VERTEX_2D));
	pDevice->SetTexture(0, g_pTexBuffRankingLabel1p);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ----- 2pラベルの描画 -----
	pDevice->SetStreamSource(0, g_pVtxBuffRankingLabel2p, 0, sizeof(VERTEX_2D));
	pDevice->SetTexture(0, g_pTexBuffRankingLabel2p);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ----- ランキング（順位）の描画 -----
	pDevice->SetStreamSource(0, g_pVtxBuffRankingPlace, 0, sizeof(VERTEX_2D));

	for (int nRanking = 0; nRanking < MAX_RANKING; nRanking++)
	{
		pDevice->SetTexture(0, g_pTexBuffRankingPlace);
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			nRanking * 4,
			2
		);
	}

	// ----- ランキング（数字, 1P）の描画 -----
	pDevice->SetStreamSource(0, g_pVtxBuffRanking1p, 0, sizeof(VERTEX_2D));

	for (int nRanking = 0; nRanking < MAX_RANKING; nRanking++)
	{
		for (int nPlace = 0; nPlace < MAX_PLACE; nPlace++)
		{
			pDevice->SetTexture(0, g_pTexBuffRanking);
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,
				(nRanking * MAX_PLACE + nPlace) * 4,
				2
			);
		}
	}

	// ----- ランキング（数字, 2P）の描画 -----
	pDevice->SetStreamSource(0, g_pVtxBuffRanking2p, 0, sizeof(VERTEX_2D));

	for (int nRanking = 0; nRanking < MAX_RANKING; nRanking++)
	{
		for (int nPlace = 0; nPlace < MAX_PLACE; nPlace++)
		{
			pDevice->SetTexture(0, g_pTexBuffRanking);
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,
				(nRanking * MAX_PLACE + nPlace) * 4,
				2
			);
		}
	}
}

int _Compare(const void* a, const void* b) {
	int L = *(int*)a;
	int R = *(int*)b;
	int lt = L < R;
	int gt = L > R;
	return gt - lt;
}