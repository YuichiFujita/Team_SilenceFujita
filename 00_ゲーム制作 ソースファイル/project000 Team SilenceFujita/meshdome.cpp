//======================================================================================================================
//
//	メッシュドーム処理 [meshdome.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "meshdome.h"
#include "weather.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_MESHDOME		(1)		// メッシュドームの最大数

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTextureMeshDome[] =	// テクスチャの相対パス
{
	"data\\TEXTURE\\sky000.png",	// メッシュドームのテクスチャの相対パス
};

//**********************************************************************************************************************
//	列挙型定義 (TEXTURE_MESHDOME)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_MESHDOME_NORMAL = 0,	// メッシュドーム (通常)
	TEXTURE_MESHDOME_MAX,			// この列挙型の総数
} TEXTURE_MESHDOME;

//**********************************************************************************************************************
//	構造体定義 (MeshDome)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き
	D3DXMATRIX  mtxWorld;		// ワールドマトリックス
	float       fRadius;		// 半径
	int         nPartWidth;		// 横の分割数
	int         nPartHeight;	// 縦の分割数
	int         nNumVtx;		// 必要頂点数
	int         nNumIdx;		// 必要インデックス数
	bool        bUse;			// 使用状況
} MeshDome;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void SetMeshDome(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, int nPartWidth, int nPartHeight);	// メッシュドームの設定処理
void TxtSetMeshDome(void);																			// メッシュドームのセットアップ処理

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureMeshDome[TEXTURE_MESHDOME_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshDome = NULL;						// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9  g_pIdxBuffMeshDome = NULL;						// インデックスバッファへのポインタ

MeshDome g_aMeshDome[MAX_MESHDOME];	// メッシュドームの情報
int      g_nNeedVtxDome;			// 必要頂点数
int      g_nNeedIdxDome;			// 必要インデックス数

//======================================================================================================================
//	メッシュドームの初期化処理
//======================================================================================================================
void InitMeshDome(void)
{
	// 変数を宣言
	D3DXVECTOR3 vecPos;					// 頂点位置の計算用
	D3DXVECTOR3 vecNor;					// 法線ベクトルの計算用
	float       fRotWidth, fRotHeight;	// 頂点位置の方向の計算用
	int         nNumVtx = 0;			// 頂点数の計測用
	WEATHERTYPE weather = GetWeather();	// 天気

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ
	WORD      *pIdx;							// インデックス情報へのポインタ
	
	// グローバル変数の初期化
	g_nNeedVtxDome = 0;	// 必要頂点の総数
	g_nNeedIdxDome = 0;	// 必要インデックスの総数

	// メッシュドームの情報の初期化
	for (int nCntMeshDome = 0; nCntMeshDome < MAX_MESHDOME; nCntMeshDome++)
	{ // メッシュドームの最大表示数分繰り返す

		g_aMeshDome[nCntMeshDome].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aMeshDome[nCntMeshDome].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き

		g_aMeshDome[nCntMeshDome].fRadius     = 0.0f;	// 半径
		g_aMeshDome[nCntMeshDome].nPartWidth  = 0;		// 横の分割数
		g_aMeshDome[nCntMeshDome].nPartHeight = 0;		// 縦の分割数
		g_aMeshDome[nCntMeshDome].nNumVtx     = 0;		// 必要頂点数
		g_aMeshDome[nCntMeshDome].nNumIdx     = 0;		// 必要インデックス数
		g_aMeshDome[nCntMeshDome].bUse        = false;	// 使用状況
	}

	// テクスチャの読み込み
	for (int nCntMeshDome = 0; nCntMeshDome < TEXTURE_MESHDOME_MAX; nCntMeshDome++)
	{ // 使用するテクスチャ数分繰り返す

		D3DXCreateTextureFromFile(pDevice, apTextureMeshDome[nCntMeshDome], &g_apTextureMeshDome[nCntMeshDome]);
	}

	// メッシュドームのセットアップ
	TxtSetMeshDome();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * g_nNeedVtxDome,	// 必要頂点数
		D3DUSAGE_WRITEONLY,					// 使用方法
		FVF_VERTEX_3D,						// 頂点フォーマット
		D3DPOOL_MANAGED,					// メモリの指定
		&g_pVtxBuffMeshDome,				// 頂点バッファへのポインタ
		NULL
	);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer
	( // 引数
		sizeof(WORD) * g_nNeedIdxDome,		// 必要インデックス数
		D3DUSAGE_WRITEONLY,					// 使用方法
		D3DFMT_INDEX16,						// インデックスバッファのフォーマット
		D3DPOOL_MANAGED,					// メモリの指定
		&g_pIdxBuffMeshDome,				// インデックスバッファへのポインタ
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshDome->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMeshDome = 0; nCntMeshDome < MAX_MESHDOME; nCntMeshDome++)
	{ // メッシュドームの最大表示数分繰り返す

		if (g_aMeshDome[nCntMeshDome].bUse == true)
		{ // メッシュドームが使用されている場合

			for (int nCntHeight = 0; nCntHeight < g_aMeshDome[nCntMeshDome].nPartHeight + 1; nCntHeight++)
			{ // 縦の分割数 +1回繰り返す

				for (int nCntWidth = 0; nCntWidth < g_aMeshDome[nCntMeshDome].nPartWidth + 1; nCntWidth++)
				{ // 横の分割数 +1回繰り返す

					// 頂点の向きを計算
					fRotWidth  = nCntWidth  * ((D3DX_PI * 2.0f) / g_aMeshDome[nCntMeshDome].nPartWidth);
					fRotHeight = nCntHeight * ((D3DX_PI * 0.5f) / g_aMeshDome[nCntMeshDome].nPartHeight);

					// 頂点座標の方向を設定
					vecPos = D3DXVECTOR3
					( // 引数
						g_aMeshDome[nCntMeshDome].fRadius * sinf(fRotHeight) * sinf(fRotWidth),	// x
						g_aMeshDome[nCntMeshDome].fRadius * cosf(fRotHeight),					// y
						g_aMeshDome[nCntMeshDome].fRadius * sinf(fRotHeight) * cosf(fRotWidth)	// z
					);

					// 頂点座標の設定
					pVtx[0].pos = g_aMeshDome[nCntMeshDome].pos + vecPos;

					// 法線ベクトルの方向を設定
					vecNor = -vecPos;

					// ベクトルを正規化
					D3DXVec3Normalize(&vecNor, &vecNor);

					// 法線ベクトルの設定
					pVtx[0].nor = vecNor;

					switch (weather)
					{ // 天気ごとの処理
					case WEATHERTYPE_SUNNY:		// 晴れ

						// 頂点カラーの設定
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						break;	// 抜け出す

					case WEATHERTYPE_RAIN:		// 雨

						// 頂点カラーの設定
						pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

						break;	// 抜け出す

					case WEATHERTYPE_SNOW:		// 雪

						// 頂点カラーの設定
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						break;	// 抜け出す

					case WEATHERTYPE_THUNDER:	// 雷雨

						// 頂点カラーの設定
						pVtx[0].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

						break;	// 抜け出す

					default:	// 例外処理
						assert(false);
						break;
					}

					// テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2
					( // 引数
						nCntWidth  * 1.0f / (float)g_aMeshDome[nCntMeshDome].nPartWidth,	// u
						nCntHeight * 1.0f / (float)g_aMeshDome[nCntMeshDome].nPartHeight	// v
					);

					// 頂点データのポインタを 1つ分進める
					pVtx += 1;
				}
			}
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffMeshDome->Unlock();

	//------------------------------------------------------------------------------------------------------------------
	//	インデックス情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshDome->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntMeshDome = 0; nCntMeshDome < MAX_MESHDOME; nCntMeshDome++)
	{ // メッシュドームの最大表示数分繰り返す

		if (g_aMeshDome[nCntMeshDome].bUse == true)
		{ // メッシュドームが使用されている場合

			for (int nCntHeight = 0, nCntWidth = 0; nCntHeight < g_aMeshDome[nCntMeshDome].nPartHeight; nCntHeight++)
			{ // 縦の分割数 +1回繰り返す

				for (nCntWidth = 0; nCntWidth < g_aMeshDome[nCntMeshDome].nPartWidth + 1; nCntWidth++)
				{ // 横の分割数 +1回繰り返す

					pIdx[0] = nNumVtx + ((g_aMeshDome[nCntMeshDome].nPartWidth + 1) * (nCntHeight + 1) + nCntWidth);
					pIdx[1] = nNumVtx + ((g_aMeshDome[nCntMeshDome].nPartWidth + 1) * nCntHeight + nCntWidth);

					// インデックスデータのポインタを 2つ分進める
					pIdx += 2;
				}

				if (nCntHeight != g_aMeshDome[nCntMeshDome].nPartHeight - 1)
				{ // 一番手前の分割場所ではない場合

					pIdx[0] = nNumVtx + ((g_aMeshDome[nCntMeshDome].nPartWidth + 1) * nCntHeight + nCntWidth - 1);
					pIdx[1] = nNumVtx + ((g_aMeshDome[nCntMeshDome].nPartWidth + 1) * (nCntHeight + 2));

					// インデックスデータのポインタを 2つ分進める
					pIdx += 2;
				}
			}

			// 頂点バッファの開始地点を必要数分ずらす
			nNumVtx += g_aMeshDome[nCntMeshDome].nNumVtx;
		}
	}

	// インデックスバッファをアンロックする
	g_pIdxBuffMeshDome->Unlock();
}

//======================================================================================================================
//	メッシュドームの終了処理
//======================================================================================================================
void UninitMeshDome(void)
{
	// テクスチャの破棄
	for (int nCntMeshDome = 0; nCntMeshDome < TEXTURE_MESHDOME_MAX; nCntMeshDome++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTextureMeshDome[nCntMeshDome] != NULL)
		{ // 変数 (g_apTextureMeshDome) がNULLではない場合

			g_apTextureMeshDome[nCntMeshDome]->Release();
			g_apTextureMeshDome[nCntMeshDome] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffMeshDome != NULL)
	{ // 変数 (g_pVtxBuffMeshDome) がNULLではない場合

		g_pVtxBuffMeshDome->Release();
		g_pVtxBuffMeshDome = NULL;
	}

	// インデックスバッファの破棄
	if (g_pIdxBuffMeshDome != NULL)
	{ // 変数 (g_pIdxBuffMeshDome) がNULLではない場合

		g_pIdxBuffMeshDome->Release();
		g_pIdxBuffMeshDome = NULL;
	}
}

//======================================================================================================================
//	メッシュドームの更新処理
//======================================================================================================================
void UpdateMeshDome(void)
{

}

//======================================================================================================================
//	メッシュドームの描画処理
//======================================================================================================================
void DrawMeshDome(void)
{
	// 変数を宣言
	int        nNumIdx = 0;						// インデックス数の計測用
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntMeshDome = 0; nCntMeshDome < MAX_MESHDOME; nCntMeshDome++)
	{ // メッシュドームの最大表示数分繰り返す

		if (g_aMeshDome[nCntMeshDome].bUse == true)
		{ // メッシュドームが使用されている場合

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMeshDome[nCntMeshDome].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshDome[nCntMeshDome].rot.y, g_aMeshDome[nCntMeshDome].rot.x, g_aMeshDome[nCntMeshDome].rot.z);
			D3DXMatrixMultiply(&g_aMeshDome[nCntMeshDome].mtxWorld, &g_aMeshDome[nCntMeshDome].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aMeshDome[nCntMeshDome].pos.x, g_aMeshDome[nCntMeshDome].pos.y, g_aMeshDome[nCntMeshDome].pos.z);
			D3DXMatrixMultiply(&g_aMeshDome[nCntMeshDome].mtxWorld, &g_aMeshDome[nCntMeshDome].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshDome[nCntMeshDome].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMeshDome, 0, sizeof(VERTEX_3D));

			// インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffMeshDome);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureMeshDome[TEXTURE_MESHDOME_NORMAL]);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive
			( // 引数
				D3DPT_TRIANGLESTRIP,					// プリミティブの種類
				0,
				0,
				g_aMeshDome[nCntMeshDome].nNumVtx,		// 使用する頂点数
				nNumIdx,								// インデックスバッファの開始地点
				g_aMeshDome[nCntMeshDome].nNumIdx - 2	// プリミティブ (ポリゴン) 数
			);

			// インデックスバッファの開始地点を必要数分ずらす
			nNumIdx += g_aMeshDome[nCntMeshDome].nNumIdx;
		}
	}

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//======================================================================================================================
//	メッシュドームの設定処理
//======================================================================================================================
void SetMeshDome(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, int nPartWidth, int nPartHeight)
{
	for (int nCntMeshDome = 0; nCntMeshDome < MAX_MESHDOME; nCntMeshDome++)
	{ // メッシュドームの最大表示数分繰り返す

		if (g_aMeshDome[nCntMeshDome].bUse == false)
		{ // メッシュドームが使用されていない場合

			// 引数を代入
			g_aMeshDome[nCntMeshDome].pos         = pos;			// 位置
			g_aMeshDome[nCntMeshDome].rot         = rot;			// 向き
			g_aMeshDome[nCntMeshDome].fRadius     = fRadius;		// 半径
			g_aMeshDome[nCntMeshDome].nPartWidth  = nPartWidth;		// 横の分割数
			g_aMeshDome[nCntMeshDome].nPartHeight = nPartHeight;	// 縦の分割数

			// 使用している状態にする
			g_aMeshDome[nCntMeshDome].bUse = true;

			// 頂点バッファとインデックスバッファの必要数を設定
			g_aMeshDome[nCntMeshDome].nNumVtx = (nPartWidth + 1) * (nPartHeight + 1);
			g_aMeshDome[nCntMeshDome].nNumIdx = (nPartWidth + 1) * (((nPartHeight + 1) * 2) - 2) + (nPartHeight * 2) - 2;

			// 頂点バッファとインデックスバッファの総数を加算
			g_nNeedVtxDome += g_aMeshDome[nCntMeshDome].nNumVtx;
			g_nNeedIdxDome += g_aMeshDome[nCntMeshDome].nNumIdx;

			// 処理を抜ける
			break;
		}
	}
}

//======================================================================================================================
//	メッシュドームのセットアップ処理
//======================================================================================================================
void TxtSetMeshDome(void)
{
	// 変数を宣言
	D3DXVECTOR3 pos;			// 位置の代入用
	D3DXVECTOR3 rot;			// 向きの代入用
	float       fRadius;		// 半径の代入用
	int         nPartWidth;		// 横の分割数の代入用
	int         nPartHeight;	// 縦の分割数の代入用
	int         nEnd;			// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;				// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = (GetMode() == MODE_TUTORIAL) ? fopen(TUTORIAL_STAGE_SETUP_TXT, "r") : fopen(GAME_STAGE_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "STAGE_MESHDOMESET") == 0)
			{ // 読み込んだ文字列が STAGE_MESHDOMESET の場合

				do
				{ // 読み込んだ文字列が END_STAGE_MESHDOMESET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "MESHDOMESET") == 0)
					{ // 読み込んだ文字列が MESHDOMESET の場合

						do
						{ // 読み込んだ文字列が END_MESHDOMESET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "POS") == 0)
							{ // 読み込んだ文字列が POS の場合
								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &pos.x);		// X座標を読み込む
								fscanf(pFile, "%f", &pos.y);		// Y座標を読み込む
								fscanf(pFile, "%f", &pos.z);		// Z座標を読み込む
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // 読み込んだ文字列が ROT の場合
								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &rot.x);		// X向きを読み込む
								fscanf(pFile, "%f", &rot.y);		// Y向きを読み込む
								fscanf(pFile, "%f", &rot.z);		// Z向きを読み込む
							}
							else if (strcmp(&aString[0], "RADIUS") == 0)
							{ // 読み込んだ文字列が RADIUS の場合
								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &fRadius);		// 半径を読み込む
							}
							else if (strcmp(&aString[0], "PARTWIDTH") == 0)
							{ // 読み込んだ文字列が PARTWIDTH の場合
								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nPartWidth);	// 横の分割数を読み込む
							}
							else if (strcmp(&aString[0], "PARTHEIGHT") == 0)
							{ // 読み込んだ文字列が PARTHEIGHT の場合
								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nPartHeight);	// 縦の分割数を読み込む
							}

						} while (strcmp(&aString[0], "END_MESHDOMESET") != 0);	// 読み込んだ文字列が END_MESHDOMESET ではない場合ループ

						// メッシュフィールドの設定
						SetMeshDome(pos, D3DXToRadian(rot), fRadius, nPartWidth, nPartHeight);
					}
				} while (strcmp(&aString[0], "END_STAGE_MESHDOMESET") != 0);	// 読み込んだ文字列が END_STAGE_MESHDOMESET ではない場合ループ
			}
		} while (nEnd != EOF);	// 読み込んだ文字列が EOF ではない場合ループ
		
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "ステージファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}