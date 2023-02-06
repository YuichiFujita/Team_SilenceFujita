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

#ifdef _DEBUG	// デバッグ処理
#include "game.h"
#endif

//************************************************************
//	マクロ定義
//************************************************************
// カメラ全般
#define VIEW_ANGLE		(D3DXToRadian(65.0f))	// 視野角
#define VIEW_NEAR		(10.0f)					// モデルが見えるZ軸の最小値
#define VIEW_FAR		(60000.0f)				// モデルが見えるZ軸の最大値

// メインカメラ
#define FIRST_ROT		(1.45f)		// 開始時の視点と注視点の間の向き (x)
#define FIRST_DIS		(-280.0f)	// 開始時の視点と注視点の間の距離

#define POS_R_PLUS		(25.0f)		// プレイヤーの位置と注視点の位置の距離 (x, z)
#define POS_R_PLUS_Y	(110.0f)	// プレイヤーの位置と注視点の位置の距離 (y)
#define POS_V_Y			(165.0f)	// 追従時の視点の位置 (y)
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

#define LIMIT_ROT_HIGH	(D3DX_PI - 0.1f)		// 回転量 (x) の回転制限値 (上)
#define LIMIT_ROT_LOW	(0.1f)					// 回転量 (x) の回転制限値 (下)

// マップカメラ
#define MAPCAM_X		(1000)		// マップカメラの左上隅のピクセル座標 (x)
#define MAPCAM_Y		(20)		// マップカメラの左上隅のピクセル座標 (y)
#define MAPCAM_SIZE_X	(260)		// マップカメラの描画する画面の横幅
#define MAPCAM_SIZE_Y	(260)		// マップカメラの描画する画面の縦幅
#define MAPCAM_POS_R	(0.0f)		// マップカメラの注視点の位置 (y)
#define MAPCAM_POS_V	(6000.0f)	// マップカメラの視点の位置 (y)

// 一人称視点カメラ
#define CAMERA_FORWARD_SHIFT	(18.5f)		// 前にカメラをずらす距離
#define CAMERA_UP_SHIFT			(80.0f)		// 上にカメラをずらす距離
#define CAMERA_RIGHT_SHIFT		(7.0f)		// 右にカメラをずらす距離
#define CAMERA_BACK_SHIFT		(35.0f)		// 後ろにカメラをずらす距離

//************************************************************
//	プロトタイプ宣言
//************************************************************
void MoveFollowCamera(void);		// メインカメラの位置の更新処理 (追従)
void MoveCamera(void);				// メインカメラの位置の更新処理 (操作)
void DisCamera(void);				// メインカメラの距離の更新処理 (操作)
void RotCamera(void);				// メインカメラの向きの更新処理 (操作)

void MoveMiniMap(void);				// マップカメラの位置の更新処理

void RevRotXCamera(void);			// カメラの向きの補正処理 (x)
void RevRotYCamera(void);			// カメラの向きの補正処理 (y)

//************************************************************
//	グローバル変数
//************************************************************
Camera g_aCamera[CAMERATYPE_MAX];	// カメラの情報

//============================================================
//	カメラの初期化処理
//============================================================
void InitCamera(void)
{
	//--------------------------------------------------------
	//	メインカメラの初期化
	//--------------------------------------------------------
	// 基本情報を初期化
	g_aCamera[CAMERATYPE_MAIN].posV     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 現在の視点
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

	//--------------------------------------------------------
	//	マップカメラの初期化
	//--------------------------------------------------------
	// 基本情報を初期化
	g_aCamera[CAMERATYPE_MAP].posV     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 現在の視点
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

	//--------------------------------------------------------
	//	UIカメラの初期化
	//--------------------------------------------------------
	// 基本情報を初期化
	g_aCamera[CAMERATYPE_UI].posV     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 現在の視点
	g_aCamera[CAMERATYPE_UI].posR     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 現在の注視点
	g_aCamera[CAMERATYPE_UI].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 目標の視点
	g_aCamera[CAMERATYPE_UI].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 目標の注視点
	g_aCamera[CAMERATYPE_UI].vecU     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 上方向ベクトル
	g_aCamera[CAMERATYPE_UI].rot      = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
	g_aCamera[CAMERATYPE_UI].fDis     = 0.0f;									// 視点と注視点の距離

	// ビューポート情報を初期化
	g_aCamera[CAMERATYPE_UI].viewport.X      = 0;				// 左上隅のピクセル座標 (x)
	g_aCamera[CAMERATYPE_UI].viewport.Y      = 0;				// 左上隅のピクセル座標 (y)
	g_aCamera[CAMERATYPE_UI].viewport.Width  = SCREEN_WIDTH;	// 描画する画面の横幅
	g_aCamera[CAMERATYPE_UI].viewport.Height = SCREEN_HEIGHT;	// 描画する画面の縦幅
	g_aCamera[CAMERATYPE_UI].viewport.MinZ   = 0.0f;
	g_aCamera[CAMERATYPE_UI].viewport.MaxZ   = 1.0f;
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
#ifdef _DEBUG	// デバッグ処理
	#if 1
		switch (GetGameMode())
		{ // ゲームモードごとの処理
		case GAMEMODE_PLAY:
	
			// カメラの位置の更新 (追従)
			MoveFollowCamera();
	
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
#endif

#ifdef NDEBUG	// リリース処理
	#if 1
		// カメラの位置の更新 (追従)
		MoveFollowCamera();
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
#endif
}

//============================================================
//	カメラの設定処理
//============================================================
void SetCamera(int nID)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// ビューポートの設定
	pDevice->SetViewport(&g_aCamera[nID].viewport);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_aCamera[nID].mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH
	( // 引数
		&g_aCamera[nID].mtxProjection,													// プロジェクションマトリックス
		VIEW_ANGLE,																		// 視野角
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
			g_aCamera[CAMERATYPE_MAIN].posV.x = pPlayer->pos.x + sinf(pPlayer->rot.y) * 18.5f;				// 目標注視点から距離分離れた位置
			g_aCamera[CAMERATYPE_MAIN].posV.y = pPlayer->pos.y + 80.0f;										// 固定の高さ
			g_aCamera[CAMERATYPE_MAIN].posV.z = pPlayer->pos.z + cosf(pPlayer->rot.y) * 18.5f;				// 目標注視点から距離分離れた位置

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
			g_aCamera[CAMERATYPE_MAIN].posV.x = pPlayer->pos.x - sinf(pPlayer->rot.y) * 35.0f;				// 目標注視点から距離分離れた位置
			g_aCamera[CAMERATYPE_MAIN].posV.y = pPlayer->pos.y + 80.0f;										// 固定の高さ
			g_aCamera[CAMERATYPE_MAIN].posV.z = pPlayer->pos.z - cosf(pPlayer->rot.y) * 35.0f;				// 目標注視点から距離分離れた位置

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
Camera *GetCamera(void)
{
	// カメラの情報の先頭アドレスを返す
	return &g_aCamera[0];
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