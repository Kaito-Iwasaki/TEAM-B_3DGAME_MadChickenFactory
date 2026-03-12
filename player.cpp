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
#include"gate.h"
#include"lift.h"
#include"team_logo.h"

// マクロ定義
#define MAX_TEXTURE				(16)						// テクスチャ数
#define MOVE_POS				(5.0f)						// 位置移動量
#define ELIGIBLE_MOVE			(15.0f)						// 追従移動量
#define MAX_JUMP				(20.0f)						// ジャンプ量
#define MOVE_DAMPINGFUNCTION	(0.3f)						// 移動量の減衰係数
#define ANGLE_DAMPINGFUNCTION	(0.1f)						// 角度の減衰係数
#define PLAYER_GRAVITY			(0.85f)						// 重力
#define ONEPLAYER_MODELPAS		"data\\motion_muneo.txt"	// プレイヤーモデルへのパス
#define TWOPLAYER_MODELPAS		"data\\motion_momoko.txt"	// プレイヤーモデルへのパス
#define DEATHLINE				(-500.0f)					// 落下死亡判定の高さ
//#define CHARACTER_TXTNAME		"data\\character.txt"		// キャラクターテキストファイル

// グローバル変数
Player g_Player[MAX_PLAYER];						// プレイヤー情報
PLAYEROPERATION g_Operation;						// 操作プレイヤー指定
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
	int pStart = GetTitle();								// プレイ人数代入

#if 0
	MOTION_INFO* pMosionInfo = &g_Player.aMotionInfo[0];	// モーション情報ポインタ
	KEY_INFO* pKeyInfo = &pMosionInfo->aKeyInfo[0];			// キー情報ポインタ
	KEY* pKey = &pKeyInfo->aKey[0];							// キー要素ポインタ
	Model* pModel = &g_Player.aModel[0];					// モデル情報ポインタ
