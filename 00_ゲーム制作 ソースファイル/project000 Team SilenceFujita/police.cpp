//======================================================================================================================
//
//	警察の処理 [Police.cpp]
//	Author：小原立暉
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "input.h"
#include "model.h"

#include "Police.h"
#include "shadow.h"
#include "sound.h"
#include "player.h"

#ifdef _DEBUG	// デバッグ処理
#include "game.h"
#include "EditObject.h"
#endif

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define POLI_LIFE				(50)		// オブジェクトの体力
#define POLI_GRAVITY			(0.75f)		// プレイヤーにかかる重力
#define POLI_MOVE_FORWARD		(0.1f)		// プレイヤー前進時の移動量
#define POLI_MOVE_BACKWARD		(0.2f)		// プレイヤー後退時の移動量
#define POLI_MOVE_ROT			(0.012f)	// プレイヤーの向き変更量
#define REV_POLI_MOVE_ROT		(0.1f)		// 移動量による向き変更量の補正係数
#define SUB_POLI_MOVE_VALUE		(15.0f)		// 向き変更時の減速が行われる移動量
#define SUB_POLI_MOVE			(0.05f)		// 向き変更時の減速量
#define MAX_POLI_FORWARD		(30.0f)		// 前進時の最高速度
#define MAX_POLI_FORWARD_PATROL (15.0f)		// パトロール中の前進時の最高速度
#define MAX_POLI_BACKWARD		(8.0f)		// 後退時の最高速度
#define REV_POLI_MOVE_SUB		(0.04f)		// 移動量の減速係数
#define POLICAR_WIDTH			(30.0f)		// パトカーの縦幅
#define POLICAR_HEIGHT			(30.0f)		// パトカーの奥行

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void MovePlayer(bool *bMove, D3DXVECTOR3 *move, D3DXVECTOR3 *rot);	// プレイヤーの移動量の更新処理
void PosPlayer(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove);	// プレイヤーの位置の更新処理
void RevPlayer(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos);		// プレイヤーの補正の更新処理
void PatrolPoliceAct(Police *pPolice);					// 警察のパトロール行動処理
void PatrolCarSearch(Police *pPolice);					// 警察車両の探知処理
void ChasePoliceAct(Police *pPolice);					// 警察の追跡処理
void PatrolBackPoliceAct(Police *pPolice);				// 警察のパトロールに戻るときの処理
void PatorolCarFileLoad(void);							// パトカーのロード処理
void CollisionStopCar(Police *pPolice);					// 車の停止処理

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
Police g_aPolice[MAX_POLICE];	// オブジェクトの情報

//======================================================================================================================
//	警察の初期化処理
//======================================================================================================================
void InitPolice(void)
{
	// パトカーのロード処理
	PatorolCarFileLoad();

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// オブジェクトの情報の初期化
	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // オブジェクトの最大表示数分繰り返す

		g_aPolice[nCntPolice].pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aPolice[nCntPolice].posOld = g_aPolice[nCntPolice].pos;			// 前回の位置
		g_aPolice[nCntPolice].move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
		g_aPolice[nCntPolice].rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
		g_aPolice[nCntPolice].rotDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目標の向き
		g_aPolice[nCntPolice].state		= POLICESTATE_PATROL;				// 警察の状態
		g_aPolice[nCntPolice].nLife		= 0;								// 体力
		g_aPolice[nCntPolice].nShadowID = NONE_SHADOW;						// 影のインデックス
		g_aPolice[nCntPolice].bUse		= false;							// 使用状況
		g_aPolice[nCntPolice].poliDest = POLICEDESTINATION_RIGHT;			// 警察の行先

		// モデル情報の初期化
		g_aPolice[nCntPolice].modelData.dwNumMat = 0;						// マテリアルの数
		g_aPolice[nCntPolice].modelData.pTexture = NULL;					// テクスチャへのポインタ
		g_aPolice[nCntPolice].modelData.pMesh    = NULL;					// メッシュ (頂点情報) へのポインタ
		g_aPolice[nCntPolice].modelData.pBuffMat = NULL;					// マテリアルへのポインタ
		g_aPolice[nCntPolice].modelData.dwNumMat = 0;						// マテリアルの数
		g_aPolice[nCntPolice].modelData.vtxMin   = INIT_VTX_MIN;			// 最小の頂点座標
		g_aPolice[nCntPolice].modelData.vtxMax   = INIT_VTX_MAX;			// 最大の頂点座標
		g_aPolice[nCntPolice].modelData.fHeight  = 0.0f;					// 縦幅
		g_aPolice[nCntPolice].modelData.fRadius  = 0.0f;					// 半径

		// 曲がり角の位置の初期化
		g_aPolice[nCntPolice].poliCurve.Far	 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 奥の曲がり角
		g_aPolice[nCntPolice].poliCurve.Left	 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 左の曲がり角
		g_aPolice[nCntPolice].poliCurve.Near	 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 手前の曲がり角
		g_aPolice[nCntPolice].poliCurve.Right	 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 右の曲がり角
	}

	//警察の設定処理
	SetPolice(D3DXVECTOR3(7000.0f, 0.0f, 1500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), POLICEDESTINATION_RIGHT);
	SetPolice(D3DXVECTOR3(7000.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), POLICEDESTINATION_RIGHT);
	SetPolice(D3DXVECTOR3(3000.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), POLICEDESTINATION_LEFT);
	//SetPolice(D3DXVECTOR3(7000.0f, 0.0f, 1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//SetPolice(D3DXVECTOR3(7000.0f, 0.0f, 1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//======================================================================================================================
