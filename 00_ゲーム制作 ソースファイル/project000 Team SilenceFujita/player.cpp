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
#include "object.h"
#include "particle.h"
#include "player.h"
#include "shadow.h"
#include "sound.h"
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
#define SUB_MOVE		(0.2f)		// 向き変更時の減速量
#define PLAY_GRAVITY	(0.75f)		// プレイヤーにかかる重力
#define MAX_BACKWARD	(-10.0f)	// 後退時の最高速度
#define REV_MOVE_SUB	(0.02f)		// 移動量の減速係数

#define BOOST_OK_MOVE	(15.0f)		// ブースト使用に必要なプレイヤーの最低速度
#define BOOST_ADD_MOVE	(0.25f)		// ブーストの加速量
#define BOOST_SUB_MOVE	(0.08f)		// ブーストの減速量
#define BOOST_UP_CNT	(180)		// ブーストの加速状態の時間

#define BOOST_XZ_SUB	(90.0f)		// ブースト噴射位置の xz減算量
#define BOOST_Y_ADD		(40.0f)		// ブースト噴射位置の y加算量
#define BOOST_SIDE_PULS	(18.0f)		// ブースト噴射位置の横位置変更量

//************************************************************
//	プロトタイプ宣言
//************************************************************
void MovePlayer(void);				// プレイヤーの移動量の更新処理
void PosPlayer(void);				// プレイヤーの位置の更新処理
void RevPlayer(void);				// プレイヤーの補正の更新処理

void CameraChangePlayer(void);		// プレイヤーのカメラの状態変化処理
void DriftPlayer(void);				// プレイヤーのドリフト処理
void FlyAwayPlayer(void);			// プレイヤーの送風処理
void SlumBoostPlayer(void);			// プレイヤーの加速処理

void UpdateBoost(void);				// 加速の更新処理
void SetBoost(void);				// 加速の設定処理

void UpdateFlyAway(void);			// 送風の更新処理

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

#if 1
		// 加速の更新
		UpdateBoost();

		// プレイヤーの移動量の更新
		MovePlayer();

		// プレイヤーの位置の更新
		PosPlayer();

		// プレイヤーの着地の更新処理
		LandObject(&g_player.pos, &g_player.move, &g_player.bJump);
		
		// プレイヤーのドリフト処理
		DriftPlayer();

		// プレイヤーの加速処理
		SlumBoostPlayer();

		// プレイヤーの送風処理
		FlyAwayPlayer();

		// 送風の更新処理
		UpdateFlyAway();			

		// プレイヤーのカメラの状態変化処理
		CameraChangePlayer();
#else
		if (GetKeyboardPress(DIK_W) == true || GetJoyKeyPress(JOYKEY_UP, 0) == true || GetJoyStickPressLY(0) > 0)
		{ // 奥移動の操作が行われた場合

			g_player.pos.z += 10.0f;
		}
		if (GetKeyboardPress(DIK_S) == true || GetJoyKeyPress(JOYKEY_DOWN, 0) == true || GetJoyStickPressLY(0) < 0)
		{ // 手前移動の操作が行われた場合

			g_player.pos.z -= 10.0f;
		}
		if (GetKeyboardPress(DIK_A) == true || GetJoyKeyPress(JOYKEY_LEFT, 0) == true || GetJoyStickPressLX(0) < 0)
		{ // 左移動の操作が行われた場合

			g_player.pos.x -= 10.0f;
		}
		if (GetKeyboardPress(DIK_D) == true || GetJoyKeyPress(JOYKEY_RIGHT, 0) == true || GetJoyStickPressLX(0) > 0)
		{ // 右移動の操作が行われた場合

			g_player.pos.x += 10.0f;
		}
#endif

		//----------------------------------------------------
		//	当たり判定
		//----------------------------------------------------
		// オブジェクトとの当たり判定
		CollisionObject
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
			COLLOBJECTTYPE_PLAYER		//対象のタイプ
		);

		// 車同士の当たり判定
		CollisionCarBody
		( // 引数
			&g_player.pos,
			&g_player.oldPos,
			g_player.rot,
			&g_player.move,
			g_player.modelData,
			COLLOBJECTTYPE_PLAYER
		);

		//----------------------------------------------------
		//	影の更新
		//----------------------------------------------------
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

				// 処理を抜ける
				break;

			default:					// それ以外の状態

				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// 通常

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

#if 0
//============================================================
//	プレイヤーの回復判定
//============================================================
void HealPlayer(MainPlayer *pPlayer, int nHeal)
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
		PlaySound(SOUND_LABEL_SE_HEAL);		// SE (回復)
	}
}

