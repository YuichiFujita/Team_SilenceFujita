//============================================================
//
//	アイコン処理 [icon.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "icon.h"
#include "player.h"
#include "camera.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MAX_ICON			(1024)		// 使用するポリゴン数 (アイコンの最大数)

#define PLAY_ICON_RADIUS	(130.0f)	// プレイヤーのアイコンの半径
#define PLAY_ICON_COL		(D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f))		// プレイヤーのアイコンの色

#define EVIL_ICON_RADIUS	(100.0f)	// 悪い奴のアイコンの半径
#define EVIL_ICON_COL		(D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f))		// 悪い奴のアイコンの色

#define POLICE_ICON_RADIUS	(100.0f)	// 警察のアイコンの半径
#define POLICE_ICON_COL		(D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f))		// 警察のアイコンの色

#define ICON_CORRECT_RIGHT	(4000.0f)	// アイコンの右側の補正係数
#define ICON_CORRECT_LEFT	(4000.0f)	// アイコンの左側の補正係数
#define ICON_CORRECT_FAR	(3800.0f)	// アイコンの奥側の補正係数
#define ICON_CORRECT_NEAR	(3800.0f)	// アイコンの手前側の補正係数

#define ICON_ALPHA_CHANGE	(0.005f)	// アイコンの透明度の変化量
#define ICON_REVIVAL_CNT	(20)		// 復活中のカウント
#define ICON_DAMAGE_COL		(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))		// ダメージ時の色
#define ICON_UNRIVALED_CNT	(10)		// 無敵状態のカウント

//************************************************************
//	グローバル変数
//************************************************************
LPDIRECT3DTEXTURE9      g_pTextureIcon = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffIcon = NULL;	// 頂点バッファへのポインタ

Icon g_aIcon[MAX_ICON];		// アイコンの情報

//=======================================================================================================
//	アイコンの初期化処理
//=======================================================================================================
void InitIcon(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ
	
	// テクスチャを読み込む
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\aaa.jpg", &g_pTextureIcon);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * 4 * MAX_ICON,		// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffIcon,
		NULL
	);

	// アイコンの情報の初期化
	for (int nCntIcon = 0; nCntIcon < MAX_ICON; nCntIcon++)
	{ // アイコンの最大表示数分繰り返す

		g_aIcon[nCntIcon].pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_aIcon[nCntIcon].type			= ICONTYPE_PLAY;						// アイコン
		g_aIcon[nCntIcon].col			= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
		g_aIcon[nCntIcon].colCopy		= g_aIcon[nCntIcon].col;				// 色のコピー
		g_aIcon[nCntIcon].nCounter		= 0;									// カウンター
		g_aIcon[nCntIcon].alpha			= ICONALPHA_NONE;						// 透明度の状態
		g_aIcon[nCntIcon].pIconIDParent = NULL;									// アイコンの親のアイコンインデックス
		g_aIcon[nCntIcon].pUseParent	= NULL;									// アイコンの親の使用状況
		g_aIcon[nCntIcon].bUse			= false;								// 使用状況
	}

	//--------------------------------------------------------
	//	頂点情報の初期化
	//--------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntIcon = 0; nCntIcon < MAX_ICON; nCntIcon++)
	{ // アイコンの最大表示数分繰り返す

	  // 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = g_aIcon[nCntIcon].col;
		pVtx[1].col = g_aIcon[nCntIcon].col;
		pVtx[2].col = g_aIcon[nCntIcon].col;
		pVtx[3].col = g_aIcon[nCntIcon].col;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffIcon->Unlock();
}

//=======================================================================================================
//	アイコンの終了処理
//=======================================================================================================
void UninitIcon(void)
{
	if (g_pTextureIcon != NULL)
	{ // 変数 (g_pTextureIcon) がNULLではない場合

		g_pTextureIcon->Release();
		g_pTextureIcon = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffIcon != NULL)
	{ // 変数 (g_pVtxBuffIcon) がNULLではない場合

		g_pVtxBuffIcon->Release();
		g_pVtxBuffIcon = NULL;
	}
}

