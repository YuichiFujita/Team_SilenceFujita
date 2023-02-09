//======================================================================================================================
//
//	送風機(フライ・アウェイ)処理 [wind.cpp]
//	Author：小原立暉
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "wind.h"
#include "player.h"
#include "calculation.h"

#include "camera.h"
#include "input.h"
#include "particle.h"
#include "object.h"
#include "Human.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_WIND			(1024)			//風の最大数

//**********************************************************************************************************************
//	構造体定義 (Bomb)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動量
	D3DXMATRIX  mtxWorld;	// ワールドマトリックス
	D3DXCOLOR   col;		// 色
	int         nLife;		// 寿命
	float       fRadius;	// 半径
	float       fAlpha;		// 1Fごとの薄くなる量
	bool        bUse;		// 使用状況
}Wind;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void FlyAwayHuman(Human *pHuman, Player player);					// 人間が吹き飛ばされる処理

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureWind = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWind = NULL;		// 頂点バッファへのポインタ
Wind g_aWind[MAX_WIND];								// 風の構造体
WindInfo g_WindInfo;								// 風の情報の構造体

//======================================================================================================================
//	送風機の初期化処理
//======================================================================================================================
void InitWind(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ

	for (int nCnt = 0; nCnt < MAX_WIND; nCnt++)
	{ // 情報の処理
		g_aWind[nCnt].pos	  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_aWind[nCnt].move	  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
		g_aWind[nCnt].col	  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
		g_aWind[nCnt].fAlpha  = 0.0f;								// 透明度
		g_aWind[nCnt].fRadius = 0.0f;								// 半径
		g_aWind[nCnt].nLife	  = 0;									// 寿命
		g_aWind[nCnt].bUse	  = false;								// 使用状況
	}

	// 風の情報の初期化
	g_WindInfo.nUseCounter = 0;										// 風のカウンターを初期化する
	g_WindInfo.nOverHeatCounter = 0;								// オーバーヒートカウンターを初期化する
	g_WindInfo.state = WIND_USABLE;									//使用可能状態

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/effect000.jpg",
		&g_pTextureWind);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * 4 * MAX_WIND,		// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffWind,
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWind->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWind = 0; nCntWind < MAX_WIND; nCntWind++)
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
	g_pVtxBuffWind->Unlock();
}

//======================================================================================================================
//	送風機の終了処理
//======================================================================================================================
void UninitWind(void)
{
	//テクスチャの破棄
	if (g_pTextureWind != NULL)
	{ // 変数 (g_pTextureWind) がNULLではない場合
		g_pTextureWind->Release();
		g_pTextureWind = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffWind != NULL)
	{ // 変数 (g_pVtxBuffWind) がNULLではない場合

		g_pVtxBuffWind->Release();
		g_pVtxBuffWind = NULL;
	}
}

//======================================================================================================================
//	送風機の更新処理
//======================================================================================================================
void UpdateWind(void)
{
	Player *pPlayer = GetPlayer();				// プレイヤーの情報を取得する
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWind->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWind = 0; nCntWind < MAX_WIND; nCntWind++)
	{
		if (g_aWind[nCntWind].bUse == true)
		{ // 使用した場合
			//移動させる
			g_aWind[nCntWind].pos += g_aWind[nCntWind].move;
			g_aWind[nCntWind].pos.x += (sinf(pPlayer->rot.y) * pPlayer->move.x);
			g_aWind[nCntWind].pos.z += (cosf(pPlayer->rot.y) * pPlayer->move.x);

			//寿命を減算させる
			g_aWind[nCntWind].nLife--;

			//透明にしていく
			g_aWind[nCntWind].col.a -= g_aWind[nCntWind].fAlpha;

			if (g_aWind[nCntWind].nLife <= 0)
			{ // 寿命が0以下になった場合
				// 使用しない
				g_aWind[nCntWind].bUse = false;
			}

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aWind[nCntWind].fRadius, +g_aWind[nCntWind].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aWind[nCntWind].fRadius, +g_aWind[nCntWind].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aWind[nCntWind].fRadius, -g_aWind[nCntWind].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aWind[nCntWind].fRadius, -g_aWind[nCntWind].fRadius, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = g_aWind[nCntWind].col;
			pVtx[1].col = g_aWind[nCntWind].col;
			pVtx[2].col = g_aWind[nCntWind].col;
			pVtx[3].col = g_aWind[nCntWind].col;
		}

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffWind->Unlock();
}

