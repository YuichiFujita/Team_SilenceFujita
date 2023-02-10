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
#include "calculation.h"
 
#include "Car.h"
#include "Police.h"
#include "shadow.h"
#include "sound.h"
#include "player.h"
#include "object.h"
#include "meshfield.h"

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
#define REV_POLI_MOVE_ROT		(0.085f)	// 移動量による向き変更量の補正係数
#define SUB_POLI_MOVE_VALUE		(15.0f)		// 向き変更時の減速が行われる移動量
#define SUB_POLI_MOVE			(0.05f)		// 向き変更時の減速量
#define MAX_POLI_FORWARD		(30.0f)		// 前進時の最高速度
#define MAX_POLI_FORWARD_PATROL (15.0f)		// パトロール中の前進時の最高速度
#define MAX_POLI_BACKWARD		(8.0f)		// 後退時の最高速度
#define REV_POLI_MOVE_SUB		(0.04f)		// 移動量の減速係数

#define POLICAR_TRAFFIC_CNT			(240)		// 渋滞が起きたときに改善する用のカウント
#define POLICAR_TRAFFIC_IMPROVE_CNT	(540)		// 渋滞状態の解除のカウント

//**********************************************************************************************************************
//	タックル関係のマクロ定義
//**********************************************************************************************************************
#define POLICAR_TACKLE_CNT		(60)		// タックル状態に移行するまでのカウント
#define POLICAR_TACKLE_ADD		(3.35f)		// 増していく移動量
#define MAX_POLICAR_TACKLE_MOVE	(50.0f)		// 追加移動量の最大数
#define FINISH_POLICAR_TACKLE	(90)		// タックル終了するまでの時間
#define STOP_POLICAR_CNT		(40)		// 止まっている間のカウント数
#define POLICAR_ATTEN_STOP		(0.1f)		// 追加移動量の減衰係数

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void PosPolice(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove);	// プレイヤーの位置の更新処理
void RevPolice(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos,D3DXVECTOR3 *move);				// プレイヤーの補正の更新処理
void PatrolPoliceAct(Police *pPolice);					// 警察のパトロール行動処理
void PatrolCarSearch(Police *pPolice);					// 警察車両の探知処理
void ChasePoliceAct(Police *pPolice);					// 警察の追跡処理
void PatrolBackAct(Police *pPolice);					// パトロールに戻る処理
void CurvePolice(Police *pPolice);						// 警察のカーブ処理
void DashPoliceAction(Police *pPolice);					// 警察の走行処理
void SetPolicePosRot(Police *pPolice);					// 警察の位置と向きの設定処理
void PolicePosRotCorrect(Police *pPolice);				// 警察の位置の補正処理
void PoliceTackle(Police *pPolice);						// 警察のタックル処理

void PoliceTrafficImprove(Police *pPolice);				// 警察の渋滞改善処理

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
Police g_aPolice[MAX_POLICE];	// オブジェクトの情報

