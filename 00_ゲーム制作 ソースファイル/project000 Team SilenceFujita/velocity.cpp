//======================================================================================================================
//
//	速度バー処理 [velocity.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "velocity.h"
#include "player.h"
#include "value.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_VELO		(3)			// 使用するポリゴン数

#define VAL_VELO_WIDTH	(28.0f)		// スコアの数値の横幅 / 2
#define VAL_VELO_HEIGHT	(28.0f)		// スコアの数値の縦幅 / 2
#define VAL_VELO_SPACE	(38.0f)		// スコアの数値間の幅 (x)
#define VAL_VELO_DIGIT	(3)			// スコアの数値間の幅 (x)

#define VELO_POS_VAL_X	(190.0f)	// スコア (値) の絶対座標 (x)
#define VELO_POS_VAL_Y	(610.0f)	// スコアの絶対座標 (y)

#define VELO_POS_X		(72.0f)		// 速度バーの絶対座標 (x)
#define VELO_POS_Y		(652.5f)	// 速度バーの絶対座標 (y)
#define VELO_WIDTH_MUL	(3.9f)		// 速度バーの横幅のプレイヤー速度乗算量
#define VELO_HEIGHT		(15.0f)		// 速度バーの縦幅 / 2
#define VELO_PULS_Y		(66.0f)		// 速度バーの縦幅の追加量

#define VELO_BG_POS_X	(190.0f)	// 速度バーの背景の絶対座標 (x)
#define VELO_BG_POS_Y	(620.0f)	// 速度バーの背景の絶対座標 (x)
#define VELO_BG_WIDTH	(155.0f)	// 速度バーの背景の横幅 / 2
#define VELO_BG_HEIGHT	(70.0f)		// 速度バーの背景の縦幅 / 2

#define MAX_SPEED		(240)		// 表示上の最高速度

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTextureVelocity[] =	// テクスチャの相対パス
{
	"data\\TEXTURE\\ui003.png",		// ライフ背景のテクスチャの相対パス
	NULL,							// NULL
	NULL,							// NULL
};

