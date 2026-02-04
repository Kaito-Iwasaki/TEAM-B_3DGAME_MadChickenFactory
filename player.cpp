//=======================================================
//
// プレイヤー処理[player.cpp]
// Author : Airi Takahashi
//
//=======================================================
#include"player.h"
#include"input.h"
#include"camera.h"
#include"shadow.h"
#include"debugproc.h"
#include"stdio.h"
#include"game.h"
#include"sound.h"
#include"fade.h"
#include"util.h"
#include"model.h"
#include"goal.h"
#include"fire.h"
#include "wall.h"
#include"effect.h"

// マクロ定義
#define MAX_TEXTURE				(16)						// テクスチャ数
#define MOVE_POS				(3.0f)						// 位置移動量
#define MAX_JUMP				(20.0f)						// ジャンプ量
#define MOVE_DAMPINGFUNCTION	(0.3f)						// 移動量の減衰係数
#define ANGLE_DAMPINGFUNCTION	(0.1f)						// 角度の減衰係数
#define PLAYER_GRAVITY			(0.85f)						// 重力
#define ONEPLAYER_MODELPAS		"data\\MODEL\\player000.x"	// プレイヤーモデルへのパス
#define TWOPLAYER_MODELPAS		"data\\MODEL\\player001.x"	// プレイヤーモデルへのパス
//#define CHARACTER_TXTNAME		"data\\character.txt"		// キャラクターテキストファイル

// グローバル変数
Player g_Player[MAX_PLAYER];								// プレイヤー情報
D3DXVECTOR3 g_vtxMinPlayer, g_vtxMaxPlayer;					// プレイヤーの最小値、最大値
const char* c_apFilenamePlayer[MAX_PLAYER] =				// プレイヤーのxファイル
{
	ONEPLAYER_MODELPAS,
	TWOPLAYER_MODELPAS
};

//=======================================================
// プレイヤーの初期化処理
//=======================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();				// デバイスの取得
	D3DXMATERIAL* pMat;										// マテリアルデータへのポインタ

#if 0
	MOTION_INFO* pMosionInfo = &g_Player.aMotionInfo[0];	// モーション情報ポインタ
	KEY_INFO* pKeyInfo = &pMosionInfo->aKeyInfo[0];			// キー情報ポインタ
	KEY* pKey = &pKeyInfo->aKey[0];							// キー要素ポインタ
	Model* pModel = &g_Player.aModel[0];					// モデル情報ポインタ
#endif

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_Player[nCntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 現在の位置初期化
		g_Player[nCntPlayer].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 前回の位置代入
		g_Player[nCntPlayer].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量初期化
		g_Player[nCntPlayer].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);		// 向き初期化
		g_Player[nCntPlayer].rotmove = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);	// 向きの移動量初期化
		g_Player[nCntPlayer].fAngle = 0.0f;									// キャラの進む方向初期化
		g_Player[nCntPlayer].nIdxShadow = -1;								// 対象の影のインデックス(番号)初期化
		g_Player[nCntPlayer].bJump = false;									// ジャンプ状態初期化
		g_Player[nCntPlayer].ModelHit = MODEL_HIT_NONE;						// 当たっていない状態にする
		g_Player[nCntPlayer].fRadius = 40;									// 半径初期化
	}

