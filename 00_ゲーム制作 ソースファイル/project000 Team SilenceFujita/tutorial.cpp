//======================================================================================================================
//
//	チュートリアル画面処理 [tutorial.cpp]
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

#include "tutorial.h"

#include "2Deffect.h"
#include "2Dparticle.h"
#include "3DValue.h"
#include "ability.h"
#include "billboard.h"
#include "bomb.h"
#include "bonus.h"
#include "buildtimer.h"
#include "camera.h"
#include "Car.h"
#include "Combo.h"
#include "effect.h"
#include "gate.h"
#include "icon.h"
#include "junk.h"
#include "life.h"
#include "light.h"
#include "meshdome.h"
#include "meshcylinder.h"
#include "meshfield.h"
#include "meshwall.h"
#include "particle.h"
#include "shadow.h"
#include "object.h"
#include "player.h"
#include "Police.h"
#include "score.h"
#include "tiremark.h"
#include "velocity.h"
#include "weather.h"
#include "wind.h"

//**********************************************************************************************************************
//	列挙型定義 (LESSON_SETUP)
//**********************************************************************************************************************
typedef enum
{
	LESSON_SETUP_SLUMBOOST = 0,	// レッスン4 (破滅疾走) の読み込み
	LESSON_SETUP_FLYAWAY,		// レッスン5 (吹飛散風) の読み込み
	LESSON_SETUP_SILENCEWORLD,	// レッスン6 (無音世界) の読み込み
	LESSON_SETUP_MAX			// この列挙型の総数
} LESSON_SETUP;

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define LESSON_SETUP_TXT	"data\\TXT\\lesson.txt"	// チュートリアルのレッスンセットアップ用のテキストファイルの相対パス

#define MAX_TUTORIAL		(1)			// 使用するポリゴン数
#define END_TUTORIAL_TIME	(120)		// チュートリアル終了までの余韻フレーム

#define TUTORIAL_POS_X		(910.0f)	// チュートリアルの絶対座標 (x)
#define TUTORIAL_POS_Y		(160.0f)	// チュートリアルの絶対座標 (y)
#define TUTORIAL_WIDTH		(360.0f)	// チュートリアルの幅 / 2 (横)
#define TUTORIAL_HEIGHT		(160.0f)	// チュートリアルの幅 / 2 (高さ)

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const int aNextLesson[] =	// レッスンのカウンター
{
	240,	// レッスン0 (移動)     のレッスンカウンター
	120,	// レッスン1 (旋回)     のレッスンカウンター
	30,		// レッスン2 (停止)     のレッスンカウンター
	1,		// レッスン3 (視点変更) のレッスンカウンター
	1,		// レッスン4 (破滅疾走) のレッスンカウンター
	1,		// レッスン5 (吹飛散風) のレッスンカウンター
	1,		// レッスン6 (無音世界) のレッスンカウンター
	0,		// レッスン7 (脱出)     のレッスンカウンター
};

const char *apTextureTutorial[] =		// テクスチャの相対パス
{
	"data\\TEXTURE\\ui005.png",	// レッスン0 (移動)     のテクスチャ相対パス
	"data\\TEXTURE\\ui005.png",	// レッスン1 (旋回)     のテクスチャ相対パス
	"data\\TEXTURE\\ui005.png",	// レッスン2 (停止)     のテクスチャ相対パス
	"data\\TEXTURE\\ui005.png",	// レッスン3 (視点変更) のテクスチャ相対パス
	"data\\TEXTURE\\ui005.png",	// レッスン4 (破滅疾走) のテクスチャ相対パス
	"data\\TEXTURE\\ui005.png",	// レッスン5 (吹飛散風) のテクスチャ相対パス
	"data\\TEXTURE\\ui005.png",	// レッスン6 (無音世界) のテクスチャ相対パス
	"data\\TEXTURE\\ui005.png",	// レッスン7 (脱出)     のテクスチャ相対パス
};

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
bool CheckNextSlumBoost(void);			// 破滅疾走のレッスン終了の確認処理
bool CheckNextFlyAway(void);			// 吹飛散風のレッスン終了の確認処理
bool CheckNextSilenceWorld(void);		// 無音世界のレッスン終了の確認処理

