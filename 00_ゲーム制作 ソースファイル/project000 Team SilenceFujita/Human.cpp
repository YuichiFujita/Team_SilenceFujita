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
#define HUMAN_SETUP_TXT			"data\\TXT\\player.txt"		// 人間のセットアップ用のテキストファイルの相対パス

#define HUMAN_LIFE				(50)		// 人の体力
#define HUMAN_GRAVITY			(1.0f)		// 人にかかる重力
#define HUMAN_MOVE_FORWARD		(0.1f)		// 人前進時の移動量
#define HUMAN_MOVE_BACKWARD		(0.2f)		// 人後退時の移動量
#define HUMAN_MOVE_ROT			(0.012f)	// 人の向き変更量
#define REV_HUMAN_MOVE_ROT		(0.1f)		// 移動量による向き変更量の補正係数
#define SUB_HUMAN_MOVE_VALUE	(15.0f)		// 向き変更時の減速が行われる移動量
#define SUB_HUMAN_MOVE			(0.05f)		// 向き変更時の減速量
#define MAX_HUMAN_BACKWARD		(8.0f)		// 後退時の最高速度
#define REV_HUMAN_MOVE_SUB		(0.04f)		// 移動量の減速係数
#define HUMAN_CURVE_ADD			(0.03f)		// 曲がり角での向きの加算数
#define HUMAN_RANDAM_MOVE		(6)			// 人間の移動量のランダム
#define HUMAN_MOVE_LEAST		(4)			// 人間の移動量の最低限
#define HUMAN_PASS_SHIFT		(40.0f)		// すれ違った時のずらす幅
#define HUMAN_RADIUS			(30.0f)		// 人間の幅
#define HUMAN_PASS_CORRECT		(0.06f)		// 人間のずらす補正倍率
#define REACTION_HUMAN_RANGE	(170.0f)	// リアクションする人間の範囲
#define REACTION_CAR_RANGE		(50.0f)		// リアクションする車の範囲

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

void UpdateMotionHuman(Human *pHuman);						// 人間モーションの更新処理
void SetMotionHuman(Human *pHuman, MOTIONTYPE type);		// 人間モーションの設定処理
void TxtSetHuman(HumanParts *setParts, KeyInfo *setmotion);	// 人間のセットアップ処理

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
Human g_aHuman[MAX_HUMAN];	// 人間の情報

