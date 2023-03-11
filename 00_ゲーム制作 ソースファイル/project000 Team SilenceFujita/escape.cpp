//=======================================
//
//脱出通知処理[Escape.cpp]
//Author 小原立暉
//
//=======================================
#include "main.h"
#include "escape.h"
#include "timer.h"

//-----------------------------------------------------
//マクロ定義
//-----------------------------------------------------
#define ESCAPE_APPEAR_CNT			(100)										// 脱出通知の出現からの遷移カウント
#define ESCAPE_LENGTH_CNT			(40)										// 脱出通知の出現からの遷移カウント

#define ESCAPE_POS					(D3DXVECTOR3(1150.0f, 40.0f, 0.0f))			// 脱出通知の位置
#define ESCAPE_RADIUS				(D3DXVECTOR3(80.0f, 20.0f, 0.0f))			// 脱出通知の半径
#define ESCAPE_INIT_ALPHA			(0.0f)										// 脱出通知の透明度の初期値
#define ESCAPE_ALPHA				(1.0f)										// 脱出通知の透明度
#define ESCAPE_ADD_ALPHA			(0.01f)										// 脱出通知の透明度の加算数
#define ESCAPE_EMPHASIS_TIME		(30)										// 脱出通知が強調状態になる時間
#define ESCAPE_RADIUS_MOVE			(D3DXVECTOR3(2.0f, 0.5f, 0.0f))				// 脱出通知の半径の拡大率
#define ESCAPE_EMPHASIS_RADIUS		(D3DXVECTOR3(120.0f, 30.0f, 0.0f))			// 脱出通知の強調時の半径
#define ESCAPE_EMPHASIS_WAIT_CNT	(20)										// 脱出通知の強調時の待機カウント数

#define ESCAPE_TEXTURE		"data/TEXTURE/Escape.png"							// 脱出通知のテクスチャ

//-----------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------
void EscapeEmphasis(void);								// 脱出通知の強調処理

//-----------------------------------------------------
//グローバル変数
//-----------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureEscape = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEscape = NULL;		// 頂点バッファへのポインタ
EscapeInfo g_Escape;									// 脱出通知の情報

//==========================================
//脱出通知の初期化処理
//==========================================
void InitEscape(void)
{
	LPDIRECT3DDEVICE9 pDevice;								//各桁の数字を格納

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		ESCAPE_TEXTURE,
		&g_pTextureEscape);

	// 脱出通知の情報の設定
	g_Escape.pos = ESCAPE_POS;					// 位置
	g_Escape.length = ESCAPE_RADIUS;			// 長さ
	g_Escape.fAlpha = ESCAPE_INIT_ALPHA;		// 透明度
	g_Escape.bUse = false;						// 使用状況

	// 脱出通知の状態関係の設定
	g_Escape.stateInfo.nStateCounter = 0;						// 状態カウンター 
	g_Escape.stateInfo.escapeState	 = ESCAPESTATE_NONE;		// 状態
	g_Escape.stateInfo.emphaState	 = EMPHASISSTATE_BIGGER;	// 拡大中状態

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEscape,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffEscape->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_Escape.pos.x - g_Escape.length.x, g_Escape.pos.y - g_Escape.length.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Escape.pos.x + g_Escape.length.x, g_Escape.pos.y - g_Escape.length.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Escape.pos.x - g_Escape.length.x, g_Escape.pos.y + g_Escape.length.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Escape.pos.x + g_Escape.length.x, g_Escape.pos.y + g_Escape.length.y, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffEscape->Unlock();
}

//========================================
//脱出通知の終了処理
//========================================
void UninitEscape(void)
{
	//テクスチャの破棄
	if (g_pTextureEscape != NULL)
	{
		g_pTextureEscape->Release();
		g_pTextureEscape = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEscape != NULL)
	{
		g_pVtxBuffEscape->Release();
		g_pVtxBuffEscape = NULL;
	}
}

//========================================
//脱出通知の更新処理
//========================================
void UpdateEscape(void)
{
	VERTEX_2D *pVtx;

	if (g_Escape.bUse == true)
	{ // 使用している場合

		switch (g_Escape.stateInfo.escapeState)
		{
		case ESCAPESTATE_NONE:			// 無状態

			// 透明度を加算する
			g_Escape.fAlpha += ESCAPE_ADD_ALPHA;

			if (g_Escape.fAlpha >= ESCAPE_ALPHA)
			{ // 一定数値を超えた場合

				// 透明度を補正する
				g_Escape.fAlpha = ESCAPE_ALPHA;

				// 表示状態にする
				g_Escape.stateInfo.escapeState = ESCAPESTATE_DISP;
			}

			break;

		case ESCAPESTATE_DISP:			// 表示状態

			if (GetTime() == ESCAPE_EMPHASIS_TIME)
			{ // 時間が一定数に達した場合

				// 強調状態にする
				g_Escape.stateInfo.escapeState = ESCAPESTATE_EMPHASIS;
			}

			break;

		case ESCAPESTATE_EMPHASIS:		// 強調状態

			// 脱出通知の強調処理
			EscapeEmphasis();

			break;

		default:						// 上記以外

			break;
		}

		//頂点バッファをロック
		g_pVtxBuffEscape->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Escape.pos.x - g_Escape.length.x, g_Escape.pos.y - g_Escape.length.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Escape.pos.x + g_Escape.length.x, g_Escape.pos.y - g_Escape.length.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Escape.pos.x - g_Escape.length.x, g_Escape.pos.y + g_Escape.length.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Escape.pos.x + g_Escape.length.x, g_Escape.pos.y + g_Escape.length.y, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);

		//頂点バッファをアンロックする
		g_pVtxBuffEscape->Unlock();
	}
}

