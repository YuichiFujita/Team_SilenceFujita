//===================================
//
//エディットメイン画面ヘッダー[Editmain.h]
//Author 小原立暉
//
//===================================
#ifndef _EDITMAIN_H_			//このマクロ定義がされていなかったら
#define _EDITMAIN_H_			//2重インクルード防止のマクロを設定する

#include"main.h"

//オブジェクトの種類
typedef enum
{
	EDITSTYLE_OBJECT = 0,								//オブジェクト
	EDITSTYLE_BILLBOARD,								//ビルボード
	EDITSTYLE_MAX
}EDITSTYLE;

//プロトタイプ宣言
void InitEditmain(void);						//エディットメイン画面の初期化処理
void UninitEditmain(void);						//エディットメイン画面の終了処理
void UpdateEditmain(void);						//エディットメイン画面の更新処理
void DrawEditmain(void);						//エディットメイン画面の描画処理
int GetStyle(void);								//スタイルの取得処理
void StyleChange(void);							//スタイル変更処理

#endif