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
#include "calculation.h"
#include "input.h"
#include "model.h"

#include "object.h"
#include "particle.h"
#include "shadow.h"
#include "sound.h"

#ifdef _DEBUG	// デバッグ処理
#include "game.h"
#endif

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define SHADOW_SETUP_TXT	"data\\TXT\\shadow.txt"		// ステージセットアップ用のテキストファイルの相対パス

#define OBJ_LIFE			(50)						// オブジェクトの体力
#define DAMAGE_TIME_OBJ		(20)						// ダメージ状態を保つ時間
#define UNR_TIME_OBJ		(DAMAGE_TIME_OBJ - 10)		// 無敵状態に変更する時間

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void TxtSetCollision(void);		// 当たり判定のセットアップ処理
void TxtSetShadow(void);		// 影の半径のセットアップ処理

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
Object    g_aObject[MAX_OBJECT];				// オブジェクトの情報
Collision g_aCollision[MODEL_OBJ_MAX];			// 当たり判定の情報
float     g_aShadowRadius[MODEL_OBJ_MAX];		// 影の半径の情報

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

		// 基本情報の初期化
		g_aObject[nCntObject].pos            = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_aObject[nCntObject].rot            = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
		g_aObject[nCntObject].scale          = D3DXVECTOR3(1.0f, 1.0f, 1.0f);		// 拡大率
		g_aObject[nCntObject].state          = ACTIONSTATE_NONE;					// 状態
		g_aObject[nCntObject].nLife          = 0;									// 体力
		g_aObject[nCntObject].nCollisionType = COLLISIONTYPE_NONE;					// 当たり判定の種類
		g_aObject[nCntObject].nShadowType    = SHADOWTYPE_NONE;						// 影の種類
		g_aObject[nCntObject].nBreakType     = BREAKTYPE_NONE;						// 壊れ方の種類
		g_aObject[nCntObject].nType          = 0;									// オブジェクトの種類
		g_aObject[nCntObject].nCounterState  = 0; 									// 状態管理カウンター
		g_aObject[nCntObject].nShadowID      = NONE_SHADOW;							// 影のインデックス
		g_aObject[nCntObject].bUse           = false;								// 使用状況

		// 当たり判定情報の初期化
		for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
		{ // 当たり判定の最大数分繰り返す

			g_aObject[nCntObject].collInfo.vecPos[nCntColl] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置ベクトル
			g_aObject[nCntObject].collInfo.fWidth[nCntColl] = 0.0f;								// 横幅
			g_aObject[nCntObject].collInfo.fDepth[nCntColl] = 0.0f;								// 奥行
		}

		// 向き状態を初期化
		g_aObject[nCntObject].collInfo.stateRot = ROTSTATE_0;

		// モデル情報の初期化
		g_aObject[nCntObject].modelData.dwNumMat = 0;								// マテリアルの数
		g_aObject[nCntObject].modelData.pTexture = NULL;							// テクスチャへのポインタ
		g_aObject[nCntObject].modelData.pMesh    = NULL;							// メッシュ (頂点情報) へのポインタ
		g_aObject[nCntObject].modelData.pBuffMat = NULL;							// マテリアルへのポインタ
		g_aObject[nCntObject].modelData.vtxMin   = INIT_VTX_MIN;					// 最小の頂点座標
		g_aObject[nCntObject].modelData.vtxMax   = INIT_VTX_MAX;					// 最大の頂点座標
		g_aObject[nCntObject].modelData.size     = INIT_SIZE;						// 大きさ
		g_aObject[nCntObject].modelData.fRadius  = 0.0f;							// 半径

		// マテリアルのコピーを初期化
		g_aObject[nCntObject].matCopy[MAX_MATERIAL] = {};

#ifdef _DEBUG	// デバッグ処理
		// エディット時の状態
		g_aObject[nCntObject].editState = OBJECTSTATE_NONE;
