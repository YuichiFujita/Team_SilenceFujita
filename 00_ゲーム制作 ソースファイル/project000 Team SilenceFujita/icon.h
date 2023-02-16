//=======================================================================================================
//
//	アイコンヘッダー [icon.h]
//	Author：小原立暉
//
//=======================================================================================================
#ifndef _ICON_H_				// このマクロ定義がされていない場合
#define _ICON_H_				// 二重インクルード防止のマクロを定義する

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "model.h"

//************************************************************
//	マクロ定義
//************************************************************
#define NONE_ICON		(-1)	// アイコンの初期値 (使用していない場合)

//************************************************************
//	列挙型定義（ICONTYPE）
//************************************************************
typedef enum
{
	ICONTYPE_PLAY = 0,			// プレイヤーのアイコン
	ICONTYPE_EVIL,				// 悪い奴のアイコン
	ICONTYPE_POLICE,			// 警察のアイコン
	ICONTYPE_MAX				// この列挙型の総数
}ICONTYPE;

//************************************************************
//	プロトタイプ宣言
//************************************************************
void InitIcon(void);		// アイコンの初期化処理
void UninitIcon(void);		// アイコンの終了処理
void UpdateIcon(void);		// アイコンの更新処理
void DrawIcon(void);		// アイコンの描画処理
int SetIcon(D3DXVECTOR3 pos, ICONTYPE type, int *pIconID, bool *pUse);			// アイコンの設定処理

void SetPositionIcon(int nIconID, D3DXVECTOR3 pos);	// アイコンの位置の設定処理


#endif