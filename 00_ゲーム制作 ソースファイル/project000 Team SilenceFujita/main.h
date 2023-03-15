//======================================================================================================================
//
//	メインヘッダー [main.h]
//	Author：藤田勇一
//
//======================================================================================================================
#ifndef _MAIN_H_							// このマクロ定義がされていない場合
#define _MAIN_H_							// 二重インクルード防止のマクロを定義する

//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include <windows.h>
#include <mmdeviceapi.h>					// 音量調節に必要
#include <endpointvolume.h>					// 音量調節に必要
#include <stdio.h>
#include <locale.h>							// 音量調節に必要
#include <tchar.h>							// 音量調節に必要
#include <time.h>
#include "d3dx9.h"							// 描画処理に必要
#define  DIRECTINPUT_VERSION	(0x0800)	// ビルド時の警告対処用マクロ
#include "dinput.h"							// 入力処理に必要
#include "xaudio2.h"						// サウンド処理に必要
#include "Xinput.h"							// パッド使用に必要
#include "resource.h"						// アイコン使用に必要

//**********************************************************************************************************************
//	ライブラリリンク
//**********************************************************************************************************************
#pragma comment(lib, "d3d9.lib")			// 描画処理に必要
#pragma comment(lib, "d3dx9.lib")			// [d3d9.lib]の拡張ライブラリ
#pragma comment(lib, "dsound.lib")			// 音関係に必要
#pragma comment(lib, "dxguid.lib")			// DirectXコンポーネント (部品) 使用に必要
#pragma comment(lib, "winmm.lib")			// システム時刻取得に必要
#pragma comment(lib, "dinput8.lib")			// 入力処理に必要
#pragma comment(lib, "xinput.lib")			// パッド使用に必要

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)				// 頂点フォーマット [2D]
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// 頂点フォーマット [3D]

#define TUTORIAL_STAGE_SETUP_TXT	"data\\TXT\\stageTutorial.txt"	// チュートリアルのステージセットアップ用のテキストファイルの相対パス
#define GAME_STAGE_SETUP_TXT		"data\\TXT\\stageGame.txt"		// ゲームのステージセットアップ用のテキストファイルの相対パス
#define COLL_SETUP_TXT				"data\\TXT\\collision.txt"		// 当たり判定セットアップ用のテキストファイルの相対パス

#define SCREEN_WIDTH	(1280)	// ウインドウの幅
#define SCREEN_HEIGHT	(720)	// ウインドウの高さ
#define MAX_STRING		(128)	// セットアップ時のテキスト読み込み文字列の最長
#define MAX_MATERIAL	(30)	// マテリアルの最大数

#define NONE_ID			(-1)	// インデックスの初期値 (インデックスがない場合)
#define NONE_PARENT		(-1)	// 親の初期値 (親がいない場合)

//**********************************************************************************************************************
//	列挙型定義 (MODE)
//**********************************************************************************************************************
typedef enum
{
	MODE_LOGO = 0,			// ロゴ画面
	MODE_TITLE,				// タイトル画面
	MODE_TUTORIAL,			// チュートリアル画面
	MODE_GAME,				// ゲーム画面
	MODE_RESULT,			// リザルト画面
	MODE_RANKING,			// ランキング画面
	MODE_MAX,				// この列挙型の総数
} MODE;

//**********************************************************************************************************************
//	列挙型定義 (TITLESTATE)
//**********************************************************************************************************************
typedef enum
{
	TITLESTATE_NONE = 0,	// 何もしていない状態
	TITLESTATE_MOVE,		// タイトル移動状態
	TITLESTATE_FADE,		// タイトル遷移状態
	TITLESTATE_MAX,			// この列挙型の総数
} TITLESTATE;

//**********************************************************************************************************************
//	列挙型定義 (ALPHASTATE)
//**********************************************************************************************************************
typedef enum
{
	ALPHASTATE_NONE = 0,	// 何もしていない状態
	ALPHASTATE_CHANGE,		// 透明度の変更状態
	ALPHASTATE_MAX,			// この列挙型の総数
} ALPHASTATE;

//**********************************************************************************************************************
//	列挙型定義 (ACTIONSTATE)
//**********************************************************************************************************************
typedef enum
{
	ACTIONSTATE_NONE = 0,	// 何もしない状態
	ACTIONSTATE_SPAWN,		// 出現状態
	ACTIONSTATE_NORMAL,		// 通常状態
	ACTIONSTATE_ATTACK,		// 攻撃状態
	ACTIONSTATE_DAMAGE,		// ダメージ状態
	ACTIONSTATE_UNRIVALED,	// 無敵状態
	ACTIONSTATE_GROW,		// 成長状態
	ACTIONSTATE_MAX,		// この列挙型の総数
} ACTIONSTATE;

//**********************************************************************************************************************
//	列挙型定義(MOVETYPE)
//**********************************************************************************************************************
typedef enum
{
	MOVETYPE_STOP = 0,		// 停止状態
	MOVETYPE_MOVE,			// 移動状態
	MOVETYPE_MAX			// この列挙型の総数
}MOVETYPE;

//**********************************************************************************************************************
//	列挙型定義 (ROTSTATE)
//**********************************************************************************************************************
typedef enum
{
	ROTSTATE_0 = 0,				// 向き 0度 (360度)
	ROTSTATE_90,				// 向き 90度
	ROTSTATE_180,				// 向き 180度
	ROTSTATE_270,				// 向き 270度
	ROTSTATE_MAX,				// この列挙型の総数
} ROTSTATE;

//**********************************************************************************************************************
//	構造体定義 (StageLimit)
//**********************************************************************************************************************
typedef struct
{
	float fNear;			// 移動の制限位置 (手前)
	float fFar;				// 移動の制限位置 (奥)
	float fRight;			// 移動の制限位置 (右)
	float fLeft;			// 移動の制限位置 (左)
	float fField;			// 移動の制限位置 (地面)
}StageLimit;

//**********************************************************************************************************************
//	構造体定義 ( 頂点情報 [2D] )
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 頂点座標
	float       rhw;		// 座標変換用係数 (1.0f で固定)
	D3DCOLOR    col;		// 頂点カラー
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_2D;

//**********************************************************************************************************************
//	構造体定義 ( 頂点情報 [3D] )
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 頂点座標
	D3DXVECTOR3 nor;		// 法線ベクトル
	D3DCOLOR    col;		// 頂点カラー
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_3D;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void TxtSetStage(void);					// ステージのセットアップ処理
void TxtSetObject(void);				// オブジェクトのセットアップ処理
void TxtSetAI(void);					// AI系のセットアップ処理
void TxtSetCollision(void);				// 当たり判定のセットアップ処理
void TxtSetShadow(void);				// 影の半径のセットアップ処理
void TxtSetIcon(void);					// アイコンのセットアップ処理

void InitCurveInfo(void);				// カーブの情報の初期化処理
void LoadCurveTxt(void);				// カーブテキストのロード処理
void SetCurvePoint(void);				// 曲がり角の設定処理
void LoadHumanCurveTxt(void);			// 人間のルートのロード処理

void SetMode(MODE mode);				// モードの設定処理
MODE GetMode(void);						// モードの取得処理
void SetLimitStage(StageLimit limit);	// ステージの移動範囲の設定処理
StageLimit GetLimitStage(void);			// ステージの移動範囲の取得処理
LPDIRECT3DDEVICE9 GetDevice(void);		// デバイスの取得処理
int GetGateNum(void);					// ゲートの数の取得処理

#endif