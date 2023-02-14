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

#include "2Deffect.h"
#include "2Dparticle.h"
#include "ability.h"
#include "billboard.h"
#include "bonus.h"
#include "camera.h"
#include "Car.h"
#include "effect.h"
#include "gate.h"
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
//	マクロ定義
//**********************************************************************************************************************
#define RESULT_TIME		(120)		// リザルトまでの余韻フレーム

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
GAMESTATE   g_gameState;			// ゲームの状態
RESULTSTATE g_resultState;			// リザルトの状態
int         g_nCounterGameState;	// 状態管理カウンター
bool        g_bPause;				// ポーズ状態の ON / OFF
bool        g_bGameEnd;				// モードの遷移状況
int         g_nGameMode;			// エディットの ON / OFF
int		    g_nSoundDJ;				// 現在流れているサウンド

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
	g_resultState       = RESULTSTATE_NONE;		// リザルトの状態
	g_nCounterGameState = 0;					// 状態管理カウンター
	g_bPause            = false;				// ポーズ状態の ON / OFF
	g_bGameEnd          = false;				// モードの遷移状況
	g_nGameMode         = GAMEMODE_PLAY;		// エディットの ON / OFF

#ifdef _DEBUG	// デバッグ処理
	g_nSoundDJ = FUJITA_DJ_LABEL_ONE;			// サウンドを初期化する
#endif

	//------------------------------------------------------------------------------------------------------------------
	//	使用するソースファイルの初期化
	//------------------------------------------------------------------------------------------------------------------
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

	// ゲートの初期化
	InitGate();

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

	// ボーナスの初期化
	InitBonus();

	// 2Dエフェクトの初期化
	Init2DEffect();

	// 2Dパーティクルの初期化
	Init2DParticle();

	// タイヤ痕の初期化
	InitTireMark();

	// 送風機の初期化
	InitWind();

	// 爆弾の初期化
	InitBomb();

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

	// ファイルをロードする全体処理
	LoadFileChunk
	( // 引数
		true,	// 車のカーブ
		true,	// 人間のカーブ
		true,	// ステージ
		true,	// 当たり判定
		true,	// 影
		true,	// オブジェクト
		true	// AI
	);

	// ゲートの設定処理
	SetGate(D3DXVECTOR3(1500.0f, 10.0f, 11500.0f), D3DXVECTOR3(0.0f, D3DXToRadian(0), 0.0f), ROTSTATE_0);

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

	// ゲートの終了
	UninitGate();

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

	// ボーナスの終了
	UninitBonus();

	// 2Dエフェクトの終了
	Uninit2DEffect();

	// 2Dパーティクルの終了
	Uninit2DParticle();

	// タイヤ痕の終了
	UninitTireMark();

	// 送風機の終了
	UninitWind();

	// 爆弾の終了
	UninitBomb();

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
}

//======================================================================================================================
//	ゲーム画面の更新処理
//======================================================================================================================
void UpdateGame(void)
{
	if (g_bGameEnd == false)
	{ // 遷移設定がされていない場合

		if (GetExit().bExit == true || GetTimerState() == TIMERSTATE_END || GetPlayer()->bUse == false)
		{ // リザルトに遷移する条件が整った場合

			// 遷移設定がされた状態にする
			g_bGameEnd = true;

			// ゲーム画面の状態設定
			SetGameState(GAMESTATE_END, RESULT_TIME);	// 終了状態

			if (GetExit().bExit == true)
			{ // クリアに成功した場合

				// リザルトをクリア成功状態にする
				g_resultState = RESULTSTATE_CLEAR;

				// サウンドの再生
				//PlaySound(SOUND_LABEL_SE_RES_00);		// SE (リザルト移行00)
			}
			else if (GetTimerState() == TIMERSTATE_END)
			{ // タイムアウトしている場合

				// リザルトをクリア失敗状態にする
				g_resultState = RESULTSTATE_TIMEOVER;

				// サウンドの再生
				//PlaySound(SOUND_LABEL_SE_RES_01);		// SE (リザルト移行01)
			}
			else if (GetPlayer()->bUse == false)
			{ // プレイヤーが死亡している場合

				// リザルトをクリア失敗状態にする
				g_resultState = RESULTSTATE_LIFEOVER;

				// サウンドの再生
				//PlaySound(SOUND_LABEL_SE_RES_01);		// SE (リザルト移行01)
			}
		}
	}

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

		if (UpdateAllClear(g_resultState) == true)
		{ // 全てのアップデートが終わっていた場合

			if (g_nCounterGameState > 0)
			{ // カウンターが 0より大きい場合

				// カウンターを減算
				g_nCounterGameState--;
			}
			else
			{ // カウンターが 0以下の場合

				// モード選択 (リザルト画面に移行)
				SetFade(MODE_RESULT);
			}
		}

		// 処理を抜ける
		break;
	}

	// ライトの更新
	UpdateLight();

	// カメラの更新
	UpdateCamera();

	if (g_nGameMode == GAMEMODE_EDIT)
	{ // エディットモードだった場合

#ifdef _DEBUG	// デバッグ処理
		// エディットメインの更新
		UpdateEditmain();
#endif
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

			// 送風機の更新
			UpdateWind();

			// 爆弾の更新
			UpdateBomb();

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

			// ゲートの更新
			UpdateGate();
			
			// エフェクトの更新
			UpdateEffect();

			// パーティクルの更新
			UpdateParticle();

			// 2Dエフェクトの更新
			Update2DEffect();

			// 2Dパーティクルの更新
			Update2DParticle();

			// ビルボードの更新
			UpdateBillboard();

			// 体力バーの更新
			UpdateLife();

#if 1
			// タイマーの更新
			UpdateTimer();
#endif

			// 能力バーの更新
			UpdateAbility();

			// 速度バーの更新
			UpdateVelocity();

			// スコアの更新
			UpdateScore();

			// ボーナスの更新処理
			UpdateBonus();

			// 影の更新
			UpdateShadow();
		}
		else
		{ // ポーズ状態の場合

			// ポーズの更新
			UpdatePause();
		}
	}

#ifdef _DEBUG	// デバッグ処理
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
#endif
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

	// 雷の描画処理
	DrawThunder();

	// メッシュシリンダーの描画
	DrawMeshCylinder();

	// メッシュフィールドの描画
	DrawMeshField(false);

	// メッシュウォールの描画
	DrawMeshWall();

	// 影の描画
	DrawShadow();

	// プレイヤーの描画
	DrawPlayer();

	// タイヤ痕の描画
	DrawTireMark();

	// オブジェクトの描画
	DrawObject();

	// 警察の描画
	DrawPolice();

	// 車の描画処理
	DrawCar();

	// 人間の描画
	DrawHuman();

	// ゲートの描画
	DrawGate();

	// ビルボードの描画
	DrawBillboard();

	// 送風機の描画
	DrawWind();

	// 爆弾の描画
	DrawBomb();

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

	//------------------------------------------------------------------------------------------------------------------
	//	マップカメラの描画
	//------------------------------------------------------------------------------------------------------------------
	// カメラの設定
	SetCamera(CAMERATYPE_MAP);

	// メッシュフィールドの描画
	DrawMeshField(true);

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

	// ボーナスの描画
	DrawBonus();

	// 2Dエフェクトの描画
	Draw2DEffect();

	// 2Dパーティクルの描画
	Draw2DParticle();
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
//	ゲーム状態の取得処理
//======================================================================================================================
RESULTSTATE GetResultState(void)
{
	// ゲーム終了時の状態を返す
	return g_resultState;
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