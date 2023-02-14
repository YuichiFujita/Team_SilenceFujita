//======================================================================================================================
//
//	がれき処理 [junk.cpp]
//	Author：小原立暉
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "calculation.h"
#include "input.h"
#include "model.h"

#include "junk.h"
#include "shadow.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define JUNK_GRAVITY		(-0.8f)		// がれきの重力

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
Junk g_aJunk[MAX_JUNK];			// がれきの情報

//======================================================================================================================
//	がれきの初期化処理
//======================================================================================================================
void InitJunk(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

												// がれきの情報の初期化
	for (int nCntJunk = 0; nCntJunk < MAX_JUNK; nCntJunk++)
	{ // がれきの最大表示数分繰り返す

		// 基本情報の初期化
		g_aJunk[nCntJunk].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_aJunk[nCntJunk].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
		g_aJunk[nCntJunk].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
		g_aJunk[nCntJunk].nShadowID = NONE_SHADOW;					// 影のインデックス
		g_aJunk[nCntJunk].nLandCount = 0;							// 着地カウント
		g_aJunk[nCntJunk].bUse = false;								// 使用状況

		// モデル情報の初期化
		g_aJunk[nCntJunk].modelData.dwNumMat = 0;					// マテリアルの数
		g_aJunk[nCntJunk].modelData.pTexture = NULL;				// テクスチャへのポインタ
		g_aJunk[nCntJunk].modelData.pMesh = NULL;					// メッシュ (頂点情報) へのポインタ
		g_aJunk[nCntJunk].modelData.pBuffMat = NULL;				// マテリアルへのポインタ
		g_aJunk[nCntJunk].modelData.vtxMin = INIT_VTX_MIN;			// 最小の頂点座標
		g_aJunk[nCntJunk].modelData.vtxMax = INIT_VTX_MAX;			// 最大の頂点座標
		g_aJunk[nCntJunk].modelData.size = INIT_SIZE;				// 大きさ
		g_aJunk[nCntJunk].modelData.fRadius = 0.0f;					// 半径

		// マテリアルのコピーを初期化
		g_aJunk[nCntJunk].matCopy[MAX_MATERIAL] = {};
	}

	//// 当たり判定のセットアップ
	//TxtSetCollision();

	//// 影の半径のセットアップ
	//TxtSetShadow();
}

//======================================================================================================================
//	がれきの終了処理
//======================================================================================================================
void UninitJunk(void)
{

}

//======================================================================================================================
//	がれきの更新処理
//======================================================================================================================
void UpdateJunk(void)
{
	for (int nCntJunk = 0; nCntJunk < MAX_JUNK; nCntJunk++)
	{ // がれきの最大表示数分繰り返す

		if (g_aJunk[nCntJunk].bUse == true)
		{ // がれきが使用されている場合

			// 重力をかける
			g_aJunk[nCntJunk].move.y += JUNK_GRAVITY;

			// 移動させる
			g_aJunk[nCntJunk].pos += g_aJunk[nCntJunk].move;

			if (g_aJunk[nCntJunk].pos.y <= 0.0f)
			{ // 地面より下に行った場合

				// 位置を補正する
				g_aJunk[nCntJunk].pos.y = 0.0f;

				// 移動量を
				g_aJunk[nCntJunk].move.y *= -0.4f;

				// 上陸カウントを加算する
				g_aJunk[nCntJunk].nLandCount++;
			}
		}
	}
}

