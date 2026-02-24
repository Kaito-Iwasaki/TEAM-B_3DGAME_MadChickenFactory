//=============================================================================
//
// サウンド処理 [sound.cpp]
// Author : KAITO IWASAKI
// Special Thanks : AKIRA TANAKA
//				　: TENMA SAITO
//
//=============================================================================
#include "sound.h"
#include "DebugProc.h"

//*****************************************************************************
// サウンド情報の構造体定義
//*****************************************************************************
typedef struct
{
	const char *pFilename;	// ファイル名
	int nCntLoop;			// ループカウント
} SOUNDINFO;

//**
//** サウンドチェック構造体 
//**
typedef struct SoundChecker : public IXAudio2VoiceCallback
{
private:
	HRESULT m_LastError;
	bool m_bEnd;
	bool m_bSubmit;

public:

	SoundChecker() : m_LastError(S_OK), m_bEnd(false), m_bSubmit(false) {}

	// バッファ―処理終了時
	void STDMETHODCALLTYPE OnBufferEnd(void* pBufferContext) override
	{
		OutputDebugString(TEXT("バッファー処理が終了したよ！\n"));

		// 必要に応じてvoidをキャストして使用
		if (pBufferContext != nullptr)
		{ // ポインタを使用した処理

		}

		m_bSubmit = false;
	}

	// バッファ―処理開始時
	void STDMETHODCALLTYPE OnBufferStart(void* pBufferContext) override
	{
		OutputDebugString(TEXT("バッファー処理スタート！\n"));

		// 必要に応じてvoidをキャストして使用
		if (pBufferContext != nullptr)
		{ // ポインタを使用した処理

		}
		m_bEnd = false;
		m_bSubmit = true;
	}

	// ループ終了時
	void STDMETHODCALLTYPE OnLoopEnd(void* pBufferContext) override
	{
		// 必要に応じてvoidをキャストして使用
		if (pBufferContext != nullptr)
		{ // ポインタを使用した処理

		}
	}

	// 再生終了時
	void STDMETHODCALLTYPE OnStreamEnd(void) override
	{
		OutputDebugString(TEXT("音源の再生が終了したよ！\n"));

		m_bEnd = true;
	}
	// エラー発生時
	void STDMETHODCALLTYPE OnVoiceError(
		void* pBufferContext,
		HRESULT Error
	) override
	{
		OutputDebugString(TEXT("ひぃん......エラー発生......\n"));

		// 必要に応じてvoidをキャストして使用
		if (pBufferContext != nullptr)
		{ // ポインタを使用した処理

		}

		m_LastError = Error;
	}

	// 音声の処理パス終了直後
	void STDMETHODCALLTYPE OnVoiceProcessingPassEnd(void) override
	{

	}

	// データを読み取る直前
	void STDMETHODCALLTYPE OnVoiceProcessingPassStart(
		UINT32 BytesRequired
	) override
	{

	}

	// 再生中か
	bool IsPlayingSound(void)
	{
		return m_bEnd;
	}

	// 再生中か
	void bEndReverse(void)
	{
		m_bEnd = false;
	}

	// バッファーの登録がされているか
	bool IsSubmitBuffer(void)
	{
		return m_bSubmit;
	}

	// 最後に発生したエラー取得
	HRESULT GetLastError(void)
	{
		return m_LastError;
	}


} SoundChecker;

typedef SoundChecker LPSOUNDCHECKER, PSOUNDCHECKER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
IXAudio2 *g_pXAudio2 = NULL;									// XAudio2オブジェクトへのインターフェイス
IXAudio2MasteringVoice *g_pMasteringVoice = NULL;				// マスターボイス
IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX][MAX_SOUND] = {};	// ソースボイス
bool g_abPlay[SOUND_LABEL_MAX][MAX_SOUND] = {};					// 再生中か判定
SoundChecker g_aChecker[SOUND_LABEL_MAX][MAX_SOUND] = {};		// オーディオチェッカー
BYTE *g_apDataAudio[SOUND_LABEL_MAX] = {};						// オーディオデータ
DWORD g_aSizeAudio[SOUND_LABEL_MAX] = {};						// オーディオデータサイズ

