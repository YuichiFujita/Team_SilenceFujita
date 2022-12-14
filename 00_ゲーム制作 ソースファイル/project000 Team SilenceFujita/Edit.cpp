//===========================================
//
//オブジェクトのメイン処理[Object.cpp]
//Author 小原立暉
//
//===========================================
#include "main.h"
#include "model.h"
#include "Edit.h"
#include "Editmain.h"
#include "input.h"
#include "camera.h"
#include "object.h"
#include "Shadow.h"
#include "EditBillboard.h"
#include "SoundDJ.h"

//プロトタイプ宣言
void TypeChangeEdit(void);								//種類変更処理
void MoveEdit(float Camerarot);							//移動処理
void RotationEdit(void);								//回転処理
void SetEdit(void);										//オブジェクトの設定処理
void DeleteEditObject(void);							//オブジェクトの消去
void ScaleObjectX(void);								//オブジェクトの拡大縮小処理(X軸)
void ScaleObjectY(void);								//オブジェクトの拡大縮小処理(Y軸)
void ScaleObjectZ(void);								//オブジェクトの拡大縮小処理(Z軸)
void ScaleObject(void);									//オブジェクトの拡大縮小処理
void ResetEdit(void);									//オブジェクトの情報リセット処理
void EditMaterialCustom(void);							//マテリアルのエディット処理

//グローバル変数
EditObject g_EditObject;								//オブジェクトの情報
int g_nStyleObject;										//スタイルの変数
int g_nSoundDJ;											//現在流れているサウンド

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
		g_EditObject.nSetNumber = -1;

		//使用しない
		g_EditObject.bUse = false;

		//エディットオブジェクトの種類を初期化する
		g_EditObject.nType = MODELTYPE_OBJECT_TREE;

		//モデルの基本情報
		g_EditObject.pModelData = GetModelData(g_EditObject.nType + FROM_OBJECT);

		//選択中のマテリアルの番号を初期化する
		g_EditObject.nCntMaterial = 0;

		//色を変えるカウントを初期化する
		g_EditObject.nColorCount = 0;
	}

	//スタイルを設定する
	g_nStyleObject = EDITSTYLE_OBJECT;

	//サウンドを初期化する
	g_nSoundDJ = FUJITA_DJ_LABEL_ONE;

	// 
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)GetModelData(FROM_OBJECT)->pBuffMat->GetBufferPointer();

	//カスタム用のマテリアル情報
	for (int nCntModel = 0; nCntModel < MODELTYPE_OBJECT_MAX; nCntModel++)
	{ // モデルの初期化

		for (int nCntMat = 0; nCntMat < (int)g_EditObject.pModelData->dwNumMat; nCntMat++)
		{

			//マテリアルをコピーする
			g_EditObject.EditMaterial[nCntModel][nCntMat] = pMat[nCntMat];
		}
	}
}

//========================================
//モデルの終了処理
//========================================
void UninitEditObject(void)
{

}