//============================================================
//	プレイヤーのダメージ判定
//============================================================
void HitPlayer(MainPlayer *pPlayer, int nDamage)
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
			PlaySound(SOUND_LABEL_SE_DMG);			// SE (ダメージ)
		}
		else
		{ // 体力が尽きた場合

			// 爆発の設定
			SetExplosion(pPlayer->pos, SOUNDTYPE_EXPLOSION);

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
#endif

//============================================================
//	プレイヤーの移動量の更新処理
//============================================================
void MovePlayer(void)
{
	if (GetKeyboardPress(DIK_W) == true || GetJoyKeyPress(JOYKEY_UP, 0) == true || GetJoyStickPressLY(0) > 0)
	{ // 前進の操作が行われた場合

		// 移動量を更新
		g_player.move.x += MOVE_FORWARD;

		// 移動している状態にする
		g_player.bMove = true;
	}
	else if (GetKeyboardPress(DIK_S) == true || GetJoyKeyPress(JOYKEY_DOWN, 0) == true || GetJoyStickPressLY(0) < 0)
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

	if (GetKeyboardPress(DIK_A) == true)
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
	else if (GetKeyboardPress(DIK_D) == true)
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
	RotNormalize(g_player.rot.y);

	//--------------------------------------------------------
	//	移動範囲の補正
	//--------------------------------------------------------
	if (g_player.pos.z > GetLimitStage().fNear - (PLAY_DEPTH * 2))
	{ // 範囲外の場合 (手前)

		// 手前に位置を補正
		g_player.pos.z = GetLimitStage().fNear - (PLAY_DEPTH * 2);

		// 移動量を削除
		g_player.move.x *= 0.95f;
	}
	if (g_player.pos.z < GetLimitStage().fFar + (PLAY_DEPTH * 2))
	{ // 範囲外の場合 (奥)

		// 奥に位置を補正
		g_player.pos.z = GetLimitStage().fFar + (PLAY_DEPTH * 2);

		// 移動量を削除
		g_player.move.x *= 0.95f;
	}
	if (g_player.pos.x > GetLimitStage().fRight - (PLAY_WIDTH * 2))
	{ // 範囲外の場合 (右)

		// 右に位置を補正
		g_player.pos.x = GetLimitStage().fRight - (PLAY_WIDTH * 2);

		// 移動量を削除
		g_player.move.x *= 0.95f;
	}
	if (g_player.pos.x < GetLimitStage().fLeft + (PLAY_WIDTH * 2))
	{ // 範囲外の場合 (左)

		// 左に位置を補正
		g_player.pos.x = GetLimitStage().fLeft + (PLAY_WIDTH * 2);

		// 移動量を削除
		g_player.move.x *= 0.95f;
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
	if (GetKeyboardPress(DIK_SPACE) == true)
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
	if (GetKeyboardPress(DIK_U) == true)
	{ // Uキーを押している場合
		if (GetWindInfo()->state == WIND_USABLE)
		{ // 風が使用可能だった場合
			// 送風機を使用する
			g_player.wind.bUseWind = true;
		}
	}
	else
	{ // Uキーを押していない場合
	
		// 送風機を使用しない
		g_player.wind.bUseWind = false;
	}
}

//============================================================
// 送風の更新処理
//============================================================
void UpdateFlyAway(void)
{
	if (g_player.wind.bUseWind == true)
	{ // 送風機を使用した場合

		// 風を出すカウントを加算する
		g_player.wind.nCount++;

		// カウンターを加算する
		GetWindInfo()->nUseCounter++;

		if (GetWindInfo()->nUseCounter >= WIND_OVERHEAT_CNT)
		{ // 3秒を超えた場合
			// 風を使用しない
			g_player.wind.bUseWind = false;

			// オーバーヒート状態にする
			GetWindInfo()->state = WIND_OVERHEAT;
		}

		if (g_player.wind.nCount % 3 == 0)
		{ // 風のカウントが一定数になったら

			// 変数を宣言
			float fRotAdd;	// 向きの追加分

			for (int nCnt = 0; nCnt < 10; nCnt++)
			{

				{ // 右の分

					// 向きの追加分を算出する
					fRotAdd = -(float)((rand() % 240 + 52) - 157) / 100;

					// 風の位置を設定する
					g_player.wind.pos = D3DXVECTOR3(g_player.pos.x + sinf(g_player.rot.y + D3DX_PI* 0.5f) * 90.0f, g_player.pos.y + 50.0f, g_player.pos.z + cosf(g_player.rot.y + D3DX_PI * 0.5f) * 90.0f);

					//風の向きを設定する
					g_player.wind.rot = D3DXVECTOR3(0.0f, g_player.rot.y + D3DX_PI * 0.5f + fRotAdd, 0.0f);

					// 風の設定処理
					SetWind(g_player.wind.pos, g_player.wind.rot);
				}

				{ // 左の分

					// 向きの追加分を算出する
					fRotAdd = (float)((rand() % 240 + 52) - 157) / 100;

					// 風の位置を設定する
					g_player.wind.pos = D3DXVECTOR3(g_player.pos.x - sinf(g_player.rot.y + D3DX_PI * 0.5f) * 90.0f, g_player.pos.y + 50.0f, g_player.pos.z - cosf(g_player.rot.y + D3DX_PI * 0.5f) * 90.0f);

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
		GetWindInfo()->nUseCounter--;

		if (GetWindInfo()->nUseCounter <= 0)
		{ // カウンターが0以下になった場合
			// カウンターを補正する
			GetWindInfo()->nUseCounter = 0;

			// 使用可能にする
			GetWindInfo()->state = WIND_USABLE;
		}
	}
}

//============================================================
//	プレイヤーのカメラの状態変化処理
//============================================================
void CameraChangePlayer(void)
{
	if (GetKeyboardTrigger(DIK_J) == true)
	{ // Jキーを押した場合

		// カメラの状態を変える
		g_player.nCameraState = (g_player.nCameraState + 1) % PLAYCAMESTATE_MAX;
	}

	if (GetKeyboardTrigger(DIK_K) == true)
	{ // Kキーを押した場合

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

#ifdef _DEBUG	// デバッグ処理
#endif