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
#include "input.h"
#include "calculation.h"

#include "tutorial.h"

#include "2Deffect.h"
#include "2Dparticle.h"
#include "3Dnotation.h"
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
#include "police.h"
#include "praise.h"
#include "score.h"
#include "tiremark.h"
#include "velocity.h"
#include "weather.h"
#include "wind.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define LESSON_SETUP_TXT	"data\\TXT\\lesson.txt"	// チュートリアルのレッスンセットアップ用のテキストファイルの相対パス

#define MAX_TUTO		(9)			// 使用するポリゴン数
#define END_TUTO_TIME	(120)		// チュートリアル終了までの余韻フレーム

#define TUTO_BG_POS_X	(970.0f)	// チュートリアルの背景の絶対座標 (x)
#define TUTO_BG_POS_Y	(125.0f)	// チュートリアルの背景の絶対座標 (y)
#define TUTO_BG_WIDTH	(300.0f)	// チュートリアルの背景の幅 / 2 (横)
#define TUTO_BG_HEIGHT	(125.0f)	// チュートリアルの背景の幅 / 2 (高さ)

#define TUTORIAL_POS_X	(990.0f)	// チュートリアルの絶対座標 (x)
#define TUTORIAL_POS_Y	(120.0f)	// チュートリアルの絶対座標 (y)
#define TUTORIAL_WIDTH	(300.0f)	// チュートリアルの幅 / 2 (横)
#define TUTORIAL_HEIGHT	(125.0f)	// チュートリアルの幅 / 2 (高さ)

#define TIPS_POS_X		(270.0f)	// チュートリアルの備考の絶対座標 (x)
#define TIPS_POS_Y		(600.0f)	// チュートリアルの備考の絶対座標 (y)
#define TIPS_WIDTH		(250.0f)	// チュートリアルの備考の幅 / 2 (横)
#define TIPS_HEIGHT		(100.0f)	// チュートリアルの備考の幅 / 2 (高さ)

#define RESET_POS_Z		(-2000.0f)	// プレイヤー再設定時の z座標
#define LESSON_NUM_COM	(10)		// コンボのレッスンクリアに必要なコンボ数

#define TUTO_PAUSE_POS	(D3DXVECTOR3(85.0f, 255.0f, 0.0f))								// ポーズの絶対座標
#define TUTO_PAUSE_SIZE	(D3DXVECTOR3(70.0f, 70.0f, 0.0f))								// ポーズの大きさ
#define TUTO_LET_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f))	// 手紙の絶対座標
#define TUTO_LET_SIZE	(D3DXVECTOR3(500.0f, 298.75f, 0.0f))							// 手紙の大きさ
#define TUTO_PAP_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 1020.0f, 0.0f))				// 便箋の初期座標
#define TUTO_PAP_SIZE	(D3DXVECTOR3(500.0f, 298.75f, 0.0f))							// 便箋の大きさ
#define TUTO_CONT_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 665.0f, 0.0f))				// 操作の初期座標
#define TUTO_CONT_SIZE	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 50.0f, 0.0f))					// 操作の大きさ

#define TUTO_LET_CHAN	(0.04f)		// 手紙のα値変更量
#define TUTO_LET_STOP	(1.0f)		// 手紙の最大α値
#define TUTO_FADE_CHAN	(0.02f)		// フェードのα値変更量
#define TUTO_FADE_STOP	(0.6f)		// フェードの最大α値

#define TUTO_PAP_MOVE	(0.5f)		// 便箋の位置の更新量
#define TUTO_PAP_STOP	(315.0f)	// 便箋の停止位置 (y)

#define TUTO_CONT_CHAN			(0.008f)	// 操作説明のα値の変更量
#define TUTO_CONT_MAX_ALPHA		(1.0f)		// 操作説明のα値の最大値
#define TUTO_CONT_MIN_ALPHA		(0.2f)		// 操作説明のα値の最小値
#define TUTO_CONT_CHAN_RETURN	(0.06f)		// 操作説明のしまい時のα値の変更量

#define TUTO_POLICE_POS	(D3DXVECTOR3(2000.0f, 0.0f, 3000.0f))	// 警察のレッスンの設定位置

//**********************************************************************************************************************
//	列挙型定義 (TEXTURE_TUTORIAL)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_TUTORIAL_BG = 0,		// 背景
	TEXTURE_TUTORIAL_LETTER,		// 手紙
	TEXTURE_TUTORIAL_PAUSE,			// ポーズアイコン
	TEXTURE_TUTORIAL_CONTROL,		// 操作表示
	TEXTURE_TUTORIAL_MAX,			// この列挙型の総数
} TEXTURE_TUTORIAL;

//**********************************************************************************************************************
//	列挙型定義 (LESSON_SETUP)
//**********************************************************************************************************************
typedef enum
{
	LESSON_SETUP_SLUMBOOST = 0,		// レッスン2 (破滅疾走) の読み込み
	LESSON_SETUP_FLYAWAY,			// レッスン3 (吹飛散風) の読み込み
	LESSON_SETUP_SILENCEWORLD,		// レッスン4 (無音世界) の読み込み
	LESSON_SETUP_COMBO,				// レッスン5 (コンボ)   の読み込み
	LESSON_SETUP_MAX				// この列挙型の総数
} LESSON_SETUP;

//**********************************************************************************************************************
//	列挙型定義 (TUTOSTAGSTATE)
//**********************************************************************************************************************
typedef enum
{
	TUTOSTAGSTATE_NONE = 0,			// 何もしない状態
	TUTOSTAGSTATE_LET_ALPHA,		// 手紙の表示状態
	TUTOSTAGSTATE_FADE_ALPHA,		// フェードの表示状態
	TUTOSTAGSTATE_PAP_TAKE,			// 便箋の取り出し状態
	TUTOSTAGSTATE_WAIT,				// 待機状態
	TUTOSTAGSTATE_PAP_RETURN,		// 便箋のしまい状態
	TUTOSTAGSTATE_MAX				// この列挙型の総数
} TUTOSTAGSTATE;

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTextureTutorial[] =		// チュートリアルテクスチャの相対パス
{
	"data\\TEXTURE\\ui005.tga",			// チュートリアル背景のテクスチャ相対パス
	"data\\TEXTURE\\tutorial000.tga",	// 手紙のテクスチャ相対パス
	"data\\TEXTURE\\tutorial001.tga",	// ポーズアイコンのテクスチャ相対パス
	"data\\TEXTURE\\tutorial002.tga",	// 操作表示のテクスチャ相対パス
};

const int aNextLesson[] =	// レッスンのカウンター
{
	240,	// レッスン0 (移動)     のレッスンカウンター
	180,	// レッスン1 (旋回)     のレッスンカウンター
	30,		// レッスン2 (破滅疾走) のレッスンカウンター
	60,		// レッスン3 (吹飛散風) のレッスンカウンター
	120,	// レッスン4 (無音世界) のレッスンカウンター
	120,	// レッスン5 (コンボ)   のレッスンカウンター
	80,		// レッスン6 (警察)     のレッスンカウンター
	0,		// レッスン7 (脱出)     のレッスンカウンター
};

