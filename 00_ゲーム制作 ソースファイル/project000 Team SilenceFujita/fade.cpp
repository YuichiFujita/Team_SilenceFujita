//======================================================================================================================
//
//	フェード処理 [fade.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "fade.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define FADE_LEVEL	(0.039f)	// フェードの 1Fの効果量

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;		// 頂点バッファへのポインタ
FADE      g_fade;				// フェードの状態
MODE      g_modeNext;			// 次の画面モード
D3DXCOLOR g_colorFade;			// ポリゴン(フェード)の色

//======================================================================================================================
//	フェードの初期化処理
//======================================================================================================================
void InitFade(MODE modeNext)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ (ポインタの宣言だが * はいらない)
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_2D) * 4,	// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL
	);

	// 変数の初期化
	g_fade = FADE_IN;									// フェードイン状態に設定
	g_modeNext = modeNext;								// 次の画面 (モード) を設定
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// ポリゴンを (不透明) に設定

	//----------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//----------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	// 頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();

	// モード設定 (次の画面に移行)
	SetMode(g_modeNext);
}

//======================================================================================================================
//	フェードの終了処理
//======================================================================================================================
void UninitFade(void)
{
	// 頂点バッファの破棄
	if (g_pVtxBuffFade != NULL)
	{ // 変数 (g_pVtxBuffFade) がNULLではない場合

		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//======================================================================================================================
//	フェードの更新処理
//======================================================================================================================
void UpdateFade(void)
{
	// ポインタを宣言
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	if (g_fade != FADE_NONE)
	{ // フェードが何もしない状態ではない場合

		if (g_fade == FADE_IN)
		{ // フェードイン状態の場合

			// α値を減算する (だんだん見えてくる)
			g_colorFade.a -= FADE_LEVEL;

			if (g_colorFade.a <= 0.0f)
			{ // α値が 0.0f を下回った場合

				g_colorFade.a = 0.0f;	// 下回っていない状態にする
				g_fade = FADE_NONE;		// 何もしない状態にする
			}
		}
		else if (g_fade == FADE_OUT)
		{ // フェードアウト状態の場合

			// α値を加算する (だんだん見えなくなる)
			g_colorFade.a += FADE_LEVEL;

			if (g_colorFade.a >= 1.0f)
			{ // α値が 1.0f を上回った場合

				g_colorFade.a = 1.0f;	// 上回っていない状態にする
				g_fade = FADE_IN;		// フェードイン状態にする

				// モード設定 (次の画面に移行)
				SetMode(g_modeNext);
			}
		}

		//--------------------------------------------------------------------------------------------------------------
		//	頂点情報の更新
		//--------------------------------------------------------------------------------------------------------------
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの更新
		pVtx[0].col = g_colorFade;
		pVtx[1].col = g_colorFade;
		pVtx[2].col = g_colorFade;
		pVtx[3].col = g_colorFade;

		// 頂点バッファをアンロックする
		g_pVtxBuffFade->Unlock();
	}
}

//======================================================================================================================
//	フェードの描画処理
//======================================================================================================================
void DrawFade(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ (ポインタの宣言だが * はいらない)

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//======================================================================================================================
//	フェードの設定処理
//======================================================================================================================
void SetFade(MODE modeNext)
{
	if (g_fade == FADE_NONE)
	{ // フェードが何もしていない状態の場合

		g_fade = FADE_OUT;									// フェードアウト状態に設定
		g_modeNext = modeNext;								// 次の画面 (モード) を設定
		g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// ポリゴンを (透明) に設定
	}
}

//======================================================================================================================
//	フェードの取得処理
//======================================================================================================================
FADE GetFade(void)
{
	// フェードの状態を返す
	return g_fade;
}