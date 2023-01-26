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

#include "Car.h"
#include "shadow.h"
#include "sound.h"
#include "player.h"
#include "Police.h"
#include "object.h"

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
#define CAR_WIDTH				(45.0f)		// 車の縦幅
#define CAR_DEPTH				(45.0f)		// 車の奥行

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void PosCar(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove);		// 車の位置の更新処理
void RevCar(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos);	// 車の補正の更新処理
void CurveCar(Car *pCar);							// 車のカーブ処理

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

		g_aCar[nCntCar].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aCar[nCntCar].posOld = g_aCar[nCntCar].pos;			// 前回の位置
		g_aCar[nCntCar].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
		g_aCar[nCntCar].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
		g_aCar[nCntCar].nShadowID = NONE_SHADOW;				// 影のインデックス
		g_aCar[nCntCar].bMove = false;							// 移動しているか
		g_aCar[nCntCar].bUse = false;							// 使用状況

		//曲がり角の情報を初期化
		for (int nCntCurve = 0; nCntCurve < MAX_CURVE; nCntCurve++)
		{
			g_aCar[nCntCar].carCurve.curveAngle[nCntCurve] = CURVE_RIGHT;						// 曲がる方向
			g_aCar[nCntCar].carCurve.curvePoint[nCntCurve] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 曲がるポイント
			g_aCar[nCntCar].carCurve.nNowCurve = 0;												// 現在の曲がり角
			g_aCar[nCntCar].carCurve.bCurveX[nCntCurve] = false;								// X軸を走っているか
			g_aCar[nCntCar].carCurve.fCurveRot[nCntCurve] = 0;									// 目標の向き
		}
		g_aCar[nCntCar].carCurve.nCurveTime = 0;				// 曲がる回数
		g_aCar[nCntCar].carCurve.nNowCurve = 0;					// 現在のルート

		// モデル情報の初期化
		g_aCar[nCntCar].modelData.dwNumMat = 0;					// マテリアルの数
		g_aCar[nCntCar].modelData.pTexture = NULL;				// テクスチャへのポインタ
		g_aCar[nCntCar].modelData.pMesh = NULL;					// メッシュ (頂点情報) へのポインタ
		g_aCar[nCntCar].modelData.pBuffMat = NULL;				// マテリアルへのポインタ
		g_aCar[nCntCar].modelData.vtxMin = INIT_VTX_MIN;		// 最小の頂点座標
		g_aCar[nCntCar].modelData.vtxMax = INIT_VTX_MAX;		// 最大の頂点座標
		g_aCar[nCntCar].modelData.size = INIT_SIZE;				// 大きさ
		g_aCar[nCntCar].modelData.fRadius = 0.0f;				// 半径
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

			// 前回位置の更新
			g_aCar[nCntCar].posOld = g_aCar[nCntCar].pos;

			//----------------------------------------------------
			//	影の更新
			//----------------------------------------------------
			// 影の位置設定
			SetPositionShadow
			( // 引数
				g_aCar[nCntCar].nShadowID,	// 影のインデックス
				g_aCar[nCntCar].pos,			// 位置
				g_aCar[nCntCar].rot,			// 向き
				NONE_SCALE							// 拡大率
			);

			//車のカーブ処理
			CurveCar(&g_aCar[nCntCar]);

			// プレイヤーの位置の更新
			PosCar(&g_aCar[nCntCar].move, &g_aCar[nCntCar].pos, &g_aCar[nCntCar].rot, g_aCar[nCntCar].bMove);

			if (g_aCar[nCntCar].pos.y < 0.0f)
			{//Y軸の位置が0.0fだった場合
				//縦への移動量を0.0fにする
				g_aCar[nCntCar].move.y = 0.0f;

				//位置を0.0fに戻す
				g_aCar[nCntCar].pos.y = 0.0f;
			}

			// プレイヤーの補正の更新処理
			RevCar(&g_aCar[nCntCar].rot, &g_aCar[nCntCar].pos);

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

			// 車の停止処理
			CollisionStopCar
			( // 引数
				g_aCar[nCntCar].pos,		//位置
				g_aCar[nCntCar].rot,		//向き
				&g_aCar[nCntCar].move,		//移動量
				g_aCar[nCntCar].modelData.fRadius,	//半径
				COLLOBJECTTYPE_CAR			//対象のサイズ
			);

			// 車同士の当たり判定
			CollisionCarBody
			( // 引数
				&g_aCar[nCntCar].pos, 
				&g_aCar[nCntCar].posOld,
				g_aCar[nCntCar].rot,
				&g_aCar[nCntCar].move,
				g_aCar[nCntCar].modelData,
				COLLOBJECTTYPE_CAR
			);
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
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ

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

				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

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
void SetCar(D3DXVECTOR3 pos, CURVE carCurve)
{
	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aCar[nCntCar].bUse == false)
		{ // オブジェクトが使用されていない場合
			// 引数を代入
			g_aCar[nCntCar].pos = pos;								// 現在の位置
			g_aCar[nCntCar].posOld = g_aCar[nCntCar].pos;			// 前回の位置
			g_aCar[nCntCar].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
			g_aCar[nCntCar].bMove = false;							// 移動していない

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

			//曲がり角関係の設定
			for (int nCntCurve = 0; nCntCurve < carCurve.nCurveTime; nCntCurve++)
			{
				g_aCar[nCntCar].carCurve.fCurveRot[nCntCurve] = carCurve.fCurveRot[nCntCurve];		// 次曲がる目標の方向
				g_aCar[nCntCar].carCurve.curvePoint[nCntCurve] = carCurve.curvePoint[nCntCurve];	// 次曲がる位置
			}
			g_aCar[nCntCar].carCurve.nNowCurve = 0;													// 現在のルート
			g_aCar[nCntCar].carCurve.nCurveTime = carCurve.nCurveTime;								// 曲がる回数

			//カーブポイントの設定処理
			SetCurvePoint(&g_aCar[nCntCar].carCurve, &g_aCar[nCntCar].rot, &g_aCar[nCntCar].pos);

			if (g_aCar[nCntCar].carCurve.bCurveX[0] == true)
			{ // 最初X軸を走っている場合
				if (g_aCar[nCntCar].carCurve.bCurvePlus[0] == true)
				{ // 右に走っている場合
					// 位置を補正する
					g_aCar[nCntCar].pos.z = g_aCar[nCntCar].carCurve.curvePoint[0].z - (CAR_WIDTH * 2);
				}
				else
				{ // 左に走っている場合
					// 位置を補正する
					g_aCar[nCntCar].pos.z = g_aCar[nCntCar].carCurve.curvePoint[0].z + (CAR_WIDTH * 2);
				}
			}
			else
			{ // 最初Z軸を走っている場合
				if (g_aCar[nCntCar].carCurve.bCurvePlus[0] == true)
				{ // 奥に走っている場合
					// 位置を補正する
					g_aCar[nCntCar].pos.x = g_aCar[nCntCar].carCurve.curvePoint[0].x + (CAR_WIDTH * 2);
				}
				else
				{ // 手前に走っている場合
					// 位置を補正する
					g_aCar[nCntCar].pos.x = g_aCar[nCntCar].carCurve.curvePoint[0].x - (CAR_WIDTH * 2);
				}
			}

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
		move->x += (0.0f - move->x) * 0.04f;
	}
}

