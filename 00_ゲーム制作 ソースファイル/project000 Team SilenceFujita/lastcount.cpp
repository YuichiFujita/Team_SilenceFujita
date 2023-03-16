//=======================================
//
// ラストカウント処理[lastcount.cpp]
// Author 小原立暉
//
//=======================================
#include "main.h"
#include "lastcount.h"
#include "timer.h"
#include "value.h"

//マクロ定義
#define LASTCOUNT_POS				(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f))			// ラストカウントの位置
#define LASTCOUNT_INIT_SIZE			(D3DXVECTOR3(10.0f, 10.0f, 0.0f))										// ラストカウントの半径
#define LASTCOUNT_SIZE				(D3DXVECTOR3(100.0f, 100.0f, 0.0f))										// ラストカウントの半径
#define LASTCOUNT_ALPHA				(0.7f)				// ラストカウントの透明度
#define LASTCOUNT_ADD_SIZE			(5.0f)				// ラストカウントの追加サイズ
#define LASTCOUNT_TIME				(10)				// ラストカウントが使われるようになる時間
#define LASTCOUNT_CLEAR_ADDITION	(0.05f)				// ラストカウントの透明度の加算
#define LASTCOUNT_CLEAR_INTERVAL	(5)					// ラストカウントが薄くなる間隔
#define LASTCOUNT_INIT_ALPHA		(0.0f)				// ラストカウントの初期数値
#define LASTCOUNT_SHIFT				(130.0f)			// ラストカウントのずらす幅

//グローバル変数
LASTCOUNT g_LastCount;									//ラストカウントの情報

//==========================================
// ラストカウントの初期化処理
//==========================================
void InitLastCount(void)
{
	// 情報を初期化する
	g_LastCount.pos = LASTCOUNT_POS;				// 位置
	g_LastCount.size = LASTCOUNT_INIT_SIZE;			// 半径
	g_LastCount.nCount = 0;							// カウント
	g_LastCount.nTime = 0;							// 時間
	g_LastCount.nTimeOld = -1;						// 前回の時間
	g_LastCount.nDigit = 2;							// 桁数
	g_LastCount.fClear = LASTCOUNT_INIT_ALPHA;		// 透明度
	g_LastCount.bUse = false;						// 使用状況
}

//========================================
// ラストカウントの終了処理
//========================================
void UninitLastCount(void)
{
	
}

//========================================
// ラストカウントの更新処理
//========================================
void UpdateLastCount(void)
{
	// 変数を宣言
	g_LastCount.nTime = GetTime();		// 時間を取得

	if (g_LastCount.nTime <= LASTCOUNT_TIME)
	{ // 一定時間を過ぎたら

		// ラストカウントを使っていることにする
		g_LastCount.bUse = true;

		if (g_LastCount.nTimeOld != g_LastCount.nTime)
		{ // 時間が前回の時間と違う場合

			// 前回の数値を設定する
			g_LastCount.nTimeOld = g_LastCount.nTime;

			// 透明度を設定する
			g_LastCount.fClear = LASTCOUNT_ALPHA;

			// 半径を設定する
			g_LastCount.size = LASTCOUNT_INIT_SIZE;

			if (g_LastCount.nTime >= LASTCOUNT_TIME)
			{ // 時間が2桁の場合

				// 位置を設定する
				g_LastCount.pos = D3DXVECTOR3
				(
					LASTCOUNT_POS.x + (LASTCOUNT_SHIFT * 0.5f),
					LASTCOUNT_POS.y,
					LASTCOUNT_POS.z
				);

				// 桁数を設定する
				g_LastCount.nDigit = 2;
			}
			else
			{ // 時間が1桁の場合

				// 位置を設定する
				g_LastCount.pos = LASTCOUNT_POS;

				// 桁数を設定する
				g_LastCount.nDigit = 1;
			}
		}
	}

	if (g_LastCount.bUse == true)
	{ // ラストカウントが使用されている場合

		// カウントを加算する
		g_LastCount.nCount++;

		if ((g_LastCount.nCount % LASTCOUNT_CLEAR_INTERVAL) == 0)
		{ // 5F経過したら

			// 透明度を加算する
			g_LastCount.fClear -= LASTCOUNT_CLEAR_ADDITION;

			if (g_LastCount.fClear <= 0.0f)
			{ // 透明度が0.0f以下になった場合

				// 透明度を補正する
				g_LastCount.fClear = 0.0f;
			}
		}

		// 半径を加算する
		g_LastCount.size.x += LASTCOUNT_ADD_SIZE;
		g_LastCount.size.y += LASTCOUNT_ADD_SIZE;

		if (g_LastCount.size.x > LASTCOUNT_SIZE.x)
		{ // 半径が一定数を超えた場合

			// 半径を設定する
			g_LastCount.size = LASTCOUNT_SIZE;
		}
	}
}

//=====================================
//ラストカウントの描画処理
//=====================================
void DrawLastCount(void)
{
	// 数値の設定
	SetValue
	( // 引数
		g_LastCount.pos,		// 位置
		g_LastCount.nTime,		// 値
		LASTCOUNT_TIME,			// 値の最大値
		g_LastCount.size.x,		// 横幅
		g_LastCount.size.y,		// 縦幅
		LASTCOUNT_SHIFT,		// 数値間の幅
		g_LastCount.fClear		// α値
	);

	// 数値の描画
	DrawValue(g_LastCount.nDigit, VALUETYPE_BURNING);
}