//======================================================================================================================
//	がれきの描画処理
//======================================================================================================================
void DrawJunk(void)
{
	// 変数を宣言
	D3DXMATRIX   mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ

	for (int nCntJunk = 0; nCntJunk < MAX_JUNK; nCntJunk++)
	{ // がれきの最大表示数分繰り返す

		if (g_aJunk[nCntJunk].bUse == true)
		{ // がれきが使用されている場合

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aJunk[nCntJunk].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aJunk[nCntJunk].rot.y, g_aJunk[nCntJunk].rot.x, g_aJunk[nCntJunk].rot.z);
			D3DXMatrixMultiply(&g_aJunk[nCntJunk].mtxWorld, &g_aJunk[nCntJunk].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aJunk[nCntJunk].pos.x, g_aJunk[nCntJunk].pos.y, g_aJunk[nCntJunk].pos.z);
			D3DXMatrixMultiply(&g_aJunk[nCntJunk].mtxWorld, &g_aJunk[nCntJunk].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aJunk[nCntJunk].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = &g_aJunk[nCntJunk].matCopy[0];

			for (int nCntMat = 0; nCntMat < (int)g_aJunk[nCntJunk].modelData.dwNumMat; nCntMat++)
			{ // マテリアルの数分繰り返す

				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// 通常

				// テクスチャの設定
				pDevice->SetTexture(0, g_aJunk[nCntJunk].modelData.pTexture[nCntMat]);

				//if (g_aJunk[nCntJunk].scale != NONE_SCALE)
				//{ // 拡大率が変更されている場合

				//	// 頂点法線の自動正規化を有効にする
				//	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
				//}

				// モデルの描画
				g_aJunk[nCntJunk].modelData.pMesh->DrawSubset(nCntMat);

				// 頂点法線の自動正規化を無効にする
				pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
			}

			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	がれきの設定処理
//======================================================================================================================
void SetJunk(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// ポインタを宣言
	D3DXMATERIAL *pMatModel;	// マテリアルデータへのポインタ

	for (int nCntJunk = 0; nCntJunk < MAX_JUNK; nCntJunk++)
	{ // がれきの最大表示数分繰り返す

		if (g_aJunk[nCntJunk].bUse == false)
		{ // がれきが使用されていない場合

			// 引数を代入
			g_aJunk[nCntJunk].pos = pos;					// 位置
			g_aJunk[nCntJunk].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
			g_aJunk[nCntJunk].rot = rot;					// 向き
			g_aJunk[nCntJunk].nLandCount = 0;				// 着地カウント
			g_aJunk[nCntJunk].bUse = true;					// 使用している状態にする

			// モデル情報を設定
			g_aJunk[nCntJunk].modelData = GetModelData(MODELTYPE_OBJECT_JUNK + FROM_OBJECT);	// モデル情報

			// マテリアルデータへのポインタを取得
			pMatModel = (D3DXMATERIAL*)g_aJunk[nCntJunk].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aJunk[nCntJunk].modelData.dwNumMat; nCntMat++, pMatModel++)
			{ // マテリアルの数分繰り返す

				// マテリアルをコピーする
				g_aJunk[nCntJunk].matCopy[nCntMat] = *pMatModel;
			}

			// 影のインデックスを設定
			g_aJunk[nCntJunk].nShadowID = SetCircleShadow
			( // 引数
				0.5f,									// α値
				g_aJunk[nCntJunk].modelData.fRadius,	// 半径
				&g_aJunk[nCntJunk].nShadowID,			// 影の親の影インデックス
				&g_aJunk[nCntJunk].bUse					// 影の親の使用状況
			);

			// 影の位置設定
			SetPositionShadow(g_aJunk[nCntJunk].nShadowID, g_aJunk[nCntJunk].pos, g_aJunk[nCntJunk].rot, NONE_SCALE);

			// 処理を抜ける
			break;
		}
	}
}

#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================
//**********************************************************************************************************************
//	がれきの総数取得処理
//**********************************************************************************************************************
int GetNumJunk(void)
{
	// 変数を宣言
	int nNumJunk = 0;	// がれきの総数の確認用

	for (int nCntJunk = 0; nCntJunk < MAX_JUNK; nCntJunk++)
	{ // がれきの最大表示数分繰り返す

		if (g_aJunk[nCntJunk].bUse == true)
		{ // がれきが使用されている場合

			// カウンターを加算
			nNumJunk++;
		}
	}

	// 変数の値を返す
	return nNumJunk;	// がれきの総数
}
#endif