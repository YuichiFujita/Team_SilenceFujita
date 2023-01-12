//======================================================================================================================
//
//	オブジェクト処理 [object.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "input.h"
#include "model.h"

#include "object.h"
#include "particle.h"
#include "shadow.h"
#include "sound.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define OBJ_LIFE	(50)		// オブジェクトの体力

#define DAMAGE_TIME_OBJ		(20)					// ダメージ状態を保つ時間
#define UNR_TIME_OBJ		(DAMAGE_TIME_OBJ - 10)	// 無敵状態に変更する時間

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
Object g_aObject[MAX_OBJECT];	// オブジェクトの情報

//======================================================================================================================
//	オブジェクトの初期化処理
//======================================================================================================================
void InitObject(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// オブジェクトの情報の初期化
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // オブジェクトの最大表示数分繰り返す

		g_aObject[nCntObject].pos           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_aObject[nCntObject].rot           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
		g_aObject[nCntObject].scale         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 拡大率
		g_aObject[nCntObject].pModelData    = NULL;									// モデル情報
		g_aObject[nCntObject].state         = ACTIONSTATE_NONE;						// 状態
		g_aObject[nCntObject].nLife         = 0;									// 体力
		g_aObject[nCntObject].nBreakType    = BREAKTYPE_NONE;						// 壊れ方の種類
		g_aObject[nCntObject].nType         = 0;									// オブジェクトの種類
		g_aObject[nCntObject].nCounterState = 0; 									// 状態管理カウンター
		g_aObject[nCntObject].nShadowID     = NONE_SHADOW;							// 影のインデックス
		g_aObject[nCntObject].bUse          = false;								// 使用状況
	}
}

//======================================================================================================================
//	オブジェクトの終了処理
//======================================================================================================================
void UninitObject(void)
{

}

//======================================================================================================================
//	オブジェクトの更新処理
//======================================================================================================================
void UpdateObject(void)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aObject[nCntObject].bUse == true)
		{ // オブジェクトが使用されている場合

			switch (g_aObject[nCntObject].nBreakType)
			{ // 壊れ方の種類ごとの処理
			case BREAKTYPE_NONE:	// 壊れない種類

				// 無し

				// 処理を抜ける
				break;

			case BREAKTYPE_ON:		// 壊れる種類

				if (g_aObject[nCntObject].nCounterState > 0)
				{ // カウンターが 0より大きい場合

					// カウンターを減算
					g_aObject[nCntObject].nCounterState--;

					if (g_aObject[nCntObject].nCounterState == UNR_TIME_OBJ)
					{ // カウンターが一定値の場合

						// 無敵状態にする
						g_aObject[nCntObject].state = ACTIONSTATE_UNRIVALED;
					}
					else if (g_aObject[nCntObject].nCounterState <= 0)
					{ // カウンターが 0以下の場合

						// 通常状態にする
						g_aObject[nCntObject].state = ACTIONSTATE_NORMAL;
					}
				}

				// 処理を抜ける
				break;
			}
		}
	}
}

//======================================================================================================================
//	オブジェクトの描画処理
//======================================================================================================================
void DrawObject(void)
{
	// 変数を宣言
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ
	D3DXMATERIAL      blackMat;					// マテリアルデータ (黄＋黒)

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aObject[nCntObject].bUse == true)
		{ // オブジェクトが使用されている場合

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aObject[nCntObject].mtxWorld);

			//拡大率を反映
			D3DXMatrixScaling(&mtxScale, g_aObject[nCntObject].scale.x, g_aObject[nCntObject].scale.y, g_aObject[nCntObject].scale.z);
			D3DXMatrixMultiply(&g_aObject[nCntObject].mtxWorld, &g_aObject[nCntObject].mtxWorld, &mtxScale);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aObject[nCntObject].rot.y, g_aObject[nCntObject].rot.x, g_aObject[nCntObject].rot.z);
			D3DXMatrixMultiply(&g_aObject[nCntObject].mtxWorld, &g_aObject[nCntObject].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aObject[nCntObject].pos.x, g_aObject[nCntObject].pos.y, g_aObject[nCntObject].pos.z);
			D3DXMatrixMultiply(&g_aObject[nCntObject].mtxWorld, &g_aObject[nCntObject].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aObject[nCntObject].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aObject[nCntObject].pModelData->pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aObject[nCntObject].pModelData->dwNumMat; nCntMat++)
			{ // マテリアルの数分繰り返す

				switch (g_aObject[nCntObject].state)
				{ // 状態ごとの処理
				case ACTIONSTATE_DAMAGE:	// ダメージ状態

					// 読み込んだマテリアルを代入
					blackMat.MatD3D = pMat[nCntMat].MatD3D;

					// 拡散光・環境光・自己発光を赤にする
					blackMat.MatD3D.Diffuse  = D3DXCOLOR(0.3f, 0.3f, 0.0f, 1.0f);
					blackMat.MatD3D.Ambient  = D3DXCOLOR(0.3f, 0.3f, 0.0f, 1.0f);
					blackMat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

					// マテリアルの設定
					pDevice->SetMaterial(&blackMat.MatD3D);			// 黄＋黒

					// 処理を抜ける
					break;

				default:					// それ以外の状態

					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// 通常

					// 処理を抜ける
					break;
				}

				// テクスチャの設定
				pDevice->SetTexture(0, g_aObject[nCntObject].pModelData->pTexture[nCntMat]);

				// モデルの描画
				g_aObject[nCntObject].pModelData->pMesh->DrawSubset(nCntMat);
			}

			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	オブジェクトの設定処理
