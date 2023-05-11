//===========================================
//
//オブジェクトのメイン処理[EditBillboard.cpp]
//Author 小原立暉
//
//===========================================
#ifdef _DEBUG	// デバッグ処理

#include "main.h"
#include "EditBillboard.h"
#include "input.h"
#include "camera.h"
#include "Shadow.h"
#include "billboard.h"
#include "EditObject.h"
#include "Editmain.h"
#include "player.h"

// マクロ定義
#define INIT_RADIUS						(30.0f)		// 半径の初期値
#define EDITBB_DELETE_RANGE				(30.0f)		// ビルボード消去の範囲
#define EDITBB_MOVE						(16.0f)		// 通常の移動量
#define EDITBB_ADJUSTMENT_MOVE			(4.0f)		// 調整用の移動量
#define EDITBB_SCALING					(1.0f)		// 拡大縮小率
#define EDITBB_COL_CHANGE_CNT			(5)			// 色が変わるカウント
#define EDITBB_COL_CONVERSION			(0.01f)		// 色の変化量
#define EDITBB_UPDOWN_MOVE				(6.0f)		// 上下移動の移動量
#define EDITBB_UPDOWN_ADJUSTMENT_MOVE	(2.0f)		// 調整用の上下移動の移動量

// プロトタイプ宣言
void TypeChangeEditBillboard(void);		// 種類変更処理
void MoveEditBillboard(void);			// 移動処理
void SetEditBillboard(D3DXVECTOR3 rot);	// ビルボードの設定処理
void DeleteEditBillboard(void);			// ビルボードの削除処理
void ScaleBillboardX(void);				// ビルボードの拡大縮小処理(X軸)
void ScaleBillboardY(void);				// ビルボードの拡大縮小処理(Y軸)
void ScaleBillboard(void);				// ビルボードの拡大縮小処理
void ResetEditBillboard(void);			// ビルボードの情報リセット処理
void CustomBillboardColor(void);		// ビルボードの色のエディット処理
void CustomBillboardAnim(void);			// ビルボードのアニメーションカスタム処理
void ReplayBillboard(void);				// ビルボードのアニメーション再生処理
void UpDownEditBillboard(void);			// ビルボードの上下移動処理
void CustomShadowBillboard(void);		// 影のカスタム処理
void PlayerPosSetEditBillboard(void);	// ビルボードのプレイヤー位置移動

// グローバル変数
EditBillboard g_EditBillboard;	// ビルボードの情報
int g_nStyleBillboard;			// スタイルの変数

// テクスチャファイル名
const char *c_apTexturenameEditBillboard[BILLBOARD_MAX] =
{
	"data/TEXTURE/explosion000.png",	// 爆発
	"data/TEXTURE/bullet000.png",		// 弾
};

//========================================
//ビルボードの初期化処理
//========================================
void InitEditBillboard(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	{ // エディットビルボードの初期化

		// スタイルを初期化する
		g_nStyleBillboard = EDITSTYLE_BILLBOARD;

		// 基本情報を初期化
		g_EditBillboard.pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置を初期化する
		g_EditBillboard.col		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色を初期化する
		g_EditBillboard.Radius	= D3DXVECTOR2(INIT_RADIUS, INIT_RADIUS);	// 幅を初期化する

		g_EditBillboard.nSetNumber	= -1;				// 番号を初期化する
		g_EditBillboard.nType		= BILLBOARD_EXPL;	// 種類を初期化する
		g_EditBillboard.nColorCount = 0;				// 色変えカウンターを初期化する
		g_EditBillboard.bUse		= false;			// 使用しない
		g_EditBillboard.bAnimReplay	= false;			// アニメーションを再生しない
		g_EditBillboard.bShadow		= true;				// 影を付ける

		// アニメーション情報を初期化
		g_EditBillboard.EditAnim.bAnim			= false;	// アニメーションしない
		g_EditBillboard.EditAnim.nAnimCounter	= 1;		// アニメーションカウンターを初期化する
		g_EditBillboard.EditAnim.nAnimPattern	= 1;		// アニメーションパターンを初期化する
		g_EditBillboard.EditAnim.nReplayCount	= 0;		// 再生カウントを初期化する
		g_EditBillboard.EditAnim.nReplayPattern	= 0;		// 再生パターンを初期化する
		g_EditBillboard.EditAnim.TexPattern		= 1.0f;		// テクスチャパターンを初期化する

		// ポリゴン情報を初期化
		g_EditBillboard.VtxBuff = NULL;	// バッファをNULLにする

		for (int nCntBill = 0; nCntBill < BILLBOARD_MAX; nCntBill++)
		{
			// テクスチャを初期化する
			g_EditBillboard.Texture[nCntBill] = NULL;

			// 下地のテクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				c_apTexturenameEditBillboard[nCntBill],
				&g_EditBillboard.Texture[nCntBill]);
		}
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_EditBillboard.VtxBuff,
		NULL);

	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = g_EditBillboard.col;
	pVtx[1].col = g_EditBillboard.col;
	pVtx[2].col = g_EditBillboard.col;
	pVtx[3].col = g_EditBillboard.col;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_EditBillboard.VtxBuff->Unlock();
}

