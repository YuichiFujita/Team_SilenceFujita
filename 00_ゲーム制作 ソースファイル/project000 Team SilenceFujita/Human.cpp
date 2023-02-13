//======================================================================================================================
//
//	人間の処理 [Human.cpp]
//	Author：小原立暉
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "input.h"
#include "model.h"
#include "calculation.h"

#include "Human.h"
#include "shadow.h"
#include "sound.h"
#include "player.h"
#include "Police.h"
#include "curve.h"
#include "object.h"
#include "wind.h"
#include "Car.h"

#ifdef _DEBUG	// デバッグ処理
#include "game.h"
#endif

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define HUMAN_LIFE					(50)		// 人の体力
#define HUMAN_GRAVITY				(1.0f)		// 人にかかる重力
#define HUMAN_MOVE_FORWARD			(0.1f)		// 人前進時の移動量
#define HUMAN_MOVE_BACKWARD			(0.2f)		// 人後退時の移動量
#define HUMAN_MOVE_ROT				(0.012f)	// 人の向き変更量
#define REV_HUMAN_MOVE_ROT			(0.1f)		// 移動量による向き変更量の補正係数
#define SUB_HUMAN_MOVE_VALUE		(15.0f)		// 向き変更時の減速が行われる移動量
#define SUB_HUMAN_MOVE				(0.05f)		// 向き変更時の減速量
#define MAX_HUMAN_BACKWARD			(8.0f)		// 後退時の最高速度
#define REV_HUMAN_MOVE_SUB			(0.04f)		// 移動量の減速係数
#define HUMAN_CURVE_ADD				(0.03f)		// 曲がり角での向きの加算数
#define HUMAN_RANDAM_MOVE			(6)			// 人間の移動量のランダム
#define HUMAN_MOVE_LEAST			(4)			// 人間の移動量の最低限
#define HUMAN_PASS_SHIFT			(40.0f)		// すれ違った時のずらす幅
#define HUMAN_RADIUS				(30.0f)		// 人間の幅
#define HUMAN_PASS_CORRECT			(0.06f)		// 人間のずらす補正倍率
#define REACTION_HUMAN_RANGE		(170.0f)	// リアクションする人間の範囲
#define REACTION_CAR_RANGE			(50.0f)		// リアクションする車の範囲

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void PosHuman(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove, float fMaxMove);		// 人間の位置の更新処理
void RevHuman(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos);	// 人間の補正の更新処理
void CurveHuman(Human *pHuman);						// 人間のカーブ処理
void StopHuman(Human *pHuman);						// 人間の停止処理
void ReactionHuman(Human *pHuman);					// 人間のリアクション処理
void CollisionCarHuman(Human *pHuman);				// 人間と車の当たり判定
void CurveRotHuman(Human *pHuman);					// 人間の角度更新処理
void WalkHuman(Human *pHuman);						// 人間の歩く処理
void PassingHuman(Human *pHuman);					// 人間のすれ違い処理

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
Human g_aHuman[MAX_HUMAN];		// 人間の情報

//======================================================================================================================
//	人間の初期化処理
//======================================================================================================================
void InitHuman(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// 人間の情報の初期化
	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++)
	{ // オブジェクトの最大表示数分繰り返す

		g_aHuman[nCntHuman].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aHuman[nCntHuman].posOld = g_aHuman[nCntHuman].pos;		// 前回の位置
		g_aHuman[nCntHuman].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
		g_aHuman[nCntHuman].fMaxMove = 0.0f;						// 移動量の最大数
		g_aHuman[nCntHuman].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
		g_aHuman[nCntHuman].nShadowID = NONE_SHADOW;				// 影のインデックス
		g_aHuman[nCntHuman].bJump = false;							// ジャンプしているかどうか
		g_aHuman[nCntHuman].bMove = false;							// 移動しているか
		g_aHuman[nCntHuman].bUse = false;							// 使用状況
		g_aHuman[nCntHuman].state = HUMANSTATE_WALK;				// 状態

		// モデル情報の初期化
		g_aHuman[nCntHuman].modelData.dwNumMat = 0;					// マテリアルの数
		g_aHuman[nCntHuman].modelData.pTexture = NULL;				// テクスチャへのポインタ
		g_aHuman[nCntHuman].modelData.pMesh = NULL;					// メッシュ (頂点情報) へのポインタ
		g_aHuman[nCntHuman].modelData.pBuffMat = NULL;				// マテリアルへのポインタ
		g_aHuman[nCntHuman].modelData.vtxMin = INIT_VTX_MIN;		// 最小の頂点座標
		g_aHuman[nCntHuman].modelData.vtxMax = INIT_VTX_MAX;		// 最大の頂点座標
		g_aHuman[nCntHuman].modelData.size = INIT_SIZE;				// 縦幅
		g_aHuman[nCntHuman].modelData.fRadius = 0.0f;				// 半径

		// 曲がり角関係の初期化
		g_aHuman[nCntHuman].curveInfo.actionState = HUMANACT_WALK;	// 歩行状態
		g_aHuman[nCntHuman].curveInfo.nRandamRoute = 0;				// 進むルートの種類
		g_aHuman[nCntHuman].curveInfo.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目標の位置
		g_aHuman[nCntHuman].curveInfo.curveInfo.nCurveTime = 0;		// 曲がり角の数
		g_aHuman[nCntHuman].curveInfo.curveInfo.nNowCurve = 0;		// 現在の曲がり角

		for (int nCntCur = 0; nCntCur < MAX_HUMAN_CURVE; nCntCur++)
		{
			g_aHuman[nCntHuman].curveInfo.curveInfo.curvePoint[nCntCur] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 曲がり角の位置
			g_aHuman[nCntHuman].curveInfo.curveInfo.curveAngle[nCntCur] = CURVE_RIGHT;							// 右に曲がる
			g_aHuman[nCntHuman].curveInfo.curveInfo.dashAngle[nCntCur]  = DASH_RIGHT;							// 右に向かって走っている
		}
	}
}

