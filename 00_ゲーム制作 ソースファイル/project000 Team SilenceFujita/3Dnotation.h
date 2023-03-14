//============================================================
//
//	強調表示ヘッダー [3Dnotation.h]
//	Author：藤田勇一
//
//============================================================
#ifndef _3D_NOTATION_H_		// このマクロ定義がされていない場合
#define _3D_NOTATION_H_		// 二重インクルード防止のマクロを定義する

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	マクロ定義
//************************************************************
#define NONE_3D_NOTATION	(-1)		// 強調表示の初期値 (使用していない場合)
#define NOTA_PLUS_POS_OBJ	(250.0f)	// 建物の強調表示の y座標加算量
#define NOTA_PLUS_POS_CAR	(130.0f)	// 車の強調表示の y座標加算量
#define NOTA_PLUS_POS_EXIT	(300.0f)	// 脱出の強調表示の y座標加算量
#define NOTA_PLUS_POS_HUMAN	(40.0f)	// 人間の強調表示の y座標加算量

//************************************************************
//	列挙型定義 (NOTATIONTYPE)
//************************************************************
typedef enum
{
	NOTATIONTYPE_BREAK = 0,			// 破壊
	NOTATIONTYPE_SHOT,				// 射出
	NOTATIONTYPE_FLY,				// 飛翔
	NOTATIONTYPE_EXIT,				// 脱出
	NOTATIONTYPE_MAX,				// この列挙型の総数
} NOTATIONTYPE;

//************************************************************
//	プロトタイプ宣言
//************************************************************
void Init3DNotation(void);			// 強調表示の初期化処理
void Uninit3DNotation(void);		// 強調表示の終了処理
void Update3DNotation(void);		// 強調表示の更新処理
void Draw3DNotation(void);			// 強調表示の描画処理

int  Set3DNotation(NOTATIONTYPE type, int *p3DNotationID, bool *pUse);	// 強調表示の設定処理
void SetPosition3DNotation(int n3DNotationID, D3DXVECTOR3 pos);			// 強調表示の位置の設定処理

#endif