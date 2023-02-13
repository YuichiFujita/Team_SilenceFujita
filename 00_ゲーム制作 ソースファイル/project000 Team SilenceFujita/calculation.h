//=============================================================================
//
//	汎用関数ヘッダー [calculation.h]
//	Author：藤田勇一 & 小原立暉
//
//=============================================================================
#ifndef _CALCULATION_H_		// このマクロ定義がされていない場合
#define _CALCULATION_H_		// 二重インクルード防止のマクロを定義する

//*****************************************************************************
//	インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//	プロトタイプ宣言
//*****************************************************************************
void  VecSizePos(D3DXVECTOR3 *sizePos, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);	// 四頂点の位置の計算処理
float LineOuterProduct(D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 pos);						// 外積の左右判断処理
void  LandObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, bool *pbJump);									// モデルの着地の更新処理

void RotNormalize(float *rot);			// 向きの正規化

// 万能シリーズ
void LoadFileChunk(bool bCurve, bool bHumanCurve, bool bStage, bool bCollision, bool bShadow, bool bObject, bool bAI);	// ファイルをロードする全体処理

void InitAllAroundChunk(void);			// 万能初期化の全体処理
void UninitAllAroundChunk(void);		// 万能終了の全体処理
void DrawAllAroundChunk(void);			// 万能描画の全体処理

void InitResultChunk(void);				// リザルトの初期化全体処理
void UninitResultChunk(void);			// リザルトの終了全体処理
void DrawResultChunk(void);				// リザルトの描画全体処理

#endif