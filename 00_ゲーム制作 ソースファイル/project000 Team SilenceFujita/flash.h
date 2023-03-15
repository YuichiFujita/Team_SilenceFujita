//======================================================================================================================
//
//	フラッシュヘッダー [flash.h]
//	Author：小原立暉
//
//======================================================================================================================
#ifndef _FLASH_H_		// このマクロ定義がされていない場合
#define _FLASH_H_		// 二重インクルード防止のマクロを定義する

//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define WEATHER_ALPHA		(0.7f)	// 雷のフラッシュのアルファ値
#define REV_WEATHER_ALPHA	(0.2f)	// 雷のフラッシュの補正係数

#define TITLE_FLASH_ALPHA		(1.0f)	// タイトルのフラッシュのアルファ値
#define REV_TITLE_FLASH_ALPHA	(0.01f)	// タイトルのフラッシュの補正係数

#define REV_TUTORIAL_ALPHA	(0.05f)	// チュートリアルのフラッシュの補正係数

//**********************************************************************************************************************
//	構造体定義 (Flash)
//**********************************************************************************************************************
typedef struct
{
	D3DXCOLOR col;		// 位置
	float fDestAlpha;	// 目標の透明度
	float fDiffAlpha;	// 透明度の差分
	float fRevAlpha;	// 透明度の補正係数
	bool bUse;			// 使用状況
}Flash;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void InitFlash(void);							// フラッシュの初期化処理
void UninitFlash(void);							// フラッシュの終了処理
void UpdateFlash(void);							// フラッシュの更新処理
void DrawFlash(void);							// フラッシュの描画処理
void SetFlash(float fAlpha,float fRevAlpha);	// フラッシュの設定処理
Flash GetFlash(void);							// フラッシュの取得処理

#endif