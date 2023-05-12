//=======================================
//
//セットマークのメイン処理[tiremark.cpp]
//Author 小原立暉
//
//=======================================
#include "main.h"
#include "tiremark.h"

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTireMark = NULL;	// 頂点バッファへのポインタ
TireMark g_aTireMark[MAX_TIREMARK];					// セットマークの情報

//==========================================
// タイヤ痕の初期化処理
//==========================================
void InitTireMark(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//爆発の情報の初期化
	for (int nCntTireMark = 0; nCntTireMark < MAX_TIREMARK; nCntTireMark++)
	{

		// 位置を初期化する
		g_aTireMark[nCntTireMark].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 向きを初期化する
		g_aTireMark[nCntTireMark].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 色を初期化する
		g_aTireMark[nCntTireMark].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// 縦の広さを初期化する
		g_aTireMark[nCntTireMark].fDepth = 0.0f;

		// 横の広さを初期化する
		g_aTireMark[nCntTireMark].fWidth = 0.0f;

		// 消去カウントを初期化する
		g_aTireMark[nCntTireMark].nClearCount = 0;

		// 使用していない
		g_aTireMark[nCntTireMark].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_TIREMARK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTireMark,
		NULL);

	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffTireMark->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTireMark = 0; nCntTireMark < MAX_TIREMARK; nCntTireMark++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aTireMark[nCntTireMark].fWidth, 0.0f, +g_aTireMark[nCntTireMark].fDepth);
		pVtx[1].pos = D3DXVECTOR3(+g_aTireMark[nCntTireMark].fWidth, 0.0f, +g_aTireMark[nCntTireMark].fDepth);
		pVtx[2].pos = D3DXVECTOR3(-g_aTireMark[nCntTireMark].fWidth, 0.0f, -g_aTireMark[nCntTireMark].fDepth);
		pVtx[3].pos = D3DXVECTOR3(+g_aTireMark[nCntTireMark].fWidth, 0.0f, -g_aTireMark[nCntTireMark].fDepth);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = g_aTireMark[nCntTireMark].col;
		pVtx[1].col = g_aTireMark[nCntTireMark].col;
		pVtx[2].col = g_aTireMark[nCntTireMark].col;
		pVtx[3].col = g_aTireMark[nCntTireMark].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;			//頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTireMark->Unlock();
}

//========================================
//経験値の終了処理
//========================================
void UninitTireMark(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffTireMark != NULL)
	{
		g_pVtxBuffTireMark->Release();
		g_pVtxBuffTireMark = NULL;
	}
}

