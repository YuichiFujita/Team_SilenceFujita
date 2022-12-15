//===========================================
//
//オブジェクトのメイン処理[Object.cpp]
//Author 小原立暉
//
//===========================================
#include "main.h"
#include "Object.h"
#include "Edit.h"
#include "input.h"
#include "camera.h"
#include "Shadow.h"
#include "billboard.h"

//プロトタイプ宣言
void TypeChangeEdit(void);								//種類変更処理
void StyleChangeEdit(void);								//スタイル変更処理
void MoveEdit(float Camerarot);							//移動処理
void RotationEdit(void);								//回転処理
void SetEdit(void);										//オブジェクトの設定処理
void ScaleObjectX(void);								//オブジェクトの拡大縮小処理(X軸)
void ScaleObjectY(void);								//オブジェクトの拡大縮小処理(Y軸)
void ScaleObjectZ(void);								//オブジェクトの拡大縮小処理(Z軸)
void ScaleObject(void);									//オブジェクトの拡大縮小処理
void ResetEdit(void);									//オブジェクトの情報リセット処理

//グローバル変数
EditObject g_EditObject;								//オブジェクトの情報
EditBillboard g_EditBillboard;							//ビルボードの情報
Doll g_DollEditObject[OBJECTTYPE_MAX];					//モデルの基本情報
int g_nStyle;											//スタイルの種類

//Xファイル名
const char *c_apModelnameEditObject[OBJECTTYPE_MAX] =
{
	"data/MODEL/Tree.x",								//木
	"data/MODEL/Rock.x",								//岩
	"data/MODEL/Forest.x",								//森
	"data/MODEL/BigTree.x",								//大木
	"data/MODEL/YellowTree.x",							//黄色い木
	"data/MODEL/Grass.x"								//草
};

//テクスチャファイル名
const char *c_apTexturenameEditBillboard[BILLBOARD_MAX] =
{
	"data/TEXTURE/explosion000.png",					//爆発
	"data/TEXTURE/bullet000.png"						//弾
};

//==========================================
//モデルの初期化処理
//==========================================
void InitEditObject(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	{//エディットオブジェクトの初期化
		//エディットオブジェクトの位置を初期化する
		g_EditObject.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//エディットオブジェクトの向きを初期化する
		g_EditObject.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//エディットオブジェクトの拡大率を初期化する
		g_EditObject.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		//エディットオブジェクトの番号を初期化する
		g_EditObject.nSetNumber = 0;

		//使用しない
		g_EditObject.bUse = false;

		//エディットオブジェクトの種類を初期化する
		g_EditObject.nType = OBJECTTYPE_TREE;

		for (int nCntEdit = 0; nCntEdit < MAX_TEXTURE; nCntEdit++)
		{
			//テクスチャを初期化する
			g_EditObject.pDollData.apTexture[nCntEdit] = NULL;
		}

		//マテリアルの数を0にする
		g_EditObject.pDollData.g_dwNumMat = 0;

		//マテリアルへのポインタをNULLにする
		g_EditObject.pDollData.g_pBuffMat = NULL;

		//メッシュ(頂点情報)へのポインタをNULLにする
		g_EditObject.pDollData.g_pMesh = NULL;
	}

	//スタイルを設定する
	g_nStyle = EDITSTYLE_OBJECT;

	for (int nCntDoll = 0; nCntDoll < OBJECTTYPE_MAX; nCntDoll++)
	{//モデルの初期化
		for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
		{
			//テクスチャを初期化する
			g_DollEditObject[nCntDoll].apTexture[nCnt] = NULL;
		}

		//マテリアルの数を初期化する
		g_DollEditObject[nCntDoll].g_dwNumMat = 0;

		//マテリアルへのポインタを初期化する
		g_DollEditObject[nCntDoll].g_pBuffMat = NULL;

		//メッシュ(頂点情報)へのポインタを初期化する
		g_DollEditObject[nCntDoll].g_pMesh = NULL;
	}

	for (int nCntModel = 0; nCntModel < OBJECTTYPE_MAX; nCntModel++)
	{//モデルの読み込み
		//Xファイルの読み込み
		D3DXLoadMeshFromX(c_apModelnameEditObject[nCntModel],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_DollEditObject[nCntModel].g_pBuffMat,
			NULL,
			&g_DollEditObject[nCntModel].g_dwNumMat,
			&g_DollEditObject[nCntModel].g_pMesh);

		D3DXMATERIAL *pMat;					//マテリアルへのポインタ

		//マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_DollEditObject[nCntModel].g_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_DollEditObject[nCntModel].g_dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//テクスチャファイル名が存在する
			 //このファイル名を使用してテクスチャを読み込む
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_DollEditObject[nCntModel].apTexture[nCntMat]);
			}
		}
	}
}