//======================================================================================================================
//	警察の初期化処理
//======================================================================================================================
void InitPolice(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// オブジェクトの情報の初期化
	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // オブジェクトの最大表示数分繰り返す

		// 基本情報を初期化
		g_aPolice[nCntPolice].pos		  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aPolice[nCntPolice].posOld	  = g_aPolice[nCntPolice].pos;		// 前回の位置
		g_aPolice[nCntPolice].posCopy	  = g_aPolice[nCntPolice].pos;		// 最初の位置
		g_aPolice[nCntPolice].move		  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
		g_aPolice[nCntPolice].rot		  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
		g_aPolice[nCntPolice].rotDest	  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目標の向き
		g_aPolice[nCntPolice].state		  = POLICESTATE_PATROL;				// 警察の状態
		g_aPolice[nCntPolice].bombState   = BOMBSTATE_NONE;					// ボムの状態
		g_aPolice[nCntPolice].nLife		  = 0;								// 体力
		g_aPolice[nCntPolice].nShadowID   = NONE_SHADOW;					// 影のインデックス
		g_aPolice[nCntPolice].bJump		  = false;							// ジャンプしていない
		g_aPolice[nCntPolice].nTrafficCnt = 0;								// 渋滞カウント
		g_aPolice[nCntPolice].bUse		  = false;							// 使用状況

		// 曲がり角関係を初期化
		g_aPolice[nCntPolice].policeCurve.curveInfo.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 曲がり角の位置
		g_aPolice[nCntPolice].policeCurve.curveInfo.nCurveNumber = -1;						// 番号
		g_aPolice[nCntPolice].policeCurve.curveInfo.curveAngle = CURVE_RIGHT;				// 右に曲がる
		g_aPolice[nCntPolice].policeCurve.curveInfo.dashAngle = DASH_RIGHT;					// 右に進む
		g_aPolice[nCntPolice].policeCurve.curveInfo.bDeadEnd = false;						// 行き止まりかどうか
		g_aPolice[nCntPolice].policeCurve.actionState = CARACT_DASH;						// 状態
		g_aPolice[nCntPolice].policeCurve.nSKipCnt = 0;										// スキップする回数
		g_aPolice[nCntPolice].policeCurve.rotDest = g_aPolice[nCntPolice].rot;				// 目標の向き

		g_aPolice[nCntPolice].policeCurveCopy = g_aPolice[nCntPolice].policeCurve;			// 曲がり角の情報を代入

		// モデル情報の初期化
		g_aPolice[nCntPolice].modelData.dwNumMat = 0;						// マテリアルの数
		g_aPolice[nCntPolice].modelData.pTexture = NULL;					// テクスチャへのポインタ
		g_aPolice[nCntPolice].modelData.pMesh    = NULL;					// メッシュ (頂点情報) へのポインタ
		g_aPolice[nCntPolice].modelData.pBuffMat = NULL;					// マテリアルへのポインタ
		g_aPolice[nCntPolice].modelData.dwNumMat = 0;						// マテリアルの数
		g_aPolice[nCntPolice].modelData.vtxMin   = INIT_VTX_MIN;			// 最小の頂点座標
		g_aPolice[nCntPolice].modelData.vtxMax   = INIT_VTX_MAX;			// 最大の頂点座標
		g_aPolice[nCntPolice].modelData.size     = INIT_SIZE;				// 大きさ
		g_aPolice[nCntPolice].modelData.fRadius  = 0.0f;					// 半径

		// タックル関係の情報の初期化
		g_aPolice[nCntPolice].tackle.nTackleCnt = 0;						// タックルのカウント
		g_aPolice[nCntPolice].tackle.Tacklemove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// タックル時の追加移動量
	}
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
	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // オブジェクトの最大表示数分繰り返す
		if (g_aPolice[nCntPolice].bUse == true)
		{ // オブジェクトが使用されている場合
			if (g_aPolice[nCntPolice].bombState != BOMBSTATE_BAR_IN)
			{ // バリア内状態ではない場合
				// 前回位置の更新
				g_aPolice[nCntPolice].posOld = g_aPolice[nCntPolice].pos;

				// プレイヤーの着地の更新処理
				LandObject(&g_aPolice[nCntPolice].pos, &g_aPolice[nCntPolice].move, &g_aPolice[nCntPolice].bJump);

				//----------------------------------------------------
				//	影の更新
				//----------------------------------------------------
				// 影の位置設定
				SetPositionShadow
				( // 引数
					g_aPolice[nCntPolice].nShadowID,	// 影のインデックス
					g_aPolice[nCntPolice].pos,			// 位置
					g_aPolice[nCntPolice].rot,			// 向き
					NONE_SCALE							// 拡大率
				);

				// プレイヤーの位置の更新
				PosPolice(&g_aPolice[nCntPolice].move, &g_aPolice[nCntPolice].pos, &g_aPolice[nCntPolice].rot, g_aPolice[nCntPolice].bMove);

				switch (g_aPolice[nCntPolice].state)
				{//状態で判断する
				case POLICESTATE_PATROL:		// パトロール状態

					// 警察のパトロール行動処理
					PatrolPoliceAct(&g_aPolice[nCntPolice]);

					// 車の停止処理
					CollisionStopCar
					( // 引数
						g_aPolice[nCntPolice].pos,		//位置
						g_aPolice[nCntPolice].rot,		//向き
						&g_aPolice[nCntPolice].move,	//移動量
						g_aPolice[nCntPolice].modelData.fRadius,	//半径
						COLLOBJECTTYPE_POLICE,			//対象のサイズ
						&g_aPolice[nCntPolice].nTrafficCnt
					);

					break;						// 抜け出す

				case POLICESTATE_CHASE:			// 追跡処理

					// 警察の追跡処理
					ChasePoliceAct(&g_aPolice[nCntPolice]);

					break;						// 抜け出す

				case POLICESTATE_PATBACK:		// パトロールへ戻る処理			

					break;						// 抜け出す

				case POLICESTATE_POSBACK:		// 最初の座標に戻る処理

					// 最初の移動量を元に戻す
					g_aPolice[nCntPolice].move.x = 0.0f;

					break;						// 抜け出す

				case POLICESTATE_TACKLE:		// タックル状態

					// 警察のタックル処理
					PoliceTackle(&g_aPolice[nCntPolice]);

					break;						// 抜け出す

				case POLICESTATE_TRAFFIC:		// 渋滞状態

					// 警察車両の渋滞改善処理
					PoliceTrafficImprove(&g_aPolice[nCntPolice]);

					break;						// 抜け出す
				}

			}

			if (GetBarrierState(&g_aPolice[nCntPolice]) != BARRIERSTATE_SET)
			{ // バリアセット状態じゃなかった場合
				if (g_aPolice[nCntPolice].state != POLICESTATE_TRAFFIC)
				{ // 渋滞状態じゃない場合
					//----------------------------------------------------
					//	当たり判定
					//----------------------------------------------------
					// オブジェクトとの当たり判定
					CollisionObject
					( // 引数
						&g_aPolice[nCntPolice].pos,		// 現在の位置
						&g_aPolice[nCntPolice].posOld,	// 前回の位置
						&g_aPolice[nCntPolice].move,	// 移動量
						POLICAR_WIDTH,					// 横幅
						POLICAR_DEPTH,					// 奥行
						&g_aPolice[nCntPolice].nTrafficCnt	// 渋滞カウント
					);
				}

				if (g_aPolice[nCntPolice].state != POLICESTATE_PATBACK && g_aPolice[nCntPolice].state != POLICESTATE_POSBACK)
				{ // パトロールから戻る処理じゃないかつ、初期値に戻る時以外の場合

					// 車同士の当たり判定
					CollisionCarBody
					( // 引数
						&g_aPolice[nCntPolice].pos,
						&g_aPolice[nCntPolice].posOld,
						g_aPolice[nCntPolice].rot,
						&g_aPolice[nCntPolice].move,
						POLICAR_WIDTH,
						POLICAR_DEPTH,
						COLLOBJECTTYPE_POLICE,
						&g_aPolice[nCntPolice].nTrafficCnt,
						(int)(g_aPolice[nCntPolice].state)
					);
				}
			}

			if (g_aPolice[nCntPolice].bombState != BOMBSTATE_BAR_IN)
			{ // バリア内状態ではない場合

				if (g_aPolice[nCntPolice].pos.y < 0.0f)
				{//Y軸の位置が0.0fだった場合
					//縦への移動量を0.0fにする
					g_aPolice[nCntPolice].move.y = 0.0f;

					//位置を0.0fに戻す
					g_aPolice[nCntPolice].pos.y = 0.0f;
				}

				// プレイヤーの補正の更新処理
				RevPolice(&g_aPolice[nCntPolice].rot, &g_aPolice[nCntPolice].pos, &g_aPolice[nCntPolice].move);
			}
		}
	}
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
	Player           *pPlayer = GetPlayer();	// プレイヤーの情報
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
			pMat = &g_aPolice[nCntPolice].MatCopy[0];

			for (int nCntMat = 0; nCntMat < (int)g_aPolice[nCntPolice].modelData.dwNumMat; nCntMat++)
			{ // マテリアルの数分繰り返す

				if (pPlayer->atkState == ATTACKSTATE_BOMB)
				{ // 攻撃状態がボム攻撃状態の場合

					switch (g_aPolice[nCntPolice].bombState)
					{ // ボムの状態
					case BOMBSTATE_RANGE:	// 範囲内状態

						// 範囲内時のマテリアルの色を設定
						pMat[nCntMat].MatD3D.Diffuse = BOMB_RANGE_COL;

						// 処理を抜ける
						break;

					case BOMBSTATE_AIM:		// 狙い状態

						// 狙い時のマテリアルの色を設定
						pMat[nCntMat].MatD3D.Diffuse = BOMB_AIM_COL;

						// 処理を抜ける
						break;

					default:				// 上記以外

						// 範囲外時のマテリアルの色を設定
						pMat[nCntMat].MatD3D.Diffuse = BOMB_NONE_COL;

						// 処理を抜ける
						break;
					}
				}
				else
				{ // 攻撃状態がそれ以外の状態の場合

					switch (g_aPolice[nCntPolice].state)
					{ // 状態ごとの処理
					case POLICESTATE_PATBACK:	// パトロールに戻っている状態

						// 透明度を下げる
						pMat[nCntMat].MatD3D.Diffuse.a  -= 0.005f;
						pMat[nCntMat].MatD3D.Ambient.a  -= 0.005f;
						pMat[nCntMat].MatD3D.Emissive.a -= 0.005f;

						if (pMat->MatD3D.Emissive.a <= 0.0f)
						{ // 透明度が0.0f以下になった場合

							// 透明度を下げる
							pMat[nCntMat].MatD3D.Diffuse.a  = 0.0f;
							pMat[nCntMat].MatD3D.Ambient.a  = 0.0f;
							pMat[nCntMat].MatD3D.Emissive.a = 0.0f;

							// パトロールに戻る処理
							PatrolBackAct(&g_aPolice[nCntPolice]);

							// 最初の位置に戻す処理
							g_aPolice[nCntPolice].state = POLICESTATE_POSBACK;
						}

						// 処理を抜ける
						break;

					case POLICESTATE_POSBACK:	// 最初の位置に戻る状態

						// 透明度を下げる
						pMat[nCntMat].MatD3D.Diffuse.a  += 0.005f;
						pMat[nCntMat].MatD3D.Ambient.a  += 0.005f;
						pMat[nCntMat].MatD3D.Emissive.a += 0.005f;

						if (pMat->MatD3D.Emissive.a >= 1.0f)
						{ // 透明度が0.0f以下になった場合

							// 透明度を下げる
							pMat[nCntMat].MatD3D.Diffuse.a  = 1.0f;
							pMat[nCntMat].MatD3D.Ambient.a  = 1.0f;
							pMat[nCntMat].MatD3D.Emissive.a = 1.0f;

							// 最初の位置に戻す処理
							g_aPolice[nCntPolice].state = POLICESTATE_PATROL;
						}

						// 処理を抜ける
						break;

					default:					// 上記以外

						// 元々のマテリアルを代入する
						pMat = (D3DXMATERIAL*)g_aPolice[nCntPolice].modelData.pBuffMat->GetBufferPointer();

						// 処理を抜ける
						break;
					}
				}

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
void SetPolice(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aPolice[nCntPolice].bUse == false)
		{ // オブジェクトが使用されていない場合
			// 引数を代入
			g_aPolice[nCntPolice].pos		  = pos;							// 現在の位置
			g_aPolice[nCntPolice].posCopy     = g_aPolice[nCntPolice].pos;		// 最初の位置
			g_aPolice[nCntPolice].posOld	  = g_aPolice[nCntPolice].pos;		// 前回の位置
			g_aPolice[nCntPolice].rotDest	  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目標の向き
			g_aPolice[nCntPolice].move		  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
			g_aPolice[nCntPolice].state		  = POLICESTATE_PATROL;				// パトロール状態にする
			g_aPolice[nCntPolice].bombState   = BOMBSTATE_NONE;					// 何もしていない状態にする
			g_aPolice[nCntPolice].nLife		  = POLI_LIFE;						// 体力
			g_aPolice[nCntPolice].bJump		  = false;							// ジャンプしていない
			g_aPolice[nCntPolice].nTrafficCnt = 0;								// 渋滞カウント
			g_aPolice[nCntPolice].bMove		  = false;							// 移動していない

			// 使用している状態にする
			g_aPolice[nCntPolice].bUse = true;

			// モデル情報を設定
			g_aPolice[nCntPolice].modelData = GetModelData(MODELTYPE_CAR_POLICE);	// モデル情報

			// 影のインデックスを設定
			g_aPolice[nCntPolice].nShadowID = SetModelShadow
			( // 引数
				g_aPolice[nCntPolice].modelData,	// モデル情報
				&g_aPolice[nCntPolice].nShadowID,	// 影の親の影インデックス
				&g_aPolice[nCntPolice].bUse			// 影の親の使用状況
			);

			// 影の位置設定
			SetPositionShadow(g_aPolice[nCntPolice].nShadowID, g_aPolice[nCntPolice].pos, g_aPolice[nCntPolice].rot, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

			D3DXMATERIAL *pMat;					//マテリアルへのポインタ

			//マテリアル情報に対するポインタを取得
			pMat = (D3DXMATERIAL*)g_aPolice[nCntPolice].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aPolice[nCntPolice].modelData.dwNumMat; nCntMat++)
			{
				//マテリアルの情報を取得する
				g_aPolice[nCntPolice].MatCopy[nCntMat] = pMat[nCntMat];
			}

			// 車の位置と向きの設定処理
			SetPolicePosRot(&g_aPolice[nCntPolice]);
			g_aPolice[nCntPolice].policeCurve.nSKipCnt = 0;										// スキップする曲がり角の回数
			g_aPolice[nCntPolice].policeCurve.rotDest = g_aPolice[nCntPolice].rot;				// 前回の向き
			g_aPolice[nCntPolice].policeCurve.actionState = CARACT_DASH;						// 走っている状態

			// 初期位置を設定する
			g_aPolice[nCntPolice].posCopy = g_aPolice[nCntPolice].pos;
			g_aPolice[nCntPolice].rotCopy = g_aPolice[nCntPolice].rot;

			// タックル関係の変数の初期化
			g_aPolice[nCntPolice].tackle.nTackleCnt = 0;			// タックルのカウント
			g_aPolice[nCntPolice].tackle.Tacklemove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// タックル時の追加移動量

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
//	オブジェクトの取得処理
//======================================================================================================================
Police *GetPoliceData(void)
{
	// オブジェクトの情報の先頭アドレスを返す
	return &g_aPolice[0];
}

//============================================================
//	プレイヤーの位置の更新処理
//============================================================
void PosPolice(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove)
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
void RevPolice(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos, D3DXVECTOR3 *move)
{
	// 向きの正規化
	RotNormalize(&rot->y);

	//--------------------------------------------------------
	//	移動範囲の補正
	//--------------------------------------------------------
	if (pos->z > GetLimitStage().fNear - (30.0f * 2))
	{ // 範囲外の場合 (手前)

		// 手前に位置を補正
		pos->z = GetLimitStage().fNear - (30.0f * 2);

		// 移動量を削除
		move->x *= 0.95f;
	}
	if (pos->z < GetLimitStage().fFar + (30.0f * 2))
	{ // 範囲外の場合 (奥)

		// 奥に位置を補正
		pos->z = GetLimitStage().fFar + (30.0f * 2);

		// 移動量を削除
		move->x *= 0.95f;
	}
	if (pos->x > GetLimitStage().fRight - (30.0f * 2))
	{ // 範囲外の場合 (右)

		// 右に位置を補正
		pos->x = GetLimitStage().fRight - (30.0f * 2);

		// 移動量を削除
		move->x *= 0.95f;
	}
	if (pos->x < GetLimitStage().fLeft + (30.0f * 2))
	{ // 範囲外の場合 (左)

		// 左に位置を補正
		pos->x = GetLimitStage().fLeft + (30.0f * 2);

		// 移動量を削除
		move->x *= 0.95f;
	}
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

	// 車のカーブ処理
	CurvePolice(pPolice);

	if (pPolice->move.x > MAX_POLI_FORWARD_PATROL)
	{ // プレイヤーの移動量 (x) が一定値以上の場合

	  // プレイヤーの移動量 (x) を補正
		pPolice->move.x = MAX_POLI_FORWARD_PATROL;
	}

	if (pPlayer->bUse == true)
	{ // プレイヤーが使用されていた場合
		//目的の距離を設定する
		fDist = fabsf(sqrtf((pPlayer->pos.x - pPolice->pos.x) * (pPlayer->pos.x - pPolice->pos.x) + (pPlayer->pos.z - pPolice->pos.z) * (pPlayer->pos.z - pPolice->pos.z)));

		if (fDist <= 1000.0f)
		{ // 目的の距離が一定以内に入ったら
		  // 追跡状態に移行する
			pPolice->state = POLICESTATE_CHASE;
		}
	}
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

	if (fDist <= 3000.0f)
	{ // 目的の距離が一定以内に入ったら
		// 追跡状態に移行する
		pPolice->state = POLICESTATE_CHASE;

		// タックルカウントを加算する
		pPolice->tackle.nTackleCnt++;

		if (pPolice->tackle.nTackleCnt >= POLICAR_TACKLE_CNT)
		{ // 一定時間経ったら
			// タックルのカウントを0にする
			pPolice->tackle.nTackleCnt = 0;

			// タックル状態に移行
			pPolice->state = POLICESTATE_TACKLE;
		}

		// 向きの差分を求める
		fRotDiff = fRotDest - pPolice->rot.y;

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
		pPolice->rot.y += fRotDiff * 0.1f;

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
	// 移動量を更新
	pPolice->move.x += POLI_MOVE_FORWARD;

	// 移動している状態にする
	pPolice->bMove = true;

	//警察車両の探知処理
	PatrolCarSearch(pPolice);

	if (pPolice->move.x >= 20.0f)
	{ // 移動量が一定値以上の場合
		// 移動量を更新
		pPolice->move.x = 20.0f;
	}
}

//============================================================
// パトロールに戻る処理
//============================================================
void PatrolBackAct(Police *pPolice)
{
	//情報をリセットする
	pPolice->pos = pPolice->posCopy;					// 位置
	pPolice->posOld = pPolice->pos;						// 前回の位置
	pPolice->rot = pPolice->rotCopy;					// 向き
	pPolice->move.x = 0.0f;								// 移動量
	pPolice->policeCurve = pPolice->policeCurveCopy;	// 曲がり角の情報を代入
}

//============================================================
// 車のカーブ処理
//============================================================
void CurvePolice(Police *pPolice)
{
	switch (pPolice->policeCurve.actionState)
	{
	case CARACT_DASH:		// 走行状態

		// 車の走行処理
		DashPoliceAction(pPolice);

		break;				// 抜け出す

	case CARACT_CURVE:		// カーブ状態

		// 車の角度更新・補正処理
		CurveInfoRotCar(&pPolice->policeCurve, &pPolice->rot, &pPolice->move, &pPolice->pos);

		break;				// 抜け出す
	}
}

//============================================================
// 車の走行処理
//============================================================
void DashPoliceAction(Police *pPolice)
{
	for (int nCnt = 0; nCnt < MAX_CURVEPOINT; nCnt++)
	{
		switch (pPolice->policeCurve.curveInfo.dashAngle)
		{
		case DASH_RIGHT:		//右に走っている場合

			//這わせる
			pPolice->pos.z = pPolice->policeCurve.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pPolice->pos.z == GetCurveInfo(nCnt).pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
			{ // 同じZ軸上を走っている場合
				if (pPolice->pos.x >= GetCurveInfo(nCnt).pos.x - (CAR_WIDTH * 2) &&
					pPolice->posOld.x <= GetCurveInfo(nCnt).pos.x - (CAR_WIDTH * 2))
				{ // 位置が一致した場合
					if (GetCurveInfo(nCnt).dashAngle == DASH_RIGHT)
					{ // 右に走る場合のみ
					  // スキップカウントを減算する
						pPolice->policeCurve.nSKipCnt--;

						if (pPolice->policeCurve.nSKipCnt == 0 || GetCurveInfo(nCnt).bDeadEnd == true)
						{ // スキップ回数が0になったまたは、行き止まりだった場合
						  // スキップ回数を0にする
							pPolice->policeCurve.nSKipCnt = 0;

							// 曲がり角の情報を更新する
							pPolice->policeCurve.curveInfo = GetCurveInfo(nCnt);
						}
					}
				}
			}

			break;				//抜け出す

		case DASH_LEFT:			//左を走っている場合

								//這わせる
			pPolice->pos.z = pPolice->policeCurve.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pPolice->pos.z == GetCurveInfo(nCnt).pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
			{ // 同じZ軸上を走っている場合
				if (pPolice->pos.x <= GetCurveInfo(nCnt).pos.x + (CAR_WIDTH * 2) &&
					pPolice->posOld.x >= GetCurveInfo(nCnt).pos.x + (CAR_WIDTH * 2))
				{ // 位置が一致した場合
					if (GetCurveInfo(nCnt).dashAngle == DASH_LEFT)
					{ // 左に走る場合のみ
					  // スキップカウントを減算する
						pPolice->policeCurve.nSKipCnt--;

						if (pPolice->policeCurve.nSKipCnt == 0 || GetCurveInfo(nCnt).bDeadEnd == true)
						{ // スキップ回数が0になった場合
						  // スキップ回数を0にする
							pPolice->policeCurve.nSKipCnt = 0;

							// 曲がり角の情報を更新する
							pPolice->policeCurve.curveInfo = GetCurveInfo(nCnt);
						}
					}
				}
			}

			break;				//抜け出す

		case DASH_FAR:			//奥に走っている場合

								//這わせる
			pPolice->pos.x = pPolice->policeCurve.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pPolice->pos.x == GetCurveInfo(nCnt).pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
			{ // 同じZ軸上を走っている場合
				if (pPolice->pos.z >= GetCurveInfo(nCnt).pos.z - (CAR_WIDTH * 2) &&
					pPolice->posOld.z <= GetCurveInfo(nCnt).pos.z - (CAR_WIDTH * 2))
				{ // 位置が一致した場合
					if (GetCurveInfo(nCnt).dashAngle == DASH_FAR)
					{ // 奥に走る場合のみ
					  // スキップカウントを減算する
						pPolice->policeCurve.nSKipCnt--;

						if (pPolice->policeCurve.nSKipCnt == 0 || GetCurveInfo(nCnt).bDeadEnd == true)
						{ // スキップ回数が0になった場合
						  // スキップ回数を0にする
							pPolice->policeCurve.nSKipCnt = 0;

							// 曲がり角の情報を更新する
							pPolice->policeCurve.curveInfo = GetCurveInfo(nCnt);
						}
					}
				}
			}

			break;				//抜け出す

		case DASH_NEAR:			//奥に走っている場合

								//這わせる
			pPolice->pos.x = pPolice->policeCurve.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pPolice->pos.x == GetCurveInfo(nCnt).pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
			{ // 同じZ軸上を走っている場合
				if (pPolice->pos.z <= GetCurveInfo(nCnt).pos.z + (CAR_WIDTH * 2) &&
					pPolice->posOld.z >= GetCurveInfo(nCnt).pos.z + (CAR_WIDTH * 2))
				{ // 位置が一致した場合
					if (GetCurveInfo(nCnt).dashAngle == DASH_NEAR)
					{ // 手前に走る場合のみ
					  // スキップカウントを減算する
						pPolice->policeCurve.nSKipCnt--;

						if (pPolice->policeCurve.nSKipCnt == 0 || GetCurveInfo(nCnt).bDeadEnd == true)
						{ // スキップ回数が0になった場合
						  // スキップ回数を0にする
							pPolice->policeCurve.nSKipCnt = 0;

							// 曲がり角の情報を更新する
							pPolice->policeCurve.curveInfo = GetCurveInfo(nCnt);

							if (pPolice->policeCurve.curveInfo.curveAngle == CURVE_LEFT)
							{ // 曲がる方向が左方向だった場合
							  // 角度を補正する
								pPolice->rot.y = D3DX_PI;
							}
							else
							{ // 曲がる方向が右方向だった場合
							  // 角度を補正する
								pPolice->rot.y = -D3DX_PI;
							}
						}
					}
				}
			}

			break;				//抜け出す
		}
	}

	if (pPolice->policeCurve.nSKipCnt == 0)
	{ // スキップカウントが0の場合
		switch (pPolice->policeCurve.curveInfo.dashAngle)
		{
		case DASH_RIGHT:			//右に走っている

			//這わせる
			pPolice->pos.z = pPolice->policeCurve.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pPolice->policeCurve.curveInfo.curveAngle == CURVE_RIGHT)
			{ // 右に曲がる場合
				if (pPolice->pos.x >= pPolice->policeCurve.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // 車の位置が曲がる位置に達した場合
					// 位置を補正する
					pPolice->pos.x = pPolice->policeCurve.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// カーブ状態にする
					pPolice->policeCurve.actionState = CARACT_CURVE;
				}
			}
			else if (pPolice->policeCurve.curveInfo.curveAngle == CURVE_LEFT)
			{ // 左に曲がる場合
				if (pPolice->pos.x >= pPolice->policeCurve.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // 車の位置が曲がる位置に達した場合
					// 位置を補正する
					pPolice->pos.x = pPolice->policeCurve.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// カーブ状態にする
					pPolice->policeCurve.actionState = CARACT_CURVE;
				}
			}

			break;					//抜け出す

		case DASH_LEFT:				//左に走っている

			//這わせる
			pPolice->pos.z = pPolice->policeCurve.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pPolice->policeCurve.curveInfo.curveAngle == CURVE_RIGHT)
			{ // 右に曲がる場合
				if (pPolice->pos.x <= pPolice->policeCurve.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // 車の位置が曲がる位置に達した場合
					// 位置を補正する
					pPolice->pos.x = pPolice->policeCurve.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// カーブ状態にする
					pPolice->policeCurve.actionState = CARACT_CURVE;
				}
			}
			else if (pPolice->policeCurve.curveInfo.curveAngle == CURVE_LEFT)
			{ // 左に曲がる場合
				if (pPolice->pos.x <= pPolice->policeCurve.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // 車の位置が曲がる位置に達した場合
					// 位置を補正する
					pPolice->pos.x = pPolice->policeCurve.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// カーブ状態にする
					pPolice->policeCurve.actionState = CARACT_CURVE;
				}
			}

			break;					//抜け出す

		case DASH_FAR:				//奥に走っている

									//這わせる
			pPolice->pos.x = pPolice->policeCurve.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pPolice->policeCurve.curveInfo.curveAngle == CURVE_RIGHT)
			{ // 右に曲がる場合
				if (pPolice->pos.z >= pPolice->policeCurve.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // 車の位置が曲がる位置に達した場合
				  // 位置を補正する
					pPolice->pos.z = pPolice->policeCurve.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// カーブ状態にする
					pPolice->policeCurve.actionState = CARACT_CURVE;
				}
			}
			else if (pPolice->policeCurve.curveInfo.curveAngle == CURVE_LEFT)
			{ // 左に曲がる場合
				if (pPolice->pos.z >= pPolice->policeCurve.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // 車の位置が曲がる位置に達した場合
				  // 位置を補正する
					pPolice->pos.z = pPolice->policeCurve.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// カーブ状態にする
					pPolice->policeCurve.actionState = CARACT_CURVE;
				}
			}

			break;					//抜け出す

		case DASH_NEAR:				//手前に走っている

									//這わせる
			pPolice->pos.x = pPolice->policeCurve.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pPolice->policeCurve.curveInfo.curveAngle == CURVE_RIGHT)
			{ // 右に曲がる場合
				if (pPolice->pos.z <= pPolice->policeCurve.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // 車の位置が曲がる位置に達した場合
				  // 位置を補正する
					pPolice->pos.z = pPolice->policeCurve.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// カーブ状態にする
					pPolice->policeCurve.actionState = CARACT_CURVE;
				}
			}
			else if (pPolice->policeCurve.curveInfo.curveAngle == CURVE_LEFT)
			{ // 左に曲がる場合
				if (pPolice->pos.z <= pPolice->policeCurve.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // 車の位置が曲がる位置に達した場合
				  // 位置を補正する
					pPolice->pos.z = pPolice->policeCurve.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// カーブ状態にする
					pPolice->policeCurve.actionState = CARACT_CURVE;
				}
			}

			break;					//抜け出す
		}
	}
}

