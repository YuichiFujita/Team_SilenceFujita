//======================================================================================================================
//
//	パーティクル処理 [particle.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "particle.h"
#include "effect.h"
#include "player.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_PARTICLE	(128)	// パーティクルの最大数

//**********************************************************************************************************************
//	構造体定義 (Particle)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3  pos;		// 位置
	D3DXCOLOR    col;		// 色
	PARTICLETYPE type;		// 種類
	int          nSpawn;	// エフェクト数
	int          nLife;		// 寿命
	bool         bUse;		// 使用状況
}Particle;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void ParticleDamage(Particle *pParticle);		// ダメージエフェクト
void ParticleExplosion(Particle *pParticle);	// 爆発エフェクト
void ParticleBoost(Particle *pParticle);		// ブーストエフェクト
void ParticleSpark(Particle *pParticle);		// 火花エフェクト
void ParticleDust(Particle *pParticle);			// 埃エフェクト
void ParticleRainSpray(Particle *pParticle);	// 雨の水しぶきエフェクト
void ParticleItemLost(Particle *pParticle);		// アイテム消失エフェクト
void ParticlePlaySmoke(Particle *pParticle);	// プレイヤーの黒煙エフェクト
void ParticleBreakArticle(Particle *pParticle);	// 小物を破壊した時のエフェクト
void ParticleBreakObject(Particle *pParticle);	// オブジェクトを破壊した時のエフェクト
void ParticlePlayerDeath(Particle *pParticle);	// プレイヤーの死亡爆発のエフェクト
void ParticleObjectSmash(Particle *pParticle);	// オブジェクトを吹き飛ばした時のエフェクト
void ParticleExhaustGas(Particle *pParticle);	// 排気ガスのエフェクト
void ParticleShotBarrier(Particle *pParticle);	// バリア発射エフェクト

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
Particle g_aParticle[MAX_PARTICLE];	// パーティクルの情報

//======================================================================================================================
//	パーティクルの初期化処理
//======================================================================================================================
void InitParticle(void)
{
	// パーティクルの情報の初期化
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{ // パーティクルの最大表示数分繰り返す

		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色

		g_aParticle[nCntParticle].type   = PARTICLETYPE_NONE;	// 種類
		g_aParticle[nCntParticle].nSpawn = 0;					// エフェクト数
		g_aParticle[nCntParticle].nLife  = 0;					// 寿命
		g_aParticle[nCntParticle].bUse   = false;				// 使用状況
	}
}

//======================================================================================================================
//	パーティクルの終了処理
//======================================================================================================================
void UninitParticle(void)
{
	
}

//======================================================================================================================
//	パーティクルの更新処理
//======================================================================================================================
void UpdateParticle(void)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{ // パーティクルの最大表示数分繰り返す

		if (g_aParticle[nCntParticle].bUse == true)
		{ // パーティクルが使用されている場合

			// 寿命を減らす
			g_aParticle[nCntParticle].nLife--;

			if (g_aParticle[nCntParticle].nLife <= 0)
			{ // 寿命が尽きた場合

				// 使用されていない状態にする
				g_aParticle[nCntParticle].bUse = false;
			}
		}
	}
}

//======================================================================================================================
//	パーティクルの描画処理
//======================================================================================================================
void DrawParticle(void)
{
	
}

