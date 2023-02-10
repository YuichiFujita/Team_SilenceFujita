//======================================================================================================================
//
//	ゲート処理 [gate.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "calculation.h"
#include "input.h"
#include "model.h"

#include "gate.h"
#include "shadow.h"
#include "sound.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define GATE_GRAVITY		(1.0f)		// 重力

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
Gate      g_aGate[MAX_GATE];			// オブジェクトの情報
Collision g_aCollGate[MODEL_GATE_MAX];	// 当たり判定の情報

//======================================================================================================================
//	オブジェクトの初期化処理
//======================================================================================================================
void InitGate(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// オブジェクトの情報の初期化
	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // オブジェクトの最大表示数分繰り返す

		// 基本情報の初期化
		g_aGate[nCntGate].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_aGate[nCntGate].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
		g_aGate[nCntGate].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
		g_aGate[nCntGate].state = GATESTATE_FLY;					// 状態
		g_aGate[nCntGate].bUse = false;								// 使用状況

		// 当たり判定情報の初期化
		for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
		{ // 当たり判定の最大数分繰り返す

			g_aGate[nCntGate].collInfo.vecPos[nCntColl] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置ベクトル
			g_aGate[nCntGate].collInfo.fWidth[nCntColl] = 0.0f;								// 横幅
			g_aGate[nCntGate].collInfo.fDepth[nCntColl] = 0.0f;								// 奥行
		}

		// モデル情報の初期化
		g_aGate[nCntGate].modelData.dwNumMat = 0;					// マテリアルの数
		g_aGate[nCntGate].modelData.pTexture = NULL;				// テクスチャへのポインタ
		g_aGate[nCntGate].modelData.pMesh = NULL;					// メッシュ (頂点情報) へのポインタ
		g_aGate[nCntGate].modelData.pBuffMat = NULL;				// マテリアルへのポインタ
		g_aGate[nCntGate].modelData.vtxMin = INIT_VTX_MIN;			// 最小の頂点座標
		g_aGate[nCntGate].modelData.vtxMax = INIT_VTX_MAX;			// 最大の頂点座標
		g_aGate[nCntGate].modelData.size = INIT_SIZE;				// 大きさ
		g_aGate[nCntGate].modelData.fRadius = 0.0f;					// 半径
	}

	// 当たり判定の情報の初期化
	for (int nCntGate = 0; nCntGate < MODEL_GATE_MAX; nCntGate++)
	{ // ゲートの種類の総数分繰り返す

		// 当たり判定の総数を初期化
		g_aCollGate[nCntGate].nNumColl = 1;

		for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
		{ // 当たり判定の最大数分繰り返す

			g_aCollGate[nCntGate].vecPos[nCntColl] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置ベクトル
			g_aCollGate[nCntGate].scale[nCntColl]  = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// 拡大率
			g_aCollGate[nCntGate].fWidth[nCntColl] = 0.0f;							// 横幅
			g_aCollGate[nCntGate].fDepth[nCntColl] = 0.0f;							// 奥行
		}
	}

	// ゲートの設定処理
	SetGate(D3DXVECTOR3(0.0f, 300.0f, 700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//======================================================================================================================
//	オブジェクトの終了処理
//======================================================================================================================
void UninitGate(void)
{

}

//======================================================================================================================
//	オブジェクトの更新処理
//======================================================================================================================
void UpdateGate(void)
{
	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // オブジェクトの最大表示数分繰り返す

		if (GetKeyboardTrigger(DIK_RSHIFT) == true)
		{ // 右シフトキーを押した場合
			// 落下状態にする
			g_aGate[nCntGate].state = GATESTATE_LAND;
		}

		if (g_aGate[nCntGate].bUse == true)
		{ // オブジェクトが使用されている場合
			switch (g_aGate[nCntGate].state)
			{
			case GATESTATE_FLY:		// 未閉状態

				// 移動量を0にする
				g_aGate[nCntGate].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				break;				// 抜け出す

			case GATESTATE_LAND:	// 落下状態

				// 重力をかける
				g_aGate[nCntGate].move.y -= GATE_GRAVITY;

				// 位置を更新する
				g_aGate[nCntGate].pos.y += g_aGate[nCntGate].move.y;

				if (g_aGate[nCntGate].pos.y <= 0.0f)
				{ // 地面に到達した場合
					// 位置を補正する
					g_aGate[nCntGate].pos.y = 0.0f;

					// 移動量を設定する
					g_aGate[nCntGate].move = D3DXVECTOR3(0.0f, 8.0f, 0.0f);

					// バウンド状態にする
					g_aGate[nCntGate].state = GATESTATE_BOUND;
				}

				break;				// 抜け出す

			case GATESTATE_BOUND:	// バウンド状態

				// 重力をかける
				g_aGate[nCntGate].move.y -= GATE_GRAVITY;

				// 位置を更新する
				g_aGate[nCntGate].pos.y += g_aGate[nCntGate].move.y;

				if (g_aGate[nCntGate].pos.y <= 0.0f)
				{ // 地面に到達した場合
					// 位置を補正する
					g_aGate[nCntGate].pos.y = 0.0f;

					// 移動量を0にする
					g_aGate[nCntGate].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					// 停止状態にする
					g_aGate[nCntGate].state = GATESTATE_STOP;
				}

				break;				// 抜け出す

			case GATESTATE_STOP:	// 停止状態

				// 移動量を0にする
				g_aGate[nCntGate].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				break;				// 抜け出す
			}
		}
	}
}

//======================================================================================================================
//	オブジェクトの描画処理
//======================================================================================================================
void DrawGate(void)
{
	// 変数を宣言
	D3DXMATRIX   mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ

	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aGate[nCntGate].bUse == true)
		{ // オブジェクトが使用されている場合

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aGate[nCntGate].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aGate[nCntGate].rot.y, g_aGate[nCntGate].rot.x, g_aGate[nCntGate].rot.z);
			D3DXMatrixMultiply(&g_aGate[nCntGate].mtxWorld, &g_aGate[nCntGate].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aGate[nCntGate].pos.x, g_aGate[nCntGate].pos.y, g_aGate[nCntGate].pos.z);
			D3DXMatrixMultiply(&g_aGate[nCntGate].mtxWorld, &g_aGate[nCntGate].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aGate[nCntGate].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aGate[nCntGate].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aGate[nCntGate].modelData.dwNumMat; nCntMat++)
			{ // マテリアルの数分繰り返す

				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_aGate[nCntGate].modelData.pTexture[nCntMat]);

				// モデルの描画
				g_aGate[nCntGate].modelData.pMesh->DrawSubset(nCntMat);
			}

			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	オブジェクトの設定処理
