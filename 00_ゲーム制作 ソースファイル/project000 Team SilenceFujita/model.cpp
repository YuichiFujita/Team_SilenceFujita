//============================================================
//
//	モデル処理 [model.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "model.h"

//************************************************************
//	マクロ定義
//************************************************************
#define INIT_VTX_MIN	(D3DXVECTOR3( 9999.0f,  9999.0f,  9999.0f))		// モデルの最小の頂点座標の初期値
#define INIT_VTX_MAX	(D3DXVECTOR3(-9999.0f, -9999.0f, -9999.0f))		// モデルの最小の頂点座標の初期値

//************************************************************
//	コンスト定義
//************************************************************
const char *apModelData[] =		// モデルの相対パス
{
	// オブジェクトモデル
	"data\\MODEL_OBJECT\\Tree.x",		// 木
	"data\\MODEL_OBJECT\\Rock.x",		// 岩
	"data\\MODEL_OBJECT\\Forest.x",		// 森
	"data\\MODEL_OBJECT\\BigTree.x",	// 大木
	"data\\MODEL_OBJECT\\YellowTree.x",	// 黄色い木
	"data\\MODEL_OBJECT\\Grass.x",		// 草

	// プレイヤーモデル
	"data\\MODEL_PLAYER\\head.x",		// 頭
};

//************************************************************
//	プロトタイプ宣言
//************************************************************
HRESULT LoadXFileModel(void);	// xファイルの読み込み
void SetCollisionModel(void);	// 当たり判定の作成
HRESULT LoadTextureModel(void);	// テクスチャの読み込み

//************************************************************
//	グローバル変数
//************************************************************
Model g_aModel[MODELTYPE_MAX];	// モデルの情報

