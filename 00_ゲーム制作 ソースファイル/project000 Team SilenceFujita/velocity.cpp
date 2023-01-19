//======================================================================================================================
//
//	体力バー処理 [life.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "life.h"
#include "player.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_LIFE		(3)			// 使用するポリゴン数

#define LIFE_POS_X		(70.0f)	// 体力バーの絶対座標 (x)
#define LIFE_POS_Y		(60.0f)		// 体力バーの絶対座標 (y)
#define LIFE_WIDTH_MUL	(3.8f)		// 体力バーの横幅のプレイヤー体力乗算量
#define LIFE_HEIGHT		(25.0f)		// 体力バーの縦幅 / 2

#define LIFE_BG_POS_X	(260.0f)	// 体力バーの背景の絶対座標 (x)
#define LIFE_BG_WIDTH	(225.0f)	// 体力バーの背景の横幅 / 2
#define LIFE_BG_HEIGHT	(45.0f)		// 体力バーの背景の縦幅 / 2

#define CNT_FRAME		((int)(DAMAGE_TIME_PLAY * 0.5f))	// 体力を減らしきるフレーム数

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTextureLife[] =		// テクスチャの相対パス
{
	"data\\TEXTURE\\ui000.png",		// ライフ背景のテクスチャの相対パス
	NULL,							// NULL
	NULL,							// NULL
};

//**********************************************************************************************************************
//	列挙型定義 (TEXTURE_LIFE)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_LIFE_NORMAL = 0,		// ライフの背景
	TEXTURE_LIFE_NULL_00,			// NULL
	TEXTURE_LIFE_NULL_01,			// NULL
	TEXTURE_LIFE_MAX,				// この列挙型の総数
} TEXTURE_LIFE;

