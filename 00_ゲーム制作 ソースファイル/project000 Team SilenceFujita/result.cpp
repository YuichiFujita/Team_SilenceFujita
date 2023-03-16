//========================================
//
//リザルト画面のメイン処理[Result.cpp]
//Author 小原立暉
//
//========================================
#include "main.h"
#include "game.h"
#include "sound.h"
#include "result.h"
#include "input.h"
#include "Fade.h"
#include "calculation.h"

#include "value.h"
#include "camera.h"
#include "Car.h"
#include "Human.h"
#include "light.h"
#include "object.h"
#include "Police.h"
#include "score.h"
#include "shadow.h"
#include "weather.h"

//**********************************************************************************************************************
// マクロ定義
//**********************************************************************************************************************
#define RESULT_FINISH_COUNT		(30)			// リザルトが終了するまでのカウント
#define RESULT_SCORE_WIDTH		(60.0f)			// 値の縦幅
#define RESULT_SCORE_HEIGHT		(60.0f)			// 値の横幅
#define RESULT_SCORE_SHIFT		(90.0f)			// 値のずらす幅
#define RESULT_SCORE_POS		(D3DXVECTOR3(1000.0f, 560.0f, 0.0f))
#define RESULT_SCORE_BACK_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 550.0f, 0.0f))
#define RESULT_SCORE_BACK_SIZE	(D3DXVECTOR3(540.0f, 120.0f, 0.0f))

#define RESULT_WORD_RADIUS		(D3DXVECTOR3(540.0f, 75.0f, 0.0f))						// 言葉の半径
#define RESULT_WORD_BACK_RADIUS	(D3DXVECTOR3(640.0f, 160.0f, 0.0f))						// 言葉の背景の半径
#define RESULT_WORD_POS			(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 180.0f, 0.0f))		// 言葉の位置

#define RANK_CHANGE_CNT			(450)			// 自動遷移のカウント
#define RANK_AFTERGLOW_CNT		(60)			// スコア完成後の余韻のカウント

//**********************************************************************************************************************
// リザルト画面のテクスチャ
//**********************************************************************************************************************
typedef enum
{
	RSL_TEX_GAMECLEAR = 0,		// ゲームクリア
	RSL_TEX_GAMEOVER,			// ゲームオーバー
	RSL_TEX_WORD_BACK,			// 言葉の背景
	RSL_TEX_SCORE_BACK,			// スコアの背景
	RSL_TEX_MAX					// この列挙型の総数
}RSLTEXTURE;

//**********************************************************************************************************************
// リザルト表示のポリゴン数
//**********************************************************************************************************************
typedef enum
{
	RSL_POLI_BACK = 0,		// 背景のポリゴン
	RSL_POLI_WORD,			// 言葉のポリゴン
	RSL_POLI_MAX			// この列挙型の総数
}RSLPOLIGON;

//**********************************************************************************************************************
//	構造体定義 (WORD)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 中心
	D3DXVECTOR3 move;		// 移動量
	D3DXVECTOR2 radius;		// 半径
}RSLWORD;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************

//**********************************************************************************************************************
// グローバル変数宣言
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureResult[RSL_TEX_MAX] = {};		// テクスチャ(2枚分)へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultWord = NULL;		// 画面の頂点バッファへのポインタ(リザルト表示関係)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultNumBack = NULL;		// 画面の頂点バッファへのポインタ(数値の背景)

int g_nFadeTitleCounter;									// タイトル遷移カウンター
int g_nResultCounter;										// リザルトカウンター

int g_nResultScore;											// リザルト画面のスコア
int g_nDispRslScore;										// 表示用のスコア
int g_nAddDispScore;										// 表示用のスコアの加算倍率

bool g_bRslFade;											// リザルトから遷移するかどうか

D3DLIGHT9 g_RslLight;										// リザルトのライト
RESULTSTATE g_ResultState;									// ゲーム終了時の状態
RSLWORD g_RslWord;											// 文字の情報

