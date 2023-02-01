//======================================================================================================================
//
//	パーティクルヘッダー [particle.h]
//	Author：藤田勇一
//
//======================================================================================================================
#ifndef _PARTICLE_H_			// このマクロ定義がされていない場合
#define _PARTICLE_H_			// 二重インクルード防止のマクロを定義する

//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define SPAWN_PARTICLE_EXPLOSION	(8)		// パーティクル (爆発) のエフェクト数
#define SPAWN_PARTICLE_DAMAGE		(6)		// パーティクル (ダメージ) のエフェクト数

//**********************************************************************************************************************
//	列挙型定義 (PARTICLETYPE)
//**********************************************************************************************************************
typedef enum
{
	PARTICLETYPE_NONE = 0,		// パーティクル (何もしない)
	PARTICLETYPE_EXPLOSION,		// パーティクル (爆発)
	PARTICLETYPE_DAMAGE,		// パーティクル (ダメージ)
	PARTICLETYPE_BOOST,			// パーティクル (ブースト)
	PARTICLETYPE_SPARK,			// パーティクル（火花）
	PARTICLETYPE_MAX,			// この列挙型の総数
} PARTICLETYPE;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void InitParticle(void);		// パーティクルの初期化処理
void UninitParticle(void);		// パーティクルの終了処理
void UpdateParticle(void);		// パーティクルの更新処理
void DrawParticle(void);		// パーティクルの描画処理
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, PARTICLETYPE type, int nSpawn, int nLife);		// パーティクルの設定処理

//**********************************************************************************************************************
//	プロトタイプ宣言 (デバッグ用)
//**********************************************************************************************************************
int GetNumParticle(void);		// パーティクルの総数取得処理

#endif