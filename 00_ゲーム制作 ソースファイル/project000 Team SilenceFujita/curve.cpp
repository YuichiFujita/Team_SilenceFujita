//======================================================================================================================
//
//	カーブの処理 [Curve.cpp]
//	Author：小原立暉
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "curve.h"
#include "calculation.h"

#ifdef _DEBUG	// デバッグ処理
#include "game.h"
#endif

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define CURVE_MOVE_SLOW_MAGNI			(0.3f)				// カーブするときの減速係数
#define CURVE_ADD						(0.03f)				// カーブするときの向きの追加量

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
CURVEINFO g_aCurveInfo[MAX_CURVEPOINT];		// 車のカーブの位置の情報

//======================================================================================================================
// カーブの情報の初期化処理
//======================================================================================================================
void InitCurveInfo(void)
{
	g_aCurveInfo[0].pos = D3DXVECTOR3(-7000.0f, 0.0f, 7000.0f);
	g_aCurveInfo[0].nCurveNumber = 0;
	g_aCurveInfo[0].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[0].nPairNumber = 1;
	g_aCurveInfo[0].dashAngle = DASH_FAR;
	g_aCurveInfo[0].bDeadEnd = true;

	g_aCurveInfo[1].pos = D3DXVECTOR3(-6500.0f, 0.0f, 6500.0f);
	g_aCurveInfo[1].nCurveNumber = 1;
	g_aCurveInfo[1].curveAngle = CURVE_LEFT;
	g_aCurveInfo[1].nPairNumber = 0;
	g_aCurveInfo[1].dashAngle = DASH_LEFT;
	g_aCurveInfo[1].bDeadEnd = true;

	g_aCurveInfo[2].pos = D3DXVECTOR3(-4375.0f, 0.0f, 7000.0f);
	g_aCurveInfo[2].nCurveNumber = 2;
	g_aCurveInfo[2].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[2].nPairNumber = 3;
	g_aCurveInfo[2].dashAngle = DASH_RIGHT;
	g_aCurveInfo[2].bDeadEnd = false;

	g_aCurveInfo[3].pos = D3DXVECTOR3(-4875.0f, 0.0f, 6500.0f);
	g_aCurveInfo[3].nCurveNumber = 3;
	g_aCurveInfo[3].curveAngle = CURVE_LEFT;
	g_aCurveInfo[3].nPairNumber = 2;
	g_aCurveInfo[3].dashAngle = DASH_FAR;
	g_aCurveInfo[3].bDeadEnd = false;

	g_aCurveInfo[4].pos = D3DXVECTOR3(-4375.0f, 0.0f, 6500.0f);
	g_aCurveInfo[4].nCurveNumber = 4;
	g_aCurveInfo[4].curveAngle = CURVE_LEFT;
	g_aCurveInfo[4].nPairNumber = 5;
	g_aCurveInfo[4].dashAngle = DASH_LEFT;
	g_aCurveInfo[4].bDeadEnd = false;

	g_aCurveInfo[5].pos = D3DXVECTOR3(-4875.0f, 0.0f, 7000.0f);
	g_aCurveInfo[5].nCurveNumber = 5;
	g_aCurveInfo[5].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[5].nPairNumber = 4;
	g_aCurveInfo[5].dashAngle = DASH_FAR;
	g_aCurveInfo[5].bDeadEnd = true;

	g_aCurveInfo[6].pos = D3DXVECTOR3(-2250.0f, 0.0f, 7000.0f);
	g_aCurveInfo[6].nCurveNumber = 6;
	g_aCurveInfo[6].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[6].nPairNumber = 7;
	g_aCurveInfo[6].dashAngle = DASH_RIGHT;
	g_aCurveInfo[6].bDeadEnd = true;

	g_aCurveInfo[7].pos = D3DXVECTOR3(-2750.0f, 0.0f, 6500.0f);
	g_aCurveInfo[7].nCurveNumber = 7;
	g_aCurveInfo[7].curveAngle = CURVE_LEFT;
	g_aCurveInfo[7].nPairNumber = 6;
	g_aCurveInfo[7].dashAngle = DASH_FAR;
	g_aCurveInfo[7].bDeadEnd = true;

	g_aCurveInfo[8].pos = D3DXVECTOR3(-2750.0f, 0.0f, 4750.0f);
	g_aCurveInfo[8].nCurveNumber = 8;
	g_aCurveInfo[8].curveAngle = CURVE_LEFT;
	g_aCurveInfo[8].nPairNumber = 9;
	g_aCurveInfo[8].dashAngle = DASH_RIGHT;
	g_aCurveInfo[8].bDeadEnd = false;

	g_aCurveInfo[9].pos = D3DXVECTOR3(-2250.0f, 0.0f, 4250.0f);
	g_aCurveInfo[9].nCurveNumber = 9;
	g_aCurveInfo[9].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[9].nPairNumber = 8;
	g_aCurveInfo[9].dashAngle = DASH_NEAR;
	g_aCurveInfo[9].bDeadEnd = false;

	g_aCurveInfo[10].pos = D3DXVECTOR3(-2750.0f, 0.0f, 4250.0f);
	g_aCurveInfo[10].nCurveNumber = 10;
	g_aCurveInfo[10].curveAngle = CURVE_LEFT;
	g_aCurveInfo[10].nPairNumber = 11;
	g_aCurveInfo[10].dashAngle = DASH_FAR;
	g_aCurveInfo[10].bDeadEnd = false;

	g_aCurveInfo[11].pos = D3DXVECTOR3(-2250.0f, 0.0f, 4750.0f);
	g_aCurveInfo[11].nCurveNumber = 11;
	g_aCurveInfo[11].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[11].nPairNumber = 10;
	g_aCurveInfo[11].dashAngle = DASH_RIGHT;
	g_aCurveInfo[11].bDeadEnd = true;

	g_aCurveInfo[12].pos = D3DXVECTOR3(-4375.0f, 0.0f, 4250.0f);
	g_aCurveInfo[12].nCurveNumber = 12;
	g_aCurveInfo[12].curveAngle = CURVE_LEFT;
	g_aCurveInfo[12].nPairNumber = 13;
	g_aCurveInfo[12].dashAngle = DASH_LEFT;
	g_aCurveInfo[12].bDeadEnd = false;

	g_aCurveInfo[13].pos = D3DXVECTOR3(-4875.0f, 0.0f, 4750.0f);
	g_aCurveInfo[13].nCurveNumber = 13;
	g_aCurveInfo[13].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[13].nPairNumber = 12;
	g_aCurveInfo[13].dashAngle = DASH_FAR;
	g_aCurveInfo[13].bDeadEnd = false;

	g_aCurveInfo[14].pos = D3DXVECTOR3(-4375.0f, 0.0f, 4750.0f);
	g_aCurveInfo[14].nCurveNumber = 14;
	g_aCurveInfo[14].curveAngle = CURVE_LEFT;
	g_aCurveInfo[14].nPairNumber = 15;
	g_aCurveInfo[14].dashAngle = DASH_NEAR;
	g_aCurveInfo[14].bDeadEnd = false;

	g_aCurveInfo[15].pos = D3DXVECTOR3(-4875.0f, 0.0f, 4250.0f);
	g_aCurveInfo[15].nCurveNumber = 15;
	g_aCurveInfo[15].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[15].nPairNumber = 14;
	g_aCurveInfo[15].dashAngle = DASH_LEFT;
	g_aCurveInfo[15].bDeadEnd = true;

	g_aCurveInfo[16].pos = D3DXVECTOR3(-4875.0f, 0.0f, 2500.0f);
	g_aCurveInfo[16].nCurveNumber = 16;
	g_aCurveInfo[16].curveAngle = CURVE_LEFT;
	g_aCurveInfo[16].nPairNumber = 17;
	g_aCurveInfo[16].dashAngle = DASH_RIGHT;
	g_aCurveInfo[16].bDeadEnd = false;

	g_aCurveInfo[17].pos = D3DXVECTOR3(-4375.0f, 0.0f, 2000.0f);
	g_aCurveInfo[17].nCurveNumber = 17;
	g_aCurveInfo[17].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[17].nPairNumber = 16;
	g_aCurveInfo[17].dashAngle = DASH_NEAR;
	g_aCurveInfo[17].bDeadEnd = true;

	g_aCurveInfo[18].pos = D3DXVECTOR3(-4875.0f, 0.0f, 2000.0f);
	g_aCurveInfo[18].nCurveNumber = 18;
	g_aCurveInfo[18].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[18].nPairNumber = 19;
	g_aCurveInfo[18].dashAngle = DASH_LEFT;
	g_aCurveInfo[18].bDeadEnd = false;

	g_aCurveInfo[19].pos = D3DXVECTOR3(-4375.0f, 0.0f, 2500.0f);
	g_aCurveInfo[19].nCurveNumber = 19;
	g_aCurveInfo[19].curveAngle = CURVE_LEFT;
	g_aCurveInfo[19].nPairNumber = 18;
	g_aCurveInfo[19].dashAngle = DASH_NEAR;
	g_aCurveInfo[19].bDeadEnd = false;

	g_aCurveInfo[20].pos = D3DXVECTOR3(-2750.0f, 0.0f, 2500.0f);
	g_aCurveInfo[20].nCurveNumber = 20;
	g_aCurveInfo[20].curveAngle = CURVE_LEFT;
	g_aCurveInfo[20].nPairNumber = 21;
	g_aCurveInfo[20].dashAngle = DASH_RIGHT;
	g_aCurveInfo[20].bDeadEnd = true;

	g_aCurveInfo[21].pos = D3DXVECTOR3(-2250.0f, 0.0f, 2000.0f);
	g_aCurveInfo[21].nCurveNumber = 21;
	g_aCurveInfo[21].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[21].nPairNumber = 20;
	g_aCurveInfo[21].dashAngle = DASH_NEAR;
	g_aCurveInfo[21].bDeadEnd = true;

	g_aCurveInfo[22].pos = D3DXVECTOR3(-6500.0f, 0.0f, 2500.0f);
	g_aCurveInfo[22].nCurveNumber = 22;
	g_aCurveInfo[22].curveAngle = CURVE_LEFT;
	g_aCurveInfo[22].nPairNumber = 23;
	g_aCurveInfo[22].dashAngle = DASH_NEAR;
	g_aCurveInfo[22].bDeadEnd = true;

	g_aCurveInfo[23].pos = D3DXVECTOR3(-7000.0f, 0.0f, 2000.0f);
	g_aCurveInfo[23].nCurveNumber = 23;
	g_aCurveInfo[23].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[23].nPairNumber = 22;
	g_aCurveInfo[23].dashAngle = DASH_LEFT;
	g_aCurveInfo[23].bDeadEnd = true;
}

