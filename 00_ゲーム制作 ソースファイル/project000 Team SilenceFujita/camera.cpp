//============================================================
//
//	カメラ処理 [camera.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "input.h"
#include "camera.h"
#include "player.h"

#include "Human.h"

#include "ranking.h"

#ifdef _DEBUG	// デバッグ処理
#include "game.h"
#endif

//************************************************************
//	マクロ定義
//************************************************************
// カメラ全般
#define VIEW_NEAR		(10.0f)		// モデルが見えるZ軸の最小値
#define VIEW_FAR		(80000.0f)	// モデルが見えるZ軸の最大値

// メインカメラ
#define FIRST_ROT		(1.45f)		// 開始時の視点と注視点の間の向き (x)
#define FIRST_DIS		(-340.0f)	// 開始時の視点と注視点の間の距離

#define REV_ROT_FOLLOW	(0.05f)		// 追従時の回り込みの補正係数
#define REV_POS_V		(0.225f)	// 視点の位置の補正係数 (x, z)
#define REV_POS_R		(0.25f)		// 注視点の位置の補正係数 (x, z)
#define REV_POS_V_Y		(0.045f)	// 視点の位置の補正係数 (y)
#define REV_POS_R_Y		(0.05f)		// 注視点の位置の補正係数 (y)

#define MOVE_CAMERA		(4.0f)		// カメラの移動量
#define MOVE_ROT_X		(0.015f)	// 向きの変更量 (x)
#define MOVE_ROT_Y		(0.03f)		// 向きの変更量 (y)
#define MOVE_DIS		(2.0f)		// 距離の変更量
#define REV_DIS			(-1.0f)		// 距離の補正値

#define REV_DIS_MOUSE	(-0.15f)	// マウス操作でのカメラの距離の補正係数
#define REV_ROT_MOUSE	(0.008f)	// マウス操作でのカメラの回転量の補正係数

#define MUL_ANGLE_PLUS	(1.8f)		// ブースト時の視野角増加の乗算量

#define LIMIT_ROT_HIGH	(D3DX_PI - 0.1f)	// 回転量 (x) の回転制限値 (上)
#define LIMIT_ROT_LOW	(0.1f)				// 回転量 (x) の回転制限値 (下)

// 一人称視点カメラ
#define CAMERA_FORWARD_SHIFT	(18.5f)			// 前にカメラをずらす距離
#define CAMERA_UP_SHIFT			(100.0f)		// 上にカメラをずらす距離
#define CAMERA_RIGHT_SHIFT		(7.0f)			// 右にカメラをずらす距離
#define CAMERA_BACK_SHIFT		(35.0f)			// 後ろにカメラをずらす距離

// タイトルカメラ
#define TITLE_DISTANCE				(500.0f)			// ランキング時のカメラの距離
#define TITLE_POSV_Y				(200.0f)			// ランキング時のカメラの視点初期位置(Y軸)
#define TITLE_POSR_Y				(350.0f)			// ランキング時のカメラの注視点初期位置(Y軸)

#define TITLE_POS_X_ROAD_ONE		(2500.0f)			// 一つ目の道路の視点の位置
#define TITLE_POS_Z_ROAD_ONE		(-7000.0f)			// 一つ目の道路の視点の位置
#define TITLE_POS_X_ROAD_TWO		(11000.0f)			// 二つ目の道路の視点の位置
#define TITLE_POS_Z_ROAD_TWO		(2250.0f)			// 二つ目の道路の視点の位置
#define TITLE_POS_X_ROAD_THREE		(9000.0f)			// 三つ目の道路の視点の位置
#define TITLE_POS_Z_ROAD_THREE		(11000.0f)			// 三つ目の道路の視点の位置

#define TITLE_ROT_ROAD_ONE			(0)					// 一つ目の道路の角度
#define TITLE_ROT_ROAD_TWO			(-D3DX_PI * 0.5f)	// 二つ目の道路の角度
#define TITLE_ROT_ROAD_THREE		(-D3DX_PI)			// 三つ目の道路の角度

#define TITLE_POS_MOVE_ROAD			(10.0f)				// 道路の移動量
#define TITLE_POS_END_ROAD_ONE		(10000.0f)			// 一つ目の道路の終着点
#define TITLE_POS_END_ROAD_TWO		(-5000.0f)			// 二つ目の道路の終着点
#define TITLE_POS_END_ROAD_THREE	(-1000.0f)			// 三つ目の道路の終着点

// リザルトカメラ
#define CAMERA_RSL_ROT_MOVE		(0.003f)			// カメラの向きの移動量
#define RESULT_DISTANCE			(13000.0f)			// リザルト時のカメラの距離
#define RESULT_INIT_POS_Y		(4500.0f)			// リザルト時のカメラの初期位置(Y軸)
	
// ランキングカメラ
#define RANK_DISTANCE			(500.0f)			// ランキング時のカメラの距離
#define RANK_POSV_Y				(200.0f)			// ランキング時のカメラの視点初期位置(Y軸)
#define RANK_POSR_Y				(350.0f)			// ランキング時のカメラの注視点初期位置(Y軸)

#define RANK_POS_X_ROAD_ONE		(2500.0f)			// 一つ目の道路の視点の位置
#define RANK_POS_Z_ROAD_ONE		(-7000.0f)			// 一つ目の道路の視点の位置
#define RANK_POS_X_ROAD_TWO		(11000.0f)			// 二つ目の道路の視点の位置
#define RANK_POS_Z_ROAD_TWO		(2250.0f)			// 二つ目の道路の視点の位置
#define RANK_POS_X_ROAD_THREE	(9000.0f)			// 三つ目の道路の視点の位置
#define RANK_POS_Z_ROAD_THREE	(11000.0f)			// 三つ目の道路の視点の位置

#define RANK_ROT_ROAD_ONE		(0)					// 一つ目の道路の角度
#define RANK_ROT_ROAD_TWO		(-D3DX_PI * 0.5f)	// 二つ目の道路の角度
#define RANK_ROT_ROAD_THREE		(-D3DX_PI)			// 三つ目の道路の角度

#define RANK_POS_MOVE_ROAD		(10.0f)		// 道路の移動量
#define RANK_POS_END_ROAD_ONE	(10000.0f)	// 一つ目の道路の終着点
#define RANK_POS_END_ROAD_TWO	(-5000.0f)	// 二つ目の道路の終着点
#define RANK_POS_END_ROAD_THREE	(-1000.0f)	// 三つ目の道路の終着点

//************************************************************
//	列挙型
//************************************************************
//**********************************
//* 道路の列挙型
//**********************************
typedef enum
{
	ROAD_CAMERA_TYPE_ONE = 0,		// 一つ目の道路（真ん中）
	ROAD_CAMERA_TYPE_TWO,			// 二つ目の道路（横から）
	ROAD_CAMERA_TYPE_THREE,			// 三つ目の道路（商店街）
	ROAD_CAMERA_TYPE_MAX
}ROADTYPE;

//************************************************************
//	プロトタイプ宣言
//************************************************************
void InitMapCamera(void);			// マップカメラの初期化処理
void InitUiCamera(void);			// UIカメラの初期化処理

void UpdateResultCamera(void);		// リザルト時のカメラの更新処理
void UpdateTitleCamera(void);		// タイトル時のカメラの更新処理
void UpdateRankingCamera(void);		// ランキング時のカメラの更新処理

void MoveFollowCamera(void);		// メインカメラの位置の更新処理 (追従)
void MoveCamera(void);				// メインカメラの位置の更新処理 (操作)
void DisCamera(void);				// メインカメラの距離の更新処理 (操作)
void RotCamera(void);				// メインカメラの向きの更新処理 (操作)

