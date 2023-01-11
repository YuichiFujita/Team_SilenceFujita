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

#include "object.h"
#include "particle.h"
#include "shadow.h"
#include "sound.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define OBJ_LIFE			(50)		// オブジェクトの体力

#define DAMAGE_TIME_OBJ		(PLAY_SLASH_LIFE)		// ダメージ状態を保つ時間
#define UNR_TIME_OBJ		(PLAY_SLASH_LIFE - 10)	// 無敵状態に変更する時間

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apModelObject[] =			// モデルの相対パス
{
	"data\\MODEL_STAGE\\wood000.x",		// 木のモデル相対パス
	"data\\MODEL_STAGE\\wood001.x",		// 木のモデル相対パス
	"data\\MODEL_STAGE\\wood002.x",		// 木のモデル相対パス
	"data\\MODEL_STAGE\\stone000.x",	// 石のモデル相対パス
	"data\\MODEL_STAGE\\stone001.x",	// 石のモデル相対パス
	"data\\MODEL_STAGE\\stone002.x",	// 石のモデル相対パス
	"data\\MODEL_STAGE\\box000.x",		// 箱のモデル相対パス
	"data\\MODEL_STAGE\\box001.x",		// 箱のモデル相対パス
};

//**********************************************************************************************************************
//	列挙型定義 (OBJECTTYPE)
//**********************************************************************************************************************
typedef enum
{
	OBJECTTYPE_WOOD_S = 0,				// 木 (小)
	OBJECTTYPE_WOOD_M,					// 木 (中)
	OBJECTTYPE_WOOD_L,					// 木 (大)
	OBJECTTYPE_STONE_S,					// 石 (小)
	OBJECTTYPE_STONE_M,					// 石 (中)
	OBJECTTYPE_STONE_L,					// 石 (大)
	OBJECTTYPE_BOX_S,					// 箱 (小)
	OBJECTTYPE_BOX_L,					// 箱 (大)
	OBJECTTYPE_MAX,						// この列挙型の総数
} OBJECTTYPE;

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
Model  g_aModelObject[OBJECTTYPE_MAX];	// オブジェクトのモデル情報
Object g_aObject[MAX_OBJECT];			// オブジェクトの情報