#endif
	}

	// 当たり判定の情報の初期化
	for (int nCntObject = 0; nCntObject < MODEL_OBJ_MAX; nCntObject++)
	{ // オブジェクトの種類の総数分繰り返す

		// 当たり判定の総数を初期化
		g_aCollision[nCntObject].nNumColl = 1;

		for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
		{ // 当たり判定の最大数分繰り返す

			g_aCollision[nCntObject].vecPos[nCntColl] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置ベクトル
			g_aCollision[nCntObject].scale[nCntColl]  = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// 拡大率
			g_aCollision[nCntObject].fWidth[nCntColl] = 0.0f;							// 横幅
			g_aCollision[nCntObject].fDepth[nCntColl] = 0.0f;							// 奥行
		}
	}

	// 影の半径の情報の初期化
	for (int nCntObject = 0; nCntObject < MODEL_OBJ_MAX; nCntObject++)
	{ // オブジェクトの種類の総数分繰り返す

		g_aShadowRadius[nCntObject] = FIRST_RADIUS;
	}

	// 当たり判定のセットアップ
	TxtSetCollision();

	// 影の半径のセットアップ
	TxtSetShadow();
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

#ifdef _DEBUG	// デバッグ処理
	D3DXMATERIAL deleteMat;						// 削除マテリアルデータ (赤)
#endif

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aObject[nCntObject].bUse == true)
		{ // オブジェクトが使用されている場合

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aObject[nCntObject].mtxWorld);

			// 拡大率を反映
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
			pMat = &g_aObject[nCntObject].matCopy[0];

			for (int nCntMat = 0; nCntMat < (int)g_aObject[nCntObject].modelData.dwNumMat; nCntMat++)
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

#ifdef _DEBUG	// デバッグ処理
				if (GetGameMode() == GAMEMODE_EDIT)
				{ // エディットモードの場合

					switch (g_aObject[nCntObject].editState)
					{ // 状態ごとの処理
					case OBJECTSTATE_DELETETARGET:	// 削除対象

						// 拡散光・環境光・自己発光を赤にする
						deleteMat.MatD3D.Diffuse  = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						deleteMat.MatD3D.Ambient  = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						deleteMat.MatD3D.Emissive = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						// マテリアルの設定
						pDevice->SetMaterial(&deleteMat.MatD3D);		// 赤

						// 処理を抜ける
						break;

					default:						// それ以外の状態

						// マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// 通常

						// 処理を抜ける
						break;
					}
				}
#endif

				// テクスチャの設定
				pDevice->SetTexture(0, g_aObject[nCntObject].modelData.pTexture[nCntMat]);

				if (g_aObject[nCntObject].scale != NONE_SCALE)
				{ // 拡大率が変更されている場合

					// 頂点法線の自動正規化を有効にする
					pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
				}

				// モデルの描画
				g_aObject[nCntObject].modelData.pMesh->DrawSubset(nCntMat);

				// 頂点法線の自動正規化を無効にする
				pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
			}

			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	オブジェクトの設定処理