//**********************************************************************************************************************
//	列挙型定義 (TEXTURE_VELO)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_VELO_NORMAL = 0,		// ライフの背景
	TEXTURE_VELO_NULL_00,			// NULL
	TEXTURE_VELO_NULL_01,			// NULL
	TEXTURE_VELO_MAX,				// この列挙型の総数
} TEXTURE_VELO;

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureVelocity[TEXTURE_VELO_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffVelocity = NULL;					// 頂点バッファへのポインタ

//======================================================================================================================
//	速度バーの初期化処理
//======================================================================================================================
void InitVelocity(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_2D *pVtx;							// 頂点情報へのポインタ

	for (int nCntVelocity = 0; nCntVelocity < TEXTURE_VELO_MAX; nCntVelocity++)
	{ // 使用するテクスチャ数分繰り返す

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, apTextureVelocity[nCntVelocity], &g_apTextureVelocity[nCntVelocity]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_2D) * 4 * MAX_VELO,		// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffVelocity,
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffVelocity->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------------------------------------------------------------------------------------------
	//	速度バー (背景)
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[0].pos = D3DXVECTOR3(VELO_BG_POS_X - VELO_BG_WIDTH, VELO_BG_POS_Y - VELO_BG_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(VELO_BG_POS_X + VELO_BG_WIDTH, VELO_BG_POS_Y - VELO_BG_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(VELO_BG_POS_X - VELO_BG_WIDTH, VELO_BG_POS_Y + VELO_BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(VELO_BG_POS_X + VELO_BG_WIDTH, VELO_BG_POS_Y + VELO_BG_HEIGHT, 0.0f);

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
	//	速度バー (赤)
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[4].pos = D3DXVECTOR3(VELO_POS_X, VELO_POS_Y - VELO_HEIGHT, 0.0f);

	pVtx[5].pos.x = VELO_POS_X + (MAX_REAL_SPEED * VELO_WIDTH_MUL);
	pVtx[5].pos.y = (VELO_POS_Y - VELO_HEIGHT) - VELO_PULS_Y;
	pVtx[5].pos.z = 0.0f;

	pVtx[6].pos = D3DXVECTOR3(VELO_POS_X,                                     VELO_POS_Y + VELO_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(VELO_POS_X + (MAX_REAL_SPEED * VELO_WIDTH_MUL), VELO_POS_Y + VELO_HEIGHT, 0.0f);

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
	//	速度バー (黄)
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[8].pos = D3DXVECTOR3(VELO_POS_X, VELO_POS_Y - VELO_HEIGHT, 0.0f);

	pVtx[9].pos.x = VELO_POS_X;
	pVtx[9].pos.y = VELO_POS_Y - VELO_HEIGHT;
	pVtx[9].pos.z = 0.0f;

	pVtx[10].pos = D3DXVECTOR3(VELO_POS_X, VELO_POS_Y + VELO_HEIGHT, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(VELO_POS_X, VELO_POS_Y + VELO_HEIGHT, 0.0f);

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
	g_pVtxBuffVelocity->Unlock();
}

//======================================================================================================================
//	速度バーの終了処理
//======================================================================================================================
void UninitVelocity(void)
{
	// テクスチャの破棄
	for (int nCntVelocity = 0; nCntVelocity < TEXTURE_VELO_MAX; nCntVelocity++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTextureVelocity[nCntVelocity] != NULL)
		{ // 変数 (g_apTextureVelocity) がNULLではない場合

			g_apTextureVelocity[nCntVelocity]->Release();
			g_apTextureVelocity[nCntVelocity] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffVelocity != NULL)
	{ // 変数 (g_pVtxBuffVelocity) がNULLではない場合

		g_pVtxBuffVelocity->Release();
		g_pVtxBuffVelocity = NULL;
	}
}

//======================================================================================================================
//	速度バーの更新処理
//======================================================================================================================
void UpdateVelocity(void)
{
	// 変数を宣言
	float fCurrentSpeed;	// 現在のプレイヤー速度の絶対値

	// ポインタを宣言
	VERTEX_2D *pVtx;					// 頂点情報へのポインタ
	Player    *pPlayer = GetPlayer();	// プレイヤーの情報

	// 現在のプレイヤー速度の絶対値を求める
	fCurrentSpeed = fabsf(pPlayer->move.x + pPlayer->boost.plusMove.x);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffVelocity->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標を設定
	pVtx[8].pos = D3DXVECTOR3(VELO_POS_X, VELO_POS_Y - VELO_HEIGHT, 0.0f);

	pVtx[9].pos.x = VELO_POS_X + (fCurrentSpeed * VELO_WIDTH_MUL);
	pVtx[9].pos.y = (VELO_POS_Y - VELO_HEIGHT) - (fCurrentSpeed * (VELO_PULS_Y / MAX_REAL_SPEED));
	pVtx[9].pos.z = 0.0f;

	pVtx[10].pos = D3DXVECTOR3(VELO_POS_X,                                    VELO_POS_Y + VELO_HEIGHT, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(VELO_POS_X + (fCurrentSpeed * VELO_WIDTH_MUL), VELO_POS_Y + VELO_HEIGHT, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffVelocity->Unlock();
}

//======================================================================================================================
//	速度バーの描画処理
//======================================================================================================================
void DrawVelocity(void)
{
	// 変数を宣言
	int nCurrentSpeed;		// 現在のスピード

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	Player           *pPlayer = GetPlayer();	// プレイヤーの情報

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffVelocity, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntVelocity = 0; nCntVelocity < MAX_VELO; nCntVelocity++)
	{ // 使用するポリゴン数分繰り返す

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureVelocity[nCntVelocity]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntVelocity * 4, 2);
	}

	//------------------------------------------------------------------------------------------------------------------
	//	数値の描画
	//------------------------------------------------------------------------------------------------------------------
	// 現在のスピードを求める
	nCurrentSpeed = (int)(fabsf(pPlayer->move.x + pPlayer->boost.plusMove.x) * (MAX_SPEED / MAX_REAL_SPEED));

	// 数値の設定
	SetValue
	( // 引数
		D3DXVECTOR3
		( // 引数
			VELO_POS_VAL_X,	// 位置 (x)
			VELO_POS_VAL_Y,	// 位置 (y)
			0.0f			// 位置 (z)
		),
		nCurrentSpeed,		// 値
		MAX_SPEED,			// 値の最大値
		VAL_VELO_WIDTH,		// 横幅
		VAL_VELO_HEIGHT,	// 縦幅
		VAL_VELO_SPACE,		// 数値間の幅
		1.0f				// α値
	);

	// 数値の描画
	DrawValue(VAL_VELO_DIGIT, VALUETYPE_NORMAL);
}