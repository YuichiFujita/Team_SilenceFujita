#ifdef _DEBUG	// デバッグ処理
//===================================
//
//エディットメイン画面ヘッダー[Editmain.h]
//Author 小原立暉
//
//===================================
#ifndef _EDITMAIN_H_			//このマクロ定義がされていなかったら
#define _EDITMAIN_H_			//2重インクルード防止のマクロを設定する

#include"main.h"

//オブジェクトの状態
typedef enum
{
	OBJECTSTATE_NONE = 0,		//オブジェクト不使用状態
	OBJECTSTATE_USE,			//オブジェクト使用中
	OBJECTSTATE_DELETETARGET,	//オブジェクト削除判定中
	OBJECTSTATE_MAX
}OBJECTSTATE;

//オブジェクトの種類
typedef enum
{
	EDITSTYLE_OBJECT = 0,		//オブジェクト
	EDITSTYLE_BILLBOARD,		//ビルボード
	EDITSTYLE_PARTICLE,			//パーティクル
	EDITSTYLE_MAX
}EDITSTYLE;

//プロトタイプ宣言
void InitEditmain(void);		//エディットメイン画面の初期化処理
void UninitEditmain(void);		//エディットメイン画面の終了処理
void UpdateEditmain(void);		//エディットメイン画面の更新処理
void DrawEditmain(void);		//エディットメイン画面の描画処理
int  GetStyle(void);			//スタイルの取得処理
void StyleChange(void);			//スタイル変更処理

#endif

#endif