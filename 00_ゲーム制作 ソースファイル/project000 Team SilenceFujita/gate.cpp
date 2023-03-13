//======================================================================================================================
//
//	ゲート処理 [gate.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "calculation.h"
#include "model.h"

#include "game.h"
#include "tutorial.h"

#include "gate.h"
#include "3Dnotation.h"
#include "player.h"
#include "timer.h"
#include "particle.h"
#include "shadow.h"
#include "sound.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define GATE_GRAVITY			(1.0f)		// 重力
#define GATE_OPEN				(2.5f)		// ゲートの開く速度
#define GATE_DOOR_PLUSPOS		(285.0f)	// ドアの初期位置の y座標加算量
#define GATE_EXIT_WIDESIZE_MUL	(0.3f)		// 脱出口の横位置加算量の倍率
#define GATE_EXIT_FORWARDPLUS	(200.0f)	// 脱出口の前方加算量

#define GATE_PINCH_CNT			(60)		// ゲートのピンチの境界線
#define GATE_PINCH_COL			(D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f))			// 制限時間間近のゲートの色
#define GATE_ATTEN_CNT			(10)		// ゲートの注意の境界線
#define GATE_ATTEN_COL_CNT		(30)		// ゲートの色の変わるカウント数
#define GATE_ATTEN_EMI			(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))
#define GATE_ATTEN_DIF			(D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f))
#define GATE_ATTEN_AMB			(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void GateAttention(Gate *pGate);			// ゲートの注意処理

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
Gate      g_aGate[MAX_GATE];				// オブジェクトの情報
Exit      g_exit;							// 脱出の情報
Collision g_aCollGate[MODEL_GATE_MAX];		// 当たり判定の情報

