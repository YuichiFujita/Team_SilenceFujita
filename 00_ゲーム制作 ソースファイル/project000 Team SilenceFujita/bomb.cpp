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
#include "input.h"

#include "bomb.h"
#include "calculation.h"
#include "player.h"
#include "car.h"
#include "police.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_BOMB		(128)		// 爆弾の範囲内表示の最大数

#define BOMB_PULS		(1200.0f)	// プレイヤー位置からボム検知の中心位置へのずれ
#define BOMB_RADIUS		(1000.0f)	// ボム検知範囲の半径
#define BOMB_CAR_RADIUS	(80.0f)		// 車の検知サイズの半径

#define BOMB_FRONT_PLUS	(100.0f)				// プレイヤーの前方位置の計算用
#define BOMB_LEFT_PLUS	(BOMB_RADIUS + 100.0f)	// 車の位置関係の設定用ベクトルの計算用

#define BOMB_XZ_ADD		(90.0f)		// ボム発射位置の xz加算量
#define BOMB_Y_ADD		(155.0f)	// ボム発射位置の y加算量

#define SHOT_BAR_POS_XZ	(5.0f)		// バリアの発射位置の調整量
#define SHOT_BAR_POS_Y	(15.0f)		// バリアの発射位置の調整量
#define HORIZONTAL_MOVE	(60.0f)		// 横の移動量
#define VERTICAL_MOVE	(45.0f)		// 縦の移動量

#define BARRIER_GRAVITY	(0.5f)		// 発射されたバリアにかかる重力
#define LAND_CNT		(5)			// 着弾時のカウント
#define REV_UP_POS		(450.0f)	// 上昇時の補正値
#define REV_DOWN_POS	(0.0f)		// 下降時の補正値
#define POS_Y_CHANGE	(1.0f)		// 位置の変更量
#define MOVEPOS_ADD		(0.2f)		// 上昇・下降時の加算量
#define DOWN_RADIUS		(240.0f)	// 下降時の下の車の確認半径
#define CARPOS_MUL		(0.9f)		// 車の位置の乗算量
#define SET_CNT			(2400)		// 完成時のカウント
#define ENLARGE_SCALE	(9.0f)		// 拡大率の補正値
#define REDUCE_SCALE	(0.05f)		// 縮小率の補正値
#define SCALE_CHANGE	(0.5f)		// 拡大率の変更量

//**********************************************************************************************************************
//	構造体定義 (Bomb)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	BOMBTYPE    type;		// 種類
	void       *pCar;		// 車アドレス
	float       fOrder;		// 判定距離
	bool        bUse;		// 使用状況
}Bomb;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void SetBomb(D3DXVECTOR3 pos, BOMBTYPE type, void *pCar);	// ボムの設定

void UpdateBombData(void);		// ボムの情報の更新
void UpdateBarrierData(void);	// バリアの情報の更新

void ResetBomb(void);		// ボムの再設定
void CollisionBomb(void);	// ボム検知範囲の当たり判定
void SortBomb(void);		// ボムの配列のソート
void ChangeAim(void);		// 狙いの変更
void CurrentAim(void);		// 選択中の車両判定
void SetAim(void);			// 狙い状態の設定

void HomingBarrier(int nCntBarrier);	// バリアのホーミング
bool DownBarrier(int nCntBarrier);		// バリアの下降

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
int     g_nCurrentMaxBomb;			// 現在狙い中の車の総数
int     g_nID;						// 現在狙い中の車のID
void   *g_pCurrentAim;				// 現在狙い中の車のアドレス
Bomb    g_aBomb[MAX_BOMB];			// 爆弾の情報
Barrier g_aBarrier[MAX_BARRIER];	// バリアの情報