//======================================================================================================================
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXMATERIAL *pMat, int nType, int nBreakType, int nShadowType, int nCollisionType, ROTSTATE stateRot)
{
	// 変数を宣言
	float AverageScale;			// 拡大率の平均値

	// ポインタを宣言
	D3DXMATERIAL *pMatModel;	// マテリアルデータへのポインタ

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aObject[nCntObject].bUse == false)
		{ // オブジェクトが使用されていない場合

			// 引数を代入
			g_aObject[nCntObject].pos            = pos;					// 位置
			g_aObject[nCntObject].rot            = rot;					// 向き
			g_aObject[nCntObject].scale          = scale;				// 拡大率
			g_aObject[nCntObject].nType          = nType;				// オブジェクトの種類
			g_aObject[nCntObject].nBreakType     = nBreakType;			// 壊れ方の種類
			g_aObject[nCntObject].nShadowType    = nShadowType;			// 影の種類
			g_aObject[nCntObject].nCollisionType = nCollisionType;		// 当たり判定の種類

			// オブジェクトの情報を初期化
			g_aObject[nCntObject].state         = ACTIONSTATE_NORMAL;	// 状態
			g_aObject[nCntObject].nLife         = OBJ_LIFE;				// 体力
			g_aObject[nCntObject].nCounterState = 0; 					// 状態管理カウンター

			// 使用している状態にする
			g_aObject[nCntObject].bUse = true;

			// 当たり判定情報を設定
			g_aObject[nCntObject].collInfo.stateRot = stateRot;			// 向き状態

			for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
			{ // 当たり判定の最大数分繰り返す

				// 横幅と縦幅を計算
				if (stateRot == ROTSTATE_0
				||  stateRot == ROTSTATE_180)
				{ // 角度が0度、または180度の場合
					g_aObject[nCntObject].collInfo.fWidth[nCntColl] = g_aCollision[nType].fWidth[nCntColl];
					g_aObject[nCntObject].collInfo.fDepth[nCntColl] = g_aCollision[nType].fDepth[nCntColl];
				}
				else
				{ // 角度90度、または270度の場合
					g_aObject[nCntObject].collInfo.fWidth[nCntColl] = g_aCollision[nType].fDepth[nCntColl];
					g_aObject[nCntObject].collInfo.fDepth[nCntColl] = g_aCollision[nType].fWidth[nCntColl];
				}
			}

			for (int nCntColl = 0; nCntColl < g_aCollision[g_aObject[nCntObject].nType].nNumColl; nCntColl++)
			{ // 当たり判定の総数分繰り返す

				// 位置ベクトルを設定
				g_aObject[nCntObject].collInfo.vecPos[nCntColl] = g_aCollision[nType].vecPos[nCntColl];

				for (int nCntRot = ROTSTATE_0; nCntRot < stateRot; nCntRot++)
				{ // 設定された向きになるまで繰り返す

					// 位置ベクトルを90度回転
					g_aObject[nCntObject].collInfo.vecPos[nCntColl] = D3DXVECTOR3
					( // 引数
						-g_aObject[nCntObject].collInfo.vecPos[nCntColl].z,
						 g_aObject[nCntObject].collInfo.vecPos[nCntColl].y,
						 g_aObject[nCntObject].collInfo.vecPos[nCntColl].x
					);
				}
			}

			// モデル情報を設定
			g_aObject[nCntObject].modelData = GetModelData(nType + FROM_OBJECT);	// モデル情報

			// マテリアルデータへのポインタを取得
			pMatModel = (D3DXMATERIAL*)g_aObject[nCntObject].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aObject[nCntObject].modelData.dwNumMat; nCntMat++, pMatModel++)
			{ // マテリアルの数分繰り返す

				// マテリアルをコピーする
				g_aObject[nCntObject].matCopy[nCntMat] = *pMatModel;
			}

			// 最大値を反映する
			g_aObject[nCntObject].modelData.vtxMax.x *= g_aObject[nCntObject].scale.x;
			g_aObject[nCntObject].modelData.vtxMax.y *= g_aObject[nCntObject].scale.y;
			g_aObject[nCntObject].modelData.vtxMax.z *= g_aObject[nCntObject].scale.z;

			// 最小値を反映する
			g_aObject[nCntObject].modelData.vtxMin.x *= g_aObject[nCntObject].scale.x;
			g_aObject[nCntObject].modelData.vtxMin.y *= g_aObject[nCntObject].scale.y;
			g_aObject[nCntObject].modelData.vtxMin.z *= g_aObject[nCntObject].scale.z;

			for (int nCntMat = 0; nCntMat < (int)g_aObject[nCntObject].modelData.dwNumMat; nCntMat++)
			{ // マテリアルの数分繰り返す

				// 引数の色を代入
				g_aObject[nCntObject].matCopy[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Ambient;
				g_aObject[nCntObject].matCopy[nCntMat].MatD3D.Diffuse = pMat[nCntMat].MatD3D.Diffuse;
			}

			if (nShadowType == SHADOWTYPE_CIRCLE)
			{ // 丸影の場合

				// 拡大率の平均を求める
				AverageScale = (g_aObject[nCntObject].scale.x + g_aObject[nCntObject].scale.z) * 0.5f;

				// 影のインデックスを設定
				g_aObject[nCntObject].nShadowID = SetCircleShadow
				( // 引数
					0.5f,										// α値
					g_aShadowRadius[nType] * AverageScale,		// 半径
					&g_aObject[nCntObject].nShadowID,			// 影の親の影インデックス
					&g_aObject[nCntObject].bUse					// 影の親の使用状況
				);
			}
			else if (nShadowType == SHADOWTYPE_MODEL)
			{ // モデル影の場合

				// 影のインデックスを設定
				g_aObject[nCntObject].nShadowID = SetModelShadow
				( // 引数
					g_aObject[nCntObject].modelData,	// モデル情報
					&g_aObject[nCntObject].nShadowID,	// 影の親の影インデックス
					&g_aObject[nCntObject].bUse			// 影の親の使用状況
				);
			}
			else
			{ // 影無しの場合

				// 影のインデックスを設定
				g_aObject[nCntObject].nShadowID = NONE_SHADOW;	// 影を設定しない
			}

			// 影の位置設定
			SetPositionShadow(g_aObject[nCntObject].nShadowID, g_aObject[nCntObject].pos, g_aObject[nCntObject].rot, g_aObject[nCntObject].scale);

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
void CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pOldPos, D3DXVECTOR3 *pMove, float fWidth, float fDepth)
{
	// 変数を宣言
	D3DXVECTOR3 collPos;	// 当たり判定の中心座標

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aObject[nCntObject].bUse == true)
		{ // オブジェクトが使用されている場合

			switch (g_aObject[nCntObject].nCollisionType)
			{ // 当たり判定の種類ごとの処理
			case COLLISIONTYPE_NONE:	// 当たり判定無し

				// 無し

				// 処理を抜ける
				break;

			case COLLISIONTYPE_MODEL:	// モデル頂点の当たり判定

				// 前後の当たり判定
				if (pPos->x + fWidth > g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMin.x
				&&  pPos->x - fWidth < g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMax.x)
				{ // ブロックの左右の範囲内の場合

					if (pPos->z    + fDepth >  g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMin.z
					&&  pOldPos->z + fDepth <= g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMin.z)
					{ // 前からの当たり判定

						// 位置を補正
						pPos->z = g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMin.z - fDepth - 0.01f;

						// 移動量を削除
						pMove->x *= 0.95f;
					}
					else if (pPos->z    - fDepth <  g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMax.z
					     &&  pOldPos->z - fDepth >= g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMax.z)
					{ // 後ろからの当たり判定

						// 位置を補正
						pPos->z = g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMax.z + fDepth + 0.01f;

						// 移動量を削除
						pMove->x *= 0.95f;
					}
				}

				// 左右の当たり判定
				if (pPos->z + fDepth > g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMin.z
				&&  pPos->z - fDepth < g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMax.z)
				{ // ブロックの前後の範囲内の場合

					if (pPos->x    + fWidth >  g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMin.x
					&&  pOldPos->x + fWidth <= g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMin.x)
					{ // 左からの当たり判定

						// 位置を補正
						pPos->x = g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMin.x - fWidth - 0.01f;

						// 移動量を削除
						pMove->x *= 0.95f;
					}
					else if (pPos->x    - fWidth <  g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMax.x
					     &&  pOldPos->x - fWidth >= g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMax.x)
					{ // 右からの当たり判定
						
						// 位置を補正
						pPos->x = g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMax.x + fWidth + 0.01f;

						// 移動量を削除
						pMove->x *= 0.95f;
					}
				}

				// 処理を抜ける
				break;

			case COLLISIONTYPE_CREATE:	// 作成した当たり判定 (汎用)

				for (int nCntColl = 0; nCntColl < g_aCollision[g_aObject[nCntObject].nType].nNumColl; nCntColl++)
				{ // 当たり判定数分繰り返す

					// 当たり判定の中心座標を設定
					collPos = g_aObject[nCntObject].pos - g_aObject[nCntObject].collInfo.vecPos[nCntColl];

					// 前後の当たり判定
					if (pPos->x + fWidth > collPos.x - g_aObject[nCntObject].collInfo.fWidth[nCntColl]
					&&  pPos->x - fWidth < collPos.x + g_aObject[nCntObject].collInfo.fWidth[nCntColl])
					{ // ブロックの左右の範囲内の場合

						if (pPos->z    + fDepth >  collPos.z - g_aObject[nCntObject].collInfo.fDepth[nCntColl]
						&&  pOldPos->z + fDepth <= collPos.z - g_aObject[nCntObject].collInfo.fDepth[nCntColl])
						{ // 前からの当たり判定

							// 位置を補正
							pPos->z = collPos.z - g_aObject[nCntObject].collInfo.fDepth[nCntColl] - fDepth - 0.01f;

							if (pMove->x >= 13.0f)
							{ // 移動量が一定以上の場合
								// パーティクルの設定処理
								SetParticle
								(
									D3DXVECTOR3(pPos->x, pPos->y + 50.0f, pPos->z + fDepth),	// 位置
									D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),					// 色
									PARTICLETYPE_SPARK,									// パーティクルの種類
									10,													// 生成数
									2													// 寿命
								);
							}

							// 移動量を削除
							pMove->x *= 0.95f;
						}
						else if (pPos->z    - fDepth <  collPos.z + g_aObject[nCntObject].collInfo.fDepth[nCntColl]
						     &&  pOldPos->z - fDepth >= collPos.z + g_aObject[nCntObject].collInfo.fDepth[nCntColl])
						{ // 後ろからの当たり判定

							// 位置を補正
							pPos->z = collPos.z + g_aObject[nCntObject].collInfo.fDepth[nCntColl] + fDepth + 0.01f;

							if (pMove->x >= 13.0f)
							{ // 移動量が一定以上の場合
							  // パーティクルの設定処理
								SetParticle
								(
									D3DXVECTOR3(pPos->x, pPos->y + 50.0f, pPos->z - fDepth),	// 位置
									D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),					// 色
									PARTICLETYPE_SPARK,									// パーティクルの種類
									10,													// 生成数
									2													// 寿命
								);
							}

							// 移動量を削除
							pMove->x *= 0.95f;
						}
					}

					// 左右の当たり判定
					if (pPos->z + fDepth > collPos.z - g_aObject[nCntObject].collInfo.fDepth[nCntColl]
					&&  pPos->z - fDepth < collPos.z + g_aObject[nCntObject].collInfo.fDepth[nCntColl])
					{ // ブロックの前後の範囲内の場合

						if (pPos->x    + fWidth >  collPos.x - g_aObject[nCntObject].collInfo.fWidth[nCntColl]
						&&  pOldPos->x + fWidth <= collPos.x - g_aObject[nCntObject].collInfo.fWidth[nCntColl])
						{ // 左からの当たり判定

							// 位置を補正
							pPos->x = collPos.x - g_aObject[nCntObject].collInfo.fWidth[nCntColl] - fWidth - 0.01f;

							if (pMove->x >= 13.0f)
							{ // 移動量が一定以上の場合
								// パーティクルの設定処理
								SetParticle
								(
									D3DXVECTOR3(pPos->x + fWidth, pPos->y + 50.0f, pPos->z),	// 位置
									D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),					// 色
									PARTICLETYPE_SPARK,									// パーティクルの種類
									10,													// 生成数
									2													// 寿命
								);
							}

							// 移動量を削除
							pMove->x *= 0.95f;
						}
						else if (pPos->x    - fWidth <  collPos.x + g_aObject[nCntObject].collInfo.fWidth[nCntColl]
						     &&  pOldPos->x - fWidth >= collPos.x + g_aObject[nCntObject].collInfo.fWidth[nCntColl])
						{ // 右からの当たり判定
							
							// 位置を補正
							pPos->x = collPos.x + g_aObject[nCntObject].collInfo.fWidth[nCntColl] + fWidth + 0.01f;

							if (pMove->x >= 13.0f)
							{ // 移動量が一定以上の場合
								// パーティクルの設定処理
								SetParticle
								(
									D3DXVECTOR3(pPos->x - fWidth, pPos->y + 50.0f, pPos->z),	// 位置
									D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),					// 色
									PARTICLETYPE_SPARK,									// パーティクルの種類
									10,													// 生成数
									2													// 寿命
								);
							}

							// 移動量を削除
							pMove->x *= 0.95f;
						}
					}
				}

				// 処理を抜ける
				break;
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

