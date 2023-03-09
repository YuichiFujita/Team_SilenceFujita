//===================================
//
//コンボヘッダー[Combo.h]
//Author 小原立暉
//
//===================================
#ifndef _COMBO_H_
#define _COMBO_H_

#include "main.h"

//**********************************************************************************************************************
// マクロ定義
//**********************************************************************************************************************
#define COMBO_INTERRUPTION	(-1)	// コンボが途切れる時の値

//**********************************************************************************************************************
// コンボの構造体
//**********************************************************************************************************************
typedef struct
{
	int  nMagni;				// 倍率
	int  nMoveCount;			// ムーブカウント
	int  nDigit;				// 文字数の長さ
	float fAlpha;				// 透明度
	bool bMoveUp;				// 位置が下に動くか上に動くか
	bool bUse;					// 使用状況
}Combo;

//**********************************************************************************************************************
//プロトタイプ宣言
//**********************************************************************************************************************
void InitCombo(void);			// コンボの初期化処理
void UninitCombo(void);			// コンボの終了処理
void UpdateCombo(void);			// コンボの更新処理
void DrawCombo(void);			// コンボの描画処理
void MagnificCombo(int nMagni);	// コンボの倍率処理
void AddComboScore(int nScore);	// コンボのスコアの加算処理
int  GetCurrentCombo(void);		// コンボ数の取得処理

#endif