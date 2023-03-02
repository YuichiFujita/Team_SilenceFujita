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
#define WHEEL_RADIUS			(300.0f)		// タイヤの半径
#define RESULT_FINISH_COUNT		(30)			// リザルトが終了するまでのカウント
#define RESULT_SCORE_WIDTH		(70.0f)			// 値の縦幅
#define RESULT_SCORE_HEIGHT		(100.0f)			// 値の横幅
#define RESULT_SCORE_SHIFT		(110.0f)		// 値の横幅
#define ADD_DISP_SCORE			(1000)			// スコアの増加率

#define RESULT_WORD_RADIUS_X	(500.0f)		// 言葉の半径(X軸)
#define RESULT_WORD_RADIUS_Y	(100.0f)		// 言葉の半径(Y軸)

#define TIT_CHANGE_CNT			(600)			// 自動遷移のカウント

//**********************************************************************************************************************
// リザルト画面のテクスチャ
//**********************************************************************************************************************
typedef enum
{
	RSL_WHEEL = 0,			// タイヤ
	RSL_GAMECLEAR,			// ゲームクリア
	RSL_GAMEOVER,			// ゲームオーバー
	RSL_MAX					// この列挙型の総数
}RSLTEXTURE;

//**********************************************************************************************************************
//	構造体定義 (WHEEL)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	float		fRot;		// 向き
	float		fRotMove;	// 向きの移動量
	float		fAngle;		// 方向
	float		fLength;	// 長さ
}WHEEL;

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
void WheelUpdateRsl(void);			// タイヤの更新処理
void RslWordUpdateRsl(void);		// 言葉の更新処理

//**********************************************************************************************************************
// グローバル変数宣言
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureResult[RSL_MAX] = {};			// テクスチャ(2枚分)へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultWheel = NULL;		// 画面の頂点バッファへのポインタ(タイヤ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultWord = NULL;		// 画面の頂点バッファへのポインタ(タイヤ)
int g_nFadeTitleCounter;									// タイトル遷移カウンター
int g_nResultCounter;										// リザルトカウンター
WHEEL g_Wheel;												// タイヤの情報
int g_nResultScore;											// リザルト画面のスコア
int g_nDispRslScore;										// 表示用のスコア
bool g_bRslFade;											// リザルトから遷移するかどうか
D3DLIGHT9 g_RslLight;										// リザルトのライト
RESULTSTATE g_ResultState;									// ゲーム終了時の状態
RSLWORD g_RslWord;											// 文字の情報

