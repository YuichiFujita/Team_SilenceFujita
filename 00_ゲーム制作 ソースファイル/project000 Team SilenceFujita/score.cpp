//======================================================================================================================
//
//	スコア処理 [score.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "score.h"
#include "value.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_SCORE		(1)			// 使用するポリゴン数

#define VAL_SCO_WIDTH	(30.0f)		// スコアの数値の横幅 / 2
#define VAL_SCO_HEIGHT	(30.0f)		// スコアの数値の縦幅 / 2
#define VAL_SCO_SPACE	(47.0f)		// スコアの数値間の幅 (x)

#define SCORE_POS_VAL_X	(1210.0f)	// スコア (値) の絶対座標 (x)
#define SCORE_WIDTH		(235.0f)	// スコアの横幅 / 2
#define SCORE_HEIGHT	(45.0f)		// スコアの縦幅 / 2

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTextureScore[] =		// テクスチャの相対パス
{
	"Data\\Texture\\ui000.png",		// スコア背景のテクスチャ相対パス
};

//**********************************************************************************************************************
//	列挙型定義 (TEXTURE_SCORE)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_SCORE_BG = 0,			// スコアの背景
	TEXTURE_SCORE_MAX,				// この列挙型の総数
} TEXTURE_SCORE;

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureScore[TEXTURE_SCORE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;						// 頂点バッファへのポインタ

int g_nScore;	// スコアの値

//======================================================================================================================
//	スコアの初期化処理
//======================================================================================================================
void InitScore(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_2D         *pVtx;					// 頂点情報へのポインタ

	for (int nCntScore = 0; nCntScore < TEXTURE_SCORE_MAX; nCntScore++)
	{ // 使用するテクスチャ数分繰り返す

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, apTextureScore[nCntScore], &g_apTextureScore[nCntScore]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_2D) * 4 * MAX_SCORE,		// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL
	);

	// グローバル変数を初期化
	g_nScore = 0;				// スコアの値

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(SCORE_POS_X - SCORE_WIDTH, SCORE_POS_Y - SCORE_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCORE_POS_X + SCORE_WIDTH, SCORE_POS_Y - SCORE_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCORE_POS_X - SCORE_WIDTH, SCORE_POS_Y + SCORE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCORE_POS_X + SCORE_WIDTH, SCORE_POS_Y + SCORE_HEIGHT, 0.0f);

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
	g_pVtxBuffScore->Unlock();
}

//======================================================================================================================
//	スコアの終了処理
//======================================================================================================================
void UninitScore(void)
{
	// テクスチャの破棄
	for (int nCntScore = 0; nCntScore < TEXTURE_SCORE_MAX; nCntScore++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTextureScore[nCntScore] != NULL)
		{ // 変数 (g_apTextureScore) がNULLではない場合

			g_apTextureScore[nCntScore]->Release();
			g_apTextureScore[nCntScore] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{ // 変数 (g_pVtxBuffScore) がNULLではない場合

		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//======================================================================================================================
//	スコアの更新処理
//======================================================================================================================
void UpdateScore(void)
{

}

//======================================================================================================================
//	スコアの描画処理
//======================================================================================================================
void DrawScore(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//------------------------------------------------------------------------------------------------------------------
	//	背景の描画
	//------------------------------------------------------------------------------------------------------------------
	// テクスチャの設定
	pDevice->SetTexture(0, g_apTextureScore[TEXTURE_SCORE_BG]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//------------------------------------------------------------------------------------------------------------------
	//	数値の描画
	//------------------------------------------------------------------------------------------------------------------
	// 数値の設定
	SetValue
	( // 引数
		D3DXVECTOR3
		( // 引数
			SCORE_POS_VAL_X,	// 位置 (x)
			SCORE_POS_Y,		// 位置 (y)
			0.0f				// 位置 (z)
		),
		g_nScore,		// 値
		VAL_MAX_SCORE,	// 値の最大値
		VAL_SCO_WIDTH,	// 横幅
		VAL_SCO_HEIGHT,	// 縦幅
		VAL_SCO_SPACE,	// 数値間の幅
		1.0f			// α値
	);

	// 数値の描画
	DrawValue(VAL_SCO_DIGIT, VALUETYPE_NORMAL);
}

//======================================================================================================================
//	スコアの加算処理
//======================================================================================================================
void AddScore(int nValse)
{
	if (g_nScore < VAL_MAX_SCORE)
	{ // 現在のスコアの値がスコアの最大値より小さい場合

		// スコアに引数の値を加算
		g_nScore += nValse;

		if (g_nScore > VAL_MAX_SCORE)
		{ // 現在のスコアの値がスコアの最大値より大きい場合

			// スコアを補正
			g_nScore = VAL_MAX_SCORE;
		}
	}
}

//======================================================================================================================
//	スコアの取得処理
//======================================================================================================================
int GetScore(void)
{
	// スコアの値を返す
	return g_nScore;
}