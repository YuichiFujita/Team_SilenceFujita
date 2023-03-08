//=======================================
//
// ボーナスのメイン処理[Bonus.cpp]
// Author 小原立暉
//
//=======================================
#include "bonus.h"

#include "Combo.h"
#include "score.h"
#include "game.h"
#include "Police.h"
#include "value.h"
#include "2Deffect.h"
#include "2Dparticle.h"

//**********************************************************************************************************************
// マクロ定義
//**********************************************************************************************************************
#define BONUS_MAX_VALUE		(9999)		// ボーナスの最大値
#define BONUS_WIDTH			(26.0f)		// ボーナスの幅
#define BONUS_HEIGHT		(26.0f)		// ボーナスの高さ
#define BONUS_SHIFT			(37.0f)		// ボーナスのずらす幅
#define BONUS_ALPHA			(1.0f)		// ボーナスの透明度
#define PLUS_WIDTH			(30.0f)		// プラスの幅
#define PLUS_HEIGHT			(30.0f)		// プラスの高さ
#define PLUS_SHIFT			(40.0f)		// プラスのずらす幅
#define BONUS_RIGHT_X		(1080.0f)	// 右にボーナスを出す時の座標(X座標)
#define BONUS_LEFT_X		(40.0f)		// 左にボーナスを出す時の座標(X座標)
#define BONUS_RIGHT_Y		(400)		// 右にボーナスを出す時の座標(Y座標)
#define BONUS_RIGHT_SHIFT	(120)		// 右にボーナスを出す時のずらす座標(Y座標)
#define BONUS_LEFT_Y		(200)		// 左にボーナスを出す時の座標(Y座標)
#define BONUS_LEFT_SHIFT	(350)		// 左にボーナスを出す時のずらす座標(Y座標)
#define BONUS_SCORE_POS		(D3DXVECTOR3( 918.0f, 573.0f, 0.0f))		// スコアの位置

#define BONUS_MOVE_MAGNI	(0.02f)		// プラスの移動量の倍率
#define BONUS_STATE_CNT		(120)		// 加算状態になるまでのカウント
#define BONUS_ADD_ALPHA		(0.02f)		// ボーナスの透明度に加算する値

#define BONUS_EFFECT_LIFE	(8)			// ボーナスのエフェクトの寿命
#define BONUS_EFFECT_RADIUS	(40.0f)		// ボーナスのエフェクトの半径
#define BONUS_EFFECT_SUB	(4.0f)		// ボーナスのエフェクトの減衰係数

#define BONUS_DIGIT_ONE		(9)			// 1桁の境界
#define BONUS_DIGIT_TWO		(99)		// 2桁の境界
#define BONUS_DIGIT_THREE	(999)		// 3桁の境界
#define BONUS_DIGIT_FOUR	(9999)		// 4桁の境界
#define BONUS_DIGIT_FIVE	(99999)		// 5桁の境界

//**********************************************************************************************************************
//	ボーナステクスチャ(BONUSTEX)
//**********************************************************************************************************************
typedef enum
{
	BONUSTEX_PLUS = 0,		// プラスのテクスチャ
	BONUSTEX_MAX			// この列挙型の総数
}BONUSTEX;

//**********************************************************************************************************************
//グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBonus[BONUSTEX_MAX] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBonus = NULL;	//頂点バッファへのポインタ
Bonus g_aBonus[MAX_BONUS];			// ボーナスの情報

//**********************************************************************************************************************
// テクスチャファイル名
//**********************************************************************************************************************
const char *c_apFilenameBonus[BONUSTEX_MAX] = 
{
	"data/TEXTURE/Plus.png",
};

