//============================================================
//
//	プレイヤー処理 [player.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "input.h"
#include "game.h"
#include "calculation.h"

#include "camera.h"
#include "gate.h"
#include "object.h"
#include "particle.h"
#include "player.h"
#include "shadow.h"
#include "sound.h"
#include "life.h"
#include "bomb.h"
#include "wind.h"

#include "meshfield.h"
#include "Police.h"
#include "Car.h"
#include "tiremark.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MOVE_FORWARD	(0.1f)		// プレイヤー前進時の移動量
#define MOVE_BACKWARD	(0.2f)		// プレイヤー後退時の移動量
#define MOVE_ROT		(0.012f)	// プレイヤーの向き変更量
#define REV_MOVE_ROT	(0.085f)	// 移動量による向き変更量の補正係数
#define SUB_MOVE_VALUE	(10.0f)		// 向き変更時の減速が行われる移動量
#define SUB_MOVE		(0.15f)		// 向き変更時の減速量
#define REV_MOVE_BRAKE	(0.1f)		// ブレーキ時の減速係数
#define DEL_MOVE_ABS	(1.9f)		// 移動量の削除範囲の絶対値
#define PLAY_GRAVITY	(0.75f)		// プレイヤーにかかる重力
#define MAX_BACKWARD	(-10.0f)	// 後退時の最高速度
#define REV_MOVE_SUB	(0.08f)		// 移動量の減速係数
#define UNRIVALED_CNT	(10)		// 無敵時にチカチカさせるカウント

#define PLAY_CLEAR_MOVE		(4.0f)	// クリア成功時のプレイヤーの自動移動量
#define REV_PLAY_CLEAR_MOVE	(0.1f)	// クリア成功時のプレイヤーの減速係数
#define REV_PLAY_OVER_MOVE	(0.02f)	// クリア失敗時のプレイヤーの減速係数

//------------------------------------------------------------
//	破滅疾走 (スラム・ブースト) マクロ定義
//------------------------------------------------------------
#define BOOST_OK_MOVE	(15.0f)		// ブースト使用に必要なプレイヤーの最低速度
#define BOOST_ADD_MOVE	(0.25f)		// ブーストの加速量
#define BOOST_SUB_MOVE	(0.08f)		// ブーストの減速量
#define BOOST_UP_CNT	(180)		// ブーストの加速状態の時間

#define BOOST_XZ_SUB	(90.0f)		// ブースト噴射位置の xz減算量
#define BOOST_Y_ADD		(40.0f)		// ブースト噴射位置の y加算量
#define BOOST_SIDE_PULS	(18.0f)		// ブースト噴射位置の横位置変更量

//------------------------------------------------------------
//	吹飛散風 (フライ・アウェイ) マクロ定義
//------------------------------------------------------------
#define FLYAWAY_INTERVAL_CNT	(3)			// 風の出る間隔
#define FLYAWAY_SET_CNT			(10)		// 風の出る量
#define FLYAWAY_SHIFT_WIDTH		(90.0f)		// 風の出る位置をずらす幅
#define FLYAWAY_SHIFT_HEIGHT	(50.0f)		// 風の出る位置をずらす距離
#define FLYAWAY_OVERHEAT_CNT	(80)		// 風がオーバーヒートしたときのクールダウンまでの時間

//------------------------------------------------------------
//	無音世界 (サイレンス・ワールド) マクロ定義
//------------------------------------------------------------
#define BOMB_CNT	(2400)	// ボム再使用時間

//************************************************************
//	プロトタイプ宣言
//************************************************************
void UpdateNormalPlayer(void);		// 通常時のプレイヤー更新処理
void UpdateClearPlayer(void);		// クリア成功時のプレイヤー更新処理
void UpdateOverPlayer(void);		// クリア失敗時のプレイヤー更新処理

void MovePlayer(void);				// プレイヤーの移動量の更新処理
void PosPlayer(void);				// プレイヤーの位置の更新処理
void RevPlayer(void);				// プレイヤーの補正の更新処理

void CameraChangePlayer(void);		// プレイヤーのカメラの状態変化処理
void DriftPlayer(void);				// プレイヤーのドリフト処理
void SlumBoostPlayer(void);			// プレイヤーの加速処理
void FlyAwayPlayer(void);			// プレイヤーの送風処理
void SilenceWorldPlayer(void);		// プレイヤーの爆弾処理

void UpdateBoost(void);				// 加速の更新処理
void SetBoost(void);				// 加速の設定処理
void UpdateFlyAway(void);			// 送風の更新処理
void UpdateSilenceWorld(void);		// 爆弾の更新処理

//************************************************************
//	グローバル変数
//************************************************************
Player g_player;	// プレイヤー情報

