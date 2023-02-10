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
#include "light.h"
#include "score.h"
#include "weather.h"

//マクロ定義
#define WHEEL_RADIUS		(300.0f)		// タイヤの半径
#define RESULT_FINISH_COUNT	(30)			// リザルトが終了するまでのカウント
#define RESULT_SCORE_WIDTH	(20.0f)			// 値の縦幅
#define RESULT_SCORE_HEIGHT	(30.0f)			// 値の横幅
#define RESULT_SCORE_SHIFT	(50.0f)			// 値の横幅
#define ADD_DISP_SCORE		(1000)			// スコアの増加率

//ランキング画面のテクスチャ
typedef enum
{
	RSL_WHEEL = 0,			// タイヤ
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

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureResult[RSL_MAX] = {};			// テクスチャ(2枚分)へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultWheel = NULL;		// 画面の頂点バッファへのポインタ(タイヤ)
int g_nResultCounter;										// リザルトカウンター
WHEEL g_Wheel;												// タイヤの情報
int g_nResultScore;											// リザルト画面のスコア
int g_nDispRslScore;										// 表示用のスコア
bool g_bRslFade;											// リザルトから遷移するかどうか
D3DLIGHT9 g_RslLight;										// リザルトのライト

//テクスチャファイル名
const char *c_apFilenameResult[RSL_MAX] =
{
	"data/TEXTURE/ResultWheel.png",							// タイヤ
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
		// リザルトカウンターを初期化する
		g_nResultCounter = 0;

		g_Wheel.pos = D3DXVECTOR3(400.0f, 300.0f, 0.0f);		// 位置
		g_Wheel.fRot = 0.0f;									// 向き
		g_Wheel.fLength = sqrtf(WHEEL_RADIUS * WHEEL_RADIUS + WHEEL_RADIUS * WHEEL_RADIUS) * 0.5f;			// 長さ
		g_Wheel.fAngle = atan2f(WHEEL_RADIUS, WHEEL_RADIUS);	// 方向
		g_Wheel.fRotMove = 0.2f;								// 向きの移動量

		g_nResultScore = 30000;				// リザルトのスコア
		g_nDispRslScore = 0;				// 表示用のスコア
		g_bRslFade = false;					// リザルトから遷移するかどうか
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

	// リザルトの初期化全体処理
	InitResultChunk();

	// ファイルをロードする全体処理
	LoadFileChunk
	(
		false,
		false,
		true,
		true,
		false
	);
}

//======================================
//リザルト画面の終了処理
//======================================
void UninitResult(void)
{
	//サウンドの停止
	StopSound();

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
}

//=======================================
//リザルト画面の更新処理
//=======================================
void UpdateResult(void)
{
	FADE pFade = GetFade();									//フェードの状態を取得する
	VERTEX_2D * pVtx;										//頂点情報へのポインタ

	// 天気の設定処理
	SetWeather();

	// 天気の更新処理
	UpdateWeather();

	// カメラの更新処理
	UpdateCamera();

	// ライトの更新処理
	UpdateLight();

	if (g_bRslFade == true)
	{ // フェードしない場合
		// リザルト画面になった瞬間加算し始める
		g_nResultCounter++;
	}

	if (g_nDispRslScore <= g_nResultScore)
	{ // 表示スコアがほんとのスコアになるまで
		// 表示スコアを加算する
		g_nDispRslScore += ADD_DISP_SCORE;
	}

	if (g_nDispRslScore > g_nResultScore)
	{ // 表示スコアが本当のスコアを超えた場合
		// スコアを固定する
		g_nDispRslScore = g_nResultScore;
	}

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

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_SPACE) == true
		|| GetJoyKeyTrigger(JOYKEY_A, 0) == true || GetJoyKeyTrigger(JOYKEY_B, 0) == true
		|| GetJoyKeyTrigger(JOYKEY_X, 0) == true || GetJoyKeyTrigger(JOYKEY_Y, 0) == true)
	{ // 決定の操作が行われた場合
		// フェードする
		g_bRslFade = true;

		// スコアを表示する
		g_nDispRslScore = g_nResultScore;

		if (g_nResultCounter >= RESULT_FINISH_COUNT)
		{ // カウントが一定数に達した場合
			if (pFade == FADE_NONE)
			{ // フェードしていない場合

				// タイトルに移行
				SetFade(MODE_TITLE);
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

	//------------------------------------------------------------------------------------------------------------------
	//	数値の描画
	//------------------------------------------------------------------------------------------------------------------
	// 数値の設定
	SetValue
	( // 引数
		D3DXVECTOR3
		( // 引数
			SCREEN_WIDTH * 0.5f,	// 位置 (x)
			SCREEN_HEIGHT * 0.5f,	// 位置 (y)
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