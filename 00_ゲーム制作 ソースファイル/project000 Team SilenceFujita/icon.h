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
#include "game.h"
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
	ICONTYPE_EVIL_OBJECT = 0,	// 悪いオブジェクト
	ICONTYPE_OBJECT,			// 良いオブジェクト
	ICONTYPE_EVIL_HUMAN,		// 悪い人間のアイコン
	ICONTYPE_EVIL_CAR,			// 悪い車のアイコン
	ICONTYPE_POLICE,			// 警察のアイコン
	ICONTYPE_BARRIER,			// バリアのアイコン
	ICONTYPE_ITEM,				// アイテム
	ICONTYPE_GATE_VERT,			// ゲートの奥手前のアイコン
	ICONTYPE_GATE_HORIZ,		// ゲートの左右のアイコン
	ICONTYPE_PLAY,				// プレイヤーのアイコン
	ICONTYPE_MAX				// この列挙型の総数
}ICONTYPE;

//************************************************************
//	列挙型定義（ICONSTATE）
//************************************************************
typedef enum
{
	ICONSTATE_NONE = 0,			// 何もなし
	ICONSTATE_DISAPPEAR,		// 消える途中
	ICONSTATE_REVIVAL,			// 復活中
	ICONSTATE_DAMAGE,			// ダメージ中
	ICONSTATE_UNRIVALED,		// 無敵状態
	ICONSTATE_FLYAWAY,			// 人が飛んでいる状態
	ICONSTATE_ENLARGE,			// 拡大状態
	ICONSTATE_REDUCE,			// 縮小状態
	ICONSTATE_MAX				// この列挙型の総数
}ICONSTATE;

//************************************************************
//	列挙型定義（ICONALPHA）
//************************************************************
typedef enum
{
	ICONALPHA_NONE = 0,			// 不透明
	ICONALPHA_CLEAR,			// 透明
	ICONALPHA_MAX				// この列挙型の定義
}ICONALPHA;

//************************************************************
//	構造体定義 (IconInfo)
//************************************************************
typedef struct
{
	int nIconID;					// アイコンのインデックス
	ICONSTATE state;				// アイコンの状態
} IconInfo;

//************************************************************
//	構造体定義 (Icon)
//************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	ICONTYPE	type;				// 種類
	D3DXCOLOR	col;				// 色
	D3DXCOLOR	colCopy;			// 色のコピー
	ICONALPHA	alpha;				// 透明度の状態
	D3DXVECTOR3	radius;				// 半径
	D3DXMATRIX  mtxWorld;			// ワールドマトリックス
	int        *pIconIDParent;		// アイコンの親のアイコンインデックス
	int			nCounter;			// カウンター
	bool       *pUseParent;			// アイコンの親の使用状況
	ICONSTATE  *pState;				// アイコンの状態
	bool        bUse;				// 使用状況
} Icon;

//************************************************************
//	プロトタイプ宣言
//************************************************************
void InitIcon(void);		// アイコンの初期化処理
void UninitIcon(void);		// アイコンの終了処理
void UpdateIcon(void);		// アイコンの更新処理
void DrawIcon(void);		// アイコンの描画処理
int SetIcon(D3DXVECTOR3 pos, ICONTYPE type, int *pIconID, bool *pUse, ICONSTATE *pState);			// アイコンの設定処理
int SetIconObject(D3DXVECTOR3 radius, D3DXVECTOR3 pos, JUDGESTATE type, int *pIconID, bool *pUse, ICONSTATE *pState, ROTSTATE rot);	// オブジェクトのアイコンの設定処理

void SetPositionIcon(int nIconID, D3DXVECTOR3 pos);			// アイコンの位置の設定処理


#endif