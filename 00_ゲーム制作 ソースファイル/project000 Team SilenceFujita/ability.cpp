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
#define MAX_ABILITY			(3)			// 能力表示に使用するポリゴン数

#define ABI_BG_POS_X		(245.0f)	// 体力バーの背景の絶対座標 (x)
#define ABI_BG_POS_Y		(145.0f)	// 体力バーの背景の絶対座標 (y)
#define ABI_BG_WIDTH		(240.0f)	// 体力バーの背景の横幅 / 2
#define ABI_BG_HEIGHT		(46.0f)		// 体力バーの背景の縦幅 / 2

#define ABI_INIT_BG			(0)			// 背景時の初期値
#define ABI_LOOP_BG			(2)			// 背景時の繰り返し数
#define ABI_INIT_FRAME		(2)			// 枠時の初期値
#define ABI_LOOP_FRAME		(3)			// 枠時の繰り返し数

//----------------------------------------------------------------------------------------------------------------------
//	破滅疾走 (スラム・ブースト) マクロ定義
//----------------------------------------------------------------------------------------------------------------------
#define MAX_ABI_BOOST		(2)			// ブーストバーに使用するポリゴン数

#define ABI_BOOST_POS_X		(65.0f)		// 能力バーの絶対座標 (x)
#define ABI_BOOST_POS_Y		(138.0f)	// 能力バーの絶対座標 (y)
#define ABI_BOOST_WIDTH_MUL	(0.66f)		// 能力バーの横幅のプレイヤー能力乗算量
#define ABI_BOOST_HEIGHT	(35.0f)		// 能力バーの縦幅 / 2

//----------------------------------------------------------------------------------------------------------------------
//	吹飛散風 (フライ・アウェイ) マクロ定義
//----------------------------------------------------------------------------------------------------------------------
#define MAX_ABI_WIND		(2)			// ウィンドバーに使用するポリゴン数

#define ABI_WIND_POS_X		(185.0f)	// 能力バーの絶対座標 (x)
#define ABI_WIND_POS_Y		(138.0f)	// 能力バーの絶対座標 (y)
#define ABI_WIND_WIDTH_MUL	(0.655f)	// 能力バーの横幅のプレイヤー能力乗算量
#define ABI_WIND_HEIGHT		(35.0f)		// 能力バーの縦幅 / 2

//----------------------------------------------------------------------------------------------------------------------
//	無音世界 (サイレンス・ワールド) マクロ定義
//----------------------------------------------------------------------------------------------------------------------
#define MAX_ABI_BOMB		(2)			// ボムバーに使用するポリゴン数

#define ABI_BOMB_POS_X		(305.0f)	// 能力バーの絶対座標 (x)
#define ABI_BOMB_POS_Y		(138.0f)	// 能力バーの絶対座標 (y)
#define ABI_BOMB_WIDTH_MUL	(0.062f)	// 能力バーの横幅のプレイヤー能力乗算量
#define ABI_BOMB_HEIGHT		(35.0f)		// 能力バーの縦幅 / 2

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTextureAbility[] =	// テクスチャの相対パス
{
	"data\\TEXTURE\\alpha001.png",	// 能力表示の透過型のテクスチャの相対パス
	"data\\TEXTURE\\ui001.png",		// 能力表示の背景のテクスチャの相対パス
	"data\\TEXTURE\\frame001.png",	// 能力表示の型のテクスチャの相対パス
};

//**********************************************************************************************************************
//	列挙型定義 (TEXTURE_ABI)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_ABI_ALPHA = 0,			// 能力表示の透過型
	TEXTURE_ABI_BG,					// 能力表示の背景
	TEXTURE_ABI_FRAME,				// 能力表示の型
	TEXTURE_ABI_MAX,				// この列挙型の総数
} TEXTURE_ABI;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void InitAbi(void);					// 能力表示の初期化
void UninitAbi(void);				// 能力表示の終了
void UpdateAbi(void);				// 能力表示の更新
void DrawAbi(bool bBG);				// 能力表示の描画

void InitAbiBoost(void);			// ブーストゲージの初期化
void UninitAbiBoost(void);			// ブーストゲージの終了
void UpdateAbiBoost(void);			// ブーストゲージの更新
void DrawAbiBoost(void);			// ブーストゲージの描画

void InitAbiWind(void);				// ウィンドゲージの初期化
void UninitAbiWind(void);			// ウィンドゲージの終了
void UpdateAbiWind(void);			// ウィンドゲージの更新
void DrawAbiWind(void);				// ウィンドゲージの描画

