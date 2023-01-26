#ifdef _DEBUG	// デバッグ処理
//===========================================
//
//オブジェクトのメイン処理[EditCollision.cpp]
//Author 小原立暉
//
//===========================================
#include "EditCollision.h"
#include "EditObject.h"
#include "Editmain.h"
#include "input.h"
#include "object.h"

//プロトタイプ宣言
void SelectChangeEdit(void);		// 選択変更処理
void NumChangeEdit(void);			// 判定数変更処理
void CollisionMoveEdit(void);		// 移動処理
void CollisionRotationEdit(void);	// 回転処理
void CollisionScaleObjectX(void);	// オブジェクトの拡大縮小処理(X軸)
void CollisionScaleObjectY(void);	// オブジェクトの拡大縮小処理(Y軸)
void CollisionScaleObjectZ(void);	// オブジェクトの拡大縮小処理(Z軸)
void CollisionScaleObject(void);	// オブジェクトの拡大縮小処理
void CollisionResetEdit(void);		// オブジェクトの情報リセット処理
void CollisionUpDownEdit(void);		// オブジェクトの上下移動処理

//グローバル変数
EditCollision g_EditCollision;		// エディット当たり判定の情報

//==========================================
//モデルの初期化処理
//==========================================
void InitEditCollision(void)
{
	//ポインタを宣言
	Collision *pCollision = GetCollision();

	// 基本情報の初期化
	g_EditCollision.rot         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
	g_EditCollision.stateRot    = ROTSTATE_0;								// 向き状態
	g_EditCollision.modelData   = GetModelData(MODELTYPE_EDIT_COLLISION);	// モデル情報
	g_EditCollision.nType       = GetEditObject()->nType;					// オブジェクトの種類
	g_EditCollision.nSelectColl = 0;										// 選択中の当たり判定

	// 位置情報の初期化
	for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
	{ // 当たり判定の最大数分繰り返す

		g_EditCollision.pos[nCntColl] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	}

	// エディット当たり判定の情報を初期化
	g_EditCollision.collision = *pCollision;
}

//========================================
//モデルの終了処理
//========================================
void UninitEditCollision(void)
{

}

//========================================
//モデルの更新処理
//========================================
void UpdateEditCollision(void)
{
	if (GetStyle() == EDITSTYLE_OBJECT)
	{ // オブジェクト設置モードだった場合

		if (GetEditObject()->Collisiontype.Collisiontype == COLLISIONTYPE_CREATE
		/*||  GetEditObject()->Collisiontype.Collisiontype == COLLISIONTYPE_ONLY*/)
		{//当たり判定の種類が作成関連の種類の場合

			if (GetCollisionStyle() == COLLISIONSTYLE_COLLISION)
			{ // 当たり判定スタイルが当たり判定変更状態の場合

				//選択変更処理
				SelectChangeEdit();

				//判定数変更処理
				NumChangeEdit();

				//回転処理
				CollisionRotationEdit();

				//移動処理
				CollisionMoveEdit();

				//オブジェクトの拡大縮小処理(X軸)
				CollisionScaleObjectX();

				//オブジェクトの拡大縮小処理(Y軸)
				CollisionScaleObjectY();

				//オブジェクトの拡大縮小処理(Z軸)
				CollisionScaleObjectZ();

				//オブジェクトの拡大縮小処理
				CollisionScaleObject();

				//オブジェクトの情報リセット処理
				CollisionResetEdit();

				//オブジェクトの上下移動処理
				CollisionUpDownEdit();
			}
		}
	}
}

