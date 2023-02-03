//======================================================================================================================
//
//	天気処理 [weather.cpp]
//	Author：小原立暉
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "weather.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define RAIN_TEXTURE		"data\\TEXTURE\\effect000.jpg"		// 雨のテクスチャ

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureWeather = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWeather = NULL;		// 頂点バッファへのポインタ

Rain g_aRain[MAX_RAIN];		// エフェクトの情報

//======================================================================================================================
//	エフェクトの初期化処理
//======================================================================================================================
void InitWeather(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, RAIN_TEXTURE, &g_apTextureWeather);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * 4 * MAX_RAIN,		// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffWeather,
		NULL
	);

	// 雨の情報の初期化
	for (int nCntWeather = 0; nCntWeather < MAX_RAIN; nCntWeather++)
	{ // 雨の最大表示数分繰り返す

		g_aRain[nCntWeather].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_aRain[nCntWeather].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
		g_aRain[nCntWeather].fRadius = 0.0f;							// 半径
		g_aRain[nCntWeather].bUse = false;								// 使用状況

	}

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWeather->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_RAIN; nCntWeather++)
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
	g_pVtxBuffWeather->Unlock();
}

//======================================================================================================================
//	エフェクトの終了処理
//======================================================================================================================
void UninitWeather(void)
{
	if (g_apTextureWeather != NULL)
	{ // 変数 (g_apTextureWeather) がNULLではない場合

		g_apTextureWeather->Release();
		g_apTextureWeather = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffWeather != NULL)
	{ // 変数 (g_pVtxBuffWeather) がNULLではない場合

		g_pVtxBuffWeather->Release();
		g_pVtxBuffWeather = NULL;
	}
}

//======================================================================================================================
//	エフェクトの更新処理
//======================================================================================================================
void UpdateWeather(void)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

						// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWeather->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_RAIN; nCntWeather++)
	{ // エフェクトの最大表示数分繰り返す

		if (g_aRain[nCntWeather].bUse == true)
		{ // エフェクトが使用されている場合

			// 移動の更新
			g_aRain[nCntWeather].pos += g_aRain[nCntWeather].move;

			// 半径の補正
			if (g_aRain[nCntWeather].fRadius < 0.0f)
			{ // 半径が 0.0f より小さい場合

				// 半径に 0.0f を代入
				g_aRain[nCntWeather].fRadius = 0.0f;
			}

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aRain[nCntWeather].fRadius, +g_aRain[nCntWeather].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aRain[nCntWeather].fRadius, +g_aRain[nCntWeather].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aRain[nCntWeather].fRadius, -g_aRain[nCntWeather].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aRain[nCntWeather].fRadius, -g_aRain[nCntWeather].fRadius, 0.0f);
		}

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffWeather->Unlock();
}

//======================================================================================================================
//	エフェクトの描画処理
//======================================================================================================================
void DrawWeather(void)
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

	for (int nCntWeather = 0; nCntWeather < MAX_RAIN; nCntWeather++)
	{ // エフェクトの最大表示数分繰り返す

		if (g_aRain[nCntWeather].bUse == true)
		{ // エフェクトが使用されている場合

		  // ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aRain[nCntWeather].mtxWorld);

			// ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aRain[nCntWeather].mtxWorld, NULL, &mtxView);	// 逆行列を求める
			g_aRain[nCntWeather].mtxWorld._41 = 0.0f;
			g_aRain[nCntWeather].mtxWorld._42 = 0.0f;
			g_aRain[nCntWeather].mtxWorld._43 = 0.0f;

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aRain[nCntWeather].pos.x, g_aRain[nCntWeather].pos.y, g_aRain[nCntWeather].pos.z);
			D3DXMatrixMultiply(&g_aRain[nCntWeather].mtxWorld, &g_aRain[nCntWeather].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aRain[nCntWeather].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffWeather, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureWeather);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWeather * 4, 2);
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
//	雨の設定処理
//======================================================================================================================
void SetRain(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRadius)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

						// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWeather->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_RAIN; nCntWeather++)
	{ // エフェクトの最大表示数分繰り返す

		if (g_aRain[nCntWeather].bUse == false)
		{ // エフェクトが使用されていない場合

			// 引数を代入
			g_aRain[nCntWeather].pos = pos;			// 位置
			g_aRain[nCntWeather].move = move;		// 移動量
			g_aRain[nCntWeather].fRadius = fRadius;	// 半径

			// 使用している状態にする
			g_aRain[nCntWeather].bUse = true;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aRain[nCntWeather].fRadius, +g_aRain[nCntWeather].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aRain[nCntWeather].fRadius, +g_aRain[nCntWeather].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aRain[nCntWeather].fRadius, -g_aRain[nCntWeather].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aRain[nCntWeather].fRadius, -g_aRain[nCntWeather].fRadius, 0.0f);

			// 処理を抜ける
			break;
		}

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffWeather->Unlock();
}

#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================
////**********************************************************************************************************************
////	エフェクトの総数取得処理
////**********************************************************************************************************************
//int GetNumWeather(void)
//{
//	// 変数を宣言
//	int nNumWeather = 0;	// エフェクトの総数の確認用
//
//	for (int nCntWeather = 0; nCntWeather < MAX_EFFECT; nCntWeather++)
//	{ // エフェクトの最大表示数分繰り返す
//
//		if (g_aWeather[nCntWeather].bUse == true)
//		{ // エフェクトが使用されている場合
//
//		  // カウンターを加算
//			nNumWeather++;
//		}
//	}
//
//	// 変数の値を返す
//	return nNumWeather;	// エフェクトの総数
//}
#endif