//**********************************************************************************************************************
// テクスチャファイル名
//**********************************************************************************************************************
const char *c_apFilenameResult[RSL_MAX] =
{
	"data/TEXTURE/ResultWheel.png",							// タイヤ
	"data/TEXTURE/GAMECLEAR.png",							// ゲームクリア
	"data/TEXTURE/GAMEOVER.png",							// ゲームオーバー
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

	for (int nCntTex = 0; nCntTex < RSL_MAX; nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameResult[nCntTex],
			&g_apTextureResult[nCntTex]);
	}

	{ // 情報の初期化
		// タイトル遷移カウンターを初期化する
		g_nFadeTitleCounter = TIT_CHANGE_CNT;

		// リザルトカウンターを初期化する
		g_nResultCounter = 0;

		// タイヤの情報を初期化
		g_Wheel.pos = D3DXVECTOR3(400.0f, 300.0f, 0.0f);		// 位置
		g_Wheel.fRot = 0.0f;									// 向き
		g_Wheel.fLength = sqrtf(WHEEL_RADIUS * WHEEL_RADIUS + WHEEL_RADIUS * WHEEL_RADIUS) * 0.5f;			// 長さ
		g_Wheel.fAngle = atan2f(WHEEL_RADIUS, WHEEL_RADIUS);	// 方向
		g_Wheel.fRotMove = 0.2f;								// 向きの移動量

		// 言葉の情報を初期化
		g_RslWord.pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 150.0f, 0.0f);		// 中心
		g_RslWord.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量

		g_nDispRslScore = 0;				// 表示用のスコア
		g_bRslFade = false;					// リザルトから遷移するかどうか

		g_ResultState = GetResultState();	// ゲームの状態
	}

	{ // タイヤ
		//頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffResultWheel,
			NULL);

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffResultWheel->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos.x = g_Wheel.pos.x + sinf(g_Wheel.fRot - D3DX_PI + g_Wheel.fAngle) * g_Wheel.fLength;
		pVtx[0].pos.y = g_Wheel.pos.y + cosf(g_Wheel.fRot - D3DX_PI + g_Wheel.fAngle) * g_Wheel.fLength;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = g_Wheel.pos.x + sinf(g_Wheel.fRot + D3DX_PI - g_Wheel.fAngle) * g_Wheel.fLength;
		pVtx[1].pos.y = g_Wheel.pos.y + cosf(g_Wheel.fRot + D3DX_PI - g_Wheel.fAngle) * g_Wheel.fLength;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = g_Wheel.pos.x + sinf(g_Wheel.fRot - g_Wheel.fAngle) * g_Wheel.fLength;
		pVtx[2].pos.y = g_Wheel.pos.y + cosf(g_Wheel.fRot - g_Wheel.fAngle) * g_Wheel.fLength;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = g_Wheel.pos.x + sinf(g_Wheel.fRot + g_Wheel.fAngle) * g_Wheel.fLength;
		pVtx[3].pos.y = g_Wheel.pos.y + cosf(g_Wheel.fRot + g_Wheel.fAngle) * g_Wheel.fLength;
		pVtx[3].pos.z = 0.0f;

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
		g_pVtxBuffResultWheel->Unlock();
	}

	{ // 言葉
		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffResultWord,
			NULL);

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffResultWord->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_RslWord.pos.x - RESULT_WORD_RADIUS_X, g_RslWord.pos.y - RESULT_WORD_RADIUS_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_RslWord.pos.x + RESULT_WORD_RADIUS_X, g_RslWord.pos.y - RESULT_WORD_RADIUS_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_RslWord.pos.x - RESULT_WORD_RADIUS_X, g_RslWord.pos.y + RESULT_WORD_RADIUS_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_RslWord.pos.x + RESULT_WORD_RADIUS_X, g_RslWord.pos.y + RESULT_WORD_RADIUS_Y, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		if (g_ResultState == RESULTSTATE_CLEAR)
		{ // ゲームクリア状態の場合

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f);
		}
		else if (g_ResultState == RESULTSTATE_TIMEOVER
			|| g_ResultState == RESULTSTATE_LIFEOVER)
		{ // ゲームオーバー状態の場合

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.7f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.7f, 1.0f);

			// スコアの加算処理
			AddScore(-(int)(GetScore() * 0.5f));
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//頂点バッファをアンロックする
		g_pVtxBuffResultWord->Unlock();
	}

	g_nResultScore = GetScore();		// リザルトのスコア

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
			true	// AI
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
			false	// AI
		);
	}

	////サウンドの再生（ランキング終了まで）※AnarchyCarsBGM
	//PlaySound(SOUND_LABEL_BGM_RESULT_000);

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

	for (int nReslCount = 0; nReslCount < RSL_MAX; nReslCount++)
	{//破棄
		//テクスチャの破棄
		if (g_apTextureResult[nReslCount] != NULL)
		{
			g_apTextureResult[nReslCount]->Release();
			g_apTextureResult[nReslCount] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffResultWheel != NULL)
	{
		g_pVtxBuffResultWheel->Release();
		g_pVtxBuffResultWheel = NULL;
	}

	if (g_pVtxBuffResultWord != NULL)
	{
		g_pVtxBuffResultWord->Release();
		g_pVtxBuffResultWord = NULL;
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

	// タイヤの更新処理
	WheelUpdateRsl();

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
		g_nDispRslScore += ADD_DISP_SCORE;
	}
	else
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

	//{ // タイヤ
	//	//頂点バッファをデータストリームに設定
	//	pDevice->SetStreamSource(0,
	//		g_pVtxBuffResultWheel,						//頂点バッファへのポインタ
	//		0,
	//		sizeof(VERTEX_2D));							//頂点情報構造体のサイズ

	//	//テクスチャの設定
	//	pDevice->SetTexture(0, g_apTextureResult[RSL_WHEEL]);

	//	//ポリゴンの描画
	//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
	//		0,											//描画する最初の頂点インデックス
	//		2);											//描画するプリミティブ数
	//}

	{ // 言葉
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0,
			g_pVtxBuffResultWord,						//頂点バッファへのポインタ
			0,
			sizeof(VERTEX_2D));							//頂点情報構造体のサイズ


		if (g_ResultState == RESULTSTATE_CLEAR)
		{ // ゲームクリア状態の場合
	
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureResult[RSL_GAMECLEAR]);
		}
		else if (g_ResultState == RESULTSTATE_TIMEOVER
			 ||  g_ResultState == RESULTSTATE_LIFEOVER)
		{ // ゲームオーバー状態の場合
	
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureResult[RSL_GAMEOVER]);
		}

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
		D3DXVECTOR3
		( // 引数
			1100.0f,				// 位置 (x)
			500.0f,					// 位置 (y)
			0.0f					// 位置 (z)
		),
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

