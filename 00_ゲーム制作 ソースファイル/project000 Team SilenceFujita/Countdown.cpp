//=======================================
//
//カウントダウン処理[Countdown.cpp]
//Author 小原立暉
//
//=======================================
#include"main.h"
#include"Countdown.h"
#include "escape.h"

//マクロ定義
#define COUNTDOWN_APPEAR_CNT	(100)											// カウントダウンの出現からの遷移カウント
#define COUNTDOWN_LENGTH_CNT	(40)											// カウントダウンの出現からの遷移カウント

#define COUNTDOWN_RADIUS		(D3DXVECTOR3(600.0f, 100.0f, 0.0f))														// カウントダウン半径
#define COUNTDOWN_INIT_RADIUS	(D3DXVECTOR3(0.0f, 100.0f, 0.0f))														// カウントダウン半径
#define COUNTDOWN_POS			(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - (COUNTDOWN_RADIUS.x * 0.5f), SCREEN_HEIGHT * 0.5f, 0.0f))		// カウントダウンの位置
#define COUNTDOWN_LENGTH_MOVE	(COUNTDOWN_RADIUS.x / COUNTDOWN_LENGTH_CNT)		// カウントダウンの長さの移動量
#define COUNTDOWN_INIT_TEXU		(0.0f)											// カウントダウンのテクスチャの初期値
#define COUNTDOWN_INIT_ALPHA	(0.0f)											// カウントダウンの透明度の初期値
#define COUNTDOWN_ALPHA			(0.5f)											// カウントダウンの透明度
#define COUNTDOWN_TEXTURE		"data/TEXTURE/Countdown.png"					// カウントダウンのテクスチャ

#define COUNTDOWN_ALPHA_SUB		(0.05f)											// カウントダウンの透明度の減少量

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureCountDown = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCountDown = NULL;		//頂点バッファへのポインタ
CountDown g_CountDown;									//カウントダウンの情報

