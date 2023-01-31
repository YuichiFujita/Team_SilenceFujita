//======================================================================================================================
//
//	能力バー処理 [ability.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "ability.h"
#include "player.h"
#include "wind.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_ABI_BOOST		(2)			// ブーストバーに使用するポリゴン数

#define ABI_BOOST_POS_X		(60.0f)		// 能力バーの絶対座標 (x)
#define ABI_BOOST_POS_Y		(135.0f)	// 能力バーの絶対座標 (y)
#define ABI_BOOST_WIDTH_MUL	(1.4f)		// 能力バーの横幅のプレイヤー能力乗算量
#define ABI_BOOST_HEIGHT	(15.0f)		// 能力バーの縦幅 / 2

#define MAX_ABI_WIND		(2)			// ウィンドバーに使用するポリゴン数

#define ABI_WIND_POS_X		(60.0f)		// 能力バーの絶対座標 (x)
#define ABI_WIND_POS_Y		(175.0f)	// 能力バーの絶対座標 (y)
#define ABI_WIND_WIDTH_MUL	(1.4f)		// 能力バーの横幅のプレイヤー能力乗算量
#define ABI_WIND_HEIGHT		(15.0f)		// 能力バーの縦幅 / 2

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTextureAbility[] =	// テクスチャの相対パス
{
	"data\\TEXTURE\\ui000.png",		// 能力バー背景のテクスチャの相対パス
};

//**********************************************************************************************************************
//	列挙型定義 (TEXTURE_ABI)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_ABI_NORMAL = 0,			// 能力バーの背景
	TEXTURE_ABI_MAX,				// この列挙型の総数
} TEXTURE_ABI;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void InitAbiBoost(void);			// ブーストゲージの初期化
void UninitAbiBoost(void);			// ブーストゲージの終了
void UpdateAbiBoost(void);			// ブーストゲージの更新
void DrawAbiBoost(void);			// ブーストゲージの描画