//========================================
//モデルの更新処理
//========================================
void UpdateEditObject(void)
{
	Object *pObject = GetObjectData();				//オブジェクトを取得する
	Camera *pCamera = GetCamera();					//カメラの情報を取得する

	g_nStyleObject = GetStyle();					//スタイルを取得する

	if (g_nStyleObject == EDITSTYLE_OBJECT)
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
				g_EditObject.pModelData = GetModelData(g_EditObject.nType + FROM_OBJECT);

				break;							//抜け出す
			}
		}
	}
	else
	{//オブジェクト設置モードじゃなかった場合
		//使用しない
		g_EditObject.bUse = false;
	}

	//種類変更処理
	TypeChangeEdit();

	//移動処理
	MoveEdit(pCamera->rot.y);

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

	//オブジェクトの消去
	DeleteEditObject();

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

	//マテリアルのエディット処理
	EditMaterialCustom();

	if (GetKeyboardTrigger(DIK_F5) == true)
	{//F11キーを押した場合
		//サウンドの停止
		StopSoundDJ();

		//サウンドを流す
		PlaySound(g_nSoundDJ, true);
	}

	if (GetKeyboardTrigger(DIK_F4) == true)
	{//F10キーを押した場合
		//サウンドを変える
		g_nSoundDJ = (g_nSoundDJ + 1) % SOUND_DJ_LABEL_MAX;
	}
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
	D3DXMATERIAL *pEditMat;						//エディットマテリアルのデータ

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
		pMat = (D3DXMATERIAL*)g_EditObject.pModelData->pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_EditObject.pModelData->dwNumMat; nCntMat++)
		{
			pEditMat = &g_EditObject.EditMaterial[g_EditObject.nType][nCntMat];

			if (nCntMat == g_EditObject.nCntMaterial)
			{//選択中のマテリアルだった場合
				//透明度を設定する
				pEditMat->MatD3D.Ambient.a = 1.0f;
				pEditMat->MatD3D.Diffuse.a = 1.0f;
			}
			else
			{//選択されていないマテリアルだった場合
				//透明度を設定する
				pEditMat->MatD3D.Ambient.a = 0.5f;
				pEditMat->MatD3D.Diffuse.a = 0.5f;
			}

			//マテリアルの設定
			pDevice->SetMaterial(&g_EditObject.EditMaterial[g_EditObject.nType][nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_EditObject.pModelData->pTexture[nCntMat]);

			//モデル(パーツ)の描画
			g_EditObject.pModelData->pMesh->DrawSubset(nCntMat);
		}
		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=======================================
//エディットオブジェクトの取得処理
//=======================================
EditObject *GetEditObject(void)
{
	//エディットオブジェクトの情報を返す
	return &g_EditObject;
}

//=======================================
//オブジェクトの消去
//=======================================
void DeleteEditObject(void)
{
	Object *pObject = GetObjectData();				//オブジェクトの情報を取得する

	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++, pObject++)
	{
		if (pObject->bUse == true)
		{//オブジェクトが使用されていた場合
			if (g_EditObject.pos.x >= pObject->pos.x - 50.0f &&
				g_EditObject.pos.x <= pObject->pos.x + 50.0f &&
				g_EditObject.pos.z >= pObject->pos.z - 50.0f &&
				g_EditObject.pos.z <= pObject->pos.z + 50.0f &&
				g_EditObject.bUse == true)
			{//範囲内に入った場合
				//削除対象状態にする
				pObject->editState = OBJECTSTATE_DELETETARGET;

				if (GetKeyboardTrigger(DIK_9) == true)
				{//9キーを押した場合

					//使用していない
					pObject->bUse = false;
				}
			}
			else
			{//範囲外だった場合
				//使用状態にする
				pObject->editState = OBJECTSTATE_USE;
			}
		}
	}
}

//=======================================
//種類変更処理
//=======================================
void TypeChangeEdit(void)
{
	if (g_nStyleObject == EDITSTYLE_OBJECT)
	{//オブジェクト設置モードだった場合
		if (GetKeyboardTrigger(DIK_1) == true)
		{//1キーを押した場合
			//設定オブジェクトの種類を選択する
			g_EditObject.nType = (g_EditObject.nType + 1) % MODELTYPE_OBJECT_MAX;

			//設定する
			g_EditObject.pModelData = GetModelData(g_EditObject.nType + FROM_OBJECT);
		}
	}
}

//=======================================
//移動処理
//=======================================
void MoveEdit(float Camerarot)
{
	if (g_nStyleObject == EDITSTYLE_OBJECT)
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
	if (g_nStyleObject == EDITSTYLE_OBJECT)
	{//オブジェクト設置モードだった場合
		if (GetKeyboardTrigger(DIK_0) == true)
		{//0キーを押した場合
			for (int nCount = 0; nCount < MAX_MATERIAL; nCount++)
			{//マテリアルの透明度を1.0fにする
				//透明度を1.0fにする
				g_EditObject.EditMaterial[g_EditObject.nType][nCount].MatD3D.Ambient.a = 1.0f;
				g_EditObject.EditMaterial[g_EditObject.nType][nCount].MatD3D.Diffuse.a = 1.0f;
			}

			//オブジェクトの設定処理
			SetObject(g_EditObject.pos, g_EditObject.rot, g_EditObject.scale, &g_EditObject.EditMaterial[g_EditObject.nType][0], BREAKTYPE_NONE, g_EditObject.nType);

			//エディットオブジェクトの番号を初期化する
			g_EditObject.nSetNumber = -1;
		}
	}
}

