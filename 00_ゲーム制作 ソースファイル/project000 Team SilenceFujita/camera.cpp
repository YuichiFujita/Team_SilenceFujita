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
#define VIEW_ANGLE		(D3DXToRadian(65.0f))	// 視野角
#define VIEW_NEAR		(10.0f)					// モデルが見えるZ軸の最小値
#define VIEW_FAR		(10000.0f)				// モデルが見えるZ軸の最大値

#define FIRST_ROT		(1.45f)		// 開始時の視点と注視点の間の向き (x)
#define FIRST_DIS		(-280.0f)	// 開始時の視点と注視点の間の距離

#define POS_R_PLUS		(25.0f)		// プレイヤーの位置と注視点の位置の距離 (x, z)
#define POS_R_PLUS_Y	(110.0f)		// プレイヤーの位置と注視点の位置の距離 (y)
#define POS_V_Y			(200.0f)	// 追従時の視点の位置 (y)
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

//************************************************************
//	プロトタイプ宣言
//************************************************************
void MoveFollowCamera(void);		// カメラの位置の更新処理 (追従)
void MoveCamera(void);				// カメラの位置の更新処理 (操作)
void DisCamera(void);				// カメラの距離の更新処理 (操作)
void RotCamera(void);				// カメラの向きの更新処理 (操作)

void RevRotXCamera(void);			// カメラの向きの補正処理 (x)
void RevRotYCamera(void);			// カメラの向きの補正処理 (y)

//************************************************************
//	グローバル変数
//************************************************************
Camera g_camera;					// カメラの情報

//============================================================
//	カメラの初期化処理
//============================================================
void InitCamera(void)
{
	// カメラの情報を初期化
	g_camera.posV     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 現在の視点
	g_camera.posR     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 現在の注視点
	g_camera.destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 目標の視点
	g_camera.destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 目標の注視点
	g_camera.vecU     = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// 上方向ベクトル
	g_camera.rot      = D3DXVECTOR3(FIRST_ROT, 0.0f, 0.0f);		// 向き
	g_camera.fDis     = FIRST_DIS;								// 視点と注視点の距離
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
#endif
}

