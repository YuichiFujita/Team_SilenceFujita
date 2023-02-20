//===========================================
//
// 再建築タイマーのメイン処理[buildtimer.cpp]
// Author 小原立暉
//
//===========================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "buildtimer.h"
#include "object.h"
#include "3DValue.h"
#include "player.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define BUILDTIMER_TEXTURE		"data/TEXTURE/BuildTimerUI.png"		// 再建築タイマーのテクスチャ
#define BUILDTIMER_RADIUS_X		(200.0f)				// 再建築タイマーの半径(X軸)
#define BUILDTIMER_RADIUS_Y		(80.0f)					// 再建築タイマーの半径(Y軸)
#define BUILDTIMER_POS_Y_SHIFT	(100.0f)				// 再建築タイマーのずらす高さ(Y軸)

#define MAX_VAL_BUILD			(2)						// 再建築タイマーの表示数値数
#define MAX_BUILD_MIN			(60)					// タイム (分) の最大値
#define MAX_BUILD_SEC			(60)					// タイム (秒) の最大値
#define BUILD_WAIT_TIME			(40)					// 待機状態のカウント数
#define BUILD_APPEAR_MAGNI		(0.05f)					// 出現時の拡大率

//**********************************************************************************************************************
//	構造体宣言(Build)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 radius;		// 半径
	D3DXMATRIX	mtx;		// ワールドマトリックス
	Object		object;		// オブジェクトのポインタ
	BUILDSTATE	state;		// 状態
	int			nWaitCount;	// 待機カウント
	int			nCount;		// カウント
	bool		bUse;		// 使用状況
}Build;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBuild = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBuild = NULL;	// 頂点バッファへのポインタ
Build g_aBuild[MAX_BUILDTIMER];					// 再建築タイマーの情報

//==========================================
// 再建築タイマーの初期化処理
//==========================================
void InitBuildtimer(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//下地のテクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			BUILDTIMER_TEXTURE,
			&g_pTextureBuild);

	for (int nCntSet = 0; nCntSet < MAX_BUILDTIMER; nCntSet++)
	{
		// 情報の初期化
		g_aBuild[nCntSet].pos	 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aBuild[nCntSet].nCount = 0;								// カウンター
		g_aBuild[nCntSet].nWaitCount = 0;							// 待機カウント
		g_aBuild[nCntSet].radius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 半径
		g_aBuild[nCntSet].state	 = BUILDSTATE_APPEAR;				// 状態
		g_aBuild[nCntSet].bUse	 = false;							// 使用状況
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BUILDTIMER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBuild,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffBuild->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BUILDTIMER; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-BUILDTIMER_RADIUS_X, +BUILDTIMER_RADIUS_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+BUILDTIMER_RADIUS_X, +BUILDTIMER_RADIUS_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BUILDTIMER_RADIUS_X, -BUILDTIMER_RADIUS_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+BUILDTIMER_RADIUS_X, -BUILDTIMER_RADIUS_Y, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

		pVtx += 4;						//頂点データを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBuild->Unlock();
}

//========================================
// 再建築タイマーの終了処理
//========================================
void UninitBuildtimer(void)
{
	//テクスチャの破棄
	if (g_pTextureBuild != NULL)
	{
		g_pTextureBuild->Release();
		g_pTextureBuild = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBuild != NULL)
	{
		g_pVtxBuffBuild->Release();
		g_pVtxBuffBuild = NULL;
	}
}