void InitAbiWind(void);				// ウィンドゲージの初期化
void UninitAbiWind(void);			// ウィンドゲージの終了
void UpdateAbiWind(void);			// ウィンドゲージの更新
void DrawAbiWind(void);				// ウィンドゲージの描画

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureAbility[TEXTURE_ABI_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAbiBoost = NULL;					// ブーストゲージの頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAbiWind  = NULL;					// ウィンドゲージの頂点バッファへのポインタ

//======================================================================================================================
//	能力バーの初期化処理
//======================================================================================================================
void InitAbility(void)
{
	// ブーストゲージの初期化
	InitAbiBoost();

	// ウィンドゲージの初期化
	InitAbiWind();
}

//======================================================================================================================
//	能力バーの終了処理
//======================================================================================================================
void UninitAbility(void)
{
	// ブーストゲージの終了
	UninitAbiBoost();
	
	// ウィンドゲージの終了
	UninitAbiWind();
}

//======================================================================================================================
//	能力バーの更新処理
//======================================================================================================================
void UpdateAbility(void)
{
	// ブーストゲージの更新
	UpdateAbiBoost();

	// ウィンドゲージの更新
	UpdateAbiWind();
}

//======================================================================================================================
//	能力バーの描画処理
//======================================================================================================================
void DrawAbility(void)
{
	// ブーストゲージの描画
	DrawAbiBoost();

	// ウィンドゲージの描画
	DrawAbiWind();
}

//======================================================================================================================
//	ブーストゲージの初期化処理
//======================================================================================================================
void InitAbiBoost(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_2D *pVtx;							// 頂点情報へのポインタ

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_2D) * 4 * MAX_ABI_BOOST,	// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffAbiBoost,
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAbiBoost->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------------------------------------------------------------------------------------------
	//	能力バー (赤)
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[0].pos = D3DXVECTOR3(ABI_BOOST_POS_X,                                          ABI_BOOST_POS_Y - ABI_BOOST_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(ABI_BOOST_POS_X + (BOOST_WAIT_CNT * ABI_BOOST_WIDTH_MUL), ABI_BOOST_POS_Y - ABI_BOOST_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(ABI_BOOST_POS_X,                                          ABI_BOOST_POS_Y + ABI_BOOST_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(ABI_BOOST_POS_X + (BOOST_WAIT_CNT * ABI_BOOST_WIDTH_MUL), ABI_BOOST_POS_Y + ABI_BOOST_HEIGHT, 0.0f);

	// rhw の設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 50, 50, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 50, 50, 255);
	pVtx[2].col = D3DCOLOR_RGBA(205,  0,  0, 255);
	pVtx[3].col = D3DCOLOR_RGBA(205,  0,  0, 255);

	//------------------------------------------------------------------------------------------------------------------
	//	能力バー (黄)
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[4].pos = D3DXVECTOR3(ABI_BOOST_POS_X,                                          ABI_BOOST_POS_Y - ABI_BOOST_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(ABI_BOOST_POS_X + (BOOST_WAIT_CNT * ABI_BOOST_WIDTH_MUL), ABI_BOOST_POS_Y - ABI_BOOST_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(ABI_BOOST_POS_X,                                          ABI_BOOST_POS_Y + ABI_BOOST_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(ABI_BOOST_POS_X + (BOOST_WAIT_CNT * ABI_BOOST_WIDTH_MUL), ABI_BOOST_POS_Y + ABI_BOOST_HEIGHT, 0.0f);

	// rhw の設定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[4].col = D3DCOLOR_RGBA(255, 250, 50, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 250, 50, 255);
	pVtx[6].col = D3DCOLOR_RGBA(205, 160,  0, 255);
	pVtx[7].col = D3DCOLOR_RGBA(205, 160,  0, 255);

	// 頂点バッファをアンロックする
	g_pVtxBuffAbiBoost->Unlock();
}

//======================================================================================================================
//	ブーストゲージの終了処理
//======================================================================================================================
void UninitAbiBoost(void)
{
	// 頂点バッファの破棄
	if (g_pVtxBuffAbiBoost != NULL)
	{ // 変数 (g_pVtxBuffAbiBoost) がNULLではない場合

		g_pVtxBuffAbiBoost->Release();
		g_pVtxBuffAbiBoost = NULL;
	}
}

//======================================================================================================================
//	ブーストゲージの更新処理
//======================================================================================================================
void UpdateAbiBoost(void)
{
	// 変数を宣言
	float fCurrentBoost = 0.0f;			// 現在のブーストの待機、使用時間

	// ポインタを宣言
	VERTEX_2D *pVtx;					// 頂点情報へのポインタ
	Player    *pPlayer = GetPlayer();	// プレイヤーの情報

	// 現在のブーストの待機、使用時間を求める
	if (pPlayer->boost.state == BOOSTSTATE_NONE
	||  pPlayer->boost.state == BOOSTSTATE_WAIT)
	{ // ブーストが何もしない状態、または待機状態の場合

		// ブーストの待機時間を計算
		fCurrentBoost = (float)(BOOST_WAIT_CNT - pPlayer->boost.nCounter);
	}
	else if (pPlayer->boost.state == BOOSTSTATE_UP)
	{ // ブーストが加速状態の場合

		// ブーストの使用時間を計算
		fCurrentBoost = (float)pPlayer->boost.nCounter;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAbiBoost->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標を設定
	pVtx[4].pos = D3DXVECTOR3(ABI_BOOST_POS_X,                                         ABI_BOOST_POS_Y - ABI_BOOST_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(ABI_BOOST_POS_X + (fCurrentBoost * ABI_BOOST_WIDTH_MUL), ABI_BOOST_POS_Y - ABI_BOOST_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(ABI_BOOST_POS_X,                                         ABI_BOOST_POS_Y + ABI_BOOST_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(ABI_BOOST_POS_X + (fCurrentBoost * ABI_BOOST_WIDTH_MUL), ABI_BOOST_POS_Y + ABI_BOOST_HEIGHT, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffAbiBoost->Unlock();
}

//======================================================================================================================
//	ブーストゲージの描画処理
//======================================================================================================================
void DrawAbiBoost(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffAbiBoost, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntAbility = 0; nCntAbility < MAX_ABI_BOOST; nCntAbility++)
	{ // 使用するポリゴン数分繰り返す

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAbility * 4, 2);
	}
}

//======================================================================================================================
//	ウィンドゲージの初期化処理
//======================================================================================================================
void InitAbiWind(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_2D *pVtx;							// 頂点情報へのポインタ

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_2D) * 4 * MAX_ABI_WIND,	// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffAbiWind,
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAbiWind->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------------------------------------------------------------------------------------------
	//	能力バー (赤)
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[0].pos = D3DXVECTOR3(ABI_WIND_POS_X,                                         ABI_WIND_POS_Y - ABI_WIND_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(ABI_WIND_POS_X + (BOOST_WAIT_CNT * ABI_WIND_WIDTH_MUL), ABI_WIND_POS_Y - ABI_WIND_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(ABI_WIND_POS_X,                                         ABI_WIND_POS_Y + ABI_WIND_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(ABI_WIND_POS_X + (BOOST_WAIT_CNT * ABI_WIND_WIDTH_MUL), ABI_WIND_POS_Y + ABI_WIND_HEIGHT, 0.0f);

	// rhw の設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 50, 50, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 50, 50, 255);
	pVtx[2].col = D3DCOLOR_RGBA(205,  0,  0, 255);
	pVtx[3].col = D3DCOLOR_RGBA(205,  0,  0, 255);

	//------------------------------------------------------------------------------------------------------------------
	//	能力バー (黄)
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[4].pos = D3DXVECTOR3(ABI_WIND_POS_X,                                         ABI_WIND_POS_Y - ABI_WIND_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(ABI_WIND_POS_X + (BOOST_WAIT_CNT * ABI_WIND_WIDTH_MUL), ABI_WIND_POS_Y - ABI_WIND_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(ABI_WIND_POS_X,                                         ABI_WIND_POS_Y + ABI_WIND_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(ABI_WIND_POS_X + (BOOST_WAIT_CNT * ABI_WIND_WIDTH_MUL), ABI_WIND_POS_Y + ABI_WIND_HEIGHT, 0.0f);

	// rhw の設定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[4].col = D3DCOLOR_RGBA(255, 250, 50, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 250, 50, 255);
	pVtx[6].col = D3DCOLOR_RGBA(205, 160,  0, 255);
	pVtx[7].col = D3DCOLOR_RGBA(205, 160,  0, 255);

	// 頂点バッファをアンロックする
	g_pVtxBuffAbiWind->Unlock();
}

//======================================================================================================================
//	ウィンドゲージの終了処理
//======================================================================================================================
void UninitAbiWind(void)
{
	// 頂点バッファの破棄
	if (g_pVtxBuffAbiWind != NULL)
	{ // 変数 (g_pVtxBuffAbiWind) がNULLではない場合

		g_pVtxBuffAbiWind->Release();
		g_pVtxBuffAbiWind = NULL;
	}
}

//======================================================================================================================
//	ウィンドゲージの更新処理
//======================================================================================================================
void UpdateAbiWind(void)
{
	// 変数を宣言
	float fCurrentWind = 0.0f;			// 現在のウィンドの待機、使用時間

	// ポインタを宣言
	VERTEX_2D *pVtx;					// 頂点情報へのポインタ
	WindInfo  *pWind = GetWindInfo();	// ウィンドの情報

	// 現在のウィンドの待機、使用時間を求める
	fCurrentWind = (float)(WIND_OVERHEAT_CNT - pWind->nUseCounter);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAbiWind->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標を設定
	pVtx[4].pos = D3DXVECTOR3(ABI_WIND_POS_X,                                       ABI_WIND_POS_Y - ABI_WIND_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(ABI_WIND_POS_X + (fCurrentWind * ABI_WIND_WIDTH_MUL), ABI_WIND_POS_Y - ABI_WIND_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(ABI_WIND_POS_X,                                       ABI_WIND_POS_Y + ABI_WIND_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(ABI_WIND_POS_X + (fCurrentWind * ABI_WIND_WIDTH_MUL), ABI_WIND_POS_Y + ABI_WIND_HEIGHT, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffAbiWind->Unlock();
}

//======================================================================================================================
//	ウィンドゲージの描画処理
//======================================================================================================================
void DrawAbiWind(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffAbiWind, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntAbility = 0; nCntAbility < MAX_ABI_WIND; nCntAbility++)
	{ // 使用するポリゴン数分繰り返す

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAbility * 4, 2);
	}
}