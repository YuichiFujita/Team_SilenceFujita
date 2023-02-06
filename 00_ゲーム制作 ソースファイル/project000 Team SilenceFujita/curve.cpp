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
#include "Car.h"

#ifdef _DEBUG	// デバッグ処理
#include "game.h"
#endif

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define CURVE_MOVE_SLOW_MAGNI			(0.3f)				// カーブするときの減速係数
#define CURVE_ADD						(0.03f)				// カーブするときの向きの追加量
#define CURVE_TXT						"data\\TXT\\Curve.txt"		// カーブ設定用のテキストファイルの相対パス
#define CURVE_SKIP_RANDOM				(3)					// スキップする回数のランダムの範囲
#define CURVE_SKIP_LEAST				(2)					// スキップする回数の最低数

//**********************************************************************************************************************
//	プロトタイプ宣言
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
	for (int nCnt = 0; nCnt < MAX_CURVEPOINT; nCnt++)
	{ // 曲がり角の情報の初期化
		g_aCurveInfo[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_aCurveInfo[nCnt].nCurveNumber = nCnt;						// 番号
		g_aCurveInfo[nCnt].curveAngle = CURVE_RIGHT;				// 曲がる方向
		g_aCurveInfo[nCnt].dashAngle = DASH_RIGHT;					// 曲がり角にたどり着くまでの走っている方向
		g_aCurveInfo[nCnt].bDeadEnd = false;						// 行き止まりかどうか
	}
}

