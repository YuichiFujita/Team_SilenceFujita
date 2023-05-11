//======================================================================================================================
//
//	2D2Dエフェクト処理 [2Deffect.cpp]
//	Author：小原立暉
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "2Deffect.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_EFFECT		(4096)		// 使用するポリゴン数 (2Dエフェクトの最大数)

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTexture2DEffect[] =		// テクスチャの相対パス
{
	"data\\TEXTURE\\effect000.jpg",	// 2Dエフェクトのテクスチャの相対パス
};

//**********************************************************************************************************************
//	列挙型定義 (TEXTURE_EFFECT)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_2DEFFECT_NORMAL = 0,		// 2Dエフェクト (通常)
	TEXTURE_2DEFFECT_MAX,				// この列挙型の総数
} TEXTURE_2DEFFECT;

//**********************************************************************************************************************
//	構造体定義 (2DEffect)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 move;				// 移動量
	D3DXCOLOR   col;				// 色
	int         nLife;				// 寿命
	float       fRadius;			// 半径
	float       fSub;				// 減算量 (半径)
	float       fAlpha;				// 1Fごとの薄くなる量
	bool        bUse;				// 使用状況
}Effect2D;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTexture2DEffect[TEXTURE_2DEFFECT_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuff2DEffect = NULL;						// 頂点バッファへのポインタ

Effect2D g_a2DEffect[MAX_EFFECT];		// 2Dエフェクトの情報

//======================================================================================================================
//	2Dエフェクトの初期化処理
//======================================================================================================================
void Init2DEffect(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_2D *pVtx;							// 頂点情報へのポインタ

	for (int nCnt2DEffect = 0; nCnt2DEffect < TEXTURE_2DEFFECT_MAX; nCnt2DEffect++)
	{ // 使用するテクスチャ数分繰り返す

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, apTexture2DEffect[nCnt2DEffect], &g_apTexture2DEffect[nCnt2DEffect]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_2D) * 4 * MAX_EFFECT,		// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuff2DEffect,
		NULL
	);

	// 2Dエフェクトの情報の初期化
	for (int nCnt2DEffect = 0; nCnt2DEffect < MAX_EFFECT; nCnt2DEffect++)
	{ // 2Dエフェクトの最大表示数分繰り返す

		g_a2DEffect[nCnt2DEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_a2DEffect[nCnt2DEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
		g_a2DEffect[nCnt2DEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 色
		g_a2DEffect[nCnt2DEffect].nLife = 0;								// 寿命
		g_a2DEffect[nCnt2DEffect].fRadius = 0.0f;							// 半径
		g_a2DEffect[nCnt2DEffect].fSub = 0.0f;								// 減算量 (半径)
		g_a2DEffect[nCnt2DEffect].fAlpha = 0.0f;							// 1Fごとの薄くなる量
		g_a2DEffect[nCnt2DEffect].bUse = false;								// 使用状況
	}

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuff2DEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt2DEffect = 0; nCnt2DEffect < MAX_EFFECT; nCnt2DEffect++)
	{ // 2Dエフェクトの最大表示数分繰り返す

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// rhw の設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuff2DEffect->Unlock();
}

//======================================================================================================================
//	2Dエフェクトの終了処理
//======================================================================================================================
void Uninit2DEffect(void)
{
	// テクスチャの破棄
	for (int nCnt2DEffect = 0; nCnt2DEffect < TEXTURE_2DEFFECT_MAX; nCnt2DEffect++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTexture2DEffect[nCnt2DEffect] != NULL)
		{ // 変数 (g_apTexture2DEffect) がNULLではない場合

			g_apTexture2DEffect[nCnt2DEffect]->Release();
			g_apTexture2DEffect[nCnt2DEffect] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuff2DEffect != NULL)
	{ // 変数 (g_pVtxBuff2DEffect) がNULLではない場合

		g_pVtxBuff2DEffect->Release();
		g_pVtxBuff2DEffect = NULL;
	}
}

//======================================================================================================================
//	2Dエフェクトの更新処理
//======================================================================================================================
void Update2DEffect(void)
{
	// ポインタを宣言
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuff2DEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt2DEffect = 0; nCnt2DEffect < MAX_EFFECT; nCnt2DEffect++)
	{ // 2Dエフェクトの最大表示数分繰り返す

		if (g_a2DEffect[nCnt2DEffect].bUse == true)
		{ // 2Dエフェクトが使用されている場合

			// 移動の更新
			g_a2DEffect[nCnt2DEffect].pos += g_a2DEffect[nCnt2DEffect].move;

			// 構造体の要素を減算する
			g_a2DEffect[nCnt2DEffect].nLife--;									// 寿命を減算
			g_a2DEffect[nCnt2DEffect].fRadius -= g_a2DEffect[nCnt2DEffect].fSub;	// 半径を減算
			g_a2DEffect[nCnt2DEffect].col.a -= g_a2DEffect[nCnt2DEffect].fAlpha;	// α値を減算

																			// 半径の補正
			if (g_a2DEffect[nCnt2DEffect].fRadius < 0.0f)
			{ // 半径が 0.0f より小さい場合

			  // 半径に 0.0f を代入
				g_a2DEffect[nCnt2DEffect].fRadius = 0.0f;
			}

			// 透明度の補正
			if (g_a2DEffect[nCnt2DEffect].col.a < 0.0f)
			{ // α値が 0.0f より小さい場合

			  // α値に 0.0f を代入
				g_a2DEffect[nCnt2DEffect].col.a = 0.0f;
			}

			if (g_a2DEffect[nCnt2DEffect].nLife <= 0)
			{ // 寿命を迎えた場合

			  // 使用していない状態にする
				g_a2DEffect[nCnt2DEffect].bUse = false;
			}

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_a2DEffect[nCnt2DEffect].pos.x - g_a2DEffect[nCnt2DEffect].fRadius, g_a2DEffect[nCnt2DEffect].pos.y - g_a2DEffect[nCnt2DEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_a2DEffect[nCnt2DEffect].pos.x + g_a2DEffect[nCnt2DEffect].fRadius, g_a2DEffect[nCnt2DEffect].pos.y - g_a2DEffect[nCnt2DEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_a2DEffect[nCnt2DEffect].pos.x - g_a2DEffect[nCnt2DEffect].fRadius, g_a2DEffect[nCnt2DEffect].pos.y + g_a2DEffect[nCnt2DEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_a2DEffect[nCnt2DEffect].pos.x + g_a2DEffect[nCnt2DEffect].fRadius, g_a2DEffect[nCnt2DEffect].pos.y + g_a2DEffect[nCnt2DEffect].fRadius, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = g_a2DEffect[nCnt2DEffect].col;
			pVtx[1].col = g_a2DEffect[nCnt2DEffect].col;
			pVtx[2].col = g_a2DEffect[nCnt2DEffect].col;
			pVtx[3].col = g_a2DEffect[nCnt2DEffect].col;
		}

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuff2DEffect->Unlock();
}

//======================================================================================================================
//	2Dエフェクトの描画処理
//======================================================================================================================
void Draw2DEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuff2DEffect,										//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D));										//頂点情報構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//αブレンディングを加算処理に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_a2DEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTexture2DEffect[TEXTURE_2DEFFECT_NORMAL]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
				4 * nCntEffect,									//描画する最初の頂点インデックス
				2);												//描画するプリミティブ数
		}
	}

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//======================================================================================================================
//	2Dエフェクトの設定処理
//======================================================================================================================
void Set2DEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fRadius, float fSub)
{
	// ポインタを宣言
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuff2DEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt2DEffect = 0; nCnt2DEffect < MAX_EFFECT; nCnt2DEffect++)
	{ // 2Dエフェクトの最大表示数分繰り返す

		if (g_a2DEffect[nCnt2DEffect].bUse == false)
		{ // 2Dエフェクトが使用されていない場合

			// 引数を代入
			g_a2DEffect[nCnt2DEffect].pos = pos;			// 位置
			g_a2DEffect[nCnt2DEffect].move = move;			// 移動量
			g_a2DEffect[nCnt2DEffect].col = col;			// 色
			g_a2DEffect[nCnt2DEffect].nLife = nLife;		// 寿命
			g_a2DEffect[nCnt2DEffect].fRadius = fRadius;	// 半径
			g_a2DEffect[nCnt2DEffect].fSub = fSub;			// 減算量 (半径)

			// 1F ごとの薄くなる量を設定
			g_a2DEffect[nCnt2DEffect].fAlpha = 1.0f / nLife;

			// 使用している状態にする
			g_a2DEffect[nCnt2DEffect].bUse = true;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_a2DEffect[nCnt2DEffect].pos.x - g_a2DEffect[nCnt2DEffect].fRadius, g_a2DEffect[nCnt2DEffect].pos.y - g_a2DEffect[nCnt2DEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_a2DEffect[nCnt2DEffect].pos.x + g_a2DEffect[nCnt2DEffect].fRadius, g_a2DEffect[nCnt2DEffect].pos.y - g_a2DEffect[nCnt2DEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_a2DEffect[nCnt2DEffect].pos.x - g_a2DEffect[nCnt2DEffect].fRadius, g_a2DEffect[nCnt2DEffect].pos.y + g_a2DEffect[nCnt2DEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_a2DEffect[nCnt2DEffect].pos.x + g_a2DEffect[nCnt2DEffect].fRadius, g_a2DEffect[nCnt2DEffect].pos.y + g_a2DEffect[nCnt2DEffect].fRadius, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = g_a2DEffect[nCnt2DEffect].col;
			pVtx[1].col = g_a2DEffect[nCnt2DEffect].col;
			pVtx[2].col = g_a2DEffect[nCnt2DEffect].col;
			pVtx[3].col = g_a2DEffect[nCnt2DEffect].col;

			// 処理を抜ける
			break;
		}

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuff2DEffect->Unlock();
}

#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================
//**********************************************************************************************************************
//	2Dエフェクトの総数取得処理
//**********************************************************************************************************************
int GetNum2DEffect(void)
{
	// 変数を宣言
	int nNum2DEffect = 0;	// 2Dエフェクトの総数の確認用

	for (int nCnt2DEffect = 0; nCnt2DEffect < MAX_EFFECT; nCnt2DEffect++)
	{ // 2Dエフェクトの最大表示数分繰り返す

		if (g_a2DEffect[nCnt2DEffect].bUse == true)
		{ // 2Dエフェクトが使用されている場合

		  // カウンターを加算
			nNum2DEffect++;
		}
	}

	// 変数の値を返す
	return nNum2DEffect;	// 2Dエフェクトの総数
}
#endif