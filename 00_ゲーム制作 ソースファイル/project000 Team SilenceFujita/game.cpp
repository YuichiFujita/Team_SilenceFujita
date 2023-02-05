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
#include "calculation.h"

#include "game.h"

#include "ability.h"
#include "billboard.h"
#include "camera.h"
#include "Car.h"
#include "effect.h"
#include "life.h"
#include "light.h"
#include "meshdome.h"
#include "meshcylinder.h"
#include "meshfield.h"
#include "meshwall.h"
#include "particle.h"
#include "pause.h"
#include "shadow.h"
#include "object.h"
#include "player.h"
#include "Police.h"
#include "score.h"
#include "timer.h"
#include "tiremark.h"
#include "velocity.h"
#include "weather.h"
#include "wind.h"

#ifdef _DEBUG	// デバッグ処理
#include "Editmain.h"
#include "SoundDJ.h"
#endif

#include "bomb.h"

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
GAMESTATE g_gameState;			// ゲームの状態
int       g_nCounterGameState;	// 状態管理カウンター
bool      g_bPause;				// ポーズ状態の ON / OFF
int       g_nGameMode;			// エディットの ON / OFF
int		  g_nSoundDJ;			//現在流れているサウンド

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
	g_nGameMode         = GAMEMODE_PLAY;		// エディットの ON / OFF

#ifdef _DEBUG	// デバッグ処理
	g_nSoundDJ = FUJITA_DJ_LABEL_ONE;			// サウンドを初期化する
#endif

	//------------------------------------------------------------------------------------------------------------------
	//	使用するソースファイルの初期化
	//------------------------------------------------------------------------------------------------------------------
	// カーブの情報の初期化処理
	InitCurveInfo();

	// カーブテキストのロード処理
	LoadCurveTxt();

	// 天気の初期化処理
	InitWeather();

	// 影の初期化
	InitShadow();

	// プレイヤーの初期化
	InitPlayer();

	// 警察の初期化
	InitPolice();

	// オブジェクトの初期化
	InitObject();

	// 車の初期化
	InitCar();

	// 人間の初期化
	InitHuman();

	// カメラの初期化
	InitCamera();

	// ライトの初期化
	InitLight();

	// メッシュドームの初期化
	InitMeshDome();

	// メッシュシリンダーの初期化
	InitMeshCylinder();

	// メッシュフィールドの初期化
	InitMeshField();

	// メッシュウォールの初期化
	InitMeshWall();

	// ビルボードの初期化
	InitBillboard();

	// タイヤ痕の初期化
	InitTireMark();

	// 送風機の初期化
	InitWind();

	// エフェクトの初期化
	InitEffect();

	// パーティクルの初期化
	InitParticle();

	// 体力バーの初期化
	InitLife();

	// タイマーの初期化
	InitTimer();

	// 能力バーの初期化
	InitAbility();

	// 速度バーの初期化
	InitVelocity();

	// スコアの初期化
	InitScore();

	// ポーズの初期化
	InitPause();

	// ステージのセットアップ
	TxtSetStage();

#ifdef _DEBUG	// デバッグ処理
	// エディットメインの初期化
	InitEditmain();
#endif


	// 爆弾の初期化
	InitBomb();
}

