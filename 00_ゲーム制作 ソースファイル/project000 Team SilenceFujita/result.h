//===================================
//
//リザルト画面ヘッダー[result.h]
//Author 小原立暉
//
//===================================
#ifndef _RESULT_H_			//このマクロ定義がされていなかったら
#define _RESULT_H_			//2重インクルード防止のマクロを設定する

#include"main.h"

//マクロ定義

//プロトタイプ宣言
void InitResult(void);										//リザルト画面の初期化処理
void UninitResult(void);									//リザルト画面の終了処理
void UpdateResult(void);									//リザルト画面の更新処理
void DrawResult(void);										//リザルト画面の描画処理

#endif