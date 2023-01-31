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

#ifdef _DEBUG	// デバッグ処理
#include "game.h"
#endif

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************

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

//======================================================================================================================
//	カーブの設定処理
//======================================================================================================================
void SetCurvePoint(CURVE *pCurve, D3DXVECTOR3 *rot, D3DXVECTOR3 *pos)
{
	for (int nCnt = 0; nCnt < pCurve->nCurveTime; nCnt++)
	{ // 走る軸と方向を決める
		if ((int)pCurve->curvePoint[nCnt].x == (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].x)
		{ // X座標が同じだったら
			// Z軸を走っている
			pCurve->bCurveX[(nCnt + 1) % pCurve->nCurveTime] = false;

			if ((int)pCurve->curvePoint[nCnt].z < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].z)
			{ // 行先のZ座標が今のポイントよりも奥だった場合
				// プラス方向に走っている(奥に走っている)
				pCurve->bCurvePlus[(nCnt + 1) % pCurve->nCurveTime] = true;
			}
			else
			{ // 行先のZ座標が今のポイントよりも手前だった場合
				// マイナス方向に走っている(手前に走っている)
				pCurve->bCurvePlus[(nCnt + 1) % pCurve->nCurveTime] = false;
			}
		}
		else
		{ // Z座標が同じだったら
			// X軸を走っている
			pCurve->bCurveX[(nCnt + 1) % pCurve->nCurveTime] = true;

			if ((int)pCurve->curvePoint[nCnt].x < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].x)
			{ // 行先のX座標が今のポイントよりも右だった場合
				// プラス方向に走っている
				pCurve->bCurvePlus[(nCnt + 1) % pCurve->nCurveTime] = true;
			}
			else
			{ // 行先のX座標が今のポイントよりも左だった場合
				// マイナス方向に走っている
				pCurve->bCurvePlus[(nCnt + 1) % pCurve->nCurveTime] = false;
			}
		}
	}

	for (int nCnt = 0; nCnt < pCurve->nCurveTime; nCnt++)
	{ // 走る曲がる方向を決める
		if (pCurve->bCurveX[nCnt] == true)
		{//X軸を走っている場合
			if (pCurve->bCurvePlus[nCnt] == true)
			{//右に走っている場合
				if ((int)pCurve->curvePoint[nCnt].z < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].z)
				{ // 次の目的地が奥だった場合
					// 左曲がりになる
					pCurve->curveAngle[nCnt] = CURVE_LEFT;
				}
				else
				{ // 次の目的地が手前だった場合
					// 右曲がりになる
					pCurve->curveAngle[nCnt] = CURVE_RIGHT;
				}
			}
			else
			{//左に走っている場合
				if ((int)pCurve->curvePoint[nCnt].z < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].z)
				{ // 次の目的地が奥だった場合
					// 左曲がりになる
					pCurve->curveAngle[nCnt] = CURVE_RIGHT;
				}
				else
				{ // 次の目的地が手前だった場合
					// 右曲がりになる
					pCurve->curveAngle[nCnt] = CURVE_LEFT;
				}
			}
		}
		else
		{//Z軸を走っている場合
			if (pCurve->bCurvePlus[nCnt] == true)
			{//奥に走っている場合
				if ((int)pCurve->curvePoint[nCnt].x < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].x)
				{ // 次の目的地が右だった場合
					//右曲がるになる
					pCurve->curveAngle[nCnt] = CURVE_RIGHT;
				}
				else
				{ // 次の目的地が左だった場合
					//右曲がるになる
					pCurve->curveAngle[nCnt] = CURVE_LEFT;
				}
			}
			else
			{//手前に走っている場合
				if ((int)pCurve->curvePoint[nCnt].x < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].x)
				{ // 次の目的地が右だった場合
					//右曲がるになる
					pCurve->curveAngle[nCnt] = CURVE_LEFT;
				}
				else
				{ // 次の目的地が左だった場合
					//右曲がるになる
					pCurve->curveAngle[nCnt] = CURVE_RIGHT;
				}
			}
		}
	}

	for (int nCnt = 0; nCnt < pCurve->nCurveTime; nCnt++)
	{ // 曲がる回数分回す
		if (pCurve->bCurveX[nCnt] == true)
		{ // X軸を移動していた場合
			if (pCurve->bCurvePlus[nCnt] == true)
			{ // 右方向に移動していた場合

				//曲がるポイントのZ軸を75.0f戻す
				pCurve->curvePoint[nCnt].z -= SHIFT_CAR_CURVE;

				if (nCnt == 0)
				{//一回目の場合
					//向きを90度に設定する
					rot->y = D3DXToRadian(90);
				}

				if (pCurve->curveAngle[nCnt] == CURVE_RIGHT)
				{ // 右に曲がる場合
					//曲がるポイントのX軸を75.0f進める
					pCurve->curvePoint[nCnt].x -= SHIFT_CAR_CURVE;
				}
				else if (pCurve->curveAngle[nCnt] == CURVE_LEFT)
				{ // 左に曲がる場合
				  //曲がるポイントのX軸を75.0f戻す
					pCurve->curvePoint[nCnt].x += SHIFT_CAR_CURVE;
				}
			}
			else
			{ // 左方向に移動していた場合

				//曲がるポイントのZ軸を75.0f進める
				pCurve->curvePoint[nCnt].z += SHIFT_CAR_CURVE;

				if (nCnt == 0)
				{//一回目の場合
					//向きを-90度に設定する
					rot->y = D3DXToRadian(-90);
				}

				if (pCurve->curveAngle[nCnt] == CURVE_RIGHT)
				{ // 右に曲がる場合
				  //曲がるポイントのX軸を75.0f進める
					pCurve->curvePoint[nCnt].x += SHIFT_CAR_CURVE;
				}
				else if (pCurve->curveAngle[nCnt] == CURVE_LEFT)
				{ // 左に曲がる場合
				  //曲がるポイントのX軸を75.0f戻る
					pCurve->curvePoint[nCnt].x -= SHIFT_CAR_CURVE;
				}
			}
		}
		else
		{ // Z軸を移動していた場合
			if (pCurve->bCurvePlus[nCnt] == true)
			{ // 奥方向に移動していた場合

				//曲がるポイントのX軸を進める
				pCurve->curvePoint[nCnt].x += SHIFT_CAR_CURVE;

				if (nCnt == 0)
				{//一回目の場合
				 //向きを0度に設定する
					rot->y = D3DXToRadian(0);
				}

				if (pCurve->curveAngle[nCnt] == CURVE_RIGHT)
				{ // 右に曲がる場合
				  //曲がるポイントのZ軸を75.0f進める
					pCurve->curvePoint[nCnt].z -= SHIFT_CAR_CURVE;
				}
				else if (pCurve->curveAngle[nCnt] == CURVE_LEFT)
				{ // 左に曲がる場合
				  //曲がるポイントのZ軸を75.0f戻す
					pCurve->curvePoint[nCnt].z += SHIFT_CAR_CURVE;
				}
			}
			else
			{ // 手前方向に移動していた場合

				//曲がるポイントのX軸を75.0f戻す
				pCurve->curvePoint[nCnt].x -= SHIFT_CAR_CURVE;

				if (pCurve->curveAngle[nCnt] == CURVE_RIGHT)
				{ // 右に曲がる場合
					//曲がるポイントのZ軸を75.0f進める
					pCurve->curvePoint[nCnt].z += SHIFT_CAR_CURVE;

					if (nCnt == 0)
					{//一回目の場合
						//向きを-180度に設定する
						rot->y = D3DXToRadian(-180);
					}
				}
				else if (pCurve->curveAngle[nCnt] == CURVE_LEFT)
				{ // 左に曲がる場合
					//曲がるポイントのZ軸を75.0f戻す
					pCurve->curvePoint[nCnt].z -= SHIFT_CAR_CURVE;

					if (nCnt == 0)
					{//一回目の場合
					 //向きを180度に設定する
						rot->y = D3DXToRadian(180);
					}
				}
			}
		}
	}
}