//======================================================================================================================
//	人間の終了処理
//======================================================================================================================
void UninitHuman(void)
{

}

//======================================================================================================================
//	人間の更新処理
//======================================================================================================================
void UpdateHuman(void)
{
	int nCnt = 0;

	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++)
	{ // オブジェクトの最大表示数分繰り返す
		if (g_aHuman[nCntHuman].bUse == true)
		{ // オブジェクトが使用されている場合

			// 前回位置の更新
			g_aHuman[nCntHuman].posOld = g_aHuman[nCntHuman].pos;

			// オブジェクトの着地の更新処理
			LandObject(&g_aHuman[nCntHuman].pos, &g_aHuman[nCntHuman].move, &g_aHuman[nCntHuman].bJump);

			//----------------------------------------------------
			//	影の更新
			//----------------------------------------------------
			// 影の位置設定
			SetPositionShadow
			( // 引数
				g_aHuman[nCntHuman].nShadowID,		// 影のインデックス
				g_aHuman[nCntHuman].pos,			// 位置
				g_aHuman[nCntHuman].rot,			// 向き
				NONE_SCALE							// 拡大率
			);

			// プレイヤーの位置の更新
			PosHuman
			(
				&g_aHuman[nCntHuman].move,			// 移動量
				&g_aHuman[nCntHuman].pos,			// 位置
				&g_aHuman[nCntHuman].rot,			// 向き
				g_aHuman[nCntHuman].bMove,			// 移動しているか
				g_aHuman[nCntHuman].fMaxMove		// 移動量の最大数
			);

			//人間のリアクション処理
			ReactionHuman(&g_aHuman[nCntHuman]);

			// 風の当たり判定
			CollisionWind(&g_aHuman[nCntHuman]);

			switch (g_aHuman[nCntHuman].state)
			{
			case HUMANSTATE_WALK:		//歩き状態

				//人間のカーブ処理
				CurveHuman(&g_aHuman[nCntHuman]);

				if (g_aHuman[nCntHuman].pos.y < 0.0f)
				{//Y軸の位置が0.0fだった場合
					//縦への移動量を0.0fにする
					g_aHuman[nCntHuman].move.y = 0.0f;

					//位置を0.0fに戻す
					g_aHuman[nCntHuman].pos.y = 0.0f;
				}

				break;					//抜け出す

			case HUMANSTATE_STOP:		//止まった状態

				// 人間の停止処理
				StopHuman(&g_aHuman[nCntHuman]);

				if (g_aHuman[nCntHuman].pos.y < 0.0f)
				{//Y軸の位置が0.0fだった場合
					//縦への移動量を0.0fにする
					g_aHuman[nCntHuman].move.y = 0.0f;

					//位置を0.0fに戻す
					g_aHuman[nCntHuman].pos.y = 0.0f;
				}

				break;					//抜け出す

			case HUMANSTATE_FLY:		//吹き飛んだ状態

				// 飛ばす
				g_aHuman[nCntHuman].pos.x += g_aHuman[nCntHuman].move.x;
				g_aHuman[nCntHuman].pos.z += g_aHuman[nCntHuman].move.z;

				if (g_aHuman[nCntHuman].pos.y <= 0.0f)
				{ // 位置が0.0f以下になった場合
					//使用していない
					g_aHuman[nCntHuman].bUse = false;
				}

				break;					//抜け出す
			}

			//----------------------------------------------------
			//	当たり判定
			//----------------------------------------------------
			// オブジェクトとの当たり判定
			CollisionObject
			( // 引数
				&g_aHuman[nCntHuman].pos,		// 現在の位置
				&g_aHuman[nCntHuman].posOld,	// 前回の位置
				&g_aHuman[nCntHuman].move,		// 移動量
				HUMAN_WIDTH,					// 横幅
				HUMAN_DEPTH,					// 奥行
				&nCnt							// 渋滞カウント
			);

			// プレイヤーの補正の更新処理
			RevHuman(&g_aHuman[nCntHuman].rot, &g_aHuman[nCntHuman].pos);
		}
	}
}

