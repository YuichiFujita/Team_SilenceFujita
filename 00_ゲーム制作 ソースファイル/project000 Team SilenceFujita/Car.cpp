//======================================================================================================================
//
//	車の処理 [Car.cpp]
//	Author：小原立暉
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "input.h"
#include "model.h"
#include "calculation.h"

#include "Car.h"
#include "shadow.h"
#include "sound.h"
#include "player.h"
#include "Police.h"
#include "object.h"
#include "meshfield.h"

#ifdef _DEBUG	// デバッグ処理
#include "game.h"
#endif

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define CAR_LIFE				(50)		// オブジェクトの体力
#define CAR_GRAVITY				(0.75f)		// プレイヤーにかかる重力
#define CAR_MOVE_FORWARD		(0.1f)		// プレイヤー前進時の移動量
#define CAR_MOVE_BACKWARD		(0.2f)		// プレイヤー後退時の移動量
#define CAR_MOVE_ROT			(0.012f)	// プレイヤーの向き変更量
#define REV_CAR_MOVE_ROT		(0.1f)		// 移動量による向き変更量の補正係数
#define SUB_CAR_MOVE_VALUE		(15.0f)		// 向き変更時の減速が行われる移動量
#define SUB_CAR_MOVE			(0.05f)		// 向き変更時の減速量
#define MAX_CAR_FORWARD			(30.0f)		// 前進時の最高速度
#define MAX_CAR_FORWARD_PATROL  (15.0f)		// パトロール中の前進時の最高速度
#define MAX_CAR_BACKWARD		(8.0f)		// 後退時の最高速度
#define REV_CAR_MOVE_SUB		(0.04f)		// 移動量の減速係数
#define CAR_NOTMOVE_SLOW		(0.04f)		// 車の移動していないときの減速係数
#define CAR_STOP_RADIUS_DIST	(300.0f)	// 車の止まる指標の円のずらす距離
#define CAR_STOP_ADD_RADIUS		(50.0f)		// 車の止まる半径の追加分

#define CAR_TRAFFIC_CNT			(240)		// 渋滞が起きたときに改善する用のカウント
#define CAR_TRAFFIC_IMPROVE_CNT	(540)		// 渋滞状態の解除のカウント

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void PosCar(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove);		// 車の位置の更新処理
void RevCar(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos);	// 車の補正の更新処理
void CurveCar(Car *pCar);							// 車のカーブ処理
void DashCarAction(Car *pCar);						// 車の走行処理
void SetCarPosRot(Car *pCar);						// 車の位置と向きの設定処理
void CarPosRotCorrect(Car *pCar);					// 車の位置の補正処理
void CarBodyStopPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 rot, D3DXVECTOR3 *pMove, float fWidth, float fDepth, COLLOBJECTTYPE collObject, int *pTraCnt);		// プレイヤーとの当たり判定
void CarBodyStopCar(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 rot, D3DXVECTOR3 *pMove, float fWidth, float fDepth, COLLOBJECTTYPE collObject, int *pTraCnt);		// 車との当たり判定
void CarBodyStopPolice(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 rot, D3DXVECTOR3 *pMove, float fWidth, float fDepth, COLLOBJECTTYPE collObject, int *pTraCnt);		// 警察との当たり判定

void CarTrafficImprove(Car *pCar);					// 車の渋滞改善処理

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
Car g_aCar[MAX_CAR];		// 車の情報

//======================================================================================================================
//	車の初期化処理
//======================================================================================================================
void InitCar(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスへのポインタ

	// 車の情報の初期化
	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
	{ // オブジェクトの最大表示数分繰り返す

		g_aCar[nCntCar].pos         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aCar[nCntCar].posOld      = g_aCar[nCntCar].pos;				// 前回の位置
		g_aCar[nCntCar].move        = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
		g_aCar[nCntCar].rot         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
		g_aCar[nCntCar].nShadowID   = NONE_SHADOW;						// 影のインデックス
		g_aCar[nCntCar].bombState   = BOMBSTATE_NONE;					// ボムの状態
		g_aCar[nCntCar].bJump       = false;							// ジャンプしているかどうか
		g_aCar[nCntCar].bMove       = false;							// 移動しているか
		g_aCar[nCntCar].nTrafficCnt = 0;								// 渋滞カウント
		g_aCar[nCntCar].state		= CARSTATE_NORMAL;					// 状態
		g_aCar[nCntCar].bUse		= false;							// 使用状況

		//曲がり角の情報の初期化
		g_aCar[nCntCar].carCurveInfo.curveInfo.curveAngle   = CURVE_LEFT;	// 左に曲がる
		g_aCar[nCntCar].carCurveInfo.curveInfo.nCurveNumber = 0;			// カーブする番地
		g_aCar[nCntCar].carCurveInfo.nSKipCnt    = 0;						// スキップする曲がり角の回数
		g_aCar[nCntCar].carCurveInfo.rotDest     = g_aCar[nCntCar].rot;		// 前回の向き
		g_aCar[nCntCar].carCurveInfo.actionState = CARACT_DASH;				// 現在の車の行動

		// モデル情報の初期化
		g_aCar[nCntCar].modelData.dwNumMat = 0;						// マテリアルの数
		g_aCar[nCntCar].modelData.pTexture = NULL;					// テクスチャへのポインタ
		g_aCar[nCntCar].modelData.pMesh    = NULL;					// メッシュ (頂点情報) へのポインタ
		g_aCar[nCntCar].modelData.pBuffMat = NULL;					// マテリアルへのポインタ
		g_aCar[nCntCar].modelData.vtxMin   = INIT_VTX_MIN;			// 最小の頂点座標
		g_aCar[nCntCar].modelData.vtxMax   = INIT_VTX_MAX;			// 最大の頂点座標
		g_aCar[nCntCar].modelData.size     = INIT_SIZE;				// 大きさ
		g_aCar[nCntCar].modelData.fRadius  = 0.0f;					// 半径
	}
	
	for (int nCnt = 0; nCnt < MAX_CAR; nCnt++)
	{

		//車の設定処理
		SetCar(D3DXVECTOR3(-3000.0f, 0.0f, 3000.0f));
	}
}

//======================================================================================================================
//	車の終了処理
//======================================================================================================================
void UninitCar(void)
{

}

