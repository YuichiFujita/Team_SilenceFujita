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
void VecSizePos(D3DXVECTOR3 *sizePos, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);	// 四頂点の位置の計算処理
float LineOuterProduct(D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 pos);						// 外積の左右判断処理
void LandObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, bool *pbJump);									// モデルの着地の更新処理
float RotNormalize(float rot);		// 向きの正規化

#endif