//======================================================================================================================
//	人間の描画処理
//======================================================================================================================
void DrawHuman(void)
{
	// 変数を宣言
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

												// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ
	D3DXMATERIAL	  blueMat;					// 青ざめマテリアルポインタ

	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aHuman[nCntHuman].bUse == true)
		{ // オブジェクトが使用されている場合
		  // ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aHuman[nCntHuman].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aHuman[nCntHuman].rot.y, g_aHuman[nCntHuman].rot.x, g_aHuman[nCntHuman].rot.z);
			D3DXMatrixMultiply(&g_aHuman[nCntHuman].mtxWorld, &g_aHuman[nCntHuman].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aHuman[nCntHuman].pos.x, g_aHuman[nCntHuman].pos.y, g_aHuman[nCntHuman].pos.z);
			D3DXMatrixMultiply(&g_aHuman[nCntHuman].mtxWorld, &g_aHuman[nCntHuman].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aHuman[nCntHuman].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aHuman[nCntHuman].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aHuman[nCntHuman].modelData.dwNumMat; nCntMat++)
			{ // マテリアルの数分繰り返す

				switch (g_aHuman[nCntHuman].state)
				{
				case HUMANSTATE_STOP:		//停止状態

					// 構造体の要素をクリア
					ZeroMemory(&blueMat, sizeof(D3DXMATERIAL));

					// 拡散光・環境光・自己発光を赤にする
					blueMat.MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
					blueMat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
					blueMat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

					// マテリアルの設定
					pDevice->SetMaterial(&blueMat.MatD3D);			// 青

					break;					//抜け出す

				default:		//上記以外

					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					break;					//抜け出す
				}

				// テクスチャの設定
				pDevice->SetTexture(0, g_aHuman[nCntHuman].modelData.pTexture[nCntMat]);

				// モデルの描画
				g_aHuman[nCntHuman].modelData.pMesh->DrawSubset(nCntMat);
			}

			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	人間の設定処理
//======================================================================================================================
void SetHuman(D3DXVECTOR3 pos)
{
	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aHuman[nCntHuman].bUse == false)
		{ // オブジェクトが使用されていない場合
		  // 引数を代入
			g_aHuman[nCntHuman].pos = pos;								// 現在の位置
			g_aHuman[nCntHuman].posOld = g_aHuman[nCntHuman].pos;		// 前回の位置
			g_aHuman[nCntHuman].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
			g_aHuman[nCntHuman].fMaxMove = (float)(rand() % HUMAN_RANDAM_MOVE + HUMAN_MOVE_LEAST);		// 移動量の最大数
			g_aHuman[nCntHuman].bJump = false;							// ジャンプしているかどうか
			g_aHuman[nCntHuman].bMove = false;							// 移動していない
			g_aHuman[nCntHuman].state = HUMANSTATE_WALK;				// 歩き状態

			// 使用している状態にする
			g_aHuman[nCntHuman].bUse = true;

			// モデル情報を設定
			g_aHuman[nCntHuman].modelData = GetModelData(MODELTYPE_OBJECT_BIGTREE + FROM_OBJECT);	// モデル情報

			// 影のインデックスを設定
			g_aHuman[nCntHuman].nShadowID = SetModelShadow
			( // 引数
				g_aHuman[nCntHuman].modelData,	// モデル情報
				&g_aHuman[nCntHuman].nShadowID,	// 影の親の影インデックス
				&g_aHuman[nCntHuman].bUse		// 影の親の使用状況
			);

			// 影の位置設定
			SetPositionShadow(g_aHuman[nCntHuman].nShadowID, g_aHuman[nCntHuman].pos, g_aHuman[nCntHuman].rot, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

			// 曲がり角情報の設置
			g_aHuman[nCntHuman].curveInfo.actionState = HUMANACT_WALK;			// 状態
			g_aHuman[nCntHuman].curveInfo.nRandamRoute = rand() % MAX_HUMAN_ROUTE;		// ルートの種類
			g_aHuman[nCntHuman].curveInfo.rotDest = g_aHuman[nCntHuman].rot;	// 目標の向き
			g_aHuman[nCntHuman].rot.y = GetDefaultRot(g_aHuman[nCntHuman].curveInfo.nRandamRoute);		// 初期の向き
			g_aHuman[nCntHuman].curveInfo.curveInfo = GetHumanRoute(g_aHuman[nCntHuman].curveInfo.nRandamRoute);		// ルート

			switch (g_aHuman[nCntHuman].curveInfo.curveInfo.dashAngle[0])
			{
			case DASH_RIGHT:	// 右に向かって走っている

				// 位置を補正する
				g_aHuman[nCntHuman].pos.z = g_aHuman[nCntHuman].curveInfo.curveInfo.curvePoint[0].z - (HUMAN_WIDTH * 2);

				break;			// 抜け出す

			case DASH_LEFT:		// 左に向かって走っている

				// 位置を補正する
				g_aHuman[nCntHuman].pos.z = g_aHuman[nCntHuman].curveInfo.curveInfo.curvePoint[0].z + (HUMAN_WIDTH * 2);

				break;			// 抜け出す

			case DASH_FAR:		// 奥に向かって走っている

				// 位置を補正する
				g_aHuman[nCntHuman].pos.x = g_aHuman[nCntHuman].curveInfo.curveInfo.curvePoint[0].x + (HUMAN_WIDTH * 2);

				break;			// 抜け出す

			case DASH_NEAR:		// 手前に向かって走っている

				// 位置を補正する
				g_aHuman[nCntHuman].pos.x = g_aHuman[nCntHuman].curveInfo.curveInfo.curvePoint[0].x - (HUMAN_WIDTH * 2);

				break;			// 抜け出す
			}

			// 処理を抜ける
			break;
		}
	}
}

