//======================================================================================================================
//
//	ポーズ画面処理 [pause.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

#include "pause.h"

#include "game.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_PAUSE		(4)			// 使用するポリゴン数

#define PAUSE_POS_X		(640.0f)	// セレクトメニューの絶対座標 (x)
#define PAUSE_POS_Y		(210.0f)	// セレクトメニューの絶対座標 (y)

#define PAUSE_WIDTH		(220.0f)	// セレクトメニューの幅 / 2 (横)
#define PAUSE_HEIGHT	(55.0f)		// セレクトメニューの幅 / 2 (高さ)
#define PAUSE_SPACE		(150.0f)	// セレクトメニュー間の幅

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTexturePause[] =		// テクスチャの相対パス
{
	NULL,							// NULL
	"data\\TEXTURE\\pause000.png",	// RESUME のテクスチャ相対パス
	"data\\TEXTURE\\pause001.png",	// RETRY のテクスチャ相対パス
	"data\\TEXTURE\\pause002.png",	// EXIT のテクスチャ相対パス
};

//**********************************************************************************************************************
//	列挙型定義 (TEXTURE_PAUSE)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_PAUSE_NULL_00 = 0,		// NULL
	TEXTURE_PAUSE_RESUME,			// RESUME
	TEXTURE_PAUSE_RETRY,			// RETRY
	TEXTURE_PAUSE_EXIT,				// EXIT
	TEXTURE_PAUSE_MAX,				// この列挙型の総数
} TEXTURE_PAUSE;

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTexturePause[TEXTURE_PAUSE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;						// 頂点バッファへのポインタ

int  g_nSelectPause;				// 現在の選択中メニュー
int  g_nOldSelectPause;				// 前回の選択中メニュー
bool g_bPauseEnd;					// ポーズ終了確認用

//======================================================================================================================
//	ポーズ画面の初期化処理
//======================================================================================================================
void InitPause(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ (ポインタの宣言だが * はいらない)
	VERTEX_2D *pVtx;				// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	for (int nCntPause = 0; nCntPause < TEXTURE_PAUSE_MAX; nCntPause++)
	{ // 使用するテクスチャ数分繰り返す

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, apTexturePause[nCntPause], &g_apTexturePause[nCntPause]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_2D) * 4 * MAX_PAUSE,		// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL
	);

	// 変数の値を初期化
	g_nSelectPause    = PAUSESELECT_RESUME;		// 現在の選択中メニューを初期化
	g_nOldSelectPause = PAUSESELECT_RESUME;		// 前回の選択中メニューを初期化

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------------------------------------------------------------------------------------------
	//	背景 (フェード) の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[0].pos = D3DXVECTOR3(0.0f,         0.0f,          0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f,          0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f,         SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhw の設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);

	//------------------------------------------------------------------------------------------------------------------
	//	セレクトメニューの初期化
	//------------------------------------------------------------------------------------------------------------------
	for (int nCntPause = 0; nCntPause < PAUSESELECT_MAX; nCntPause++)
	{ // ポーズの項目数分繰り返す

		// 頂点座標を設定
		pVtx[4].pos = D3DXVECTOR3(PAUSE_POS_X - PAUSE_WIDTH, PAUSE_POS_Y + (nCntPause * PAUSE_SPACE) - PAUSE_HEIGHT, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(PAUSE_POS_X + PAUSE_WIDTH, PAUSE_POS_Y + (nCntPause * PAUSE_SPACE) - PAUSE_HEIGHT, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(PAUSE_POS_X - PAUSE_WIDTH, PAUSE_POS_Y + (nCntPause * PAUSE_SPACE) + PAUSE_HEIGHT, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(PAUSE_POS_X + PAUSE_WIDTH, PAUSE_POS_Y + (nCntPause * PAUSE_SPACE) + PAUSE_HEIGHT, 0.0f);

		// rhw の設定
		pVtx[4].rhw = 1.0f;
		pVtx[5].rhw = 1.0f;
		pVtx[6].rhw = 1.0f;
		pVtx[7].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[4].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[5].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[6].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[7].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

		// テクスチャ座標の設定
		pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();
}

//======================================================================================================================
//	ポーズ画面の終了処理
//======================================================================================================================
void UninitPause(void)
{
	// テクスチャの破棄
	for (int nCntPause = 0; nCntPause < TEXTURE_PAUSE_MAX; nCntPause++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTexturePause[nCntPause] != NULL)
		{ // 変数 (g_apTexturePause) がNULLではない場合

			g_apTexturePause[nCntPause]->Release();
			g_apTexturePause[nCntPause] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{ // 変数 (g_pVtxBuffPause) がNULLではない場合

		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//======================================================================================================================
//	ポーズ画面の更新処理
//======================================================================================================================
void UpdatePause(void)
{
	// ポインタを宣言
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	//------------------------------------------------------------------------------------------------------------------
	//	操作の更新
	//------------------------------------------------------------------------------------------------------------------
	if (GetFade() == FADE_NONE)
	{ // フェードしていない場合

		if (GetKeyboardTrigger(DIK_W) == true || GetKeyboardTrigger(DIK_UP) == true || GetJoyKeyTrigger(JOYKEY_UP, 0) == true)
		{ // 上移動の操作が行われた場合

			// 変数 (g_nSelectPause) を 1ずつ引いていく ( 0の場合 1に戻す)
			g_nSelectPause = (g_nSelectPause + (PAUSESELECT_MAX - 1)) % PAUSESELECT_MAX;

			// サウンドの再生
			//PlaySound(SOUND_LABEL_SE_MOVE);			// 移動音 (システム)
		}
		else if (GetKeyboardTrigger(DIK_S) == true || GetKeyboardTrigger(DIK_DOWN) == true || GetJoyKeyTrigger(JOYKEY_DOWN, 0) == true)
		{ // 下移動の操作が行われた場合

			// 変数 (g_nSelectPause) を 1ずつ足していく ( 1の場合 0に戻す)
			g_nSelectPause = (g_nSelectPause + 1) % PAUSESELECT_MAX;

			// サウンドの再生
			//PlaySound(SOUND_LABEL_SE_MOVE);			// 移動音 (システム)
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_SPACE) == true
		||  GetJoyKeyTrigger(JOYKEY_A, 0)  == true || GetJoyKeyTrigger(JOYKEY_B, 0) == true
		||  GetJoyKeyTrigger(JOYKEY_X, 0)  == true || GetJoyKeyTrigger(JOYKEY_Y, 0) == true)
		{ // 決定の操作が行われた場合

			// サウンドの再生
			//PlaySound(SOUND_LABEL_SE_DECISION_01);	// 決定音01 (システム)

			switch (g_nSelectPause)
			{ // 画面の移行先の確認
			case PAUSESELECT_RESUME:	// 再開の場合

				// ポーズを終了した状態にする
				g_bPauseEnd = true;

				// ポーズ状態の設定
				SetEnablePause(false);

				// 処理を抜ける
				break;

			case PAUSESELECT_RETRY:		// リトライの場合

				// モード選択 (ゲーム画面に移行)
				SetFade(MODE_GAME);

				// 処理を抜ける
				break;

			case PAUSESELECT_EXIT:		// 終了の場合

				// モード選択 (タイトル画面に移行)
				SetFade(MODE_TITLE);

				// 処理を抜ける
				break;
			}
		}
	}

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の更新
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点データのポインタを前フレームにセレクトしていた要素の頂点に移動
	pVtx += (g_nOldSelectPause * 4);

	// 頂点カラーの更新
	pVtx[4].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	pVtx[5].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	pVtx[6].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	pVtx[7].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

	// 頂点データのポインタを先頭アドレスに戻す
	pVtx -= (g_nOldSelectPause * 4);

	// 頂点データのポインタを今フレームでセレクトしている要素の頂点に移動
	pVtx += (g_nSelectPause * 4);

	// 頂点カラーの更新
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 現在セレクト中の要素を代入
	g_nOldSelectPause = g_nSelectPause;

	// 頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();
}

//======================================================================================================================
//	ポーズ画面の描画処理
//======================================================================================================================
void DrawPause(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ (ポインタの宣言だが * はいらない)

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	{ // 使用するポリゴン数分繰り返す

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTexturePause[nCntPause]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);
	}
}

//======================================================================================================================
//	ポーズの真偽の設定処理
//======================================================================================================================
void SetEnablePauseEnd(bool bPauseEnd)
{
	// 現在のポーズ終了状態に引数のポーズ終了状態を代入
	g_bPauseEnd = bPauseEnd;
}

//======================================================================================================================
//	ポーズの状態の取得処理
//======================================================================================================================
bool GetPauseEnd(void)
{
	// ポーズ終了状態の真偽を返す
	return g_bPauseEnd;
}

//======================================================================================================================
//	ポーズの選択項目の再設定処理
//======================================================================================================================
void ResetPauseSelect(void)
{
	// ポーズの選択項目を初期化
	g_nSelectPause = PAUSESELECT_RESUME;
}