//=====================================
//モデルの描画処理
//=====================================
void DrawEditCollision(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;		// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;							// マテリアルデータへのポインタ
	D3DXMATERIAL blueMat;						// マテリアルデータ (青)


	if (GetEditObject()->Collisiontype.Collisiontype == COLLISIONTYPE_CREATE
	/*||  GetEditObject()->Collisiontype.Collisiontype == COLLISIONTYPE_ONLY*/)
	{//当たり判定の種類が作成関連の種類の場合

		for (int nCntColl = 0; nCntColl < g_EditCollision.collision.nNumColl; nCntColl++)
		{ // 当たり判定の最大数分繰り返す

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_EditCollision.mtxWorld);

			//拡大率を反映
			D3DXMatrixScaling(&mtxScale, g_EditCollision.collision.scale[nCntColl].x, g_EditCollision.collision.scale[nCntColl].y, g_EditCollision.collision.scale[nCntColl].z);
			D3DXMatrixMultiply(&g_EditCollision.mtxWorld, &g_EditCollision.mtxWorld, &mtxScale);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_EditCollision.rot.y, g_EditCollision.rot.x, g_EditCollision.rot.z);
			D3DXMatrixMultiply(&g_EditCollision.mtxWorld, &g_EditCollision.mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_EditCollision.pos[nCntColl].x, g_EditCollision.pos[nCntColl].y, g_EditCollision.pos[nCntColl].z);
			D3DXMatrixMultiply(&g_EditCollision.mtxWorld, &g_EditCollision.mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_EditCollision.mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_EditCollision.modelData.pBuffMat->GetBufferPointer();;

			for (int nCntMat = 0; nCntMat < (int)g_EditCollision.modelData.dwNumMat; nCntMat++)
			{
				if (g_EditCollision.nSelectColl == nCntColl
				&&  GetCollisionStyle() == COLLISIONSTYLE_COLLISION)
				{ // 現在選択中の当たり判定且つ、当たり判定エディット状態の場合

					// 構造体の要素をクリア
					ZeroMemory(&blueMat, sizeof(D3DXMATERIAL));

					// 拡散光・環境光・自己発光を赤にする
					blueMat.MatD3D.Diffuse  = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f);
					blueMat.MatD3D.Ambient  = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f);

					// マテリアルの設定
					pDevice->SetMaterial(&blueMat.MatD3D);			// 青
				}
				else
				{ // それ以外の当たり判定の場合

					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// 通常
				}

				//テクスチャの設定
				pDevice->SetTexture(0, g_EditCollision.modelData.pTexture[nCntMat]);

				//モデル(パーツ)の描画
				g_EditCollision.modelData.pMesh->DrawSubset(nCntMat);
			}
			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=======================================
//エディットオブジェクトの取得処理
//=======================================
EditCollision *GetEditCollision(void)
{
	//エディットオブジェクトの情報を返す
	return &g_EditCollision;
}

//=======================================
//選択変更処理
//=======================================
void SelectChangeEdit(void)
{
	if (g_EditCollision.nSelectColl >= g_EditCollision.collision.nNumColl)
	{//選択の補正
		g_EditCollision.nSelectColl = g_EditCollision.collision.nNumColl - 1;
	}

	if (GetKeyboardTrigger(DIK_SPACE) == true)
	{//SPACEキーを押した場合
		//選択中の当たり判定を変更する
		g_EditCollision.nSelectColl = (g_EditCollision.nSelectColl + 1) % g_EditCollision.collision.nNumColl;
	}
}

//=======================================
//判定数変更処理
//=======================================
void NumChangeEdit(void)
{
	if (GetKeyboardTrigger(DIK_UP) == true)
	{//↑キーを押した場合
		//当たり判定の使用数を加算する
		g_EditCollision.collision.nNumColl = g_EditCollision.collision.nNumColl % MAX_COLLISION + 1;
	}
	else if (GetKeyboardTrigger(DIK_DOWN) == true)
	{//↓キーを押した場合
		//当たり判定の使用数を減算する
		//g_EditCollision.collision.nNumColl = g_EditCollision.collision.nNumColl % MAX_COLLISION - 1;
	}
}

