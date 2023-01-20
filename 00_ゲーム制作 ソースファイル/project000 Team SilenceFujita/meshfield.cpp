//======================================================================================================================
//
//	メッシュフィールド処理 [meshfield.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "calculation.h"
#include "meshfield.h"
#include "player.h"

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTextureMeshField[] =		// テクスチャの相対パス
{
	"data\\TEXTURE\\road000.png",		// 直線のテクスチャの相対パス
	"data\\TEXTURE\\road001.png",		// 曲がり角度のテクスチャの相対パス
	"data\\TEXTURE\\road002.png",		// 交差点のテクスチャの相対パス
	"data\\TEXTURE\\road003.png",		// 丁字路のテクスチャの相対パス
	"data\\TEXTURE\\sidewalk000.png",	// 歩道のテクスチャの相対パス
};

//**********************************************************************************************************************
//	列挙型定義 (TEXTURE_MESHFIELD)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_MESHFIELD_ROAD_LINE = 0,	// (道路) 直線
	TEXTURE_MESHFIELD_ROAD_TURN,		// (道路) 曲がり角度
	TEXTURE_MESHFIELD_ROAD_CROSS,		// (道路) 交差点
	TEXTURE_MESHFIELD_ROAD_TJUNC,		// (道路) 丁字路
	TEXTURE_MESHFIELD_SIDEWALK,			// 歩道
	TEXTURE_MESHFIELD_MAX,				// この列挙型の総数
} TEXTURE_MESHFIELD;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nPartWidth, int nPartHeight, int nType);	// メッシュフィールドの設定処理
void TxtSetMeshField(void);																										// メッシュフィールドのセットアップ処理

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureMeshField[TEXTURE_MESHFIELD_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;							// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9  g_pIdxBuffMeshField = NULL;							// インデックスバッファへのポインタ

MeshField g_aMeshField[MAX_MESHFIELD];			// メッシュフィールドの情報
int       g_nNeedVtxField;						// 必要頂点数
int       g_nNeedIdxField;						// 必要インデックス数

//======================================================================================================================
//	メッシュフィールドの初期化処理
//======================================================================================================================
void InitMeshField(void)
{
	// 変数を宣言
	int nNumVtx = 0;							// 頂点数の計測用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ
	WORD      *pIdx;							// インデックス情報へのポインタ

	// グローバル変数の初期化
	g_nNeedVtxField = 0;						// 必要頂点の総数
	g_nNeedIdxField = 0;						// 必要インデックスの総数

	// メッシュフィールドの情報の初期化
	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++)
	{ // メッシュフィールドの最大表示数分繰り返す

		g_aMeshField[nCntMeshField].pos         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aMeshField[nCntMeshField].rot         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
		g_aMeshField[nCntMeshField].fWidth      = 0.0f;								// 横幅
		g_aMeshField[nCntMeshField].fHeight     = 0.0f;								// 縦幅
		g_aMeshField[nCntMeshField].nPartWidth  = 0;								// 横の分割数
		g_aMeshField[nCntMeshField].nPartHeight = 0;								// 縦の分割数
		g_aMeshField[nCntMeshField].nNumVtx     = 0;								// 必要頂点数
		g_aMeshField[nCntMeshField].nNumIdx     = 0;								// 必要インデックス数
		g_aMeshField[nCntMeshField].nType       = 0;								// 種類
		g_aMeshField[nCntMeshField].bUse        = false;							// 使用状況
	}

	// テクスチャの読み込み
	for (int nCntMeshField = 0; nCntMeshField < TEXTURE_MESHFIELD_MAX; nCntMeshField++)
	{ // 使用するテクスチャ数分繰り返す

		D3DXCreateTextureFromFile(pDevice, apTextureMeshField[nCntMeshField], &g_apTextureMeshField[nCntMeshField]);
	}

	// メッシュフィールドのセットアップ
	TxtSetMeshField();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * g_nNeedVtxField,	// 必要頂点数
		D3DUSAGE_WRITEONLY,						// 使用方法
		FVF_VERTEX_3D,							// 頂点フォーマット
		D3DPOOL_MANAGED,						// メモリの指定
		&g_pVtxBuffMeshField,					// 頂点バッファへのポインタ
		NULL
	);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer
	( // 引数
		sizeof(WORD) * g_nNeedIdxField,			// 必要インデックス数
		D3DUSAGE_WRITEONLY,						// 使用方法
		D3DFMT_INDEX16,							// インデックスバッファのフォーマット
		D3DPOOL_MANAGED,						// メモリの指定
		&g_pIdxBuffMeshField,					// インデックスバッファへのポインタ
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++)
	{ // メッシュフィールドの最大表示数分繰り返す

		if (g_aMeshField[nCntMeshField].bUse == true)
		{ // メッシュフィールドが使用されている場合

			for (int nCntHeight = 0; nCntHeight < g_aMeshField[nCntMeshField].nPartHeight + 1; nCntHeight++)
			{ // 縦の分割数 +1回繰り返す

				for (int nCntWidth = 0; nCntWidth < g_aMeshField[nCntMeshField].nPartWidth + 1; nCntWidth++)
				{ // 横の分割数 +1回繰り返す

					// 頂点座標の設定
					pVtx[0].pos = D3DXVECTOR3
					( // 引数
						nCntWidth * (g_aMeshField[nCntMeshField].fWidth / (float)g_aMeshField[nCntMeshField].nPartWidth) - (g_aMeshField[nCntMeshField].fWidth * 0.5f),			// x
						0.0f,																																					// y
						-(nCntHeight * (g_aMeshField[nCntMeshField].fHeight / (float)g_aMeshField[nCntMeshField].nPartHeight) - (g_aMeshField[nCntMeshField].fHeight * 0.5f))	// z
					);

					// 法線ベクトルの設定
					pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
	g_pVtxBuffMeshField->Unlock();

	//------------------------------------------------------------------------------------------------------------------
	//	インデックス情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++)
	{ // メッシュフィールドの最大表示数分繰り返す

		if (g_aMeshField[nCntMeshField].bUse == true)
		{ // メッシュフィールドが使用されている場合

			for (int nCntHeight = 0, nCntWidth = 0; nCntHeight < g_aMeshField[nCntMeshField].nPartHeight; nCntHeight++)
			{ // 縦の分割数 +1回繰り返す

				for (nCntWidth = 0; nCntWidth < g_aMeshField[nCntMeshField].nPartWidth + 1; nCntWidth++)
				{ // 横の分割数 +1回繰り返す

					pIdx[0] = nNumVtx + ((g_aMeshField[nCntMeshField].nPartWidth + 1) * (nCntHeight + 1) + nCntWidth);
					pIdx[1] = nNumVtx + ((g_aMeshField[nCntMeshField].nPartWidth + 1) * nCntHeight + nCntWidth);

					// インデックスデータのポインタを 2つ分進める
					pIdx += 2;
				}

				if (nCntHeight != g_aMeshField[nCntMeshField].nPartHeight - 1)
				{ // 一番手前の分割場所ではない場合

					pIdx[0] = nNumVtx + ((g_aMeshField[nCntMeshField].nPartWidth + 1) * nCntHeight + nCntWidth - 1);
					pIdx[1] = nNumVtx + ((g_aMeshField[nCntMeshField].nPartWidth + 1) * (nCntHeight + 2));

					// インデックスデータのポインタを 2つ分進める
					pIdx += 2;
				}
			}

			// 頂点バッファの開始地点を必要数分ずらす
			nNumVtx += g_aMeshField[nCntMeshField].nNumVtx;
		}
	}

	// インデックスバッファをアンロックする
	g_pIdxBuffMeshField->Unlock();
}

//======================================================================================================================
//	メッシュフィールドの終了処理
//======================================================================================================================
void UninitMeshField(void)
{
	// テクスチャの破棄
	for (int nCntMeshField = 0; nCntMeshField < TEXTURE_MESHFIELD_MAX; nCntMeshField++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTextureMeshField[nCntMeshField] != NULL)
		{ // 変数 (g_apTextureMeshField) がNULLではない場合

			g_apTextureMeshField[nCntMeshField]->Release();
			g_apTextureMeshField[nCntMeshField] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffMeshField != NULL)
	{ // 変数 (g_pVtxBuffMeshField) がNULLではない場合

		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	// インデックスバッファの破棄
	if (g_pIdxBuffMeshField != NULL)
	{ // 変数 (g_pIdxBuffMeshField) がNULLではない場合

		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//======================================================================================================================
//	メッシュフィールドの更新処理
//======================================================================================================================
void UpdateMeshField(void)
{

}

//======================================================================================================================
//	メッシュフィールドの描画処理
//======================================================================================================================
void DrawMeshField(void)
{
	// 変数を宣言
	int        nNumIdx = 0;						// インデックス数の計測用
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++)
	{ // メッシュフィールドの最大表示数分繰り返す

		if (g_aMeshField[nCntMeshField].bUse == true)
		{ // メッシュフィールドが使用されている場合

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMeshField[nCntMeshField].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshField[nCntMeshField].rot.y, g_aMeshField[nCntMeshField].rot.x, g_aMeshField[nCntMeshField].rot.z);
			D3DXMatrixMultiply(&g_aMeshField[nCntMeshField].mtxWorld, &g_aMeshField[nCntMeshField].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aMeshField[nCntMeshField].pos.x, g_aMeshField[nCntMeshField].pos.y, g_aMeshField[nCntMeshField].pos.z);
			D3DXMatrixMultiply(&g_aMeshField[nCntMeshField].mtxWorld, &g_aMeshField[nCntMeshField].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshField[nCntMeshField].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

			// インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffMeshField);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureMeshField[g_aMeshField[nCntMeshField].nType]);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive
			( // 引数
				D3DPT_TRIANGLESTRIP,					// プリミティブの種類
				0,
				0,
				g_aMeshField[nCntMeshField].nNumVtx,	// 使用する頂点数
				nNumIdx,								// インデックスバッファの開始地点
				g_aMeshField[nCntMeshField].nNumIdx - 2	// プリミティブ (ポリゴン) 数
			);

			// インデックスバッファの開始地点を必要数分ずらす
			nNumIdx += g_aMeshField[nCntMeshField].nNumIdx;
		}
	}
}

//======================================================================================================================
//	メッシュフィールドの設定処理
//======================================================================================================================
void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nPartWidth, int nPartHeight, int nType)
{
	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++)
	{ // メッシュフィールドの最大表示数分繰り返す

		if (g_aMeshField[nCntMeshField].bUse == false)
		{ // メッシュフィールドが使用されていない場合

			// 引数を代入
			g_aMeshField[nCntMeshField].pos         = pos;			// 位置
			g_aMeshField[nCntMeshField].rot         = rot;			// 向き
			g_aMeshField[nCntMeshField].fWidth      = fWidth;		// 横幅
			g_aMeshField[nCntMeshField].fHeight     = fHeight;		// 縦幅
			g_aMeshField[nCntMeshField].nPartWidth  = nPartWidth;	// 横の分割数
			g_aMeshField[nCntMeshField].nPartHeight = nPartHeight;	// 縦の分割数
			g_aMeshField[nCntMeshField].nType       = nType;		// 種類

			// 使用している状態にする
			g_aMeshField[nCntMeshField].bUse = true;

			// 頂点バッファとインデックスバッファの必要数を設定
			g_aMeshField[nCntMeshField].nNumVtx = (nPartWidth + 1) * (nPartHeight + 1);
			g_aMeshField[nCntMeshField].nNumIdx = (nPartWidth + 1) * (((nPartHeight + 1) * 2) - 2) + (nPartHeight * 2) - 2;

			// 頂点バッファとインデックスバッファの総数を加算
			g_nNeedVtxField += g_aMeshField[nCntMeshField].nNumVtx;
			g_nNeedIdxField += g_aMeshField[nCntMeshField].nNumIdx;

			// 処理を抜ける
			break;
		}
	}
}

//======================================================================================================================
//	メッシュフィールドとの当たり判定
//======================================================================================================================
float CollisionMeshField(D3DXVECTOR3 pos)
{
	// 変数を宣言
	float fLandPosY = GetLimitStage().fField;		// 着地予定の y座標

	// 変数配列を宣言
	D3DXVECTOR3 vexPos[4];	// 頂点位置 ([※] 0：右上　1：左上　2：左下　3：右下)

	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++)
	{ // メッシュフィールドの最大表示数分繰り返す

		if (g_aMeshField[nCntMeshField].bUse == true)
		{ // メッシュフィールドが使用されている場合

			// 四頂点の位置の計算
			VecSizePos
			( // 引数
				&vexPos[0],
				g_aMeshField[nCntMeshField].pos,	// 絶対座標
				g_aMeshField[nCntMeshField].rot,	// 向き
				g_aMeshField[nCntMeshField].fWidth,	// 横幅 / 2
				g_aMeshField[nCntMeshField].fHeight	// 縦幅 / 2
			);

			if (LineOuterProduct(vexPos[0], vexPos[1], pos) < 0
			&&  LineOuterProduct(vexPos[1], vexPos[2], pos) < 0
			&&  LineOuterProduct(vexPos[2], vexPos[3], pos) < 0
			&&  LineOuterProduct(vexPos[3], vexPos[0], pos) < 0)
			{ // 四辺の内側にいる場合 (当たっている場合)

				if (fLandPosY < g_aMeshField[nCntMeshField].pos.y)
				{ // 現在の着地点よりメッシュフィールドの方が上の場合

					// 現在の着地点にメッシュフィールドの座標を設定
					fLandPosY = g_aMeshField[nCntMeshField].pos.y;
				}
			}
		}
	}

	// 着地する y座標を返す
	return fLandPosY;
}

//======================================================================================================================
//	メッシュフィールドの取得処理
//======================================================================================================================
MeshField *GetMeshField(void)
{
	// メッシュフィールドの情報の先頭アドレスを返す
	return &g_aMeshField[0];
}

//======================================================================================================================
//	メッシュフィールドのセットアップ処理
//======================================================================================================================
void TxtSetMeshField(void)
{
	// 変数を宣言
	D3DXVECTOR3 pos;			// 位置の代入用
	D3DXVECTOR3 rot;			// 向きの代入用
	float       fWidth;			// 横幅の代入用
	float       fHeight;		// 縦幅の代入用
	int         nPartWidth;		// 横の分割数の代入用
	int         nPartHeight;	// 縦の分割数の代入用
	int         nType;			// 種類の代入用
	int         nEnd;			// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;				// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(STAGE_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "STAGE_MESHFIELDSET") == 0)
			{ // 読み込んだ文字列が STAGE_MESHFIELDSET の場合

				do
				{ // 読み込んだ文字列が END_STAGE_MESHFIELDSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "MESHFIELDSET") == 0)
					{ // 読み込んだ文字列が MESHFIELDSET の場合

						do
						{ // 読み込んだ文字列が END_MESHFIELDSET ではない場合ループ

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
							else if (strcmp(&aString[0], "WIDTH") == 0)
							{ // 読み込んだ文字列が WIDTH の場合
								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &fWidth);		// 横幅を読み込む
							}
							else if (strcmp(&aString[0], "HEIGHT") == 0)
							{ // 読み込んだ文字列が HEIGHT の場合
								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &fHeight);		// 縦幅を読み込む
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
							else if (strcmp(&aString[0], "TYPE") == 0)
							{ // 読み込んだ文字列が TYPE の場合
								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nType);		// 種類を読み込む
							}

						} while (strcmp(&aString[0], "END_MESHFIELDSET") != 0);	// 読み込んだ文字列が END_MESHFIELDSET ではない場合ループ

						// メッシュフィールドの設定
						SetMeshField(pos, D3DXToRadian(rot), fWidth, fHeight, nPartWidth, nPartHeight, nType);
					}
				} while (strcmp(&aString[0], "END_STAGE_MESHFIELDSET") != 0);	// 読み込んだ文字列が END_STAGE_MESHFIELDSET ではない場合ループ
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