//======================================================================================================================
//	パーティクルの設定処理
//======================================================================================================================
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, PARTICLETYPE type, int nSpawn, int nLife)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{ // パーティクルの最大表示数分繰り返す

		if (g_aParticle[nCntParticle].bUse == false)
		{ // パーティクルが使用されていない場合

			// 引数を代入
			g_aParticle[nCntParticle].pos    = pos;		// 位置
			g_aParticle[nCntParticle].col    = col;		// 色
			g_aParticle[nCntParticle].type   = type;	// 種類
			g_aParticle[nCntParticle].nSpawn = nSpawn;	// エフェクト数
			g_aParticle[nCntParticle].nLife  = nLife;	// 寿命

			// 使用している状態にする
			g_aParticle[nCntParticle].bUse = true;

			switch (g_aParticle[nCntParticle].type)
			{ // パーティクルの種類ごとの設定
			case PARTICLETYPE_EXPLOSION:

				// 爆発エフェクト
				ParticleExplosion(&g_aParticle[nCntParticle]);

				// 処理を抜ける
				break;

			case PARTICLETYPE_DAMAGE:

				// ダメージエフェクト
				ParticleDamage(&g_aParticle[nCntParticle]);

				// 処理を抜ける
				break;

			case PARTICLETYPE_BOOST:

				// ブーストエフェクト
				ParticleBoost(&g_aParticle[nCntParticle]);

				// 処理を抜ける
				break;

			case PARTICLETYPE_SPARK:

				// 火花エフェクト
				ParticleSpark(&g_aParticle[nCntParticle]);

				// 処理を抜ける
				break;

			case PARTICLETYPE_DUST:

				// 埃エフェクト
				ParticleDust(&g_aParticle[nCntParticle]);

				// 処理を抜ける
				break;

			case PARTICLETYPE_RAINSPRAY:

				// 水しぶきエフェクト
				ParticleRainSpray(&g_aParticle[nCntParticle]);

				// 処理を抜ける
				break;

			case PARTICLETYPE_ITEM_LOST:

				// アイテム消失エフェクト
				ParticleItemLost(&g_aParticle[nCntParticle]);

				break;

			case PARTICLETYPE_PLAY_SMOKE:

				// プレイヤーの黒煙エフェクト
				ParticlePlaySmoke(&g_aParticle[nCntParticle]);

				break;

			case PARTICLETYPE_BREAK_ARTICLE:

				// 小物を破壊した時のエフェクト
				ParticleBreakArticle(&g_aParticle[nCntParticle]);

				break;

			case PARTICLETYPE_BREAK_OBJECT:

				// オブジェクトを破壊した時のエフェクト
				ParticleBreakObject(&g_aParticle[nCntParticle]);

				break;

			case PARTICLETYPE_PLAY_DEATH:

				// プレイヤーの死亡爆発のエフェクト
				ParticlePlayerDeath(&g_aParticle[nCntParticle]);

				break;

			case PARTICLETYPE_OBJECT_SMASH:

				// オブジェクトを吹き飛ばした時のエフェクト
				ParticleObjectSmash(&g_aParticle[nCntParticle]);

				break;

			case PARTICLETYPE_EXHAUST_GAS:

				// 排気ガスのエフェクト
				ParticleExhaustGas(&g_aParticle[nCntParticle]);

				break;
				
			case PARTICLETYPE_SHOT_BARRIER:

				// バリア発射エフェクト
				ParticleShotBarrier(&g_aParticle[nCntParticle]);

				break;
			}

			// 処理を抜ける
			break;
		}
	}
}

//======================================================================================================================
//	パーティクルの削除
//======================================================================================================================
void AllFalseParticle(void)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{ // パーティクルの最大表示数分繰り返す

		// 使用していない状態にする
		g_aParticle[nCntParticle].bUse = false;
	}
}

//======================================================================================================================
//	ダメージエフェクト
//======================================================================================================================
void ParticleDamage(Particle *pParticle)
{
	// 変数を宣言
	D3DXVECTOR3 move;	// エフェクトの移動量の代入用

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // パーティクルの 1Fで生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f);
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// ベクトルを正規化
		D3DXVec3Normalize(&move, &move);

		// 移動量を乗算
		move.x *= 9.0f;
		move.y *= 9.0f;
		move.z *= 9.0f;

		// エフェクトの設定
		SetEffect
		( // 引数
			pParticle->pos,	// 位置
			move,			// 移動量
			pParticle->col,	// 色
			30,				// 寿命
			90.0f,			// 半径
			2.0f,			// 減算量 (半径)
			EFFECTTYPE_NONE	// その他
		);
	}
}

//======================================================================================================================
//	爆発エフェクト
//======================================================================================================================
void ParticleExplosion(Particle *pParticle)
{
	// 変数を宣言
	D3DXVECTOR3 move;	// エフェクトの移動量の代入用
	int nLife;			// エフェクトの寿命の代入用

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // パーティクルの 1Fで生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f);
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// ベクトルを正規化
		D3DXVec3Normalize(&move, &move);

		// 移動量を乗算
		move.x *= 4.0f;
		move.y *= 3.0f;
		move.z *= 4.0f;

		// 寿命をランダムで設定
		nLife = rand() % 30 + 50;

		// エフェクトの設定
		SetEffect
		( // 引数
			pParticle->pos,	// 位置
			move,			// 移動量
			pParticle->col,	// 色
			nLife,			// 寿命
			70.0f,			// 半径
			1.5f,			// 減算量 (半径)
			EFFECTTYPE_NONE	// その他
		);
	}
}