const char *apTextureLesson[] =		// レッスンテクスチャの相対パス
{
	"data\\TEXTURE\\lesson000.png",	// レッスン0 (移動)     のテクスチャ相対パス
	"data\\TEXTURE\\lesson001.png",	// レッスン1 (旋回)     のテクスチャ相対パス
	"data\\TEXTURE\\lesson002.png",	// レッスン2 (破滅疾走) のテクスチャ相対パス
	"data\\TEXTURE\\lesson003.png",	// レッスン3 (吹飛散風) のテクスチャ相対パス
	"data\\TEXTURE\\lesson004.png",	// レッスン4 (無音世界) のテクスチャ相対パス
	"data\\TEXTURE\\lesson005.png",	// レッスン5 (コンボ)   のテクスチャ相対パス
	"data\\TEXTURE\\lesson006.png",	// レッスン6 (警察)     のテクスチャ相対パス
	"data\\TEXTURE\\lesson007.png",	// レッスン7 (脱出)     のテクスチャ相対パス
};

const char *apTextureTips[] =		// 備考テクスチャの相対パス
{
	"data\\TEXTURE\\tips000.png",	// レッスン0 (移動)     の備考のテクスチャ相対パス
	"data\\TEXTURE\\tips001.png",	// レッスン1 (旋回)     の備考のテクスチャ相対パス
	"data\\TEXTURE\\tips002.png",	// レッスン2 (破滅疾走) の備考のテクスチャ相対パス
	"data\\TEXTURE\\tips003.png",	// レッスン3 (吹飛散風) の備考のテクスチャ相対パス
	"data\\TEXTURE\\tips004.png",	// レッスン4 (無音世界) の備考のテクスチャ相対パス
	"data\\TEXTURE\\tips005.png",	// レッスン5 (コンボ)   の備考のテクスチャ相対パス
	"data\\TEXTURE\\tips006.png",	// レッスン6 (警察)     の備考のテクスチャ相対パス
	"data\\TEXTURE\\tips007.png",	// レッスン7 (脱出)     の備考のテクスチャ相対パス
};

const char *apTexturePaper[] =		// 便箋テクスチャの相対パス
{
	"data\\TEXTURE\\paper000.png",	// レッスン0 (移動)     の便箋のテクスチャ相対パス
	"data\\TEXTURE\\paper001.png",	// レッスン1 (旋回)     の便箋のテクスチャ相対パス
	"data\\TEXTURE\\paper002.png",	// レッスン2 (破滅疾走) の便箋のテクスチャ相対パス
	"data\\TEXTURE\\paper003.png",	// レッスン3 (吹飛散風) の便箋のテクスチャ相対パス
	"data\\TEXTURE\\paper004.png",	// レッスン4 (無音世界) の便箋のテクスチャ相対パス
	"data\\TEXTURE\\paper005.png",	// レッスン5 (コンボ)   の便箋のテクスチャ相対パス
	"data\\TEXTURE\\paper006.png",	// レッスン6 (警察)     の便箋のテクスチャ相対パス
	"data\\TEXTURE\\paper007.png",	// レッスン7 (脱出)     の便箋のテクスチャ相対パス
};

//**********************************************************************************************************************
//	構造体定義 (Tutorial)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3   pos;				// 便箋の位置
	TUTOSTAGSTATE state;			// 演出の状態
	float         fMove;			// 便箋の移動量
	float         fAlphaLetter;		// 手紙のα値
	float         fAlphaFade;		// フェードのα値
	float         fAlphaControl;	// 操作のα値
	float         fChangeControl;	// 操作のα値の変更量
}Tutorial;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void UpdateTutorialUi(void);			// チュートリアルのUIの更新処理
void DrawTutorialUi(bool bBefore);		// チュートリアルのUIの描画処理

bool CheckNextSlumBoost(void);			// 破滅疾走のレッスン終了の確認処理
bool CheckNextFlyAway(void);			// 吹飛散風のレッスン終了の確認処理
bool CheckNextSilenceWorld(void);		// 無音世界のレッスン終了の確認処理

void AllFalseSlumBoost(void);			// 破滅疾走のレッスン終了後の削除処理
void AllFalseFlyAway(void);				// 吹飛散風のレッスン終了後の削除処理
void AllFalseSilenceWorld(void);		// 無音世界のレッスン終了後の削除処理

void ResetPlayer(void);					// プレイヤーの再設定処理

void TxtSetLesson(LESSON_SETUP lesson);	// レッスンのセットアップ処理

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureTutorial[TEXTURE_TUTORIAL_MAX] = {};	// チュートリアルテクスチャへのポインタ
LPDIRECT3DTEXTURE9      g_apTextureLesson[LESSON_MAX] = {};				// レッスンテクスチャへのポインタ
LPDIRECT3DTEXTURE9      g_apTextureTips[LESSON_MAX] = {};				// 備考テクスチャへのポインタ
LPDIRECT3DTEXTURE9      g_apTexturePaper[LESSON_MAX] = {};				// 便箋テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;						// 頂点バッファへのポインタ