//======================================================================================================================
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXMATERIAL *pMat, int nBreakType, int nType)
{
	// ポインタを宣言
	D3DXMATERIAL *pMatModel;		// マテリアルデータへのポインタ

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aObject[nCntObject].bUse == false)
		{ // オブジェクトが使用されていない場合

			// 引数を代入
			g_aObject[nCntObject].pos        = pos;			// 位置
			g_aObject[nCntObject].rot        = rot;			// 向き
			g_aObject[nCntObject].scale      = scale;		// 拡大率
			g_aObject[nCntObject].nBreakType = nBreakType;	// 壊れ方の種類
			g_aObject[nCntObject].nType      = nType;		// オブジェクトの種類

			// オブジェクトの情報を初期化
			g_aObject[nCntObject].state         = ACTIONSTATE_NORMAL;	// 状態
			g_aObject[nCntObject].nLife         = OBJ_LIFE;				// 体力
			g_aObject[nCntObject].nCounterState = 0; 					// 状態管理カウンター

			// 使用している状態にする
			g_aObject[nCntObject].bUse = true;

			// モデル情報を設定
			g_aObject[nCntObject].pModelData = GetModelData(nType + FROM_OBJECT);	// モデル情報

			// 最大値を反映する
			g_aObject[nCntObject].pModelData->vtxMax.x *= g_aObject[nCntObject].scale.x;
			g_aObject[nCntObject].pModelData->vtxMax.y *= g_aObject[nCntObject].scale.y;
			g_aObject[nCntObject].pModelData->vtxMax.z *= g_aObject[nCntObject].scale.z;

			// 最小値を反映する
			g_aObject[nCntObject].pModelData->vtxMin.x *= g_aObject[nCntObject].scale.x;
			g_aObject[nCntObject].pModelData->vtxMin.y *= g_aObject[nCntObject].scale.y;
			g_aObject[nCntObject].pModelData->vtxMin.z *= g_aObject[nCntObject].scale.z;

			// マテリアルデータへのポインタを取得
			pMatModel = (D3DXMATERIAL*)g_aObject[nCntObject].pModelData->pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aObject[nCntObject].pModelData->dwNumMat; nCntMat++)
			{ // マテリアルの数分繰り返す

				// 引数の色を代入
				pMatModel[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Ambient;
				pMatModel[nCntMat].MatD3D.Diffuse = pMat[nCntMat].MatD3D.Diffuse;
			}

			// 影のインデックスを設定
			g_aObject[nCntObject].nShadowID = SetShadow
			( // 引数
				0.5f,																																// α値
				fabsf(g_aObject[g_aObject[nCntObject].nType].pModelData->vtxMax.x - g_aObject[g_aObject[nCntObject].nType].pModelData->vtxMin.x),	// 半径
				&g_aObject[nCntObject].nShadowID,																									// 影の親の影インデックス
				&g_aObject[nCntObject].bUse																											// 影の親の使用状況
			);

			// 影の位置設定
			SetPositionShadow(g_aObject[nCntObject].nShadowID, g_aObject[nCntObject].pos);

			// 処理を抜ける
			break;
		}
	}
}