//======================================================================================================================
//	人間のカーブの設定処理
//======================================================================================================================
void SetCurvePointHuman(CURVE *pCurve, D3DXVECTOR3 *rot, D3DXVECTOR3 *pos)
{
	for (int nCnt = 0; nCnt < pCurve->nCurveTime; nCnt++)
	{ // 走る軸と方向を決める
		if ((int)pCurve->curvePoint[nCnt].x == (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].x)
		{ // X座標が同じだったら
			// Z軸を走っている
			pCurve->bCurveX[(nCnt + 1) % pCurve->nCurveTime] = false;

			if ((int)pCurve->curvePoint[nCnt].z < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].z)
			{ // 行先のZ座標が今のポイントよりも奥だった場合
				// プラス方向に走っている(奥に走っている)
				pCurve->bCurvePlus[(nCnt + 1) % pCurve->nCurveTime] = true;
			}
			else
			{ // 行先のZ座標が今のポイントよりも手前だった場合
				// マイナス方向に走っている(手前に走っている)
				pCurve->bCurvePlus[(nCnt + 1) % pCurve->nCurveTime] = false;
			}
		}
		else
		{ // Z座標が同じだったら
			// X軸を走っている
			pCurve->bCurveX[(nCnt + 1) % pCurve->nCurveTime] = true;

			if ((int)pCurve->curvePoint[nCnt].x < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].x)
			{ // 行先のX座標が今のポイントよりも右だった場合
				// プラス方向に走っている
				pCurve->bCurvePlus[(nCnt + 1) % pCurve->nCurveTime] = true;
			}
			else
			{ // 行先のX座標が今のポイントよりも左だった場合
				// マイナス方向に走っている
				pCurve->bCurvePlus[(nCnt + 1) % pCurve->nCurveTime] = false;
			}
		}
	}

	for (int nCnt = 0; nCnt < pCurve->nCurveTime; nCnt++)
	{ // 走る曲がる方向を決める
		if (pCurve->bCurveX[nCnt] == true)
		{//X軸を走っている場合
			if (pCurve->bCurvePlus[nCnt] == true)
			{//右に走っている場合
				if ((int)pCurve->curvePoint[nCnt].z < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].z)
				{ // 次の目的地が奥だった場合
					// 左曲がりになる
					pCurve->curveAngle[nCnt] = CURVE_LEFT;
				}
				else
				{ // 次の目的地が手前だった場合
					// 右曲がりになる
					pCurve->curveAngle[nCnt] = CURVE_RIGHT;
				}
			}
			else
			{//左に走っている場合
				if ((int)pCurve->curvePoint[nCnt].z < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].z)
				{ // 次の目的地が奥だった場合
					// 左曲がりになる
					pCurve->curveAngle[nCnt] = CURVE_RIGHT;
				}
				else
				{ // 次の目的地が手前だった場合
					// 右曲がりになる
					pCurve->curveAngle[nCnt] = CURVE_LEFT;
				}
			}
		}
		else
		{//Z軸を走っている場合
			if (pCurve->bCurvePlus[nCnt] == true)
			{//奥に走っている場合
				if ((int)pCurve->curvePoint[nCnt].x < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].x)
				{ // 次の目的地が右だった場合
					//右曲がるになる
					pCurve->curveAngle[nCnt] = CURVE_RIGHT;
				}
				else
				{ // 次の目的地が左だった場合
					//右曲がるになる
					pCurve->curveAngle[nCnt] = CURVE_LEFT;
				}
			}
			else
			{//手前に走っている場合
				if ((int)pCurve->curvePoint[nCnt].x < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].x)
				{ // 次の目的地が右だった場合
					//右曲がるになる
					pCurve->curveAngle[nCnt] = CURVE_LEFT;
				}
				else
				{ // 次の目的地が左だった場合
					//右曲がるになる
					pCurve->curveAngle[nCnt] = CURVE_RIGHT;
				}
			}
		}
	}

	for (int nCnt = 0; nCnt < pCurve->nCurveTime; nCnt++)
	{ // 曲がる回数分回す
		if (pCurve->bCurveX[nCnt] == true)
		{ // X軸を移動していた場合
			if (pCurve->bCurvePlus[nCnt] == true)
			{ // 右方向に移動していた場合

				//曲がるポイントのZ軸を75.0f戻す
				pCurve->curvePoint[nCnt].z += SHIFT_HUMAN_CURVE;

				if (nCnt == 0)
				{//一回目の場合
					//向きを90度に設定する
					rot->y = D3DXToRadian(90);
				}

				if (pCurve->curveAngle[nCnt] == CURVE_RIGHT)
				{ // 右に曲がる場合
					//曲がるポイントのX軸を75.0f進める
					pCurve->curvePoint[nCnt].x += SHIFT_HUMAN_CURVE;
				}
				else if (pCurve->curveAngle[nCnt] == CURVE_LEFT)
				{ // 左に曲がる場合
					//曲がるポイントのX軸を75.0f戻す
					pCurve->curvePoint[nCnt].x -= SHIFT_HUMAN_CURVE;
				}
			}
			else
			{ // 左方向に移動していた場合

				//曲がるポイントのZ軸を75.0f進める
				pCurve->curvePoint[nCnt].z -= SHIFT_HUMAN_CURVE;

				if (nCnt == 0)
				{//一回目の場合
					//向きを-90度に設定する
					rot->y = D3DXToRadian(-90);
				}

				if (pCurve->curveAngle[nCnt] == CURVE_RIGHT)
				{ // 右に曲がる場合
					//曲がるポイントのX軸を75.0f進める
					pCurve->curvePoint[nCnt].x -= SHIFT_HUMAN_CURVE;
				}
				else if (pCurve->curveAngle[nCnt] == CURVE_LEFT)
				{ // 左に曲がる場合
					//曲がるポイントのX軸を75.0f戻る
					pCurve->curvePoint[nCnt].x += SHIFT_HUMAN_CURVE;
				}
			}
		}
		else
		{ // Z軸を移動していた場合
			if (pCurve->bCurvePlus[nCnt] == true)
			{ // 奥方向に移動していた場合

				//曲がるポイントのX軸を進める
				pCurve->curvePoint[nCnt].x -= SHIFT_HUMAN_CURVE;

				if (nCnt == 0)
				{//一回目の場合
					//向きを0度に設定する
					rot->y = D3DXToRadian(0);
				}

				if (pCurve->curveAngle[nCnt] == CURVE_RIGHT)
				{ // 右に曲がる場合
					//曲がるポイントのZ軸を75.0f進める
					pCurve->curvePoint[nCnt].z += SHIFT_HUMAN_CURVE;
				}
				else if (pCurve->curveAngle[nCnt] == CURVE_LEFT)
				{ // 左に曲がる場合
					//曲がるポイントのZ軸を75.0f戻す
					pCurve->curvePoint[nCnt].z -= SHIFT_HUMAN_CURVE;
				}
			}
			else
			{ // 手前方向に移動していた場合

				//曲がるポイントのX軸を75.0f戻す
				pCurve->curvePoint[nCnt].x += SHIFT_HUMAN_CURVE;

				if (pCurve->curveAngle[nCnt] == CURVE_RIGHT)
				{ // 右に曲がる場合
					//曲がるポイントのZ軸を75.0f進める
					pCurve->curvePoint[nCnt].z -= SHIFT_HUMAN_CURVE;

					if (nCnt == 0)
					{//一回目の場合
						//向きを-180度に設定する
						rot->y = D3DXToRadian(-180);
					}
				}
				else if (pCurve->curveAngle[nCnt] == CURVE_LEFT)
				{ // 左に曲がる場合
					//曲がるポイントのZ軸を75.0f戻す
					pCurve->curvePoint[nCnt].z += SHIFT_HUMAN_CURVE;

					if (nCnt == 0)
					{//一回目の場合
						//向きを180度に設定する
						rot->y = D3DXToRadian(180);
					}
				}
			}
		}
	}
}

