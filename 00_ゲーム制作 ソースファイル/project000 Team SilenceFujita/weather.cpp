//======================================================================================================================
//
//	天気処理 [weather.cpp]
//	Author：小原立暉
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "weather.h"
#include "calculation.h"
#include "game.h"

#include "camera.h"
#include "player.h"
#include "flash.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define SNOW_TEXTURE	"data\\TEXTURE\\effect000.jpg"			// 雪のテクスチャ
#define THUNDER_FLASH_RADIUS	(100000.0f)						// 雷のフラッシュが生まれるの半径
#define THUNDER_FLASH_ANGLE		(D3DXToRadian(90))				// 雷のフラッシュが生まれるの角度

#define RAIN_COL		(D3DXCOLOR(0.5f, 0.5f, 0.95f, 1.0f))	// 雨の頂点カラー
#define SNOW_COL		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))		// 雪の頂点カラー
#define THUNDER_COL		(D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f))		// 雷の頂点カラー

#define WEATHER_RAND	(50)		// 天気のランダムの範囲
#define SUNNY_RAND		(40)		// ランダムでの晴れの範囲
#define RAIN_RAND		(44)		// ランダムでの雨の範囲
#define SNOW_RAND		(48)		// ランダムでの雪の範囲
#define THUNDER_RAND	(49)		// ランダムでの雷雨の範囲

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
// 雨
void InitRain(void);			// 雨の初期化処理
void UpdateRain(void);			// 雨の更新処理
void DrawRain(void);			// 雨の描画処理

// 雪
void InitSnow(void);			// 雪の初期化処理
void UpdateSnow(void);			// 雪の更新処理
void DrawSnow(void);			// 雪の描画処理

// 雷
void InitThunder(void);			// 雷の初期化処理
void UpdateThunder(void);		// 雷の更新処理

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureWeather = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWeather  = NULL;		// 頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffThunder  = NULL;		// 頂点バッファへのポインタ(雷)

Rain g_aRain[MAX_RAIN];				// 雨の情報
Snow g_aSnow[MAX_SNOW];				// 雪の情報
Thunder g_aThunder[MAX_THUNDER];	// 雷の情報
WEATHERTYPE g_Weather;				// 天気の状態
int g_NumWeather;					// 降っている数を取得する
int g_nThunderCount;				// 雷のカウント
MODE g_WeatherMode;					// モード(天気専用)