//======================================================================================================================
//	人間の初期化処理
//======================================================================================================================
void InitHuman(void)
{
	// 変数配列を宣言
	HumanParts setParts[MAX_PARTS];				// パーツのセットアップ用
	KeyInfo    setMotion[MAX_MOTION];			// モーションのセットアップ用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// 人間のセットアップ
	TxtSetHuman(&setParts[0], &setMotion[0]);

	// 人間の情報の初期化
	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++)
	{ // 人間の最大表示数分繰り返す

		// 基本情報の初期化
		g_aHuman[nCntHuman].pos       = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aHuman[nCntHuman].posOld    = g_aHuman[nCntHuman].pos;		// 前回の位置
		g_aHuman[nCntHuman].move      = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
		g_aHuman[nCntHuman].fMaxMove  = 0.0f;							// 移動量の最大数
		g_aHuman[nCntHuman].rot       = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
		g_aHuman[nCntHuman].nShadowID = NONE_SHADOW;					// 影のインデックス
		g_aHuman[nCntHuman].bJump     = false;							// ジャンプしているかどうか
		g_aHuman[nCntHuman].bMove     = false;							// 移動しているか
		g_aHuman[nCntHuman].bUse      = false;							// 使用状況
		g_aHuman[nCntHuman].state     = HUMANSTATE_WALK;				// 状態

		// モーションの基本情報の初期化
		g_aHuman[nCntHuman].motion.type     = MOTIONTYPE_NONE;	// モーションの種類
		g_aHuman[nCntHuman].motion.nPose    = 0;				// モーションのポーズ番号
		g_aHuman[nCntHuman].motion.nCounter = 0;				// モーションのカウンター

		// モーションの情報を初期化
		for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
		{ // モーションの最大数分繰り返す

			// モーションの情報を初期化
			g_aHuman[nCntHuman].motion.aMotion[nCntMotion] = setMotion[nCntMotion];
		}

		// パーツ情報の初期化
		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{ // パーツの最大数分繰り返す

			// パーツの基本情報を初期化
			g_aHuman[nCntHuman].aParts[nCntParts] = setParts[nCntParts];

			// パーツのモデル情報を初期化
			g_aHuman[nCntHuman].aParts[nCntParts].modelData = GetModelData(nCntParts + FROM_HUMAN);
		}

		// モーションの設定
		SetMotionHuman(&g_aHuman[nCntHuman], MOTIONTYPE_MOVE);

		// ジャッジの情報の初期化
		g_aHuman[nCntHuman].judge.col      = JUDGE_WHITE;			// ピカピカの色
		g_aHuman[nCntHuman].judge.state    = JUDGESTATE_JUSTICE;	// 善悪
		g_aHuman[nCntHuman].judge.ticatica = CHICASTATE_BLACKOUT;	// チカチカ状態

		// 曲がり角関係の初期化
		g_aHuman[nCntHuman].curveInfo.actionState  = HUMANACT_WALK;					// 歩行状態
		g_aHuman[nCntHuman].curveInfo.nRandamRoute = 0;								// 進むルートの種類
		g_aHuman[nCntHuman].curveInfo.rotDest      = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目標の位置
		g_aHuman[nCntHuman].curveInfo.curveInfo.nCurveTime = 0;						// 曲がり角の数
		g_aHuman[nCntHuman].curveInfo.curveInfo.nNowCurve  = 0;						// 現在の曲がり角

		// アイコンの情報の初期化
		g_aHuman[nCntHuman].icon.nIconID = NONE_ICON;		// アイコンのインデックス
		g_aHuman[nCntHuman].icon.state   = ICONSTATE_NONE;	// アイコンの状態

		// 移動の種類
		g_aHuman[nCntHuman].typeMove = MOVETYPE_STOP;

		for (int nCntCur = 0; nCntCur < MAX_HUMAN_CURVE; nCntCur++)
		{
			g_aHuman[nCntHuman].curveInfo.curveInfo.curvePoint[nCntCur] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 曲がり角の位置
			g_aHuman[nCntHuman].curveInfo.curveInfo.curveAngle[nCntCur] = CURVE_RIGHT;						// 右に曲がる
			g_aHuman[nCntHuman].curveInfo.curveInfo.dashAngle[nCntCur]  = DASH_RIGHT;						// 右に向かって走っている
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
	int nCnt = 0;		// 引数設定用
	int nTackleCnt = 0;	// 引数設定用
	float fMove = 0.0f;	// 引数設定用
	POLICESTATE policeState = POLICESTATE_CHASE;	// 警察の状態(オブジェクトとの当たり判定に使うため無意味)

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

			if (g_aHuman[nCntHuman].judge.state == JUDGESTATE_EVIL)
			{ // 悪者だった場合

				// ジャッジの更新処理
				UpdateJudge(&g_aHuman[nCntHuman].judge);

				// アイコンの位置設定処理
				SetPositionIcon(g_aHuman[nCntHuman].icon.nIconID, g_aHuman[nCntHuman].pos);
			}

			//人間のリアクション処理
			ReactionHuman(&g_aHuman[nCntHuman]);

			// 風の当たり判定
			CollisionWind(&g_aHuman[nCntHuman]);

			switch (g_aHuman[nCntHuman].state)
			{
			case HUMANSTATE_WALK:		//歩き状態

				if (g_aHuman[nCntHuman].typeMove == MOVETYPE_MOVE)
				{ // 動く種類の場合

					//人間のカーブ処理
					CurveHuman(&g_aHuman[nCntHuman]);
				}

				break;					//抜け出す

			case HUMANSTATE_STOP:		//止まった状態

				if (g_aHuman[nCntHuman].typeMove == MOVETYPE_MOVE)
				{ // 動く種類の場合

					// 人間の停止処理
					StopHuman(&g_aHuman[nCntHuman]);
				}

				break;					//抜け出す

			case HUMANSTATE_FLY:		//吹き飛んだ状態

				// 飛ばす
				g_aHuman[nCntHuman].pos.x += g_aHuman[nCntHuman].move.x;
				g_aHuman[nCntHuman].pos.z += g_aHuman[nCntHuman].move.z;

				if (g_aHuman[nCntHuman].pos.y <= 0.0f)
				{ // 位置が0.0f以下になった場合

					// 使用していない
					g_aHuman[nCntHuman].bUse = false;
				}

				break;					//抜け出す
			}

			if (g_aHuman[nCntHuman].pos.y < 0.0f)
			{ // Y軸の位置が0.0fだった場合

				// 縦への移動量を0.0fにする
				g_aHuman[nCntHuman].move.y = 0.0f;

				// 位置を0.0fに戻す
				g_aHuman[nCntHuman].pos.y = 0.0f;
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
				&nCnt,							// 渋滞カウント
				BOOSTSTATE_NONE,				// ブースト状態
				&policeState,					// 警察の状態
				&nTackleCnt,					// タックルカウント
				&fMove							// タックル時の移動量
			);

			// プレイヤーの補正の更新処理
			RevHuman(&g_aHuman[nCntHuman].rot, &g_aHuman[nCntHuman].pos);

			// モーションの更新
			UpdateMotionHuman(&g_aHuman[nCntHuman]);
		}
	}
}

