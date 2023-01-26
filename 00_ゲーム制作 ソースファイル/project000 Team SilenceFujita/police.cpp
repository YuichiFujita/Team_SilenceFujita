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
 
#include "Car.h"
#include "Police.h"
#include "shadow.h"
#include "sound.h"
#include "player.h"
#include "object.h"

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
#define POLICAR_WIDTH			(45.0f)		// パトカーの縦幅
#define POLICAR_DEPTH			(45.0f)		// パトカーの奥行

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void PosPolice(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove);	// プレイヤーの位置の更新処理
void RevPolice(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos);		// プレイヤーの補正の更新処理
void PatrolPoliceAct(Police *pPolice);					// 警察のパトロール行動処理
void PatrolCarSearch(Police *pPolice);					// 警察車両の探知処理
void ChasePoliceAct(Police *pPolice);					// 警察の追跡処理
void PatrolBackAct(Police *pPolice);					// パトロールに戻る処理
void ReturnPoliceRot(Police *pPolice);					// 警察の向きを直す処理

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

		g_aPolice[nCntPolice].pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aPolice[nCntPolice].posOld	= g_aPolice[nCntPolice].pos;		// 前回の位置
		g_aPolice[nCntPolice].posCopy	= g_aPolice[nCntPolice].pos;		// 最初の位置
		g_aPolice[nCntPolice].move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
		g_aPolice[nCntPolice].rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
		g_aPolice[nCntPolice].rotDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目標の向き
		g_aPolice[nCntPolice].state		= POLICESTATE_PATROL;				// 警察の状態
		g_aPolice[nCntPolice].nLife		= 0;								// 体力
		g_aPolice[nCntPolice].nShadowID = NONE_SHADOW;						// 影のインデックス
		g_aPolice[nCntPolice].bUse		= false;							// 使用状況

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

		// 曲がり角の位置の初期化
		for (int nCnt = 0; nCnt < MAX_CURVE; nCnt++)
		{
			g_aPolice[nCntPolice].policeCurve.bCurvePlus[nCnt] = false;			// ＋方向に走るか
			g_aPolice[nCntPolice].policeCurve.bCurveX[nCnt] = false;			// X軸を走るか
			g_aPolice[nCntPolice].policeCurve.curveAngle[nCnt] = CURVE_LEFT;	// どっちに曲がるか
			g_aPolice[nCntPolice].policeCurve.curvePoint[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// どこで曲がるか
			g_aPolice[nCntPolice].policeCurve.fCurveRot[nCnt] = 0;				// 次向く角度
		}
		g_aPolice[nCntPolice].policeCurve.nCurveTime = 0;						// 曲がり角の回数
		g_aPolice[nCntPolice].policeCurve.nNowCurve = 0;						// 現在の曲がり角
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
	if (GetKeyboardTrigger(DIK_RSHIFT) == true)
	{
		g_aPolice[0].state = POLICESTATE_PATBACK;
	}

	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // オブジェクトの最大表示数分繰り返す
		if (g_aPolice[nCntPolice].bUse == true)
		{ // オブジェクトが使用されている場合

			// 前回位置の更新
			g_aPolice[nCntPolice].posOld = g_aPolice[nCntPolice].pos;

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

			switch (g_aPolice[nCntPolice].state)
			{//状態で判断する
			case POLICESTATE_PATROL:				//パトロール状態

				// 警察のパトロール行動処理
				PatrolPoliceAct(&g_aPolice[nCntPolice]);

				break;						//抜け出す

			case POLICESTATE_CHASE:			//追跡処理

				// 警察の追跡処理
				ChasePoliceAct(&g_aPolice[nCntPolice]);

				break;						//抜け出す

			case POLICESTATE_PATBACK:		//パトロールへ戻る処理			

				break;						//抜け出す

			case POLICESTATE_POSBACK:		//最初の座標に戻る処理

				break;						//抜け出す
			}

			if (g_aPolice[nCntPolice].pos.y < 0.0f)
			{//Y軸の位置が0.0fだった場合
				//縦への移動量を0.0fにする
				g_aPolice[nCntPolice].move.y = 0.0f;

				//位置を0.0fに戻す
				g_aPolice[nCntPolice].pos.y = 0.0f;
			}

			// プレイヤーの補正の更新処理
			RevPolice(&g_aPolice[nCntPolice].rot, &g_aPolice[nCntPolice].pos);

			// プレイヤーの位置の更新
			PosPolice(&g_aPolice[nCntPolice].move, &g_aPolice[nCntPolice].pos, &g_aPolice[nCntPolice].rot, g_aPolice[nCntPolice].bMove);

			//----------------------------------------------------
			//	当たり判定
			//----------------------------------------------------
			// オブジェクトとの当たり判定
			CollisionObject
			( // 引数
				&g_aPolice[nCntPolice].pos,		// 現在の位置
				&g_aPolice[nCntPolice].posOld,	// 前回の位置
				&g_aPolice[nCntPolice].move,	// 移動量
				POLICAR_WIDTH,			// 横幅
				POLICAR_DEPTH			// 奥行
			);

			// 車の停止処理
			CollisionStopCar
			( // 引数
				g_aPolice[nCntPolice].pos,		//位置
				g_aPolice[nCntPolice].rot,		//向き
				&g_aPolice[nCntPolice].move,	//移動量
				g_aPolice[nCntPolice].modelData.fRadius,	//半径
				COLLOBJECTTYPE_POLICE			//対象のサイズ
			);

			// 車同士の当たり判定
			CollisionCarBody
			( // 引数
				&g_aPolice[nCntPolice].pos,
				&g_aPolice[nCntPolice].posOld,
				g_aPolice[nCntPolice].rot,
				&g_aPolice[nCntPolice].move,
				g_aPolice[nCntPolice].modelData,
				COLLOBJECTTYPE_POLICE
			);
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

				switch (g_aPolice[nCntPolice].state)
				{
				case POLICESTATE_PATBACK:	//パトロールに戻っている状態

					// 透明度を下げる
					pMat[nCntMat].MatD3D.Diffuse.a -= 0.005f;
					pMat[nCntMat].MatD3D.Ambient.a -= 0.005f;
					pMat[nCntMat].MatD3D.Emissive.a -= 0.005f;

					if (pMat->MatD3D.Emissive.a <= 0.0f)
					{ // 透明度が0.0f以下になった場合
						// 透明度を下げる
						pMat[nCntMat].MatD3D.Diffuse.a = 0.0f;
						pMat[nCntMat].MatD3D.Ambient.a = 0.0f;
						pMat[nCntMat].MatD3D.Emissive.a = 0.0f;

						// パトロールに戻る処理
						PatrolBackAct(&g_aPolice[nCntPolice]);

						//最初の位置に戻す処理
						g_aPolice[nCntPolice].state = POLICESTATE_POSBACK;
					}

					break;					//抜け出す

				case POLICESTATE_POSBACK:	//最初の位置に戻る状態

					// 透明度を下げる
					pMat[nCntMat].MatD3D.Diffuse.a += 0.005f;
					pMat[nCntMat].MatD3D.Ambient.a += 0.005f;
					pMat[nCntMat].MatD3D.Emissive.a += 0.005f;

					if (pMat->MatD3D.Emissive.a >= 1.0f)
					{ // 透明度が0.0f以下になった場合
					  // 透明度を下げる
						pMat[nCntMat].MatD3D.Diffuse.a = 1.0f;
						pMat[nCntMat].MatD3D.Ambient.a = 1.0f;
						pMat[nCntMat].MatD3D.Emissive.a = 1.0f;

						// 最初の移動量を元に戻す
						g_aPolice[nCntPolice].move.x = 0.0f;

						//最初の位置に戻す処理
						g_aPolice[nCntPolice].state = POLICESTATE_PATROL;
					}

					break;					//抜け出す

				default:					//上記以外

					//元々のマテリアルを代入する
					pMat = (D3DXMATERIAL*)g_aPolice[nCntPolice].modelData.pBuffMat->GetBufferPointer();

					break;					//抜け出す
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
void SetPolice(D3DXVECTOR3 pos, CURVE poliCurve)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aPolice[nCntPolice].bUse == false)
		{ // オブジェクトが使用されていない場合
			// 引数を代入
			g_aPolice[nCntPolice].pos		= pos;								// 現在の位置
			g_aPolice[nCntPolice].posCopy   = g_aPolice[nCntPolice].pos;		// 最初の位置
			g_aPolice[nCntPolice].posOld	= g_aPolice[nCntPolice].pos;		// 前回の位置
			g_aPolice[nCntPolice].rotDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目標の向き
			g_aPolice[nCntPolice].move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
			g_aPolice[nCntPolice].state		= POLICESTATE_PATROL;				// パトロール状態にする
			g_aPolice[nCntPolice].nLife		= POLI_LIFE;						// 体力
			g_aPolice[nCntPolice].bMove		= false;							// 移動していない

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

			D3DXMATERIAL *pMat;					//マテリアルへのポインタ

			//マテリアル情報に対するポインタを取得
			pMat = (D3DXMATERIAL*)g_aPolice[nCntPolice].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aPolice[nCntPolice].modelData.dwNumMat; nCntMat++)
			{
				//マテリアルの情報を取得する
				g_aPolice[nCntPolice].MatCopy[nCntMat] = pMat[nCntMat];
			}

			g_aPolice[nCntPolice].policeCurve.nCurveTime = poliCurve.nCurveTime;				// 曲がる回数

			for (int nCnt = 0; nCnt < g_aPolice[nCntPolice].policeCurve.nCurveTime; nCnt++)
			{
				g_aPolice[nCntPolice].policeCurve.curvePoint[nCnt] = poliCurve.curvePoint[nCnt];	// 曲がるポイント
				g_aPolice[nCntPolice].policeCurve.fCurveRot[nCnt] = poliCurve.fCurveRot[nCnt];		// 次曲がる角度
			}

			g_aPolice[nCntPolice].policeCurve.nNowCurve = 0;									// 現在の曲がり角

			// 曲がる位置の設定処理
			SetCurvePoint(&g_aPolice[nCntPolice].policeCurve, &g_aPolice[nCntPolice].rot, &g_aPolice[nCntPolice].pos);

			if (g_aPolice[nCntPolice].policeCurve.bCurveX[0] == true)
			{ // 最初X軸を走っている場合
				if (g_aPolice[nCntPolice].policeCurve.bCurvePlus[0] == true)
				{ // 右に走っている場合
					// 位置を補正する
					g_aPolice[nCntPolice].pos.z = g_aPolice[nCntPolice].policeCurve.curvePoint[0].z - (POLICAR_WIDTH * 2);
					g_aPolice[nCntPolice].posCopy = g_aPolice[nCntPolice].pos;
				}
				else
				{ // 左に走っている場合
					// 位置を補正する
					g_aPolice[nCntPolice].pos.z = g_aPolice[nCntPolice].policeCurve.curvePoint[0].z + (POLICAR_WIDTH * 2);
					g_aPolice[nCntPolice].posCopy = g_aPolice[nCntPolice].pos;
				}
			}
			else
			{ // 最初Z軸を走っている場合
				if (g_aPolice[nCntPolice].policeCurve.bCurvePlus[0] == true)
				{ // 奥に走っている場合
					// 位置を補正する
					g_aPolice[nCntPolice].pos.x = g_aPolice[nCntPolice].policeCurve.curvePoint[0].x + (POLICAR_WIDTH * 2);
					g_aPolice[nCntPolice].posCopy = g_aPolice[nCntPolice].pos;
				}
				else
				{ // 手前に走っている場合
					// 位置を補正する
					g_aPolice[nCntPolice].pos.x = g_aPolice[nCntPolice].policeCurve.curvePoint[0].x - (POLICAR_WIDTH * 2);
					g_aPolice[nCntPolice].posCopy = g_aPolice[nCntPolice].pos;
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
void RevPolice(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos)
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

	if (pPolice->policeCurve.bCurveX[pPolice->policeCurve.nNowCurve] == true)
	{//X軸を走っていた場合
		if (pPolice->policeCurve.bCurvePlus[pPolice->policeCurve.nNowCurve] == true)
		{ // 右に走っている場合
			if (pPolice->policeCurve.curveAngle[pPolice->policeCurve.nNowCurve] == CURVE_RIGHT)
			{//右に曲がる場合
				if (pPolice->pos.x >= pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x - (POLICAR_WIDTH * 2))
				{ // 左にある壁が途切れたら

					// 車の角度更新・補正処理
					CurveRotCar(&pPolice->policeCurve, &pPolice->rot, &pPolice->move);

					// 移動量を減速
					pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

					// 右の壁に這わせる
					pPolice->pos.x = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x - (POLICAR_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
					// 左の壁に這わせる
					pPolice->pos.z = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z - (POLICAR_WIDTH * 2);
				}
			}
			else if (pPolice->policeCurve.curveAngle[pPolice->policeCurve.nNowCurve] == CURVE_LEFT)
			{//左に曲がる場合
				if (pPolice->pos.x >= pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x + (POLICAR_WIDTH * 2))
				{ // 左にある壁が途切れたら

					// 車の角度更新・補正処理
					CurveRotCar(&pPolice->policeCurve, &pPolice->rot, &pPolice->move);

					// 移動量を減速
					pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

					// 右の壁に這わせる
					pPolice->pos.x = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x + (POLICAR_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
					// 左の壁に這わせる
					pPolice->pos.z = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z - (POLICAR_WIDTH * 2);
				}
			}
		}
		else
		{ // 左に走っている場合
			if (pPolice->policeCurve.curveAngle[pPolice->policeCurve.nNowCurve] == CURVE_RIGHT)
			{//右に曲がる場合
				if (pPolice->pos.x <= pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x + (POLICAR_WIDTH * 2))
				{ // 左にある壁が途切れたら

					// 車の角度更新・補正処理
					CurveRotCar(&pPolice->policeCurve, &pPolice->rot, &pPolice->move);

					// 移動量を減速
					pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

					// 左の壁に這わせる
					pPolice->pos.x = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x + (POLICAR_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
					// 手前の壁に這わせる
					pPolice->pos.z = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z + (POLICAR_WIDTH * 2);
				}
			}
			else if (pPolice->policeCurve.curveAngle[pPolice->policeCurve.nNowCurve] == CURVE_LEFT)
			{//左に曲がる場合
				if (pPolice->pos.x <= pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x - (POLICAR_WIDTH * 2))
				{ // 左にある壁が途切れたら

					// 車の角度更新・補正処理
					CurveRotCar(&pPolice->policeCurve, &pPolice->rot, &pPolice->move);

					// 移動量を減速
					pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

					// 左の壁に這わせる
					pPolice->pos.x = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x - (POLICAR_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
				  // 手前の壁に這わせる
					pPolice->pos.z = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z + (POLICAR_WIDTH * 2);
				}
			}
		}
	}
	else
	{//Z軸を走っていた場合
		if (pPolice->policeCurve.bCurvePlus[pPolice->policeCurve.nNowCurve] == true)
		{ // 奥に走っている場合
			if (pPolice->policeCurve.curveAngle[pPolice->policeCurve.nNowCurve] == CURVE_RIGHT)
			{//右に曲がる場合
				if (pPolice->pos.z >= pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z - (POLICAR_WIDTH * 2))
				{ // 左にある壁が途切れたら

					// 車の角度更新・補正処理
					CurveRotCar(&pPolice->policeCurve, &pPolice->rot, &pPolice->move);

					// 移動量を減速
					pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

					// 手前の壁に這わせる
					pPolice->pos.z = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z - (POLICAR_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
					// 右の壁に這わせる
					pPolice->pos.x = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x + (POLICAR_WIDTH * 2);
				}
			}
			else if (pPolice->policeCurve.curveAngle[pPolice->policeCurve.nNowCurve] == CURVE_LEFT)
			{ // 左に曲がる場合
				if (pPolice->pos.z >= pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z + (POLICAR_WIDTH * 2))
				{ // 左にある壁が途切れたら

					// 車の角度更新・補正処理
					CurveRotCar(&pPolice->policeCurve, &pPolice->rot, &pPolice->move);

					// 移動量を減速
					pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

					// 手前の壁に這わせる
					pPolice->pos.z = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z + (POLICAR_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
					// 右の壁に這わせる
					pPolice->pos.x = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x + (POLICAR_WIDTH * 2);
				}
			}
		}
		else
		{ // 手前に走っている場合
			if (pPolice->policeCurve.curveAngle[pPolice->policeCurve.nNowCurve] == CURVE_RIGHT)
			{//右に曲がる場合
				if (pPolice->pos.z <= pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z + (POLICAR_WIDTH * 2))
				{ // 左にある壁が途切れたら

					// 車の角度更新・補正処理
					CurveRotCar(&pPolice->policeCurve, &pPolice->rot, &pPolice->move);

					// 移動量を減速
					pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

					// 左の壁に這わせる
					pPolice->pos.z = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z + (POLICAR_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
					// 左の壁に這わせる
					pPolice->pos.x = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x - (POLICAR_WIDTH * 2);
				}
			}
			else if (pPolice->policeCurve.curveAngle[pPolice->policeCurve.nNowCurve] == CURVE_LEFT)
			{
				if (pPolice->pos.z <= pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z - (POLICAR_WIDTH * 2))
				{ // 左にある壁が途切れたら

					// 車の角度更新・補正処理
					CurveRotCar(&pPolice->policeCurve, &pPolice->rot, &pPolice->move);

					// 移動量を減速
					pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

					// 左の壁に這わせる
					pPolice->pos.z = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z - (POLICAR_WIDTH * 2);
				}
				else
				{ // 左にある壁がまだあったら
					// 左の壁に這わせる
					pPolice->pos.x = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x - (POLICAR_WIDTH * 2);
				}
			}
		}
	}

	if (pPolice->move.x > MAX_POLI_FORWARD_PATROL)
	{ // プレイヤーの移動量 (x) が一定値以上の場合

	  // プレイヤーの移動量 (x) を補正
		pPolice->move.x = MAX_POLI_FORWARD_PATROL;
	}

	//目的の距離を設定する
	fDist = fabsf(sqrtf((pPlayer->pos.x - pPolice->pos.x) * (pPlayer->pos.x - pPolice->pos.x) + (pPlayer->pos.z - pPolice->pos.z) * (pPlayer->pos.z - pPolice->pos.z)));

	if (fDist <= 1000.0f)
	{ // 目的の距離が一定以内に入ったら
		// 追跡状態に移行する
		pPolice->state = POLICESTATE_CHASE;
	}

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

	if (fDist <= 3000.0f)
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
	// 移動量を更新
	pPolice->move.x += POLI_MOVE_FORWARD;

	// 移動している状態にする
	pPolice->bMove = true;

	//警察車両の探知処理
	PatrolCarSearch(pPolice);

	if (pPolice->move.x >= 25.0f)
	{ // 移動量が一定値以上の場合
		// 移動量を更新
		pPolice->move.x = 25.0f;
	}
}

//============================================================
// パトロールに戻る処理
//============================================================
void PatrolBackAct(Police *pPolice)
{
	//最初の位置と向きと移動量、今進むべき方向をリセットする
	pPolice->pos = pPolice->posCopy;
	ReturnPoliceRot(pPolice);				//移動量の設定処理
	pPolice->move.x = 0.0f;
	pPolice->policeCurve.nNowCurve = 0;
}

//============================================================
// 警察の向きを直す処理
//============================================================
void ReturnPoliceRot(Police *pPolice)
{
	if (pPolice->policeCurve.bCurveX[0] == true)
	{ // X軸を移動していた場合
		if (pPolice->policeCurve.bCurvePlus[0] == true)
		{ // 右方向に移動していた場合
			// 向きを90度に設定する
			pPolice->rot.y = D3DXToRadian(90);
		}
		else
		{ // 左方向に移動していた場合
			// 向きを-90度に設定する
			pPolice->rot.y = D3DXToRadian(-90);
		}
	}
	else
	{ // Z軸を移動していた場合
		if (pPolice->policeCurve.bCurvePlus[0] == true)
		{ // 奥方向に移動していた場合
			// 向きを0度に設定する
			pPolice->rot.y = D3DXToRadian(0);
		}
		else
		{ // 手前方向に移動していた場合
			if (pPolice->policeCurve.curveAngle[0] == CURVE_RIGHT)
			{ // 右に曲がる場合
				// 向きを-180度に設定する
				pPolice->rot.y = D3DXToRadian(-180);
			}
			else if (pPolice->policeCurve.curveAngle[0] == CURVE_LEFT)
			{ // 左に曲がる場合
				// 向きを180度に設定する
				pPolice->rot.y = D3DXToRadian(180);

			}
		}
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