//======================================================================================================================
//	オブジェクトの初期化処理
//======================================================================================================================
void InitGate(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// オブジェクトの情報の初期化
	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // オブジェクトの最大表示数分繰り返す

		//--------------------------------------------------------------------------------------------------------------
		//	門の情報を初期化
		//--------------------------------------------------------------------------------------------------------------
		// 基本情報の初期化
		g_aGate[nCntGate].pos     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aGate[nCntGate].rot     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
		g_aGate[nCntGate].state   = GATESTATE_FLY;					// 状態
		g_aGate[nCntGate].nNotaID = NONE_3D_NOTATION;				// 強調表示のインデックス
		g_aGate[nCntGate].bUse    = false;							// 使用状況

		// モデル情報の初期化
		g_aGate[nCntGate].modelData.dwNumMat = 0;					// マテリアルの数
		g_aGate[nCntGate].modelData.pTexture = NULL;				// テクスチャへのポインタ
		g_aGate[nCntGate].modelData.pMesh    = NULL;				// メッシュ (頂点情報) へのポインタ
		g_aGate[nCntGate].modelData.pBuffMat = NULL;				// マテリアルへのポインタ
		g_aGate[nCntGate].modelData.vtxMin   = INIT_VTX_MIN;		// 最小の頂点座標
		g_aGate[nCntGate].modelData.vtxMax   = INIT_VTX_MAX;		// 最大の頂点座標
		g_aGate[nCntGate].modelData.size     = INIT_SIZE;			// 大きさ
		g_aGate[nCntGate].modelData.fRadius  = 0.0f;				// 半径

		// アイコンの情報の初期化
		g_aGate[nCntGate].icon.nIconID = NONE_ICON;					// アイコンのインデックス
		g_aGate[nCntGate].icon.state = ICONSTATE_NONE;				// アイコンの状態

		// 当たり判定情報の初期化
		for (int nCntCollGate = 0; nCntCollGate < MODEL_GATE_MAX; nCntCollGate++)
		{ // ゲートの種類の総数分繰り返す

			for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
			{ // 当たり判定の最大数分繰り返す

				g_aGate[nCntGate].collInfo[nCntCollGate].vecPos[nCntColl] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置ベクトル
				g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl] = 0.0f;							// 横幅
				g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl] = 0.0f;							// 奥行
			}
		}

		//--------------------------------------------------------------------------------------------------------------
		//	鉄板の情報を初期化
		//--------------------------------------------------------------------------------------------------------------
		// 基本情報の初期化
		g_aGate[nCntGate].doorData.fPos  = 0.0f;	// 位置
		g_aGate[nCntGate].doorData.fMove = 0.0f;	// 移動量

		// モデル情報の初期化
		g_aGate[nCntGate].doorData.modelData.dwNumMat = 0;					// マテリアルの数
		g_aGate[nCntGate].doorData.modelData.pTexture = NULL;				// テクスチャへのポインタ
		g_aGate[nCntGate].doorData.modelData.pMesh    = NULL;				// メッシュ (頂点情報) へのポインタ
		g_aGate[nCntGate].doorData.modelData.pBuffMat = NULL;				// マテリアルへのポインタ
		g_aGate[nCntGate].doorData.modelData.vtxMin   = INIT_VTX_MIN;		// 最小の頂点座標
		g_aGate[nCntGate].doorData.modelData.vtxMax   = INIT_VTX_MAX;		// 最大の頂点座標
		g_aGate[nCntGate].doorData.modelData.size     = INIT_SIZE;			// 大きさ
		g_aGate[nCntGate].doorData.modelData.fRadius  = 0.0f;				// 半径

		// 注意の情報の初期化
		g_aGate[nCntGate].atten.bUse = false;								// 使用状況
		g_aGate[nCntGate].atten.nAttenCounter = 0;							// 注意カウント
		g_aGate[nCntGate].atten.state = ATTENTIONSTATE_NONE;				// 注意の状態
	}

	// 脱出の情報の初期化
	g_exit.pGate = NULL;	// 脱出ゲートのアドレス
	g_exit.bExit = false;	// 脱出状況

	// 当たり判定の情報の初期化
	for (int nCntGate = 0; nCntGate < MODEL_GATE_MAX; nCntGate++)
	{ // ゲートの種類の総数分繰り返す

		// 当たり判定の総数を初期化
		g_aCollGate[nCntGate].nNumColl = 1;

		for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
		{ // 当たり判定の最大数分繰り返す

			g_aCollGate[nCntGate].vecPos[nCntColl] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置ベクトル
			g_aCollGate[nCntGate].scale[nCntColl]  = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// 拡大率
			g_aCollGate[nCntGate].fWidth[nCntColl] = 0.0f;							// 横幅
			g_aCollGate[nCntGate].fDepth[nCntColl] = 0.0f;							// 奥行
		}
	}
}

//======================================================================================================================
//	オブジェクトの終了処理
//======================================================================================================================
void UninitGate(void)
{

}

