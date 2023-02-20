//======================================================================================================================
//
//	タイマー処理 [timer.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "fade.h"
#include "game.h"
#include "gate.h"
#include "timer.h"
#include "value.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_TIMER		(1)			// 使用するポリゴン数
#define MAX_VAL_TIMER	(2)			// タイマーの数値表示の最大数
#define MAX_MIN			(60)		// タイム (分) の最大値
#define MAX_SEC			(60)		// タイム (秒) の最大値
#define NUM_RED_TIMER	(10)		// タイマーが赤くなり始める時間

#define VAL_TIME_WIDTH	(28.0f)		// タイマーの数値の横幅 / 2
#define VAL_TIME_HEIGHT	(28.0f)		// タイマーの数値の縦幅 / 2
#define VAL_TIME_SPACE	(38.0f)		// タイマーの数値間の幅 (x)

#define TIMER_POS_MIN_X	(1085.0f)	// タイマー (分) の絶対座標 (x)
#define TIMER_POS_SEC_X	(1185.0f)	// タイマー (秒) の絶対座標 (x)
#define TIMER_POS_X		(1115.0f)	// タイマー (背景) の絶対座標 (x)
#define TIMER_POS_Y		(320.0f)	// タイマーの絶対座標 (y)
#define TIMER_WIDTH		(32.0f)		// タイマーの横幅 / 2
#define TIMER_HEIGHT	(32.0f)		// タイマーの縦幅 / 2

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTextureTimer[] =		// テクスチャの相対パス
{
	"data\\TEXTURE\\timer000.png",	// タイマーコロン(：) (通常) のテクスチャ相対パス
	"data\\TEXTURE\\timer001.png",	// タイマーコロン(：) (赤) のテクスチャ相対パス
};

//**********************************************************************************************************************
//	列挙型定義 (TEXTURE_TIMER)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_TIMER_NORMAL = 0,		// タイマーコロン(：) (通常)
	TEXTURE_TIMER_RED,				// タイマーコロン(：) (赤)
	TEXTURE_TIMER_MAX,				// この列挙型の総数
} TEXTURE_TIMER;

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureTimer[TEXTURE_TIMER_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;						// 頂点バッファへのポインタ

TIMERSTATE g_timerState;			// タイマーの状態
int        g_nTime;					// 現在のタイム

//======================================================================================================================
//	タイマーの初期化処理
//======================================================================================================================
void InitTimer(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_2D         *pVtx;					// 頂点情報へのポインタ

	for (int nCntTimer = 0; nCntTimer < TEXTURE_TIMER_MAX; nCntTimer++)
	{ // 使用するテクスチャ数分繰り返す

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, apTextureTimer[nCntTimer], &g_apTextureTimer[nCntTimer]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_2D) * 4 * MAX_TIMER,		// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL
	);

	// グローバル変数を初期化
	g_timerState = TIMERSTATE_COUNT;	// タイマーの状態
	g_nTime      = NUM_TIME_GAME;		// 現在のタイム

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(TIMER_POS_X - TIMER_WIDTH, TIMER_POS_Y - TIMER_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TIMER_POS_X + TIMER_WIDTH, TIMER_POS_Y - TIMER_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TIMER_POS_X - TIMER_WIDTH, TIMER_POS_Y + TIMER_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TIMER_POS_X + TIMER_WIDTH, TIMER_POS_Y + TIMER_HEIGHT, 0.0f);

	// rhw の設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffTimer->Unlock();
}

//======================================================================================================================
//	タイマーの終了処理
//======================================================================================================================
void UninitTimer(void)
{
	// テクスチャの破棄
	for (int nCntTimer = 0; nCntTimer < TEXTURE_TIMER_MAX; nCntTimer++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTextureTimer[nCntTimer] != NULL)
		{ // 変数 (g_apTextureTimer) がNULLではない場合

			g_apTextureTimer[nCntTimer]->Release();
			g_apTextureTimer[nCntTimer] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTimer != NULL)
	{ // 変数 (g_pVtxBuffTimer) がNULLではない場合

		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}
}