//**********************************************************************************************************************
// テクスチャファイル名
//**********************************************************************************************************************
const char *c_apFilenameResult[RSL_TEX_MAX] =
{
	"data/TEXTURE/GAMECLEAR.tga",							// ゲームクリア
	"data/TEXTURE/GAMEOVER.tga",							// ゲームオーバー
	"data/TEXTURE/ui008.tga",								// 言葉の背景
	"data/TEXTURE/ui009.tga",								// スコアの背景
};

//===============================
//リザルト画面の初期化処理
//===============================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;								//デバイスへのポインタ
	VERTEX_2D * pVtx;										//頂点情報へのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCntTex = 0; nCntTex < RSL_TEX_MAX; nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameResult[nCntTex],
			&g_apTextureResult[nCntTex]);
	}

	{ // 情報の初期化

		// タイトル遷移カウンターを初期化する
		g_nFadeTitleCounter = RANK_CHANGE_CNT;

		// リザルトカウンターを初期化する
		g_nResultCounter = 0;

		// 言葉の情報を初期化
		g_RslWord.pos = RESULT_WORD_POS;					// 中心
		g_RslWord.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量

		g_bRslFade = false;					// リザルトから遷移するかどうか

		g_ResultState = GetResultState();	// ゲームの状態
	}

	if (g_ResultState == RESULTSTATE_TIMEOVER
	 || g_ResultState == RESULTSTATE_LIFEOVER)
	{ // タイムオーバー・ライフオーバー状態だった場合

		// スコアの加算処理
		AddScore(-(int)(GetScore() * 0.5f));
	}

	{ // リザルト表示系(ResultWord)

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * RSL_POLI_MAX,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffResultWord,
			NULL);

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffResultWord->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtx = 0; nCntVtx < RSL_POLI_MAX; nCntVtx++)
		{
			switch (nCntVtx)
			{
			case RSL_POLI_WORD:		// 言葉

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_RslWord.pos.x - RESULT_WORD_RADIUS.x, g_RslWord.pos.y - RESULT_WORD_RADIUS.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_RslWord.pos.x + RESULT_WORD_RADIUS.x, g_RslWord.pos.y - RESULT_WORD_RADIUS.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_RslWord.pos.x - RESULT_WORD_RADIUS.x, g_RslWord.pos.y + RESULT_WORD_RADIUS.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_RslWord.pos.x + RESULT_WORD_RADIUS.x, g_RslWord.pos.y + RESULT_WORD_RADIUS.y, 0.0f);

				break;

			case RSL_POLI_BACK:		// 背景

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_RslWord.pos.x - RESULT_WORD_BACK_RADIUS.x, g_RslWord.pos.y - RESULT_WORD_BACK_RADIUS.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_RslWord.pos.x + RESULT_WORD_BACK_RADIUS.x, g_RslWord.pos.y - RESULT_WORD_BACK_RADIUS.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_RslWord.pos.x - RESULT_WORD_BACK_RADIUS.x, g_RslWord.pos.y + RESULT_WORD_BACK_RADIUS.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_RslWord.pos.x + RESULT_WORD_BACK_RADIUS.x, g_RslWord.pos.y + RESULT_WORD_BACK_RADIUS.y, 0.0f);

				break;
			}

			//rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx += 4;			// 頂点データを4つ分進める
		}

		//頂点バッファをアンロックする
		g_pVtxBuffResultWord->Unlock();
	}

	{ // 数値の背景(NumBack)

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffResultNumBack,
			NULL);

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffResultNumBack->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(RESULT_SCORE_BACK_POS.x - RESULT_SCORE_BACK_SIZE.x, RESULT_SCORE_BACK_POS.y - RESULT_SCORE_BACK_SIZE.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(RESULT_SCORE_BACK_POS.x + RESULT_SCORE_BACK_SIZE.x, RESULT_SCORE_BACK_POS.y - RESULT_SCORE_BACK_SIZE.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(RESULT_SCORE_BACK_POS.x - RESULT_SCORE_BACK_SIZE.x, RESULT_SCORE_BACK_POS.y + RESULT_SCORE_BACK_SIZE.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(RESULT_SCORE_BACK_POS.x + RESULT_SCORE_BACK_SIZE.x, RESULT_SCORE_BACK_POS.y + RESULT_SCORE_BACK_SIZE.y, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//頂点バッファをアンロックする
		g_pVtxBuffResultNumBack->Unlock();
	}

	// リザルトの初期化全体処理
	InitResultChunk();

	if (g_ResultState == RESULTSTATE_CLEAR)
	{ // ゲームクリア状態の場合

		// ファイルをロードする全体処理
		LoadFileChunk
		( // 引数
			true,	// 車のカーブ
			true,	// 人間のカーブ
			true,	// ステージ
			false,	// 当たり判定
			true,	// 影
			true,	// オブジェクト
			true,	// AI
			false	// アイコン
		);
	}
	else if (g_ResultState == RESULTSTATE_TIMEOVER
		|| g_ResultState == RESULTSTATE_LIFEOVER)
	{ // ゲームオーバー状態の場合

		// ファイルをロードする全体処理
		LoadFileChunk
		( // 引数
			false,	// 車のカーブ
			false,	// 人間のカーブ
			true,	// ステージ
			false,	// 当たり判定
			true,	// 影
			true,	// オブジェクト
			false,	// AI
			false	// アイコン
		);
	}

	//メインBGMの再生
	if (GetSoundType(SOUND_TYPE_MAIN_BGM) == true)
	{
		//サウンドの再生（ランキング終了まで）
		PlaySound(SOUND_LABEL_BGM_RESULT_000);
	}

	// スコア系の初期化処理
	g_nResultScore = GetScore();		// リザルトのスコア
	g_nAddDispScore = g_nResultScore / (RANK_CHANGE_CNT - RANK_AFTERGLOW_CNT);		// リザルトのスコアの加算倍率
	g_nDispRslScore = 0;				// 表示用のスコア
}

//======================================
//リザルト画面の終了処理
//======================================
void UninitResult(void)
{
	////サウンドの停止
	//StopSound();

	// リザルトの終了全体処理
	UninitResultChunk();

	for (int nReslCount = 0; nReslCount < RSL_TEX_MAX; nReslCount++)
	{//破棄
		//テクスチャの破棄
		if (g_apTextureResult[nReslCount] != NULL)
		{
			g_apTextureResult[nReslCount]->Release();
			g_apTextureResult[nReslCount] = NULL;
		}
	}

	{ // バッファの破棄

		// バッファの破棄(ResultWord)
		if (g_pVtxBuffResultWord != NULL)
		{
			g_pVtxBuffResultWord->Release();
			g_pVtxBuffResultWord = NULL;
		}

		// バッファの破棄(NumBack)
		if (g_pVtxBuffResultNumBack != NULL)
		{
			g_pVtxBuffResultNumBack->Release();
			g_pVtxBuffResultNumBack = NULL;
		}
	}
}

//=======================================
//リザルト画面の更新処理
//=======================================
void UpdateResult(void)
{
	// ポインタを宣言
	FADE pFade = GetFade();	//フェードの状態を取得する

	// 天気の設定処理
	SetWeather();

	// 天気の更新処理
	UpdateWeather();

	// カメラの更新処理
	UpdateCamera();

	// 車の更新処理
	UpdateCar();

	// オブジェクトの更新処理
	UpdateObject();

	// 警察の更新処理
	UpdatePolice();

	// 人間の更新処理
	UpdateHuman();

	// ライトの更新処理
	UpdateLight();

	// 影の更新処理
	UpdateShadow();

	if (g_bRslFade == true)
	{ // フェードしない場合

		// リザルト画面になった瞬間加算し始める
		g_nResultCounter++;
	}

	if (g_nDispRslScore < g_nResultScore)
	{ // 表示スコアがほんとのスコアになるまで

		// 表示スコアを加算する
		g_nDispRslScore += g_nAddDispScore;
	}

	if (g_nDispRslScore >= g_nResultScore)
	{ // 表示スコアが本当のスコアを超えた場合

		// スコアを固定する
		g_nDispRslScore = g_nResultScore;
	}

	// タイマーを加算
	g_nFadeTitleCounter--;

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_SPACE) == true
	||  GetJoyKeyTrigger(JOYKEY_A, 0)  == true || GetJoyKeyTrigger(JOYKEY_B, 0) == true
	||  GetJoyKeyTrigger(JOYKEY_X, 0)  == true || GetJoyKeyTrigger(JOYKEY_Y, 0) == true
	||  g_nFadeTitleCounter <= 0)
	{ // 決定の操作が行われた場合 (もしくは時間経過)

		// フェードする
		g_bRslFade = true;

		// スコアを表示する
		g_nDispRslScore = g_nResultScore;

		if (g_nResultCounter >= RESULT_FINISH_COUNT)
		{ // カウントが一定数に達した場合

			if (pFade == FADE_NONE)
			{ // フェードしていない場合

				// タイトルに移行
				SetFade(MODE_RANKING);
			}
		}
	}
}

