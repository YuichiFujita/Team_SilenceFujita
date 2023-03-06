//======================================================================================================================
//
//	アイテム処理 [item.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "input.h"

#include "item.h"
#include "calculation.h"
#include "particle.h"
#include "player.h"
#include "shadow.h"
#include "sound.h"
//#include "tutorial.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define ITEM_SCALE				(D3DXVECTOR3(0.5f, 0.5f, 0.5f))		// アイテムの拡大率
#define ITEM_HEAL				(5)		// アイテムの回復量
#define EFFECT_TIME_ITEM		(160)	// パーティクルを出す間隔
#define MOVE_ROT_ITEM			(0.03f)	// アイテムの回転量
#define ITEM_LOST_COUNT			(600)	// アイテムが消えるカウント数

#define ITEM_POS_DEST_UP		(100.0f)	// アイテムの目標の位置(上昇状態)
#define ITEM_POS_DEST_DOWN		(50.0f)		// アイテムの目標の位置(下降状態)

#define ITEM_LOST_EFFECT_COL	(D3DXCOLOR(0.1f,0.1f,1.0f,1.0f))	// アイテム消失のエフェクトの色
#define ITEM_GET_EFFECT_COL		(D3DXCOLOR(0.1f,0.1f,1.0f,1.0f))	// アイテム取得のエフェクトの色
#define ITEM_MARK_EFFECT_COL	(D3DXCOLOR(0.1f,0.9f,1.0f,1.0f))	// アイテムの目印のエフェクトの色

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apModelItem[] =			// モデルの相対パス
{
	"data\\MODEL_ITEM\\heal000.x",	// 回復アイテムのモデル相対パス
};

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void TutorialItem(void);			// チュートリアルのアイテムの更新処理
void GameItem(void);				// ゲームのアイテムの更新処理

void CollisionPlayer(Item *pItem);	// アイテムとプレイヤーの当たり判定

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
Item  g_aItem[MAX_ITEM];			// アイテムの情報

//======================================================================================================================
//	アイテムの初期化処理
//======================================================================================================================
void InitItem(void)
{
	// アイテムの情報の初期化
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{ // アイテムの最大表示数分繰り返す

		g_aItem[nCntItem].pos            = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aItem[nCntItem].move			 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
		g_aItem[nCntItem].rot            = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
		g_aItem[nCntItem].nType          = 0;								// 種類
		g_aItem[nCntItem].nCounterEffect = 0;								// エフェクト管理カウンター
		g_aItem[nCntItem].nShadowID      = NONE_SHADOW;						// 影のインデックス
		g_aItem[nCntItem].nLostCounter	 = 0;								// 消失カウンター
		g_aItem[nCntItem].bUse           = false;							// 使用状況

		// 状態関係の初期化
		g_aItem[nCntItem].stateInfo.fPosDest = 0.0f;						// 目標の位置
		g_aItem[nCntItem].stateInfo.state	 = ITEMSTATE_UP;				// 状態
		g_aItem[nCntItem].stateInfo.nCounter = 0;							// カウンター

		// モデル情報の初期化
		g_aItem[nCntItem].modelData.dwNumMat = 0;							// マテリアルの数
		g_aItem[nCntItem].modelData.pTexture = NULL;						// テクスチャへのポインタ
		g_aItem[nCntItem].modelData.pMesh	 = NULL;						// メッシュ (頂点情報) へのポインタ
		g_aItem[nCntItem].modelData.pBuffMat = NULL;						// マテリアルへのポインタ
		g_aItem[nCntItem].modelData.dwNumMat = 0;							// マテリアルの数
		g_aItem[nCntItem].modelData.vtxMin	 = INIT_VTX_MIN;				// 最小の頂点座標
		g_aItem[nCntItem].modelData.vtxMax	 = INIT_VTX_MAX;				// 最大の頂点座標
		g_aItem[nCntItem].modelData.size	 = INIT_SIZE;					// 大きさ
		g_aItem[nCntItem].modelData.fRadius	 = 0.0f;						// 半径
	}
}