Tutorial      g_tutorial;				// チュートリアルの情報
TUTORIALSTATE g_tutorialState;			// チュートリアルの状態
int           g_nLessonState;			// レッスンの状態
int           g_nCounterTutorialState;	// チュートリアルの状態管理カウンター
int           g_nCounterLessonState;	// レッスンの状態管理カウンター
bool          g_bTutorialEnd;			// モードの遷移状況
bool          g_bComboClear;			// コンボのレッスンのクリア判定

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

	// チュートリアルテクスチャの読み込み
	for (int nCntTutorial = 0; nCntTutorial < TEXTURE_TUTORIAL_MAX; nCntTutorial++)
	{ // 使用するテクスチャ数分繰り返す

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, apTextureTutorial[nCntTutorial], &g_apTextureTutorial[nCntTutorial]);
	}

	// レッスン・備考・便箋テクスチャの読み込み
	for (int nCntTutorial = 0; nCntTutorial < LESSON_MAX; nCntTutorial++)
	{ // 使用するテクスチャ数分繰り返す

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, apTextureLesson[nCntTutorial], &g_apTextureLesson[nCntTutorial]);
		D3DXCreateTextureFromFile(pDevice, apTextureTips[nCntTutorial], &g_apTextureTips[nCntTutorial]);
		D3DXCreateTextureFromFile(pDevice, apTexturePaper[nCntTutorial], &g_apTexturePaper[nCntTutorial]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_2D) * 4 * MAX_TUTO,	// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL
	);

	// グローバル変数を初期化
	g_tutorialState         = TUTORIALSTATE_NORMAL;			// チュートリアルの状態
	g_nLessonState          = LESSON_00;					// レッスンの状態
	g_nCounterTutorialState = 0;							// チュートリアルの状態管理カウンター
	g_nCounterLessonState   = 0;							// レッスンの状態管理カウンター
	g_bTutorialEnd          = false;						// モードの遷移状況
	g_bComboClear           = false;						// コンボのレッスンのクリア判定

	// チュートリアルの情報を初期化
	g_tutorial.pos            = TUTO_PAP_POS;				// 便箋の位置
	g_tutorial.state          = TUTOSTAGSTATE_LET_ALPHA;	// 演出の状態
	g_tutorial.fMove          = 0.0f;						// 便箋の移動量
	g_tutorial.fAlphaLetter   = 0.0f;						// 手紙のα値
	g_tutorial.fAlphaFade     = 0.0f;						// フェードのα値
	g_tutorial.fAlphaControl  = 0.0f;						// 操作のα値
	g_tutorial.fChangeControl = TUTO_CONT_CHAN;				// 操作のα値の変更量

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------------------------------------------------------------------------------------------
	//	レッスンの背景の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[0].pos = D3DXVECTOR3(TUTO_BG_POS_X - TUTO_BG_WIDTH, TUTO_BG_POS_Y - TUTO_BG_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TUTO_BG_POS_X + TUTO_BG_WIDTH, TUTO_BG_POS_Y - TUTO_BG_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TUTO_BG_POS_X - TUTO_BG_WIDTH, TUTO_BG_POS_Y + TUTO_BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TUTO_BG_POS_X + TUTO_BG_WIDTH, TUTO_BG_POS_Y + TUTO_BG_HEIGHT, 0.0f);

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

	//------------------------------------------------------------------------------------------------------------------
	//	レッスンの初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[4].pos = D3DXVECTOR3(TUTORIAL_POS_X - TUTORIAL_WIDTH, TUTORIAL_POS_Y - TUTORIAL_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(TUTORIAL_POS_X + TUTORIAL_WIDTH, TUTORIAL_POS_Y - TUTORIAL_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(TUTORIAL_POS_X - TUTORIAL_WIDTH, TUTORIAL_POS_Y + TUTORIAL_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(TUTORIAL_POS_X + TUTORIAL_WIDTH, TUTORIAL_POS_Y + TUTORIAL_HEIGHT, 0.0f);

	// rhw の設定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//------------------------------------------------------------------------------------------------------------------
	//	備考の背景の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[8].pos  = D3DXVECTOR3(TIPS_POS_X - TIPS_WIDTH, TIPS_POS_Y - TIPS_HEIGHT, 0.0f);
	pVtx[9].pos  = D3DXVECTOR3(TIPS_POS_X + TIPS_WIDTH, TIPS_POS_Y - TIPS_HEIGHT, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(TIPS_POS_X - TIPS_WIDTH, TIPS_POS_Y + TIPS_HEIGHT, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(TIPS_POS_X + TIPS_WIDTH, TIPS_POS_Y + TIPS_HEIGHT, 0.0f);

	// rhw の設定
	pVtx[8].rhw  = 1.0f;
	pVtx[9].rhw  = 1.0f;
	pVtx[10].rhw = 1.0f;
	pVtx[11].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[8].col  = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
	pVtx[9].col  = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
	pVtx[10].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
	pVtx[11].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);

	// テクスチャ座標の設定
	pVtx[8].tex  = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex  = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);

	//------------------------------------------------------------------------------------------------------------------
	//	備考の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[12].pos = D3DXVECTOR3(TIPS_POS_X - TIPS_WIDTH, TIPS_POS_Y - TIPS_HEIGHT, 0.0f);
	pVtx[13].pos = D3DXVECTOR3(TIPS_POS_X + TIPS_WIDTH, TIPS_POS_Y - TIPS_HEIGHT, 0.0f);
	pVtx[14].pos = D3DXVECTOR3(TIPS_POS_X - TIPS_WIDTH, TIPS_POS_Y + TIPS_HEIGHT, 0.0f);
	pVtx[15].pos = D3DXVECTOR3(TIPS_POS_X + TIPS_WIDTH, TIPS_POS_Y + TIPS_HEIGHT, 0.0f);

	// rhw の設定
	pVtx[12].rhw = 1.0f;
	pVtx[13].rhw = 1.0f;
	pVtx[14].rhw = 1.0f;
	pVtx[15].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[12].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[13].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[14].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[15].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[12].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[13].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[14].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[15].tex = D3DXVECTOR2(1.0f, 1.0f);

	//------------------------------------------------------------------------------------------------------------------
	//	ポーズの初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[16].pos = D3DXVECTOR3(TUTO_PAUSE_POS.x - TUTO_PAUSE_SIZE.x, TUTO_PAUSE_POS.y - TUTO_PAUSE_SIZE.y, 0.0f);
	pVtx[17].pos = D3DXVECTOR3(TUTO_PAUSE_POS.x + TUTO_PAUSE_SIZE.x, TUTO_PAUSE_POS.y - TUTO_PAUSE_SIZE.y, 0.0f);
	pVtx[18].pos = D3DXVECTOR3(TUTO_PAUSE_POS.x - TUTO_PAUSE_SIZE.x, TUTO_PAUSE_POS.y + TUTO_PAUSE_SIZE.y, 0.0f);
	pVtx[19].pos = D3DXVECTOR3(TUTO_PAUSE_POS.x + TUTO_PAUSE_SIZE.x, TUTO_PAUSE_POS.y + TUTO_PAUSE_SIZE.y, 0.0f);

	// rhw の設定
	pVtx[16].rhw = 1.0f;
	pVtx[17].rhw = 1.0f;
	pVtx[18].rhw = 1.0f;
	pVtx[19].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[16].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[17].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[18].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[19].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[16].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[17].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[18].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[19].tex = D3DXVECTOR2(1.0f, 1.0f);

	//------------------------------------------------------------------------------------------------------------------
	//	手紙の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[20].pos = D3DXVECTOR3(TUTO_LET_POS.x - TUTO_LET_SIZE.x, TUTO_LET_POS.y - TUTO_LET_SIZE.y, 0.0f);
	pVtx[21].pos = D3DXVECTOR3(TUTO_LET_POS.x + TUTO_LET_SIZE.x, TUTO_LET_POS.y - TUTO_LET_SIZE.y, 0.0f);
	pVtx[22].pos = D3DXVECTOR3(TUTO_LET_POS.x - TUTO_LET_SIZE.x, TUTO_LET_POS.y + TUTO_LET_SIZE.y, 0.0f);
	pVtx[23].pos = D3DXVECTOR3(TUTO_LET_POS.x + TUTO_LET_SIZE.x, TUTO_LET_POS.y + TUTO_LET_SIZE.y, 0.0f);

	// rhw の設定
	pVtx[20].rhw = 1.0f;
	pVtx[21].rhw = 1.0f;
	pVtx[22].rhw = 1.0f;
	pVtx[23].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[20].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);
	pVtx[21].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);
	pVtx[22].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);
	pVtx[23].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);

	// テクスチャ座標の設定
	pVtx[20].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[21].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[22].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[23].tex = D3DXVECTOR2(1.0f, 1.0f);

	//------------------------------------------------------------------------------------------------------------------
	//	フェードの初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[24].pos = D3DXVECTOR3(0.0f,         0.0f,          0.0f);
	pVtx[25].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f,          0.0f);
	pVtx[26].pos = D3DXVECTOR3(0.0f,         SCREEN_HEIGHT, 0.0f);
	pVtx[27].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhw の設定
	pVtx[24].rhw = 1.0f;
	pVtx[25].rhw = 1.0f;
	pVtx[26].rhw = 1.0f;
	pVtx[27].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[24].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);
	pVtx[25].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);
	pVtx[26].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);
	pVtx[27].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);

	//------------------------------------------------------------------------------------------------------------------
	//	便箋の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[28].pos = D3DXVECTOR3(g_tutorial.pos.x - TUTO_PAP_SIZE.x, g_tutorial.pos.y - TUTO_PAP_SIZE.y, 0.0f);
	pVtx[29].pos = D3DXVECTOR3(g_tutorial.pos.x + TUTO_PAP_SIZE.x, g_tutorial.pos.y - TUTO_PAP_SIZE.y, 0.0f);
	pVtx[30].pos = D3DXVECTOR3(g_tutorial.pos.x - TUTO_PAP_SIZE.x, g_tutorial.pos.y + TUTO_PAP_SIZE.y, 0.0f);
	pVtx[31].pos = D3DXVECTOR3(g_tutorial.pos.x + TUTO_PAP_SIZE.x, g_tutorial.pos.y + TUTO_PAP_SIZE.y, 0.0f);

	// rhw の設定
	pVtx[28].rhw = 1.0f;
	pVtx[29].rhw = 1.0f;
	pVtx[30].rhw = 1.0f;
	pVtx[31].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[28].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[29].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[30].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[31].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[28].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[29].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[30].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[31].tex = D3DXVECTOR2(1.0f, 1.0f);

	//------------------------------------------------------------------------------------------------------------------
	//	操作の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[32].pos = D3DXVECTOR3(TUTO_CONT_POS.x - TUTO_CONT_SIZE.x, TUTO_CONT_POS.y - TUTO_CONT_SIZE.y, 0.0f);
	pVtx[33].pos = D3DXVECTOR3(TUTO_CONT_POS.x + TUTO_CONT_SIZE.x, TUTO_CONT_POS.y - TUTO_CONT_SIZE.y, 0.0f);
	pVtx[34].pos = D3DXVECTOR3(TUTO_CONT_POS.x - TUTO_CONT_SIZE.x, TUTO_CONT_POS.y + TUTO_CONT_SIZE.y, 0.0f);
	pVtx[35].pos = D3DXVECTOR3(TUTO_CONT_POS.x + TUTO_CONT_SIZE.x, TUTO_CONT_POS.y + TUTO_CONT_SIZE.y, 0.0f);

	// rhw の設定
	pVtx[32].rhw = 1.0f;
	pVtx[33].rhw = 1.0f;
	pVtx[34].rhw = 1.0f;
	pVtx[35].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[32].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);
	pVtx[33].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);
	pVtx[34].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);
	pVtx[35].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);

	// テクスチャ座標の設定
	pVtx[32].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[33].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[34].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[35].tex = D3DXVECTOR2(1.0f, 1.0f);

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

	// 強調表示の初期化
	Init3DNotation();

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

	// 褒めの初期化
	InitPraise();

	// ファイルをロードする全体処理
	LoadFileChunk
	( // 引数
		false,	// 車のカーブ
		false,	// 人間のカーブ
		true,	// ステージ
		true,	// 当たり判定
		true,	// 影
		true,	// オブジェクト
		false,	// AI
		false	// アイコン
	);

	//メインBGMの再生
	if (GetSoundType(SOUND_TYPE_MAIN_BGM) == true)
	{
		// サウンドの再生（チュートリアルBGM）
		PlaySound(SOUND_LABEL_BGM_TUTORIAL_000);
	}
}