//============================================================
//	プレイヤーの補正の更新処理
//============================================================
void RevCar(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos)
{
	//--------------------------------------------------------
	//	向きの正規化
	//--------------------------------------------------------
	if (rot->y > D3DX_PI) { rot->y -= D3DX_PI * 2; }
	else if (rot->y < -D3DX_PI) { rot->y += D3DX_PI * 2; }

	//--------------------------------------------------------
	//	移動範囲の補正
	//--------------------------------------------------------
	if (pos->z > GetLimitStage().fNear - (30.0f * 2))
	{ // 範囲外の場合 (手前)

		// 手前に位置を補正
		pos->z = GetLimitStage().fNear - (30.0f * 2);
	}
	if (pos->z < GetLimitStage().fFar + (30.0f * 2))
	{ // 範囲外の場合 (奥)

		// 奥に位置を補正
		pos->z = GetLimitStage().fFar + (30.0f * 2);
	}
	if (pos->x > GetLimitStage().fRight - (30.0f * 2))
	{ // 範囲外の場合 (右)

		// 右に位置を補正
		pos->x = GetLimitStage().fRight - (30.0f * 2);
	}
	if (pos->x < GetLimitStage().fLeft + (30.0f * 2))
	{ // 範囲外の場合 (左)

		// 左に位置を補正
		pos->x = GetLimitStage().fLeft + (30.0f * 2);
	}
}