//======================================================================================================================
//	オブジェクトの更新処理
//======================================================================================================================
void UpdateGate(void)
{
	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aGate[nCntGate].bUse == true)
		{ // オブジェクトが使用されている場合

			switch (g_aGate[nCntGate].state)
			{ // 状態ごとの処理
			case GATESTATE_FLY:		// 未閉状態

				// ゲートの脱出判定
				CollisionExitGate(&g_aGate[nCntGate]);

				// 処理を抜ける
				break;

			case GATESTATE_LAND:	// 落下状態

				// 重力をかける
				g_aGate[nCntGate].doorData.fMove -= GATE_GRAVITY;

				// 位置を更新する
				g_aGate[nCntGate].doorData.fPos += g_aGate[nCntGate].doorData.fMove;

				if (g_aGate[nCntGate].doorData.fPos <= 0.0f)
				{ // 地面に到達した場合

					// 位置を補正する
					g_aGate[nCntGate].doorData.fPos = 0.0f;

					// 移動量を設定する
					g_aGate[nCntGate].doorData.fMove = 8.0f;

					// バウンド状態にする
					g_aGate[nCntGate].state = GATESTATE_BOUND;
				}

				// 処理を抜ける
				break;

			case GATESTATE_BOUND:	// バウンド状態

				// 重力をかける
				g_aGate[nCntGate].doorData.fMove -= GATE_GRAVITY;

				// 位置を更新する
				g_aGate[nCntGate].doorData.fPos += g_aGate[nCntGate].doorData.fMove;

				if (g_aGate[nCntGate].doorData.fPos <= 0.0f)
				{ // 地面に到達した場合

					// 位置を補正する
					g_aGate[nCntGate].doorData.fPos = 0.0f;

					// 移動量を0にする
					g_aGate[nCntGate].doorData.fMove = 0.0f;

					// 停止状態にする
					g_aGate[nCntGate].state = GATESTATE_STOP;
				}

				// 処理を抜ける
				break;

			case GATESTATE_STOP:	// 停止状態

				// 無し

				// 処理を抜ける
				break;

			case GATESTATE_OPEN:	// 開き状態

				// 重力をかける
				g_aGate[nCntGate].doorData.fMove = GATE_OPEN;

				// 位置を更新する
				g_aGate[nCntGate].doorData.fPos += g_aGate[nCntGate].doorData.fMove;

				if (g_aGate[nCntGate].doorData.fPos >= GATE_DOOR_PLUSPOS)
				{ // 地面に到達した場合

					// 位置を補正する
					g_aGate[nCntGate].doorData.fPos = GATE_DOOR_PLUSPOS;

					// 移動量を設定する
					g_aGate[nCntGate].doorData.fMove = 0.0f;

					// バウンド状態にする
					g_aGate[nCntGate].state = GATESTATE_FLY;
				}

				// 処理を抜ける
				break;
			}

			// ゲートの注意処理
			GateAttention(&g_aGate[nCntGate]);

			// アイコンの位置設定処理
			SetPositionIcon
			(
				g_aGate[nCntGate].icon.nIconID,
				g_aGate[nCntGate].pos
			);

			// 強調表示の位置設定
			SetPosition3DNotation
			( // 引数
				g_aGate[nCntGate].nNotaID,	// 強調表示インデックス
				D3DXVECTOR3					// 位置
				( // 引数
					g_aGate[nCntGate].pos.x,															// x
					g_aGate[nCntGate].pos.y + g_aGate[nCntGate].modelData.size.y + NOTA_PLUS_POS_EXIT,	// y
					g_aGate[nCntGate].pos.z																// z
				)
			);
		}
	}
}

