//======================================================================================================================
//
//	エディットパーティクル処理 [EditParticle.cpp]
//	Author：小原立暉
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "Editmain.h"
#include "EditParticle.h"
#include "EditEffect.h"
#include "input.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************

//**********************************************************************************************************************
//プロトタイプ宣言
//**********************************************************************************************************************
void EditParticleUpdate(void);			// パーティクルの更新
void CustomRandomParticle(void);		// パーティクルのランダムのカスタム
void CustomRandomSpeed(void);			// パーティクルの速度のカスタム

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
EditParticle g_EditParticle;			// パーティクルの情報
int g_nEditStyleParticle;				// エディットスタイル(パーティクル)

//======================================================================================================================
//	エディットパーティクルの初期化処理
//======================================================================================================================
void InitEditParticle(void)
{
	g_EditParticle.pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	g_EditParticle.col			= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
	g_EditParticle.Radius		= D3DXVECTOR3(3.0f, 3.0f, 0.0f);			// 半径
	g_EditParticle.nRandom		= 6.28f;									// ランダムの範囲
	g_EditParticle.nSpawn		= 4;										// エフェクト数
	g_EditParticle.nCountPress	= 0;										// カウントプレス
	g_EditParticle.fAngle		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 角度
	g_EditParticle.bUse			= false;									// 使用状況
	g_EditParticle.move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	g_EditParticle.Speed		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);			// 速度
	g_EditParticle.SpeedRandomX = 1;										// 移動量のランダム(X軸)
	g_EditParticle.SpeedRandomY = 1;										// 移動量のランダム(Y軸)
	g_EditParticle.SpeedRandomZ = 1;										// 移動量のランダム(Z軸)
}

//======================================================================================================================
//	エディットパーティクルの終了処理
//======================================================================================================================
void UninitEditParticle(void)
{

}

//======================================================================================================================
//	エディットパーティクルの更新処理
//======================================================================================================================
void UpdateEditParticle(void)
{
	g_nEditStyleParticle = GetStyle();			//スタイルを取得する
	
	if (g_nEditStyleParticle == EDITSTYLE_PARTICLE)
	{//パーティクルエディットモードだった場合
		// 使用している
		g_EditParticle.bUse = true;
	}
	else
	{//パーティクルエディットモードじゃなかった場合
		// 使用していない
		g_EditParticle.bUse = false;
	}

	if (g_EditParticle.bUse == true)
	{ // エディットパーティクルが使用されている場合
		// パーティクルのランダムのカスタム
		CustomRandomParticle();

		// パーティクルの速度のカスタム
		CustomRandomSpeed();

		// パーティクルの更新
		EditParticleUpdate();
	}
}

//======================================================================================================================
//	エディットパーティクルの描画処理
//======================================================================================================================
void DrawEditParticle(void)
{

}

//======================================================================================================================
// パーティクルの取得処理
//======================================================================================================================
EditParticle *GetEditParticle(void)
{
	//パーティクルの情報を返す
	return &g_EditParticle;
}