#if 0

	g_Player.bFinishMotion = false;							// 現在モーション未終了
	g_Player.motionTypeBlend = MOTIONTYPE_MAX;				// ブレンドモーションタイプ初期化
	g_Player.bBlendMotion = false;							// ブレンドモーションループの初期化
	g_Player.nNumKeyBlend = -1;								// ブレンドモーション最大キー初期化
	g_Player.nKeyBlend = 0;									// ブレンドモーション現在キー初期化
	g_Player.nCounterMotionBlend = 0;						// ブレンドモーションカウンター初期化
	g_Player.nFrameBlend = -1;								// ブレンドモーションフレーム数初期化
	g_Player.nCounterBlend = 0;								// ブレンドカウンター初期化

	// ファイルを開く
	pFile = fopen(CHARACTER_TXTNAME, "r");

	if (pFile != NULL)
	{// ファイルが開けた場合

		while (1)
		{

			// 文字読み取り
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "SCRIPT") == 0)
			{
				while (1)
				{// 情報読み取り開始

					// 文字読み取り
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NUM_MODEL") == 0)
					{// モデル数読み取り

						fscanf(pFile, "%s", &aString[0]);

						fscanf(pFile, "%d", &g_Player.nNumModel);

					}
					else if (strcmp(&aString[0], "MODEL_FILENAME") == 0)
					{// xファイル名代入

						fscanf(pFile, "%s", &aString[0]);

						fscanf(pFile, "%s", &aFileName[nCntModelFile][0]);

						nCntModelFile++;		// モデルファイルカウント加算
					}
					else if (strcmp(&aString[0], "CHARACTERSET") == 0)
					{// キャラクター情報読み取り開始

						while (1)
						{
							// 文字読み取り
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "NUM_PARTS") == 0)
							{// パーツ数読み取り

								fscanf(pFile, "%s", &aString[0]);

								fscanf(pFile, "%d", &g_Player.nNumModelParts);

							}
							else if (strcmp(&aString[0], "POS") == 0)
							{// 位置読み取り

								fscanf(pFile, "%s", &aString[0]);

								fscanf(pFile, "%f", &g_Player.pos.x);

								fscanf(pFile, "%f", &g_Player.pos.y);

								fscanf(pFile, "%f", &g_Player.pos.z);

							}
							else if (strcmp(&aString[0], "RADIUS") == 0)
							{// 半径読み取り

								fscanf(pFile, "%s", &aString[0]);

								fscanf(pFile, "%f", &g_Player.fRadius);

							}
							else if (strcmp(&aString[0], "HEIGHT") == 0)
							{// 高さ読み取り

								fscanf(pFile, "%s", &aString[0]);

								fscanf(pFile, "%f", &g_Player.fHeight);

							}
							else if (strcmp(&aString[0], "LIFE") == 0)
							{// 体力読み取り

								fscanf(pFile, "%s", &aString[0]);

								fscanf(pFile, "%d", &g_Player.nMaxLife);

								// 体力設定
								g_Player.nLife = g_Player.nMaxLife;
							}
							else if (strcmp(&aString[0], "PARTSSET") == 0)
							{// パーツ情報読み取り開始

								while (1)
								{
									// 文字読み取り
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "INDEX") == 0)
									{// 種類読み取り

										fscanf(pFile, "%s", &aString[0]);

										fscanf(pFile, "%d", &pModel->nType);

									}
									else if (strcmp(&aString[0], "PARENT") == 0)
									{// 親モデルのインデックス読み取り

										fscanf(pFile, "%s", &aString[0]);

										fscanf(pFile, "%d", &pModel->nIdxModelParent);

									}
									else if (strcmp(&aString[0], "POS") == 0)
									{// 位置(オフセット読み取り)

										fscanf(pFile, "%s", &aString[0]);

										fscanf(pFile, "%f", &pModel->pos.x);

										fscanf(pFile, "%f", &pModel->pos.y);

										fscanf(pFile, "%f", &pModel->pos.z);

										pModel->posRecord = pModel->pos;

									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{// 向き読み取り

										fscanf(pFile, "%s", &aString[0]);

										fscanf(pFile, "%f", &pModel->rot.x);

										fscanf(pFile, "%f", &pModel->rot.y);

										fscanf(pFile, "%f", &pModel->rot.z);

										pModel->rotRecord = pModel->rot;

									}
									else if (strcmp(&aString[0], "#") == 0)
									{// コメント

										fgets(&aString[0], 512, pFile);

									}
									else if (strcmp(&aString[0], "END_PARTSSET") == 0)
									{// パーツ情報読み取り終了

										pModel++;		// パーツカウント加算

										break;
									}
								}
							}
							else if (strcmp(&aString[0], "#") == 0)
							{// コメント

								fgets(&aString[0], 512, pFile);

							}
							else if (strcmp(&aString[0], "END_CHARACTERSET") == 0)
							{// キャラクター情報読み取り終了

								pModel = &g_Player.aModel[0];	// モデル情報ポインタ初期化
								break;
							}

						}
					}
					else if (strcmp(&aString[0], "MOTIONSET") == 0)
					{// モーション情報読み取り開始

						while (1)
						{
							// 文字読み取り
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "LOOP") == 0)
							{// ループ状態読み取り

								fscanf(pFile, "%s", &aString[0]);

								fscanf(pFile, "%d", &pMosionInfo->bLoop);

							}
							else if (strcmp(&aString[0], "NUM_KEY") == 0)
							{// キーの総数読み取り

								fscanf(pFile, "%s", &aString[0]);

								fscanf(pFile, "%d", &pMosionInfo->nNumKey);

							}
							else if (strcmp(&aString[0], "KEYSET") == 0)
							{// キー情報読み取り開始

								while (1)
								{
									// 文字読み取り
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "FRAME") == 0)
									{// 再生フレーム読み取り

										fscanf(pFile, "%s", &aString[0]);

										fscanf(pFile, "%d", &pKeyInfo->nFrame);

									}
									else if (strcmp(&aString[0], "KEY") == 0)
									{// キー要素読み取り開始

										fscanf(pFile, "%s", &aString[0]);

										while (1)
										{
											// 文字読み取り
											fscanf(pFile, "%s", &aString[0]);

											if (strcmp(&aString[0], "POS") == 0)
											{// 位置読み取り

												fscanf(pFile, "%s", &aString[0]);

												// X座標設定
												fscanf(pFile, "%f", &pKey->fPosX);

												// Y座標設定
												fscanf(pFile, "%f", &pKey->fPosY);

												// Z座標設定
												fscanf(pFile, "%f", &pKey->fPosZ);

											}
											else if (strcmp(&aString[0], "ROT") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);

												// X軸回転設定
												fscanf(pFile, "%f", &pKey->fRotX);

												// Y軸回転設定
												fscanf(pFile, "%f", &pKey->fRotY);

												// Z軸回転設定
												fscanf(pFile, "%f", &pKey->fRotZ);

											}
											else if (strcmp(&aString[0], "#") == 0)
											{// コメント

												fgets(&aString[0], 512, pFile);

											}
											else if (strcmp(&aString[0], "END_KEY") == 0)
											{// キー要素読み取り終了

												pKey++;		// キー要素ポインタを進める
												pModel++;	// モデル情報ポインタを進める
												break;
											}
										}
									}
									else if (strcmp(&aString[0], "#") == 0)
									{// コメント

										fgets(&aString[0], 512, pFile);

									}
									else if (strcmp(&aString[0], "END_KEYSET") == 0)
									{// キー情報読み取り終了

										pKeyInfo++;						// キー情報ポインタを進める
										pKey = &pKeyInfo->aKey[0];		// キー要素ポインタ初期化
										pModel = &g_Player.aModel[0];	// モデル情報ポインタ初期化

										break;
									}
								}
							}
							else if (strcmp(&aString[0], "#") == 0)
							{// コメント

								fgets(&aString[0], 512, pFile);

							}
							else if (strcmp(&aString[0], "END_MOTIONSET") == 0)
							{
								pMosionInfo++;								// モーション情報ポインタを進める
								pKeyInfo = &pMosionInfo->aKeyInfo[0];		// キー情報ポインタ初期化
								pKey = &pKeyInfo->aKey[0];					// キー要素ポインタ初期化
								break;
							}
						}
					}
					else if (strcmp(&aString[0], "#") == 0)
					{// コメント

						fgets(&aString[0], 512, pFile);

					}
					else if (strcmp(&aString[0], "END_SCRIPT") == 0)
					{// 情報読み取り終了

						break;
					}
				}

				break;
			}
			else if (strcmp(&aString[0], "#") == 0)
			{// コメント

				fgets(&aString[0], 512, pFile);
			}
		}
	}

	for (int nCntModel = 0; nCntModel < g_Player.nNumModel; nCntModel++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX(&aFileName[nCntModel][0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_Player.aModel[nCntModel].pBuffMat,
			NULL,
			&g_Player.aModel[nCntModel].dwNumMat,
			&g_Player.aModel[nCntModel].pMesh);
	}

	for (int nCntModel = 0; nCntModel < g_Player.nNumModel; nCntModel++)
	{
		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_Player.aModel[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_Player.aModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				// テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_Player.aModel[nCntModel].apTexture[nCntMat]);
			}
		}
	}

	// モーションの設定
	SetMosion(MOTIONTYPE_NEUTRAL, false, 10);

