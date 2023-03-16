//========================================
//
//褒めのメイン処理[Praise.cpp]
//Author 小原立暉
//
//========================================
#include "main.h"
#include "praise.h"

#include "tutorial.h"

//===========================================
//マクロ定義
//===========================================
#define MAX_PRAISE			(30)									// 褒めの最大数

#define PRAISE_INIT_POS		(D3DXVECTOR3(	0.0f,	0.0f, 0.0f))	// 褒めの初期位置
#define PRAISE_POS			(D3DXVECTOR3(1100.0f, 500.0f, 0.0f))	// 褒めの位置
#define PRAISE_SIZE			(D3DXVECTOR3( 162.0f,  30.0f, 0.0f))	// 褒めのサイズ
#define PRAISE_LAPSE_CNT	(90)									// 褒めを表示しておくカウント

//===========================================
//列挙型定義(PRAISE_TEXTURE)
//===========================================
typedef enum
{
	PRAISE_TEXTURE_NICE = 0,			// ナイスコンボテクスチャ
	PRAISE_TEXTURE_GREAT,				// グレートコンボテクスチャ
	PRAISE_TEXTURE_GOOD,				// グッドコンボテクスチャ
	PRAISE_TEXTURE_MAX					// この列挙型の総数
}PRAISE_TEXTURE;

//===========================================
//プロトタイプ宣言
//===========================================