//=====================================
//脱出通知の描画処理
//=====================================
void DrawEscape(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffEscape,							//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D));							//頂点情報構造体のサイズ

	if (g_Escape.bUse == true)
	{//使用されていた場合

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureEscape);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,											//描画する最初の頂点インデックス
			2);											//描画するプリミティブ数
	}
}

//=====================================
// 脱出通知の設定処理
//=====================================
void SetEscape(void)
{
	VERTEX_2D *pVtx;

	if (g_Escape.bUse == false)
	{ // 使用していない場合

		// 情報の設定
		g_Escape.pos = ESCAPE_POS;				// 位置
		g_Escape.length = ESCAPE_RADIUS;		// 長さ
		g_Escape.fAlpha = ESCAPE_INIT_ALPHA;	// 透明度

		// 状態関係の情報の設定
		g_Escape.stateInfo.escapeState	 = ESCAPESTATE_NONE;		// 状態
		g_Escape.stateInfo.nStateCounter = 0;						// 状態カウント
		g_Escape.stateInfo.emphaState	 = EMPHASISSTATE_BIGGER;	// 拡大中状態

		//頂点バッファをロック
		g_pVtxBuffEscape->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Escape.pos.x - g_Escape.length.x, g_Escape.pos.y - g_Escape.length.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Escape.pos.x + g_Escape.length.x, g_Escape.pos.y - g_Escape.length.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Escape.pos.x - g_Escape.length.x, g_Escape.pos.y + g_Escape.length.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Escape.pos.x + g_Escape.length.x, g_Escape.pos.y + g_Escape.length.y, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);

		//頂点バッファをアンロックする
		g_pVtxBuffEscape->Unlock();

		g_Escape.bUse = true;						// 使用する
	}
}

//=====================================
// 脱出通知の強調処理
//=====================================
void EscapeEmphasis(void)
{
	switch (g_Escape.stateInfo.emphaState)
	{
	case EMPHASISSTATE_BIGGER:		// 拡大中状態

		if (g_Escape.stateInfo.nStateCounter <= 0)
		{ // カウンターが0以下の場合

			// 半径を拡大する
			g_Escape.length.x += ESCAPE_RADIUS_MOVE.x;
			g_Escape.length.y += ESCAPE_RADIUS_MOVE.y;

			if (g_Escape.length.x >= ESCAPE_EMPHASIS_RADIUS.x)
			{ // 一定の半径を超えた場合

				// 半径を補正する
				g_Escape.length = ESCAPE_EMPHASIS_RADIUS;

				// 状態カウンターを加算する
				g_Escape.stateInfo.nStateCounter++;
			}
		}
		else
		{ // カウンターが0を超過していた場合

			// 状態カウンターを加算する
			g_Escape.stateInfo.nStateCounter++;

			if (g_Escape.stateInfo.nStateCounter >= ESCAPE_EMPHASIS_WAIT_CNT)
			{ // 待機カウントに達した場合

				// カウントを初期化する
				g_Escape.stateInfo.nStateCounter = 0;

				// 縮小状態にする
				g_Escape.stateInfo.emphaState = EMPHASISSTATE_SMALLER;
			}
		}

		break;

	case EMPHASISSTATE_SMALLER:		// 縮小中状態

		if (g_Escape.stateInfo.nStateCounter <= 0)
		{ // カウンターが0以下の場合

			// 半径を拡大する
			g_Escape.length.x -= ESCAPE_RADIUS_MOVE.x;
			g_Escape.length.y -= ESCAPE_RADIUS_MOVE.y;

			if (g_Escape.length.x <= ESCAPE_RADIUS.x)
			{ // 一定の半径を超えた場合

				// 半径を補正する
				g_Escape.length = ESCAPE_RADIUS;

				// 状態カウンターを加算する
				g_Escape.stateInfo.nStateCounter++;
			}
		}
		else
		{ // カウンターが0を超過していた場合

			// 状態カウンターを加算する
			g_Escape.stateInfo.nStateCounter++;

			if (g_Escape.stateInfo.nStateCounter >= ESCAPE_EMPHASIS_WAIT_CNT)
			{ // 待機カウントに達した場合

				// カウントを初期化する
				g_Escape.stateInfo.nStateCounter = 0;

				// 拡大状態にする
				g_Escape.stateInfo.emphaState = EMPHASISSTATE_BIGGER;
			}
		}

		break;

	default:						// 上記以外

		break;
	}
}