//======================================================================================================================
//	車の更新処理
//======================================================================================================================
void UpdateCar(void)
{
	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
	{ // オブジェクトの最大表示数分繰り返す
		if (g_aCar[nCntCar].bUse == true)
		{ // オブジェクトが使用されている場合
			if (g_aCar[nCntCar].bombState != BOMBSTATE_BAR_IN)
			{ // バリア内状態ではない場合
				// 前回位置の更新
				g_aCar[nCntCar].posOld = g_aCar[nCntCar].pos;

				// オブジェクトの着地の更新処理
				LandObject(&g_aCar[nCntCar].pos, &g_aCar[nCntCar].move, &g_aCar[nCntCar].bJump);

				//----------------------------------------------------
				//	影の更新
				//----------------------------------------------------
				// 影の位置設定
				SetPositionShadow
				( // 引数
					g_aCar[nCntCar].nShadowID,	// 影のインデックス
					g_aCar[nCntCar].pos,		// 位置
					g_aCar[nCntCar].rot,		// 向き
					NONE_SCALE					// 拡大率
				);

				// プレイヤーの位置の更新
				PosCar(&g_aCar[nCntCar].move, &g_aCar[nCntCar].pos, &g_aCar[nCntCar].rot, g_aCar[nCntCar].bMove);

				//車のカーブ処理
				CurveCar(&g_aCar[nCntCar]);

				if (g_aCar[nCntCar].state == CARSTATE_TRAFFIC)
				{ // 渋滞状態だった場合
					// 車の渋滞改善処理
					CarTrafficImprove(&g_aCar[nCntCar]);
				}

				if (g_aCar[nCntCar].pos.y < 0.0f)
				{//Y軸の位置が0.0fだった場合
					//縦への移動量を0.0fにする
					g_aCar[nCntCar].move.y = 0.0f;

					//位置を0.0fに戻す
					g_aCar[nCntCar].pos.y = 0.0f;
				}
			}

				//----------------------------------------------------
				//	当たり判定
				//----------------------------------------------------
				// オブジェクトとの当たり判定
				CollisionObject
				( // 引数
					&g_aCar[nCntCar].pos,		// 現在の位置
					&g_aCar[nCntCar].posOld,	// 前回の位置
					&g_aCar[nCntCar].move,		// 移動量
					CAR_WIDTH,					// 横幅
					CAR_DEPTH					// 奥行
				);

				if (g_aCar[nCntCar].state != CARSTATE_TRAFFIC && GetBarrierState(&g_aCar[nCntCar]) != BARRIERSTATE_SET)
				{ // 渋滞状態じゃないかつ、バリアセット状態じゃなかった場合

					// 車の停止処理
					CollisionStopCar
					( // 引数
						g_aCar[nCntCar].pos,				// 位置
						g_aCar[nCntCar].rot,				// 向き
						&g_aCar[nCntCar].move,				// 移動量
						g_aCar[nCntCar].modelData.fRadius,	// 半径
						COLLOBJECTTYPE_CAR,					// 対象のサイズ
						&g_aCar[nCntCar].nTrafficCnt		// 渋滞カウント
					);

					if (g_aCar[nCntCar].nTrafficCnt >= CAR_TRAFFIC_CNT)
					{ // 渋滞に巻き込まれた場合
						// 渋滞状態にする
						g_aCar[nCntCar].state = CARSTATE_TRAFFIC;
					}

					// 車同士の当たり判定
					CollisionCarBody
					( // 引数
						&g_aCar[nCntCar].pos,
						&g_aCar[nCntCar].posOld,
						g_aCar[nCntCar].rot,
						&g_aCar[nCntCar].move,
						CAR_WIDTH,
						CAR_DEPTH,
						COLLOBJECTTYPE_CAR,
						&g_aCar[nCntCar].nTrafficCnt
					);
				}

				if (g_aCar[nCntCar].bombState != BOMBSTATE_BAR_IN)
				{ // バリア内状態ではない場合
					// 車の補正の更新処理
					RevCar(&g_aCar[nCntCar].rot, &g_aCar[nCntCar].pos);
				}
		}
	}
}

//======================================================================================================================
//	車の描画処理
//======================================================================================================================
void DrawCar(void)
{
	// 変数を宣言
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	Player           *pPlayer = GetPlayer();	// プレイヤーの情報
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ
	D3DXMATERIAL      bombMat;					// マテリアルデータ (ボム用)

	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aCar[nCntCar].bUse == true)
		{ // オブジェクトが使用されている場合
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aCar[nCntCar].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aCar[nCntCar].rot.y, g_aCar[nCntCar].rot.x, g_aCar[nCntCar].rot.z);
			D3DXMatrixMultiply(&g_aCar[nCntCar].mtxWorld, &g_aCar[nCntCar].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aCar[nCntCar].pos.x, g_aCar[nCntCar].pos.y, g_aCar[nCntCar].pos.z);
			D3DXMatrixMultiply(&g_aCar[nCntCar].mtxWorld, &g_aCar[nCntCar].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aCar[nCntCar].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aCar[nCntCar].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aCar[nCntCar].modelData.dwNumMat; nCntMat++)
			{ // マテリアルの数分繰り返す

				if (pPlayer->atkState == ATTACKSTATE_BOMB)
				{ // 攻撃状態がボム攻撃状態の場合

					// 構造体の要素をクリア
					ZeroMemory(&bombMat, sizeof(D3DXMATERIAL));

					switch (g_aCar[nCntCar].bombState)
					{ // ボムの状態
					case BOMBSTATE_RANGE:	// 範囲内状態

						// 範囲内時のマテリアルの色を設定
						bombMat.MatD3D.Diffuse = BOMB_RANGE_COL;

						// 処理を抜ける
						break;

					case BOMBSTATE_AIM:		// 狙い状態

						// 狙い時のマテリアルの色を設定
						bombMat.MatD3D.Diffuse = BOMB_AIM_COL;

						// 処理を抜ける
						break;

					default:				// 上記以外

						// 範囲外時のマテリアルの色を設定
						bombMat.MatD3D.Diffuse = BOMB_NONE_COL;

						// 処理を抜ける
						break;
					}

					// マテリアルの設定
					pDevice->SetMaterial(&bombMat.MatD3D);
				}
				else
				{ // 攻撃状態がそれ以外の状態の場合

					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				}

				// テクスチャの設定
				pDevice->SetTexture(0, g_aCar[nCntCar].modelData.pTexture[nCntMat]);

				// モデルの描画
				g_aCar[nCntCar].modelData.pMesh->DrawSubset(nCntMat);
			}

			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	車の設定処理
