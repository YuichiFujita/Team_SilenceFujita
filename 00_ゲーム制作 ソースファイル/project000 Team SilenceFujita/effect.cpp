//======================================================================================================================
//
//	エフェクト処理 [effect.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "effect.h"
#include "player.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_EFFECT		(4096)		// 使用するポリゴン数 (エフェクトの最大数)

#define SPARK_MOVE_Y	(1.0f)		// 火花エフェクトの移動量(Y軸)
#define SPARK_SUB_COL_G	(0.05f)		// 火花エフェクトの緑の減衰係数
#define SPARK_REV_COL_G	(0.5f)		// 火花エフェクトの緑の最低限補正数

#define SPRAY_MOVE_Y	(1.3f)		// 水しぶきエフェクトの移動量(Y軸)

#define BREAK_OBJECT_MOVE_Y	(1.8f)	// オブジェクトの破壊時の煙の重力

#define EXPL_SUB_COL_G	(0.05f)		// 爆発エフェクトの緑の減衰係数
#define EXPL_REV_COL_G	(0.7f)		// 爆発エフェクトの緑の最低限補正数

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTextureEffect[] =		// テクスチャの相対パス
{
	"data\\TEXTURE\\effect000.jpg",	// エフェクトのテクスチャの相対パス
	"data\\TEXTURE\\effect002.tga",	// エフェクトのテクスチャの相対パス(煙)
};

//**********************************************************************************************************************
//	列挙型定義 (TEXTURE_EFFECT)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_EFFECT_NORMAL = 0,		// エフェクト (通常)
	TEXTURE_EFFECT_SMOKE,			// エフェクト（煙）
	TEXTURE_EFFECT_MAX,				// この列挙型の総数
} TEXTURE_EFFECT;