//======================================================================================================================
//	チュートリアル画面の終了処理
//======================================================================================================================
void UninitTutorial(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	チュートリアルの終了
	//------------------------------------------------------------------------------------------------------------------
	// チュートリアルテクスチャの破棄
	for (int nCntTutorial = 0; nCntTutorial < TEXTURE_TUTORIAL_MAX; nCntTutorial++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTextureTutorial[nCntTutorial] != NULL)
		{ // 変数 (g_apTextureTutorial) がNULLではない場合

			g_apTextureTutorial[nCntTutorial]->Release();
			g_apTextureTutorial[nCntTutorial] = NULL;
		}
	}

	// レッスン・備考・便箋テクスチャの破棄
	for (int nCntTutorial = 0; nCntTutorial < LESSON_MAX; nCntTutorial++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTextureLesson[nCntTutorial] != NULL)
		{ // 変数 (g_apTextureLesson) がNULLではない場合

			g_apTextureLesson[nCntTutorial]->Release();
			g_apTextureLesson[nCntTutorial] = NULL;
		}

		if (g_apTextureTips[nCntTutorial] != NULL)
		{ // 変数 (g_apTextureTips) がNULLではない場合

			g_apTextureTips[nCntTutorial]->Release();
			g_apTextureTips[nCntTutorial] = NULL;
		}

		if (g_apTexturePaper[nCntTutorial] != NULL)
		{ // 変数 (g_apTexturePaper) がNULLではない場合

			g_apTexturePaper[nCntTutorial]->Release();
			g_apTexturePaper[nCntTutorial] = NULL;
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

	// 強調表示の終了
	Uninit3DNotation();

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

	// 褒めの終了
	UninitPraise();

	// サウンドの停止
	StopSound();
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
			SetTutorialState(TUTORIALSTATE_END, END_TUTO_TIME);	// 終了状態
		}
	}

	// チュートリアルのUIの更新
	UpdateTutorialUi();

	switch (g_nLessonState)
	{ // レッスンごとの処理
	case LESSON_02:	// レッスン2 (破滅疾走)

		if (CheckNextSlumBoost() == true)
		{ // 次のレッスンに遷移可能な場合

			// レッスンの状態の加算
			AddLessonState();
		}

		// 処理を抜ける
		break;

	case LESSON_03:	// レッスン3 (吹飛散風)

		if (CheckNextFlyAway() == true)
		{ // 次のレッスンに遷移可能な場合

			// レッスンの状態の加算
			AddLessonState();
		}

		// 処理を抜ける
		break;

	case LESSON_04:	// レッスン4 (無音世界)

		if (CheckNextSilenceWorld() == true)
		{ // 次のレッスンに遷移可能な場合

			// レッスンの状態の加算
			AddLessonState();
		}

		// 処理を抜ける
		break;

	case LESSON_05:	// レッスン5 (コンボ)

		if (g_bComboClear == true)
		{ // コンボのレッスンがクリア判定の場合

			// レッスンの状態の加算
			AddLessonState();
		}
		else
		{ // コンボのレッスンがクリア判定ではない場合

			if (GetCurrentCombo() >= LESSON_NUM_COM)
			{ // 現在のコンボが一定値以上の場合

				// コンボのレッスンをクリア判定にする
				g_bComboClear = true;
			}
		}

		// 処理を抜ける
		break;

	case LESSON_06:	// レッスン6 (警察)

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
	case TUTORIALSTATE_SKIP:		// スキップ状態

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
	if (g_tutorial.state == TUTOSTAGSTATE_NONE)
	{ // 何もしない状態の場合

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

		// 強調表示の更新
		Update3DNotation();

		// 警察のチュートリアル更新
		UpdateTutorialPolice();

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
	else
	{ // 何もしない状態ではない場合

		// カメラの更新
		UpdateCamera();
	}
}

//======================================================================================================================
//	チュートリアル画面の描画処理
//======================================================================================================================
void DrawTutorial(void)
{
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
	DrawMeshField(false);

	// メッシュウォールの描画
	DrawMeshWall();

	// 影の描画
	DrawShadow();

	// 強調表示の描画
	Draw3DNotation();

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

	// 警察のチュートリアル描画
	DrawTutorialPolice();

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

	// チュートリアルのUIの描画
	DrawTutorialUi(false);

	// ボーナスの描画
	DrawBonus();

	// 2Dエフェクトの描画
	Draw2DEffect();

	// 2Dパーティクルの描画
	Draw2DParticle();

	// チュートリアルのUIの描画
	DrawTutorialUi(true);
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

			case LESSON_02:	// レッスン2 (破滅疾走)

				// レッスンのセットアップ
				TxtSetLesson(LESSON_SETUP_SLUMBOOST);

				// 処理を抜ける
				break;

			case LESSON_03:	// レッスン3 (吹飛散風)

				// 破滅疾走のレッスン終了後の削除
				AllFalseSlumBoost();

				// レッスンのセットアップ
				TxtSetLesson(LESSON_SETUP_FLYAWAY);

				// 処理を抜ける
				break;

			case LESSON_04:	// レッスン4 (無音世界)

				// 吹飛散風のレッスン終了後の削除
				AllFalseFlyAway();

				// レッスンのセットアップ
				TxtSetLesson(LESSON_SETUP_SILENCEWORLD);

				// 処理を抜ける
				break;

			case LESSON_05:	// レッスン5 (コンボ)

				// 無音世界のレッスン終了後の削除
				AllFalseSilenceWorld();

				// レッスンのセットアップ
				TxtSetLesson(LESSON_SETUP_COMBO);

				// 処理を抜ける
				break;

			case LESSON_06:	// レッスン6 (警察)

				// 破滅疾走のレッスン終了後の削除
				AllFalseSlumBoost();

				// 吹飛散風のレッスン終了後の削除
				AllFalseFlyAway();

				// 無音世界のレッスン終了後の削除
				AllFalseSilenceWorld();

				// 警察のチュートリアルの設定
				SetTutorialPolice(TUTO_POLICE_POS);

				// 処理を抜ける
				break;

			case LESSON_07:	// レッスン7 (脱出)

				// ゲートの全開け
				AllOpenGate();

				// 処理を抜ける
				break;
			}

			// プレイヤーの再設定
			ResetPlayer();

			// エフェクトの削除
			AllFalseEffect();

			// パーティクルの削除
			AllFalseParticle();

			// 風の全消去
			WindAllClear();

			// 演出の状態を手紙の表示状態に変更
			g_tutorial.state = TUTOSTAGSTATE_LET_ALPHA;

			// サウンドの再生
			PlaySound(SOUND_LABEL_SE_SCORE_000);	// SE (決定00)
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
//	チュートリアルのUIの更新処理
//======================================================================================================================
void UpdateTutorialUi(void)
{
	// ポインタを宣言
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	switch (g_tutorial.state)
	{ // 状態ごとの処理
	case TUTOSTAGSTATE_NONE:		// 何もしない状態

		if (GetKeyboardTrigger(DIK_P) == true
		||  GetJoyKeyTrigger(JOYKEY_START, 0) == true)
		{ // 便箋を取り出す操作が行われた場合

			if (g_tutorialState == TUTORIALSTATE_NORMAL)
			{ // チュートリアルが通常状態の場合

				// 手紙の表示状態にする
				g_tutorial.state = TUTOSTAGSTATE_LET_ALPHA;

				// サウンドの再生
				PlaySound(SOUND_LABEL_SE_TUTORIAL_PAGE_000);		// SE (チュートリアルをめくるSE_000)
			}
		}

		// 処理を抜ける
		break;

	case TUTOSTAGSTATE_LET_ALPHA:	// 手紙の表示状態

		// 手紙のα値を加算
		g_tutorial.fAlphaLetter += TUTO_LET_CHAN;

		if (g_tutorial.fAlphaLetter >= TUTO_LET_STOP)
		{ // 手紙のα値が一定値以上の場合

			// 手紙のα値を補正
			g_tutorial.fAlphaLetter = TUTO_LET_STOP;

			// フェードの表示状態にする
			g_tutorial.state = TUTOSTAGSTATE_FADE_ALPHA;
		}

		// 頂点カラーの設定
		pVtx[20].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);
		pVtx[21].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);
		pVtx[22].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);
		pVtx[23].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);

		// 処理を抜ける
		break;

	case TUTOSTAGSTATE_FADE_ALPHA:	// フェードの表示状態

		// フェードのα値を加算
		g_tutorial.fAlphaFade += TUTO_FADE_CHAN;
		
		if (g_tutorial.fAlphaFade >= TUTO_FADE_STOP)
		{ // フェードのα値が一定値以上の場合

			// フェードのα値を補正
			g_tutorial.fAlphaFade = TUTO_FADE_STOP;

			// 便箋の取り出し状態にする
			g_tutorial.state = TUTOSTAGSTATE_PAP_TAKE;

			// 効果音の再生
			if (GetSoundType(SOUND_TYPE_SE) == true)
			{
				// サウンド（チュートリアルのページをめくる音）を再生
				PlaySound(SOUND_LABEL_SE_TUTORIAL_PAGE_000);
			}
		}

		// 頂点カラーの設定
		pVtx[24].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);
		pVtx[25].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);
		pVtx[26].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);
		pVtx[27].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);

		// 処理を抜ける
		break;

	case TUTOSTAGSTATE_PAP_TAKE:	// 便箋の取り出し状態

		// 便箋の位置減算量を設定
		g_tutorial.fMove += TUTO_PAP_MOVE;

		// 便箋の位置を減算
		g_tutorial.pos.y -= g_tutorial.fMove;

		if (g_tutorial.pos.y <= TUTO_PAP_STOP)
		{ // 便箋の位置が一定値以下の場合

			// 便箋の位置を補正
			g_tutorial.pos.y = TUTO_PAP_STOP;

			// 便箋の位置減算量を初期化
			g_tutorial.fMove = 0;

			// 待機状態にする
			g_tutorial.state = TUTOSTAGSTATE_WAIT;
		}

		// 頂点座標を設定
		pVtx[28].pos = D3DXVECTOR3(g_tutorial.pos.x - TUTO_PAP_SIZE.x, g_tutorial.pos.y - TUTO_PAP_SIZE.y, 0.0f);
		pVtx[29].pos = D3DXVECTOR3(g_tutorial.pos.x + TUTO_PAP_SIZE.x, g_tutorial.pos.y - TUTO_PAP_SIZE.y, 0.0f);
		pVtx[30].pos = D3DXVECTOR3(g_tutorial.pos.x - TUTO_PAP_SIZE.x, g_tutorial.pos.y + TUTO_PAP_SIZE.y, 0.0f);
		pVtx[31].pos = D3DXVECTOR3(g_tutorial.pos.x + TUTO_PAP_SIZE.x, g_tutorial.pos.y + TUTO_PAP_SIZE.y, 0.0f);

		// 処理を抜ける
		break;

	case TUTOSTAGSTATE_WAIT:		// 待機状態

		if (GetKeyboardTrigger(DIK_SPACE) == true
		||  GetJoyKeyTrigger(JOYKEY_A, 0) == true)
		{ // 便箋をしまう操作が行われた場合

			// 便箋のしまい状態にする
			g_tutorial.state = TUTOSTAGSTATE_PAP_RETURN;

			// 効果音の再生
			if (GetSoundType(SOUND_TYPE_SE) == true)
			{
				// サウンド（チュートリアルのページをめくる音）を再生
				PlaySound(SOUND_LABEL_SE_TUTORIAL_PAGE_000);
			}
		}

		if (GetKeyboardTrigger(DIK_P) == true
		||  GetJoyKeyTrigger(JOYKEY_START, 0) == true)
		{ // タイトル遷移の操作が行われた場合

			if (g_bTutorialEnd == false)
			{ // 遷移設定がされていない場合

				// 遷移設定がされた状態にする
				g_bTutorialEnd = true;

				// 便箋のしまい状態にする
				g_tutorial.state = TUTOSTAGSTATE_PAP_RETURN;

				// 効果音の再生
				if (GetSoundType(SOUND_TYPE_SE) == true)
				{
					// サウンド（チュートリアルのページをめくる音）を再生
					PlaySound(SOUND_LABEL_SE_TUTORIAL_PAGE_000);
				}

				// ゲーム画面の状態設定
				SetTutorialState(TUTORIALSTATE_SKIP, END_TUTO_TIME);	// スキップ状態
			}
		}

		// 操作説明のα値を変更
		g_tutorial.fAlphaControl += g_tutorial.fChangeControl;

		if (g_tutorial.fAlphaControl < TUTO_CONT_MIN_ALPHA
		||  g_tutorial.fAlphaControl > TUTO_CONT_MAX_ALPHA)
		{ // 透明度が範囲外の場合

			// 変数の符号を反転
			g_tutorial.fChangeControl *= -1.0f;

			// 透明度の補正
			g_tutorial.fAlphaControl = (g_tutorial.fAlphaControl < TUTO_CONT_MIN_ALPHA) ? TUTO_CONT_MIN_ALPHA : TUTO_CONT_MAX_ALPHA;
		}

		// 頂点カラーの設定
		pVtx[32].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);
		pVtx[33].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);
		pVtx[34].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);
		pVtx[35].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);

		// 処理を抜ける
		break;

	case TUTOSTAGSTATE_PAP_RETURN:	// 便箋のしまい状態

		// 手紙の透明化
		if (g_tutorial.fAlphaLetter > 0.0f)
		{ // 手紙のα値が一定値より大きい場合

			// 手紙のα値を減算
			g_tutorial.fAlphaLetter -= TUTO_LET_CHAN;

			if (g_tutorial.fAlphaLetter <= 0.0f)
			{ // 手紙のα値が一定値以下の場合

				// 手紙のα値を補正
				g_tutorial.fAlphaLetter = 0.0f;
			}

			// 頂点カラーの設定
			pVtx[20].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);
			pVtx[21].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);
			pVtx[22].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);
			pVtx[23].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);
		}

		// フェードの透明化
		if (g_tutorial.fAlphaFade > 0.0f)
		{ // フェードのα値が一定値より大きい場合

			// フェードのα値を減算
			g_tutorial.fAlphaFade -= TUTO_FADE_CHAN;

			if (g_tutorial.fAlphaFade <= 0.0f)
			{ // フェードのα値が一定値以下の場合

				// フェードのα値を補正
				g_tutorial.fAlphaFade = 0.0f;
			}

			// 頂点カラーの設定
			pVtx[24].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);
			pVtx[25].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);
			pVtx[26].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);
			pVtx[27].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);
		}

		// 操作説明の透明化
		if (g_tutorial.fAlphaControl > 0.0f)
		{ // 操作説明のα値が一定値より大きい場合

			// 操作説明のα値を減算
			g_tutorial.fAlphaControl -= TUTO_CONT_CHAN_RETURN;

			if (g_tutorial.fAlphaControl <= 0.0f)
			{ // 操作説明のα値が一定値以下の場合

				// 操作説明のα値を補正
				g_tutorial.fAlphaControl = 0.0f;

				// 操作のα値の変更量を初期化
				g_tutorial.fChangeControl = TUTO_CONT_CHAN;
			}

			// 頂点カラーの設定
			pVtx[32].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);
			pVtx[33].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);
			pVtx[34].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);
			pVtx[35].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);
		}

		// 便箋の移動
		if (g_tutorial.pos.y < TUTO_PAP_POS.y)
		{ // 便箋の位置が一定値より小さい場合

			// 便箋の位置加算量を設定
			g_tutorial.fMove += TUTO_PAP_MOVE;

			// 便箋の位置を加算
			g_tutorial.pos.y += g_tutorial.fMove;
		}
		else if (g_tutorial.pos.y >= TUTO_PAP_POS.y)
		{ // 便箋の位置が一定値以上の場合

			// 便箋の位置を補正
			g_tutorial.pos.y = TUTO_PAP_POS.y;

			// 便箋の位置減算量を初期化
			g_tutorial.fMove = 0;

			if (g_tutorial.fAlphaLetter  <= 0.0f
			&&  g_tutorial.fAlphaFade    <= 0.0f
			&&  g_tutorial.fAlphaControl <= 0.0f)
			{ // 全てのα値が下がり切っている場合

				// 何もしない状態にする
				g_tutorial.state = TUTOSTAGSTATE_NONE;
			}
		}

		// 頂点座標を設定
		pVtx[28].pos = D3DXVECTOR3(g_tutorial.pos.x - TUTO_PAP_SIZE.x, g_tutorial.pos.y - TUTO_PAP_SIZE.y, 0.0f);
		pVtx[29].pos = D3DXVECTOR3(g_tutorial.pos.x + TUTO_PAP_SIZE.x, g_tutorial.pos.y - TUTO_PAP_SIZE.y, 0.0f);
		pVtx[30].pos = D3DXVECTOR3(g_tutorial.pos.x - TUTO_PAP_SIZE.x, g_tutorial.pos.y + TUTO_PAP_SIZE.y, 0.0f);
		pVtx[31].pos = D3DXVECTOR3(g_tutorial.pos.x + TUTO_PAP_SIZE.x, g_tutorial.pos.y + TUTO_PAP_SIZE.y, 0.0f);

		// 処理を抜ける
		break;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();
}