//	警察の終了処理
//======================================================================================================================
void UninitPolice(void)
{

}

//======================================================================================================================
//	警察の更新処理
//======================================================================================================================
void UpdatePolice(void)
{
#if 0

	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // オブジェクトの最大表示数分繰り返す
		if (g_aPolice[nCntPolice].bUse == true)
		{ // オブジェクトが使用されている場合

			// 前回位置の更新
			g_aPolice[nCntPolice].posOld = g_aPolice[nCntPolice].pos;

			//// プレイヤーの移動量の更新
			//MovePlayer(&g_aPolice[nCntPolice].bMove, &g_aPolice[nCntPolice].move, &g_aPolice[nCntPolice].rot);

			//----------------------------------------------------
			//	影の更新
			//----------------------------------------------------
			// 影の位置設定
			SetPositionShadow
			( // 引数
				g_aPolice[nCntPolice].nShadowID,	// 影のインデックス
				g_aPolice[nCntPolice].pos,		// 位置
				g_aPolice[nCntPolice].rot,		// 向き
				NONE_SCALE			// 拡大率
			);

			switch (g_aPolice[nCntPolice].state)
			{//状態で判断する
			case POLICESTATE_PATROL:		//パトロール状態

				//警察のパトロール行動処理
				PatrolPoliceAct(&g_aPolice[nCntPolice]);

				break;						//抜け出す

			case POLICESTATE_CHASE:			//追跡処理

				//警察の追跡処理
				ChasePoliceAct(&g_aPolice[nCntPolice]);

				break;						//抜け出す

			case POLICESTATE_PATBACK:		//パトロールへ戻る処理

				break;						//抜け出す

			case POLICESTATE_STOP:			//足止め処理

				break;						//抜け出す
			}

			// プレイヤーの位置の更新
			PosPlayer(&g_aPolice[nCntPolice].move, &g_aPolice[nCntPolice].pos, &g_aPolice[nCntPolice].rot, g_aPolice[nCntPolice].bMove);

			//----------------------------------------------------
			//	当たり判定
			//----------------------------------------------------
			// オブジェクトとの当たり判定
			CollisionPolice
			( // 引数
				&g_aPolice[nCntPolice].pos,		// 現在の位置
				&g_aPolice[nCntPolice].posOld,	// 前回の位置
				POLICAR_WIDTH,					// 横幅
				POLICAR_HEIGHT,					// 奥行
				&g_aPolice[nCntPolice].move.x	// 移動量
			);

			if (g_aPolice[nCntPolice].pos.y < 0.0f)
			{//Y軸の位置が0.0fだった場合
				//縦への移動量を0.0fにする
				g_aPolice[nCntPolice].move.y = 0.0f;

				//位置を0.0fに戻す
				g_aPolice[nCntPolice].pos.y = 0.0f;
			}

			// プレイヤーの補正の更新処理
			RevPlayer(&g_aPolice[nCntPolice].rot, &g_aPolice[nCntPolice].pos);
		}
	}
