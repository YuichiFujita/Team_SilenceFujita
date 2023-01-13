//======================================================================================================================
//
//	エディットエフェクト処理 [EditEffect.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "EditEffect.h"

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTextureEditEffect[] =		// テクスチャの相対パス
{
	"data\\TEXTURE\\effect000.jpg",		// エフェクトのテクスチャの相対パス
};

//**********************************************************************************************************************
//	テクスチャの定義 (EditEffect)
//**********************************************************************************************************************
typedef enum
{
	EDITEFFECTTEX_NORMAL = 0,		//普通のエフェクト
	EDITEFFECTTEX_MAX
}EditEffectTex;

//**********************************************************************************************************************
//	構造体定義 (EditEffect)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 move;				// 移動量
	D3DXMATRIX  mtxWorld;			// ワールドマトリックス
	D3DXCOLOR   col;				// 色
	int         nLife;				// 寿命
	D3DXVECTOR3 fRadius;			// 半径
	bool        bUse;				// 使用状況
}EditEffect;

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureEditEffect[EDITEFFECTTEX_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEditEffect = NULL;					// 頂点バッファへのポインタ

EditEffect g_aEditEffect[MAX_EDITEFFECT];		// エフェクトの情報

//======================================================================================================================
//	エフェクトの初期化処理
//======================================================================================================================
void InitEditEffect(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ

	for (int nCntEditEffect = 0; nCntEditEffect < EDITEFFECTTEX_MAX; nCntEditEffect++)
	{ // 使用するテクスチャ数分繰り返す

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, apTextureEditEffect[nCntEditEffect], &g_apTextureEditEffect[nCntEditEffect]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * 4 * MAX_EDITEFFECT,	// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffEditEffect,
		NULL
	);

	// エフェクトの情報の初期化
	for (int nCntEditEffect = 0; nCntEditEffect < MAX_EDITEFFECT; nCntEditEffect++)
	{ // エフェクトの最大表示数分繰り返す

		g_aEditEffect[nCntEditEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_aEditEffect[nCntEditEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
		g_aEditEffect[nCntEditEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 色
		g_aEditEffect[nCntEditEffect].nLife = 0;								// 寿命
		g_aEditEffect[nCntEditEffect].fRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 半径
		g_aEditEffect[nCntEditEffect].bUse = false;								// 使用状況
	}

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEditEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEditEffect = 0; nCntEditEffect < MAX_EDITEFFECT; nCntEditEffect++)
	{ // エフェクトの最大表示数分繰り返す

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
	g_pVtxBuffEditEffect->Unlock();
}

//======================================================================================================================
//	エフェクトの終了処理
//======================================================================================================================
void UninitEditEffect(void)
{
	// テクスチャの破棄
	for (int nCntEditEffect = 0; nCntEditEffect < EDITEFFECTTEX_MAX; nCntEditEffect++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTextureEditEffect[nCntEditEffect] != NULL)
		{ // 変数 (g_apTextureEditEffect) がNULLではない場合

			g_apTextureEditEffect[nCntEditEffect]->Release();
			g_apTextureEditEffect[nCntEditEffect] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffEditEffect != NULL)
	{ // 変数 (g_pVtxBuffEditEffect) がNULLではない場合

		g_pVtxBuffEditEffect->Release();
		g_pVtxBuffEditEffect = NULL;
	}
}

//======================================================================================================================
//	エフェクトの更新処理
//======================================================================================================================
void UpdateEditEffect(void)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEditEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEditEffect = 0; nCntEditEffect < MAX_EDITEFFECT; nCntEditEffect++)
	{ // エフェクトの最大表示数分繰り返す

		if (g_aEditEffect[nCntEditEffect].bUse == true)
		{ // エフェクトが使用されている場合
			//移動量を下に向けていく
			g_aEditEffect[nCntEditEffect].move.y -= 1.0f;

			// 移動の更新
			g_aEditEffect[nCntEditEffect].pos += g_aEditEffect[nCntEditEffect].move;

			// 構造体の要素を減算する
			g_aEditEffect[nCntEditEffect].nLife--;									// 寿命を減算

			// 半径の補正
			if (g_aEditEffect[nCntEditEffect].fRadius.x < 0.0f)
			{ // 半径が 0.0f より小さい場合
				// 半径に 0.0f を代入
				g_aEditEffect[nCntEditEffect].fRadius.x = 0.0f;
			}

			// 半径の補正
			if (g_aEditEffect[nCntEditEffect].fRadius.y < 0.0f)
			{ // 半径が 0.0f より小さい場合
				// 半径に 0.0f を代入
				g_aEditEffect[nCntEditEffect].fRadius.y = 0.0f;
			}

			// 透明度の補正
			if (g_aEditEffect[nCntEditEffect].col.a < 0.0f)
			{ // α値が 0.0f より小さい場合
				// α値に 0.0f を代入
				g_aEditEffect[nCntEditEffect].col.a = 0.0f;
			}

			if (g_aEditEffect[nCntEditEffect].nLife <= 0)
			{ // 寿命を迎えた場合
				// 使用していない状態にする
				g_aEditEffect[nCntEditEffect].bUse = false;
			}

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEditEffect[nCntEditEffect].fRadius.x, +g_aEditEffect[nCntEditEffect].fRadius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aEditEffect[nCntEditEffect].fRadius.x, +g_aEditEffect[nCntEditEffect].fRadius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEditEffect[nCntEditEffect].fRadius.x, -g_aEditEffect[nCntEditEffect].fRadius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aEditEffect[nCntEditEffect].fRadius.x, -g_aEditEffect[nCntEditEffect].fRadius.y, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = g_aEditEffect[nCntEditEffect].col;
			pVtx[1].col = g_aEditEffect[nCntEditEffect].col;
			pVtx[2].col = g_aEditEffect[nCntEditEffect].col;
			pVtx[3].col = g_aEditEffect[nCntEditEffect].col;
		}

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEditEffect->Unlock();
}

//======================================================================================================================
//	エフェクトの描画処理
//======================================================================================================================
void DrawEditEffect(void)
{
	// 変数を宣言
	D3DXMATRIX mtxTrans;						// 計算用マトリックス
	D3DXMATRIX mtxView;							// ビューマトリックス取得用

												// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

												// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Zバッファ更新の有効 / 無効の設定

																// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntEditEffect = 0; nCntEditEffect < MAX_EDITEFFECT; nCntEditEffect++)
	{ // エフェクトの最大表示数分繰り返す
		if (g_aEditEffect[nCntEditEffect].bUse == true)
		{ // エフェクトが使用されている場合

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEditEffect[nCntEditEffect].mtxWorld);

			// ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aEditEffect[nCntEditEffect].mtxWorld, NULL, &mtxView);	// 逆行列を求める
			g_aEditEffect[nCntEditEffect].mtxWorld._41 = 0.0f;
			g_aEditEffect[nCntEditEffect].mtxWorld._42 = 0.0f;
			g_aEditEffect[nCntEditEffect].mtxWorld._43 = 0.0f;

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEditEffect[nCntEditEffect].pos.x, g_aEditEffect[nCntEditEffect].pos.y, g_aEditEffect[nCntEditEffect].pos.z);
			D3DXMatrixMultiply(&g_aEditEffect[nCntEditEffect].mtxWorld, &g_aEditEffect[nCntEditEffect].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEditEffect[nCntEditEffect].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEditEffect, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEditEffect[EDITEFFECTTEX_NORMAL]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEditEffect * 4, 2);
		}
	}

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Zバッファ更新の有効 / 無効の設定

																// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//======================================================================================================================
//	エフェクトの設定処理
//======================================================================================================================
void SetEditEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, D3DXVECTOR3 fRadius)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

						// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEditEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEditEffect = 0; nCntEditEffect < MAX_EDITEFFECT; nCntEditEffect++)
	{ // エフェクトの最大表示数分繰り返す

		if (g_aEditEffect[nCntEditEffect].bUse == false)
		{ // エフェクトが使用されていない場合

			// 引数を代入
			g_aEditEffect[nCntEditEffect].pos = pos;		// 位置
			g_aEditEffect[nCntEditEffect].move = move;		// 移動量
			g_aEditEffect[nCntEditEffect].col = col;		// 色
			g_aEditEffect[nCntEditEffect].nLife = nLife;		// 寿命
			g_aEditEffect[nCntEditEffect].fRadius = fRadius;	// 半径

			// 使用している状態にする
			g_aEditEffect[nCntEditEffect].bUse = true;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEditEffect[nCntEditEffect].fRadius.x, +g_aEditEffect[nCntEditEffect].fRadius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aEditEffect[nCntEditEffect].fRadius.x, +g_aEditEffect[nCntEditEffect].fRadius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEditEffect[nCntEditEffect].fRadius.x, -g_aEditEffect[nCntEditEffect].fRadius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aEditEffect[nCntEditEffect].fRadius.x, -g_aEditEffect[nCntEditEffect].fRadius.y, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = g_aEditEffect[nCntEditEffect].col;
			pVtx[1].col = g_aEditEffect[nCntEditEffect].col;
			pVtx[2].col = g_aEditEffect[nCntEditEffect].col;
			pVtx[3].col = g_aEditEffect[nCntEditEffect].col;

			// 処理を抜ける
			break;
		}

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEditEffect->Unlock();
}

#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================

#endif