//======================================================================================================================
//	オブジェクトの初期化処理
//======================================================================================================================
void InitObject(void)
{
	// 変数を宣言
	int         nNumVtx;		// モデルの頂点数
	DWORD       dwSizeFVF;		// モデルの頂点フォーマットのサイズ
	BYTE        *pVtxBuff;		// モデルの頂点バッファへのポインタ
	D3DXVECTOR3 vtx;			// モデルの頂点座標
	D3DXVECTOR3 size;			// モデルの大きさ

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL      *pMat;					// マテリアルへのポインタ

	// オブジェクトのモデル情報の初期化
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // オブジェクトの種類分繰り返す

		g_aModelObject[nCntObject].apTexture[MAX_MODEL_TEXTURE] = {};						// テクスチャへのポインタ
		g_aModelObject[nCntObject].pMesh    = NULL;											// メッシュ (頂点情報) へのポインタ
		g_aModelObject[nCntObject].pBuffMat = NULL;											// マテリアルへのポインタ
		g_aModelObject[nCntObject].dwNumMat = 0;											// マテリアルの数
		g_aModelObject[nCntObject].vtxMin   = D3DXVECTOR3( 9999.0f,  9999.0f,  9999.0f);	// 最小の頂点座標
		g_aModelObject[nCntObject].vtxMax   = D3DXVECTOR3(-9999.0f, -9999.0f, -9999.0f);	// 最大の頂点座標
		g_aModelObject[nCntObject].fHeight  = 0.0f;											// 縦幅
		g_aModelObject[nCntObject].fRadius  = 0.0f;											// 半径
	}

	// オブジェクトの情報の初期化
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // オブジェクトの最大表示数分繰り返す

		g_aObject[nCntObject].pos           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_aObject[nCntObject].rot           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
		g_aObject[nCntObject].pModelData    = NULL;									// モデル情報
		g_aObject[nCntObject].state         = ACTIONSTATE_NONE;						// 状態
		g_aObject[nCntObject].nLife         = 0;									// 体力
		g_aObject[nCntObject].nBreakType    = BREAKTYPE_NONE;						// 壊れ方の種類
		g_aObject[nCntObject].nType         = 0;									// オブジェクトの種類
		g_aObject[nCntObject].nCounterState = 0; 									// 状態管理カウンター
		g_aObject[nCntObject].nShadowID     = NONE_SHADOW;							// 影のインデックス
		g_aObject[nCntObject].bUse          = false;								// 使用状況
	}

	// xファイルの読み込み
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // オブジェクトの種類分繰り返す

		D3DXLoadMeshFromX
		( // 引数
			apModelObject[nCntObject],				// モデルの相対パス
			D3DXMESH_SYSTEMMEM,
			pDevice,								// デバイスへのポインタ
			NULL,
			&g_aModelObject[nCntObject].pBuffMat,	// マテリアルへのポインタ
			NULL,
			&g_aModelObject[nCntObject].dwNumMat,	// マテリアルの数
			&g_aModelObject[nCntObject].pMesh		// メッシュ (頂点情報) へのポインタ
		);
	}

	// 当たり判定の作成
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // オブジェクトの種類分繰り返す

		// モデルの頂点数を取得
		nNumVtx = g_aModelObject[nCntObject].pMesh->GetNumVertices();

		// モデルの頂点フォーマットのサイズを取得
		dwSizeFVF = D3DXGetFVFVertexSize(g_aModelObject[nCntObject].pMesh->GetFVF());

		// モデルの頂点バッファをロック
		g_aModelObject[nCntObject].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{ // モデルの頂点数分繰り返す

			// モデルの頂点座標を代入
			vtx = *(D3DXVECTOR3*)pVtxBuff;

			// 頂点座標 (x) の設定
			if (vtx.x < g_aModelObject[nCntObject].vtxMin.x)
			{ // 今回の頂点座標 (x) が、現状の頂点座標 (x) よりも小さい場合

				// 今回の頂点情報 (x) を代入
				g_aModelObject[nCntObject].vtxMin.x = vtx.x;
			}
			else if (vtx.x > g_aModelObject[nCntObject].vtxMax.x)
			{ // 今回の頂点座標 (x) が、現状の頂点座標 (x) よりも大きい場合

				// 今回の頂点情報 (x) を代入
				g_aModelObject[nCntObject].vtxMax.x = vtx.x;
			}

			// 頂点座標 (y) の設定
			if (vtx.y < g_aModelObject[nCntObject].vtxMin.y)
			{ // 今回の頂点座標 (y) が、現状の頂点座標 (y) よりも小さい場合

				// 今回の頂点情報 (y) を代入
				g_aModelObject[nCntObject].vtxMin.y = vtx.y;
			}
			else if (vtx.y > g_aModelObject[nCntObject].vtxMax.y)
			{ // 今回の頂点座標 (y) が、現状の頂点座標 (y) よりも大きい場合

				// 今回の頂点情報 (y) を代入
				g_aModelObject[nCntObject].vtxMax.y = vtx.y;
			}

			// 頂点座標 (z) の設定
			if (vtx.z < g_aModelObject[nCntObject].vtxMin.z)
			{ // 今回の頂点座標 (z) が、現状の頂点座標 (z) よりも小さい場合

				// 今回の頂点情報 (z) を代入
				g_aModelObject[nCntObject].vtxMin.z = vtx.z;
			}
			else if (vtx.z > g_aModelObject[nCntObject].vtxMax.z)
			{ // 今回の頂点座標 (z) が、現状の頂点座標 (z) よりも大きい場合

				// 今回の頂点情報 (z) を代入
				g_aModelObject[nCntObject].vtxMax.z = vtx.z;
			}

			// 頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += dwSizeFVF;
		}

		// モデルの頂点バッファをアンロック
		g_aModelObject[nCntObject].pMesh->UnlockVertexBuffer();

		// モデルサイズを求める
		size = g_aModelObject[nCntObject].vtxMax - g_aModelObject[nCntObject].vtxMin;

		// モデルの縦幅を代入
		g_aModelObject[nCntObject].fHeight = size.y;

		// モデルの円の当たり判定を作成
		g_aModelObject[nCntObject].fRadius = ((size.x * 0.5f) + (size.z * 0.5f)) * 0.5f;
	}

	// テクスチャの読み込み
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // オブジェクトの種類分繰り返す

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_aModelObject[nCntObject].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aModelObject[nCntObject].dwNumMat; nCntMat++)
		{ // マテリアルの数分繰り返す

			if (pMat[nCntMat].pTextureFilename != NULL)
			{ // テクスチャファイルが存在する場合

				// テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aModelObject[nCntObject].apTexture[nCntMat]);
			}
		}
	}
}