//======================================================================================================================
void SetCar(D3DXVECTOR3 pos)
{
	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aCar[nCntCar].bUse == false)
		{ // オブジェクトが使用されていない場合
			// 引数を代入
			g_aCar[nCntCar].pos			= pos;								// 現在の位置
			g_aCar[nCntCar].posOld		= g_aCar[nCntCar].pos;				// 前回の位置
			g_aCar[nCntCar].move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
			g_aCar[nCntCar].bombState	= BOMBSTATE_NONE;					// 何もしていない状態にする
			g_aCar[nCntCar].bJump		= false;							// ジャンプしているかどうか
			g_aCar[nCntCar].nTrafficCnt = 0;								// 渋滞カウント
			g_aCar[nCntCar].state		= CARSTATE_NORMAL;					// 状態
			g_aCar[nCntCar].bMove		= false;							// 移動していない

			// 使用している状態にする
			g_aCar[nCntCar].bUse = true;

			// モデル情報を設定
			g_aCar[nCntCar].modelData = GetModelData(MODELTYPE_PLAYER_CAR + FROM_OBJECT);	// モデル情報

			// 影のインデックスを設定
			g_aCar[nCntCar].nShadowID = SetModelShadow
			( // 引数
				g_aCar[nCntCar].modelData,	// モデル情報
				&g_aCar[nCntCar].nShadowID,	// 影の親の影インデックス
				&g_aCar[nCntCar].bUse		// 影の親の使用状況
			);

			// 影の位置設定
			SetPositionShadow(g_aCar[nCntCar].nShadowID, g_aCar[nCntCar].pos, g_aCar[nCntCar].rot, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

			// 車の位置と向きの設定処理
			SetCarPosRot(&g_aCar[nCntCar]);
			g_aCar[nCntCar].carCurveInfo.nSKipCnt = 0;																					// スキップする曲がり角の回数
			g_aCar[nCntCar].carCurveInfo.rotDest = g_aCar[nCntCar].rot;																	// 前回の向き
			g_aCar[nCntCar].carCurveInfo.actionState = CARACT_DASH;																		// 走っている状態

			// 処理を抜ける
			break;
		}
	}
}

#if 0
//======================================================================================================================
//	オブジェクトのダメージ判定
//======================================================================================================================
void HitCar(Car *pCar, int nDamage)
{
	if (pCar->state == ACTIONSTATE_NORMAL)
	{ // オブジェクトが通常状態の場合

	  // 引数のダメージ分を体力から減算
		pCar->nLife -= nDamage;

		if (pCar->nLife > 0)
		{ // 体力が残っている場合

		  // ダメージ状態にする
			pCar->state = ACTIONSTATE_DAMAGE;

			// パーティクルの設定
			SetParticle
			( // 引数
				pCar->pos,						// 位置
				D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f),	// 色
				PARTICLETYPE_DAMAGE,				// 種類
				SPAWN_PARTICLE_DAMAGE,				// エフェクト数
				2									// 寿命
			);

			// カウンターを設定
			pCar->nCounterState = DAMAGE_TIME_OBJ;

			// サウンドの再生
			PlaySound(SOUND_LABEL_SE_DMG);			// SE (ダメージ)
		}
		else
		{ // 体力が尽きた場合

		  // 爆発の設定
			SetExplosion(pCar->pos, SOUNDTYPE_BREAK);

			// パーティクルの設定
			SetParticle
			( // 引数
				pCar->pos,						// 位置
				D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f),	// 色
				PARTICLETYPE_EXPLOSION,				// 種類
				SPAWN_PARTICLE_EXPLOSION,			// エフェクト数
				2									// 寿命
			);

			// アイテムの設定
			SetItem(pCar->pos, ITEMTYPE_HEAL);

			// 使用していない状態にする
			pCar->bUse = false;
		}
	}
}
#endif

//======================================================================================================================
//	オブジェクトの取得処理
//======================================================================================================================
Car *GetCarData(void)
{
	// オブジェクトの情報の先頭アドレスを返す
	return &g_aCar[0];
}

//============================================================
//	プレイヤーの位置の更新処理
//============================================================
void PosCar(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove)
{
	//--------------------------------------------------------
	//	重力の更新
	//--------------------------------------------------------
	move->y -= CAR_GRAVITY;

	//--------------------------------------------------------
	//	移動量の補正
	//--------------------------------------------------------
	if (move->x > MAX_CAR_FORWARD)
	{ // プレイヤーの移動量 (x) が一定値以上の場合

	  // プレイヤーの移動量 (x) を補正
		move->x = MAX_CAR_FORWARD;
	}
	else if (move->x < -MAX_CAR_BACKWARD)
	{ // プレイヤーの移動量 (x) が一定値以下の場合

	  // プレイヤーの移動量 (x) を補正
		move->x = -MAX_CAR_BACKWARD;
	}

	//--------------------------------------------------------
	//	位置の更新
	//--------------------------------------------------------
	pos->x += sinf(rot->y) * move->x;
	pos->y += move->y;
	pos->z += cosf(rot->y) * move->x;

	//--------------------------------------------------------
	//	移動量の減衰
	//--------------------------------------------------------
	if (bMove == false)
	{ // 移動していない状態の場合

	  // 移動量を減速
		move->x += (0.0f - move->x) * CAR_NOTMOVE_SLOW;
	}
}

//============================================================
//	プレイヤーの補正の更新処理
//============================================================
void RevCar(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos)
{
	// 向きの正規化
	RotNormalize(&rot->y);

	//--------------------------------------------------------
	//	移動範囲の補正
	//--------------------------------------------------------
	if (pos->z > GetLimitStage().fNear - (CAR_DEPTH * 2))
	{ // 範囲外の場合 (手前)

		// 手前に位置を補正
		pos->z = GetLimitStage().fNear - (CAR_DEPTH * 2);
	}
	if (pos->z < GetLimitStage().fFar + (CAR_DEPTH * 2))
	{ // 範囲外の場合 (奥)

		// 奥に位置を補正
		pos->z = GetLimitStage().fFar + (CAR_DEPTH * 2);
	}
	if (pos->x > GetLimitStage().fRight - (CAR_WIDTH * 2))
	{ // 範囲外の場合 (右)

		// 右に位置を補正
		pos->x = GetLimitStage().fRight - (CAR_WIDTH * 2);
	}
	if (pos->x < GetLimitStage().fLeft + (CAR_WIDTH * 2))
	{ // 範囲外の場合 (左)

		// 左に位置を補正
		pos->x = GetLimitStage().fLeft + (CAR_WIDTH * 2);
	}
}