//======================================================================================================================
//	アイテムの終了処理
//======================================================================================================================
void UninitItem(void)
{

}

//======================================================================================================================
//	アイテムの更新処理
//======================================================================================================================
void UpdateItem(void)
{
	switch (GetMode())
	{ // モードごとの更新
	case MODE_TUTORIAL:	// チュートリアル画面

		// チュートリアルのアイテムの更新
		TutorialItem();

		// 処理を抜ける
		break;

	case MODE_GAME:		// ゲーム画面

		// ゲームのアイテムの更新
		GameItem();

		// 処理を抜ける
		break;
	}
}

//======================================================================================================================
//	アイテムの描画処理
//======================================================================================================================
void DrawItem(void)
{
	// 変数を宣言
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{ // アイテムの最大表示数分繰り返す

		if (g_aItem[nCntItem].bUse == true)
		{ // アイテムが使用されている場合

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aItem[nCntItem].mtxWorld);

			// 拡大率を反映
			D3DXMatrixScaling(&mtxScale, ITEM_SCALE.x, ITEM_SCALE.y, ITEM_SCALE.z);
			D3DXMatrixMultiply(&g_aItem[nCntItem].mtxWorld, &g_aItem[nCntItem].mtxWorld, &mtxScale);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aItem[nCntItem].rot.y, g_aItem[nCntItem].rot.x, g_aItem[nCntItem].rot.z);
			D3DXMatrixMultiply(&g_aItem[nCntItem].mtxWorld, &g_aItem[nCntItem].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y, g_aItem[nCntItem].pos.z);
			D3DXMatrixMultiply(&g_aItem[nCntItem].mtxWorld, &g_aItem[nCntItem].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aItem[nCntItem].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aItem[nCntItem].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aItem[nCntItem].modelData.dwNumMat; nCntMat++)
			{ // マテリアルの数分繰り返す

				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_aItem[nCntItem].modelData.pTexture[nCntMat]);

				// モデルの描画
				g_aItem[nCntItem].modelData.pMesh->DrawSubset(nCntMat);
			}

			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	アイテムの設定処理
//======================================================================================================================
void SetItem(D3DXVECTOR3 pos, int nType)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{ // アイテムの最大表示数分繰り返す

		if (g_aItem[nCntItem].bUse == false)
		{ // アイテムが使用されていない場合

			// 引数を代入
			g_aItem[nCntItem].pos	 = D3DXVECTOR3(pos.x, pos.y + 50.0f, pos.z);		// 位置
			g_aItem[nCntItem].nType  = nType;	// 種類

			// アイテムの情報を初期化
			g_aItem[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
			g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
			g_aItem[nCntItem].nCounterEffect = 0;					// エフェクト管理カウンター
			g_aItem[nCntItem].nLostCounter = 0;						// 消失カウンター

			// 使用している状態にする
			g_aItem[nCntItem].bUse = true;

			// モデル情報を設定
			g_aItem[nCntItem].modelData = GetModelData(MODELTYPE_PLAYER_BARRIER);

			// 影のインデックスを設定
			g_aItem[nCntItem].nShadowID = SetCircleShadow
			( // 引数
				0.5f,										// α値
				g_aItem[nCntItem].modelData.fRadius,		// 半径
				&g_aItem[nCntItem].nShadowID,				// 影の親の影インデックス
				&g_aItem[nCntItem].bUse						// 影の親の使用状況
			);

			// 影の位置設定
			SetPositionShadow
			(
				g_aItem[nCntItem].nShadowID,		// 影のインデックス
				g_aItem[nCntItem].pos,				// 位置
				g_aItem[nCntItem].rot,				// 向き
				NONE_SCALE							// 拡大率
			);

			// 状態関係の設定
			g_aItem[nCntItem].stateInfo.fPosDest = ITEM_POS_DEST_UP;		// 目標の位置
			g_aItem[nCntItem].stateInfo.state	 = ITEMSTATE_UP;			// 状態
			g_aItem[nCntItem].stateInfo.nCounter = 0;						// カウンター

			// 処理を抜ける
			break;
		}
	}
}

