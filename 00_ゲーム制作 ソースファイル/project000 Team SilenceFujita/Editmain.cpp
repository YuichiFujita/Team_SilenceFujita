#ifdef _DEBUG	// デバッグ処理
//========================================
//
//エディットメインのメイン処理[Editmain.cpp]
//Author 小原立暉
//
//========================================
#include "main.h"
#include "game.h"
#include "EditObject.h"
#include "EditBillboard.h"
#include "Editmain.h"
#include "SoundDJ.h"
#include "input.h"
#include "object.h"

//マクロ定義
#define SAVE_STAGE_TXT	"data\\TXT\\save_stage.txt"		// ステージ保存の外部ファイルの相対パス

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

//=======================================
//	ステージの保存処理
//=======================================
void TxtSaveStage(void)
{
	// ポインタを宣言
	FILE      *pFile;							// ファイルポインタ
	Object    *pObject    = GetObjectData();	// オブジェクトの情報ポインタ
	Billboard *pBillboard = GetBillboard();		// ビルボードの情報ポインタ

	// ファイルを書き出し方式で開く
	pFile = fopen(SAVE_STAGE_TXT, "w");

	if (pFile != NULL)
	{ // ファイルが開けた場合
		
		// 見出し
		fprintf(pFile, "#===========================================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#	エディットの保存ステージ [save_stage.txt]\n");
		fprintf(pFile, "#	Author：小原 立暉 & 藤田 勇一 & you\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#===========================================================\n");

		// オブジェクトの書き出し
		fprintf(pFile, "#===========================================================\n");
		fprintf(pFile, "#	オブジェクトの設定\n");
		fprintf(pFile, "#===========================================================\n");
		fprintf(pFile, "#-----------------------------------------------------------\n");
		fprintf(pFile, "#	 BREAKTYPE：0  壊れない\n");
		fprintf(pFile, "#	          ：1  壊れる\n");
		fprintf(pFile, "#-----------------------------------------------------------\n");
		fprintf(pFile, "#	SHADOWTYPE：0  影無し\n");
		fprintf(pFile, "#	          ：1  丸影\n");
		fprintf(pFile, "#	          ：2  モデル影\n");
		fprintf(pFile, "#-----------------------------------------------------------\n");
		fprintf(pFile, "#	  COLLTYPE：0  当たらない\n");
		fprintf(pFile, "#	          ：1  当たる\n");
		fprintf(pFile, "#-----------------------------------------------------------\n");

		// オブジェクトの設定の開始地点をテキストに書き出し
		fprintf(pFile, "SETSTAGE_OBJECT\n\n");

		for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pObject++)
		{ // オブジェクトの最大表示数分繰り返す

			if (pObject->bUse == true)
			{ // オブジェクトが使用されている場合

				// オブジェクトの情報の開始地点テキストに書き出し
				fprintf(pFile, "	SET_OBJECT\n");

				// 基本情報の書き出し
				fprintf(pFile, "		POS        = %.1f %.1f %.1f\n", pObject->pos.x,   pObject->pos.y,   pObject->pos.z);	// 位置
				fprintf(pFile, "		ROT        = %.1f %.1f %.1f\n", pObject->rot.x,   pObject->rot.y,   pObject->rot.z);	// 向き
				fprintf(pFile, "		SCALE      = %.1f %.1f %.1f\n", pObject->scale.x, pObject->scale.y, pObject->scale.z);	// 拡大率
				fprintf(pFile, "		TYPE       = %d\n",   pObject->nType);			// オブジェクトの種類
				fprintf(pFile, "		BREAKTYPE  = %d\n",   pObject->nBreakType);		// 壊れ方の種類
				fprintf(pFile, "		SHADOWTYPE = %d\n",   pObject->nShadowType);	// 影の種類
				fprintf(pFile, "		COLLTYPE   = %d\n\n", pObject->nCollisionType);	// 当たり判定の種類

				// マテリアル数の書き出し
				fprintf(pFile, "		NUMMAT     = %d\n", pObject->modelData.dwNumMat);

				// マテリアル情報の書き出し
				for (int nCntMat = 0; nCntMat < (int)pObject->modelData.dwNumMat; nCntMat++)
				{ // マテリアルの数分繰り返す

					// 拡散光の書き出し
					fprintf(pFile, "		%02d_MAT DIF = %.2f %.2f %.2f %.2f\n", nCntMat,
							pObject->matCopy[nCntMat].MatD3D.Diffuse.r, pObject->matCopy[nCntMat].MatD3D.Diffuse.g,
							pObject->matCopy[nCntMat].MatD3D.Diffuse.b, pObject->matCopy[nCntMat].MatD3D.Diffuse.a);

					// 環境光の書き出し
					fprintf(pFile, "		%02d_MAT AMB = %.2f %.2f %.2f %.2f\n", nCntMat,
							pObject->matCopy[nCntMat].MatD3D.Ambient.r, pObject->matCopy[nCntMat].MatD3D.Ambient.g,
							pObject->matCopy[nCntMat].MatD3D.Ambient.b, pObject->matCopy[nCntMat].MatD3D.Ambient.a);
				}

				// オブジェクトの情報の終了地点テキストに書き出し
				fprintf(pFile, "	END_SET_OBJECT\n\n");
			}
		}

		// オブジェクトの設定の終了地点をテキストに書き出し
		fprintf(pFile, "END_SETSTAGE_OBJECT\n\n");

		// ビルボードの書き出し
		fprintf(pFile, "#===========================================================\n");
		fprintf(pFile, "#	ビルボードの設定\n");
		fprintf(pFile, "#===========================================================\n");
		fprintf(pFile, "#-----------------------------------------------------------\n");
		fprintf(pFile, "#	 ANIMATION：0  アニメーション OFF\n");
		fprintf(pFile, "#	          ：1  アニメーション ON\n");
		fprintf(pFile, "#-----------------------------------------------------------\n");
		fprintf(pFile, "#	SHADOWTYPE：0  丸影\n");
		fprintf(pFile, "#	          ：1  影無し\n");
		fprintf(pFile, "#-----------------------------------------------------------\n");

		// ビルボードの設定の開始地点をテキストに書き出し
		fprintf(pFile, "SETSTAGE_BILLBOARD\n\n");

		for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++, pBillboard++)
		{ // ビルボードの最大表示数分繰り返す

			if (pBillboard->bUse == true)
			{ // ビルボードが使用されている場合

				// ビルボードの情報の開始地点テキストに書き出し
				fprintf(pFile, "	SET_BILLBOARD\n");

				// 基本情報の書き出し
				fprintf(pFile, "		POS        = %.1f %.1f %.1f\n",      pBillboard->pos.x, pBillboard->pos.y, pBillboard->pos.z);						// 位置
				fprintf(pFile, "		ROT        = %.1f %.1f %.1f\n",      pBillboard->rot.x, pBillboard->rot.y, pBillboard->rot.z);						// 向き
				fprintf(pFile, "		COL        = %.1f %.1f %.1f %.1f\n", pBillboard->col.r, pBillboard->col.g, pBillboard->col.b, pBillboard->col.a);	// 色
				fprintf(pFile, "		RADIUS     = %.1f %.1f\n",           pBillboard->Radius.x, pBillboard->Radius.y);	// 半径
				fprintf(pFile, "		TYPE       = %d\n",                  pBillboard->Type);								// 種類
				fprintf(pFile, "		SHADOWTYPE = %d\n\n",                pBillboard->bShadow);							// 影の種類

				// アニメーション情報の書き出し
				fprintf(pFile, "		ANIMATION  = %d\n", pBillboard->Anim.bAnim);		// アニメーションの ON / OFF
				fprintf(pFile, "		ANIMCNT    = %d\n", pBillboard->Anim.nAnimCounter);	// 再生カウント
				fprintf(pFile, "		ANIMPAT    = %d\n", pBillboard->Anim.nAnimPattern);	// 再生パターン

				// ビルボードの情報の終了地点テキストに書き出し
				fprintf(pFile, "	END_SET_BILLBOARD\n\n");
			}
		}

		// ビルボードの設定の終了地点をテキストに書き出し
		fprintf(pFile, "END_SETSTAGE_BILLBOARD\n\n");

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "ステージファイルの書き出しに失敗！", "警告！", MB_ICONWARNING);
	}
}
#endif