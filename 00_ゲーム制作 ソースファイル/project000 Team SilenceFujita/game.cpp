//======================================================================================================================
//
//	ゲーム画面処理 [game.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

#include "game.h"

#include "billboard.h"
#include "camera.h"
#include "effect.h"
#include "light.h"
#include "meshfield.h"
#include "meshwall.h"
#include "particle.h"
#include "pause.h"

#ifdef _DEBUG	// デバッグ処理
#include "Editmain.h"
#endif

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
GAMESTATE g_gameState;			// ゲームの状態
int       g_nCounterGameState;	// 状態管理カウンター
bool      g_bPause;				// ポーズ状態の ON / OFF
int       g_nGameMode;			// エディットの ON / OFF

//======================================================================================================================
//	ゲーム画面の初期化処理
//======================================================================================================================
void InitGame(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	ゲームの初期化
	//------------------------------------------------------------------------------------------------------------------
	// グローバル変数を初期化
	g_gameState         = GAMESTATE_NORMAL;		// ゲームの状態
	g_nCounterGameState = 0;					// 状態管理カウンター
	g_bPause            = false;				// ポーズ状態の ON / OFF
	g_nGameMode = GAMEMODE_PLAY;		// エディットの ON / OFF

	//------------------------------------------------------------------------------------------------------------------
	//	使用するソースファイルの初期化
	//------------------------------------------------------------------------------------------------------------------
	// カメラの初期化
	InitCamera();

	// ライトの初期化
	InitLight();

	// メッシュフィールドの初期化
	InitMeshField();

	// メッシュウォールの初期化
	InitMeshWall();

	// ビルボードの初期化
	InitBillboard();

	// エフェクトの初期化
	InitEffect();

	// パーティクルの初期化
	InitParticle();

	// ポーズの初期化
	InitPause();

#ifdef _DEBUG	// デバッグ処理
	// エディットメインの初期化
	InitEditmain();
#endif
}

//======================================================================================================================
//	ゲーム画面の終了処理
//======================================================================================================================
void UninitGame(void)
{
	// カメラの終了
	UninitCamera();

	// ライトの終了
	UninitLight();

	// メッシュフィールドの終了
	UninitMeshField();

	// メッシュウォールの終了
	UninitMeshWall();

	// ビルボードの終了
	UninitBillboard();

	// エフェクトの終了
	UninitEffect();

	// パーティクルの終了
	UninitParticle();

	// ポーズの終了
	UninitPause();

#ifdef _DEBUG	// デバッグ処理
	// エディットメインの終了
	UninitEditmain();
#endif
}

//======================================================================================================================
//	ゲーム画面の更新処理
//======================================================================================================================
void UpdateGame(void)
{
	switch (g_gameState)
	{ // 状態ごとの処理
	case GAMESTATE_NORMAL:

		if (GetFade() == FADE_NONE)
		{ // フェードしていない場合

			if (GetKeyboardTrigger(DIK_P) == true || GetJoyKeyTrigger(JOYKEY_START, 0) == true)
			{ // ポーズの操作が行われた場合

				// ポーズの選択項目の再設定
				ResetPauseSelect();

				// ポーズの開始、解除
				g_bPause = (g_bPause == false) ? true : false;
			}
		}

		// 処理を抜ける
		break;

	case GAMESTATE_END:

		// 無し

		// 処理を抜ける
		break;
	}

#ifdef _DEBUG	// デバッグ処理

	// ライトの更新
	UpdateLight();

	// カメラの更新
	UpdateCamera();

	if (g_nGameMode == GAMEMODE_EDIT)
	{ // エディットモードだった場合

		// エディットメインの更新
		UpdateEditmain();
	}
	else
	{ // ゲームモードだった場合

		if (g_bPause == false)
		{ // ポーズ状態ではない場合

			// メッシュフィールドの更新
			UpdateMeshField();

			// メッシュウォールの更新
			UpdateMeshWall();
		}
		else
		{ // ポーズ状態の場合

			// ポーズの更新
			UpdatePause();
		}
	}

	// ビルボードの更新
	UpdateBillboard();

	// エフェクトの更新
	UpdateEffect();

	// パーティクルの更新
	UpdateParticle();

	if (GetKeyboardTrigger(DIK_F2) == true)
	{ // [F2] が押された場合

		// ゲームモードを入れ替える
		g_nGameMode = (g_nGameMode + 1) % GAMEMODE_MAX;
	}
	if (GetKeyboardTrigger(DIK_F3) == true)
	{ // [F3] が押された場合

		// オブジェクトのセーブ処理
		//SaveObject();
	}

#else

	if (g_bPause == false)
	{ // ポーズ状態ではない場合

		// ライトの更新
		UpdateLight();

		// カメラの更新
		UpdateCamera();

		// メッシュフィールドの更新
		UpdateMeshField();

		// メッシュウォールの更新
		UpdateMeshWall();

		// ビルボードの更新
		UpdateBillboard();

		// エフェクトの更新
		UpdateEffect();

		// パーティクルの更新
		UpdateParticle();
	}
	else
	{ // ポーズ状態の場合

		// ポーズの更新
		UpdatePause();
	}

#endif
}

//======================================================================================================================
//	ゲーム画面の描画処理
//======================================================================================================================
void DrawGame(void)
{
	// カメラの設定
	SetCamera();

	// メッシュフィールドの描画
	DrawMeshField();

	// メッシュウォールの描画
	DrawMeshWall();

	// ビルボードの描画
	DrawBillboard();

	// エフェクトの描画
	DrawEffect();

	// パーティクルの描画
	DrawParticle();

	if (g_bPause == true)
	{ // ポーズ状態の場合

		// ポーズの描画
		DrawPause();
	}

#ifdef _DEBUG	// デバッグ処理
	if (g_nGameMode == GAMEMODE_EDIT)
	{ // エディットモードの場合

		// エディットメインの描画処理
		DrawEditmain();
	}
#endif
}

//======================================================================================================================
//	ゲーム画面の状態設定処理
//======================================================================================================================
void SetGameState(GAMESTATE state, int nCounter)
{
	// 引数の状態を代入
	g_gameState = state;

	// 引数の余韻を代入
	g_nCounterGameState = nCounter;
}

//======================================================================================================================
//	ポーズ状態の設定処理
//======================================================================================================================
void SetEnablePause(bool bPause)
{
	// 現在のポーズ状態に引数のポーズ状態を代入
	g_bPause = bPause;
}

//======================================================================================================================
//	ゲーム画面の状態取得処理
//======================================================================================================================
GAMESTATE GetGameState(void)
{
	// ゲームの状態を返す
	return g_gameState;
}

//======================================================================================================================
//	ポーズ状態の取得処理
//======================================================================================================================
bool GetPause(void)
{
	// ポーズ状態の真偽を返す
	return g_bPause;
}

//======================================================================================================================
//	ゲームモードの取得処理
//======================================================================================================================
int GetGameMode(void)
{
	// ゲームモードを返す
	return g_nGameMode;
}