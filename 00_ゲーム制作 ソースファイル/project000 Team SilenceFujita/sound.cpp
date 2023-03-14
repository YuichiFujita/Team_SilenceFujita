//======================================================================================================================
//
//	サウンド処理 [sound.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "sound.h"

//**********************************************************************************************************************
//	マクロ定義 (追加で入れたやつ)
//**********************************************************************************************************************
#define SOUND_SWITCH_RELEASE	(SOUND_SWITCH_ON)		//リリースで使用する音
#define SOUND_SWITCH_DEBUG		(SOUND_SWITCH_OFF)		//デバックで使用する音

//**********************************************************************************************************************
//	構造体定義 (SOUNDINFO)
//**********************************************************************************************************************
typedef struct
{
	char *pFilename;	// ファイル名
	int   nCntLoop;		// ループカウント
} SOUNDINFO;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
IXAudio2 *g_pXAudio2 = NULL;									// XAudio2オブジェクトへのインターフェイス
IXAudio2MasteringVoice *g_pMasteringVoice = NULL;				// マスターボイス
IXAudio2SourceVoice    *g_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
BYTE *g_apDataAudio[SOUND_LABEL_MAX] = {};						// オーディオデータ
DWORD g_aSizeAudio[SOUND_LABEL_MAX] = {};						// オーディオデータサイズ

// サウンドの情報
SOUNDINFO g_aSoundInfo[SOUND_LABEL_MAX] =
{
	{ "data/BGM/bgm000.wav", -1 },			// BGM
	{ "data/BGM/game_bgm000.wav", -1 },		// ゲーム内BGM_000
	{ "data/BGM/game_bgm001.wav", -1 },		// ゲーム内BGM_001
	{ "data/BGM/title_bgm000.wav", -1 },	// タイトル内BGM_000
	{ "data/BGM/result_bgm000.wav", -1 },	// リザルト内BGM_000
	{ "data/BGM/tutorial_bgm000.wav", -1 },	// チュートリアル内BGM_000
	{ "data/BGM/firecar000.wav", -1 },		// 消防車の効果音BGM_000
	{ "data/BGM/yakiimo000.wav", -1 },		// 焼き芋屋の効果音BGM_000
	{ "data/BGM/bousoucar000.wav", -1 },	// 暴走車の効果音BGM_000
	{ "data/BGM/senkyocar000.wav", -1 },	// 選挙カーの効果音BGM_000
	{ "data/BGM/police000.wav", -1 },		// 警察の効果音BGM_000
	{ "data/BGM/wind000.wav", -1 },			// プレイヤー能力（送風機）の効果音BGM_000
	{ "data/BGM/car000.wav", -1 },			// 車走行時のBGM_000
	{ "data/SE/move000.wav", 0 },			// SE
	{ "data/SE/select000.wav", 0 },			// 選択のSE_000
	{ "data/SE/decide000.wav", 0 },			// 決定のSE_000
	{ "data/SE/startcount000.wav", 0 },		// スタートのカウントダウン（ピッ！）のSE_000
	{ "data/SE/startgo000.wav", 0 },		// 開始時のスタート（ピー！）のSE_000
	{ "data/SE/startslide000.wav", 0 },		// スタートのスライド（シュッ！）のSE_000
	{ "data/SE/startlanding000.wav", 0 },	// スタートシグナルの着地の音（ズドン）のSE_000
	{ "data/SE/boost000.wav", 0 },			// プレイヤー能力（ブースト）のSE_000
	{ "data/SE/bomb000.wav", 0 },			// プレイヤー能力（ボム）のSE_000
	{ "data/SE/carbrake000.wav", 0 },		// ブレーキ音のSE_000
	{ "data/SE/break000.wav", 0 },			// 破壊音のSE_000
	{ "data/SE/damage000.wav", 0 },			// プレイヤーダメージのSE_000
	{ "data/SE/fly000.wav", 0 },			// 吹き飛ばし時のSE_000
	{ "data/SE/barrier000.wav", 0 },		// バリア生成時のSE_000
	{ "data/SE/gareki000.wav", 0 },			// 崩れる（建物系）のSE_000
	{ "data/SE/gareki001.wav", 0 },			// 崩れる（小物系）のSE_001
	{ "data/SE/score000.wav", 0 },			// スコア獲得のSE_000
	{ "data/SE/tutorialpage000.wav", 0 },	// チュートリアルをめくるSE_000
	
};