//======================================================================================================================
//	ブーストエフェクト
//======================================================================================================================
void ParticleBoost(Particle *pParticle)
{
	// 変数を宣言
	D3DXVECTOR3 move;	// エフェクトの移動量の代入用

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // パーティクルの 1Fで生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f);
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// ベクトルを正規化
		D3DXVec3Normalize(&move, &move);

		// 移動量を乗算
		move.x *= 5.0f;
		move.y *= 5.0f;
		move.z *= 5.0f;

		// エフェクトの設定
		SetEffect
		( // 引数
			pParticle->pos,	// 位置
			move,			// 移動量
			pParticle->col,	// 色
			6,				// 寿命
			26.0f,			// 半径
			0.2f,			// 減算量 (半径)
			EFFECTTYPE_NONE	// その他
		);
	}
}

//======================================================================================================================
// 火花エフェクト
//======================================================================================================================
void ParticleSpark(Particle *pParticle)
{
	// 変数を宣言
	D3DXVECTOR3 shiftPos;	// エフェクトのずらす位置
	D3DXVECTOR3 move;		// エフェクトの移動量の代入用
	float fRadius;			// エフェクトの半径の代入用
	int nLife;				// エフェクトの寿命の代入用
	Player *pPlayer = GetPlayer();		// プレイヤーの情報

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // パーティクルの 1Fで生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f);
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// ベクトルを正規化
		D3DXVec3Normalize(&move, &move);

		// 移動量を乗算
		move.x *= 5.0f;
		move.y *= 9.0f;
		move.z *= 5.0f;

		// ずらす位置を設定する
		shiftPos.x = (float)((rand() % 600) / 100.0f);
		shiftPos.y = (float)((rand() % 600) / 100.0f);
		shiftPos.z = (float)((rand() % 600) / 100.0f);

		// 半径をランダムに設定
		fRadius = (float)((rand() % 900) / 100.0f) + (pPlayer->move.x * 0.1f);

		// 寿命をランダムに設定
		nLife = rand() % 5 + 10;

		// エフェクトの設定
		SetEffect
		( // 引数
			pParticle->pos + shiftPos,	// 位置
			move,			// 移動量
			pParticle->col,	// 色
			nLife,			// 寿命
			fRadius,		// 半径
			0.2f,			// 減算量 (半径)
			EFFECTTYPE_SPARK// 火花
		);
	}
}

//======================================================================================================================
// 埃エフェクト
//======================================================================================================================
void ParticleDust(Particle *pParticle)
{
	// 変数を宣言
	D3DXVECTOR3 move;		// エフェクトの移動量の代入用
	float fRadius;			// エフェクトの半径の代入用
	int nLife;				// エフェクトの寿命の代入用

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // パーティクルの 1Fで生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = D3DX_PI;
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// ベクトルを正規化
		D3DXVec3Normalize(&move, &move);

		// 移動量を乗算
		move.x *= (float)(rand() % 5 - 2);
		move.y *= 4.0f;
		move.z *= (float)(rand() % 5 - 2);

		// 半径をランダムに設定
		fRadius = (float)(rand() % 200 + 50.0f);

		// 寿命を設定
		nLife = (rand() % 20) + 160;

		// エフェクトの設定
		SetEffect
		( // 引数
			pParticle->pos,		// 位置
			move,				// 移動量
			pParticle->col,		// 色
			nLife,				// 寿命
			fRadius,			// 半径
			0.1f,				// 減算量 (半径)
			EFFECTTYPE_NONE		// 火花
		);
	}
}

//======================================================================================================================
// 雨の水しぶきエフェクト
//======================================================================================================================
void ParticleRainSpray(Particle *pParticle)
{
	// 変数を宣言
	D3DXVECTOR3 shiftPos;	// エフェクトのずらす位置
	D3DXVECTOR3 move;		// エフェクトの移動量の代入用
	float fRadius;			// エフェクトの半径の代入用
	int nLife;				// エフェクトの寿命の代入用

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // パーティクルの 1Fで生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f);
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// ベクトルを正規化
		D3DXVec3Normalize(&move, &move);

		// 移動量を乗算
		move.x *= 4.0f;
		move.y *= 8.0f;
		move.z *= 4.0f;

		// ずらす位置を設定する
		shiftPos.x = (float)((rand() % 800) / 100.0f);
		shiftPos.y = (float)((rand() % 800) / 100.0f);
		shiftPos.z = (float)((rand() % 800) / 100.0f);

		// 半径をランダムに設定
		fRadius = (float)((rand() % 600) / 100.0f);

		// 寿命をランダムに設定
		nLife = rand() % 5 + 10;

		// エフェクトの設定
		SetEffect
		( // 引数
			pParticle->pos + shiftPos,	// 位置
			move,			// 移動量
			pParticle->col,	// 色
			nLife,			// 寿命
			fRadius,		// 半径
			0.2f,			// 減算量 (半径)
			EFFECTTYPE_RAINSPRAY		// 火花
		);
	}
}

