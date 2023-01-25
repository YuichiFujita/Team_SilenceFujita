//======================================================================================================================
//
//	送風機(フライ・アウェイ)ヘッダー [wind.h]
//	Author：小原立暉
//
//======================================================================================================================
#ifndef _WIND_H_			// このマクロ定義がされていない場合
#define _WIND_H_			// 二重インクルード防止のマクロを定義する

#include "main.h"
#include "Human.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void InitWind(void);		// 送風機の初期化処理
void UninitWind(void);		// 送風機の終了処理
void UpdateWind(void);		// 送風機の更新処理
void DrawWind(void);		// 送風機の描画処理
void SetWind(D3DXVECTOR3 pos, D3DXVECTOR3 rot);			// 送風機の設定処理
void CollisionWind(Human *pHuman);						// 風の当たり判定

//**********************************************************************************************************************
//	プロトタイプ宣言 (デバッグ用)
//**********************************************************************************************************************

#endif