//============================================================
// 車の曲がる処理
//============================================================
void CurveCar(Car *pCar)
{
	switch (pCar->carCurveInfo.actionState)
	{
	case CARACT_DASH:		// 走行状態

		// 車の走行処理
		DashCarAction(pCar);

		break;				// 抜け出す

	case CARACT_CURVE:		// カーブ状態

		// 移動している状態にする
		pCar->bMove = false;

		// 車の角度更新・補正処理
		CurveInfoRotCar(&pCar->carCurveInfo, &pCar->rot, &pCar->move, &pCar->pos);

		break;				// 抜け出す
	}

	if (pCar->move.x > MAX_CAR_FORWARD_PATROL)
	{ // プレイヤーの移動量 (x) が一定値以上の場合

	  // プレイヤーの移動量 (x) を補正
		pCar->move.x = MAX_CAR_FORWARD_PATROL;
	}
}

//============================================================
// 車の走行処理
//============================================================
void DashCarAction(Car *pCar)
{
	// 移動量を更新
	pCar->move.x += CAR_MOVE_FORWARD;

	// 移動している状態にする
	pCar->bMove = true;

	for (int nCnt = 0; nCnt < MAX_CURVEPOINT; nCnt++)
	{
		switch (pCar->carCurveInfo.curveInfo.dashAngle)
		{
		case DASH_RIGHT:		//右に走っている場合

			//這わせる
			pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->pos.z == GetCurveInfo(nCnt).pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
			{ // 同じZ軸上を走っている場合
				if (pCar->pos.x >= GetCurveInfo(nCnt).pos.x - (CAR_WIDTH * 2) &&
					pCar->posOld.x <= GetCurveInfo(nCnt).pos.x - (CAR_WIDTH * 2))
				{ // 位置が一致した場合
					if (GetCurveInfo(nCnt).dashAngle == DASH_RIGHT)
					{ // 右に走る場合のみ
						// スキップカウントを減算する
						pCar->carCurveInfo.nSKipCnt--;

						if (pCar->carCurveInfo.nSKipCnt == 0 || GetCurveInfo(nCnt).bDeadEnd == true)
						{ // スキップ回数が0になったまたは、行き止まりだった場合
							// スキップ回数を0にする
							pCar->carCurveInfo.nSKipCnt = 0;

							// 曲がり角の情報を更新する
							pCar->carCurveInfo.curveInfo = GetCurveInfo(nCnt);
						}
					}
				}
			}

			break;				//抜け出す

		case DASH_LEFT:			//左を走っている場合

			//這わせる
			pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->pos.z == GetCurveInfo(nCnt).pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
			{ // 同じZ軸上を走っている場合
				if (pCar->pos.x <= GetCurveInfo(nCnt).pos.x + (CAR_WIDTH * 2) &&
					pCar->posOld.x >= GetCurveInfo(nCnt).pos.x + (CAR_WIDTH * 2))
				{ // 位置が一致した場合
					if (GetCurveInfo(nCnt).dashAngle == DASH_LEFT)
					{ // 左に走る場合のみ
						// スキップカウントを減算する
						pCar->carCurveInfo.nSKipCnt--;

						if (pCar->carCurveInfo.nSKipCnt == 0 || GetCurveInfo(nCnt).bDeadEnd == true)
						{ // スキップ回数が0になった場合
							// スキップ回数を0にする
							pCar->carCurveInfo.nSKipCnt = 0;

							// 曲がり角の情報を更新する
							pCar->carCurveInfo.curveInfo = GetCurveInfo(nCnt);
						}
					}
				}
			}

			break;				//抜け出す

		case DASH_FAR:			//奥に走っている場合

			//這わせる
			pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->pos.x == GetCurveInfo(nCnt).pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
			{ // 同じZ軸上を走っている場合
				if (pCar->pos.z >= GetCurveInfo(nCnt).pos.z - (CAR_WIDTH * 2) &&
					pCar->posOld.z <= GetCurveInfo(nCnt).pos.z - (CAR_WIDTH * 2))
				{ // 位置が一致した場合
					if (GetCurveInfo(nCnt).dashAngle == DASH_FAR)
					{ // 奥に走る場合のみ
						// スキップカウントを減算する
						pCar->carCurveInfo.nSKipCnt--;

						if (pCar->carCurveInfo.nSKipCnt == 0 || GetCurveInfo(nCnt).bDeadEnd == true)
						{ // スキップ回数が0になった場合
							// スキップ回数を0にする
							pCar->carCurveInfo.nSKipCnt = 0;

							// 曲がり角の情報を更新する
							pCar->carCurveInfo.curveInfo = GetCurveInfo(nCnt);
						}
					}
				}
			}

			break;				//抜け出す

		case DASH_NEAR:			//奥に走っている場合

			//這わせる
			pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->pos.x == GetCurveInfo(nCnt).pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
			{ // 同じZ軸上を走っている場合
				if (pCar->pos.z <= GetCurveInfo(nCnt).pos.z + (CAR_WIDTH * 2) &&
					pCar->posOld.z >= GetCurveInfo(nCnt).pos.z + (CAR_WIDTH * 2))
				{ // 位置が一致した場合
					if (GetCurveInfo(nCnt).dashAngle == DASH_NEAR)
					{ // 手前に走る場合のみ
						// スキップカウントを減算する
						pCar->carCurveInfo.nSKipCnt--;

						if (pCar->carCurveInfo.nSKipCnt == 0 || GetCurveInfo(nCnt).bDeadEnd == true)
						{ // スキップ回数が0になった場合
							// スキップ回数を0にする
							pCar->carCurveInfo.nSKipCnt = 0;

							// 曲がり角の情報を更新する
							pCar->carCurveInfo.curveInfo = GetCurveInfo(nCnt);

							if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_LEFT)
							{ // 曲がる方向が左方向だった場合
							  // 角度を補正する
								pCar->rot.y = D3DX_PI;
							}
							else
							{ // 曲がる方向が右方向だった場合
							  // 角度を補正する
								pCar->rot.y = -D3DX_PI;
							}
						}
					}
				}
			}

			break;				//抜け出す
		}
	}

	if (pCar->carCurveInfo.nSKipCnt == 0)
	{ // スキップカウントが0の場合
		switch (pCar->carCurveInfo.curveInfo.dashAngle)
		{
		case DASH_RIGHT:			//右に走っている

			//這わせる
			pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_RIGHT)
			{ // 右に曲がる場合
				if (pCar->pos.x >= pCar->carCurveInfo.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // 車の位置が曲がる位置に達した場合
					// 位置を補正する
					pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// カーブ状態にする
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}
			else if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_LEFT)
			{ // 左に曲がる場合
				if (pCar->pos.x >= pCar->carCurveInfo.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // 車の位置が曲がる位置に達した場合
					// 位置を補正する
					pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// カーブ状態にする
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}

			break;					//抜け出す

		case DASH_LEFT:				//左に走っている

			//這わせる
			pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_RIGHT)
			{ // 右に曲がる場合
				if (pCar->pos.x <= pCar->carCurveInfo.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // 車の位置が曲がる位置に達した場合
					// 位置を補正する
					pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// カーブ状態にする
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}
			else if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_LEFT)
			{ // 左に曲がる場合
				if (pCar->pos.x <= pCar->carCurveInfo.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // 車の位置が曲がる位置に達した場合
					// 位置を補正する
					pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// カーブ状態にする
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}

			break;					//抜け出す

		case DASH_FAR:				//奥に走っている

			//這わせる
			pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_RIGHT)
			{ // 右に曲がる場合
				if (pCar->pos.z >= pCar->carCurveInfo.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // 車の位置が曲がる位置に達した場合
					// 位置を補正する
					pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// カーブ状態にする
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}
			else if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_LEFT)
			{ // 左に曲がる場合
				if (pCar->pos.z >= pCar->carCurveInfo.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // 車の位置が曲がる位置に達した場合
					// 位置を補正する
					pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// カーブ状態にする
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}

			break;					//抜け出す

		case DASH_NEAR:				//手前に走っている

			//這わせる
			pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_RIGHT)
			{ // 右に曲がる場合
				if (pCar->pos.z <= pCar->carCurveInfo.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // 車の位置が曲がる位置に達した場合
					// 位置を補正する
					pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// カーブ状態にする
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}
			else if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_LEFT)
			{ // 左に曲がる場合
				if (pCar->pos.z <= pCar->carCurveInfo.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // 車の位置が曲がる位置に達した場合
					// 位置を補正する
					pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// カーブ状態にする
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}

			break;					//抜け出す
		}
	}
}