//========================================
//ビルボードの終了処理
//========================================
void UninitEditBillboard(void)
{
	for (int nCntBill = 0; nCntBill < BILLBOARD_MAX; nCntBill++)
	{
		// テクスチャの破棄
		if (g_EditBillboard.Texture[nCntBill] != NULL)
		{
			g_EditBillboard.Texture[nCntBill]->Release();
			g_EditBillboard.Texture[nCntBill] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_EditBillboard.VtxBuff != NULL)
	{
		g_EditBillboard.VtxBuff->Release();
		g_EditBillboard.VtxBuff = NULL;
	}
}

//========================================
//ビルボードの更新処理
//========================================
void UpdateEditBillboard(void)
{
	Billboard *pBillboard = GetBillboard();			//ビルボードの情報を取得する
	Camera *pCamera = GetCamera(CAMERATYPE_MAIN);	//カメラの情報を取得する

	g_nStyleBillboard = GetStyle();					//スタイルを取得する

	if (g_nStyleBillboard == EDITSTYLE_BILLBOARD)
	{//ビルボード設置モードだった場合
		for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++, pBillboard++)
		{
			if (pBillboard->bUse == false && g_EditBillboard.bUse == false)
			{//手が加えられていないビルボードだった場合
				//ビルボードを設定状態にする
				g_EditBillboard.bUse = true;

				//番号を初期化する
				g_EditBillboard.nSetNumber = nCntBillboard;

				break;							//抜け出す
			}
		}
	}
	else
	{//ビルボード設置モードじゃなかった場合
		g_EditBillboard.bUse = false;
	}

	//種類変更処理
	TypeChangeEditBillboard();

	//移動処理
	MoveEditBillboard();

	//ビルボードの削除処理
	DeleteEditBillboard();

	//ビルボードの設定処理
	SetEditBillboard(pCamera->rot);

	//ビルボードの拡大縮小処理(X軸)
	ScaleBillboardX();

	//ビルボードの拡大縮小処理(Y軸)
	ScaleBillboardY();

	//ビルボードの拡大縮小処理
	ScaleBillboard();

	//ビルボードの色のエディット処理
	CustomBillboardColor();

	//ビルボードのアニメーションカスタム処理
	CustomBillboardAnim();

	//ビルボードのアニメーション再生処理
	ReplayBillboard();

	//ビルボードの情報リセット処理
	ResetEditBillboard();

	//影のカスタム処理
	CustomShadowBillboard();

	//ビルボードの上下移動処理
	UpDownEditBillboard();

	//ビルボードのプレイヤー位置移動
	PlayerPosSetEditBillboard();
}

