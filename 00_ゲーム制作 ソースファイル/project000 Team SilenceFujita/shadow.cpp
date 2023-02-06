//============================================================
//
//	影処理 [shadow.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "shadow.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MAX_SHADOW		(512)		// 使用するポリゴン数 (影の最大数)

//************************************************************
//	コンスト定義
//************************************************************
const char *apTextureShadow[] =		// テクスチャの相対パス
{
	"data\\TEXTURE\\shadow000.jpg",	// 影のテクスチャの相対パス
};

//************************************************************
//	列挙型定義 (TEXTURE_SHADOW)
//************************************************************
typedef enum
{
	TEXTURE_SHADOW_NORMAL = 0,		// 影 (通常)
	TEXTURE_SHADOW_MAX,				// この列挙型の総数
} TEXTURE_SHADOW;

//************************************************************
//	構造体定義 (Shadow)
//************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 rot;				// 向き
	D3DXMATRIX  mtxWorld;			// ワールドマトリックス
	D3DXMATRIX  mtxParent;			// 影の親のワールドマトリックス
	Model       modelDataParent;	// 影の親のモデル情報
	SHADOWTYPE  type;				// 種類
	int        *pShadowIDParent;	// 影の親の影インデックス
	bool       *pUseParent;			// 影の親の使用状況
	bool        bUse;				// 使用状況
} Shadow;

