//============================================================
//
//	プレイヤーヘッダー [player.h]
//	Author：藤田勇一
//
//============================================================
#ifndef _PLAYER_H_	// このマクロ定義がされていない場合
#define _PLAYER_H_	// 二重インクルード防止のマクロを定義する

//************************************************************
//	インクルードファイル
//************************************************************
#include "model.h"

//************************************************************
//	マクロ定義
//************************************************************
#define PLAY_WIDTH			(18.0f)	// プレイヤーの横幅 / 2
#define PLAY_HEIGHT			(55.0f)	// プレイヤーの縦幅
#define PLAY_DEPTH			(18.0f)	// プレイヤーの奥行 / 2

#define PLAY_LIFE			(100)	// プレイヤーの体力
#define MAX_FORWARD			(35.0f)	// 前進時の最高速度

#define DAMAGE_TIME_PLAY	(30)					// ダメージ状態を保つ時間
#define UNR_TIME_PLAY		(DAMAGE_TIME_PLAY - 10)	// 無敵状態に変更する時間

//************************************************************
//	カメラの列挙 (PlayerCamera)
//************************************************************
typedef enum
{
	PLAYCAMESTATE_NORMAL = 0,		// 通常のカメラ
	PLAYCAMESTATE_BACK,				// 一人称カメラ
	PLAYCAMESTATE_MAX				// この列挙型の総数
}PLAYCAMESTATE;

//************************************************************
//	構造体定義 (Player)
//************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// 現在の位置
	D3DXVECTOR3 oldPos;				// 前回の位置
	D3DXVECTOR3 move;				// 移動量
	D3DXVECTOR3 rot;				// 現在の向き
	D3DXVECTOR3 destRot;			// 目標の向き
	D3DXMATRIX  mtxWorld;			// ワールドマトリックス
	ACTIONSTATE state;				// プレイヤーの状態
	Model       modelData;			// モデル情報
	int         nLife;				// 体力
	int         nCounterState;		// 状態管理カウンター
	int         nShadowID;			// 影のインデックス
	int			nCameraState;		// 前向きカメラの状態
	bool		bCameraFirst;		// 一人称カメラの状況
	bool        bMove;				// 移動状況
	bool        bJump;				// ジャンプ状況
	bool        bUse;				// 使用状況
}Player;

//************************************************************
//	プロトタイプ宣言
//************************************************************
void InitPlayer(void);		// プレイヤーの初期化処理
void UninitPlayer(void);	// プレイヤーの終了処理
void UpdatePlayer(void);	// プレイヤーの更新処理
void DrawPlayer(void);		// プレイヤーの描画処理

void SetPositionPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// プレイヤーの位置・向きの設定処理
void HealPlayer(Player *pPlayer, int nHeal);				// プレイヤーの回復判定
void HitPlayer(Player *pPlayer, int nDamage);				// プレイヤーのダメージ判定
Player *GetPlayer(void);									// プレイヤーの取得処理

//************************************************************
//	プロトタイプ宣言 (デバッグ用)
//************************************************************

#endif