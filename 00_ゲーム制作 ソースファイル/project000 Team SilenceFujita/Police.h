//======================================================================================================================
//
//	警察ヘッダー [Police.h]
//	Author：小原立暉
//
//======================================================================================================================
#ifndef _POLICE_H_						// このマクロ定義がされていない場合
#define _POLICE_H_						// 二重インクルード防止のマクロを定義する

//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "model.h"
#include "bomb.h"
#include "curve.h"
#include "icon.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_POLICE			(128)		// 使用する警察の最大数
#define POLICAR_WIDTH		(45.0f)		// パトカーの縦幅
#define POLICAR_DEPTH		(45.0f)		// パトカーの奥行

//**********************************************************************************************************************
//	警察車両の状態
//**********************************************************************************************************************
typedef enum
{
	POLICESTATE_SPAWN = 0,				// 出現
	POLICESTATE_PATROL,					// パトロール
	POLICESTATE_CHASE,					// 追跡処理
	POLICESTATE_PATBACK,				// パトロールに戻るときの処理
	POLICESTATE_POSBACK,				// 最初の座標に戻る
	POLICESTATE_TACKLE,					// タックル状態
	POLICESTATE_TRAFFIC,				// 渋滞状態
	POLICESTATE_WAIT,					// 待機状態
	POLICESTATE_MAX						// この列挙型の総数
}POLICESTATE;

//**********************************************************************************************************************
//	タックルの状態
//**********************************************************************************************************************
typedef enum
{
	TACKLESTATE_CHARGE = 0,				// チャージ中
	TACKLESTATE_BOOST,					// 射出
	TACKLESTATE_MAX						// この列挙型の総数
}TACKLESTATE;

//**********************************************************************************************************************
//	構造体定義 (PoliceTackle)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 tacklemove;				// タックル時の追加移動量
	TACKLESTATE tackleState;			// タックル時の状態
	int nTackleCnt;						// タックル状態に移行する
}PoliTackle;

//**********************************************************************************************************************
//	構造体定義 (Police)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;					// 現在の位置
	D3DXVECTOR3 posOld;					// 前回の位置
	D3DXVECTOR3 move;					// 移動量
	D3DXVECTOR3 rot;					// 向き
	D3DXVECTOR3 rotDest;				// 目標の向き
	D3DXMATRIX  mtxWorld;				// ワールドマトリックス
	Model       modelData;				// モデル情報
	POLICESTATE state;					// 警察車両の状態
	BOMBSTATE   bombState;				// ボムの状態
	int			nShadowID;				// 影のインデックス
	IconInfo	icon;					// アイコンの情報
	int			nLife;					// 寿命
	bool		bMove;					// 移動しているかどうか
	bool		bJump;					// ジャンプの状況
	bool		bUse;					// 使用しているか
	D3DXMATERIAL MatCopy[MAX_MATERIAL];	// マテリアルのコピー
	CARCURVE	policeCurve;			// 曲がり角関係の情報
	CARCURVE	policeCurveCopy;		// 曲がり角の情報のコピー
	PoliTackle  tackle;					// タックル関係の変数
	int			nTrafficCnt;			// 渋滞カウント
	float		fAlpha;					// 透明度
	int			nNumSpawnGate;			// 出てくるゲートの番号
}Police;

//**********************************************************************************************************************
//	構造体定義(Reinforce)
//**********************************************************************************************************************
typedef struct
{
	int nBonus;							// 得点が入った回数
}Reinforce;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void InitPolice(void);					// 警察の初期化処理
void UninitPolice(void);				// 警察の終了処理
void UpdatePolice(void);				// 警察の更新処理
void DrawPolice(void);					// 警察の描画処理
void SetPolice(D3DXVECTOR3 pos);		// 警察の設定処理
void HitPolice(Police *pPolice, int nDamage);	// 警察のダメージ判定
void PatrolCarSearch(Police *pPolice);			// 警察車両の探知処理
Police *GetPoliceData(void);					// 警察の取得処理
void AddPolice(void);					// 警察の追加処理
Reinforce *GetReinforce(void);			// 増援情報の取得処理

//**********************************************************************************************************************
//	プロトタイプ宣言 (デバッグ用)
//**********************************************************************************************************************
int GetNumPolice(void);					// オブジェクトの総数取得処理

#endif