//************************************************************
//	グローバル変数
//************************************************************
LPDIRECT3DTEXTURE9      g_apTextureShadow[TEXTURE_SHADOW_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;					// 頂点バッファへのポインタ

Shadow g_aShadow[MAX_SHADOW];		// 影の情報

//=======================================================================================================
//	影の初期化処理
//=======================================================================================================
void InitShadow(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ

	// テクスチャの読み込み
	for (int nCntShadow = 0; nCntShadow < TEXTURE_SHADOW_MAX; nCntShadow++)
	{ // 使用するテクスチャ数分繰り返す

		D3DXCreateTextureFromFile(pDevice, apTextureShadow[nCntShadow], &g_apTextureShadow[nCntShadow]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * 4 * MAX_SHADOW,		// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL
	);

	// 影の情報の初期化
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{ // 影の最大表示数分繰り返す

		g_aShadow[nCntShadow].pos             = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aShadow[nCntShadow].rot             = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
		g_aShadow[nCntShadow].type            = SHADOWTYPE_NONE;				// 種類
		g_aShadow[nCntShadow].pShadowIDParent = NULL;							// 影の親の影インデックス
		g_aShadow[nCntShadow].pUseParent      = NULL;							// 影の親の使用状況
		g_aShadow[nCntShadow].bUse            = false;							// 使用状況

		// モデル情報の初期化
		g_aShadow[nCntShadow].modelDataParent.dwNumMat = 0;				// マテリアルの数
		g_aShadow[nCntShadow].modelDataParent.pTexture = NULL;			// テクスチャへのポインタ
		g_aShadow[nCntShadow].modelDataParent.pMesh    = NULL;			// メッシュ (頂点情報) へのポインタ
		g_aShadow[nCntShadow].modelDataParent.pBuffMat = NULL;			// マテリアルへのポインタ
		g_aShadow[nCntShadow].modelDataParent.dwNumMat = 0;				// マテリアルの数
		g_aShadow[nCntShadow].modelDataParent.vtxMin   = INIT_VTX_MIN;	// 最小の頂点座標
		g_aShadow[nCntShadow].modelDataParent.vtxMax   = INIT_VTX_MAX;	// 最大の頂点座標
		g_aShadow[nCntShadow].modelDataParent.size     = INIT_SIZE;		// 大きさ
		g_aShadow[nCntShadow].modelDataParent.fRadius  = 0.0f;			// 半径
	}

	//--------------------------------------------------------
	//	頂点情報の初期化
	//--------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{ // 影の最大表示数分繰り返す

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
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();
}

//=======================================================================================================
//	影の終了処理
//=======================================================================================================
void UninitShadow(void)
{
	// テクスチャの破棄
	for (int nCntShadow = 0; nCntShadow < TEXTURE_SHADOW_MAX; nCntShadow++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTextureShadow[nCntShadow] != NULL)
		{ // 変数 (g_apTextureShadow) がNULLではない場合

			g_apTextureShadow[nCntShadow]->Release();
			g_apTextureShadow[nCntShadow] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{ // 変数 (g_pVtxBuffShadow) がNULLではない場合

		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//=======================================================================================================
//	影の更新処理
//=======================================================================================================
void UpdateShadow(void)
{
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{ // 影の最大表示数分繰り返す

		if (g_aShadow[nCntShadow].bUse == true && g_aShadow[nCntShadow].pUseParent != NULL)
		{ // 影が使用されている且つ、影の親の使用状況アドレスが設定されている場合

			if (*g_aShadow[nCntShadow].pUseParent == false)
			{ // 影の親が使用されていない場合

				// 影を使用していない状態にする
				g_aShadow[nCntShadow].bUse = false;
			}
			else
			{ // 影の親が使用されている場合

				if (g_aShadow[nCntShadow].pShadowIDParent != NULL)
				{ // 影の親の影インデックスアドレスが設定されている場合

					if (*g_aShadow[nCntShadow].pShadowIDParent != nCntShadow)
					{ // 影の親の影インデックスと、現在の影インデックスの値が違う場合

						// 影を使用していない状態にする
						g_aShadow[nCntShadow].bUse = false;

						// [※] 使用しない状態になった親が同フレーム内で同じインデックスが使われて
						//      また使用された場合に影が一つの親に二つ生成されてしまうため、その防止。
					}
				}
			}
		}
	}
}

//=======================================================================================================
//	影の描画処理
//=======================================================================================================
void DrawShadow(void)
{
	// 変数を宣言
	D3DXMATRIX   mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATRIX   mtxShadow;			// シャドウマトリックス
	D3DMATERIAL9 matDef;			// マテリアル保存用
	D3DLIGHT9    light;				// ライトの情報
	D3DXVECTOR4  posLight;			// ライトの位置
	D3DXVECTOR3  pos, normal;		// 平面上の任意の位置、法線ベクトル
	D3DXPLANE    plane;				// 平面情報
	D3DXMATERIAL blackMat;			// マテリアルデータ (黒)

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Zバッファ更新の有効 / 無効の設定

	// ライトの位置を設定
	pDevice->GetLight(0, &light);
	posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{ // 影の最大表示数分繰り返す

		if (g_aShadow[nCntShadow].bUse == true)
		{ // 影が使用されている場合

			switch (g_aShadow[nCntShadow].type)
			{ // 種類ごとの処理
			case SHADOWTYPE_CIRCLE:	// 丸影

				// αブレンディングを減算合成に設定
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

				// 向きを反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
				D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

				// 位置を反映
				D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
				D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

				// 頂点バッファをデータストリームに設定
				pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_apTextureShadow[TEXTURE_SHADOW_NORMAL]);

				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);

				// αブレンディングを元に戻す
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

				// 処理を抜ける
				break;

			case SHADOWTYPE_MODEL:	// モデル影

				// 平面情報を生成
				pos    = g_aShadow[nCntShadow].pos;
				normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				D3DXPlaneFromPointNormal(&plane, &pos, &normal);

				// シャドウマトリックスの初期化
				D3DXMatrixIdentity(&mtxShadow);

				// シャドウマトリックスの生成
				D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
				D3DXMatrixMultiply(&mtxShadow, &g_aShadow[nCntShadow].mtxParent, &mtxShadow);

				// シャドウマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				for (int nCntMat = 0; nCntMat < (int)g_aShadow[nCntShadow].modelDataParent.dwNumMat; nCntMat++)
				{ // マテリアルの数分繰り返す

					// 構造体の要素をクリア
					ZeroMemory(&blackMat, sizeof(D3DXMATERIAL));

					// 拡散光・環境光・自己発光を黒にする
					blackMat.MatD3D.Diffuse  = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					blackMat.MatD3D.Ambient  = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					blackMat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

					// マテリアルの設定
					pDevice->SetMaterial(&blackMat.MatD3D);	// 黒

					// テクスチャの設定
					pDevice->SetTexture(0, NULL);

					// モデルの描画
					g_aShadow[nCntShadow].modelDataParent.pMesh->DrawSubset(nCntMat);
				}

				// 保存していたマテリアルを戻す
				pDevice->SetMaterial(&matDef);

				// 処理を抜ける
				break;
			}
		}
	}

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Zバッファ更新の有効 / 無効の設定
}

//=======================================================================================================
//	丸影の設定処理
//=======================================================================================================
int SetCircleShadow(float fAlpha, float fRadius, int *pShadowID, bool *pUse)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{ // 影の最大表示数分繰り返す

		if (g_aShadow[nCntShadow].bUse == false)
		{ // 影が使用されていない場合

			// 引数のアドレスを代入
			g_aShadow[nCntShadow].pShadowIDParent = pShadowID;	// 影の親の影インデックス
			g_aShadow[nCntShadow].pUseParent      = pUse;		// 影の親の使用状況

			// 種類を丸影に設定
			g_aShadow[nCntShadow].type = SHADOWTYPE_CIRCLE;

			// 使用している状態にする
			g_aShadow[nCntShadow].bUse = true;

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点データのポインタを使用する影のポインタまで進める
			pVtx += nCntShadow * 4;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-fRadius, 0.0f,  fRadius);
			pVtx[1].pos = D3DXVECTOR3( fRadius, 0.0f,  fRadius);
			pVtx[2].pos = D3DXVECTOR3(-fRadius, 0.0f, -fRadius);
			pVtx[3].pos = D3DXVECTOR3( fRadius, 0.0f, -fRadius);

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);

			// 頂点バッファをアンロックする
			g_pVtxBuffShadow->Unlock();

			// 使用できる影のIDを返す
			return nCntShadow;
		}
	}

	// 影の総数が足りない場合 -1を返す
	return -1;
}