#endif

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX(c_apFilenamePlayer[nCntPlayer],
						D3DXMESH_SYSTEMMEM,
						pDevice,
						NULL,
						&g_Player[nCntPlayer].BuffMat,
						NULL,
						&g_Player[nCntPlayer].dwNumMat,
						&g_Player[nCntPlayer].Mesh);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_Player[nCntPlayer].BuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_Player[nCntPlayer].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				// テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
										pMat[nCntMat].pTextureFilename,
										&g_Player[nCntPlayer].Texture[nCntMat]);
			}
		}

		// 影のインデックス設定
		g_Player[nCntPlayer].nIdxShadow = SetShadow(D3DXVECTOR3(g_Player[nCntPlayer].pos.x, g_Player[nCntPlayer].pos.y + 1.0f, g_Player[nCntPlayer].pos.z), 50.0f);
	}
}

//=======================================================
// プレイヤーの終了処理
//=======================================================
void UninitPlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		// メッシュの破棄
		if (g_Player[nCntPlayer].Mesh != NULL)
		{
			g_Player[nCntPlayer].Mesh->Release();
			g_Player[nCntPlayer].Mesh = NULL;
		}

		// マテリアルの破棄
		if (g_Player[nCntPlayer].BuffMat != NULL)
		{
			g_Player[nCntPlayer].BuffMat->Release();
			g_Player[nCntPlayer].BuffMat = NULL;
		}

		// テクスチャの破棄
		for (int nCntMat = 0; nCntMat < (int)g_Player[nCntPlayer].dwNumMat; nCntMat++)
		{
			if (g_Player[nCntPlayer].Texture[nCntMat] != NULL)
			{
				g_Player[nCntPlayer].Texture[nCntMat]->Release();
				g_Player[nCntPlayer].Texture[nCntMat] = NULL;
			}
		}
	}
}