//======================================================================================================================
//	オブジェクトの描画処理
//======================================================================================================================
void DrawGate(void)
{
	// 変数を宣言
	D3DXMATRIX   mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ
	D3DXMATERIAL	  redMat;					// マテリアルデータ(赤)
	D3DXMATERIAL	  attenMat;					// マテリアルデータ(注意色)

	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aGate[nCntGate].bUse == true)
		{ // オブジェクトが使用されている場合

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//----------------------------------------------------------------------------------------------------------
			//	門の描画
			//----------------------------------------------------------------------------------------------------------
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aGate[nCntGate].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aGate[nCntGate].rot.y, g_aGate[nCntGate].rot.x, g_aGate[nCntGate].rot.z);
			D3DXMatrixMultiply(&g_aGate[nCntGate].mtxWorld, &g_aGate[nCntGate].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aGate[nCntGate].pos.x, g_aGate[nCntGate].pos.y, g_aGate[nCntGate].pos.z);
			D3DXMatrixMultiply(&g_aGate[nCntGate].mtxWorld, &g_aGate[nCntGate].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aGate[nCntGate].mtxWorld);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aGate[nCntGate].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aGate[nCntGate].modelData.dwNumMat; nCntMat++)
			{ // マテリアルの数分繰り返す

				switch (GetMode())
				{
				case MODE_GAME:			// ゲームの場合

					if (GetTime() <= GATE_ATTEN_CNT)
					{ // タイムが注意カウント以下だった場合

						switch (g_aGate[nCntGate].atten.state)
						{
						case ATTENTIONSTATE_NONE:		// 通常状態

							// マテリアルの設定(通常色で描画)
							pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

							break;

						case ATTENTIONSTATE_FLASH:		// 点滅状態

							// 構造体の要素をクリア
							ZeroMemory(&attenMat, sizeof(D3DXMATERIAL));

							// 拡散光・環境光・自己発光
							attenMat.MatD3D.Emissive = GATE_ATTEN_EMI;
							attenMat.MatD3D.Diffuse = GATE_ATTEN_DIF;
							attenMat.MatD3D.Ambient = GATE_ATTEN_AMB;

							// マテリアルの設定(注意色で描画)
							pDevice->SetMaterial(&attenMat.MatD3D);

							break;
						}

					}
					else if (GetTime() <= GATE_PINCH_CNT)
					{ // タイムがピンチカウント以下だった場合

						// 構造体の要素をクリア
						ZeroMemory(&redMat, sizeof(D3DXMATERIAL));

						// 色を設定する
						redMat.MatD3D.Diffuse = GATE_PINCH_COL;
						redMat.MatD3D.Ambient = GATE_PINCH_COL;

						// マテリアルの設定
						pDevice->SetMaterial(&redMat.MatD3D);
					}
					else
					{ // 上記以外

						// マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
					}

					break;

				default:				// 上記以外

					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					break;
				}

				// テクスチャの設定
				pDevice->SetTexture(0, g_aGate[nCntGate].modelData.pTexture[nCntMat]);

				// モデルの描画
				g_aGate[nCntGate].modelData.pMesh->DrawSubset(nCntMat);
			}

			//----------------------------------------------------------------------------------------------------------
			//	鉄板の描画
			//----------------------------------------------------------------------------------------------------------
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aGate[nCntGate].doorData.mtxWorld);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, 0.0f, g_aGate[nCntGate].doorData.fPos, 0.0f);
			D3DXMatrixMultiply(&g_aGate[nCntGate].doorData.mtxWorld, &g_aGate[nCntGate].doorData.mtxWorld, &mtxTrans);

			// ゲートのマトリックスと掛け合わせる
			D3DXMatrixMultiply
			( // 引数
				&g_aGate[nCntGate].mtxWorld,
				&g_aGate[nCntGate].mtxWorld,
				&g_aGate[nCntGate].doorData.mtxWorld
			);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aGate[nCntGate].mtxWorld);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aGate[nCntGate].doorData.modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aGate[nCntGate].doorData.modelData.dwNumMat; nCntMat++)
			{ // マテリアルの数分繰り返す

				switch (GetMode())
				{
				case MODE_GAME:			// ゲームの場合

					if (GetTime() <= GATE_ATTEN_CNT)
					{ // タイムが注意カウント以下だった場合

						switch (g_aGate[nCntGate].atten.state)
						{
						case ATTENTIONSTATE_NONE:		// 通常状態

							// マテリアルの設定(通常色で描画)
							pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

							break;

						case ATTENTIONSTATE_FLASH:		// 点滅状態

							// 構造体の要素をクリア
							ZeroMemory(&attenMat, sizeof(D3DXMATERIAL));

							// 拡散光・環境光・自己発光
							attenMat.MatD3D.Emissive = GATE_ATTEN_EMI;
							attenMat.MatD3D.Diffuse = GATE_ATTEN_DIF;
							attenMat.MatD3D.Ambient = GATE_ATTEN_AMB;

							// マテリアルの設定(注意色で描画)
							pDevice->SetMaterial(&attenMat.MatD3D);

							break;
						}

					}
					else if (GetTime() <= GATE_PINCH_CNT)
					{ // タイムがピンチカウント以下だった場合

						// 構造体の要素をクリア
						ZeroMemory(&redMat, sizeof(D3DXMATERIAL));

						// 色を設定する
						redMat.MatD3D.Diffuse = GATE_PINCH_COL;
						redMat.MatD3D.Ambient = GATE_PINCH_COL;

						// マテリアルの設定
						pDevice->SetMaterial(&redMat.MatD3D);
					}
					else
					{ // 上記以外

						// マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
					}

					break;

				default:				// 上記以外

					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					break;
				}

				// テクスチャの設定
				pDevice->SetTexture(0, g_aGate[nCntGate].doorData.modelData.pTexture[nCntMat]);

				// モデルの描画
				g_aGate[nCntGate].doorData.modelData.pMesh->DrawSubset(nCntMat);
			}

			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	オブジェクトの設定処理
