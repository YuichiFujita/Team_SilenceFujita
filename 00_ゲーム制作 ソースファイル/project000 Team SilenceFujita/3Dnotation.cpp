//============================================================
//
//	強調表示処理 [3Dnotation.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "3Dnotation.h"
#include "model.h"
#include "calculation.h"
#include "tutorial.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MAX_3D_NOTATION		(128)		// 強調表示の最大数
#define NOTA_MOVE_ROT		(0.02f)		// 強調表示の回転量

//************************************************************
//	構造体定義 (3DNotation)
//************************************************************
typedef struct
{
	D3DXVECTOR3  pos;					// 位置
	D3DXVECTOR3  rot;					// 向き
	D3DXMATRIX   mtxWorld;				// ワールドマトリックス
	NOTATIONTYPE type;					// 種類
	Model        modelData;				// モデル情報
	int         *p3DNotationIDParent;	// 親の強調表示インデックス
	bool        *pUseParent;			// 親の使用状況
	bool         bUse;					// 使用状況
} Notation3D;

//************************************************************
//	プロトタイプ宣言
//************************************************************
void MoveNotation(Notation3D *pNota);					// 強調表示の動作の更新処理
void FalseNotation(Notation3D *pNota, int nCntNota);	// 強調表示の削除の更新処理

//************************************************************
//	グローバル変数
//************************************************************
Notation3D g_a3DNotation[MAX_3D_NOTATION];		// 強調表示の情報
float      g_fNotationRot;						// 強調表示の向き

//============================================================
//	強調表示の初期化処理
//============================================================
void Init3DNotation(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// 強調表示の向きを初期化
	g_fNotationRot = D3DX_PI;

	// 強調表示の情報の初期化
	for (int nCntNota = 0; nCntNota < MAX_3D_NOTATION; nCntNota++)
	{ // 強調表示の最大表示数分繰り返す

		// 基本情報を初期化
		g_a3DNotation[nCntNota].pos                 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_a3DNotation[nCntNota].rot                 = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);	// 向き
		g_a3DNotation[nCntNota].type                = NOTATIONTYPE_BREAK;				// 種類
		g_a3DNotation[nCntNota].p3DNotationIDParent = NULL;								// 親の強調表示インデックス
		g_a3DNotation[nCntNota].pUseParent          = NULL;								// 親の使用状況
		g_a3DNotation[nCntNota].bUse                = false;							// 使用状況

		// モデル情報の初期化
		g_a3DNotation[nCntNota].modelData.dwNumMat = 0;				// マテリアルの数
		g_a3DNotation[nCntNota].modelData.pTexture = NULL;			// テクスチャへのポインタ
		g_a3DNotation[nCntNota].modelData.pMesh    = NULL;			// メッシュ (頂点情報) へのポインタ
		g_a3DNotation[nCntNota].modelData.pBuffMat = NULL;			// マテリアルへのポインタ
		g_a3DNotation[nCntNota].modelData.dwNumMat = 0;				// マテリアルの数
		g_a3DNotation[nCntNota].modelData.vtxMin   = INIT_VTX_MIN;	// 最小の頂点座標
		g_a3DNotation[nCntNota].modelData.vtxMax   = INIT_VTX_MAX;	// 最大の頂点座標
		g_a3DNotation[nCntNota].modelData.size     = INIT_SIZE;		// 大きさ
		g_a3DNotation[nCntNota].modelData.fRadius  = 0.0f;			// 半径
	}
}

//============================================================
//	強調表示の終了処理
//============================================================
void Uninit3DNotation(void)
{

}

//============================================================
//	強調表示の更新処理
//============================================================
void Update3DNotation(void)
{
	// 向きを加算
	g_fNotationRot += NOTA_MOVE_ROT;

	// 向きを正規化
	RotNormalize(&g_fNotationRot);

	for (int nCntNota = 0; nCntNota < MAX_3D_NOTATION; nCntNota++)
	{ // 強調表示の最大表示数分繰り返す

		if (g_a3DNotation[nCntNota].bUse == true)
		{ // 強調表示が使用されている場合

			// 強調表示の動作の更新
			MoveNotation(&g_a3DNotation[nCntNota]);

			// 強調表示の削除の更新
			FalseNotation(&g_a3DNotation[nCntNota], nCntNota);
		}
	}
}

