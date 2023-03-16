//======================================================================================================================
//
//	サウンドヘッダー [sound.h]
//	Author：藤田勇一
//
//======================================================================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"

//**********************************************************************************************************************
//	列挙型定義 (SOUND_LABEL)
//**********************************************************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_ = 0,				// BGM
	SOUND_LABEL_BGM_GAME_000,			// ゲーム内BGM_000
	SOUND_LABEL_BGM_GAME_001,			// ゲーム内BGM_001
	SOUND_LABEL_BGM_TITLE_000,			// タイトル内BGM_000
	SOUND_LABEL_BGM_RESULT_000,			// リザルト内BGM_000
	SOUND_LABEL_BGM_TUTORIAL_000,		// チュートリアル内BGM_000
	SOUND_LABEL_BGM_FIRECAR_000,		// 消防車の効果音BGM_000
	SOUND_LABEL_BGM_YAKIIMO_000,		// 石焼き芋の効果音BGM_000
	SOUND_LABEL_BGM_BOUSOUCAR_000,		// 暴走車の効果音BGM_000
	SOUND_LABEL_BGM_SENKYOCAR_000,		// 選挙カーの効果音BGM_000
	SOUND_LABEL_BGM_POLICE_000,			// 警察の効果音BGM_000
	SOUND_LABEL_BGM_ABILITY_WIND_000,	// プレイヤーの能力（送風機）の効果音BGM_000
	SOUND_LABEL_BGM_CAR_000,			// 車走行音のSE_000
	SOUND_LABEL_SE_,					// SE
	SOUND_LABEL_SE_SELECT_000,			// 選択のSE
	SOUND_LABEL_SE_DECIDE_000,			// 決定のSE
	SOUND_LABEL_SE_START_COUNT_000,		// スタートのカウントダウン（ピッ！）のSE_000
	SOUND_LABEL_SE_START_GO_000,		// 開始時のスタート（ピー！）のSE_000
	SOUND_LABEL_SE_START_SLIDE_000,		// スタートスライド（シュッ）のSE_000
	SOUND_LABEL_SE_START_LANDING_000,	// スタートシグナルの着地の音（ズドン）のSE_000
	SOUND_LABEL_SE_ABILITY_BOOST_000,	// プレイヤーの能力（ブースト）のBGM_000
	SOUND_LABEL_SE_ABILITY_BOMB_000,	// プレイヤーの能力（ボム）のSE_000
	SOUND_LABEL_SE_CARBRAKE_000,		// ブレーキ時のSE_000
	SOUND_LABEL_SE_BREAK_000,			// 破壊音のSE_000
	SOUND_LABEL_SE_DAMAGE_000,			// プレイヤーダメージのSE_000
	SOUND_LABEL_SE_LOST_000,			// プレイヤー死亡時のSE_000
	SOUND_LABEL_SE_FLY_000,				// 吹き飛ばし時のSE_000
	SOUND_LABEL_SE_BARRIER_000,			// バリア生成時のSE_000
	SOUND_LABEL_SE_GAREKI_000,			// 崩壊音（建物系）のSE_000
	SOUND_LABEL_SE_GAREKI_001,			// 崩壊音（小物系）のSE_001
	SOUND_LABEL_SE_SCORE_000,			// スコア獲得のSE_000
	SOUND_LABEL_SE_TUTORIAL_PAGE_000,	// チュートリアルをめくるSE_000
	SOUND_LABEL_MAX,					// この列挙型の総数
} SOUND_LABEL;

//**********************************************************************************************************************
//	列挙型定義 (サウンドの種類)
//**********************************************************************************************************************
typedef enum
{
	SOUND_TYPE_MAIN_BGM = 0,	//基本BGMのみ
	SOUND_TYPE_SUB_BGM,			//効果音系のBGMのみ
	SOUND_TYPE_SE,				//SEのみ
	SOUND_TYPE_MAX
}SOUND_TYPE;

//**********************************************************************************************************************
//	列挙型定義 (サウンドの有無の設定)
//**********************************************************************************************************************
typedef enum
{
	SOUND_SWITCH_OFF = 0,		//音なし
	SOUND_SWITCH_ON,			//全部
	SOUND_SWITCH_NO_BGM,		//BGM以外
	SOUND_SWITCH_MAX
}SOUND_SWITCH;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
HRESULT InitSound(HWND hWnd);			// サウンドの初期化処理
void UninitSound(void);					// サウンドの終了処理
HRESULT PlaySound(SOUND_LABEL label);	// サウンドの再生処理
void StopSound(SOUND_LABEL label);
void StopSound(void);
void SetSoundVolume(SOUND_LABEL label, float fdata);	// サウンドの音量調整
float GetSoundVolume(SOUND_LABEL label);				// サウンドの音量の取得
bool GetSoundType(SOUND_TYPE);							// サウンドの有無の取得

#endif