//======================================================================================================================
//	送風機の描画処理
//======================================================================================================================
void DrawWind(void)
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

	for (int nCntWind = 0; nCntWind < MAX_WIND; nCntWind++)
	{ // エフェクトの最大表示数分繰り返す

		if (g_aWind[nCntWind].bUse == true)
		{ // エフェクトが使用されている場合

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aWind[nCntWind].mtxWorld);

			// ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aWind[nCntWind].mtxWorld, NULL, &mtxView);	// 逆行列を求める
			g_aWind[nCntWind].mtxWorld._41 = 0.0f;
			g_aWind[nCntWind].mtxWorld._42 = 0.0f;
			g_aWind[nCntWind].mtxWorld._43 = 0.0f;

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aWind[nCntWind].pos.x, g_aWind[nCntWind].pos.y, g_aWind[nCntWind].pos.z);
			D3DXMatrixMultiply(&g_aWind[nCntWind].mtxWorld, &g_aWind[nCntWind].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aWind[nCntWind].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffWind, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureWind);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWind * 4, 2);
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
//	送風機の設定処理
//======================================================================================================================
void SetWind(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ

	float fMoveMagni;							// 移動量の倍率
	float fRadius;								// 半径
	int	  nLife;								// 寿命

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWind->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWind = 0; nCntWind < MAX_WIND; nCntWind++)
	{
		if (g_aWind[nCntWind].bUse == false)
		{ // 風を使用していない場合
			// 位置を設置する
			g_aWind[nCntWind].pos = pos;

			// 移動量の倍率を設定する
			fMoveMagni = (float)(rand() % 100) / 5;

			// 移動量を設定する
			g_aWind[nCntWind].move.x = sinf(rot.y) * fMoveMagni;
			g_aWind[nCntWind].move.y = 0.0f;
			g_aWind[nCntWind].move.z = cosf(rot.y) * fMoveMagni;

			// 色を設定する
			g_aWind[nCntWind].col = D3DXCOLOR(0.6f, 0.7f, 1.0f, 1.0f);

			//寿命を算出する
			nLife = (rand() % 10) + 6;

			// 寿命を設定する
			g_aWind[nCntWind].nLife = nLife;

			// 透明度を設定する
			g_aWind[nCntWind].fAlpha = 1.0f / nLife;

			//半径を算出する
			fRadius = (float)((rand() % 470) / 10);

			// 半径を設定する
			g_aWind[nCntWind].fRadius = fRadius;

			// 使用する
			g_aWind[nCntWind].bUse = true;

			// 抜け出す
			break;
		}

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffWind->Unlock();
}

//======================================================================================================================
//	風の当たり判定
//======================================================================================================================
void CollisionWind(Human *pHuman)
{
	Player *pPlayer = GetPlayer();			//プレイヤーの情報を取得する

	// 変数配列を宣言
	D3DXVECTOR3 vecPos[4];	// 頂点位置 ([※] 0：右上　1：左上　2：左下　3：右下)

	if (pHuman->pos.y <= 50.0f)
	{ // 300.0f以下にいる場合

		// 四頂点の位置の計算
		VecSizePos
		( // 引数
			&vecPos[0],
			pPlayer->pos,	// 絶対座標
			pPlayer->rot,	// 向き
			1000.0f,		// 横幅
			100.0f			// 縦幅
		);

		if (LineOuterProduct(vecPos[0], vecPos[1], pHuman->pos) < 0
		&&  LineOuterProduct(vecPos[1], vecPos[2], pHuman->pos) < 0
		&&  LineOuterProduct(vecPos[2], vecPos[3], pHuman->pos) < 0
		&&  LineOuterProduct(vecPos[3], vecPos[0], pHuman->pos) < 0)
		{ // 四辺の内側にいる場合 (当たっている場合)

			if (pPlayer->wind.bUseWind == true)
			{ // 風を使用している場合

				// 吹き飛んでいる状態にする
				pHuman->state = HUMANSTATE_FLY;

				// 人間が吹き飛ばされる処理
				FlyAwayHuman(pHuman, *pPlayer);

				// カメラの状態を変える
				*GetCameraState() = CAMERASTATE_GOODJOB;
			}
		}
	}
}

//============================================================
//	人間が吹き飛ばされる処理
//============================================================
void FlyAwayHuman(Human *pHuman, Player player)
{
	float FlyAngle = atan2f(pHuman->pos.x - player.pos.x, pHuman->pos.z - player.pos.z);

	// 移動量を設定する
	pHuman->move.x = sinf(FlyAngle) * 100.0f;
	pHuman->move.y = 25.0f;
	pHuman->move.z = cosf(FlyAngle) * 100.0f;

	// 飛ばす
	pHuman->pos += pHuman->move;
}

//============================================================
//	風の情報の取得処理
//============================================================
WindInfo *GetWindInfo(void)
{
	//風の情報を返す
	return &g_WindInfo;
}

#ifdef _DEBUG	// デバッグ処理
#endif