void InitAbiBomb(void);				// ボムゲージの初期化
void UninitAbiBomb(void);			// ボムゲージの終了
void UpdateAbiBomb(void);			// ボムゲージの更新
void DrawAbiBomb(void);				// ボムゲージの描画

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureAbility[TEXTURE_ABI_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAbility  = NULL;					// 能力表示の頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAbiBoost = NULL;					// ブーストゲージの頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAbiWind  = NULL;					// ウィンドゲージの頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAbiBomb  = NULL;					// ボムゲージの頂点バッファへのポインタ

//======================================================================================================================
//	能力バーの初期化処理
//======================================================================================================================
void InitAbility(void)
{
	// 能力表示の初期化
	InitAbi();

	// ブーストゲージの初期化
	InitAbiBoost();

	// ウィンドゲージの初期化
	InitAbiWind();

	// ボムゲージの初期化
	InitAbiBomb();
}

//======================================================================================================================
//	能力バーの終了処理
//======================================================================================================================
void UninitAbility(void)
{
	// 能力表示の終了
	UninitAbi();

	// ブーストゲージの終了
	UninitAbiBoost();
	
	// ウィンドゲージの終了
	UninitAbiWind();

	// ボムゲージの終了
	UninitAbiBomb();
}

//======================================================================================================================
//	能力バーの更新処理
//======================================================================================================================
void UpdateAbility(void)
{
	// 能力表示の更新
	UpdateAbi();

	// ブーストゲージの更新
	UpdateAbiBoost();

	// ウィンドゲージの更新
	UpdateAbiWind();

	// ボムゲージの更新
	UpdateAbiBomb();
}

//======================================================================================================================
//	能力バーの描画処理
//======================================================================================================================
void DrawAbility(void)
{
	// 能力表示の描画 (背景)
	DrawAbi(true);

	// ブーストゲージの描画
	DrawAbiBoost();

	// ウィンドゲージの描画
	DrawAbiWind();

	// ボムゲージの描画
	DrawAbiBomb();

	// 能力表示の描画 (枠)
	DrawAbi(false);
}

//======================================================================================================================
//	能力表示の初期化処理
//======================================================================================================================
void InitAbi(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_2D *pVtx;							// 頂点情報へのポインタ

	for (int nCntAbility = 0; nCntAbility < TEXTURE_ABI_MAX; nCntAbility++)
	{ // 使用するテクスチャ数分繰り返す

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, apTextureAbility[nCntAbility], &g_apTextureAbility[nCntAbility]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_2D) * 4 * MAX_ABILITY,	// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffAbility,
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAbility->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------------------------------------------------------------------------------------------
	//	透過型
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[0].pos = D3DXVECTOR3(ABI_BG_POS_X - ABI_BG_WIDTH, ABI_BG_POS_Y - ABI_BG_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(ABI_BG_POS_X + ABI_BG_WIDTH, ABI_BG_POS_Y - ABI_BG_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(ABI_BG_POS_X - ABI_BG_WIDTH, ABI_BG_POS_Y + ABI_BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(ABI_BG_POS_X + ABI_BG_WIDTH, ABI_BG_POS_Y + ABI_BG_HEIGHT, 0.0f);

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
	//	背景
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[4].pos = D3DXVECTOR3(ABI_BG_POS_X - ABI_BG_WIDTH, ABI_BG_POS_Y - ABI_BG_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(ABI_BG_POS_X + ABI_BG_WIDTH, ABI_BG_POS_Y - ABI_BG_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(ABI_BG_POS_X - ABI_BG_WIDTH, ABI_BG_POS_Y + ABI_BG_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(ABI_BG_POS_X + ABI_BG_WIDTH, ABI_BG_POS_Y + ABI_BG_HEIGHT, 0.0f);

	// rhw の設定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//------------------------------------------------------------------------------------------------------------------
	//	型
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[8].pos  = D3DXVECTOR3(ABI_BG_POS_X - ABI_BG_WIDTH, ABI_BG_POS_Y - ABI_BG_HEIGHT, 0.0f);
	pVtx[9].pos  = D3DXVECTOR3(ABI_BG_POS_X + ABI_BG_WIDTH, ABI_BG_POS_Y - ABI_BG_HEIGHT, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(ABI_BG_POS_X - ABI_BG_WIDTH, ABI_BG_POS_Y + ABI_BG_HEIGHT, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(ABI_BG_POS_X + ABI_BG_WIDTH, ABI_BG_POS_Y + ABI_BG_HEIGHT, 0.0f);

	// rhw の設定
	pVtx[8].rhw  = 1.0f;
	pVtx[9].rhw  = 1.0f;
	pVtx[10].rhw = 1.0f;
	pVtx[11].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[8].col  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[9].col  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[8].tex  = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex  = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffAbility->Unlock();
}

//======================================================================================================================
//	能力表示の終了処理
//======================================================================================================================
void UninitAbi(void)
{
	// テクスチャの破棄
	for (int nCntAbility = 0; nCntAbility < TEXTURE_ABI_MAX; nCntAbility++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTextureAbility[nCntAbility] != NULL)
		{ // 変数 (g_apTextureAbility) がNULLではない場合

			g_apTextureAbility[nCntAbility]->Release();
			g_apTextureAbility[nCntAbility] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffAbility != NULL)
	{ // 変数 (g_pVtxBuffAbility) がNULLではない場合

		g_pVtxBuffAbility->Release();
		g_pVtxBuffAbility = NULL;
	}
}

//======================================================================================================================
//	能力表示の更新処理
//======================================================================================================================
void UpdateAbi(void)
{

}

//======================================================================================================================
//	能力表示の描画処理
//======================================================================================================================
void DrawAbi(bool bBG)
{
	// 変数を宣言
	int nInit, nLoop;	// 繰り返し用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffAbility, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (bBG)
	{ // 背景の場合

		// 繰り返しの設定
		nInit = ABI_INIT_BG;	// 初期値
		nLoop = ABI_LOOP_BG;	// 繰り返し数
	}
	else
	{ // 枠の場合

		// 繰り返しの設定
		nInit = ABI_INIT_FRAME;	// 初期値
		nLoop = ABI_LOOP_FRAME;	// 繰り返し数
	}

	for (int nCntAbility = nInit; nCntAbility < nLoop; nCntAbility++)
	{ // 使用するポリゴン数分繰り返す

		switch (nCntAbility)
		{ // 描画するポリゴンごとの処理
		case TEXTURE_ABI_ALPHA:

			// αブレンディングを加算合成に設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			// αテストを有効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);	// αテストの有効 / 無効の設定
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_LESS);	// αテストの設定
			pDevice->SetRenderState(D3DRS_ALPHAREF, 255);		 	// αテストの参照値設定

			// 処理を抜ける
			break;

		case TEXTURE_ABI_BG:

			// Zテストを無効にする
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL);		// Zテストの設定
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Zバッファ更新の有効 / 無効の設定

			// 処理を抜ける
			break;

		case TEXTURE_ABI_FRAME:

			// 無し

			// 処理を抜ける
			break;
		}

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureAbility[nCntAbility]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAbility * 4, 2);

		// αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// αテストを無効にする
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストの有効 / 無効の設定
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// αテストの設定
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// αテストの参照値設定

		// Zテストを有効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Zテストの設定
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Zバッファ更新の有効 / 無効の設定
	}
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
	//	能力バー (オレンジ)
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
	pVtx[4].col = D3DCOLOR_RGBA(255, 160, 0, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 160, 0, 255);
	pVtx[6].col = D3DCOLOR_RGBA(205, 110, 0, 255);
	pVtx[7].col = D3DCOLOR_RGBA(205, 110, 0, 255);

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

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Zバッファ更新の有効 / 無効の設定

	for (int nCntAbility = 0; nCntAbility < MAX_ABI_BOOST; nCntAbility++)
	{ // 使用するポリゴン数分繰り返す

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAbility * 4, 2);
	}

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Zバッファ更新の有効 / 無効の設定
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
	pVtx[0].pos = D3DXVECTOR3(ABI_WIND_POS_X,                                            ABI_WIND_POS_Y - ABI_WIND_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(ABI_WIND_POS_X + (WIND_OVERHEAT_CNT * ABI_WIND_WIDTH_MUL), ABI_WIND_POS_Y - ABI_WIND_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(ABI_WIND_POS_X,                                            ABI_WIND_POS_Y + ABI_WIND_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(ABI_WIND_POS_X + (WIND_OVERHEAT_CNT * ABI_WIND_WIDTH_MUL), ABI_WIND_POS_Y + ABI_WIND_HEIGHT, 0.0f);

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
	//	能力バー (緑)
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[4].pos = D3DXVECTOR3(ABI_WIND_POS_X,                                            ABI_WIND_POS_Y - ABI_WIND_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(ABI_WIND_POS_X + (WIND_OVERHEAT_CNT * ABI_WIND_WIDTH_MUL), ABI_WIND_POS_Y - ABI_WIND_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(ABI_WIND_POS_X,                                            ABI_WIND_POS_Y + ABI_WIND_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(ABI_WIND_POS_X + (WIND_OVERHEAT_CNT * ABI_WIND_WIDTH_MUL), ABI_WIND_POS_Y + ABI_WIND_HEIGHT, 0.0f);

	// rhw の設定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[4].col = D3DCOLOR_RGBA(120, 255, 110, 255);
	pVtx[5].col = D3DCOLOR_RGBA(120, 255, 110, 255);
	pVtx[6].col = D3DCOLOR_RGBA( 70, 205,  60, 255);
	pVtx[7].col = D3DCOLOR_RGBA( 70, 205,  60, 255);

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

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Zバッファ更新の有効 / 無効の設定

	for (int nCntAbility = 0; nCntAbility < MAX_ABI_WIND; nCntAbility++)
	{ // 使用するポリゴン数分繰り返す

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAbility * 4, 2);
	}

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Zバッファ更新の有効 / 無効の設定
}

//======================================================================================================================
//	ボムゲージの初期化処理
//======================================================================================================================
void InitAbiBomb(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_2D *pVtx;							// 頂点情報へのポインタ

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_2D) * 4 * MAX_ABI_BOMB,	// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffAbiBomb,
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAbiBomb->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------------------------------------------------------------------------------------------
	//	能力バー (赤)
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[0].pos = D3DXVECTOR3(ABI_BOMB_POS_X,                                        ABI_BOMB_POS_Y - ABI_BOMB_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(ABI_BOMB_POS_X + (BOMB_WAIT_CNT * ABI_BOMB_WIDTH_MUL), ABI_BOMB_POS_Y - ABI_BOMB_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(ABI_BOMB_POS_X,                                        ABI_BOMB_POS_Y + ABI_BOMB_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(ABI_BOMB_POS_X + (BOMB_WAIT_CNT * ABI_BOMB_WIDTH_MUL), ABI_BOMB_POS_Y + ABI_BOMB_HEIGHT, 0.0f);

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
	//	能力バー (青)
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[4].pos = D3DXVECTOR3(ABI_BOMB_POS_X,                                         ABI_BOMB_POS_Y - ABI_BOMB_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(ABI_BOMB_POS_X + (BOOST_WAIT_CNT * ABI_BOMB_WIDTH_MUL), ABI_BOMB_POS_Y - ABI_BOMB_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(ABI_BOMB_POS_X,                                         ABI_BOMB_POS_Y + ABI_BOMB_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(ABI_BOMB_POS_X + (BOOST_WAIT_CNT * ABI_BOMB_WIDTH_MUL), ABI_BOMB_POS_Y + ABI_BOMB_HEIGHT, 0.0f);

	// rhw の設定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[4].col = D3DCOLOR_RGBA(110, 210, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(110, 210, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA( 60, 160, 205, 255);
	pVtx[7].col = D3DCOLOR_RGBA( 60, 160, 205, 255);

	// 頂点バッファをアンロックする
	g_pVtxBuffAbiBomb->Unlock();
}

//======================================================================================================================
//	ボムゲージの終了処理
//======================================================================================================================
void UninitAbiBomb(void)
{
	// 頂点バッファの破棄
	if (g_pVtxBuffAbiBomb != NULL)
	{ // 変数 (g_pVtxBuffAbiBomb) がNULLではない場合

		g_pVtxBuffAbiBomb->Release();
		g_pVtxBuffAbiBomb = NULL;
	}
}

//======================================================================================================================
//	ボムゲージの更新処理
//======================================================================================================================
void UpdateAbiBomb(void)
{
	// 変数を宣言
	float fCurrentBomb = 0.0f;			// 現在のボムの待機、使用時間

	// ポインタを宣言
	VERTEX_2D *pVtx;					// 頂点情報へのポインタ
	Player    *pPlayer = GetPlayer();	// プレイヤーの情報

	// 現在のボムの待機、使用時間を求める
	//if (pPlayer->boost.state == BOOSTSTATE_UP)
	{ // ボムが加速状態の場合

		// ボムの使用時間を計算
		fCurrentBomb = (float)pPlayer->bomb.nCounterState;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAbiBomb->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標を設定
	pVtx[4].pos = D3DXVECTOR3(ABI_BOMB_POS_X,                                       ABI_BOMB_POS_Y - ABI_BOMB_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(ABI_BOMB_POS_X + (fCurrentBomb * ABI_BOMB_WIDTH_MUL), ABI_BOMB_POS_Y - ABI_BOMB_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(ABI_BOMB_POS_X,                                       ABI_BOMB_POS_Y + ABI_BOMB_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(ABI_BOMB_POS_X + (fCurrentBomb * ABI_BOMB_WIDTH_MUL), ABI_BOMB_POS_Y + ABI_BOMB_HEIGHT, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffAbiBomb->Unlock();
}

//======================================================================================================================
//	ボムゲージの描画処理
//======================================================================================================================
void DrawAbiBomb(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffAbiBomb, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Zバッファ更新の有効 / 無効の設定

	for (int nCntAbility = 0; nCntAbility < MAX_ABI_BOMB; nCntAbility++)
	{ // 使用するポリゴン数分繰り返す

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAbility * 4, 2);
	}

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Zバッファ更新の有効 / 無効の設定
}