//======================================================================================================================
//
//	カーブヘッダー [curve.h]
//	Author：小原立暉
//
//======================================================================================================================
#ifndef _CURVE_H_						// このマクロ定義がされていない場合
#define _CURVE_H_						// 二重インクルード防止のマクロを定義する

//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "model.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_CURVEPOINT		(299)		// 曲がり角の最大数
#define MAX_HUMAN_CURVE		(20)		// 人が曲がれる最大数
#define MAX_HUMAN_ROUTE		(12)		// 人が歩くルート
#define SHIFT_CAR_CURVE		(45.0f)		// 車のカーブでずらす幅
#define SHIFT_HUMAN_CURVE	(80.0f)		// 人のカーブでずらす幅
#define CURVE_SKIP_RANDOM	(3)			// スキップする回数のランダムの範囲
#define CURVE_SKIP_LEAST	(2)			// スキップする回数の最低数

//**********************************************************************************************************************
//	曲がり角で曲がる角度(CURVEANGLE)
//**********************************************************************************************************************
typedef enum
{
	CURVE_RIGHT = 0,					// 右に曲がる
	CURVE_LEFT,							// 左に曲がる
	CURVE_MAX							// この列挙型の総数
}CURVEANGLE;

//**********************************************************************************************************************
//	現在走っている状態(DASHANGLE)
//**********************************************************************************************************************
typedef enum
{
	DASH_RIGHT = 0,						// 右に走っている
	DASH_LEFT,							// 左に走っている
	DASH_FAR,							// 奥に走っている
	DASH_NEAR,							// 手前に走っている
	DASH_MAX							// この列挙型の総数
}DASHANGLE;

//===========================
// 車系
//===========================

//**********************************************************************************************************************
//	列挙型定義(CARACT)
//**********************************************************************************************************************
typedef enum
{
	CARACT_DASH = 0,					// 通っている
	CARACT_CURVE,						// カーブする
	CARACT_MAX							// この列挙型の総数
}CARACTIONSTATE;

//**********************************************************************************************************************
//	曲がり角の情報の構造体
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;					// 曲がり角の位置
	CURVEANGLE curveAngle;				// カーブ出来る方向
	int nCurveNumber;					// カーブの番号
	DASHANGLE dashAngle;				// 走っている方向
	bool bDeadEnd;						// 行き止まりかどうか
}CURVEINFO;

//**********************************************************************************************************************
//	構造体定義 (CARCURVE)
//**********************************************************************************************************************
typedef struct
{
	CURVEINFO	   curveInfo;			// 曲がり角の情報
	int			   nSKipCnt;			// スキップする回数
	CARACTIONSTATE actionState;			// 現在の行動
	D3DXVECTOR3	   rotDest;				// 目標の向き
}CARCURVE;

//===========================
// 人間系
//===========================

//**********************************************************************************************************************
//	列挙型定義(CARACT)
//**********************************************************************************************************************
typedef enum
{
	HUMANACT_WALK = 0,					// 通っている
	HUMANACT_CURVE,						// カーブする
	HUMANACT_MAX						// この列挙型の総数
}HUMANACTIONSTATE;

//**********************************************************************************************************************
//	列挙型定義(ROADPOS)
//**********************************************************************************************************************
typedef enum
{
	ROADPOS_RIGHT = 0,					// 曲がり角より右を歩いている
	ROADPOS_LEFT,						// 曲がり角より左を歩いている
	ROADPOS_MAX							// この列挙型の総数
}ROADPOS;

//**********************************************************************************************************************
//	曲がり角の構造体(HUMANCURVEINFO)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 curvePoint[MAX_HUMAN_CURVE];	// 曲がるポイント
	CURVEANGLE  curveAngle[MAX_HUMAN_CURVE];	// 曲がる方向
	int nNowCurve;								// 現在の曲がり角
	int nCurveTime;								// 曲がる回数
	DASHANGLE dashAngle[MAX_HUMAN_CURVE];		// どの方向に走っているか
	ROADPOS roadPos[MAX_HUMAN_CURVE];			// 曲がり角よりどっちを歩いているか
}HUMANCURVEINFO;

//**********************************************************************************************************************
//	構造体定義 (HUMANCURVE)
//**********************************************************************************************************************
typedef struct
{
	HUMANCURVEINFO		curveInfo;					// 曲がり角の情報
	int					nRandamRoute;				// 走るルート
	D3DXVECTOR3			rotDest;					// 目標の向き
	HUMANACTIONSTATE	actionState;				// 行動パターン
}HUMANCURVE;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void CurveInfoRotCar(CARCURVE *pCurve, D3DXVECTOR3 *rot, D3DXVECTOR3 *move, D3DXVECTOR3 *pPos);				// 車の角度更新・補正処理
CURVEINFO GetCurveInfo(int nID);		// 曲がり角の位置の取得処理

HUMANCURVEINFO GetHumanRoute(int nID);	// 人間のルートの取得処理

#endif