//==========================================
//得点表示の初期化処理
//==========================================
void InitBonus(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < BONUSTEX_MAX; nCnt++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameBonus[nCnt],
			&g_pTextureBonus[nCnt]);
	}

	//得点の情報の初期化
	for (int nCntBonus = 0; nCntBonus < MAX_BONUS; nCntBonus++)
	{
		// 情報の初期化
		g_aBonus[nCntBonus].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_aBonus[nCntBonus].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
		g_aBonus[nCntBonus].state = BONUSSTATE_FADE;					// 状態
		g_aBonus[nCntBonus].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 色
		g_aBonus[nCntBonus].nScore = 0;									// 得点
		g_aBonus[nCntBonus].whereBonus = WHEREBONUS_RIGHT;				// ボーナスの左右
		g_aBonus[nCntBonus].nStateCounter = 0;							// 状態カウンター
		g_aBonus[nCntBonus].nDigit = 0;									// 桁数
		g_aBonus[nCntBonus].bUse = false;								// 使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BONUS,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBonus,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffBonus->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBonus = 0; nCntBonus < MAX_BONUS; nCntBonus++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x - PLUS_WIDTH, g_aBonus[nCntBonus].pos.y - PLUS_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x + PLUS_WIDTH, g_aBonus[nCntBonus].pos.y - PLUS_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x - PLUS_WIDTH, g_aBonus[nCntBonus].pos.y + PLUS_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x + PLUS_WIDTH, g_aBonus[nCntBonus].pos.y + PLUS_HEIGHT, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = g_aBonus[nCntBonus].col;
		pVtx[1].col = g_aBonus[nCntBonus].col;
		pVtx[2].col = g_aBonus[nCntBonus].col;
		pVtx[3].col = g_aBonus[nCntBonus].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;			//頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBonus->Unlock();
}

//========================================
//得点表示の終了処理
//========================================
void UninitBonus(void)
{
	for (int nCntBonus = 0; nCntBonus < BONUSTEX_MAX; nCntBonus++)
	{
		//テクスチャの破棄
		if (g_pTextureBonus[nCntBonus] != NULL)
		{
			g_pTextureBonus[nCntBonus]->Release();
			g_pTextureBonus[nCntBonus] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBonus != NULL)
	{
		g_pVtxBuffBonus->Release();
		g_pVtxBuffBonus = NULL;
	}
}

//========================================
//得点表示の更新処理
//========================================
void UpdateBonus(void)
{
	int nCntBonus;					//回数の変数を宣言する

	D3DXVECTOR3 ScorePos = BONUS_SCORE_POS;		// スコアの位置を取得

	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBonus->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBonus = 0; nCntBonus < MAX_BONUS; nCntBonus++)
	{
		if (g_aBonus[nCntBonus].bUse == true)
		{ // 得点表示が使用されている

			switch (g_aBonus[nCntBonus].state)
			{
			case BONUSSTATE_FADE:		// フェード状態

				// 色を加算する
				g_aBonus[nCntBonus].col.a += BONUS_ADD_ALPHA;

				if (g_aBonus[nCntBonus].col.a >= 1.0f)
				{ // 透明度が1.0fになった場合

					// 透明度を1.0fにする
					g_aBonus[nCntBonus].col.a = 1.0f;

					// 表示状態にする
					g_aBonus[nCntBonus].state = BONUSSTATE_DISP;
				}

				break;					// 抜け出す

			case BONUSSTATE_DISP:		// 表示状態

				// 状態カウンターを加算する
				g_aBonus[nCntBonus].nStateCounter++;

				if (g_aBonus[nCntBonus].nStateCounter >= BONUS_STATE_CNT)
				{ // 状態カウンターが一定以上になったら

					// 移動量を計算する
					g_aBonus[nCntBonus].move.x = (ScorePos.x - g_aBonus[nCntBonus].pos.x) * BONUS_MOVE_MAGNI;
					g_aBonus[nCntBonus].move.y = (ScorePos.y - g_aBonus[nCntBonus].pos.y) * BONUS_MOVE_MAGNI;

					// 透明度を設定する
					g_aBonus[nCntBonus].col.a = 0.0f;

					// 加算状態にする
					g_aBonus[nCntBonus].state = BONUSSTATE_ADDSCORE;
				}

				break;					// 抜け出す

			case BONUSSTATE_ADDSCORE:	// スコア加算状態

				if (g_aBonus[nCntBonus].whereBonus == WHEREBONUS_RIGHT)
				{ // 右側に出た場合

					// 位置を移動させる
					g_aBonus[nCntBonus].pos += g_aBonus[nCntBonus].move;

					if (g_aBonus[nCntBonus].pos.x <= ScorePos.x)
					{ // スコアの位置を過ぎた場合

						// 位置を補正する
						g_aBonus[nCntBonus].pos = ScorePos;

						// 使用しない
						g_aBonus[nCntBonus].bUse = false;

						// コンボのスコアの加算処理
						AddComboScore(g_aBonus[nCntBonus].nScore);

						// 2Dパーティクルの発生
						Set2DParticle
						( // 引数
							g_aBonus[nCntBonus].pos,			// 位置
							D3DXCOLOR(0.8f, 0.5f, 0.0f, 1.0f),	// 色
							PARTICLE2DTYPE_SCORE_FIRE,			// 花火
							20,									// 発生数
							1									// 寿命
						);
					}
				}
				else
				{ // 左側に出た場合

					// 位置を移動させる
					g_aBonus[nCntBonus].pos += g_aBonus[nCntBonus].move;

					if (g_aBonus[nCntBonus].pos.x >= ScorePos.x)
					{ // スコアの位置を過ぎた場合

						// 位置を補正する
						g_aBonus[nCntBonus].pos = ScorePos;

						// 使用しない
						g_aBonus[nCntBonus].bUse = false;

						// コンボのスコアの加算処理
						AddComboScore(g_aBonus[nCntBonus].nScore);

						// 2Dパーティクルの発生
						Set2DParticle
						( // 引数
							g_aBonus[nCntBonus].pos,			// 位置
							D3DXCOLOR(0.8f, 0.5f, 0.0f, 1.0f),	// 色
							PARTICLE2DTYPE_SCORE_FIRE,			// 花火
							20,									// 発生数
							1									// 寿命
						);
					}
				}

				// 2Dパーティクルを発生させる
				Set2DEffect
				(
					g_aBonus[nCntBonus].pos,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.5f, 0.0f, 1.0f),
					BONUS_EFFECT_LIFE,
					BONUS_EFFECT_RADIUS,
					BONUS_EFFECT_SUB
				);

				break;					// 抜け出す
			}

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x - PLUS_WIDTH, g_aBonus[nCntBonus].pos.y - PLUS_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x + PLUS_WIDTH, g_aBonus[nCntBonus].pos.y - PLUS_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x - PLUS_WIDTH, g_aBonus[nCntBonus].pos.y + PLUS_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x + PLUS_WIDTH, g_aBonus[nCntBonus].pos.y + PLUS_HEIGHT, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = g_aBonus[nCntBonus].col;
			pVtx[1].col = g_aBonus[nCntBonus].col;
			pVtx[2].col = g_aBonus[nCntBonus].col;
			pVtx[3].col = g_aBonus[nCntBonus].col;
		}
		pVtx += 4;			// 頂点データを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBonus->Unlock();
}

