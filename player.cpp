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
#include"wall.h"
#include"effect.h"
#include"field.h"
#include"motion_loader.h"
#include"conveyer.h"
#include"motion.h"

// マクロ定義
#define MAX_TEXTURE				(16)						// テクスチャ数
#define MOVE_POS				(3.0f)						// 位置移動量
#define MAX_JUMP				(20.0f)						// ジャンプ量
#define MOVE_DAMPINGFUNCTION	(0.3f)						// 移動量の減衰係数
#define ANGLE_DAMPINGFUNCTION	(0.1f)						// 角度の減衰係数
#define PLAYER_GRAVITY			(0.85f)						// 重力
#define ONEPLAYER_MODELPAS		"data\\motion_muneo.txt"	// プレイヤーモデルへのパス
#define TWOPLAYER_MODELPAS		"data\\motion_muneo.txt"	// プレイヤーモデルへのパス
//#define CHARACTER_TXTNAME		"data\\character.txt"		// キャラクターテキストファイル

// グローバル変数
Player g_Player[MAX_PLAYER];						// プレイヤー情報
const char* c_apFilenamePlayer[MAX_PLAYER] =		// プレイヤーのxファイル
{
	ONEPLAYER_MODELPAS,
	TWOPLAYER_MODELPAS
};

// プレイヤー別操作キー [前キー、後ろキー、左キー、右キー、ジャンプキー、インタラクトキー]
BYTE g_playerControlKey[MAX_PLAYER][6] = {
	{DIK_W, DIK_S, DIK_A, DIK_D, DIK_SPACE, DIK_RETURN},
	{DIK_UP, DIK_DOWN, DIK_LEFT, DIK_RIGHT, DIK_RSHIFT, DIK_NUMPAD1},
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

	// モーションの初期化
	InitMotion(&g_Player[0].PlayerMotion, ONEPLAYER_MODELPAS);
	InitMotion(&g_Player[1].PlayerMotion, TWOPLAYER_MODELPAS);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		// 影のインデックス設定
		g_Player[nCntPlayer].nIdxShadow = SetShadow(D3DXVECTOR3(g_Player[nCntPlayer].pos.x, g_Player[nCntPlayer].pos.y + 1.0f, g_Player[nCntPlayer].pos.z), 50.0f);

		SetMotion(&g_Player[nCntPlayer].PlayerMotion, 0, 0);
	}
}

//=======================================================
// プレイヤーの終了処理
//=======================================================
void UninitPlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		UninitMotion(&g_Player[nCntPlayer].PlayerMotion);

	}
}