//============================================================
// 車の停止処理
//============================================================
void CollisionStopCar(D3DXVECTOR3 targetpos, D3DXVECTOR3 targetrot, D3DXVECTOR3 *move, float fTargetRadius, COLLOBJECTTYPE collObject, int *pTraCnt)
{
	D3DXVECTOR3 stopCarpos =
		D3DXVECTOR3(
			targetpos.x + sinf(targetrot.y) * CAR_STOP_RADIUS_DIST,
			0.0f,
			targetpos.z + cosf(targetrot.y) * CAR_STOP_RADIUS_DIST
		);				// 止まる車の位置

	float fLength;										// 長さの変数

	{// 警察との当たり判定
		Police *pPolice = GetPoliceData();					// 警察の情報を取得する

		for (int nCntPoli = 0; nCntPoli < MAX_POLICE; nCntPoli++, pPolice++)
		{
			if (pPolice->bUse == true)
			{ // 使用している場合
				// 長さを測る
				fLength = (pPolice->pos.x - stopCarpos.x) * (pPolice->pos.x - stopCarpos.x)
					+ (pPolice->pos.z - stopCarpos.z) * (pPolice->pos.z - stopCarpos.z);

				if (fLength <= (pPolice->modelData.fRadius + CAR_STOP_ADD_RADIUS) * (fTargetRadius + CAR_STOP_ADD_RADIUS))
				{ // オブジェクトが当たっている
					switch (collObject)
					{
					case COLLOBJECTTYPE_PLAYER:		//プレイヤーの場合

						break;						//抜け出す

					case COLLOBJECTTYPE_POLICE:		//車の場合

						// 目標の移動量をセーブする
						move->x += (0.0f - move->x) * 0.5f;

						// カウントを加算する
						*pTraCnt += 1;

						break;						//抜け出す

					case COLLOBJECTTYPE_CAR:		//車の場合

						// 目標の移動量をセーブする
						move->x = 0.0f;

						// カウントを加算する
						*pTraCnt += 1;

						break;						//抜け出す
					}
				}
			}
		}
	}

	{//プレイヤーとの当たり判定
		Player *pPlayer = GetPlayer();				//プレイヤーの情報を取得する

		if (pPlayer->bUse == true)
		{ // 使用している場合
		  // 長さを測る
			fLength = (pPlayer->pos.x - stopCarpos.x) * (pPlayer->pos.x - stopCarpos.x)
				+ (pPlayer->pos.z - stopCarpos.z) * (pPlayer->pos.z - stopCarpos.z);

			if (fLength <= (pPlayer->modelData.fRadius + CAR_STOP_ADD_RADIUS) * (fTargetRadius + CAR_STOP_ADD_RADIUS))
			{ // オブジェクトが当たっている

				switch (collObject)
				{
				case COLLOBJECTTYPE_PLAYER:		//プレイヤーの場合

					break;						//抜け出す

				case COLLOBJECTTYPE_POLICE:		//車の場合

					// 目標の移動量をセーブする
					move->x = move->x;

					break;						//抜け出す

				case COLLOBJECTTYPE_CAR:		//車の場合

					// 目標の移動量をセーブする
					move->x = sinf(targetrot.y) * -8.0f;

					break;						//抜け出す
				}
			}
		}
	}

	{//車との当たり判定
		Car *pCar = GetCarData();				//車の情報を取得する

		for (int nCntPoli = 0; nCntPoli < MAX_CAR; nCntPoli++, pCar++)
		{
			if (pCar->bUse == true)
			{ // 使用している場合
				// 長さを測る
				fLength = (pCar->pos.x - stopCarpos.x) * (pCar->pos.x - stopCarpos.x)
					+ (pCar->pos.z - stopCarpos.z) * (pCar->pos.z - stopCarpos.z);

				if (fLength <= (pCar->modelData.fRadius + CAR_STOP_ADD_RADIUS) * (fTargetRadius + CAR_STOP_ADD_RADIUS))
				{ // オブジェクトが当たっている
					switch (collObject)
					{
					case COLLOBJECTTYPE_PLAYER:		//プレイヤーの場合

						break;						//抜け出す

					case COLLOBJECTTYPE_POLICE:		//車の場合

						// 目標の移動量をセーブする
						move->x += (0.0f - move->x) * 0.5f;

						// カウントを加算する
						*pTraCnt += 1;

						break;						//抜け出す

					case COLLOBJECTTYPE_CAR:		//車の場合

						// 目標の移動量をセーブする
						move->x = 0.0f;

						// カウントを加算する
						*pTraCnt += 1;

						break;						//抜け出す
					}
				}
			}
		}
	}
}