#endif
}

//======================================================================================================================
//	警察の描画処理
//======================================================================================================================
void DrawPolice(void)
{
	// 変数を宣言
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ

	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aPolice[nCntPolice].bUse == true)
		{ // オブジェクトが使用されている場合
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aPolice[nCntPolice].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPolice[nCntPolice].rot.y, g_aPolice[nCntPolice].rot.x, g_aPolice[nCntPolice].rot.z);
			D3DXMatrixMultiply(&g_aPolice[nCntPolice].mtxWorld, &g_aPolice[nCntPolice].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aPolice[nCntPolice].pos.x, g_aPolice[nCntPolice].pos.y, g_aPolice[nCntPolice].pos.z);
			D3DXMatrixMultiply(&g_aPolice[nCntPolice].mtxWorld, &g_aPolice[nCntPolice].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aPolice[nCntPolice].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aPolice[nCntPolice].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aPolice[nCntPolice].modelData.dwNumMat; nCntMat++)
			{ // マテリアルの数分繰り返す

				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_aPolice[nCntPolice].modelData.pTexture[nCntMat]);

				// モデルの描画
				g_aPolice[nCntPolice].modelData.pMesh->DrawSubset(nCntMat);
			}

			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	警察の設定処理
//======================================================================================================================
void SetPolice(D3DXVECTOR3 pos, D3DXVECTOR3 rot, POLICEDEST poliDest)
{
	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aPolice[nCntPolice].bUse == false)
		{ // オブジェクトが使用されていない場合
			// 引数を代入
			g_aPolice[nCntPolice].pos		= pos;								// 現在の位置
			g_aPolice[nCntPolice].posOld	= g_aPolice[nCntPolice].pos;		// 前回の位置
			g_aPolice[nCntPolice].rot		= rot;								// 向き
			g_aPolice[nCntPolice].rotDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目標の向き
			g_aPolice[nCntPolice].move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
			g_aPolice[nCntPolice].state		= POLICESTATE_PATROL;				// パトロール状態にする
			g_aPolice[nCntPolice].nLife		= POLI_LIFE;						// 体力
			g_aPolice[nCntPolice].bMove		= false;							// 移動していない
			g_aPolice[nCntPolice].poliDest = poliDest;							// 警察の向き

			// 使用している状態にする
			g_aPolice[nCntPolice].bUse = true;

			// モデル情報を設定
			g_aPolice[nCntPolice].modelData = GetModelData(MODELTYPE_PLAYER_CAR + FROM_OBJECT);	// モデル情報

			// 影のインデックスを設定
			g_aPolice[nCntPolice].nShadowID = SetModelShadow
			( // 引数
				g_aPolice[nCntPolice].modelData,	// モデル情報
				&g_aPolice[nCntPolice].nShadowID,	// 影の親の影インデックス
				&g_aPolice[nCntPolice].bUse			// 影の親の使用状況
			);

			// 影の位置設定
			SetPositionShadow(g_aPolice[nCntPolice].nShadowID, g_aPolice[nCntPolice].pos, g_aPolice[nCntPolice].rot, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

			//曲がり角を初期化する(75.0fずらす)
			g_aPolice[nCntPolice].poliCurve.Far = D3DXVECTOR3(2925.0f, 0.0f, -2075.0f);
			g_aPolice[nCntPolice].poliCurve.Left = D3DXVECTOR3(2925.0f, 0.0f, 2075.0f);
			g_aPolice[nCntPolice].poliCurve.Near = D3DXVECTOR3(6575.0f, 0.0f, 2075.0f);
			g_aPolice[nCntPolice].poliCurve.Right = D3DXVECTOR3(6575.0f, 0.0f, -2075.0f);

			// 処理を抜ける
			break;
		}
	}
}