//============================================================
// 車の位置と向きの設定処理
//============================================================
void SetPolicePosRot(Police *pPolice)
{
	float fCurveDist;			// 最近の曲がり角との距離
	int nCurveNumber = 0;		// 最近の曲がり角の番号

	fCurveDist = fabsf(sqrtf((pPolice->pos.x - GetCurveInfo(0).pos.x) * (pPolice->pos.x - GetCurveInfo(0).pos.x) +
		(pPolice->pos.z - GetCurveInfo(0).pos.z) * (pPolice->pos.z - GetCurveInfo(0).pos.z)));

	for (int nCnt = 1; nCnt < MAX_CURVEPOINT; nCnt++)
	{ // 全ての曲がり角と参照

		float fCurvePoint;		// 曲がり角の値

		// カーブの距離
		fCurvePoint = fabsf(sqrtf((pPolice->pos.x - GetCurveInfo(nCnt).pos.x) * (pPolice->pos.x - GetCurveInfo(nCnt).pos.x) +
			(pPolice->pos.z - GetCurveInfo(nCnt).pos.z) * (pPolice->pos.z - GetCurveInfo(nCnt).pos.z)));

		if (fCurvePoint <= fCurveDist)
		{ // 距離の近さが更新された場合
			// 最近値を更新する
			fCurveDist = fCurvePoint;

			// 番号を更新する
			nCurveNumber = nCnt;
		}
	}

	// 曲がり角の情報を代入する
	pPolice->policeCurve.curveInfo = GetCurveInfo(nCurveNumber);
	pPolice->policeCurveCopy = pPolice->policeCurve;			// 曲がり角の情報のコピーを設定

	// 車の位置の補正処理
	PolicePosRotCorrect(pPolice);
}

