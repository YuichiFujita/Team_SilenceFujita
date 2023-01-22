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

//プロトタイプ宣言
void CollisionMoveEdit(void);				// 移動処理
void CollisionRotationEdit(void);			// 回転処理
void CollisionScaleObjectX(void);			// オブジェクトの拡大縮小処理(X軸)
void CollisionScaleObjectY(void);			// オブジェクトの拡大縮小処理(Y軸)
void CollisionScaleObjectZ(void);			// オブジェクトの拡大縮小処理(Z軸)
void CollisionScaleObject(void);			// オブジェクトの拡大縮小処理
void CollisionResetEdit(void);				// オブジェクトの情報リセット処理
void CollisionUpDownEditCollision(void);	// オブジェクトの上下移動処理

//グローバル変数
EditCollision g_EditCollision;	// エディット当たり判定の情報

//==========================================
//モデルの初期化処理
//==========================================
void InitEditCollision(void)
{
	// エディット当たり判定の情報の初期化
	g_EditCollision.collision.pos      = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	g_EditCollision.collision.vecPos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置ベクトル
	g_EditCollision.collision.rot      = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	g_EditCollision.collision.stateRot = ROTSTATE_0;					// 向き状態
	g_EditCollision.collision.scale    = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// 拡大率
	g_EditCollision.collision.fWidth   = 0.0f;							// 横幅
	g_EditCollision.collision.fHeight  = 0.0f;							// 縦幅

	g_EditCollision.modelData = GetModelData(MODELTYPE_EDIT_COLLISION);	// モデル情報
	g_EditCollision.nType     = GetEditObject()->nType;					// オブジェクトの種類

#if 0
	//エディットオブジェクトの位置を初期化する
	g_EditCollision.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//エディットオブジェクトの向きを初期化する
	g_EditCollision.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//エディットオブジェクトの拡大率を初期化する
	g_EditCollision.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	//エディットオブジェクトの番号を初期化する
	g_EditCollision.nSetNumber = -1;

	//使用しない
	g_EditCollision.bUse = false;

	//エディットオブジェクトの種類を初期化する
	g_EditCollision.nType = MODELTYPE_OBJECT_TREE;

	//モデルの基本情報
	g_EditCollision.modelData = GetModelData(g_EditCollision.nType + FROM_OBJECT);

	//選択中のマテリアルの番号を初期化する
	g_EditCollision.nCntMaterial = 0;

	//色を変えるカウントを初期化する
	g_EditCollision.nColorCount = 0;

	//壊れない
	g_EditCollision.Break.Breaktype = BREAKTYPE_NONE;

	//影無し
	g_EditCollision.Shadowtype.Shadowtype = SHADOWTYPE_NONE;

	//当たり判定あり
	g_EditCollision.Collisiontype.Collisiontype = COLLISIONTYPE_ON;
	
	//スタイルを設定する
	g_nStyleObject = EDITSTYLE_OBJECT;
#endif
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
				CollisionUpDownEditCollision();
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

	if (GetEditObject()->Collisiontype.Collisiontype == COLLISIONTYPE_CREATE
	/*||  GetEditObject()->Collisiontype.Collisiontype == COLLISIONTYPE_ONLY*/)
	{//当たり判定の種類が作成関連の種類の場合

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_EditCollision.mtxWorld);

		//拡大率を反映
		D3DXMatrixScaling(&mtxScale, g_EditCollision.collision.scale.x, g_EditCollision.collision.scale.y, g_EditCollision.collision.scale.z);
		D3DXMatrixMultiply(&g_EditCollision.mtxWorld, &g_EditCollision.mtxWorld, &mtxScale);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_EditCollision.collision.rot.y, g_EditCollision.collision.rot.x, g_EditCollision.collision.rot.z);
		D3DXMatrixMultiply(&g_EditCollision.mtxWorld, &g_EditCollision.mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_EditCollision.collision.pos.x, g_EditCollision.collision.pos.y, g_EditCollision.collision.pos.z);
		D3DXMatrixMultiply(&g_EditCollision.mtxWorld, &g_EditCollision.mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_EditCollision.mtxWorld);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_EditCollision.modelData.pBuffMat->GetBufferPointer();;

		for (int nCntMat = 0; nCntMat < (int)g_EditCollision.modelData.dwNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// 通常

			//テクスチャの設定
			pDevice->SetTexture(0, g_EditCollision.modelData.pTexture[nCntMat]);

			//モデル(パーツ)の描画
			g_EditCollision.modelData.pMesh->DrawSubset(nCntMat);
		}
		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
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
			g_EditCollision.collision.pos.z += 4.0f;
		}

		if (GetKeyboardTrigger(DIK_S) == true)
		{//Sキーを押した場合
			//位置を手前に進める
			g_EditCollision.collision.pos.z -= 4.0f;
		}

		if (GetKeyboardTrigger(DIK_A) == true)
		{//Aキーを押した場合
			//位置を左に進める
			g_EditCollision.collision.pos.x -= 4.0f;
		}

		if (GetKeyboardTrigger(DIK_D) == true)
		{//Dキーを押した場合
			//位置を右に進める
			g_EditCollision.collision.pos.x += 4.0f;
		}
	}
	else
	{//左コントロールを押していない場合

		if (GetKeyboardPress(DIK_W) == true)
		{//Wキーを押した場合
		 //位置を奥に進める
			g_EditCollision.collision.pos.z += 16.0f;
		}

		if (GetKeyboardPress(DIK_S) == true)
		{//Sキーを押した場合
		 //位置を手前に進める
			g_EditCollision.collision.pos.z -= 16.0f;
		}

		if (GetKeyboardPress(DIK_A) == true)
		{//Aキーを押した場合
		 //位置を左に進める
			g_EditCollision.collision.pos.x -= 16.0f;
		}

		if (GetKeyboardPress(DIK_D) == true)
		{//Dキーを押した場合
		 //位置を右に進める
			g_EditCollision.collision.pos.x += 16.0f;
		}
	}

	//オブジェクトから見た当たり判定の絶対座標ベクトルを設定
	g_EditCollision.collision.vecPos = pEditObject->pos - g_EditCollision.collision.pos;
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
		g_EditCollision.collision.rot.y += D3DXToRadian(90);

		//向き状態を変更
		g_EditCollision.collision.stateRot = (ROTSTATE)((g_EditCollision.collision.stateRot + 1) % ROTSTATE_MAX);

		//位置ベクトルを90度回転
		g_EditCollision.collision.vecPos = D3DXVECTOR3(-g_EditCollision.collision.vecPos.z, g_EditCollision.collision.vecPos.y, g_EditCollision.collision.vecPos.x);

		//位置を反映 (ベクトルの方向)
		g_EditCollision.collision.pos = pEditObject->pos + g_EditCollision.collision.vecPos;
	}
	else if (GetKeyboardTrigger(DIK_E) == true)
	{//Eキーを押した場合
		//向きを変更
		g_EditCollision.collision.rot.y -= D3DXToRadian(90);

		//向き状態を変更
		g_EditCollision.collision.stateRot = (ROTSTATE)((g_EditCollision.collision.stateRot + (ROTSTATE_MAX - 1)) % ROTSTATE_MAX);

		//位置ベクトルを90度回転
		g_EditCollision.collision.vecPos = D3DXVECTOR3(g_EditCollision.collision.vecPos.z, g_EditCollision.collision.vecPos.y, -g_EditCollision.collision.vecPos.x);

		//位置を反映 (ベクトルの方向)
		g_EditCollision.collision.pos = pEditObject->pos + g_EditCollision.collision.vecPos;
	}

	if (g_EditCollision.collision.rot.y > D3DX_PI)
	{//3.14fより大きくなった場合
		//-3.14fに補正する
		g_EditCollision.collision.rot.y -= D3DX_PI * 2;
	}
	else if (g_EditCollision.collision.rot.y < -D3DX_PI)
	{//-3.14fより小さくなった場合
		//3.14fに補正する
		g_EditCollision.collision.rot.y += D3DX_PI * 2;
	}
}

