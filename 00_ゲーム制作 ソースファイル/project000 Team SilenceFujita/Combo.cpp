//========================================
//
//コンボのメイン処理[Combo.cpp]
//Author 小原立暉
//
//========================================
#include "main.h"
#include "Combo.h"
#include "score.h"
#include "value.h"

//マクロ定義
#define MAX_COMBO			(2)			// 使用するポリゴン数

#define MAX_CONBOCOUNT		(99)		// コンボカウントの最大
#define COMBOSTOP_CNT		(600)		// コンボの止まるカウント
#define COMBO_CLEAR_CNT		(300)		// コンボが透明になり始めるカウント
#define DIGIT_ONE			(1)			// 1桁の境界
#define DIGIT_TWO			(10)		// 2桁の境界

#define COM_COMBO_BG_POS	(D3DXVECTOR3(1138.0f, 583.0f, 0.0f))	// コンボの背景の位置
#define COM_SCORE_BG_POS	(D3DXVECTOR3( 918.0f, 573.0f, 0.0f))	// コンボスコアの背景の位置
#define COM_COMBO_VAL_POS	(D3DXVECTOR3(1139.0f, 589.0f, 0.0f))	// コンボの値の位置
#define COM_SCORE_VAL_POS	(D3DXVECTOR3(1026.0f, 576.0f, 0.0f))	// コンボスコアの値の位置

#define COM_COMBO_BG_SIZE	(D3DXVECTOR3( 100.0f,  50.0f, 0.0f))	// コンボの背景サイズ
#define COM_SCORE_BG_SIZE	(D3DXVECTOR3( 150.0f,  50.0f, 0.0f))	// コンボスコアの背景サイズ
#define COM_COMBO_VAL_SIZE	(D3DXVECTOR3(  24.0f,  24.0f, 0.0f))	// コンボの値のサイズ
#define COM_SCORE_VAL_SIZE	(D3DXVECTOR3(  19.0f,  19.0f, 0.0f))	// コンボスコアの値のサイズ

#define COM_DIGIT_PLUS		(14.0f)		// コンボの桁増加時の位置加算量
#define COM_COMBO_VAL_SPACE	(30.0f)		// コンボの値の空白サイズ
#define COM_SCORE_VAL_SPACE	(26.0f)		// コンボスコアの値の空白サイズ

//コンボのテクスチャ
typedef enum
{
	COMBOTEX_GROUND_COMBO = 0,	// コンボ下地
	COMBOTEX_GROUND_SCORE,		// コンボスコア下地
	COMBOTEX_MAX
}COMBOTEX;

//プロトタイプ宣言