//============================================================
// 車の角度更新・補正処理
//============================================================
void CurveRotCar(CURVE *pCurve,D3DXVECTOR3 *rot,D3DXVECTOR3 *move)
{
	if (pCurve->curveAngle[pCurve->nNowCurve] == CURVE_LEFT)
	{ // 曲がる方向が左だった場合
		// 向きを更新
		rot->y -= 0.05f * (move->x * 0.1f);

		if (rot->y <= (float)D3DXToRadian(pCurve->fCurveRot[pCurve->nNowCurve]))
		{ // 一定の向きに達した場合
			// 向きを補正
			rot->y = (float)D3DXToRadian(pCurve->fCurveRot[pCurve->nNowCurve]);

			if (pCurve->curveAngle[(pCurve->nNowCurve + 1) % pCurve->nCurveTime] == CURVE_LEFT)
			{//次の曲がり角が左だった場合
				if (pCurve->fCurveRot[(pCurve->nNowCurve + 1) % pCurve->nCurveTime] == 90)
				{//次の曲がり角がマイナス方向に曲がる場合
					//向きをプラスに戻す
					rot->y = (float)D3DXToRadian(180);
				}
			}

			//// 車の補正の更新処理
			//RevCar(&pCar->rot, &pCar->pos);

			// 警察の行先を設定する
			pCurve->nNowCurve = (pCurve->nNowCurve + 1) % pCurve->nCurveTime;
		}
	}
	else if (pCurve->curveAngle[pCurve->nNowCurve] == CURVE_RIGHT)
	{ // 曲がる方向が右だった場合
		// 向きを更新
		rot->y += 0.05f * (move->x * 0.1f);

		if (rot->y >= (float)D3DXToRadian(pCurve->fCurveRot[pCurve->nNowCurve]))
		{ // 一定の向きに達した場合
			// 向きを補正
			rot->y = (float)D3DXToRadian(pCurve->fCurveRot[pCurve->nNowCurve]);

			if (pCurve->curveAngle[(pCurve->nNowCurve + 1) % pCurve->nCurveTime] == CURVE_RIGHT)
			{//次の曲がり角が右だった場合
				if (pCurve->fCurveRot[(pCurve->nNowCurve + 1) % pCurve->nCurveTime] == -90)
				{//次の曲がり角がマイナス方向に曲がる場合
					//向きをマイナスに戻す
					rot->y = (float)D3DXToRadian(-180);
				}
			}

			//// 車の補正の更新処理
			//RevCar(&pCar->rot, &pCar->pos);

			// 警察の行先を設定する
			pCurve->nNowCurve = (pCurve->nNowCurve + 1) % pCurve->nCurveTime;
		}
	}
}

