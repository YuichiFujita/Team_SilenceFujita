//======================================================================================================================
//
//	爆弾処理 [bomb.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "bomb.h"
#include "calculation.h"
#include "player.h"
#include "Police.h"

#include "input.h"
#include "particle.h"
#include "object.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_BOMB		(128)		// 爆弾の範囲内表示の最大数

#define BOMB_PULS		(400.0f)	// プレイヤー位置からボム検知の中心位置へのずれ
#define BOMB_RADIUS		(400.0f)	// ボム検知範囲の半径
#define BOMB_CAR_RADIUS	(80.0f)		// 車の検知サイズの半径

#define BOMB_FRONT_PLUS	(100.0f)				// プレイヤーの前方位置の計算用
#define BOMB_LEFT_PLUS	(BOMB_RADIUS + 100.0f)	// 車の位置関係の設定用ベクトルの計算用

//**********************************************************************************************************************
//	列挙型定義 (CARTYPE)
//**********************************************************************************************************************
typedef enum
{
	CARTYPE_NORMAL = 0,		// 通常車
	CARTYPE_POLICE,			// 警察車
	CARTYPE_MAX,			// この列挙型の総数
} CARTYPE;

//**********************************************************************************************************************
//	構造体定義 (Bomb)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	BOMBSTATE   state;		// 状態
	CARTYPE     type;		// 種類
	void       *pCar;		// 車アドレス
	float       fOrder;		// 判定距離
	bool        bUse;		// 使用状況
}Bomb;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void SetBomb(D3DXVECTOR3 pos, CARTYPE type, void *pCar);	// ボムの設定

void ResetBomb(void);		// ボムの再設定
void CollisionBomb(void);	// ボム検知範囲の当たり判定
void SortBomb(void);		// ボムの配列のソート
int  CurrentAim(void);		// 選択中の車両

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
int     g_nCurrentMaxBomb;	// 現在狙い中の車の総数
CARTYPE g_nType;			// 現在狙い中の車の種類
int     g_nID;				// 現在狙い中の車のID
void   *g_pCurrentAim;		// 現在狙い中の車のアドレス
Bomb    g_aBomb[MAX_BOMB];	// 爆弾の情報

//======================================================================================================================
//	爆弾の初期化処理
//======================================================================================================================
void InitBomb(void)
{
	// グローバル変数を初期化
	g_nCurrentMaxBomb = 0;			// 現在狙い中の車の総数
	g_nType       = CARTYPE_NORMAL;	// 現在狙い中の車の種類
	g_nID         = 0;				// 現在狙い中の車のID
	g_pCurrentAim = NULL;			// 現在狙い中の車のアドレス

	// 爆弾の情報の初期化
	for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
	{ // 爆弾の最大表示数分繰り返す

		g_aBomb[nCntBomb].pos    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aBomb[nCntBomb].state  = BOMBSTATE_NONE;					// 状態
		g_aBomb[nCntBomb].pCar   = NULL;							// 車アドレス
		g_aBomb[nCntBomb].fOrder = 0.0f;							// 判定距離
		g_aBomb[nCntBomb].bUse   = false;							// 使用状況
	}
}

//======================================================================================================================
//	爆弾の終了処理
//======================================================================================================================
void UninitBomb(void)
{
	
}

//======================================================================================================================
//	爆弾の更新処理
//======================================================================================================================
void UpdateBomb(void)
{
	// 変数を宣言
	int nAim = -1;

	// ボムの再設定
	ResetBomb();

	// ボム検知範囲の当たり判定
	CollisionBomb();

	// ボムの配列のソート
	SortBomb();

	if (g_nCurrentMaxBomb > 0)
	{

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{

			g_pCurrentAim = g_aBomb[g_nID].pCar;
			g_nID = (g_nID + 1) % g_nCurrentMaxBomb;
		}
	}

	// 選択中の車両
	nAim = CurrentAim();

	if (nAim == -1)
	{
		g_pCurrentAim = NULL;
	}

	if (g_aBomb[nAim].bUse == true)
	{ // 

		// パーティクルの設定
		SetParticle
		( // 引数
			g_aBomb[nAim].pos,					// 位置
			D3DXCOLOR(0.8f, 0.0f, 0.3f, 1.0f),	// 色
			PARTICLETYPE_DAMAGE,				// 種類
			SPAWN_PARTICLE_DAMAGE,				// エフェクト数
			2									// 寿命
		);
	}
}