//========================================
//ビルボードの初期化処理
//========================================
void InitEditBillboard(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	{//エディットビルボードの初期化
		//位置を初期化する
		g_EditBillboard.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//番号を初期化する
		g_EditBillboard.nSetNumber = 0;

		//種類を初期化する
		g_EditBillboard.nType = BILLBOARD_EXPL;

		//使用しない
		g_EditBillboard.bUse = false;

		//幅を初期化する
		g_EditBillboard.Radius = D3DXVECTOR2(30.0f, 30.0f);

		//バッファをNULLにする
		g_EditBillboard.VtxBuff = NULL;

		for (int nCntBill = 0; nCntBill < BILLBOARD_MAX; nCntBill++)
		{
			//テクスチャを初期化する
			g_EditBillboard.Texture[nCntBill] = NULL;
		}
	}

	for (int nCntTexture = 0; nCntTexture < BILLBOARD_MAX; nCntTexture++)
	{//テクスチャの設定
		//下地のテクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apTexturenameEditBillboard[nCntTexture],
			&g_EditBillboard.Texture[nCntTexture]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_EditBillboard.VtxBuff,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);

	//法線ベクトルの設定
	pVtx[0].hor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].hor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].hor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].hor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_EditBillboard.VtxBuff->Unlock();
}