//======================================================================================================================
//	当たり判定の取得処理
//======================================================================================================================
Collision *GetCollision(void)
{
	// 当たり判定の情報の先頭アドレスを返す
	return &g_aCollision[0];
}

//======================================================================================================================
//	当たり判定のセットアップ処理
//======================================================================================================================
void TxtSetCollision(void)
{
	// 変数を宣言
	int nEnd;					// テキスト読み込み終了の確認用
	int nType;					// 種類の代入用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;				// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(COLLISION_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "SETCOLL_OBJECT") == 0)
			{ // 読み込んだ文字列が SETCOLL_OBJECT の場合

				do
				{ // 読み込んだ文字列が END_SETCOLL_OBJECT ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "SET_COLLISION") == 0)
					{ // 読み込んだ文字列が SET_COLLISION の場合

						do
						{ // 読み込んだ文字列が END_SET_COLLISION ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "TYPE") == 0)
							{ // 読み込んだ文字列が TYPE の場合
								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nType);		// 種類を読み込む
							}
							else if (strcmp(&aString[0], "NUMCOLL") == 0)
							{ // 読み込んだ文字列が NUMCOLL の場合
								fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
								fscanf(pFile, "%d", &g_aCollision[nType].nNumColl);	// 当たり判定数を読み込む

								for (int nCntColl = 0; nCntColl < g_aCollision[nType].nNumColl; nCntColl++)
								{ // 当たり判定数分繰り返す

									fscanf(pFile, "%s", &aString[0]);			// 当たり判定の番号・要素を読み込む (不要)
									fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)

									fscanf(pFile, "%f%f%f",
									&g_aCollision[nType].vecPos[nCntColl].x,	// 位置ベクトル (x) を読み込む
									&g_aCollision[nType].vecPos[nCntColl].y,	// 位置ベクトル (y) を読み込む
									&g_aCollision[nType].vecPos[nCntColl].z);	// 位置ベクトル (z) を読み込む
									
									fscanf(pFile, "%s", &aString[0]);			// 当たり判定の番号・要素を読み込む (不要)
									fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)

									fscanf(pFile, "%f%f%f",
									&g_aCollision[nType].scale[nCntColl].x,		// 拡大率 (x) を読み込む
									&g_aCollision[nType].scale[nCntColl].y,		// 拡大率 (y) を読み込む
									&g_aCollision[nType].scale[nCntColl].z);	// 拡大率 (z) を読み込む

									fscanf(pFile, "%s", &aString[0]);							// 当たり判定の番号・要素を読み込む (不要)
									fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)

									fscanf(pFile, "%f", &g_aCollision[nType].fWidth[nCntColl]);	// 横幅を読み込む

									fscanf(pFile, "%s", &aString[0]);							// 当たり判定の番号・要素を読み込む (不要)
									fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)

									fscanf(pFile, "%f", &g_aCollision[nType].fDepth[nCntColl]);	// 奥行を読み込む
								}
							}
						} while (strcmp(&aString[0], "END_SET_COLLISION") != 0);	// 読み込んだ文字列が END_SET_COLLISION ではない場合ループ
					}
				} while (strcmp(&aString[0], "END_SETCOLL_OBJECT") != 0);			// 読み込んだ文字列が END_SETCOLL_OBJECT ではない場合ループ
			}
		} while (nEnd != EOF);														// 読み込んだ文字列が EOF ではない場合ループ
		
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "当たり判定ファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}

