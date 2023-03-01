//========================================
//
//プロジェクト画面のメイン処理[LogoBy.cpp]
//Author 小原立暉
//
//========================================
#include "main.h"
#include "Logo.h"
#include "input.h"
#include "Fade.h"

//マクロ定義
#define AUTO_LOGO						(90)				//自動で画面が進む時間
#define LOGO_X							(450.0f)			//ロゴの半径(X軸)
#define LOGO_Y							(250.0f)			//ロゴの半径(Y軸)

//プロジェクト画面のテクスチャ
typedef enum
{
	LOGO_SCREEN = 0,		//背景
	LOGO_PROJECT,			//ロゴ
	LOGO_MAX
}LOGO;

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureLogo[LOGO_MAX] = {};		//テクスチャ(2枚分)へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLogo		 = NULL;	//頂点バッファへのポインタ

int g_nLogoCounter = 0;									//タイトルに移行する間でのカウンター
D3DXVECTOR3 LogoPos;									// ロゴの中心

// テクスチャファイル名
const char *c_apFilenameLogo[LOGO_MAX] = 
{
	"data/TEXTURE/Start_Screen.png",
	"data/TEXTURE/logo000.png",
};

//===============================
//ロゴ画面の初期化処理
//===============================
void InitLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;								//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < LOGO_MAX; nCnt++)
	{
		//テクスチャ1枚目の読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameLogo[nCnt],
			&g_apTextureLogo[nCnt]);
	}

	//カウンターを初期化する
	g_nLogoCounter = 0;

	// ロゴマークの中心の設定
	LogoPos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * LOGO_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLogo,
		NULL);

	VERTEX_2D * pVtx;										//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLogo->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLogo = 0; nCntLogo < LOGO_MAX; nCntLogo++)
	{
		switch (nCntLogo)
		{
		case LOGO_SCREEN:		// 画面

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

			break;				// 抜け出す

		case LOGO_PROJECT:		// ロゴマーク

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(LogoPos.x - LOGO_X, LogoPos.y - LOGO_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(LogoPos.x + LOGO_X, LogoPos.y - LOGO_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(LogoPos.x - LOGO_X, LogoPos.y + LOGO_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(LogoPos.x + LOGO_X, LogoPos.y + LOGO_Y, 0.0f);

			break;				// 抜け出す
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
	g_pVtxBuffLogo->Unlock();
}

//======================================
//ロゴ画面の終了処理
//======================================
void UninitLogo(void)
{
	int nLogoCnt;					//回数の変数を宣言する

	for (nLogoCnt = 0; nLogoCnt < LOGO_MAX; nLogoCnt++)
	{
		//テクスチャの破棄
		if (g_apTextureLogo[nLogoCnt] != NULL)
		{
			g_apTextureLogo[nLogoCnt]->Release();
			g_apTextureLogo[nLogoCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffLogo != NULL)
	{
		g_pVtxBuffLogo->Release();
		g_pVtxBuffLogo = NULL;
	}
}

//=======================================
//ロゴ画面の更新処理
//=======================================
void UpdateLogo(void)
{
	FADE pFade = GetFade();							//フェードの状態を取得する

	//リザルト画面になった瞬間加算し始める
	g_nLogoCounter++;

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_SPACE) == true
		|| GetJoyKeyTrigger(JOYKEY_A, 0) == true || GetJoyKeyTrigger(JOYKEY_B, 0) == true
		|| GetJoyKeyTrigger(JOYKEY_X, 0) == true || GetJoyKeyTrigger(JOYKEY_Y, 0) == true
		|| g_nLogoCounter >= AUTO_LOGO)
	{//一定時間経過した

		if (pFade == FADE_NONE)
		{//フェード処理に移行していないとき

			//モード設定(ゲーム画面に移行)
			SetFade(MODE_TITLE);
		}
	}
}

//=======================================
//ロゴ画面の描画処理
//=======================================
void DrawLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nLogoCnt = 0; nLogoCnt < LOGO_MAX; nLogoCnt++)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0,
			g_pVtxBuffLogo,								//頂点バッファへのポインタ
			0,
			sizeof(VERTEX_2D));							//頂点情報構造体のサイズ

		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureLogo[nLogoCnt]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
			4 * nLogoCnt,									//描画する最初の頂点インデックス
			2);												//描画するプリミティブ数
	}
}