//======================================================================================================================
//	爆弾の初期化処理
//======================================================================================================================
void InitBomb(void)
{
	// グローバル変数を初期化
	g_nCurrentMaxBomb = 0;			// 現在狙い中の車の総数
	g_nID             = 0;			// 現在狙い中の車のID
	g_pCurrentAim     = NULL;		// 現在狙い中の車のアドレス

	// 爆弾の情報の初期化
	for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
	{ // 爆弾の最大表示数分繰り返す

		g_aBomb[nCntBomb].pos    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aBomb[nCntBomb].type   = BOMBTYPE_CAR;					// 種類
		g_aBomb[nCntBomb].pCar   = NULL;							// 車アドレス
		g_aBomb[nCntBomb].fOrder = 0.0f;							// 判定距離
		g_aBomb[nCntBomb].bUse   = false;							// 使用状況
	}

	// バリアの情報の初期化
	for (int nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++)
	{ // バリアの最大表示数分繰り返す

		g_aBarrier[nCntBarrier].pos       = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
		g_aBarrier[nCntBarrier].pCarPos   = NULL;									// 車の位置
		g_aBarrier[nCntBarrier].move      = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
		g_aBarrier[nCntBarrier].rot       = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
		g_aBarrier[nCntBarrier].scale     = D3DXVECTOR3(1.0f, 1.0f, 1.0f);			// 拡大率
		g_aBarrier[nCntBarrier].state     = BARRIERSTATE_NONE;						// 状態
		g_aBarrier[nCntBarrier].type      = BOMBTYPE_CAR;							// 種類
		g_aBarrier[nCntBarrier].modelData = GetModelData(MODELTYPE_PLAYER_BARRIER);	// モデル情報
		g_aBarrier[nCntBarrier].pCar      = NULL;									// 車アドレス
		g_aBarrier[nCntBarrier].nCounter  = 0;										// バリアカウンター
		g_aBarrier[nCntBarrier].bUse      = false;									// 使用状況

		// アイコンの情報の初期化
		g_aBarrier[nCntBarrier].icon.nIconID = NONE_ICON;		// アイコンのインデックス
		g_aBarrier[nCntBarrier].icon.state = ICONSTATE_NONE;	// アイコンの状態
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
	// ボムの情報の更新
	UpdateBombData();

	// バリアの情報の更新
	UpdateBarrierData();
}

//======================================================================================================================
//	爆弾の描画処理
//======================================================================================================================
void DrawBomb(void)
{
	// 変数を宣言
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ

	// 頂点法線の自動正規化を有効にする
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	for (int nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++)
	{ // バリアの最大表示数分繰り返す

		if (g_aBarrier[nCntBarrier].bUse == true)
		{ // バリアが使用されている場合

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBarrier[nCntBarrier].mtxWorld);

			// 拡大率を反映
			D3DXMatrixScaling(&mtxScale, g_aBarrier[nCntBarrier].scale.x, g_aBarrier[nCntBarrier].scale.y, g_aBarrier[nCntBarrier].scale.z);
			D3DXMatrixMultiply(&g_aBarrier[nCntBarrier].mtxWorld, &g_aBarrier[nCntBarrier].mtxWorld, &mtxScale);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBarrier[nCntBarrier].rot.y, g_aBarrier[nCntBarrier].rot.x, g_aBarrier[nCntBarrier].rot.z);
			D3DXMatrixMultiply(&g_aBarrier[nCntBarrier].mtxWorld, &g_aBarrier[nCntBarrier].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBarrier[nCntBarrier].pos.x, g_aBarrier[nCntBarrier].pos.y, g_aBarrier[nCntBarrier].pos.z);
			D3DXMatrixMultiply(&g_aBarrier[nCntBarrier].mtxWorld, &g_aBarrier[nCntBarrier].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBarrier[nCntBarrier].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aBarrier[nCntBarrier].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aBarrier[nCntBarrier].modelData.dwNumMat; nCntMat++)
			{ // マテリアルの数分繰り返す

				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_aBarrier[nCntBarrier].modelData.pTexture[nCntMat]);

				// モデルの描画
				g_aBarrier[nCntBarrier].modelData.pMesh->DrawSubset(nCntMat);
			}

			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}

	// 頂点法線の自動正規化を無効にする
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//======================================================================================================================
//	バリアの発射処理
//======================================================================================================================
void ShotBarrier(void)
{
	// ポインタを宣言
	Player *pPlayer = GetPlayer();	// プレイヤーの情報
	Car    *pCar;					// 車の情報
	Police *pPolice;				// 警察の情報

	if (g_pCurrentAim != NULL)
	{ // 現在狙っている車のアドレスが NULLではない場合

		if (g_aBomb[g_nID].bUse == true)
		{ // 現在狙い中の車が使用されている場合

			for (int nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++)
			{ // バリアの最大表示数分繰り返す

				if (g_aBarrier[nCntBarrier].bUse == false)
				{ // バリアが使用されていない場合

					// 発射位置を設定
					g_aBarrier[nCntBarrier].pos.x = pPlayer->pos.x + sinf(pPlayer->rot.y) * BOMB_XZ_ADD;	// x
					g_aBarrier[nCntBarrier].pos.y = pPlayer->pos.y + BOMB_Y_ADD;							// y
					g_aBarrier[nCntBarrier].pos.z = pPlayer->pos.z + cosf(pPlayer->rot.y) * BOMB_XZ_ADD;	// z

					// 移動量を設定
					g_aBarrier[nCntBarrier].move.x = sinf(pPlayer->rot.y) * (SHOT_BAR_POS_XZ + pPlayer->move.x + pPlayer->boost.plusMove.x);	// x
					g_aBarrier[nCntBarrier].move.y = SHOT_BAR_POS_Y;																			// y
					g_aBarrier[nCntBarrier].move.z = cosf(pPlayer->rot.y) * (SHOT_BAR_POS_XZ + pPlayer->move.x + pPlayer->boost.plusMove.x);	// z

					// ボムの情報を代入
					g_aBarrier[nCntBarrier].type = g_aBomb[g_nID].type;	// 種類
					g_aBarrier[nCntBarrier].pCar = g_aBomb[g_nID].pCar;	// 車アドレス

					// 構造体の情報を初期化
					g_aBarrier[nCntBarrier].scale    = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// 拡大率
					g_aBarrier[nCntBarrier].nCounter = 0;								// バリアカウンター

					// 発射状態にする
					g_aBarrier[nCntBarrier].state = BARRIERSTATE_FLY;

					switch (g_aBomb[g_nID].type)
					{ // 現在狙い中の車の種類ごとの処理	
					case BOMBTYPE_CAR:		// 車

						// 狙っている車のアドレスを代入
						pCar = (Car*)g_aBomb[g_nID].pCar;

						// 車の位置アドレスを設定
						g_aBarrier[nCntBarrier].pCarPos = &pCar->pos;

						// 車の移動量を初期化
						pCar->move.x = 0.0f;

						// バリア接近状態にする
						pCar->bombState = BOMBSTATE_BAR_NEAR;

						// 処理を抜ける
						break;

					case BOMBTYPE_POLICE:	// 警察

						// 狙っている警察のアドレスを代入
						pPolice = (Police*)g_aBomb[g_nID].pCar;

						// 警察の位置アドレスを設定
						g_aBarrier[nCntBarrier].pCarPos = &pPolice->pos;

						// 警察の移動量を初期化
						pPolice->move.x = 0.0f;

						// バリア接近状態にする
						pPolice->bombState = BOMBSTATE_BAR_NEAR;

						// 処理を抜ける
						break;
					}

					// 攻撃モードを待機状態に変更
					pPlayer->bomb.state = ATTACKSTATE_WAIT;

					// カウンターを初期化
					pPlayer->bomb.nCounterState = 0;

					// 使用している状態にする
					g_aBarrier[nCntBarrier].bUse = true;

					// 処理を抜ける
					break;
				}
			}
		}
	}
}