//========================================
//モデルの終了処理
//========================================
void UninitEditObject(void)
{
	for (int nCntDoll = 0; nCntDoll < OBJECTTYPE_MAX; nCntDoll++)
	{
		for (int nCntMat = 0; nCntMat < MAX_TEXTURE; nCntMat++)
		{
			//テクスチャの破棄
			if (g_DollEditObject[nCntDoll].apTexture[nCntMat] != NULL)
			{
				g_DollEditObject[nCntDoll].apTexture[nCntMat]->Release();
				g_DollEditObject[nCntDoll].apTexture[nCntMat] = NULL;
			}
		}

		//メッシュの破棄
		if (g_DollEditObject[nCntDoll].g_pMesh != NULL)
		{
			g_DollEditObject[nCntDoll].g_pMesh->Release();
			g_DollEditObject[nCntDoll].g_pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_DollEditObject[nCntDoll].g_pBuffMat != NULL)
		{
			g_DollEditObject[nCntDoll].g_pBuffMat->Release();
			g_DollEditObject[nCntDoll].g_pBuffMat = NULL;
		}
	}

	for (int nCntBill = 0; nCntBill < BILLBOARD_MAX; nCntBill++)
	{
		//テクスチャの破棄
		if (g_EditBillboard.Texture[nCntBill] != NULL)
		{
			g_EditBillboard.Texture[nCntBill]->Release();
			g_EditBillboard.Texture[nCntBill] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_EditBillboard.VtxBuff != NULL)
	{
		g_EditBillboard.VtxBuff->Release();
		g_EditBillboard.VtxBuff = NULL;
	}
}

//========================================
//モデルの更新処理
//========================================
void UpdateEditObject(void)
{
	Object *pObject = GetObje();					//オブジェクトの情報を取得する
	Billboard *pBillboard = GetBillboard();			//ビルボードの情報を取得する

	D3DXVECTOR3 Camerarot = GetCamera().rot;		//カメラの情報を取得する

	//スタイル変更処理
	StyleChangeEdit();

	if (g_nStyle == EDITSTYLE_OBJECT)
	{//オブジェクト設置モードだった場合
		for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pObject++)
		{
			if (pObject->bUse == false && g_EditObject.bUse == false)
			{//手が加えられていないオブジェクトだった場合
				//オブジェクトを設定状態にする
				g_EditObject.bUse = true;

				//オブジェクトの番号を保存する
				g_EditObject.nSetNumber = nCntObject;

				//モデル情報を読み込む
				g_EditObject.pDollData = g_DollEditObject[g_EditObject.nType];

				break;							//抜け出す
			}
		}

		//位置を設定する
		g_EditBillboard.pos = g_EditObject.pos;
	}
	else if (g_nStyle == EDITSTYLE_BILLBOARD)
	{//ビルボード設置モードだった場合
		for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++, pBillboard++)
		{
			if (pBillboard->bUse == false && g_EditBillboard.bUse == false)
			{//手が加えられていないビルボードだった場合
				//ビルボードを設定状態にする
				g_EditBillboard.bUse = true;

				//ビルボードの番号を保存する
				g_EditBillboard.nSetNumber = nCntBillboard;

				break;							//抜け出す
			}
		}

		//位置を設定する
		g_EditObject.pos = g_EditBillboard.pos;
	}

	//種類変更処理
	TypeChangeEdit();

	//移動処理
	MoveEdit(Camerarot.y);

	//回転処理
	RotationEdit();

	if (g_EditObject.rot.y > D3DX_PI)
	{//3.14fより大きくなった場合
		//-3.14fに補正する
		g_EditObject.rot.y = -D3DX_PI;
	}
	else if (g_EditObject.rot.y < -D3DX_PI)
	{//-3.14fより小さくなった場合
		//3.14fに補正する
		g_EditObject.rot.y = D3DX_PI;
	}

	//オブジェクトの設定処理
	SetEdit();										

	//オブジェクトの拡大縮小処理(X軸)
	ScaleObjectX();								

	//オブジェクトの拡大縮小処理(Y軸)
	ScaleObjectY();

	//オブジェクトの拡大縮小処理(Z軸)
	ScaleObjectZ();

	//オブジェクトの拡大縮小処理
	ScaleObject();

	//オブジェクトの情報リセット処理
	ResetEdit();
}

//=====================================
//モデルの描画処理
//=====================================
void DrawEditObject(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;		//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	if (g_EditObject.bUse == true)
	{//設定中か使用している場合
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_EditObject.mtx);

		//拡大率を反映
		D3DXMatrixScaling(&mtxScale, g_EditObject.scale.x, g_EditObject.scale.y, g_EditObject.scale.z);
		D3DXMatrixMultiply(&g_EditObject.mtx, &g_EditObject.mtx, &mtxScale);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_EditObject.rot.y, g_EditObject.rot.x, g_EditObject.rot.z);
		D3DXMatrixMultiply(&g_EditObject.mtx, &g_EditObject.mtx, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_EditObject.pos.x, g_EditObject.pos.y, g_EditObject.pos.z);
		D3DXMatrixMultiply(&g_EditObject.mtx, &g_EditObject.mtx, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_EditObject.mtx);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_EditObject.pDollData.g_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_EditObject.pDollData.g_dwNumMat; nCntMat++)
		{
			//透明度を設定する
			pMat[nCntMat].MatD3D.Ambient.a = 0.5f;
			pMat[nCntMat].MatD3D.Diffuse.a = 0.5f;

			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_EditObject.pDollData.apTexture[nCntMat]);

			//モデル(パーツ)の描画
			g_EditObject.pDollData.g_pMesh->DrawSubset(nCntMat);
		}
		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
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
}

