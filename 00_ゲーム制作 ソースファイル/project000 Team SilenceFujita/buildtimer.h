//===================================
//
// 再建築タイマーヘッダー[buildtimer.h]
// Author 小原立暉
//
//===================================
#ifndef _BUILDTIMER_H_
#define _BUILDTIMER_H_

#include "main.h"
#include "object.h"

//マクロ定義
#define MAX_BUILDTIMER			(256)	//再建築タイマーの最大数

//プロトタイプ宣言
void InitBuildtimer(void);				// 再建築の初期化処理
void UninitBuildtimer(void);			// 再建築の終了処理
void UpdateBuildtimer(void);			// 再建築の更新処理
void DrawBuildtimer(void);				// 再建築の描画処理
void SetBuildtimer(D3DXVECTOR3 pos, int nCount, Object *pObject);	// 再建築の設定処理

#endif