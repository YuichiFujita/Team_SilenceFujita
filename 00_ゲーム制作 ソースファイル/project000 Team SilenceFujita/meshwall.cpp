//======================================================================================================================
//
//	メッシュウォール処理 [meshwall.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "meshwall.h"

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTextureMeshWall[] =		// テクスチャの相対パス
{
	"data\\TEXTURE\\wall000.png",		// メッシュウォールのテクスチャの相対パス
};

//**********************************************************************************************************************
//	列挙型定義 (TEXTURE_MESHWALL)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_MESHWALL_NORMAL = 0,		// メッシュウォール (通常)
	TEXTURE_MESHWALL_MAX,				// この列挙型の総数
} TEXTURE_MESHWALL;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nPartWidth, int nPartHeight);	// メッシュウォールの設定処理

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureMeshWall[TEXTURE_MESHWALL_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshWall = NULL;						// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9  g_pIdxBuffMeshWall = NULL;						// インデックスバッファへのポインタ

MeshWall g_aMeshWall[MAX_MESHWALL];		// メッシュウォールの情報
int      g_nNeedVtxWall;				// 必要頂点数
int      g_nNeedIdxWall;				// 必要インデックス数

//======================================================================================================================
//	メッシュウォールの初期化処理
//======================================================================================================================
void InitMeshWall(void)
{
	// 変数を宣言
	int nNumVtx = 0;							// 頂点数の計測用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ
	WORD      *pIdx;							// インデックス情報へのポインタ

	// グローバル変数の初期化
	g_nNeedVtxWall = 0;							// 必要頂点の総数
	g_nNeedIdxWall = 0;							// 必要インデックスの総数

	// メッシュウォールの情報の初期化
	for (int nCntMeshWall = 0; nCntMeshWall < MAX_MESHWALL; nCntMeshWall++)
	{ // メッシュウォールの最大表示数分繰り返す

		g_aMeshWall[nCntMeshWall].pos         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aMeshWall[nCntMeshWall].rot         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
		g_aMeshWall[nCntMeshWall].fWidth      = 0.0f;							// 横幅
		g_aMeshWall[nCntMeshWall].fHeight     = 0.0f;							// 縦幅
		g_aMeshWall[nCntMeshWall].nPartWidth  = 0;								// 横の分割数
		g_aMeshWall[nCntMeshWall].nPartHeight = 0;								// 縦の分割数
		g_aMeshWall[nCntMeshWall].nNumVtx     = 0;								// 必要頂点数
		g_aMeshWall[nCntMeshWall].nNumIdx     = 0;								// 必要インデックス数
		g_aMeshWall[nCntMeshWall].bUse        = false;							// 使用状況
	}

	// テクスチャの読み込み
	for (int nCntMeshWall = 0; nCntMeshWall < TEXTURE_MESHWALL_MAX; nCntMeshWall++)
	{ // 使用するテクスチャ数分繰り返す

		D3DXCreateTextureFromFile(pDevice, apTextureMeshWall[nCntMeshWall], &g_apTextureMeshWall[nCntMeshWall]);
	}

	// メッシュウォールの設定
	SetMeshWall(D3DXVECTOR3(0.0f, 0.0f, 800.0f),  D3DXVECTOR3(0.0f, D3DXToRadian(0),   0.0f), 1600.0f, 200.0f, 8, 1);
	SetMeshWall(D3DXVECTOR3(800.0f, 0.0f, 0.0f),  D3DXVECTOR3(0.0f, D3DXToRadian(90),  0.0f), 1600.0f, 200.0f, 8, 1);
	SetMeshWall(D3DXVECTOR3(0.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, D3DXToRadian(180), 0.0f), 1600.0f, 200.0f, 8, 1);
	SetMeshWall(D3DXVECTOR3(-800.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DXToRadian(270), 0.0f), 1600.0f, 200.0f, 8, 1);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * g_nNeedVtxWall,	// 必要頂点数
		D3DUSAGE_WRITEONLY,					// 使用方法
		FVF_VERTEX_3D,						// 頂点フォーマット
		D3DPOOL_MANAGED,					// メモリの指定
		&g_pVtxBuffMeshWall,				// 頂点バッファへのポインタ
		NULL
	);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer
	( // 引数
		sizeof(WORD) * g_nNeedIdxWall,		// 必要インデックス数
		D3DUSAGE_WRITEONLY,					// 使用方法
		D3DFMT_INDEX16,						// インデックスバッファのフォーマット
		D3DPOOL_MANAGED,					// メモリの指定
		&g_pIdxBuffMeshWall,				// インデックスバッファへのポインタ
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMeshWall = 0; nCntMeshWall < MAX_MESHWALL; nCntMeshWall++)
	{ // メッシュウォールの最大表示数分繰り返す

		if (g_aMeshWall[nCntMeshWall].bUse == true)
		{ // メッシュウォールが使用されている場合

			for (int nCntHeight = 0; nCntHeight < g_aMeshWall[nCntMeshWall].nPartHeight + 1; nCntHeight++)
			{ // 縦の分割数 +1回繰り返す

				for (int nCntWidth = 0; nCntWidth < g_aMeshWall[nCntMeshWall].nPartWidth + 1; nCntWidth++)
				{ // 横の分割数 +1回繰り返す

					// 頂点座標の設定
					pVtx[0].pos = D3DXVECTOR3
					( // 引数
						nCntWidth * (g_aMeshWall[nCntMeshWall].fWidth / (float)g_aMeshWall[nCntMeshWall].nPartWidth) - (g_aMeshWall[nCntMeshWall].fWidth * 0.5f),	// x
						-(nCntHeight * (g_aMeshWall[nCntMeshWall].fHeight / (float)g_aMeshWall[nCntMeshWall].nPartHeight)) + g_aMeshWall[nCntMeshWall].fHeight,		// y
						0.0f																																		// z
					);

					// 法線ベクトルの設定
					pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

					// 頂点カラーの設定
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(1.0f * (nCntWidth % 2), 1.0f * nCntHeight);

					// 頂点データのポインタを 1つ分進める
					pVtx += 1;
				}
			}
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffMeshWall->Unlock();

	//------------------------------------------------------------------------------------------------------------------
	//	インデックス情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshWall->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntMeshWall = 0; nCntMeshWall < MAX_MESHWALL; nCntMeshWall++)
	{ // メッシュウォールの最大表示数分繰り返す

		if (g_aMeshWall[nCntMeshWall].bUse == true)
		{ // メッシュウォールが使用されている場合

			for (int nCntHeight = 0; nCntHeight < g_aMeshWall[nCntMeshWall].nPartHeight; nCntHeight++)
			{ // 縦の分割数 +1回繰り返す

				for (int nCntWidth = 0; nCntWidth < g_aMeshWall[nCntMeshWall].nPartWidth + 1; nCntWidth++)
				{ // 横の分割数 +1回繰り返す

					pIdx[0] = nNumVtx + ((g_aMeshWall[nCntMeshWall].nPartWidth + 1) * (nCntHeight + 1) + nCntWidth);
					pIdx[1] = nNumVtx + ((g_aMeshWall[nCntMeshWall].nPartWidth + 1) * nCntHeight + nCntWidth);

					// インデックスデータのポインタを 2つ分進める
					pIdx += 2;
				}

				if (nCntHeight != g_aMeshWall[nCntMeshWall].nPartHeight - 1)
				{ // 一番手前の分割場所ではない場合

					pIdx[0] = nNumVtx + ((g_aMeshWall[nCntMeshWall].nPartWidth + 1) * nCntHeight + g_aMeshWall[nCntMeshWall].nPartWidth);
					pIdx[1] = nNumVtx + ((g_aMeshWall[nCntMeshWall].nPartWidth + 1) * (nCntHeight + 2));

					// インデックスデータのポインタを 2つ分進める
					pIdx += 2;
				}
			}

			// 頂点バッファの開始地点を必要数分ずらす
			nNumVtx += g_aMeshWall[nCntMeshWall].nNumVtx;
		}
	}

	// インデックスバッファをアンロックする
	g_pIdxBuffMeshWall->Unlock();
}

//======================================================================================================================
//	メッシュウォールの終了処理
//======================================================================================================================
void UninitMeshWall(void)
{
	// テクスチャの破棄
	for (int nCntMeshWall = 0; nCntMeshWall < TEXTURE_MESHWALL_MAX; nCntMeshWall++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTextureMeshWall[nCntMeshWall] != NULL)
		{ // 変数 (g_apTextureMeshWall) がNULLではない場合

			g_apTextureMeshWall[nCntMeshWall]->Release();
			g_apTextureMeshWall[nCntMeshWall] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffMeshWall != NULL)
	{ // 変数 (g_pVtxBuffMeshWall) がNULLではない場合

		g_pVtxBuffMeshWall->Release();
		g_pVtxBuffMeshWall = NULL;
	}

	// インデックスバッファの破棄
	if (g_pIdxBuffMeshWall != NULL)
	{ // 変数 (g_pIdxBuffMeshWall) がNULLではない場合

		g_pIdxBuffMeshWall->Release();
		g_pIdxBuffMeshWall = NULL;
	}
}

//======================================================================================================================
//	メッシュウォールの更新処理
//======================================================================================================================
void UpdateMeshWall(void)
{

}

//======================================================================================================================
//	メッシュウォールの描画処理
//======================================================================================================================
void DrawMeshWall(void)
{
	// 変数を宣言
	int        nNumIdx = 0;						// インデックス数の計測用
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	for (int nCntMeshWall = 0; nCntMeshWall < MAX_MESHWALL; nCntMeshWall++)
	{ // メッシュウォールの最大表示数分繰り返す

		if (g_aMeshWall[nCntMeshWall].bUse == true)
		{ // メッシュウォールが使用されている場合

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMeshWall[nCntMeshWall].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshWall[nCntMeshWall].rot.y, g_aMeshWall[nCntMeshWall].rot.x, g_aMeshWall[nCntMeshWall].rot.z);
			D3DXMatrixMultiply(&g_aMeshWall[nCntMeshWall].mtxWorld, &g_aMeshWall[nCntMeshWall].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aMeshWall[nCntMeshWall].pos.x, g_aMeshWall[nCntMeshWall].pos.y, g_aMeshWall[nCntMeshWall].pos.z);
			D3DXMatrixMultiply(&g_aMeshWall[nCntMeshWall].mtxWorld, &g_aMeshWall[nCntMeshWall].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshWall[nCntMeshWall].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMeshWall, 0, sizeof(VERTEX_3D));

			// インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffMeshWall);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureMeshWall[TEXTURE_MESHWALL_NORMAL]);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive
			( // 引数
				D3DPT_TRIANGLESTRIP,					// プリミティブの種類
				0,
				0,
				g_aMeshWall[nCntMeshWall].nNumVtx,		// 使用する頂点数
				nNumIdx,								// インデックスバッファの開始地点
				g_aMeshWall[nCntMeshWall].nNumIdx - 2	// プリミティブ (ポリゴン) 数
			);

			// インデックスバッファの開始地点を必要数分ずらす
			nNumIdx += g_aMeshWall[nCntMeshWall].nNumIdx;
		}
	}
}

//======================================================================================================================
//	メッシュウォールの設定処理
//======================================================================================================================
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nPartWidth, int nPartHeight)
{
	for (int nCntMeshWall = 0; nCntMeshWall < MAX_MESHWALL; nCntMeshWall++)
	{ // メッシュウォールの最大表示数分繰り返す

		if (g_aMeshWall[nCntMeshWall].bUse == false)
		{ // メッシュウォールが使用されていない場合

			// 引数を代入
			g_aMeshWall[nCntMeshWall].pos         = pos;			// 位置
			g_aMeshWall[nCntMeshWall].rot         = rot;			// 向き
			g_aMeshWall[nCntMeshWall].fWidth      = fWidth;			// 横幅
			g_aMeshWall[nCntMeshWall].fHeight     = fHeight;		// 縦幅
			g_aMeshWall[nCntMeshWall].nPartWidth  = nPartWidth;		// 横の分割数
			g_aMeshWall[nCntMeshWall].nPartHeight = nPartHeight;	// 縦の分割数

			// 使用している状態にする
			g_aMeshWall[nCntMeshWall].bUse = true;

			// 頂点バッファとインデックスバッファの必要数を設定
			g_aMeshWall[nCntMeshWall].nNumVtx = (nPartWidth + 1) * (nPartHeight + 1);
			g_aMeshWall[nCntMeshWall].nNumIdx = (nPartWidth + 1) * (((nPartHeight + 1) * 2) - 2) + (nPartHeight * 2) - 2;

			// 頂点バッファとインデックスバッファの総数を加算
			g_nNeedVtxWall += g_aMeshWall[nCntMeshWall].nNumVtx;
			g_nNeedIdxWall += g_aMeshWall[nCntMeshWall].nNumIdx;

			// 処理を抜ける
			break;
		}
	}
}

//======================================================================================================================
//	メッシュウォールの取得処理
//======================================================================================================================
MeshWall *GetMeshWall(void)
{
	// メッシュウォールの情報の先頭アドレスを返す
	return &g_aMeshWall[0];
}