//======================================================================================================================
//	オブジェクトの終了処理
//======================================================================================================================
void UninitObject(void)
{
	// テクスチャの破棄
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // オブジェクトの種類分繰り返す

		for (int nCntMat = 0; nCntMat < MAX_MODEL_TEXTURE; nCntMat++)
		{ // 使用するテクスチャ数分繰り返す

			if (g_aModelObject[nCntObject].apTexture[nCntMat] != NULL)
			{ // 変数 (g_aModelObject[nCntObject].apTexture) がNULLではない場合

				g_aModelObject[nCntObject].apTexture[nCntMat]->Release();
				g_aModelObject[nCntObject].apTexture[nCntMat] = NULL;
			}
		}
	}

	// メッシュの破棄
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // オブジェクトの種類分繰り返す

		if (g_aModelObject[nCntObject].pMesh != NULL)
		{ // 変数 (g_aModelObject[nCntObject].pMesh) がNULLではない場合

			g_aModelObject[nCntObject].pMesh->Release();
			g_aModelObject[nCntObject].pMesh = NULL;
		}
	}

	// マテリアルの破棄
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // オブジェクトの種類分繰り返す

		if (g_aModelObject[nCntObject].pBuffMat != NULL)
		{ // 変数 (g_aModelObject[nCntObject].pBuffMat) がNULLではない場合

			g_aModelObject[nCntObject].pBuffMat->Release();
			g_aModelObject[nCntObject].pBuffMat = NULL;
		}
	}
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
	D3DXMATRIX   mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用

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
				pDevice->SetTexture(0, g_aObject[nCntObject].pModelData->apTexture[nCntMat]);

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
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nBreakType, int nType)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aObject[nCntObject].bUse == false)
		{ // オブジェクトが使用されていない場合

			// 引数を代入
			g_aObject[nCntObject].pos        = pos;			// 位置
			g_aObject[nCntObject].rot        = rot;			// 向き
			g_aObject[nCntObject].nBreakType = nBreakType;	// 壊れ方の種類
			g_aObject[nCntObject].nType      = nType;		// オブジェクトの種類

			// オブジェクトの情報を初期化
			g_aObject[nCntObject].state         = ACTIONSTATE_NORMAL;	// 状態
			g_aObject[nCntObject].nLife         = OBJ_LIFE;				// 体力
			g_aObject[nCntObject].nCounterState = 0; 					// 状態管理カウンター

			// 使用している状態にする
			g_aObject[nCntObject].bUse = true;

			// モデル情報を設定
			g_aObject[nCntObject].pModelData = &g_aModelObject[nType];	// モデル情報

			// 影のインデックスを設定
			g_aObject[nCntObject].nShadowID = SetShadow
			( // 引数
				0.5f,																												// α値
				fabsf(g_aModelObject[g_aObject[nCntObject].nType].vtxMax.x - g_aModelObject[g_aObject[nCntObject].nType].vtxMin.x),	// 半径
				&g_aObject[nCntObject].nShadowID,																					// 影の親の影インデックス
				&g_aObject[nCntObject].bUse																							// 影の親の使用状況
			);

			// 影の位置設定
			SetPositionShadow(g_aObject[nCntObject].nShadowID, g_aObject[nCntObject].pos);

			// 処理を抜ける
			break;
		}
	}
}

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