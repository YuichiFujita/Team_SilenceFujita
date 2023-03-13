//======================================================================================================================
//
//	オブジェクトヘッダー [object.h]
//	Author：藤田勇一
//
//======================================================================================================================
#ifndef _OBJECT_H_						// このマクロ定義がされていない場合
#define _OBJECT_H_						// 二重インクルード防止のマクロを定義する

//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "model.h"
#include "game.h"
#include "player.h"
#include "Police.h"
#include "calculation.h"
#include "icon.h"

#ifdef _DEBUG	// デバッグ処理
#include "Editmain.h"
#endif

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_OBJECT		(2160)	// 使用するモデル数 (オブジェクトの最大数)
#define MAX_COLLISION	(20)	// 当たり判定の最大数

//**********************************************************************************************************************
//	列挙型定義 (COLLISIONTYPE)
//**********************************************************************************************************************
typedef enum
{
	COLLISIONTYPE_NONE = 0,		// 当たり判定無し
	COLLISIONTYPE_MODEL,		// モデル頂点の当たり判定
	COLLISIONTYPE_CREATE,		// 作成した当たり判定 (汎用)
	COLLISIONTYPE_MAX,			// この列挙型の総数
} COLLISIONTYPE;

//**********************************************************************************************************************
//	列挙型定義 (BREAKTYPE)
//**********************************************************************************************************************
typedef enum
{
	BREAKTYPE_NONE = 0,			// 壊れない種類
	BREAKTYPE_ON,				// 壊れる種類
	BREAKTYPE_SMASH,			// 吹っ飛ぶ種類
	BREAKTYPE_MAX,				// この列挙型の総数
} BREAKTYPE;

//**********************************************************************************************************************
//	列挙型定義 (SMASHSTATE)
//**********************************************************************************************************************
typedef enum
{
	SMASHSTATE_NONE = 0,		// 何もない状態
	SMASHSTATE_ON,				// 吹っ飛び中状態
	SMASHSTATE_MAX,				// この列挙型の総数
} SMASHSTATE;

//**********************************************************************************************************************
//	列挙型定義 (APPEARSTATE)
//**********************************************************************************************************************
typedef enum
{
	APPEARSTATE_NONE = 0,		// 一瞬で出現
	APPEARSTATE_SLOWLY,			// 徐々に出現
	APPEARSTATE_COMPLETE,		// 出現完了
	APPEARSTATE_MAX,			// この列挙型の総数
} APPEARSTATE;

//**********************************************************************************************************************
//	構造体定義 (Collision)
//**********************************************************************************************************************
typedef struct
{
	int         nNumColl;				// 当たり判定の総数
	D3DXVECTOR3 vecPos[MAX_COLLISION];	// 位置ベクトル
	D3DXVECTOR3 scale[MAX_COLLISION];	// 拡大率
	float       fWidth[MAX_COLLISION];	// 横幅
	float       fDepth[MAX_COLLISION];	// 奥行
}Collision;

//**********************************************************************************************************************
//	構造体定義 (Coll_Info)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 vecPos[MAX_COLLISION];	// 位置ベクトル
	ROTSTATE    stateRot;				// 向き状態
	float       fWidth[MAX_COLLISION];	// 横幅
	float       fDepth[MAX_COLLISION];	// 奥行
}Coll_Info;

//**********************************************************************************************************************
//	構造体定義 (Smash_Object)
//**********************************************************************************************************************
typedef struct
{
	SMASHSTATE State;		// 状態
	int nCounter;			// カウンター
	D3DXVECTOR3 move;		// 移動量
	bool bJump;				// ジャンプの状態
	int nSmashCount;		// 吹っ飛ばしカウント
	D3DXVECTOR3 rotMove;	// 向きの移動量
}Smash_Object;

//**********************************************************************************************************************
//	構造体定義 (Appear)
//**********************************************************************************************************************
typedef struct
{
	APPEARSTATE state;		// 出現状態
	D3DXVECTOR3 scaleCopy;	// 拡大率のコピー
	D3DXVECTOR3 scaleAdd;	// 拡大率の加算数
	float fAlpha;			// 透明度
}Appear;

//**********************************************************************************************************************
//	構造体定義 (Object)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3  pos;					// 位置
	D3DXVECTOR3  rot;					// 向き
	D3DXVECTOR3  scale;					// 拡大率
	D3DXMATRIX   mtxWorld;				// ワールドマトリックス
	Model        modelData;				// モデル情報
	D3DXMATERIAL matCopy[MAX_MATERIAL];	// マテリアルのコピー
	ACTIONSTATE  state;					// 状態
	Coll_Info    collInfo;				// 当たり判定情報
	Smash_Object smash;					// 吹っ飛び状態
	Judge		 judge;					// ジャッジ
	Appear		 appear;				// 出現関係
	IconInfo	 icon;					// アイコン関係の情報
	int          nLife;					// 体力
	int          nCollisionType;		// 当たり判定の種類
	int          nShadowType;			// 影の種類
	int          nBreakType;			// 壊れ方の種類
	int          nType;					// オブジェクトの種類
	int          nCounterState;			// 状態管理カウンター
	int          nShadowID;				// 影のインデックス
	int          nNotaID;				// 強調表示のインデックス
	bool         bUse;					// 使用状況

#ifdef _DEBUG	// デバッグ処理
	OBJECTSTATE  editState;				// エディット時の状態
#endif
}Object;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void InitObject(void);			// オブジェクトの初期化処理
void UninitObject(void);		// オブジェクトの終了処理
void UpdateObject(void);		// オブジェクトの更新処理
void DrawObject(void);			// オブジェクトの描画処理

void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXMATERIAL *pMat, int nType, int nBreakType, int nShadowType, int nCollisionType, ROTSTATE stateRot, APPEARSTATE appear, int nJudge);	// オブジェクトの設定処理
void HitObject(Object *pObject, int nDamage);																					// オブジェクトのダメージ判定
void CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pOldPos, D3DXVECTOR3 *pMove, float fWidth, float fDepth, int *pTraCnt, BOOSTSTATE state, POLICESTATE *pPolice, int *pTackleCnt, float *pTackleMove);	// オブジェクトとの当たり判定
void SmashCollision(D3DXVECTOR3 pos, float fRadius, float fSpeed);				// 吹っ飛ぶオブジェクトとの当たり判定

Object *GetObjectData(void);	// オブジェクトの取得処理
Collision *GetCollision(void);	// 当たり判定の取得処理
float *GetShadowRadius(void);	// 影の半径の取得処理
bool *GetIconSet(void);			// アイコンセットの取得処理

//**********************************************************************************************************************
//	プロトタイプ宣言 (デバッグ用)
//**********************************************************************************************************************
int GetNumObject(void);			// オブジェクトの総数取得処理

#endif