//======================================================================================================================
//	天気の初期化処理
//======================================================================================================================
void InitWeather(void)
{
	int nRandWeather;		// 天気の変数

	g_WeatherMode = GetMode();	// モード
	RESULTSTATE resultState = GetResultState();	// ゲームの状態

	switch (g_WeatherMode)
	{
	case MODE_TITLE:

		// 天気を晴れに設定する
		g_Weather = WEATHERTYPE_SUNNY;

		break;			// 抜け出す

	case MODE_TUTORIAL:	// チュートリアル

		// 天気を晴れに設定する
		g_Weather = WEATHERTYPE_SUNNY;

		break;			// 抜け出す

	case MODE_GAME:		// ゲーム

#ifdef _DEBUG	// デバッグ処理
		// 晴れに固定する
		nRandWeather = 0;
#else
		// 天候をランダムに設定
		nRandWeather = rand() % WEATHER_RAND;
#endif
		if (nRandWeather <= SUNNY_RAND)
		{ // 晴れの場合
			// 天気を晴れに設定する
			g_Weather = WEATHERTYPE_SUNNY;
		}
		else if (nRandWeather <= RAIN_RAND)
		{ // 雨の場合
			// 天気を雨に設定する
			g_Weather = WEATHERTYPE_RAIN;
		}
		else if (nRandWeather <= SNOW_RAND)
		{ // 雪の場合
			// 天気を雪に設定する
			g_Weather = WEATHERTYPE_SNOW;
		}
		else if (nRandWeather <= THUNDER_RAND)
		{ // 雷雨の場合
			// 天気を雷雨に設定する
			g_Weather = WEATHERTYPE_THUNDER;
		}

		break;			// 抜け出す

	case MODE_RESULT:	// リザルト

		if (resultState == RESULTSTATE_CLEAR)
		{ // ゲームクリア状態の場合
	
			// 天気を晴れに設定する
			g_Weather = WEATHERTYPE_SUNNY;
		}
		else if (resultState == RESULTSTATE_TIMEOVER
			 ||  resultState == RESULTSTATE_LIFEOVER)
		{ // ゲームオーバー状態の場合
	
			// 天気を雷雨に設定する
			g_Weather = WEATHERTYPE_THUNDER;
		}

		break;

	case MODE_RANKING:	// ランキング

		if (resultState == RESULTSTATE_CLEAR)
		{ // ゲームクリア状態の場合

			// 天気を晴れに設定する
			g_Weather = WEATHERTYPE_SUNNY;

		}
		else if (resultState == RESULTSTATE_TIMEOVER
			|| resultState == RESULTSTATE_LIFEOVER)
		{ // ゲームオーバー状態の場合

		  // 天気を雷雨に設定する
			g_Weather = WEATHERTYPE_THUNDER;
		}
		break;			// 抜け出す
	}

	// 総数を初期化する
	g_NumWeather = 0;

	// 雷のカウントを初期化する
	g_nThunderCount = 0;

	// 雨の情報の初期化
	for (int nCntRain = 0; nCntRain < MAX_RAIN; nCntRain++)
	{ // 雨の最大表示数分繰り返す

		g_aRain[nCntRain].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
		g_aRain[nCntRain].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
		g_aRain[nCntRain].fRadius = D3DXVECTOR2(0.0f, 0.0f);			// 半径
		g_aRain[nCntRain].bUse = false;									// 使用状況

	}

	// 雪の情報の初期化
	for (int nCntSnow = 0; nCntSnow < MAX_SNOW; nCntSnow++)
	{ // 雪の最大表示数分繰り返す

		g_aSnow[nCntSnow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
		g_aSnow[nCntSnow].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
		g_aSnow[nCntSnow].fRadius = D3DXVECTOR2(0.0f, 0.0f);			// 半径
		g_aSnow[nCntSnow].bUse = false;									// 使用状況
	}

	// 雷の情報の初期化
	for (int nCntThunder = 0; nCntThunder < MAX_THUNDER; nCntThunder++)
	{ // 雷の最大表示分繰り返す
		g_aThunder[nCntThunder].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aThunder[nCntThunder].fRadius = D3DXVECTOR2(0.0f, 0.0f);		// 半径
		g_aThunder[nCntThunder].fShiftWidth = 0.0f;						// ずらす幅
		g_aThunder[nCntThunder].nVariCount = 0;							// カウント
		g_aThunder[nCntThunder].bUse = false;							// 使用状況
	}

	switch (g_Weather)
	{
	case WEATHERTYPE_SUNNY:		// 晴れ

		break;					// 抜け出す

	case WEATHERTYPE_RAIN:		// 雨

		// 雨の初期化処理
		InitRain();

		break;					// 抜け出す

	case WEATHERTYPE_SNOW:		// 雪

		// 雪の初期化処理
		InitSnow();

		break;					// 抜け出す

	case WEATHERTYPE_THUNDER:	// 雷雨

		// 雨の初期化処理
		InitRain();

		// 雷の初期化処理
		InitThunder();

		break;					// 抜け出す
	}
}

//======================================================================================================================
// 雨の初期化処理
//======================================================================================================================
void InitRain(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, NULL, &g_apTextureWeather);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * 4 * MAX_RAIN,		// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffWeather,
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWeather->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_RAIN; nCntWeather++)
	{ // エフェクトの最大表示数分繰り返す

	  // 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffWeather->Unlock();
}

//======================================================================================================================
// 雪の初期化処理
//======================================================================================================================
void InitSnow(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, SNOW_TEXTURE, &g_apTextureWeather);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * 4 * MAX_SNOW,		// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffWeather,
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWeather->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_SNOW; nCntWeather++)
	{ // エフェクトの最大表示数分繰り返す

	  // 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffWeather->Unlock();
}

//======================================================================================================================
// 雷の初期化処理
//======================================================================================================================
void InitThunder(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * 8 * MAX_THUNDER,	// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffThunder,
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffThunder->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_THUNDER; nCntWeather++)
	{ // エフェクトの最大表示数分繰り返す

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[4].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[5].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[6].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[7].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラーの設定
		pVtx[0].col = THUNDER_COL;
		pVtx[1].col = THUNDER_COL;
		pVtx[2].col = THUNDER_COL;
		pVtx[3].col = THUNDER_COL;
		pVtx[4].col = THUNDER_COL;
		pVtx[5].col = THUNDER_COL;
		pVtx[6].col = THUNDER_COL;
		pVtx[7].col = THUNDER_COL;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データのポインタを 8つ分進める
		pVtx += 8;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffThunder->Unlock();
}