//=======================================================
// プレイヤーの更新処理
//=======================================================
void UpdatePlayer(void)
{
	CAMERA* pCamera = GetCamera(0);				// カメラ情報の取得
	float fRotDiff = 0.0f;						// 角度の差分代入
	int nValueH, nValueV;						// スティック値代入

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{

		// 前回の位置保存
		g_Player[nCntPlayer].posOld = g_Player[nCntPlayer].pos;

		if (nCntPlayer == 0)
		{// 1P移動操作

			if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_UP, nCntPlayer) == TRUE)
			{// Wキー入力(Z軸+の方向に移動)

				if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_LEFT, nCntPlayer) == TRUE)
				{// Aキー入力(X軸-の方向に移動)

					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y - (D3DX_PI / 4)) * MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y - (D3DX_PI / 4)) * MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = ((D3DX_PI / 4) * 3) + pCamera->rot.y;
				}
				else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_RIGHT, nCntPlayer) == TRUE)
				{// Dキー入力(X軸+の方向に移動)

					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y + (D3DX_PI / 4)) * MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y + (D3DX_PI / 4)) * MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = ((-D3DX_PI / 4) * 3) + pCamera->rot.y;
				}
				else
				{
					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y) * MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y) * MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = D3DX_PI + pCamera->rot.y;
				}
			}
			else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_DOWN, nCntPlayer) == TRUE)
			{/// Sキー入力(Z軸-の方向に移動)

				if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_LEFT, nCntPlayer) == TRUE)
				{// Aキー入力(X軸-の方向に移動)

					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y - (D3DX_PI / 4 * 3)) * MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y - (D3DX_PI / 4 * 3)) * MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = (D3DX_PI / 4) + pCamera->rot.y;
				}
				else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_RIGHT, nCntPlayer) == TRUE)
				{// Dキー入力(X軸+の方向に移動)

					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y + (D3DX_PI / 4 * 3)) * MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y + (D3DX_PI / 4 * 3)) * MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = (-D3DX_PI / 4) + pCamera->rot.y;
				}
				else
				{
					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y) * -MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y) * -MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = pCamera->rot.y;
				}
			}
			else if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_LEFT, nCntPlayer) == TRUE)
			{// Aキー入力(X軸-の方向に移動)

				if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_UP, nCntPlayer) == TRUE)
				{// Wキー入力(Z軸+の方向に移動)

					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y - (D3DX_PI / 4)) * MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y - (D3DX_PI / 4)) * MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = (D3DX_PI / 4 * 3) + pCamera->rot.y;
				}
				else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_DOWN, nCntPlayer) == TRUE)
				{// Sキー入力(Z軸+の方向に移動)

					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y - (D3DX_PI / 4 * 3)) * MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y - (D3DX_PI / 4 * 3)) * MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = (D3DX_PI / 4) + pCamera->rot.y;
				}
				else
				{
					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y + (D3DX_PI / 2)) * -MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y + (D3DX_PI / 2)) * -MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = D3DX_PI / 2 + pCamera->rot.y;
				}
			}
			else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_RIGHT, nCntPlayer) == TRUE)
			{// Dキー入力(X軸+の方向に移動)

				if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_UP, nCntPlayer) == TRUE)
				{// Wキー入力(Z軸+の方向に移動)

					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y + (D3DX_PI / 4)) * MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y + (D3DX_PI / 4)) * MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = (-D3DX_PI / 4 * 3) + pCamera->rot.y;
				}
				else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_DOWN, nCntPlayer) == TRUE)
				{// Sキー入力(Z軸+の方向に移動)

					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y + (D3DX_PI / 4 * 3)) * MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y + (D3DX_PI / 4 * 3)) * MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = (-D3DX_PI / 4) + pCamera->rot.y;
				}
				else
				{
					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y + (D3DX_PI / 2)) * MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y + (D3DX_PI / 2)) * MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = -D3DX_PI / 2 + pCamera->rot.y;
				
				}
			}

			if (GetKeyboardTrigger(DIK_SPACE) == true || GetJoypadPress(JOYKEY_A, nCntPlayer) == true)
			{// ジャンプ押下

				if (g_Player[nCntPlayer].bJump == false)
				{// ジャンプする

					g_Player[nCntPlayer].bJump = true;		// ジャンプ中にする
					g_Player[nCntPlayer].move.y = MAX_JUMP;	// ジャンプ量

					// モーションの設定処理
					//SetMosion(MOTIONTYPE_JUMP, true, 30);

					// サウンドの再生
					//PlaySound(SOUND_LABEL_SE_JUMP);
				}
			}

		}
		else if (nCntPlayer == 1)
		{// 2P移動操作

			if (GetKeyboardPress(DIK_UP) == true || GetJoypadPress(JOYKEY_UP, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_UP, nCntPlayer) == TRUE)
			{// Wキー入力(Z軸+の方向に移動)

				if (GetKeyboardPress(DIK_LEFT) == true || GetJoypadPress(JOYKEY_LEFT, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_LEFT, nCntPlayer) == TRUE)
				{// Aキー入力(X軸-の方向に移動)

					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y - (D3DX_PI / 4)) * MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y - (D3DX_PI / 4)) * MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = ((D3DX_PI / 4) * 3) + pCamera->rot.y;
				}
				else if (GetKeyboardPress(DIK_RIGHT) == true || GetJoypadPress(JOYKEY_RIGHT, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_RIGHT, nCntPlayer) == TRUE)
				{// Dキー入力(X軸+の方向に移動)

					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y + (D3DX_PI / 4)) * MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y + (D3DX_PI / 4)) * MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = ((-D3DX_PI / 4) * 3) + pCamera->rot.y;
				}
				else
				{
					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y) * MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y) * MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = D3DX_PI + pCamera->rot.y;
				}
			}
			else if (GetKeyboardPress(DIK_DOWN) == true || GetJoypadPress(JOYKEY_DOWN, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_DOWN, nCntPlayer) == TRUE)
			{/// Sキー入力(Z軸-の方向に移動)

				if (GetKeyboardPress(DIK_LEFT) == true || GetJoypadPress(JOYKEY_LEFT, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_LEFT, nCntPlayer) == TRUE)
				{// Aキー入力(X軸-の方向に移動)

					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y - (D3DX_PI / 4 * 3)) * MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y - (D3DX_PI / 4 * 3)) * MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = (D3DX_PI / 4) + pCamera->rot.y;
				}
				else if (GetKeyboardPress(DIK_RIGHT) == true || GetJoypadPress(JOYKEY_RIGHT, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_RIGHT, nCntPlayer) == TRUE)
				{// Dキー入力(X軸+の方向に移動)

					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y + (D3DX_PI / 4 * 3)) * MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y + (D3DX_PI / 4 * 3)) * MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = (-D3DX_PI / 4) + pCamera->rot.y;
				}
				else
				{
					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y) * -MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y) * -MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = pCamera->rot.y;
				}
			}
			else if (GetKeyboardPress(DIK_LEFT) == true || GetJoypadPress(JOYKEY_LEFT, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_LEFT, nCntPlayer) == TRUE)
			{// Aキー入力(X軸-の方向に移動)

				if (GetKeyboardPress(DIK_UP) == true || GetJoypadPress(JOYKEY_UP, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_UP, nCntPlayer) == TRUE)
				{// Wキー入力(Z軸+の方向に移動)

					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y - (D3DX_PI / 4)) * MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y - (D3DX_PI / 4)) * MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = (D3DX_PI / 4 * 3) + pCamera->rot.y;
				}
				else if (GetKeyboardPress(DIK_DOWN) == true || GetJoypadPress(JOYKEY_DOWN, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_DOWN, nCntPlayer) == TRUE)
				{// Sキー入力(Z軸+の方向に移動)

					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y - (D3DX_PI / 4 * 3)) * MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y - (D3DX_PI / 4 * 3)) * MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = (D3DX_PI / 4) + pCamera->rot.y;
				}
				else
				{
					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y + (D3DX_PI / 2)) * -MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y + (D3DX_PI / 2)) * -MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = D3DX_PI / 2 + pCamera->rot.y;
				}
			}
			else if (GetKeyboardPress(DIK_RIGHT) == true || GetJoypadPress(JOYKEY_RIGHT, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_RIGHT, nCntPlayer) == TRUE)
			{// Dキー入力(X軸+の方向に移動)

				if (GetKeyboardPress(DIK_UP) == true || GetJoypadPress(JOYKEY_UP, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_UP, nCntPlayer) == TRUE)
				{// Wキー入力(Z軸+の方向に移動)

					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y + (D3DX_PI / 4)) * MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y + (D3DX_PI / 4)) * MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = (-D3DX_PI / 4 * 3) + pCamera->rot.y;
				}
				else if (GetKeyboardPress(DIK_DOWN) == true || GetJoypadPress(JOYKEY_DOWN, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_DOWN, nCntPlayer) == TRUE)
				{// Sキー入力(Z軸+の方向に移動)

					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y + (D3DX_PI / 4 * 3)) * MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y + (D3DX_PI / 4 * 3)) * MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = (-D3DX_PI / 4) + pCamera->rot.y;
				}
				else
				{
					// 位置更新
					g_Player[nCntPlayer].move.x += sinf(pCamera->rot.y + (D3DX_PI / 2)) * MOVE_POS;
					g_Player[nCntPlayer].move.z += cosf(pCamera->rot.y + (D3DX_PI / 2)) * MOVE_POS;

					// 向き更新
					g_Player[nCntPlayer].rotmove.y = -D3DX_PI / 2 + pCamera->rot.y;

				
				}
			}

			if (GetKeyboardTrigger(DIK_RSHIFT) == true || GetJoypadPress(JOYKEY_A, nCntPlayer) == true)
			{// ジャンプ押下

				if (g_Player[nCntPlayer].bJump == false)
				{// ジャンプする

					g_Player[nCntPlayer].bJump = true;		// ジャンプ中にする
					g_Player[nCntPlayer].move.y = MAX_JUMP;	// ジャンプ量

					// モーションの設定処理
					//SetMosion(MOTIONTYPE_JUMP, true, 30);

					// サウンドの再生
					//PlaySound(SOUND_LABEL_SE_JUMP);
				}
			}

		}

		// 角度の値補正
		g_Player[nCntPlayer].rotmove.y = GetFixedRotation(g_Player[nCntPlayer].rotmove.y);

		// 移動量を更新
		g_Player[nCntPlayer].move.x += (0.0f - g_Player[nCntPlayer].move.x) * MOVE_DAMPINGFUNCTION;
		g_Player[nCntPlayer].move.z += (0.0f - g_Player[nCntPlayer].move.z) * MOVE_DAMPINGFUNCTION;

		// 重力を加える
		g_Player[nCntPlayer].move.y -= PLAYER_GRAVITY;

		// 位置を更新
		g_Player[nCntPlayer].pos.x += g_Player[nCntPlayer].move.x;
		g_Player[nCntPlayer].pos.y += g_Player[nCntPlayer].move.y;
		g_Player[nCntPlayer].pos.z += g_Player[nCntPlayer].move.z;


		if (g_Player[nCntPlayer].pos.y < 0.0f)
		{// 下画面端

			g_Player[nCntPlayer].bJump = false;
			g_Player[nCntPlayer].move.y = 0.0f;
			g_Player[nCntPlayer].pos.y = 0.0f;

			
#if 0
			if (g_Player.motionType == MOTIONTYPE_JUMP && g_Player.motionTypeBlend == MOTIONTYPE_JUMP)
			{// 現在のモーションがジャンプ状態

				// モーションの設定(着地状態)
				SetMosion(MOTIONTYPE_LANDING, true, 15);

				// 振動の設定
				SetJoypadVibration(10000, 10000, 10);
			}
#endif
		}

		// モデルとの当たり判定
		g_Player[nCntPlayer].ModelHit = CollisionModel(&g_Player[nCntPlayer].pos, g_Player[nCntPlayer].posOld, D3DXVECTOR3(200.0f, 200.0f, 500.0f));

		// プレイヤーの移動量の設定
		SetMove(&g_Player[nCntPlayer].move, g_Player[nCntPlayer].ModelHit, &g_Player[nCntPlayer].bJump);

		// ゴールとの当たり判定
		CollisionGoal(&g_Player[nCntPlayer].pos, &g_Player[nCntPlayer].posOld, &g_Player[nCntPlayer].move, g_Player[nCntPlayer].fRadius);

		// 火炎放射器との当たり判定
		CollisionFlamethrower(&g_Player[nCntPlayer].pos, &g_Player[nCntPlayer].posOld, &g_Player[nCntPlayer].move, g_Player[nCntPlayer].fRadius);

			// 壁との当たり判定
			CollisionWall(&g_Player[nCntPlayer].pos, g_Player[nCntPlayer].posOld, &g_Player[nCntPlayer].move, D3DXVECTOR3_ZERO);

		if (g_Player[nCntPlayer].move.y != 0.0f)
		{// 落下中

			if (g_Player[nCntPlayer].bJump == false)
			{// ジャンプ状態にする

				g_Player[nCntPlayer].bJump = true;		// ジャンプ中にする
			}
		}
		
		// 目標の移動方向までの差分算出
		fRotDiff = g_Player[nCntPlayer].rotmove.y - g_Player[nCntPlayer].rot.y;

		// 角度の値補正
		fRotDiff = GetFixedRotation(fRotDiff);

		// 移動方向(角度)の補正
		g_Player[nCntPlayer].rot.y += fRotDiff * ANGLE_DAMPINGFUNCTION;

		// 角度の値補正
		g_Player[nCntPlayer].rot.y = GetFixedRotation(g_Player[nCntPlayer].rot.y);

		// 影の位置を設定(更新)
		SetPositionShadow(g_Player[nCntPlayer].nIdxShadow, g_Player[nCntPlayer].pos, (g_Player[nCntPlayer].pos.y - g_Player[nCntPlayer].posOld.y) / 10.0f);

