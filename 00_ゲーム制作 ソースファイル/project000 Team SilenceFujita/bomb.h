//======================================================================================================================
//
//	爆弾ヘッダー [bomb.h]
//	Author：藤田勇一
//
//======================================================================================================================
#ifndef _BOMB_H_			// このマクロ定義がされていない場合
#define _BOMB_H_			// 二重インクルード防止のマクロを定義する

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define BOMB_NONE_COL	(D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f))		// 範囲外時のマテリアルの色
#define BOMB_RANGE_COL	(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.6f))		// 範囲内時のマテリアルの色
#define BOMB_AIM_COL	(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.6f))		// 狙い時のマテリアルの色

//**********************************************************************************************************************
//	列挙型定義 (BOMBSTATE)
//**********************************************************************************************************************
typedef enum
{
	BOMBSTATE_NONE = 0,		// 何もしない状態
	BOMBSTATE_RANGE,		// 範囲内状態
	BOMBSTATE_AIM,			// 狙い状態
	BOMBSTATE_BAR_NEAR,		// バリア接近状態
	BOMBSTATE_BAR_IN,		// バリア内状態
	BOMBSTATE_MAX,			// この列挙型の総数
} BOMBSTATE;

//**********************************************************************************************************************
//	列挙型定義 (BARRIERSTATE)
//**********************************************************************************************************************
typedef enum
{
	BARRIERSTATE_NONE = 0,	// 何もしない状態
	BARRIERSTATE_FLY,		// 発射状態
	BARRIERSTATE_HOMING,	// 追尾状態
	BARRIERSTATE_LAND,		// 着弾状態
	BARRIERSTATE_ENLARGE,	// 拡大状態
	BARRIERSTATE_UP,		// 上昇状態
	BARRIERSTATE_SET,		// 完成状態
	BARRIERSTATE_DOWN,		// 下降状態
	BARRIERSTATE_REDUCE,	// 縮小状態
	BARRIERSTATE_MAX,		// この列挙型の総数
} BARRIERSTATE;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void InitBomb(void);						// 爆弾の初期化処理
void UninitBomb(void);						// 爆弾の終了処理
void UpdateBomb(void);						// 爆弾の更新処理
void DrawBomb(void);						// 爆弾の描画処理
void ShotBarrier(void);						// バリアの発射処理
BARRIERSTATE GetBarrierState(void *pCar);	// バリアの状態の取得処理

//**********************************************************************************************************************
//	プロトタイプ宣言 (デバッグ用)
//**********************************************************************************************************************

#endif