//=====================================
//ビルボードの描画処理
//=====================================
void DrawEditBillboard(void)
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

	if (g_EditBillboard.bUse == true)
	{//使用していた場合
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_EditBillboard.mtx);

		//ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//ポリゴンをカメラに向けて正面に向ける
		D3DXMatrixInverse(&g_EditBillboard.mtx, NULL, &mtxView);

		//逆行列を求める
		g_EditBillboard.mtx._41 = 0.0f;
		g_EditBillboard.mtx._42 = 0.0f;
		g_EditBillboard.mtx._43 = 0.0f;

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_EditBillboard.pos.x, g_EditBillboard.pos.y, g_EditBillboard.pos.z);
		D3DXMatrixMultiply(&g_EditBillboard.mtx, &g_EditBillboard.mtx, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_EditBillboard.mtx);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0,
			g_EditBillboard.VtxBuff,							//頂点バッファへのポインタ
			0,
			sizeof(VERTEX_3D));									//頂点情報構造体のサイズ

		//テクスチャの設定
		pDevice->SetTexture(0, g_EditBillboard.Texture[g_EditBillboard.nType]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//プリミティブの種類
			0,													//描画する最初の頂点インデックス
			2);													//描画するプリミティブ数
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

//=======================================
//ビルボードの消去
//=======================================
void DeleteEditBillboard(void)
{
	Billboard *Billboard = GetBillboard();		//ビルボードの情報を取得する

	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++, Billboard++)
	{
		if (Billboard->bUse == true)
		{//オブジェクトが使用されていた場合
			if (g_EditBillboard.pos.x >= Billboard->pos.x - EDITBB_DELETE_RANGE &&
				g_EditBillboard.pos.x <= Billboard->pos.x + EDITBB_DELETE_RANGE &&
				g_EditBillboard.pos.z >= Billboard->pos.z - EDITBB_DELETE_RANGE &&
				g_EditBillboard.pos.z <= Billboard->pos.z + EDITBB_DELETE_RANGE &&
				g_EditBillboard.bUse == true)
			{//範囲内に入った場合
				//削除対象状態にする
				Billboard->State = BILLBOARDSTATE_DELETETARGET;

				if (GetKeyboardTrigger(DIK_9) == true)
				{//9キーを押した場合

					//使用していない
					Billboard->bUse = false;
				}
			}
			else
			{//範囲外だった場合
				//使用状態にする
				Billboard->State = BILLBOARDSTATE_USE;
			}
		}
	}
}

//=======================================
//種類変更処理
//=======================================
void TypeChangeEditBillboard(void)
{
	if (g_nStyleBillboard == EDITSTYLE_BILLBOARD)
	{//ビルボードエディットモードだった場合
		if (GetKeyboardTrigger(DIK_1) == true)
		{//1キーを押した場合
			//設定オブジェクトの種類を選択する
			g_EditBillboard.nType = (g_EditBillboard.nType + 1) % BILLBOARD_MAX;
		}
	}
}

//=======================================
//移動処理
//=======================================
void MoveEditBillboard(void)
{
	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{//左シフトキーが押されていた場合
		//処理を抜ける
		return;
	}

	if (GetKeyboardPress(DIK_LCONTROL) == true)
	{//左コントロールキーを押していた場合
		if (g_nStyleBillboard == EDITSTYLE_BILLBOARD)
		{//ビルボード設置モードだった場合
			if (GetKeyboardTrigger(DIK_W) == true)
			{//Wキーを押した場合
				//位置を奥に進める
				g_EditBillboard.pos.z += EDITBB_ADJUSTMENT_MOVE;
			}

			if (GetKeyboardTrigger(DIK_S) == true)
			{//Sキーを押した場合
				//位置を手前に進める
				g_EditBillboard.pos.z -= EDITBB_ADJUSTMENT_MOVE;
			}

			if (GetKeyboardTrigger(DIK_A) == true)
			{//Aキーを押した場合
				//位置を左に進める
				g_EditBillboard.pos.x -= EDITBB_ADJUSTMENT_MOVE;
			}

			if (GetKeyboardTrigger(DIK_D) == true)
			{//Dキーを押した場合
				//位置を右に進める
				g_EditBillboard.pos.x += EDITBB_ADJUSTMENT_MOVE;
			}
		}
	}
	else
	{//左コントロールキーを押していない場合
		if (g_nStyleBillboard == EDITSTYLE_BILLBOARD)
		{//ビルボード設置モードだった場合
			if (GetKeyboardPress(DIK_W) == true)
			{//Wキーを押した場合
				//位置を奥に進める
				g_EditBillboard.pos.z += EDITBB_MOVE;
			}

			if (GetKeyboardPress(DIK_S) == true)
			{//Sキーを押した場合
				//位置を手前に進める
				g_EditBillboard.pos.z -= EDITBB_MOVE;
			}

			if (GetKeyboardPress(DIK_A) == true)
			{//Aキーを押した場合
				//位置を左に進める
				g_EditBillboard.pos.x -= EDITBB_MOVE;
			}

			if (GetKeyboardPress(DIK_D) == true)
			{//Dキーを押した場合
				//位置を右に進める
				g_EditBillboard.pos.x += EDITBB_MOVE;
			}
		}
	}
}