//======================================================================================================================
void SetGate(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aGate[nCntGate].bUse == false)
		{ // オブジェクトが使用されていない場合

			// 引数を代入
			g_aGate[nCntGate].pos = pos;					// 位置
			g_aGate[nCntGate].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
			g_aGate[nCntGate].rot = rot;					// 向き
			g_aGate[nCntGate].state = GATESTATE_FLY;		// 状態

			// 使用している状態にする
			g_aGate[nCntGate].bUse = true;

			// モデル情報を設定
			g_aGate[nCntGate].modelData = GetModelData(MODELTYPE_OBJECT_BAKERY + FROM_OBJECT);	// モデル情報

			// 処理を抜ける
			break;
		}
	}
}

//======================================================================================================================
//	オブジェクトとの当たり判定
//======================================================================================================================
void CollisionGate(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pOldPos, D3DXVECTOR3 *pMove, float fWidth, float fDepth)
{
	// 変数を宣言
	D3DXVECTOR3 collPos;	// 当たり判定の中心座標

	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aGate[nCntGate].bUse == true)
		{ // オブジェクトが使用されている場合
			if (g_aGate[nCntGate].state != GATESTATE_FLY)
			{ // 空中状態以外の場合
				// 前後の当たり判定
				if (pPos->x + fWidth > g_aGate[nCntGate].pos.x + g_aGate[nCntGate].modelData.vtxMin.x
					&&  pPos->x - fWidth < g_aGate[nCntGate].pos.x + g_aGate[nCntGate].modelData.vtxMax.x)
				{ // ブロックの左右の範囲内の場合

					if (pPos->z + fDepth >  g_aGate[nCntGate].pos.z + g_aGate[nCntGate].modelData.vtxMin.z
						&&  pOldPos->z + fDepth <= g_aGate[nCntGate].pos.z + g_aGate[nCntGate].modelData.vtxMin.z)
					{ // 前からの当たり判定

						// 位置を補正
						pPos->z = g_aGate[nCntGate].pos.z + g_aGate[nCntGate].modelData.vtxMin.z - fDepth - 0.01f;

						// 移動量を削除
						pMove->x *= 0.95f;
					}
					else if (pPos->z - fDepth <  g_aGate[nCntGate].pos.z + g_aGate[nCntGate].modelData.vtxMax.z
						&&  pOldPos->z - fDepth >= g_aGate[nCntGate].pos.z + g_aGate[nCntGate].modelData.vtxMax.z)
					{ // 後ろからの当たり判定

						// 位置を補正
						pPos->z = g_aGate[nCntGate].pos.z + g_aGate[nCntGate].modelData.vtxMax.z + fDepth + 0.01f;

						// 移動量を削除
						pMove->x *= 0.95f;
					}
				}

				// 左右の当たり判定
				if (pPos->z + fDepth > g_aGate[nCntGate].pos.z + g_aGate[nCntGate].modelData.vtxMin.z
					&&  pPos->z - fDepth < g_aGate[nCntGate].pos.z + g_aGate[nCntGate].modelData.vtxMax.z)
				{ // ブロックの前後の範囲内の場合

					if (pPos->x + fWidth >  g_aGate[nCntGate].pos.x + g_aGate[nCntGate].modelData.vtxMin.x
						&&  pOldPos->x + fWidth <= g_aGate[nCntGate].pos.x + g_aGate[nCntGate].modelData.vtxMin.x)
					{ // 左からの当たり判定

						// 位置を補正
						pPos->x = g_aGate[nCntGate].pos.x + g_aGate[nCntGate].modelData.vtxMin.x - fWidth - 0.01f;

						// 移動量を削除
						pMove->x *= 0.95f;
					}
					else if (pPos->x - fWidth <  g_aGate[nCntGate].pos.x + g_aGate[nCntGate].modelData.vtxMax.x
						&&  pOldPos->x - fWidth >= g_aGate[nCntGate].pos.x + g_aGate[nCntGate].modelData.vtxMax.x)
					{ // 右からの当たり判定

						// 位置を補正
						pPos->x = g_aGate[nCntGate].pos.x + g_aGate[nCntGate].modelData.vtxMax.x + fWidth + 0.01f;

						// 移動量を削除
						pMove->x *= 0.95f;
					}
				}
			}
		}
	}
}

//======================================================================================================================
//	オブジェクトの取得処理
//======================================================================================================================
Gate *GetGateData(void)
{
	// オブジェクトの情報の先頭アドレスを返す
	return &g_aGate[0];
}

//======================================================================================================================
//	ゲートの当たり判定の取得処理
//======================================================================================================================
Collision *GetCollGate(void)
{
	// ゲートの当たり判定の情報の先頭アドレスを返す
	return &g_aCollGate[0];
}

#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================
//**********************************************************************************************************************
//	オブジェクトの総数取得処理
//**********************************************************************************************************************
int GetNumGate(void)
{
	// 変数を宣言
	int nNumGate = 0;	// オブジェクトの総数の確認用

	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aGate[nCntGate].bUse == true)
		{ // オブジェクトが使用されている場合

			// カウンターを加算
			nNumGate++;
		}
	}

	// 変数の値を返す
	return nNumGate;	// オブジェクトの総数
}
#endif