//======================================================================================================================
//	影の半径のセットアップ処理
//======================================================================================================================
void TxtSetShadow(void)
{
	// 変数を宣言
	int nEnd;					// テキスト読み込み終了の確認用
	int nType;					// 種類の代入用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;				// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(SHADOW_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "SETSHADOW_OBJECT") == 0)
			{ // 読み込んだ文字列が SETSHADOW_OBJECT の場合

				do
				{ // 読み込んだ文字列が END_SETSHADOW_OBJECT ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "SET_SHADOW") == 0)
					{ // 読み込んだ文字列が SET_SHADOW の場合

						do
						{ // 読み込んだ文字列が END_SET_SHADOW ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "TYPE") == 0)
							{ // 読み込んだ文字列が TYPE の場合
								fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
								fscanf(pFile, "%d", &nType);					// 種類を読み込む
							}
							else if (strcmp(&aString[0], "RADIUS") == 0)
							{ // 読み込んだ文字列が RADIUS の場合
								fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
								fscanf(pFile, "%f", &g_aShadowRadius[nType]);	// 影の半径を読み込む
							}
						} while (strcmp(&aString[0], "END_SET_SHADOW") != 0);	// 読み込んだ文字列が END_SET_SHADOW ではない場合ループ
					}
				} while (strcmp(&aString[0], "END_SETSHADOW_OBJECT") != 0);		// 読み込んだ文字列が END_SETSHADOW_OBJECT ではない場合ループ
			}
		} while (nEnd != EOF);													// 読み込んだ文字列が EOF ではない場合ループ
		
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "影の半径ファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
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