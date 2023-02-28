//===========================================
//
// 再建築タイマーのメイン処理[buildtimer.cpp]
// Author 小原立暉
//
//===========================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "buildtimer.h"
#include "object.h"
#include "3DValue.h"
#include "player.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define BUILDTIMER_TEXTURE		"data/TEXTURE/BuildTimerUI.png"		// 再建築タイマーのテクスチャ
#define BUILDTIMER_RADIUS_X		(200.0f)				// 再建築タイマーの半径(X軸)
#define BUILDTIMER_RADIUS_Y		(80.0f)					// 再建築タイマーの半径(Y軸)
#define BUILDTIMER_POS_Y_SHIFT	(100.0f)				// 再建築タイマーのずらす高さ(Y軸)

#define MAX_VAL_BUILD			(2)						// 再建築タイマーの表示数値数
#define MAX_BUILD_MIN			(60)					// タイム (分) の最大値
#define MAX_BUILD_SEC			(60)					// タイム (秒) の最大値
#define BUILD_WAIT_TIME			(40)					// 待機状態のカウント数
#define BUILD_APPEAR_MAGNI		(0.05f)					// 出現時の拡大率

//**********************************************************************************************************************
//	構造体宣言(Build)
//**********************************************************************************************************************
typedef struct
{
	Object		object;		// オブジェクトのポインタ
	BUILDSTATE	state;		// 状態
	int			nCount;		// カウント
	bool		bUse;		// 使用状況
}Build;

//グローバル変数
Build g_aBuild[MAX_BUILDTIMER];					// 再建築タイマーの情報

//==========================================
// 再建築タイマーの初期化処理
//==========================================
void InitBuildtimer(void)
{
	for (int nCntSet = 0; nCntSet < MAX_BUILDTIMER; nCntSet++)
	{
		// 情報の初期化
		g_aBuild[nCntSet].state = BUILDSTATE_COUNT;					// 状態
		g_aBuild[nCntSet].nCount = 0;								// カウンター
		g_aBuild[nCntSet].bUse	 = false;							// 使用状況
	}
}

//========================================
// 再建築タイマーの終了処理
//========================================
void UninitBuildtimer(void)
{

}

//========================================
// 再建築タイマーの更新処理
//========================================
void UpdateBuildtimer(void)
{
	for (int nCntBill = 0; nCntBill < MAX_BUILDTIMER; nCntBill++)
	{
		if (g_aBuild[nCntBill].bUse == true)
		{//使用されていた場合

			switch (g_aBuild[nCntBill].state)
			{
			case BUILDSTATE_COUNT:		// カウント状態

				// カウントを減算する
				g_aBuild[nCntBill].nCount--;

				if (g_aBuild[nCntBill].nCount <= 0)
				{ // カウントが0を超えた場合

					// カウントを0にする
					g_aBuild[nCntBill].nCount = 0;

					// 消滅状態にする
					g_aBuild[nCntBill].state = BUILDSTATE_DISAPPEAR;
				}

				break;					// 抜け出す

			case BUILDSTATE_DISAPPEAR:	// 消滅状態

				// オブジェクトの設定処理
				SetObject
				(
					g_aBuild[nCntBill].object.pos,					// 位置
					g_aBuild[nCntBill].object.rot,					// 向き
					g_aBuild[nCntBill].object.scale,				// 拡大率
					&g_aBuild[nCntBill].object.matCopy[0],			// マテリアルデータ
					g_aBuild[nCntBill].object.nType,				// 種類
					g_aBuild[nCntBill].object.nBreakType,			// 壊れ方の種類
					g_aBuild[nCntBill].object.nShadowType,			// 影の種類
					g_aBuild[nCntBill].object.nCollisionType,		// 当たり判定の種類
					g_aBuild[nCntBill].object.collInfo.stateRot,	// 当たり判定の向きの状態
					APPEARSTATE_SLOWLY								// 徐々に出現
				);

				// 使用しない
				g_aBuild[nCntBill].bUse = false;

				break;					// 抜け出す
			}
		}
	}
}

//=====================================
// 再建築タイマーの描画処理
//=====================================
void DrawBuildtimer(void)
{
	
}

//======================================
// 再建築タイマーの設定処理
//======================================
void SetBuildtimer(D3DXVECTOR3 pos, int nCount, Object object)
{
	for (int nCntBill = 0; nCntBill < MAX_BUILDTIMER; nCntBill++)
	{
		if (g_aBuild[nCntBill].bUse == false)
		{//使用されていなかった場合

			// 情報の設定
			g_aBuild[nCntBill].object = object;					// オブジェクトの情報
			g_aBuild[nCntBill].nCount = nCount;					// カウント
			g_aBuild[nCntBill].state = BUILDSTATE_COUNT;		// 状態

			//使用する
			g_aBuild[nCntBill].bUse = true;

			break;									//抜け出す
		}
	}
}

//======================================
// 再建築タイマーの総数取得処理
//======================================
int GetNumBuildTimer(void)
{
	int nNumBuild = 0;

	for (int nCnt = 0; nCnt < MAX_BUILDTIMER; nCnt++)
	{
		if (g_aBuild[nCnt].bUse == true)
		{ // 使用している場合

			// 再建築の総数を加算する
			nNumBuild++;
		}
	}

	// 再建築タイマーの総数を返す
	return nNumBuild;
}