//======================================================================================================================
//	天気の終了処理
//======================================================================================================================
void UninitWeather(void)
{
	if (g_apTextureWeather != NULL)
	{ // 変数 (g_apTextureWeather) がNULLではない場合

		g_apTextureWeather->Release();
		g_apTextureWeather = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffWeather != NULL)
	{ // 変数 (g_pVtxBuffWeather) がNULLではない場合

		g_pVtxBuffWeather->Release();
		g_pVtxBuffWeather = NULL;
	}

	if (g_pVtxBuffThunder != NULL)
	{ //変数(g_pVtxBuffThunder)がNULLではない場合

		g_pVtxBuffThunder->Release();
		g_pVtxBuffThunder = NULL;
	}
}

//======================================================================================================================
//	天気の更新処理
//======================================================================================================================
void UpdateWeather(void)
{
	switch (g_Weather)
	{
	case WEATHERTYPE_SUNNY:		// 晴れ

		break;					// 抜け出す

	case WEATHERTYPE_RAIN:		// 雨

		// 雨の更新処理
		UpdateRain();									

		break;					// 抜け出す

	case WEATHERTYPE_SNOW:		// 雪

		// 雪の更新処理
		UpdateSnow();

		break;					// 抜け出す

	case WEATHERTYPE_THUNDER:	// 雷

		// 雨の更新処理
		UpdateRain();

		// 雷の更新処理
		UpdateThunder();

		break;					// 抜け出す
	}
}

//======================================================================================================================
// 雨の更新処理
//======================================================================================================================
void UpdateRain(void)
{
	int nNumWeather = 0;	// 降っている物の数

	// ポインタを宣言
	VERTEX_3D *pVtx;								// 頂点情報へのポインタ
	Player *pPlayer = GetPlayer();					// プレイヤーの情報
	Camera *pCamera = GetCamera(CAMERATYPE_MAIN);	// カメラの情報

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWeather->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_RAIN; nCntWeather++)
	{ // エフェクトの最大表示数分繰り返す

		if (g_aRain[nCntWeather].bUse == true)
		{ // エフェクトが使用されている場合

			switch (g_WeatherMode)
			{
			case MODE_GAME:		// ゲーム

				// プレイヤー分の移動量を足す
				g_aRain[nCntWeather].pos.x += pPlayer->pos.x - pPlayer->oldPos.x;
				g_aRain[nCntWeather].pos.z += pPlayer->pos.z - pPlayer->oldPos.z;

				break;			// 抜け出す

			case MODE_RESULT:	// リザルト

				// プレイヤー分の移動量を足す
				g_aRain[nCntWeather].pos.x += pCamera->posV.x - pCamera->posVOld.x;
				g_aRain[nCntWeather].pos.z += pCamera->posV.z - pCamera->posVOld.z;

				break;			// 抜け出す
			}

			// 降っているものの総数を加算する
			nNumWeather++;

			// 移動の更新
			g_aRain[nCntWeather].pos -= g_aRain[nCntWeather].move;

			// 半径の補正
			if (g_aRain[nCntWeather].fRadius.x < 0.0f)
			{ // 半径が 0.0f より小さい場合

				// 半径に 0.0f を代入
				g_aRain[nCntWeather].fRadius.x = 0.0f;
			}

			// 半径の補正
			if (g_aRain[nCntWeather].fRadius.y < 0.0f)
			{ // 半径が 0.0f より小さい場合

				// 半径に 0.0f を代入
				g_aRain[nCntWeather].fRadius.y = 0.0f;
			}

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aRain[nCntWeather].fRadius.x, +g_aRain[nCntWeather].fRadius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aRain[nCntWeather].fRadius.x, +g_aRain[nCntWeather].fRadius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aRain[nCntWeather].fRadius.x, -g_aRain[nCntWeather].fRadius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aRain[nCntWeather].fRadius.x, -g_aRain[nCntWeather].fRadius.y, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = RAIN_COL;
			pVtx[1].col = RAIN_COL;
			pVtx[2].col = RAIN_COL;
			pVtx[3].col = RAIN_COL;

			switch (g_WeatherMode)
			{
			case MODE_GAME:		// ゲーム

				if (g_aRain[nCntWeather].pos.y <= 0.0f)
				{ // 雨が地面に落ちたとき
					// 使用しない
					g_aRain[nCntWeather].bUse = false;
				}

				break;			// 抜け出す

			case MODE_RESULT:	// リザルト

				if (g_aRain[nCntWeather].pos.y <= RESULT_RAIN_LINE)
				{ // 雨が地面に落ちたとき
					// 使用しない
					g_aRain[nCntWeather].bUse = false;
				}

				break;			// 抜け出す
			}
		}

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffWeather->Unlock();

	g_NumWeather = nNumWeather;	// 総数を代入する
}