//======================================================================================================================
//	バリアの取得処理
//======================================================================================================================
Barrier *GetBarrierData(void)
{
	// バリアの情報の先頭アドレスを返す
	return &g_aBarrier[0];
}

//======================================================================================================================
//	バリアの状態の取得処理
//======================================================================================================================
BARRIERSTATE GetBarrierState(void *pCar)
{
	for (int nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++)
	{ // バリアの最大表示数分繰り返す

		if (g_aBarrier[nCntBarrier].bUse == true)
		{ // バリアが使用されている場合

			if (g_aBarrier[nCntBarrier].pCar == pCar)
			{ // 引数の車アドレスとバリアの車アドレスが同じ場合

				// 現在の車のバリア状態を返す
				return g_aBarrier[nCntBarrier].state;
			}
		}
	}

	// 同じアドレスの車がなかった場合
	return BARRIERSTATE_NONE;			// 何もしない状態を返す
}

//======================================================================================================================
//	ボムの情報の更新
//======================================================================================================================
void UpdateBombData(void)
{
	if (GetPlayer()->bomb.state == ATTACKSTATE_BOMB)
	{ // プレイヤーの攻撃状態がボムの場合

		// ボムの再設定
		ResetBomb();

		// ボム検知範囲の当たり判定
		CollisionBomb();

		// ボムの配列のソート
		SortBomb();

		// 狙いの変更
		ChangeAim();

		// 選択中の車両判定
		CurrentAim();

		// 狙い状態の設定
		SetAim();
	}
	else
	{ // プレイヤーの攻撃状態がそれ以外の場合

		// 現在狙い中の車のIDを初期化
		g_nID = 0;

		// 現在狙い中の車を初期化
		g_pCurrentAim = NULL;
	}
}