//========================================
//経験値の更新処理
//========================================
void UpdateTireMark(void)
{
	VERTEX_3D * pVtx;				//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTireMark->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTireMark = 0; nCntTireMark < MAX_TIREMARK; nCntTireMark++)
	{
		if (g_aTireMark[nCntTireMark].bUse == true)
		{//使用していた場合
			// 消去カウントを加算する
			g_aTireMark[nCntTireMark].nClearCount++;

			if (g_aTireMark[nCntTireMark].nClearCount >= 50)
			{ // 消去カウントが30を超えた場合
				// 透明度を段々下げていく
				g_aTireMark[nCntTireMark].col.a -= 0.001f;
			}

			if (g_aTireMark[nCntTireMark].col.a <= 0.0f)
			{ // 透明度が0.0fを越した場合
				// 透明度を0.0fにする
				g_aTireMark[nCntTireMark].col.a = 0.0f;

				//使用しない
				g_aTireMark[nCntTireMark].bUse = false;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aTireMark[nCntTireMark].fWidth, 0.0f, +g_aTireMark[nCntTireMark].fDepth);
			pVtx[1].pos = D3DXVECTOR3(+g_aTireMark[nCntTireMark].fWidth, 0.0f, +g_aTireMark[nCntTireMark].fDepth);
			pVtx[2].pos = D3DXVECTOR3(-g_aTireMark[nCntTireMark].fWidth, 0.0f, -g_aTireMark[nCntTireMark].fDepth);
			pVtx[3].pos = D3DXVECTOR3(+g_aTireMark[nCntTireMark].fWidth, 0.0f, -g_aTireMark[nCntTireMark].fDepth);

			//頂点カラーの設定
			pVtx[0].col = g_aTireMark[nCntTireMark].col;
			pVtx[1].col = g_aTireMark[nCntTireMark].col;
			pVtx[2].col = g_aTireMark[nCntTireMark].col;
			pVtx[3].col = g_aTireMark[nCntTireMark].col;
		}

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffTireMark->Unlock();
}

//=====================================
//経験値の描画処理
//=====================================
void DrawTireMark(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	D3DXMATRIX mtxRot, mtxTrans;			// 計算用マトリックス
	D3DXMATRIX mtxView;						// ビューマトリックス取得用

	//デバイスの取得
	pDevice = GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//ライティングをOFFにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCntTireMark = 0; nCntTireMark < MAX_TIREMARK; nCntTireMark++)
	{
		if (g_aTireMark[nCntTireMark].bUse == true)
		{//セットマークが使用されていた場合
		 //ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aTireMark[nCntTireMark].mtx);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aTireMark[nCntTireMark].rot.y, g_aTireMark[nCntTireMark].rot.x, g_aTireMark[nCntTireMark].rot.z);
			D3DXMatrixMultiply(&g_aTireMark[nCntTireMark].mtx, &g_aTireMark[nCntTireMark].mtx, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aTireMark[nCntTireMark].pos.x, g_aTireMark[nCntTireMark].pos.y, g_aTireMark[nCntTireMark].pos.z);
			D3DXMatrixMultiply(&g_aTireMark[nCntTireMark].mtx, &g_aTireMark[nCntTireMark].mtx, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aTireMark[nCntTireMark].mtx);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0,
				g_pVtxBuffTireMark,									//頂点バッファへのポインタ
				0,
				sizeof(VERTEX_3D));									//頂点情報構造体のサイズ

			//テクスチャの設定
			pDevice->SetTexture(0, NULL);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//プリミティブの種類
				nCntTireMark * 4,									//描画する最初の頂点インデックス
				2);													//描画するプリミティブ数
		}
	}

	//ライティングをONにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//========================================
//経験値の設定処理
//========================================
void SetTireMark(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	VERTEX_3D * pVtx;					//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTireMark->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTireMark = 0; nCntTireMark < MAX_TIREMARK; nCntTireMark++)
	{
		if (g_aTireMark[nCntTireMark].bUse == false)
		{//弾が使用されていない

			// 情報を設定する
			g_aTireMark[nCntTireMark].pos = pos;
			g_aTireMark[nCntTireMark].rot = rot;
			g_aTireMark[nCntTireMark].fDepth = 8.0f;
			g_aTireMark[nCntTireMark].fWidth = 12.0f;
			g_aTireMark[nCntTireMark].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			g_aTireMark[nCntTireMark].nClearCount = 0;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aTireMark[nCntTireMark].fWidth, 0.0f, +g_aTireMark[nCntTireMark].fDepth);
			pVtx[1].pos = D3DXVECTOR3(+g_aTireMark[nCntTireMark].fWidth, 0.0f, +g_aTireMark[nCntTireMark].fDepth);
			pVtx[2].pos = D3DXVECTOR3(-g_aTireMark[nCntTireMark].fWidth, 0.0f, -g_aTireMark[nCntTireMark].fDepth);
			pVtx[3].pos = D3DXVECTOR3(+g_aTireMark[nCntTireMark].fWidth, 0.0f, -g_aTireMark[nCntTireMark].fDepth);

			//頂点カラーの設定
			pVtx[0].col = g_aTireMark[nCntTireMark].col;
			pVtx[1].col = g_aTireMark[nCntTireMark].col;
			pVtx[2].col = g_aTireMark[nCntTireMark].col;
			pVtx[3].col = g_aTireMark[nCntTireMark].col;

			//使用している状態にする
			g_aTireMark[nCntTireMark].bUse = true;				//使用している状態にする

			break;
		}
		pVtx += 4;//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffTireMark->Unlock();
}

//========================================
//セットマークの取得処理
//========================================
TireMark *GetTireMark(void)
{
	//セットマークの情報を取得する
	return &g_aTireMark[0];
}