void TxtSetLesson(LESSON_SETUP lesson);	// レッスンのセットアップ処理

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureTutorial[LESSON_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;				// 頂点バッファへのポインタ

TUTORIALSTATE g_tutorialState;			// チュートリアルの状態
int           g_nLessonState;			// レッスンの状態
int           g_nCounterTutorialState;	// チュートリアルの状態管理カウンター
int           g_nCounterLessonState;	// レッスンの状態管理カウンター
bool          g_bTutorialEnd;			// モードの遷移状況

//======================================================================================================================
//	チュートリアル画面の初期化処理
//======================================================================================================================
void InitTutorial(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	チュートリアルの初期化
	//------------------------------------------------------------------------------------------------------------------
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_2D        *pVtx;						// 頂点情報へのポインタ

	for (int nCntTutorial = 0; nCntTutorial < LESSON_MAX; nCntTutorial++)
	{ // 使用するテクスチャ数分繰り返す

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, apTextureTutorial[nCntTutorial], &g_apTextureTutorial[nCntTutorial]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_2D) * 4 * MAX_TUTORIAL,	// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL
	);

	// グローバル変数を初期化
	g_tutorialState         = TUTORIALSTATE_NORMAL;		// チュートリアルの状態
	g_nLessonState          = LESSON_00;				// レッスンの状態
	g_nCounterTutorialState = 0;						// チュートリアルの状態管理カウンター
	g_nCounterLessonState   = 0;						// レッスンの状態管理カウンター
	g_bTutorialEnd          = false;					// モードの遷移状況

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標を設定
	pVtx[0].pos = D3DXVECTOR3(TUTORIAL_POS_X - TUTORIAL_WIDTH, TUTORIAL_POS_Y - TUTORIAL_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TUTORIAL_POS_X + TUTORIAL_WIDTH, TUTORIAL_POS_Y - TUTORIAL_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TUTORIAL_POS_X - TUTORIAL_WIDTH, TUTORIAL_POS_Y + TUTORIAL_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TUTORIAL_POS_X + TUTORIAL_WIDTH, TUTORIAL_POS_Y + TUTORIAL_HEIGHT, 0.0f);

	// rhw の設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();

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

	// 3Dの数値の初期化
	Init3DValue();

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

	// エフェクトの初期化
	InitEffect();

	// パーティクルの初期化
	InitParticle();

	// 体力バーの初期化
	InitLife();

	// 能力バーの初期化
	InitAbility();

	// 速度バーの初期化
	InitVelocity();

	// スコアの初期化
	InitScore();

	// ファイルをロードする全体処理
	LoadFileChunk
	( // 引数
		false,	// 車のカーブ
		false,	// 人間のカーブ
		true,	// ステージ
		true,	// 当たり判定
		true,	// 影
		false,	// オブジェクト
		false	// AI
	);

	//// サウンドの再生
	//PlaySound(SOUND_LABEL_BGM_001);	// BGM (チュートリアル画面)
}

