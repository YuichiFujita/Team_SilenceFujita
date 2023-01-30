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

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_ABI			(3)			// 使用するポリゴン数

#define ABI_POS_X		(60.0f)		// 能力バーの絶対座標 (x)
#define ABI_POS_Y		(135.0f)	// 能力バーの絶対座標 (y)
#define ABI_WIDTH_MUL	(1.4f)		// 能力バーの横幅のプレイヤー能力乗算量
#define ABI_HEIGHT		(15.0f)		// 能力バーの縦幅 / 2

#define ABI_BG_POS_X	(190.0f)	// 能力バーの背景の絶対座標 (x)
#define ABI_BG_POS_Y	(620.0f)	// 能力バーの背景の絶対座標 (x)
#define ABI_BG_WIDTH	(165.0f)	// 能力バーの背景の横幅 / 2
#define ABI_BG_HEIGHT	(80.0f)		// 能力バーの背景の縦幅 / 2

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTextureAbility[] =	// テクスチャの相対パス
{
	"data\\TEXTURE\\ui000.png",		// ライフ背景のテクスチャの相対パス
	NULL,							// NULL
	NULL,							// NULL
};

//**********************************************************************************************************************
//	列挙型定義 (TEXTURE_ABI)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_ABI_NORMAL = 0,			// ライフの背景
	TEXTURE_ABI_NULL_00,			// NULL
	TEXTURE_ABI_NULL_01,			// NULL
	TEXTURE_ABI_MAX,				// この列挙型の総数
} TEXTURE_ABI;

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureAbility[TEXTURE_ABI_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAbility = NULL;					// 頂点バッファへのポインタ

//======================================================================================================================
//	能力バーの初期化処理
//======================================================================================================================
void InitAbility(void)
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
		sizeof(VERTEX_2D) * 4 * MAX_ABI,		// 必要頂点数
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
	//	能力バー (背景)
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
	//	能力バー (赤)
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[4].pos = D3DXVECTOR3(ABI_POS_X,                                    ABI_POS_Y - ABI_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(ABI_POS_X + (BOOST_WAIT_CNT * ABI_WIDTH_MUL), ABI_POS_Y - ABI_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(ABI_POS_X,                                    ABI_POS_Y + ABI_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(ABI_POS_X + (BOOST_WAIT_CNT * ABI_WIDTH_MUL), ABI_POS_Y + ABI_HEIGHT, 0.0f);

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
	//	能力バー (黄)
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[8].pos  = D3DXVECTOR3(ABI_POS_X,                                    ABI_POS_Y - ABI_HEIGHT, 0.0f);
	pVtx[9].pos  = D3DXVECTOR3(ABI_POS_X + (BOOST_WAIT_CNT * ABI_WIDTH_MUL), ABI_POS_Y - ABI_HEIGHT, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(ABI_POS_X,                                    ABI_POS_Y + ABI_HEIGHT, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(ABI_POS_X + (BOOST_WAIT_CNT * ABI_WIDTH_MUL), ABI_POS_Y + ABI_HEIGHT, 0.0f);

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
	g_pVtxBuffAbility->Unlock();
}

//======================================================================================================================
//	能力バーの終了処理
//======================================================================================================================
void UninitAbility(void)
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
//	能力バーの更新処理
//======================================================================================================================
void UpdateAbility(void)
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
	g_pVtxBuffAbility->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標を設定
	pVtx[8].pos  = D3DXVECTOR3(ABI_POS_X,                                   ABI_POS_Y - ABI_HEIGHT, 0.0f);
	pVtx[9].pos  = D3DXVECTOR3(ABI_POS_X + (fCurrentBoost * ABI_WIDTH_MUL), ABI_POS_Y - ABI_HEIGHT, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(ABI_POS_X,                                   ABI_POS_Y + ABI_HEIGHT, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(ABI_POS_X + (fCurrentBoost * ABI_WIDTH_MUL), ABI_POS_Y + ABI_HEIGHT, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffAbility->Unlock();
}

//======================================================================================================================
//	能力バーの描画処理
//======================================================================================================================
void DrawAbility(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	Player           *pPlayer = GetPlayer();	// プレイヤーの情報

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffAbility, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntAbility = 0; nCntAbility < MAX_ABI; nCntAbility++)
	{ // 使用するポリゴン数分繰り返す

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureAbility[nCntAbility]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAbility * 4, 2);
	}
}