void MoveMiniMap(void);				// マップカメラの位置の更新処理

void RevRotXCamera(void);			// カメラの向きの補正処理 (x)
void RevRotYCamera(void);			// カメラの向きの補正処理 (y)

//void MoveGoodjobCamera(void);		// ミッション成功時のカメラ更新処理(追従)

//************************************************************
//	グローバル変数
//************************************************************
Camera g_aCamera[CAMERATYPE_MAX];	// カメラの情報
CAMERASTATE g_CameraState;			// カメラの状態
ROADTYPE g_roadType;				// 取得する道路の種類

//============================================================
//	カメラの初期化処理
//============================================================
void InitCamera(void)
{
	MODE mode  = GetMode();			// モードの状態
	g_roadType = ROADTYPE(rand() % int(ROAD_CAMERA_TYPE_MAX));

	// カメラの状態を初期化
	g_CameraState = CAMERASTATE_NORMAL;

	if (mode == MODE_TUTORIAL || mode == MODE_GAME)
	{ // モードがチュートリアル、またはゲームの場合

		// 基本情報を初期化
		g_aCamera[CAMERATYPE_MAIN].posV     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 現在の視点
		g_aCamera[CAMERATYPE_MAIN].posVOld  = g_aCamera[CAMERATYPE_MAIN].posV;		// 前回の視点
		g_aCamera[CAMERATYPE_MAIN].posR     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 現在の注視点
		g_aCamera[CAMERATYPE_MAIN].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目標の視点
		g_aCamera[CAMERATYPE_MAIN].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目標の注視点
		g_aCamera[CAMERATYPE_MAIN].vecU     = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 上方向ベクトル
		g_aCamera[CAMERATYPE_MAIN].rot      = D3DXVECTOR3(FIRST_ROT, 0.0f, 0.0f);	// 向き
		g_aCamera[CAMERATYPE_MAIN].fDis     = FIRST_DIS;							// 視点と注視点の距離

		// ビューポート情報を初期化
		g_aCamera[CAMERATYPE_MAIN].viewport.X      = 0;				// 左上隅のピクセル座標 (x)
		g_aCamera[CAMERATYPE_MAIN].viewport.Y      = 0;				// 左上隅のピクセル座標 (y)
		g_aCamera[CAMERATYPE_MAIN].viewport.Width  = SCREEN_WIDTH;	// 描画する画面の横幅
		g_aCamera[CAMERATYPE_MAIN].viewport.Height = SCREEN_HEIGHT;	// 描画する画面の縦幅
		g_aCamera[CAMERATYPE_MAIN].viewport.MinZ   = 0.0f;
		g_aCamera[CAMERATYPE_MAIN].viewport.MaxZ   = 1.0f;

		// マップカメラの初期化
		InitMapCamera();

		// UIカメラの初期化
		InitUiCamera();
	}
	else if (mode == MODE_RESULT)
	{ // モードがリザルトの場合

		// メインカメラの初期化
		g_aCamera[CAMERATYPE_MAIN].fDis     = RESULT_DISTANCE;															// 視点と注視点の距離
		g_aCamera[CAMERATYPE_MAIN].posV     = D3DXVECTOR3(0.0f, RESULT_INIT_POS_Y, -g_aCamera[CAMERATYPE_MAIN].fDis);	// 視点の位置

		g_aCamera[CAMERATYPE_MAIN].posVOld  = g_aCamera[CAMERATYPE_MAIN].posV;		// 前回の視点
		g_aCamera[CAMERATYPE_MAIN].posR     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 注視点の位置
		g_aCamera[CAMERATYPE_MAIN].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目標の視点
		g_aCamera[CAMERATYPE_MAIN].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目標の注視点
		g_aCamera[CAMERATYPE_MAIN].vecU     = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 上方向ベクトル
		g_aCamera[CAMERATYPE_MAIN].rot      = D3DXVECTOR3(FIRST_ROT, 0.0f, 0.0f);	// 向き

		// ビューポート情報を初期化
		g_aCamera[CAMERATYPE_MAIN].viewport.X      = 0;				// 左上隅のピクセル座標 (x)
		g_aCamera[CAMERATYPE_MAIN].viewport.Y      = 0;				// 左上隅のピクセル座標 (y)
		g_aCamera[CAMERATYPE_MAIN].viewport.Width  = SCREEN_WIDTH;	// 描画する画面の横幅
		g_aCamera[CAMERATYPE_MAIN].viewport.Height = SCREEN_HEIGHT;	// 描画する画面の縦幅
		g_aCamera[CAMERATYPE_MAIN].viewport.MinZ   = 0.0f;
		g_aCamera[CAMERATYPE_MAIN].viewport.MaxZ   = 1.0f;

		// マップカメラの初期化
		InitMapCamera();

		// UIカメラの初期化
		InitUiCamera();
	}
	else if (mode == MODE_TITLE)
	{ // モードがリザルトの場合

	  // メインカメラの初期化
		switch (g_roadType)
		{
		case ROAD_CAMERA_TYPE_ONE:		// 一つ目の道路

			g_aCamera[CAMERATYPE_MAIN].fDis = RANK_DISTANCE;																		// 視点と注視点の距離
			g_aCamera[CAMERATYPE_MAIN].posV = D3DXVECTOR3(RANK_POS_X_ROAD_ONE, RANK_POSV_Y, -g_aCamera[CAMERATYPE_MAIN].fDis);		// 視点の位置
			g_aCamera[CAMERATYPE_MAIN].posVOld = g_aCamera[CAMERATYPE_MAIN].posV;														// 前回の視点
			g_aCamera[CAMERATYPE_MAIN].posR = D3DXVECTOR3(RANK_POS_X_ROAD_ONE, RANK_POSR_Y, RANK_POS_Z_ROAD_ONE);					// 注視点の位置
			g_aCamera[CAMERATYPE_MAIN].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// 目標の視点
			g_aCamera[CAMERATYPE_MAIN].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// 目標の注視点
			g_aCamera[CAMERATYPE_MAIN].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);														// 上方向ベクトル
			g_aCamera[CAMERATYPE_MAIN].rot = D3DXVECTOR3(0.0f, RANK_ROT_ROAD_ONE, 0.0f);											// 向き

			break;

		case ROAD_CAMERA_TYPE_TWO:		// 二つ目の道路

			g_aCamera[CAMERATYPE_MAIN].fDis = RANK_DISTANCE;																		// 視点と注視点の距離
			g_aCamera[CAMERATYPE_MAIN].posV = D3DXVECTOR3(RANK_POS_X_ROAD_TWO, RANK_POSV_Y, -g_aCamera[CAMERATYPE_MAIN].fDis);		// 視点の位置
			g_aCamera[CAMERATYPE_MAIN].posVOld = g_aCamera[CAMERATYPE_MAIN].posV;														// 前回の視点
			g_aCamera[CAMERATYPE_MAIN].posR = D3DXVECTOR3(RANK_POS_X_ROAD_TWO, RANK_POSR_Y, RANK_POS_Z_ROAD_TWO);					// 注視点の位置
			g_aCamera[CAMERATYPE_MAIN].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// 目標の視点
			g_aCamera[CAMERATYPE_MAIN].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// 目標の注視点
			g_aCamera[CAMERATYPE_MAIN].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);														// 上方向ベクトル
			g_aCamera[CAMERATYPE_MAIN].rot = D3DXVECTOR3(0.0f, RANK_ROT_ROAD_TWO, 0.0f);											// 向き

			break;

		case ROAD_CAMERA_TYPE_THREE:	// 三つ目の道路

			g_aCamera[CAMERATYPE_MAIN].fDis = RANK_DISTANCE;																		// 視点と注視点の距離
			g_aCamera[CAMERATYPE_MAIN].posV = D3DXVECTOR3(RANK_POS_X_ROAD_THREE, RANK_POSV_Y, -g_aCamera[CAMERATYPE_MAIN].fDis);	// 視点の位置
			g_aCamera[CAMERATYPE_MAIN].posVOld = g_aCamera[CAMERATYPE_MAIN].posV;														// 前回の視点
			g_aCamera[CAMERATYPE_MAIN].posR = D3DXVECTOR3(RANK_POS_X_ROAD_THREE, RANK_POSR_Y, RANK_POS_Z_ROAD_THREE);				// 注視点の位置
			g_aCamera[CAMERATYPE_MAIN].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// 目標の視点
			g_aCamera[CAMERATYPE_MAIN].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// 目標の注視点
			g_aCamera[CAMERATYPE_MAIN].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);														// 上方向ベクトル
			g_aCamera[CAMERATYPE_MAIN].rot = D3DXVECTOR3(0.0f, RANK_ROT_ROAD_THREE, 0.0f);											// 向き

			break;
		}

		// ビューポート情報を初期化
		g_aCamera[CAMERATYPE_MAIN].viewport.X = 0;				// 左上隅のピクセル座標 (x)
		g_aCamera[CAMERATYPE_MAIN].viewport.Y = 0;				// 左上隅のピクセル座標 (y)
		g_aCamera[CAMERATYPE_MAIN].viewport.Width = SCREEN_WIDTH;	// 描画する画面の横幅
		g_aCamera[CAMERATYPE_MAIN].viewport.Height = SCREEN_HEIGHT;	// 描画する画面の縦幅
		g_aCamera[CAMERATYPE_MAIN].viewport.MinZ = 0.0f;
		g_aCamera[CAMERATYPE_MAIN].viewport.MaxZ = 1.0f;

		// マップカメラの初期化
		InitMapCamera();

		// UIカメラの初期化
		InitUiCamera();
	}
	else if (mode == MODE_RANKING)
	{ // モードがリザルトの場合

		// メインカメラの初期化
		switch (g_roadType)
		{
		case ROAD_CAMERA_TYPE_ONE:		// 一つ目の道路

			g_aCamera[CAMERATYPE_MAIN].fDis     = RANK_DISTANCE;																		// 視点と注視点の距離
			g_aCamera[CAMERATYPE_MAIN].posV     = D3DXVECTOR3(RANK_POS_X_ROAD_ONE, RANK_POSV_Y, -g_aCamera[CAMERATYPE_MAIN].fDis);		// 視点の位置
			g_aCamera[CAMERATYPE_MAIN].posVOld  = g_aCamera[CAMERATYPE_MAIN].posV;														// 前回の視点
			g_aCamera[CAMERATYPE_MAIN].posR     = D3DXVECTOR3(RANK_POS_X_ROAD_ONE, RANK_POSR_Y, RANK_POS_Z_ROAD_ONE);					// 注視点の位置
			g_aCamera[CAMERATYPE_MAIN].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// 目標の視点
			g_aCamera[CAMERATYPE_MAIN].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// 目標の注視点
			g_aCamera[CAMERATYPE_MAIN].vecU     = D3DXVECTOR3(0.0f, 1.0f, 0.0f);														// 上方向ベクトル
			g_aCamera[CAMERATYPE_MAIN].rot      = D3DXVECTOR3(0.0f, RANK_ROT_ROAD_ONE, 0.0f);											// 向き

			break;

		case ROAD_CAMERA_TYPE_TWO:		// 二つ目の道路

			g_aCamera[CAMERATYPE_MAIN].fDis     = RANK_DISTANCE;																		// 視点と注視点の距離
			g_aCamera[CAMERATYPE_MAIN].posV     = D3DXVECTOR3(RANK_POS_X_ROAD_TWO, RANK_POSV_Y, -g_aCamera[CAMERATYPE_MAIN].fDis);		// 視点の位置
			g_aCamera[CAMERATYPE_MAIN].posVOld  = g_aCamera[CAMERATYPE_MAIN].posV;														// 前回の視点
			g_aCamera[CAMERATYPE_MAIN].posR     = D3DXVECTOR3(RANK_POS_X_ROAD_TWO, RANK_POSR_Y, RANK_POS_Z_ROAD_TWO);					// 注視点の位置
			g_aCamera[CAMERATYPE_MAIN].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// 目標の視点
			g_aCamera[CAMERATYPE_MAIN].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// 目標の注視点
			g_aCamera[CAMERATYPE_MAIN].vecU     = D3DXVECTOR3(0.0f, 1.0f, 0.0f);														// 上方向ベクトル
			g_aCamera[CAMERATYPE_MAIN].rot      = D3DXVECTOR3(0.0f, RANK_ROT_ROAD_TWO, 0.0f);											// 向き

			break;

		case ROAD_CAMERA_TYPE_THREE:	// 三つ目の道路

			g_aCamera[CAMERATYPE_MAIN].fDis     = RANK_DISTANCE;																		// 視点と注視点の距離
			g_aCamera[CAMERATYPE_MAIN].posV     = D3DXVECTOR3(RANK_POS_X_ROAD_THREE, RANK_POSV_Y, -g_aCamera[CAMERATYPE_MAIN].fDis);	// 視点の位置
			g_aCamera[CAMERATYPE_MAIN].posVOld  = g_aCamera[CAMERATYPE_MAIN].posV;														// 前回の視点
			g_aCamera[CAMERATYPE_MAIN].posR     = D3DXVECTOR3(RANK_POS_X_ROAD_THREE, RANK_POSR_Y, RANK_POS_Z_ROAD_THREE);				// 注視点の位置
			g_aCamera[CAMERATYPE_MAIN].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// 目標の視点
			g_aCamera[CAMERATYPE_MAIN].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// 目標の注視点
			g_aCamera[CAMERATYPE_MAIN].vecU     = D3DXVECTOR3(0.0f, 1.0f, 0.0f);														// 上方向ベクトル
			g_aCamera[CAMERATYPE_MAIN].rot      = D3DXVECTOR3(0.0f, RANK_ROT_ROAD_THREE, 0.0f);											// 向き

			break;
		}

		// ビューポート情報を初期化
		g_aCamera[CAMERATYPE_MAIN].viewport.X      = 0;				// 左上隅のピクセル座標 (x)
		g_aCamera[CAMERATYPE_MAIN].viewport.Y      = 0;				// 左上隅のピクセル座標 (y)
		g_aCamera[CAMERATYPE_MAIN].viewport.Width  = SCREEN_WIDTH;	// 描画する画面の横幅
		g_aCamera[CAMERATYPE_MAIN].viewport.Height = SCREEN_HEIGHT;	// 描画する画面の縦幅
		g_aCamera[CAMERATYPE_MAIN].viewport.MinZ   = 0.0f;
		g_aCamera[CAMERATYPE_MAIN].viewport.MaxZ   = 1.0f;

		// マップカメラの初期化
		InitMapCamera();

		// UIカメラの初期化
		InitUiCamera();
	}
}