//===========================================
//グローバル変数宣言
//===========================================
LPDIRECT3DTEXTURE9      g_apTexturePraise[PRAISE_TEXTURE_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPraise = NULL;	// 頂点バッファへのポインタ

Praise g_aPraise[MAX_PRAISE];						// 褒めの情報

// コンスト定義
const char *c_apPraiseTextureName[PRAISE_TEXTURE_MAX] = 
{
	"data/TEXTURE/praise000.png",		// ナイスコンボテクスチャ
	"data/TEXTURE/praise001.png",		// グレートコンボテクスチャ
	"data/TEXTURE/praise002.png",		// グッドコンボテクスチャ
};

//===========================================
//褒めの初期化処理
//===========================================
void InitPraise(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_2D *pVtx;							// 頂点情報へのポインタ

	for (int nCntPra = 0; nCntPra < MAX_PRAISE; nCntPra++)
	{
		// 情報の初期化
		g_aPraise[nCntPra].pos	= PRAISE_INIT_POS;					// 位置
		g_aPraise[nCntPra].rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
		g_aPraise[nCntPra].nCount = 0;								// 消滅カウント
		g_aPraise[nCntPra].nNumTex = 0;								// 
		g_aPraise[nCntPra].fAngle = atan2f((PRAISE_SIZE.x * 2), (PRAISE_SIZE.y * 2));		// 方向
		g_aPraise[nCntPra].fLength = sqrtf((PRAISE_SIZE.x * 2) * (PRAISE_SIZE.x * 2) + (PRAISE_SIZE.y * 2) * (PRAISE_SIZE.y * 2)) * 0.5f;		// 長さ
		g_aPraise[nCntPra].bUse = false;							// 使用状況
	}

	for (int nCntTex = 0; nCntTex < PRAISE_TEXTURE_MAX; nCntTex++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, c_apPraiseTextureName[nCntTex], &g_apTexturePraise[nCntTex]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_2D) * 4 * MAX_PRAISE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPraise,
		NULL
	);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPraise->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPra = 0; nCntPra < MAX_PRAISE; nCntPra++)
	{
		// 頂点座標の設定
		pVtx[0].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + (D3DX_PI + g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
		pVtx[0].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + (D3DX_PI + g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + (D3DX_PI + -g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
		pVtx[1].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + (D3DX_PI + -g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + -g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
		pVtx[2].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + -g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
		pVtx[3].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
		pVtx[3].pos.z = 0.0f;

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;			// 頂点データを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPraise->Unlock();
}

//===========================================
//褒めの終了処理
//===========================================
void UninitPraise(void)
{
	for (int nCntPra = 0; nCntPra < PRAISE_TEXTURE_MAX; nCntPra++)
	{
		//テクスチャの破棄
		if (g_apTexturePraise[nCntPra] != NULL)
		{
			g_apTexturePraise[nCntPra]->Release();
			g_apTexturePraise[nCntPra] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPraise != NULL)
	{
		g_pVtxBuffPraise->Release();
		g_pVtxBuffPraise = NULL;
	}
}

//===========================================
//褒めの更新処理
//===========================================
void UpdatePraise(void)
{
	VERTEX_2D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPraise->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPra = 0; nCntPra < MAX_PRAISE; nCntPra++)
	{
		if (g_aPraise[nCntPra].bUse == true)
		{//使用していた場合

			// 消滅カウンターを加算する
			g_aPraise[nCntPra].nCount++;

			if (g_aPraise[nCntPra].nCount % PRAISE_LAPSE_CNT == 0)
			{ // カウンターが一定数以上になったら

				// カウンターを初期化する
				g_aPraise[nCntPra].nCount = 0;

				// 使用しない
				g_aPraise[nCntPra].bUse = false;
			}

			// 頂点座標の設定
			pVtx[0].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + (D3DX_PI + g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
			pVtx[0].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + (D3DX_PI + g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + (D3DX_PI + -g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
			pVtx[1].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + (D3DX_PI + -g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + -g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
			pVtx[2].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + -g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
			pVtx[3].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
			pVtx[3].pos.z = 0.0f;
		}

		pVtx += 4;		// 頂点データを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPraise->Unlock();
}

//===========================================
//褒めの描画処理
//===========================================
void DrawPraise(void)
{
	// 変数を宣言
	D3DXVECTOR3 pos;	// 桁ごとの倍率位置の設定用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffPraise,									//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D));									//頂点情報構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPra = 0; nCntPra < MAX_PRAISE; nCntPra++)
	{
		if (g_aPraise[nCntPra].bUse == true)
		{//使用されている場合

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTexturePraise[g_aPraise[nCntPra].nNumTex]);

			// ポリゴンの描画
			pDevice->DrawPrimitive
			( // 引数
				D3DPT_TRIANGLESTRIP,	// プリミティブの種類
				nCntPra * 4,			// 描画する最初の頂点インデックス
				2						// 描画するプリミティブ数
			);
		}
	}
}

//===========================================
// 褒め言葉の設定処理
//===========================================
void SetPraise(void)
{
	VERTEX_2D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPraise->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPra = 0; nCntPra < MAX_PRAISE; nCntPra++)
	{
		if (g_aPraise[nCntPra].bUse == false)
		{//使用していなかった場合

			// 情報の設定
			g_aPraise[nCntPra].pos = PRAISE_POS;		// 位置
			g_aPraise[nCntPra].rot = D3DXVECTOR3		// 向き
			(
				0.0f,
				0.0f,
				D3DXToRadian((rand() % 35 - 17))
			);
			g_aPraise[nCntPra].nNumTex = rand() % PRAISE_TEXTURE_MAX;	// テクスチャの番号
			g_aPraise[nCntPra].nCount = 0;								// 消滅カウント
			g_aPraise[nCntPra].fAngle = atan2f((PRAISE_SIZE.x * 2), (PRAISE_SIZE.y * 2));		// 方向
			g_aPraise[nCntPra].fLength = sqrtf((PRAISE_SIZE.x * 2) * (PRAISE_SIZE.x * 2) + (PRAISE_SIZE.y * 2) * (PRAISE_SIZE.y * 2)) * 0.5f;		// 長さ

			// 頂点座標の設定
			pVtx[0].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + (D3DX_PI + g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
			pVtx[0].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + (D3DX_PI + g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + (D3DX_PI + -g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
			pVtx[1].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + (D3DX_PI + -g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + -g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
			pVtx[2].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + -g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
			pVtx[3].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
			pVtx[3].pos.z = 0.0f;

			g_aPraise[nCntPra].bUse = true;							// 使用する

			break;			// 抜け出す
		}

		pVtx += 4;		// 頂点データを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPraise->Unlock();
}