//**********************************************************************************************************************
//	構造体定義 (Effect)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 move;				// 移動量
	D3DXMATRIX  mtxWorld;			// ワールドマトリックス
	D3DXCOLOR   col;				// 色
	int         nLife;				// 寿命
	float       fRadius;			// 半径
	float       fSub;				// 減算量 (半径)
	float       fAlpha;				// 1Fごとの薄くなる量
	EFFECTTYPE	effectType;			// エフェクトの種類
	bool        bUse;				// 使用状況
}Effect;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void EffectSparkUpdate(Effect *pEffect);			// 火花エフェクトの更新処理
void EffectSprayUpdate(Effect *pEffect);			// 水しぶきエフェクトの更新処理
void EffectBreakObjectUpdate(Effect *pEffect);		// オブジェクトの破壊時のエフェクトの更新処理
void EffectExplosionUpdate(Effect *pEffect);		// 爆発系のエフェクトの更新処理

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureEffect[TEXTURE_EFFECT_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;					// 頂点バッファへのポインタ

Effect g_aEffect[MAX_EFFECT];		// エフェクトの情報

//======================================================================================================================
//	エフェクトの初期化処理
//======================================================================================================================
void InitEffect(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ

	for (int nCntEffect = 0; nCntEffect < TEXTURE_EFFECT_MAX; nCntEffect++)
	{ // 使用するテクスチャ数分繰り返す

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, apTextureEffect[nCntEffect], &g_apTextureEffect[nCntEffect]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * 4 * MAX_EFFECT,		// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL
	);

	// エフェクトの情報の初期化
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{ // エフェクトの最大表示数分繰り返す

		g_aEffect[nCntEffect].pos     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_aEffect[nCntEffect].move    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
		g_aEffect[nCntEffect].col     = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 色
		g_aEffect[nCntEffect].nLife   = 0;									// 寿命
		g_aEffect[nCntEffect].fRadius = 0.0f;								// 半径
		g_aEffect[nCntEffect].fSub    = 0.0f;								// 減算量 (半径)
		g_aEffect[nCntEffect].fAlpha  = 0.0f;								// 1Fごとの薄くなる量
		g_aEffect[nCntEffect].effectType = EFFECTTYPE_NONE;					// エフェクトの種類
		g_aEffect[nCntEffect].bUse    = false;								// 使用状況
	}

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{ // エフェクトの最大表示数分繰り返す

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//======================================================================================================================
//	エフェクトの終了処理
//======================================================================================================================
void UninitEffect(void)
{
	// テクスチャの破棄
	for (int nCntEffect = 0; nCntEffect < TEXTURE_EFFECT_MAX; nCntEffect++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTextureEffect[nCntEffect] != NULL)
		{ // 変数 (g_apTextureEffect) がNULLではない場合

			g_apTextureEffect[nCntEffect]->Release();
			g_apTextureEffect[nCntEffect] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{ // 変数 (g_pVtxBuffEffect) がNULLではない場合

		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//======================================================================================================================
//	エフェクトの更新処理
//======================================================================================================================
void UpdateEffect(void)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{ // エフェクトの最大表示数分繰り返す

		if (g_aEffect[nCntEffect].bUse == true)
		{ // エフェクトが使用されている場合

			switch (g_aEffect[nCntEffect].effectType)
			{
			case EFFECTTYPE_SPARK:		// 火花

				// 火花エフェクトの更新処理
				EffectSparkUpdate(&g_aEffect[nCntEffect]);

				// 処理から抜ける
				break;

			case EFFECTTYPE_RAINSPRAY:	// 雨の水しぶき

				// 水しぶきエフェクトの更新処理
				EffectSprayUpdate(&g_aEffect[nCntEffect]);

				break;					// 抜け出す

			case EFFECTTYPE_PLAY_SMOKE:	// プレイヤーの黒煙

				// プレイヤーの移動量を加算する
				g_aEffect[nCntEffect].pos.x += (GetPlayer()->pos.x - GetPlayer()->oldPos.x);
				g_aEffect[nCntEffect].pos.z += (GetPlayer()->pos.z - GetPlayer()->oldPos.z);

				break;					// 抜け出す

			case EFFECTTYPE_BREAKOBJECT:	// オブジェクトの破壊時の煙

				// オブジェクトの破壊時のエフェクトの更新処理
				EffectBreakObjectUpdate(&g_aEffect[nCntEffect]);

				break;					// 抜け出す

			case EFFECTTYPE_EXPLOSION:		// 爆発

				// 爆発系のエフェクトの更新処理
				EffectExplosionUpdate(&g_aEffect[nCntEffect]);

				break;					// 抜け出す

			default:					// その他

				// 特に無し

				// 処理から抜ける
				break;
			}

			// 移動の更新
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

			// 構造体の要素を減算する
			g_aEffect[nCntEffect].nLife--;									// 寿命を減算
			g_aEffect[nCntEffect].fRadius -= g_aEffect[nCntEffect].fSub;	// 半径を減算
			g_aEffect[nCntEffect].col.a   -= g_aEffect[nCntEffect].fAlpha;	// α値を減算

			// 半径の補正
			if (g_aEffect[nCntEffect].fRadius < 0.0f)
			{ // 半径が 0.0f より小さい場合

				// 半径に 0.0f を代入
				g_aEffect[nCntEffect].fRadius = 0.0f;
			}

			// 透明度の補正
			if (g_aEffect[nCntEffect].col.a < 0.0f)
			{ // α値が 0.0f より小さい場合

				// α値に 0.0f を代入
				g_aEffect[nCntEffect].col.a = 0.0f;
			}

			if (g_aEffect[nCntEffect].nLife <= 0)
			{ // 寿命を迎えた場合

				// 使用していない状態にする
				g_aEffect[nCntEffect].bUse = false;
			}

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;
		}

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//======================================================================================================================
//	エフェクトの描画処理
//======================================================================================================================
void DrawEffect(void)
{
	// 変数を宣言
	D3DXMATRIX mtxTrans;						// 計算用マトリックス
	D3DXMATRIX mtxView;							// ビューマトリックス取得用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Zバッファ更新の有効 / 無効の設定

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{ // エフェクトの最大表示数分繰り返す

		if (g_aEffect[nCntEffect].bUse == true)
		{ // エフェクトが使用されている場合

			if (g_aEffect[nCntEffect].effectType != EFFECTTYPE_PLAY_SMOKE)
			{ // エフェクトのタイプが煙系以外だった場合

				// αブレンディングを加算合成に設定
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);

			// ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aEffect[nCntEffect].mtxWorld, NULL, &mtxView);	// 逆行列を求める
			g_aEffect[nCntEffect].mtxWorld._41 = 0.0f;
			g_aEffect[nCntEffect].mtxWorld._42 = 0.0f;
			g_aEffect[nCntEffect].mtxWorld._43 = 0.0f;

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			switch (g_aEffect[nCntEffect].effectType)
			{
			case EFFECTTYPE_PLAY_SMOKE:		// プレイヤーの黒煙

				// テクスチャの設定
				pDevice->SetTexture(0, g_apTextureEffect[TEXTURE_EFFECT_SMOKE]);

				break;				// 抜け出す

			case EFFECTTYPE_BREAKOBJECT:	// オブジェクトの破壊時

				// テクスチャの設定
				pDevice->SetTexture(0, g_apTextureEffect[TEXTURE_EFFECT_SMOKE]);

				break;				//抜け出す

			default:

				// テクスチャの設定
				pDevice->SetTexture(0, g_apTextureEffect[TEXTURE_EFFECT_NORMAL]);

				break;
			}

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}

		// αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Zバッファ更新の有効 / 無効の設定

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//======================================================================================================================
//	エフェクトの設定処理
//======================================================================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fRadius, float fSub,EFFECTTYPE effectType)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{ // エフェクトの最大表示数分繰り返す

		if (g_aEffect[nCntEffect].bUse == false)
		{ // エフェクトが使用されていない場合

			// 引数を代入
			g_aEffect[nCntEffect].pos     = pos;		// 位置
			g_aEffect[nCntEffect].move    = move;		// 移動量
			g_aEffect[nCntEffect].col     = col;		// 色
			g_aEffect[nCntEffect].nLife   = nLife;		// 寿命
			g_aEffect[nCntEffect].fRadius = fRadius;	// 半径
			g_aEffect[nCntEffect].fSub    = fSub;		// 減算量 (半径)
			g_aEffect[nCntEffect].effectType = effectType;		// エフェクトの種類

			// 1F ごとの薄くなる量を設定
			g_aEffect[nCntEffect].fAlpha = 1.0f / nLife;

			// 使用している状態にする
			g_aEffect[nCntEffect].bUse = true;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			// 処理を抜ける
			break;
		}

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//======================================================================================================================
//	エフェクトの削除
//======================================================================================================================
void AllFalseEffect(void)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{ // エフェクトの最大表示数分繰り返す

		// 使用していない状態にする
		g_aEffect[nCntEffect].bUse = false;
	}
}

//======================================================================================================================
// 火花エフェクトの更新処理
//======================================================================================================================
void EffectSparkUpdate(Effect *pEffect)
{
	// 下に移動量を足していく
	pEffect->move.y -= SPARK_MOVE_Y;

	// 火花を赤くしていく
	pEffect->col.g -= SPARK_SUB_COL_G;

	if (pEffect->col.g <= SPARK_REV_COL_G)
	{ // 一定の値以下になった場合
		// G値を補正する
		pEffect->col.g = SPARK_REV_COL_G;
	}
}

//======================================================================================================================
// 水しぶきエフェクトの更新処理
//======================================================================================================================
void EffectSprayUpdate(Effect *pEffect)
{
	// 下に移動量を足していく
	pEffect->move.y -= SPRAY_MOVE_Y;
}

//======================================================================================================================
// オブジェクトの破壊時のエフェクトの更新処理
//======================================================================================================================
void EffectBreakObjectUpdate(Effect *pEffect)
{
	// 下に移動量を足していく
	pEffect->move.y -= BREAK_OBJECT_MOVE_Y;

	if (pEffect->pos.y <= 0.0f)
	{ // エフェクトが地面を超えた場合

		// エフェクトの位置を補正する
		pEffect->pos.y = 0.0f;

		// 使用しない
		pEffect->bUse = false;
	}
}

//======================================================================================================================
// 爆発系のエフェクトの更新処理
//======================================================================================================================
void EffectExplosionUpdate(Effect *pEffect)
{
	if (pEffect->col.g > EXPL_REV_COL_G)
	{ // 一定の値を超過した場合

		// G値を補正する
		pEffect->col.g = EXPL_REV_COL_G;
	}
	else
	{ // 一定の値以下の場合
		// 火花を赤くしていく
		pEffect->col.g += EXPL_SUB_COL_G;
	}
}

#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================
//**********************************************************************************************************************
//	エフェクトの総数取得処理
//**********************************************************************************************************************
int GetNumEffect(void)
{
	// 変数を宣言
	int nNumEffect = 0;	// エフェクトの総数の確認用

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{ // エフェクトの最大表示数分繰り返す

		if (g_aEffect[nCntEffect].bUse == true)
		{ // エフェクトが使用されている場合

			// カウンターを加算
			nNumEffect++;
		}
	}

	// 変数の値を返す
	return nNumEffect;	// エフェクトの総数
}
#endif