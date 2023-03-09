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
#include "bomb.h"
#include "bonus.h"
#include "buildtimer.h"
#include "camera.h"
#include "Car.h"
#include "Combo.h"
#include "Countdown.h"
#include "effect.h"
#include "flash.h"
#include "Human.h"
#include "gate.h"
#include "icon.h"
#include "item.h"
#include "junk.h"
#include "life.h"
#include "light.h"
#include "map.h"
#include "meshdome.h"
#include "meshcylinder.h"
#include "meshfield.h"
#include "meshwall.h"
#include "particle.h"
#include "pause.h"
#include "shadow.h"
#include "start.h"
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
#endif

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define END_GAME_TIME	(120)		// ゲーム終了までの余韻フレーム

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
GAMESTATE   g_gameState;			// ゲームの状態
RESULTSTATE g_resultState;			// リザルトの状態
int         g_nCounterGameState;	// 状態管理カウンター
bool        g_bPause;				// ポーズ状態の ON / OFF
bool        g_bGameEnd;				// モードの遷移状況
int         g_nGameMode;			// エディットの ON / OFF

//======================================================================================================================
//	ゲーム画面の初期化処理
//======================================================================================================================
void InitGame(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	ゲームの初期化
	//------------------------------------------------------------------------------------------------------------------
	// グローバル変数を初期化
	g_gameState         = GAMESTATE_START;		// ゲームの状態
	g_resultState       = RESULTSTATE_NONE;		// リザルトの状態
	g_nCounterGameState = 0;					// 状態管理カウンター
	g_bPause            = false;				// ポーズ状態の ON / OFF
	g_bGameEnd          = false;				// モードの遷移状況
	g_nGameMode         = GAMEMODE_PLAY;		// エディットの ON / OFF

	//------------------------------------------------------------------------------------------------------------------
	//	使用するソースファイルの初期化
	//------------------------------------------------------------------------------------------------------------------
	// 天気の初期化処理
	InitWeather();

	// 影の初期化
	InitShadow();

	// アイコンの初期化
	InitIcon();

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

	// がれきの初期化
	InitJunk();

	// アイテムの初期化
	InitItem();

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

	// 再建築タイマーの初期化
	InitBuildtimer();

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

	// コンボの初期化
	InitCombo();

	// カウントダウンの初期化
	InitCountDown();

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

	// マップの初期化
	InitMap();

	// 速度バーの初期化
	InitVelocity();

	// スコアの初期化
	InitScore();

	// ポーズの初期化
	InitPause();

	// フラッシュの初期化
	InitFlash();

	// スタートの初期化
	InitStart();

	// ファイルをロードする全体処理
	LoadFileChunk
	( // 引数
		true,	// 車のカーブ
		true,	// 人間のカーブ
		true,	// ステージ
		true,	// 当たり判定
		true,	// 影
		true,	// オブジェクト
		true,	// AI
		true	// アイコン
	);

	// プレイヤーの位置・向きの設定
	SetPositionPlayer(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// プレイヤーのゲートの設定処理
	SetPlayerGate();

	//メインBGMの再生
	if (GetSoundType(SOUND_TYPE_MAIN_BGM) == true)
	{
		//サウンド（ゲームBGM）の再生
		PlaySound(SOUND_LABEL_BGM_GAME_000);
	}

	//効果音BGMの再生
	if (GetSoundType(SOUND_TYPE_SUB_BGM) == true)
	{
		//効果音サウンド（消防車）の再生
		PlaySound(SOUND_LABEL_BGM_FIRECAR_000);

		//効果音サウンド（焼き芋）の再生
		PlaySound(SOUND_LABEL_BGM_YAKIIMO_000);

		//効果音サウンド（暴走車）の再生
		PlaySound(SOUND_LABEL_BGM_BOUSOUCAR_000);

		//効果音サウンド（選挙カー）の再生
		PlaySound(SOUND_LABEL_BGM_SENKYOCAR_000);

		//効果音サウンド（警察）の再生
		PlaySound(SOUND_LABEL_BGM_POLICE_000);

		//効果音サウンド（走行音）の再生
		PlaySound(SOUND_LABEL_BGM_CAR_000);

		//車の効果音サウンドの音量を0に変更
		SetSoundVolume(SOUND_LABEL_BGM_FIRECAR_000, 0.0f);
		SetSoundVolume(SOUND_LABEL_BGM_YAKIIMO_000, 0.0f);
		SetSoundVolume(SOUND_LABEL_BGM_BOUSOUCAR_000, 0.0f);
		SetSoundVolume(SOUND_LABEL_BGM_SENKYOCAR_000, 0.0f);
		SetSoundVolume(SOUND_LABEL_BGM_POLICE_000, 0.0f);
		SetSoundVolume(SOUND_LABEL_BGM_CAR_000, 1.0f);
	}

#ifdef _DEBUG	// デバッグ処理
	// エディットメインの初期化6
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

	// アイコンの終了
	UninitIcon();

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

	// がれきの終了
	UninitJunk();

	// アイテムの終了
	UninitItem();

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

	// 再建築タイマーの終了
	UninitBuildtimer();

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

	// コンボの終了
	UninitCombo();

	// カウントダウンの終了
	UninitCountDown();

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

	// マップの終了
	UninitMap();

	// 速度バーの終了
	UninitVelocity();

	// スコアの終了
	UninitScore();

	// ポーズの終了
	UninitPause();

	// フラッシュの終了
	UninitFlash();

	// スタートの終了
	UninitStart();

	// BGMの停止
	StopSound();

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
			SetGameState(GAMESTATE_END, END_GAME_TIME);	// 終了状態

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

				// コンボの倍率処理(得点の代入処理)
				MagnificCombo(COMBO_INTERRUPTION);

				// モード選択 (リザルト画面に移行)
				SetFade(MODE_RESULT);
			}
		}

		// 処理を抜ける
		break;
	}

	// ライトの更新
	UpdateLight();

	if (g_nGameMode == GAMEMODE_EDIT)
	{ // エディットモードだった場合

#ifdef _DEBUG	// デバッグ処理

		// カメラの更新
		UpdateCamera();

		// エディットメインの更新
		UpdateEditmain();
#endif
	}
	else
	{ // ゲームモードだった場合

		if (g_bPause == false)
		{ // ポーズ状態ではない場合

			// スタートの更新
			UpdateStart();

			// 天気の設定処理
			SetWeather();

			// 天気の更新処理
			UpdateWeather();

			// フラッシュの更新処理
			UpdateFlash();

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

			// プレイヤーのゲーム更新
			UpdateGamePlayer();

			// カメラの更新
			UpdateCamera();

			// タイヤ痕の更新
			UpdateTireMark();

			// 警察の更新
			UpdatePolice();

			// オブジェクトの更新
			UpdateObject();

			// がれきの更新
			UpdateJunk();

			// アイテムの更新
			UpdateItem();

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

			// 再建築タイマーの更新
			UpdateBuildtimer();

			// 体力バーの更新
			UpdateLife();
#if 0
			// タイマーの更新
			UpdateTimer();
#endif
			// カウントダウンの更新
			UpdateCountDown();

			// 能力バーの更新
			UpdateAbility();

			// マップの更新
			UpdateMap();

			// 速度バーの更新
			UpdateVelocity();

			// スコアの更新
			UpdateScore();

			// コンボの更新
			UpdateCombo();

			// ボーナスの更新処理
			UpdateBonus();

			// アイコンの更新
			UpdateIcon();

			// 影の更新
			UpdateShadow();

			// 警察の追加処理
			AddPolice();
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

	// 再建築タイマーの描画
	DrawBuildtimer();

	// オブジェクトの描画
	DrawObject();

	// がれきの描画
	DrawJunk();

	// アイテムの描画
	DrawItem();

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

#if 1
	//------------------------------------------------------------------------------------------------------------------
	//	UIの描画
	//------------------------------------------------------------------------------------------------------------------
	// カメラの設定
	SetCamera(CAMERATYPE_UI);

	// マップの描画
	DrawMap();

	//------------------------------------------------------------------------------------------------------------------
	//	マップカメラの描画
	//------------------------------------------------------------------------------------------------------------------
	// カメラの設定
	SetCamera(CAMERATYPE_MAP);

	// メッシュフィールドの描画
	DrawMeshField(true);

	// アイコンの描画
	DrawIcon();

	//------------------------------------------------------------------------------------------------------------------
	//	UIの描画
	//------------------------------------------------------------------------------------------------------------------
	// カメラの設定
	SetCamera(CAMERATYPE_UI);

	// 体力バーの描画
	DrawLife();

	// タイマーの描画
	DrawTimer();

	// 能力バーの描画
	DrawAbility();

	// 速度バーの描画
	DrawVelocity();

	// コンボの描画
	DrawCombo();

	// スコアの描画
	DrawScore();

	// ボーナスの描画
	DrawBonus();

	// 2Dエフェクトの描画
	Draw2DEffect();

	// 2Dパーティクルの描画
	Draw2DParticle();
#endif

	// フラッシュの描画
	DrawFlash();

	// スタートの描画
	DrawStart();

	// カウントダウンの描画
	DrawCountDown();

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
GAMESTATE *GetGameState(void)
{
	// ゲームの状態を返す
	return &g_gameState;
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