//======================================================================================================================
//	爆弾の描画処理
//======================================================================================================================
void DrawBomb(void)
{
	
}

//======================================================================================================================
//	ボムの設定
//======================================================================================================================
void SetBomb(D3DXVECTOR3 pos, CARTYPE type, void *pCar)
{
	// 変数を宣言
	D3DXVECTOR3 playerPos;			// プレイヤーの位置
	D3DXVECTOR3 frontPos;			// プレイヤーの前方位置

	// ポインタを宣言
	Player *pPlayer = GetPlayer();	// プレイヤーの情報

	// プレイヤーの位置を代入
	playerPos = D3DXVECTOR3(pPlayer->pos.x, 0.0f, pPlayer->pos.z);

	// プレイヤーの前方位置を設定
	frontPos.x = pPlayer->pos.x + sinf(pPlayer->rot.y) * BOMB_FRONT_PLUS;
	frontPos.y = 0.0f;
	frontPos.z = pPlayer->pos.z + cosf(pPlayer->rot.y) * BOMB_FRONT_PLUS;

	// プレイヤーの位置、前方位置を横にずらす
	playerPos.x += sinf(pPlayer->rot.y - (D3DX_PI * 0.5f)) * BOMB_LEFT_PLUS;
	playerPos.z += cosf(pPlayer->rot.y - (D3DX_PI * 0.5f)) * BOMB_LEFT_PLUS;
	frontPos.x  += sinf(pPlayer->rot.y - (D3DX_PI * 0.5f)) * BOMB_LEFT_PLUS;
	frontPos.z  += cosf(pPlayer->rot.y - (D3DX_PI * 0.5f)) * BOMB_LEFT_PLUS;

	for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
	{ // 爆弾の最大表示数分繰り返す

		if (g_aBomb[nCntBomb].bUse == false)
		{ // 爆弾が使用されていない場合

			// 引数を設定
			g_aBomb[nCntBomb].pos  = pos;	// 位置
			g_aBomb[nCntBomb].type = type;	// 種類
			g_aBomb[nCntBomb].pCar = pCar;	// 車アドレス

			// 判定距離を求める
			g_aBomb[nCntBomb].fOrder = LineOuterProduct(frontPos, playerPos, pos);

			// 現在狙っている車の総数を加算
			g_nCurrentMaxBomb++;

			// 使用している状態にする
			g_aBomb[nCntBomb].bUse = true;

			// 処理を抜ける
			break;
		}
	}
}

//======================================================================================================================
//	ボムの再設定
//======================================================================================================================
void ResetBomb(void)
{
	// 現在狙っている車の総数を初期化
	g_nCurrentMaxBomb = 0;

	// ボムの使用状況を初期化
	for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
	{ // 爆弾の最大表示数分繰り返す

		// 判定距離を初期化
		g_aBomb[nCntBomb].fOrder = 0.0f;

		// 使用していない状態にする
		g_aBomb[nCntBomb].bUse = false;
	}
}