//=======================================
// タイヤの更新処理
//=======================================
void WheelUpdateRsl(void)
{
	VERTEX_2D * pVtx;										//頂点情報へのポインタ

	// 向きを移動させる
	g_Wheel.fRot += g_Wheel.fRotMove;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultWheel->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = g_Wheel.pos.x + sinf(g_Wheel.fRot - D3DX_PI + g_Wheel.fAngle) * g_Wheel.fLength;
	pVtx[0].pos.y = g_Wheel.pos.y + cosf(g_Wheel.fRot - D3DX_PI + g_Wheel.fAngle) * g_Wheel.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_Wheel.pos.x + sinf(g_Wheel.fRot + D3DX_PI - g_Wheel.fAngle) * g_Wheel.fLength;
	pVtx[1].pos.y = g_Wheel.pos.y + cosf(g_Wheel.fRot + D3DX_PI - g_Wheel.fAngle) * g_Wheel.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_Wheel.pos.x + sinf(g_Wheel.fRot - g_Wheel.fAngle) * g_Wheel.fLength;
	pVtx[2].pos.y = g_Wheel.pos.y + cosf(g_Wheel.fRot - g_Wheel.fAngle) * g_Wheel.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_Wheel.pos.x + sinf(g_Wheel.fRot + g_Wheel.fAngle) * g_Wheel.fLength;
	pVtx[3].pos.y = g_Wheel.pos.y + cosf(g_Wheel.fRot + g_Wheel.fAngle) * g_Wheel.fLength;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする
	g_pVtxBuffResultWheel->Unlock();
}

//=======================================
// 言葉の更新処理
//=======================================
void RslWordUpdateRsl(void)
{
	VERTEX_2D * pVtx;										//頂点情報へのポインタ

	//g_RslWord.pos.x += 

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultWord->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_RslWord.pos.x - RESULT_WORD_RADIUS_X, g_RslWord.pos.y - RESULT_WORD_RADIUS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_RslWord.pos.x + RESULT_WORD_RADIUS_X, g_RslWord.pos.y - RESULT_WORD_RADIUS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_RslWord.pos.x - RESULT_WORD_RADIUS_X, g_RslWord.pos.y + RESULT_WORD_RADIUS_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_RslWord.pos.x + RESULT_WORD_RADIUS_X, g_RslWord.pos.y + RESULT_WORD_RADIUS_Y, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffResultWord->Unlock();
}