//======================================================================================================================
void SetGate(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ROTSTATE stateRot, bool bOpen)
{
	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aGate[nCntGate].bUse == false)
		{ // オブジェクトが使用されていない場合

			// 引数を代入
			g_aGate[nCntGate].pos = pos;	// 位置
			g_aGate[nCntGate].rot = rot;	// 向き

			if (bOpen == true)
			{ // オープン状態の場合
				g_aGate[nCntGate].state = GATESTATE_FLY;				// 状態
				g_aGate[nCntGate].doorData.fPos = GATE_DOOR_PLUSPOS;	// 位置
			}
			else
			{ // クローズ状態だった場合
				g_aGate[nCntGate].state = GATESTATE_STOP;				// 状態
				g_aGate[nCntGate].doorData.fPos = 0.0f;					// 位置
			}

			// 情報を初期化
			g_aGate[nCntGate].doorData.fMove = 0.0f;				// 移動量

			// モデル情報を設定
			g_aGate[nCntGate].modelData          = GetModelData(MODELTYPE_OBJECT_GATE000);	// 門のモデル情報
			g_aGate[nCntGate].doorData.modelData = GetModelData(MODELTYPE_OBJECT_GATE001);	// 鉄板のモデル情報

			// 使用している状態にする
			g_aGate[nCntGate].bUse = true;

			if (stateRot == ROTSTATE_0
			||  stateRot == ROTSTATE_180)
			{ // 角度が0度、または180度の場合

				// アイコンの設定処理
				g_aGate[nCntGate].icon.nIconID = SetIcon
				(
					g_aGate[nCntGate].pos,
					ICONTYPE_GATE_VERT,
					&g_aGate[nCntGate].icon.nIconID,
					&g_aGate[nCntGate].bUse,
					&g_aGate[nCntGate].icon.state
				);
			}
			else
			{ // 角度が90度、または270度の場合

				// アイコンの設定処理
				g_aGate[nCntGate].icon.nIconID = SetIcon
				(
					g_aGate[nCntGate].pos,
					ICONTYPE_GATE_HORIZ,
					&g_aGate[nCntGate].icon.nIconID,
					&g_aGate[nCntGate].bUse,
					&g_aGate[nCntGate].icon.state
				);
			}

			// 当たり判定情報を設定
			for (int nCntCollGate = 0; nCntCollGate < MODEL_GATE_MAX; nCntCollGate++)
			{ // ゲートの種類の総数分繰り返す

				// 向き状態を設定
				g_aGate[nCntGate].collInfo[nCntCollGate].stateRot = stateRot;

				for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
				{ // 当たり判定の最大数分繰り返す

					// 横幅と縦幅を計算
					if (stateRot == ROTSTATE_0
					||  stateRot == ROTSTATE_180)
					{ // 角度が0度、または180度の場合

						// 判定サイズを設定
						g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl] = g_aCollGate[nCntCollGate].fWidth[nCntColl];
						g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl] = g_aCollGate[nCntCollGate].fDepth[nCntColl];
					}
					else
					{ // 角度90度、または270度の場合

						// 判定サイズを設定
						g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl] = g_aCollGate[nCntCollGate].fDepth[nCntColl];
						g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl] = g_aCollGate[nCntCollGate].fWidth[nCntColl];
					}
				}

				// 当たり判定を設定
				for (int nCntColl = 0; nCntColl < g_aCollGate[nCntCollGate].nNumColl; nCntColl++)
				{ // 当たり判定の総数分繰り返す

					// 位置ベクトルを設定
					g_aGate[nCntGate].collInfo[nCntCollGate].vecPos[nCntColl] = g_aCollGate[nCntCollGate].vecPos[nCntColl];

					for (int nCntRot = ROTSTATE_0; nCntRot < stateRot; nCntRot++)
					{ // 設定された向きになるまで繰り返す

						// 位置ベクトルを90度回転
						g_aGate[nCntGate].collInfo[nCntCollGate].vecPos[nCntColl] = D3DXVECTOR3
						( // 引数
							-g_aGate[nCntGate].collInfo[nCntCollGate].vecPos[nCntColl].z,
							 g_aGate[nCntGate].collInfo[nCntCollGate].vecPos[nCntColl].y,
							 g_aGate[nCntGate].collInfo[nCntCollGate].vecPos[nCntColl].x
						);
					}
				}
			}

			// 強調表示のインデックスを設定
			g_aGate[nCntGate].nNotaID = Set3DNotation
			( // 引数
				NOTATIONTYPE_EXIT,			// 強調表示の種類
				&g_aGate[nCntGate].nNotaID,	// 親の強調表示インデックス
				&g_aGate[nCntGate].bUse		// 親の使用状況
			);

			// 強調表示の位置設定
			SetPosition3DNotation
			( // 引数
				g_aGate[nCntGate].nNotaID,	// 強調表示インデックス
				D3DXVECTOR3					// 位置
				( // 引数
					g_aGate[nCntGate].pos.x,															// x
					g_aGate[nCntGate].pos.y + g_aGate[nCntGate].modelData.size.y + NOTA_PLUS_POS_EXIT,	// y
					g_aGate[nCntGate].pos.z																// z
				)
			);

			// 処理を抜ける
			break;
		}
	}
}