//======================================================================================================================
// アイテム消失エフェクト
//======================================================================================================================
void ParticleItemLost(Particle *pParticle)
{
	// 変数を宣言
	D3DXVECTOR3 move;	// エフェクトの移動量の代入用

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // パーティクルの 1Fで生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f);
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// ベクトルを正規化
		D3DXVec3Normalize(&move, &move);

		// 移動量を乗算
		move.x *= 2.0f;
		move.y *= 2.0f;
		move.z *= 2.0f;

		// エフェクトの設定
		SetEffect
		( // 引数
			pParticle->pos,	// 位置
			move,			// 移動量
			pParticle->col,	// 色
			34,				// 寿命
			30.0f,			// 半径
			0.5f,			// 減算量 (半径)
			EFFECTTYPE_NONE	// その他
		);
	}
}

//======================================================================================================================
// プレイヤーの黒煙エフェクト
//======================================================================================================================
void ParticlePlaySmoke(Particle *pParticle)
{
	// 変数を宣言
	D3DXVECTOR3 move;		// エフェクトの移動量の代入用
	float fRadius;			// エフェクトの半径の代入用
	int nLife;				// エフェクトの寿命の代入用

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // パーティクルの 1Fで生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = D3DX_PI;
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// ベクトルを正規化
		D3DXVec3Normalize(&move, &move);

		// 移動量を乗算
		move.x *= (float)(rand() % 5 - 2);
		move.y *= (float)(rand() % 5 + 2);
		move.z *= (float)(rand() % 5 - 2);

		// 半径をランダムに設定
		fRadius = (float)(rand() % 30 + 100.0f);

		// 寿命を設定
		nLife = (rand() % 20) + 30;

		// エフェクトの設定
		SetEffect
		( // 引数
			D3DXVECTOR3
			(
				pParticle->pos.x + (rand() % 80 - 40),
				pParticle->pos.y,
				pParticle->pos.z + (rand() % 80 - 40)
			),		// 位置
			move,				// 移動量
			pParticle->col,		// 色
			nLife,				// 寿命
			fRadius,			// 半径
			-4.0f,				// 減算量 (半径)
			EFFECTTYPE_PLAY_SMOKE	// プレイヤーの黒煙
		);
	}
}

//======================================================================================================================
// 小物を破壊した時のエフェクト
//======================================================================================================================
void ParticleBreakArticle(Particle *pParticle)
{
	// 変数を宣言
	D3DXVECTOR3 move;	// エフェクトの移動量の代入用

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // パーティクルの 1Fで生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f);
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// ベクトルを正規化
		D3DXVec3Normalize(&move, &move);

		// 移動量を乗算
		move.x *= 25.0f;
		move.y *= 25.0f;
		move.z *= 25.0f;

		// エフェクトの設定
		SetEffect
		( // 引数
			pParticle->pos,	// 位置
			move,			// 移動量
			pParticle->col,	// 色
			40,				// 寿命
			110.0f,			// 半径
			-1.0f,			// 減算量 (半径)
			EFFECTTYPE_BREAKOBJECT	// その他
		);
	}
}

//======================================================================================================================
// オブジェクトを破壊した時のエフェクト
//======================================================================================================================
void ParticleBreakObject(Particle *pParticle)
{
	// 変数を宣言
	D3DXVECTOR3 move;	// エフェクトの移動量の代入用

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // パーティクルの 1Fで生成されるエフェクト数分繰り返す

	  // ベクトルをランダムに設定
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f);
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// ベクトルを正規化
		D3DXVec3Normalize(&move, &move);

		// 移動量を乗算
		move.x *= 25.0f;
		move.y *= 25.0f;
		move.z *= 25.0f;

		// エフェクトの設定
		SetEffect
		( // 引数
			pParticle->pos,	// 位置
			move,			// 移動量
			pParticle->col,	// 色
			40,				// 寿命
			220.0f,			// 半径
			-2.0f,			// 減算量 (半径)
			EFFECTTYPE_BREAKOBJECT	// その他
		);
	}
}