//============================================================
// 車の位置の補正処理
//============================================================
void PolicePosRotCorrect(Police *pPolice)
{
	switch (pPolice->policeCurve.curveInfo.dashAngle)
	{
	case DASH_RIGHT:		// 右に向かって走る

		//這わせる
		pPolice->pos.z = pPolice->policeCurve.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

		// 向きを変える
		pPolice->rot.y = D3DX_PI * 0.5f;

		break;				// 抜け出す

	case DASH_LEFT:			// 左に向かって走る

		//這わせる
		pPolice->pos.z = pPolice->policeCurve.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

		// 向きを変える
		pPolice->rot.y = -D3DX_PI * 0.5f;

		break;				// 抜け出す

	case DASH_FAR:			// 奥に向かって走る

		//這わせる
		pPolice->pos.x = pPolice->policeCurve.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

		// 向きを変える
		pPolice->rot.y = 0.0f;

		break;				// 抜け出す

	case DASH_NEAR:			// 手前に向かって走る

		//這わせる
		pPolice->pos.x = pPolice->policeCurve.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

		switch (pPolice->policeCurve.curveInfo.curveAngle)
		{
		case CURVE_RIGHT:	// 右に曲がる

			// 向きを変える
			pPolice->rot.y = -D3DX_PI;

			break;			// 抜け出す

		case CURVE_LEFT:	// 左に曲がる

			// 向きを変える
			pPolice->rot.y = D3DX_PI;

			break;			// 抜け出す
		}

		break;				// 抜け出す
	}
}

