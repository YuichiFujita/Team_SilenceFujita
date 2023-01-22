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
#define CAR_WIDTH				(30.0f)		// パトカーの縦幅
#define CAR_HEIGHT				(30.0f)		// パトカーの奥行

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void PosCar(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove);		// 車の位置の更新処理
void RevCar(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos);	// 車の補正の更新処理
void CurveCar(Car *pCar);							// 車のカーブ処理
void CurveRotCar(Car *pCar);						// 車の角度更新処理

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
Car g_aCar[MAX_CAR];		// 車の情報

//======================================================================================================================
//	車の初期化処理
//======================================================================================================================
void InitCar(void)
{
	CARCURVE CarCurveInfo;				//情報

	//曲がる回数
	CarCurveInfo.nCurveTime = 8;

	for (int nCntInfo = 0; nCntInfo < CarCurveInfo.nCurveTime; nCntInfo++)
	{
		switch (nCntInfo)
		{
		case 0:

			//次曲がるポイント
			CarCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(6575.0f, 0.0f, 2075.0f);

			//今走ってる軸
			CarCurveInfo.bCurveX[nCntInfo] = false;

			//今走っている方向
			CarCurveInfo.bCurvePlus[nCntInfo] = true;

			//次曲がる方向
			CarCurveInfo.curveAngle[nCntInfo] = CURVE_LEFT;

			//次に向く角度
			CarCurveInfo.fCurveRot[nCntInfo] = -90;

			break;				//抜け出す

		case 1:

			//次曲がるポイント
			CarCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(2075.0f, 0.0f, 2075.0f);

			//今走ってる軸
			CarCurveInfo.bCurveX[nCntInfo] = true;

			//今走っている方向
			CarCurveInfo.bCurvePlus[nCntInfo] = false;

			//次曲がる方向
			CarCurveInfo.curveAngle[nCntInfo] = CURVE_RIGHT;

			//次に向く角度
			CarCurveInfo.fCurveRot[nCntInfo] = 0;

			break;				//抜け出す

		case 2:

			//次曲がるポイント
			CarCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(2075.0f, 0.0f, 6575.0f);

			//今走ってる軸
			CarCurveInfo.bCurveX[nCntInfo] = false;

			//今走っている方向
			CarCurveInfo.bCurvePlus[nCntInfo] = true;

			//次曲がる方向
			CarCurveInfo.curveAngle[nCntInfo] = CURVE_LEFT;

			//次に向く角度
			CarCurveInfo.fCurveRot[nCntInfo] = -90;

			break;

		case 3:

			//次曲がるポイント
			CarCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(-6575.0f, 0.0f, 6575.0f);

			//今走ってる軸
			CarCurveInfo.bCurveX[nCntInfo] = true;

			//今走っている方向
			CarCurveInfo.bCurvePlus[nCntInfo] = false;

			//次曲がる方向
			CarCurveInfo.curveAngle[nCntInfo] = CURVE_LEFT;

			//次に向く角度
			CarCurveInfo.fCurveRot[nCntInfo] = -180;

			break;				//抜け出す

		case 4:

			//次曲がるポイント
			CarCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(-6575.0f, 0.0f, 2425.0f);

			//今走ってる軸
			CarCurveInfo.bCurveX[nCntInfo] = false;

			//今走っている方向
			CarCurveInfo.bCurvePlus[nCntInfo] = false;

			//次曲がる方向
			CarCurveInfo.curveAngle[nCntInfo] = CURVE_LEFT;

			//次に向く角度
			CarCurveInfo.fCurveRot[nCntInfo] = 90;

			break;				//抜け出す

		case 5:

			//次曲がるポイント
			CarCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(-2325.0f, 0.0f, 2425.0f);

			//今走ってる軸
			CarCurveInfo.bCurveX[nCntInfo] = true;

			//今走っている方向
			CarCurveInfo.bCurvePlus[nCntInfo] = true;

			//次曲がる方向
			CarCurveInfo.curveAngle[nCntInfo] = CURVE_RIGHT;

			//次に向く角度
			CarCurveInfo.fCurveRot[nCntInfo] = 180;

			break;				//抜け出す

		case 6:

			//次曲がるポイント
			CarCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(-2325.0f, 0.0f, -2075.0f);

			//今走ってる軸
			CarCurveInfo.bCurveX[nCntInfo] = false;

			//今走っている方向
			CarCurveInfo.bCurvePlus[nCntInfo] = false;

			//次曲がる方向
			CarCurveInfo.curveAngle[nCntInfo] = CURVE_LEFT;

			//次に向く角度
			CarCurveInfo.fCurveRot[nCntInfo] = 90;

			break;				//抜け出す

		case 7:

			//次曲がるポイント
			CarCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(6575.0f, 0.0f, -2075.0f);

			//今走ってる軸
			CarCurveInfo.bCurveX[nCntInfo] = true;

			//今走っている方向
			CarCurveInfo.bCurvePlus[nCntInfo] = true;

			//次曲がる方向
			CarCurveInfo.curveAngle[nCntInfo] = CURVE_LEFT;

			//次に向く角度
			CarCurveInfo.fCurveRot[nCntInfo] = 0;

			break;				//抜け出す
		}
	}

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

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

	//車の設定処理
	SetCar(D3DXVECTOR3(6000.0f, 0.0f, -1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CarCurveInfo);
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

			// 車の停止処理
			CollisionStopCar
			( // 引数
				g_aCar[nCntCar].pos,		//位置
				g_aCar[nCntCar].rot,		//向き
				&g_aCar[nCntCar].move,		//移動量
				g_aCar[nCntCar].modelData.fRadius,	//半径
				COLLOBJECTTYPE_CAR			//対象のサイズ
			);

			// プレイヤーの位置の更新
			PosCar(&g_aCar[nCntCar].move, &g_aCar[nCntCar].pos, &g_aCar[nCntCar].rot, g_aCar[nCntCar].bMove);

			//----------------------------------------------------
			//	当たり判定
			//----------------------------------------------------
			// オブジェクトとの当たり判定
			CollisionCar
			( // 引数
				&g_aCar[nCntCar].pos,		// 現在の位置
				&g_aCar[nCntCar].posOld,	// 前回の位置
				CAR_WIDTH,					// 横幅
				CAR_HEIGHT					// 奥行	
			);

			if (g_aCar[nCntCar].pos.y < 0.0f)
			{//Y軸の位置が0.0fだった場合
				//縦への移動量を0.0fにする
				g_aCar[nCntCar].move.y = 0.0f;

				//位置を0.0fに戻す
				g_aCar[nCntCar].pos.y = 0.0f;
			}

			// プレイヤーの補正の更新処理
			RevCar(&g_aCar[nCntCar].rot, &g_aCar[nCntCar].pos);
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
void SetCar(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CARCURVE carCurve)
{
	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aCar[nCntCar].bUse == false)
		{ // オブジェクトが使用されていない場合
			// 引数を代入
			g_aCar[nCntCar].pos = pos;								// 現在の位置
			g_aCar[nCntCar].posOld = g_aCar[nCntCar].pos;			// 前回の位置
			g_aCar[nCntCar].rot = rot;								// 向き
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
				&g_aCar[nCntCar].bUse			// 影の親の使用状況
			);

			// 影の位置設定
			SetPositionShadow(g_aCar[nCntCar].nShadowID, g_aCar[nCntCar].pos, g_aCar[nCntCar].rot, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

			//曲がり角関係の設定
			for (int nCntCurve = 0; nCntCurve < carCurve.nCurveTime; nCntCurve++)
			{
				g_aCar[nCntCar].carCurve.curveAngle[nCntCurve] = carCurve.curveAngle[nCntCurve];	// 次曲がる方向
				g_aCar[nCntCar].carCurve.curvePoint[nCntCurve] = carCurve.curvePoint[nCntCurve];	// 次曲がるポイント
				g_aCar[nCntCar].carCurve.bCurveX[nCntCurve] = carCurve.bCurveX[nCntCurve];			// 現在走っている軸
				g_aCar[nCntCar].carCurve.bCurvePlus[nCntCurve] = carCurve.bCurvePlus[nCntCurve];	// 現在走っている方向
				g_aCar[nCntCar].carCurve.fCurveRot[nCntCurve] = carCurve.fCurveRot[nCntCurve];		// 次曲がる目標の方向
			}
			g_aCar[nCntCar].carCurve.nNowCurve = 0;													// 現在のルート
			g_aCar[nCntCar].carCurve.nCurveTime = carCurve.nCurveTime;								// 曲がる回数

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
//	オブジェクトとの当たり判定
//======================================================================================================================
void CollisionCar(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pOldPos, float fWidth, float fDepth)
{
	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
	{ // オブジェクトの最大表示数分繰り返す
		if (g_aCar[nCntCar].bUse == true)
		{ // オブジェクトが使用されている場合

		  // 前後の当たり判定
			if (pPos->x + fWidth > g_aCar[nCntCar].pos.x + g_aCar[nCntCar].modelData.vtxMin.x
				&&  pPos->x - fWidth < g_aCar[nCntCar].pos.x + g_aCar[nCntCar].modelData.vtxMax.x)
			{ // ブロックの左右の範囲内の場合

				if (pPos->z + fDepth > g_aCar[nCntCar].pos.z + g_aCar[nCntCar].modelData.vtxMin.z
					&&  pOldPos->z + fDepth <= g_aCar[nCntCar].pos.z + g_aCar[nCntCar].modelData.vtxMin.z)
				{ // 前からの当たり判定
				  // 位置を補正
					pPos->z = g_aCar[nCntCar].pos.z + g_aCar[nCntCar].modelData.vtxMin.z - fDepth - 0.01f;
				}
				else if (pPos->z - fDepth < g_aCar[nCntCar].pos.z + g_aCar[nCntCar].modelData.vtxMax.z
					&&  pOldPos->z - fDepth >= g_aCar[nCntCar].pos.z + g_aCar[nCntCar].modelData.vtxMax.z)
				{ // 後ろからの当たり判定
				  // 位置を補正
					pPos->z = g_aCar[nCntCar].pos.z + g_aCar[nCntCar].modelData.vtxMax.z + fDepth + 0.01f;
				}
			}

			// 左右の当たり判定
			if (pPos->z + fDepth > g_aCar[nCntCar].pos.z + g_aCar[nCntCar].modelData.vtxMin.z
				&&  pPos->z - fDepth < g_aCar[nCntCar].pos.z + g_aCar[nCntCar].modelData.vtxMax.z)
			{ // ブロックの前後の範囲内の場合

				if (pPos->x + fWidth > g_aCar[nCntCar].pos.x + g_aCar[nCntCar].modelData.vtxMin.x
					&&  pOldPos->x + fWidth <= g_aCar[nCntCar].pos.x + g_aCar[nCntCar].modelData.vtxMin.x)
				{ // 左からの当たり判定
				  // 位置を補正
					pPos->x = g_aCar[nCntCar].pos.x + g_aCar[nCntCar].modelData.vtxMin.x - fWidth - 0.01f;
				}
				else if (pPos->x - fWidth < g_aCar[nCntCar].pos.x + g_aCar[nCntCar].modelData.vtxMax.x
					&&  pOldPos->x - fWidth >= g_aCar[nCntCar].pos.x + g_aCar[nCntCar].modelData.vtxMax.x)
				{ // 右からの当たり判定
				  // 位置を補正
					pPos->x = g_aCar[nCntCar].pos.x + g_aCar[nCntCar].modelData.vtxMax.x + fWidth + 0.01f;
				}
			}
		}
	}
}

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

	////--------------------------------------------------------
	////	移動範囲の補正
	////--------------------------------------------------------
	//if (pos->z > GetLimitStage().fNear - (30.0f * 2))
	//{ // 範囲外の場合 (手前)

	//	// 手前に位置を補正
	//	pos->z = GetLimitStage().fNear - (30.0f * 2);
	//}
	//if (pos->z < GetLimitStage().fFar + (30.0f * 2))
	//{ // 範囲外の場合 (奥)

	//	// 奥に位置を補正
	//	pos->z = GetLimitStage().fFar + (30.0f * 2);
	//}
	//if (pos->x > GetLimitStage().fRight - (30.0f * 2))
	//{ // 範囲外の場合 (右)

	//	// 右に位置を補正
	//	pos->x = GetLimitStage().fRight - (30.0f * 2);
	//}
	//if (pos->x < GetLimitStage().fLeft + (30.0f * 2))
	//{ // 範囲外の場合 (左)

	//	// 左に位置を補正
	//	pos->x = GetLimitStage().fLeft + (30.0f * 2);
	//}
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
			if (pCar->pos.x >= pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x + (CAR_WIDTH * 2))
			{ // 左にある壁が途切れたら

				// 車の角度更新・補正処理
				CurveRotCar(pCar);

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
		else
		{ // 左に走っている場合
			if (pCar->pos.x <= pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x - (CAR_WIDTH * 2))
			{ // 左にある壁が途切れたら

				// 車の角度更新・補正処理
				CurveRotCar(pCar);

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
	else
	{//Z軸を走っていた場合
		if (pCar->carCurve.bCurvePlus[pCar->carCurve.nNowCurve] == true)
		{ // 右の壁が警察より左側にある場合
			if (pCar->pos.z >= pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z + (CAR_WIDTH * 2))
			{ // 左にある壁が途切れたら

				// 車の角度更新・補正処理
				CurveRotCar(pCar);

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
		else
		{//左の壁が警察より右にある場合
			if (pCar->pos.z <= pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z - (CAR_WIDTH * 2))
			{ // 左にある壁が途切れたら

				// 車の角度更新・補正処理
				CurveRotCar(pCar);

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
		Police *pCar = GetPoliceData();					// 警察の情報を取得する

		for (int nCntPoli = 0; nCntPoli < MAX_POLICE; nCntPoli++, pCar++)
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

						// 移動量を設定する
						move->x = sinf(targetrot.y) * -3.0f;

						break;						//抜け出す

					case COLLOBJECTTYPE_POLICE:		//車の場合

						// 目標の移動量をセーブする
						move->x += (0.0f - move->x) * 0.5f;

						break;						//抜け出す

					case COLLOBJECTTYPE_CAR:		//車の場合

						// 目標の移動量をセーブする
						move->x += (0.0f - move->x) * 0.5f;

						break;						//抜け出す
					}
				}
			}
		}
	}

	{//プレイヤーとの当たり判定
		Player *pPlayer = GetPlayer();

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

												// 移動量を設定する
					move->x = sinf(targetrot.y) * -3.0f;

					break;						//抜け出す

				case COLLOBJECTTYPE_POLICE:		//車の場合

												// 目標の移動量をセーブする
					move->x += (0.0f - move->x) * 0.5f;

					break;						//抜け出す

				case COLLOBJECTTYPE_CAR:		//車の場合

					// 目標の移動量をセーブする
					move->x += (0.0f - move->x) * 0.5f;

					break;						//抜け出す
				}
			}
		}
	}
}

//============================================================
// 車の角度更新・補正処理
//============================================================
void CurveRotCar(Car *pCar)
{
	if (pCar->carCurve.curveAngle[pCar->carCurve.nNowCurve] == CURVE_LEFT)
	{ // 曲がる方向が左だった場合
		// 向きを更新
		pCar->rot.y -= 0.05f * (pCar->move.x * 0.1f);

		if (pCar->rot.y <= (float)D3DXToRadian(pCar->carCurve.fCurveRot[pCar->carCurve.nNowCurve]))
		{ // 一定の向きに達した場合
			// 向きを補正
			pCar->rot.y = (float)D3DXToRadian(pCar->carCurve.fCurveRot[pCar->carCurve.nNowCurve]);

			if (pCar->carCurve.curveAngle[(pCar->carCurve.nNowCurve + 1) % pCar->carCurve.nCurveTime] == CURVE_LEFT)
			{//次の曲がり角が左だった場合
				if (pCar->carCurve.fCurveRot[(pCar->carCurve.nNowCurve + 1) % pCar->carCurve.nCurveTime] == 90)
				{//次の曲がり角がマイナス方向に曲がる場合
					//向きをプラスに戻す
					pCar->rot.y = (float)D3DXToRadian(180);
				}
			}

			//// 車の補正の更新処理
			//RevCar(&pCar->rot, &pCar->pos);

			// 警察の行先を設定する
			pCar->carCurve.nNowCurve = (pCar->carCurve.nNowCurve + 1) % pCar->carCurve.nCurveTime;
		}
	}
	else if (pCar->carCurve.curveAngle[pCar->carCurve.nNowCurve] == CURVE_RIGHT)
	{ // 曲がる方向が右だった場合
		// 向きを更新
		pCar->rot.y += 0.05f * (pCar->move.x * 0.1f);

		if (pCar->rot.y >= (float)D3DXToRadian(pCar->carCurve.fCurveRot[pCar->carCurve.nNowCurve]))
		{ // 一定の向きに達した場合
		  // 向きを補正
			pCar->rot.y = (float)D3DXToRadian(pCar->carCurve.fCurveRot[pCar->carCurve.nNowCurve]);

			if (pCar->carCurve.curveAngle[(pCar->carCurve.nNowCurve + 1) % pCar->carCurve.nCurveTime] == CURVE_RIGHT)
			{//次の曲がり角が右だった場合
				if (pCar->carCurve.fCurveRot[(pCar->carCurve.nNowCurve + 1) % pCar->carCurve.nCurveTime] == -90)
				{//次の曲がり角がマイナス方向に曲がる場合
					//向きをプラスに戻す
					pCar->rot.y = (float)D3DXToRadian(-180);
				}
			}

			//// 車の補正の更新処理
			//RevCar(&pCar->rot, &pCar->pos);

			// 警察の行先を設定する
			pCar->carCurve.nNowCurve = (pCar->carCurve.nNowCurve + 1) % pCar->carCurve.nCurveTime;
		}
	}
}

#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================
#endif