//======================================================================================================================
//	チュートリアルのUIの描画処理
//======================================================================================================================
void DrawTutorialUi(bool bBefore)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (bBefore == false)
	{ // 後の描画の場合

		//--------------------------------------------------------------------------------------------------------------
		//	レッスンの背景の描画
		//--------------------------------------------------------------------------------------------------------------
		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureTutorial[TEXTURE_TUTORIAL_BG]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		//--------------------------------------------------------------------------------------------------------------
		//	レッスンの描画
		//--------------------------------------------------------------------------------------------------------------
		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureLesson[g_nLessonState]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);

		//--------------------------------------------------------------------------------------------------------------
		//	備考の背景の描画
		//--------------------------------------------------------------------------------------------------------------
		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);

		//--------------------------------------------------------------------------------------------------------------
		//	備考の描画
		//--------------------------------------------------------------------------------------------------------------
		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureTips[g_nLessonState]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, 2);

		//--------------------------------------------------------------------------------------------------------------
		//	ポーズの描画
		//--------------------------------------------------------------------------------------------------------------
		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureTutorial[TEXTURE_TUTORIAL_PAUSE]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, 2);
	}
	else
	{ // 前の描画の場合

		if (g_tutorial.state != TUTOSTAGSTATE_NONE)
		{ // 何もしない状態ではない場合

			//----------------------------------------------------------------------------------------------------------
			//	手紙の描画
			//----------------------------------------------------------------------------------------------------------
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureTutorial[TEXTURE_TUTORIAL_LETTER]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, 2);

			//----------------------------------------------------------------------------------------------------------
			//	フェードの描画
			//----------------------------------------------------------------------------------------------------------
			// テクスチャの設定
			pDevice->SetTexture(0, NULL);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 24, 2);

			//----------------------------------------------------------------------------------------------------------
			//	便箋の描画
			//----------------------------------------------------------------------------------------------------------
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTexturePaper[g_nLessonState]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 28, 2);

			//----------------------------------------------------------------------------------------------------------
			//	操作の描画
			//----------------------------------------------------------------------------------------------------------
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureTutorial[TEXTURE_TUTORIAL_CONTROL]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 32, 2);
		}
	}
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
	Junk   *pJunk   = GetJunkData();		// がれきの情報

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pObject++)
	{ // オブジェクトの最大表示数分繰り返す

		if (pObject->bUse == true)
		{ // オブジェクトが使用されている場合

			if (pObject->nCollisionType != COLLISIONTYPE_NONE)
			{ // 当たり判定があるオブジェクトの場合

				// 次のレッスン遷移をできない状態にする
				bNext = false;

				// 処理を抜ける
				break;
			}
		}
	}

	for (int nCntJunk = 0; nCntJunk < MAX_JUNK; nCntJunk++, pJunk++)
	{ // がれきの最大表示数分繰り返す

		if (pJunk->bUse == true)
		{ // がれきが使用されている場合

			// 次のレッスン遷移をできない状態にする
			bNext = false;

			// 処理を抜ける
			break;
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

	// ポインタを宣言
	Human *pHuman = GetHumanData();		// 人間の情報

	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++, pHuman++)
	{ // 人間の最大表示数分繰り返す

		if (pHuman->bUse == true)
		{ // 人間が使用されている場合

			// 次のレッスン遷移をできない状態にする
			bNext = false;

			// 処理を抜ける
			break;
		}
	}

	// 次のレッスン遷移状況を返す
	return bNext;
}