//=======================================
//移動処理
//=======================================
void CollisionMoveEdit(void)
{
	// ポインタを宣言
	EditObject *pEditObject = GetEditObject();

	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{//左シフトキーが押されていた場合

		//処理を抜ける
		return;
	}

	if (GetKeyboardPress(DIK_LCONTROL) == true)
	{//左コントロールキーを押していた場合

		if (GetKeyboardTrigger(DIK_W) == true)
		{//Wキーを押した場合
			//位置を奥に進める
			g_EditCollision.pos[g_EditCollision.nSelectColl].z += 4.0f;
		}

		if (GetKeyboardTrigger(DIK_S) == true)
		{//Sキーを押した場合
			//位置を手前に進める
			g_EditCollision.pos[g_EditCollision.nSelectColl].z -= 4.0f;
		}

		if (GetKeyboardTrigger(DIK_A) == true)
		{//Aキーを押した場合
			//位置を左に進める
			g_EditCollision.pos[g_EditCollision.nSelectColl].x -= 4.0f;
		}

		if (GetKeyboardTrigger(DIK_D) == true)
		{//Dキーを押した場合
			//位置を右に進める
			g_EditCollision.pos[g_EditCollision.nSelectColl].x += 4.0f;
		}
	}
	else
	{//左コントロールを押していない場合

		if (GetKeyboardPress(DIK_W) == true)
		{//Wキーを押した場合
		 //位置を奥に進める
			g_EditCollision.pos[g_EditCollision.nSelectColl].z += 16.0f;
		}

		if (GetKeyboardPress(DIK_S) == true)
		{//Sキーを押した場合
		 //位置を手前に進める
			g_EditCollision.pos[g_EditCollision.nSelectColl].z -= 16.0f;
		}

		if (GetKeyboardPress(DIK_A) == true)
		{//Aキーを押した場合
		 //位置を左に進める
			g_EditCollision.pos[g_EditCollision.nSelectColl].x -= 16.0f;
		}

		if (GetKeyboardPress(DIK_D) == true)
		{//Dキーを押した場合
		 //位置を右に進める
			g_EditCollision.pos[g_EditCollision.nSelectColl].x += 16.0f;
		}
	}

	//オブジェクトから見た当たり判定の絶対座標ベクトルを設定
	g_EditCollision.collision.vecPos[g_EditCollision.nSelectColl] = pEditObject->pos - g_EditCollision.pos[g_EditCollision.nSelectColl];
}

//=======================================
//回転処理
//=======================================
void CollisionRotationEdit(void)
{
	// ポインタを宣言
	EditObject *pEditObject = GetEditObject();

	if (GetKeyboardTrigger(DIK_Q) == true)
	{//Qキーを押した場合
		//向きを変更
		g_EditCollision.rot.y += D3DX_PI * 0.5f;

		//向き状態を変更
		g_EditCollision.stateRot = (ROTSTATE)((g_EditCollision.stateRot + 1) % ROTSTATE_MAX);

		for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
		{ // 当たり判定の最大数分繰り返す

			//位置ベクトルを90度回転
			g_EditCollision.collision.vecPos[nCntColl] = D3DXVECTOR3(-g_EditCollision.collision.vecPos[nCntColl].z, g_EditCollision.collision.vecPos[nCntColl].y, g_EditCollision.collision.vecPos[nCntColl].x);

			//位置を反映 (ベクトルの方向)
			g_EditCollision.pos[nCntColl] = pEditObject->pos - g_EditCollision.collision.vecPos[nCntColl];
		}
	}
	else if (GetKeyboardTrigger(DIK_E) == true)
	{//Eキーを押した場合
		//向きを変更
		g_EditCollision.rot.y -= D3DX_PI * 0.5f;

		//向き状態を変更
		g_EditCollision.stateRot = (ROTSTATE)((g_EditCollision.stateRot + (ROTSTATE_MAX - 1)) % ROTSTATE_MAX);

		for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
		{ // 当たり判定の最大数分繰り返す

			//位置ベクトルを90度回転
			g_EditCollision.collision.vecPos[nCntColl] = D3DXVECTOR3(g_EditCollision.collision.vecPos[nCntColl].z, g_EditCollision.collision.vecPos[nCntColl].y, -g_EditCollision.collision.vecPos[nCntColl].x);

			//位置を反映 (ベクトルの方向)
			g_EditCollision.pos[nCntColl] = pEditObject->pos - g_EditCollision.collision.vecPos[nCntColl];
		}
	}

	if (g_EditCollision.rot.y > D3DX_PI)
	{//3.14fより大きくなった場合
		//-3.14fに補正する
		g_EditCollision.rot.y -= D3DX_PI * 2;
	}
	else if (g_EditCollision.rot.y < -D3DX_PI)
	{//-3.14fより小さくなった場合
		//3.14fに補正する
		g_EditCollision.rot.y += D3DX_PI * 2;
	}
}