//============================================================
// 車同士の当たり判定
//============================================================
void CollisionCarBody(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 rot, D3DXVECTOR3 *pMove, float fWidth, float fDepth, COLLOBJECTTYPE collObject, int *pTraCnt)
{
	// プレイヤーとの当たり判定
	CarBodyStopPlayer(pPos, pPosOld, rot, pMove, fWidth, fDepth, collObject, pTraCnt);

	if (collObject != COLLOBJECTTYPE_PLAYER)
	{ // プレイヤー以外の場合
		// 車との当たり判定
		CarBodyStopCar(pPos, pPosOld, rot, pMove, fWidth, fDepth, collObject, pTraCnt);

		// 警察との当たり判定
		CarBodyStopPolice(pPos, pPosOld, rot, pMove, fWidth, fDepth, collObject, pTraCnt);
	}
}

//============================================================
// 車の位置と向きの設定処理
//============================================================
void SetCarPosRot(Car *pCar)
{
	float fCurveDist;			// 最近の曲がり角との距離
	int nCurveNumber = 0;		// 最近の曲がり角の番号

	fCurveDist = fabsf(sqrtf((pCar->pos.x - GetCurveInfo(0).pos.x) * (pCar->pos.x - GetCurveInfo(0).pos.x) +
		(pCar->pos.z - GetCurveInfo(0).pos.z) * (pCar->pos.z - GetCurveInfo(0).pos.z)));

	for (int nCnt = 1; nCnt < MAX_CURVEPOINT; nCnt++)
	{ // 全ての曲がり角と参照

		float fCurvePoint;		// 曲がり角の値

		// カーブの距離
		fCurvePoint = fabsf(sqrtf((pCar->pos.x - GetCurveInfo(nCnt).pos.x) * (pCar->pos.x - GetCurveInfo(nCnt).pos.x) +
			(pCar->pos.z - GetCurveInfo(nCnt).pos.z) * (pCar->pos.z - GetCurveInfo(nCnt).pos.z)));

		if (fCurvePoint <= fCurveDist)
		{ // 距離の近さが更新された場合
			// 最近値を更新する
			fCurveDist = fCurvePoint;

			// 番号を更新する
			nCurveNumber = nCnt;
		}
	}

	// 曲がり角の情報を代入する
	pCar->carCurveInfo.curveInfo = GetCurveInfo(nCurveNumber);

	// 車の位置の補正処理
	CarPosRotCorrect(pCar);
}

//============================================================
// 車の位置の補正処理
//============================================================
void CarPosRotCorrect(Car *pCar)
{
	switch (pCar->carCurveInfo.curveInfo.dashAngle)
	{
	case DASH_RIGHT:		// 右に向かって走る

		//這わせる
		pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

		// 向きを変える
		pCar->rot.y = D3DX_PI * 0.5f;

		break;				// 抜け出す

	case DASH_LEFT:			// 左に向かって走る

		//這わせる
		pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

		// 向きを変える
		pCar->rot.y = -D3DX_PI * 0.5f;

		break;				// 抜け出す

	case DASH_FAR:			// 奥に向かって走る

		//這わせる
		pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

		// 向きを変える
		pCar->rot.y = 0.0f;

		break;				// 抜け出す

	case DASH_NEAR:			// 手前に向かって走る

		//這わせる
		pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

		switch (pCar->carCurveInfo.curveInfo.curveAngle)
		{
		case CURVE_RIGHT:	// 右に曲がる

			// 向きを変える
			pCar->rot.y = -D3DX_PI;

			break;			// 抜け出す

		case CURVE_LEFT:	// 左に曲がる

			// 向きを変える
			pCar->rot.y = D3DX_PI;

			break;			// 抜け出す
		}

		break;				// 抜け出す
	}
}

//============================================================
// プレイヤーとの当たり判定
//============================================================
void CarBodyStopPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 rot, D3DXVECTOR3 *pMove, float fWidth, float fDepth, COLLOBJECTTYPE collObject, int *pTraCnt)
{
	Player *pPlayer = GetPlayer();				// 車の情報を取得する

	if (pPlayer->bUse == true)
	{ // プレイヤーが使用されていた場合
		if (pPos->x - fWidth <= pPlayer->pos.x + PLAY_WIDTH
			&& pPos->x + fWidth >= pPlayer->pos.x - PLAY_WIDTH)
		{ // 車のX幅の中にいた場合
			if (pPosOld->z + fDepth <= pPlayer->oldPos.z - PLAY_DEPTH
				&& pPos->z + fDepth >= pPlayer->pos.z - PLAY_DEPTH)
			{//前回の位置がブロックより手前かつ、現在の位置がブロックよりも奥かつだった場合(手前で止められる処理)
				switch (collObject)
				{
				case COLLOBJECTTYPE_POLICE:		// 警察の場合

					//位置をずらす
					pPlayer->pos.z = pPos->z + (PLAY_DEPTH + fDepth);
					
					//プレイヤーの移動量を削除
					pPlayer->move.x *= 0.95f;

					// 移動量を削除
					pMove->x *= 0.95f;

					break;						// 抜け出す

				case COLLOBJECTTYPE_CAR:		// 車の場合

					//位置をずらす
					pPlayer->pos.z = pPos->z + (PLAY_DEPTH + fDepth);

					//プレイヤーの移動量を削除
					pPlayer->move.x *= 0.95f;

					// 移動量を削除
					pMove->x = 0.0f;

					break;						// 抜け出す
				}
			}							//手前で止められる処理
			else if (pPosOld->z - fDepth >= pPlayer->oldPos.z + PLAY_DEPTH
				&& pPos->z - fDepth <= pPlayer->pos.z + PLAY_DEPTH)
			{//前回の位置がプレイヤーの位置よりも奥かつ、現在の位置がプレイヤーの位置よりも手前だった場合(奥で止められる処理)
				switch (collObject)
				{
				case COLLOBJECTTYPE_POLICE:		// 警察の場合

					//位置をずらす
					pPlayer->pos.z = pPos->z - (PLAY_DEPTH + fDepth);

					//プレイヤーの移動量を削除
					pPlayer->move.x *= 0.95f;

					// 移動量を削除
					pMove->x *= 0.95f;

					break;						// 抜け出す

				case COLLOBJECTTYPE_CAR:		// 車の場合

					//位置をずらす
					pPlayer->pos.z = pPos->z - (PLAY_DEPTH + fDepth);

					//プレイヤーの移動量を削除
					pPlayer->move.x *= 0.95f;

					// 移動量を削除
					pMove->x = 0.0f;

					break;						// 抜け出す
				}
			}							//奥で止められる処理
		}

		if (pPos->z - fDepth <= pPlayer->pos.z + PLAY_DEPTH
			&& pPos->z + fDepth >= pPlayer->pos.z - PLAY_DEPTH)
		{//塔のZ幅の中にいた場合
			if (pPosOld->x + fWidth <= pPlayer->oldPos.x - PLAY_WIDTH
				&& pPos->x + fWidth >= pPlayer->pos.x - PLAY_WIDTH)
			{//前回の位置がブロックの左端より左かつ、現在の位置がブロックの左側より右だった場合(左の処理)
				switch (collObject)
				{
				case COLLOBJECTTYPE_POLICE:		// 警察の場合

					//位置をずらす
					pPlayer->pos.x = pPos->x + (PLAY_WIDTH + fWidth);

					//プレイヤーの移動量を削除
					pPlayer->move.x *= 0.95f;

					// 移動量を削除
					pMove->x *= 0.95f;

					break;						// 抜け出す

				case COLLOBJECTTYPE_CAR:		// 車の場合

					//位置をずらす
					pPlayer->pos.x = pPos->x + (PLAY_WIDTH + fWidth);

					//プレイヤーの移動量を削除
					pPlayer->move.x *= 0.95f;

					// 移動量を削除
					pMove->x = 0.0f;

					break;						// 抜け出す
				}
			}							//左端の処理
			else if (pPosOld->x - fWidth >= pPlayer->oldPos.x + PLAY_WIDTH
				&& pPos->x - fWidth <= pPlayer->pos.x + PLAY_WIDTH)
			{//前回の位置がブロックの右端より右かつ、現在の位置がブロックの左側より右だった場合(右の処理)
				switch (collObject)
				{
				case COLLOBJECTTYPE_POLICE:		// 警察の場合

					//位置をずらす
					pPlayer->pos.x = pPos->x - (PLAY_WIDTH + fWidth);

					//プレイヤーの移動量を削除
					pPlayer->move.x *= 0.95f;

					// 移動量を削除
					pMove->x *= 0.95f;

					break;						// 抜け出す

				case COLLOBJECTTYPE_CAR:		// 車の場合

					//位置をずらす
					pPlayer->pos.x = pPos->x - (PLAY_WIDTH + fWidth);

					//プレイヤーの移動量を削除
					pPlayer->move.x *= 0.95f;

					// 移動量を削除
					pMove->x = 0.0f;

					break;						// 抜け出す
				}
			}							//右端の処理
		}
	}
}

