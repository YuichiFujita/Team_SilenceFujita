//======================================================================================================================
//
//	タイトル画面処理 [title.cpp]
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

#include "title.h"

#include "calculation.h"
#include "light.h"
#include "camera.h"
#include "icon.h"
#include "Human.h"
#include "weather.h"
#include "shadow.h"
#include "Car.h"
#include "Police.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_TITLE			(3)			// 使用するポリゴン数

#define TITLE_POS_X			(640.0f)	// タイトルの停止時の絶対座標 (x)
#define TITLE_POS_Y			(250.0f)	// タイトルの停止時の絶対座標 (y)
#define TITLE_WIDTH			(300.0f)	// タイトルの横幅 / 2
#define TITLE_HEIGHT		(120.0f)	// タイトルの縦幅 / 2

#define TITLE_SELECT_POS_X			(SCREEN_WIDTH * 0.5f)			//タイトル選択肢の開始位置（X)
#define TITLE_SELECT_POS_Y			(SCREEN_HEIGHT * 0.70f)			//タイトル選択肢の開始位置（Y）	
#define TITLE_SELECT_SIZE_X			(200.0f)						//タイトル選択肢の大きさ（X）
#define TITLE_SELECT_SIZE_Y			(60.0f)							//タイトル選択肢の大きさ（Y）
#define TITLE_SELECT_INTERVAL_Y		(125.0f)						//タイトル選択肢の間隔（Y）

#define TITLE_MOVE			(2.0f)		// タイトルの移動量
#define TIT_FIRST_ALPHA		(0.65f)		// 背景の初期の透明度
#define TIT_CHANGE_ALPHA	(0.003f)	// 背景の透明度の変更値
#define TIT_ALPHA_HIGH		(0.5f)		// 不透明度の最大値
#define TIT_ALPHA_LOW		(0.5f)		// 透明度の最大値

#define TRANS_CNT			(900)		// 自動遷移するまでのカウント

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTextureTitle[] =			// テクスチャの相対パス
{
	"data\\TEXTURE\\title000.png",				// タイトルのテクスチャ相対パス
	"data\\TEXTURE\\title_select_start.png",	// タイトル選択（スタート）
	"data\\TEXTURE\\title_select_tutorial.png",	// タイトル選択（チュートリアル）
};

//**********************************************************************************************************************
//	列挙型定義 (TEXTURE_TITLE)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_TITLE_TITLE = 0,			// タイトル
	TEXTURE_TITLE_SELECT_START,			// タイトル選択（スタート）
	TEXTURE_TITLE_SELECT_TUTORIAL,		// タイトル選択（チュートリアル）
	TEXTURE_TITLE_MAX,					// この列挙型の総数
} TEXTURE_TITLE;

//**********************************************************************************************************************
//	列挙型定義 (TEXTURE_TITLE)
//**********************************************************************************************************************
typedef enum
{
	TRANSITION_TITLE_LOGO = 0,			// ロゴ画面
	TRANSITION_TITLE_RANKING,			// ランキング
	TRANSITION_TITLE_MAX,				// この列挙型の総数
} TRANSITION_TITLE;

//**********************************************************************************************************************
//	タイトル選択の種類の列挙型定義 (TITLE_SELECT_TYPE)
//**********************************************************************************************************************
typedef enum
{
	TITLE_SELECT_TYPE_START = 0,	//選択肢（スタート）
	TITLE_SELECT_TYPE_TUTORIAL,		//選択肢（チュートリアル）
	TITLE_SELECT_TYPE_MAX
}TITLE_SELECT_TYPE;

//**********************************************************************************************************************
//	構造体定義 (タイトルの選択)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXCOLOR col;				//色
}TitleSelect;