//=====================================
//得点表示の描画処理
//=====================================
void DrawBonus(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBonus;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffBonus,										// 頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D));										// 頂点情報構造体のサイズ

	for (nCntBonus = 0; nCntBonus < MAX_BONUS; nCntBonus++)
	{
		if (g_aBonus[nCntBonus].bUse == true)
		{ // 得点表示が使用されている

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBonus[BONUSTEX_PLUS]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				4 * nCntBonus,								// 描画する最初の頂点インデックス
				2);											// 描画するプリミティブ数
		}
	}

	for (nCntBonus = 0; nCntBonus < MAX_BONUS; nCntBonus++)
	{
		if (g_aBonus[nCntBonus].bUse == true)
		{ // 得点表示が使用されている

			// 数値の設定処理
			SetValue
			(
				D3DXVECTOR3
				(
					g_aBonus[nCntBonus].pos.x + (g_aBonus[nCntBonus].nDigit * PLUS_SHIFT),
					g_aBonus[nCntBonus].pos.y,
					0.0f
				),
				g_aBonus[nCntBonus].nScore,
				BONUS_MAX_VALUE,
				BONUS_WIDTH,
				BONUS_HEIGHT,
				BONUS_SHIFT,
				g_aBonus[nCntBonus].col.a
			);

			// 数値の描画処理
			DrawValue(g_aBonus[nCntBonus].nDigit, VALUETYPE_NORMAL);
		}
	}
}

