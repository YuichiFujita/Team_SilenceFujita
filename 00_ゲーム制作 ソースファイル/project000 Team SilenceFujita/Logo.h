//===================================
//
//ロゴヘッダー[Logo.h]
//Author 小原立暉
//
//===================================
#ifndef _LOGO_H_			//このマクロ定義がされていなかったら
#define _LOGO_H_			//2重インクルード防止のマクロを設定する

#include"main.h"

//プロトタイプ宣言
void InitLogo(void);					// ロゴ画面の初期化処理
void UninitLogo(void);					// ロゴ画面の終了処理
void UpdateLogo(void);					// ロゴ画面の更新処理
void DrawLogo(void);					// ロゴ画面の描画処理
#endif