//============================================================
//	カメラの終了処理
//============================================================
void UninitCamera(void)
{

}

//============================================================
//	カメラの更新処理
//============================================================
void UpdateCamera(void)
{
	MODE mode = GetMode();			// モードの状態

	switch (mode)
	{ // モードごとの処理
	case MODE_TUTORIAL:	// チュートリアル

		// カメラの位置の更新 (追従)
		MoveFollowCamera();

		// 処理を抜ける
		break;

	case MODE_GAME:		// ゲーム

#if 1
		switch (GetGameMode())
		{ // ゲームモードごとの処理
		case GAMEMODE_PLAY:

	#if 0
			switch (g_CameraState)
			{
			case CAMERASTATE_NORMAL:		// 通常カメラ

				// カメラの位置の更新 (追従)
				MoveFollowCamera();

				break;						// 抜け出す

			case CAMERASTATE_GOODJOB:		// ミッション成功

				// ミッション成功時のカメラ更新処理(追従)
				MoveGoodjobCamera();

				break;						// 抜け出す
			}
	#else
			// カメラの位置の更新 (追従)
			MoveFollowCamera();
	#endif
			// 処理を抜ける
			break;

		case GAMEMODE_EDIT:

			// カメラの位置の更新 (操作)
			MoveCamera();

			// カメラの距離の更新 (操作)
			DisCamera();

			// カメラの向きの更新 (操作)
			RotCamera();

			// 処理を抜ける
			break;
		}
#else
		// カメラの位置の更新 (操作)
		MoveCamera();

		// カメラの距離の更新 (操作)
		DisCamera();

		// カメラの向きの更新 (操作)
		RotCamera();
#endif
		// マップカメラの位置の更新
		MoveMiniMap();

		// 処理を抜ける
		break;

	case MODE_TITLE:	// タイトル
		
		//タイトル時のカメラの更新
		UpdateTitleCamera();
		
		//処理を抜ける
		break;

	case MODE_RESULT:	// リザルト

		// リザルト時のカメラの更新
		UpdateResultCamera();

		// 処理を抜ける
		break;

	case MODE_RANKING:	// ランキング

		// ランキング時のカメラの更新
		UpdateRankingCamera();

		// 処理を抜ける
		break;
	}
}