//============================================================
//	カメラの設定処理
//============================================================
void SetCamera(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスへのポインタ

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH
	( // 引数
		&g_camera.mtxProjection,
		VIEW_ANGLE,									// 視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// 画面のアスペクト比
		VIEW_NEAR,									// Z軸の最小値
		VIEW_FAR									// Z軸の最大値
	);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH
	( // 引数
		&g_camera.mtxView,
		&g_camera.posV,		// 視点
		&g_camera.posR,		// 注視点
		&g_camera.vecU		// 上方向ベクトル
	);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//======================================================================================================================
//	カメラの位置の更新処理 (追従)
//======================================================================================================================
void MoveFollowCamera(void)
{
	// 変数を宣言
	D3DXVECTOR3 diffPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// カメラの視点の位置の計算代入用
	D3DXVECTOR3 diffPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// カメラの注視点の位置の計算代入用

	// ポインタを宣言
	Player *pPlayer = GetPlayer();		// プレイヤーの情報

	if (pPlayer->nCameraState == PLAYERCAME_NORMAL)
	{ // 通常のカメラの場合
		// 目標の注視点の位置を更新
		g_camera.destPosR.x = pPlayer->pos.x + sinf(pPlayer->rot.y + D3DX_PI) * POS_R_PLUS;	// プレイヤーの位置より少し前
		g_camera.destPosR.y = pPlayer->pos.y + POS_R_PLUS_Y;								// プレイヤーの位置と同じ
		g_camera.destPosR.z = pPlayer->pos.z + cosf(pPlayer->rot.y + D3DX_PI) * POS_R_PLUS;	// プレイヤーの位置より少し前

		// 目標の視点の位置を更新
		g_camera.destPosV.x = g_camera.destPosR.x + ((g_camera.fDis * sinf(g_camera.rot.x)) * sinf(g_camera.rot.y));	// 目標注視点から距離分離れた位置
		g_camera.destPosV.y = POS_V_Y;																					// 固定の高さ
		g_camera.destPosV.z = g_camera.destPosR.z + ((g_camera.fDis * sinf(g_camera.rot.x)) * cosf(g_camera.rot.y));	// 目標注視点から距離分離れた位置

		// 目標の位置までの差分を計算
		diffPosV = g_camera.destPosV - g_camera.posV;	// 視点
		diffPosR = g_camera.destPosR - g_camera.posR;	// 注視点

		// 視点の位置を更新
		g_camera.posV.x += diffPosV.x * REV_POS_V;
		g_camera.posV.y += diffPosV.y * REV_POS_V_Y;
		g_camera.posV.z += diffPosV.z * REV_POS_V;

		// 注視点の位置を更新
		g_camera.posR.x += diffPosR.x * REV_POS_R;
		g_camera.posR.y += diffPosR.y * REV_POS_R_Y;
		g_camera.posR.z += diffPosR.z * REV_POS_R;
	}
	else if (pPlayer->nCameraState == PLAYERCAME_BACK)
	{ // 後ろを観るカメラだった場合
		// 目標の注視点の位置を更新
		g_camera.posR.x = pPlayer->pos.x + sinf(pPlayer->rot.y + D3DX_PI) * -80.0f;		// プレイヤーの位置より少し後ろ
		g_camera.posR.y = pPlayer->pos.y + POS_R_PLUS_Y;								// プレイヤーの位置と同じ
		g_camera.posR.z = pPlayer->pos.z + cosf(pPlayer->rot.y + D3DX_PI) * -80.0f;		// プレイヤーの位置より少し後ろ

		// 目標の視点の位置を更新
		g_camera.posV.x = g_camera.posR.x - ((g_camera.fDis * sinf(g_camera.rot.x)) * sinf(g_camera.rot.y));	// 目標注視点から距離分離れた位置
		g_camera.posV.y = POS_V_Y;																				// 固定の高さ
		g_camera.posV.z = g_camera.posR.z - ((g_camera.fDis * sinf(g_camera.rot.x)) * cosf(g_camera.rot.y));	// 目標注視点から距離分離れた位置
	}
	else if (pPlayer->nCameraState == PLAYERCAME_FIRST)
	{ // 一人称カメラだった場合
		// 目標の視点の位置を更新
		g_camera.posV.x = pPlayer->pos.x + sinf(pPlayer->rot.y) * 50.0f;				// 目標注視点から距離分離れた位置
		g_camera.posV.y = pPlayer->pos.y + 80.0f;		// 固定の高さ
		g_camera.posV.z = pPlayer->pos.z + cosf(pPlayer->rot.y) * 50.0f;				// 目標注視点から距離分離れた位置

		// 目標の注始点を更新する
		g_camera.posR.x = g_camera.posV.x - ((g_camera.fDis * sinf(g_camera.rot.x)) * sinf(g_camera.rot.y));	// 目標注視点から距離分離れた位置
		g_camera.posR.y = g_camera.posV.y;				// 視点と同じ高さ
		g_camera.posR.z = g_camera.posV.z - ((g_camera.fDis * sinf(g_camera.rot.x)) * cosf(g_camera.rot.y));	// 目標注視点から距離分離れた位置
	}

	// カメラの向きをプレイヤーの向きに合わせる
	g_camera.rot.y = pPlayer->rot.y;
}

//============================================================
//	カメラの位置の更新処理 (操作)
//============================================================
void MoveCamera(void)
{
	//--------------------------------------------------------
	//	マウス操作の更新
	//--------------------------------------------------------
	if (GetMousePress(MOUKEY_LEFT) == true && GetMousePress(MOUKEY_RIGHT) == true)
	{ // 右クリックと左クリックが押されている場合

		// 視点を移動
		g_camera.posV.x -= sinf(g_camera.rot.y + (D3DX_PI * 0.5f)) * GetMouseMove().x;
		g_camera.posV.z -= cosf(g_camera.rot.y + (D3DX_PI * 0.5f)) * GetMouseMove().x;

		g_camera.posV.x += sinf(g_camera.rot.y) * GetMouseMove().y;
		g_camera.posV.z += cosf(g_camera.rot.y) * GetMouseMove().y;

		// 注視点を移動
		g_camera.posR.x -= sinf(g_camera.rot.y + (D3DX_PI * 0.5f)) * GetMouseMove().x;
		g_camera.posR.z -= cosf(g_camera.rot.y + (D3DX_PI * 0.5f)) * GetMouseMove().x;

		g_camera.posR.x += sinf(g_camera.rot.y) * GetMouseMove().y;
		g_camera.posR.z += cosf(g_camera.rot.y) * GetMouseMove().y;
	}
}

//============================================================
//	カメラの距離の更新処理 (操作)
//============================================================
void DisCamera(void)
{
	//--------------------------------------------------------
	//	マウス操作の更新
	//--------------------------------------------------------
	if (GetMouseMove().z != 0.0f)
	{ // マウスホイールの移動量が 0.0f ではない場合

		// マウスの z移動量の方向に視点を移動
		g_camera.fDis += GetMouseMove().z * REV_DIS_MOUSE;
	}

	//--------------------------------------------------------
	//	距離の補正
	//--------------------------------------------------------
	if (g_camera.fDis > REV_DIS)
	{ // 最低距離を上回った場合

		// 最低距離に補正
		g_camera.fDis = REV_DIS;
	}
}

//============================================================
//	カメラの向きの更新処理 (操作)
//============================================================
void RotCamera(void)
{
	//--------------------------------------------------------
	//	マウス操作の更新
	//--------------------------------------------------------
	if (GetMousePress(MOUKEY_LEFT) == true && GetMousePress(MOUKEY_RIGHT) == false)
	{ // 左クリックだけが押されている場合

		// マウスの x移動量の方向に y軸を回転
		g_camera.rot.y += GetMouseMove().x * REV_ROT_MOUSE;

		// マウスの y移動量の方向に x軸を回転
		g_camera.rot.x += GetMouseMove().y * REV_ROT_MOUSE;
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
	g_camera.posV.x = g_camera.posR.x + ((g_camera.fDis * sinf(g_camera.rot.x)) * sinf(g_camera.rot.y));
	g_camera.posV.y = g_camera.posR.y + ((g_camera.fDis * cosf(g_camera.rot.x)));
	g_camera.posV.z = g_camera.posR.z + ((g_camera.fDis * sinf(g_camera.rot.x)) * cosf(g_camera.rot.y));

	//--------------------------------------------------------
	//	マウス操作の更新
	//--------------------------------------------------------
	if (GetMousePress(MOUKEY_RIGHT) == true && GetMousePress(MOUKEY_LEFT) == false)
	{ // 右クリックだけが押されている場合

		// マウスの x移動量の方向に y軸を回転
		g_camera.rot.y += GetMouseMove().x * REV_ROT_MOUSE;

		// マウスの y移動量の方向に x軸を回転
		g_camera.rot.x += GetMouseMove().y * REV_ROT_MOUSE;
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
	g_camera.posR.x = g_camera.posV.x + ((-g_camera.fDis * sinf(g_camera.rot.x)) * sinf(g_camera.rot.y));
	g_camera.posR.y = g_camera.posV.y + ((-g_camera.fDis * cosf(g_camera.rot.x)));
	g_camera.posR.z = g_camera.posV.z + ((-g_camera.fDis * sinf(g_camera.rot.x)) * cosf(g_camera.rot.y));
}

//============================================================
//	カメラの向きの補正処理 (x)
//============================================================
void RevRotXCamera(void)
{
	if (g_camera.rot.x > LIMIT_ROT_HIGH)
	{ // 回転量 (x) が LIMIT_ROT_HIGH を超えた場合

		// LIMIT_ROT_HIGH を代入 (制限)
		g_camera.rot.x = LIMIT_ROT_HIGH;
	}
	else if (g_camera.rot.x < LIMIT_ROT_LOW)
	{ // 回転量 (x) が LIMIT_ROT_LOW を超えた場合

		// LIMIT_ROT_LOW を代入 (制限)
		g_camera.rot.x = LIMIT_ROT_LOW;
	}
}

//============================================================
//	カメラの向きの補正処理 (y)
//============================================================
void RevRotYCamera(void)
{
	if (g_camera.rot.y > D3DX_PI)
	{ // 回転量 (y) が D3DX_PI を超えた場合

		// D3DX_PIを代入 (制限)
		g_camera.rot.y = D3DX_PI;

		// 回転量 (y) を反転させる
		g_camera.rot.y *= -1.0f;
	}
	else if (g_camera.rot.y < -D3DX_PI)
	{ // 回転量 (y) が -D3DX_PI を超えた場合

		// -D3DX_PIを代入 (制限)
		g_camera.rot.y = -D3DX_PI;

		// 回転量 (y) を反転させる
		g_camera.rot.y *= -1.0f;
	}
}

//============================================================
//	カメラの取得処理
//============================================================
Camera *GetCamera(void)
{
	// カメラの情報アドレスを返す
	return &g_camera;
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
	return g_camera.posV;
}

//************************************************************
//	注視点座標の取得処理
//************************************************************
D3DXVECTOR3 GetCameraPosR(void)
{
	// 注視点座標を返す
	return g_camera.posR;
}

//************************************************************
//	向きの取得処理
//************************************************************
D3DXVECTOR3 GetCameraRot(void)
{
	// 向きを返す
	return g_camera.rot;
}

//************************************************************
//	距離の取得処理
//************************************************************
float GetCameraDis(void)
{
	// 距離を返す
	return g_camera.fDis;
}

//************************************************************
//	カメラの再設定処理
//************************************************************
void ResetCamera(void)
{
	// 構造体の要素を再設定
	g_camera.posV     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 現在の視点
	g_camera.posR     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 現在の注視点
	g_camera.destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 目標の視点
	g_camera.destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 目標の注視点
	g_camera.vecU     = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// 上方向ベクトル
	g_camera.rot      = D3DXVECTOR3(FIRST_ROT, 0.0f, 0.0f);		// 向き
	g_camera.fDis     = FIRST_DIS;								// 視点と注視点の距離
}
#endif