#if 0
//======================================================================================================================
//	オブジェクトのダメージ判定
//======================================================================================================================
void HitHuman(Human *pHuman, int nDamage)
{
	if (pHuman->state == ACTIONSTATE_NORMAL)
	{ // オブジェクトが通常状態の場合

	  // 引数のダメージ分を体力から減算
		pHuman->nLife -= nDamage;

		if (pHuman->nLife > 0)
		{ // 体力が残っている場合

		  // ダメージ状態にする
			pHuman->state = ACTIONSTATE_DAMAGE;

			// パーティクルの設定
			SetParticle
			( // 引数
				pHuman->pos,						// 位置
				D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f),	// 色
				PARTICLETYPE_DAMAGE,				// 種類
				SPAWN_PARTICLE_DAMAGE,				// エフェクト数
				2									// 寿命
			);

			// カウンターを設定
			pHuman->nCounterState = DAMAGE_TIME_OBJ;

			// サウンドの再生
			PlaySound(SOUND_LABEL_SE_DMG);			// SE (ダメージ)
		}
		else
		{ // 体力が尽きた場合

		  // 爆発の設定
			SetExplosion(pHuman->pos, SOUNDTYPE_BREAK);

			// パーティクルの設定
			SetParticle
			( // 引数
				pHuman->pos,						// 位置
				D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f),	// 色
				PARTICLETYPE_EXPLOSION,				// 種類
				SPAWN_PARTICLE_EXPLOSION,			// エフェクト数
				2									// 寿命
			);

			// アイテムの設定
			SetItem(pHuman->pos, ITEMTYPE_HEAL);

			// 使用していない状態にする
			pHuman->bUse = false;
		}
	}
}
#endif

//======================================================================================================================
//	オブジェクトの取得処理
//======================================================================================================================
Human *GetHumanData(void)
{
	// オブジェクトの情報の先頭アドレスを返す
	return &g_aHuman[0];
}

