//=======================================================================================================
//
//	影ヘッダー [shadow.h]
//	Author：藤田勇一
//
//=======================================================================================================
#ifndef _SHADOW_H_				// このマクロ定義がされていない場合
#define _SHADOW_H_				// 二重インクルード防止のマクロを定義する

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "model.h"

//************************************************************
//	マクロ定義
//************************************************************
#define NONE_SHADOW		(-1)	// 影の初期値 (使用していない場合)

#define NONE_ROT		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		// 向きの初期値
#define NONE_SCALE		(D3DXVECTOR3(1.0f, 1.0f, 1.0f))		// 拡大率の初期値

#define FIRST_RADIUS	(60.0f)	// 初期化時のオブジェクトの影の半径

//************************************************************
//	列挙型定義 (SHADOWTYPE)
//************************************************************
typedef enum
{
	SHADOWTYPE_NONE = 0,		// 影無し
	SHADOWTYPE_CIRCLE,			// 丸影
	SHADOWTYPE_MODEL,			// モデル影
	SHADOWTYPE_MAX,				// この列挙型の総数
} SHADOWTYPE;

//************************************************************
//	プロトタイプ宣言
//************************************************************
void InitShadow(void);			// 影の初期化処理
void UninitShadow(void);		// 影の終了処理
void UpdateShadow(void);		// 影の更新処理
void DrawShadow(void);			// 影の描画処理

int  SetCircleShadow(float fAlpha, float fRadius, int *pShadowID, bool *pUse);				// 丸影の設定処理
int  SetModelShadow(Model model, int *pShadowID, bool *pUse);								// モデル影の設定処理
void SetPositionShadow(int nShadowID, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale);	// 影の位置の設定処理


#endif