//======================================================================================================================
//	オブジェクトとの当たり判定
//======================================================================================================================
void CollisionGate(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pOldPos, D3DXVECTOR3 *pMove, float fWidth, float fDepth)
{
	// 変数を宣言
	D3DXVECTOR3 collPos;	// 当たり判定の中心座標
	int         nNumLoop;	// 繰り返し数の設定用

	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aGate[nCntGate].bUse == true)
		{ // オブジェクトが使用されている場合

			// 繰り返し数を設定
			nNumLoop = (g_aGate[nCntGate].state != GATESTATE_FLY || g_aGate[nCntGate].state != GATESTATE_OPEN) ? MODEL_GATE_MAX : 1;

			for (int nCntCollGate = 0; nCntCollGate < nNumLoop; nCntCollGate++)
			{ // 設定された繰り返し数分ループ

				for (int nCntColl = 0; nCntColl < g_aCollGate[nCntCollGate].nNumColl; nCntColl++)
				{ // 当たり判定数分繰り返す

					// 当たり判定の中心座標を設定
					collPos = g_aGate[nCntGate].pos - g_aGate[nCntGate].collInfo[nCntCollGate].vecPos[nCntColl];

					// 前後の当たり判定
					if (pPos->x + fWidth > collPos.x - g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl]
					&&  pPos->x - fWidth < collPos.x + g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl])
					{ // ブロックの左右の範囲内の場合

						if (pPos->z    + fDepth >  collPos.z - g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl]
						&&  pOldPos->z + fDepth <= collPos.z - g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl])
						{ // 前からの当たり判定

							// 位置を補正
							pPos->z = collPos.z - g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl] - fDepth - 0.01f;

							if (pMove->x >= 13.0f)
							{ // 移動量が一定以上の場合

								// パーティクルの設定処理
								SetParticle
								( // 引数
									D3DXVECTOR3(pPos->x, pPos->y + 50.0f, collPos.z - g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl]),	// 位置
									D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),	// 色
									PARTICLETYPE_SPARK,					// パーティクルの種類
									10,									// 生成数
									2									// 寿命
								);
							}

							// 移動量を削除
							CollisionSlow(&pMove->x);
						}
						else if (pPos->z    - fDepth <  collPos.z + g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl]
						     &&  pOldPos->z - fDepth >= collPos.z + g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl])
						{ // 後ろからの当たり判定

							// 位置を補正
							pPos->z = collPos.z + g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl] + fDepth + 0.01f;

							if (pMove->x >= 13.0f)
							{ // 移動量が一定以上の場合

								// パーティクルの設定処理
								SetParticle
								( // 引数
									D3DXVECTOR3(pPos->x, pPos->y + 50.0f, collPos.z + g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl]),	// 位置
									D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),	// 色
									PARTICLETYPE_SPARK,					// パーティクルの種類
									10,									// 生成数
									2									// 寿命
								);
							}

							// 移動量を削除
							CollisionSlow(&pMove->x);
						}
					}

					// 左右の当たり判定
					if (pPos->z + fDepth > collPos.z - g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl]
					&&  pPos->z - fDepth < collPos.z + g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl])
					{ // ブロックの前後の範囲内の場合

						if (pPos->x    + fWidth >  collPos.x - g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl]
						&&  pOldPos->x + fWidth <= collPos.x - g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl])
						{ // 左からの当たり判定

							// 位置を補正
							pPos->x = collPos.x - g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl] - fWidth - 0.01f;

							if (pMove->x >= 13.0f)
							{ // 移動量が一定以上の場合

								// パーティクルの設定処理
								SetParticle
								( // 引数
									D3DXVECTOR3(collPos.x - g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl], pPos->y + 50.0f, pPos->z),	// 位置
									D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),	// 色
									PARTICLETYPE_SPARK,					// パーティクルの種類
									10,									// 生成数
									2									// 寿命
								);
							}

							// 移動量を削除
							CollisionSlow(&pMove->x);
						}
						else if (pPos->x    - fWidth <  collPos.x + g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl]
						     &&  pOldPos->x - fWidth >= collPos.x + g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl])
						{ // 右からの当たり判定
							
							// 位置を補正
							pPos->x = collPos.x + g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl] + fWidth + 0.01f;

							if (pMove->x >= 13.0f)
							{ // 移動量が一定以上の場合

								// パーティクルの設定処理
								SetParticle
								( // 引数
									D3DXVECTOR3(collPos.x + g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl], pPos->y + 50.0f, pPos->z),	// 位置
									D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),	// 色
									PARTICLETYPE_SPARK,					// パーティクルの種類
									10,									// 生成数
									2									// 寿命
								);
							}

							// 移動量を削除
							CollisionSlow(&pMove->x);
						}
					}
				}
			}
		}
	}
}