//============================================================
//	カメラの設定処理
//============================================================
void SetCamera(int nID)
{
	// 変数を宣言
	float viewAngle;	// 視野角の設定用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// ビューポートの設定
	pDevice->SetViewport(&g_aCamera[nID].viewport);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_aCamera[nID].mtxProjection);

	if (GetMode() == MODE_TUTORIAL || GetMode() == MODE_GAME)
	{ // プレイモードの場合

		if (nID == CAMERATYPE_MAIN && GetPlayer()->bCameraFirst == false)
		{ // メインカメラ且つ、一人称ではない場合

			// ブースト速度に応じて視野角を変動
			viewAngle = VIEW_ANGLE + D3DXToRadian(GetPlayer()->boost.plusMove.x * MUL_ANGLE_PLUS);
		}
		else
		{ // 非メインカメラ、もしくは一人称の場合

			// 通常の視野角を設定
			viewAngle = VIEW_ANGLE;
		}
	}
	else
	{ // 非プレイモードの場合

		// 通常の視野角を設定
		viewAngle = VIEW_ANGLE;
	}

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH
	( // 引数
		&g_aCamera[nID].mtxProjection,													// プロジェクションマトリックス
		viewAngle,																		// 視野角
		(float)g_aCamera[nID].viewport.Width / (float)g_aCamera[nID].viewport.Height,	// 画面のアスペクト比
		VIEW_NEAR,																		// Z軸の最小値
		VIEW_FAR																		// Z軸の最大値
	);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_aCamera[nID].mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_aCamera[nID].mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH
	( // 引数
		&g_aCamera[nID].mtxView,	// ビューマトリックス
		&g_aCamera[nID].posV,		// 視点
		&g_aCamera[nID].posR,		// 注視点
		&g_aCamera[nID].vecU		// 上方向ベクトル
	);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_aCamera[nID].mtxView);
}

//======================================================================================================================
//	マップカメラの初期化処理
//======================================================================================================================
void InitMapCamera(void)
{
	// 基本情報を初期化
	g_aCamera[CAMERATYPE_MAP].posV     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 現在の視点
	g_aCamera[CAMERATYPE_MAP].posVOld  = g_aCamera[CAMERATYPE_MAP].posV;		// 前回の視点
	g_aCamera[CAMERATYPE_MAP].posR     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 現在の注視点
	g_aCamera[CAMERATYPE_MAP].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 目標の視点
	g_aCamera[CAMERATYPE_MAP].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 目標の注視点
	g_aCamera[CAMERATYPE_MAP].vecU     = D3DXVECTOR3(0.0f, 0.0f, 1.0f);			// 上方向ベクトル
	g_aCamera[CAMERATYPE_MAP].rot      = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
	g_aCamera[CAMERATYPE_MAP].fDis     = 0.0f;									// 視点と注視点の距離

	// ビューポート情報を初期化
	g_aCamera[CAMERATYPE_MAP].viewport.X      = MAPCAM_X;		// 左上隅のピクセル座標 (x)
	g_aCamera[CAMERATYPE_MAP].viewport.Y      = MAPCAM_Y;		// 左上隅のピクセル座標 (y)
	g_aCamera[CAMERATYPE_MAP].viewport.Width  = MAPCAM_SIZE_X;	// 描画する画面の横幅
	g_aCamera[CAMERATYPE_MAP].viewport.Height = MAPCAM_SIZE_Y;	// 描画する画面の縦幅
	g_aCamera[CAMERATYPE_MAP].viewport.MinZ   = 0.0f;
	g_aCamera[CAMERATYPE_MAP].viewport.MaxZ   = 1.0f;
}

//======================================================================================================================
//	UIカメラの初期化処理
//======================================================================================================================
void InitUiCamera(void)
{
	// 基本情報を初期化
	g_aCamera[CAMERATYPE_UI].posV     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 現在の視点
	g_aCamera[CAMERATYPE_UI].posVOld  = g_aCamera[CAMERATYPE_UI].posV;		// 前回の視点
	g_aCamera[CAMERATYPE_UI].posR     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 現在の注視点
	g_aCamera[CAMERATYPE_UI].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目標の視点
	g_aCamera[CAMERATYPE_UI].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目標の注視点
	g_aCamera[CAMERATYPE_UI].vecU     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 上方向ベクトル
	g_aCamera[CAMERATYPE_UI].rot      = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	g_aCamera[CAMERATYPE_UI].fDis     = 0.0f;								// 視点と注視点の距離

	// ビューポート情報を初期化
	g_aCamera[CAMERATYPE_UI].viewport.X      = 0;				// 左上隅のピクセル座標 (x)
	g_aCamera[CAMERATYPE_UI].viewport.Y      = 0;				// 左上隅のピクセル座標 (y)
	g_aCamera[CAMERATYPE_UI].viewport.Width  = SCREEN_WIDTH;	// 描画する画面の横幅
	g_aCamera[CAMERATYPE_UI].viewport.Height = SCREEN_HEIGHT;	// 描画する画面の縦幅
	g_aCamera[CAMERATYPE_UI].viewport.MinZ   = 0.0f;
	g_aCamera[CAMERATYPE_UI].viewport.MaxZ   = 1.0f;
}