//=======================================
//リザルト画面の描画処理
//=======================================
void DrawResult(void)
{
	// リザルトの描画全体処理
	DrawResultChunk();

	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	{ // リザルト表示系(ResultWord)

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0,
			g_pVtxBuffResultWord,						//頂点バッファへのポインタ
			0,
			sizeof(VERTEX_2D));							//頂点情報構造体のサイズ

		for (int nCntDraw = 0; nCntDraw < RSL_POLI_MAX; nCntDraw++)
		{
			switch (nCntDraw)
			{
			case RSL_POLI_BACK:			// 背景

				// テクスチャの設定
				pDevice->SetTexture(0, g_apTextureResult[RSL_TEX_WORD_BACK]);

				break;

			case RSL_POLI_WORD:			// 言葉

				if (g_ResultState == RESULTSTATE_CLEAR)
				{ // ゲームクリア状態の場合

					// テクスチャの設定
					pDevice->SetTexture(0, g_apTextureResult[RSL_TEX_GAMECLEAR]);
				}
				else if (g_ResultState == RESULTSTATE_TIMEOVER
					|| g_ResultState == RESULTSTATE_LIFEOVER)
				{ // ゲームオーバー状態の場合

					// テクスチャの設定
					pDevice->SetTexture(0, g_apTextureResult[RSL_TEX_GAMEOVER]);
				}

				break;
			}

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				4 * nCntDraw,							//描画する最初の頂点インデックス
				2);										//描画するプリミティブ数
		}
	}

	{ // スコアの背景(NumBack)

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0,
			g_pVtxBuffResultNumBack,					//頂点バッファへのポインタ
			0,
			sizeof(VERTEX_2D));							//頂点情報構造体のサイズ

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureResult[RSL_TEX_SCORE_BACK]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,											//描画する最初の頂点インデックス
			2);											//描画するプリミティブ数
	}

	//------------------------------------------------------------------------------------------------------------------
	//	数値の描画
	//------------------------------------------------------------------------------------------------------------------
	// 数値の設定
	SetValue
	( // 引数
		RESULT_SCORE_POS,			// 位置
		g_nDispRslScore,			// 値
		VAL_MAX_SCORE,				// 値の最大値
		RESULT_SCORE_WIDTH,			// 横幅
		RESULT_SCORE_HEIGHT,		// 縦幅
		RESULT_SCORE_SHIFT,			// 数値間の幅
		1.0f						// α値
	);

	// 数値の描画
	DrawValue(VAL_SCO_DIGIT, VALUETYPE_NORMAL);
}