//=======================================
//オブジェクトの設定処理
//=======================================
void SetEditBillboard(D3DXVECTOR3 rot)
{
	if (g_nStyleBillboard == EDITSTYLE_BILLBOARD)
	{//ビルボード設置モードだった場合
		if (GetKeyboardTrigger(DIK_0) == true)
		{//0キーを押した場合
			//ビルボードの設定処理
			SetBillboard(rot, g_EditBillboard.pos, g_EditBillboard.nType, g_EditBillboard.Radius, g_EditBillboard.col
				, g_EditBillboard.EditAnim.nAnimCounter, g_EditBillboard.EditAnim.nAnimPattern, g_EditBillboard.EditAnim.bAnim, g_EditBillboard.bShadow);

			//番号を初期化する
			g_EditBillboard.nSetNumber = -1;
		}
	}
}

//=======================================
//ビルボードの拡大縮小処理(X軸)
//=======================================
void ScaleBillboardX(void)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nStyleBillboard == EDITSTYLE_BILLBOARD)
	{//ビルボード設置モードだった場合
		if (GetKeyboardPress(DIK_U) == true)
		{//Uキーを押した場合
			//X軸を拡大する
			g_EditBillboard.Radius.x += EDITBB_SCALING;
		}
		else if (GetKeyboardPress(DIK_J) == true)
		{//Jキーを押した場合
			//X軸を縮小する
			g_EditBillboard.Radius.x -= EDITBB_SCALING;
		}

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);
	}

	//頂点バッファをアンロックする
	g_EditBillboard.VtxBuff->Unlock();
}

//=======================================
//ビルボードの拡大縮小処理(Y軸)
//=======================================
void ScaleBillboardY(void)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nStyleBillboard == EDITSTYLE_BILLBOARD)
	{//ビルボード設置モードだった場合
		if (GetKeyboardPress(DIK_I) == true)
		{//Iキーを押した場合
			//Y軸を拡大する
			g_EditBillboard.Radius.y += EDITBB_SCALING;
		}
		else if (GetKeyboardPress(DIK_K) == true)
		{//Kキーを押した場合
			//Y軸を縮小する
			g_EditBillboard.Radius.y -= EDITBB_SCALING;
		}

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);
	}

	//頂点バッファをアンロックする
	g_EditBillboard.VtxBuff->Unlock();
}

//=======================================
//ビルボードの拡大縮小処理
//=======================================
void ScaleBillboard(void)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nStyleBillboard == EDITSTYLE_BILLBOARD)
	{//ビルボード設置モードだった場合
		if (GetKeyboardPress(DIK_4) == true)
		{//4キーを押した場合
			//拡大する
			g_EditBillboard.Radius.x += EDITBB_SCALING;
			g_EditBillboard.Radius.y += EDITBB_SCALING;
		}
		else if (GetKeyboardPress(DIK_5) == true)
		{//5キーを押した場合
			//縮小する
			g_EditBillboard.Radius.x -= EDITBB_SCALING;
			g_EditBillboard.Radius.y -= EDITBB_SCALING;
		}

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);
	}

	//頂点バッファをアンロックする
	g_EditBillboard.VtxBuff->Unlock();
}