//=======================================
//オブジェクトの拡大縮小処理(X軸)
//=======================================
void CollisionScaleObjectX(void)
{
	if (GetKeyboardPress(DIK_U) == true)
	{//Uキーを押した場合
		//X軸を拡大する
		g_EditCollision.collision.scale.x += 0.02f;
	}
	else if (GetKeyboardPress(DIK_J) == true)
	{//Jキーを押した場合
		//X軸を縮小する
		g_EditCollision.collision.scale.x -= 0.02f;
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
		g_EditCollision.collision.scale.y += 0.02f;
	}
	else if (GetKeyboardPress(DIK_K) == true)
	{//Kキーを押した場合
		//Y軸を縮小する
		g_EditCollision.collision.scale.y -= 0.02f;
	}
}

//=======================================
//オブジェクトの拡大縮小処理(Z軸)
//=======================================
void CollisionScaleObjectZ(void)
{
	if (GetKeyboardPress(DIK_O) == true)
	{//Oキーを押した場合
		//Z軸を拡大する
		g_EditCollision.collision.scale.z += 0.02f;
	}
	else if (GetKeyboardPress(DIK_L) == true)
	{//Lキーを押した場合
		//Z軸を縮小する
		g_EditCollision.collision.scale.z -= 0.02f;
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
		g_EditCollision.collision.scale.x += 0.02f;
		g_EditCollision.collision.scale.y += 0.02f;
		g_EditCollision.collision.scale.z += 0.02f;
	}
	else if (GetKeyboardPress(DIK_5) == true)
	{//5キーを押した場合
		//縮小する
		g_EditCollision.collision.scale.x -= 0.02f;
		g_EditCollision.collision.scale.y -= 0.02f;
		g_EditCollision.collision.scale.z -= 0.02f;
	}
}

