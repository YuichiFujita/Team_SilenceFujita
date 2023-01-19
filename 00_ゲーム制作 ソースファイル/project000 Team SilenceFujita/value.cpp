//======================================================================================================================
//
//	数値処理 [value.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "value.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_VALUE		(9)			// 使用するポリゴン数 (桁の最大数)

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTextureValue[] =		// テクスチャの相対パス
{
	"data\\TEXTURE\\number000.png",	// 数字 (通常) のテクスチャの相対パス
};

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureValue[VALUETYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffValue = NULL;					// 頂点バッファへのポインタ

//======================================================================================================================
//	数値の初期化処理
//======================================================================================================================
void InitValue(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_2D         *pVtx;					// 頂点情報へのポインタ

	for (int nCntValue = 0; nCntValue < VALUETYPE_MAX; nCntValue++)
	{ // 使用するテクスチャ数分繰り返す

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, apTextureValue[nCntValue], &g_apTextureValue[nCntValue]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_2D) * 4 * MAX_VALUE,		// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffValue,
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffValue->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntValue = 0; nCntValue < MAX_VALUE; nCntValue++)
	{ // 桁の最大数分繰り返す

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
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffValue->Unlock();
}

//======================================================================================================================
//	数値の終了処理
//======================================================================================================================
void UninitValue(void)
{
	// テクスチャの破棄
	for (int nCntValue = 0; nCntValue < VALUETYPE_MAX; nCntValue++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTextureValue[nCntValue] != NULL)
		{ // 変数 (g_apTextureValue) がNULLではない場合

			g_apTextureValue[nCntValue]->Release();
			g_apTextureValue[nCntValue] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffValue != NULL)
	{ // 変数 (g_pVtxBuffValue) がNULLではない場合

		g_pVtxBuffValue->Release();
		g_pVtxBuffValue = NULL;
	}
}

//======================================================================================================================
//	数値の更新処理
//======================================================================================================================
void UpdateValue(void)
{

}

//======================================================================================================================
//	数値の描画処理
//======================================================================================================================
void DrawValue(int nDigit, int nType)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ (ポインタの宣言だが * はいらない)

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffValue, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntValue = 0; nCntValue < nDigit; nCntValue++)
	{ // 引数の桁数分繰り返す

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureValue[nType]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntValue * 4, 2);
	}
}

//======================================================================================================================
//	数値の設定処理
//======================================================================================================================
void SetValue(D3DXVECTOR3 pos, int nValue, int nMaxValue, float fWidth, float fHeight, float fSpace, float fAlpha)
{
	// 変数配列を宣言
	int aTexU[MAX_VALUE];	// 各桁の数字を格納

	// ポインタを宣言
	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 値の補正
	if (nValue > nMaxValue)
	{ // 値が最大値を超えた場合

		// 値に最大値を代入
		nValue = nMaxValue;
	}
	else if (nValue < 0)
	{ // 値が最小値を下回った場合

		// 値に最小値を代入
		nValue = 0;
	}

	for (int nCntValue = 0; nCntValue < MAX_VALUE; nCntValue++)
	{ // 桁の最大数分繰り返す

		// 配列 (aTexU) の要素に引数の値を一桁ずつ代入
		//aTexU[4] = nValue % 100000 / 10000;		// 5桁目
		//aTexU[3] = nValue % 10000 / 1000;		// 4桁目
		//aTexU[2] = nValue % 1000 / 100;			// 3桁目
		//aTexU[1] = nValue % 100 / 10;			// 2桁目

		aTexU[nCntValue] = nValue % 10 / 1;				// 1桁目


		aTexU[nCntValue] = nValue % (10 + (nCntValue * 10)) / 1;				// 1桁目
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffValue->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntValue = 0; nCntValue < MAX_VALUE; nCntValue++)
	{ // 桁の最大数分繰り返す

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3((pos.x - fWidth) - (nCntValue * fSpace), pos.y - fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3((pos.x + fWidth) - (nCntValue * fSpace), pos.y - fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3((pos.x - fWidth) - (nCntValue * fSpace), pos.y + fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((pos.x + fWidth) - (nCntValue * fSpace), pos.y + fHeight, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntValue] * 0.1f,        0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntValue] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntValue] * 0.1f,        1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntValue] * 0.1f + 0.1f, 1.0f);

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffValue->Unlock();
}