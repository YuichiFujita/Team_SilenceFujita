//===========================================
//
//ビルボードのメイン処理[Billboard.cpp]
//Author 小原立暉
//
//===========================================
#include"main.h"
#include "game.h"
#include "billboard.h"
#include "input.h"
#include "shadow.h"
#include "meshfield.h"
#include "Editmain.h"

//プロトタイプ宣言
void CollisionWall(Billboard *Billboard);		//壁の当たり判定
void PlayBillboard(BillboardAnim *Anim);		//ビルボードの再生処理

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBillboard[BILLBOARD_MAX] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;	//頂点バッファへのポインタ
Billboard g_aBillboard[MAX_BILLBOARD];				//ビルボードの情報

//テクスチャファイル名
const char *c_apFilenameBillboard[BILLBOARD_MAX] = 
{
	"data/TEXTURE/explosion000.png",				//爆発のテクスチャ
	"data/TEXTURE/bullet000.png",					//弾のテクスチャ
};

//==========================================
//ビルボードの初期化処理
//==========================================
void InitBillboard(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntTexture = 0; nCntTexture < BILLBOARD_MAX; nCntTexture++)
	{//テクスチャの設定
		//下地のテクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameBillboard[nCntTexture],
			&g_pTextureBillboard[nCntTexture]);
	}
	
	for (int nCntSet = 0; nCntSet < MAX_BILLBOARD; nCntSet++)
	{
		//位置の初期化
		g_aBillboard[nCntSet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//向きの初期化
		g_aBillboard[nCntSet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//半径を初期化する
		g_aBillboard[nCntSet].Radius = D3DXVECTOR2(30.0f, 30.0f);

		//影の番号を初期化する
		g_aBillboard[nCntSet].nShadow = -1;

		//アニメカウンターを初期化する
		g_aBillboard[nCntSet].nAnimCounter = 0;

		//アニメパターンNo.を初期化する
		g_aBillboard[nCntSet].nAnimPattern = 0;

		//使用していない
		g_aBillboard[nCntSet].bUse = false;

		//色を初期化する
		g_aBillboard[nCntSet].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//色のコピーを初期化する
		g_aBillboard[nCntSet].colCopy = g_aBillboard[nCntSet].col;

		//状態を初期化する
		g_aBillboard[nCntSet].State = BILLBOARDSTATE_NONE;

		//アニメーションしない
		g_aBillboard[nCntSet].Anim.bAnim = false;

		//アニメカウントを初期化する
		g_aBillboard[nCntSet].Anim.nAnimCounter = 1;

		//アニメパターンを初期化する
		g_aBillboard[nCntSet].Anim.nAnimPattern = 1;

		//再生カウントを初期化する
		g_aBillboard[nCntSet].Anim.nReplayCount = 0;

		//再生パターンを初期化する
		g_aBillboard[nCntSet].Anim.nReplayPattern = 0;

		//テクスチャのパターンを初期化する
		g_aBillboard[nCntSet].Anim.TexPattern = 1.0f;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aBillboard[nCnt].Radius.x, +g_aBillboard[nCnt].Radius.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+g_aBillboard[nCnt].Radius.x, +g_aBillboard[nCnt].Radius.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aBillboard[nCnt].Radius.x, -g_aBillboard[nCnt].Radius.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+g_aBillboard[nCnt].Radius.x, -g_aBillboard[nCnt].Radius.y, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = g_aBillboard[nCnt].col;
		pVtx[1].col = g_aBillboard[nCnt].col;
		pVtx[2].col = g_aBillboard[nCnt].col;
		pVtx[3].col = g_aBillboard[nCnt].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;						//頂点データを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();
}

//========================================
//ビルボードの終了処理
//========================================
void UninitBillboard(void)
{
	for (int nCntBill = 0; nCntBill < BILLBOARD_MAX; nCntBill++)
	{
		//テクスチャの破棄
		if (g_pTextureBillboard[nCntBill] != NULL)
		{
			g_pTextureBillboard[nCntBill]->Release();
			g_pTextureBillboard[nCntBill] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//========================================
//ビルボードの更新処理
//========================================
void UpdateBillboard(void)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		if (g_aBillboard[nCntBill].bUse == true)
		{//使用されていた場合
			switch (g_aBillboard[nCntBill].State)
			{
			case BILLBOARDSTATE_DELETETARGET:			//削除対象の場合
				//赤色に変える
				g_aBillboard[nCntBill].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				break;									//抜け出す

			default:									//上記以外
				//元の色に変える
				g_aBillboard[nCntBill].col = g_aBillboard[nCntBill].colCopy;

				break;									//抜け出す
			}

			if (g_aBillboard[nCntBill].Anim.bAnim == true)
			{//アニメーションする場合
				//ビルボードの再生処理
				PlayBillboard(&g_aBillboard[nCntBill].Anim);

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2((g_aBillboard[nCntBill].Anim.TexPattern * g_aBillboard[nCntBill].Anim.nReplayPattern), 0.0f);
				pVtx[1].tex = D3DXVECTOR2((g_aBillboard[nCntBill].Anim.TexPattern * g_aBillboard[nCntBill].Anim.nReplayPattern) + g_aBillboard[nCntBill].Anim.TexPattern, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((g_aBillboard[nCntBill].Anim.TexPattern * g_aBillboard[nCntBill].Anim.nReplayPattern), 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_aBillboard[nCntBill].Anim.TexPattern * g_aBillboard[nCntBill].Anim.nReplayPattern) + g_aBillboard[nCntBill].Anim.TexPattern, 1.0f);
			}
			else
			{//アニメーションしない場合
				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			//頂点カラーの設定
			pVtx[0].col = g_aBillboard[nCntBill].col;
			pVtx[1].col = g_aBillboard[nCntBill].col;
			pVtx[2].col = g_aBillboard[nCntBill].col;
			pVtx[3].col = g_aBillboard[nCntBill].col;

			//影の位置の設定処理
			SetPositionShadow(g_aBillboard[nCntBill].nShadow, g_aBillboard[nCntBill].pos, NONE_ROT, NONE_SCALE);
		}
		pVtx += 4;					//頂点データを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();
}

//=====================================
//ビルボードの描画処理
//=====================================
void DrawBillboard(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	D3DXMATRIX mtxTrans;			//計算用マトリックス
	D3DXMATRIX mtxView;				//ビューマトリックス取得用

	//ライティングをOFFにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);					//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						//Zテストの有効/無効設定

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);					//アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);				//アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 75);							//アルファテストの参照値設定

	for (int nCntDraw = 0; nCntDraw < MAX_BILLBOARD; nCntDraw++)
	{
		if (g_aBillboard[nCntDraw].bUse == true)
		{//使用していた場合
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBillboard[nCntDraw].mtx);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに向けて正面に向ける
			D3DXMatrixInverse(&g_aBillboard[nCntDraw].mtx, NULL, &mtxView);

			//逆行列を求める
			g_aBillboard[nCntDraw].mtx._41 = 0.0f;
			g_aBillboard[nCntDraw].mtx._42 = 0.0f;
			g_aBillboard[nCntDraw].mtx._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBillboard[nCntDraw].pos.x, g_aBillboard[nCntDraw].pos.y, g_aBillboard[nCntDraw].pos.z);
			D3DXMatrixMultiply(&g_aBillboard[nCntDraw].mtx, &g_aBillboard[nCntDraw].mtx, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCntDraw].mtx);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0,
				g_pVtxBuffBillboard,								//頂点バッファへのポインタ
				0,
				sizeof(VERTEX_3D));									//頂点情報構造体のサイズ

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBillboard[g_aBillboard[nCntDraw].Type]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//プリミティブの種類
				4 * nCntDraw,										//描画する最初の頂点インデックス
				2);													//描画するプリミティブ数
		}
	}

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			//アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		//アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						//アルファテストの参照値設定

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				//Zテストの有効/無効設定

	//ライティングをONにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//======================================
