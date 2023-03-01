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
#define COMBO_GROUND_X			(100.0f)						// 下地の半径(X軸)
#define COMBO_GROUND_Y			(50.0f)							// 下地の半径(Y軸)
#define COMBO_NUMBER_X			(19.0f)							// 数字の半径(X軸)
#define COMBO_NUMBER_Y			(19.0f)							// 数字の半径(Y軸)
#define COMBO_NUMBER_SHIFT		(26.0f)							// 数字のずらす数

#define MAX_CONBOCOUNT			(99)							// コンボカウントの最大
#define COMBOSTOP_CNT			(600)							// コンボの止まるカウント
#define DIGIT_ONE				(1)								// 1桁の境界
#define DIGIT_TWO				(10)							// 2桁の境界

//コンボのテクスチャ
typedef enum
{
	COMBOTEX_GROUND = 0,			//下地
	COMBOTEX_MAX
}COMBOTEX;

//プロトタイプ宣言

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureCombo[COMBOTEX_MAX] = {};			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCombo = NULL;					// 頂点バッファへのポインタ

Combo g_Combo;													// コンボの情報
int g_nComboScore;												// 倍率でかけるスコア
int g_nComboCount;												// コンボが止まるまでのカウント

//テクスチャファイル名
const char *c_apFilenameCombo[COMBOTEX_MAX] =
{
	"data/TEXTURE/ui007.png",								//下地
};

//===========================================
//敵の初期化処理
//===========================================
void InitCombo(void)
{
	LPDIRECT3DDEVICE9 pDevice;									//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	// 情報の初期化
	g_Combo.pos = D3DXVECTOR3(1150.0f, 600.0f, 0.0f);		// 位置
	g_Combo.nMagni		= 0;								// 倍率
	g_Combo.nDigit		= 0;								// 桁数
	g_Combo.nMoveCount  = 0;								// 移動カウント
	g_Combo.bMoveUp		= true;								// 上に移動しているか下に移動しているか
	g_Combo.bUse		= false;							// 使用状況

	// 倍率でかけるスコアを初期化
	g_nComboScore = 0;

	// コンボが止まるまでのカウントを初期化
	g_nComboCount = 0;

	for (int nCntTexture = 0; nCntTexture < COMBOTEX_MAX; nCntTexture++)
	{//テクスチャの設定
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameCombo[nCntTexture],
			&g_pTextureCombo[nCntTexture]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCombo,
		NULL);

	VERTEX_2D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_Combo.pos.x - COMBO_GROUND_X, g_Combo.pos.y - COMBO_GROUND_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Combo.pos.x + COMBO_GROUND_X, g_Combo.pos.y - COMBO_GROUND_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Combo.pos.x - COMBO_GROUND_X, g_Combo.pos.y + COMBO_GROUND_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Combo.pos.x + COMBO_GROUND_X, g_Combo.pos.y + COMBO_GROUND_Y, 0.0f);

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
	g_pVtxBuffCombo->Unlock();
}

//===========================================
//敵の終了処理
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
//敵の更新処理
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

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Combo.pos.x - COMBO_GROUND_X, g_Combo.pos.y - COMBO_GROUND_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Combo.pos.x + COMBO_GROUND_X, g_Combo.pos.y - COMBO_GROUND_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Combo.pos.x - COMBO_GROUND_X, g_Combo.pos.y + COMBO_GROUND_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Combo.pos.x + COMBO_GROUND_X, g_Combo.pos.y + COMBO_GROUND_Y, 0.0f);

		if (g_nComboCount >= COMBOSTOP_CNT)
		{ // カウントが一定数に達したら

			// コンボの倍率処理(スコア加算)
			MagnificCombo(COMBO_INTERRUPTION);
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffCombo->Unlock();
}

//===========================================
//プレイヤーの描画処理
//===========================================
void DrawCombo(void)
{
	LPDIRECT3DDEVICE9 pDevice;								//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffCombo,									//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D));									//頂点情報構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_Combo.bUse == true)
	{//使用されている場合

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureCombo[COMBOTEX_GROUND]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,											//描画する最初の頂点インデックス
			2);											//描画するプリミティブ数

		// 数値の設定処理
		SetValue
		(
			g_Combo.pos,
			g_Combo.nMagni,
			MAX_CONBOCOUNT,
			COMBO_NUMBER_X,
			COMBO_NUMBER_Y,
			COMBO_NUMBER_SHIFT,
			1.0f);

		// 数値の描画処理
		DrawValue
		(
			g_Combo.nDigit,
			VALUETYPE_RED
		);

		// 数値の設定処理
		SetValue
		(
			D3DXVECTOR3(g_Combo.pos.x - 100.0f, g_Combo.pos.y, g_Combo.pos.z),
			g_nComboScore,
			VAL_MAX_SCORE,
			COMBO_NUMBER_X,
			COMBO_NUMBER_Y,
			COMBO_NUMBER_SHIFT,
			1.0f);

		// 数値の描画処理
		DrawValue
		(
			VAL_SCO_DIGIT,
			VALUETYPE_NORMAL
		);
	}
}

//===========================================
// コンボの倍率処理
//===========================================
void MagnificCombo(int nMagni)
{
	if (nMagni <= COMBO_INTERRUPTION)
	{ // 倍率が-1以下だった場合

		// スコアの加算処理
		AddScore(g_nComboScore * g_Combo.nMagni);

		// 倍率を0にする
		g_Combo.nMagni = 0;

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