//======================================================================================================================
//	無音世界のレッスン終了の確認処理
//======================================================================================================================
bool CheckNextSilenceWorld(void)
{
	// 変数を宣言
	bool bNext = true;			// 次のレッスン遷移状況

	// ポインタを宣言
	Car *pCar = GetCarData();	// 車の情報

	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++, pCar++)
	{ // 車の最大表示数分繰り返す

		if (pCar->bUse == true)
		{ // 車が使用されている場合

			if (GetBarrierState(pCar) != BARRIERSTATE_SET)
			{ // 車のバリア状態が完成状態ではない場合

				// 次のレッスン遷移をできる状態にする
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
//	破滅疾走のレッスン終了後の削除処理
//======================================================================================================================
void AllFalseSlumBoost(void)
{
	// ポインタを宣言
	Object *pObject = GetObjectData();	// オブジェクトの情報

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pObject++)
	{ // オブジェクトの最大表示数分繰り返す

		if (pObject->nCollisionType != COLLISIONTYPE_NONE)
		{ // 当たり判定があるオブジェクトの場合

			// オブジェクトを使用していない状態にする
			pObject->bUse = false;
		}
	}

	// 影の更新
	UpdateShadow();		// 影の削除用

	// 強調表示の更新
	Update3DNotation();	// 強調表示の削除用
}

//======================================================================================================================
//	吹飛散風のレッスン終了後の削除処理
//======================================================================================================================
void AllFalseFlyAway(void)
{
	// ポインタを宣言
	Human *pHuman = GetHumanData();		// 人間の情報

	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++, pHuman++)
	{ // 人間の最大表示数分繰り返す

		// 人間を使用していない状態にする
		pHuman->bUse = false;
	}

	// 影の更新
	UpdateShadow();		// 影の削除用

	// 強調表示の更新
	Update3DNotation();	// 強調表示の削除用
}

//======================================================================================================================
//	無音世界のレッスン終了後の削除処理
//======================================================================================================================
void AllFalseSilenceWorld(void)
{
	// ポインタを宣言
	Car         *pCar     = GetCarData();			// 車の情報
	Barrier     *pBarrier = GetBarrierData();		// バリアの情報
	BarrierInfo *pBarInfo = GetBarrierInfoData();	// バリアのまとまりの情報

	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++, pCar++)
	{ // 車の最大表示数分繰り返す

		// 車を使用していない状態にする
		pCar->bUse = false;
	}

	for (int nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++, pBarrier++)
	{ // バリアの最大表示数分繰り返す

		// バリアを使用していない状態にする
		pBarrier->bUse = false;
	}

	for (int nCntBarInfo = 0; nCntBarInfo < MAX_BARINFO; nCntBarInfo++, pBarInfo++)
	{ // バリアのまとまりの最大表示数分繰り返す

		// バリアのまとまりを使用していない状態にする
		pBarInfo->bUse = false;
	}

	// 影の更新
	UpdateShadow();		// 影の削除用

	// 強調表示の更新
	Update3DNotation();	// 強調表示の削除用
}

