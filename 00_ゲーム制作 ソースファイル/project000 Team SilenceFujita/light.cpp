//======================================================================================================================
//
//	ライト処理 [light.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "game.h"
#include "light.h"
#include "weather.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_LIGHT		(3)			// 使用するライト数

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
D3DLIGHT9 g_aLight[MAX_LIGHT];		// ライトの情報

//======================================================================================================================
//	ライトの初期化処理
//======================================================================================================================
void InitLight(void)
{
	MODE mode = GetMode();			// モード
	RESULTSTATE ResultState = GetResultState();	// ゲームの状態を取得する

	// 変数配列を宣言
	D3DXCOLOR   aDiffuse[MAX_LIGHT];			// 設定用拡散光カラー
	D3DXVECTOR3 aVecDir[MAX_LIGHT];				// 設定用方向ベクトル
	WEATHERTYPE weather = GetWeather();			// 天気を取得する

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	switch (mode)
	{ // モードごとの処理
	case MODE_TUTORIAL:	// チュートリアル
		
		// 拡散光カラーを設定
		aDiffuse[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		aDiffuse[1] = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
		aDiffuse[2] = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

		// 処理を抜ける
		break;

	case MODE_GAME:		// ゲーム

		switch (weather)
		{ // 天候ごとの処理
		case WEATHERTYPE_SUNNY:		// 晴れ

			// 拡散光カラーを設定
			aDiffuse[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			aDiffuse[1] = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
			aDiffuse[2] = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

			// 処理を抜ける
			break;

		case WEATHERTYPE_RAIN:		// 雨

			// 拡散光カラーを設定
			aDiffuse[0] = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
			aDiffuse[1] = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
			aDiffuse[2] = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

			// 処理を抜ける
			break;

		case WEATHERTYPE_SNOW:		// 雪

			// 拡散光カラーを設定
			aDiffuse[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			aDiffuse[1] = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
			aDiffuse[2] = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

			// 処理を抜ける
			break;

		case WEATHERTYPE_THUNDER:	// 雷雨

			// 拡散光カラーを設定
			aDiffuse[0] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			aDiffuse[1] = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
			aDiffuse[2] = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);

			// 処理を抜ける
			break;
		}

		// 処理を抜ける
		break;

	case MODE_RESULT:	// リザルト

		if (ResultState == RESULTSTATE_CLEAR)
		{ // ゲームクリア状態の場合
	
			// 拡散光カラーを設定
			aDiffuse[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			aDiffuse[1] = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
			aDiffuse[2] = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
		}
		else if (ResultState == RESULTSTATE_TIMEOVER
			 ||  ResultState == RESULTSTATE_LIFEOVER)
		{ // ゲームオーバー状態の場合
	
			// 拡散光カラーを設定
			aDiffuse[0] = D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f);
			aDiffuse[1] = D3DXCOLOR(0.7f, 0.2f, 0.2f, 1.0f);
			aDiffuse[2] = D3DXCOLOR(0.3f, 0.1f, 0.1f, 1.0f);
		}

		// 処理を抜ける
		break;

	case MODE_RANKING:	// ランキング
		
		// 拡散光カラーを設定
		aDiffuse[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		aDiffuse[1] = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
		aDiffuse[2] = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

		// 処理を抜ける
		break;
	}

	// 方向ベクトルを設定
	aVecDir[0] = D3DXVECTOR3( 0.34f, -0.87f,  0.35f);
	aVecDir[1] = D3DXVECTOR3(-0.59f, -0.10f, -0.80f);
	aVecDir[2] = D3DXVECTOR3(-0.22f, -0.74f,  0.63f);

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{ // 使用するライト数分繰り返す

		// 構造体の要素をクリア
		ZeroMemory(&g_aLight[nCntLight], sizeof(D3DLIGHT9));

		// ライトの種類を設定
		g_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		// ライトの拡散光を設定
		g_aLight[nCntLight].Diffuse = aDiffuse[nCntLight];

		// ライトの方向を設定
		D3DXVec3Normalize(&aVecDir[nCntLight], &aVecDir[nCntLight]);	// ベクトルを正規化
		g_aLight[nCntLight].Direction = aVecDir[nCntLight];

		// ライトを設定する
		pDevice->SetLight(nCntLight, &g_aLight[nCntLight]);

		// ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//======================================================================================================================
//	ライトの終了処理
//======================================================================================================================
void UninitLight(void)
{

}

//======================================================================================================================
//	ライトの更新処理
//======================================================================================================================
void UpdateLight(void)
{

}