//========================================
// 再建築タイマーの更新処理
//========================================
void UpdateBuildtimer(void)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffBuild->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBill = 0; nCntBill < MAX_BUILDTIMER; nCntBill++)
	{
		if (g_aBuild[nCntBill].bUse == true)
		{//使用されていた場合

			switch (g_aBuild[nCntBill].state)
			{
			case BUILDSTATE_WAIT:		// 待機状態

				// 待機カウントを加算する
				g_aBuild[nCntBill].nWaitCount++;

				if (g_aBuild[nCntBill].nWaitCount >= BUILD_WAIT_TIME)
				{ // 待機カウント数を超えた場合

					// 待機カウントを0にする
					g_aBuild[nCntBill].nWaitCount = 0;

					// 出現状態にする
					g_aBuild[nCntBill].state = BUILDSTATE_APPEAR;
				}

				break;					// 抜け出す

			case BUILDSTATE_APPEAR:		// 出現状態

				// 拡大していく
				g_aBuild[nCntBill].radius.x += (BUILDTIMER_RADIUS_X * BUILD_APPEAR_MAGNI);	// X軸
				g_aBuild[nCntBill].radius.y += (BUILDTIMER_RADIUS_Y * BUILD_APPEAR_MAGNI);	// Y軸

				if (g_aBuild[nCntBill].radius.x >= BUILDTIMER_RADIUS_X
					|| g_aBuild[nCntBill].radius.y >= BUILDTIMER_RADIUS_Y)
				{ // どちらかの半径が超えた場合

					// 半径を補正する
					g_aBuild[nCntBill].radius = D3DXVECTOR3(BUILDTIMER_RADIUS_X, BUILDTIMER_RADIUS_Y, 0.0f);

					// カウント状態にする
					g_aBuild[nCntBill].state = BUILDSTATE_COUNT;
				}

				break;					// 抜け出す

			case BUILDSTATE_COUNT:		// カウント状態

				// カウントを減算する
				g_aBuild[nCntBill].nCount--;

				if (g_aBuild[nCntBill].nCount <= 0)
				{ // カウントが0を超えた場合

					// カウントを0にする
					g_aBuild[nCntBill].nCount = 0;

					// 消滅状態にする
					g_aBuild[nCntBill].state = BUILDSTATE_DISAPPEAR;
				}

				break;					// 抜け出す

			case BUILDSTATE_DISAPPEAR:	// 消滅状態

				// オブジェクトの設定処理
				SetObject
				(
					g_aBuild[nCntBill].object.pos,					// 位置
					g_aBuild[nCntBill].object.rot,					// 向き
					g_aBuild[nCntBill].object.scale,				// 拡大率
					&g_aBuild[nCntBill].object.matCopy[0],			// マテリアルデータ
					g_aBuild[nCntBill].object.nType,				// 種類
					g_aBuild[nCntBill].object.nBreakType,			// 壊れ方の種類
					g_aBuild[nCntBill].object.nShadowType,			// 影の種類
					g_aBuild[nCntBill].object.nCollisionType,		// 当たり判定の種類
					g_aBuild[nCntBill].object.collInfo.stateRot,	// 当たり判定の向きの状態
					APPEARSTATE_SLOWLY								// 徐々に出現
				);

				// 使用しない
				g_aBuild[nCntBill].bUse = false;

				break;					// 抜け出す
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aBuild[nCntBill].radius.x, +g_aBuild[nCntBill].radius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aBuild[nCntBill].radius.x, +g_aBuild[nCntBill].radius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aBuild[nCntBill].radius.x, -g_aBuild[nCntBill].radius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aBuild[nCntBill].radius.x, -g_aBuild[nCntBill].radius.y, 0.0f);
		}
		pVtx += 4;					//頂点データを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBuild->Unlock();
}