//============================================================
// 車の角度更新・補正処理
//============================================================
void CurveInfoRotCar(CARCURVE *pCurve, D3DXVECTOR3 *rot, D3DXVECTOR3 *move, D3DXVECTOR3 *pPos)
{
	//速度を落とす
	move->x *= CURVE_MOVE_SLOW_MAGNI;

	if (pCurve->curveInfo.curveAngle == CURVE_LEFT)
	{ // 曲がる方向が左だった場合
		if (pCurve->curveInfo.dashAngle == DASH_RIGHT)
		{ // 右に走っている場合

			// 位置を補正する
			pPos->x = pCurve->curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + CURVE_SKIP_LEAST;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_LEFT)
		{ // 左に走っている場合

			// 位置を補正する
			pPos->x = pCurve->curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + CURVE_SKIP_LEAST;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_FAR)
		{ // 奥に走っている場合

			// 位置を補正する
			pPos->z = pCurve->curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + CURVE_SKIP_LEAST;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_NEAR)
		{ // 手前に走っている場合

			// 位置を補正する
			pPos->z = pCurve->curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + CURVE_SKIP_LEAST;
			}
		}
	}
	else
	{ // 曲がる方向が右だった場合
		//速度を落とす
		move->x *= 0.3f;

		if (pCurve->curveInfo.dashAngle == DASH_RIGHT)
		{ // 右に走っている場合

			// 位置を補正する
			pPos->x = pCurve->curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + CURVE_SKIP_LEAST;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_LEFT)
		{ // 左に走っている場合

			// 位置を補正する
			pPos->x = pCurve->curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + CURVE_SKIP_LEAST;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_FAR)
		{ // 奥に走っている場合

			// 位置を補正する
			pPos->z = pCurve->curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + CURVE_SKIP_LEAST;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_NEAR)
		{ // 手前に走っている場合

			// 位置を補正する
			pPos->z = pCurve->curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + CURVE_SKIP_LEAST;
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

//============================================================
// カーブテキストのロード処理
//============================================================
void LoadCurveTxt(void)
{
	// 変数配列を宣言
	int			 nEnd;					// テキスト読み込み終了の確認用
	char         aString[MAX_STRING];	// テキストの文字列の代入用
	int			 nCurveNumber = 0;		// 曲がり角の番号
	char		 aCurveAngle[10];		// 曲がる方向
	char		 aDashAngle[10];		// 進んでいる方向
	char		 aDeadEnd[10];			// 行き止まりかどうかの代入用

	// ポインタを宣言
	FILE *pFile;				// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(CURVE_TXT, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			//------------------------------------------------
			//	カーブの設定
			//------------------------------------------------
			if (strcmp(&aString[0], "SET_CURVE") == 0)
			{ // 読み込んだ文字列が SET_CURVE の場合

				do
				{ // 読み込んだ文字列が END_SET_CURVE ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "CURVE_INFO") == 0)
					{ // 読み込んだ文字列が CURVE_INFO の場合

						fscanf(pFile, "%s", &aString[0]);			// NUMBER を読み込む (不要)
						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%d", &nCurveNumber);			// 番号を読み込む

						// 番号を代入する
						g_aCurveInfo[nCurveNumber].nCurveNumber = nCurveNumber;

						do
						{ // 読み込んだ文字列が END_CURVE_INFO ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "POS") == 0)
							{ // 読み込んだ文字列が POS の場合
								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%f%f%f", 
									&g_aCurveInfo[nCurveNumber].pos.x, 
									&g_aCurveInfo[nCurveNumber].pos.y, 
									&g_aCurveInfo[nCurveNumber].pos.z);						// 位置を読み込む
							}
							else if (strcmp(&aString[0], "CURVEANGLE") == 0)
							{ // 読み込んだ文字列が CURVEANGLE の場合
								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%s", &aCurveAngle[0]);						// 曲がる方向を読み込む

								if (strcmp(&aCurveAngle[0], "RIGHT") == 0)
								{ // 右を示していた場合
									// 右に曲がる設定をする
									g_aCurveInfo[nCurveNumber].curveAngle = CURVE_RIGHT;
								}
								else if (strcmp(&aCurveAngle[0], "LEFT") == 0)
								{ // 左を示していた場合
									// 左に曲がる設定をする
									g_aCurveInfo[nCurveNumber].curveAngle = CURVE_LEFT;
								}
							}
							else if (strcmp(&aString[0], "DASHANGLE") == 0)
							{ // 読み込んだ文字列が SCALE の場合
								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%s", &aDashAngle[0]);						// 進んでいる方向を読み込む

								if (strcmp(&aDashAngle[0], "RIGHT") == 0)
								{ // 右を示していた場合
									// 右に進んでいく設定をする
									g_aCurveInfo[nCurveNumber].dashAngle = DASH_RIGHT;
								}
								else if (strcmp(&aDashAngle[0], "LEFT") == 0)
								{ // 左を示していた場合
									// 左に進んでいく設定をする
									g_aCurveInfo[nCurveNumber].dashAngle = DASH_LEFT;
								}
								else if (strcmp(&aDashAngle[0], "FAR") == 0)
								{ // 奥を示していた場合
									// 奥に進んでいく設定をする
									g_aCurveInfo[nCurveNumber].dashAngle = DASH_FAR;
								}
								else if (strcmp(&aDashAngle[0], "NEAR") == 0)
								{ // 手前を示していた場合
									// 手前に進んでいく設定をする
									g_aCurveInfo[nCurveNumber].dashAngle = DASH_NEAR;
								}
							}
							else if (strcmp(&aString[0], "DEADEND") == 0)
							{ // 読み込んだ文字列が TYPE の場合
								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%s", &aDeadEnd[0]);							// 行き止まりかどうかを読み込む

								if (strcmp(&aDeadEnd[0], "true") == 0)
								{ // trueと書かれていた場合
									// 行き止まりにする
									g_aCurveInfo[nCurveNumber].bDeadEnd = true;
								}
								else
								{ // false(上記以外の言葉)と書かれていた場合
									// 行き止まりにしない
									g_aCurveInfo[nCurveNumber].bDeadEnd = false;
								}
							}
						} while (strcmp(&aString[0], "END_CURVE_INFO") != 0);	// 読み込んだ文字列が END_CURVE_INFO ではない場合ループ
					}
				} while (strcmp(&aString[0], "END_SET_CURVE") != 0);			// 読み込んだ文字列が END_SET_CURVE ではない場合ループ
			}
		} while (nEnd != EOF);													// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "カーブファイルの読み込みに失敗です", "ざまぁみろwww", MB_ICONWARNING);
	}
}