//=======================================
//オブジェクトの情報リセット処理
//=======================================
void CollisionResetEdit(void)
{
	// ポインタを宣言
	EditObject *pEditObject = GetEditObject();

	//if (GetKeyboardTrigger(DIK_2) == true)
	//{//2キーを押した場合
	//	//角度を初期化する
	//	g_EditCollision.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//}
	if (GetKeyboardTrigger(DIK_2) == true)
	{//2キーを押した場合
		//位置をオブジェクトに合わせる
		g_EditCollision.collision.pos = pEditObject->pos;
	}
	if (GetKeyboardTrigger(DIK_3) == true)
	{//3キーを押した場合
		//拡大率を初期化する
		g_EditCollision.collision.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}
}

//=======================================
//オブジェクトの上下移動処理
//=======================================
void CollisionUpDownEditCollision(void)
{
	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{//左SHIFTキーを押している場合
		if (GetKeyboardPress(DIK_LCONTROL) == true)
		{//左コントロールキーを押していた場合
			if (GetKeyboardTrigger(DIK_W) == true)
			{//Wキーを押している場合
				//位置を奥に進める
				g_EditCollision.collision.pos.y += 2.0f;
			}

			if (GetKeyboardTrigger(DIK_S) == true)
			{//Sキーを押している場合
				//位置を手前に進める
				g_EditCollision.collision.pos.y -= 2.0f;
			}
		}
		else
		{//左コントロールキーを押していない場合
			if (GetKeyboardPress(DIK_W) == true)
			{//Wキーを押している場合
			 //位置を奥に進める
				g_EditCollision.collision.pos.y += 6.0f;
			}

			if (GetKeyboardPress(DIK_S) == true)
			{//Sキーを押している場合
			 //位置を手前に進める
				g_EditCollision.collision.pos.y -= 6.0f;
			}
		}

		if (GetKeyboardTrigger(DIK_A) == true || GetKeyboardTrigger(DIK_D) == true)
		{//AキーかDキーを押した場合
			//地面に戻す
			g_EditCollision.collision.pos.y = 0.0f;
		}
	}
}
#endif