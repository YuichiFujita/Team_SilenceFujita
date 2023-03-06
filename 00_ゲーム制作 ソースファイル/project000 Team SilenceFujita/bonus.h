//===========================================
//
// ボーナスヘッダー[bonus.h]
// Author 小原立暉
//
//===========================================
#ifndef _BONUS_H_
#define _BONUS_H_

#include "main.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_BONUS		(128)		// ボーナスの最大数

//**********************************************************************************************************************
//	ボーナス理由(ADDSCORETYPE)
//**********************************************************************************************************************
typedef enum
{
	ADDSCORE_HUMAN = 0,		// 人を吹き飛ばした時
	ADDSCORE_CAR,			// 車をバリアで閉じ込めたとき
	ADDSCORE_OBJECT,		// オブジェクトを破壊したとき
	ADDSCORE_ITEM,			// アイテムをゲージ満杯状態で取ったとき
	ADDSCORE_MAX			// この列挙型の総数
}ADDSCORETYPE;

//**********************************************************************************************************************
//	ボーナステクスチャ(BONUSTEX)
//**********************************************************************************************************************
typedef enum
{
	BONUSTEX_PLUS = 0,		// プラスのテクスチャ
	BONUSTEX_MAX			// この列挙型の総数
}BONUSTEX;

//**********************************************************************************************************************
//	ボーナスの状態(BONUSSTATE)
//**********************************************************************************************************************
typedef enum
{
	BONUSSTATE_FADE = 0,	// フェード状態
	BONUSSTATE_DISP,		// 表示状態
	BONUSSTATE_ADDSCORE,	// 加算状態
	BONUSSTATE_MAX			// この列挙型の総数
}BONUSSTATE;

//**********************************************************************************************************************
//得点表示の構造体
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 move;			// 移動量
	BONUSSTATE  state;			// 状態
	D3DXCOLOR	col;			// 色
	int			nScore;			// スコアの値
	int			nStateCounter;	// 状態カウンター
	int			nDigit;			// 桁数
	bool		bUse;			// 使用しているかどうか
}Bonus;

//**********************************************************************************************************************
//プロトタイプ宣言
//**********************************************************************************************************************
void InitBonus(void);		// ボーナスの初期化処理
void UninitBonus(void);		// ボーナスの終了処理
void UpdateBonus(void);		// ボーナスの更新処理
void DrawBonus(void);		// ボーナスの描画処理
void SetBonus(ADDSCORETYPE Reason);	// ボーナスの設定処理
Bonus *GetBonus(void);		// ボーナスの取得処理

#endif