//======================================================================================================================
// 雪の更新処理
//======================================================================================================================
void UpdateSnow(void)
{
	int nNumWeather = 0;	// 降っている物の数

	// ポインタを宣言
	VERTEX_3D *pVtx;		// 頂点情報へのポインタ
	Player *pPlayer = GetPlayer();		// プレイヤーの情報

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWeather->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_SNOW; nCntWeather++)
	{ // エフェクトの最大表示数分繰り返す

		if (g_aSnow[nCntWeather].bUse == true)
		{ // エフェクトが使用されている場合

			// 降っているものの総数を加算する
			nNumWeather++;

			// プレイヤー分の移動量を足す
			g_aSnow[nCntWeather].pos.x += pPlayer->pos.x - pPlayer->oldPos.x;
			g_aSnow[nCntWeather].pos.z += pPlayer->pos.z - pPlayer->oldPos.z;

			// 移動の更新
			g_aSnow[nCntWeather].pos -= g_aSnow[nCntWeather].move;

			// 半径の補正
			if (g_aSnow[nCntWeather].fRadius.x < 0.0f)
			{ // 半径が 0.0f より小さい場合

				// 半径に 0.0f を代入
				g_aSnow[nCntWeather].fRadius.x = 0.0f;
			}

			// 半径の補正
			if (g_aSnow[nCntWeather].fRadius.y < 0.0f)
			{ // 半径が 0.0f より小さい場合

				// 半径に 0.0f を代入
				g_aSnow[nCntWeather].fRadius.y = 0.0f;
			}

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aSnow[nCntWeather].fRadius.x, +g_aSnow[nCntWeather].fRadius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aSnow[nCntWeather].fRadius.x, +g_aSnow[nCntWeather].fRadius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aSnow[nCntWeather].fRadius.x, -g_aSnow[nCntWeather].fRadius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aSnow[nCntWeather].fRadius.x, -g_aSnow[nCntWeather].fRadius.y, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = SNOW_COL;
			pVtx[1].col = SNOW_COL;
			pVtx[2].col = SNOW_COL;
			pVtx[3].col = SNOW_COL;

			if (g_aSnow[nCntWeather].pos.y <= 0.0f)
			{ // 雨が地面に落ちたとき
				// 使用しない
				g_aSnow[nCntWeather].bUse = false;
			}
		}

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffWeather->Unlock();

	g_NumWeather = nNumWeather;	// 総数を代入する
}

//======================================================================================================================
// 雷の更新処理
//======================================================================================================================
void UpdateThunder(void)
{
	int nNumWeather = 0;	// 降っている物の数

	// ポインタを宣言
	VERTEX_3D *pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffThunder->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_THUNDER; nCntWeather++)
	{ // エフェクトの最大表示数分繰り返す

		if (g_aThunder[nCntWeather].bUse == true)
		{ // エフェクトが使用されている場合

			// 降っているものの総数を加算する
			nNumWeather++;

			// カウントを加算する
			g_aThunder[nCntWeather].nVariCount++;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aThunder[nCntWeather].fRadius.x, +g_aThunder[nCntWeather].fRadius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aThunder[nCntWeather].fRadius.x, +g_aThunder[nCntWeather].fRadius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aThunder[nCntWeather].fRadius.x + g_aThunder[nCntWeather].fShiftWidth, -g_aThunder[nCntWeather].fRadius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aThunder[nCntWeather].fRadius.x + g_aThunder[nCntWeather].fShiftWidth, -g_aThunder[nCntWeather].fRadius.y, 0.0f);
			pVtx[4].pos = D3DXVECTOR3(-g_aThunder[nCntWeather].fRadius.x - (g_aThunder[nCntWeather].fShiftWidth * 2), -(g_aThunder[nCntWeather].fRadius.y * 2), 0.0f);
			pVtx[5].pos = D3DXVECTOR3(+g_aThunder[nCntWeather].fRadius.x - (g_aThunder[nCntWeather].fShiftWidth * 2), -(g_aThunder[nCntWeather].fRadius.y * 2), 0.0f);
			pVtx[6].pos = D3DXVECTOR3(-g_aThunder[nCntWeather].fRadius.x + (g_aThunder[nCntWeather].fShiftWidth * 2), -(g_aThunder[nCntWeather].fRadius.y * 3), 0.0f);
			pVtx[7].pos = D3DXVECTOR3(+g_aThunder[nCntWeather].fRadius.x + (g_aThunder[nCntWeather].fShiftWidth * 2), -(g_aThunder[nCntWeather].fRadius.y * 3), 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = THUNDER_COL;
			pVtx[1].col = THUNDER_COL;
			pVtx[2].col = THUNDER_COL;
			pVtx[3].col = THUNDER_COL;
			pVtx[4].col = THUNDER_COL;
			pVtx[5].col = THUNDER_COL;
			pVtx[6].col = THUNDER_COL;
			pVtx[7].col = THUNDER_COL;

			if (g_aThunder[nCntWeather].nVariCount >= THUNDER_COUNT)
			{ // 一定時間経過したら
				// 使用しない
				g_aThunder[nCntWeather].bUse = false;
			}
		}

		// 頂点データのポインタを 4つ分進める
		pVtx += 8;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffThunder->Unlock();

	g_NumWeather = nNumWeather;	// 総数を代入する
}