//======================================================================================================================
//	人間の描画処理
//======================================================================================================================
void DrawHuman(void)
{
	// 変数を宣言
	D3DXMATRIX   mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATRIX   mtxRotParts, mtxTransParts;	// 階層構造の計算用マトリックス
	D3DXMATRIX   mtxParent;						// 親のマトリックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ
	D3DXMATERIAL	  blueMat;					// 青ざめマテリアルポインタ
	D3DXMATERIAL	  evilMat;					// 悪い奴のマテリアルデータ

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

			for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
			{ // パーツの最大数分繰り返す

				// パーツのワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_aHuman[nCntHuman].aParts[nCntParts].mtxWorld);

				// 向きを反映
				D3DXMatrixRotationYawPitchRoll(&mtxRotParts, g_aHuman[nCntHuman].aParts[nCntParts].rot.y, g_aHuman[nCntHuman].aParts[nCntParts].rot.x, g_aHuman[nCntHuman].aParts[nCntParts].rot.z);
				D3DXMatrixMultiply(&g_aHuman[nCntHuman].aParts[nCntParts].mtxWorld, &g_aHuman[nCntHuman].aParts[nCntParts].mtxWorld, &mtxRotParts);

				// 位置を反映
				D3DXMatrixTranslation(&mtxTransParts, g_aHuman[nCntHuman].aParts[nCntParts].pos.x, g_aHuman[nCntHuman].aParts[nCntParts].pos.y, g_aHuman[nCntHuman].aParts[nCntParts].pos.z);
				D3DXMatrixMultiply(&g_aHuman[nCntHuman].aParts[nCntParts].mtxWorld, &g_aHuman[nCntHuman].aParts[nCntParts].mtxWorld, &mtxTransParts);

				// パーツの親マトリックスを設定
				if (g_aHuman[nCntHuman].aParts[nCntParts].nParentID == NONE_PARENT)
				{ // 現在のパーツに親が存在しない場合

					// プレイヤーのマトリックスを設定
					mtxParent = g_aHuman[nCntHuman].mtxWorld;
				}
				else
				{ // 現在のパーツに親が存在する場合

					// 親のマトリックスを設定
					mtxParent = g_aHuman[nCntHuman].aParts[g_aHuman[nCntHuman].aParts[nCntParts].nParentID].mtxWorld;
				}

				// パーツのワールドマトリックスと親マトリックスを掛け合わせる
				D3DXMatrixMultiply(&g_aHuman[nCntHuman].aParts[nCntParts].mtxWorld, &g_aHuman[nCntHuman].aParts[nCntParts].mtxWorld, &mtxParent);

				// パーツのワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aHuman[nCntHuman].aParts[nCntParts].mtxWorld);

				// マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_aHuman[nCntHuman].aParts[nCntParts].modelData.pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aHuman[nCntHuman].aParts[nCntParts].modelData.dwNumMat; nCntMat++)
				{ // マテリアルの数分繰り返す

					switch (g_aHuman[nCntHuman].state)
					{
					case HUMANSTATE_STOP:	// 停止状態

						// 構造体の要素をクリア
						ZeroMemory(&blueMat, sizeof(D3DXMATERIAL));

						// 拡散光・環境光・自己発光を赤にする
						blueMat.MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
						blueMat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
						blueMat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

						// マテリアルの設定
						pDevice->SetMaterial(&blueMat.MatD3D);	// 青

						// 処理を抜ける
						break;

					default:				// 上記以外

						// 構造体の要素をクリア
						ZeroMemory(&evilMat, sizeof(D3DXMATERIAL));

						// マテリアルのコピーに代入する
						evilMat = pMat[nCntMat];

						if (g_aHuman[nCntHuman].judge.state == JUDGESTATE_JUSTICE)
						{ // 良い奴の場合

							// マテリアルの設定
							pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
						}
						else
						{ // 悪い奴の場合

							// 自己発光を代入する
							evilMat.MatD3D.Diffuse = g_aHuman[nCntHuman].judge.col;

							// マテリアルの設定
							pDevice->SetMaterial(&evilMat.MatD3D);
						}

						// 処理を抜ける
						break;
					}

					// テクスチャの設定
					pDevice->SetTexture(0, g_aHuman[nCntHuman].aParts[nCntParts].modelData.pTexture[nCntMat]);

					// モデルの描画
					g_aHuman[nCntHuman].aParts[nCntParts].modelData.pMesh->DrawSubset(nCntMat);
				}
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
	int nHumanType;

	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aHuman[nCntHuman].bUse == false)
		{ // オブジェクトが使用されていない場合

			// 引数の位置を代入
			g_aHuman[nCntHuman].pos      = pos;		// 現在の位置
			g_aHuman[nCntHuman].posOld   = pos;		// 前回の位置

			// 情報を初期化
			g_aHuman[nCntHuman].move     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
			g_aHuman[nCntHuman].bJump    = false;							// ジャンプしているかどうか
			g_aHuman[nCntHuman].bMove    = false;							// 移動していない
			g_aHuman[nCntHuman].state    = HUMANSTATE_WALK;					// 歩き状態

			// 移動量の最大値を設定
			g_aHuman[nCntHuman].fMaxMove = (float)(rand() % HUMAN_RANDAM_MOVE + HUMAN_MOVE_LEAST);

			// 使用している状態にする
			g_aHuman[nCntHuman].bUse = true;

			// 人間の種類をランダムで算出する
			nHumanType = rand() % HUMANTYPE_MAX;

			// 種類を設定する
			g_aHuman[nCntHuman].type = (HUMANTYPE)nHumanType;

			// 影のインデックスを設定
			g_aHuman[nCntHuman].nShadowID = SetCircleShadow
			( // 引数
				0.5f,							// α値
				HUMAN_WIDTH,					// 半径
				&g_aHuman[nCntHuman].nShadowID,	// 影の親の影インデックス
				&g_aHuman[nCntHuman].bUse		// 影の親の使用状況
			);

			// 影の位置設定
			SetPositionShadow(g_aHuman[nCntHuman].nShadowID, g_aHuman[nCntHuman].pos, g_aHuman[nCntHuman].rot, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

			// 移動の種類を設定する
			g_aHuman[nCntHuman].typeMove = (MOVETYPE)(rand() % MOVETYPE_MAX);

			// 曲がり角情報の設置
			g_aHuman[nCntHuman].curveInfo.actionState  = HUMANACT_WALK;												// 状態
			g_aHuman[nCntHuman].curveInfo.nRandamRoute = rand() % MAX_HUMAN_ROUTE;									// ルートの種類
			g_aHuman[nCntHuman].curveInfo.rotDest      = g_aHuman[nCntHuman].rot;									// 目標の向き
			g_aHuman[nCntHuman].rot.y                  = GetDefaultRot(g_aHuman[nCntHuman].curveInfo.nRandamRoute);	// 初期の向き
			g_aHuman[nCntHuman].curveInfo.curveInfo    = GetHumanRoute(g_aHuman[nCntHuman].curveInfo.nRandamRoute);	// ルート

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

			// ジャッジの情報の設定
			g_aHuman[nCntHuman].judge.col = JUDGE_WHITE;			// ピカピカの色

			if (g_aHuman[nCntHuman].type == HUMANTYPE_CIGARETTE ||
				g_aHuman[nCntHuman].type == HUMANTYPE_SMARTPHONE)
			{ // 悪い奴だった場合

				g_aHuman[nCntHuman].judge.state = JUDGESTATE_EVIL;			// 善悪
				g_aHuman[nCntHuman].judge.ticatica = CHICASTATE_BLACKOUT;	// チカチカ状態

				// アイコンの情報の初期化
				g_aHuman[nCntHuman].icon.nIconID = NONE_ICON;				// アイコンのインデックス
				g_aHuman[nCntHuman].icon.state = ICONSTATE_NONE;			// アイコンの状態
			}
			else
			{ // 良い奴だった場合

				g_aHuman[nCntHuman].judge.state = JUDGESTATE_JUSTICE;		// 善悪
				g_aHuman[nCntHuman].judge.ticatica = CHICASTATE_BLACKOUT;	// チカチカ状態
			}

			if (g_aHuman[nCntHuman].judge.state == JUDGESTATE_EVIL)
			{ // 悪い奴だった場合

				// アイコンの設定処理
				g_aHuman[nCntHuman].icon.nIconID = SetIcon
				( // 引数
					g_aHuman[nCntHuman].pos,
					ICONTYPE_EVIL,
					&g_aHuman[nCntHuman].icon.nIconID,
					&g_aHuman[nCntHuman].bUse,
					&g_aHuman[nCntHuman].icon.state
				);
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
	if      (rot->y > D3DX_PI) { rot->y -= D3DX_PI * 2; }
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
	switch (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve])
	{
	case DASH_RIGHT:	// 右に進んでいる場合

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

		break;			//抜け出す

	case DASH_LEFT:		// 左に進んでいる場合

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

		break;			// 抜け出す

	case DASH_FAR:		// 奥に進んでいる場合

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

		break;			// 抜け出す

	case DASH_NEAR:		// 手前に進んでいる場合

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

		break;			// 抜け出す
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
	float fLength;	// 長さの変数

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
				if (pHuman->pos.x - HUMAN_WIDTH <= pCar[nCntCar].pos.x + pCar[nCntCar].modelData.vtxMax.x 
					&& pHuman->pos.x + HUMAN_WIDTH >= pCar[nCntCar].pos.x + pCar[nCntCar].modelData.vtxMin.x)
				{ // 車のX幅の中にいた場合
					if (pHuman->posOld.z + HUMAN_DEPTH <= pCar[nCntCar].posOld.z + pCar[nCntCar].modelData.vtxMin.z
						&& pHuman->pos.z + HUMAN_DEPTH >= pCar[nCntCar].pos.z + pCar[nCntCar].modelData.vtxMin.z)
					{//前回の位置がブロックより手前かつ、現在の位置がブロックよりも奥かつだった場合(手前で止められる処理)

						// 移動量を0にする
						pCar[nCntCar].move.x = 0.0f;

					}							//手前で止められる処理
					else if (pHuman->posOld.z - HUMAN_DEPTH >= pCar[nCntCar].posOld.z + pCar[nCntCar].modelData.vtxMax.z
						&& pHuman->pos.z - HUMAN_DEPTH <= pCar[nCntCar].pos.z + pCar[nCntCar].modelData.vtxMax.z)
					{//前回の位置が塔の位置よりも奥かつ、現在の位置が塔の位置よりも手前だった場合(奥で止められる処理)
					
						// 移動量を0にする
						pCar[nCntCar].move.x = 0.0f;

					}							//奥で止められる処理
				}

				if (pHuman->pos.z - HUMAN_DEPTH <= pCar[nCntCar].pos.z + pCar[nCntCar].modelData.vtxMax.z
					&& pHuman->pos.z + HUMAN_DEPTH >= pCar[nCntCar].pos.z + pCar[nCntCar].modelData.vtxMin.z)
				{//塔のZ幅の中にいた場合
					if (pHuman->posOld.x + HUMAN_WIDTH <= pCar[nCntCar].posOld.x + pCar[nCntCar].modelData.vtxMin.x
						&& pHuman->pos.x + HUMAN_WIDTH >= pCar[nCntCar].pos.x + pCar[nCntCar].modelData.vtxMin.x)
					{//前回の位置がブロックの左端より左かつ、現在の位置がブロックの左側より右だった場合(左の処理)
						
						//移動量を0にする
						pCar[nCntCar].move.x = 0.0f;

					}							//左端の処理
					else if (pHuman->posOld.x - HUMAN_WIDTH >= pCar[nCntCar].posOld.x + pCar[nCntCar].modelData.vtxMax.x
						&& pHuman->pos.x - HUMAN_WIDTH <= pCar[nCntCar].pos.x + pCar[nCntCar].modelData.vtxMax.x)
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
				if (pHuman->pos.x - HUMAN_WIDTH <= pPolice[nCntPolice].pos.x + pPolice[nCntPolice].modelData.vtxMax.x 
					&& pHuman->pos.x + HUMAN_WIDTH >= pPolice[nCntPolice].pos.x + pPolice[nCntPolice].modelData.vtxMin.x)
				{ // 車のX幅の中にいた場合
					if (pHuman->posOld.z + HUMAN_DEPTH <= pPolice[nCntPolice].posOld.z + pPolice[nCntPolice].modelData.vtxMin.z
						&& pHuman->pos.z + HUMAN_DEPTH >= pPolice[nCntPolice].pos.z + pPolice[nCntPolice].modelData.vtxMin.z)
					{//前回の位置がブロックより手前かつ、現在の位置がブロックよりも奥かつだった場合(手前で止められる処理)

						// 移動量を0.0fにする
						pPolice[nCntPolice].move.x = 0.0f;

					}							//手前で止められる処理
					else if (pHuman->posOld.z - HUMAN_DEPTH >= pPolice[nCntPolice].posOld.z + pPolice[nCntPolice].modelData.vtxMax.z
						&& pHuman->pos.z - HUMAN_DEPTH <= pPolice[nCntPolice].pos.z + pPolice[nCntPolice].modelData.vtxMax.z)
					{//前回の位置が塔の位置よりも奥かつ、現在の位置が塔の位置よりも手前だった場合(奥で止められる処理)

						// 移動量を0.0fにする
						pPolice[nCntPolice].move.x = 0.0f;

					}							//奥で止められる処理
				}

				if (pHuman->pos.z - HUMAN_DEPTH <= pPolice[nCntPolice].pos.z + pPolice[nCntPolice].modelData.vtxMax.z
					&& pHuman->pos.z + HUMAN_DEPTH >= pPolice[nCntPolice].pos.z + pPolice[nCntPolice].modelData.vtxMin.z)
				{//塔のZ幅の中にいた場合
					if (pHuman->posOld.x + HUMAN_WIDTH <= pPolice[nCntPolice].posOld.x + pPolice[nCntPolice].modelData.vtxMin.x
						&& pHuman->pos.x + HUMAN_WIDTH >= pPolice[nCntPolice].pos.x + pPolice[nCntPolice].modelData.vtxMin.x)
					{//前回の位置がブロックの左端より左かつ、現在の位置がブロックの左側より右だった場合(左の処理)
						
						// 移動量を0.0fにする
						pPolice[nCntPolice].move.x = 0.0f;

					}							//左端の処理
					else if (pHuman->posOld.x - HUMAN_WIDTH >= pPolice[nCntPolice].posOld.x + pPolice[nCntPolice].modelData.vtxMax.x
						&& pHuman->pos.x - HUMAN_WIDTH <= pPolice[nCntPolice].pos.x + pPolice[nCntPolice].modelData.vtxMax.x)
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
	switch (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve])
	{
	case CURVE_LEFT:	// 左に曲がる場合

		// 移動量を減速
		pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

		// 向きを更新
		pHuman->rot.y -= 0.05f * (pHuman->move.x * 0.1f);

		switch (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve])
		{
		case DASH_RIGHT:	// 右に進んでいる場合

			// 位置を補正する
			pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;

			// 目標の向きを設定する
			pHuman->curveInfo.rotDest.y = 0.0f;

			break;			// 抜け出す

		case DASH_LEFT:		// 左に進んでいる場合

			// 位置を補正する
			pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;

			// 目標の向きを設定する
			pHuman->curveInfo.rotDest.y = -D3DX_PI;

			break;			// 抜け出す

		case DASH_FAR:		// 奥に進んでいる場合

			// 位置を補正する
			pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;

			// 目標の向きを設定する
			pHuman->curveInfo.rotDest.y = -D3DX_PI * 0.5f;

			break;			// 抜け出す

		case DASH_NEAR:		// 手前に進んでいる場合

			// 位置を補正する
			pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;

			// 目標の向きを設定する
			pHuman->curveInfo.rotDest.y = D3DX_PI * 0.5f;

			break;			// 抜け出す
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

		break;			// 抜け出す

	default:

		// 移動量を減速
		pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

		// 向きを更新
		pHuman->rot.y += 0.05f * (pHuman->move.x * 0.1f);

		switch (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve])
		{
		case DASH_RIGHT:	// 右に進んでいる場合

			//位置を補正する
			pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;

			// 目標の向きを設定する
			pHuman->curveInfo.rotDest.y = D3DX_PI;

			break;			// 抜け出す

		case DASH_LEFT:		// 左に進んでいる場合

			// 位置を補正する
			pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;

			// 目標の向きを設定する
			pHuman->curveInfo.rotDest.y = 0.0f;

			break;			// 抜け出す

		case DASH_FAR:		// 奥に進んでいる場合

			// 位置を補正する
			pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;

			// 目標の向きを設定する
			pHuman->curveInfo.rotDest.y = D3DX_PI * 0.5f;

			break;			// 抜け出す

		case DASH_NEAR:		// 手前に進んでいる場合

			// 位置を補正する
			pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;

			// 目標の向きを設定する
			pHuman->curveInfo.rotDest.y = -D3DX_PI * 0.5f;

			break;			// 抜け出す
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

		break;			// 抜け出す
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

	for (int nCnt = 0; nCnt < MAX_HUMAN; nCnt++, pPassHuman++)
	{
		if (pPassHuman->bUse == true)
		{ // 人間が使用されていた場合
			if (pPassHuman != pHuman)
			{ // 比較する人間が自分自身じゃない場合

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
			else
			{ // 上記以外

				// 人間の歩く処理
				WalkHuman(pHuman);
			}
		}
	}
}

//======================================================================================================================
//	モーションの更新処理
//======================================================================================================================
void UpdateMotionHuman(Human *pHuman)
{
	// 変数を宣言
	D3DXVECTOR3 diffPos;		// 次のポーズまでの差分 (位置)
	D3DXVECTOR3 diffRot;		// 次のポーズまでの差分 (向き)
	D3DXVECTOR3 currentPos;		// 現在のフレームの位置
	D3DXVECTOR3 currentRot;		// 現在のフレームの向き

	MOTIONTYPE  type  = pHuman->motion.type;	// モーションの種類
	int         nPose = pHuman->motion.nPose;	// モーションのポーズ番号

	// パーツの位置を更新
	for (int nCntKey = 0; nCntKey < MAX_PARTS; nCntKey++)
	{ // プレイヤーのパーツの最大数分繰り返す

		// 位置の差分を求める
		diffPos = pHuman->motion.aMotion[type].aKey[(nPose + 1) % pHuman->motion.aMotion[type].nNumKey].aPos[nCntKey] - pHuman->motion.aMotion[type].aKey[nPose].aPos[nCntKey];

		// 向きの差分を求める
		diffRot = pHuman->motion.aMotion[type].aKey[(nPose + 1) % pHuman->motion.aMotion[type].nNumKey].aRot[nCntKey] - pHuman->motion.aMotion[type].aKey[nPose].aRot[nCntKey];

		// 差分向きの正規化
		RotNormalize(&diffRot.x);
		RotNormalize(&diffRot.y);
		RotNormalize(&diffRot.z);

		// 現在のフレームの位置を設定
		currentPos = pHuman->motion.aMotion[type].aKey[nPose].aPos[nCntKey] + diffPos * ((float)pHuman->motion.nCounter / (float)pHuman->motion.aMotion[type].aKey[nPose].nFrame);

		// 現在のフレームの向きを設定
		currentRot = pHuman->motion.aMotion[type].aKey[nPose].aRot[nCntKey] + diffRot * ((float)pHuman->motion.nCounter / (float)pHuman->motion.aMotion[type].aKey[nPose].nFrame);

		// 差分向きの正規化
		RotNormalize(&currentRot.x);
		RotNormalize(&currentRot.y);
		RotNormalize(&currentRot.z);

		// 現在のパーツの位置を更新
		pHuman->aParts[nCntKey].pos = pHuman->aParts[nCntKey].initPos + currentPos;

		// 現在のパーツの向きを更新
		pHuman->aParts[nCntKey].rot = pHuman->aParts[nCntKey].initRot + currentRot;

		// 向きの正規化
		RotNormalize(&pHuman->aParts[nCntKey].rot.x);
		RotNormalize(&pHuman->aParts[nCntKey].rot.y);
		RotNormalize(&pHuman->aParts[nCntKey].rot.z);
	}

	// モーションの遷移の更新
	if (pHuman->motion.nCounter >= pHuman->motion.aMotion[type].aKey[nPose].nFrame)
	{ // 現在のモーションカウンターが現在のポーズの再生フレーム数を超えている場合

		// 次のポーズに移行
		if (pHuman->motion.aMotion[type].bLoop == true)
		{ // モーションがループする設定の場合

			// モーションカウンターを初期化
			pHuman->motion.nCounter = 0;

			// ポーズカウントを加算 (総数に達した場合 0に戻す)
			pHuman->motion.nPose = (pHuman->motion.nPose + 1) % pHuman->motion.aMotion[type].nNumKey;
		}
		else
		{ // モーションがループしない設定の場合

#if 0
			if (pHuman->motion.nPose < pHuman->motion.aMotion[type].nNumKey - SUB_MOTION_STOP)
			{ // 現在のポーズが最終のポーズではない場合

				// モーションカウンターを初期化
				pHuman->motion.nCounter = 0;

				// ポーズカウントを加算
				pHuman->motion.nPose++;
			}
			else
			{ // 現在のポーズが最終のポーズの場合

				switch (type)
				{ // モーションの種類ごとの処理
				case MOTIONTYPE_ATTACK:		// 攻撃状態

					// 待機モーションに移行
					SetMotionHuman(pHuman, MOTIONTYPE_NONE);

					// 処理を抜ける
					break;

				case MOTIONTYPE_JUMP:		// ジャンプ状態

					// 無し

					// 処理を抜ける
					break;

				case MOTIONTYPE_LANDING:	// 着地状態

					// 無し

					// 処理を抜ける
					break;
				}
			}
#endif

		}
	}
	else
	{ // 現在のモーションカウンターが現在のポーズの再生フレーム数を超えていない場合

		// モーションカウンターを加算
		pHuman->motion.nCounter++;
	}
}

//======================================================================================================================
//	モーションの設定処理
//======================================================================================================================
void SetMotionHuman(Human *pHuman, MOTIONTYPE type)
{
	// 引数を代入
	pHuman->motion.type = type;		// モーションの種類

	// モーション情報を初期化
	pHuman->motion.nPose    = 0;	// モーションのポーズ番号
	pHuman->motion.nCounter = 0;	// モーションのカウンター
}

//======================================================================================================================
//	人間のセットアップ処理
//======================================================================================================================
void TxtSetHuman(HumanParts *setParts, KeyInfo *setMotion)
{
	// 変数を宣言
	int nID        = 0;			// インデックスの代入用
	int nNowMotion = 0;			// 現在のモーション番号
	int nNowPose   = 0;			// 現在のポーズ番号
	int nNowKey    = 0;			// 現在のキー番号
	int nLoop      = 0;			// ループの ON / OFF の変換用
	int nEnd       = 0;			// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;				// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(HUMAN_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			//----------------------------------------------------------------------------------------------------------
			//	キャラクターの設定
			//----------------------------------------------------------------------------------------------------------
			if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{ // 読み込んだ文字列が CHARACTERSET の場合

				do
				{ // 読み込んだ文字列が END_CHARACTERSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "PARTSSET") == 0)
					{ // 読み込んだ文字列が PARTSSET の場合

						do
						{ // 読み込んだ文字列が END_PARTSSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "INDEX") == 0)
							{ // 読み込んだ文字列が INDEX の場合
								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nID);			// モデル (パーツ) のインデックスを読み込む
							}
							else if (strcmp(&aString[0], "PARENT") == 0)
							{ // 読み込んだ文字列が PARENT の場合
								fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
								fscanf(pFile, "%d", &setParts[nID].nParentID);	// モデル (パーツ) の親のインデックスを読み込む
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // 読み込んだ文字列が POS の場合
								fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
								fscanf(pFile, "%f", &setParts[nID].initPos.x);	// X座標を読み込む
								fscanf(pFile, "%f", &setParts[nID].initPos.y);	// Y座標を読み込む
								fscanf(pFile, "%f", &setParts[nID].initPos.z);	// Z座標を読み込む

								// 現在の位置に読み込んだ値を設定
								setParts[nID].pos = setParts[nID].initPos;
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // 読み込んだ文字列が ROT の場合
								fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
								fscanf(pFile, "%f", &setParts[nID].initRot.x);	// X向きを読み込む
								fscanf(pFile, "%f", &setParts[nID].initRot.y);	// Y向きを読み込む
								fscanf(pFile, "%f", &setParts[nID].initRot.z);	// Z向きを読み込む

								// 現在の向きに読み込んだ値を設定
								setParts[nID].rot = setParts[nID].initRot;
							}

						} while (strcmp(&aString[0], "END_PARTSSET") != 0);		// 読み込んだ文字列が END_PARTSSET ではない場合ループ
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);			// 読み込んだ文字列が END_CHARACTERSET ではない場合ループ
			}

			//----------------------------------------------------------------------------------------------------------
			//	モーションの設定
			//----------------------------------------------------------------------------------------------------------
			else if (strcmp(&aString[0], "MOTIONSET") == 0)
			{ // 読み込んだ文字列が MOTIONSET の場合

				// 現在のポーズ番号を初期化
				nNowPose = 0;

				do
				{ // 読み込んだ文字列が END_MOTIONSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "LOOP") == 0)
					{ // 読み込んだ文字列が LOOP の場合
						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nLoop);		// ループの ON / OFF を読み込む

						// 読み込んだ値が 0なら true、0以外なら falseを代入
						setMotion[nNowMotion].bLoop = (nLoop == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "NUM_KEY") == 0)
					{ // 読み込んだ文字列が NUM_KEY の場合
						fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
						fscanf(pFile, "%d", &setMotion[nNowMotion].nNumKey);	// キーの総数を読み込む
					}
					else if (strcmp(&aString[0], "KEYSET") == 0)
					{ // 読み込んだ文字列が KEYSET の場合

						// 現在のキー番号を初期化
						nNowKey = 0;

						do
						{ // 読み込んだ文字列が END_KEYSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "FRAME") == 0)
							{ // 読み込んだ文字列が FRAME の場合
								fscanf(pFile, "%s", &aString[0]);									// = を読み込む (不要)
								fscanf(pFile, "%d", &setMotion[nNowMotion].aKey[nNowPose].nFrame);	// キーが切り替わるまでのフレーム数を読み込む
							}
							else if (strcmp(&aString[0], "KEY") == 0)
							{ // 読み込んだ文字列が KEY の場合

								do
								{ // 読み込んだ文字列が END_KEY ではない場合ループ

									// ファイルから文字列を読み込む
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "POS") == 0)
									{ // 読み込んだ文字列が POS の場合
										fscanf(pFile, "%s", &aString[0]);											// = を読み込む (不要)
										fscanf(pFile, "%f", &setMotion[nNowMotion].aKey[nNowPose].aPos[nNowKey].x);	// 位置 (x) を読み込む
										fscanf(pFile, "%f", &setMotion[nNowMotion].aKey[nNowPose].aPos[nNowKey].y);	// 位置 (y) を読み込む
										fscanf(pFile, "%f", &setMotion[nNowMotion].aKey[nNowPose].aPos[nNowKey].z);	// 位置 (z) を読み込む
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // 読み込んだ文字列が ROT の場合
										fscanf(pFile, "%s", &aString[0]);											// = を読み込む (不要)
										fscanf(pFile, "%f", &setMotion[nNowMotion].aKey[nNowPose].aRot[nNowKey].x);	// 向き (x) を読み込む
										fscanf(pFile, "%f", &setMotion[nNowMotion].aKey[nNowPose].aRot[nNowKey].y);	// 向き (y) を読み込む
										fscanf(pFile, "%f", &setMotion[nNowMotion].aKey[nNowPose].aRot[nNowKey].z);	// 向き (z) を読み込む
									}

								} while (strcmp(&aString[0], "END_KEY") != 0);	// 読み込んだ文字列が END_KEY ではない場合ループ

								// 現在のキー番号を加算
								nNowKey++;
							}
						} while (strcmp(&aString[0], "END_KEYSET") != 0);		// 読み込んだ文字列が END_KEYSET ではない場合ループ

						// 現在のポーズ番号を加算
						nNowPose++;
					}
				} while (strcmp(&aString[0], "END_MOTIONSET") != 0);			// 読み込んだ文字列が END_MOTIONSET ではない場合ループ

				// 現在のモーション番号を加算
				nNowMotion++;
			}
		} while (nEnd != EOF);													// 読み込んだ文字列が EOF ではない場合ループ
		
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "人間のセットアップファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}

#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================
#endif