//=======================================
//オブジェクトの拡大縮小処理(X軸)
//=======================================
void CollisionScaleObjectX(void)
{
	// 変数を宣言
	float fScale = 0.0f;

	if (GetKeyboardPress(DIK_U) == true)
	{//Uキーを押した場合
		//拡大量を保存する
		fScale += 0.02f;
	}
	else if (GetKeyboardPress(DIK_J) == true)
	{//Jキーを押した場合
		//縮小量を保存する
		fScale -= 0.02f;
	}

	// 横幅と縦幅を計算
	if (g_EditCollision.stateRot == ROTSTATE_0
	||  g_EditCollision.stateRot == ROTSTATE_180)
	{ // 角度が0度、または180度の場合
		//X軸を拡縮する
		g_EditCollision.collision.scale[g_EditCollision.nSelectColl].x += fScale;
	}
	else
	{ // 角度90度、または270度の場合
		//Z軸を拡縮する
		g_EditCollision.collision.scale[g_EditCollision.nSelectColl].z += fScale;
	}
}

//=======================================
//オブジェクトの拡大縮小処理(Y軸)
//=======================================
void CollisionScaleObjectY(void)
{
	if (GetKeyboardPress(DIK_I) == true)
	{//Iキーを押した場合
		//Y軸を拡大する
		g_EditCollision.collision.scale[g_EditCollision.nSelectColl].y += 0.02f;
	}
	else if (GetKeyboardPress(DIK_K) == true)
	{//Kキーを押した場合
		//Y軸を縮小する
		g_EditCollision.collision.scale[g_EditCollision.nSelectColl].y -= 0.02f;
	}
}

//=======================================
//オブジェクトの拡大縮小処理(Z軸)
//=======================================
void CollisionScaleObjectZ(void)
{
	// 変数を宣言
	float fScale = 0.0f;

	if (GetKeyboardPress(DIK_O) == true)
	{//Oキーを押した場合
		//拡大量を保存する
		fScale += 0.02f;
	}
	else if (GetKeyboardPress(DIK_L) == true)
	{//Lキーを押した場合
		//縮小量を保存する
		fScale -= 0.02f;
	}

	// 横幅と縦幅を計算
	if (g_EditCollision.stateRot == ROTSTATE_0
	||  g_EditCollision.stateRot == ROTSTATE_180)
	{ // 角度が0度、または180度の場合
		//Z軸を拡縮する
		g_EditCollision.collision.scale[g_EditCollision.nSelectColl].z += fScale;
	}
	else
	{ // 角度90度、または270度の場合
		//X軸を拡縮する
		g_EditCollision.collision.scale[g_EditCollision.nSelectColl].x += fScale;
	}
}