//============================================================
//	プレイヤーの初期化処理
//============================================================
void InitPlayer(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// プレイヤーの本体情報の初期化
	g_player.pos           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 現在の位置
	g_player.oldPos        = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 前回の位置
	g_player.move          = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	g_player.rot           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 現在の向き
	g_player.destRot       = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目標の向き
	g_player.state         = ACTIONSTATE_NORMAL;				// プレイヤーの状態
	g_player.nLife         = PLAY_LIFE;							// 体力
	g_player.nCounterState = 0;									// 状態管理カウンター
	g_player.nShadowID     = NONE_SHADOW;						// 影のインデックス
	g_player.bMove         = false;								// 移動状況
	g_player.bJump         = false;								// ジャンプ状況
	g_player.nCameraState  = PLAYCAMESTATE_NORMAL;				// カメラの状態
	g_player.bCameraFirst  = false;								// 一人称カメラの状況
	g_player.bUnrivaled	   = false;								// 透明状況
	g_player.bUse          = false;								// 使用状況

	// モデル情報の初期化
	g_player.modelData.dwNumMat = 0;				// マテリアルの数
	g_player.modelData.pTexture = NULL;				// テクスチャへのポインタ
	g_player.modelData.pMesh    = NULL;				// メッシュ (頂点情報) へのポインタ
	g_player.modelData.pBuffMat = NULL;				// マテリアルへのポインタ
	g_player.modelData.dwNumMat = 0;				// マテリアルの数
	g_player.modelData.vtxMin   = INIT_VTX_MIN;		// 最小の頂点座標
	g_player.modelData.vtxMax   = INIT_VTX_MAX;		// 最大の頂点座標
	g_player.modelData.size     = INIT_SIZE;		// 大きさ
	g_player.modelData.fRadius  = 0.0f;				// 半径

	// ブーストの情報の初期化
	g_player.boost.plusMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 追加移動量
	g_player.boost.state    = BOOSTSTATE_NONE;					// 加速状態
	g_player.boost.nCounter = 0;								// 加速管理カウンター

	// 風の情報の初期化
	g_player.wind.bUseWind     = false;							// 風の使用状況
	g_player.wind.nCircleCount = 0;								// どこに出すか
	g_player.wind.nCount       = 0;								// 風を出すカウント
	g_player.wind.rot          = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 風を出す方向

	// 爆弾の情報の初期化
	g_player.bomb.state    = ATTACKSTATE_NONE;					// 攻撃状態
	g_player.bomb.nCounter = BOMB_CNT;							// 攻撃管理カウンター

	// ドリフトの情報の初期化
	g_player.drift.bDrift = false;	// ドリフト状況

	// プレイヤーの位置・向きの設定
	SetPositionPlayer(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//============================================================
//	プレイヤーの終了処理
//============================================================
void UninitPlayer(void)
{

}

//============================================================
//	プレイヤーの更新処理
//============================================================
void UpdatePlayer(void)
{
	if (g_player.bUse == true)
	{ // プレイヤーが使用されている場合

		if (GetGameState() == GAMESTATE_NORMAL)
		{ // ゲームが通常状態の場合

			// 通常時のプレイヤー更新
			UpdateNormalPlayer();
		}
		else if (GetResultState() == RESULTSTATE_CLEAR)
		{ // リザルトがクリア成功状態の場合

			// クリア成功時のプレイヤー更新
			UpdateClearPlayer();
		}
		else if (GetResultState() == RESULTSTATE_TIMEOVER)
		{ // リザルトがクリア失敗状態の場合

			// クリア失敗時のプレイヤー更新
			UpdateOverPlayer();
		}
	}
}

//============================================================
//	プレイヤーの描画処理
//============================================================
void DrawPlayer(void)
{
	// 変数を宣言
	D3DXMATRIX   mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ
	D3DXMATERIAL      redMat;					// マテリアルデータ (赤)
	D3DXMATERIAL	  UnrivaledMat;				// マテリアルデータ (無敵)

	if (g_player.bUse == true)
	{ // プレイヤーが使用されている場合

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_player.mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
		D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
		D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_player.modelData.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_player.modelData.dwNumMat; nCntMat++)
		{ // マテリアルの数分繰り返す

			switch (g_player.state)
			{ // 状態ごとの処理
			case ACTIONSTATE_DAMAGE:	// ダメージ状態

				// 構造体の要素をクリア
				ZeroMemory(&redMat, sizeof(D3DXMATERIAL));

				// 拡散光・環境光・自己発光を赤にする
				redMat.MatD3D.Diffuse  = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				redMat.MatD3D.Ambient  = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				redMat.MatD3D.Emissive = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				// マテリアルの設定
				pDevice->SetMaterial(&redMat.MatD3D);			// 赤

				// 透明状態をOFFにする
				g_player.bUnrivaled = false;

				// 処理を抜ける
				break;

			case ACTIONSTATE_UNRIVALED:	// 無敵状態

				if (g_player.nCounterState % UNRIVALED_CNT == 0)
				{ // 一定時間経過時

					// 透明状況の入れ替え
					g_player.bUnrivaled = g_player.bUnrivaled ? false : true;
				}

				if (g_player.bUnrivaled == true)
				{ // 透明状況の場合

					// マテリアルを代入する
					UnrivaledMat = pMat[nCntMat];

					// 自己発光を透明にする
					UnrivaledMat.MatD3D.Emissive = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

					// マテリアルの設定
					pDevice->SetMaterial(&UnrivaledMat.MatD3D);			// 透明
				}
				else
				{ // 透明状況じゃない場合

					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// 通常
				}

				break;					// 抜け出す

			default:					// それ以外の状態

				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// 通常

				// 透明状態をOFFにする
				g_player.bUnrivaled = false;

				// 処理を抜ける
				break;
			}

			// テクスチャの設定
			pDevice->SetTexture(0, g_player.modelData.pTexture[nCntMat]);

			// モデルの描画
			g_player.modelData.pMesh->DrawSubset(nCntMat);
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//============================================================
//	プレイヤーの位置・向きの設定処理
//============================================================
void SetPositionPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// 引数を代入
	g_player.pos     = pos;		// 現在の位置
	g_player.oldPos  = pos;		// 前回の位置
	g_player.rot     = rot;		// 現在の向き
	g_player.destRot = rot;		// 目標の向き

	// モデル情報を設定
	g_player.modelData = GetModelData(MODELTYPE_PLAYER_CAR);

	// 影のインデックスを設定
	g_player.nShadowID = SetModelShadow(g_player.modelData, &g_player.nShadowID, &g_player.bUse);

	// 影の位置設定
	SetPositionShadow(g_player.nShadowID, g_player.pos, g_player.rot, NONE_SCALE);

	// 使用している状態にする
	g_player.bUse = true;
}

//============================================================
//	プレイヤーの回復判定
//============================================================
void HealPlayer(Player *pPlayer, int nHeal)
{
	if (pPlayer->state == ACTIONSTATE_NORMAL)
	{ // プレイヤーが通常状態の場合

		// 体力バーの設定
		SetLife(pPlayer->nLife, nHeal);

		// 引数の回復分を体力に加算
		pPlayer->nLife += nHeal;

		if (pPlayer->nLife > PLAY_LIFE)
		{ // 体力が最大体力より多くなった場合

			// 体力を補正
			pPlayer->nLife = PLAY_LIFE;
		}

		// サウンドの再生
		//PlaySound(SOUND_LABEL_SE_HEAL);		// SE (回復)
	}
}

//============================================================
//	プレイヤーのダメージ判定
//============================================================
void HitPlayer(Player *pPlayer, int nDamage)
{
	if (pPlayer->state == ACTIONSTATE_NORMAL)
	{ // プレイヤーが通常状態の場合

		// 体力バーの設定
		SetLife(pPlayer->nLife, -nDamage);

		// 引数のダメージ分を体力から減算
		pPlayer->nLife -= nDamage;

		if (pPlayer->nLife > 0)
		{ // 体力が残っている場合

			// ダメージ状態にする
			pPlayer->state = ACTIONSTATE_DAMAGE;

			// パーティクルの設定
			SetParticle
			( // 引数
				pPlayer->pos,						// 位置
				D3DXCOLOR(0.8f, 0.0f, 0.3f, 1.0f),	// 色
				PARTICLETYPE_DAMAGE,				// 種類
				SPAWN_PARTICLE_DAMAGE,				// エフェクト数
				2									// 寿命
			);

			// カウンターを設定
			pPlayer->nCounterState = DAMAGE_TIME_PLAY;

			// サウンドの再生
			//PlaySound(SOUND_LABEL_SE_DMG);			// SE (ダメージ)
		}
		else
		{ // 体力が尽きた場合

			// パーティクルの設定
			SetParticle
			( // 引数
				pPlayer->pos,						// 位置
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),	// 色
				PARTICLETYPE_EXPLOSION,				// 種類
				SPAWN_PARTICLE_EXPLOSION,			// エフェクト数
				2									// 寿命
			);

			// 使用していない状態にする
			pPlayer->bUse = false;
		}
	}
}

//============================================================
//	通常時のプレイヤー更新処理
//============================================================
void UpdateNormalPlayer(void)
{
	// 変数を宣言
	int nTrafficCnt = 0;	// 引数設定用
	POLICESTATE policeState = POLICESTATE_CHASE;	// 警察の状態(オブジェクトとの当たり判定に使うため無意味)

	// 前回位置の更新
	g_player.oldPos = g_player.pos;

	// 状態の更新
	if (g_player.nCounterState > 0)
	{ // カウンターが 0より大きい場合

		// カウンターを減算
		g_player.nCounterState--;

		if (g_player.nCounterState == UNR_TIME_PLAY)
		{ // カウンターが一定値の場合

			// 無敵状態にする
			g_player.state = ACTIONSTATE_UNRIVALED;
		}
		else if (g_player.nCounterState <= 0)
		{ // カウンターが 0以下の場合

			// 通常状態にする
			g_player.state = ACTIONSTATE_NORMAL;
		}
	}

	// 加速の更新
	UpdateBoost();

	// プレイヤーの移動量の更新
	MovePlayer();

	// プレイヤーの位置の更新
	PosPlayer();

	// プレイヤーの着地の更新
	LandObject(&g_player.pos, &g_player.move, &g_player.bJump);
	
	// プレイヤーのドリフト
	DriftPlayer();

	// プレイヤーの加速
	SlumBoostPlayer();

	// プレイヤーの送風
	FlyAwayPlayer();

	// プレイヤーの爆弾
	SilenceWorldPlayer();

	// 送風の更新
	UpdateFlyAway();

	// 爆弾の更新
	UpdateSilenceWorld();

	// プレイヤーのカメラの状態変化
	CameraChangePlayer();

	//--------------------------------------------------------
	//	当たり判定
	//--------------------------------------------------------
	// オブジェクトとの当たり判定
	CollisionObject
	( // 引数
		&g_player.pos,			// 現在の位置
		&g_player.oldPos,		// 前回の位置
		&g_player.move,			// 移動量
		PLAY_WIDTH,				// 横幅
		PLAY_DEPTH,				// 奥行
		&nTrafficCnt,			// 渋滞カウント
		g_player.boost.state,	// ブーストの状態
		&policeState			// 警察の状態
	);

	// 吹っ飛ぶオブジェクトとの当たり判定
	SmashCollision
	( // 引数
		g_player.pos, 
		g_player.modelData.fRadius
	);				

	// ゲートとの当たり判定
	CollisionGate
	( // 引数
		&g_player.pos,		// 現在の位置
		&g_player.oldPos,	// 前回の位置
		&g_player.move,		// 移動量
		PLAY_WIDTH,			// 横幅
		PLAY_DEPTH			// 奥行
	);	

	// 車の停止処理
	CollisionStopCar
	( // 引数
		g_player.pos,				//位置
		g_player.rot,				//向き
		&g_player.move,				//移動量
		g_player.modelData.fRadius,	//半径
		COLLOBJECTTYPE_PLAYER,		//対象のタイプ
		&nTrafficCnt
	);

	// 車同士の当たり判定
	CollisionCarBody
	( // 引数
		&g_player.pos,
		&g_player.oldPos,
		g_player.rot,
		&g_player.move,
		PLAY_WIDTH,
		PLAY_DEPTH,
		COLLOBJECTTYPE_PLAYER,
		&nTrafficCnt,
		nTrafficCnt
	);

	//--------------------------------------------------------
	//	影の更新
	//--------------------------------------------------------
	// 影の位置設定
	SetPositionShadow
	( // 引数
		g_player.nShadowID,	// 影のインデックス
		g_player.pos,		// 位置
		g_player.rot,		// 向き
		NONE_SCALE			// 拡大率
	);

	// プレイヤーの補正の更新処理
	RevPlayer();
}

//============================================================
//	クリア成功時のプレイヤー更新処理
//============================================================
void UpdateClearPlayer(void)
{
	// カメラの状態を通常状態に設定
	g_player.bCameraFirst = false;
	g_player.nCameraState = PLAYCAMESTATE_NORMAL;

	// 角度をゲートの向きに設定
	g_player.rot.y = GetExit().pGate->rot.y;

	// 移動量を減速
	g_player.move.x += (PLAY_CLEAR_MOVE - g_player.move.x) * REV_PLAY_CLEAR_MOVE;

	// 追加移動量を減速
	g_player.boost.plusMove.x += (0.0f - g_player.boost.plusMove.x) * REV_PLAY_CLEAR_MOVE;

	// 前回位置の更新
	g_player.oldPos = g_player.pos;

	// プレイヤーの位置の更新
	PosPlayer();

	// プレイヤーの着地の更新
	LandObject(&g_player.pos, &g_player.move, &g_player.bJump);

	//--------------------------------------------------------
	//	影の更新
	//--------------------------------------------------------
	// 影の位置設定
	SetPositionShadow
	( // 引数
		g_player.nShadowID,	// 影のインデックス
		g_player.pos,		// 位置
		g_player.rot,		// 向き
		NONE_SCALE			// 拡大率
	);
}

//============================================================
//	クリア失敗時のプレイヤー更新処理
//============================================================
void UpdateOverPlayer(void)
{
	// 変数を宣言
	int nTrafficCnt = 0;	// 引数設定用

	POLICESTATE policeState = POLICESTATE_CHASE;	// 警察の状態(オブジェクトとの当たり判定に使うため無意味)

	// カメラの状態を通常状態に設定
	g_player.bCameraFirst = false;
	g_player.nCameraState = PLAYCAMESTATE_NORMAL;

	// 移動していない状態にする
	g_player.bMove = false;

	// 移動量を減速
	g_player.move.x += (0.0f - g_player.move.x) * REV_PLAY_OVER_MOVE;

	// 追加移動量を減速
	g_player.boost.plusMove.x += (0.0f - g_player.boost.plusMove.x) * REV_PLAY_OVER_MOVE;

	// 前回位置の更新
	g_player.oldPos = g_player.pos;

	// プレイヤーの位置の更新
	PosPlayer();

	// プレイヤーの着地の更新
	LandObject(&g_player.pos, &g_player.move, &g_player.bJump);

	//--------------------------------------------------------
	//	当たり判定
	//--------------------------------------------------------
	// オブジェクトとの当たり判定
	CollisionObject
	( // 引数
		&g_player.pos,			// 現在の位置
		&g_player.oldPos,		// 前回の位置
		&g_player.move,			// 移動量
		PLAY_WIDTH,				// 横幅
		PLAY_DEPTH,				// 奥行
		&nTrafficCnt,			// 渋滞カウント
		g_player.boost.state,	// ブーストの状態
		&policeState			// 警察の状態
	);

	// 吹っ飛ぶオブジェクトとの当たり判定
	SmashCollision
	( // 引数
		g_player.pos, 
		g_player.modelData.fRadius
	);				

	// ゲートとの当たり判定
	CollisionGate
	( // 引数
		&g_player.pos,		// 現在の位置
		&g_player.oldPos,	// 前回の位置
		&g_player.move,		// 移動量
		PLAY_WIDTH,			// 横幅
		PLAY_DEPTH			// 奥行
	);	

	// 車の停止処理
	CollisionStopCar
	( // 引数
		g_player.pos,				//位置
		g_player.rot,				//向き
		&g_player.move,				//移動量
		g_player.modelData.fRadius,	//半径
		COLLOBJECTTYPE_PLAYER,		//対象のタイプ
		&nTrafficCnt
	);

	// 車同士の当たり判定
	CollisionCarBody
	( // 引数
		&g_player.pos,
		&g_player.oldPos,
		g_player.rot,
		&g_player.move,
		PLAY_WIDTH,
		PLAY_DEPTH,
		COLLOBJECTTYPE_PLAYER,
		&nTrafficCnt,
		nTrafficCnt
	);

	//--------------------------------------------------------
	//	影の更新
	//--------------------------------------------------------
	// 影の位置設定
	SetPositionShadow
	( // 引数
		g_player.nShadowID,	// 影のインデックス
		g_player.pos,		// 位置
		g_player.rot,		// 向き
		NONE_SCALE			// 拡大率
	);

	// プレイヤーの補正の更新処理
	RevPlayer();
}

//============================================================
//	プレイヤーの移動量の更新処理
//============================================================
void MovePlayer(void)
{
	if (GetKeyboardPress(DIK_W) == true || GetJoyKeyR2Press(0) == true)
	{ // 前進の操作が行われた場合

		// 移動量を更新
		g_player.move.x += MOVE_FORWARD;

		// 移動している状態にする
		g_player.bMove = true;
	}
	else if (GetKeyboardPress(DIK_S) == true || GetJoyKeyL2Press(0) == true)
	{ // 後退の操作が行われた場合

		if (g_player.move.x >= 5.0f)
		{ // 移動量が一定以上だった場合
		
			// ドリフトする
			g_player.drift.bDrift = true;
			
			// 移動している状態にする
			g_player.bMove = true;
		}
		else
		{ // 移動量が一定以下だった場合
		
			// 移動量を更新
			g_player.move.x -= MOVE_BACKWARD;

			// 移動している状態にする
			g_player.bMove = true;
		}
	}
	else
	{ // 移動していない場合

		// 移動していない状態にする
		g_player.bMove = false;
	}

	if (GetKeyboardPress(DIK_A) == true || GetJoyStickPressLX(0) < 0)
	{ // 左方向の操作が行われた場合

		if (g_player.drift.bDrift == true)
		{ // ドリフト中だった場合
		
			// 向きを更新
			g_player.rot.y -= 0.04f;
		}
		else
		{ // ドリフト中じゃ無かった場合
		
			// 向きを更新
			g_player.rot.y -= MOVE_ROT * ((g_player.move.x + g_player.boost.plusMove.x) * REV_MOVE_ROT);

			if (g_player.move.x >= SUB_MOVE_VALUE)
			{ // 移動量が一定値以上の場合

				// 移動量を更新
				g_player.move.x -= SUB_MOVE;

				if (g_player.move.x < SUB_MOVE_VALUE)
				{ // 移動量が一定値より小さい場合

					// 最低限の移動量を代入
					g_player.move.x = SUB_MOVE_VALUE;
				}
			}
		}
	}
	else if (GetKeyboardPress(DIK_D) == true || GetJoyStickPressLX(0) > 0)
	{ // 右方向の操作が行われた場合

		if (g_player.drift.bDrift == true)
		{ // ドリフト中だった場合
		
			// 向きを更新
			g_player.rot.y += 0.04f;
		}
		else
		{ // ドリフト中じゃなかった場合
		
			// 向きを更新
			g_player.rot.y += MOVE_ROT * ((g_player.move.x + g_player.boost.plusMove.x) * REV_MOVE_ROT);

			if (g_player.move.x >= SUB_MOVE_VALUE)
			{ // 移動量が一定値以上の場合

				// 移動量を更新
				g_player.move.x -= SUB_MOVE;

				if (g_player.move.x < SUB_MOVE_VALUE)
				{ // 移動量が一定値より小さい場合

					// 最低限の移動量を代入
					g_player.move.x = SUB_MOVE_VALUE;
				}
			}
		}
	}
	else
	{ // A・Dキーを押していない場合
	
		// ドリフトしていない状態にする
		g_player.drift.bDrift = false;
	}

	if (GetKeyboardPress(DIK_X) == true || GetJoyKeyPress(JOYKEY_X, 0) == true)
	{ // ブレーキの操作が行われた場合

		// 移動量を減速
		g_player.move.x += (0.0f - g_player.move.x) * REV_MOVE_BRAKE;

		// 移動量の補正
		if (g_player.move.x <=  DEL_MOVE_ABS
		&&  g_player.move.x >= -DEL_MOVE_ABS)
		{ // 移動量が削除の範囲内の場合

			// 移動量を削除
			g_player.move.x = 0.0f;
		}
	}
}

//============================================================
//	プレイヤーの位置の更新処理
//============================================================
void PosPlayer(void)
{
	//--------------------------------------------------------
	//	重力の更新
	//--------------------------------------------------------
	g_player.move.y -= PLAY_GRAVITY;

	//--------------------------------------------------------
	//	移動量の補正
	//--------------------------------------------------------
	if (g_player.move.x > MAX_FORWARD)
	{ // プレイヤーの移動量 (x) が一定値以上の場合

		// プレイヤーの移動量 (x) を補正
		g_player.move.x = MAX_FORWARD;
	}
	else if (g_player.move.x < MAX_BACKWARD)
	{ // プレイヤーの移動量 (x) が一定値以下の場合

		// プレイヤーの移動量 (x) を補正
		g_player.move.x = MAX_BACKWARD;
	}

	//--------------------------------------------------------
	//	位置の更新
	//--------------------------------------------------------
	g_player.pos.x += sinf(g_player.rot.y) * (g_player.move.x + g_player.boost.plusMove.x);
	g_player.pos.y += g_player.move.y;
	g_player.pos.z += cosf(g_player.rot.y) * (g_player.move.x + g_player.boost.plusMove.x);

	//--------------------------------------------------------
	//	移動量の減衰
	//--------------------------------------------------------
	if (g_player.bMove == false)
	{ // 移動していない状態の場合

		// 移動量を減速
		g_player.move.x += (0.0f - g_player.move.x) * REV_MOVE_SUB;
	}
}

//============================================================
//	プレイヤーの補正の更新処理
//============================================================
void RevPlayer(void)
{
	// 向きの正規化
	RotNormalize(&g_player.rot.y);

	//--------------------------------------------------------
	//	移動範囲の補正
	//--------------------------------------------------------
	if (g_player.pos.z > GetLimitStage().fNear - (PLAY_DEPTH * 2))
	{ // 範囲外の場合 (手前)

		// 手前に位置を補正
		g_player.pos.z = GetLimitStage().fNear - (PLAY_DEPTH * 2);

		// 移動量を削除
		g_player.move.x *= 0.99f;
	}
	if (g_player.pos.z < GetLimitStage().fFar + (PLAY_DEPTH * 2))
	{ // 範囲外の場合 (奥)

		// 奥に位置を補正
		g_player.pos.z = GetLimitStage().fFar + (PLAY_DEPTH * 2);

		// 移動量を削除
		g_player.move.x *= 0.99f;
	}
	if (g_player.pos.x > GetLimitStage().fRight - (PLAY_WIDTH * 2))
	{ // 範囲外の場合 (右)

		// 右に位置を補正
		g_player.pos.x = GetLimitStage().fRight - (PLAY_WIDTH * 2);

		// 移動量を削除
		g_player.move.x *= 0.99f;
	}
	if (g_player.pos.x < GetLimitStage().fLeft + (PLAY_WIDTH * 2))
	{ // 範囲外の場合 (左)

		// 左に位置を補正
		g_player.pos.x = GetLimitStage().fLeft + (PLAY_WIDTH * 2);

		// 移動量を削除
		g_player.move.x *= 0.99f;
	}
}

//============================================================
//	プレイヤーの取得処理
//============================================================
Player *GetPlayer(void)
{
	// プレイヤー本体の情報アドレスを返す
	return &g_player;
}

//============================================================
//	プレイヤーのドリフト処理
//============================================================
void DriftPlayer(void)
{
	if (g_player.drift.bDrift == true)
	{ // ドリフトしている場合

		// 移動量を少し下げる
		g_player.move.x += (0.0f - g_player.move.x) * 0.04f;
	}
}

//============================================================
//	プレイヤーの加速処理
//============================================================
void SlumBoostPlayer(void)
{
	if (GetKeyboardPress(DIK_SPACE) == true || GetJoyKeyPress(JOYKEY_A, 0) == true)
	{ // 加速の操作が行われている場合

		// 加速の設定
		SetBoost();
	}
}

//============================================================
//	プレイヤーの送風処理
//============================================================
void FlyAwayPlayer(void)
{
	if (GetKeyboardPress(DIK_U) == true || GetJoyKeyPress(JOYKEY_Y, 0) == true)
	{ // 送風機の操作が行われている場合

		if (GetWindInfo()->state == WIND_USABLE)
		{ // 風が使用可能だった場合

			// 送風機を使用する
			g_player.wind.bUseWind = true;
		}
	}
	else
	{ // 送風機の操作が行われていない場合
	
		// 送風機を使用しない
		g_player.wind.bUseWind = false;
	}
}

//============================================================
//	プレイヤーの爆弾処理
//============================================================
void SilenceWorldPlayer(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyKeyTrigger(JOYKEY_B, 0))
	{ // 攻撃モードの変更の操作が行われた場合

		if (g_player.bomb.state != ATTACKSTATE_WAIT)
		{ // 攻撃状態が攻撃待機状態ではない場合

			// 攻撃モードを変更
			g_player.bomb.state = (ATTACKSTATE)((g_player.bomb.state + 1) % ATTACKSTATE_MAX);
		}
	}

	if (GetKeyboardTrigger(DIK_BACKSPACE) == true)
	{ // 爆弾の発射の操作が行われた場合

		// バリアの発射
		ShotBarrier();
	}
}

//============================================================
//	プレイヤーのカメラの状態変化処理
//============================================================
void CameraChangePlayer(void)
{
	if (GetKeyboardTrigger(DIK_J) == true || GetJoyKeyPress(JOYKEY_UP, 0) == true)
	{ // カメラ方向の変更操作が行われた場合

		// カメラの状態を変える
		g_player.nCameraState = (g_player.nCameraState + 1) % PLAYCAMESTATE_MAX;
	}

	if (GetKeyboardTrigger(DIK_K) == true || GetJoyKeyPress(JOYKEY_DOWN, 0) == true)
	{ // 一人称カメラの ON / OFF の変更操作が行われた場合

		// 一人称カメラの状況を変える
		g_player.bCameraFirst = g_player.bCameraFirst ? false : true;
	}
}

//============================================================
//	加速の更新処理
//============================================================
void UpdateBoost(void)
{
	// 変数を宣言
	D3DXVECTOR3 posLeft, posRight;	// ブーストの放出位置

	switch (g_player.boost.state)
	{ // 加速状態ごとの処理
	case BOOSTSTATE_NONE:	// 何もしない状態

		// 無し

		// 処理を抜ける
		break;

	case BOOSTSTATE_UP:		// 加速状態

		// 追加速度を加算
		g_player.boost.plusMove.x += BOOST_ADD_MOVE;

		if (g_player.boost.plusMove.x > MAX_BOOST)
		{ // 加速度が一定値以上の場合

			// 加速度の補正
			g_player.boost.plusMove.x = MAX_BOOST;
		}

		if (g_player.boost.nCounter > 0)
		{ // カウンターが 0より大きい場合

			// カウンターを減算
			g_player.boost.nCounter--;

			// タイヤ痕を出す
			SetTireMark(D3DXVECTOR3(g_player.pos.x + sinf(g_player.rot.y + D3DX_PI * 0.5f) * 55.0f, g_player.pos.y + 0.01f,
				g_player.pos.z + cosf(g_player.rot.y + D3DX_PI * 0.5f) * 55.0f), g_player.rot);

			// タイヤ痕を出す
			SetTireMark(D3DXVECTOR3(g_player.pos.x - sinf(g_player.rot.y + D3DX_PI * 0.5f) * 55.0f, g_player.pos.y + 0.01f,
				g_player.pos.z - cosf(g_player.rot.y + D3DX_PI * 0.5f) * 55.0f), g_player.rot);
		}
		else
		{ // カウンターが 0以下の場合

			// 減速状態にする
			g_player.boost.state = BOOSTSTATE_DOWN;
		}

		// 左ブーストの放出位置を求める
		posLeft.x = g_player.pos.x - sinf(g_player.rot.y) * BOOST_XZ_SUB + sinf(g_player.rot.y - (D3DX_PI * 0.5f)) * BOOST_SIDE_PULS;
		posLeft.y = g_player.pos.y + BOOST_Y_ADD;
		posLeft.z = g_player.pos.z - cosf(g_player.rot.y) * BOOST_XZ_SUB + cosf(g_player.rot.y - (D3DX_PI * 0.5f)) * BOOST_SIDE_PULS;

		// 右ブーストの放出位置を求める
		posRight.x = g_player.pos.x - sinf(g_player.rot.y) * BOOST_XZ_SUB + sinf(g_player.rot.y + (D3DX_PI * 0.5f)) * BOOST_SIDE_PULS;
		posRight.y = g_player.pos.y + BOOST_Y_ADD;
		posRight.z = g_player.pos.z - cosf(g_player.rot.y) * BOOST_XZ_SUB + cosf(g_player.rot.y + (D3DX_PI * 0.5f)) * BOOST_SIDE_PULS;

		// パーティクルの設定
		SetParticle
		( // 引数
			posLeft,							// 位置
			D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f),	// 色
			PARTICLETYPE_BOOST,					// 種類
			3,									// エフェクト数
			2									// 寿命
		);

		// パーティクルの設定
		SetParticle
		( // 引数
			posRight,							// 位置
			D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f),	// 色
			PARTICLETYPE_BOOST,					// 種類
			3,									// エフェクト数
			2									// 寿命
		);

		// 処理を抜ける
		break;

	case BOOSTSTATE_DOWN:	// 減速状態

		// 追加速度を減速
		g_player.boost.plusMove.x -= BOOST_SUB_MOVE;

		if (g_player.boost.plusMove.x <= 0.0f)
		{ // 移動量がなくなり切った場合

			// 移動量を補正
			g_player.boost.plusMove.x = 0.0f;

			// カウンターを設定
			g_player.boost.nCounter = BOOST_WAIT_CNT;

			// 使用可能の待機状態にする
			g_player.boost.state = BOOSTSTATE_WAIT;
		}

		// 処理を抜ける
		break;

	case BOOSTSTATE_WAIT:	// 使用可能の待機状態

		if (g_player.boost.nCounter > 0)
		{ // カウンターが 0より大きい場合

			// カウンターを減算
			g_player.boost.nCounter--;
		}
		else
		{ // カウンターが 0以下の場合

			// 何もしない状態にする
			g_player.boost.state = BOOSTSTATE_NONE;
		}

		// 処理を抜ける
		break;
	}
}