//グローバル変数宣言
LPDIRECT3DTEXTURE9      g_pTextureCombo[COMBOTEX_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCombo = NULL;					// 頂点バッファへのポインタ

Combo g_Combo;			// コンボの情報
int   g_nComboScore;	// 倍率でかけるスコア
int   g_nComboCount;	// コンボが止まるまでのカウント

//テクスチャファイル名
const char *c_apFilenameCombo[COMBOTEX_MAX] =
{
	"data/TEXTURE/ui006.png",	// コンボ下地
	"data/TEXTURE/ui007.png",	// コンボスコア下地
};

//===========================================
//コンボの初期化処理
//===========================================
void InitCombo(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_2D *pVtx;							// 頂点情報へのポインタ

	// 情報の初期化
	g_Combo.nMagni     = 0;		// 倍率
	g_Combo.nDigit     = 0;		// 桁数
	g_Combo.nMoveCount = 0;		// 移動カウント
	g_Combo.fAlpha	   = 1.0f;	// 透明度
	g_Combo.bMoveUp    = true;	// 上に移動しているか下に移動しているか
	g_Combo.bUse       = false;	// 使用状況

	// 倍率でかけるスコアを初期化
	g_nComboScore = 0;

	// コンボが止まるまでのカウントを初期化
	g_nComboCount = 0;

	for (int nCntTexture = 0; nCntTexture < COMBOTEX_MAX; nCntTexture++)
	{ // テクスチャの設定

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, c_apFilenameCombo[nCntTexture], &g_pTextureCombo[nCntTexture]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_2D) * 4 * MAX_COMBO,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCombo,
		NULL
	);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);

	//---------------------------------------
	//	コンボの背景の初期化
	//---------------------------------------
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(COM_COMBO_BG_POS.x - COM_COMBO_BG_SIZE.x, COM_COMBO_BG_POS.y - COM_COMBO_BG_SIZE.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(COM_COMBO_BG_POS.x + COM_COMBO_BG_SIZE.x, COM_COMBO_BG_POS.y - COM_COMBO_BG_SIZE.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(COM_COMBO_BG_POS.x - COM_COMBO_BG_SIZE.x, COM_COMBO_BG_POS.y + COM_COMBO_BG_SIZE.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(COM_COMBO_BG_POS.x + COM_COMBO_BG_SIZE.x, COM_COMBO_BG_POS.y + COM_COMBO_BG_SIZE.y, 0.0f);

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

	//---------------------------------------
	//	コンボスコアの背景の初期化
	//---------------------------------------
	//頂点座標の設定
	pVtx[4].pos = D3DXVECTOR3(COM_SCORE_BG_POS.x - COM_SCORE_BG_SIZE.x, COM_SCORE_BG_POS.y - COM_SCORE_BG_SIZE.y, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(COM_SCORE_BG_POS.x + COM_SCORE_BG_SIZE.x, COM_SCORE_BG_POS.y - COM_SCORE_BG_SIZE.y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(COM_SCORE_BG_POS.x - COM_SCORE_BG_SIZE.x, COM_SCORE_BG_POS.y + COM_SCORE_BG_SIZE.y, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(COM_SCORE_BG_POS.x + COM_SCORE_BG_SIZE.x, COM_SCORE_BG_POS.y + COM_SCORE_BG_SIZE.y, 0.0f);

	//rhwの設定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffCombo->Unlock();
}

//===========================================
//コンボの終了処理
//===========================================
void UninitCombo(void)
{
	int nCntCombo;

	for (nCntCombo = 0; nCntCombo < COMBOTEX_MAX; nCntCombo++)
	{
		//テクスチャの破棄
		if (g_pTextureCombo[nCntCombo] != NULL)
		{
			g_pTextureCombo[nCntCombo]->Release();
			g_pTextureCombo[nCntCombo] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffCombo != NULL)
	{
		g_pVtxBuffCombo->Release();
		g_pVtxBuffCombo = NULL;
	}
}

//===========================================
//コンボの更新処理
//===========================================
void UpdateCombo(void)
{
	if (g_Combo.nMagni > 0)
	{ // 倍率が0より超過していた場合

		// 使用する
		g_Combo.bUse = true;
	}
	else
	{ // 倍率が0以下の場合

		// 使用していない
		g_Combo.bUse = false;
	}

	VERTEX_2D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);

	if (g_Combo.bUse == true)
	{//使用していた場合

		// コンボの止まるカウントを加算
		g_nComboCount++;

		if (g_nComboCount >= COMBO_CLEAR_CNT)
		{ // カウントが一定以上になった場合

			// 透明度を減らしていく
			g_Combo.fAlpha -= (1.0f / (COMBOSTOP_CNT - COMBO_CLEAR_CNT));

			if (g_Combo.fAlpha <= 0.0f)
			{ // 透明度が0.0f以下になった場合

				// 透明度を補正する
				g_Combo.fAlpha = 0.0f;
			}
		}

		if (g_nComboCount >= COMBOSTOP_CNT)
		{ // カウントが一定数に達したら

			// 透明度を補正する
			g_Combo.fAlpha = 0.0f;

			// コンボの倍率処理(スコア加算)
			MagnificCombo(COMBO_INTERRUPTION);
		}

		for (int nCnt = 0; nCnt < MAX_COMBO; nCnt++)
		{
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Combo.fAlpha);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Combo.fAlpha);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Combo.fAlpha);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Combo.fAlpha);

			pVtx += 4;		// 頂点データを4つ分進める
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffCombo->Unlock();
}

//===========================================
//コンボの描画処理
//===========================================
void DrawCombo(void)
{
	// 変数を宣言
	D3DXVECTOR3 pos;	// 桁ごとの倍率位置の設定用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffCombo,									//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D));									//頂点情報構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_Combo.bUse == true)
	{//使用されている場合

		//-----------------------------------
		//	背景の描画
		//-----------------------------------
		for (int nCntCombo = 0; nCntCombo < MAX_COMBO; nCntCombo++)
		{ // 使用するポリゴン数分繰り返す

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureCombo[nCntCombo]);

			// ポリゴンの描画
			pDevice->DrawPrimitive
			( // 引数
				D3DPT_TRIANGLESTRIP,	// プリミティブの種類
				nCntCombo * 4,			// 描画する最初の頂点インデックス
				2						// 描画するプリミティブ数
			);
		}
		
		//-----------------------------------
		//	数値の描画
		//-----------------------------------
		// 桁ごとの倍率位置の設定
		pos = COM_COMBO_VAL_POS;
		pos.x += (g_Combo.nDigit == 2) ? COM_DIGIT_PLUS : 0;	// 2桁の場合位置をずらす

		// 数値の設定処理
		SetValue
		( // 引数
			pos,					// 位置
			g_Combo.nMagni,			// 値
			MAX_CONBOCOUNT,			// 値の最大値
			COM_COMBO_VAL_SIZE.x,	// 横幅
			COM_COMBO_VAL_SIZE.y,	// 縦幅
			COM_COMBO_VAL_SPACE,	// 数値間の幅
			g_Combo.fAlpha			// α値
		);

		// 数値の描画処理
		DrawValue(g_Combo.nDigit, VALUETYPE_RED);

		// 数値の設定処理
		SetValue
		( // 引数
			COM_SCORE_VAL_POS,		// 位置
			g_nComboScore,			// 値
			VAL_MAX_SCORE,			// 値の最大値
			COM_SCORE_VAL_SIZE.x,	// 横幅
			COM_SCORE_VAL_SIZE.y,	// 縦幅
			COM_SCORE_VAL_SPACE,	// 数値間の幅
			g_Combo.fAlpha			// α値
		);

		// 数値の描画処理
		DrawValue(VAL_SCO_DIGIT, VALUETYPE_NORMAL);
	}
}

//===========================================
// コンボの倍率処理
//===========================================
void MagnificCombo(int nMagni)
{
	if (nMagni <= COMBO_INTERRUPTION)
	{ // 倍率が-1以下だった場合

		if (g_Combo.nMagni <= 0)
		{ // 倍率が0以下だった場合

			// 倍率を1に設定する
			g_Combo.nMagni = 1;
		}

		// スコアの加算処理
		AddScore(g_nComboScore * g_Combo.nMagni);

		// 倍率を0にする
		g_Combo.nMagni = 0;

		// コンボ倍率を使用しない
		g_Combo.bUse = false;

		// 倍率でかけるスコアを初期化
		g_nComboScore = 0;

		// コンボの止まるカウントを初期化
		g_nComboCount = 0;
	}
	else
	{ // 倍率が0よりも高かった場合

		// 倍率を加算する
		g_Combo.nMagni += nMagni;

		if (g_Combo.nMagni >= MAX_CONBOCOUNT)
		{ // 倍率が最大数以上だった場合

			// コンボの倍率を補正する
			g_Combo.nMagni = MAX_CONBOCOUNT;
		}

		if (g_Combo.nMagni >= DIGIT_TWO)
		{ // 10以上だった場合

			// 桁数を設定する
			g_Combo.nDigit = 2;
		}
		else if (g_Combo.nMagni >= DIGIT_ONE)
		{ // 1以上だった場合

			// 桁数を設定する
			g_Combo.nDigit = 1;
		}

		// コンボの止まるカウントを初期化
		g_nComboCount = 0;

		// 透明度を初期化
		g_Combo.fAlpha = 1.0f;
	}
}

//===========================================
// コンボ倍率のスコアの加算処理
//===========================================
void AddComboScore(int nScore)
{
	if (g_nComboScore < VAL_MAX_SCORE)
	{ // 現在のスコアの値がスコアの最大値より小さい場合

		// スコアに引数の値を加算
		g_nComboScore += nScore;

		if (g_nComboScore > VAL_MAX_SCORE)
		{ // 現在のスコアの値がスコアの最大値より大きい場合

			// スコアを補正
			g_nComboScore = VAL_MAX_SCORE;
		}
	}
}