#if 0
		if (g_Player.bFinishMotion == true)
		{// 現在モーション終了時

			if (g_Player.motionType != MOTIONTYPE_JUMP && g_Player.motionTypeBlend != MOTIONTYPE_JUMP)
			{// 現在のモーションがジャンプ状態以外

				// モーションの設定処理(待機状態にする)
				SetMosion(MOTIONTYPE_NEUTRAL, true, 30);

				g_Player.bFinishMotion = false;		// 未終了にする

			}
		}
#endif

		// モーションの更新処理
		//UpdateMotion();

		// デバック表示
		//PrintDebugProc("\n\n - プレイヤー情報 -");
		//PrintDebugProc("\n 位置 (%f, %f, %f)", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
		//PrintDebugProc("\n 移動量 (%f, %f, %f)", g_Player.move.x, g_Player.move.y, g_Player.move.z);
		//PrintDebugProc("\n 目的の向き (%f)", g_Player.rotmove.y);
		//PrintDebugProc("\n 向き (%f)", g_Player.rot.y);

	}
}

//=======================================================
// プレイヤーの描画処理
//=======================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;					// 計算用マトリックス
	D3DMATERIAL9 matDef;							// 現在のマテリアル保存
	D3DXMATERIAL* pMat;								// マテリアルデータへのポインタ

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		// プレイヤーの影の描画
		DrawPlayerShadow(nCntPlayer);

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Player[nCntPlayer].mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_Player[nCntPlayer].rot.y, g_Player[nCntPlayer].rot.x, g_Player[nCntPlayer].rot.z);

		D3DXMatrixMultiply(&g_Player[nCntPlayer].mtxWorld, &g_Player[nCntPlayer].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans,
			g_Player[nCntPlayer].pos.x, g_Player[nCntPlayer].pos.y, g_Player[nCntPlayer].pos.z);

		D3DXMatrixMultiply(&g_Player[nCntPlayer].mtxWorld, &g_Player[nCntPlayer].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Player[nCntPlayer].mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_Player[nCntPlayer].BuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_Player[nCntPlayer].dwNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャ設定
			pDevice->SetTexture(0, g_Player[nCntPlayer].Texture[nCntMat]);

			// モデル(パーツの描画)
			g_Player[nCntPlayer].Mesh->DrawSubset(nCntMat);
		}
#if 0
	// 全モデル(パーツ)の描画
	for (int nCntModel = 0; nCntModel < g_Player.nNumModelParts; nCntModel++)
	{
		D3DXMATRIX mtxRotModel, mtxTransModel;		// 計算用マトリックス
		D3DXMATRIX mtxParent;						// 親のマトリックス

		// パーツのワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Player.aModel[nCntModel].mtxWorld);

		// パーツの向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
			g_Player.aModel[nCntModel].rot.y, g_Player.aModel[nCntModel].rot.x, g_Player.aModel[nCntModel].rot.z);

		D3DXMatrixMultiply(&g_Player.aModel[nCntModel].mtxWorld, &g_Player.aModel[nCntModel].mtxWorld, &mtxRotModel);

		// パーツの位置を反映
		D3DXMatrixTranslation(&mtxTransModel,
			g_Player.aModel[nCntModel].pos.x, g_Player.aModel[nCntModel].pos.y, g_Player.aModel[nCntModel].pos.z);

		D3DXMatrixMultiply(&g_Player.aModel[nCntModel].mtxWorld, &g_Player.aModel[nCntModel].mtxWorld, &mtxTransModel);

		// パーツの「親マトリックス」を設定
		if (g_Player.aModel[nCntModel].nIdxModelParent != -1)
		{// 親モデルがある場合

			mtxParent = g_Player.aModel[g_Player.aModel[nCntModel].nIdxModelParent].mtxWorld;
		}
		else
		{// 親モデルが無い場合

			mtxParent = g_Player.mtxWorld;
		}

		// 算出した「パーツのワールドマトリックス」と「親のマトリックス」をかけ合わせる
		D3DXMatrixMultiply(&g_Player.aModel[nCntModel].mtxWorld,
							&g_Player.aModel[nCntModel].mtxWorld,
							&mtxParent);

		// パーツのワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD,
								&g_Player.aModel[nCntModel].mtxWorld);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_Player.aModel[g_Player.aModel[nCntModel].nType].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_Player.aModel[g_Player.aModel[nCntModel].nType].dwNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャ設定
			pDevice->SetTexture(0, g_Player.aModel[g_Player.aModel[nCntModel].nType].apTexture[nCntMat]);

			// パーツの描画
			g_Player.aModel[g_Player.aModel[nCntModel].nType].pMesh->DrawSubset(nCntMat);
		}
	}