//=======================================
//オブジェクトの情報リセット処理
//=======================================
void ResetEditBillboard(void)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nStyleBillboard == EDITSTYLE_BILLBOARD)
	{//ビルボード設置モードだった場合
		if (GetKeyboardTrigger(DIK_3) == true)
		{//3キーを押した場合
			//拡大率を初期化する
			g_EditBillboard.Radius = D3DXVECTOR2(INIT_RADIUS, INIT_RADIUS);

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);
		}
	}

	//頂点バッファをアンロックする
	g_EditBillboard.VtxBuff->Unlock();
}

//=======================================
//エディットビルボードの取得処理
//=======================================
EditBillboard *GetEditBillboard(void)
{
	//エディットビルボードの情報を返す
	return &g_EditBillboard;
}

//=======================================
//ビルボードの色のエディット処理
//=======================================
void CustomBillboardColor(void)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nStyleBillboard == EDITSTYLE_BILLBOARD)
	{//ビルボード設置状態だった場合
		if (GetKeyboardPress(DIK_LSHIFT) == true)
		{//左シフトキーを押した場合
			if (GetKeyboardPress(DIK_R) == true)
			{//Rキーを押した場合
				//色カウントを加算する
				g_EditBillboard.nColorCount++;

				if (g_EditBillboard.nColorCount % EDITBB_COL_CHANGE_CNT == 0)
				{//一定時間経過したら
					//赤色を強くする
					g_EditBillboard.col.r += EDITBB_COL_CONVERSION;

					if (g_EditBillboard.col.r > 1.0f)
					{//赤色が1.0fを超えたら
						//赤色を0.0fにする
						g_EditBillboard.col.r = 0.0f;
					}
				}
			}
			else if (GetKeyboardPress(DIK_F) == true)
			{//Fキーを押した場合
				//色カウントを加算する
				g_EditBillboard.nColorCount++;

				if (g_EditBillboard.nColorCount % EDITBB_COL_CHANGE_CNT == 0)
				{//一定時間経過したら
					//赤色を強くする
					g_EditBillboard.col.r -= EDITBB_COL_CONVERSION;

					if (g_EditBillboard.col.r < 0.0f)
					{//赤色が0.0fを超えたら
						//赤色を1.0fにする
						g_EditBillboard.col.r = 1.0f;
					}
				}
			}

			if (GetKeyboardPress(DIK_T) == true)
			{//Tキーを押した場合
				//色カウントを加算する
				g_EditBillboard.nColorCount++;

				if (g_EditBillboard.nColorCount % EDITBB_COL_CHANGE_CNT == 0)
				{//一定時間経過したら
					//緑色を強くする
					g_EditBillboard.col.g += EDITBB_COL_CONVERSION;

					if (g_EditBillboard.col.g > 1.0f)
					{//緑色が1.0fを超えたら
						//緑色を0.0fにする
						g_EditBillboard.col.g = 0.0f;
					}
				}
			}
			else if (GetKeyboardPress(DIK_G) == true)
			{//Gキーを押した場合
				//色カウントを加算する
				g_EditBillboard.nColorCount++;

				if (g_EditBillboard.nColorCount % EDITBB_COL_CHANGE_CNT == 0)
				{//一定時間経過したら
					//緑色を強くする
					g_EditBillboard.col.g -= EDITBB_COL_CONVERSION;

					if (g_EditBillboard.col.g < 0.0f)
					{//緑色が0.0fを超えたら
						//緑色を1.0fにする
						g_EditBillboard.col.g = 1.0f;
					}
				}
			}

			if (GetKeyboardPress(DIK_Y) == true)
			{//Yキーを押した場合
				//色カウントを加算する
				g_EditBillboard.nColorCount++;

				if (g_EditBillboard.nColorCount % EDITBB_COL_CHANGE_CNT == 0)
				{//一定時間経過したら
					//青色を強くする
					g_EditBillboard.col.b += EDITBB_COL_CONVERSION;

					if (g_EditBillboard.col.b > 1.0f)
					{//青色が1.0fを超えたら
						//青色を0.0fにする
						g_EditBillboard.col.b = 0.0f;
					}
				}
			}
			else if (GetKeyboardPress(DIK_H) == true)
			{//Gキーを押した場合
				//色カウントを加算する
				g_EditBillboard.nColorCount++;

				if (g_EditBillboard.nColorCount % EDITBB_COL_CHANGE_CNT == 0)
				{//一定時間経過したら
					//青色を強くする
					g_EditBillboard.col.b -= EDITBB_COL_CONVERSION;

					if (g_EditBillboard.col.b < 0.0f)
					{//青色が0.0fを超えたら
						//青色を1.0fにする
						g_EditBillboard.col.b = 1.0f;
					}
				}
			}

			if (GetKeyboardTrigger(DIK_V) == true)
			{//Vキーを押した場合
				//赤色を1.0fにする
				g_EditBillboard.col.r = 1.0f;
			}

			if (GetKeyboardTrigger(DIK_B) == true)
			{//Bキーを押した場合
				//緑色を1.0fにする
				g_EditBillboard.col.g = 1.0f;
			}

			if (GetKeyboardTrigger(DIK_N) == true)
			{//Nキーを押した場合
				//青色を1.0fにする
				g_EditBillboard.col.b = 1.0f;
			}
		}

		//頂点カラーの設定
		pVtx[0].col = g_EditBillboard.col;
		pVtx[1].col = g_EditBillboard.col;
		pVtx[2].col = g_EditBillboard.col;
		pVtx[3].col = g_EditBillboard.col;
	}

	//頂点バッファをアンロックする
	g_EditBillboard.VtxBuff->Unlock();
}