//=======================================================================================================
//	アイコンの更新処理
//=======================================================================================================
void UpdateIcon(void)
{
	D3DXVECTOR3 cameraPos = GetCamera(CAMERATYPE_MAP)->posV;	// カメラの視点を取得

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntIcon = 0; nCntIcon < MAX_ICON; nCntIcon++)
	{ // アイコンの最大表示数分繰り返す

		if (g_aIcon[nCntIcon].bUse == true && g_aIcon[nCntIcon].pUseParent != NULL)
		{ // アイコンが使用されている且つ、アイコンの親の使用状況アドレスが設定されている場合

			if (*g_aIcon[nCntIcon].pUseParent == false)
			{ // アイコンの親が使用されていない場合

				// アイコンを使用していない状態にする
				g_aIcon[nCntIcon].bUse = false;
			}
			else
			{ // アイコンの親が使用されている場合

				if (g_aIcon[nCntIcon].pIconIDParent != NULL)
				{ // アイコンの親のアイコンインデックスアドレスが設定されている場合

					if (*g_aIcon[nCntIcon].pIconIDParent != nCntIcon)
					{ // アイコンの親のアイコンインデックスと、現在のアイコンインデックスの値が違う場合

						// アイコンを使用していない状態にする
						g_aIcon[nCntIcon].bUse = false;

						// [※] 使用しない状態になった親が同フレーム内で同じインデックスが使われて
						//      また使用された場合にアイコンが一つの親に二つ生成されてしまうため、その防止。
					}
				}

				switch (*g_aIcon[nCntIcon].pState)
				{
				case ICONSTATE_NONE:		// 何もなし

					// カウンターを初期化する
					g_aIcon[nCntIcon].nCounter = 0;

					// 透明度を設定する
					g_aIcon[nCntIcon].col.a = 1.0f;

					// 透明度の位を設定する
					g_aIcon[nCntIcon].alpha = ICONALPHA_NONE;

					// 色を設定する
					g_aIcon[nCntIcon].col = g_aIcon[nCntIcon].colCopy;

					break;					// 抜け出す

				case ICONSTATE_DISAPPEAR:	// 消える途中

					// カウンターを初期化する
					g_aIcon[nCntIcon].nCounter = 0;

					// 透明度を下げる
					g_aIcon[nCntIcon].col.a -= ICON_ALPHA_CHANGE;

					if (g_aIcon[nCntIcon].col.a <= 0.0f)
					{ //透明度が1.0fを超えた場合

						// 透明度を1.0fにする
						g_aIcon[nCntIcon].col.a = 0.0f;
					}

					break;					// 抜け出す

				case ICONSTATE_REVIVAL:		// 復活中

					// カウンターを加算する
					g_aIcon[nCntIcon].nCounter++;

					if (g_aIcon[nCntIcon].nCounter % ICON_REVIVAL_CNT == 0)
					{ // 一定数ごとに

						// 透明度の状態を設定
						g_aIcon[nCntIcon].alpha = (ICONALPHA)((g_aIcon[nCntIcon].alpha + 1) % ICONALPHA_MAX);
					}

					switch (g_aIcon[nCntIcon].alpha)
					{
					case ICONALPHA_NONE:	// 不透明

						// 透明度を設定する
						g_aIcon[nCntIcon].col.a = 1.0f;

						break;				// 抜け出す

					case ICONALPHA_CLEAR:	// 透明

						// 透明度を設定する
						g_aIcon[nCntIcon].col.a = 0.0f;

						break;				// 抜け出す
					}

					break;					// 抜け出す

				case ICONSTATE_DAMAGE:		// ダメージ中

					// カウンターを初期化する
					g_aIcon[nCntIcon].nCounter = 0;

					// 色を赤くする
					g_aIcon[nCntIcon].col = ICON_DAMAGE_COL;

					break;					// 抜け出す

				case ICONSTATE_UNRIVALED:	// 無敵状態

					// 色を設定する
					g_aIcon[nCntIcon].col = g_aIcon[nCntIcon].colCopy;

					// カウンターを加算する
					g_aIcon[nCntIcon].nCounter++;

					if (g_aIcon[nCntIcon].nCounter % ICON_UNRIVALED_CNT == 0)
					{ // 一定数ごとに

						// 透明度の状態を設定
						g_aIcon[nCntIcon].alpha = (ICONALPHA)((g_aIcon[nCntIcon].alpha + 1) % ICONALPHA_MAX);
					}

					switch (g_aIcon[nCntIcon].alpha)
					{
					case ICONALPHA_NONE:	// 不透明

						// 透明度を設定する
						g_aIcon[nCntIcon].col.a = 1.0f;

						break;				// 抜け出す

					case ICONALPHA_CLEAR:	// 透明

						// 透明度を設定する
						g_aIcon[nCntIcon].col.a = 0.0f;

						break;				// 抜け出す
					}

					break;					// 抜け出す
				}

				if (cameraPos.x + ICON_CORRECT_RIGHT <= g_aIcon[nCntIcon].pos.x + g_aIcon[nCntIcon].fRadius)
				{ // 右側よりも外側にアイコンがある場合
					// アイコンを補正する
					g_aIcon[nCntIcon].pos.x = cameraPos.x + ICON_CORRECT_RIGHT - g_aIcon[nCntIcon].fRadius;
				}

				if (cameraPos.x - ICON_CORRECT_LEFT >= g_aIcon[nCntIcon].pos.x - g_aIcon[nCntIcon].fRadius)
				{ // 左側よりも外側にアイコンがある場合
					// アイコンを補正する
					g_aIcon[nCntIcon].pos.x = cameraPos.x - ICON_CORRECT_LEFT + g_aIcon[nCntIcon].fRadius;
				}

				if (cameraPos.z + ICON_CORRECT_FAR <= g_aIcon[nCntIcon].pos.z + g_aIcon[nCntIcon].fRadius)
				{ // 奥側よりも外側にアイコンがある場合
					// アイコンを補正する
					g_aIcon[nCntIcon].pos.z = cameraPos.z + ICON_CORRECT_FAR - g_aIcon[nCntIcon].fRadius;
				}

				if (cameraPos.z - ICON_CORRECT_NEAR >= g_aIcon[nCntIcon].pos.z - g_aIcon[nCntIcon].fRadius)
				{ // 手前側よりも外側にアイコンがある場合
					// アイコンを補正する
					g_aIcon[nCntIcon].pos.z = cameraPos.z - ICON_CORRECT_NEAR + g_aIcon[nCntIcon].fRadius;
				}
			}

			// 頂点カラーの設定
			pVtx[0].col = g_aIcon[nCntIcon].col;
			pVtx[1].col = g_aIcon[nCntIcon].col;
			pVtx[2].col = g_aIcon[nCntIcon].col;
			pVtx[3].col = g_aIcon[nCntIcon].col;
		}

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffIcon->Unlock();
}