//======================================================================================================================
// プレイヤーの死亡爆発のエフェクト
//======================================================================================================================
void ParticlePlayerDeath(Particle *pParticle)
{
	// 変数を宣言
	D3DXVECTOR3 shiftPos;	// エフェクトのずらす位置
	D3DXVECTOR3 move;		// エフェクトの移動量の代入用
	float fRadius;			// エフェクトの半径の代入用
	int nLife;				// エフェクトの寿命の代入用

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // パーティクルの 1Fで生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f);
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// ベクトルを正規化
		D3DXVec3Normalize(&move, &move);

		// 移動量を乗算
		move.x *= 8.0f;
		move.y *= 8.0f;
		move.z *= 8.0f;

		// ずらす位置を設定する
		shiftPos.x = (float)((rand() % 600) / 100.0f);
		shiftPos.y = (float)((rand() % 600) / 100.0f);
		shiftPos.z = (float)((rand() % 600) / 100.0f);

		// 半径をランダムに設定
		fRadius = (float)((rand() % 600) / 100.0f) + 80.0f;

		// 寿命をランダムに設定
		nLife = rand() % 7 + 20;

		// エフェクトの設定
		SetEffect
		( // 引数
			pParticle->pos + shiftPos,	// 位置
			move,						// 移動量
			pParticle->col,				// 色
			nLife,						// 寿命
			fRadius,					// 半径
			0.4f,						// 減算量 (半径)
			EFFECTTYPE_EXPLOSION		// 爆発
		);
	}
}

//======================================================================================================================
// オブジェクトを吹き飛ばした時のエフェクト
//======================================================================================================================
void ParticleObjectSmash(Particle *pParticle)
{
	// 変数を宣言
	D3DXVECTOR3 move;	// エフェクトの移動量の代入用

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // パーティクルの 1Fで生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f);
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// ベクトルを正規化
		D3DXVec3Normalize(&move, &move);

		// 移動量を乗算
		move.x *= 10.0f;
		move.y *= 10.0f;
		move.z *= 10.0f;

		// エフェクトの設定
		SetEffect
		( // 引数
			pParticle->pos,	// 位置
			move,			// 移動量
			pParticle->col,	// 色
			20,				// 寿命
			30.0f,			// 半径
			-3.0f,			// 減算量 (半径)
			EFFECTTYPE_BREAKOBJECT	// その他
		);
	}
}

//======================================================================================================================
// 排気ガスのエフェクト
//======================================================================================================================
void ParticleExhaustGas(Particle *pParticle)
{
	// 変数を宣言
	D3DXVECTOR3 move;		// エフェクトの移動量の代入用
	float fRadius;			// エフェクトの半径の代入用
	int nLife;				// エフェクトの寿命の代入用

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // パーティクルの 1Fで生成されるエフェクト数分繰り返す

		//// ベクトルを正規化
		//D3DXVec3Normalize(&move, &move);

		// 移動量を乗算
		move.x = sinf(GetPlayer()->rot.y) * (float)((rand() % 4));
		move.y = (float)((rand() % 5 + 3) * 0.01f);
		move.z = cosf(GetPlayer()->rot.y) * (float)((rand() % 4));

		// 半径をランダムに設定
		fRadius = (float)(rand() % 10 + 30);

		// 寿命を設定
		nLife = (rand() % 10) + 10;

		// エフェクトの設定
		SetEffect
		( // 引数
			D3DXVECTOR3
			(
				pParticle->pos.x,
				pParticle->pos.y,
				pParticle->pos.z
			),		// 位置
			move,				// 移動量
			pParticle->col,		// 色
			nLife,				// 寿命
			fRadius,			// 半径
			-4.0f,				// 減算量 (半径)
			EFFECTTYPE_BREAKOBJECT	// 破壊の煙
		);
	}
}

//======================================================================================================================
//	バリア発射エフェクト
//======================================================================================================================
void ParticleShotBarrier(Particle *pParticle)
{
	// 変数を宣言
	D3DXVECTOR3 move;	// エフェクトの移動量の代入用

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // パーティクルの 1Fで生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f);
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// ベクトルを正規化
		D3DXVec3Normalize(&move, &move);

		// 移動量を乗算
		move.x *= 4.0f;
		move.y *= 4.0f;
		move.z *= 4.0f;

		// エフェクトの設定
		SetEffect
		( // 引数
			pParticle->pos,	// 位置
			move,			// 移動量
			pParticle->col,	// 色
			30,				// 寿命
			40.0f,			// 半径
			1.2f,			// 減算量 (半径)
			EFFECTTYPE_NONE	// その他
		);
	}
}

#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================
//**********************************************************************************************************************
//	パーティクルの総数取得処理
//**********************************************************************************************************************
int GetNumParticle(void)
{
	// 変数を宣言
	int nNumParticle = 0;	// パーティクルの総数の確認用

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{ // パーティクルの最大表示数分繰り返す

		if (g_aParticle[nCntParticle].bUse == true)
		{ // パーティクルが使用されている場合

			// カウンターを加算
			nNumParticle++;
		}
	}

	// 変数の値を返す
	return nNumParticle;	// パーティクルの総数
}
#endif