//============================================================
//	プレイヤーの位置の更新処理
//============================================================
void PosHuman(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove, float fMaxMove)
{
	//--------------------------------------------------------
	//	重力の更新
	//--------------------------------------------------------
	move->y -= HUMAN_GRAVITY;

	//--------------------------------------------------------
	//	移動量の補正
	//--------------------------------------------------------
	if (move->x > fMaxMove)
	{ // プレイヤーの移動量 (x) が一定値以上の場合

	  // プレイヤーの移動量 (x) を補正
		move->x = fMaxMove;
	}
	else if (move->x < -MAX_HUMAN_BACKWARD)
	{ // プレイヤーの移動量 (x) が一定値以下の場合

	  // プレイヤーの移動量 (x) を補正
		move->x = -MAX_HUMAN_BACKWARD;
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
void RevHuman(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos)
{
	//--------------------------------------------------------
	//	向きの正規化
	//--------------------------------------------------------
	if (rot->y > D3DX_PI) { rot->y -= D3DX_PI * 2; }
	else if (rot->y < -D3DX_PI) { rot->y += D3DX_PI * 2; }

	//--------------------------------------------------------
	//	移動範囲の補正
	//--------------------------------------------------------
	if (pos->z > GetLimitStage().fNear - (30.0f * 2))
	{ // 範囲外の場合 (手前)

		// 手前に位置を補正
		pos->z = GetLimitStage().fNear - (30.0f * 2);
	}
	if (pos->z < GetLimitStage().fFar + (30.0f * 2))
	{ // 範囲外の場合 (奥)

		// 奥に位置を補正
		pos->z = GetLimitStage().fFar + (30.0f * 2);
	}
	if (pos->x > GetLimitStage().fRight - (30.0f * 2))
	{ // 範囲外の場合 (右)

		// 右に位置を補正
		pos->x = GetLimitStage().fRight - (30.0f * 2);
	}
	if (pos->x < GetLimitStage().fLeft + (30.0f * 2))
	{ // 範囲外の場合 (左)

		// 左に位置を補正
		pos->x = GetLimitStage().fLeft + (30.0f * 2);
	}
}

//============================================================
// 人間の曲がる処理
//============================================================
void CurveHuman(Human *pHuman)
{
	// 移動量を更新
	pHuman->move.x += HUMAN_MOVE_FORWARD;

	// 移動している状態にする
	pHuman->bMove = true;

	switch (pHuman->curveInfo.actionState)
	{
	case HUMANACT_WALK:		// 歩き状態

		// 人間のすれ違い処理
		PassingHuman(pHuman);

		break;				// 抜け出す

	case HUMANACT_CURVE:	// カーブ状態

		// 人間の角度更新・補正処理
		CurveRotHuman(pHuman);

		break;				// 抜け出す
	}

	if (pHuman->move.x > pHuman->fMaxMove)
	{ // プレイヤーの移動量 (x) が一定値以上の場合

		// プレイヤーの移動量 (x) を補正
		pHuman->move.x = pHuman->fMaxMove;
	}
}

//============================================================
// 人間の歩く処理
//============================================================
void WalkHuman(Human *pHuman)
{
	if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_RIGHT)
	{ // 右に走っている場合

		if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_RIGHT)
		{ // 右に曲がる場合

			if (pHuman->pos.x >= pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x)
			{ // 目的地に着いたら

				// カーブ状態になる
				pHuman->curveInfo.actionState = HUMANACT_CURVE;
			}
			else
			{ // 左にある壁がまだあったら

				// 左の壁に這わせる
				pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;
			}
		}
		else if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_LEFT)
		{ // 左に曲がる場合
			if (pHuman->pos.x >= pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x)
			{ // 目的地に着いたら

				// カーブ状態になる
				pHuman->curveInfo.actionState = HUMANACT_CURVE;
			}
			else
			{ // 左にある壁がまだあったら

				// 左の壁に這わせる
				pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;
			}
		}
	}
	else if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_LEFT)
	{ // 左に走っている場合
		if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_RIGHT)
		{//右に曲がる場合
			if (pHuman->pos.x <= pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x)
			{ // 目的地に着いたら

				// カーブ状態になる
				pHuman->curveInfo.actionState = HUMANACT_CURVE;
			}
			else
			{ // 左にある壁がまだあったら

				// 手前の壁に這わせる
				pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;
			}
		}
		else if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_LEFT)
		{//左に曲がる場合
			if (pHuman->pos.x <= pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x)
			{ // 目的地に着いたら

				// カーブ状態になる
				pHuman->curveInfo.actionState = HUMANACT_CURVE;
			}
			else
			{ // 左にある壁がまだあったら

				// 手前の壁に這わせる
				pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;
			}
		}
	}
	else if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_FAR)
	{ // 奥に走っている場合
		if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_RIGHT)
		{//右に曲がる場合
			if (pHuman->pos.z >= pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z)
			{ // 目的地に着いたら

				// カーブ状態になる
				pHuman->curveInfo.actionState = HUMANACT_CURVE;
			}
			else
			{ // 左にある壁がまだあったら

				// 右の壁に這わせる
				pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;
			}
		}
		else if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_LEFT)
		{ // 左に曲がる場合
			if (pHuman->pos.z >= pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z)
			{ // 目的地に着いたら

				// カーブ状態になる
				pHuman->curveInfo.actionState = HUMANACT_CURVE;
			}
			else
			{ // 左にある壁がまだあったら

				// 右の壁に這わせる
				pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;
			}
		}
	}
	else if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_NEAR)
	{ // 手前に走っている場合
		if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_RIGHT)
		{ // 右に曲がる場合
			if (pHuman->pos.z <= pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z)
			{ // 左にある壁が途切れたら

				// カーブ状態になる
				pHuman->curveInfo.actionState = HUMANACT_CURVE;
			}
			else
			{ // 左にある壁がまだあったら

				// 左の壁に這わせる
				pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;
			}
		}
		else if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_LEFT)
		{ // 左に曲がる場合
			if (pHuman->pos.z <= pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z)
			{ // 左にある壁が途切れたら

				// カーブ状態になる
				pHuman->curveInfo.actionState = HUMANACT_CURVE;
			}
			else
			{ // 左にある壁がまだあったら

				// 左の壁に這わせる
				pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;
			}
		}
	}
}

//============================================================
// 人間の停止処理
//============================================================
void StopHuman(Human *pHuman)
{
	//移動量を0にする
	pHuman->move.x = 0.0f;
}