//======================================================================================================================
//	アイテムとプレイヤーの当たり判定
//======================================================================================================================
void CollisionPlayer(Item *pItem)
{
	// 変数を宣言
	float fLength = 0.0f;				// アイテムとプレイヤーとの間の距離

	// ポインタを宣言
	Player *pPlayer = GetPlayer();	// プレイヤーの情報

	if (pPlayer->bUse == true)
	{ // プレイヤーが使用されている場合

		// アイテムとプレイヤーとの間の距離を求める
		fLength = (pItem->pos.x - pPlayer->pos.x) * (pItem->pos.x - pPlayer->pos.x)
				+ (pItem->pos.z - pPlayer->pos.z) * (pItem->pos.z - pPlayer->pos.z);

		if (fLength < ((pItem->modelData.fRadius + PLAY_WIDTH) * (pItem->modelData.fRadius + PLAY_WIDTH)))
		{ // アイテムがプレイヤーに当たっている場合

			// パーティクルの設定
			SetParticle
			( // 引数
				pItem->pos,							// 位置
				ITEM_GET_EFFECT_COL,				// 色
				PARTICLETYPE_EXPLOSION,				// 種類
				SPAWN_PARTICLE_EXPLOSION,			// エフェクト数
				1									// 寿命
			);

			// バリアの回復判定
			HealBarrier(pPlayer, ITEM_HEAL);	

			// 使用していない状態にする
			pItem->bUse = false;
		}
	}
}

//======================================================================================================================
//	チュートリアルのアイテムの更新処理
//======================================================================================================================
void TutorialItem(void)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{ // アイテムの最大表示数分繰り返す

		if (g_aItem[nCntItem].bUse == true)
		{ // アイテムが使用されている場合

		  //--------------------------------------------------------------------------------------------------------------
		  //	向きの更新
		  //--------------------------------------------------------------------------------------------------------------
		  // 向きの更新
			g_aItem[nCntItem].rot.y += MOVE_ROT_ITEM;

			// 向きの正規化
			RotNormalize(&g_aItem[nCntItem].rot.y);

			switch (g_aItem[nCntItem].stateInfo.state)
			{
			case ITEMSTATE_UP:		// 上昇状態

									// 移動量を設定する
				g_aItem[nCntItem].move.y = 1.0f;

				// 移動量を加算する
				g_aItem[nCntItem].pos.y += g_aItem[nCntItem].move.y;

				if (g_aItem[nCntItem].pos.y >= g_aItem[nCntItem].stateInfo.fPosDest)
				{ // 位置が目標に一定量近づいた場合

				  // 位置を補正する
					g_aItem[nCntItem].pos.y = g_aItem[nCntItem].stateInfo.fPosDest;

					// 目標の位置を設定する
					g_aItem[nCntItem].stateInfo.fPosDest = ITEM_POS_DEST_DOWN;

					// 状態を設定する
					g_aItem[nCntItem].stateInfo.state = (ITEMSTATE)((g_aItem[nCntItem].stateInfo.state + 1) % ITEMSTATE_MAX);
				}

				break;

			case ITEMSTATE_DOWN:	// 下降状態

									// 移動量を設定する
				g_aItem[nCntItem].move.y = -1.0f;

				// 移動量を加算する
				g_aItem[nCntItem].pos.y += g_aItem[nCntItem].move.y;

				if (g_aItem[nCntItem].pos.y <= g_aItem[nCntItem].stateInfo.fPosDest)
				{ // 位置が目標に一定量近づいた場合

				  // 位置を補正する
					g_aItem[nCntItem].pos.y = g_aItem[nCntItem].stateInfo.fPosDest;

					// 目標の位置を設定する
					g_aItem[nCntItem].stateInfo.fPosDest = ITEM_POS_DEST_UP;

					// 状態を設定する
					g_aItem[nCntItem].stateInfo.state = (ITEMSTATE)((g_aItem[nCntItem].stateInfo.state + 1) % ITEMSTATE_MAX);
				}

				break;
			}

			//--------------------------------------------------------------------------------------------------------------
			//	パーティクルの更新
			//--------------------------------------------------------------------------------------------------------------
			if (g_aItem[nCntItem].nCounterEffect < EFFECT_TIME_ITEM)
			{ // カウンターが一定値より小さい場合

			  // カウンターを加算
				g_aItem[nCntItem].nCounterEffect++;
			}
			else
			{ // カウンターが一定値以上の場合

			  // カウンターを初期化
				g_aItem[nCntItem].nCounterEffect = 0;

				// パーティクルの設定
				SetParticle
				( // 引数
					g_aItem[nCntItem].pos,				// 位置
					ITEM_MARK_EFFECT_COL,				// 色
					PARTICLETYPE_DAMAGE,				// 種類
					SPAWN_PARTICLE_DAMAGE,				// エフェクト数
					1									// 寿命
				);
			}

			// 消失カウントを加算する
			g_aItem[nCntItem].nLostCounter++;

			if (g_aItem[nCntItem].nLostCounter % ITEM_LOST_COUNT == 0)
			{ // 消失カウントが一定数になった場合

				// パーティクルの設定処理
				SetParticle
				(
					g_aItem[nCntItem].pos,
					ITEM_LOST_EFFECT_COL,
					PARTICLETYPE_ITEM_LOST,
					SPAWN_PARTICLE_ITEM_LOST,
					3
				);

				// 消失カウントを初期化する
				g_aItem[nCntItem].nLostCounter = 0;

				// 使用しない
				g_aItem[nCntItem].bUse = false;
			}

			//------------------------------------------------------------------------------------------------------
			//	当たり判定
			//------------------------------------------------------------------------------------------------------
			// アイテムとプレイヤーの当たり判定
			CollisionPlayer(&g_aItem[nCntItem]);

			//------------------------------------------------------------------------------------------------------
			//	影の更新
			//------------------------------------------------------------------------------------------------------
			// 影の位置設定
			SetPositionShadow
			(
				g_aItem[nCntItem].nShadowID,		// 影のインデックス
				D3DXVECTOR3(g_aItem[nCntItem].pos.x, 0.0f, g_aItem[nCntItem].pos.z),				// 位置
				g_aItem[nCntItem].rot,				// 向き
				ITEM_SCALE							// 拡大率
			);
		}
	}
}