#endif

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	}
}
//=======================================================
// プレイヤーの影の描画処理
//=======================================================
void DrawPlayerShadow(int nCnt)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスの取得
	D3DMATERIAL9 matDef;								// 現在のマテリアル保存
	D3DXMATERIAL* pMat;									// マテリアルデータへのポインタ
	D3DXMATRIX mtxShadow;								// シャドウマトリックス
	D3DLIGHT9 light;									// ライト情報
	D3DXVECTOR4 posLight;								// ライトの位置
	D3DXVECTOR3 pos, normal;							// 平面上の任意の点、ベクトル
	D3DXPLANE plane;									// 平面情報
	D3DMATERIAL9 Mat;									// マテリアル
	Mat.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// マテリアルカラー設定

	// ライトの位置を設定
	pDevice->GetLight(0, &light);
	posLight = D3DXVECTOR4(-0.2f, 0.8f, -0.4f, 0.0f);

	// 平面情報を生成
	pos = D3DXVECTOR3(0.0f, 0.7f, 0.0f);
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXPlaneFromPointNormal(&plane, &pos, &normal);

	// シャドウマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);

	// シャドウマトリックスの作成
	D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
	D3DXMatrixMultiply(&mtxShadow, &g_Player[nCnt].mtxWorld, &mtxShadow);

	// シャドウマトリックスをワールドマトリックスに設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_Player[nCnt].BuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_Player[nCnt].dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&Mat);

		// テクスチャ設定
		pDevice->SetTexture(0, NULL);

		// モデル(パーツの描画)
		g_Player[nCnt].Mesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=======================================================
// プレイヤー情報の取得処理
//=======================================================
Player* GetPlayer(void)
{
	return &g_Player[0];
}