//======================================================================================================================
//	天気の描画処理
//======================================================================================================================
void DrawWeather(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Zバッファ更新の有効 / 無効の設定

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	switch (g_Weather)
	{
	case WEATHERTYPE_SUNNY:		// 晴れ

		break;					// 抜け出す

	case WEATHERTYPE_RAIN:		// 雨

		// 雨の描画処理
		DrawRain();

		break;					// 抜け出す

	case WEATHERTYPE_SNOW:		// 雪

		// 雪の描画処理
		DrawSnow();

		break;					// 抜け出す

	case WEATHERTYPE_THUNDER:	// 雷

		// 雨の描画処理
		DrawRain();

		break;					// 抜け出す
	}

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Zバッファ更新の有効 / 無効の設定

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//======================================================================================================================
// 雨の描画処理
//======================================================================================================================
void DrawRain(void)
{
	// 変数を宣言
	D3DXMATRIX mtxTrans;						// 計算用マトリックス
	D3DXMATRIX mtxView;							// ビューマトリックス取得用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	for (int nCntWeather = 0; nCntWeather < MAX_RAIN; nCntWeather++)
	{ // エフェクトの最大表示数分繰り返す

		if (g_aRain[nCntWeather].bUse == true)
		{ // エフェクトが使用されている場合

		  // ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aRain[nCntWeather].mtxWorld);

			// ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aRain[nCntWeather].mtxWorld, NULL, &mtxView);	// 逆行列を求める
			g_aRain[nCntWeather].mtxWorld._41 = 0.0f;
			g_aRain[nCntWeather].mtxWorld._42 = 0.0f;
			g_aRain[nCntWeather].mtxWorld._43 = 0.0f;

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aRain[nCntWeather].pos.x, g_aRain[nCntWeather].pos.y, g_aRain[nCntWeather].pos.z);
			D3DXMatrixMultiply(&g_aRain[nCntWeather].mtxWorld, &g_aRain[nCntWeather].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aRain[nCntWeather].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffWeather, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureWeather);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWeather * 4, 2);
		}
	}
}