//======================================================================================================================
//	ゲームのアイテムの更新処理
//======================================================================================================================
void GameItem(void)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{ // アイテムの最大表示数分繰り返す

		if (g_aItem[nCntItem].bUse == true)
		{ // アイテムが使用されている場合

			//--------------------------------------------------------------------------------------------------------------
			//	向きの更新
			//--------------------------------------------------------------------------------------------------------------
			// 向きの更新
			g_aItem[nCntItem].rot.y += MOVE_ROT_ITEM;

			// 向きの正規化
			RotNormalize(&g_aItem[nCntItem].rot.y);

			switch (g_aItem[nCntItem].stateInfo.state)
			{
			case ITEMSTATE_UP:		// 上昇状態

				// 移動量を設定する
				g_aItem[nCntItem].move.y = 1.0f;

				// 移動量を加算する
				g_aItem[nCntItem].pos.y += g_aItem[nCntItem].move.y;

				if (g_aItem[nCntItem].pos.y >= g_aItem[nCntItem].stateInfo.fPosDest)
				{ // 位置が目標に一定量近づいた場合

					// 位置を補正する
					g_aItem[nCntItem].pos.y = g_aItem[nCntItem].stateInfo.fPosDest;

					// 目標の位置を設定する
					g_aItem[nCntItem].stateInfo.fPosDest = ITEM_POS_DEST_DOWN;

					// 状態を設定する
					g_aItem[nCntItem].stateInfo.state = (ITEMSTATE)((g_aItem[nCntItem].stateInfo.state + 1) % ITEMSTATE_MAX);
				}

				break;

			case ITEMSTATE_DOWN:	// 下降状態

				// 移動量を設定する
				g_aItem[nCntItem].move.y = -1.0f;

				// 移動量を加算する
				g_aItem[nCntItem].pos.y += g_aItem[nCntItem].move.y;

				if (g_aItem[nCntItem].pos.y <= g_aItem[nCntItem].stateInfo.fPosDest)
				{ // 位置が目標に一定量近づいた場合

					// 位置を補正する
					g_aItem[nCntItem].pos.y = g_aItem[nCntItem].stateInfo.fPosDest;

					// 目標の位置を設定する
					g_aItem[nCntItem].stateInfo.fPosDest = ITEM_POS_DEST_UP;

					// 状態を設定する
					g_aItem[nCntItem].stateInfo.state = (ITEMSTATE)((g_aItem[nCntItem].stateInfo.state + 1) % ITEMSTATE_MAX);
				}

				break;
			}

			//--------------------------------------------------------------------------------------------------------------
			//	パーティクルの更新
			//--------------------------------------------------------------------------------------------------------------
			if (g_aItem[nCntItem].nCounterEffect < EFFECT_TIME_ITEM)
			{ // カウンターが一定値より小さい場合

				// カウンターを加算
				g_aItem[nCntItem].nCounterEffect++;
			}
			else
			{ // カウンターが一定値以上の場合

				// カウンターを初期化
				g_aItem[nCntItem].nCounterEffect = 0;

				// パーティクルの設定
				SetParticle
				( // 引数
					g_aItem[nCntItem].pos,				// 位置
					ITEM_MARK_EFFECT_COL,				// 色
					PARTICLETYPE_DAMAGE,				// 種類
					SPAWN_PARTICLE_DAMAGE,				// エフェクト数
					1									// 寿命
				);
			}

			// 消失カウントを加算する
			g_aItem[nCntItem].nLostCounter++;

			if (g_aItem[nCntItem].nLostCounter % ITEM_LOST_COUNT == 0)
			{ // 消失カウントが一定数になった場合

				// パーティクルの設定処理
				SetParticle
				(
					g_aItem[nCntItem].pos,
					ITEM_LOST_EFFECT_COL,
					PARTICLETYPE_ITEM_LOST,
					SPAWN_PARTICLE_ITEM_LOST,
					3
				);

				// 消失カウントを初期化する
				g_aItem[nCntItem].nLostCounter = 0;

				// 使用しない
				g_aItem[nCntItem].bUse = false;
			}

			//------------------------------------------------------------------------------------------------------
			//	当たり判定
			//------------------------------------------------------------------------------------------------------
			// アイテムとプレイヤーの当たり判定
			CollisionPlayer(&g_aItem[nCntItem]);

			//------------------------------------------------------------------------------------------------------
			//	影の更新
			//------------------------------------------------------------------------------------------------------
			// 影の位置設定
			SetPositionShadow
			(
				g_aItem[nCntItem].nShadowID,		// 影のインデックス
				D3DXVECTOR3(g_aItem[nCntItem].pos.x, 0.0f, g_aItem[nCntItem].pos.z),				// 位置
				g_aItem[nCntItem].rot,				// 向き
				ITEM_SCALE							// 拡大率
			);
		}
	}
}

//======================================================================================================================
//	アイテムの取得処理
//======================================================================================================================
Item *GetItem(void)
{
	// アイテムの情報の先頭アドレスを返す
	return &g_aItem[0];
}

#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================
//**********************************************************************************************************************
//	アイテムの総数取得処理
//**********************************************************************************************************************
int GetNumItem(void)
{
	// 変数を宣言
	int nNumItem = 0;	// アイテムの総数の確認用

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{ // アイテムの最大表示数分繰り返す

		if (g_aItem[nCntItem].bUse == true)
		{ // アイテムが使用されている場合

			// カウンターを加算
			nNumItem++;
		}
	}

	// 変数の値を返す
	return nNumItem;	// アイテムの総数
}
#endif