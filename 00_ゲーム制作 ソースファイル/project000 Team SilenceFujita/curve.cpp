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
#include "Human.h"

#ifdef _DEBUG	// デバッグ処理
#include "game.h"
#endif

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define CURVE_MOVE_SLOW_MAGNI			(0.3f)				// カーブするときの減速係数
#define CURVE_ADD						(0.03f)				// カーブするときの向きの追加量
#define CURVE_TXT						"data\\TXT\\Curve.txt"		// カーブ設定用のテキストファイルの相対パス
#define HUMAN_CURVE_TXT					"data\\TXT\\HumanCurve.txt"	// 人間の曲がり角(ルート)の保存用テキストファイルの相対パス
#define CURVE_SKIP_RANDOM				(3)					// スキップする回数のランダムの範囲
#define CURVE_SKIP_LEAST				(2)					// スキップする回数の最低数

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
CURVEINFO g_aCurveInfo[MAX_CURVEPOINT];						// 車のカーブの情報
HUMANCURVEINFO g_aCurveHuman[MAX_HUMAN_ROUTE];				// 人間のカーブの情報
float g_aDefaultRot[MAX_HUMAN_ROUTE];						// 最初の曲がり角

//======================================================================================================================
// カーブの情報の初期化処理
//======================================================================================================================
void InitCurveInfo(void)
{
	for (int nCnt = 0; nCnt < MAX_CURVEPOINT; nCnt++)
	{ // 車の曲がり角の情報の初期化
		g_aCurveInfo[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_aCurveInfo[nCnt].nCurveNumber = nCnt;						// 番号
		g_aCurveInfo[nCnt].curveAngle = CURVE_RIGHT;				// 曲がる方向
		g_aCurveInfo[nCnt].dashAngle = DASH_RIGHT;					// 曲がり角にたどり着くまでの走っている方向
		g_aCurveInfo[nCnt].bDeadEnd = false;						// 行き止まりかどうか
	}

	for (int nCnt = 0; nCnt < MAX_HUMAN_ROUTE; nCnt++)
	{ // 人間のルートの情報の初期化
		for (int nCntCur = 0; nCntCur < MAX_HUMAN_CURVE; nCntCur++)
		{
			g_aCurveHuman[nCnt].curvePoint[nCntCur] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
			g_aCurveHuman[nCnt].curveAngle[nCntCur] = CURVE_LEFT;						// 曲がる方向
			g_aCurveHuman[nCnt].dashAngle[nCntCur] = DASH_RIGHT;						// 走っている方向
			g_aCurveHuman[nCnt].roadPos[nCntCur] = ROADPOS_RIGHT;						// 曲がり角よりどっち側に歩いているか
			g_aCurveHuman[nCnt].nNowCurve = 0;											// 現在の曲がり角の番号
			g_aCurveHuman[nCnt].nCurveTime = 1;											// 曲がる回数
		}
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
	RotNormalize(&rot->y);
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

//============================================================
// 人間のルートのロード処理
//============================================================
void LoadHumanCurveTxt(void)
{
	// 変数配列を宣言
	int			 nEnd;					// テキスト読み込み終了の確認用
	char         aString[MAX_STRING];	// テキストの文字列の代入用
	int			 nRoute = 0;			// 何個目のルートか

	// ポインタを宣言
	FILE *pFile;				// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(HUMAN_CURVE_TXT, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			//------------------------------------------------
			//	カーブの設定
			//------------------------------------------------

			if (strcmp(&aString[0], "CURVE_INFO") == 0)
			{ // 読み込んだ文字列が CURVE_INFO の場合

				do
				{ // 読み込んだ文字列が END_CURVE_INFO ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "TIME") == 0)
					{ // 読み込んだ文字列が TIME の場合
						fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
						fscanf(pFile, "%d", &g_aCurveHuman[nRoute].nCurveTime);		// 曲がる回数を読み込む
					}
					else if (strcmp(&aString[0], "POINT") == 0)
					{ // 読み込んだ文字列が POINT の場合
						for (int nCnt = 0; nCnt < g_aCurveHuman[nRoute].nCurveTime; nCnt++)
						{
							fscanf(pFile, "%s", &aString[0]);						// POS を読み込む (不要)
							fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
							fscanf(pFile, "%f%f%f",
								&g_aCurveHuman[nRoute].curvePoint[nCnt].x,			// 位置を読み込む
								&g_aCurveHuman[nRoute].curvePoint[nCnt].y,
								&g_aCurveHuman[nRoute].curvePoint[nCnt].z);

							fscanf(pFile, "%s", &aString[0]);						// ROADPOS を読み込む (不要)
							fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
							fscanf(pFile, "%s", &aString[0]);						// 書かれている方向を読み込む

							if (strcmp(&aString[0], "RIGHT") == 0)
							{ // 右と書いてあった場合
								// 曲がり角より右に設定する
								g_aCurveHuman[nRoute].roadPos[nCnt] = ROADPOS_RIGHT;
							}
							else if (strcmp(&aString[0], "LEFT") == 0)
							{ // 左と書いてあった場合
								// 曲がり角より左に設定する
								g_aCurveHuman[nRoute].roadPos[nCnt] = ROADPOS_LEFT;
							}
						}
					}
				} while (strcmp(&aString[0], "END_CURVE_INFO") != 0);	// 読み込んだ文字列が END_CURVE_INFO ではない場合ループ

				// ルートの数を加算する
				nRoute++;
			}
		} while (nEnd != EOF);											// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

	  // エラーメッセージボックス
		MessageBox(NULL, "カーブファイルの読み込みに失敗です", "ざまぁみろwww", MB_ICONWARNING);
	}
}

//============================================================
// 曲がり角の設定処理
//============================================================
void SetCurvePoint(void)
{
	for (int nCntCur = 0; nCntCur < MAX_HUMAN_ROUTE; nCntCur++)
	{ // ルート分設定する
		for (int nCnt = 0; nCnt < g_aCurveHuman[nCntCur].nCurveTime; nCnt++)
		{ // 走る軸と方向を決める

			if ((int)g_aCurveHuman[nCntCur].curvePoint[nCnt].x == (int)g_aCurveHuman[nCntCur].curvePoint[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime].x)
			{ // X座標が同じだったら

				if ((int)g_aCurveHuman[nCntCur].curvePoint[nCnt].z < (int)g_aCurveHuman[nCntCur].curvePoint[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime].z)
				{ // 行先のZ座標が今のポイントよりも奥だった場合

					// 奥に走っている
					g_aCurveHuman[nCntCur].dashAngle[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] = DASH_FAR;
				}
				else
				{ // 行先のZ座標が今のポイントよりも手前だった場合

					// 手前に走っている
					g_aCurveHuman[nCntCur].dashAngle[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] = DASH_NEAR;
				}
			}
			else
			{ // Z座標が同じだったら
				if ((int)g_aCurveHuman[nCntCur].curvePoint[nCnt].x < (int)g_aCurveHuman[nCntCur].curvePoint[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime].x)
				{ // 行先のX座標が今のポイントよりも右だった場合

					// 右に走っている
					g_aCurveHuman[nCntCur].dashAngle[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] = DASH_RIGHT;
				}
				else
				{ // 行先のX座標が今のポイントよりも左だった場合

					// 左方向に走っている
					g_aCurveHuman[nCntCur].dashAngle[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] = DASH_LEFT;
				}
			}
		}

		for (int nCnt = 0; nCnt < g_aCurveHuman[nCntCur].nCurveTime; nCnt++)
		{ // 走る曲がる方向を決める

			if (g_aCurveHuman[nCntCur].dashAngle[nCnt] == DASH_RIGHT)
			{ // 右に走っている場合
				if ((int)g_aCurveHuman[nCntCur].curvePoint[nCnt].z < (int)g_aCurveHuman[nCntCur].curvePoint[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime].z)
				{ // 次の目的地が奥だった場合

					// 左曲がりになる
					g_aCurveHuman[nCntCur].curveAngle[nCnt] = CURVE_LEFT;
				}
				else
				{ // 次の目的地が手前だった場合

					// 右曲がりになる
					g_aCurveHuman[nCntCur].curveAngle[nCnt] = CURVE_RIGHT;
				}
			}
			else if (g_aCurveHuman[nCntCur].dashAngle[nCnt] == DASH_LEFT)
			{ // 左に走っている場合

				if ((int)g_aCurveHuman[nCntCur].curvePoint[nCnt].z < (int)g_aCurveHuman[nCntCur].curvePoint[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime].z)
				{ // 次の目的地が奥だった場合

					// 右曲がりになる
					g_aCurveHuman[nCntCur].curveAngle[nCnt] = CURVE_RIGHT;
				}
				else
				{ // 次の目的地が手前だった場合

					// 左曲がりになる
					g_aCurveHuman[nCntCur].curveAngle[nCnt] = CURVE_LEFT;
				}
			}
			else if (g_aCurveHuman[nCntCur].dashAngle[nCnt] == DASH_FAR)
			{ // 奥に走っている場合

				if ((int)g_aCurveHuman[nCntCur].curvePoint[nCnt].x < (int)g_aCurveHuman[nCntCur].curvePoint[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime].x)
				{ // 次の目的地が右だった場合

					// 右曲がりになる
					g_aCurveHuman[nCntCur].curveAngle[nCnt] = CURVE_RIGHT;
				}
				else
				{ // 次の目的地が左だった場合

					// 左曲がりになる
					g_aCurveHuman[nCntCur].curveAngle[nCnt] = CURVE_LEFT;
				}
			}
			else if (g_aCurveHuman[nCntCur].dashAngle[nCnt] == DASH_NEAR)
			{ // 手前に走っている場合

				if ((int)g_aCurveHuman[nCntCur].curvePoint[nCnt].x < (int)g_aCurveHuman[nCntCur].curvePoint[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime].x)
				{ // 次の目的地が右だった場合

					// 左曲がりになる
					g_aCurveHuman[nCntCur].curveAngle[nCnt] = CURVE_LEFT;
				}
				else
				{ // 次の目的地が左だった場合

					// 右曲がりになる
					g_aCurveHuman[nCntCur].curveAngle[nCnt] = CURVE_RIGHT;
				}
			}
		}

		for (int nCnt = 0; nCnt < g_aCurveHuman[nCntCur].nCurveTime; nCnt++)
		{ // 曲がる回数分回す
			if (g_aCurveHuman[nCntCur].dashAngle[nCnt] == DASH_RIGHT)
			{ // 右方向に移動していた場合

				if (nCnt == 0)
				{ // 一回目の場合
					// 向きを90度に設定する
					g_aDefaultRot[nCntCur] = D3DXToRadian(90);
				}

				if (g_aCurveHuman[nCntCur].roadPos[nCnt] == ROADPOS_RIGHT)
				{ // 曲がり角より右側を通る場合
					// 曲がるポイントのZ軸を75.0f戻す
					g_aCurveHuman[nCntCur].curvePoint[nCnt].z -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
				}
				else if (g_aCurveHuman[nCntCur].roadPos[nCnt] == ROADPOS_LEFT)
				{ // 曲がり角より左側を通る場合
					// 曲がるポイントのZ軸を75.0f戻す
					g_aCurveHuman[nCntCur].curvePoint[nCnt].z += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
				}

				switch (g_aCurveHuman[nCntCur].curveAngle[nCnt])
				{
				case CURVE_RIGHT:	// 右に曲がる

					if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_RIGHT)
					{ // 次の通るところが曲がり角より右だったら
						// 曲がるポイントのX軸を75.0fずらす
						g_aCurveHuman[nCntCur].curvePoint[nCnt].x -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}
					else if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_LEFT)
					{ // 次の通るところが曲がり角より左だったら
						// 曲がるポイントのX軸を75.0fずらす
						g_aCurveHuman[nCntCur].curvePoint[nCnt].x += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}

					break;			// 抜け出す

				case CURVE_LEFT:	// 左に曲がる

					if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_RIGHT)
					{ // 次の通るところが曲がり角より右だったら
						// 曲がるポイントのX軸を75.0fずらす
						g_aCurveHuman[nCntCur].curvePoint[nCnt].x += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}
					else if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_LEFT)
					{ // 次の通るところが曲がり角より左だったら
						// 曲がるポイントのX軸を75.0fずらす
						g_aCurveHuman[nCntCur].curvePoint[nCnt].x -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}

					break;			// 抜け出す
				}
			}
			else if (g_aCurveHuman[nCntCur].dashAngle[nCnt] == DASH_LEFT)
			{ // 左方向に移動していた場合

				if (nCnt == 0)
				{ // 一回目の場合
					// 向きを-90度に設定する
					g_aDefaultRot[nCntCur] = D3DXToRadian(-90);
				}

				if (g_aCurveHuman[nCntCur].roadPos[nCnt] == ROADPOS_RIGHT)
				{ // 曲がり角より右側を通る場合
					// 曲がるポイントのZ軸を75.0f進める
					g_aCurveHuman[nCntCur].curvePoint[nCnt].z += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
				}
				else if (g_aCurveHuman[nCntCur].roadPos[nCnt] == ROADPOS_LEFT)
				{ // 曲がり角より左側を通る場合
					// 曲がるポイントのZ軸を75.0f進める
					g_aCurveHuman[nCntCur].curvePoint[nCnt].z -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
				}

				switch (g_aCurveHuman[nCntCur].curveAngle[nCnt])
				{
				case CURVE_RIGHT:	// 右に曲がる

					if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_RIGHT)
					{ // 次の通るところが曲がり角より右だったら
						// 曲がるポイントのX軸を75.0f進める
						g_aCurveHuman[nCntCur].curvePoint[nCnt].x += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}
					else if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_LEFT)
					{ // 次の通るところが曲がり角より左だったら
						// 曲がるポイントのX軸を75.0f進める
						g_aCurveHuman[nCntCur].curvePoint[nCnt].x -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}

					break;			// 抜け出す

				case CURVE_LEFT:	// 左に曲がる

					if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_RIGHT)
					{ // 次の通るところが曲がり角より右だったら
						// 曲がるポイントのX軸を75.0f進める
						g_aCurveHuman[nCntCur].curvePoint[nCnt].x -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}
					else if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_LEFT)
					{ // 次の通るところが曲がり角より左だったら
						// 曲がるポイントのX軸を75.0f進める
						g_aCurveHuman[nCntCur].curvePoint[nCnt].x += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}

					break;			// 抜け出す
				}
			}
			else if (g_aCurveHuman[nCntCur].dashAngle[nCnt] == DASH_FAR)
			{ // 奥方向に移動していた場合

				if (nCnt == 0)
				{ // 一回目の場合
					// 向きを0度に設定する
					g_aDefaultRot[nCntCur] = D3DXToRadian(0);
				}

				if (g_aCurveHuman[nCntCur].roadPos[nCnt] == ROADPOS_RIGHT)
				{ // 曲がり角より右側を通る場合
					// 曲がるポイントのX軸を進める
					g_aCurveHuman[nCntCur].curvePoint[nCnt].x += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
				}
				else if (g_aCurveHuman[nCntCur].roadPos[nCnt] == ROADPOS_LEFT)
				{ // 曲がり角より左側を通る場合
					// 曲がるポイントのX軸を進める
					g_aCurveHuman[nCntCur].curvePoint[nCnt].x -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
				}

				switch (g_aCurveHuman[nCntCur].curveAngle[nCnt])
				{
				case CURVE_RIGHT:	// 右に曲がる

					if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_RIGHT)
					{ // 次の通るところが曲がり角より右だったら
						// 曲がるポイントのZ軸を75.0f進める
						g_aCurveHuman[nCntCur].curvePoint[nCnt].z -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}
					else if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_LEFT)
					{ // 次の通るところが曲がり角より左だったら
						// 曲がるポイントのZ軸を75.0f進める
						g_aCurveHuman[nCntCur].curvePoint[nCnt].z += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}

					break;			// 抜け出す

				case CURVE_LEFT:	// 左に曲がる

					if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_RIGHT)
					{ // 次の通るところが曲がり角より右だったら
						// 曲がるポイントのZ軸を75.0f進める
						g_aCurveHuman[nCntCur].curvePoint[nCnt].z += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}
					else if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_LEFT)
					{ // 次の通るところが曲がり角より左だったら
						// 曲がるポイントのZ軸を75.0f進める
						g_aCurveHuman[nCntCur].curvePoint[nCnt].z -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}

					break;			// 抜け出す
				}
			}
			else if(g_aCurveHuman[nCntCur].dashAngle[nCnt] == DASH_NEAR)
			{ // 手前方向に移動していた場合

				if (g_aCurveHuman[nCntCur].roadPos[nCnt] == ROADPOS_RIGHT)
				{ // 曲がり角より右側を通る場合
					// 曲がるポイントのX軸を75.0f戻す
					g_aCurveHuman[nCntCur].curvePoint[nCnt].x -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
				}
				else if (g_aCurveHuman[nCntCur].roadPos[nCnt] == ROADPOS_LEFT)
				{ // 曲がり角より左側を通る場合
					// 曲がるポイントのX軸を75.0f戻す
					g_aCurveHuman[nCntCur].curvePoint[nCnt].x += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
				}

				switch (g_aCurveHuman[nCntCur].curveAngle[nCnt])
				{
				case CURVE_RIGHT:		// 右に曲がる

					if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_RIGHT)
					{ // 次の通るところが曲がり角より右だったら
						// 曲がるポイントのZ軸を75.0f進める
						g_aCurveHuman[nCntCur].curvePoint[nCnt].z += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}
					else if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_LEFT)
					{ // 次の通るところが曲がり角より左だったら
						// 曲がるポイントのZ軸を75.0f進める
						g_aCurveHuman[nCntCur].curvePoint[nCnt].z -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}

					if (nCnt == 0)
					{ // 一回目の場合
						// 向きを-180度に設定する
						g_aDefaultRot[nCntCur] = D3DXToRadian(-180);
					}

					break;				// 抜け出す

				case CURVE_LEFT:		// 左に曲がる

					if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_RIGHT)
					{ // 次の通るところが曲がり角より右だったら
						// 曲がるポイントのZ軸を75.0f進める
						g_aCurveHuman[nCntCur].curvePoint[nCnt].z -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}
					else if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_LEFT)
					{ // 次の通るところが曲がり角より左だったら
						// 曲がるポイントのZ軸を75.0f進める
						g_aCurveHuman[nCntCur].curvePoint[nCnt].z += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}

					if (nCnt == 0)
					{ // 一回目の場合
						// 向きを180度に設定する
						g_aDefaultRot[nCntCur] = D3DXToRadian(180);
					}

					break;				// 抜け出す
				}
			}
		}
	}
}

//============================================================
// 人間のルートの取得処理
//============================================================
HUMANCURVEINFO GetHumanRoute(int nID)
{
	// カーブの情報を返す
	return g_aCurveHuman[nID];
}

//============================================================
// 人間の初期向きの取得処理
//============================================================
float GetDefaultRot(int nID)
{
	// 初期向きを返す
	return g_aDefaultRot[nID];
}