//============================================================
//	加速の設定処理
//============================================================
void SetBoost(void)
{
	if (g_player.boost.state == BOOSTSTATE_NONE
	&&  g_player.move.x      >= BOOST_OK_MOVE)
	{ // ブーストを行える状態の場合

		// 加速状態にする
		g_player.boost.state = BOOSTSTATE_UP;

		// カウンターを設定
		g_player.boost.nCounter = BOOST_UP_CNT;
	}
}

//============================================================
//	送風の更新処理
//============================================================
void UpdateFlyAway(void)
{
	// ポインタを宣言
	WindInfo *pWindInfo = GetWindInfo();

	switch (pWindInfo->state)
	{
	case WIND_USABLE:			// 風が使える場合

		if (g_player.wind.bUseWind == true)
		{ // 送風機を使用した場合

			// 風を出すカウントを加算する
			g_player.wind.nCount++;

			// カウンターを加算する
			pWindInfo->nUseCounter++;

			if (pWindInfo->nUseCounter >= WIND_OVERHEAT_CNT)
			{ // 3秒を超えた場合

				// 風を使用しない
				g_player.wind.bUseWind = false;

				// オーバーヒート状態にする
				pWindInfo->state = WIND_OVERHEAT;
			}

			if (g_player.wind.nCount % FLYAWAY_INTERVAL_CNT == 0)
			{ // 風のカウントが一定数になったら

				// 変数を宣言
				float fRotAdd;	// 向きの追加分

				for (int nCnt = 0; nCnt < FLYAWAY_SET_CNT; nCnt++)
				{

					{ // 右の分

						// 向きの追加分を算出する
						fRotAdd = -(float)((rand() % 240 + 52) - 157) / 100;

						// 風の位置を設定する
						g_player.wind.pos = D3DXVECTOR3
						(
							g_player.pos.x + sinf(g_player.rot.y + D3DX_PI* 0.5f) * FLYAWAY_SHIFT_WIDTH,		// X座標
							g_player.pos.y + FLYAWAY_SHIFT_HEIGHT,												// Y座標
							g_player.pos.z + cosf(g_player.rot.y + D3DX_PI * 0.5f) * FLYAWAY_SHIFT_WIDTH		// Z座標
						);

						//風の向きを設定する
						g_player.wind.rot = D3DXVECTOR3(0.0f, g_player.rot.y + D3DX_PI * 0.5f + fRotAdd, 0.0f);

						// 風の設定処理
						SetWind(g_player.wind.pos, g_player.wind.rot);
					}

					{ // 左の分

						// 向きの追加分を算出する
						fRotAdd = (float)((rand() % 240 + 52) - 157) / 100;

						// 風の位置を設定する
						g_player.wind.pos = D3DXVECTOR3
						(
							g_player.pos.x - sinf(g_player.rot.y + D3DX_PI * 0.5f) * FLYAWAY_SHIFT_WIDTH,		// X座標
							g_player.pos.y + FLYAWAY_SHIFT_HEIGHT, 												// Y座標
							g_player.pos.z - cosf(g_player.rot.y + D3DX_PI * 0.5f) * FLYAWAY_SHIFT_WIDTH		// Z座標
						);

						//風の向きを設定する
						g_player.wind.rot = D3DXVECTOR3(0.0f, g_player.rot.y - D3DX_PI * 0.5f + fRotAdd, 0.0f);

						// 風の設定処理
						SetWind(g_player.wind.pos, g_player.wind.rot);
					}
				}
			}
		}
		else
		{ // 送風機を使用していない場合

			// カウントを初期化する
			g_player.wind.nCount = 0;

			// カウンターを減算する
			pWindInfo->nUseCounter--;

			if (pWindInfo->nUseCounter <= 0)
			{ // カウンターが0以下になった場合

				// カウンターを補正する
				pWindInfo->nUseCounter = 0;

				// 使用可能にする
				pWindInfo->state = WIND_USABLE;
			}
		}

		break;					// 抜け出す

	case WIND_OVERHEAT:			// オーバーヒート状態

		// 風のカウントを180に固定する
		pWindInfo->nUseCounter = WIND_OVERHEAT_CNT;

		// オーバーヒートカウンターを加算する
		pWindInfo->nOverHeatCounter++;

		if (pWindInfo->nOverHeatCounter >= FLYAWAY_OVERHEAT_CNT)
		{ // オーバーヒートカウンターが一定数に達したら

			// オーバーヒートカウンターを初期化する
			pWindInfo->nOverHeatCounter = 0;

			// クールダウン状態にする
			pWindInfo->state = WIND_COOLDOWN;
		}

		// 風を使用しない
		g_player.wind.bUseWind = false;

		break;					// 抜け出す

	case WIND_COOLDOWN:			// クールダウン状態

		// カウントを初期化する
		g_player.wind.nCount = 0;

		// カウンターを減算する
		pWindInfo->nUseCounter--;

		if (pWindInfo->nUseCounter <= 0)
		{ // カウンターが0以下になった場合

			// カウンターを補正する
			pWindInfo->nUseCounter = 0;

			// 使用可能にする
			pWindInfo->state = WIND_USABLE;
		}

		break;					// 抜け出す
	}
}

//============================================================
//	爆弾の更新処理
//============================================================
void UpdateSilenceWorld(void)
{
	switch (g_player.bomb.state)
	{ // 攻撃状態ごとの処理
	case ATTACKSTATE_NONE:	// 何もしない状態

		// 無し

		// 処理を抜ける
		break;

	case ATTACKSTATE_BOMB:	// ボム攻撃状態

		if (g_player.bomb.nCounter > 0)
		{ // カウンターが 0より大きい場合

			// カウンターを減算
			g_player.bomb.nCounter--;
		}
		else
		{ // カウンターが 0以下の場合

			// 攻撃待機状態にする
			g_player.bomb.state = ATTACKSTATE_WAIT;
		}

		// 処理を抜ける
		break;

	case ATTACKSTATE_WAIT:	// 攻撃待機状態

		if (g_player.bomb.nCounter < BOMB_CNT)
		{ // カウンターが一定値より小さい場合

			// カウンターを加算
			g_player.bomb.nCounter++;
		}
		else
		{ // カウンターが一定値以上の場合

			// 何もしない状態にする
			g_player.bomb.state = ATTACKSTATE_NONE;
		}

		// 処理を抜ける
		break;
	}
}

#ifdef _DEBUG	// デバッグ処理
#endif