//======================================================================================================================
//	ゲートの脱出判定
//======================================================================================================================
void CollisionExitGate(Gate *pGate)
{
	// 変数を宣言
	D3DXVECTOR3 leftPos;			// ゲートの左位置
	D3DXVECTOR3 rightPos;			// ゲートの右位置
	D3DXVECTOR3 leftForwardPos;		// ゲートの左の前方位置
	D3DXVECTOR3 rightForwardPos;	// ゲートの右の前方位置
	float       fSideSize;			// ゲートの横幅
	bool        bExit = false;		// 脱出可能状況

	// プレイヤーを宣言
	Player *pPlayer = GetPlayer();	// プレイヤーを宣言

	switch (GetMode())
	{ // モードごとの処理
	case MODE_GAME:		// ゲーム画面

		if (*GetGameState() == GAMESTATE_NORMAL)
		{ // ゲームが通常状態の場合

			// 脱出可能にする
			bExit = true;
		}

		// 処理を抜ける
		break;

	case MODE_TUTORIAL:	// チュートリアル画面

		if (GetTutorialState() == TUTORIALSTATE_NORMAL)
		{ // チュートリアルが通常状態の場合

			// 脱出可能にする
			bExit = true;
		}

		// 処理を抜ける
		break;
	}

	if (bExit == true)
	{ // 脱出可能な場合

		// ゲートの横幅加算量を設定
		fSideSize = pGate->modelData.size.x * GATE_EXIT_WIDESIZE_MUL;

		// 左位置を求める
		leftPos.x = pGate->pos.x - sinf(pGate->rot.y + (D3DX_PI * 0.5f)) * fSideSize;	// x
		leftPos.y = 0.0f;																// y
		leftPos.z = pGate->pos.z - cosf(pGate->rot.y + (D3DX_PI * 0.5f)) * fSideSize;	// z
		leftPos.x -= sinf(pGate->rot.y) * GATE_EXIT_FORWARDPLUS;	// x
		leftPos.z -= cosf(pGate->rot.y) * GATE_EXIT_FORWARDPLUS;	// z

		// 左の前方位置を求める
		leftForwardPos = leftPos;
		leftForwardPos.x -= sinf(pGate->rot.y) * 1.0f;	// x
		leftForwardPos.z -= cosf(pGate->rot.y) * 1.0f;	// z

		// 右位置を求める
		rightPos.x = pGate->pos.x + sinf(pGate->rot.y + (D3DX_PI * 0.5f)) * fSideSize;	// x
		rightPos.y = 0.0f;																// y
		rightPos.z = pGate->pos.z + cosf(pGate->rot.y + (D3DX_PI * 0.5f)) * fSideSize;	// z
		rightPos.x -= sinf(pGate->rot.y) * GATE_EXIT_FORWARDPLUS;	// x
		rightPos.z -= cosf(pGate->rot.y) * GATE_EXIT_FORWARDPLUS;	// z

		// 右の前方位置を求める
		rightForwardPos = rightPos;
		rightForwardPos.x -= sinf(pGate->rot.y) * 1.0f;	// x
		rightForwardPos.z -= cosf(pGate->rot.y) * 1.0f;	// z

		if (LineOuterProduct(leftPos,  rightPos,        pPlayer->pos) < 0
		&&  LineOuterProduct(leftPos,  leftForwardPos,  pPlayer->pos) < 0
		&&  LineOuterProduct(rightPos, rightForwardPos, pPlayer->pos) > 0)
		{ // ゲートを通った場合

			// 脱出ゲートのアドレスを代入
			g_exit.pGate = pGate;

			// 脱出した状態にする
			g_exit.bExit = true;
		}
	}
}