// サウンドの情報
SOUNDINFO g_aSoundInfo[SOUND_LABEL_MAX] = 
{
	{"data/SOUND/SE/Select.wav",0},					// SE 回転ノコギリ(仮置き)
	{"data/SOUND/SE/hit.wav",0},						// SE プレス機
	{"data/SOUND/SE/donald.wav",0},						// SE バーナー
	{"data/SOUND/SE/donald.wav",0},						// SE 敵
	{"data/SOUND/SE/blood.wav",0},						// SE 血
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSound(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "COMライブラリの初期化に失敗！", "警告！", MB_ICONWARNING);

		return E_FAIL;
	}

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&g_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}
	
	// マスターボイスの生成
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if(g_pXAudio2 != NULL)
		{
			// XAudio2オブジェクトの開放
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for(int nCntLabel = 0; nCntLabel < SOUND_LABEL_MAX; nCntLabel++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(g_aSoundInfo[nCntLabel].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);

			return HRESULT_FROM_WIN32(GetLastError());
		}

		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);

			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);

			return S_FALSE;
		}

		// チャンクデータの読み込み
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);

			return S_FALSE;
		}

		if(dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);

			return S_FALSE;
		}
	
		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);

			return S_FALSE;
		}

		// チャンクデータの読み込み
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);

			return S_FALSE;
		}

		// データチェック
		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntLabel], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);

			return S_FALSE;
		}

		// チャンクデータの読み込み
		g_apDataAudio[nCntLabel] = (BYTE*)malloc(g_aSizeAudio[nCntLabel]);
		hr = ReadChunkData(hFile, g_apDataAudio[nCntLabel], g_aSizeAudio[nCntLabel], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);

			return S_FALSE;
		}
	
		for (int nCntSound = 0; nCntSound < MAX_SOUND; nCntSound++)
		{
			// ソースボイスの生成
			hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntLabel][nCntSound], &(wfx.Format), 0, 2.0f,&g_aChecker[nCntLabel][nCntSound]);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);

				return S_FALSE;
			}


			// バッファの値設定
			memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
			buffer.AudioBytes = g_aSizeAudio[nCntLabel];
			buffer.pAudioData = g_apDataAudio[nCntLabel];
			buffer.Flags = XAUDIO2_END_OF_STREAM;
			buffer.LoopCount = g_aSoundInfo[nCntLabel].nCntLoop;

			// //オーディオバッファの登録
			//g_apSourceVoice[nCntLabel][nCntSound]->SubmitSourceBuffer(&buffer);
		}
		// ファイルをクローズ
		CloseHandle(hFile);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSound(void)
{
	// サウンドの停止
	StopSound();

	// 一時停止
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		for (int nCntSource = 0; nCntSource < MAX_SOUND; nCntSource++)
		{
			if (g_apSourceVoice[nCntSound][nCntSource] != NULL)
			{
				// 一時停止
				g_apSourceVoice[nCntSound][nCntSource]->Stop(0);

				// ソースボイスの破棄
				g_apSourceVoice[nCntSound][nCntSource]->DestroyVoice();
				g_apSourceVoice[nCntSound][nCntSource] = NULL;

				if (g_apDataAudio[nCntSound] != NULL)
				{
					// オーディオデータの開放
					free(g_apDataAudio[nCntSound]);
					g_apDataAudio[nCntSound] = NULL;
				}
			}
		}
	}
	
	// マスターボイスの破棄
	g_pMasteringVoice->DestroyVoice();
	g_pMasteringVoice = NULL;
	
	if(g_pXAudio2 != NULL)
	{
		// XAudio2オブジェクトの開放
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}
	
	// COMライブラリの終了処理
	CoUninitialize();
}

//=============================================================================
// セグメント再生(再生中なら停止)
//=============================================================================
HRESULT PlaySound(SOUND_LABEL label, int* pOut)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[label];
	buffer.pAudioData = g_apDataAudio[label];
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = g_aSoundInfo[label].nCntLoop;

	for (int nCntSource = 0; nCntSource < MAX_SOUND; nCntSource++)
	{
		if (g_aChecker[label][nCntSource].IsPlayingSound() == false && g_abPlay[label][nCntSource] == false)
		{// 指定の配列番号目が不使用

			// 再生中に変更
			g_abPlay[label][nCntSource] = true;

			// オーディオバッファの登録
			g_apSourceVoice[label][nCntSource]->SubmitSourceBuffer(&buffer);

			// 再生
			g_apSourceVoice[label][nCntSource]->Start(0);

			*pOut = nCntSource;

			break;
		}
	}

	return S_OK;
}

//=============================================================================
// セグメント停止(ラベル指定)
//=============================================================================
void StopSound(int label, int* pOut)
{
	XAUDIO2_VOICE_STATE xa2state;
	if (g_abPlay[label][*pOut] == true)
	{
		// 一時停止
		g_apSourceVoice[label][*pOut]->Stop(0);

		// オーディオバッファの削除
		g_apSourceVoice[label][*pOut]->FlushSourceBuffers();

		g_aChecker[label][*pOut].bEndReverse();

		g_abPlay[label][*pOut] = false;
	}
}

//=============================================================================
// セグメント停止(全て)
//=============================================================================
void StopSound(void)
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		for (int nCntSource = 0; nCntSource < MAX_SOUND; nCntSource++)
		{
			if (g_apSourceVoice[nCntSound][nCntSource] != NULL)
			{
				// 一時停止
				g_apSourceVoice[nCntSound][nCntSource]->Stop(0);
				g_abPlay[nCntSound][nCntSource] = false;
			}
		}
	}
}

//=============================================================================
// チャンクのチェック
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

//=============================================================================
// 音量調整
//=============================================================================
void SetVolume(SOUND_LABEL sound,int nIdx, float Volume)
{
	g_apSourceVoice[sound][nIdx]->SetVolume(Volume);
}

//=============================================================================
// サウンド情報取得
//=============================================================================
XAUDIO2_VOICE_STATE* GetSoundState(SOUND_LABEL label, int nIdx)
{
	XAUDIO2_VOICE_STATE state;
	g_apSourceVoice[label][nIdx]->GetState(&state);

	return &state;
}
//=============================================================================
// サウンド再生情報取得
//=============================================================================
bool GetPlaySound(SOUND_LABEL label, int nIdx)
{
	return g_abPlay[label][nIdx];
}
//=============================================================================
// サウンド再生確認
//=============================================================================
void CheckSoundStop(int *nCnt)
{
	bool bCheck = false;
	int nCntdel = 0;

	for (int nCntLabel = 0; nCntLabel < SOUND_LABEL_MAX; nCntLabel++)
	{
		for (int nCntSource = 0; nCntSource < MAX_SOUND; nCntSource++)
		{
			if (g_abPlay[nCntLabel][nCntSource] == true)
			{
				bCheck = g_aChecker[nCntLabel][nCntSource].IsPlayingSound();

				if (bCheck == true)
				{
					StopSound(nCntLabel, &nCntSource);
					nCntdel++;
				}
			}
		}
		nCnt++;
	}
	*nCnt -= nCntdel;
}