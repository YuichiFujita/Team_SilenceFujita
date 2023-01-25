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
#define SHIFT_CAR_CURVE		(45.0f)			//車のカーブでずらす幅
#define SHIFT_HUMAN_CURVE	(100.0f)		//人のカーブでずらす幅

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