//============================================================
// 車の角度更新・補正処理
//============================================================
void CurveInfoRotCar(CARCURVE *pCurve, D3DXVECTOR3 *rot, D3DXVECTOR3 *move)
{
	//速度を落とす
	move->x *= CURVE_MOVE_SLOW_MAGNI;

	if (pCurve->curveInfo.curveAngle == CURVE_LEFT)
	{ // 曲がる方向が左だった場合
		if (pCurve->curveInfo.dashAngle == DASH_RIGHT)
		{ // 右に走っている場合
			// 目標の向きを設定する
			pCurve->rotDest.y = 0.0f;

			// 向きを加える
			rot->y -= CURVE_ADD;

			if (rot->y <= pCurve->rotDest.y)
			{ // 向きが目標値を超えた場合

				// 向きを補正する
				rot->y = pCurve->rotDest.y;

				// 奥に走る
				pCurve->curveInfo.dashAngle = DASH_FAR;

				// 走行状態にする
				pCurve->actionState = CARACT_DASH;

				// スキップする回数を設定する
				pCurve->nSKipCnt = rand() % 2 + 1;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_LEFT)
		{ // 左に走っている場合
			// 目標の向きを設定する
			pCurve->rotDest.y = -D3DX_PI;

			// 向きを加える
			rot->y -= CURVE_ADD;

			if (rot->y <= pCurve->rotDest.y)
			{ // 向きが目標値を超えた場合

				// 向きを補正する
				rot->y = pCurve->rotDest.y;

				// 手前に走る
				pCurve->curveInfo.dashAngle = DASH_NEAR;

				// 走行状態にする
				pCurve->actionState = CARACT_DASH;

				// スキップする回数を設定する
				pCurve->nSKipCnt = rand() % 2 + 1;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_FAR)
		{ // 奥に走っている場合
			// 目標の向きを設定する
			pCurve->rotDest.y = -D3DX_PI * 0.5f;

			// 向きを加える
			rot->y -= CURVE_ADD;

			if (rot->y <= pCurve->rotDest.y)
			{ // 向きが目標値を超えた場合

				// 向きを補正する
				rot->y = pCurve->rotDest.y;

				// 左に走る
				pCurve->curveInfo.dashAngle = DASH_LEFT;

				// 走行状態にする
				pCurve->actionState = CARACT_DASH;

				// スキップする回数を設定する
				pCurve->nSKipCnt = rand() % 2 + 1;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_NEAR)
		{ // 手前に走っている場合
			// 目標の向きを設定する
			pCurve->rotDest.y = D3DX_PI * 0.5f;

			// 向きを加える
			rot->y -= CURVE_ADD;

			if (rot->y <= pCurve->rotDest.y)
			{ // 向きが目標値を超えた場合

				// 向きを補正する
				rot->y = pCurve->rotDest.y;

				// 右に走る
				pCurve->curveInfo.dashAngle = DASH_RIGHT;

				// 走行状態にする
				pCurve->actionState = CARACT_DASH;

				// スキップする回数を設定する
				pCurve->nSKipCnt = rand() % 2 + 1;
			}
		}
	}
	else
	{ // 曲がる方向が右だった場合
		//速度を落とす
		move->x *= 0.3f;

		if (pCurve->curveInfo.dashAngle == DASH_RIGHT)
		{ // 右に走っている場合

			// 目標の向きを設定する
			pCurve->rotDest.y = D3DX_PI;

			// 向きを加える
			rot->y += CURVE_ADD;

			if (rot->y >= pCurve->rotDest.y)
			{ // 向きが目標値を超えた場合

				// 向きを補正する
				rot->y = pCurve->rotDest.y;

				// 手前に走る
				pCurve->curveInfo.dashAngle = DASH_NEAR;

				// 走行状態にする
				pCurve->actionState = CARACT_DASH;

				// スキップする回数を設定する
				pCurve->nSKipCnt = rand() % 2 + 1;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_LEFT)
		{ // 左に走っている場合

			// 目標の向きを設定する
			pCurve->rotDest.y = 0.0f;

			// 向きを加える
			rot->y += CURVE_ADD;

			if (rot->y >= pCurve->rotDest.y)
			{ // 向きが目標値を超えた場合

				// 向きを補正する
				rot->y = pCurve->rotDest.y;

				// 奥に走る
				pCurve->curveInfo.dashAngle = DASH_FAR;

				// 走行状態にする
				pCurve->actionState = CARACT_DASH;

				// スキップする回数を設定する
				pCurve->nSKipCnt = rand() % 2 + 1;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_FAR)
		{ // 奥に走っている場合

			// 目標の向きを設定する
			pCurve->rotDest.y = D3DX_PI * 0.5f;

			// 向きを加える
			rot->y += CURVE_ADD;

			if (rot->y >= pCurve->rotDest.y)
			{ // 向きが目標値を超えた場合

				// 向きを補正する
				rot->y = pCurve->rotDest.y;

				// 右に走る
				pCurve->curveInfo.dashAngle = DASH_RIGHT;

				// 走行状態にする
				pCurve->actionState = CARACT_DASH;

				// スキップする回数を設定する
				pCurve->nSKipCnt = rand() % 2 + 1;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_NEAR)
		{ // 手前に走っている場合

			// 目標の向きを設定する
			pCurve->rotDest.y = -D3DX_PI * 0.5f;

			// 向きを加える
			rot->y += CURVE_ADD;

			if (rot->y >= pCurve->rotDest.y)
			{ // 向きが目標値を超えた場合

				// 向きを補正する
				rot->y = pCurve->rotDest.y;

				// 左に走る
				pCurve->curveInfo.dashAngle = DASH_LEFT;

				// 走行状態にする
				pCurve->actionState = CARACT_DASH;

				// スキップする回数を設定する
				pCurve->nSKipCnt = rand() % 2 + 1;
			}
		}
	}

	// 向きの正規化
	RotNormalize(rot->y);
}

//============================================================
//曲がり角の位置の取得処理
//============================================================
CURVEINFO GetCurveInfo(int nID)
{
	//曲がり角の情報を返す
	return g_aCurveInfo[nID];
}