#if 0
//======================================================================================================================
//	オブジェクトのダメージ判定
//======================================================================================================================
void HitPolice(Police *pPolice, int nDamage)
{
	if (pPolice->state == ACTIONSTATE_NORMAL)
	{ // オブジェクトが通常状態の場合

	  // 引数のダメージ分を体力から減算
		pPolice->nLife -= nDamage;

		if (pPolice->nLife > 0)
		{ // 体力が残っている場合

		  // ダメージ状態にする
			pPolice->state = ACTIONSTATE_DAMAGE;

			// パーティクルの設定
			SetParticle
			( // 引数
				pPolice->pos,						// 位置
				D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f),	// 色
				PARTICLETYPE_DAMAGE,				// 種類
				SPAWN_PARTICLE_DAMAGE,				// エフェクト数
				2									// 寿命
			);

			// カウンターを設定
			pPolice->nCounterState = DAMAGE_TIME_OBJ;

			// サウンドの再生
			PlaySound(SOUND_LABEL_SE_DMG);			// SE (ダメージ)
		}
		else
		{ // 体力が尽きた場合

		  // 爆発の設定
			SetExplosion(pPolice->pos, SOUNDTYPE_BREAK);

			// パーティクルの設定
			SetParticle
			( // 引数
				pPolice->pos,						// 位置
				D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f),	// 色
				PARTICLETYPE_EXPLOSION,				// 種類
				SPAWN_PARTICLE_EXPLOSION,			// エフェクト数
				2									// 寿命
			);

			// アイテムの設定
			SetItem(pPolice->pos, ITEMTYPE_HEAL);

			// 使用していない状態にする
			pPolice->bUse = false;
		}
	}
}
#endif

//======================================================================================================================
//	オブジェクトとの当たり判定
//======================================================================================================================
void CollisionPolice(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pOldPos, float fWidth, float fDepth, float *pMove)
{
	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // オブジェクトの最大表示数分繰り返す
		if (g_aPolice[nCntPolice].bUse == true)
		{ // オブジェクトが使用されている場合

			// 前後の当たり判定
			if (pPos->x + fWidth > g_aPolice[nCntPolice].pos.x + g_aPolice[nCntPolice].modelData.vtxMin.x
				&&  pPos->x - fWidth < g_aPolice[nCntPolice].pos.x + g_aPolice[nCntPolice].modelData.vtxMax.x)
			{ // ブロックの左右の範囲内の場合

				if (pPos->z + fDepth > g_aPolice[nCntPolice].pos.z + g_aPolice[nCntPolice].modelData.vtxMin.z
					&&  pOldPos->z + fDepth <= g_aPolice[nCntPolice].pos.z + g_aPolice[nCntPolice].modelData.vtxMin.z)
				{ // 前からの当たり判定
					// 移動量を0.0fにする
					*pMove = 0.0f;
				}
				else if (pPos->z - fDepth < g_aPolice[nCntPolice].pos.z + g_aPolice[nCntPolice].modelData.vtxMax.z
					&&  pOldPos->z - fDepth >= g_aPolice[nCntPolice].pos.z + g_aPolice[nCntPolice].modelData.vtxMax.z)
				{ // 後ろからの当たり判定
					// 移動量を0.0fにする
					*pMove = 0.0f;
				}
			}

			// 左右の当たり判定
			if (pPos->z + fDepth > g_aPolice[nCntPolice].pos.z + g_aPolice[nCntPolice].modelData.vtxMin.z
				&&  pPos->z - fDepth < g_aPolice[nCntPolice].pos.z + g_aPolice[nCntPolice].modelData.vtxMax.z)
			{ // ブロックの前後の範囲内の場合

				if (pPos->x + fWidth > g_aPolice[nCntPolice].pos.x + g_aPolice[nCntPolice].modelData.vtxMin.x
					&&  pOldPos->x + fWidth <= g_aPolice[nCntPolice].pos.x + g_aPolice[nCntPolice].modelData.vtxMin.x)
				{ // 左からの当たり判定
					// 移動量を0.0fにする
					*pMove = 0.0f;
				}
				else if (pPos->x - fWidth < g_aPolice[nCntPolice].pos.x + g_aPolice[nCntPolice].modelData.vtxMax.x
					&&  pOldPos->x - fWidth >= g_aPolice[nCntPolice].pos.x + g_aPolice[nCntPolice].modelData.vtxMax.x)
				{ // 右からの当たり判定
					// 移動量を0.0fにする
					*pMove = 0.0f;
				}
			}
		}
	}
}

//======================================================================================================================
//	オブジェクトの取得処理
//======================================================================================================================
Police *GetPoliceData(void)
{
	// オブジェクトの情報の先頭アドレスを返す
	return &g_aPolice[0];
}