//============================================================
//人間のリアクション処理
//============================================================
void ReactionHuman(Human *pHuman)
{
	float fLength;										// 長さの変数

	Player *pPlayer = GetPlayer();

	if (pHuman->state != HUMANSTATE_FLY)
	{ // 吹き飛び状態じゃない場合
		if (pPlayer->bUse == true)
		{ // 使用している場合
			// 長さを測る
			fLength = (pPlayer->pos.x - pHuman->pos.x) * (pPlayer->pos.x - pHuman->pos.x)
				+ (pPlayer->pos.z - pHuman->pos.z) * (pPlayer->pos.z - pHuman->pos.z);

			if (fLength <= (pPlayer->modelData.fRadius + REACTION_CAR_RANGE) * REACTION_HUMAN_RANGE)
			{ // プレイヤーが近くに来た場合
				//停止処理に移行
				pHuman->state = HUMANSTATE_STOP;
			}
			else
			{ // プレイヤーに近くにいない場合
				//停止処理に移行
				pHuman->state = HUMANSTATE_WALK;
			}
		}
	}
}

//============================================================
// 人間と車の当たり判定
//============================================================
void CollisionCarHuman(Human *pHuman)
{
	{ // 車の当たり判定
		Car *pCar = GetCarData();					// 車の情報を取得する

		for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
		{
			if (pCar[nCntCar].bUse == true)
			{ // 車が使用されていた場合
				if (pHuman->pos.x + pHuman->modelData.vtxMin.x <= pCar[nCntCar].pos.x + pCar[nCntCar].modelData.vtxMax.x 
					&& pHuman->pos.x + pHuman->modelData.vtxMax.x >= pCar[nCntCar].pos.x + pCar[nCntCar].modelData.vtxMin.x)
				{ // 車のX幅の中にいた場合
					if (pHuman->posOld.z + pHuman->modelData.vtxMax.z <= pCar[nCntCar].posOld.z + pCar[nCntCar].modelData.vtxMin.z
						&& pHuman->pos.z + pHuman->modelData.vtxMax.z >= pCar[nCntCar].pos.z + pCar[nCntCar].modelData.vtxMin.z)
					{//前回の位置がブロックより手前かつ、現在の位置がブロックよりも奥かつだった場合(手前で止められる処理)

						// 移動量を0にする
						pCar[nCntCar].move.x = 0.0f;

					}							//手前で止められる処理
					else if (pHuman->posOld.z + pHuman->modelData.vtxMin.z >= pCar[nCntCar].posOld.z + pCar[nCntCar].modelData.vtxMax.z
						&& pHuman->pos.z + pHuman->modelData.vtxMin.z <= pCar[nCntCar].pos.z + pCar[nCntCar].modelData.vtxMax.z)
					{//前回の位置が塔の位置よりも奥かつ、現在の位置が塔の位置よりも手前だった場合(奥で止められる処理)
					
						// 移動量を0にする
						pCar[nCntCar].move.x = 0.0f;

					}							//奥で止められる処理
				}

				if (pHuman->pos.z + pHuman->modelData.vtxMin.z <= pCar[nCntCar].pos.z + pCar[nCntCar].modelData.vtxMax.z
					&& pHuman->pos.z + pHuman->modelData.vtxMax.z >= pCar[nCntCar].pos.z + pCar[nCntCar].modelData.vtxMin.z)
				{//塔のZ幅の中にいた場合
					if (pHuman->posOld.x + pHuman->modelData.vtxMax.x <= pCar[nCntCar].posOld.x + pCar[nCntCar].modelData.vtxMin.x
						&& pHuman->pos.x + pHuman->modelData.vtxMax.x >= pCar[nCntCar].pos.x + pCar[nCntCar].modelData.vtxMin.x)
					{//前回の位置がブロックの左端より左かつ、現在の位置がブロックの左側より右だった場合(左の処理)
						
						//移動量を0にする
						pCar[nCntCar].move.x = 0.0f;

					}							//左端の処理
					else if (pHuman->posOld.x + pHuman->modelData.vtxMin.x >= pCar[nCntCar].posOld.x + pCar[nCntCar].modelData.vtxMax.x
						&& pHuman->pos.x + pHuman->modelData.vtxMin.x <= pCar[nCntCar].pos.x + pCar[nCntCar].modelData.vtxMax.x)
					{//前回の位置がブロックの右端より右かつ、現在の位置がブロックの左側より右だった場合(右の処理)
						
						// 移動量を0にする
						pCar[nCntCar].move.x = 0.0f;

					}							//右端の処理
				}
			}
		}
	}

	{ // 警察の当たり判定
		Police *pPolice = GetPoliceData();					// 警察の情報を取得する

		for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
		{
			if (pPolice[nCntPolice].bUse == true)
			{ // 車が使用されていた場合
				if (pHuman->pos.x + pHuman->modelData.vtxMin.x <= pPolice[nCntPolice].pos.x + pPolice[nCntPolice].modelData.vtxMax.x 
					&& pHuman->pos.x + pHuman->modelData.vtxMax.x >= pPolice[nCntPolice].pos.x + pPolice[nCntPolice].modelData.vtxMin.x)
				{ // 車のX幅の中にいた場合
					if (pHuman->posOld.z + pHuman->modelData.vtxMax.z <= pPolice[nCntPolice].posOld.z + pPolice[nCntPolice].modelData.vtxMin.z
						&& pHuman->pos.z + pHuman->modelData.vtxMax.z >= pPolice[nCntPolice].pos.z + pPolice[nCntPolice].modelData.vtxMin.z)
					{//前回の位置がブロックより手前かつ、現在の位置がブロックよりも奥かつだった場合(手前で止められる処理)

						// 移動量を0.0fにする
						pPolice[nCntPolice].move.x = 0.0f;

					}							//手前で止められる処理
					else if (pHuman->posOld.z + pHuman->modelData.vtxMin.z >= pPolice[nCntPolice].posOld.z + pPolice[nCntPolice].modelData.vtxMax.z
						&& pHuman->pos.z + pHuman->modelData.vtxMin.z <= pPolice[nCntPolice].pos.z + pPolice[nCntPolice].modelData.vtxMax.z)
					{//前回の位置が塔の位置よりも奥かつ、現在の位置が塔の位置よりも手前だった場合(奥で止められる処理)

						// 移動量を0.0fにする
						pPolice[nCntPolice].move.x = 0.0f;

					}							//奥で止められる処理
				}

				if (pHuman->pos.z + pHuman->modelData.vtxMin.z <= pPolice[nCntPolice].pos.z + pPolice[nCntPolice].modelData.vtxMax.z
					&& pHuman->pos.z + pHuman->modelData.vtxMax.z >= pPolice[nCntPolice].pos.z + pPolice[nCntPolice].modelData.vtxMin.z)
				{//塔のZ幅の中にいた場合
					if (pHuman->posOld.x + pHuman->modelData.vtxMax.x <= pPolice[nCntPolice].posOld.x + pPolice[nCntPolice].modelData.vtxMin.x
						&& pHuman->pos.x + pHuman->modelData.vtxMax.x >= pPolice[nCntPolice].pos.x + pPolice[nCntPolice].modelData.vtxMin.x)
					{//前回の位置がブロックの左端より左かつ、現在の位置がブロックの左側より右だった場合(左の処理)
						
						// 移動量を0.0fにする
						pPolice[nCntPolice].move.x = 0.0f;

					}							//左端の処理
					else if (pHuman->posOld.x + pHuman->modelData.vtxMin.x >= pPolice[nCntPolice].posOld.x + pPolice[nCntPolice].modelData.vtxMax.x
						&& pHuman->pos.x + pHuman->modelData.vtxMin.x <= pPolice[nCntPolice].pos.x + pPolice[nCntPolice].modelData.vtxMax.x)
					{//前回の位置がブロックの右端より右かつ、現在の位置がブロックの左側より右だった場合(右の処理)
						
						// 移動量を0.0fにする
						pPolice[nCntPolice].move.x = 0.0f;

					}							//右端の処理
				}
			}
		}
	}
}