//======================================================================================================================
//	ボム検知範囲の当たり判定
//======================================================================================================================
void CollisionBomb(void)
{
	// 変数を宣言
	D3DXVECTOR3 rangePos;	// ボムの検知範囲の中心
	float       fLength;	// 検知範囲と検知車の距離

	// ポインタを宣言
	Player *pPlayer = GetPlayer();		// プレイヤーの情報
	Police *pPolice = GetPoliceData();	// 警察の情報
	Object *pObject = GetObjectData();	// オブジェクトの情報

	// ボムの検知範囲の中心を設定
	rangePos.x = pPlayer->pos.x + sinf(pPlayer->rot.y) * BOMB_PULS;
	rangePos.y = pPlayer->pos.y + 0.0f;
	rangePos.z = pPlayer->pos.z + cosf(pPlayer->rot.y) * BOMB_PULS;

	// 警察の検知を確認
	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++, pPolice++)
	{ // 警察の最大表示数分繰り返す

		if (pPolice->bUse == true)
		{ // 警察が使用されている場合

			// 検知範囲と検知車の距離を求める
			fLength = (rangePos.x - pPolice->pos.x) * (rangePos.x - pPolice->pos.x)
					+ (rangePos.z - pPolice->pos.z) * (rangePos.z - pPolice->pos.z);

			if (fLength < ((BOMB_RADIUS + BOMB_CAR_RADIUS) * (BOMB_RADIUS + BOMB_CAR_RADIUS)))
			{ // 検知範囲内の場合

				// ボムの設定
				SetBomb(pPolice->pos, CARTYPE_POLICE, pPolice);
			}
		}
	}

	// オブジェクトの検知を確認
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pObject++)
	{ // オブジェクトの最大表示数分繰り返す

		if (pObject->bUse == true)
		{ // オブジェクトが使用されている場合

			// 検知範囲と検知車の距離を求める
			fLength = (rangePos.x - pObject->pos.x) * (rangePos.x - pObject->pos.x)
					+ (rangePos.z - pObject->pos.z) * (rangePos.z - pObject->pos.z);

			if (fLength < ((BOMB_RADIUS + BOMB_CAR_RADIUS) * (BOMB_RADIUS + BOMB_CAR_RADIUS)))
			{ // 検知範囲内の場合

				// ボムの設定
				SetBomb(pObject->pos, CARTYPE_NORMAL, pObject);
			}
		}
	}
}

//======================================================================================================================
//	ボムの配列のソート
//======================================================================================================================
void SortBomb(void)
{
	// 変数を宣言
	Bomb keepBomb;		// ソート入れ替え時の代入用
	int  nNowMaxID;		// 最大値が入っている配列の要素番号の記憶用

	for (int nCntBomb = 0; nCntBomb < (g_nCurrentMaxBomb - 1); nCntBomb++)
	{ // 現在狙い中の車の総数 -1回分繰り返す

		// 現在の繰り返し数を代入 (要素1とする)
		nNowMaxID = nCntBomb;

		for (int nCntSort = nCntBomb + 1; nCntSort < g_nCurrentMaxBomb; nCntSort++)
		{ // 現在狙い中の車の総数分繰り返す

			if (g_aBomb[nNowMaxID].fOrder < g_aBomb[nCntSort].fOrder)
			{ // 最大値に設定されている値より、現在の値のほうが大きい場合

				// 現在の要素番号を最大値に設定
				nNowMaxID = nCntSort;
			}
		}

		if (nCntBomb != nNowMaxID)
		{ // 最大値の要素番号に変動があった場合

			// 値の入れ替え
			keepBomb           = g_aBomb[nCntBomb];
			g_aBomb[nCntBomb]  = g_aBomb[nNowMaxID];
			g_aBomb[nNowMaxID] = keepBomb;
		}

		// ここでID変更？
	}
}

//======================================================================================================================
//	選択中の車両
//======================================================================================================================
int CurrentAim(void)
{
	for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
	{ // 爆弾の最大表示数分繰り返す

		if (g_aBomb[nCntBomb].bUse == true)
		{ // 爆弾が使用されている場合

			if (g_pCurrentAim == g_aBomb[nCntBomb].pCar)
			{ // アドレスが同じ場合 (選択中の車)

				// 選択中のインデックスを返す
				return nCntBomb;
			}
		}
	}

	// 選択中のインデックスがない場合
	return -1;	// -1を返す
}

#ifdef _DEBUG	// デバッグ処理
#endif