#endif

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_Player[nCntPlayer].pos = INIT_PLAYER_POS;		// 現在の位置初期化
		g_Player[nCntPlayer].posOld = INIT_PLAYER_POS;		// 前回の位置代入
		g_Player[nCntPlayer].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量初期化
		g_Player[nCntPlayer].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);		// 向き初期化
		g_Player[nCntPlayer].rotmove = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);	// 向きの移動量初期化
		g_Player[nCntPlayer].fAngle = 0.0f;									// キャラの進む方向初期化
		g_Player[nCntPlayer].fStandPos = 0.0f;								// 現在の立つ位置初期化
		g_Player[nCntPlayer].nIdxShadow = -1;								// 対象の影のインデックス(番号)初期化
		g_Player[nCntPlayer].bJump = false;									// ジャンプ状態初期化
		g_Player[nCntPlayer].ModelHit = MODEL_HIT_NONE;						// 当たっていない状態にする
		g_Player[nCntPlayer].fRadius = 40.0f;								// 半径初期化
		g_Player[nCntPlayer].fHeight = 100.0f;								// 高さ初期化
		g_Player[nCntPlayer].bDisableControl = false;						// 操作受け付け状態に設定
		g_Player[nCntPlayer].bDisableFollow = false;						// 追従受け付け状態に設定
		g_Player[nCntPlayer].bUse = true;									// 使用状態にする
		g_Player[nCntPlayer].nIdx = nCntPlayer;
	}

	// モーションの初期化
	InitMotion(&g_Player[0].PlayerMotion, ONEPLAYER_MODELPAS);
	InitMotion(&g_Player[1].PlayerMotion, TWOPLAYER_MODELPAS);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (g_Player[nCntPlayer].bUse == true)
		{// 使用している場合

			// 影のインデックス設定
			g_Player[nCntPlayer].nIdxShadow = SetShadow(D3DXVECTOR3(g_Player[nCntPlayer].pos.x, g_Player[nCntPlayer].pos.y + 1.0f, g_Player[nCntPlayer].pos.z), 50.0f);

			SetMotion(&g_Player[nCntPlayer].PlayerMotion, MOTIONTYPE_NEUTRAL, 0);

		}
	}
	int pStaet = GetTitle();		// プレイ人数情報取得

	if (pStaet == 0)
	{// ソロプレイ

		g_Operation = PLAYEROPERATION_1P;
	}
	else
	{// 2人プレイ

		g_Operation = PLAYEROPERATION_2PL;
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

		if (g_Player[nCntPlayer].bDisableControl == false)
		{// 操作可能

			// 前回の位置保存
			g_Player[nCntPlayer].posOld = g_Player[nCntPlayer].pos;

			if (g_Operation == PLAYEROPERATION_2PL || g_Operation == (PLAYEROPERATION)nCntPlayer)
			{// 操作プレイヤーと一致

				if (g_Operation == PLAYEROPERATION_2PL)
				{// 2PL操作(それぞれのコントローラーで移動)

					// プレイヤーの移動処理
					PlayerMoveControl(&g_Player[nCntPlayer], nCntPlayer);
				}
				else
				{// 1PL操作(1つのコントローラーで移動)

					// プレイヤーの移動処理
					PlayerMoveControl(&g_Player[nCntPlayer], 0);
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

			// 当たり判定処理
			CollisionPlayer(&g_Player[nCntPlayer], nCntPlayer);

			// 目標の移動方向までの差分算出
			fRotDiff = g_Player[nCntPlayer].rotmove.y - g_Player[nCntPlayer].rot.y;

			// 角度の値補正
			fRotDiff = GetFixedRotation(fRotDiff);

			// 移動方向(角度)の補正
			g_Player[nCntPlayer].rot.y += fRotDiff * ANGLE_DAMPINGFUNCTION;

			// 角度の値補正
			g_Player[nCntPlayer].rot.y = GetFixedRotation(g_Player[nCntPlayer].rot.y);

			// 影の位置を設定(更新)
			if (g_Player[nCntPlayer].move.y == 0.0f)
			{// 現在の着地している高さに影を設定

				SetPositionShadow(g_Player[nCntPlayer].nIdxShadow,
					D3DXVECTOR3(g_Player[nCntPlayer].pos.x, g_Player[nCntPlayer].fStandPos, g_Player[nCntPlayer].pos.z),
					0.0f, true);

			}
			else
			{// y座標変更無し

				SetPositionShadow(g_Player[nCntPlayer].nIdxShadow,
					D3DXVECTOR3(g_Player[nCntPlayer].pos.x, g_Player[nCntPlayer].fStandPos, g_Player[nCntPlayer].pos.z),
					(g_Player[nCntPlayer].pos.y - g_Player[nCntPlayer].posOld.y) / 10.0f, false);
			}

			for (int nCntPart = 0; nCntPart < g_Player[nCntPlayer].PlayerMotion.nNumPart; nCntPart++)
			{
				PART* pPart = &g_Player[nCntPlayer].PlayerMotion.aPart[nCntPart];
				pPart->pos = pPart->posOffset;
				pPart->rot = pPart->rotOffset;
			}

			if (
				Magnitude(D3DXVECTOR3(g_Player[nCntPlayer].move.x, 0, g_Player[nCntPlayer].move.z)) >= 0.3f &&
				g_Player[nCntPlayer].PlayerMotion.nIdxMotionBlend != MOTIONTYPE_MOVE &&
				g_Player[nCntPlayer].PlayerMotion.nIdxMotionBlend != MOTIONTYPE_JUMP &&
				g_Player[nCntPlayer].PlayerMotion.nIdxMotionBlend != MOTIONTYPE_PREPARATIONFOREXTRUSION
				)
			{// 移動モーションに変更(moveの値が一定以上ある時&ジャンプ中モーション以外の時)

				SetMotion(&g_Player[nCntPlayer].PlayerMotion, MOTIONTYPE_MOVE, 30);
			}

			if (g_Player[nCntPlayer].move.x < 0.3f && g_Player[nCntPlayer].move.x > -0.3f
				&& g_Player[nCntPlayer].move.z < 0.3f && g_Player[nCntPlayer].move.z > -0.3f
				&& g_Player[nCntPlayer].PlayerMotion.nIdxMotionBlend != MOTIONTYPE_NEUTRAL && g_Player[nCntPlayer].PlayerMotion.nIdxMotionBlend != MOTIONTYPE_JUMP)
			{// 待機モーションに変更(移動していない&ジャンプモーション以外の時)

				if (g_Player[nCntPlayer].PlayerMotion.nIdxMotionBlend == MOTIONTYPE_LANDING
					&& g_Player[nCntPlayer].PlayerMotion.bFinishMotion == true)
				{// 着地モーション終了時

					SetMotion(&g_Player[nCntPlayer].PlayerMotion, MOTIONTYPE_NEUTRAL, 0);
				}
				else if (g_Player[nCntPlayer].PlayerMotion.nIdxMotionBlend != MOTIONTYPE_LANDING)
				{// 着地モーション以外

					SetMotion(&g_Player[nCntPlayer].PlayerMotion, MOTIONTYPE_NEUTRAL, 30);
				}
			}

			if (g_Player[nCntPlayer].posOld.y == g_Player[nCntPlayer].pos.y && g_Player[nCntPlayer].move.y <= 0.0f && g_Player[nCntPlayer].PlayerMotion.nIdxMotionBlend == MOTIONTYPE_JUMP)
			{// 着地モーション(posがposOldよりも低い位置&move.yが0&ジャンプ状態)

				if (g_Player[nCntPlayer].move.x >= 0.3f || g_Player[nCntPlayer].move.x <= -0.3f || g_Player[nCntPlayer].move.z >= 0.3f || g_Player[nCntPlayer].move.z <= -0.3f)
				{// 移動モーションに設定

					SetMotion(&g_Player[nCntPlayer].PlayerMotion, MOTIONTYPE_MOVE, 10);
				}
				else
				{
					SetMotion(&g_Player[nCntPlayer].PlayerMotion, MOTIONTYPE_LANDING, 10);
				}
			}
		}
		else if (g_Player[nCntPlayer].bDisableControl == true)
		{// 操作不能

			// 重力を加える
			g_Player[nCntPlayer].move.y -= PLAYER_GRAVITY;

			// 位置を更新
			g_Player[nCntPlayer].pos.x += g_Player[nCntPlayer].move.x;
			g_Player[nCntPlayer].pos.y += g_Player[nCntPlayer].move.y;
			g_Player[nCntPlayer].pos.z += g_Player[nCntPlayer].move.z;

			// 当たり判定処理
			CollisionPlayer(&g_Player[nCntPlayer], nCntPlayer);

			if (g_Player[nCntPlayer].PlayerMotion.nIdxMotionBlend == MOTIONTYPE_ACTION && g_Player[nCntPlayer].PlayerMotion.bFinishMotion == true)
			{// 死亡モーション終了

				FADE pFade = GetFade();				// フェード状態取得処理

				if (pFade.state == FADESTATE_NONE)
				{// フェード状態ではない

					// フェード処理(ゲーム画面に移行)
					SetFade(MODE_GAME);
				}
			}
		}

		// モーションの更新処理
		UpdateMotion(&g_Player[nCntPlayer].PlayerMotion);

		if (g_Player[nCntPlayer].pos.y <= DEATHLINE && g_Player[nCntPlayer].PlayerMotion.nIdxMotionBlend != MOTIONTYPE_ACTION)
		{// 落下した

			KillPlayer(&g_Player[nCntPlayer]);
		}

#ifdef _DEBUG

		// ステージ移動(デバック用)
		if (GetKeyboardTrigger(DIK_1) == true)
		{// 1ステージ移動

			g_Player[nCntPlayer].pos = D3DXVECTOR3(-2000.0f, 0.0f, 800.0f);
		}
		else if (GetKeyboardTrigger(DIK_2) == true)
		{// 2ステージ移動

			g_Player[nCntPlayer].pos = D3DXVECTOR3(8000.0f, 0.0f, 0.0f);
		}
		else if (GetKeyboardTrigger(DIK_3) == true)
		{// 3ステージ移動

			g_Player[nCntPlayer].pos = D3DXVECTOR3(11900.0f, 0.0f, 0.0f);
		}
		else if (GetKeyboardTrigger(DIK_4) == true)
		{// 4ステージ移動

			g_Player[nCntPlayer].pos = D3DXVECTOR3(16000.0f, 0.0f, 0.0f);
		}
		else if (GetKeyboardTrigger(DIK_5) == true)
		{// 4ステージ移動

			g_Player[nCntPlayer].pos = D3DXVECTOR3(19400.0f, 0.0f, 0.0f);
		}
		else if (GetKeyboardTrigger(DIK_0) == true)
		{// 4ステージ移動

			g_Player[nCntPlayer].pos = D3DXVECTOR3(23200.0f, 0.0f, 0.0f);
		}
#endif
	}

	if (g_Player[0].pos.x - g_Player[1].pos.x >= PLAYER_BETWEEN || g_Player[0].pos.x - g_Player[1].pos.x <= -PLAYER_BETWEEN)
	{// プレイヤー間の距離が最大値を超えた

		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		{// 位置を戻す

			g_Player[nCntPlayer].pos.x = g_Player[nCntPlayer].posOld.x;
		}
	}

	if (GetKeyboardTrigger(DIK_LSHIFT) == true && g_Operation != PLAYEROPERATION_2PL
		|| GetJoypadTrigger(JOYKEY_X, 0) == true && g_Operation != PLAYEROPERATION_2PL)
	{// 左シフトorXボタン入力

		g_Operation = (PLAYEROPERATION)(g_Operation ^ 1);		// 操作プレイヤー切り替え

		if (g_Operation == PLAYEROPERATION_1P)
		{// カメラを1Pにフォーカス

			GetCamera(0)->mode = CAMERAMODE_SIDEVIEWFOCUS1;
		}
		else if (g_Operation == PLAYEROPERATION_2P)
		{
			GetCamera(0)->mode = CAMERAMODE_SIDEVIEWFOCUS2;
		}
	}

	if (GetKeyboardPress(DIK_LCONTROL) == true && g_Operation != PLAYEROPERATION_2PL
		|| GetJoypadPress(JOYKEY_Y, 0) == true && g_Operation != PLAYEROPERATION_2PL)
	{// プレイヤー追従処理

		PlayerFollow(&g_Player[(int)g_Operation], &g_Player[(int)g_Operation ^ 1]);
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

		if (pPlayer->bUse == true)
		{// 使用している場合

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
// プレイヤーの移動量設定処理
//=======================================================
void SetMove(Player *pPlayer)
{
	if (pPlayer->ModelHit & MODEL_HIT_FRONT)
	{// 正面(z方向の移動量を0にする)

		pPlayer->move.z = 0.0f;
	}
	else if (pPlayer->ModelHit & MODEL_HIT_BACK)
	{// 後ろ(z方向の移動量を0にする)

		pPlayer->move.z = 0.0f;
	}
	else if (pPlayer->ModelHit & MODEL_HIT_RIGHT)
	{// 右(x方向の移動量を0にする)

		pPlayer->move.x = 0.0f;
	}
	else if (pPlayer->ModelHit & MODEL_HIT_LEFT)
	{// 左(x方向の移動量を0にする)

		pPlayer->move.x = 0.0f;
	}

	if (pPlayer->ModelHit & MODEL_HIT_TOP)
	{// 上(y方向の移動量を0にする)

		pPlayer->move.y = 0.0f;
		pPlayer->bJump = false;
		pPlayer->fStandPos = pPlayer->pos.y;
	}
	else if (pPlayer->ModelHit & MODEL_HIT_BOTTOM)
	{//下(y方向の移動量を0にする)

		pPlayer->move.y = 0.0f;
	}
}

//=======================================================
// プレイヤーの死亡処理
//=======================================================
void KillPlayer(Player *pPlayer)
{
	pPlayer->bDisableControl = true;		// 操作不能にする

	// モーションの設定
	SetMotion(&pPlayer->PlayerMotion, MOTIONTYPE_ACTION, 30);

	switch (pPlayer->nIdx)
	{
	case 0:
		PlaySound(SOUND_LABEL_SE_DIED00);
		break;

	case 1:
		PlaySound(SOUND_LABEL_SE_DIED01);
		break;

	default:
		break;
	}
}

//=======================================================
// プレイヤーとの当たり判定処理
//=======================================================
void CollisionPlayer(Player* pPlayer, int nCntPlayer)
{
	// 壁との当たり判定
	CollisionWall(&pPlayer->pos, pPlayer->posOld, &pPlayer->move, D3DXVECTOR3_ZERO);

	// モデルとの当たり判定
	pPlayer->ModelHit = CollisionModel(&pPlayer->pos, pPlayer->posOld, D3DXVECTOR3(pPlayer->fRadius, pPlayer->fHeight, pPlayer->fRadius));

	//床との当たり判定
	if (CollisionField(&pPlayer->pos, pPlayer->posOld))
	{
		pPlayer->bJump = false;						// ジャンプ状態解除
		pPlayer->move.y = 0.0f;						// 重力リセット
		pPlayer->fStandPos = pPlayer->pos.y;		// 立っている位置更新
	}
	else if (pPlayer->ModelHit != MODEL_HIT_NONE)
	{// 何かに当たった

		// プレイヤーの移動量の設定
		SetMove(pPlayer);
	}
	else
	{// 落下中

		if (pPlayer->bJump == false)
		{// ジャンプ状態にする

			pPlayer->bJump = true;		// ジャンプ中にする
		}
	}

	// ゴールとの当たり判定
	CollisionGoal(&pPlayer->pos, &pPlayer->posOld, &pPlayer->move, pPlayer->fRadius);

	// 火炎放射器との当たり判定
	CollisionFlamethrower(&pPlayer->pos, &pPlayer->posOld, &pPlayer->move, pPlayer->fRadius, &pPlayer->bJump, &pPlayer->fStandPos);

	// ゲートとの当たり判定
	CollisionGate(&pPlayer->pos, &pPlayer->posOld, &pPlayer->move, pPlayer->fRadius);

	// コンベアとの当たり判定
	if (CollisioncConveyer(&pPlayer->pos, &pPlayer->posOld, &pPlayer->move, &pPlayer->fStandPos))
	{
		if (pPlayer->bJump == true)
		{// ジャンプ中の場合ジャンプ状態を解除する

			pPlayer->bJump = false;
		}
	}

	// リフトとの当たり判定
	CollisionLift(pPlayer);

	// 可動箱の当たり判定
	CollisionMoveBox(nCntPlayer);
}

//=======================================================
// プレイヤーの移動処理
//=======================================================
void PlayerMoveControl(Player* pPlayer, int nCntControl)
{
	CAMERA* pCamera = GetCamera(0);			// カメラ情報の取得
	D3DXVECTOR3 move = D3DXVECTOR3_ZERO;	// move代入用

	if (GetKeyboardPress(g_playerControlKey[nCntControl][0]) == true || GetJoypadPress(JOYKEY_UP, nCntControl) == true || GetJoystickPress(JOYSTICK_L_UP, nCntControl) == TRUE)
	{// Wキー入力(Z軸+の方向に移動)
		move.x += sinf(pCamera->rot.y);
		move.z += cosf(pCamera->rot.y);
	}
	if (GetKeyboardPress(g_playerControlKey[nCntControl][1]) == true || GetJoypadPress(JOYKEY_DOWN, nCntControl) == true || GetJoystickPress(JOYSTICK_L_DOWN, nCntControl) == TRUE)
	{/// Sキー入力(Z軸-の方向に移動)
		move.x -= sinf(pCamera->rot.y);
		move.z -= cosf(pCamera->rot.y);
	}
	if (GetKeyboardPress(g_playerControlKey[nCntControl][2]) == true || GetJoypadPress(JOYKEY_LEFT, nCntControl) == true || GetJoystickPress(JOYSTICK_L_LEFT, nCntControl) == TRUE)
	{// Aキー入力(X軸-の方向に移動)
		move.x -= sinf(pCamera->rot.y + D3DX_PI * 0.5f);
		move.z -= cosf(pCamera->rot.y + D3DX_PI * 0.5f);
	}
	if (GetKeyboardPress(g_playerControlKey[nCntControl][3]) == true || GetJoypadPress(JOYKEY_RIGHT, nCntControl) == true || GetJoystickPress(JOYSTICK_L_RIGHT, nCntControl) == TRUE)
	{// Dキー入力(X軸+の方向に移動)
		move.x += sinf(pCamera->rot.y + D3DX_PI * 0.5f);
		move.z += cosf(pCamera->rot.y + D3DX_PI * 0.5f);
	}

	if (Magnitude(move) != 0)
	{
		move = Normalize(move);
		pPlayer->move.x += move.x * MOVE_POS;
		pPlayer->move.z += move.z * MOVE_POS;

		pPlayer->rotmove.y = atan2f(move.x, move.z) + D3DX_PI;
	}

	if (GetKeyboardTrigger(g_playerControlKey[nCntControl][4]) == true || GetJoypadPress(JOYKEY_A, nCntControl) == true)
	{// ジャンプ押下

		if (pPlayer->bJump == false)
		{// ジャンプする

			pPlayer->bJump = true;		// ジャンプ中にする
			pPlayer->move.y = MAX_JUMP;	// ジャンプ量

			// ジャンプモーションに変更
			SetMotion(&pPlayer->PlayerMotion, MOTIONTYPE_JUMP, 20);
		}
	}
}

//=======================================================
// プレイヤーの追従処理
//=======================================================
void PlayerFollow(Player *pTargetPlayer, Player *EligiblePlayer)
{
	if (EligiblePlayer->bDisableFollow) return;
	if (Magnitude(EligiblePlayer->pos, pTargetPlayer->pos) < 100) return;	// 一定距離近かったら止まる

	float fRot = 0.0f;		// 角度代入用

	// 角度算出
	fRot = atan2f((pTargetPlayer->pos.x - EligiblePlayer->pos.x), (pTargetPlayer->pos.z - EligiblePlayer->pos.z));
	fRot = GetFixedRotation(fRot);

	// 移動量設定
	EligiblePlayer->move.x = sinf(fRot) * ELIGIBLE_MOVE;
	EligiblePlayer->move.z = cosf(fRot) * ELIGIBLE_MOVE;

	// プレイヤーの向き設定
	EligiblePlayer->rotmove.y = fRot + D3DX_PI;

}

//=======================================================
// 操作プレイヤー情報取得処理
//=======================================================
PLAYEROPERATION GetPlayerOperation(void)
{
	return g_Operation;
}