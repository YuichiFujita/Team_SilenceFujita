//======================================================================================================================
//
//	亡骸処理 [death.cpp]
//	Author：小原立暉
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "shadow.h"
#include "calculation.h"

#include "death.h"
#include "particle.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define DEATH_INIT_POS		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		// 亡骸の初期位置
#define DEATH_INIT_ROT		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		// 亡骸の初期向き
#define DEATH_SMOKE_CNT		(30)								// 黒煙が出るカウント数
#define DEATH_SMOKE_COL		(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f))	// 黒煙の色
#define DEATH_ADD_ROT		(40)								// 向きの追加量

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
Death g_death;				// 亡骸の情報

//======================================================================================================================
//	亡骸の初期化処理
//======================================================================================================================
void InitDeath(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	 // 基本情報の初期化
	g_death.pos = DEATH_INIT_POS;				// 位置
	g_death.rot = DEATH_INIT_ROT;				// 向き
	g_death.nShadowID = NONE_SHADOW;			// 影
	g_death.nSmokeCnt = 0;						// 黒煙のカウント
	g_death.bUse = false;						// 使用状況

	// モデル情報の初期化
	g_death.modelData.dwNumMat = 0;					// マテリアルの数
	g_death.modelData.pTexture = NULL;				// テクスチャへのポインタ
	g_death.modelData.pMesh = NULL;					// メッシュ (頂点情報) へのポインタ
	g_death.modelData.pBuffMat = NULL;				// マテリアルへのポインタ
	g_death.modelData.vtxMin = INIT_VTX_MIN;		// 最小の頂点座標
	g_death.modelData.vtxMax = INIT_VTX_MAX;		// 最大の頂点座標
	g_death.modelData.size = INIT_SIZE;				// 大きさ
	g_death.modelData.fRadius = 0.0f;				// 半径
}

//======================================================================================================================
//	亡骸の終了処理
//======================================================================================================================
void UninitDeath(void)
{

}

//======================================================================================================================
//	亡骸の更新処理
//======================================================================================================================
void UpdateDeath(void)
{
	if (g_death.bUse == true)
	{ // 亡骸が使用されている場合

		// 黒煙のカウントを加算する
		g_death.nSmokeCnt++;

		if (g_death.nSmokeCnt % DEATH_SMOKE_CNT == 0)
		{ // 黒煙カウントが一定数に達した場合

			// パーティクルの設定処理
			SetParticle
			(
				g_death.pos,
				DEATH_SMOKE_COL,
				PARTICLETYPE_PLAY_SMOKE,
				SPAWN_PARTICLE_PLAY_SMOKE,
				3
			);
		}

		// 影の位置設定処理
		SetPositionShadow
		(
			g_death.nShadowID,
			g_death.pos,
			g_death.rot,
			NONE_SCALE
		);
	}
}

//======================================================================================================================
//	亡骸の描画処理
//======================================================================================================================
void DrawDeath(void)
{
	// 変数を宣言
	D3DXMATRIX   mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ

	if (g_death.bUse == true)
	{ // 亡骸が使用されている場合

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_death.mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_death.rot.y, g_death.rot.x, g_death.rot.z);
		D3DXMatrixMultiply(&g_death.mtxWorld, &g_death.mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_death.pos.x, g_death.pos.y, g_death.pos.z);
		D3DXMatrixMultiply(&g_death.mtxWorld, &g_death.mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_death.mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_death.modelData.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_death.modelData.dwNumMat; nCntMat++)
		{ // マテリアルの数分繰り返す

			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// 通常

			// テクスチャの設定
			pDevice->SetTexture(0, g_death.modelData.pTexture[nCntMat]);

			// モデルの描画
			g_death.modelData.pMesh->DrawSubset(nCntMat);
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//======================================================================================================================
//	亡骸の設定処理
//======================================================================================================================
void SetDeath(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (g_death.bUse == false)
	{ // 亡骸が使用されていない場合

		// 変数を代入
		g_death.pos = pos;		// 位置
		g_death.rot = rot;		// 向き
		g_death.rot.y -= D3DXToRadian(DEATH_ADD_ROT);		// 向きを少し傾ける

		// 向きの正規化
		RotNormalize(&g_death.rot.y);

		g_death.nSmokeCnt = 0;	// 黒煙カウント
		g_death.bUse = true;	// 使用している状態にする

		// モデル情報を設定
		g_death.modelData = GetModelData(MODELTYPE_PLAYER_DEATH);	// モデル情報

		// 影のインデックスを設定
		g_death.nShadowID = SetModelShadow
		( // 引数
			g_death.modelData,			// モデル情報
			&g_death.nShadowID,			// 影のインデックス
			&g_death.bUse				// 使用状況
		);

		// 影の位置設定
		SetPositionShadow(g_death.nShadowID, g_death.pos, g_death.rot, NONE_SCALE);
	}
}

#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================

#endif