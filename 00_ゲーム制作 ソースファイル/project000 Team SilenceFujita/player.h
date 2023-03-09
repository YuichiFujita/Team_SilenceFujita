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
#include "icon.h"

//************************************************************
//	マクロ定義
//************************************************************
#define PLAY_WIDTH			(100.0f)	// プレイヤーの横幅 / 2
#define PLAY_HEIGHT			(25.0f)		// プレイヤーの縦幅
#define PLAY_DEPTH			(100.0f)	// プレイヤーの奥行 / 2

#define MAX_FORWARD			(35.0f)						// 前進時の最高速度
#define MAX_BOOST			(15.0f)						// ブーストの最大移動量
#define MAX_REAL_SPEED		(MAX_FORWARD + MAX_BOOST)	// 数値上の最高速度

#define PLAY_REV_ROT_MIN	(0.75f)		// プレイヤーの向き変更量の減速係数の最小値
#define PLAY_LIFE			(100)		// プレイヤーの体力
#define PLAY_DAMAGE			(10)		// プレイヤーのダメージ数
#define BOOST_WAIT_CNT		(180)		// ブーストの再使用までの時間
#define WIND_OVERHEAT_CNT	(180)		// ウィンドのオーバーヒートまでの時間
#define BOMB_WAIT_CNT		(2400)		// ボムの再使用までの時間

#define DAMAGE_TIME_PLAY	(90)						// ダメージ状態を保つ時間
#define UNR_TIME_PLAY		(DAMAGE_TIME_PLAY - 10)		// 無敵状態に変更する時間

//************************************************************
//	列挙型定義 (ATTACKSTATE)
//************************************************************
typedef enum
{
	ATTACKSTATE_NONE = 0,			// 何もしない状態
	ATTACKSTATE_WAIT,				// 攻撃待機状態
	ATTACKSTATE_HEAL,				// ゲージ回復状態
	ATTACKSTATE_MAX					// この列挙型の総数
}ATTACKSTATE;

//************************************************************
//	列挙型定義 (PLAYCAMESTATE)
//************************************************************
typedef enum
{
	PLAYCAMESTATE_NORMAL = 0,		// 通常のカメラ
	PLAYCAMESTATE_BACK,				// 一人称カメラ
	PLAYCAMESTATE_MAX				// この列挙型の総数
}PLAYCAMESTATE;

//************************************************************
//	列挙型定義 (BOOSTSTATE)
//************************************************************
typedef enum
{
	BOOSTSTATE_NONE = 0,			// 何もしない状態
	BOOSTSTATE_UP,					// 加速状態
	BOOSTSTATE_DOWN,				// 減速状態
	BOOSTSTATE_WAIT,				// 使用可能の待機状態
	BOOSTSTATE_MAX					// この列挙型の総数
}BOOSTSTATE;

//************************************************************
//	構造体定義 (PlayerBoost)
//************************************************************
typedef struct
{
	D3DXVECTOR3 plusMove;			// 追加移動量
	BOOSTSTATE  state;				// 加速状態
	int         nCounter;			// 加速管理カウンター
}PlayerBoost;

//************************************************************
//	構造体定義 (PlayerWind)
//************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// 風を出す位置
	D3DXVECTOR3	rot;				// 飛ばす方向
	int			nCount;				// 風を出すカウント
	int			nCircleCount;		// 風を出す位置のカウント
	bool		bUseWind;			// 風の使用状況
}PlayerWind;

//************************************************************
//	構造体定義 (PlayerBomb)
//************************************************************
typedef struct
{
	ATTACKSTATE state;				// 攻撃状態
	int         nCounterState;		// 攻撃管理カウンター
	int         nCounterControl;	// 操作管理カウンター
	int			nHeal;				// ゲージの回復量
	bool        bShot;				// 発射待機状況
}PlayerBomb;

//************************************************************
//	構造体定義 (Player)
//************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// 現在の位置
	D3DXVECTOR3 oldPos;				// 前回の位置
	D3DXVECTOR3 move;				// 移動量
	D3DXVECTOR3 rot;				// 向き
	D3DXVECTOR3 moveRot;			// 向き変更量
	D3DXMATRIX  mtxWorld;			// ワールドマトリックス
	ACTIONSTATE state;				// プレイヤーの状態
	Model       modelData;			// モデル情報
	PlayerBoost boost;				// ブーストの情報
	PlayerWind  wind;				// 風の情報
	PlayerBomb  bomb;				// 爆弾の情報
	int         nLife;				// 体力
	int         nCounterState;		// 状態管理カウンター
	int         nShadowID;			// 影のインデックス
	int			nNumEnterGate;		// 出てくるゲート
	IconInfo	icon;				// アイコンの情報
	int			nCameraState;		// 前向きカメラの状態
	bool		bCameraFirst;		// 一人称カメラの状況
	bool        bMove;				// 移動状況
	bool        bJump;				// ジャンプ状況
	bool		bUnrivaled;			// 透明状況
	bool        bUse;				// 使用状況
}Player;

//************************************************************
//	プロトタイプ宣言
//************************************************************
void InitPlayer(void);				// プレイヤーの初期化処理
void UninitPlayer(void);			// プレイヤーの終了処理
void UpdateGamePlayer(void);		// プレイヤーのゲーム更新処理
void UpdateTutorialPlayer(void);	// プレイヤーのチュートリアル更新処理
void DrawPlayer(void);				// プレイヤーの描画処理

void SetPositionPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// プレイヤーの位置・向きの設定処理
void SetPlayerGate(void);									// プレイヤーのゲートの設定処理

void HealPlayer(Player *pPlayer, int nHeal);	// プレイヤーの回復判定
void HealBarrier(Player *pPlayer, int nHeal);	// バリアの回復判定
void HitPlayer(Player *pPlayer, int nDamage);	// プレイヤーのダメージ判定
Player *GetPlayer(void);						// プレイヤーの取得処理

//************************************************************
//	プロトタイプ宣言 (デバッグ用)
//************************************************************

#endif