//=======================================================
// プレイヤーの更新処理
//=======================================================
void UpdatePlayer(void)
{
	CAMERA* pCamera = GetCamera(0);		// カメラ情報の取得
	Player* pPlayer = GetPlayer();		// プレイヤー
	float fRotDiff = 0.0f;				// 角度の差分代入
	int nValueH, nValueV;				// スティック値代入

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		// 前回の位置保存
		g_Player[nCntPlayer].posOld = g_Player[nCntPlayer].pos;

		D3DXVECTOR3 move = D3DXVECTOR3_ZERO;

		if (GetKeyboardPress(g_playerControlKey[nCntPlayer][0]) == true || GetJoypadPress(JOYKEY_UP, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_UP, nCntPlayer) == TRUE)
		{// Wキー入力(Z軸+の方向に移動)
			move.x += sinf(pCamera->rot.y);
			move.z += cosf(pCamera->rot.y);
		}
		if (GetKeyboardPress(g_playerControlKey[nCntPlayer][1]) == true || GetJoypadPress(JOYKEY_DOWN, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_DOWN, nCntPlayer) == TRUE)
		{/// Sキー入力(Z軸-の方向に移動)
			move.x -= sinf(pCamera->rot.y);
			move.z -= cosf(pCamera->rot.y);
		}
		if (GetKeyboardPress(g_playerControlKey[nCntPlayer][2]) == true || GetJoypadPress(JOYKEY_LEFT, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_LEFT, nCntPlayer) == TRUE)
		{// Aキー入力(X軸-の方向に移動)
			move.x -= sinf(pCamera->rot.y + D3DX_PI * 0.5f);
			move.z -= cosf(pCamera->rot.y + D3DX_PI * 0.5f);
		}
		if (GetKeyboardPress(g_playerControlKey[nCntPlayer][3]) == true || GetJoypadPress(JOYKEY_RIGHT, nCntPlayer) == true || GetJoystickPress(JOYSTICK_L_RIGHT, nCntPlayer) == TRUE)
		{// Dキー入力(X軸+の方向に移動)
			move.x += sinf(pCamera->rot.y + D3DX_PI * 0.5f);
			move.z += cosf(pCamera->rot.y + D3DX_PI * 0.5f);
		}

		if (Magnitude(move) != 0)
		{
			move = Normalize(move);
			g_Player[nCntPlayer].move.x += move.x * MOVE_POS;
			g_Player[nCntPlayer].move.z += move.z * MOVE_POS;

			g_Player[nCntPlayer].rotmove.y = atan2f(move.x, move.z) + D3DX_PI;
		}

		if (GetKeyboardTrigger(g_playerControlKey[nCntPlayer][4]) == true || GetJoypadPress(JOYKEY_A, nCntPlayer) == true)
		{// ジャンプ押下

			if (g_Player[nCntPlayer].bJump == false)
			{// ジャンプする

				g_Player[nCntPlayer].bJump = true;		// ジャンプ中にする
				g_Player[nCntPlayer].move.y = MAX_JUMP;	// ジャンプ量

				// ジャンプモーションに変更
				SetMotion(&g_Player[nCntPlayer].PlayerMotion, 3, 20);
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

		// 壁との当たり判定
		CollisionWall(&g_Player[nCntPlayer].pos, g_Player[nCntPlayer].posOld, &g_Player[nCntPlayer].move, D3DXVECTOR3_ZERO);

		// モデルとの当たり判定
		g_Player[nCntPlayer].ModelHit = CollisionModel(&g_Player[nCntPlayer].pos, g_Player[nCntPlayer].posOld, D3DXVECTOR3(200.0f, 200.0f, 500.0f));

		//床との当たり判定
		if (CollisionField(&g_Player[nCntPlayer].pos, g_Player[nCntPlayer].posOld))
		{
			g_Player[nCntPlayer].bJump = false;		// ジャンプ状態解除
			g_Player[nCntPlayer].move.y = 0.0f;		// 重力リセット
		}
		else if (g_Player[nCntPlayer].ModelHit != MODEL_HIT_NONE)
		{// 何かに当たった

			// プレイヤーの移動量の設定
			SetMove(&g_Player[nCntPlayer].move, g_Player[nCntPlayer].ModelHit, &g_Player[nCntPlayer].bJump);
		}
		else
		{// 落下中

			if (g_Player[nCntPlayer].bJump == false)
			{// ジャンプ状態にする

				g_Player[nCntPlayer].bJump = true;		// ジャンプ中にする
			}
		}

		// ゴールとの当たり判定
		CollisionGoal(&g_Player[nCntPlayer].pos, &g_Player[nCntPlayer].posOld, &g_Player[nCntPlayer].move, g_Player[nCntPlayer].fRadius);

		// 火炎放射器との当たり判定
		CollisionFlamethrower(&g_Player[nCntPlayer].pos, &g_Player[nCntPlayer].posOld, &g_Player[nCntPlayer].move, g_Player[nCntPlayer].fRadius);

		// コンベアとの当たり判定
		if (CollisioncConveyer(&g_Player[nCntPlayer].pos, &g_Player[nCntPlayer].posOld, &g_Player[nCntPlayer].move))
		{
			if (g_Player[nCntPlayer].bJump == true)
			{// ジャンプ中の場合ジャンプ状態を解除する

				g_Player[nCntPlayer].bJump = false;
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
		SetPositionShadow(g_Player[nCntPlayer].nIdxShadow, g_Player[nCntPlayer].pos, 0.0f/*(g_Player[nCntPlayer].pos.y - g_Player[nCntPlayer].posOld.y) / 10.0f*/);

		for (int nCntPart = 0; nCntPart < g_Player[nCntPlayer].PlayerMotion.nNumPart; nCntPart++)
		{
			PART* pPart = &g_Player[nCntPlayer].PlayerMotion.aPart[nCntPart];
			pPart->pos = pPart->posOffset;
			pPart->rot = pPart->rotOffset;
		}

		if (g_Player[nCntPlayer].move.x >= 0.3f && g_Player[nCntPlayer].PlayerMotion.nIdxMotionBlend != 1 && g_Player[nCntPlayer].PlayerMotion.nIdxMotionBlend != 3
			||g_Player[nCntPlayer].move.x <= -0.3f && g_Player[nCntPlayer].PlayerMotion.nIdxMotionBlend != 1 && g_Player[nCntPlayer].PlayerMotion.nIdxMotionBlend != 3
			|| g_Player[nCntPlayer].move.z >= 0.3f && g_Player[nCntPlayer].PlayerMotion.nIdxMotionBlend != 1 && g_Player[nCntPlayer].PlayerMotion.nIdxMotionBlend != 3
			|| g_Player[nCntPlayer].move.z <= -0.3f && g_Player[nCntPlayer].PlayerMotion.nIdxMotionBlend != 1 && g_Player[nCntPlayer].PlayerMotion.nIdxMotionBlend != 3)
		{// 移動モーションに変更(moveの値が一定以上ある時&ジャンプ中モーション以外の時)

			SetMotion(&g_Player[nCntPlayer].PlayerMotion, 1, 30);
		}

		if (g_Player[nCntPlayer].move.x < 0.3f && g_Player[nCntPlayer].move.x > -0.3f 
			&& g_Player[nCntPlayer].move.z < 0.3f && g_Player[nCntPlayer].move.z > -0.3f
			&& g_Player[nCntPlayer].PlayerMotion.nIdxMotionBlend != 0 && g_Player[nCntPlayer].PlayerMotion.nIdxMotionBlend != 3)
		{// 待機モーションに変更(移動していない&ジャンプモーション以外の時)

			if (g_Player[nCntPlayer].PlayerMotion.nIdxMotionBlend == 4
				&& g_Player[nCntPlayer].PlayerMotion.bFinishMotion == true
				|| g_Player[nCntPlayer].PlayerMotion.nIdxMotionBlend != 4)
			{// 着地モーション終了時or着地モーション以外

				SetMotion(&g_Player[nCntPlayer].PlayerMotion, 0, 30);
			}
		}

		if (g_Player[nCntPlayer].posOld.y >= g_Player[nCntPlayer].pos.y && g_Player[nCntPlayer].move.y <= 0.0f && g_Player[nCntPlayer].PlayerMotion.nIdxMotionBlend == 3)
		{// 着地モーション(posがposOldよりも低い位置&move.yが0&ジャンプ状態)

			SetMotion(&g_Player[nCntPlayer].PlayerMotion, 4, 30);
		}

		// モーションの更新処理
		UpdateMotion(&g_Player[nCntPlayer].PlayerMotion);
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
	Player* pPlayer = GetPlayer();

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pPlayer++)
	{
		pDevice->GetMaterial(&matDef);

		// プレイヤーのマトリックスを設定
		MatrixRotationPosition(&pPlayer->mtxWorld, pPlayer->rot, pPlayer->pos);

		for (int nCntPart = 0; nCntPart < pPlayer->PlayerMotion.nNumPart; nCntPart++)
		{
			D3DXMATRIX mtxRotModel, mtxTransModel;		// 計算用マトリックス
			D3DXMATRIX mtxParent;						// 親のマトリックス

			// パーツへのポインタを取得
			PART* pPart = &pPlayer->PlayerMotion.aPart[nCntPart];

			// パーツのワールドマトリックスの初期化
			MatrixRotationPosition(&pPart->mtxWorld, pPart->rot, pPart->pos);

			// パーツの「親マトリックス」を設定
			if (pPart->nIdxModelParent != -1)
			{// 親モデルがある場合
				// 親のインデックスを使用して、同じプレイヤーのパーツ配列から取得
				int parentIdx = pPart->nIdxModelParent;
				mtxParent = pPlayer->PlayerMotion.aPart[parentIdx].mtxWorld;
			}
			else
			{// 親モデルが無い場合
				mtxParent = pPlayer->mtxWorld;
			}

			// 算出した「パーツのワールドマトリックス」と「親のマトリックス」をかけ合わせる
			D3DXMatrixMultiply(&pPart->mtxWorld, &pPart->mtxWorld, &mtxParent);

			// パーツのワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pPart->mtxWorld);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)pPart->pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)pPart->dwNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャ設定
				pDevice->SetTexture(0, pPart->apTexture[nCntMat]);

				// パーツの描画
				pPart->pMesh->DrawSubset(nCntMat);
			}
		}

	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
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
void SetMove(D3DXVECTOR3* move, byte HitModel, bool* pbjump)
{
	if (HitModel & MODEL_HIT_FRONT)
	{// 正面(z方向の移動量を0にする)

		move->z = 0.0f;
	}
	else if (HitModel & MODEL_HIT_BACK)
	{// 後ろ(z方向の移動量を0にする)

		move->z = 0.0f;
	}
	else if (HitModel & MODEL_HIT_RIGHT)
	{// 右(x方向の移動量を0にする)

		move->x = 0.0f;
	}
	else if (HitModel & MODEL_HIT_LEFT)
	{// 左(x方向の移動量を0にする)

		move->x = 0.0f;
	}

	if (HitModel & MODEL_HIT_TOP)
	{// 上(y方向の移動量を0にする)

		move->y = 0.0f;
		*pbjump = false;
	}
	else if (HitModel & MODEL_HIT_BOTTOM)
	{//下(y方向の移動量を0にする)

		move->y = 0.0f;
	}
}