//============================================================
//	強調表示の描画処理
//============================================================
void Draw3DNotation(void)
{
	// 変数を宣言
	D3DXMATRIX   mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	bool         bDrawExit = false;				// Exit表示の有無

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ

	switch (GetMode())
	{ // モードごとの処理
	case MODE_TUTORIAL:	// チュートリアル

		if (GetLessonState() >= LESSON_07)
		{ // レッスン7に挑戦中、またはクリアしている場合

			// Exitを表示状態にする
			bDrawExit = true;
		}

		// 処理を抜ける
		break;

	case MODE_GAME:		// ゲーム

		// Exitを表示状態にする
		bDrawExit = true;

		// 処理を抜ける
		break;
	}

	for (int nCntNota = 0; nCntNota < MAX_3D_NOTATION; nCntNota++)
	{ // 強調表示の最大表示数分繰り返す

		if (g_a3DNotation[nCntNota].bUse == true)
		{ // 強調表示が使用されている場合

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_a3DNotation[nCntNota].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_a3DNotation[nCntNota].rot.y, g_a3DNotation[nCntNota].rot.x, g_a3DNotation[nCntNota].rot.z);
			D3DXMatrixMultiply(&g_a3DNotation[nCntNota].mtxWorld, &g_a3DNotation[nCntNota].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_a3DNotation[nCntNota].pos.x, g_a3DNotation[nCntNota].pos.y, g_a3DNotation[nCntNota].pos.z);
			D3DXMatrixMultiply(&g_a3DNotation[nCntNota].mtxWorld, &g_a3DNotation[nCntNota].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_a3DNotation[nCntNota].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_a3DNotation[nCntNota].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_a3DNotation[nCntNota].modelData.dwNumMat; nCntMat++)
			{ // マテリアルの数分繰り返す

				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_a3DNotation[nCntNota].modelData.pTexture[nCntMat]);

				switch (g_a3DNotation[nCntNota].type)
				{ // 種類ごとの処理
				case NOTATIONTYPE_EXIT:	// 脱出

					if (bDrawExit)
					{ // 脱出が表示状態の場合

						// モデルの描画
						g_a3DNotation[nCntNota].modelData.pMesh->DrawSubset(nCntMat);
					}

					// 処理を抜ける
					break;

				default:				// 上記以外

					// モデルの描画
					g_a3DNotation[nCntNota].modelData.pMesh->DrawSubset(nCntMat);

					// 処理を抜ける
					break;
				}
			}

			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//============================================================
//	強調表示の設定処理
//============================================================
int Set3DNotation(NOTATIONTYPE type, int *p3DNotationID, bool *pUse)
{
	for (int nCntNota = 0; nCntNota < MAX_3D_NOTATION; nCntNota++)
	{ // 強調表示の最大表示数分繰り返す

		if (g_a3DNotation[nCntNota].bUse == false)
		{ // 強調表示が使用されていない場合

			// 種類を設定
			g_a3DNotation[nCntNota].type = type;

			// 引数のアドレスを代入
			g_a3DNotation[nCntNota].p3DNotationIDParent = p3DNotationID;	// 親の強調表示インデックス
			g_a3DNotation[nCntNota].pUseParent          = pUse;				// 親の使用状況

			// 情報を初期化
			g_a3DNotation[nCntNota].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
			g_a3DNotation[nCntNota].rot = D3DXVECTOR3(0.0f, g_fNotationRot, 0.0f);	// 向き

			switch (type)
			{ // 引数の種類ごとの処理
			case NOTATIONTYPE_BREAK:	// 破壊

				// モデルの情報を設定
				g_a3DNotation[nCntNota].modelData = GetModelData(MODELTYPE_FONT_BREAK);

				// 処理を抜ける
				break;

			case NOTATIONTYPE_SHOT:		// 射出

				// モデルの情報を設定
				g_a3DNotation[nCntNota].modelData = GetModelData(MODELTYPE_FONT_SHOT);

				// 処理を抜ける
				break;

			case NOTATIONTYPE_FLY:		// 飛翔

				// モデルの情報を設定
				g_a3DNotation[nCntNota].modelData = GetModelData(MODELTYPE_FONT_FLY);

				// 処理を抜ける
				break;

			case NOTATIONTYPE_EXIT:		// 脱出

				// モデルの情報を設定
				g_a3DNotation[nCntNota].modelData = GetModelData(MODELTYPE_FONT_EXIT);

				// 処理を抜ける
				break;
			}

			// 使用している状態にする
			g_a3DNotation[nCntNota].bUse = true;

			// 使用できる強調表示のIDを返す
			return nCntNota;
		}
	}

	// 強調表示の総数が足りない場合 -1を返す
	return -1;
}

//============================================================
//	強調表示の位置の設定処理
//============================================================
void SetPosition3DNotation(int n3DNotationID, D3DXVECTOR3 pos)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	if (n3DNotationID > NONE_3D_NOTATION)
	{ // 使用できるIDの場合

		// 引数の強調表示の位置を設定
		g_a3DNotation[n3DNotationID].pos = pos;
	}
}

//============================================================
//	強調表示の動作の更新処理
//============================================================
void MoveNotation(Notation3D *pNota)
{
	// 向きを代入
	pNota->rot.y = g_fNotationRot;
}

//============================================================
//	強調表示の削除の更新処理
//============================================================
void FalseNotation(Notation3D *pNota, int nCntNota)
{
	if (pNota->pUseParent != NULL)
	{ // 親の使用状況アドレスが設定されている場合

		if (*pNota->pUseParent == false)
		{ // 親が使用されていない場合

			// 強調表示を使用していない状態にする
			pNota->bUse = false;
		}
		else
		{ // 親が使用されている場合

			if (pNota->p3DNotationIDParent != NULL)
			{ // 親の強調表示インデックスアドレスが設定されている場合

				if (*pNota->p3DNotationIDParent != nCntNota)
				{ // 親の強調表示インデックスと、現在の強調表示インデックスの値が違う場合

					// 強調表示を使用していない状態にする
					pNota->bUse = false;

					// [※] 使用しない状態になった親が同フレーム内で同じインデックスが使われて
					//      また使用された場合に強調表示が一つの親に二つ生成されてしまうため、その防止。
				}
			}
		}
	}
}