//============================================================
// 車の角度更新・補正処理
//============================================================
void CurveInfoRotCar(CURVEINFO *pCurve, D3DXVECTOR3 *rot, D3DXVECTOR3 *move, int *pCnt)
{
	if (pCurve->curveAngle == CURVE_LEFT)
	{ // 曲がる方向が左だった場合
		// 向きを更新
		rot->y -= D3DX_PI * 0.5f;

		//速度を落とす
		move->x *= 0.3f;

		if (pCurve->dashAngle == DASH_RIGHT)
		{ // 右に走っている場合
			// 奥に走る
			pCurve->dashAngle = DASH_FAR;
		}
		else if (pCurve->dashAngle == DASH_LEFT)
		{ // 左に走っている場合
			// 手前に走る
			pCurve->dashAngle = DASH_NEAR;
		}
		else if (pCurve->dashAngle == DASH_FAR)
		{ // 奥に走っている場合
			// 左に走る
			pCurve->dashAngle = DASH_LEFT;
		}
		else if (pCurve->dashAngle == DASH_NEAR)
		{ // 手前に走っている場合
			// 右に走る
			pCurve->dashAngle = DASH_RIGHT;
		}

		*pCnt = rand() % 2 + 1;
	}
	else
	{ // 曲がる方向が右だった場合
		// 向きを更新
		rot->y += D3DX_PI * 0.5f;

		//速度を落とす
		move->x *= 0.3f;

		if (pCurve->dashAngle == DASH_RIGHT)
		{ // 右に走っている場合
			// 奥に走る
			pCurve->dashAngle = DASH_NEAR;
		}
		else if (pCurve->dashAngle == DASH_LEFT)
		{ // 左に走っている場合
			// 手前に走る
			pCurve->dashAngle = DASH_FAR;
		}
		else if (pCurve->dashAngle == DASH_FAR)
		{ // 奥に走っている場合
			// 左に走る
			pCurve->dashAngle = DASH_RIGHT;
		}
		else if (pCurve->dashAngle == DASH_NEAR)
		{ // 手前に走っている場合
			// 右に走る
			pCurve->dashAngle = DASH_LEFT;
		}

		*pCnt = rand() % 2 + 1;
	}
}

//============================================================
//曲がり角の位置の取得処理
//============================================================
CURVEINFO GetCurveInfo(int nID)
{
	//曲がり角の情報を返す
	return g_aCurveInfo[nID];
}