//**********************************************************************************************************************
//	グローバル変数（追加で入れたやつ）
//**********************************************************************************************************************
SOUND_SWITCH g_soundSwitch;				//サウンドの有無の設定
float g_aSoundVolume[SOUND_LABEL_MAX];	//サウンドボリュームの値

//======================================================================================================================
//	サウンドの初期化処理
//======================================================================================================================
HRESULT InitSound(HWND hWnd)
{
	////追加で入れた変数の初期化
	g_soundSwitch = SOUND_SWITCH_RELEASE;

	for (int nCut = 0; nCut < SOUND_LABEL_MAX; nCut++)
	{
		g_aSoundVolume[nCut] = 0.0f;
	}

#ifdef _DEBUG	// デバッグ処理

	//デバック用のサウンド設定
	g_soundSwitch = SOUND_SWITCH_DEBUG;

#endif


	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

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
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
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
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		g_apDataAudio[nCntSound] = (BYTE*)malloc(g_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// ソースボイスの生成
		hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudio[nCntSound];
		buffer.pAudioData = g_apDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = g_aSoundInfo[nCntSound].nCntLoop;

		// オーディオバッファの登録
		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	return S_OK;
}

//======================================================================================================================
//	サウンドの終了処理
//======================================================================================================================
void UninitSound(void)
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);
	
			// ソースボイスの破棄
			g_apSourceVoice[nCntSound]->DestroyVoice();
			g_apSourceVoice[nCntSound] = NULL;
	
			// オーディオデータの開放
			free(g_apDataAudio[nCntSound]);
			g_apDataAudio[nCntSound] = NULL;
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

//======================================================================================================================
//	セグメント再生 (再生中なら停止)
//======================================================================================================================
HRESULT PlaySound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[label];
	buffer.pAudioData = g_apDataAudio[label];
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = g_aSoundInfo[label].nCntLoop;

	// 状態取得
	g_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		g_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		g_apSourceVoice[label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	g_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// 再生
	g_apSourceVoice[label]->Start(0);

	//// 追加で入れたやつ
	// labelのサウンドを設定
	g_aSoundVolume[label] = 1.0f;

	return S_OK;
}

//======================================================================================================================
//	セグメント停止 (ラベル指定)
//======================================================================================================================
void StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	g_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		g_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		g_apSourceVoice[label]->FlushSourceBuffers();

		//// 追加で入れたやつ
		// labelのサウンドを設定
		g_aSoundVolume[label] = 0.0f;
	}
}

//======================================================================================================================
//	セグメント停止 (全て)
//======================================================================================================================
void StopSound(void)
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);

			//// 追加で入れたやつ
			// labelのサウンドを設定
			g_aSoundVolume[nCntSound] = 0.0f;
		}
	}
}

//======================================================================================================================
//	音量調整 (ラベル指定)
//======================================================================================================================
void SetSoundVolume(SOUND_LABEL label,float fVolume)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	g_apSourceVoice[label]->GetState(&xa2state);
	
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		g_apSourceVoice[label]->SetVolume(fVolume);

		// labelのサウンドを設定
		g_aSoundVolume[label] = fVolume;
	}
}

//======================================================================================================================
//	音量の取得 (ラベル指定)
//======================================================================================================================
float GetSoundVolume(SOUND_LABEL label)
{
	return g_aSoundVolume[label];
}

//======================================================================================================================
//	チャンクのチェック
//======================================================================================================================
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

//======================================================================================================================
//	チャンクデータの読み込み
//======================================================================================================================
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

//======================================================================================================================
//	サウンドの取得
//======================================================================================================================
bool GetSoundType(SOUND_TYPE SoundType)
{
	bool bSound = false;

	switch (SoundType)
	{
		//メインのBGM
	case SOUND_TYPE_MAIN_BGM:
		if (g_soundSwitch == SOUND_SWITCH_ON)
		{//サウンドの設定がBGMがオンのとき
			
			//再生
			bSound = true;

		}
		break;

		//効果音系のBGM
	case SOUND_TYPE_SUB_BGM:
		if (g_soundSwitch == SOUND_SWITCH_ON ||
			g_soundSwitch == SOUND_SWITCH_NO_BGM)
		{//サウンドの設定が効果音系BGMがオンのとき

			//再生
			bSound = true;

		}
		break;

	case SOUND_TYPE_SE:
		if (g_soundSwitch == SOUND_SWITCH_ON ||
			g_soundSwitch == SOUND_SWITCH_NO_BGM)
		{//サウンドの設定がSEがオンのとき

			//再生
			bSound = true;

		}
		break;
	}
	 
	return bSound;
}