//============================================================
//	モデルの初期化処理
//============================================================
HRESULT InitModel(void)
{
	// モデル情報の初期化
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{ // モデルに使用するモデルの最大数分繰り返す

		g_aModel[nCntModel].pTexture = NULL;			// テクスチャへのポインタ
		g_aModel[nCntModel].pMesh    = NULL;			// メッシュ (頂点情報) へのポインタ
		g_aModel[nCntModel].pBuffMat = NULL;			// マテリアルへのポインタ
		g_aModel[nCntModel].dwNumMat = 0;				// マテリアルの数
		g_aModel[nCntModel].vtxMin   = INIT_VTX_MIN;	// 最小の頂点座標
		g_aModel[nCntModel].vtxMax   = INIT_VTX_MAX;	// 最大の頂点座標
		g_aModel[nCntModel].fHeight  = 0.0f;			// 縦幅
		g_aModel[nCntModel].fRadius  = 0.0f;			// 半径
	}

	// xファイルの読み込み
	if (FAILED(LoadXFileModel()))
	{ // xファイルの読み込みに失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 当たり判定の作成
	SetCollisionModel();

	// テクスチャの読み込み
	if (FAILED(LoadTextureModel()))
	{ // テクスチャの読み込みに失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	モデルの終了処理
//============================================================
void UninitModel(void)
{
	// テクスチャの破棄
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{ // モデルの最大数分繰り返す

		for (int nCntMat = 0; nCntMat < (int)g_aModel[nCntModel].dwNumMat; nCntMat++)
		{ // マテリアルの数分繰り返す

			if (g_aModel[nCntModel].pTexture[nCntMat] != NULL)
			{ // 変数 (g_aModel[nCntModel].pTexture) がNULLではない場合

				g_aModel[nCntModel].pTexture[nCntMat]->Release();
				g_aModel[nCntModel].pTexture[nCntMat] = NULL;
			}
		}

		// 動的確保したメモリを解放する
		free(g_aModel[nCntModel].pTexture);
	}

	// メッシュの破棄
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{ // モデルの最大数分繰り返す

		if (g_aModel[nCntModel].pMesh != NULL)
		{ // 変数 (g_aModel[nCntModel].pMesh) がNULLではない場合

			g_aModel[nCntModel].pMesh->Release();
			g_aModel[nCntModel].pMesh = NULL;
		}
	}

	// マテリアルの破棄
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{ // モデルの最大数分繰り返す

		if (g_aModel[nCntModel].pBuffMat != NULL)
		{ // 変数 (g_aModel[nCntModel].pBuffMat) がNULLではない場合

			g_aModel[nCntModel].pBuffMat->Release();
			g_aModel[nCntModel].pBuffMat = NULL;
		}
	}
}

//============================================================
//	xファイルの読み込み
//============================================================
HRESULT LoadXFileModel(void)
{
	// 変数を宣言
	HRESULT hr;		// 異常終了の確認用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{ // モデルの最大数分繰り返す

		// xファイルの読み込み
		hr = D3DXLoadMeshFromX
		( // 引数
			apModelData[nCntModel],				// モデルの相対パス
			D3DXMESH_SYSTEMMEM,					// メッシュ作成用オプション
			pDevice,							// デバイスへのポインタ
			NULL,								// 隣接性データ
			&g_aModel[nCntModel].pBuffMat,		// マテリアルへのポインタ
			NULL,								// エフェクトデータ
			&g_aModel[nCntModel].dwNumMat,		// マテリアルの数
			&g_aModel[nCntModel].pMesh			// メッシュ (頂点情報) へのポインタ
		);

		if (FAILED(hr))
		{ // xファイルの読み込みに失敗した場合

			// エラーメッセージボックス
			MessageBox(NULL, "xファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);

			// 失敗を返す
			return E_FAIL;
		}

		// 確保したメモリのアドレスを取得
		g_aModel[nCntModel].pTexture = (LPDIRECT3DTEXTURE9*)malloc(g_aModel[nCntModel].dwNumMat * sizeof(LPDIRECT3DTEXTURE9*));

		if (g_aModel[nCntModel].pTexture == NULL)
		{ // 動的確保に失敗した場合

			// エラーメッセージボックス
			MessageBox(NULL, "動的確保に失敗！", "警告！", MB_ICONWARNING);

			for (int nCntFree = 0; nCntFree < nCntModel; nCntFree++)
			{ // 動的確保に成功した回数分繰り返す

				// 動的確保したメモリを解放する
				free(g_aModel[nCntFree].pTexture);
			}

			// 失敗を返す
			return E_FAIL;
		}
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	当たり判定の作成
//============================================================
void SetCollisionModel(void)
{
	// 変数を宣言
	int         nNumVtx;		// モデルの頂点数
	DWORD       dwSizeFVF;		// モデルの頂点フォーマットのサイズ
	BYTE        *pVtxBuff;		// モデルの頂点バッファへのポインタ
	D3DXVECTOR3 vtx;			// モデルの頂点座標
	D3DXVECTOR3 size;			// モデルの大きさ

	// 当たり判定の作成
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{ // モデルの最大数分繰り返す

		// モデルの頂点数を取得
		nNumVtx = g_aModel[nCntModel].pMesh->GetNumVertices();

		// モデルの頂点フォーマットのサイズを取得
		dwSizeFVF = D3DXGetFVFVertexSize(g_aModel[nCntModel].pMesh->GetFVF());

		// モデルの頂点バッファをロック
		g_aModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{ // モデルの頂点数分繰り返す

			// モデルの頂点座標を代入
			vtx = *(D3DXVECTOR3*)pVtxBuff;

			// 頂点座標 (x) の設定
			if (vtx.x < g_aModel[nCntModel].vtxMin.x)
			{ // 今回の頂点座標 (x) が、現状の頂点座標 (x) よりも小さい場合

				// 今回の頂点情報 (x) を代入
				g_aModel[nCntModel].vtxMin.x = vtx.x;
			}
			else if (vtx.x > g_aModel[nCntModel].vtxMax.x)
			{ // 今回の頂点座標 (x) が、現状の頂点座標 (x) よりも大きい場合

				// 今回の頂点情報 (x) を代入
				g_aModel[nCntModel].vtxMax.x = vtx.x;
			}

			// 頂点座標 (y) の設定
			if (vtx.y < g_aModel[nCntModel].vtxMin.y)
			{ // 今回の頂点座標 (y) が、現状の頂点座標 (y) よりも小さい場合

				// 今回の頂点情報 (y) を代入
				g_aModel[nCntModel].vtxMin.y = vtx.y;
			}
			else if (vtx.y > g_aModel[nCntModel].vtxMax.y)
			{ // 今回の頂点座標 (y) が、現状の頂点座標 (y) よりも大きい場合

				// 今回の頂点情報 (y) を代入
				g_aModel[nCntModel].vtxMax.y = vtx.y;
			}

			// 頂点座標 (z) の設定
			if (vtx.z < g_aModel[nCntModel].vtxMin.z)
			{ // 今回の頂点座標 (z) が、現状の頂点座標 (z) よりも小さい場合

				// 今回の頂点情報 (z) を代入
				g_aModel[nCntModel].vtxMin.z = vtx.z;
			}
			else if (vtx.z > g_aModel[nCntModel].vtxMax.z)
			{ // 今回の頂点座標 (z) が、現状の頂点座標 (z) よりも大きい場合

				// 今回の頂点情報 (z) を代入
				g_aModel[nCntModel].vtxMax.z = vtx.z;
			}

			// 頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += dwSizeFVF;
		}

		// モデルの頂点バッファをアンロック
		g_aModel[nCntModel].pMesh->UnlockVertexBuffer();

		// モデルサイズを求める
		size = g_aModel[nCntModel].vtxMax - g_aModel[nCntModel].vtxMin;

		// モデルの縦幅を代入
		g_aModel[nCntModel].fHeight = size.y;

		// モデルの円の当たり判定を作成
		g_aModel[nCntModel].fRadius = ((size.x * 0.5f) + (size.z * 0.5f)) * 0.5f;
	}
}

//============================================================
//	テクスチャの読み込み
//============================================================
HRESULT LoadTextureModel(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルへのポインタ

	// テクスチャの読み込み
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{ // モデルに使用するモデルの最大数分繰り返す

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_aModel[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aModel[nCntModel].dwNumMat; nCntMat++)
		{ // マテリアルの数分繰り返す

			if (pMat[nCntMat].pTextureFilename != NULL)
			{ // テクスチャファイルが存在する場合

				// テクスチャの読み込み
				if (FAILED(D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aModel[nCntModel].pTexture[nCntMat])))
				{ // テクスチャの読み込みが失敗した場合

					// エラーメッセージボックス
					MessageBox(NULL, "テクスチャの読み込みに失敗！", "警告！", MB_ICONWARNING);

					// 失敗を返す
					return E_FAIL;
				}
			}
			else
			{ // テクスチャファイルが存在しない場合

				// NULLを設定
				g_aModel[nCntModel].pTexture[nCntMat] = NULL;
			}
		}
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	モデル情報の取得処理
//============================================================
Model *GetModelData(int nID)
{
	// 引数のモデルの情報アドレスを返す
	return &g_aModel[nID];
}