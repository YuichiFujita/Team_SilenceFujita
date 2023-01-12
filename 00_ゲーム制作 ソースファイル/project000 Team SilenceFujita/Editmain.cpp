//========================================
//
//エディットメインのメイン処理[Editmain.cpp]
//Author 小原立暉
//
//========================================
#include "main.h"
#include "game.h"
#include "Edit.h"
#include "EditBillboard.h"
#include "Editmain.h"
#include "SoundDJ.h"
#include "input.h"

//プロトタイプ宣言
void DrawDebugEditObject(void);			//エディットオブジェクトモードのデバッグ表示
void DrawDebugEditBillboard(void);		//エディットビルボードモードのデバッグ表示
void DrawDebugControlObject(void);		//エディットオブジェクト操作説明
void DrawDebugControlBillboard(void);	//エディットビルボード操作説明

//グローバル変数
int g_EditStyle;						//スタイル

//======================
//初期化
//======================
void InitEditmain()
{
	//エディットオブジェクトの初期化処理
	InitEditObject();

	//エディットビルボードの初期化処理
	InitEditBillboard();

	//エディットオブジェクトスタイルにする
	g_EditStyle = EDITSTYLE_OBJECT;
}

//============================
//終了
//============================
void UninitEditmain(void)
{
	//エディットの終了処理
	UninitEditObject();

	//エディットビルボードの終了処理
	UninitEditBillboard();
}

//==========================
//更新処理
//==========================
void UpdateEditmain(void)
{
	//スタイル変更処理
	StyleChange();

	if (g_EditStyle == EDITSTYLE_OBJECT)
	{//オブジェクトエディットモードの場合
		//エディットの更新処理
		UpdateEditObject();
	}
	else if (g_EditStyle == EDITSTYLE_BILLBOARD)
	{//ビルボードエディットモードの場合
		//エディットビルボードの更新処理
		UpdateEditBillboard();
	}
}

//============================
//ゲーム画面の描画処理
//============================
void DrawEditmain(void)
{
	if (g_EditStyle == EDITSTYLE_OBJECT)
	{//オブジェクトエディットモードの場合
		//エディットオブジェクトの描画処理
		DrawEditObject();
	}
	else if (g_EditStyle == EDITSTYLE_BILLBOARD)
	{//ビルボードエディットモードの場合
		//エディットビルボードの描画処理
		DrawEditBillboard();
	}
}

//=======================================
//スタイル変更処理
//=======================================
void StyleChange(void)
{
	D3DXVECTOR3 *pEditBillpos = &GetEditBillboard()->pos;		//エディットビルボードの情報を取得する
	D3DXVECTOR3 *pEditObjepos = &GetEditObject()->pos;			//エディットオブジェクトの情報を取得する

	if (GetKeyboardTrigger(DIK_6) == true)
	{//6キーを押した場合
		//スタイルを選択する
		g_EditStyle = (g_EditStyle + 1) % EDITSTYLE_MAX;

		if (g_EditStyle == EDITSTYLE_BILLBOARD)
		{//ビルボードモードだった場合
			//オブジェクトがあった位置を代入する
			*pEditBillpos = *pEditObjepos;
		}
		else if (g_EditStyle == EDITSTYLE_OBJECT)
		{//オブジェクトモードだった場合
			//ビルボードがあった位置を代入する
			*pEditObjepos = *pEditBillpos;
		}
	}
}

//=======================================
//スタイルの取得処理
//=======================================
int GetStyle(void)
{
	//エディットスタイルを返す
	return g_EditStyle;
}