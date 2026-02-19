//==================================================
//
// ギミックの距離からSEを調整[SE_controller.cpp]
// Author : Keitaro Nagate
//
//==================================================

//==================================================
//
//	ファイルインクルード
//
//==================================================
#include "main.h"
#include "SE_controller.h"
#include "player.h"
#include "util.h"
#include "math.h"
#include "DebugProc.h"
#include "input.h"

//==================================================
//
//	マクロ定義
//
//==================================================
#define MAX_SOUNDSPOT			(1024)		// SEの発生地点の最大個数
#define AUDIBLE_DISTANCE		(1500.0f)	// SEが聞こえる距離

//==================================================
//
//	列挙型
//
//==================================================

//==================================================
//
//	構造体定義
//
//==================================================

//==================================================
//
//	グローバル変数宣言
//
//==================================================
SoundSpot g_aSoundSpot[MAX_SOUNDSPOT];			// SEの発生位置

//==================================================
//
//	SEコントローラー初期化
//
//==================================================
void InitSEController(void)
{
	for (int nCntSEC = 0; nCntSEC < MAX_SOUNDSPOT; nCntSEC++)
	{
		g_aSoundSpot[nCntSEC].pos = D3DXVECTOR3_ZERO;
		g_aSoundSpot[nCntSEC].Sound = SOUND_LABEL_MAX;
		g_aSoundSpot[nCntSEC].bwithin = false;
		g_aSoundSpot[nCntSEC].bUse = false;
	}
}

//==================================================
//
//	SEコントローラー更新
//
//==================================================
void UpdateSEController(void)
{
	if (GetKeyboardTrigger(DIK_0))
	{
		PlaySound(SOUND_LABEL_SE_PRESS);
	}
	if (GetKeyboardTrigger(DIK_9))
	{
		PlaySound(SOUND_LABEL_SE_SAW);
	}
	if (GetKeyboardTrigger(DIK_8))
	{
		StopSound(SOUND_LABEL_SE_SAW);
	}

	UpdateSound();

	float fSoundLabelVolume[SOUND_LABEL_MAX] = {};	// ラベルごとの音量
	Player* pPlayer = GetPlayer();
	float Distance;	// プレイヤーとオブジェクトサウンドとの距離

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntSEC = 0; nCntSEC < MAX_SOUNDSPOT; nCntSEC++)
		{
			if (g_aSoundSpot[nCntSEC].bUse == true)
			{
				Distance = Magnitude(g_aSoundSpot[nCntSEC].pos, pPlayer->pos);

				if (Distance <= AUDIBLE_DISTANCE)
				{// 範囲内
					// 再生するボリュームを取得
					g_aSoundSpot[nCntSEC].fVolume = 1.0f - (Distance / AUDIBLE_DISTANCE);

					if (GetSoundState(g_aSoundSpot[nCntSEC].Sound, g_aSoundSpot[nCntSEC].nIdx) == NULL)
					{// 情報を取得出来ない
						PrintDebugProc("これ、ないです");
					}
					else if (GetSoundState(g_aSoundSpot[nCntSEC].Sound, g_aSoundSpot[nCntSEC].nIdx)->BuffersQueued == 0)
					{// 再生したいサウンドが再生中でない
						//取得したボリュームで再生
						fSoundLabelVolume[g_aSoundSpot[nCntSEC].Sound] = g_aSoundSpot[nCntSEC].fVolume;	// LabelVolume保存
						/*PlaySound(g_aSoundSpot[nCntSEC].Sound);			*/								// 再生
						SetVolume(g_aSoundSpot[nCntSEC].Sound, g_aSoundSpot[nCntSEC].nIdx, g_aSoundSpot[nCntSEC].fVolume);			// 音量調整
					}
					else
					{// 再生していた
						if (g_aSoundSpot[nCntSEC].fVolume > fSoundLabelVolume[g_aSoundSpot[nCntSEC].Sound])
						{// 先に再生していたものより近かった
							fSoundLabelVolume[g_aSoundSpot[nCntSEC].Sound] = g_aSoundSpot[nCntSEC].fVolume;	// LabelVolume上書
							SetVolume(g_aSoundSpot[nCntSEC].Sound, g_aSoundSpot[nCntSEC].nIdx, g_aSoundSpot[nCntSEC].fVolume);			// 音量調整
						}
					}
				}
			}
		}
		pPlayer++;
	}
}

//==================================================
//
//	サウンドスポット設定
//
//==================================================
int SetSoundSpot(D3DXVECTOR3 pos, SOUND_LABEL label)
{
	int nCntSSpot = 0;

	for (nCntSSpot = 0; nCntSSpot < MAX_SOUNDSPOT; nCntSSpot++)
	{
		if (g_aSoundSpot[nCntSSpot].bUse == false)
		{
			g_aSoundSpot[nCntSSpot].nIdx  = nCntSSpot;
			g_aSoundSpot[nCntSSpot].pos = pos;
			g_aSoundSpot[nCntSSpot].Sound = label;
			g_aSoundSpot[nCntSSpot].bUse = true;
			return nCntSSpot;
		}
	}

	return nCntSSpot;
}

//==================================================
//
//	サウンドスポットの距離計算
//
//==================================================
void SoundDistance(void)
{
	Player* pPlayer = GetPlayer();
	float Distance;	// プレイヤーとオブジェクトサウンドとの距離

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntSEC = 0; nCntSEC < MAX_SOUNDSPOT; nCntSEC++)
		{
			if (g_aSoundSpot[nCntSEC].bUse == true)
			{
				Distance = sqrtf(powf(g_aSoundSpot[nCntSEC].pos.x - pPlayer->pos.x, 2.0f) +
					powf(g_aSoundSpot[nCntSEC].pos.y - pPlayer->pos.y, 2.0f) +
					powf(g_aSoundSpot[nCntSEC].pos.z - pPlayer->pos.z, 2.0f));

				if (Distance <= AUDIBLE_DISTANCE)
				{// 範囲内
					g_aSoundSpot[nCntSEC].bwithin = true;
				}
			}
		}
		pPlayer++;
	}
}