//======================================================================================================================
// 雪の描画処理
//======================================================================================================================
void DrawSnow(void)
{
	// 変数を宣言
	D3DXMATRIX mtxTrans;						// 計算用マトリックス
	D3DXMATRIX mtxView;							// ビューマトリックス取得用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntWeather = 0; nCntWeather < MAX_SNOW; nCntWeather++)
	{ // エフェクトの最大表示数分繰り返す

		if (g_aSnow[nCntWeather].bUse == true)
		{ // エフェクトが使用されている場合

		  // ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aSnow[nCntWeather].mtxWorld);

			// ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aSnow[nCntWeather].mtxWorld, NULL, &mtxView);	// 逆行列を求める
			g_aSnow[nCntWeather].mtxWorld._41 = 0.0f;
			g_aSnow[nCntWeather].mtxWorld._42 = 0.0f;
			g_aSnow[nCntWeather].mtxWorld._43 = 0.0f;

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aSnow[nCntWeather].pos.x, g_aSnow[nCntWeather].pos.y, g_aSnow[nCntWeather].pos.z);
			D3DXMatrixMultiply(&g_aSnow[nCntWeather].mtxWorld, &g_aSnow[nCntWeather].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aSnow[nCntWeather].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffWeather, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureWeather);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWeather * 4, 2);
		}
	}

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//======================================================================================================================
// 雷の描画処理
//======================================================================================================================
void DrawThunder(void)
{
	// 変数を宣言
	D3DXMATRIX mtxTrans;						// 計算用マトリックス
	D3DXMATRIX mtxView;							// ビューマトリックス取得用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Zバッファ更新の有効 / 無効の設定

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntWeather = 0; nCntWeather < MAX_THUNDER; nCntWeather++)
	{ // エフェクトの最大表示数分繰り返す

		if (g_aThunder[nCntWeather].bUse == true)
		{ // エフェクトが使用されている場合

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aThunder[nCntWeather].mtxWorld);

			// ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aThunder[nCntWeather].mtxWorld, NULL, &mtxView);	// 逆行列を求める
			g_aThunder[nCntWeather].mtxWorld._41 = 0.0f;
			g_aThunder[nCntWeather].mtxWorld._42 = 0.0f;
			g_aThunder[nCntWeather].mtxWorld._43 = 0.0f;

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aThunder[nCntWeather].pos.x, g_aThunder[nCntWeather].pos.y, g_aThunder[nCntWeather].pos.z);
			D3DXMatrixMultiply(&g_aThunder[nCntWeather].mtxWorld, &g_aThunder[nCntWeather].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aThunder[nCntWeather].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffThunder, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, NULL);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWeather * 8, 6);
		}
	}

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Zバッファ更新の有効 / 無効の設定

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//======================================================================================================================
//	雨の設定処理
//======================================================================================================================
void SetRain(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 fRadius)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWeather->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_RAIN; nCntWeather++)
	{ // エフェクトの最大表示数分繰り返す

		if (g_aRain[nCntWeather].bUse == false)
		{ // エフェクトが使用されていない場合

			// 引数を代入
			g_aRain[nCntWeather].pos = pos;			// 位置
			g_aRain[nCntWeather].move = move;		// 移動量
			g_aRain[nCntWeather].fRadius = fRadius;	// 半径

			// 使用している状態にする
			g_aRain[nCntWeather].bUse = true;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aRain[nCntWeather].fRadius.x, +g_aRain[nCntWeather].fRadius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aRain[nCntWeather].fRadius.x, +g_aRain[nCntWeather].fRadius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aRain[nCntWeather].fRadius.x, -g_aRain[nCntWeather].fRadius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aRain[nCntWeather].fRadius.x, -g_aRain[nCntWeather].fRadius.y, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = RAIN_COL;
			pVtx[1].col = RAIN_COL;
			pVtx[2].col = RAIN_COL;
			pVtx[3].col = RAIN_COL;

			// 処理を抜ける
			break;
		}

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffWeather->Unlock();
}

//======================================================================================================================
// 雪の設定処理
//======================================================================================================================
void SetSnow(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 fRadius)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWeather->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_SNOW; nCntWeather++)
	{ // エフェクトの最大表示数分繰り返す

		if (g_aSnow[nCntWeather].bUse == false)
		{ // エフェクトが使用されていない場合

			// 引数を代入
			g_aSnow[nCntWeather].pos = pos;			// 位置
			g_aSnow[nCntWeather].move = move;		// 移動量
			g_aSnow[nCntWeather].fRadius = fRadius;	// 半径

			// 使用している状態にする
			g_aSnow[nCntWeather].bUse = true;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aSnow[nCntWeather].fRadius.x, +g_aSnow[nCntWeather].fRadius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aSnow[nCntWeather].fRadius.x, +g_aSnow[nCntWeather].fRadius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aSnow[nCntWeather].fRadius.x, -g_aSnow[nCntWeather].fRadius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aSnow[nCntWeather].fRadius.x, -g_aSnow[nCntWeather].fRadius.y, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = SNOW_COL;
			pVtx[1].col = SNOW_COL;
			pVtx[2].col = SNOW_COL;
			pVtx[3].col = SNOW_COL;

			// 処理を抜ける
			break;
		}

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffWeather->Unlock();
}