//======================================================================================================================
//	バリアの情報の更新
//======================================================================================================================
void UpdateBarrierData(void)
{
	// ポインタを宣言
	Car    *pCar;		// 車の情報
	Police *pPolice;	// 警察の情報

	for (int nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++)
	{ // バリアの最大表示数分繰り返す

		if (g_aBarrier[nCntBarrier].bUse == true)
		{ // バリアが使用されている場合

			switch (g_aBarrier[nCntBarrier].state)
			{ // 状態ごとの処理
			case BARRIERSTATE_NONE:		// 何もしない状態

				// 無し

				// 処理を抜ける
				break;

			case BARRIERSTATE_FLY:		// 発射状態

				// 位置を移動
				g_aBarrier[nCntBarrier].pos += g_aBarrier[nCntBarrier].move;

				// 重力を与える
				g_aBarrier[nCntBarrier].move.y -= BARRIER_GRAVITY;

				if (g_aBarrier[nCntBarrier].move.y <= 0.0f)
				{ // y移動量が 0.0f以下の場合 (下降)

					// 移動量を初期化
					g_aBarrier[nCntBarrier].move = D3DXVECTOR3(0.0f, 0.0f, 0.0);

					// 追尾状態にする
					g_aBarrier[nCntBarrier].state = BARRIERSTATE_HOMING;
				}

				// 処理を抜ける
				break;

			case BARRIERSTATE_HOMING:	// 追尾状態

				// バリアのホーミング
				HomingBarrier(nCntBarrier);

				// 処理を抜ける
				break;

			case BARRIERSTATE_LAND:		// 着弾状態

				if (g_aBarrier[nCntBarrier].nCounter > 0)
				{ // カウンターが 0より大きい場合

					// カウンターを減算
					g_aBarrier[nCntBarrier].nCounter--;
				}
				else
				{ // カウンターが 0以下の場合

					// 拡大状態にする
					g_aBarrier[nCntBarrier].state = BARRIERSTATE_ENLARGE;

					// アイコンの設定処理
					g_aBarrier[nCntBarrier].icon.nIconID = SetIcon
					(
						g_aBarrier[nCntBarrier].pos,
						ICONTYPE_BARRIER,
						&g_aBarrier[nCntBarrier].icon.nIconID,
						&g_aBarrier[nCntBarrier].bUse,
						&g_aBarrier[nCntBarrier].icon.state
					);
				}

				// 処理を抜ける
				break;

			case BARRIERSTATE_ENLARGE:	// 拡大状態

				if (g_aBarrier[nCntBarrier].scale.x < ENLARGE_SCALE)
				{ // 拡大率が一定値より小さい場合

					// 拡大率を加算
					g_aBarrier[nCntBarrier].scale.x += SCALE_CHANGE;
					g_aBarrier[nCntBarrier].scale.y += SCALE_CHANGE;
					g_aBarrier[nCntBarrier].scale.z += SCALE_CHANGE;

					// アイコンの状態を拡大状態にする
					g_aBarrier[nCntBarrier].icon.state = ICONSTATE_ENLARGE;
				}
				else
				{ // 拡大率が一定値以上の場合

					// 拡大率を補正
					g_aBarrier[nCntBarrier].scale.x = ENLARGE_SCALE;
					g_aBarrier[nCntBarrier].scale.y = ENLARGE_SCALE;
					g_aBarrier[nCntBarrier].scale.z = ENLARGE_SCALE;

					// カウンターを設定
					g_aBarrier[nCntBarrier].nCounter = SET_CNT;

					// 上昇状態にする
					g_aBarrier[nCntBarrier].state = BARRIERSTATE_UP;

					// アイコンの状態を通常状態にする
					g_aBarrier[nCntBarrier].icon.state = ICONSTATE_NONE;
				}

				// 処理を抜ける
				break;

			case BARRIERSTATE_UP:		// 上昇状態

				if (g_aBarrier[nCntBarrier].pos.y < REV_UP_POS)
				{ // バリアの位置が一定座標より下の場合

					// 移動量を加算
					g_aBarrier[nCntBarrier].move.y += MOVEPOS_ADD;

					// 位置を上昇
					g_aBarrier[nCntBarrier].pos.y      +=  g_aBarrier[nCntBarrier].move.y + POS_Y_CHANGE;				// バリアの位置
					g_aBarrier[nCntBarrier].pCarPos->y += (g_aBarrier[nCntBarrier].move.y * CARPOS_MUL) + POS_Y_CHANGE;	// 車の位置
				}
				else
				{ // バリアの位置が一定座標以上の場合

					// 位置を補正
					g_aBarrier[nCntBarrier].pos.y      = REV_UP_POS;				// バリアの位置
					g_aBarrier[nCntBarrier].pCarPos->y = REV_UP_POS * CARPOS_MUL;	// 車の位置

					// 移動量を初期化
					g_aBarrier[nCntBarrier].move = D3DXVECTOR3(0.0f, 0.0f, 0.0);

					// 完成状態にする
					g_aBarrier[nCntBarrier].state = BARRIERSTATE_SET;
				}

				// 処理を抜ける
				break;

			case BARRIERSTATE_SET:		// 完成状態

				if (g_aBarrier[nCntBarrier].nCounter > 0)
				{ // カウンターが 0より大きい場合

					// カウンターを減算
					g_aBarrier[nCntBarrier].nCounter--;
				}
				else
				{ // カウンターが 0以下の場合

					if (DownBarrier(nCntBarrier) == true)
					{ // 下降可能だった場合

						// 下降状態にする
						g_aBarrier[nCntBarrier].state = BARRIERSTATE_DOWN;
					}
				}

				// 処理を抜ける
				break;

			case BARRIERSTATE_DOWN:		// 下降状態

				if (g_aBarrier[nCntBarrier].pos.y > REV_DOWN_POS)
				{ // バリアの位置が一定座標より上の場合

					// 移動量を加算
					g_aBarrier[nCntBarrier].move.y += MOVEPOS_ADD;

					// 位置を下降
					g_aBarrier[nCntBarrier].pos.y      -=  g_aBarrier[nCntBarrier].move.y + POS_Y_CHANGE;				// バリアの位置
					g_aBarrier[nCntBarrier].pCarPos->y -= (g_aBarrier[nCntBarrier].move.y * CARPOS_MUL) + POS_Y_CHANGE;	// 車の位置
				}
				else
				{ // バリアの位置が一定座標以下の場合

					// 位置を補正
					g_aBarrier[nCntBarrier].pos.y      = REV_DOWN_POS;	// バリアの位置
					g_aBarrier[nCntBarrier].pCarPos->y = REV_DOWN_POS;	// 車の位置

					// 移動量を初期化
					g_aBarrier[nCntBarrier].move = D3DXVECTOR3(0.0f, 0.0f, 0.0);

					// 縮小状態にする
					g_aBarrier[nCntBarrier].state = BARRIERSTATE_REDUCE;
				}

				// 処理を抜ける
				break;

			case BARRIERSTATE_REDUCE:	// 縮小状態

				if (g_aBarrier[nCntBarrier].scale.x > REDUCE_SCALE)
				{ // 拡大率が一定値より大きい場合

					// 拡大率を減算
					g_aBarrier[nCntBarrier].scale.x -= SCALE_CHANGE;
					g_aBarrier[nCntBarrier].scale.y -= SCALE_CHANGE;
					g_aBarrier[nCntBarrier].scale.z -= SCALE_CHANGE;

					// アイコンの状態を縮小状態にする
					g_aBarrier[nCntBarrier].icon.state = ICONSTATE_REDUCE;
				}
				else
				{ // 拡大率が一定値以下の場合

					// 拡大率を補正
					g_aBarrier[nCntBarrier].scale.x = REDUCE_SCALE;
					g_aBarrier[nCntBarrier].scale.y = REDUCE_SCALE;
					g_aBarrier[nCntBarrier].scale.z = REDUCE_SCALE;

					switch (g_aBarrier[nCntBarrier].type)
					{ // バリア内の車の種類ごとの処理	
					case BOMBTYPE_CAR:		// 車

						// バリア内の車のアドレスを代入
						pCar = (Car*)g_aBarrier[nCntBarrier].pCar;

						// 車の状態を通常状態にする
						pCar->state = CARSTATE_NORMAL;

						// 渋滞カウントを初期化
						pCar->nTrafficCnt = 0;

						// ボムの状態を何もしない状態にする
						pCar->bombState = BOMBSTATE_NONE;

						// 処理を抜ける
						break;

					case BOMBTYPE_POLICE:	// 警察

						// バリア内の警察のアドレスを代入
						pPolice = (Police*)g_aBarrier[nCntBarrier].pCar;

						// 警察の状態をパトロール状態にする
						pPolice->state = POLICESTATE_PATROL;

						// 渋滞カウントを初期化
						pPolice->nTrafficCnt = 0;

						// ボムの状態を何もしない状態にする
						pPolice->bombState = BOMBSTATE_NONE;

						// パトロールの探知処理
						PatrolCarSearch(pPolice);

						// 処理を抜ける
						break;
					}

					// バリアの状態を何もしない状態にする
					g_aBarrier[nCntBarrier].state = BARRIERSTATE_NONE;

					// 使用していない状態にする
					g_aBarrier[nCntBarrier].bUse = false;
				}

				// 処理を抜ける
				break;
			}

			// アイコンの位置設定処理
			SetPositionIcon
			(
				g_aBarrier[nCntBarrier].icon.nIconID, 
				g_aBarrier[nCntBarrier].pos
			);
		}
	}
}

