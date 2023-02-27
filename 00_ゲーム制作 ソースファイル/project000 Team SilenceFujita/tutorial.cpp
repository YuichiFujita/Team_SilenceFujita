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
//	マクロ定義
//**********************************************************************************************************************
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
	0,		// レッスン0 (移動)     のレッスンカウンター
	0,		// レッスン1 (旋回)     のレッスンカウンター
	0,		// レッスン2 (停止)     のレッスンカウンター
	0,		// レッスン3 (視点変更) のレッスンカウンター
	0,		// レッスン4 (破滅疾走) のレッスンカウンター
	0,		// レッスン5 (吹飛散風) のレッスンカウンター
	0,		// レッスン6 (無音世界) のレッスンカウンター
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

		// 無し

		// 処理を抜ける
		break;

	case LESSON_05:	// レッスン5 (吹飛散風)

		// 無し

		// 処理を抜ける
		break;

	case LESSON_06:	// レッスン6 (無音世界)

		// 無し

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

				// 無し

				// 処理を抜ける
				break;

			case LESSON_05:	// レッスン5 (吹飛散風)

				// 無し

				// 処理を抜ける
				break;

			case LESSON_06:	// レッスン6 (無音世界)

				// 無し

				// 処理を抜ける
				break;

			case LESSON_07:	// レッスン7 (脱出)

				// 無し

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
TUTORIALSTATE GetLessonState(void)
{
	// レッスンの状態を返す
	return (TUTORIALSTATE)g_nLessonState;

}