//=======================================================================================================
//	アイコンの描画処理
//=======================================================================================================
void DrawIcon(void)
{
	D3DXMATRIX mtxTrans;			//計算用マトリックス
	D3DXMATRIX mtxView;				//ビューマトリックス取得用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	//ライティングをOFFにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);					//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						//Zテストの有効/無効設定

	for (int nCntIcon = 0; nCntIcon < MAX_ICON; nCntIcon++)
	{ // アイコンの最大表示数分繰り返す

		if (g_aIcon[nCntIcon].bUse == true)
		{ // アイコンが使用されている場合

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aIcon[nCntIcon].mtxWorld);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aIcon[nCntIcon].pos.x, g_aIcon[nCntIcon].pos.y, g_aIcon[nCntIcon].pos.z);
			D3DXMatrixMultiply(&g_aIcon[nCntIcon].mtxWorld, &g_aIcon[nCntIcon].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aIcon[nCntIcon].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffIcon, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureIcon);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntIcon * 4, 2);
		}
	}

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				//Zテストの有効/無効設定

	//ライティングをONにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//===============================
//アイコンの設定処理
//===============================
int SetIcon(D3DXVECTOR3 pos, ICONTYPE type, int *pIconID, bool *pUse, ICONSTATE *pState)
{
	//アイコンの番号を初期化する
	int nIdxIcon = NONE_ICON;

	D3DXVECTOR3 playerPos = GetPlayer()->pos;		// プレイヤーの位置を取得

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntIcon = 0; nCntIcon < MAX_ICON; nCntIcon++)
	{
		if (g_aIcon[nCntIcon].bUse == false)
		{//使用していなかった場合

			// 情報の設定
			g_aIcon[nCntIcon].pos.x = pos.x;			//位置
			g_aIcon[nCntIcon].pos.z = pos.z;
			g_aIcon[nCntIcon].nCounter = 0;				// カウンター
			g_aIcon[nCntIcon].alpha = ICONALPHA_NONE;	// 透明度の状態

			// 種類を設定する
			g_aIcon[nCntIcon].type = type;

			// 引数のアドレスを設定する
			g_aIcon[nCntIcon].pIconIDParent = pIconID;		// アイコンのインデックス
			g_aIcon[nCntIcon].pUseParent = pUse;			// 親の使用状況
			g_aIcon[nCntIcon].pState = pState;				// 状態

			switch (g_aIcon[nCntIcon].type)
			{
			case ICONTYPE_PLAY:		// プレイヤー

				// 半径を設定
				g_aIcon[nCntIcon].fRadius = PLAY_ICON_RADIUS;	

				// 色を設定
				g_aIcon[nCntIcon].col = PLAY_ICON_COL;

				break;				// 抜け出す

			case ICONTYPE_EVIL:		// 悪い奴

				// 半径を設定
				g_aIcon[nCntIcon].fRadius = EVIL_ICON_RADIUS;

				// 色を設定
				g_aIcon[nCntIcon].col = EVIL_ICON_COL;

				break;				// 抜け出す

			case ICONTYPE_POLICE:	// 警察

				// 半径を設定
				g_aIcon[nCntIcon].fRadius = POLICE_ICON_RADIUS;

				// 色を設定
				g_aIcon[nCntIcon].col = POLICE_ICON_COL;

				break;				// 抜け出す
			}

			// 色のコピーを設定する
			g_aIcon[nCntIcon].colCopy = g_aIcon[nCntIcon].col;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aIcon[nCntIcon].fRadius, +0.0f, +g_aIcon[nCntIcon].fRadius);
			pVtx[1].pos = D3DXVECTOR3(+g_aIcon[nCntIcon].fRadius, +0.0f, +g_aIcon[nCntIcon].fRadius);
			pVtx[2].pos = D3DXVECTOR3(-g_aIcon[nCntIcon].fRadius, +0.0f, -g_aIcon[nCntIcon].fRadius);
			pVtx[3].pos = D3DXVECTOR3(+g_aIcon[nCntIcon].fRadius, +0.0f, -g_aIcon[nCntIcon].fRadius);

			// 頂点カラーの設定
			pVtx[0].col = g_aIcon[nCntIcon].col;
			pVtx[1].col = g_aIcon[nCntIcon].col;
			pVtx[2].col = g_aIcon[nCntIcon].col;
			pVtx[3].col = g_aIcon[nCntIcon].col;

			//使用する
			g_aIcon[nCntIcon].bUse = true;

			//アイコンの番号をコピーする
			nIdxIcon = nCntIcon;

			break;				//抜け出す
		}
		pVtx += 4;				//頂点データを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffIcon->Unlock();

	//アイコンの番号を返す
	return nIdxIcon;
}

//=======================================================================================================
//	アイコンの位置の設定処理
//=======================================================================================================
void SetPositionIcon(int nIconID, D3DXVECTOR3 pos)
{
	// 変数を宣言
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;		// 計算用マトリックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	if (nIconID > NONE_ICON)
	{ // 使用できるIDの場合

		// 引数のアイコンの位置を設定
		g_aIcon[nIconID].pos = pos;
	}
}