//=======================================
//オブジェクトの拡大縮小処理(X軸)
//=======================================
void ScaleObjectX(void)
{
	if (g_nStyleObject == EDITSTYLE_OBJECT)
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
}

//=======================================
//オブジェクトの拡大縮小処理(Y軸)
//=======================================
void ScaleObjectY(void)
{
	if (g_nStyleObject == EDITSTYLE_OBJECT)
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
}

//=======================================
//オブジェクトの拡大縮小処理(Z軸)
//=======================================
void ScaleObjectZ(void)
{
	if (g_nStyleObject == EDITSTYLE_OBJECT)
	{//オブジェクト設置モードだった場合
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
}

//=======================================
//オブジェクトの拡大縮小処理
//=======================================
void ScaleObject(void)
{
	if (g_nStyleObject == EDITSTYLE_OBJECT)
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
}

//=======================================
//オブジェクトの情報リセット処理
//=======================================
void ResetEdit(void)
{
	if (GetKeyboardTrigger(DIK_2) == true)
	{//2キーを押した場合
		//角度を初期化する
		g_EditObject.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	if (g_nStyleObject == EDITSTYLE_OBJECT)
	{//オブジェクト設置モードだった場合
		if (GetKeyboardTrigger(DIK_3) == true)
		{//3キーを押した場合
			//拡大率を初期化する
			g_EditObject.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		}
	}
}

//=======================================
//マテリアルのエディット処理
//=======================================
void EditMaterialCustom(void)
{
	D3DMATERIAL9 *pMatEdit;						//マテリアルデータへのポインタ

	if (g_EditObject.bUse == true)
	{//設定中か使用している場合
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{//SPACEキーを押した場合
			//マテリアルを変更する
			g_EditObject.nCntMaterial = (g_EditObject.nCntMaterial + 1) % (int)(g_EditObject.pModelData->dwNumMat);
		}

		for (int nCntMat = 0; nCntMat < (int)g_EditObject.pModelData->dwNumMat; nCntMat++)
		{
			//マテリアルデータへのポインタを取得
			pMatEdit = &g_EditObject.EditMaterial[g_EditObject.nType][nCntMat].MatD3D;

			if (nCntMat == g_EditObject.nCntMaterial)
			{//選択中のマテリアルだった場合
				if (GetKeyboardPress(DIK_LSHIFT) == true)
				{//左シフトキーを押した場合
					if (GetKeyboardPress(DIK_R) == true)
					{//Rキーを押した場合
						//カウントを加算する
						g_EditObject.nColorCount++;

						if (g_EditObject.nColorCount % 5 == 0)
						{//20Fごとに
							//R値を増やす
							pMatEdit->Ambient.r += 0.01f;

							if (pMatEdit->Ambient.r > 1.0f)
							{//R値が限界を超えたら
								//R値を0に戻す
								pMatEdit->Ambient.r = 0.0f;
							}

							pMatEdit->Diffuse.r += 0.01f;

							if (pMatEdit->Diffuse.r > 1.0f)
							{//R値が限界を超えたら
								//R値を0に戻す
								pMatEdit->Diffuse.r = 0.0f;
							}
						}
					}
					else if (GetKeyboardPress(DIK_F) == true)
					{//Fキーを押した場合
					 //カウントを加算する
						g_EditObject.nColorCount++;

						if (g_EditObject.nColorCount % 5 == 0)
						{//20Fごとに
							//R値を増やす
							pMatEdit->Ambient.r -= 0.01f;

							if (pMatEdit->Ambient.r < 0.0f)
							{//R値が限界を超えたら
								//R値を0に戻す
								pMatEdit->Ambient.r = 1.0f;
							}

							pMatEdit->Diffuse.r -= 0.01f;

							if (pMatEdit->Diffuse.r < 0.0f)
							{//R値が限界を超えたら
								//R値を0に戻す
								pMatEdit->Diffuse.r = 1.0f;
							}
						}
					}

					if (GetKeyboardPress(DIK_T) == true)
					{//Tキーを押した場合
						//カウントを加算する
						g_EditObject.nColorCount++;

						if (g_EditObject.nColorCount % 5 == 0)
						{//20Fごとに
							//G値を増やす
							pMatEdit->Ambient.g += 0.01f;

							if (pMatEdit->Ambient.g > 1.0f)
							{//G値が限界を超えたら
								//G値を0に戻す
								pMatEdit->Ambient.g = 0.0f;
							}

							//G値を増やす
							pMatEdit->Diffuse.g += 0.01f;

							if (pMatEdit->Diffuse.g > 1.0f)
							{//G値が限界を超えたら
								//G値を0に戻す
								pMatEdit->Diffuse.g = 0.0f;
							}
						}
					}
					else if (GetKeyboardPress(DIK_G) == true)
					{//Gキーを押した場合
						//カウントを加算する
						g_EditObject.nColorCount++;

						if (g_EditObject.nColorCount % 5 == 0)
						{//20Fごとに
							//R値を増やす
							pMatEdit->Ambient.g -= 0.01f;

							if (pMatEdit->Ambient.g < 0.0f)
							{//R値が限界を超えたら
								//R値を0に戻す
								pMatEdit->Ambient.g = 1.0f;
							}

							//G値を増やす
							pMatEdit->Diffuse.g -= 0.01f;

							if (pMatEdit->Diffuse.g < 0.0f)
							{//R値が限界を超えたら
								//R値を0に戻す
								pMatEdit->Diffuse.g = 1.0f;
							}
						}
					}

					if (GetKeyboardPress(DIK_Y) == true)
					{//Yキーを押した場合
						//カウントを加算する
						g_EditObject.nColorCount++;

						if (g_EditObject.nColorCount % 5 == 0)
						{//20Fごとに
							//B値を増やす
							pMatEdit->Ambient.b += 0.01f;

							if (pMatEdit->Ambient.b > 1.0f)
							{//B値が限界を超えたら
								//B値を0に戻す
								pMatEdit->Ambient.b = 0.0f;
							}

							//B値を増やす
							pMatEdit->Diffuse.b += 0.01f;

							if (pMatEdit->Diffuse.b > 1.0f)
							{//B値が限界を超えたら
								//B値を0に戻す
								pMatEdit->Diffuse.b = 0.0f;
							}
						}
					}
					else if (GetKeyboardPress(DIK_H) == true)
					{//Hキーを押した場合
						//カウントを加算する
						g_EditObject.nColorCount++;

						if (g_EditObject.nColorCount % 5 == 0)
						{//20Fごとに
							//R値を増やす
							pMatEdit->Ambient.b -= 0.01f;

							if (pMatEdit->Ambient.b < 0.0f)
							{//R値が限界を超えたら
								//R値を0に戻す
								pMatEdit->Ambient.b = 1.0f;
							}

							pMatEdit->Diffuse.b -= 0.01f;

							if (pMatEdit->Diffuse.b < 0.0f)
							{//R値が限界を超えたら
								//R値を0に戻す
								pMatEdit->Diffuse.b = 1.0f;
							}
						}
					}

					//アドレスを入れる
					D3DMATERIAL9 *pMatOriginal = &g_EditObject.EditMaterial[g_EditObject.nType][nCntMat].MatD3D;

					if (GetKeyboardPress(DIK_V) == true)
					{//Vキーを押した場合
						//R値のリセット
						pMatEdit->Ambient.r = pMatOriginal->Ambient.r;
						pMatEdit->Diffuse.r = pMatOriginal->Diffuse.r;
					}

					if (GetKeyboardPress(DIK_B) == true)
					{//Bキーを押した場合
						//G値のリセット
						pMatEdit->Ambient.g = pMatOriginal->Ambient.g;
						pMatEdit->Diffuse.g = pMatOriginal->Diffuse.g;
					}

					if (GetKeyboardPress(DIK_N) == true)
					{//Nキーを押した場合
						//B値のリセット
						pMatEdit->Ambient.b = pMatOriginal->Ambient.b;
						pMatEdit->Diffuse.b = pMatOriginal->Diffuse.b;
					}
				}
			}
		}
	}
}

////=======================================
////マテリアルの情報取得処理
////=======================================
//EditMaterial GetCustomMaterial(void)
//{
//	//マテリアルの情報を返す
//	return g_EditMaterial[g_EditObject.nType];
//}