//=====================================
// 再建築タイマーの描画処理
//=====================================
void DrawBuildtimer(void)
{
	Player *pPlayer = GetPlayer();

	/*float fAngle;*/

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	D3DXMATRIX mtxTrans;			//計算用マトリックス
	D3DXMATRIX mtxView;				//ビューマトリックス取得用

	//ライティングをOFFにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);					//アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);				//アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 75);							//アルファテストの参照値設定

	for (int nCntDraw = 0; nCntDraw < MAX_BUILDTIMER; nCntDraw++)
	{
		if (g_aBuild[nCntDraw].bUse == true)
		{//使用していた場合

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBuild[nCntDraw].mtx);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに向けて正面に向ける
			D3DXMatrixInverse(&g_aBuild[nCntDraw].mtx, NULL, &mtxView);

			//逆行列を求める
			g_aBuild[nCntDraw].mtx._41 = 0.0f;
			g_aBuild[nCntDraw].mtx._42 = 0.0f;
			g_aBuild[nCntDraw].mtx._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBuild[nCntDraw].pos.x, g_aBuild[nCntDraw].pos.y, g_aBuild[nCntDraw].pos.z);
			D3DXMatrixMultiply(&g_aBuild[nCntDraw].mtx, &g_aBuild[nCntDraw].mtx, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBuild[nCntDraw].mtx);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0,
				g_pVtxBuffBuild,									//頂点バッファへのポインタ
				0,
				sizeof(VERTEX_3D));									//頂点情報構造体のサイズ

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBuild);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//プリミティブの種類
				4 * nCntDraw,										//描画する最初の頂点インデックス
				2);													//描画するプリミティブ数
		}
	}

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			//アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		//アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						//アルファテストの参照値設定

	//ライティングをONにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);


	//for (int nCntDraw = 0; nCntDraw < MAX_BUILDTIMER; nCntDraw++)
	//{
	//	if (g_aBuild[nCntDraw].bUse == true)
	//	{//使用していた場合

	//		// 角度を取る
	//		fAngle = atan2f(g_aBuild[nCntDraw].pos.x - pPlayer->pos.x, g_aBuild[nCntDraw].pos.z - pPlayer->pos.z);

	//		// 角度に足す
	//		fAngle += D3DX_PI * 0.5f;

	//		for (int nCntValue = 0; nCntValue < 2; nCntValue++)
	//		{
	//			// 時間を設定する
	//			int nMin = g_aBuild[nCntDraw].nCount / 3600;			// タイマー (分)
	//			int nSec = (g_aBuild[nCntDraw].nCount % 3600) / 60;		// タイマー (秒)

	//			int aValue[MAX_VAL_BUILD] = { nMin,    nSec };			// タイマー表示用
	//			int aMaxValue[MAX_VAL_BUILD] = { MAX_BUILD_MIN, MAX_BUILD_SEC };	// タイマー表示の値の制限用

	//			// 3Dの数値の設定
	//			Set3DValue
	//			( // 引数
	//				aValue[nCntValue],		// 値
	//				aMaxValue[nCntValue],	// 値の最大値
	//				50.0f,					// 横幅
	//				70.0f,					// 縦幅
	//				1.0f					// α値
	//			);

	//			// 3Dの数値の描画処理
	//			Draw3DValue
	//			( // 引数
	//				D3DXVECTOR3(g_aBuild[nCntDraw].pos.x + cosf(fAngle) * (30.0f * nCntValue), g_aBuild[nCntDraw].pos.y, g_aBuild[nCntDraw].pos.z + sinf(fAngle) * (30.0f * nCntValue)),
	//				MAX_VAL_BUILD,
	//				VALUE3DTYPE_NORMAL
	//			);
	//		}
	//	}
	//}
}

//======================================
// 再建築タイマーの設定処理
//======================================
void SetBuildtimer(D3DXVECTOR3 pos, int nCount, Object object)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffBuild->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBill = 0; nCntBill < MAX_BUILDTIMER; nCntBill++)
	{
		if (g_aBuild[nCntBill].bUse == false)
		{//使用されていなかった場合
			
			// 情報の設定
			g_aBuild[nCntBill].pos		= D3DXVECTOR3(pos.x, pos.y + BUILDTIMER_POS_Y_SHIFT, pos.z);	// 位置
			g_aBuild[nCntBill].radius	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);									// 半径
			g_aBuild[nCntBill].object	= object;					// オブジェクトの情報
			g_aBuild[nCntBill].nCount	= nCount;					// カウント
			g_aBuild[nCntBill].nWaitCount = 0;						// 待機カウント
			g_aBuild[nCntBill].state	= BUILDSTATE_WAIT;			// 状態

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aBuild[nCntBill].radius.x, +g_aBuild[nCntBill].radius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aBuild[nCntBill].radius.x, +g_aBuild[nCntBill].radius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aBuild[nCntBill].radius.x, -g_aBuild[nCntBill].radius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aBuild[nCntBill].radius.x, -g_aBuild[nCntBill].radius.y, 0.0f);

			//使用する
			g_aBuild[nCntBill].bUse = true;

			break;									//抜け出す
		}
		pVtx += 4;									//頂点データを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBuild->Unlock();
}

//======================================
// 再建築タイマーの総数取得処理
//======================================
int GetNumBuildTimer(void)
{
	int nNumBuild = 0;

	for (int nCnt = 0; nCnt < MAX_BUILDTIMER; nCnt++)
	{
		if (g_aBuild[nCnt].bUse == true)
		{ // 使用している場合

			// 再建築の総数を加算する
			nNumBuild++;
		}
	}

	// 再建築タイマーの総数を返す
	return nNumBuild;
}