//======================================================================================================================
//	タイマーの更新処理
//======================================================================================================================
void UpdateTimer(void)
{
	switch (g_timerState)
	{ // 状態ごとの処理
	case TIMERSTATE_COUNT:	// カウント状態の場合

		if (GetGameState() == GAMESTATE_NORMAL
		&&  GetFade()      == FADE_NONE)
		{ // ゲームが通常状態且つ、フェードしていない場合

			if (g_nTime > 0)
			{ // タイマーが 0より大きい場合

				// タイマーを減算
				g_nTime--;
			}
			else
			{ // カウンターが 0以下の場合

				// ゲートの全閉め処理
				AllShutOutGate();

				// カウント終了状態にする
				g_timerState = TIMERSTATE_END;
			}
		}

		// 処理を抜ける
		break;

	case TIMERSTATE_END:	// カウント終了状態の場合

		// 無し

		// 処理を抜ける
		break;
	}
}

//======================================================================================================================
//	タイマーの描画処理
//======================================================================================================================
void DrawTimer(void)
{
	// 変数を宣言
	TEXTURE_TIMER texBG;					// タイマー (背景) のテクスチャ
	VALUETYPE     texValue;					// タイマー (値) のテクスチャ

	int nMin =  g_nTime / 3600;				// タイマー (分)
	int nSec = (g_nTime % 3600) / 60;		// タイマー (秒)

	if (GetTime() > NUM_RED_TIMER)
	{ // タイマーが 10秒より大きい場合

		texBG    = TEXTURE_TIMER_NORMAL;	// 通常のテクスチャを設定
		texValue = VALUETYPE_NORMAL;		// 通常のテクスチャを設定
	}
	else
	{ // タイマーが 10秒以下の場合

		texBG    = TEXTURE_TIMER_RED;		// 赤のテクスチャを設定
		texValue = VALUETYPE_RED;			// 赤のテクスチャを設定
	}

	// 変数配列を宣言
	D3DXVECTOR3 aPosValue[MAX_VAL_TIMER] =					// タイマーの位置用
	{ // 初期値
		D3DXVECTOR3(TIMER_POS_MIN_X, TIMER_POS_Y, 0.0f),	// タイマー (分) の位置
		D3DXVECTOR3(TIMER_POS_SEC_X, TIMER_POS_Y, 0.0f)		// タイマー (秒) の位置
	};

	int aValue[MAX_VAL_TIMER]    = { nMin,    nSec    };	// タイマー表示用
	int aMaxValue[MAX_VAL_TIMER] = { MAX_MIN, MAX_SEC };	// タイマー表示の値の制限用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTimer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//------------------------------------------------------------------------------------------------------------------
	//	背景の描画
	//------------------------------------------------------------------------------------------------------------------
	// テクスチャの設定
	pDevice->SetTexture(0, g_apTextureTimer[texBG]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//------------------------------------------------------------------------------------------------------------------
	//	数値の描画
	//------------------------------------------------------------------------------------------------------------------
	for (int nCntTimer = 0; nCntTimer < MAX_VAL_TIMER; nCntTimer++)
	{ // タイマーの数値表示の最大数分繰り返す

		// 数値の設定
		SetValue
		( // 引数
			aPosValue[nCntTimer],	// 位置
			aValue[nCntTimer],		// 値
			aMaxValue[nCntTimer],	// 値の最大値
			VAL_TIME_WIDTH,			// 横幅
			VAL_TIME_HEIGHT,		// 縦幅
			VAL_TIME_SPACE,			// 数値間の幅
			1.0f					// α値
		);

		// 数値の描画
		DrawValue(VAL_TIME_DIGIT, texValue);
	}
}

//======================================================================================================================
//	タイムの取得処理
//======================================================================================================================
int GetTime(void)
{
	// タイムの値 (秒換算) を返す
	return g_nTime / 60;
}

//======================================================================================================================
//	タイマーの状態の取得処理
//======================================================================================================================
TIMERSTATE GetTimerState(void)
{
	// タイマーの状態を返す
	return g_timerState;
}