//======================================================================================================================
//	ゲーム画面の終了処理
//======================================================================================================================
void UninitGame(void)
{
	// 天気の終了処理
	UninitWeather();

	// 影の終了
	UninitShadow();

	// プレイヤーの終了
	UninitPlayer();

	// 警察の終了
	UninitPolice();

	// オブジェクトの終了
	UninitObject();

	// 車の終了
	UninitCar();

	// 人間の終了
	UninitHuman();

	// カメラの終了
	UninitCamera();

	// ライトの終了
	UninitLight();

	// メッシュドームの終了
	UninitMeshDome();

	// メッシュシリンダーの終了
	UninitMeshCylinder();

	// メッシュフィールドの終了
	UninitMeshField();

	// メッシュウォールの終了
	UninitMeshWall();

	// ビルボードの終了
	UninitBillboard();

	// タイヤ痕の終了
	UninitTireMark();

	// 送風機の終了
	UninitWind();

	// エフェクトの終了
	UninitEffect();

	// パーティクルの終了
	UninitParticle();

	// 体力バーの終了
	UninitLife();

	// タイマーの終了
	UninitTimer();

	// 能力バーの終了
	UninitAbility();

	// 速度バーの終了
	UninitVelocity();

	// スコアの終了
	UninitScore();

	// ポーズの終了
	UninitPause();

#ifdef _DEBUG	// デバッグ処理
	// エディットメインの終了
	UninitEditmain();
#endif


	// 爆弾の終了
	UninitBomb();
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

#ifdef _DEBUG	// デバッグ処理
			if (g_nGameMode == GAMEMODE_PLAY)
			{ // ゲームモードだった場合

				if (GetKeyboardTrigger(DIK_P) == true || GetJoyKeyTrigger(JOYKEY_START, 0) == true)
				{ // ポーズの操作が行われた場合

					// ポーズの選択項目の再設定
					ResetPauseSelect();

					// ポーズの開始、解除
					g_bPause = (g_bPause == false) ? true : false;
				}
			}
#else
			if (GetKeyboardTrigger(DIK_P) == true || GetJoyKeyTrigger(JOYKEY_START, 0) == true)
			{ // ポーズの操作が行われた場合

				// ポーズの選択項目の再設定
				ResetPauseSelect();

				// ポーズの開始、解除
				g_bPause = (g_bPause == false) ? true : false;
			}
#endif
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

	if (g_nGameMode == GAMEMODE_EDIT)
	{ // エディットモードだった場合

		// エディットメインの更新
		UpdateEditmain();

		// カメラの更新
		UpdateCamera();
	}
	else
	{ // ゲームモードだった場合

		if (g_bPause == false)
		{ // ポーズ状態ではない場合

			// 天気の設定処理
			SetWeather();

			// 天気の更新処理
			UpdateWeather();

			// メッシュドームの更新
			UpdateMeshDome();

			// メッシュシリンダーの更新
			UpdateMeshCylinder();

			// メッシュフィールドの更新
			UpdateMeshField();

			// メッシュウォールの更新
			UpdateMeshWall();

			// 風の更新
			UpdateWind();

			// プレイヤーの更新
			UpdatePlayer();

			// タイヤ痕の更新
			UpdateTireMark();

			// 警察の更新
			UpdatePolice();

			// オブジェクトの更新
			UpdateObject();

			// 車の更新処理
			UpdateCar();

			// 人間の更新
			UpdateHuman();
			
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
	}

	// カメラの更新
	UpdateCamera();

	// ビルボードの更新
	UpdateBillboard();

	// 体力バーの更新
	UpdateLife();

	// タイマーの更新
	UpdateTimer();

	// 能力バーの更新
	UpdateAbility();

	// 速度バーの更新
	UpdateVelocity();

	// スコアの更新
	UpdateScore();

	// 影の更新
	UpdateShadow();

	if (GetKeyboardTrigger(DIK_F2) == true)
	{ // [F2] が押された場合

		// ゲームモードを入れ替える
		g_nGameMode = (g_nGameMode + 1) % GAMEMODE_MAX;
	}
	if (GetKeyboardTrigger(DIK_F3) == true)
	{ // [F3] が押された場合

		// ステージの保存
		TxtSaveStage();
	}
	if (GetKeyboardTrigger(DIK_F4) == true)
	{ // [F4] が押された場合

		// サウンドを変える
		g_nSoundDJ = (g_nSoundDJ + 1) % SOUND_DJ_LABEL_MAX;
	}
	if (GetKeyboardTrigger(DIK_F5) == true)
	{ // [F5] が押された場合

		// サウンドの停止
		StopSoundDJ();

		// サウンドを流す
		PlaySound(g_nSoundDJ, true);
	}
	if (GetKeyboardTrigger(DIK_F6) == true)
	{ // [F6]が押された場合

		// サウンドの停止
		StopSoundDJ((SOUND_DJ_LABEL)g_nSoundDJ);
	}
	if (GetKeyboardTrigger(DIK_F9) == true)
	{ // [F9] が押された場合

		// 当たり判定の保存
		TxtSaveCollision();
	}
#else
	if (g_bPause == false)
	{ // ポーズ状態ではない場合

		// ライトの更新
		UpdateLight();

		// 天気の設定処理
		SetWeather();

		// 天気の更新処理
		UpdateWeather();

		// メッシュドームの更新
		UpdateMeshDome();

		// メッシュシリンダーの更新
		UpdateMeshCylinder();

		// メッシュフィールドの更新
		UpdateMeshField();

		// メッシュウォールの更新
		UpdateMeshWall();

		// プレイヤーの更新
		UpdatePlayer();

		// タイヤ痕の更新
		UpdateTireMark();

		// 警察の更新
		UpdatePolice();

		// 車の更新処理
		UpdateCar();

		// 人間の更新
		UpdateHuman();

		// オブジェクトの更新
		UpdateObject();

		// ビルボードの更新
		UpdateBillboard();

		// 風の更新
		UpdateWind();

		// カメラの更新
		UpdateCamera();

		// エフェクトの更新
		UpdateEffect();

		// パーティクルの更新
		UpdateParticle();

		// 体力バーの更新
		UpdateLife();

		// タイマーの更新
		UpdateTimer();

		// 能力バーの更新
		UpdateAbility();

		// 速度バーの更新
		UpdateVelocity();

		// スコアの更新
		UpdateScore();

		// 影の更新
		UpdateShadow();
	}
	else
	{ // ポーズ状態の場合

		// ポーズの更新
		UpdatePause();
	}
#endif


	// 爆弾の更新
	UpdateBomb();
}

//======================================================================================================================
//	ゲーム画面の描画処理
//======================================================================================================================
void DrawGame(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	//------------------------------------------------------------------------------------------------------------------
	//	メインカメラの描画
	//------------------------------------------------------------------------------------------------------------------
	// カメラの設定
	SetCamera(CAMERATYPE_MAIN);

	// メッシュドームの描画
	DrawMeshDome();

	// メッシュシリンダーの描画
	DrawMeshCylinder();

	// メッシュフィールドの描画
	DrawMeshField();

	// メッシュウォールの描画
	DrawMeshWall();

	// 影の描画
	DrawShadow();

	// プレイヤーの描画
	DrawPlayer();

	// タイヤ痕の描画
	DrawTireMark();

	// 警察の描画
	DrawPolice();

	// 車の描画処理
	DrawCar();

	// オブジェクトの描画
	DrawObject();

	// 人間の描画
	DrawHuman();

	// ビルボードの描画
	DrawBillboard();

	// 風の描画
	DrawWind();

	// エフェクトの描画
	DrawEffect();

	// パーティクルの描画
	DrawParticle();

	// 天気の描画処理
	DrawWeather();

#ifdef _DEBUG	// デバッグ処理
	if (g_nGameMode == GAMEMODE_EDIT)
	{ // エディットモードの場合

		// エディットメインの描画処理
		DrawEditmain();
	}
#endif


	// 爆弾の描画
	DrawBomb();

	//------------------------------------------------------------------------------------------------------------------
	//	マップカメラの描画
	//------------------------------------------------------------------------------------------------------------------
	// カメラの設定
	SetCamera(CAMERATYPE_MAP);

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);	// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Zバッファ更新の有効 / 無効の設定

	// メッシュフィールドの描画
	DrawMeshField();

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		// Zバッファ更新の有効 / 無効の設定

	//------------------------------------------------------------------------------------------------------------------
	//	UIの描画
	//------------------------------------------------------------------------------------------------------------------
	// カメラの設定
	SetCamera(CAMERATYPE_UI);

#if 1
	// 体力バーの描画
	DrawLife();

	// タイマーの描画
	DrawTimer();

	// 能力バーの描画
	DrawAbility();

	// 速度バーの描画
	DrawVelocity();

	// スコアの描画
	DrawScore();
#endif

	if (g_bPause == true)
	{ // ポーズ状態の場合

		// ポーズの描画
		DrawPause();
	}
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

//================================
//外積の当たり判定
//================================
void CollisionOuterProduct(D3DXVECTOR3 *Targetpos, D3DXVECTOR3 *TargetposOld, D3DXVECTOR3 WallRpos, D3DXVECTOR3 WallLpos, bool *bUse, int *nShadow)
{
	D3DXVECTOR3 vecMove, vecLine, vecTopos, posCross;	//外積の変数
	float fRate;										//割合の変数

	//弾の軌跡を測る
	vecMove.x = (Targetpos->x - TargetposOld->x);
	vecMove.y = (Targetpos->y - TargetposOld->y);
	vecMove.z = (Targetpos->z - TargetposOld->z);

	//壁の境界線を測る
	vecLine.x = (WallRpos.x - WallLpos.x);
	vecLine.y = (WallRpos.y - WallLpos.y);
	vecLine.z = (WallRpos.z - WallLpos.z);

	//壁の始点から弾の位置の距離を測る
	vecTopos.x = (Targetpos->x - WallLpos.x);
	vecTopos.y = (Targetpos->y - WallLpos.y);
	vecTopos.z = (Targetpos->z - WallLpos.z);

	//割合を求める
	fRate = ((vecTopos.z * vecMove.x) - (vecTopos.x * vecMove.z)) / ((vecLine.z * vecMove.x) - (vecLine.x * vecMove.z));

	//交点を求める
	posCross.x = vecLine.x * fRate - WallLpos.x;
	posCross.y = vecLine.y * fRate - WallLpos.y;
	posCross.z = vecLine.z * fRate - WallLpos.z;

	if ((vecLine.z * vecTopos.x) - (vecLine.x * vecTopos.z) < 0.0f)
	{//境界線を超えた場合
		if (fRate >= 0.0f && fRate <= 1.0f)
		{//割合が0.0f～1.0fの間だった(境界線を超えた)場合
			//位置を設定する
			Targetpos = &posCross;
		}
	}
}