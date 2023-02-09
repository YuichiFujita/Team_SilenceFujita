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

//マクロ定義
#define NUM_SCOREPOLY				(9)						// スコアのポリゴン数
#define NUM_RSL_PLACE				(8)						// スコアの桁
#define DESTROY_NUMBER				(130)					// デストロイヤーボーナスを取るのに必要な数値
#define NUMBER_POS_X				(480.0f)				// 数値の中心(X軸)
#define NUMBER_POS_Y				(470.0f)				// 数値の中心(Y軸)
#define INIT_NUMBER					(0)						// 番号の初期化の数値
#define NODAMAGE_BONUS_COUNT		(0)						// ノーダメージボーナスの条件の数値
#define ND_SETDASCORE_POS_X			(360.0f)				// ノーダメージの得点表示の座標(X軸)
#define ND_SETDASCORE_POS_Y			(-25.0f)				// ノーダメージの得点表示の座標(Y軸)
#define NOPREVENT_BONUS_COUNT		(0)						// ノー弾かれボーナスの条件の数値
#define NP_SETDASCORE_POS_X			(1075.0f)				// 弾かれなかったボーナスの得点表示の座標(X軸)
#define NP_SETDASCORE_POS_Y			(-25.0f)				// 弾かれなかったボーナスの得点表示の座標(Y軸)
#define DEST_SETDASCORE_POS_X		(360.0f)				// デストロイボーナスの得点表示の座標(X軸)
#define DEST_SETDASCORE_POS_Y		(55.0f)					// デストロイボーナスの得点表示の座標(Y軸)
#define PERFECT_SETDASCORE_POS_X	(1075.0f)				// パーフェクトボーナスの得点表示の座標(X軸)
#define PERFECT_SETDASCORE_POS_Y	(55.0f)					// パーフェクトボーナスの得点表示の座標(Y軸)
#define ADD_SCORE					(10000)					// 追加されていくスコア
#define SOUND_ADD_SCORE_COUNT		(5)						// サウンドが鳴るカウント
#define SET_RANKING_COUNT			(300)					// ランキング画面に移行するカウント
#define NUMBER_TEXTURE_PATTERN		(0.1f)					// 数値のテクスチャのパターン

//ランキング画面のテクスチャ
typedef enum
{
	RSL_SCREEN = 0,			//背景
	RSL_MAX
}RSLTEXTURE;

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureResult[RSL_MAX] = {};			// テクスチャ(2枚分)へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;			// 画面の頂点バッファへのポインタ
int g_nResultCounter;										// リザルトカウンター

//テクスチャファイル名
const char *c_apFilenameResult[RSL_MAX] =
{
	"data/TEXTURE/Forest.jpg",						//背景
};

//===============================
//リザルト画面の初期化処理
//===============================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;								//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCntTex = 0; nCntTex < RSL_MAX; nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameResult[nCntTex],
			&g_apTextureResult[nCntTex]);
	}

	// リザルトカウンターを初期化する
	g_nResultCounter = 0;

	{//背景
		//頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffResult,
			NULL);

		VERTEX_2D * pVtx;										//頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
		g_pVtxBuffResult->Unlock();
	}
}

//======================================
//リザルト画面の終了処理
//======================================
void UninitResult(void)
{
	//サウンドの停止
	StopSound();

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
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//=======================================
//リザルト画面の更新処理
//=======================================
void UpdateResult(void)
{
	FADE pFade = GetFade();									//フェードの状態を取得する

	//リザルト画面になった瞬間加算し始める
	g_nResultCounter++;

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_SPACE) == true
		|| GetJoyKeyTrigger(JOYKEY_A, 0) == true || GetJoyKeyTrigger(JOYKEY_B, 0) == true
		|| GetJoyKeyTrigger(JOYKEY_X, 0) == true || GetJoyKeyTrigger(JOYKEY_Y, 0) == true
		|| g_nResultCounter >= SET_RANKING_COUNT)
	{ // 決定の操作が行われた場合

		if (pFade == FADE_NONE)
		{//フェードしていない場合

			//タイトルに移行
			SetFade(MODE_TITLE);
		}
	}
}

//=======================================
//リザルト画面の描画処理
//=======================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	{//背景
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0,
			g_pVtxBuffResult,							//頂点バッファへのポインタ
			0,
			sizeof(VERTEX_2D));							//頂点情報構造体のサイズ

		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureResult[RSL_SCREEN]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,											//描画する最初の頂点インデックス
			2);											//描画するプリミティブ数
	}
}