//======================================================================================================================
//	ボムの設定
//======================================================================================================================
void SetBomb(D3DXVECTOR3 pos, BOMBTYPE type, void *pCar)
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
	Car    *pCar    = GetCarData();		// 車の情報

	// ボムの検知範囲の中心を設定
	rangePos.x = pPlayer->pos.x + sinf(pPlayer->rot.y) * BOMB_PULS;
	rangePos.y = pPlayer->pos.y + 0.0f;
	rangePos.z = pPlayer->pos.z + cosf(pPlayer->rot.y) * BOMB_PULS;

	// 車の検知を確認
	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++, pCar++)
	{ // 車の最大表示数分繰り返す

		if (pCar->bUse      == true
		&&  pCar->bombState != BOMBSTATE_BAR_NEAR
		&&  pCar->bombState != BOMBSTATE_BAR_IN)
		{ // 車が使用されている且つ、バリア関連の状態ではない場合

			// 検知範囲と検知車の距離を求める
			fLength = (rangePos.x - pCar->pos.x) * (rangePos.x - pCar->pos.x)
					+ (rangePos.z - pCar->pos.z) * (rangePos.z - pCar->pos.z);

			if (fLength < ((BOMB_RADIUS + BOMB_CAR_RADIUS) * (BOMB_RADIUS + BOMB_CAR_RADIUS)))
			{ // 検知範囲内の場合

				// ボムの状態を範囲内状態にする
				pCar->bombState = BOMBSTATE_RANGE;

				// ボムの設定
				SetBomb(pCar->pos, BOMBTYPE_CAR, pCar);
			}
			else
			{ // 検知範囲外の場合

				// ボムの状態を何もしない状態にする
				pCar->bombState = BOMBSTATE_NONE;
			}
		}
	}

	// 警察の検知を確認
	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++, pPolice++)
	{ // 警察の最大表示数分繰り返す

		if (pPolice->bUse      == true
		&&  pPolice->bombState != BOMBSTATE_BAR_NEAR
		&&  pPolice->bombState != BOMBSTATE_BAR_IN)
		{ // 警察が使用されている且つ、バリア関連の状態ではない場合

			// 検知範囲と検知車の距離を求める
			fLength = (rangePos.x - pPolice->pos.x) * (rangePos.x - pPolice->pos.x)
					+ (rangePos.z - pPolice->pos.z) * (rangePos.z - pPolice->pos.z);

			if (fLength < ((BOMB_RADIUS + BOMB_CAR_RADIUS) * (BOMB_RADIUS + BOMB_CAR_RADIUS)))
			{ // 検知範囲内の場合

				// ボムの状態を範囲内状態にする
				pPolice->bombState = BOMBSTATE_RANGE;

				// ボムの設定
				SetBomb(pPolice->pos, BOMBTYPE_POLICE, pPolice);
			}
			else
			{ // 検知範囲外の場合

				// ボムの状態を何もしない状態にする
				pPolice->bombState = BOMBSTATE_NONE;
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

			// 要素の入れ替え
			keepBomb           = g_aBomb[nCntBomb];
			g_aBomb[nCntBomb]  = g_aBomb[nNowMaxID];
			g_aBomb[nNowMaxID] = keepBomb;
		}
	}
}