//============================================================
//	プレイヤーの移動量の更新処理
//============================================================
void MovePlayer(bool *bMove, D3DXVECTOR3 *move, D3DXVECTOR3 *rot)
{
	if (GetKeyboardPress(DIK_W) == true || GetJoyKeyPress(JOYKEY_UP, 0) == true || GetJoyStickPressLY(0) > 0)
	{ // 前進の操作が行われた場合

		// 移動量を更新
		move->x += 0.1f;

		// 移動している状態にする
		*bMove = true;
	}
	else if (GetKeyboardPress(DIK_S) == true || GetJoyKeyPress(JOYKEY_DOWN, 0) == true || GetJoyStickPressLY(0) < 0)
	{ // 後退の操作が行われた場合

		// 移動量を更新
		move->x -= 0.2f;

		// 移動している状態にする
		*bMove = true;
	}
	else
	{ // 移動していない場合

		// 移動していない状態にする
		*bMove = false;
	}

	if (GetKeyboardPress(DIK_A) == true)
	{ // 左方向の操作が行われた場合

		// 向きを更新
		rot->y -= 0.012f * (move->x * 0.1f);

		if (move->x >= 15.0f)
		{ // 移動量が一定値以上の場合

			// 移動量を更新
			move->x -= 0.05f;
		}
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{ // 右方向の操作が行われた場合

		// 向きを更新
		rot->y += 0.012f * (move->x * 0.1f);

		if (move->x >= 15.0f)
		{ // 移動量が一定値以上の場合

			// 移動量を更新
			move->x -= 0.05f;
		}
	}

	//if (GetKeyboardPress(DIK_SPACE) == true)
	//{ // 加速の操作が行われた場合

	//	// 移動量を更新
	//	g_player.move.x += 20.0f;
	//}
}

//============================================================
//	プレイヤーの位置の更新処理
//============================================================
void PosPlayer(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove)
{

	//--------------------------------------------------------
	//	重力の更新
	//--------------------------------------------------------
	move->y -= POLI_GRAVITY;

	//--------------------------------------------------------
	//	移動量の補正
	//--------------------------------------------------------
	if (move->x > MAX_POLI_FORWARD)
	{ // プレイヤーの移動量 (x) が一定値以上の場合

		// プレイヤーの移動量 (x) を補正
		move->x = MAX_POLI_FORWARD;
	}
	else if (move->x < -MAX_POLI_BACKWARD)
	{ // プレイヤーの移動量 (x) が一定値以下の場合

		// プレイヤーの移動量 (x) を補正
		move->x = -MAX_POLI_BACKWARD;
	}

	//--------------------------------------------------------
	//	位置の更新
	//--------------------------------------------------------
	pos->x += sinf(rot->y) * move->x;
	pos->y += move->y;
	pos->z += cosf(rot->y) * move->x;

	//--------------------------------------------------------
	//	移動量の減衰
	//--------------------------------------------------------
	if (bMove == false)
	{ // 移動していない状態の場合

		// 移動量を減速
		move->x += (0.0f - move->x) * 0.04f;
	}
}

//============================================================
//	プレイヤーの補正の更新処理
//============================================================
void RevPlayer(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos)
{
	//--------------------------------------------------------
	//	向きの正規化
	//--------------------------------------------------------
	if (rot->y > D3DX_PI) { rot->y -= D3DX_PI * 2; }
	else if (rot->y < -D3DX_PI) { rot->y += D3DX_PI * 2; }

	////--------------------------------------------------------
	////	移動範囲の補正
	////--------------------------------------------------------
	//if (pos->z > GetLimitStage().fNear - (30.0f * 2))
	//{ // 範囲外の場合 (手前)

	//	// 手前に位置を補正
	//	pos->z = GetLimitStage().fNear - (30.0f * 2);
	//}
	//if (pos->z < GetLimitStage().fFar + (30.0f * 2))
	//{ // 範囲外の場合 (奥)

	//	// 奥に位置を補正
	//	pos->z = GetLimitStage().fFar + (30.0f * 2);
	//}
	//if (pos->x > GetLimitStage().fRight - (30.0f * 2))
	//{ // 範囲外の場合 (右)

	//	// 右に位置を補正
	//	pos->x = GetLimitStage().fRight - (30.0f * 2);
	//}
	//if (pos->x < GetLimitStage().fLeft + (30.0f * 2))
	//{ // 範囲外の場合 (左)

	//	// 左に位置を補正
	//	pos->x = GetLimitStage().fLeft + (30.0f * 2);
	//}

	////--------------------------------------------------------
	////	ジャンプ判定
	////--------------------------------------------------------
	//if (g_player.pos.y < GetLimitStage().fField)
	//{ // 地面に当たっている場合

	//	// ジャンプしていない状態にする
	//	g_player.bJump = false;

	//	// 地面に位置を補正
	//	g_player.pos.y = GetLimitStage().fField;

	//	// 移動量を初期化
	//	g_player.move.y = 0.0f;
	//}
	//else
	//{ // 地面に当たっていない場合

	//	// ジャンプしている状態にする
	//	g_player.bJump = true;
	//}
}

//============================================================
//警察のパトロール行動処理
//============================================================
void PatrolPoliceAct(Police *pPolice)
{
	Player *pPlayer = GetPlayer();					//プレイヤーの情報を取得する

	float fDist;									//目標の距離

	// 移動量を更新
	pPolice->move.x += POLI_MOVE_FORWARD;

	// 移動している状態にする
	pPolice->bMove = true;

	if (/*pPolice->pos.x >= GetLimitStage().fRight + (POLICAR_WIDTH * 2) || */pPolice->poliDest == POLICEDESTINATION_RIGHT)
	{ // 右の壁が警察より左側にある場合
		if (pPolice->pos.z >= pPolice->poliCurve.Near.z + (POLICAR_WIDTH * 2))
		{ // 左にある壁が途切れたら

			// 向きを更新
			pPolice->rot.y -= 0.05f * (pPolice->move.x * 0.1f);

			if (pPolice->rot.y <= D3DXToRadian(-90))
			{//一定の向きに達した場合
				// 向きを補正
				pPolice->rot.y = D3DXToRadian(-90);

				//警察の行先を設定する
				pPolice->poliDest = POLICEDESTINATION_NEAR;
			}

			// 移動量を減速
			pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

			// 手前の壁に這わせる
			pPolice->pos.z = pPolice->poliCurve.Near.z + (POLICAR_WIDTH * 2);
		}
		else
		{ // 左にある壁がまだあったら
			// 右の壁に這わせる
			pPolice->pos.x = pPolice->poliCurve.Near.x + (POLICAR_WIDTH * 2);
		}
	}

	if (/*pPolice->pos.z >= GetLimitStage().fNear + (POLICAR_WIDTH * 2) || */pPolice->poliDest == POLICEDESTINATION_NEAR)
	{ // 手前の壁が警察より奥にある場合
		if (pPolice->pos.x <= pPolice->poliCurve.Left.x - (POLICAR_WIDTH * 2))
		{ // 左にある壁が途切れたら

			// 向きを更新
			pPolice->rot.y -= 0.05f * (pPolice->move.x * 0.1f);

			if (pPolice->rot.y <= D3DXToRadian(-180))
			{ // 一定の向きに達した場合
				// 向きを補正
				pPolice->rot.y = D3DXToRadian(180);

				//警察の行先を設定する
				pPolice->poliDest = POLICEDESTINATION_LEFT;
			}

			// 移動量を減速
			pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

			// 左の壁に這わせる
			pPolice->pos.x = pPolice->poliCurve.Left.x - (POLICAR_WIDTH * 2);
		}
		else
		{ // 左にある壁がまだあったら
			// 手前の壁に這わせる
			pPolice->pos.z = pPolice->poliCurve.Left.z + (POLICAR_WIDTH * 2);
		}
	}

	if (/*pPolice->pos.x <= GetLimitStage().fLeft - (POLICAR_WIDTH * 2) || */pPolice->poliDest == POLICEDESTINATION_LEFT)
	{//左の壁が警察より右にある場合
		if (pPolice->pos.z <= pPolice->poliCurve.Far.z - (POLICAR_WIDTH * 2))
		{ // 左にある壁が途切れたら

			// 向きを更新
			pPolice->rot.y -= 0.05f * (pPolice->move.x * 0.1f);

			if (pPolice->rot.y <= D3DXToRadian(90))
			{ // 一定の向きに達した場合
				// 向きを補正
				pPolice->rot.y = D3DXToRadian(90);

				//警察の行先を設定する
				pPolice->poliDest = POLICEDESTINATION_FAR;
			}

			// 移動量を減速
			pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

			// 左の壁に這わせる
			pPolice->pos.z = pPolice->poliCurve.Far.z - (POLICAR_WIDTH * 2);
		}
		else
		{ // 左にある壁がまだあったら
			// 左の壁に這わせる
			pPolice->pos.x = pPolice->poliCurve.Far.x - (POLICAR_WIDTH * 2);
		}
	}

	if (/*pPolice->pos.z <= GetLimitStage().fFar - (POLICAR_WIDTH * 2) || */pPolice->poliDest == POLICEDESTINATION_FAR)
	{//奥の壁が警察より手前にある場合
		if (pPolice->pos.x >= pPolice->poliCurve.Right.x + (POLICAR_WIDTH * 2))
		{ // 左にある壁が途切れたら

			// 向きを更新
			pPolice->rot.y -= 0.05f * (pPolice->move.x * 0.1f);

			if (pPolice->rot.y <= D3DXToRadian(0))
			{ // 一定の向きに達した場合
				// 向きを補正
				pPolice->rot.y = D3DXToRadian(0);

				//警察の行先を設定する
				pPolice->poliDest = POLICEDESTINATION_RIGHT;
			}

			// 移動量を減速
			pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

			// 右の壁に這わせる
			pPolice->pos.x = pPolice->poliCurve.Right.x + (POLICAR_WIDTH * 2);
		}
		else
		{ // 左にある壁がまだあったら
			// 左の壁に這わせる
			pPolice->pos.z = pPolice->poliCurve.Right.z - (POLICAR_WIDTH * 2);
		}
	}

	if (pPolice->move.x > MAX_POLI_FORWARD_PATROL)
	{ // プレイヤーの移動量 (x) が一定値以上の場合

		// プレイヤーの移動量 (x) を補正
		pPolice->move.x = MAX_POLI_FORWARD_PATROL;
	}

	////目的の距離を設定する
	//fDist = fabsf(sqrtf((pPlayer->pos.x - pPolice->pos.x) * (pPlayer->pos.x - pPolice->pos.x) + (pPlayer->pos.z - pPolice->pos.z) * (pPlayer->pos.z - pPolice->pos.z)));

	//if (fDist <= 700.0f)
	//{ // 目的の距離が一定以内に入ったら
	//	// 追跡状態に移行する
	//	pPolice->State = POLICESTATE_CHASE;
	//}

	// 車の停止処理
	CollisionStopCar(pPolice);

	//if (GetLimitStage().fFar > pPolice->pos.z)
	//{ // 右の壁に当たりそうな場合
	//	// 向きを更新
	//	pPolice->rot.y -= 0.012f * (pPolice->move.x * 0.5f);

	//	if (pPolice->move.x >= 15.0f)
	//	{ // 移動量が一定値以上の場合

	//		// 移動量を更新
	//		pPolice->move.x -= 0.05f;
	//	}

	//	// 移動量を減速
	//	pPolice->move.x += (0.0f - pPolice->move.x) * 0.04f;
	//}

	//if (GetLimitStage().fLeft > pPolice->pos.x)
	//{ // 右の壁に当たりそうな場合
	//	// 向きを更新
	//	pPolice->rot.y -= 0.012f * (pPolice->move.x * 0.5f);

	//	if (pPolice->move.x >= 15.0f)
	//	{ // 移動量が一定値以上の場合

	//		// 移動量を更新
	//		pPolice->move.x -= 0.05f;
	//	}

	//	// 移動量を減速
	//	pPolice->move.x += (0.0f - pPolice->move.x) * 0.04f;
	//}

	//if (GetLimitStage().fRight < pPolice->pos.x)
	//{ // 右の壁に当たりそうな場合
	//	// 向きを更新
	//	pPolice->rot.y -= 0.012f * (pPolice->move.x * 0.5f);

	//	if (pPolice->move.x >= 15.0f)
	//	{ // 移動量が一定値以上の場合

	//		// 移動量を更新
	//		pPolice->move.x -= 0.05f;
	//	}

	//	// 移動量を減速
	//	pPolice->move.x += (0.0f - pPolice->move.x) * 0.04f;
	//}
}

//============================================================
//警察車両の探知処理
//============================================================
void PatrolCarSearch(Police *pPolice)
{
	Player *pPlayer = GetPlayer();					// プレイヤーの情報を取得する

	float fDist, fRotDest, fRotDiff;				// 目標の距離,角度

	// 目的の距離を設定する
	fDist = fabsf(sqrtf((pPlayer->pos.x - pPolice->pos.x) * (pPlayer->pos.x - pPolice->pos.x) + (pPlayer->pos.z - pPolice->pos.z) * (pPlayer->pos.z - pPolice->pos.z)));

	// 目的の向きを設定する
	fRotDest = atan2f(pPlayer->pos.x - pPolice->pos.x, pPlayer->pos.z - pPolice->pos.z);

	if (fDist <= 700.0f)
	{ // 目的の距離が一定以内に入ったら
		// 追跡状態に移行する
		pPolice->state = POLICESTATE_CHASE;

		// 向きの差分を求める
		fRotDiff = fRotDest - pPolice->rot.y * 0.8f;

		if (fRotDiff > D3DX_PI)
		{ // 角度が3.14fより大きかった場合
			// 角度から1周分減らす
			fRotDiff = -D3DX_PI;
		}
		else if (fRotDiff < -D3DX_PI)
		{ // 角度が-3.14fより小さかった場合
			// 角度に1周分加える
			fRotDiff = D3DX_PI;
		}

		// 角度を補正する
		pPolice->rot.y += fRotDiff;

		if (pPolice->rot.y > D3DX_PI)
		{ // 角度が3.14fより大きかった場合
			// 角度から1周分減らす
			pPolice->rot.y = -D3DX_PI;
		}
		else if (pPolice->rot.y < -D3DX_PI)
		{ // 角度が-3.14fより小さかった場合
			// 角度に1周分加える
			pPolice->rot.y = D3DX_PI;
		}
	}
	else
	{ // 範囲内に入っていない場合
		//巡回に戻る状態にする
		pPolice->state = POLICESTATE_PATBACK;
	}
}

//============================================================
//警察の追跡処理
//============================================================
void ChasePoliceAct(Police *pPolice)
{
	//警察車両の探知処理
	PatrolCarSearch(pPolice);

	if (pPolice->move.x >= 28.0f)
	{ // 移動量が一定値以上の場合
		// 移動量を更新
		pPolice->move.x -= 0.05f;
	}
}

//============================================================
// 車の停止処理
//============================================================
void CollisionStopCar(Police *pPolice)
{
	D3DXVECTOR3 Policepos = D3DXVECTOR3(pPolice->pos.x + sinf(pPolice->rot.y) * 300.0f, 0.0f, pPolice->pos.z + cosf(pPolice->rot.y) * 300.0f);				// 警察の位置
	Police *pComparePoli = GetPoliceData();			// 警察の情報を取得する

	float fLength;									// 長さの変数

	for (int nCntPoli = 0; nCntPoli < MAX_POLICE; nCntPoli++, pComparePoli++)
	{
		if (pComparePoli->bUse == true)
		{ // 使用している場合
			// 長さを測る
			fLength = (pComparePoli->pos.x - Policepos.x) * (pComparePoli->pos.x - Policepos.x)
				+ (pComparePoli->pos.z - Policepos.z) * (pComparePoli->pos.z - Policepos.z);

			if (fLength <= (pComparePoli->modelData.fRadius * pPolice->modelData.fRadius))
			{ // オブジェクトが当たっている
				// 移動量を0.0fにする
				pPolice->move.x = 0.0f;
			}
		}
	}
}

//============================================================
//警察のパトロールに戻るときの処理
//============================================================
void PatrolBackPoliceAct(Police *pPolice)
{

}

//============================================================
// パトカーのロード処理
//============================================================
void PatorolCarFileLoad(void)
{

}

#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================
//**********************************************************************************************************************
//	オブジェクトの総数取得処理
//**********************************************************************************************************************
int GetNumPolice(void)
{
	// 変数を宣言
	int nNumPolice = 0;	// オブジェクトの総数の確認用

	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aPolice[nCntPolice].bUse == true)
		{ // オブジェクトが使用されている場合

		  // カウンターを加算
			nNumPolice++;
		}
	}

	// 変数の値を返す
	return nNumPolice;	// オブジェクトの総数
}
#endif