//======================================================================================================================
//	ゲートの全閉め処理
//======================================================================================================================
void AllShutOutGate(void)
{
	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aGate[nCntGate].bUse == true && g_aGate[nCntGate].state != GATESTATE_STOP && g_aGate[nCntGate].state != GATESTATE_OPEN)
		{ // オブジェクトが使用されている場合

			// 落下状態にする
			g_aGate[nCntGate].state = GATESTATE_LAND;
		}
	}
}

//======================================================================================================================
//	ゲートの全開け処理
//======================================================================================================================
void AllOpenGate(void)
{
	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aGate[nCntGate].bUse == true && g_aGate[nCntGate].state != GATESTATE_FLY)
		{ // オブジェクトが使用されている場合

			// 開放状態にする
			g_aGate[nCntGate].state = GATESTATE_OPEN;
		}
	}
}

//======================================================================================================================
// ゲートの注意処理
//======================================================================================================================
void GateAttention(Gate *pGate)
{
	if (GetTime() <= GATE_ATTEN_CNT
		&& pGate->atten.bUse == false)
	{ // 注意時間以内に入っているかつ、注意状態になっていなかった場合

		// 使用していた場合
		pGate->atten.bUse = true;
	}

	if (pGate->atten.bUse == true)
	{ // ゲートの注意情報を使用していた場合

		// 注意カウントを加算する
		pGate->atten.nAttenCounter++;

		if (pGate->atten.nAttenCounter % GATE_ATTEN_COL_CNT == 0)
		{ // 注意カウントが一定数カウントに達した場合

			// 状態を変える
			pGate->atten.state = (ATTENTIONSTATE)((pGate->atten.state + 1) % ATTENTIONSTATE_MAX);
		}
	}
}

//======================================================================================================================
//	オブジェクトの取得処理
//======================================================================================================================
Gate *GetGateData(void)
{
	// オブジェクトの情報の先頭アドレスを返す
	return &g_aGate[0];
}

//======================================================================================================================
//	脱出の情報の取得処理
//======================================================================================================================
Exit GetExit(void)
{
	// 脱出の情報を返す
	return g_exit;
}

//======================================================================================================================
//	ゲートの当たり判定の取得処理
//======================================================================================================================
Collision *GetCollGate(void)
{
	// ゲートの当たり判定の情報の先頭アドレスを返す
	return &g_aCollGate[0];
}

#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================
//**********************************************************************************************************************
//	オブジェクトの総数取得処理
//**********************************************************************************************************************
int GetNumGate(void)
{
	// 変数を宣言
	int nNumGate = 0;	// オブジェクトの総数の確認用

	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aGate[nCntGate].bUse == true)
		{ // オブジェクトが使用されている場合

			// カウンターを加算
			nNumGate++;
		}
	}

	// 変数の値を返す
	return nNumGate;	// オブジェクトの総数
}
#endif