//======================================================================================================================
//	タイトル時のカメラの更新処理
//======================================================================================================================
void UpdateTitleCamera(void)
{
	//道路ごとの移動量を設定
	switch (g_roadType)
	{
	case ROAD_CAMERA_TYPE_ONE:	//一つ目の道路

		g_aCamera[CAMERATYPE_MAIN].posV.z += TITLE_POS_MOVE_ROAD;
		g_aCamera[CAMERATYPE_MAIN].posR.z += TITLE_POS_MOVE_ROAD;

		break;

	case ROAD_CAMERA_TYPE_TWO:	//二つ目の道路

		g_aCamera[CAMERATYPE_MAIN].posV.x -= TITLE_POS_MOVE_ROAD;
		g_aCamera[CAMERATYPE_MAIN].posR.x -= TITLE_POS_MOVE_ROAD;

		break;

	case ROAD_CAMERA_TYPE_THREE://三つ目の道路

		g_aCamera[CAMERATYPE_MAIN].posV.z -= TITLE_POS_MOVE_ROAD;
		g_aCamera[CAMERATYPE_MAIN].posR.z -= TITLE_POS_MOVE_ROAD;

		break;
	}

	// 前回の位置を記録する
	g_aCamera[CAMERATYPE_MAIN].posVOld = g_aCamera[CAMERATYPE_MAIN].posV;

	if (g_aCamera[CAMERATYPE_MAIN].rot.y > D3DX_PI)
	{//角度が3.14fより大きかった場合

	 //角度から1周分減らす
		g_aCamera[CAMERATYPE_MAIN].rot.y -= (2 * D3DX_PI);
	}
	else if (g_aCamera[CAMERATYPE_MAIN].rot.y < -D3DX_PI)
	{//角度が-3.14fより小さかった場合

	 //角度に1周分加える
		g_aCamera[CAMERATYPE_MAIN].rot.y += (2 * D3DX_PI);
	}

	//カメラの視点を更新する
	g_aCamera[CAMERATYPE_MAIN].posV.x = g_aCamera[CAMERATYPE_MAIN].posR.x + sinf(g_aCamera[CAMERATYPE_MAIN].rot.y) * -g_aCamera[CAMERATYPE_MAIN].fDis;
	g_aCamera[CAMERATYPE_MAIN].posV.z = g_aCamera[CAMERATYPE_MAIN].posR.z + cosf(g_aCamera[CAMERATYPE_MAIN].rot.y) * -g_aCamera[CAMERATYPE_MAIN].fDis;

	//カメラの注視点を更新する
	g_aCamera[CAMERATYPE_MAIN].posR.x = g_aCamera[CAMERATYPE_MAIN].posV.x + sinf(g_aCamera[CAMERATYPE_MAIN].rot.y) * g_aCamera[CAMERATYPE_MAIN].fDis;
	g_aCamera[CAMERATYPE_MAIN].posR.z = g_aCamera[CAMERATYPE_MAIN].posV.z + cosf(g_aCamera[CAMERATYPE_MAIN].rot.y) * g_aCamera[CAMERATYPE_MAIN].fDis;
}

//======================================================================================================================
//	リザルト時のカメラの更新処理
//======================================================================================================================
void UpdateResultCamera(void)
{
	//カメラの注始点の位置を設定する
	g_aCamera[CAMERATYPE_MAIN].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 前回の位置を記録する
	g_aCamera[CAMERATYPE_MAIN].posVOld = g_aCamera[CAMERATYPE_MAIN].posV;

	//視点を右に回す
	g_aCamera[CAMERATYPE_MAIN].rot.y += CAMERA_RSL_ROT_MOVE;

	if (g_aCamera[CAMERATYPE_MAIN].rot.y > D3DX_PI)
	{//角度が3.14fより大きかった場合

		//角度から1周分減らす
		g_aCamera[CAMERATYPE_MAIN].rot.y -= (2 * D3DX_PI);
	}
	else if (g_aCamera[CAMERATYPE_MAIN].rot.y < -D3DX_PI)
	{//角度が-3.14fより小さかった場合

		//角度に1周分加える
		g_aCamera[CAMERATYPE_MAIN].rot.y += (2 * D3DX_PI);
	}

	//カメラの視点を更新する
	g_aCamera[CAMERATYPE_MAIN].posV.x = g_aCamera[CAMERATYPE_MAIN].posR.x + sinf(g_aCamera[CAMERATYPE_MAIN].rot.y) * -g_aCamera[CAMERATYPE_MAIN].fDis;
	g_aCamera[CAMERATYPE_MAIN].posV.z = g_aCamera[CAMERATYPE_MAIN].posR.z + cosf(g_aCamera[CAMERATYPE_MAIN].rot.y) * -g_aCamera[CAMERATYPE_MAIN].fDis;

	//カメラの注視点を更新する
	g_aCamera[CAMERATYPE_MAIN].posR.x = g_aCamera[CAMERATYPE_MAIN].posV.x + sinf(g_aCamera[CAMERATYPE_MAIN].rot.y) * g_aCamera[CAMERATYPE_MAIN].fDis;
	g_aCamera[CAMERATYPE_MAIN].posR.z = g_aCamera[CAMERATYPE_MAIN].posV.z + cosf(g_aCamera[CAMERATYPE_MAIN].rot.y) * g_aCamera[CAMERATYPE_MAIN].fDis;
}

//======================================================================================================================
//	ランキング時のカメラの更新処理
//======================================================================================================================
void UpdateRankingCamera(void)
{
	//道路ごとの移動量を設定
	switch (g_roadType)
	{
	case ROAD_CAMERA_TYPE_ONE:	//一つ目の道路

		g_aCamera[CAMERATYPE_MAIN].posV.z += RANK_POS_MOVE_ROAD;
		g_aCamera[CAMERATYPE_MAIN].posR.z += RANK_POS_MOVE_ROAD;

		if (g_aCamera[CAMERATYPE_MAIN].posV.z >= RANK_POS_END_ROAD_ONE)
		{
			//画面を遷移させる
			SetRankingRoadTrance();
		}

		break;

	case ROAD_CAMERA_TYPE_TWO:	//二つ目の道路

		g_aCamera[CAMERATYPE_MAIN].posV.x -= RANK_POS_MOVE_ROAD;
		g_aCamera[CAMERATYPE_MAIN].posR.x -= RANK_POS_MOVE_ROAD;

		//道路の終着点を判定
		if (g_aCamera[CAMERATYPE_MAIN].posV.x <= RANK_POS_END_ROAD_TWO)
		{
			//画面を遷移させる
			SetRankingRoadTrance();
		}

		break;

	case ROAD_CAMERA_TYPE_THREE://三つ目の道路

		g_aCamera[CAMERATYPE_MAIN].posV.z -= RANK_POS_MOVE_ROAD;
		g_aCamera[CAMERATYPE_MAIN].posR.z -= RANK_POS_MOVE_ROAD;

		//道路の終着点を判定
		if (g_aCamera[CAMERATYPE_MAIN].posV.z <= RANK_POS_END_ROAD_THREE)
		{
			//画面を遷移させる
			SetRankingRoadTrance();
		}

		break;
	}

	// 前回の位置を記録する
	g_aCamera[CAMERATYPE_MAIN].posVOld = g_aCamera[CAMERATYPE_MAIN].posV;

	if (g_aCamera[CAMERATYPE_MAIN].rot.y > D3DX_PI)
	{//角度が3.14fより大きかった場合

		//角度から1周分減らす
		g_aCamera[CAMERATYPE_MAIN].rot.y -= (2 * D3DX_PI);
	}
	else if (g_aCamera[CAMERATYPE_MAIN].rot.y < -D3DX_PI)
	{//角度が-3.14fより小さかった場合

		//角度に1周分加える
		g_aCamera[CAMERATYPE_MAIN].rot.y += (2 * D3DX_PI);
	}

	//カメラの視点を更新する
	g_aCamera[CAMERATYPE_MAIN].posV.x = g_aCamera[CAMERATYPE_MAIN].posR.x + sinf(g_aCamera[CAMERATYPE_MAIN].rot.y) * -g_aCamera[CAMERATYPE_MAIN].fDis;
	g_aCamera[CAMERATYPE_MAIN].posV.z = g_aCamera[CAMERATYPE_MAIN].posR.z + cosf(g_aCamera[CAMERATYPE_MAIN].rot.y) * -g_aCamera[CAMERATYPE_MAIN].fDis;

	//カメラの注視点を更新する
	g_aCamera[CAMERATYPE_MAIN].posR.x = g_aCamera[CAMERATYPE_MAIN].posV.x + sinf(g_aCamera[CAMERATYPE_MAIN].rot.y) * g_aCamera[CAMERATYPE_MAIN].fDis;
	g_aCamera[CAMERATYPE_MAIN].posR.z = g_aCamera[CAMERATYPE_MAIN].posV.z + cosf(g_aCamera[CAMERATYPE_MAIN].rot.y) * g_aCamera[CAMERATYPE_MAIN].fDis;
}