//======================================================================================================================
// 雷の設定処理
//======================================================================================================================
void SetThunder(D3DXVECTOR3 pos, D3DXVECTOR2 fRadius)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	Player *pPlayer = GetPlayer();		// プレイヤーの情報

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffThunder->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_THUNDER; nCntWeather++)
	{ // エフェクトの最大表示数分繰り返す

		if (g_aThunder[nCntWeather].bUse == false)
		{ // エフェクトが使用されていない場合

			// 引数を代入
			g_aThunder[nCntWeather].pos = pos;			// 位置
			g_aThunder[nCntWeather].fRadius = fRadius;	// 半径
			g_aThunder[nCntWeather].nVariCount = 0;		// カウント

			// ずらす幅をランダムで変える
			g_aThunder[nCntWeather].fShiftWidth = (float)(rand() % 300 + 100.0f);

			if (CollisionSector(pPlayer->pos, g_aThunder[nCntWeather].pos, pPlayer->rot.y, THUNDER_FLASH_RADIUS, THUNDER_FLASH_ANGLE) == true)
			{ // 視界内に雷が落ちた場合

				// フラッシュの設定処理
				SetFlash(WEATHER_ALPHA,REV_WEATHER_ALPHA);
			}

			// 使用している状態にする
			g_aThunder[nCntWeather].bUse = true;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aThunder[nCntWeather].fRadius.x, +g_aThunder[nCntWeather].fRadius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aThunder[nCntWeather].fRadius.x, +g_aThunder[nCntWeather].fRadius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aThunder[nCntWeather].fRadius.x, -g_aThunder[nCntWeather].fRadius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aThunder[nCntWeather].fRadius.x, -g_aThunder[nCntWeather].fRadius.y, 0.0f);
			pVtx[4].pos = D3DXVECTOR3(-g_aThunder[nCntWeather].fRadius.x, -g_aThunder[nCntWeather].fRadius.y, 0.0f);
			pVtx[5].pos = D3DXVECTOR3(+g_aThunder[nCntWeather].fRadius.x, -g_aThunder[nCntWeather].fRadius.y, 0.0f);
			pVtx[6].pos = D3DXVECTOR3(-g_aThunder[nCntWeather].fRadius.x, -g_aThunder[nCntWeather].fRadius.y, 0.0f);
			pVtx[7].pos = D3DXVECTOR3(+g_aThunder[nCntWeather].fRadius.x, -g_aThunder[nCntWeather].fRadius.y, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = THUNDER_COL;
			pVtx[1].col = THUNDER_COL;
			pVtx[2].col = THUNDER_COL;
			pVtx[3].col = THUNDER_COL;
			pVtx[4].col = THUNDER_COL;
			pVtx[5].col = THUNDER_COL;
			pVtx[6].col = THUNDER_COL;
			pVtx[7].col = THUNDER_COL;

			// 処理を抜ける
			break;
		}

		// 頂点データのポインタを 4つ分進める
		pVtx += 8;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffThunder->Unlock();
}

//======================================================================================================================
// 天気の取得処理
//======================================================================================================================
WEATHERTYPE GetWeather(void)
{
	// 天気の状態を返す
	return g_Weather;
}

//======================================================================================================================
//雨を降らせる処理
//======================================================================================================================
void WeatherRain(void)
{
	D3DXVECTOR3 Camerapos = GetCamera(CAMERATYPE_MAIN)->posV;	// カメラの視点
	D3DXVECTOR3 Camerarot = GetCamera(CAMERATYPE_MAIN)->rot;	// カメラの向き
	D3DXVECTOR3 posRain;	// 雨の降る位置
	float moveRain;			// 雨の移動量

	for (int nCnt = 0; nCnt < RAIN_GENERATE; nCnt++)
	{
		// 雨の位置を設定する
		posRain.x = Camerapos.x + (sinf(Camerarot.y) * SHIFT_RAIN) + (rand() % RAIN_RANGE - (int)(RAIN_RANGE * 0.5f));
		posRain.y = Camerapos.y + RAIN_HEIGHT;
		posRain.z = Camerapos.z + (cosf(Camerarot.y) * SHIFT_RAIN) + (rand() % RAIN_RANGE - (int)(RAIN_RANGE * 0.5f));

		// 速度を設定する
		moveRain = (rand() % RAIN_MOVE_RANGE) + RAIN_MOVE_LEAST;

		// 雨の設定処理
		SetRain
		(
			posRain,									// 位置
			D3DXVECTOR3(0.0f, moveRain, 0.0f),			// 移動量
			D3DXVECTOR2(RAIN_RADIUS_X,RAIN_RADIUS_Y)	// 半径
		);
	}
}

