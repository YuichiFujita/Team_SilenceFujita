//======================================================================================================================
//
//	3D数値処理 [value.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "3DValue.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_3DVALUE		(9)			// 使用するポリゴン数 (桁の最大数)

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTexture3DValue[] =		// テクスチャの相対パス
{
	"data\\TEXTURE\\number000.png",	// 数字 (通常) のテクスチャの相対パス
	"data\\TEXTURE\\number001.png",	// 数字 (赤) のテクスチャの相対パス
};

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTexture3DValue[VALUE3DTYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuff3DValue = NULL;					// 頂点バッファへのポインタ
D3DXMATRIX g_aMtx[MAX_3DVALUE];										// マトリックス

//======================================================================================================================
//	3D数値の初期化処理
//======================================================================================================================
void Init3DValue(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_3D         *pVtx;					// 頂点情報へのポインタ

	for (int nCnt3DValue = 0; nCnt3DValue < VALUE3DTYPE_MAX; nCnt3DValue++)
	{ // 使用するテクスチャ数分繰り返す

	  // テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, apTexture3DValue[nCnt3DValue], &g_apTexture3DValue[nCnt3DValue]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * 4 * MAX_3DVALUE,		// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuff3DValue,
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuff3DValue->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt3DValue = 0; nCnt3DValue < MAX_3DVALUE; nCnt3DValue++)
	{ // 桁の最大数分繰り返す

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// rhw の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
	g_pVtxBuff3DValue->Unlock();
}

//======================================================================================================================
//	3D数値の終了処理
//======================================================================================================================
void Uninit3DValue(void)
{
	// テクスチャの破棄
	for (int nCnt3DValue = 0; nCnt3DValue < VALUE3DTYPE_MAX; nCnt3DValue++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTexture3DValue[nCnt3DValue] != NULL)
		{ // 変数 (g_apTexture3DValue) がNULLではない場合

			g_apTexture3DValue[nCnt3DValue]->Release();
			g_apTexture3DValue[nCnt3DValue] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuff3DValue != NULL)
	{ // 変数 (g_pVtxBuff3DValue) がNULLではない場合

		g_pVtxBuff3DValue->Release();
		g_pVtxBuff3DValue = NULL;
	}
}

//======================================================================================================================
//	3D数値の更新処理
//======================================================================================================================
void Update3DValue(void)
{

}

//======================================================================================================================
//	3D数値の描画処理
//======================================================================================================================
void Draw3DValue(D3DXVECTOR3 pos, int nDigit, int nType)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	D3DXMATRIX mtxTrans;			//計算用マトリックス
	D3DXMATRIX mtxView;				//ビューマトリックス取得用

	//ライティングをOFFにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);					//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						//Zテストの有効/無効設定

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);					//アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);				//アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 75);							//アルファテストの参照値設定

	for (int nCntDraw = 0; nCntDraw < nDigit; nCntDraw++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aMtx[nCntDraw]);

		//ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//ポリゴンをカメラに向けて正面に向ける
		D3DXMatrixInverse(&g_aMtx[nCntDraw], NULL, &mtxView);

		//逆行列を求める
		g_aMtx[nCntDraw]._41 = 0.0f;
		g_aMtx[nCntDraw]._42 = 0.0f;
		g_aMtx[nCntDraw]._43 = 0.0f;

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&g_aMtx[nCntDraw], &g_aMtx[nCntDraw], &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aMtx[nCntDraw]);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0,
			g_pVtxBuff3DValue,									//頂点バッファへのポインタ
			0,
			sizeof(VERTEX_3D));									//頂点情報構造体のサイズ

		//テクスチャの設定
		pDevice->SetTexture(0, g_apTexture3DValue[nType]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//プリミティブの種類
			4 * nCntDraw,										//描画する最初の頂点インデックス
			2);													//描画するプリミティブ数
	}

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			//アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		//アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						//アルファテストの参照値設定

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				//Zテストの有効/無効設定

	//ライティングをONにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//======================================================================================================================
//	3D数値の設定処理
//======================================================================================================================
void Set3DValue(int n3DValue, int nMax3DValue, float fWidth, float fHeight, float fAlpha)
{
	// 変数を宣言
	int nDigit = 1;			// テクスチャ座標計算用

	// 変数配列を宣言
	int aTexU[MAX_3DVALUE];	// 各桁の数字を格納

	// ポインタを宣言
	VERTEX_3D *pVtx;		// 頂点情報へのポインタ

	// 値の補正
	if (n3DValue > nMax3DValue)
	{ // 値が最大値を超えた場合

		// 値に最大値を代入
		n3DValue = nMax3DValue;
	}
	else if (n3DValue < 0)
	{ // 値が最小値を下回った場合

		// 値に最小値を代入
		n3DValue = 0;
	}

	for (int nCntDigit = 0; nCntDigit < MAX_3DVALUE; nCntDigit++)
	{ // 桁の最大数分繰り返す

		// 計算用割り出し
		nDigit *= 10;
	}

	for (int nCntTex = MAX_3DVALUE - 1; nCntTex >= 0; nCntTex--)
	{ // 桁数の最大から桁数の最小まで繰り返す

		// テクスチャ座標を設定
		aTexU[nCntTex] = (n3DValue % nDigit) / (nDigit / 10);

		// 計算用割り出し
		nDigit /= 10;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuff3DValue->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt3DValue = 0; nCnt3DValue < MAX_3DVALUE; nCnt3DValue++)
	{ // 桁の最大数分繰り返す

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-fWidth, +fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+fWidth, +fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+fWidth, -fHeight, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCnt3DValue] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCnt3DValue] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCnt3DValue] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCnt3DValue] * 0.1f + 0.1f, 1.0f);

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuff3DValue->Unlock();
}