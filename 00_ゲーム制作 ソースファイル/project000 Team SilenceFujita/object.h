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

#ifdef _DEBUG	// デバッグ処理
#include "Editmain.h"
#endif

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_OBJECT			(256)		// 使用するモデル数 (オブジェクトの最大数)

//**********************************************************************************************************************
//	列挙型定義 (BREAKTYPE)
//**********************************************************************************************************************
typedef enum
{
	BREAKTYPE_NONE = 0,					// 壊れない種類
	BREAKTYPE_ON,						// 壊れる種類
	BREAKTYPE_MAX,						// この列挙型の総数
} BREAKTYPE;

//**********************************************************************************************************************
//	構造体定義 (Object)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 rot;					// 向き
	D3DXVECTOR3 scale;					// 拡大率
	D3DXMATRIX  mtxWorld;				// ワールドマトリックス
	Model      *pModelData;				// モデル情報
	ACTIONSTATE state;					// 状態
	OBJECTSTATE editState;				// エディット時の状態
	int         nLife;					// 体力
	int         nBreakType;				// 壊れ方の種類
	int         nType;					// オブジェクトの種類
	float       fPosBlur;				// オブジェクトの位置のブレ
	int         nCounterState;			// 状態管理カウンター
	int         nShadowID;				// 影のインデックス
	bool        bUse;					// 使用状況
}Object;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void InitObject(void);					// オブジェクトの初期化処理
void UninitObject(void);				// オブジェクトの終了処理
void UpdateObject(void);				// オブジェクトの更新処理
void DrawObject(void);					// オブジェクトの描画処理

void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXMATERIAL *pMat, int nBreakType, int nType);	// オブジェクトの設定処理

void HitObject(Object *pObject, int nDamage);												// オブジェクトのダメージ判定
void CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pOldPos, float fWidth, float fDepth);	// オブジェクトとの当たり判定
Object *GetObjectData(void);																// オブジェクトの取得処理

//**********************************************************************************************************************
//	プロトタイプ宣言 (デバッグ用)
//**********************************************************************************************************************
int GetNumObject(void);					// オブジェクトの総数取得処理

#endif