//============================================================
// 車との当たり判定
//============================================================
void CarBodyStopCar(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 rot, D3DXVECTOR3 *pMove, float fWidth, float fDepth, COLLOBJECTTYPE collObject, int *pTraCnt)
{
	Car *pCar = GetCarData();				// 車の情報を取得する

	for (int nCnt = 0; nCnt < MAX_CAR; nCnt++, pCar++)
	{
		if (pCar->bUse == true)
		{ // 車が使用されていた場合
			if (pPos->x - fWidth <= pCar->pos.x + CAR_WIDTH
				&& pPos->x + fWidth >= pCar->pos.x - CAR_WIDTH)
			{ // 車のX幅の中にいた場合
				if (pPosOld->z + fDepth < pCar->posOld.z - CAR_DEPTH
					&& pPos->z + fDepth > pCar->pos.z - CAR_DEPTH)
				{//前回の位置がブロックより手前かつ、現在の位置がブロックよりも奥かつだった場合(手前で止められる処理)
					switch (collObject)
					{
					case COLLOBJECTTYPE_POLICE:		// 警察の場合

						//位置をずらす
						pCar->pos.z = pPos->z + (CAR_DEPTH + fDepth);

						// 移動量を削除
						pMove->x *= 0.95f;

						// カウントを加算する
						*pTraCnt += 1;

						break;						// 抜け出す

					case COLLOBJECTTYPE_CAR:		// 車の場合

						//位置をずらす
						pCar->pos.z = pPos->z + (CAR_DEPTH + fDepth);

						// 移動量を削除
						pMove->x *= 0.95f;

						// カウントを加算する
						*pTraCnt += 1;

						break;						// 抜け出す
					}
				}							//手前で止められる処理
				else if (pPosOld->z - fDepth >= pCar->posOld.z + CAR_DEPTH
					&& pPos->z - fDepth <= pCar->pos.z + CAR_DEPTH)
				{//前回の位置がプレイヤーの位置よりも奥かつ、現在の位置がプレイヤーの位置よりも手前だった場合(奥で止められる処理)
					switch (collObject)
					{
					case COLLOBJECTTYPE_POLICE:		// 警察の場合

						//位置をずらす
						pCar->pos.z = pPos->z - (CAR_DEPTH + fDepth);

						// 移動量を削除
						pMove->x *= 0.95f;

						// カウントを加算する
						*pTraCnt += 1;

						break;						// 抜け出す

					case COLLOBJECTTYPE_CAR:		// 車の場合

						//位置をずらす
						pCar->pos.z = pPos->z - (CAR_DEPTH + fDepth);

						// 移動量を削除
						pMove->x *= 0.95f;

						// カウントを加算する
						*pTraCnt += 1;

						break;						// 抜け出す
					}
				}							//奥で止められる処理
			}

			if (pPos->z - fDepth <= pCar->pos.z + CAR_DEPTH
				&& pPos->z + fDepth >= pCar->pos.z - CAR_DEPTH)
			{//塔のZ幅の中にいた場合
				if (pPosOld->x + fWidth <= pCar->posOld.x - CAR_WIDTH
					&& pPos->x + fWidth >= pCar->pos.x - CAR_WIDTH)
				{//前回の位置がブロックの左端より左かつ、現在の位置がブロックの左側より右だった場合(左の処理)
					switch (collObject)
					{
					case COLLOBJECTTYPE_POLICE:		// 警察の場合

						//位置をずらす
						pCar->pos.x = pPos->x + (CAR_WIDTH + fWidth);

						// 移動量を削除
						pMove->x *= 0.95f;

						// カウントを加算する
						*pTraCnt += 1;

						break;						// 抜け出す

					case COLLOBJECTTYPE_CAR:		// 車の場合

						//位置をずらす
						pCar->pos.x = pPos->x + (CAR_WIDTH + fWidth);

						// 移動量を削除
						pMove->x *= 0.95f;

						// カウントを加算する
						*pTraCnt += 1;

						break;						// 抜け出す
					}
				}							//左端の処理
				else if (pPosOld->x - fWidth >= pCar->posOld.x + CAR_WIDTH
					&& pPos->x - fWidth <= pCar->pos.x + CAR_WIDTH)
				{//前回の位置がブロックの右端より右かつ、現在の位置がブロックの左側より右だった場合(右の処理)
					switch (collObject)
					{
					case COLLOBJECTTYPE_POLICE:		// 警察の場合

						//位置をずらす
						pCar->pos.x = pPos->x - (CAR_WIDTH + fWidth);

						// 移動量を削除
						pMove->x *= 0.95f;

						// カウントを加算する
						*pTraCnt += 1;

						break;						// 抜け出す

					case COLLOBJECTTYPE_CAR:		// 車の場合

						//位置をずらす
						pCar->pos.x = pPos->x - (CAR_WIDTH + fWidth);

						// 移動量を削除
						pMove->x *= 0.95f;

						// カウントを加算する
						*pTraCnt += 1;

						break;						// 抜け出す
					}
				}							//右端の処理
			}
		}
	}
}