//=======================================
//オブジェクトの拡大縮小処理
//=======================================
void CollisionScaleObject(void)
{
	if (GetKeyboardPress(DIK_4) == true)
	{//4キーを押した場合
		//拡大する
		g_EditCollision.collision.scale[g_EditCollision.nSelectColl].x += 0.02f;
		g_EditCollision.collision.scale[g_EditCollision.nSelectColl].y += 0.02f;
		g_EditCollision.collision.scale[g_EditCollision.nSelectColl].z += 0.02f;
	}
	else if (GetKeyboardPress(DIK_5) == true)
	{//5キーを押した場合
		//縮小する
		g_EditCollision.collision.scale[g_EditCollision.nSelectColl].x -= 0.02f;
		g_EditCollision.collision.scale[g_EditCollision.nSelectColl].y -= 0.02f;
		g_EditCollision.collision.scale[g_EditCollision.nSelectColl].z -= 0.02f;
	}
}

//=======================================
//オブジェクトの情報リセット処理
//=======================================
void CollisionResetEdit(void)
{
	// ポインタを宣言
	EditObject *pEditObject = GetEditObject();

	if (GetKeyboardTrigger(DIK_1) == true)
	{//1キーを押した場合
		//位置をオブジェクトに合わせる
		g_EditCollision.pos[g_EditCollision.nSelectColl] = pEditObject->pos;
	}
	if (GetKeyboardTrigger(DIK_2) == true)
	{//2キーを押した場合
		//角度を初期化する
		g_EditCollision.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		for (int nCntRot = g_EditCollision.stateRot; nCntRot > ROTSTATE_0; nCntRot--)
		{ // 向きが0度になるまで繰り返す

			//向き状態を変更
			g_EditCollision.stateRot = (ROTSTATE)((g_EditCollision.stateRot + (ROTSTATE_MAX - 1)) % ROTSTATE_MAX);

			for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
			{ // 当たり判定の最大数分繰り返す

				//位置ベクトルを90度回転
				g_EditCollision.collision.vecPos[nCntColl] = D3DXVECTOR3(g_EditCollision.collision.vecPos[nCntColl].z, g_EditCollision.collision.vecPos[nCntColl].y, -g_EditCollision.collision.vecPos[nCntColl].x);
			}
		}

		for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
		{ // 当たり判定の最大数分繰り返す

			//位置を反映 (ベクトルの方向)
			g_EditCollision.pos[nCntColl] = pEditObject->pos - g_EditCollision.collision.vecPos[nCntColl];
		}
	}
	if (GetKeyboardTrigger(DIK_3) == true)
	{//3キーを押した場合
		//拡大率を初期化する
		g_EditCollision.collision.scale[g_EditCollision.nSelectColl] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}
}

//=======================================
//オブジェクトの上下移動処理
//=======================================
void CollisionUpDownEdit(void)
{
	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{//左SHIFTキーを押している場合
		if (GetKeyboardPress(DIK_LCONTROL) == true)
		{//左コントロールキーを押していた場合
			if (GetKeyboardTrigger(DIK_W) == true)
			{//Wキーを押している場合
				//位置を奥に進める
				g_EditCollision.pos[g_EditCollision.nSelectColl].y += 2.0f;
			}

			if (GetKeyboardTrigger(DIK_S) == true)
			{//Sキーを押している場合
				//位置を手前に進める
				g_EditCollision.pos[g_EditCollision.nSelectColl].y -= 2.0f;
			}
		}
		else
		{//左コントロールキーを押していない場合
			if (GetKeyboardPress(DIK_W) == true)
			{//Wキーを押している場合
			 //位置を奥に進める
				g_EditCollision.pos[g_EditCollision.nSelectColl].y += 6.0f;
			}

			if (GetKeyboardPress(DIK_S) == true)
			{//Sキーを押している場合
			 //位置を手前に進める
				g_EditCollision.pos[g_EditCollision.nSelectColl].y -= 6.0f;
			}
		}

		if (GetKeyboardTrigger(DIK_A) == true || GetKeyboardTrigger(DIK_D) == true)
		{//AキーかDキーを押した場合
			//地面に戻す
			g_EditCollision.pos[g_EditCollision.nSelectColl].y = 0.0f;
		}
	}
}
#endif