//============================================================
// 人間の角度更新・補正処理
//============================================================
void CurveRotHuman(Human *pHuman)
{
	if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_LEFT)
	{ // 曲がる方向が左だった場合

		// 移動量を減速
		pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

		// 向きを更新
		pHuman->rot.y -= 0.05f * (pHuman->move.x * 0.1f);

		if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_RIGHT)
		{ // 右に走っている場合

			// 位置を補正する
			pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;

			// 目標の向きを設定する
			pHuman->curveInfo.rotDest.y = 0.0f;
		}
		else if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_LEFT)
		{ // 左に走っている場合

			// 位置を補正する
			pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;

			// 目標の向きを設定する
			pHuman->curveInfo.rotDest.y = -D3DX_PI;
		}
		else if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_FAR)
		{ // 奥に走っている場合

			// 位置を補正する
			pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;

			// 目標の向きを設定する
			pHuman->curveInfo.rotDest.y = -D3DX_PI * 0.5f;
		}
		else if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_NEAR)
		{ // 手前に走っている場合

			// 位置を補正する
			pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;

			// 目標の向きを設定する
			pHuman->curveInfo.rotDest.y = D3DX_PI * 0.5f;
		}

		// 向きを加える
		pHuman->rot.y -= HUMAN_CURVE_ADD;

		if (pHuman->rot.y <= pHuman->curveInfo.rotDest.y)
		{ // 向きが目標値を超えた場合

			// 向きを補正する
			pHuman->rot.y = pHuman->curveInfo.rotDest.y;

			// 走行状態にする
			pHuman->curveInfo.actionState = HUMANACT_WALK;

			// 警察の行先を設定する
			pHuman->curveInfo.curveInfo.nNowCurve = (pHuman->curveInfo.curveInfo.nNowCurve + 1) % pHuman->curveInfo.curveInfo.nCurveTime;

			if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_NEAR)
			{ // 奥に走っている場合

				if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_LEFT)
				{ // 次の曲がり角が左だった場合

					// 向きをマイナスにする
					pHuman->rot.y = (float)D3DXToRadian(180);
				}
			}
		}
	}
	else
	{ // 曲がる方向が右だった場合

		// 移動量を減速
		pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

		// 向きを更新
		pHuman->rot.y += 0.05f * (pHuman->move.x * 0.1f);

		if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_RIGHT)
		{ // 右に走っている場合

			//位置を補正する
			pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;

			// 目標の向きを設定する
			pHuman->curveInfo.rotDest.y = D3DX_PI;
		}
		else if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_LEFT)
		{ // 左に走っている場合

			// 位置を補正する
			pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;

			// 目標の向きを設定する
			pHuman->curveInfo.rotDest.y = 0.0f;
		}
		else if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_FAR)
		{ // 奥に走っている場合

			// 位置を補正する
			pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;

			// 目標の向きを設定する
			pHuman->curveInfo.rotDest.y = D3DX_PI * 0.5f;
		}
		else if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_NEAR)
		{ // 手前に走っている場合

			// 位置を補正する
			pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;

			// 目標の向きを設定する
			pHuman->curveInfo.rotDest.y = -D3DX_PI * 0.5f;
		}

		if (pHuman->rot.y >= pHuman->curveInfo.rotDest.y)
		{ // 向きが目標値を超えた場合

			// 向きを補正する
			pHuman->rot.y = pHuman->curveInfo.rotDest.y;

			// 走行状態にする
			pHuman->curveInfo.actionState = HUMANACT_WALK;

			// 警察の行先を設定する
			pHuman->curveInfo.curveInfo.nNowCurve = (pHuman->curveInfo.curveInfo.nNowCurve + 1) % pHuman->curveInfo.curveInfo.nCurveTime;

			if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_NEAR)
			{ // 奥に走っている場合

				if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_RIGHT)
				{ // 次の曲がり角がマイナス方向に曲がる場合

					// 向きをマイナスにする
					pHuman->rot.y = (float)D3DXToRadian(-180);
				}
			}
		}
	}

	// 向きの正規化
	RotNormalize(&pHuman->rot.y);
}