//**********************************************************************************************************************
//	構造体を定義 (Life)
//**********************************************************************************************************************
typedef struct
{
	LIFESTATE state;				// 状態
	float     fChange;				// 1フレームの体力変動量
	float     fNowLife;				// 現在の体力
	int       nCounterState;		// 状態管理カウンター
} Life;

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureLife[TEXTURE_LIFE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;					// 頂点バッファへのポインタ

Life g_life;	// 体力の情報

//======================================================================================================================
//	体力バーの初期化処理
//======================================================================================================================
void InitLife(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_2D *pVtx;							// 頂点情報へのポインタ

	for (int nCntLife = 0; nCntLife < TEXTURE_LIFE_MAX; nCntLife++)
	{ // 使用するテクスチャ数分繰り返す

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, apTextureLife[nCntLife], &g_apTextureLife[nCntLife]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_2D) * 4 * MAX_LIFE,		// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife,
		NULL
	);

	// 体力の情報の初期化
	g_life.state         = LIFESTATE_NORMAL;	// 状態
	g_life.fChange       = 0;					// 1フレームの体力変動量
	g_life.fNowLife      = (float)PLAY_LIFE;	// 現在の体力
	g_life.nCounterState = 0;					// 状態管理カウンター

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------------------------------------------------------------------------------------------
	//	体力バー (背景)
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[0].pos = D3DXVECTOR3(LIFE_BG_POS_X - LIFE_BG_WIDTH, LIFE_POS_Y - LIFE_BG_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(LIFE_BG_POS_X + LIFE_BG_WIDTH, LIFE_POS_Y - LIFE_BG_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(LIFE_BG_POS_X - LIFE_BG_WIDTH, LIFE_POS_Y + LIFE_BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(LIFE_BG_POS_X + LIFE_BG_WIDTH, LIFE_POS_Y + LIFE_BG_HEIGHT, 0.0f);

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

	//------------------------------------------------------------------------------------------------------------------
	//	体力バー (赤)
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[4].pos = D3DXVECTOR3(LIFE_POS_X,                                       LIFE_POS_Y - LIFE_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(LIFE_POS_X + (float)(PLAY_LIFE * LIFE_WIDTH_MUL), LIFE_POS_Y - LIFE_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(LIFE_POS_X,                                       LIFE_POS_Y + LIFE_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(LIFE_POS_X + (float)(PLAY_LIFE * LIFE_WIDTH_MUL), LIFE_POS_Y + LIFE_HEIGHT, 0.0f);

	// rhw の設定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[4].col = D3DCOLOR_RGBA(255, 50, 50, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 50, 50, 255);
	pVtx[6].col = D3DCOLOR_RGBA(205,  0,  0, 255);
	pVtx[7].col = D3DCOLOR_RGBA(205,  0,  0, 255);

	//------------------------------------------------------------------------------------------------------------------
	//	体力バー (黄)
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[8].pos  = D3DXVECTOR3(LIFE_POS_X,                                       LIFE_POS_Y - LIFE_HEIGHT, 0.0f);
	pVtx[9].pos  = D3DXVECTOR3(LIFE_POS_X + (float)(PLAY_LIFE * LIFE_WIDTH_MUL), LIFE_POS_Y - LIFE_HEIGHT, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(LIFE_POS_X,                                       LIFE_POS_Y + LIFE_HEIGHT, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(LIFE_POS_X + (float)(PLAY_LIFE * LIFE_WIDTH_MUL), LIFE_POS_Y + LIFE_HEIGHT, 0.0f);

	// rhw の設定
	pVtx[8].rhw  = 1.0f;
	pVtx[9].rhw  = 1.0f;
	pVtx[10].rhw = 1.0f;
	pVtx[11].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[8].col  = D3DCOLOR_RGBA(255, 250, 50, 255);
	pVtx[9].col  = D3DCOLOR_RGBA(255, 250, 50, 255);
	pVtx[10].col = D3DCOLOR_RGBA(205, 160,  0, 255);
	pVtx[11].col = D3DCOLOR_RGBA(205, 160,  0, 255);

	// 頂点バッファをアンロックする
	g_pVtxBuffLife->Unlock();
}

//======================================================================================================================
//	体力バーの終了処理
//======================================================================================================================
void UninitLife(void)
{
	// テクスチャの破棄
	for (int nCntLife = 0; nCntLife < TEXTURE_LIFE_MAX; nCntLife++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTextureLife[nCntLife] != NULL)
		{ // 変数 (g_apTextureLife) がNULLではない場合

			g_apTextureLife[nCntLife]->Release();
			g_apTextureLife[nCntLife] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffLife != NULL)
	{ // 変数 (g_pVtxBuffLife) がNULLではない場合

		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}

//======================================================================================================================
//	体力バーの更新処理
//======================================================================================================================
void UpdateLife(void)
{
	// ポインタを宣言
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	if (g_life.state == LIFESTATE_CHANGE)
	{ // 体力変動状態の場合

		// カウンターを減算
		g_life.nCounterState--;

		// 現在の体力バーから受けたダメージ分を加算
		g_life.fNowLife += g_life.fChange;

		// 頂点座標を設定
		pVtx[8].pos  = D3DXVECTOR3(LIFE_POS_X,                                      LIFE_POS_Y - LIFE_HEIGHT, 0.0f);
		pVtx[9].pos  = D3DXVECTOR3(LIFE_POS_X + (g_life.fNowLife * LIFE_WIDTH_MUL), LIFE_POS_Y - LIFE_HEIGHT, 0.0f);
		pVtx[10].pos = D3DXVECTOR3(LIFE_POS_X,                                      LIFE_POS_Y + LIFE_HEIGHT, 0.0f);
		pVtx[11].pos = D3DXVECTOR3(LIFE_POS_X + (g_life.fNowLife * LIFE_WIDTH_MUL), LIFE_POS_Y + LIFE_HEIGHT, 0.0f);

		if (g_life.nCounterState <= 0)
		{ // カウンターが 0以下になった場合

			// 通常状態にする
			g_life.state = LIFESTATE_NORMAL;

			// 頂点座標を再設定
			pVtx[8].pos  = D3DXVECTOR3(LIFE_POS_X,                                           LIFE_POS_Y - LIFE_HEIGHT, 0.0f);
			pVtx[9].pos  = D3DXVECTOR3(LIFE_POS_X + (int)(g_life.fNowLife * LIFE_WIDTH_MUL), LIFE_POS_Y - LIFE_HEIGHT, 0.0f);
			pVtx[10].pos = D3DXVECTOR3(LIFE_POS_X,                                           LIFE_POS_Y + LIFE_HEIGHT, 0.0f);
			pVtx[11].pos = D3DXVECTOR3(LIFE_POS_X + (int)(g_life.fNowLife * LIFE_WIDTH_MUL), LIFE_POS_Y + LIFE_HEIGHT, 0.0f);
		}

		// 体力ゲージの補正
		if (pVtx[9].pos.x < LIFE_POS_X)
		{ // ゲージが減りすぎた場合

			// 体力ゲージを 0に補正
			pVtx[9].pos  = D3DXVECTOR3(LIFE_POS_X, LIFE_POS_Y - LIFE_HEIGHT, 0.0f);
			pVtx[11].pos = D3DXVECTOR3(LIFE_POS_X, LIFE_POS_Y + LIFE_HEIGHT, 0.0f);
		}
		else if (pVtx[9].pos.x > LIFE_POS_X + (float)(PLAY_LIFE * LIFE_WIDTH_MUL))
		{ // ゲージが増えすぎた場合

			// 体力ゲージを 100に補正
			pVtx[9].pos  = D3DXVECTOR3(LIFE_POS_X + (float)(PLAY_LIFE * LIFE_WIDTH_MUL), LIFE_POS_Y - LIFE_HEIGHT, 0.0f);
			pVtx[11].pos = D3DXVECTOR3(LIFE_POS_X + (float)(PLAY_LIFE * LIFE_WIDTH_MUL), LIFE_POS_Y + LIFE_HEIGHT, 0.0f);
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffLife->Unlock();
}

//======================================================================================================================
//	体力バーの描画処理
//======================================================================================================================
void DrawLife(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{ // 使用するポリゴン数分繰り返す

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureLife[nCntLife]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLife * 4, 2);
	}
}

//======================================================================================================================
//	体力バーの設定処理
//======================================================================================================================
void SetLife(int nLife, int nDamage)
{
	// 引数を代入
	g_life.fNowLife = (float)nLife;		// 現在の体力

	// 1フレームの体力変動量を求める
	g_life.fChange = ((float)nDamage / (float)CNT_FRAME);

	// 状態を設定
	g_life.state = LIFESTATE_CHANGE;	// 数値変動状態

	// カウンターを設定
	g_life.nCounterState = CNT_FRAME;	// 状態管理カウンター
}