//========================================
//得点表示の設定処理
//========================================
void SetBonus(int nBonus)
{
	VERTEX_2D * pVtx;					// 頂点情報へのポインタ
	D3DXVECTOR3 posBonus;				// ボーナスの位置

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBonus->Lock(0, 0, (void**)&pVtx, 0);

	if (GetGameState() == GAMESTATE_NORMAL)
	{ // ゲームの進行が通常状態の場合

		for (int nCntBonus = 0; nCntBonus < MAX_BONUS; nCntBonus++)
		{
			if (g_aBonus[nCntBonus].bUse == false)
			{ // 得点表示が使用されていない場合

				// 増援の得点を増やす
				GetReinforce()->nBonus += 1;

				// コンボの倍率処理
				MagnificCombo(1);

				// 右に出すか左に出すかをランダムで算出する
				g_aBonus[nCntBonus].whereBonus = (WHEREBONUS)(rand() % WHEREBONUS_MAX);

				switch (g_aBonus[nCntBonus].whereBonus)
				{
				case WHEREBONUS_RIGHT:	// 右に出す

					// ボーナスの位置を設定する
					posBonus.x = BONUS_RIGHT_X;
					posBonus.y = (float)(rand() % BONUS_RIGHT_SHIFT) + BONUS_RIGHT_Y;

					break;				// 抜け出す

				case WHEREBONUS_LEFT:	// 左に出す

					// ボーナスの位置を設定する
					posBonus.x = BONUS_LEFT_X;
					posBonus.y = (float)(rand() % BONUS_LEFT_SHIFT) + BONUS_LEFT_Y;

					break;				// 抜け出す
				}

				// 情報の設定
				g_aBonus[nCntBonus].pos = D3DXVECTOR3(posBonus.x, posBonus.y, 0.0f);	// 位置
				g_aBonus[nCntBonus].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
				g_aBonus[nCntBonus].state = BONUSSTATE_FADE;					// 状態
				g_aBonus[nCntBonus].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);	// 色
				g_aBonus[nCntBonus].nStateCounter = 0;							// 状態カウンター

				// スコアを設定する
				g_aBonus[nCntBonus].nScore = nBonus;

				if (g_aBonus[nCntBonus].nScore <= BONUS_DIGIT_ONE)
				{ // スコアが9以上だった場合

					// 桁数を設定する
					g_aBonus[nCntBonus].nDigit = 1;
				}
				else if (g_aBonus[nCntBonus].nScore <= BONUS_DIGIT_TWO)
				{ // スコアが99以上だった場合

					// 桁数を設定する
					g_aBonus[nCntBonus].nDigit = 2;
				}
				else if (g_aBonus[nCntBonus].nScore <= BONUS_DIGIT_THREE)
				{ // スコアが999以上だった場合

					// 桁数を設定する
					g_aBonus[nCntBonus].nDigit = 3;
				}
				else if (g_aBonus[nCntBonus].nScore <= BONUS_DIGIT_FOUR)
				{ // スコアが9999以上だった場合

					// 桁数を設定する
					g_aBonus[nCntBonus].nDigit = 4;
				}
				else if (g_aBonus[nCntBonus].nScore <= BONUS_DIGIT_FIVE)
				{ // スコアが99999以上だった場合

					// 桁数を設定する
					g_aBonus[nCntBonus].nDigit = 5;
				}

				// 頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x - PLUS_WIDTH, g_aBonus[nCntBonus].pos.y - PLUS_HEIGHT, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x + PLUS_WIDTH, g_aBonus[nCntBonus].pos.y - PLUS_HEIGHT, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x - PLUS_WIDTH, g_aBonus[nCntBonus].pos.y + PLUS_HEIGHT, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x + PLUS_WIDTH, g_aBonus[nCntBonus].pos.y + PLUS_HEIGHT, 0.0f);

				//頂点カラーの設定
				pVtx[0].col = g_aBonus[nCntBonus].col;
				pVtx[1].col = g_aBonus[nCntBonus].col;
				pVtx[2].col = g_aBonus[nCntBonus].col;
				pVtx[3].col = g_aBonus[nCntBonus].col;

				//使用している状態にする
				g_aBonus[nCntBonus].bUse = true;

				// 2Dパーティクルの発生
				Set2DParticle
				( // 引数
					g_aBonus[nCntBonus].pos,			// 位置
					D3DXCOLOR(0.8f, 0.5f, 0.0f, 1.0f),	// 色
					PARTICLE2DTYPE_BONUS_FIRE,			// 花火
					20,									// 発生数
					1									// 寿命
				);

				//抜け出す
				break;
			}

			pVtx += 4;			//頂点データを4つ分進める
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBonus->Unlock();
}

//========================================
// ボーナスの取得処理
//========================================
Bonus *GetBonus(void)
{
	// ボーナスの情報を返す
	return &g_aBonus[0];
}