//=======================================
//ビルボードのアニメーションカスタム処理
//=======================================
void CustomBillboardAnim(void)
{
	if (GetKeyboardTrigger(DIK_7) == true)
	{//7キーを押した場合
		//アニメーションの有無を切り替える
		g_EditBillboard.EditAnim.bAnim = g_EditBillboard.EditAnim.bAnim ? false : true;
	}

	if (GetKeyboardTrigger(DIK_8) == true)
	{//8キーを押した場合
		//アニメーションの再生を切り替える
		g_EditBillboard.bAnimReplay = g_EditBillboard.bAnimReplay ? false : true;
	}

	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{//左SHIFTキーを押していた場合
		if (GetKeyboardTrigger(DIK_UP) == true)
		{//上キーを押した場合
			//パターンを加算する
			g_EditBillboard.EditAnim.nAnimPattern++;
		}
		else if (GetKeyboardTrigger(DIK_DOWN) == true)
		{//下キーを押した場合
			//パターンを減算する
			g_EditBillboard.EditAnim.nAnimPattern--;
		}

		if (g_EditBillboard.EditAnim.nAnimPattern < 1)
		{//アニメーションパターンが1未満になった場合
			//アニメーションパターンを1にする
			g_EditBillboard.EditAnim.nAnimPattern = 1;
		}

		if (GetKeyboardTrigger(DIK_RIGHT) == true)
		{//右キーを押した場合
			//アニメカウンターを加算する
			g_EditBillboard.EditAnim.nAnimCounter++;
		}
		else if (GetKeyboardTrigger(DIK_LEFT) == true)
		{//右キーを押した場合
			//アニメカウンターを減算する
			g_EditBillboard.EditAnim.nAnimCounter--;
		}

		if (g_EditBillboard.EditAnim.nAnimCounter < 1)
		{//カウンターが0未満になった場合
			//カウンターを0にする
			g_EditBillboard.EditAnim.nAnimCounter = 1;
		}
	}
}