//======================================================================================================================
//	狙いの変更
//======================================================================================================================
void ChangeAim(void)
{
	if (g_nCurrentMaxBomb > 0)
	{ // 狙える車が 0より多い場合

		if (GetKeyboardTrigger(DIK_M) == true || GetJoyKeyTrigger(JOYKEY_R1, 0))
		{ // 右の車へ変更する捜査が行われた場合

			// 選択している車のインデックスを変更
			g_nID = (g_nID + 1) % g_nCurrentMaxBomb;

			// 現在狙っている車のアドレスを変更
			g_pCurrentAim = g_aBomb[g_nID].pCar;
		}
		else if (GetKeyboardTrigger(DIK_N) == true || GetJoyKeyTrigger(JOYKEY_L1, 0))
		{ // 左の車へ変更する捜査が行われた場合

			// 選択している車のインデックスを変更
			g_nID = (g_nID + (g_nCurrentMaxBomb - 1)) % g_nCurrentMaxBomb;

			// 現在狙っている車のアドレスを変更
			g_pCurrentAim = g_aBomb[g_nID].pCar;
		}

		if (g_pCurrentAim == NULL)
		{ // 現在狙っている車のアドレスが NULLの場合

			// 一番左にいる車のアドレスを設定
			g_pCurrentAim = g_aBomb[0].pCar;
		}
	}
}