//======================================================================================================================
// 雪を降らせる処理
//======================================================================================================================
void WeatherSnow(void)
{
	D3DXVECTOR3 Camerapos = GetCamera(CAMERATYPE_MAIN)->posV;	// カメラの視点
	D3DXVECTOR3 Camerarot = GetCamera(CAMERATYPE_MAIN)->rot;	// カメラの向き
	D3DXVECTOR3 posSnow;	// 雪の降る位置
	float moveSnow;			// 雪の移動量

	for (int nCnt = 0; nCnt < SNOW_GENERATE; nCnt++)
	{
		// 雨の位置を設定する
		posSnow.x = Camerapos.x + (sinf(Camerarot.y) * SHIFT_SNOW) + (rand() % SNOW_RANGE - (int)(SNOW_RANGE * 0.5f));
		posSnow.y = Camerapos.y + SNOW_HEIGHT;
		posSnow.z = Camerapos.z + (cosf(Camerarot.y) * SHIFT_SNOW) + (rand() % SNOW_RANGE - (int)(SNOW_RANGE * 0.5f));

		// 速度を設定する
		moveSnow = (float)(rand() % SNOW_MOVE_RANGE) + SNOW_MOVE_LEAST;

		// 雪の設定処理
		SetSnow
		(
			posSnow,									// 位置
			D3DXVECTOR3(0.0f, moveSnow, 0.0f),			// 移動量
			D3DXVECTOR2(SNOW_RADIUS_X, SNOW_RADIUS_Y)	// 半径
		);
	}
}

//======================================================================================================================
// 雷を降らせる処理
//======================================================================================================================
void WeatherThunder(void)
{
	D3DXVECTOR3 posThunder;						// 雷の降る位置
	float rotThunder;							// 雷の降っている方向
	int nThunderGene;							// 生産率

	switch (g_WeatherMode)
	{
	case MODE_GAME:			// ゲーム

		// 雷の間隔を設定する
		nThunderGene = THUNDER_INTERVAL;

		break;				// 抜け出す

	case MODE_RESULT:		// リザルト

		// 雷の間隔を設定する
		nThunderGene = THUNDER_RSL_INTERVAL;

		break;				// 抜け出す
	case MODE_RANKING:		// ランキング

		// 雷の間隔を設定する
		nThunderGene = THUNDER_RSL_INTERVAL;

		break;				// 抜け出す
	}

	if (g_nThunderCount % nThunderGene == 0)
	{ // 雷の降る間隔が経過した場合

		// 雷の位置を設定する
		posThunder.x = (float)(rand() % THUNDER_RANGE - (int)(THUNDER_RANGE * 0.5f));
		posThunder.y = THUNDER_HEIGHT;
		posThunder.z = (float)(rand() % THUNDER_RANGE - (int)(THUNDER_RANGE * 0.5f));

		// 角度を取る
		rotThunder = atan2f(posThunder.x - 0.0f, posThunder.z - 0.0f);

		posThunder.x += sinf(rotThunder) * SHIFT_THUNDER;
		posThunder.z += cosf(rotThunder) * SHIFT_THUNDER;

		// 雷の設定処理
		SetThunder
		(
			posThunder,										// 位置
			D3DXVECTOR2(THUNDER_RADIUS_X, THUNDER_RADIUS_Y)	// 半径
		);
	}
}

//======================================================================================================================
// 天気の設定処理
//======================================================================================================================
void SetWeather(void)
{
	switch (g_Weather)
	{
	case WEATHERTYPE_SUNNY:		// 晴れ

		break;					// 抜け出す

	case WEATHERTYPE_RAIN:		// 雨

		// 雨を降らせる処理
		WeatherRain();

		break;					// 抜け出す

	case WEATHERTYPE_SNOW:		// 雪

		// 雪を降らせる処理
		WeatherSnow();

		break;					// 抜け出す

	case WEATHERTYPE_THUNDER:	// 雷

		// 雷を降らせる間隔を加算する
		g_nThunderCount++;

		// 雨を降らせる処理
		WeatherRain();

		// 雷を降らせる処理
		WeatherThunder();		

		break;					// 抜け出す
	}
}

//======================================================================================================================
// 雨の総数取得処理
//======================================================================================================================
int GetNumWeather(void)
{
	// 降っている総数を返す
	return g_NumWeather;
}


#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================
////**********************************************************************************************************************
////	エフェクトの総数取得処理
////**********************************************************************************************************************
//int GetNumWeather(void)
//{
//	// 変数を宣言
//	int nNumWeather = 0;	// エフェクトの総数の確認用
//
//	for (int nCntWeather = 0; nCntWeather < MAX_EFFECT; nCntWeather++)
//	{ // エフェクトの最大表示数分繰り返す
//
//		if (g_aWeather[nCntWeather].bUse == true)
//		{ // エフェクトが使用されている場合
//
//		  // カウンターを加算
//			nNumWeather++;
//		}
//	}
//
//	// 変数の値を返す
//	return nNumWeather;	// エフェクトの総数
//}
#endif