//=======================================================
// モーションの更新処理
//=======================================================
#if 0
void UpdateMotion(void)
{
	float fDiffKey = 0.0f;			// 差分
	float fDiffKeyBlend = 0.0f;		// ブレンドキーの差分
	float fDiffBlend = 0.0f;		// ブレンドの差分
	float fPosXCurrent, fPosYCurrent, fPosZCurrent, fRotXCurrent, fRotYCurrent, fRotZCurrent;		// 現在のモーションの値
	float fPosXBlend, fPosYBlend, fPosZBlend, fRotXBlend, fRotYBlend, fRotZBlend;					// ブレンドモーションの値
	float fPosX, fPosY, fPosZ, fRotX, fRotY, fRotZ;													// 求める値
	KEY* pKey = &g_Player.aMotionInfo[(int)g_Player.motionType].aKeyInfo[g_Player.nKey].aKey[0];																	// 現在のモーションのキーのポインタ
	KEY* pKeyNext = &g_Player.aMotionInfo[(int)g_Player.motionType].aKeyInfo[(g_Player.nKey + 1) % g_Player.nNumKey].aKey[0];										// 現在のモーションのキーのポインタ
	float fRateKey = (float)g_Player.nCounterMotion / (float)g_Player.aMotionInfo[(int)g_Player.motionType].aKeyInfo[g_Player.nKey].nFrame;							// 現在のモーションの相対値
	Model* pMode = &g_Player.aModel[0];																																// モデル情報へのポインタ
	KEY* pKeyBlend = &g_Player.aMotionInfo[(int)g_Player.motionTypeBlend].aKeyInfo[g_Player.nKeyBlend].aKey[0];														// ブレンドモーションの現在キー
	KEY* pKeyNextBlend = &g_Player.aMotionInfo[(int)g_Player.motionTypeBlend].aKeyInfo[(g_Player.nKeyBlend + 1) % g_Player.nNumKeyBlend].aKey[0];					// ブレンドモーションの次のキー
	float fRateKeyBlend = (float)g_Player.nCounterMotionBlend / (float)g_Player.aMotionInfo[(int)g_Player.motionTypeBlend].aKeyInfo[g_Player.nKeyBlend].nFrame;		// ブレンドモーションの相対値
	float fRateBlend = (float)g_Player.nCounterBlend / (float)g_Player.nFrameBlend;																					// ブレンド状態の相対値

	for (int nCntModel = 0; nCntModel < g_Player.nNumModelParts; nCntModel++, pKey++, pKeyNext++)
	{
		if (g_Player.bBlendMotion == true)
		{// ブレンドあり

			// X座標の差分算出
			fDiffKey = pKeyNext->fPosX - pKey->fPosX;

			// X座標算出
			fPosXCurrent = pKey->fPosX + (fDiffKey * fRateKey);

			// ブレンドのX座標差分算出
			fDiffKeyBlend = pKeyNextBlend->fPosX - pKeyBlend->fPosX;

			// ブレンドのX座標算出
			fPosXBlend = pKeyBlend->fPosX + (fDiffKeyBlend * fRateKeyBlend);

			// 現在モーションとブレンドモーションの差分算出
			fDiffBlend = fPosXBlend - fPosXCurrent;

			// X座標の求める値算出
			fPosX = fPosXCurrent + (fDiffBlend * fRateBlend);

			// Y座標の差分算出
			fDiffKey = pKeyNext->fPosY - pKey->fPosY;

			// Y座標算出
			fPosYCurrent = pKey->fPosY + (fDiffKey * fRateKey);

			// ブレンドのY座標差分算出
			fDiffKeyBlend = pKeyNextBlend->fPosY - pKeyBlend->fPosY;

			// ブレンドのY座標算出
			fPosYBlend = pKeyBlend->fPosY + (fDiffKeyBlend * fRateKeyBlend);

			// 現在モーションとブレンドモーションの差分算出
			fDiffBlend = fPosYBlend - fPosYCurrent;

			// Y座標の求める値算出
			fPosY = fPosYCurrent + (fDiffBlend * fRateBlend);

			// Z座標の差分算出
			fDiffKey = pKeyNext->fPosZ - pKey->fPosZ;

			// Z座標算出
			fPosZCurrent = pKey->fPosZ + (fDiffKey * fRateKey);

			// ブレンドのZ座標差分算出
			fDiffKeyBlend = pKeyNextBlend->fPosZ - pKeyBlend->fPosZ;

			// ブレンドのZ座標算出
			fPosZBlend = pKeyBlend->fPosZ + (fDiffKeyBlend * fRateKeyBlend);

			// 現在モーションとブレンドモーションの差分算出
			fDiffBlend = fPosZBlend - fPosZCurrent;

			// Z座標の求める値算出
			fPosZ = fPosZCurrent + (fDiffBlend * fRateBlend);

			// X軸方向の向き差分算出
			fDiffKey = pKeyNext->fRotX - pKey->fRotX;

			// 角度の値補正
			AngleCorrection(&fDiffKey);

			// X軸方向の向き算出
			fRotXCurrent = pKey->fRotX + (fDiffKey * fRateKey);

			// 角度の値補正
			AngleCorrection(&fRotXCurrent);

			// ブレンドのX軸方向の向き差分算出
			fDiffKeyBlend = pKeyNextBlend->fRotX - pKeyBlend->fRotX;

			// 角度の値補正
			AngleCorrection(&fDiffKeyBlend);

			// ブレンドのX軸方向の向き算出
			fRotXBlend = pKeyBlend->fRotX + (fDiffKeyBlend * fRateKeyBlend);

			// 角度の値補正
			AngleCorrection(&fRotXBlend);

			// 現在モーションとブレンドモーションの差分算出
			fDiffBlend = fRotXBlend - fRotXCurrent;

			// 角度の値補正
			AngleCorrection(&fDiffBlend);

			// X軸方向の向きの求める値算出
			fRotX = fRotXCurrent + (fDiffBlend * fRateBlend);

			// 角度の値補正
			AngleCorrection(&fRotX);

			// Y軸方向の向き差分算出
			fDiffKey = pKeyNext->fRotY - pKey->fRotY;

			// 角度の値補正
			AngleCorrection(&fDiffKey);

			// Y軸方向の向き算出
			fRotYCurrent = pKey->fRotY + (fDiffKey * fRateKey);

			// 角度の値補正
			AngleCorrection(&fRotYCurrent);

			// ブレンドのY軸方向の向き差分算出
			fDiffKeyBlend = pKeyNextBlend->fRotY - pKeyBlend->fRotY;

			// 角度の値補正
			AngleCorrection(&fDiffKeyBlend);

			// ブレンドのY軸方向の向き算出
			fRotYBlend = pKeyBlend->fRotY + (fDiffKeyBlend * fRateKeyBlend);

			// 角度の値補正
			AngleCorrection(&fRotYBlend);

			// 現在モーションとブレンドモーションの差分算出
			fDiffBlend = fRotYBlend - fRotYCurrent;

			// 角度の値補正
			AngleCorrection(&fDiffBlend);

			// Y軸方向の向きの求める値算出
			fRotY = fRotYCurrent + (fDiffBlend * fRateBlend);

			// 角度の値補正
			AngleCorrection(&fRotY);
			
			// Z軸方向の向き差分算出
			fDiffKey = pKeyNext->fRotZ - pKey->fRotZ;

			// 角度の値補正
			AngleCorrection(&fDiffKey);

			// Z軸方向の向き算出
			fRotZCurrent = pKey->fRotZ + (fDiffKey * fRateKey);

			// 角度の値補正
			AngleCorrection(&fRotZCurrent);

			// ブレンドのZ軸方向の向き差分算出
			fDiffKeyBlend = pKeyNextBlend->fRotZ - pKeyBlend->fRotZ;

			// 角度の値補正
			AngleCorrection(&fDiffKeyBlend);

			// ブレンドのZ軸方向の向き算出
			fRotZBlend = pKeyBlend->fRotZ + (fDiffKeyBlend * fRateKeyBlend);

			// 角度の値補正
			AngleCorrection(&fRotZBlend);

			// 現在モーションとブレンドモーションの差分算出
			fDiffBlend = fRotZBlend - fRotZCurrent;

			// 角度の値補正
			AngleCorrection(&fDiffBlend);

			// Z軸方向の向きの求める値算出
			fRotZ = fRotZCurrent + (fDiffBlend * fRateBlend);

			// 角度の値補正
			AngleCorrection(&fRotZ);

			pKeyBlend++;			// ブレンドモーションの現在キーポインタを進める
			pKeyNextBlend++;		// ブレンドモーションの次キーポインタを進める
		}
		else
		{
			// 差分算出
			fDiffKey = pKeyNext->fPosX - pKey->fPosX;

			// X座標算出
			fPosXCurrent = pKey->fPosX + (fDiffKey * fRateKey);

			// 差分算出
			fDiffKey = pKeyNext->fPosY - pKey->fPosY;

			// Y座標算出
			fPosYCurrent = pKey->fPosY + (fDiffKey * fRateKey);

			// 差分算出
			fDiffKey = pKeyNext->fPosZ - pKey->fPosZ;

			// Z座標算出
			fPosZCurrent = pKey->fPosZ + (fDiffKey * fRateKey);

			// 差分算出
			fDiffKey = pKeyNext->fRotX - pKey->fRotX;

			// X方向の向き算出
			fRotXCurrent = pKey->fRotX + (fDiffKey * fRateKey);

			// 角度の値補正
			AngleCorrection(&fRotXCurrent);

			// 差分算出
			fDiffKey = pKeyNext->fRotY - pKey->fRotY;

			// Y方向の向き算出
			fRotYCurrent = pKey->fRotY + (fDiffKey * fRateKey);

			// 角度の値補正
			AngleCorrection(&fRotYCurrent);

			// 差分算出
			fDiffKey = pKeyNext->fRotZ - pKey->fRotZ;

			// Z方向の向き算出
			fRotZCurrent = pKey->fRotZ + (fDiffKey * fRateKey);

			// 角度の値補正
			AngleCorrection(&fRotZCurrent);

			// 座標・向きの設定
			fPosX = fPosXCurrent;
			fPosY = fPosYCurrent;
			fPosZ = fPosZCurrent;
			fRotX = fRotXCurrent;
			fRotY = fRotYCurrent;
			fRotZ = fRotZCurrent;

		}

		// パーツの位置・向きを設定
		g_Player.aModel[nCntModel].pos = D3DXVECTOR3(g_Player.aModel[nCntModel].posRecord.x + fPosXCurrent, g_Player.aModel[nCntModel].posRecord.y + fPosYCurrent, g_Player.aModel[nCntModel].posRecord.z + fPosZCurrent);
		g_Player.aModel[nCntModel].rot = D3DXVECTOR3(g_Player.aModel[nCntModel].rotRecord.x + fRotX, g_Player.aModel[nCntModel].rotRecord.y + fRotY, g_Player.aModel[nCntModel].rotRecord.z + fRotZ);

	}

	if (g_Player.bBlendMotion == true)
	{// ブレンドあり

		g_Player.nCounterMotionBlend++;		// モーションブレンドカウンター加算

		if (g_Player.nCounterMotionBlend >= g_Player.aMotionInfo[(int)g_Player.motionTypeBlend].aKeyInfo[g_Player.nKeyBlend].nFrame)
		{// キー切り替え

			g_Player.nCounterMotionBlend = 0;											// カウンター初期化
			g_Player.nKeyBlend = (g_Player.nKeyBlend + 1) % g_Player.nNumKeyBlend;		// キーを進める
		}

		g_Player.nCounterBlend++;		// ブレンドカウンター加算

		if (g_Player.nCounterBlend >= g_Player.nFrameBlend)
		{// ブレンドフレームに到達

			g_Player.motionType = g_Player.motionTypeBlend;				// モーション設定
			g_Player.bLoopMotion = g_Player.bLoopMotionBlend;			// ループ状態設定
			g_Player.nNumKey = g_Player.nNumKeyBlend;					// 最大キー設定
			g_Player.nCounterMotion = g_Player.nCounterMotionBlend;		// モーションカウンター設定
			g_Player.nKey = g_Player.nKeyBlend;							// 現在キー設定
			g_Player.bBlendMotion = false;								// ブレンド無効化
		}
	}
	else
	{// ブレンド無し
		g_Player.nCounterMotion++;		// カウンター加算

		if (g_Player.nCounterMotion >= g_Player.aMotionInfo[(int)g_Player.motionType].aKeyInfo[g_Player.nKey].nFrame)
		{// キー切り替え

			if (g_Player.bLoopMotion == false && g_Player.nKey == (g_Player.nNumKey - 2))
			{// 最終キーに到達 & ループしない

				if (g_Player.motionType != MOTIONTYPE_JUMP)
				{
					g_Player.bFinishMotion = true;			// 現在のモーション終了
				}
			}
			else
			{
				g_Player.nCounterMotion = 0;								// カウンター初期化
				g_Player.nKey = (g_Player.nKey + 1) % g_Player.nNumKey;		// キーを進める
			}
		}
	}
}