//============================================================
// 車の曲がる処理
//============================================================
void CurveCar(Car *pCar)
{
	// 移動量を更新
	pCar->move.x += CAR_MOVE_FORWARD;

	// 移動している状態にする
	pCar->bMove = true;

	if (pCar->carCurve.bCurveX[pCar->carCurve.nNowCurve] == true)
	{//X軸を走っていた場合
		if (pCar->carCurve.bCurvePlus[pCar->carCurve.nNowCurve] == true)
		{ // 右に走っている場合
			if (pCar->carCurve.curveAngle[pCar->carCurve.nNowCurve] == CURVE_RIGHT)
			{//右に曲がる場合
				if (pCar->pos.x >= pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x - (CAR_WIDTH * 2))
				{ // 左にある壁が途切れたら

					// 車の角度更新・補正処理
					CurveRotCar(&pCar->carCurve, &pCar->rot, &pCar->move);

					// 移動量を減速
					pCar->move.x += (0.0f - pCar->move.x) * REV_CAR_MOVE_SUB;

					// 右の壁に這わせる
					pCar->pos.x = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x - (CAR_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
					// 左の壁に這わせる
					pCar->pos.z = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z - (CAR_WIDTH * 2);
				}
			}
			else if (pCar->carCurve.curveAngle[pCar->carCurve.nNowCurve] == CURVE_LEFT)
			{//左に曲がる場合
				if (pCar->pos.x >= pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x + (CAR_WIDTH * 2))
				{ // 左にある壁が途切れたら

					// 車の角度更新・補正処理
					CurveRotCar(&pCar->carCurve, &pCar->rot, &pCar->move);

					// 移動量を減速
					pCar->move.x += (0.0f - pCar->move.x) * REV_CAR_MOVE_SUB;

					// 右の壁に這わせる
					pCar->pos.x = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x + (CAR_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
				  // 左の壁に這わせる
					pCar->pos.z = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z - (CAR_WIDTH * 2);
				}
			}
		}
		else
		{ // 左に走っている場合
			if (pCar->carCurve.curveAngle[pCar->carCurve.nNowCurve] == CURVE_RIGHT)
			{//右に曲がる場合
				if (pCar->pos.x <= pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x + (CAR_WIDTH * 2))
				{ // 左にある壁が途切れたら

					// 車の角度更新・補正処理
					CurveRotCar(&pCar->carCurve, &pCar->rot, &pCar->move);

					// 移動量を減速
					pCar->move.x += (0.0f - pCar->move.x) * REV_CAR_MOVE_SUB;

					// 左の壁に這わせる
					pCar->pos.x = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x + (CAR_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
					// 手前の壁に這わせる
					pCar->pos.z = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z + (CAR_WIDTH * 2);
				}
			}
			else if (pCar->carCurve.curveAngle[pCar->carCurve.nNowCurve] == CURVE_LEFT)
			{//左に曲がる場合
				if (pCar->pos.x <= pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x - (CAR_WIDTH * 2))
				{ // 左にある壁が途切れたら

					// 車の角度更新・補正処理
					CurveRotCar(&pCar->carCurve, &pCar->rot, &pCar->move);

					// 移動量を減速
					pCar->move.x += (0.0f - pCar->move.x) * REV_CAR_MOVE_SUB;

					// 左の壁に這わせる
					pCar->pos.x = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x - (CAR_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
					// 手前の壁に這わせる
					pCar->pos.z = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z + (CAR_WIDTH * 2);
				}
			}
		}
	}
	else
	{//Z軸を走っていた場合
		if (pCar->carCurve.bCurvePlus[pCar->carCurve.nNowCurve] == true)
		{ // 右の壁が警察より左側にある場合
			if (pCar->carCurve.curveAngle[pCar->carCurve.nNowCurve] == CURVE_RIGHT)
			{//右に曲がる場合
				if (pCar->pos.z >= pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z - (CAR_WIDTH * 2))
				{ // 左にある壁が途切れたら

					// 車の角度更新・補正処理
					CurveRotCar(&pCar->carCurve, &pCar->rot, &pCar->move);

					// 移動量を減速
					pCar->move.x += (0.0f - pCar->move.x) * REV_CAR_MOVE_SUB;

					// 手前の壁に這わせる
					pCar->pos.z = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z - (CAR_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
					// 右の壁に這わせる
					pCar->pos.x = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x + (CAR_WIDTH * 2);
				}
			}
			else if (pCar->carCurve.curveAngle[pCar->carCurve.nNowCurve] == CURVE_LEFT)
			{ // 左に曲がる場合
				if (pCar->pos.z >= pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z + (CAR_WIDTH * 2))
				{ // 左にある壁が途切れたら

					// 車の角度更新・補正処理
					CurveRotCar(&pCar->carCurve, &pCar->rot, &pCar->move);

					// 移動量を減速
					pCar->move.x += (0.0f - pCar->move.x) * REV_CAR_MOVE_SUB;

					// 手前の壁に這わせる
					pCar->pos.z = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z + (CAR_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
					// 右の壁に這わせる
					pCar->pos.x = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x + (CAR_WIDTH * 2);
				}
			}
		}
		else
		{//左の壁が警察より右にある場合
			if (pCar->carCurve.curveAngle[pCar->carCurve.nNowCurve] == CURVE_RIGHT)
			{//右に曲がる場合
				if (pCar->pos.z <= pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z + (CAR_WIDTH * 2))
				{ // 左にある壁が途切れたら

					// 車の角度更新・補正処理
					CurveRotCar(&pCar->carCurve, &pCar->rot, &pCar->move);

					// 移動量を減速
					pCar->move.x += (0.0f - pCar->move.x) * REV_CAR_MOVE_SUB;

					// 左の壁に這わせる
					pCar->pos.z = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z + (CAR_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
				  // 左の壁に這わせる
					pCar->pos.x = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x - (CAR_WIDTH * 2);
				}
			}
			else if (pCar->carCurve.curveAngle[pCar->carCurve.nNowCurve] == CURVE_LEFT)
			{
				if (pCar->pos.z <= pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z - (CAR_WIDTH * 2))
				{ // 左にある壁が途切れたら

					// 車の角度更新・補正処理
					CurveRotCar(&pCar->carCurve, &pCar->rot, &pCar->move);

					// 移動量を減速
					pCar->move.x += (0.0f - pCar->move.x) * REV_CAR_MOVE_SUB;

					// 左の壁に這わせる
					pCar->pos.z = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z - (CAR_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
				  // 左の壁に這わせる
					pCar->pos.x = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x - (CAR_WIDTH * 2);
				}
			}
		}
	}

	if (pCar->move.x > MAX_CAR_FORWARD_PATROL)
	{ // プレイヤーの移動量 (x) が一定値以上の場合

	  // プレイヤーの移動量 (x) を補正
		pCar->move.x = MAX_CAR_FORWARD_PATROL;
	}
}

//============================================================
// 車の停止処理
//============================================================
void CollisionStopCar(D3DXVECTOR3 targetpos, D3DXVECTOR3 targetrot, D3DXVECTOR3 *move, float fTargetRadius, COLLOBJECTTYPE collObject)
{
	D3DXVECTOR3 stopCarpos = D3DXVECTOR3(targetpos.x + sinf(targetrot.y) * 300.0f, 0.0f, targetpos.z + cosf(targetrot.y) * 300.0f);				// 止まる車の位置

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

				if (fLength <= (pPolice->modelData.fRadius + 50.0f) * (fTargetRadius + 50.0f))
				{ // オブジェクトが当たっている
					switch (collObject)
					{
					case COLLOBJECTTYPE_PLAYER:		//プレイヤーの場合

						break;						//抜け出す

					case COLLOBJECTTYPE_POLICE:		//車の場合

						// 目標の移動量をセーブする
						move->x += (0.0f - move->x) * 0.5f;

						break;						//抜け出す

					case COLLOBJECTTYPE_CAR:		//車の場合

						// 目標の移動量をセーブする
						move->x = 0.0f;

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

			if (fLength <= (pPlayer->modelData.fRadius + 50.0f) * (fTargetRadius + 50.0f))
			{ // オブジェクトが当たっている

				switch (collObject)
				{
				case COLLOBJECTTYPE_PLAYER:		//プレイヤーの場合

					break;						//抜け出す

				case COLLOBJECTTYPE_POLICE:		//車の場合

					//// 目標の移動量をセーブする
					//move->x += (0.0f - move->x) * 0.5f;

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

				if (fLength <= (pCar->modelData.fRadius + 50.0f) * (fTargetRadius + 50.0f))
				{ // オブジェクトが当たっている
					switch (collObject)
					{
					case COLLOBJECTTYPE_PLAYER:		//プレイヤーの場合

						break;						//抜け出す

					case COLLOBJECTTYPE_POLICE:		//車の場合

						// 目標の移動量をセーブする
						move->x += (0.0f - move->x) * 0.5f;

						break;						//抜け出す

					case COLLOBJECTTYPE_CAR:		//車の場合

						// 目標の移動量をセーブする
						move->x = 0.0f;

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
void CollisionCarBody(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 rot, D3DXVECTOR3 *pMove, Model ModelData, COLLOBJECTTYPE collObject)
{
	//float fAngle;			// 角度の処理

	{ // 車の当たり判定
		Car *pCar = GetCarData();					// 車の情報を取得する

		for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
		{
			if (pCar[nCntCar].bUse == true)
			{ // 車が使用されていた場合

				//// 角度を算出する
				//fAngle = atan2f()

				if (pPos->x - CAR_WIDTH <= pCar[nCntCar].pos.x + CAR_WIDTH
					&& pPos->x + CAR_WIDTH >= pCar[nCntCar].pos.x - CAR_WIDTH)
				{ // 車のX幅の中にいた場合
					if (pPosOld->z + CAR_DEPTH <= pCar[nCntCar].posOld.z - CAR_DEPTH
						&& pPos->z + CAR_DEPTH >= pCar[nCntCar].pos.z - CAR_DEPTH)
					{//前回の位置がブロックより手前かつ、現在の位置がブロックよりも奥かつだった場合(手前で止められる処理)
						switch (collObject)
						{
						case COLLOBJECTTYPE_PLAYER:		// プレイヤーの場合

							// 位置をずらす
							pPos->z = pCar[nCntCar].pos.z - (CAR_DEPTH * 2);

							// 移動量を削除
							pMove->x *= 0.95f;

							break;						// 抜け出す

						case COLLOBJECTTYPE_POLICE:		// 警察の場合

							//位置をずらす
							pPos->z = pPosOld->z;

							// 移動量を削除
							pMove->x *= 0.95f;

							break;						// 抜け出す

						case COLLOBJECTTYPE_CAR:		// 車の場合

							//位置をずらす
							pPos->z = pPosOld->z;

							// 移動量を削除
							pMove->x *= 0.95f;

							break;						// 抜け出す
						}
					}							//手前で止められる処理
					else if (pPosOld->z - CAR_DEPTH >= pCar[nCntCar].posOld.z + CAR_DEPTH
						&& pPos->z - CAR_DEPTH <= pCar[nCntCar].pos.z + CAR_DEPTH)
					{//前回の位置が塔の位置よりも奥かつ、現在の位置が塔の位置よりも手前だった場合(奥で止められる処理)
						switch (collObject)
						{
						case COLLOBJECTTYPE_PLAYER:		// プレイヤーの場合

							// 位置をずらす
							pPos->z = pCar[nCntCar].pos.z + (CAR_DEPTH * 2);

							// 移動量を削除
							pMove->x *= 0.95f;

							break;						// 抜け出す

						case COLLOBJECTTYPE_POLICE:		// 警察の場合

							//位置をずらす
							pPos->z = pPosOld->z;

							// 移動量を削除
							pMove->x *= 0.95f;

							break;						// 抜け出す

						case COLLOBJECTTYPE_CAR:		// 車の場合

							//位置をずらす
							pPos->z = pPosOld->z;

							// 移動量を削除
							pMove->x *= 0.95f;

							break;						// 抜け出す
						}
					}							//奥で止められる処理
				}

				if (pPos->z - CAR_DEPTH <= pCar[nCntCar].pos.z + CAR_DEPTH
					&& pPos->z + CAR_DEPTH >= pCar[nCntCar].pos.z - CAR_DEPTH)
				{//塔のZ幅の中にいた場合
					if (pPosOld->x + CAR_WIDTH <= pCar[nCntCar].posOld.x - CAR_WIDTH
						&& pPos->x + CAR_WIDTH >= pCar[nCntCar].pos.x - CAR_WIDTH)
					{//前回の位置がブロックの左端より左かつ、現在の位置がブロックの左側より右だった場合(左の処理)
						switch (collObject)
						{
						case COLLOBJECTTYPE_PLAYER:		// プレイヤーの場合

							// 位置をずらす
							pPos->x = pCar[nCntCar].pos.x - (CAR_WIDTH * 2);

							// 移動量を削除
							pMove->x *= 0.95f;

							break;						// 抜け出す

						case COLLOBJECTTYPE_POLICE:		// 警察の場合

							//位置をずらす
							pPos->x = pPosOld->x;

							// 移動量を削除
							pMove->x *= 0.95f;

							break;						// 抜け出す

						case COLLOBJECTTYPE_CAR:		// 車の場合

							//位置をずらす
							pPos->x = pPosOld->x;

							// 移動量を削除
							pMove->x *= 0.95f;

							break;						// 抜け出す
						}
					}							//左端の処理
					else if (pPosOld->x - CAR_WIDTH >= pCar[nCntCar].posOld.x + CAR_WIDTH
						&& pPos->x - CAR_WIDTH <= pCar[nCntCar].pos.x + CAR_WIDTH)
					{//前回の位置がブロックの右端より右かつ、現在の位置がブロックの左側より右だった場合(右の処理)
						switch (collObject)
						{
						case COLLOBJECTTYPE_PLAYER:		// プレイヤーの場合

							// 位置をずらす
							pPos->x = pCar[nCntCar].pos.x + (CAR_WIDTH * 2);

							// 移動量を削除
							pMove->x *= 0.95f;

							break;						// 抜け出す

						case COLLOBJECTTYPE_POLICE:		// 警察の場合

							//位置をずらす
							pPos->x = pPosOld->x;

							// 移動量を削除
							pMove->x *= 0.95f;

							break;						// 抜け出す

						case COLLOBJECTTYPE_CAR:		// 車の場合

							//位置をずらす
							pPos->x = pPosOld->x;

							// 移動量を削除
							pMove->x *= 0.95f;

							break;						// 抜け出す
						}
					}							//右端の処理
				}
			}
		}
	}

	{ // プレイヤーの当たり判定
		Player *pPlayer = GetPlayer();				// 車の情報を取得する

		if (pPlayer->bUse == true)
		{ // 車が使用されていた場合
			if (pPos->x - CAR_WIDTH <= pPlayer->pos.x + CAR_WIDTH
				&& pPos->x + CAR_WIDTH >= pPlayer->pos.x - CAR_WIDTH)
			{ // 車のX幅の中にいた場合
				if (pPosOld->z + CAR_DEPTH <= pPlayer->oldPos.z - CAR_DEPTH
					&& pPos->z + CAR_DEPTH >= pPlayer->pos.z - CAR_DEPTH)
				{//前回の位置がブロックより手前かつ、現在の位置がブロックよりも奥かつだった場合(手前で止められる処理)
					switch (collObject)
					{
					case COLLOBJECTTYPE_POLICE:		// 警察の場合

						//位置をずらす
						pPos->z = pPosOld->z;

						// 移動量を削除
						pMove->x *= 0.95f;

						break;						// 抜け出す

					case COLLOBJECTTYPE_CAR:		// 車の場合

						//位置をずらす
						pPos->z = pPosOld->z;

						// 移動量を削除
						pMove->x *= 0.95f;

						break;						// 抜け出す
					}
				}							//手前で止められる処理
				else if (pPosOld->z - CAR_DEPTH >= pPlayer->oldPos.z + CAR_DEPTH
					&& pPos->z - CAR_DEPTH <= pPlayer->pos.z + CAR_DEPTH)
				{//前回の位置が塔の位置よりも奥かつ、現在の位置が塔の位置よりも手前だった場合(奥で止められる処理)
					switch (collObject)
					{
					case COLLOBJECTTYPE_POLICE:		// 警察の場合

						//位置をずらす
						pPos->z = pPosOld->z;

						// 移動量を削除
						pMove->x *= 0.95f;

						break;						// 抜け出す

					case COLLOBJECTTYPE_CAR:		// 車の場合

						//位置をずらす
						pPos->z = pPosOld->z;

						// 移動量を削除
						pMove->x *= 0.95f;

						break;						// 抜け出す
					}
				}							//奥で止められる処理
			}

			if (pPos->z - CAR_DEPTH <= pPlayer->oldPos.z + CAR_DEPTH
				&& pPos->z + CAR_DEPTH >= pPlayer->pos.z - CAR_DEPTH)
			{//塔のZ幅の中にいた場合
				if (pPosOld->x + CAR_WIDTH <= pPlayer->oldPos.x - CAR_WIDTH
					&& pPos->x + CAR_WIDTH >= pPlayer->pos.x - CAR_WIDTH)
				{//前回の位置がブロックの左端より左かつ、現在の位置がブロックの左側より右だった場合(左の処理)
					switch (collObject)
					{
					case COLLOBJECTTYPE_POLICE:		// 警察の場合

						//位置をずらす
						pPos->x = pPosOld->x;

						// 移動量を削除
						pMove->x *= 0.95f;

						break;						// 抜け出す

					case COLLOBJECTTYPE_CAR:		// 車の場合

						//位置をずらす
						pPos->x = pPosOld->x;

						// 移動量を削除
						pMove->x *= 0.95f;

						break;						// 抜け出す
					}
				}							//左端の処理
				else if (pPosOld->x - CAR_WIDTH >= pPlayer->oldPos.x + CAR_WIDTH
					&& pPos->x - CAR_WIDTH <= pPlayer->pos.x + CAR_WIDTH)
				{//前回の位置がブロックの右端より右かつ、現在の位置がブロックの左側より右だった場合(右の処理)
					switch (collObject)
					{
					case COLLOBJECTTYPE_POLICE:		// 警察の場合

						//位置をずらす
						pPos->x = pPosOld->x;

						// 移動量を削除
						pMove->x *= 0.95f;

						break;						// 抜け出す

					case COLLOBJECTTYPE_CAR:		// 車の場合

						//位置をずらす
						pPos->x = pPosOld->x;

						// 移動量を削除
						pMove->x *= 0.95f;

						break;						// 抜け出す
					}
				}							//右端の処理
			}
		}
	}

	{ // 警察の当たり判定
		Police *pPolice = GetPoliceData();					// 警察の情報を取得する

		for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
		{
			if (pPolice[nCntPolice].bUse == true)
			{ // 車が使用されていた場合
				if (pPos->x - CAR_WIDTH <= pPolice[nCntPolice].pos.x + CAR_WIDTH
					&& pPos->x + CAR_WIDTH >= pPolice[nCntPolice].pos.x - CAR_WIDTH)
				{ // 車のX幅の中にいた場合
					if (pPosOld->z + CAR_DEPTH <= pPolice[nCntPolice].posOld.z - CAR_DEPTH
						&& pPos->z + CAR_DEPTH >= pPolice[nCntPolice].pos.z - CAR_DEPTH)
					{//前回の位置がブロックより手前かつ、現在の位置がブロックよりも奥かつだった場合(手前で止められる処理)
						switch (collObject)
						{
						case COLLOBJECTTYPE_PLAYER:		// プレイヤーの場合

							// 位置をずらす
							pPos->z = pPosOld->z;

							break;						// 抜け出す

						case COLLOBJECTTYPE_POLICE:		// 警察の場合

							//位置をずらす
							pPos->z = pPosOld->z;

							// 移動量を削除
							pMove->x *= 0.95f;

							break;						// 抜け出す

						case COLLOBJECTTYPE_CAR:		// 車の場合

							//位置をずらす
							pPos->z = pPosOld->z;

							// 移動量を削除
							pMove->x *= 0.95f;

							break;						// 抜け出す
						}
					}							//手前で止められる処理
					else if (pPosOld->z - CAR_DEPTH >= pPolice[nCntPolice].posOld.z + CAR_DEPTH
						&& pPos->z - CAR_DEPTH <= pPolice[nCntPolice].pos.z + CAR_DEPTH)
					{//前回の位置が塔の位置よりも奥かつ、現在の位置が塔の位置よりも手前だった場合(奥で止められる処理)
						switch (collObject)
						{
						case COLLOBJECTTYPE_PLAYER:		// プレイヤーの場合

							// 位置をずらす
							pPos->z = pPosOld->z;

							break;						// 抜け出す

						case COLLOBJECTTYPE_POLICE:		// 警察の場合

							//位置をずらす
							pPos->z = pPosOld->z;

							// 移動量を削除
							pMove->x *= 0.95f;

							break;						// 抜け出す

						case COLLOBJECTTYPE_CAR:		// 車の場合

							//位置をずらす
							pPos->z = pPosOld->z;

							// 移動量を削除
							pMove->x *= 0.95f;

							break;						// 抜け出す
						}
					}							//奥で止められる処理
				}

				if (pPos->z - CAR_DEPTH <= pPolice[nCntPolice].pos.z + CAR_DEPTH
					&& pPos->z + CAR_DEPTH >= pPolice[nCntPolice].pos.z - CAR_DEPTH)
				{//塔のZ幅の中にいた場合
					if (pPosOld->x + CAR_WIDTH <= pPolice[nCntPolice].posOld.x - CAR_WIDTH
						&& pPos->x + CAR_WIDTH >= pPolice[nCntPolice].pos.x - CAR_WIDTH)
					{//前回の位置がブロックの左端より左かつ、現在の位置がブロックの左側より右だった場合(左の処理)
						switch (collObject)
						{
						case COLLOBJECTTYPE_PLAYER:		// プレイヤーの場合

							// 位置をずらす
							pPos->x = pPosOld->x;

							break;						// 抜け出す

						case COLLOBJECTTYPE_POLICE:		// 警察の場合

							//位置をずらす
							pPos->x = pPosOld->x;

							// 移動量を削除
							pMove->x *= 0.95f;

							break;						// 抜け出す

						case COLLOBJECTTYPE_CAR:		// 車の場合

							//位置をずらす
							pPos->x = pPosOld->x;

							// 移動量を削除
							pMove->x *= 0.95f;

							break;						// 抜け出す
						}
					}							//左端の処理
					else if (pPosOld->x - CAR_WIDTH >= pPolice[nCntPolice].posOld.x + CAR_WIDTH
						&& pPos->x - CAR_WIDTH <= pPolice[nCntPolice].pos.x + CAR_WIDTH)
					{//前回の位置がブロックの右端より右かつ、現在の位置がブロックの左側より右だった場合(右の処理)
						switch (collObject)
						{
						case COLLOBJECTTYPE_PLAYER:		// プレイヤーの場合

							// 位置をずらす
							pPos->x = pPosOld->x;

							break;						// 抜け出す

						case COLLOBJECTTYPE_POLICE:		// 警察の場合

							//位置をずらす
							pPos->x = pPosOld->x;

							// 移動量を削除
							pMove->x *= 0.95f;

							break;						// 抜け出す

						case COLLOBJECTTYPE_CAR:		// 車の場合

							//位置をずらす
							pPos->x = pPosOld->x;

							// 移動量を削除
							pMove->x *= 0.95f;

							break;						// 抜け出す
						}
					}							//右端の処理
				}
			}
		}
	}
}

#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================
#endif