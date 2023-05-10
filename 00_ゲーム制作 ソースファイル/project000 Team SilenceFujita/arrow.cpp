//===========================================
//
//矢印のメイン処理[Arrow.cpp]
//Author 小原立暉
//
//===========================================
#include "main.h"
#include "game.h"

#include "arrow.h"
#include "player.h"

//-------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------
#define ARROW_TEXTURE		"data/TEXTURE/PlayArrow.png"			// 矢印のテクスチャ
#define ARROW_RADIUS		(D3DXVECTOR3(360.0f, 0.0f, 210.0f))		// 矢印の半径
#define ARROW_SHIFT			(450.0f)								// ずらす幅

//-------------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// グローバル変数
//-------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureArrow = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffArrow = NULL;	// 頂点バッファへのポインタ

Arrow g_arrow;				// 矢印の情報

//==========================================
// 矢印の初期化処理
//==========================================
void InitArrow(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 下地のテクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		ARROW_TEXTURE,
		&g_pTextureArrow);

	// 情報の初期化
	g_arrow.pos  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	g_arrow.rot	 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	g_arrow.bUse = false;								// 使用状況

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffArrow,
		NULL);

	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	g_pVtxBuffArrow->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-ARROW_RADIUS.x, 0.0f, +ARROW_RADIUS.z);
	pVtx[1].pos = D3DXVECTOR3(+ARROW_RADIUS.x, 0.0f, +ARROW_RADIUS.z);
	pVtx[2].pos = D3DXVECTOR3(-ARROW_RADIUS.x, 0.0f, -ARROW_RADIUS.z);
	pVtx[3].pos = D3DXVECTOR3(+ARROW_RADIUS.x, 0.0f, -ARROW_RADIUS.z);

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffArrow->Unlock();
}

//========================================
// 矢印の終了処理
//========================================
void UninitArrow(void)
{
	// テクスチャの破棄
	if (g_pTextureArrow != NULL)
	{
		g_pTextureArrow->Release();
		g_pTextureArrow = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffArrow != NULL)
	{
		g_pVtxBuffArrow->Release();
		g_pVtxBuffArrow = NULL;
	}
}

//========================================
// 矢印の更新処理
//========================================
void UpdateArrow(void)
{
	Player *pPlayer = GetPlayer();

	if (g_arrow.bUse == true)
	{ // 使用されていた場合

		// 向きを設定する
		g_arrow.rot.y = pPlayer->rot.y;

		// 位置を設定する
		g_arrow.pos.x = pPlayer->pos.x + sinf(pPlayer->rot.y) * ARROW_SHIFT;
		g_arrow.pos.z = pPlayer->pos.z + cosf(pPlayer->rot.y) * ARROW_SHIFT;
	}
}

//=====================================
// 矢印の描画処理
//=====================================
void DrawArrow(void)
{
	// 変数を宣言
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);					//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						//Zテストの有効/無効設定

	if (g_arrow.bUse == true)
	{ // メッシュフィールドが使用されている場合

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_arrow.mtx);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_arrow.rot.y, g_arrow.rot.x, g_arrow.rot.z);
		D3DXMatrixMultiply(&g_arrow.mtx, &g_arrow.mtx, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_arrow.pos.x, g_arrow.pos.y, g_arrow.pos.z);
		D3DXMatrixMultiply(&g_arrow.mtx, &g_arrow.mtx, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_arrow.mtx);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffArrow, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureArrow);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			// プリミティブの種類
			0,												// 描画する最初の頂点インデックス
			2);												// 描画するプリミティブ数
	}

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				//Zテストの有効/無効設定
}

//======================================
// 矢印の設定処理
//======================================
void SetArrow(bool bUse)
{
	if (bUse == true)
	{ // 使用する場合

		// 情報を設定する
		g_arrow.pos = GetPlayer()->pos;		// 位置
		g_arrow.rot = GetPlayer()->rot;		// 向き
		g_arrow.bUse = true;				// 使用状況
	}
	else
	{ // 使用しない場合

		if (g_arrow.bUse == true)
		{ // 使用している場合

			// 使用状況を設定する
			g_arrow.bUse = false;
		}
	}
}