//============================================================
// 人間のすれ違い処理
//============================================================
void PassingHuman(Human *pHuman)
{
	Human *pPassHuman = GetHumanData();		// 人間の情報
	float fLength;							// 距離
	float posDest;							// 目標の位置
	float posDiff;							// 位置の差分

	for (int nCnt = 0; nCnt < MAX_HUMAN; nCnt++,pPassHuman++)
	{
		if (pPassHuman != pHuman)
		{ // 比較する人間が自分自身じゃない場合
			if (pPassHuman->bUse == true)
			{ // 人間が使用されていた場合
				//長さを測る
				fLength = (pPassHuman->pos.x - pHuman->pos.x) * (pPassHuman->pos.x - pHuman->pos.x)
					+ (pPassHuman->pos.z - pHuman->pos.z) * (pPassHuman->pos.z - pHuman->pos.z);

				if (fLength <= (HUMAN_RADIUS * HUMAN_RADIUS))
				{ // オブジェクトが当たっている
					if (pHuman->move.x >= pPassHuman->move.x)
					{ // 移動量が速い場合
						switch (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve])
						{
						case DASH_RIGHT:		// 右

							// 目標の位置を設定
							posDest = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z - HUMAN_PASS_SHIFT;

							// 位置の差分を求める
							posDiff = posDest - pHuman->pos.z;

							// 幅をずらす
							pHuman->pos.z += posDiff * HUMAN_PASS_CORRECT;

							break;				// 抜け出す

						case DASH_LEFT:			// 左

							// 目標の位置を設定
							posDest = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z + HUMAN_PASS_SHIFT;

							// 位置の差分を求める
							posDiff = posDest - pHuman->pos.z;

							// 幅をずらす
							pHuman->pos.z += posDiff * HUMAN_PASS_CORRECT;

							break;				// 抜け出す

						case DASH_FAR:			// 奥

							// 目標の位置を設定
							posDest = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x + HUMAN_PASS_SHIFT;

							// 位置の差分を求める
							posDiff = posDest - pHuman->pos.x;

							// 幅をずらす
							pHuman->pos.x += posDiff * HUMAN_PASS_CORRECT;

							break;				// 抜け出す

						case DASH_NEAR:			// 手前

							// 目標の位置を設定
							posDest = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x - HUMAN_PASS_SHIFT;

							// 位置の差分を求める
							posDiff = posDest - pHuman->pos.x;

							// 幅をずらす
							pHuman->pos.x += posDiff * HUMAN_PASS_CORRECT;

							break;				// 抜け出す
						}
					}
				}
				else
				{ // オブジェクトが当たっている

					// 人間の歩く処理
					WalkHuman(pHuman);
				}
			}
		}
		else
		{ // 上記以外

			// 人間の歩く処理
			WalkHuman(pHuman);
		}
	}
}

#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================
#endif