//======================================================================================================================
//	選択中の車両判定
//======================================================================================================================
void CurrentAim(void)
{
	// 変数を宣言
	int nAim = -1;

	for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
	{ // 爆弾の最大表示数分繰り返す

		if (g_aBomb[nCntBomb].bUse == true)
		{ // 爆弾が使用されている場合

			if (g_pCurrentAim == g_aBomb[nCntBomb].pCar)
			{ // アドレスが同じ場合 (選択中の車)

				// 現在狙い中の車のIDを保存
				nAim = nCntBomb;

				// 処理を抜ける
				break;
			}
		}
	}

	if (nAim != -1)
	{ // 現在狙い中の車と同じアドレスの車が存在した場合

		// 現在狙い中の車のIDを再設定
		g_nID = nAim;
	}
	else
	{ // 現在狙い中の車と同じアドレスの車が存在しなかった場合

		// 現在狙い中の車のIDを初期化
		g_nID = 0;

		// 現在狙い中の車を初期化
		g_pCurrentAim = NULL;
	}
}

//======================================================================================================================
//	狙い状態の設定
//======================================================================================================================
void SetAim(void)
{
	// ポインタを宣言
	Car    *pCar;		// 車の情報
	Police *pPolice;	// 警察の情報

	if (g_aBomb[g_nID].bUse == true)
	{ // 現在狙い中の車が使用されている場合

		switch (g_aBomb[g_nID].type)
		{ // 現在狙い中の車の種類ごとの処理
		case BOMBTYPE_CAR:		// 車

			// 狙っている車のアドレスを代入
			pCar = (Car*)g_aBomb[g_nID].pCar;

			// 狙っている状態にする
			pCar->bombState = BOMBSTATE_AIM;

			// 処理を抜ける
			break;

		case BOMBTYPE_POLICE:	// 警察

			// 狙っている警察のアドレスを代入
			pPolice = (Police*)g_aBomb[g_nID].pCar;

			// 狙っている状態にする
			pPolice->bombState = BOMBSTATE_AIM;

			// 処理を抜ける
			break;
		}
	}
}

//======================================================================================================================
//	バリアのホーミング
//======================================================================================================================
void HomingBarrier(int nCntBarrier)
{
	// 変数を宣言
	D3DXVECTOR3 destPos;	// ホーミング目標位置
	D3DXVECTOR2 destRot;	// ホーミング計算用

	// ポインタを宣言
	Car    *pCar;			// 車の情報
	Police *pPolice;		// 警察の情報

	// 目標位置を設定
	destPos = *g_aBarrier[nCntBarrier].pCarPos;

	// 目標の移動方向を求める
	destRot.x = atan2f(destPos.x - g_aBarrier[nCntBarrier].pos.x, destPos.z - g_aBarrier[nCntBarrier].pos.z);
	destRot.y = atan2f(destPos.x - g_aBarrier[nCntBarrier].pos.x, destPos.y - g_aBarrier[nCntBarrier].pos.y);

	// 移動量を設定
	g_aBarrier[nCntBarrier].move.x = HORIZONTAL_MOVE * sinf(destRot.x);	// x
	g_aBarrier[nCntBarrier].move.y = VERTICAL_MOVE   * cosf(destRot.y);	// y
	g_aBarrier[nCntBarrier].move.z = HORIZONTAL_MOVE * cosf(destRot.x);	// z

	// 位置を移動
	g_aBarrier[nCntBarrier].pos += g_aBarrier[nCntBarrier].move;

	if (g_aBarrier[nCntBarrier].pos.y <= 0.0f)
	{ // y座標が 0.0f以下の場合 (着弾)

		switch (g_aBomb[g_nID].type)
		{ // 現在狙い中の車の種類ごとの処理	
		case BOMBTYPE_CAR:		// 車

			// 狙っている車のアドレスを代入
			pCar = (Car*)g_aBarrier[nCntBarrier].pCar;

			// バリア内状態にする
			pCar->bombState = BOMBSTATE_BAR_IN;

			// 処理を抜ける
			break;

		case BOMBTYPE_POLICE:	// 警察

			// 狙っている警察のアドレスを代入
			pPolice = (Police*)g_aBarrier[nCntBarrier].pCar;

			// バリア内状態にする
			pPolice->bombState = BOMBSTATE_BAR_IN;

			// 処理を抜ける
			break;
		}

		// 座標を補正
		g_aBarrier[nCntBarrier].pos.x = destPos.x;
		g_aBarrier[nCntBarrier].pos.y = 0.0f;
		g_aBarrier[nCntBarrier].pos.z = destPos.z;

		// 移動量を初期化
		g_aBarrier[nCntBarrier].move = D3DXVECTOR3(0.0f, 0.0f, 0.0);

		// カウンターを設定
		g_aBarrier[nCntBarrier].nCounter = LAND_CNT;

		// 着弾状態にする
		g_aBarrier[nCntBarrier].state = BARRIERSTATE_LAND;
	}
}