//=======================================
//エディットオブジェクトの取得処理
//=======================================
EditObject GetEditObject(void)
{
	//エディットオブジェクトの情報を返す
	return g_EditObject;
}

//=======================================
//オブジェクトの消去
//=======================================
void DeleteEditObject(Object *pObject)
{
	if (pObject->bUse == true)
	{//オブジェクトが使用されていた場合
		if (g_EditObject.pos.x >= pObject->pos.x - 50.0f &&
			g_EditObject.pos.x <= pObject->pos.x + 50.0f &&
			g_EditObject.pos.z >= pObject->pos.z - 50.0f &&
			g_EditObject.pos.z <= pObject->pos.z + 50.0f)
		{//範囲内に入った場合
			//削除対象状態にする
			pObject->State = OBJECTSTATE_DELETETARGET;

			if (GetKeyboardTrigger(DIK_9) == true)
			{//9キーを押した場合
				//使用していない
				pObject->bUse = false;

				//影のリセット処理
				ResetShadow(&pObject->nShadow);
			}
		}
		else
		{//範囲外だった場合
			//使用状態にする
			pObject->State = OBJECTSTATE_USE;
		}
	}
}

//=======================================
//種類変更処理
//=======================================
void TypeChangeEdit(void)
{
	if (g_nStyle == EDITSTYLE_OBJECT)
	{//オブジェクト設置モードだった場合
		if (GetKeyboardTrigger(DIK_1) == true)
		{//1キーを押した場合
			//設定オブジェクトの種類を選択する
			g_EditObject.nType = (g_EditObject.nType + 1) % OBJECTTYPE_MAX;

			//設定する
			g_EditObject.pDollData = g_DollEditObject[g_EditObject.nType];
		}
	}
	else if (g_nStyle == EDITSTYLE_BILLBOARD)
	{//ビルボード設置モードだった場合
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
void MoveEdit(float Camerarot)
{
	if (g_nStyle == EDITSTYLE_OBJECT)
	{//オブジェクト設置モードだった場合
		if (GetKeyboardPress(DIK_W) == true)
		{//Wキーを押した場合
		 //位置を奥に進める
			g_EditObject.pos.x += sinf(Camerarot) * 5.0f;
			g_EditObject.pos.z += cosf(Camerarot) * 5.0f;
		}

		if (GetKeyboardPress(DIK_S) == true)
		{//Sキーを押した場合
		 //位置を手前に進める
			g_EditObject.pos.x += -sinf(Camerarot) * 5.0f;
			g_EditObject.pos.z += -cosf(Camerarot) * 5.0f;
		}

		if (GetKeyboardPress(DIK_A) == true)
		{//Aキーを押した場合
		 //位置を左に進める
			g_EditObject.pos.x += sinf(-D3DX_PI * 0.5f - Camerarot) * 5.0f;
			g_EditObject.pos.z += -cosf(-D3DX_PI * 0.5f - Camerarot) * 5.0f;
		}

		if (GetKeyboardPress(DIK_D) == true)
		{//Dキーを押した場合
		 //位置を右に進める
			g_EditObject.pos.x += sinf(D3DX_PI * 0.5f - Camerarot) * 5.0f;
			g_EditObject.pos.z += -cosf(D3DX_PI * 0.5f - Camerarot) * 5.0f;
		}
	}

	else if (g_nStyle == EDITSTYLE_BILLBOARD)
	{//ビルボード設置モードだった場合
		if (GetKeyboardPress(DIK_W) == true)
		{//Wキーを押した場合
			//位置を奥に進める
			g_EditBillboard.pos.x += sinf(Camerarot) * 5.0f;
			g_EditBillboard.pos.z += cosf(Camerarot) * 5.0f;
		}

		if (GetKeyboardPress(DIK_S) == true)
		{//Sキーを押した場合
			//位置を手前に進める
			g_EditBillboard.pos.x += -sinf(Camerarot) * 5.0f;
			g_EditBillboard.pos.z += -cosf(Camerarot) * 5.0f;
		}

		if (GetKeyboardPress(DIK_A) == true)
		{//Aキーを押した場合
			//位置を左に進める
			g_EditBillboard.pos.x += sinf(-D3DX_PI * 0.5f - Camerarot) * 5.0f;
			g_EditBillboard.pos.z += -cosf(-D3DX_PI * 0.5f - Camerarot) * 5.0f;
		}

		if (GetKeyboardPress(DIK_D) == true)
		{//Dキーを押した場合
			//位置を右に進める
			g_EditBillboard.pos.x += sinf(D3DX_PI * 0.5f - Camerarot) * 5.0f;
			g_EditBillboard.pos.z += -cosf(D3DX_PI * 0.5f - Camerarot) * 5.0f;
		}
	}
}

//=======================================
//回転処理
//=======================================
void RotationEdit(void)
{
	if (GetKeyboardPress(DIK_Q) == true)
	{//2キーを押した場合
		//向きを変える
		g_EditObject.rot.y += 0.02f;
	}
	else if (GetKeyboardPress(DIK_E) == true)
	{//3キーを押した場合
		//向きを変える
		g_EditObject.rot.y -= 0.02f;
	}
}

//=======================================
//オブジェクトの設定処理
//=======================================
void SetEdit(void)
{
	if (g_nStyle == EDITSTYLE_OBJECT)
	{//オブジェクト設置モードだった場合
		if (GetKeyboardTrigger(DIK_0) == true)
		{//0キーを押した場合
			//オブジェクトの設定処理
			SetObject(g_EditObject.pos, g_EditObject.rot, g_EditObject.nType, g_EditObject.scale);
		}
	}
	else if (g_nStyle == EDITSTYLE_BILLBOARD)
	{//ビルボード設置モードだった場合
		if (GetKeyboardTrigger(DIK_0) == true)
		{//0キーを押した場合
			//ビルボードの設定処理
			SetBillboard(D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_EditBillboard.pos, g_EditBillboard.nType, g_EditBillboard.Radius);
		}
	}
}

//=======================================
//オブジェクトの拡大縮小処理(X軸)
//=======================================
void ScaleObjectX(void)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nStyle == EDITSTYLE_OBJECT)
	{//オブジェクト設置モードだった場合
		if (GetKeyboardPress(DIK_U) == true)
		{//Uキーを押した場合
			//X軸を拡大する
			g_EditObject.scale.x += 0.02f;
		}
		else if (GetKeyboardPress(DIK_J) == true)
		{//Jキーを押した場合
			//X軸を縮小する
			g_EditObject.scale.x -= 0.02f;
		}
	}
	if (g_nStyle == EDITSTYLE_BILLBOARD)
	{//ビルボード設置モードだった場合
		if (GetKeyboardPress(DIK_U) == true)
		{//Uキーを押した場合
			//X軸を拡大する
			g_EditBillboard.Radius.x += 1.0f;
		}
		else if (GetKeyboardPress(DIK_J) == true)
		{//Jキーを押した場合
			//X軸を縮小する
			g_EditBillboard.Radius.x -= 1.0f;
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
//オブジェクトの拡大縮小処理(Y軸)
//=======================================
void ScaleObjectY(void)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nStyle == EDITSTYLE_OBJECT)
	{//オブジェクト設置モードだった場合
		if (GetKeyboardPress(DIK_I) == true)
		{//Iキーを押した場合
			//Y軸を拡大する
			g_EditObject.scale.y += 0.02f;
		}
		else if (GetKeyboardPress(DIK_K) == true)
		{//Kキーを押した場合
			//Y軸を縮小する
			g_EditObject.scale.y -= 0.02f;
		}
	}
	if (g_nStyle == EDITSTYLE_BILLBOARD)
	{//ビルボード設置モードだった場合
		if (GetKeyboardPress(DIK_I) == true)
		{//Iキーを押した場合
			//Y軸を拡大する
			g_EditBillboard.Radius.y += 1.0f;
		}
		else if (GetKeyboardPress(DIK_K) == true)
		{//Kキーを押した場合
			//Y軸を縮小する
			g_EditBillboard.Radius.y -= 1.0f;
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
//オブジェクトの拡大縮小処理(Z軸)
//=======================================
void ScaleObjectZ(void)
{
	if (GetKeyboardPress(DIK_O) == true)
	{//Oキーを押した場合
		//Z軸を拡大する
		g_EditObject.scale.z += 0.02f;
	}
	else if (GetKeyboardPress(DIK_L) == true)
	{//Lキーを押した場合
		//Z軸を縮小する
		g_EditObject.scale.z -= 0.02f;
	}
}

//=======================================
//オブジェクトの拡大縮小処理
//=======================================
void ScaleObject(void)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nStyle == EDITSTYLE_OBJECT)
	{//オブジェクト設置モードだった場合
		if (GetKeyboardPress(DIK_4) == true)
		{//4キーを押した場合
			//拡大する
			g_EditObject.scale.x += 0.02f;
			g_EditObject.scale.y += 0.02f;
			g_EditObject.scale.z += 0.02f;
		}
		else if (GetKeyboardPress(DIK_5) == true)
		{//5キーを押した場合
			//縮小する
			g_EditObject.scale.x -= 0.02f;
			g_EditObject.scale.y -= 0.02f;
			g_EditObject.scale.z -= 0.02f;
		}
	}
	else if (g_nStyle == EDITSTYLE_BILLBOARD)
	{//ビルボード設置モードだった場合
		if (GetKeyboardPress(DIK_4) == true)
		{//4キーを押した場合
			//拡大する
			g_EditBillboard.Radius.x += 1.0f;
			g_EditBillboard.Radius.y += 1.0f;
		}
		else if (GetKeyboardPress(DIK_5) == true)
		{//5キーを押した場合
			//縮小する
			g_EditBillboard.Radius.x -= 1.0f;
			g_EditBillboard.Radius.y -= 1.0f;
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
//スタイル変更処理
//=======================================
void StyleChangeEdit(void)
{
	if (GetKeyboardTrigger(DIK_6) == true)
	{//6キーを押した場合
		//スタイルを選択する
		g_nStyle = (g_nStyle + 1) % EDITSTYLE_MAX;

		if (g_nStyle == EDITSTYLE_OBJECT)
		{//オブジェクト設置モードだった場合
			//オブジェクトを使用する
			g_EditObject.bUse = true;

			//ビルボードを使用しない
			g_EditBillboard.bUse = false;
		}
		else if (g_nStyle == EDITSTYLE_BILLBOARD)
		{//ビルボード設置モードだった場合
			//ビルボードを使用する
			g_EditBillboard.bUse = true;

			//オブジェクトを使用しない
			g_EditObject.bUse = false;
		}
	}
}

//=======================================
//オブジェクトの情報リセット処理
//=======================================
void ResetEdit(void)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (GetKeyboardTrigger(DIK_2) == true)
	{//2キーを押した場合
		//角度を初期化する
		g_EditObject.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	if (g_nStyle == EDITSTYLE_OBJECT)
	{//オブジェクト設置モードだった場合
		if (GetKeyboardTrigger(DIK_3) == true)
		{//3キーを押した場合
			//拡大率を初期化する
			g_EditObject.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		}
	}
	else if (g_nStyle == EDITSTYLE_BILLBOARD)
	{//ビルボード設置モードだった場合
		if (GetKeyboardTrigger(DIK_3) == true)
		{//3キーを押した場合
			//拡大率を初期化する
			g_EditBillboard.Radius = D3DXVECTOR2(30.0f, 30.0f);

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
EditBillboard GetEditBillboard(void)
{
	//エディットビルボードの情報を返す
	return g_EditBillboard;
}

//=======================================
//エディットスタイルの取得処理
//=======================================
int GetStyle(void)
{
	//エディットスタイルを返す
	return g_nStyle;
}