//=======================================
//ビルボードのアニメーション再生処理
//=======================================
void ReplayBillboard(void)
{
	//パターン数を設定する
	g_EditBillboard.EditAnim.TexPattern = (float)(1.0f / g_EditBillboard.EditAnim.nAnimPattern);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (g_EditBillboard.EditAnim.bAnim == true)
	{//アニメーションする場合
		if (g_EditBillboard.bAnimReplay == true)
		{//再生がONになっている場合
			//再生カウンターを加算する
			g_EditBillboard.EditAnim.nReplayCount++;

			if (g_EditBillboard.EditAnim.nReplayCount % g_EditBillboard.EditAnim.nAnimCounter == 0)
			{//再生カウントが設定されていたカウントに届いた場合
				//パターンを加算する
				g_EditBillboard.EditAnim.nReplayPattern = (g_EditBillboard.EditAnim.nReplayPattern + 1) % g_EditBillboard.EditAnim.nAnimPattern;
			}
		}
		else
		{//再生がOFFになっている
			//再生カウントをリセットする
			g_EditBillboard.EditAnim.nReplayCount = 0;

			//再生パターンをリセットする
			g_EditBillboard.EditAnim.nReplayPattern = 0;
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(g_EditBillboard.EditAnim.TexPattern * g_EditBillboard.EditAnim.nReplayPattern, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_EditBillboard.EditAnim.TexPattern * g_EditBillboard.EditAnim.nReplayPattern + (g_EditBillboard.EditAnim.TexPattern), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_EditBillboard.EditAnim.TexPattern * g_EditBillboard.EditAnim.nReplayPattern, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_EditBillboard.EditAnim.TexPattern * g_EditBillboard.EditAnim.nReplayPattern + (g_EditBillboard.EditAnim.TexPattern), 1.0f);
	}
	else
	{//アニメーションしていない場合
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをアンロックする
	g_EditBillboard.VtxBuff->Unlock();
}

//=======================================
//ビルボードの上下移動処理
//=======================================
void UpDownEditBillboard(void)
{
	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{//左SHIFTキーを押している場合
		if (GetKeyboardPress(DIK_LCONTROL) == true)
		{//左コントロールキーを押していた場合
			if (GetKeyboardTrigger(DIK_W) == true)
			{//Wキーを押している場合
				//位置を奥に進める
				g_EditBillboard.pos.y += EDITBB_UPDOWN_ADJUSTMENT_MOVE;
			}

			if (GetKeyboardTrigger(DIK_S) == true)
			{//Sキーを押している場合
				//位置を手前に進める
				g_EditBillboard.pos.y -= EDITBB_UPDOWN_ADJUSTMENT_MOVE;
			}
		}
		else
		{//左コントロールキーを押していない場合
			if (GetKeyboardPress(DIK_W) == true)
			{//Wキーを押している場合
				//位置を奥に進める
				g_EditBillboard.pos.y += EDITBB_UPDOWN_MOVE;
			}

			if (GetKeyboardPress(DIK_S) == true)
			{//Sキーを押している場合
				//位置を手前に進める
				g_EditBillboard.pos.y -= EDITBB_UPDOWN_MOVE;
			}
		}
	}

	if (GetKeyboardTrigger(DIK_A) == true || GetKeyboardTrigger(DIK_D) == true)
	{//AキーかDキーを押した場合
		//地面に戻す
		g_EditBillboard.pos.y = 0.0f;
	}
}

//=======================================
//影のカスタム処理
//=======================================
void CustomShadowBillboard(void)
{
	if (GetKeyboardTrigger(DIK_2) == true)
	{//2キーを押した場合
		//影を切り替える
		g_EditBillboard.bShadow = g_EditBillboard.bShadow ? false : true;
	}
}

//=======================================
//ビルボードのプレイヤー位置移動
//=======================================
void PlayerPosSetEditBillboard(void)
{
	// ポインタを宣言
	Player *pPlayer = GetPlayer();	// プレイヤーの情報

	if (GetKeyboardTrigger(DIK_LMENU) == true)
	{ // 左の ALTキーが押された場合

		// プレイヤーの位置に位置を移動
		g_EditBillboard.pos = pPlayer->pos;
	}
}

#endif