//=======================================================================================================
//	モデル影の設定処理
//=======================================================================================================
int SetModelShadow(Model model, int *pShadowID, bool *pUse)
{
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{ // 影の最大表示数分繰り返す

		if (g_aShadow[nCntShadow].bUse == false)
		{ // 影が使用されていない場合

			// 引数を代入
			g_aShadow[nCntShadow].modelDataParent = model;		// 影の親のモデル情報

			// 引数のアドレスを代入
			g_aShadow[nCntShadow].pShadowIDParent = pShadowID;	// 影の親の影インデックス
			g_aShadow[nCntShadow].pUseParent      = pUse;		// 影の親の使用状況

			// 種類をモデル影に設定
			g_aShadow[nCntShadow].type = SHADOWTYPE_MODEL;

			// 使用している状態にする
			g_aShadow[nCntShadow].bUse = true;

			// 使用できる影のIDを返す
			return nCntShadow;
		}
	}

	// 影の総数が足りない場合 -1を返す
	return -1;
}

//=======================================================================================================
//	影の位置の設定処理
//=======================================================================================================
void SetPositionShadow(int nShadowID, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	// 変数を宣言
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;		// 計算用マトリックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	if (nShadowID > NONE_SHADOW)
	{ // 使用できるIDの場合

		// 引数の影の位置を設定
		g_aShadow[nShadowID].pos = pos;

		switch (g_aShadow[nShadowID].type)
		{ // 種類ごとの処理
		case SHADOWTYPE_CIRCLE:	// 丸影

			// 無し

			// 処理を抜ける
			break;

		case SHADOWTYPE_MODEL:	// モデル影

			// 影の親のワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aShadow[nShadowID].mtxParent);

			// 拡大率を反映
			D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
			D3DXMatrixMultiply(&g_aShadow[nShadowID].mtxParent, &g_aShadow[nShadowID].mtxParent, &mtxScale);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
			D3DXMatrixMultiply(&g_aShadow[nShadowID].mtxParent, &g_aShadow[nShadowID].mtxParent, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&g_aShadow[nShadowID].mtxParent, &g_aShadow[nShadowID].mtxParent, &mtxTrans);

			// 影の親のワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nShadowID].mtxParent);

			// 処理を抜ける
			break;
		}
	}
}