//**********************************************************************************************************************
//	構造体定義 (タイトルの選択決定)
//**********************************************************************************************************************
typedef struct
{
	TITLE_SELECT_TYPE type;		//モード選択の種類
	bool bOk;					//決定の有無
}TitleCursor;

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureTitle[TEXTURE_TITLE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;						// 頂点バッファへのポインタ

D3DXVECTOR3 g_posTitle;								// タイトルの絶対座標
TITLESTATE  g_stateTitle;							// タイトルの状態
TitleSelect g_aTitleSelect[TITLE_SELECT_TYPE_MAX];	// タイトルの選択の情報
TitleCursor g_titleCursor;							// タイトル選択カーソルの情報

float g_fAlphaTitle;		// 背景の透明度
float g_fChangeTitle;		// 透明度の変更値
bool  g_bTitleSound;		// タイトルのサウンドの有無

int g_nTransCount;							// 遷移カウント
int g_nTransMode = TRANSITION_TITLE_LOGO;	// 遷移するモード

//======================================================================================================================
//	タイトル画面の初期化処理
//======================================================================================================================
void InitTitle(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	タイトルの初期化
	//------------------------------------------------------------------------------------------------------------------
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_2D         *pVtx;					// 頂点情報へのポインタ

	for (int nCntTitle = 0; nCntTitle < TEXTURE_TITLE_MAX; nCntTitle++)
	{ // 使用するテクスチャ数分繰り返す

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, apTextureTitle[nCntTitle], &g_apTextureTitle[nCntTitle]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_2D) * 4 * MAX_TITLE,	// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL
	);

	// グローバル変数を初期化
	g_posTitle     = D3DXVECTOR3(TITLE_POS_X, 0.0f, 0.0f);	// タイトルの絶対座標を初期化
	g_stateTitle   = TITLESTATE_MOVE;						// タイトルの状態を移動に初期化
	g_fAlphaTitle  = TIT_FIRST_ALPHA;						// 背景の透明度を初期化
	g_fChangeTitle = TIT_CHANGE_ALPHA;						// 透明度の変更値を初期化
	g_nTransCount  = 0;										// 自動遷移するまでのカウント

	//グローバル変数（タイトルの選択）の初期化
	for (int nCount = 0; nCount < TITLE_SELECT_TYPE_MAX; nCount++)
	{
		g_aTitleSelect[nCount].pos = D3DXVECTOR3(TITLE_SELECT_POS_X, TITLE_SELECT_POS_Y + (TITLE_SELECT_INTERVAL_Y * nCount), 0.0f);	//選択肢の数分Yをずらす
		g_aTitleSelect[nCount].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//タイトル選択カーソル
		g_titleCursor.type = TITLE_SELECT_TYPE_START;
		g_titleCursor.bOk = false;
	}

	if (g_bTitleSound != true)
	{//タイトルサウンドが使用中じゃない
		g_bTitleSound = false;
	}

	// 遷移するモードを変える
	g_nTransMode = (g_nTransMode + 1) % TRANSITION_TITLE_MAX;

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------------------------------------------------------------------------------------------
	//	タイトルアイコンの初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[0].pos = D3DXVECTOR3(TITLE_POS_X - TITLE_WIDTH, g_posTitle.y - TITLE_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, g_posTitle.y - TITLE_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TITLE_POS_X - TITLE_WIDTH, g_posTitle.y + TITLE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, g_posTitle.y + TITLE_HEIGHT, 0.0f);

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
	//	タイトル選択の初期化
	//------------------------------------------------------------------------------------------------------------------
	for (int nCount = 0; nCount < TITLE_SELECT_TYPE_MAX; nCount++)
	{//選択肢の数分繰り返す

		// 頂点座標を設定
		pVtx[4 + (nCount * 4)].pos = D3DXVECTOR3(g_aTitleSelect[nCount].pos.x - TITLE_SELECT_SIZE_X, g_aTitleSelect[nCount].pos.y - TITLE_SELECT_SIZE_Y, 0.0f);
		pVtx[5 + (nCount * 4)].pos = D3DXVECTOR3(g_aTitleSelect[nCount].pos.x + TITLE_SELECT_SIZE_X, g_aTitleSelect[nCount].pos.y - TITLE_SELECT_SIZE_Y, 0.0f);
		pVtx[6 + (nCount * 4)].pos = D3DXVECTOR3(g_aTitleSelect[nCount].pos.x - TITLE_SELECT_SIZE_X, g_aTitleSelect[nCount].pos.y + TITLE_SELECT_SIZE_Y, 0.0f);
		pVtx[7 + (nCount * 4)].pos = D3DXVECTOR3(g_aTitleSelect[nCount].pos.x + TITLE_SELECT_SIZE_X, g_aTitleSelect[nCount].pos.y + TITLE_SELECT_SIZE_Y, 0.0f);

		// rhw の設定
		pVtx[4 + (nCount * 4)].rhw = 1.0f;
		pVtx[5 + (nCount * 4)].rhw = 1.0f;
		pVtx[6 + (nCount * 4)].rhw = 1.0f;
		pVtx[7 + (nCount * 4)].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[4 + (nCount * 4)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
		pVtx[5 + (nCount * 4)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
		pVtx[6 + (nCount * 4)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
		pVtx[7 + (nCount * 4)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);

		// テクスチャ座標の設定
		pVtx[4 + (nCount * 4)].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[5 + (nCount * 4)].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[6 + (nCount * 4)].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[7 + (nCount * 4)].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();

	//------------------------------------------------------------------------------------------------------------------
	//	3Dマップの初期化
	//------------------------------------------------------------------------------------------------------------------
	// 万能初期化の全体処理（3Dマップ）
	InitAllAroundChunk();

	// アイコンの初期化
	InitIcon();

	// ファイルをロードする全体処理
	LoadFileChunk
	( // 引数
		true,	// 車のカーブ
		true,	// 人間のカーブ
		true,	// ステージ
		false,	// 当たり判定
		true,	// 影
		true,	// オブジェクト
		true	// AI
	);


}

//======================================================================================================================
//	タイトル画面の終了処理
//======================================================================================================================
void UninitTitle(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	タイトルの終了
	//------------------------------------------------------------------------------------------------------------------
	// テクスチャの破棄
	for (int nCntTitle = 0; nCntTitle < TEXTURE_TITLE_MAX; nCntTitle++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTextureTitle[nCntTitle] != NULL)
		{ // 変数 (g_apTextureTitle) がNULLではない場合

			g_apTextureTitle[nCntTitle]->Release();
			g_apTextureTitle[nCntTitle] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{ // 変数 (g_pVtxBuffTitle) がNULLではない場合

		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	//------------------------------------------------------------------------------------------------------------------
	//	3Dマップの終了
	//------------------------------------------------------------------------------------------------------------------
	// 万能終了の全体処理（3Dマップ）
	UninitAllAroundChunk();

	// アイコンの終了
	UninitIcon();

	//------------------------------------------------------------------------------------------------------------------
	//	サウンドの終了
	//------------------------------------------------------------------------------------------------------------------
	if (g_bTitleSound == false)
	{//タイトルのサウンドが使われていない
		// サウンドの停止
		StopSound();
	}
}

//======================================================================================================================
//	タイトル画面の更新処理
//======================================================================================================================
void UpdateTitle(void)
{
	// 遷移までのカウントを加算する
	g_nTransCount++;

	//------------------------------------------------------------------------------------------------------------------
	//	タイトルの更新
	//------------------------------------------------------------------------------------------------------------------
	// ポインタを宣言
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// 演出の更新
	switch (g_stateTitle)
	{ // 状態の確認
	case TITLESTATE_MOVE:	// 移動状態の場合

		// タイトルの絶対座標を移動 (下)
		g_posTitle.y += TITLE_MOVE;

		if (g_posTitle.y >= TITLE_POS_Y)
		{ // タイトルが下がり切った場合

			// y座標を補正
			g_posTitle.y = TITLE_POS_Y;

			// 状態を何もしないに設定
			g_stateTitle = TITLESTATE_NONE;

			//メインBGMの再生
			if (GetSoundType(SOUND_TYPE_MAIN_BGM) == true)
			{
				if (g_bTitleSound == false)
				{//タイトルサウンドを使用していないとき

					//タイトルサウンドを使用
					g_bTitleSound = true;

					// サウンドの再生（タイトルのメインBGM）
					PlaySound(SOUND_LABEL_BGM_TITLE_000);
				}
			}
		}

		// 処理を抜ける
		break;

	case TITLESTATE_NONE:	// 何もしていない状態の場合

		if (GetFade() == FADE_NONE)
		{ // フェードしていない場合

		  //選択モードの移動（上）
			if (GetKeyboardTrigger(DIK_W) == true 
			||	GetJoyKeyTrigger(JOYKEY_UP, 0) == true)
			{
				switch (g_titleCursor.type)
				{
					//スタート
				case TITLE_SELECT_TYPE_START:
					
					//チュートリアルに変更
					g_titleCursor.type = TITLE_SELECT_TYPE_TUTORIAL;

					break;

					//チュートリアル
				case TITLE_SELECT_TYPE_TUTORIAL:

					//ゲームに変更
					g_titleCursor.type = TITLE_SELECT_TYPE_START;

					break;
				}
				
			}

			//選択モードの移動（下）
			if (GetKeyboardTrigger(DIK_S) == true
			|| GetJoyKeyTrigger(JOYKEY_DOWN, 0) == true)
			{
				//カーソルの移動
				switch (g_titleCursor.type)
				{
					//スタート
				case TITLE_SELECT_TYPE_START:
					
					//チュートリアルに変更
					g_titleCursor.type = TITLE_SELECT_TYPE_TUTORIAL;

					break;

					//チュートリアル
				case TITLE_SELECT_TYPE_TUTORIAL:

					//スタートに変更
					g_titleCursor.type = TITLE_SELECT_TYPE_START;

					break;
				}

			}

			if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_SPACE) == true
			||  GetJoyKeyTrigger(JOYKEY_A, 0)  == true || GetJoyKeyTrigger(JOYKEY_B, 0) == true
			||  GetJoyKeyTrigger(JOYKEY_X, 0)  == true || GetJoyKeyTrigger(JOYKEY_Y, 0) == true)
			{ // 決定の操作が行われた場合

				//// サウンドの再生
				//PlaySound(SOUND_LABEL_SE_DECISION_00);	// 決定音00 (システム)

				//タイトルサウンドを使用を停止
				g_bTitleSound = false;

				//選択してるモードに遷移
				switch (g_titleCursor.type)
				{
					//スタート
				case TITLE_SELECT_TYPE_START:
					// モード選択 (ゲーム画面に移行)
					SetFade(MODE_GAME);
					break;
					//チュートリアル
				case TITLE_SELECT_TYPE_TUTORIAL:
					// モード選択 (チュートリアル画面に移行)
					SetFade(MODE_TUTORIAL);
					break;
				}

			}

			if (g_fAlphaTitle >= TIT_ALPHA_HIGH || g_fAlphaTitle <= TIT_ALPHA_LOW)
			{ // 透明度が不透明、または透明になった場合

				// 変動量を反転させる
				g_fChangeTitle *= -1.0f;	// ±を逆転させる
			}

			// 透明度を加算
			g_fAlphaTitle += g_fChangeTitle;

			//選択中のモードを点滅表示（他は通常に）
			switch (g_titleCursor.type)
			{

				//スタート
			case TITLE_SELECT_TYPE_START:
				// 頂点カラーの更新

				pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
				pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
				pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
				pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
				break;

				//チュートリアル
			case TITLE_SELECT_TYPE_TUTORIAL:
				// 頂点カラーの更新
				pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
				pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
				pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
				pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);

				pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,  1.0f);
				pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,  1.0f);
				pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				break;
			}

		}
		else
		{ // フェード中の場合

			// 頂点カラーの更新
			pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, TIT_ALPHA_HIGH);
			pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, TIT_ALPHA_HIGH);
			pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, TIT_ALPHA_HIGH);
			pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, TIT_ALPHA_HIGH);

			pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, TIT_ALPHA_HIGH);
			pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, TIT_ALPHA_HIGH);
			pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, TIT_ALPHA_HIGH);
			pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, TIT_ALPHA_HIGH);
		}

		// 処理を抜ける
		break;
	}

	// 演出のスキップ
	if (g_stateTitle != TITLESTATE_NONE && GetFade() == FADE_NONE)
	{ // タイトルが何もしない状態ではない且つ、フェードしていない場合

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_SPACE) == true
		||  GetJoyKeyTrigger(JOYKEY_A, 0)  == true || GetJoyKeyTrigger(JOYKEY_B, 0) == true
		||  GetJoyKeyTrigger(JOYKEY_X, 0)  == true || GetJoyKeyTrigger(JOYKEY_Y, 0) == true)
		{ // スキップ操作が行われた場合

			// y座標を補正
			g_posTitle.y = TITLE_POS_Y;

			// 状態を何もしないに設定
			g_stateTitle = TITLESTATE_NONE;

			//メインBGMの再生
			if (GetSoundType(SOUND_TYPE_MAIN_BGM) == true)
			{
				if (g_bTitleSound == false)
				{//タイトルサウンドを使用していないとき

					//タイトルサウンドを使用
					g_bTitleSound = true;

					// サウンドの再生（タイトルのメインBGM）
					PlaySound(SOUND_LABEL_BGM_TITLE_000);
				}
			}
		}
	}

	// 頂点座標を更新
	pVtx[0].pos = D3DXVECTOR3(TITLE_POS_X - TITLE_WIDTH, g_posTitle.y - TITLE_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, g_posTitle.y - TITLE_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TITLE_POS_X - TITLE_WIDTH, g_posTitle.y + TITLE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, g_posTitle.y + TITLE_HEIGHT, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();

	//------------------------------------------------------------------------------------------------------------------
	//	3Dマップの更新
	//------------------------------------------------------------------------------------------------------------------
	// 天気の更新処理
	UpdateWeather();

	// カメラの更新処理
	UpdateCamera();

	// ライトの更新処理
	UpdateLight();

	// 車の更新処理
	UpdateCar();

	// 人間の更新処理
	UpdateHuman();

	//　パトカーの更新処理
	UpdatePolice();

	//------------------------------------------------------------------------------------------------------------------
	//	自動遷移
	//------------------------------------------------------------------------------------------------------------------
	if (g_nTransCount >= TRANS_CNT)
	{ // 一定時間経ったら
		if (GetFade() == FADE_NONE)
		{ // フェードしていない場合

			switch (g_nTransMode)
			{ // モードで判断する

			case TRANSITION_TITLE_LOGO:		// ロゴ画面の場合

				//タイトルの状態を遷移に変更
				g_stateTitle = TITLESTATE_FADE;

				// ロゴ画面に遷移
				SetFade(MODE_LOGO);

				// 処理を抜ける
				break;

			case TRANSITION_TITLE_RANKING:	// ランキング画面の場合

				//タイトルの状態を遷移に変更
				g_stateTitle = TITLESTATE_FADE;

				// ランキング画面に遷移
				SetFade(MODE_RANKING);

				// 処理を抜ける
				break;
			}
		}
	}
}

//======================================================================================================================
//	タイトル画面の描画処理
//======================================================================================================================
void DrawTitle(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	3Dマップの描画
	//------------------------------------------------------------------------------------------------------------------
	// 万能描画の全体処理
	DrawAllAroundChunk();

	//------------------------------------------------------------------------------------------------------------------
	//	タイトルの描画
	//------------------------------------------------------------------------------------------------------------------
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTitle = 0; nCntTitle < MAX_TITLE; nCntTitle++)
	{ // 使用するポリゴン数分繰り返す

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureTitle[nCntTitle]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 2);
	}


}

//======================================================================================================================
//	タイトルの状態の取得処理
//======================================================================================================================
TITLESTATE GetTitleState(void)
{
	return g_stateTitle;
}