//============================================================
// 警察との当たり判定
//============================================================
void CarBodyStopPolice(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 rot, D3DXVECTOR3 *pMove, float fWidth, float fDepth, COLLOBJECTTYPE collObject, int *pTraCnt)
{
	Police *pPolice = GetPoliceData();			// 警察の情報を取得する

	for (int nCnt = 0; nCnt < MAX_POLICE; nCnt++, pPolice++)
	{
		if (pPolice->bUse == true)
		{ // 車が使用されていた場合
			if (pPos->x - fWidth <= pPolice->pos.x + POLICAR_WIDTH
				&& pPos->x + fWidth >= pPolice->pos.x - POLICAR_WIDTH)
			{ // 車のX幅の中にいた場合
				if (pPosOld->z + fDepth < pPolice->posOld.z - POLICAR_DEPTH
					&& pPos->z + fDepth > pPolice->pos.z - POLICAR_DEPTH)
				{//前回の位置がブロックより手前かつ、現在の位置がブロックよりも奥かつだった場合(手前で止められる処理)
					switch (collObject)
					{
					case COLLOBJECTTYPE_POLICE:		// 警察の場合

						//位置をずらす
						pPolice->pos.z = pPos->z + (POLICAR_DEPTH + fDepth);

						// 移動量を削除
						pMove->x *= 0.95f;

						// カウントを加算する
						*pTraCnt += 1;

						break;						// 抜け出す

					case COLLOBJECTTYPE_CAR:		// 車の場合

						//位置をずらす
						pPolice->pos.z = pPos->z + (POLICAR_DEPTH + fDepth);

						// 移動量を削除
						pMove->x *= 0.95f;

						// カウントを加算する
						*pTraCnt += 1;

						break;						// 抜け出す
					}
				}							//手前で止められる処理
				else if (pPosOld->z - fDepth >= pPolice->posOld.z + POLICAR_DEPTH
					&& pPos->z - fDepth <= pPolice->pos.z + POLICAR_DEPTH)
				{//前回の位置がプレイヤーの位置よりも奥かつ、現在の位置がプレイヤーの位置よりも手前だった場合(奥で止められる処理)
					switch (collObject)
					{
					case COLLOBJECTTYPE_POLICE:		// 警察の場合

						//位置をずらす
						pPolice->pos.z = pPos->z - (POLICAR_DEPTH + fDepth);

						// 移動量を削除
						pMove->x *= 0.95f;

						// カウントを加算する
						*pTraCnt += 1;

						break;						// 抜け出す

					case COLLOBJECTTYPE_CAR:		// 車の場合

						//位置をずらす
						pPolice->pos.z = pPos->z - (POLICAR_DEPTH + fDepth);

						// 移動量を削除
						pMove->x *= 0.95f;

						// カウントを加算する
						*pTraCnt += 1;

						break;						// 抜け出す
					}
				}							//奥で止められる処理
			}

			if (pPos->z - fDepth <= pPolice->pos.z + POLICAR_DEPTH
				&& pPos->z + fDepth >= pPolice->pos.z - POLICAR_DEPTH)
			{//塔のZ幅の中にいた場合
				if (pPosOld->x + fWidth <= pPolice->posOld.x - POLICAR_WIDTH
					&& pPos->x + fWidth >= pPolice->pos.x - POLICAR_WIDTH)
				{//前回の位置がブロックの左端より左かつ、現在の位置がブロックの左側より右だった場合(左の処理)
					switch (collObject)
					{
					case COLLOBJECTTYPE_POLICE:		// 警察の場合

						//位置をずらす
						pPolice->pos.x = pPos->x + (POLICAR_WIDTH + fWidth);

						// 移動量を削除
						pMove->x *= 0.95f;

						// カウントを加算する
						*pTraCnt += 1;

						break;						// 抜け出す

					case COLLOBJECTTYPE_CAR:		// 車の場合

						//位置をずらす
						pPolice->pos.x = pPos->x + (POLICAR_WIDTH + fWidth);

						// 移動量を削除
						pMove->x *= 0.95f;

						// カウントを加算する
						*pTraCnt += 1;

						break;						// 抜け出す
					}
				}							//左端の処理
				else if (pPosOld->x - fWidth >= pPolice->posOld.x + POLICAR_WIDTH
					&& pPos->x - fWidth <= pPolice->pos.x + POLICAR_WIDTH)
				{//前回の位置がブロックの右端より右かつ、現在の位置がブロックの左側より右だった場合(右の処理)
					switch (collObject)
					{
					case COLLOBJECTTYPE_POLICE:		// 警察の場合

						//位置をずらす
						pPolice->pos.x = pPos->x - (POLICAR_WIDTH + fWidth);

						// 移動量を削除
						pMove->x *= 0.95f;

						// カウントを加算する
						*pTraCnt += 1;

						break;						// 抜け出す

					case COLLOBJECTTYPE_CAR:		// 車の場合

						//位置をずらす
						pPolice->pos.x = pPos->x - (POLICAR_WIDTH + fWidth);

						// 移動量を削除
						pMove->x *= 0.95f;

						// カウントを加算する
						*pTraCnt += 1;

						break;						// 抜け出す
					}
				}							//右端の処理
			}
		}
	}
}

//============================================================
// 車の渋滞改善処理
//============================================================
void CarTrafficImprove(Car *pCar)
{
	// 渋滞カウントを加算する
	pCar->nTrafficCnt++;

	if (pCar->nTrafficCnt >= CAR_TRAFFIC_IMPROVE_CNT)
	{ // 渋滞カウントが解除状態に入った場合
		// カウントを0にする
		pCar->nTrafficCnt = 0;

		// 通常状態にする
		pCar->state = CARSTATE_NORMAL;
	}
}

#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================
#endif