//======================================================================================================================
//	バリアの下降
//======================================================================================================================
bool DownBarrier(int nCntBarrier)
{
	// 変数を宣言
	float fLength;	// 検知範囲と検知車の距離

	// ポインタを宣言
	Player *pPlayer = GetPlayer();		// プレイヤーの情報
	Police *pPolice = GetPoliceData();	// 警察の情報
	Car    *pCar    = GetCarData();		// 車の情報

	// プレイヤーの下判定
	if (pPlayer->bUse == true)
	{ // プレイヤーが使用されている場合

		// 検知範囲と検知車の距離を求める
		fLength = (g_aBarrier[nCntBarrier].pos.x - pPlayer->pos.x) * (g_aBarrier[nCntBarrier].pos.x - pPlayer->pos.x)
				+ (g_aBarrier[nCntBarrier].pos.z - pPlayer->pos.z) * (g_aBarrier[nCntBarrier].pos.z - pPlayer->pos.z);

		if (fLength < ((DOWN_RADIUS + BOMB_CAR_RADIUS) * (DOWN_RADIUS + BOMB_CAR_RADIUS)))
		{ // 検知範囲内の場合

			// プレイヤーが下にある状態を返す
			return false;
		}
	}

	// 車の下判定
	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++, pCar++)
	{ // 車の最大表示数分繰り返す

		if (pCar->bUse == true								// 車が使用されている
		&&  g_aBarrier[nCntBarrier].pCar != pCar			// 車アドレスが同じではない
		&&  GetBarrierState(pCar) != BARRIERSTATE_SET)		// 車のバリア状態が完成状態ではない
		{ // 上記を満たしている場合

			// 検知範囲と検知車の距離を求める
			fLength = (g_aBarrier[nCntBarrier].pos.x - pCar->pos.x) * (g_aBarrier[nCntBarrier].pos.x - pCar->pos.x)
					+ (g_aBarrier[nCntBarrier].pos.z - pCar->pos.z) * (g_aBarrier[nCntBarrier].pos.z - pCar->pos.z);

			if (fLength < ((DOWN_RADIUS + BOMB_CAR_RADIUS) * (DOWN_RADIUS + BOMB_CAR_RADIUS)))
			{ // 検知範囲内の場合

				// 車が下にある状態を返す
				return false;
			}
		}
	}

	// 警察の下判定
	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++, pPolice++)
	{ // 警察の最大表示数分繰り返す

		if (pPolice->bUse == true							// 警察が使用されている
		&&  g_aBarrier[nCntBarrier].pCar != pCar			// 車アドレスが同じではない
		&&  GetBarrierState(pPolice) != BARRIERSTATE_SET)	// 警察のバリア状態が完成状態ではない
		{ // 上記を満たしている場合

			// 検知範囲と検知車の距離を求める
			fLength = (g_aBarrier[nCntBarrier].pos.x - pPolice->pos.x) * (g_aBarrier[nCntBarrier].pos.x - pPolice->pos.x)
					+ (g_aBarrier[nCntBarrier].pos.z - pPolice->pos.z) * (g_aBarrier[nCntBarrier].pos.z - pPolice->pos.z);

			if (fLength < ((DOWN_RADIUS + BOMB_CAR_RADIUS) * (DOWN_RADIUS + BOMB_CAR_RADIUS)))
			{ // 検知範囲内の場合

				// 車が下にある状態を返す
				return false;
			}
		}
	}

	// 車が下にない状態を返す
	return true;
}

#ifdef _DEBUG	// デバッグ処理
#endif