//==========================================
//カウントダウンの初期化処理
//==========================================
void InitCountDown(void)
{
	LPDIRECT3DDEVICE9 pDevice;								//各桁の数字を格納

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		COUNTDOWN_TEXTURE,
		&g_pTextureCountDown);

	// カウントダウンの情報の設定
	g_CountDown.pos		= COUNTDOWN_POS;				// 位置
	g_CountDown.length  = COUNTDOWN_INIT_RADIUS;		// 長さ
	g_CountDown.fAlpha	= COUNTDOWN_INIT_ALPHA;			// 透明度
	g_CountDown.fTexU	= COUNTDOWN_INIT_TEXU;			// テクスチャのU軸の値
	g_CountDown.bUse	= false;						// 使用状況

	// 状態関係の設定
	g_CountDown.stateInfo.state			= CNTDOWNSTATE_NONE;	// 状態
	g_CountDown.stateInfo.nStateCounter = 0;					// 状態のカウント

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCountDown,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffCountDown->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_CountDown.pos.x, g_CountDown.pos.y - g_CountDown.length.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_CountDown.pos.x + g_CountDown.length.x, g_CountDown.pos.y - g_CountDown.length.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_CountDown.pos.x, g_CountDown.pos.y + g_CountDown.length.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_CountDown.pos.x + g_CountDown.length.x, g_CountDown.pos.y + g_CountDown.length.y, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(g_CountDown.fTexU, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(g_CountDown.fTexU, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffCountDown->Unlock();
}

//========================================
//カウントダウンの終了処理
//========================================
void UninitCountDown(void)
{
	//テクスチャの破棄
	if (g_pTextureCountDown != NULL)
	{
		g_pTextureCountDown->Release();
		g_pTextureCountDown = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffCountDown != NULL)
	{
		g_pVtxBuffCountDown->Release();
		g_pVtxBuffCountDown = NULL;
	}
}

//========================================
//カウントダウンの更新処理
//========================================
void UpdateCountDown(void)
{
	VERTEX_2D *pVtx;

	if (g_CountDown.bUse == true)
	{ // 使用している場合

		switch (g_CountDown.stateInfo.state)
		{
		case CNTDOWNSTATE_NONE:		// 無状態

			// 透明度を設定する
			g_CountDown.fAlpha = COUNTDOWN_INIT_ALPHA;

			break;

		case CNTDOWNSTATE_APPEAR:	// 出現状態

			// 長さを伸ばす
			g_CountDown.length.x += COUNTDOWN_LENGTH_MOVE;

			// テクスチャを伸ばす
			g_CountDown.fTexU += 1.0f / COUNTDOWN_LENGTH_CNT;

			// カウントを加算する
			g_CountDown.stateInfo.nStateCounter++;

			if (g_CountDown.stateInfo.nStateCounter >= COUNTDOWN_LENGTH_CNT)
			{ // カウントが一定のカウントになった場合

				// 長さを設定する
				g_CountDown.length.x = COUNTDOWN_RADIUS.x;

				// テクスチャを設定する
				g_CountDown.fTexU = 1.0f;
			}

			if (g_CountDown.stateInfo.nStateCounter >= COUNTDOWN_APPEAR_CNT)
			{ // カウントが一定のカウントになった場合

				// カウントを初期化する
				g_CountDown.stateInfo.nStateCounter = 0;

				// 拡大化状態にする
				g_CountDown.stateInfo.state = CNTDOWNSTATE_TRANS;
			}

			break;

		case CNTDOWNSTATE_TRANS:	// 透明化状態

			// 透明度を移動させる
			g_CountDown.fAlpha -= COUNTDOWN_ALPHA_SUB;

			if (g_CountDown.fAlpha <= 0.0f)
			{ // 透明度が下がった場合

				// 透明度を設定する
				g_CountDown.fAlpha = 0.0f;

				// 無状態にする
				g_CountDown.stateInfo.state = CNTDOWNSTATE_NONE;

				// 使用しない
				g_CountDown.bUse = false;

				// 脱出通知の設定処理
				SetEscape();
			}

			break;
		}

		//頂点バッファをロック
		g_pVtxBuffCountDown->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_CountDown.pos.x, g_CountDown.pos.y - g_CountDown.length.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_CountDown.pos.x + g_CountDown.length.x, g_CountDown.pos.y - g_CountDown.length.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_CountDown.pos.x, g_CountDown.pos.y + g_CountDown.length.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_CountDown.pos.x + g_CountDown.length.x, g_CountDown.pos.y + g_CountDown.length.y, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_CountDown.fTexU, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_CountDown.fTexU, 1.0f);

		//頂点バッファをアンロックする
		g_pVtxBuffCountDown->Unlock();
	}
}

//=====================================
//カウントダウンの描画処理
//=====================================
void DrawCountDown(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffCountDown,							//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D));								//頂点情報構造体のサイズ

	if (g_CountDown.bUse == true)
	{//使用されていた場合

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureCountDown);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,											//描画する最初の頂点インデックス
			2);											//描画するプリミティブ数
	}
}

//=====================================
// カウントダウンの設定処理
//=====================================
void SetCountDown(void)
{
	VERTEX_2D *pVtx;

	if (g_CountDown.bUse == false)
	{ // 使用していない場合

		// 情報の設定
		g_CountDown.pos		= COUNTDOWN_POS;			// 位置
		g_CountDown.length	= COUNTDOWN_INIT_RADIUS;	// 長さ
		g_CountDown.fAlpha	= COUNTDOWN_ALPHA;			// 透明度
		g_CountDown.fTexU	= COUNTDOWN_INIT_TEXU;		// テクスチャのU軸の値

		// 状態関係の情報の設定
		g_CountDown.stateInfo.state			= CNTDOWNSTATE_APPEAR;		// 状態
		g_CountDown.stateInfo.nStateCounter = 0;						// 状態カウント

		//頂点バッファをロック
		g_pVtxBuffCountDown->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_CountDown.pos.x, g_CountDown.pos.y - g_CountDown.length.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_CountDown.pos.x + g_CountDown.length.x, g_CountDown.pos.y - g_CountDown.length.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_CountDown.pos.x, g_CountDown.pos.y + g_CountDown.length.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_CountDown.pos.x + g_CountDown.length.x, g_CountDown.pos.y + g_CountDown.length.y, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_CountDown.fTexU, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_CountDown.fTexU, 1.0f);

		//頂点バッファをアンロックする
		g_pVtxBuffCountDown->Unlock();

		g_CountDown.bUse = true;						// 使用する
	}
}