#endif

//=======================================================
// モーションの設定処理
//=======================================================
#if 0

void SetMosion(MOTIONTYPE motiontype, bool bBlendMotion, int nFrameBlend)
{
	if (bBlendMotion == true)
	{
		// 各値の設定
		g_Player.motionTypeBlend = motiontype;													// ブレンドモーションの種類設定
		g_Player.bLoopMotionBlend = g_Player.aMotionInfo[(int)g_Player.motionTypeBlend].bLoop;	// ブレンドモーションのループ状態設定
		g_Player.nNumKeyBlend = g_Player.aMotionInfo[(int)g_Player.motionTypeBlend].nNumKey;	// ブレンドモーションの総数キー設定
		g_Player.nKeyBlend = 0;																	// ブレンドモーションの現在のキー初期化
		g_Player.nCounterMotionBlend = 0;														// ブレンドモーションのカウンター初期化
		g_Player.nFrameBlend = nFrameBlend;														// ブレンドモーションのフレーム数設定
		g_Player.nCounterBlend = 0;																// ブレンドカウンターの初期化
		g_Player.bBlendMotion = bBlendMotion;													// ブレンドモーションをする
	}
	else
	{
		// 各値の設定
		g_Player.bBlendMotion = bBlendMotion;												// ブレンドモーションをしない
		g_Player.motionType = motiontype;													// モーションの種類の設定
		g_Player.bLoopMotion = g_Player.aMotionInfo[(int)g_Player.motionType].bLoop;		// ループ設定
		g_Player.nNumKey = g_Player.aMotionInfo[(int)g_Player.motionType].nNumKey;			// 最大キー設定
		g_Player.nKey = 0;																	// 現在キーの初期化
		g_Player.nCounterMotion = 0;														// モーションカウンターの初期化

		Model* pModel = &g_Player.aModel[0];																// モデル情報ポインタ
		KEY* pKey = &g_Player.aMotionInfo[(int)g_Player.motionType].aKeyInfo[g_Player.nKey].aKey[0];		// キー要素ポインタ

		for (int nCntParts = 0; nCntParts < g_Player.nNumModelParts; nCntParts++, pModel++, pKey++)
		{
			pModel->pos.x = pKey->fPosX;
			pModel->pos.y = pKey->fPosY;
			pModel->pos.z = pKey->fPosZ;
			pModel->rot.x = pKey->fRotX;
			pModel->rot.y = pKey->fRotY;
			pModel->rot.z = pKey->fRotZ;
		}
	}
}
#endif

//=======================================================
// プレイヤーの移動量設定処理
//=======================================================
void SetMove(D3DXVECTOR3* move, byte HitModel, bool *pbjump)
{
	if (HitModel == MODEL_HIT_FRONT)
	{// 正面(z方向の移動量を0にする)

		move->z = 0.0f;
	}
	else if (HitModel == MODEL_HIT_BACK)
	{// 後ろ(z方向の移動量を0にする)

		move->z = 0.0f;
	}
	else if (HitModel == MODEL_HIT_RIGHT)
	{// 右(x方向の移動量を0にする)

		move->x = 0.0f;
	}
	else if (HitModel == MODEL_HIT_LEFT)
	{// 左(x方向の移動量を0にする)

		move->x = 0.0f;
	}
	else if (HitModel == MODEL_HIT_TOP)
	{// 上(y方向の移動量を0にする)

		move->y = 0.0f;
		*pbjump = false;
	}
	else if (HitModel == MODEL_HIT_BOTTOM)
	{//下(y方向の移動量を0にする)

		move->y = 0.0f;
	}
}
