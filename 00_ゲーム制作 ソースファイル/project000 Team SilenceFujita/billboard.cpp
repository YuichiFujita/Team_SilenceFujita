//======================================================================================================================
//
//	ビルボード処理 [billboard.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "billboard.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_BILLBOARD		(4)		// 使用するポリゴン数 (ビルボードの最大数)

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTextureBillboard[] =	// テクスチャの相対パス
{
	"data\\TEXTURE\\billboard000.png",	// ビルボードのテクスチャの相対パス
};

//**********************************************************************************************************************
//	列挙型定義 (TEXTURE_BILLBOARD)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_BILLBOARD_NORMAL = 0,	// ビルボード (通常)
	TEXTURE_BILLBOARD_MAX,			// この列挙型の総数
} TEXTURE_BILLBOARD;

//**********************************************************************************************************************
//	構造体定義 (Billboard)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	D3DXMATRIX  mtxWorld;			// ワールドマトリックス
	bool        bUse;				// 使用状況
} Billboard;

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureBillboard[TEXTURE_BILLBOARD_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;							// 頂点バッファへのポインタ

Billboard g_aBillboard[MAX_BILLBOARD];			// ビルボードの情報

//======================================================================================================================
//	ビルボードの初期化処理
//======================================================================================================================
void InitBillboard(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ

	// テクスチャの読み込み
	for (int nCntBillboard = 0; nCntBillboard < TEXTURE_BILLBOARD_MAX; nCntBillboard++)
	{ // 使用するテクスチャ数分繰り返す

		D3DXCreateTextureFromFile(pDevice, apTextureBillboard[nCntBillboard], &g_apTextureBillboard[nCntBillboard]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,	// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL
	);

	// ビルボードの情報の初期化
	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{ // ビルボードの最大表示数分繰り返す

		g_aBillboard[nCntBillboard].pos  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_aBillboard[nCntBillboard].bUse = false;								// 使用状況
	}

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{ // ビルボードの最大表示数分繰り返す

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-25.0f,  25.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3( 25.0f,  25.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-25.0f, -25.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3( 25.0f, -25.0f, 0.0f);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();
}

//======================================================================================================================
//	ビルボードの終了処理
//======================================================================================================================
void UninitBillboard(void)
{
	// テクスチャの破棄
	for (int nCntBillboard = 0; nCntBillboard < TEXTURE_BILLBOARD_MAX; nCntBillboard++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTextureBillboard[nCntBillboard] != NULL)
		{ // 変数 (g_apTextureBillboard) がNULLではない場合

			g_apTextureBillboard[nCntBillboard]->Release();
			g_apTextureBillboard[nCntBillboard] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBillboard != NULL)
	{ // 変数 (g_pVtxBuffBillboard) がNULLではない場合

		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//======================================================================================================================
//	ビルボードの更新処理
//======================================================================================================================
void UpdateBillboard(void)
{

}

//======================================================================================================================
//	ビルボードの描画処理
//======================================================================================================================
void DrawBillboard(void)
{
	// 変数を宣言
	D3DXMATRIX mtxTrans;						// 計算用マトリックス
	D3DXMATRIX mtxView;							// ビューマトリックス取得用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Zバッファ更新の有効 / 無効の設定

	// αテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// αテストの有効 / 無効の設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 160);				// αテストの参照値設定

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{ // ビルボードの最大表示数分繰り返す

		if (g_aBillboard[nCntBillboard].bUse == true)
		{ // ビルボードが使用されている場合

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBillboard[nCntBillboard].mtxWorld);

			// ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aBillboard[nCntBillboard].mtxWorld, NULL, &mtxView);	// 逆行列を求める
			g_aBillboard[nCntBillboard].mtxWorld._41 = 0.0f;
			g_aBillboard[nCntBillboard].mtxWorld._42 = 0.0f;
			g_aBillboard[nCntBillboard].mtxWorld._43 = 0.0f;

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBillboard[nCntBillboard].pos.x, g_aBillboard[nCntBillboard].pos.y, g_aBillboard[nCntBillboard].pos.z);
			D3DXMatrixMultiply(&g_aBillboard[nCntBillboard].mtxWorld, &g_aBillboard[nCntBillboard].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCntBillboard].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureBillboard[TEXTURE_BILLBOARD_NORMAL]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBillboard * 4, 2);
		}
	}

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Zバッファ更新の有効 / 無効の設定

	// αテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストの有効 / 無効の設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// αテストの参照値設定
}

//======================================================================================================================
//	ビルボードの設定処理
//======================================================================================================================
void SetBillboard(D3DXVECTOR3 pos)
{
	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{ // ビルボードの最大表示数分繰り返す

		if (g_aBillboard[nCntBillboard].bUse == false)
		{ // ビルボードが使用されていない場合

			// 引数の位置を代入
			g_aBillboard[nCntBillboard].pos = pos;

			// 使用している状態にする
			g_aBillboard[nCntBillboard].bUse = true;

			// 処理を抜ける
			break;
		}
	}
}