#if 0
//======================================================================================================================
//	オブジェクトのダメージ判定
//======================================================================================================================
void HitObject(Object *pObject, int nDamage)
{
	if (pObject->state == ACTIONSTATE_NORMAL)
	{ // オブジェクトが通常状態の場合

		// 引数のダメージ分を体力から減算
		pObject->nLife -= nDamage;

		if (pObject->nLife > 0)
		{ // 体力が残っている場合

			// ダメージ状態にする
			pObject->state = ACTIONSTATE_DAMAGE;

			// パーティクルの設定
			SetParticle
			( // 引数
				pObject->pos,						// 位置
				D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f),	// 色
				PARTICLETYPE_DAMAGE,				// 種類
				SPAWN_PARTICLE_DAMAGE,				// エフェクト数
				2									// 寿命
			);

			// カウンターを設定
			pObject->nCounterState = DAMAGE_TIME_OBJ;

			// サウンドの再生
			PlaySound(SOUND_LABEL_SE_DMG);			// SE (ダメージ)
		}
		else
		{ // 体力が尽きた場合

			// 爆発の設定
			SetExplosion(pObject->pos, SOUNDTYPE_BREAK);

			// パーティクルの設定
			SetParticle
			( // 引数
				pObject->pos,						// 位置
				D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f),	// 色
				PARTICLETYPE_EXPLOSION,				// 種類
				SPAWN_PARTICLE_EXPLOSION,			// エフェクト数
				2									// 寿命
			);

			// アイテムの設定
			SetItem(pObject->pos, ITEMTYPE_HEAL);

			// 使用していない状態にする
			pObject->bUse = false;
		}
	}
}
#endif

//======================================================================================================================
//	オブジェクトとの当たり判定
//======================================================================================================================
void CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pOldPos, float fWidth, float fDepth)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aObject[nCntObject].bUse == true)
		{ // オブジェクトが使用されている場合

			// 前後の当たり判定
			if (pPos->x + fWidth > g_aObject[nCntObject].pos.x + g_aObject[nCntObject].pModelData->vtxMin.x
			&&  pPos->x - fWidth < g_aObject[nCntObject].pos.x + g_aObject[nCntObject].pModelData->vtxMax.x)
			{ // ブロックの左右の範囲内の場合

				if (pPos->z    + fDepth >  g_aObject[nCntObject].pos.z + g_aObject[nCntObject].pModelData->vtxMin.z
				&&  pOldPos->z + fDepth <= g_aObject[nCntObject].pos.z + g_aObject[nCntObject].pModelData->vtxMin.z)
				{ // 前からの当たり判定

					// 位置を補正
					pPos->z = g_aObject[nCntObject].pos.z + g_aObject[nCntObject].pModelData->vtxMin.z - fDepth - 0.01f;
				}
				else if (pPos->z    - fDepth <  g_aObject[nCntObject].pos.z + g_aObject[nCntObject].pModelData->vtxMax.z
				     &&  pOldPos->z - fDepth >= g_aObject[nCntObject].pos.z + g_aObject[nCntObject].pModelData->vtxMax.z)
				{ // 後ろからの当たり判定

					// 位置を補正
					pPos->z = g_aObject[nCntObject].pos.z + g_aObject[nCntObject].pModelData->vtxMax.z + fDepth + 0.01f;
				}
			}

			// 左右の当たり判定
			if (pPos->z + fDepth > g_aObject[nCntObject].pos.z + g_aObject[nCntObject].pModelData->vtxMin.z
			&&  pPos->z - fDepth < g_aObject[nCntObject].pos.z + g_aObject[nCntObject].pModelData->vtxMax.z)
			{ // ブロックの前後の範囲内の場合

				if (pPos->x    + fWidth >  g_aObject[nCntObject].pos.x + g_aObject[nCntObject].pModelData->vtxMin.x
				&&  pOldPos->x + fWidth <= g_aObject[nCntObject].pos.x + g_aObject[nCntObject].pModelData->vtxMin.x)
				{ // 左からの当たり判定

					// 位置を補正
					pPos->x = g_aObject[nCntObject].pos.x + g_aObject[nCntObject].pModelData->vtxMin.x - fWidth - 0.01f;
				}
				else if (pPos->x    - fWidth <  g_aObject[nCntObject].pos.x + g_aObject[nCntObject].pModelData->vtxMax.x
				     &&  pOldPos->x - fWidth >= g_aObject[nCntObject].pos.x + g_aObject[nCntObject].pModelData->vtxMax.x)
				{ // 右からの当たり判定
					
					// 位置を補正
					pPos->x = g_aObject[nCntObject].pos.x + g_aObject[nCntObject].pModelData->vtxMax.x + fWidth + 0.01f;
				}
			}
		}
	}
}

//======================================================================================================================
//	オブジェクトの取得処理
//======================================================================================================================
Object *GetObjectData(void)
{
	// オブジェクトの情報の先頭アドレスを返す
	return &g_aObject[0];
}

#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================
//**********************************************************************************************************************
//	オブジェクトの総数取得処理
//**********************************************************************************************************************
int GetNumObject(void)
{
	// 変数を宣言
	int nNumObject = 0;	// オブジェクトの総数の確認用

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aObject[nCntObject].bUse == true)
		{ // オブジェクトが使用されている場合

			// カウンターを加算
			nNumObject++;
		}
	}

	// 変数の値を返す
	return nNumObject;	// オブジェクトの総数
}
#endif