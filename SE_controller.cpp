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
int g_nCntLabel[SOUND_LABEL_MAX] = {1};			// ラベル別再生中カウント
int g_a = 0;

//==================================================
//
//	SEコントローラー初期化
//
//==================================================
void InitSEController(void)
{
	g_nCntLabel[SOUND_LABEL_MAX] = { 1 };
	for (int nCntSEC = 0; nCntSEC < MAX_SOUNDSPOT; nCntSEC++)
	{
		g_aSoundSpot[nCntSEC].nIdx = 0;
		g_aSoundSpot[nCntSEC].nSoundIdx = 0;
		g_aSoundSpot[nCntSEC].fVolume = 0.0f;
		g_aSoundSpot[nCntSEC].pos = D3DXVECTOR3_ZERO;
		g_aSoundSpot[nCntSEC].label = SOUND_LABEL_MAX;
		g_aSoundSpot[nCntSEC].bwithin = false;
		g_aSoundSpot[nCntSEC].bUse = false;
		g_aSoundSpot[nCntSEC].bPlay = false;
	}
}

//==================================================
//
//	SEコントローラー更新
//
//==================================================
void UpdateSEController(void)
{
	CheckSoundStop(&g_nCntLabel[0]);

	float fSoundVolume[MAX_PLAYER][MAX_SOUND] = {};	// スポットごとの音量
	Player* pPlayer = GetPlayer();
	float Distance;	// プレイヤーとオブジェクトサウンドとの距離
	bool bCheck[MAX_SOUNDSPOT] = {};

	/*SoundDistance();*/

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (pPlayer->bUse == true)
		{
			for (int nCntSEC = 0; nCntSEC < MAX_SOUNDSPOT; nCntSEC++)
			{
				if (g_aSoundSpot[nCntSEC].bUse == true)
				{
					Distance = Magnitude(g_aSoundSpot[nCntSEC].pos, pPlayer->pos);

					if (Distance <= AUDIBLE_DISTANCE)
					{// 範囲内			
						// 再生するボリュームを取得
						g_aSoundSpot[nCntSEC].fVolume = (1.0f - (Distance / AUDIBLE_DISTANCE))/* / g_nCntLabel[g_aSoundSpot[nCntSEC].label]*/;

						// CallPlaySound有効化
						g_aSoundSpot[nCntSEC].bwithin = true;

						/*if (g_aSoundSpot[nCntSEC].fVolume > fSoundVolume[nCntPlayer][nCntSEC])
						{*/
						//取得したボリュームを適応
						fSoundVolume[nCntPlayer][nCntSEC] = g_aSoundSpot[nCntSEC].fVolume;	// LabelVolume保存
						SetVolume(g_aSoundSpot[nCntSEC].label, g_aSoundSpot[nCntSEC].nSoundIdx, g_aSoundSpot[nCntSEC].fVolume);			// 音量調整
					/*}*/

					//*******			!!!注意!!!			*******//
					// サウンドの再生自体は、CallPlaySoundで行います！
					//*******								*******//

					}
					else if (Distance > AUDIBLE_DISTANCE)
					{
						// CallPlaySound無効化
						g_aSoundSpot[nCntSEC].bwithin = false;
						//SetVolume(g_aSoundSpot[nCntSEC].label, g_aSoundSpot[nCntSEC].nSoundIdx, 0.0f);	// 音量調整
					}
				}
			}
			pPlayer++;
		}
	}
}

//==================================================
//
//	サウンドスポット設定
//
//==================================================
int SetSoundSpot(D3DXVECTOR3 pos, SOUND_LABEL label)
{
	g_a++;
	int nCntSSpot = 0;

	for (nCntSSpot = 0; nCntSSpot < MAX_SOUNDSPOT; nCntSSpot++)
	{
		if (g_aSoundSpot[nCntSSpot].bUse == false)
		{
			g_aSoundSpot[nCntSSpot].nIdx = nCntSSpot;
			g_aSoundSpot[nCntSSpot].label = label;
			g_aSoundSpot[nCntSSpot].pos = pos;
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

					// 再生するボリュームを取得
					g_aSoundSpot[nCntSEC].fVolume = 1.0f - (Distance / AUDIBLE_DISTANCE);
				}
			}
		}
		pPlayer++;
	}
}
//==================================================
//
//	オブジェクト側からサウンドを鳴らす
//
//==================================================
void CallPlaySound(int nSoundIdx, bool *play)
{
	if (g_aSoundSpot[nSoundIdx].bwithin == true && g_aSoundSpot[nSoundIdx].bPlay == false)
	{
		g_aSoundSpot[nSoundIdx].bPlay = true;
		PlaySound(g_aSoundSpot[nSoundIdx].label,&g_aSoundSpot[nSoundIdx].nSoundIdx);
		/*SetVolume(g_aSoundSpot[nSoundIdx].label, g_aSoundSpot[nSoundIdx].nSoundIdx, 0.0f);*/
		// ラベル別カウント増加
		g_nCntLabel[g_aSoundSpot[nSoundIdx].label]++;
	}
	else if (g_aSoundSpot[nSoundIdx].bwithin == true && g_aSoundSpot[nSoundIdx].bPlay == true)
	{
		bool bCheck = false;
		bCheck = GetPlaySound(g_aSoundSpot[nSoundIdx].label, g_aSoundSpot[nSoundIdx].nSoundIdx);

		if (bCheck == false)
		{
			g_aSoundSpot[nSoundIdx].bPlay = false;
		}
	}
}