//======================================================================================================================
//	チュートリアル画面の終了処理
//======================================================================================================================
void UninitTutorial(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	チュートリアルの終了
	//------------------------------------------------------------------------------------------------------------------
	// テクスチャの破棄
	for (int nCntTutorial = 0; nCntTutorial < LESSON_MAX; nCntTutorial++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTextureTutorial[nCntTutorial] != NULL)
		{ // 変数 (g_apTextureTutorial) がNULLではない場合

			g_apTextureTutorial[nCntTutorial]->Release();
			g_apTextureTutorial[nCntTutorial] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTutorial != NULL)
	{ // 変数 (g_pVtxBuffTutorial) がNULLではない場合

		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

	//------------------------------------------------------------------------------------------------------------------
	//	使用したソースファイルの終了
	//------------------------------------------------------------------------------------------------------------------
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

	// 3Dの数値の終了
	Uninit3DValue();

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

	// エフェクトの終了
	UninitEffect();

	// パーティクルの終了
	UninitParticle();

	// 体力バーの終了
	UninitLife();

	// 能力バーの終了
	UninitAbility();

	// 速度バーの終了
	UninitVelocity();

	// スコアの終了
	UninitScore();

	//// サウンドの停止
	//StopSound();
}

//======================================================================================================================
//	チュートリアル画面の更新処理
//======================================================================================================================
void UpdateTutorial(void)
{
#if 1
#include "input.h"

	if (GetKeyboardTrigger(DIK_0) == true)
	{
		AddLessonState();
	}
#endif

	if (g_bTutorialEnd == false)
	{ // 遷移設定がされていない場合

		if (GetExit().bExit == true)
		{ // ゲートから脱出した場合

			// 遷移設定がされた状態にする
			g_bTutorialEnd = true;

			// ゲーム画面の状態設定
			SetTutorialState(TUTORIALSTATE_END, END_TUTORIAL_TIME);	// 終了状態
		}
	}

	switch (g_nLessonState)
	{ // レッスンごとの処理
	case LESSON_04:	// レッスン4 (破滅疾走)



		// 処理を抜ける
		break;

	case LESSON_05:	// レッスン5 (吹飛散風)



		// 処理を抜ける
		break;

	case LESSON_06:	// レッスン6 (無音世界)



		// 処理を抜ける
		break;

	case LESSON_07:	// レッスン7 (脱出)

		// 無し

		// 処理を抜ける
		break;
	}

	switch (g_tutorialState)
	{ // 状態ごとの処理
	case TUTORIALSTATE_NORMAL:		// 通常状態

		// 無し

		// 処理を抜ける
		break;

	case TUTORIALSTATE_END:			// 終了状態

		if (g_nCounterTutorialState > 0)
		{ // カウンターが 0より大きい場合

			// カウンターを減算
			g_nCounterTutorialState--;
		}
		else
		{ // カウンターが 0以下の場合

			// モード選択 (タイトル画面に移行)
			SetFade(MODE_TITLE);
		}

		// 処理を抜ける
		break;
	}

	//------------------------------------------------------------------------------------------------------------------
	//	使用するソースファイルの更新
	//------------------------------------------------------------------------------------------------------------------
	// ライトの更新
	UpdateLight();

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

	// プレイヤーのチュートリアル更新
	UpdateTutorialPlayer();

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

	// 3Dの数値の更新
	Update3DValue();

	// 体力バーの更新
	UpdateLife();

	// 能力バーの更新
	UpdateAbility();

	// 速度バーの更新
	UpdateVelocity();

	// スコアの更新
	UpdateScore();

	// コンボの更新
	UpdateCombo();

	// ボーナスの更新処理
	UpdateBonus();

	// 影の更新
	UpdateShadow();
}

//======================================================================================================================
//	チュートリアル画面の描画処理
//======================================================================================================================
void DrawTutorial(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	使用するソースファイルの描画
	//------------------------------------------------------------------------------------------------------------------
	// カメラの設定
	SetCamera(CAMERATYPE_MAIN);

	// メッシュドームの描画
	DrawMeshDome();

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

	//------------------------------------------------------------------------------------------------------------------
	//	UIの描画
	//------------------------------------------------------------------------------------------------------------------
	// カメラの設定
	SetCamera(CAMERATYPE_UI);

	// 体力バーの描画
	DrawLife();

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

	//------------------------------------------------------------------------------------------------------------------
	//	チュートリアルの描画
	//------------------------------------------------------------------------------------------------------------------
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_apTextureTutorial[g_nLessonState]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//======================================================================================================================
//	レッスンの状態の加算処理
//======================================================================================================================
void AddLessonState(void)
{
	if (g_nLessonState < LESSON_MAX - 1)
	{ // 現在のレッスンがレッスンの最大数 -1より小さい場合

		// レッスンカウンターを加算
		g_nCounterLessonState++;

		if (g_nCounterLessonState >= aNextLesson[g_nLessonState])
		{ // レッスンカウンターが次のレッスンに移行する値以上になった場合

			// カウンターを初期化
			g_nCounterLessonState = 0;

			// レッスンを加算 (次のレッスンに進める)
			g_nLessonState++;

			switch (g_nLessonState)
			{ // レッスンごとの処理
			case LESSON_00:	// レッスン0 (移動)

				// 無し

				// 処理を抜ける
				break;

			case LESSON_01:	// レッスン1 (旋回)

				// 無し

				// 処理を抜ける
				break;

			case LESSON_02:	// レッスン2 (停止)

				// 無し

				// 処理を抜ける
				break;

			case LESSON_03:	// レッスン3 (視点変更)

				// 無し

				// 処理を抜ける
				break;

			case LESSON_04:	// レッスン4 (破滅疾走)

				// レッスンのセットアップ
				TxtSetLesson(LESSON_SETUP_SLUMBOOST);

				// 処理を抜ける
				break;

			case LESSON_05:	// レッスン5 (吹飛散風)

				// レッスンのセットアップ
				TxtSetLesson(LESSON_SETUP_FLYAWAY);

				// 処理を抜ける
				break;

			case LESSON_06:	// レッスン6 (無音世界)

				// レッスンのセットアップ
				TxtSetLesson(LESSON_SETUP_SILENCEWORLD);

				// 処理を抜ける
				break;

			case LESSON_07:	// レッスン7 (脱出)

				// ゲートの全開け処理
				AllOpenGate();

				// 処理を抜ける
				break;
			}

			//// サウンドの再生
			//PlaySound(SOUND_LABEL_SE_DEC_00);	// SE (決定00)
		}
	}
}

//======================================================================================================================
//	チュートリアル画面の状態設定処理
//======================================================================================================================
void SetTutorialState(TUTORIALSTATE state, int nCounter)
{
	// 引数の状態を代入
	g_tutorialState = state;

	// 引数の余韻を代入
	g_nCounterTutorialState = nCounter;
}

//======================================================================================================================
//	レッスンの状態の取得処理
//======================================================================================================================
LESSON GetLessonState(void)
{
	// レッスンの状態を返す
	return (LESSON)g_nLessonState;
}

//======================================================================================================================
//	チュートリアルの状態の取得処理
//======================================================================================================================
TUTORIALSTATE GetTutorialState(void)
{
	// チュートリアルの状態を返す
	return g_tutorialState;
}

//======================================================================================================================
//	破滅疾走のレッスン終了の確認処理
//======================================================================================================================
bool CheckNextSlumBoost(void)
{
	// 変数を宣言
	bool bNext = true;		// 次のレッスン遷移状況

	// ポインタを宣言
	Object *pObject = GetObjectData();		// オブジェクトの情報

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pObject++)
	{ // オブジェクトの最大表示数分繰り返す

		if (pObject->bUse == true)
		{ // オブジェクトが使用されている場合

			if (pObject->nBreakType == BREAKTYPE_ON)
			{ // 破壊可能オブジェクトの場合

				// 次のレッスン遷移をできない状態にする
				bNext = false;

				// 処理を抜ける
				break;
			}
		}
	}

	// 次のレッスン遷移状況を返す
	return bNext;
}

//======================================================================================================================
//	吹飛散風のレッスン終了の確認処理
//======================================================================================================================
bool CheckNextFlyAway(void)
{
	// 変数を宣言
	bool bNext = true;		// 次のレッスン遷移状況

	//// ポインタを宣言
	//Object *pObject = GetObjectData();		// オブジェクトの情報

	//for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pObject++)
	//{ // オブジェクトの最大表示数分繰り返す

	//	if (pObject->bUse == true)
	//	{ // オブジェクトが使用されている場合

	//		if (pObject->nBreakType == BREAKTYPE_ON)
	//		{ // 破壊可能オブジェクトの場合

	//			// 次のレッスン遷移をできない状態にする
	//			bNext = false;

	//			// 処理を抜ける
	//			break;
	//		}
	//	}
	//}

	// 次のレッスン遷移状況を返す
	return bNext;
}

//======================================================================================================================
//	無音世界のレッスン終了の確認処理
//======================================================================================================================
bool CheckNextSilenceWorld(void)
{
	// 変数を宣言
	bool bNext = true;		// 次のレッスン遷移状況

	//// ポインタを宣言
	//Object *pObject = GetObjectData();		// オブジェクトの情報

	//for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pObject++)
	//{ // オブジェクトの最大表示数分繰り返す

	//	if (pObject->bUse == true)
	//	{ // オブジェクトが使用されている場合

	//		if (pObject->nBreakType == BREAKTYPE_ON)
	//		{ // 破壊可能オブジェクトの場合

	//			// 次のレッスン遷移をできない状態にする
	//			bNext = false;

	//			// 処理を抜ける
	//			break;
	//		}
	//	}
	//}

	// 次のレッスン遷移状況を返す
	return bNext;
}

//======================================================================================================================
//	レッスンのセットアップ処理
//======================================================================================================================
void TxtSetLesson(LESSON_SETUP lesson)
{
	// 変数を宣言
	int         nEnd;			// テキスト読み込み終了の確認用
	D3DXVECTOR3 pos;			// 位置の代入用
	D3DXVECTOR3 rot;			// 向きの代入用
	D3DXVECTOR3 scale;			// 拡大率の代入用
	D3DXCOLOR   col;			// 色の代入用
	D3DXVECTOR2 radius;			// 半径の代入用
	int         nType;			// 種類の代入用
	int         nBreakType;		// 壊れ方の種類の代入用
	int         nShadowType;	// 影の種類の代入用
	int         nCollisionType;	// 当たり判定の種類の代入用
	int         nNumMat;		// マテリアル数の代入用
	ROTSTATE    stateRot;		// 向き状態
	int			nWalk;			// 歩きタイプの変数
	bool		bRecur;			// 復活の変数
	int			type;			// 種類

	// 変数配列を宣言
	char         aString[MAX_STRING];	// テキストの文字列の代入用
	D3DXMATERIAL aMat[MAX_MATERIAL];	// マテリアルの情報の代入用

	// ポインタを宣言
	FILE *pFile;				// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(LESSON_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			switch (lesson)
			{ // レッスンごとの処理
			case LESSON_SETUP_SLUMBOOST:	// レッスン4 (破滅疾走) の読み込み

				if (strcmp(&aString[0], "SETLESSON_OBJECT") == 0)
				{ // 読み込んだ文字列が SETLESSON_OBJECT の場合

					do
					{ // 読み込んだ文字列が END_SETLESSON_OBJECT ではない場合ループ

						// ファイルから文字列を読み込む
						fscanf(pFile, "%s", &aString[0]);

						if (strcmp(&aString[0], "SET_OBJECT") == 0)
						{ // 読み込んだ文字列が SET_OBJECT の場合

							do
							{ // 読み込んだ文字列が END_SET_OBJECT ではない場合ループ

								// ファイルから文字列を読み込む
								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "POS") == 0)
								{ // 読み込んだ文字列が POS の場合
									fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
									fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);		// 位置を読み込む
								}
								else if (strcmp(&aString[0], "ROT") == 0)
								{ // 読み込んだ文字列が ROT の場合
									fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
									fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);		// 向きを読み込む
								}
								else if (strcmp(&aString[0], "SCALE") == 0)
								{ // 読み込んだ文字列が SCALE の場合
									fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
									fscanf(pFile, "%f%f%f", &scale.x, &scale.y, &scale.z);	// 拡大率を読み込む
								}
								else if (strcmp(&aString[0], "TYPE") == 0)
								{ // 読み込んだ文字列が TYPE の場合
									fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
									fscanf(pFile, "%d", &nType);			// オブジェクトの種類を読み込む
								}
								else if (strcmp(&aString[0], "BREAKTYPE") == 0)
								{ // 読み込んだ文字列が BREAKTYPE の場合
									fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
									fscanf(pFile, "%d", &nBreakType);		// 壊れ方の種類を読み込む
								}
								else if (strcmp(&aString[0], "SHADOWTYPE") == 0)
								{ // 読み込んだ文字列が SHADOWTYPE の場合
									fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
									fscanf(pFile, "%d", &nShadowType);		// 影の種類を読み込む
								}
								else if (strcmp(&aString[0], "COLLTYPE") == 0)
								{ // 読み込んだ文字列が COLLTYPE の場合
									fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
									fscanf(pFile, "%d", &nCollisionType);	// 当たり判定の種類を読み込む
								}
								else if (strcmp(&aString[0], "COLLROT") == 0)
								{ // 読み込んだ文字列が COLLROT の場合
									fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
									fscanf(pFile, "%d", &stateRot);			// 向き状態を読み込む
								}
								else if (strcmp(&aString[0], "NUMMAT") == 0)
								{ // 読み込んだ文字列が NUMMAT の場合
									fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
									fscanf(pFile, "%d", &nNumMat);			// マテリアル数を読み込む

									for (int nCntMat = 0; nCntMat < nNumMat; nCntMat++)
									{ // マテリアル数分繰り返す

										fscanf(pFile, "%s", &aString[0]);	// マテリアルの番号を読み込む (不要)
										fscanf(pFile, "%s", &aString[0]);	// マテリアルの要素を読み込む (不要)
										fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)

										// 拡散光を読み込む
										fscanf(pFile, "%f%f%f%f",
										&aMat[nCntMat].MatD3D.Diffuse.r,
										&aMat[nCntMat].MatD3D.Diffuse.g,
										&aMat[nCntMat].MatD3D.Diffuse.b,
										&aMat[nCntMat].MatD3D.Diffuse.a);

										fscanf(pFile, "%s", &aString[0]);	// マテリアルの番号を読み込む (不要)
										fscanf(pFile, "%s", &aString[0]);	// マテリアルの要素を読み込む (不要)
										fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)

										// 環境光を読み込む
										fscanf(pFile, "%f%f%f%f",
										&aMat[nCntMat].MatD3D.Ambient.r,
										&aMat[nCntMat].MatD3D.Ambient.g,
										&aMat[nCntMat].MatD3D.Ambient.b,
										&aMat[nCntMat].MatD3D.Ambient.a);
									}
								}

							} while (strcmp(&aString[0], "END_SET_OBJECT") != 0);		// 読み込んだ文字列が END_SET_OBJECT ではない場合ループ

							// オブジェクトの設定
							SetObject(pos, rot, scale, &aMat[0], nType, nBreakType, nShadowType, nCollisionType, stateRot, APPEARSTATE_SLOWLY);
						}
					} while (strcmp(&aString[0], "END_SETLESSON_OBJECT") != 0);			// 読み込んだ文字列が END_SETLESSON_OBJECT ではない場合ループ
				}

				// 処理を抜ける
				break;

			case LESSON_SETUP_FLYAWAY:		// レッスン5 (吹飛散風) の読み込み

				if (strcmp(&aString[0], "SETLESSON_HUMAN") == 0)
				{ // 読み込んだ文字列が SETLESSON_HUMAN の場合
					do
					{ // 読み込んだ文字列が END_SETLESSON_HUMAN ではない場合ループ

						// ファイルから文字列を読み込む
						fscanf(pFile, "%s", &aString[0]);

						if (strcmp(&aString[0], "SET_HUMAN") == 0)
						{ // 読み込んだ文字列が SET_HUMAN の場合

							do
							{ // 読み込んだ文字列が END_SET_HUMAN ではない場合ループ

								// ファイルから文字列を読み込む
								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "POS") == 0)
								{ // 読み込んだ文字列が POS の場合
									fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
									fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);	// 位置を読み込む
								}
								else if (strcmp(&aString[0], "WALK") == 0)
								{ // 読み込んだ文字列が WALK の場合
									fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
									fscanf(pFile, "%d", &nWalk);						// 歩きのタイプを読み込む
								}
								else if (strcmp(&aString[0], "RECUR") == 0)
								{ // 読み込んだ文字列が RECUR の場合
									fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
									fscanf(pFile, "%s", &aString[0]);					// 復活状況を読み込む

									if (strcmp(&aString[0], "TRUE") == 0)
									{ // 読み込んだ文字列が TRUE の場合

										// 復活する
										bRecur = true;
									}
									else
									{ // 読み込んだ文字列が FALSE の場合

										// 復活しない
										bRecur = false;
									}
								}
								else if (strcmp(&aString[0], "TYPE") == 0)
								{ // 読み込んだ文字列が TYPE の場合
									fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
									fscanf(pFile, "%d", &type);							// 種類を読み込む
								}

							} while (strcmp(&aString[0], "END_SET_HUMAN") != 0);		// 読み込んだ文字列が END_SET_HUMAN ではない場合ループ

							// 人間の設定
							SetHuman(pos, nWalk, bRecur, type);
						}
					} while (strcmp(&aString[0], "END_SETLESSON_HUMAN") != 0);			// 読み込んだ文字列が END_SETLESSON_HUMAN ではない場合ループ
				}

				// 処理を抜ける
				break;

			case LESSON_SETUP_SILENCEWORLD:	// レッスン6 (無音世界) の読み込み

				if (strcmp(&aString[0], "SETLESSON_CAR") == 0)
				{ // 読み込んだ文字列が SETLESSON_CAR の場合
					do
					{ // 読み込んだ文字列が END_SETLESSON_CAR ではない場合ループ

						// ファイルから文字列を読み込む
						fscanf(pFile, "%s", &aString[0]);

						if (strcmp(&aString[0], "SET_CAR") == 0)
						{ // 読み込んだ文字列が SET_CAR の場合

							do
							{ // 読み込んだ文字列が END_SET_CAR ではない場合ループ

								// ファイルから文字列を読み込む
								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "POS") == 0)
								{ // 読み込んだ文字列が POS の場合
									fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
									fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);	// 位置を読み込む
								}

							} while (strcmp(&aString[0], "END_SET_CAR") != 0);			// 読み込んだ文字列が END_SET_CAR ではない場合ループ

							// 車の設定
							SetCar(pos);
						}
					} while (strcmp(&aString[0], "END_SETLESSON_CAR") != 0);			// 読み込んだ文字列が END_SETLESSON_CAR ではない場合ループ
				}

				// 処理を抜ける
				break;
			}
		} while (nEnd != EOF);	// 読み込んだ文字列が EOF ではない場合ループ
		
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "レッスンファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}