//ビルボードの設定処理
//======================================
void SetBillboard(D3DXVECTOR3 rot, D3DXVECTOR3 pos, int nType, D3DXVECTOR2 Radius, D3DXCOLOR col, int AnimCnt, int AnimPtrn, bool bAnim, bool bShadow)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		if (g_aBillboard[nCntBill].bUse == false)
		{//使用されていなかった場合
			//位置を設定する
			g_aBillboard[nCntBill].pos = pos;

			//向きを設定する
			g_aBillboard[nCntBill].rot = rot;

			//大きさを設定する
			g_aBillboard[nCntBill].Radius = Radius;

			//色を設定する
			g_aBillboard[nCntBill].col = col;

			//色のコピーを設定する
			g_aBillboard[nCntBill].colCopy = g_aBillboard[nCntBill].col;

			//アニメーションするかを設定する
			g_aBillboard[nCntBill].Anim.bAnim = bAnim;

			//アニメーションカウントを設定する
			g_aBillboard[nCntBill].Anim.nAnimCounter = AnimCnt;

			//アニメーションパターンを設定する
			g_aBillboard[nCntBill].Anim.nAnimPattern = AnimPtrn;

			//再生カウントを設定する
			g_aBillboard[nCntBill].Anim.nReplayCount = 0;

			//再生パターンを設定する
			g_aBillboard[nCntBill].Anim.nReplayPattern = 0;

			//テクスチャのパターンを設定する
			g_aBillboard[nCntBill].Anim.TexPattern = (float)(1.0f / g_aBillboard[nCntBill].Anim.nAnimPattern);

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aBillboard[nCntBill].Radius.x, +g_aBillboard[nCntBill].Radius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aBillboard[nCntBill].Radius.x, +g_aBillboard[nCntBill].Radius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aBillboard[nCntBill].Radius.x, -g_aBillboard[nCntBill].Radius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aBillboard[nCntBill].Radius.x, -g_aBillboard[nCntBill].Radius.y, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = g_aBillboard[nCntBill].col;
			pVtx[1].col = g_aBillboard[nCntBill].col;
			pVtx[2].col = g_aBillboard[nCntBill].col;
			pVtx[3].col = g_aBillboard[nCntBill].col;

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((g_aBillboard[nCntBill].Anim.TexPattern * g_aBillboard[nCntBill].Anim.nReplayPattern), 0.0f);
			pVtx[1].tex = D3DXVECTOR2((g_aBillboard[nCntBill].Anim.TexPattern * g_aBillboard[nCntBill].Anim.nReplayPattern) + g_aBillboard[nCntBill].Anim.TexPattern, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((g_aBillboard[nCntBill].Anim.TexPattern * g_aBillboard[nCntBill].Anim.nReplayPattern), 1.0f);
			pVtx[3].tex = D3DXVECTOR2((g_aBillboard[nCntBill].Anim.TexPattern * g_aBillboard[nCntBill].Anim.nReplayPattern) + g_aBillboard[nCntBill].Anim.TexPattern, 1.0f);

			//種類を設定する
			g_aBillboard[nCntBill].Type = nType;

			//アニメカウンターを設定する
			g_aBillboard[nCntBill].nAnimCounter = 0;

			//アニメパターンNo.を設定する
			g_aBillboard[nCntBill].nAnimPattern = 0;

			//ビルボードの状態を設定する
			g_aBillboard[nCntBill].State = BILLBOARDSTATE_USE;

			if (bShadow == true)
			{//影を入れる
				//影を設定する
				g_aBillboard[nCntBill].nShadow = SetCircleShadow(0.5f, 30.0f, &g_aBillboard[nCntBill].nShadow, &g_aBillboard[nCntBill].bUse);
			}
			else
			{//影を入れない
				//影を-1に設定する
				g_aBillboard[nCntBill].nShadow = -1;
			}

			//使用する
			g_aBillboard[nCntBill].bUse = true;

			break;									//抜け出す
		}
		pVtx += 4;									//頂点データを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();
}

//================================
//壁の当たり判定
//================================
void CollisionWall(Billboard *Billboard)
{
	if (Billboard->pos.x >= 300.0f ||
		Billboard->pos.x <= -300.0f ||
		Billboard->pos.z >= 300.0f ||
		Billboard->pos.z <= -300.0f)
	{//弾がステージの外に出た場合
		//爆発のビルボードを設定
		SetBillboard(Billboard->rot, Billboard->pos, BILLBOARD_EXPL, D3DXVECTOR2(10.0f, 10.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 4, 8, true, false);

		//使用していない
		Billboard->bUse = false;
	}
}

//================================
//ビルボードの取得処理
//================================
Billboard *GetBillboard(void)
{
	//ビルボードの情報を返す
	return &g_aBillboard[0];
}

//================================
//ビルボードの再生処理
//================================
void PlayBillboard(BillboardAnim *Anim)
{
	//再生カウンターを加算する
	Anim->nReplayCount += 1;

	if (Anim->nReplayCount % Anim->nAnimCounter == 0)
	{//再生カウントが設定されていたカウントに届いた場合
		//パターンを加算する
		Anim->nReplayPattern = (Anim->nReplayPattern + 1) % Anim->nAnimPattern;
	}
}