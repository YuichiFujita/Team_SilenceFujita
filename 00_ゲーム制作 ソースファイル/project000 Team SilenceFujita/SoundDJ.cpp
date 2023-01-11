//=============================================================================
//
// サウンド処理 [sound.cpp]
// Author 小原立暉
//
//=============================================================================
#include "SoundDJ.h"

//*****************************************************************************
// サウンド情報の構造体定義
//*****************************************************************************
typedef struct
{
	char *pFilename;	// ファイル名
	int nCntLoop;		// ループカウント
} SOUNDDJINFO;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT CheckChunkDJ(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkDataDJ(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
IXAudio2 *g_pXAudio2DJ = NULL;								// XAudio2オブジェクトへのインターフェイス
IXAudio2MasteringVoice *g_pMasteringVoiceDJ = NULL;			// マスターボイス
IXAudio2SourceVoice *g_apSourceVoiceDJ[SOUND_DJ_LABEL_MAX] = {};	// ソースボイス
BYTE *g_apDataAudioDJ[SOUND_DJ_LABEL_MAX] = {};					// オーディオデータ
DWORD g_aSizeAudioDJ[SOUND_DJ_LABEL_MAX] = {};					// オーディオデータサイズ

// サウンドの情報
SOUNDDJINFO g_aSoundInfo[SOUND_DJ_LABEL_MAX] =
{
	{ "data/SOUNDDJ/Fujita_One.wav",-1 },		//藤田1
	{ "data/SOUNDDJ/Fujita_Two.wav",-1 },		//藤田2
	{ "data/SOUNDDJ/Fujita_Three.wav",-1 },		//藤田3
	{ "data/SOUNDDJ/Fujita_Four.wav",-1 },		//藤田4
	{ "data/SOUNDDJ/Fujita_Five.wav",-1 },		//藤田5
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSoundDJ(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&g_pXAudio2DJ, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// マスターボイスの生成
	hr = g_pXAudio2DJ->CreateMasteringVoice(&g_pMasteringVoiceDJ);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if (g_pXAudio2DJ != NULL)
		{
			// XAudio2オブジェクトの開放
			g_pXAudio2DJ->Release();
			g_pXAudio2DJ = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for (int nCntSound = 0; nCntSound < SOUND_DJ_LABEL_MAX; nCntSound++)
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
		hFile = CreateFile(g_aSoundInfo[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = CheckChunkDJ(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkDataDJ(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// フォーマットチェック
		hr = CheckChunkDJ(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkDataDJ(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunkDJ(hFile, 'atad', &g_aSizeAudioDJ[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		g_apDataAudioDJ[nCntSound] = (BYTE*)malloc(g_aSizeAudioDJ[nCntSound]);
		hr = ReadChunkDataDJ(hFile, g_apDataAudioDJ[nCntSound], g_aSizeAudioDJ[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// ソースボイスの生成
		hr = g_pXAudio2DJ->CreateSourceVoice(&g_apSourceVoiceDJ[nCntSound], &(wfx.Format));
		if (FAILED(hr))
		{
			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudioDJ[nCntSound];
		buffer.pAudioData = g_apDataAudioDJ[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = g_aSoundInfo[nCntSound].nCntLoop;

		// オーディオバッファの登録
		g_apSourceVoiceDJ[nCntSound]->SubmitSourceBuffer(&buffer);

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSoundDJ(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_DJ_LABEL_MAX; nCntSound++)
	{
		if (g_apSourceVoiceDJ[nCntSound] != NULL)
		{
			// 一時停止
			g_apSourceVoiceDJ[nCntSound]->Stop(0);

			// ソースボイスの破棄
			g_apSourceVoiceDJ[nCntSound]->DestroyVoice();
			g_apSourceVoiceDJ[nCntSound] = NULL;

			// オーディオデータの開放
			free(g_apDataAudioDJ[nCntSound]);
			g_apDataAudioDJ[nCntSound] = NULL;
		}
	}

	// マスターボイスの破棄
	g_pMasteringVoiceDJ->DestroyVoice();
	g_pMasteringVoiceDJ = NULL;

	if (g_pXAudio2DJ != NULL)
	{
		// XAudio2オブジェクトの開放
		g_pXAudio2DJ->Release();
		g_pXAudio2DJ = NULL;
	}

	// COMライブラリの終了処理
	CoUninitialize();
}

//=============================================================================
// セグメント再生(再生中なら停止)
//=============================================================================
HRESULT PlaySound(int label, bool DJSign)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudioDJ[label];
	buffer.pAudioData = g_apDataAudioDJ[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = g_aSoundInfo[label].nCntLoop;

	// 状態取得
	g_apSourceVoiceDJ[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
	 // 一時停止
		g_apSourceVoiceDJ[label]->Stop(0);

		// オーディオバッファの削除
		g_apSourceVoiceDJ[label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	g_apSourceVoiceDJ[label]->SubmitSourceBuffer(&buffer);

	// 再生
	g_apSourceVoiceDJ[label]->Start(0);

	return S_OK;
}

//=============================================================================
// セグメント停止(ラベル指定)
//=============================================================================
void StopSoundDJ(SOUND_DJ_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	g_apSourceVoiceDJ[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		g_apSourceVoiceDJ[label]->Stop(0);

		//// オーディオバッファの削除
		//g_apSourceVoiceDJ[label]->FlushSourceBuffers();
	}
}

//=============================================================================
// セグメント停止(全て)
//=============================================================================
void StopSoundDJ(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_DJ_LABEL_MAX; nCntSound++)
	{
		if (g_apSourceVoiceDJ[nCntSound] != NULL)
		{
			// 一時停止
			g_apSourceVoiceDJ[nCntSound]->Stop(0);
		}
	}
}

//=============================================================================
// チャンクのチェック
//=============================================================================
HRESULT CheckChunkDJ(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//=============================================================================
HRESULT ReadChunkDataDJ(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

//=======================================
//サウンドの再生(一時停止の解除)
//=======================================
HRESULT ReplaySoundDJ(SOUND_DJ_LABEL label)
{
	// 再生
	g_apSourceVoiceDJ[label]->Start(0);

	return S_OK;
}
