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

#include "Human.h"
#include "shadow.h"
#include "sound.h"
#include "player.h"
#include "Police.h"
#include "curve.h"
#include "object.h"
#include "wind.h"

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
#define MAX_HUMAN_FORWARD			(2.0f)		// 前進時の最高速度
#define MAX_HUMAN_BACKWARD			(8.0f)		// 後退時の最高速度
#define REV_HUMAN_MOVE_SUB			(0.04f)		// 移動量の減速係数
#define HUMAN_WIDTH					(10.0f)		// 人の縦幅
#define HUMAN_DEPTH					(10.0f)		// 人の奥行

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void PosHuman(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove);		// 人間の位置の更新処理
void RevHuman(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos);	// 人間の補正の更新処理
void CurveHuman(Human *pHuman);						// 人間のカーブ処理
void CurveRotHuman(Human *pHuman);					// 人間の角度更新処理
void StopHuman(Human *pHuman);						// 人間の停止処理
void ReactionHuman(Human *pHuman);					// 人間のリアクション処理

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
Human g_aHuman[MAX_HUMAN];		// 人間の情報

//======================================================================================================================
//	人間の初期化処理
//======================================================================================================================
void InitHuman(void)
{
	CURVE HumanCurveInfo;				//情報

	//曲がる回数
	HumanCurveInfo.nCurveTime = 8;

	for (int nCntInfo = 0; nCntInfo < HumanCurveInfo.nCurveTime; nCntInfo++)
	{
		switch (nCntInfo)
		{
		case 0:

			//次曲がるポイント
			HumanCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(6500.0f, 0.0f, 2000.0f);

			//今走ってる軸
			HumanCurveInfo.bCurveX[nCntInfo] = false;

			//今走っている方向
			HumanCurveInfo.bCurvePlus[nCntInfo] = true;

			//次曲がる方向
			HumanCurveInfo.curveAngle[nCntInfo] = CURVE_LEFT;

			//次に向く角度
			HumanCurveInfo.fCurveRot[nCntInfo] = -90;

			break;				//抜け出す

		case 1:

			//次曲がるポイント
			HumanCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(2000.0f, 0.0f, 2000.0f);

			//今走ってる軸
			HumanCurveInfo.bCurveX[nCntInfo] = true;

			//今走っている方向
			HumanCurveInfo.bCurvePlus[nCntInfo] = false;

			//次曲がる方向
			HumanCurveInfo.curveAngle[nCntInfo] = CURVE_RIGHT;

			//次に向く角度
			HumanCurveInfo.fCurveRot[nCntInfo] = 0;

			break;				//抜け出す

		case 2:

			//次曲がるポイント
			HumanCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(2000.0f, 0.0f, 6500.0f);

			//今走ってる軸
			HumanCurveInfo.bCurveX[nCntInfo] = false;

			//今走っている方向
			HumanCurveInfo.bCurvePlus[nCntInfo] = true;

			//次曲がる方向
			HumanCurveInfo.curveAngle[nCntInfo] = CURVE_LEFT;

			//次に向く角度
			HumanCurveInfo.fCurveRot[nCntInfo] = -90;

			break;

		case 3:

			//次曲がるポイント
			HumanCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(-6500.0f, 0.0f, 6500.0f);

			//今走ってる軸
			HumanCurveInfo.bCurveX[nCntInfo] = true;

			//今走っている方向
			HumanCurveInfo.bCurvePlus[nCntInfo] = false;

			//次曲がる方向
			HumanCurveInfo.curveAngle[nCntInfo] = CURVE_LEFT;

			//次に向く角度
			HumanCurveInfo.fCurveRot[nCntInfo] = -180;

			break;				//抜け出す

		case 4:

			//次曲がるポイント
			HumanCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(-6500.0f, 0.0f, 2500.0f);

			//今走ってる軸
			HumanCurveInfo.bCurveX[nCntInfo] = false;

			//今走っている方向
			HumanCurveInfo.bCurvePlus[nCntInfo] = false;

			//次曲がる方向
			HumanCurveInfo.curveAngle[nCntInfo] = CURVE_LEFT;

			//次に向く角度
			HumanCurveInfo.fCurveRot[nCntInfo] = 90;

			break;				//抜け出す

		case 5:

			//次曲がるポイント
			HumanCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(-2250.0f, 0.0f, 2500.0f);

			//今走ってる軸
			HumanCurveInfo.bCurveX[nCntInfo] = true;

			//今走っている方向
			HumanCurveInfo.bCurvePlus[nCntInfo] = true;

			//次曲がる方向
			HumanCurveInfo.curveAngle[nCntInfo] = CURVE_RIGHT;

			//次に向く角度
			HumanCurveInfo.fCurveRot[nCntInfo] = 180;

			break;				//抜け出す

		case 6:

			//次曲がるポイント
			HumanCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(-2250.0f, 0.0f, -2000.0f);

			//今走ってる軸
			HumanCurveInfo.bCurveX[nCntInfo] = false;

			//今走っている方向
			HumanCurveInfo.bCurvePlus[nCntInfo] = false;

			//次曲がる方向
			HumanCurveInfo.curveAngle[nCntInfo] = CURVE_LEFT;

			//次に向く角度
			HumanCurveInfo.fCurveRot[nCntInfo] = 90;

			break;				//抜け出す

		case 7:

			//次曲がるポイント
			HumanCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(6500.0f, 0.0f, -2000.0f);

			//今走ってる軸
			HumanCurveInfo.bCurveX[nCntInfo] = true;

			//今走っている方向
			HumanCurveInfo.bCurvePlus[nCntInfo] = true;

			//次曲がる方向
			HumanCurveInfo.curveAngle[nCntInfo] = CURVE_LEFT;

			//次に向く角度
			HumanCurveInfo.fCurveRot[nCntInfo] = 0;

			break;				//抜け出す
		}
	}
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

												// 人間の情報の初期化
	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++)
	{ // オブジェクトの最大表示数分繰り返す

		g_aHuman[nCntHuman].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aHuman[nCntHuman].posOld = g_aHuman[nCntHuman].pos;			// 前回の位置
		g_aHuman[nCntHuman].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
		g_aHuman[nCntHuman].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
		g_aHuman[nCntHuman].nShadowID = NONE_SHADOW;				// 影のインデックス
		g_aHuman[nCntHuman].bMove = false;							// 移動しているか
		g_aHuman[nCntHuman].bUse = false;							// 使用状況
		g_aHuman[nCntHuman].state = HUMANSTATE_WALK;				// 状態

		//曲がり角の情報を初期化
		for (int nCntCurve = 0; nCntCurve < MAX_CURVE; nCntCurve++)
		{
			g_aHuman[nCntHuman].humanCurve.curveAngle[nCntCurve] = CURVE_RIGHT;						// 曲がる方向
			g_aHuman[nCntHuman].humanCurve.curvePoint[nCntCurve] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 曲がるポイント
			g_aHuman[nCntHuman].humanCurve.nNowCurve = 0;												// 現在の曲がり角
			g_aHuman[nCntHuman].humanCurve.bCurveX[nCntCurve] = false;								// X軸を走っているか
			g_aHuman[nCntHuman].humanCurve.fCurveRot[nCntCurve] = 0;									// 目標の向き
		}
		g_aHuman[nCntHuman].humanCurve.nCurveTime = 0;				// 曲がる回数
		g_aHuman[nCntHuman].humanCurve.nNowCurve = 0;					// 現在のルート

		// モデル情報の初期化
		g_aHuman[nCntHuman].modelData.dwNumMat = 0;					// マテリアルの数
		g_aHuman[nCntHuman].modelData.pTexture = NULL;				// テクスチャへのポインタ
		g_aHuman[nCntHuman].modelData.pMesh = NULL;					// メッシュ (頂点情報) へのポインタ
		g_aHuman[nCntHuman].modelData.pBuffMat = NULL;				// マテリアルへのポインタ
		g_aHuman[nCntHuman].modelData.vtxMin = INIT_VTX_MIN;		// 最小の頂点座標
		g_aHuman[nCntHuman].modelData.vtxMax = INIT_VTX_MAX;		// 最大の頂点座標
		g_aHuman[nCntHuman].modelData.size = INIT_SIZE;				// 縦幅
		g_aHuman[nCntHuman].modelData.fRadius = 0.0f;				// 半径
	}

	//人間の設定処理
	SetHuman(D3DXVECTOR3(6000.0f, 0.0f, -1000.0f), HumanCurveInfo);
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
	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++)
	{ // オブジェクトの最大表示数分繰り返す
		if (g_aHuman[nCntHuman].bUse == true)
		{ // オブジェクトが使用されている場合

			// 前回位置の更新
			g_aHuman[nCntHuman].posOld = g_aHuman[nCntHuman].pos;

			//----------------------------------------------------
			//	影の更新
			//----------------------------------------------------
			// 影の位置設定
			SetPositionShadow
			( // 引数
				g_aHuman[nCntHuman].nShadowID,	// 影のインデックス
				g_aHuman[nCntHuman].pos,			// 位置
				g_aHuman[nCntHuman].rot,			// 向き
				NONE_SCALE							// 拡大率
			);

			// プレイヤーの位置の更新
			PosHuman(&g_aHuman[nCntHuman].move, &g_aHuman[nCntHuman].pos, &g_aHuman[nCntHuman].rot, g_aHuman[nCntHuman].bMove);

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
				HUMAN_DEPTH						// 奥行
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
void SetHuman(D3DXVECTOR3 pos, CURVE humanCurve)
{
	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aHuman[nCntHuman].bUse == false)
		{ // オブジェクトが使用されていない場合
		  // 引数を代入
			g_aHuman[nCntHuman].pos = pos;								// 現在の位置
			g_aHuman[nCntHuman].posOld = g_aHuman[nCntHuman].pos;		// 前回の位置
			g_aHuman[nCntHuman].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
			g_aHuman[nCntHuman].bMove = false;							// 移動していない
			g_aHuman[nCntHuman].state = HUMANSTATE_WALK;				//歩き状態

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

			//曲がり角関係の設定
			for (int nCntCurve = 0; nCntCurve < humanCurve.nCurveTime; nCntCurve++)
			{
				g_aHuman[nCntHuman].humanCurve.fCurveRot[nCntCurve] = humanCurve.fCurveRot[nCntCurve];	// 次曲がる向き
				g_aHuman[nCntHuman].humanCurve.curvePoint[nCntCurve] = humanCurve.curvePoint[nCntCurve];	// 次曲がるポイント
			}
			g_aHuman[nCntHuman].humanCurve.nNowCurve = 0;													// 現在のルート
			g_aHuman[nCntHuman].humanCurve.nCurveTime = humanCurve.nCurveTime;							// 曲がる回数

			//カーブの設定処理
			SetCurvePointHuman(&g_aHuman[nCntHuman].humanCurve, &g_aHuman[nCntHuman].rot, &g_aHuman[nCntHuman].pos);

			if (g_aHuman[nCntHuman].humanCurve.bCurveX[0] == true)
			{ // 最初X軸を走っている場合
				if (g_aHuman[nCntHuman].humanCurve.bCurvePlus[0] == true)
				{ // 右に走っている場合
					// 位置を補正する
					g_aHuman[nCntHuman].pos.z = g_aHuman[nCntHuman].humanCurve.curvePoint[0].z - (HUMAN_WIDTH * 2);
				}
				else
				{ // 左に走っている場合
					// 位置を補正する
					g_aHuman[nCntHuman].pos.z = g_aHuman[nCntHuman].humanCurve.curvePoint[0].z + (HUMAN_WIDTH * 2);
				}
			}
			else
			{ // 最初Z軸を走っている場合
				if (g_aHuman[nCntHuman].humanCurve.bCurvePlus[0] == true)
				{ // 奥に走っている場合
				  // 位置を補正する
					g_aHuman[nCntHuman].pos.x = g_aHuman[nCntHuman].humanCurve.curvePoint[0].x + (HUMAN_WIDTH * 2);
				}
				else
				{ // 手前に走っている場合
				  // 位置を補正する
					g_aHuman[nCntHuman].pos.x = g_aHuman[nCntHuman].humanCurve.curvePoint[0].x - (HUMAN_WIDTH * 2);
				}
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
void PosHuman(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove)
{
	//--------------------------------------------------------
	//	重力の更新
	//--------------------------------------------------------
	move->y -= HUMAN_GRAVITY;

	//--------------------------------------------------------
	//	移動量の補正
	//--------------------------------------------------------
	if (move->x > MAX_HUMAN_FORWARD)
	{ // プレイヤーの移動量 (x) が一定値以上の場合

	  // プレイヤーの移動量 (x) を補正
		move->x = MAX_HUMAN_FORWARD;
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

	if (pHuman->humanCurve.bCurveX[pHuman->humanCurve.nNowCurve] == true)
	{//X軸を走っていた場合
		if (pHuman->humanCurve.bCurvePlus[pHuman->humanCurve.nNowCurve] == true)
		{ // 右に走っている場合
			if (pHuman->humanCurve.curveAngle[pHuman->humanCurve.nNowCurve] == CURVE_RIGHT)
			{//右に曲がる場合
				if (pHuman->pos.x >= pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x - (HUMAN_WIDTH * 2))
				{ // 左にある壁が途切れたら

				  // 人間の角度更新・補正処理
					CurveRotHuman(pHuman);

					// 移動量を減速
					pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

					// 右の壁に這わせる
					pHuman->pos.x = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x - (HUMAN_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
				  // 左の壁に這わせる
					pHuman->pos.z = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z - (HUMAN_WIDTH * 2);
				}
			}
			else if (pHuman->humanCurve.curveAngle[pHuman->humanCurve.nNowCurve] == CURVE_LEFT)
			{//左に曲がる場合
				if (pHuman->pos.x >= pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x + (HUMAN_WIDTH * 2))
				{ // 左にある壁が途切れたら

				  // 人間の角度更新・補正処理
					CurveRotHuman(pHuman);

					// 移動量を減速
					pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

					// 右の壁に這わせる
					pHuman->pos.x = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x + (HUMAN_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
				  // 左の壁に這わせる
					pHuman->pos.z = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z - (HUMAN_WIDTH * 2);
				}
			}
		}
		else
		{ // 左に走っている場合
			if (pHuman->humanCurve.curveAngle[pHuman->humanCurve.nNowCurve] == CURVE_RIGHT)
			{//右に曲がる場合
				if (pHuman->pos.x <= pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x + (HUMAN_WIDTH * 2))
				{ // 左にある壁が途切れたら

				  // 人間の角度更新・補正処理
					CurveRotHuman(pHuman);

					// 移動量を減速
					pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

					// 左の壁に這わせる
					pHuman->pos.x = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x + (HUMAN_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
				  // 手前の壁に這わせる
					pHuman->pos.z = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z + (HUMAN_WIDTH * 2);
				}
			}
			else if (pHuman->humanCurve.curveAngle[pHuman->humanCurve.nNowCurve] == CURVE_LEFT)
			{//左に曲がる場合
				if (pHuman->pos.x <= pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x - (HUMAN_WIDTH * 2))
				{ // 左にある壁が途切れたら

				  // 人間の角度更新・補正処理
					CurveRotHuman(pHuman);

					// 移動量を減速
					pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

					// 左の壁に這わせる
					pHuman->pos.x = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x - (HUMAN_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
				  // 手前の壁に這わせる
					pHuman->pos.z = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z + (HUMAN_WIDTH * 2);
				}
			}
		}
	}
	else
	{//Z軸を走っていた場合
		if (pHuman->humanCurve.bCurvePlus[pHuman->humanCurve.nNowCurve] == true)
		{ // 右の壁が警察より左側にある場合
			if (pHuman->humanCurve.curveAngle[pHuman->humanCurve.nNowCurve] == CURVE_RIGHT)
			{//右に曲がる場合
				if (pHuman->pos.z >= pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z - (HUMAN_WIDTH * 2))
				{ // 左にある壁が途切れたら

				  // 人間の角度更新・補正処理
					CurveRotHuman(pHuman);

					// 移動量を減速
					pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

					// 手前の壁に這わせる
					pHuman->pos.z = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z - (HUMAN_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
				  // 右の壁に這わせる
					pHuman->pos.x = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x + (HUMAN_WIDTH * 2);
				}
			}
			else if (pHuman->humanCurve.curveAngle[pHuman->humanCurve.nNowCurve] == CURVE_LEFT)
			{ // 左に曲がる場合
				if (pHuman->pos.z >= pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z + (HUMAN_WIDTH * 2))
				{ // 左にある壁が途切れたら

				  // 人間の角度更新・補正処理
					CurveRotHuman(pHuman);

					// 移動量を減速
					pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

					// 手前の壁に這わせる
					pHuman->pos.z = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z + (HUMAN_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
				  // 右の壁に這わせる
					pHuman->pos.x = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x + (HUMAN_WIDTH * 2);
				}
			}
		}
		else
		{//左の壁が警察より右にある場合
			if (pHuman->humanCurve.curveAngle[pHuman->humanCurve.nNowCurve] == CURVE_RIGHT)
			{//右に曲がる場合
				if (pHuman->pos.z <= pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z + (HUMAN_WIDTH * 2))
				{ // 左にある壁が途切れたら

				  // 人間の角度更新・補正処理
					CurveRotHuman(pHuman);

					// 移動量を減速
					pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

					// 左の壁に這わせる
					pHuman->pos.z = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z + (HUMAN_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
				  // 左の壁に這わせる
					pHuman->pos.x = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x - (HUMAN_WIDTH * 2);
				}
			}
			else if (pHuman->humanCurve.curveAngle[pHuman->humanCurve.nNowCurve] == CURVE_LEFT)
			{
				if (pHuman->pos.z <= pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z - (HUMAN_WIDTH * 2))
				{ // 左にある壁が途切れたら

				  // 人間の角度更新・補正処理
					CurveRotHuman(pHuman);

					// 移動量を減速
					pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

					// 左の壁に這わせる
					pHuman->pos.z = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z - (HUMAN_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
				  // 左の壁に這わせる
					pHuman->pos.x = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x - (HUMAN_WIDTH * 2);
				}
			}
		}
	}

	if (pHuman->move.x > MAX_HUMAN_FORWARD)
	{ // プレイヤーの移動量 (x) が一定値以上の場合

	  // プレイヤーの移動量 (x) を補正
		pHuman->move.x = MAX_HUMAN_FORWARD;
	}
}

//============================================================
// 人間の角度更新・補正処理
//============================================================
void CurveRotHuman(Human *pHuman)
{
	if (pHuman->humanCurve.curveAngle[pHuman->humanCurve.nNowCurve] == CURVE_LEFT)
	{ // 曲がる方向が左だった場合
	  // 向きを更新
		pHuman->rot.y -= 0.05f * (pHuman->move.x * 0.1f);

		if (pHuman->rot.y <= (float)D3DXToRadian(pHuman->humanCurve.fCurveRot[pHuman->humanCurve.nNowCurve]))
		{ // 一定の向きに達した場合
		  // 向きを補正
			pHuman->rot.y = (float)D3DXToRadian(pHuman->humanCurve.fCurveRot[pHuman->humanCurve.nNowCurve]);

			if (pHuman->humanCurve.curveAngle[(pHuman->humanCurve.nNowCurve + 1) % pHuman->humanCurve.nCurveTime] == CURVE_LEFT)
			{//次の曲がり角が左だった場合
				if (pHuman->humanCurve.fCurveRot[(pHuman->humanCurve.nNowCurve + 1) % pHuman->humanCurve.nCurveTime] == 90)
				{//次の曲がり角がマイナス方向に曲がる場合
				 //向きをプラスに戻す
					pHuman->rot.y = (float)D3DXToRadian(180);
				}
			}

			//// 人間の補正の更新処理
			//RevHuman(&pHuman->rot, &pHuman->pos);

			// 警察の行先を設定する
			pHuman->humanCurve.nNowCurve = (pHuman->humanCurve.nNowCurve + 1) % pHuman->humanCurve.nCurveTime;
		}
	}
	else if (pHuman->humanCurve.curveAngle[pHuman->humanCurve.nNowCurve] == CURVE_RIGHT)
	{ // 曲がる方向が右だった場合
	  // 向きを更新
		pHuman->rot.y += 0.05f * (pHuman->move.x * 0.1f);

		if (pHuman->rot.y >= (float)D3DXToRadian(pHuman->humanCurve.fCurveRot[pHuman->humanCurve.nNowCurve]))
		{ // 一定の向きに達した場合
		  // 向きを補正
			pHuman->rot.y = (float)D3DXToRadian(pHuman->humanCurve.fCurveRot[pHuman->humanCurve.nNowCurve]);

			if (pHuman->humanCurve.curveAngle[(pHuman->humanCurve.nNowCurve + 1) % pHuman->humanCurve.nCurveTime] == CURVE_RIGHT)
			{//次の曲がり角が右だった場合
				if (pHuman->humanCurve.fCurveRot[(pHuman->humanCurve.nNowCurve + 1) % pHuman->humanCurve.nCurveTime] == -90)
				{//次の曲がり角がマイナス方向に曲がる場合
				 //向きをプラスに戻す
					pHuman->rot.y = (float)D3DXToRadian(-180);
				}
			}

			//// 人間の補正の更新処理
			//RevHuman(&pHuman->rot, &pHuman->pos);

			// 警察の行先を設定する
			pHuman->humanCurve.nNowCurve = (pHuman->humanCurve.nNowCurve + 1) % pHuman->humanCurve.nCurveTime;
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

			if (fLength <= (pPlayer->modelData.fRadius + 50.0f) * 170.0f)
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

#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================
#endif