//============================================================
// 警察のタックル処理
//============================================================
void PoliceTackle(Police *pPolice)
{
	// タックルのカウントを加算する
	pPolice->tackle.nTackleCnt++;

	if (pPolice->tackle.nTackleCnt <= STOP_POLICAR_CNT)
	{ // 車を止めるカウント数の時
		// 移動量を減衰させていく
		pPolice->move.x *= POLICAR_ATTEN_STOP;

		if (pPolice->move.x <= 0.0f)
		{ // 移動量が0.0f以下になった場合
			// 移動量を0に設定する
			pPolice->move.x = 0.0f;
		}
	}
	else
	{ // 上記以外

		float fRotDest;			// 目標の向き
		float fRotDiff;			// 目標の差分
		Player *pPlayer = GetPlayer();		// プレイヤーの情報

		// 目的の向きを設定する
		fRotDest = atan2f(pPlayer->pos.x - pPolice->pos.x, pPlayer->pos.z - pPolice->pos.z);

		// 向きの差分を求める
		fRotDiff = fRotDest - pPolice->rot.y;

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
		pPolice->rot.y += fRotDiff * (pPolice->move.x * REV_POLI_MOVE_ROT);

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

		// 追加移動量を加算していく
		pPolice->tackle.Tacklemove.x += POLICAR_TACKLE_ADD;

		// 移動量を加算していく
		pPolice->move.x += POLI_MOVE_FORWARD;

		if (pPolice->tackle.Tacklemove.x >= MAX_POLICAR_TACKLE_MOVE)
		{ // 追加分の移動量が一定を超えた場合
			// 移動量を補正する
			pPolice->tackle.Tacklemove.x = MAX_POLICAR_TACKLE_MOVE;
		}

		if (pPolice->tackle.nTackleCnt >= FINISH_POLICAR_TACKLE)
		{ // タックルのカウントが一定数以上になった場合
			// タックルのカウントを0にする
			pPolice->tackle.nTackleCnt = 0;

			// 移動量を0にする
			pPolice->tackle.Tacklemove.x = 0.0f;

			//警察車両の探知処理
			PatrolCarSearch(pPolice);
		}
	}

	//--------------------------------------------------------
	//	位置の更新
	//--------------------------------------------------------
	pPolice->pos.x += sinf(pPolice->rot.y) * pPolice->tackle.Tacklemove.x;
	pPolice->pos.z += cosf(pPolice->rot.y) * pPolice->tackle.Tacklemove.x;
}

//============================================================
// 警察の渋滞改善処理
//============================================================
void PoliceTrafficImprove(Police *pPolice)
{
	// 渋滞カウントを加算する
	pPolice->nTrafficCnt++;

	if (pPolice->nTrafficCnt >= POLICAR_TRAFFIC_IMPROVE_CNT)
	{ // 渋滞カウントが解除状態に入った場合
		// カウントを0にする
		pPolice->nTrafficCnt = 0;

		// パトロール状態にする
		pPolice->state = POLICESTATE_PATROL;
	}
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