//======================================================================================================================
// エディットパーティクルの更新
//======================================================================================================================
void EditParticleUpdate(void)
{
	int nRandomMinus;			// ランダムの引く数

	float AngleDiv;				//角度の割る数

	if (g_EditParticle.nRandom <= 7)
	{//ランダムの値が10以下だった場合
		//割る数を1.0fに設定する
		AngleDiv = 1.0f;
	}
	else if (g_EditParticle.nRandom <= 63)
	{//ランダムの値が100以下だった場合
		//割る数を10.0fに設定する
		AngleDiv = 10.0f;
	}
	else if (g_EditParticle.nRandom <= 629)
	{//ランダムの値が100以下だった場合
		//割る数を100.0fに設定する
		AngleDiv = 100.0f;
	}

	nRandomMinus = (int)(g_EditParticle.nRandom / 2);

	for (int nCntAppear = 0; nCntAppear < g_EditParticle.nSpawn; nCntAppear++)
	{ // パーティクルの 1Fで生成されるエフェクト数分繰り返す
		//角度を設定する
		g_EditParticle.fAngle.x = (float)(rand() % (int)(g_EditParticle.nRandom * 100) - 314) / 100.0f;
		g_EditParticle.fAngle.y = (float)(rand() % (int)(g_EditParticle.nRandom * 100) - 314) / 100.0f;
		g_EditParticle.fAngle.z = (float)(rand() % (int)(g_EditParticle.nRandom * 100) - 314) / 100.0f;

		//移動量を設定する
		g_EditParticle.Speed.x = (float)(rand() % g_EditParticle.SpeedRandomX) / 10.0f;
		g_EditParticle.Speed.y = (float)(rand() % g_EditParticle.SpeedRandomY) / 10.0f;
		g_EditParticle.Speed.z = (float)(rand() % g_EditParticle.SpeedRandomZ) / 10.0f;

		//移動量を設定する
		g_EditParticle.move.x = sinf(g_EditParticle.fAngle.x) * g_EditParticle.Speed.x;
		g_EditParticle.move.y = cosf(g_EditParticle.fAngle.y) * g_EditParticle.Speed.y;
		g_EditParticle.move.z = cosf(g_EditParticle.fAngle.z) * g_EditParticle.Speed.z;

		// ベクトルを正規化
		D3DXVec3Normalize(&g_EditParticle.move, &g_EditParticle.move);

		// エフェクトの設定処理
		SetEditEffect(g_EditParticle.pos,
			g_EditParticle.move,
			g_EditParticle.col,
			40,
			g_EditParticle.Radius);
	}
}
//======================================================================================================================
// パーティクルのランダムのカスタム
//======================================================================================================================
void CustomRandomParticle(void)
{
	if (GetKeyboardPress(DIK_1) == true)
	{// 1キーを押した場合
		// カウントプレスを加算する
		g_EditParticle.nCountPress++;

		if (g_EditParticle.nCountPress % 20 == 0)
		{// カウントが一定の数になった場合
			// ランダムの範囲を増やす
			g_EditParticle.nRandom += 0.04f;

			if (g_EditParticle.nRandom > 6.29)
			{// ランダムが629より超過した場合
			 // ランダムを629に固定する
				g_EditParticle.nRandom = 6.29f;
			}
		}
	}
	else if (GetKeyboardPress(DIK_2) == true)
	{// 2キーを押した場合
		// カウントプレスを加算する
		g_EditParticle.nCountPress++;

		if (g_EditParticle.nCountPress % 20 == 0)
		{// カウントが一定の数になった場合
			// ランダムの範囲を減らす
			g_EditParticle.nRandom -= 0.04f;

			if (g_EditParticle.nRandom < 0.10f)
			{// ランダムが1未満になった場合
			 // ランダムを1に固定する
				g_EditParticle.nRandom = 0.10f;
			}
		}
	}
}

//======================================================================================================================
// パーティクルの速度のカスタム
//======================================================================================================================
void CustomRandomSpeed(void)
{
	if (GetKeyboardPress(DIK_3) == true)
	{// 3キーを押した場合
		// カウントプレスを加算する
		g_EditParticle.nCountPress++;

		if (g_EditParticle.nCountPress % 20 == 0)
		{// カウントが一定の数になった場合
			// 速度のランダム(X軸)を加算する
			g_EditParticle.SpeedRandomX++;

			if (g_EditParticle.SpeedRandomX > 100)
			{// ランダムが100より超過した場合
				// ランダムを100に固定する
				g_EditParticle.SpeedRandomX = 100;
			}
		}
	}
	else if (GetKeyboardPress(DIK_4) == true)
	{// 4キーを押した場合
		// カウントプレスを加算する
		g_EditParticle.nCountPress++;

		if (g_EditParticle.nCountPress % 20 == 0)
		{// カウントが一定の数になった場合
			// 速度のランダム(X軸)を加算する
			g_EditParticle.SpeedRandomY++;

			if (g_EditParticle.SpeedRandomY > 100)
			{// ランダムが100より超過した場合
				// ランダムを100に固定する
				g_EditParticle.SpeedRandomY = 100;
			}
		}
	}
	else if (GetKeyboardPress(DIK_5) == true)
	{// 5キーを押した場合
		// カウントプレスを加算する
		g_EditParticle.nCountPress++;

		if (g_EditParticle.nCountPress % 20 == 0)
		{// カウントが一定の数になった場合
			// 速度のランダム(X軸)を加算する
			g_EditParticle.SpeedRandomZ++;

			if (g_EditParticle.SpeedRandomZ > 100)
			{// ランダムが100より超過した場合
				// ランダムを100に固定する
				g_EditParticle.SpeedRandomZ = 100;
			}
		}
	}
}

#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================
#endif