//======================================================================================================================
//	プレイヤーの再設定処理
//======================================================================================================================
void ResetPlayer(void)
{
	// ポインタを宣言
	Player *pPlayer = GetPlayer();	// プレイヤーの情報

	// 位置を設定
	pPlayer->pos    = D3DXVECTOR3(0.0f, 0.0f, RESET_POS_Z);		// 現在の位置
	pPlayer->oldPos = D3DXVECTOR3(0.0f, 0.0f, RESET_POS_Z);		// 前回の位置

	// 本体情報の初期化
	pPlayer->move          = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	pPlayer->rot           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	pPlayer->moveRot       = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き変更量
	pPlayer->state         = ACTIONSTATE_NORMAL;				// プレイヤーの状態
	pPlayer->nLife         = PLAY_LIFE;							// 体力
	pPlayer->nCounterState = 0;									// 状態管理カウンター
	pPlayer->bMove         = false;								// 移動状況
	pPlayer->bJump         = false;								// ジャンプ状況
	pPlayer->nCameraState  = PLAYCAMESTATE_NORMAL;				// カメラの状態
	pPlayer->bCameraFirst  = false;								// 一人称カメラの状況

	// ブーストの情報の初期化
	pPlayer->boost.plusMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 追加移動量
	pPlayer->boost.state    = BOOSTSTATE_NONE;					// 加速状態
	pPlayer->boost.nCounter = 0;								// 加速管理カウンター

	// 風の情報の初期化
	pPlayer->wind.bUseWind     = false;							// 風の使用状況
	pPlayer->wind.nCircleCount = 0;								// どこに出すか
	pPlayer->wind.nCount       = 0;								// 風を出すカウント
	pPlayer->wind.rot          = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 風を出す方向

	// 爆弾の情報の初期化
	pPlayer->bomb.state           = ATTACKSTATE_NONE;			// 攻撃状態
	pPlayer->bomb.nCounterState   = BOMB_WAIT_CNT;				// 攻撃管理カウンター
	pPlayer->bomb.nCounterControl = 0;							// 操作管理カウンター
	pPlayer->bomb.bShot           = false;						// 発射待機状況

	// 風の送風機
	SetWindSound(false);

	// 影の位置設定
	SetPositionShadow(pPlayer->nShadowID, pPlayer->pos, pPlayer->rot, NONE_SCALE);
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
	int			nJudgeType;		// 善悪の種類の代入用
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
			case LESSON_SETUP_SLUMBOOST:	// レッスン2 (破滅疾走) の読み込み

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
								else if (strcmp(&aString[0], "JUDGE") == 0)
								{ // 読み込んだ文字列が JUDGE の場合
									fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
									fscanf(pFile, "%d", &nJudgeType);		// 善悪状態を読み込む
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
							SetObject(pos, rot, scale, &aMat[0], nType, nBreakType, nShadowType, nCollisionType, stateRot, APPEARSTATE_NONE, nJudgeType);
						}
					} while (strcmp(&aString[0], "END_SETLESSON_OBJECT") != 0);			// 読み込んだ文字列が END_SETLESSON_OBJECT ではない場合ループ
				}

				// 処理を抜ける
				break;

			case LESSON_SETUP_FLYAWAY:		// レッスン3 (吹飛散風) の読み込み

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
								else if (strcmp(&aString[0], "ROT") == 0)
								{ // 読み込んだ文字列が ROT の場合
									fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
									fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);	// 向きを読み込む
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
							SetHuman(pos, D3DXToRadian(rot), nWalk, bRecur, type);
						}
					} while (strcmp(&aString[0], "END_SETLESSON_HUMAN") != 0);			// 読み込んだ文字列が END_SETLESSON_HUMAN ではない場合ループ
				}

				// 処理を抜ける
				break;

			case LESSON_SETUP_SILENCEWORLD:	// レッスン4 (無音世界) の読み込み

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
								else if (strcmp(&aString[0], "ROT") == 0)
								{ // 読み込んだ文字列が ROT の場合
									fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
									fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);	// 向きを読み込む
								}
								else if (strcmp(&aString[0], "WALK") == 0)
								{ // 読み込んだ文字列が WALK の場合
									fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
									fscanf(pFile, "%d", &nWalk);						// 移動のタイプを読み込む
								}
								else if (strcmp(&aString[0], "TYPE") == 0)
								{ // 読み込んだ文字列が TYPE の場合
									fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
									fscanf(pFile, "%d", &type);							// 種類を読み込む
								}

							} while (strcmp(&aString[0], "END_SET_CAR") != 0);			// 読み込んだ文字列が END_SET_CAR ではない場合ループ

							// 車の設定
							SetCar(pos, D3DXToRadian(rot), nWalk, type);
						}
					} while (strcmp(&aString[0], "END_SETLESSON_CAR") != 0);			// 読み込んだ文字列が END_SETLESSON_CAR ではない場合ループ
				}

				// 処理を抜ける
				break;

			case LESSON_SETUP_COMBO:		// レッスン5 (コンボ) の読み込み

				if (strcmp(&aString[0], "SETLESSON_COMBO") == 0)
				{ // 読み込んだ文字列が SETLESSON_COMBO の場合

					do
					{ // 読み込んだ文字列が END_SETLESSON_COMBO ではない場合ループ

						// ファイルから文字列を読み込む
						fscanf(pFile, "%s", &aString[0]);

						//----------------------------------------------------------------------------------------------
						//	オブジェクトの設定
						//----------------------------------------------------------------------------------------------
						if (strcmp(&aString[0], "SETCOMBO_OBJECT") == 0)
						{ // 読み込んだ文字列が SETCOMBO_OBJECT の場合

							do
							{ // 読み込んだ文字列が END_SETCOMBO_OBJECT ではない場合ループ

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
										else if (strcmp(&aString[0], "JUDGE") == 0)
										{ // 読み込んだ文字列が JUDGE の場合
											fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
											fscanf(pFile, "%d", &nJudgeType);		// 善悪状態を読み込む
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
									SetObject(pos, rot, scale, &aMat[0], nType, nBreakType, nShadowType, nCollisionType, stateRot, APPEARSTATE_NONE, nJudgeType);
								}
							} while (strcmp(&aString[0], "END_SETCOMBO_OBJECT") != 0);			// 読み込んだ文字列が END_SETCOMBO_OBJECT ではない場合ループ
						}

						//----------------------------------------------------------------------------------------------
						//	人間の設定
						//----------------------------------------------------------------------------------------------
						else if (strcmp(&aString[0], "SETCOMBO_HUMAN") == 0)
						{ // 読み込んだ文字列が SETCOMBO_HUMAN の場合

							do
							{ // 読み込んだ文字列が END_SETCOMBO_HUMAN ではない場合ループ

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
										else if (strcmp(&aString[0], "ROT") == 0)
										{ // 読み込んだ文字列が ROT の場合
											fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
											fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);	// 向きを読み込む
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
									SetHuman(pos, D3DXToRadian(rot), nWalk, bRecur, type);
								}
							} while (strcmp(&aString[0], "END_SETCOMBO_HUMAN") != 0);			// 読み込んだ文字列が END_SETCOMBO_HUMAN ではない場合ループ
						}

						//----------------------------------------------------------------------------------------------
						//	車の設定
						//----------------------------------------------------------------------------------------------
						else if (strcmp(&aString[0], "SETCOMBO_CAR") == 0)
						{ // 読み込んだ文字列が SETCOMBO_CAR の場合

							do
							{ // 読み込んだ文字列が END_SETCOMBO_CAR ではない場合ループ

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
										else if (strcmp(&aString[0], "ROT") == 0)
										{ // 読み込んだ文字列が ROT の場合
											fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
											fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);	// 向きを読み込む
										}
										else if (strcmp(&aString[0], "WALK") == 0)
										{ // 読み込んだ文字列が WALK の場合
											fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
											fscanf(pFile, "%d", &nWalk);						// 移動のタイプを読み込む
										}
										else if (strcmp(&aString[0], "TYPE") == 0)
										{ // 読み込んだ文字列が TYPE の場合
											fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
											fscanf(pFile, "%d", &type);							// 種類を読み込む
										}

									} while (strcmp(&aString[0], "END_SET_CAR") != 0);			// 読み込んだ文字列が END_SET_CAR ではない場合ループ

									// 車の設定
									SetCar(pos, D3DXToRadian(rot), nWalk, type);
								}
							} while (strcmp(&aString[0], "END_SETCOMBO_CAR") != 0);				// 読み込んだ文字列が END_SETCOMBO_CAR ではない場合ループ
						}
					} while (strcmp(&aString[0], "END_SETLESSON_COMBO") != 0);					// 読み込んだ文字列が END_SETLESSON_COMBO ではない場合ループ
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