//======================================================================================================================
//	メインカメラの位置の更新処理 (追従)
//======================================================================================================================
void MoveFollowCamera(void)
{
	// 変数を宣言
	D3DXVECTOR3 diffPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// カメラの視点の位置の計算代入用
	D3DXVECTOR3 diffPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// カメラの注視点の位置の計算代入用

	// ポインタを宣言
	Player *pPlayer = GetPlayer();		// プレイヤーの情報

	switch (pPlayer->nCameraState)
	{
	case PLAYCAMESTATE_NORMAL:			// 通常のカメラ状態

		if (pPlayer->bCameraFirst == true)
		{ // 一人称がONの場合

			// 目標の視点の位置を更新
			g_aCamera[CAMERATYPE_MAIN].posV.x = pPlayer->pos.x + sinf(pPlayer->rot.y) * CAMERA_FORWARD_SHIFT;	// 目標注視点から距離分離れた位置
			g_aCamera[CAMERATYPE_MAIN].posV.y = pPlayer->pos.y + CAMERA_UP_SHIFT;								// 固定の高さ
			g_aCamera[CAMERATYPE_MAIN].posV.z = pPlayer->pos.z + cosf(pPlayer->rot.y) * CAMERA_FORWARD_SHIFT;	// 目標注視点から距離分離れた位置

			// 目標の注始点を更新する
			g_aCamera[CAMERATYPE_MAIN].posR.x = g_aCamera[CAMERATYPE_MAIN].posV.x - ((g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * sinf(g_aCamera[CAMERATYPE_MAIN].rot.y));	// 目標注視点から距離分離れた位置
			g_aCamera[CAMERATYPE_MAIN].posR.y = g_aCamera[CAMERATYPE_MAIN].posV.y;																															// 視点と同じ高さ
			g_aCamera[CAMERATYPE_MAIN].posR.z = g_aCamera[CAMERATYPE_MAIN].posV.z - ((g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * cosf(g_aCamera[CAMERATYPE_MAIN].rot.y));	// 目標注視点から距離分離れた位置
		}
		else
		{ // 一人称がOFFの場合

			// 目標の注視点の位置を更新
			g_aCamera[CAMERATYPE_MAIN].destPosR.x = pPlayer->pos.x + sinf(pPlayer->rot.y + D3DX_PI) * POS_R_PLUS;	// プレイヤーの位置より少し前
			g_aCamera[CAMERATYPE_MAIN].destPosR.y = pPlayer->pos.y + POS_R_PLUS_Y;									// プレイヤーの位置と同じ
			g_aCamera[CAMERATYPE_MAIN].destPosR.z = pPlayer->pos.z + cosf(pPlayer->rot.y + D3DX_PI) * POS_R_PLUS;	// プレイヤーの位置より少し前

			// 目標の視点の位置を更新
			g_aCamera[CAMERATYPE_MAIN].destPosV.x = g_aCamera[CAMERATYPE_MAIN].destPosR.x + ((g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * sinf(g_aCamera[CAMERATYPE_MAIN].rot.y));	// 目標注視点から距離分離れた位置
			g_aCamera[CAMERATYPE_MAIN].destPosV.y = POS_V_Y;																																						// 固定の高さ
			g_aCamera[CAMERATYPE_MAIN].destPosV.z = g_aCamera[CAMERATYPE_MAIN].destPosR.z + ((g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * cosf(g_aCamera[CAMERATYPE_MAIN].rot.y));	// 目標注視点から距離分離れた位置

																																																					// 目標の位置までの差分を計算
			diffPosV = g_aCamera[CAMERATYPE_MAIN].destPosV - g_aCamera[CAMERATYPE_MAIN].posV;	// 視点
			diffPosR = g_aCamera[CAMERATYPE_MAIN].destPosR - g_aCamera[CAMERATYPE_MAIN].posR;	// 注視点

			// 視点の位置を更新
			g_aCamera[CAMERATYPE_MAIN].posV.x += diffPosV.x * REV_POS_V;
			g_aCamera[CAMERATYPE_MAIN].posV.y += diffPosV.y * REV_POS_V_Y;
			g_aCamera[CAMERATYPE_MAIN].posV.z += diffPosV.z * REV_POS_V;

			// 注視点の位置を更新
			g_aCamera[CAMERATYPE_MAIN].posR.x += diffPosR.x * REV_POS_R;
			g_aCamera[CAMERATYPE_MAIN].posR.y += diffPosR.y * REV_POS_R_Y;
			g_aCamera[CAMERATYPE_MAIN].posR.z += diffPosR.z * REV_POS_R;
		}

		break;							// 抜け出す

	case PLAYCAMESTATE_BACK:			// 後ろを見るカメラ状態

		if (pPlayer->bCameraFirst == true)
		{ // 一人称がONの場合

			// 目標の視点の位置を更新
			g_aCamera[CAMERATYPE_MAIN].posV.x = pPlayer->pos.x - sinf(pPlayer->rot.y) * CAMERA_BACK_SHIFT;	// 目標注視点から距離分離れた位置
			g_aCamera[CAMERATYPE_MAIN].posV.y = pPlayer->pos.y + CAMERA_UP_SHIFT;							// 固定の高さ
			g_aCamera[CAMERATYPE_MAIN].posV.z = pPlayer->pos.z - cosf(pPlayer->rot.y) * CAMERA_BACK_SHIFT;	// 目標注視点から距離分離れた位置

			// 目標の注始点を更新する
			g_aCamera[CAMERATYPE_MAIN].posR.x = g_aCamera[CAMERATYPE_MAIN].posV.x + ((g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * sinf(g_aCamera[CAMERATYPE_MAIN].rot.y));	// 目標注視点から距離分離れた位置
			g_aCamera[CAMERATYPE_MAIN].posR.y = g_aCamera[CAMERATYPE_MAIN].posV.y;																															// 視点と同じ高さ
			g_aCamera[CAMERATYPE_MAIN].posR.z = g_aCamera[CAMERATYPE_MAIN].posV.z + ((g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * cosf(g_aCamera[CAMERATYPE_MAIN].rot.y));	// 目標注視点から距離分離れた位置
		}
		else
		{ // 一人称がOFFの場合

			// 目標の注視点の位置を更新
			g_aCamera[CAMERATYPE_MAIN].destPosR.x = pPlayer->pos.x + sinf(pPlayer->rot.y + D3DX_PI) * -POS_R_PLUS;	// プレイヤーの位置より少し前
			g_aCamera[CAMERATYPE_MAIN].destPosR.y = pPlayer->pos.y + POS_R_PLUS_Y;									// プレイヤーの位置と同じ
			g_aCamera[CAMERATYPE_MAIN].destPosR.z = pPlayer->pos.z + cosf(pPlayer->rot.y + D3DX_PI) * -POS_R_PLUS;	// プレイヤーの位置より少し前

			// 目標の視点の位置を更新
			g_aCamera[CAMERATYPE_MAIN].destPosV.x = g_aCamera[CAMERATYPE_MAIN].destPosR.x - ((g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * sinf(g_aCamera[CAMERATYPE_MAIN].rot.y));	// 目標注視点から距離分離れた位置
			g_aCamera[CAMERATYPE_MAIN].destPosV.y = POS_V_Y;																																						// 固定の高さ
			g_aCamera[CAMERATYPE_MAIN].destPosV.z = g_aCamera[CAMERATYPE_MAIN].destPosR.z - ((g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * cosf(g_aCamera[CAMERATYPE_MAIN].rot.y));	// 目標注視点から距離分離れた位置

																																																					// 目標の位置までの差分を計算
			diffPosV = g_aCamera[CAMERATYPE_MAIN].destPosV - g_aCamera[CAMERATYPE_MAIN].posV;	// 視点
			diffPosR = g_aCamera[CAMERATYPE_MAIN].destPosR - g_aCamera[CAMERATYPE_MAIN].posR;	// 注視点

			// 視点の位置を更新
			g_aCamera[CAMERATYPE_MAIN].posV.x += diffPosV.x * REV_POS_V;
			g_aCamera[CAMERATYPE_MAIN].posV.y += diffPosV.y * REV_POS_V_Y;
			g_aCamera[CAMERATYPE_MAIN].posV.z += diffPosV.z * REV_POS_V;

			// 注視点の位置を更新
			g_aCamera[CAMERATYPE_MAIN].posR.x += diffPosR.x * REV_POS_R;
			g_aCamera[CAMERATYPE_MAIN].posR.y += diffPosR.y * REV_POS_R_Y;
			g_aCamera[CAMERATYPE_MAIN].posR.z += diffPosR.z * REV_POS_R;
		}

		break;							//抜け出す
	}

	// カメラの向きをプレイヤーの向きに合わせる
	g_aCamera[CAMERATYPE_MAIN].rot.y = pPlayer->rot.y;
}

////======================================================================================================================
//// ミッション成功時のカメラ更新処理(追従)
////======================================================================================================================
//void MoveGoodjobCamera(void)
//{
//	// 変数を宣言
//	D3DXVECTOR3 diffPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// カメラの視点の位置の計算代入用
//	D3DXVECTOR3 diffPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// カメラの注視点の位置の計算代入用
//
//	// ポインタを宣言
//	Player *pPlayer = GetPlayer();		// プレイヤーの情報
//
//	Human *pHuman = GetHumanData();		// 人間の情報
//
//	// 向きを取る
//	float fRot = atan2f(pHuman->pos.x - pPlayer->pos.x, pHuman->pos.z - pPlayer->pos.z);
//
//	// 目標の注視点の位置を更新
//	g_aCamera[CAMERATYPE_MAIN].destPosR.x = pPlayer->pos.x + sinf(fRot) * POS_R_PLUS;	// プレイヤーの位置より少し前
//	g_aCamera[CAMERATYPE_MAIN].destPosR.y = pPlayer->pos.y + POS_R_PLUS_Y;									// プレイヤーの位置と同じ
//	g_aCamera[CAMERATYPE_MAIN].destPosR.z = pPlayer->pos.z + cosf(fRot) * POS_R_PLUS;	// プレイヤーの位置より少し前
//
//	// 目標の視点の位置を更新
//	g_aCamera[CAMERATYPE_MAIN].destPosV.x = g_aCamera[CAMERATYPE_MAIN].destPosR.x + (g_aCamera[CAMERATYPE_MAIN].fDis * sinf(fRot));	// 目標注視点から距離分離れた位置
//	g_aCamera[CAMERATYPE_MAIN].destPosV.y = POS_V_Y;																				// 固定の高さ
//	g_aCamera[CAMERATYPE_MAIN].destPosV.z = g_aCamera[CAMERATYPE_MAIN].destPosR.z + (g_aCamera[CAMERATYPE_MAIN].fDis * cosf(fRot));	// 目標注視点から距離分離れた位置
//
//	// 目標の位置までの差分を計算
//	diffPosV = g_aCamera[CAMERATYPE_MAIN].destPosV - g_aCamera[CAMERATYPE_MAIN].posV;	// 視点
//	diffPosR = g_aCamera[CAMERATYPE_MAIN].destPosR - g_aCamera[CAMERATYPE_MAIN].posR;	// 注視点
//
//	// 視点の位置を更新
//	g_aCamera[CAMERATYPE_MAIN].posV.x += diffPosV.x * REV_POS_V;
//	g_aCamera[CAMERATYPE_MAIN].posV.y += diffPosV.y * REV_POS_V_Y;
//	g_aCamera[CAMERATYPE_MAIN].posV.z += diffPosV.z * REV_POS_V;
//
//	// 注視点の位置を更新
//	g_aCamera[CAMERATYPE_MAIN].posR.x += diffPosR.x * REV_POS_R;
//	g_aCamera[CAMERATYPE_MAIN].posR.y += diffPosR.y * REV_POS_R_Y;
//	g_aCamera[CAMERATYPE_MAIN].posR.z += diffPosR.z * REV_POS_R;
//}

//============================================================
//	メインカメラの位置の更新処理 (操作)
//============================================================
void MoveCamera(void)
{
	//--------------------------------------------------------
	//	マウス操作の更新
	//--------------------------------------------------------
	if (GetMousePress(MOUKEY_LEFT) == true && GetMousePress(MOUKEY_RIGHT) == true)
	{ // 右クリックと左クリックが押されている場合

		// 視点を移動
		g_aCamera[CAMERATYPE_MAIN].posV.x -= sinf(g_aCamera[CAMERATYPE_MAIN].rot.y + (D3DX_PI * 0.5f)) * GetMouseMove().x;
		g_aCamera[CAMERATYPE_MAIN].posV.z -= cosf(g_aCamera[CAMERATYPE_MAIN].rot.y + (D3DX_PI * 0.5f)) * GetMouseMove().x;

		g_aCamera[CAMERATYPE_MAIN].posV.x += sinf(g_aCamera[CAMERATYPE_MAIN].rot.y) * GetMouseMove().y;
		g_aCamera[CAMERATYPE_MAIN].posV.z += cosf(g_aCamera[CAMERATYPE_MAIN].rot.y) * GetMouseMove().y;

		// 注視点を移動
		g_aCamera[CAMERATYPE_MAIN].posR.x -= sinf(g_aCamera[CAMERATYPE_MAIN].rot.y + (D3DX_PI * 0.5f)) * GetMouseMove().x;
		g_aCamera[CAMERATYPE_MAIN].posR.z -= cosf(g_aCamera[CAMERATYPE_MAIN].rot.y + (D3DX_PI * 0.5f)) * GetMouseMove().x;

		g_aCamera[CAMERATYPE_MAIN].posR.x += sinf(g_aCamera[CAMERATYPE_MAIN].rot.y) * GetMouseMove().y;
		g_aCamera[CAMERATYPE_MAIN].posR.z += cosf(g_aCamera[CAMERATYPE_MAIN].rot.y) * GetMouseMove().y;
	}
}

//============================================================
//	メインカメラの距離の更新処理 (操作)
//============================================================
void DisCamera(void)
{
	//--------------------------------------------------------
	//	マウス操作の更新
	//--------------------------------------------------------
	if (GetMouseMove().z != 0.0f)
	{ // マウスホイールの移動量が 0.0f ではない場合

		// マウスの z移動量の方向に視点を移動
		g_aCamera[CAMERATYPE_MAIN].fDis += GetMouseMove().z * REV_DIS_MOUSE;
	}

	//--------------------------------------------------------
	//	距離の補正
	//--------------------------------------------------------
	if (g_aCamera[CAMERATYPE_MAIN].fDis > REV_DIS)
	{ // 最低距離を上回った場合

		// 最低距離に補正
		g_aCamera[CAMERATYPE_MAIN].fDis = REV_DIS;
	}
}

//============================================================
//	メインカメラの向きの更新処理 (操作)
//============================================================
void RotCamera(void)
{
	//--------------------------------------------------------
	//	マウス操作の更新
	//--------------------------------------------------------
	if (GetMousePress(MOUKEY_LEFT) == true && GetMousePress(MOUKEY_RIGHT) == false)
	{ // 左クリックだけが押されている場合

		// マウスの x移動量の方向に y軸を回転
		g_aCamera[CAMERATYPE_MAIN].rot.y += GetMouseMove().x * REV_ROT_MOUSE;

		// マウスの y移動量の方向に x軸を回転
		g_aCamera[CAMERATYPE_MAIN].rot.x += GetMouseMove().y * REV_ROT_MOUSE;
	}

	//--------------------------------------------------------
	//	向きの補正
	//--------------------------------------------------------
	// 向きの補正 (x)
	RevRotXCamera();

	// 向きの補正 (y)
	RevRotYCamera();

	//--------------------------------------------------------
	//	視点の更新
	//--------------------------------------------------------
	g_aCamera[CAMERATYPE_MAIN].posV.x = g_aCamera[CAMERATYPE_MAIN].posR.x + ((g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * sinf(g_aCamera[CAMERATYPE_MAIN].rot.y));
	g_aCamera[CAMERATYPE_MAIN].posV.y = g_aCamera[CAMERATYPE_MAIN].posR.y + ((g_aCamera[CAMERATYPE_MAIN].fDis * cosf(g_aCamera[CAMERATYPE_MAIN].rot.x)));
	g_aCamera[CAMERATYPE_MAIN].posV.z = g_aCamera[CAMERATYPE_MAIN].posR.z + ((g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * cosf(g_aCamera[CAMERATYPE_MAIN].rot.y));

	//--------------------------------------------------------
	//	マウス操作の更新
	//--------------------------------------------------------
	if (GetMousePress(MOUKEY_RIGHT) == true && GetMousePress(MOUKEY_LEFT) == false)
	{ // 右クリックだけが押されている場合

		// マウスの x移動量の方向に y軸を回転
		g_aCamera[CAMERATYPE_MAIN].rot.y += GetMouseMove().x * REV_ROT_MOUSE;

		// マウスの y移動量の方向に x軸を回転
		g_aCamera[CAMERATYPE_MAIN].rot.x += GetMouseMove().y * REV_ROT_MOUSE;
	}

	//--------------------------------------------------------
	//	向きの補正
	//--------------------------------------------------------
	// 向きの補正 (x)
	RevRotXCamera();

	// 向きの補正 (y)
	RevRotYCamera();

	//--------------------------------------------------------
	//	注視点の更新
	//--------------------------------------------------------
	g_aCamera[CAMERATYPE_MAIN].posR.x = g_aCamera[CAMERATYPE_MAIN].posV.x + ((-g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * sinf(g_aCamera[CAMERATYPE_MAIN].rot.y));
	g_aCamera[CAMERATYPE_MAIN].posR.y = g_aCamera[CAMERATYPE_MAIN].posV.y + ((-g_aCamera[CAMERATYPE_MAIN].fDis * cosf(g_aCamera[CAMERATYPE_MAIN].rot.x)));
	g_aCamera[CAMERATYPE_MAIN].posR.z = g_aCamera[CAMERATYPE_MAIN].posV.z + ((-g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * cosf(g_aCamera[CAMERATYPE_MAIN].rot.y));
}

//============================================================
//	マップカメラの位置の更新処理
//============================================================
void MoveMiniMap(void)
{
	// ポインタを宣言
	Player *pPlayer = GetPlayer();	// プレイヤーの情報

	// 注視点の位置を更新
	g_aCamera[CAMERATYPE_MAP].posR.x = pPlayer->pos.x;	// プレイヤーの位置
	g_aCamera[CAMERATYPE_MAP].posR.y = MAPCAM_POS_R;	// 固定の高さ
	g_aCamera[CAMERATYPE_MAP].posR.z = pPlayer->pos.z;	// プレイヤーの位置

	// 視点の位置を更新
	g_aCamera[CAMERATYPE_MAP].posV.x = pPlayer->pos.x;	// プレイヤーの位置
	g_aCamera[CAMERATYPE_MAP].posV.y = MAPCAM_POS_V;	// 固定の高さ
	g_aCamera[CAMERATYPE_MAP].posV.z = pPlayer->pos.z;	// プレイヤーの位置
}

//============================================================
//	カメラの向きの補正処理 (x)
//============================================================
void RevRotXCamera(void)
{
	if (g_aCamera[CAMERATYPE_MAIN].rot.x > LIMIT_ROT_HIGH)
	{ // 回転量 (x) が LIMIT_ROT_HIGH を超えた場合

		// LIMIT_ROT_HIGH を代入 (制限)
		g_aCamera[CAMERATYPE_MAIN].rot.x = LIMIT_ROT_HIGH;
	}
	else if (g_aCamera[CAMERATYPE_MAIN].rot.x < LIMIT_ROT_LOW)
	{ // 回転量 (x) が LIMIT_ROT_LOW を超えた場合

		// LIMIT_ROT_LOW を代入 (制限)
		g_aCamera[CAMERATYPE_MAIN].rot.x = LIMIT_ROT_LOW;
	}
}

//============================================================
//	カメラの向きの補正処理 (y)
//============================================================
void RevRotYCamera(void)
{
	if (g_aCamera[CAMERATYPE_MAIN].rot.y > D3DX_PI)
	{ // 回転量 (y) が D3DX_PI を超えた場合

		// D3DX_PIを代入 (制限)
		g_aCamera[CAMERATYPE_MAIN].rot.y = D3DX_PI;

		// 回転量 (y) を反転させる
		g_aCamera[CAMERATYPE_MAIN].rot.y *= -1.0f;
	}
	else if (g_aCamera[CAMERATYPE_MAIN].rot.y < -D3DX_PI)
	{ // 回転量 (y) が -D3DX_PI を超えた場合

		// -D3DX_PIを代入 (制限)
		g_aCamera[CAMERATYPE_MAIN].rot.y = -D3DX_PI;

		// 回転量 (y) を反転させる
		g_aCamera[CAMERATYPE_MAIN].rot.y *= -1.0f;
	}
}

//============================================================
//	カメラの取得処理
//============================================================
Camera *GetCamera(int nID)
{
	// カメラの情報の引数の要素のアドレスを返す
	return &g_aCamera[nID];
}

//============================================================
// カメラの状態の取得処理
//============================================================
CAMERASTATE *GetCameraState(void)
{
	// カメラの状態を返す
	return &g_CameraState;
}

#ifdef _DEBUG	// デバッグ処理
//============================================================
//	デバッグ処理一覧
//============================================================
//************************************************************
//	視点座標の取得処理
//************************************************************
D3DXVECTOR3 GetCameraPosV(void)
{
	// 視点座標を返す
	return g_aCamera[CAMERATYPE_MAIN].posV;
}

//************************************************************
//	注視点座標の取得処理
//************************************************************
D3DXVECTOR3 GetCameraPosR(void)
{
	// 注視点座標を返す
	return g_aCamera[CAMERATYPE_MAIN].posR;
}

//************************************************************
//	向きの取得処理
//************************************************************
D3DXVECTOR3 GetCameraRot(void)
{
	// 向きを返す
	return g_aCamera[CAMERATYPE_MAIN].rot;
}

//************************************************************
//	距離の取得処理
//************************************************************
float GetCameraDis(void)
{
	// 距離を返す
	return g_aCamera[CAMERATYPE_MAIN].fDis;
}
#endif