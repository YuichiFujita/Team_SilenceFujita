
//=====================================================================
//=
//= ランキング処理[ranking.cpp]
//= Author Sakai Minato
//=
//=====================================================================

#include <stdio.h>

#include "main.h"
#include "ranking.h"

#include "Input.h"
#include "sound.h"
#include "fade.h"
#include "value.h"

#include "score.h"
#include "title.h"

#include "calculation.h"
#include "camera.h"
#include "Human.h"
#include "icon.h"
#include "light.h"
#include "score.h"
#include "weather.h"
#include "shadow.h"
#include "Car.h"
#include "Police.h"

//**********************************************
//* マクロ
//**********************************************
#define RANK_FILE_MODE		(FILE_MODE_BIN)					//ランキングファイルの入出力モード
#define RANK_FILE_TXT		("data\\TXT\\rank.txt")			//ランキングのスコアのテキストファイル
#define RANK_FILE_BIN		("data\\BIN\\rank.bin")			//ランキングのスコアのバイナリファイル

#define RANK_FLASH_MAX		(1.0f)					//点滅の最大値
#define RANK_FLASH_MIN		(0.3f)					//点滅の最小値
#define RANK_FLASH_INTERVAL	(0.02f)					//点滅の間隔

//**********************************
//* ランキングスコアの数値関係
//**********************************
#define RANK_MAX			(5)							//スコアの数
#define	RANK_NUM_PLACE		(9)							//スコアの桁数
#define RANK_TEX			(1)							//スコアのテクスチャ
#define RANK_COLUMN			(1)							//スコアを分ける列
#define RANK_POS_X			(SCREEN_WIDTH * 0.75f)		//スコアの開始位置（X)
#define RANK_POS_Y			(SCREEN_HEIGHT * 0.14f)		//スコアの開始位置（Y）	
#define RANK_SIZE_X			(50.0f)						//スコアの大きさ（X）
#define RANK_SIZE_Y			(40.0f)						//スコアの大きさ（Y）
#define RANK_INTERVAL_X		(80.0f)						//スコアの間隔（X）
#define RANK_INTERVAL_Y		(90.0f)						//スコアの間隔（Y）
#define RANK_MAX_NUMBER		(999999999)					//スコアの最大数

//**********************************
//* ランキングニュースコアの数値関係
//**********************************
#define	RANK_NEW_NUM_PLACE	(9)							//ニュースコアの桁数
#define RANK_NEW_TEX		(1)							//ニュースコアのテクスチャ
#define RANK_NEW_COLUMN		(1)							//ニュースコアを分ける列
#define RANK_NEW_POS_X		(SCREEN_WIDTH * 0.70f)		//ニュースコアの開始位置（X)
#define RANK_NEW_POS_Y		(SCREEN_HEIGHT * 0.85f)		//ニュースコアの開始位置（Y）	
#define RANK_NEW_SIZE_X		(45.0f)						//ニュースコアの大きさ（X）
#define RANK_NEW_SIZE_Y		(35.0f)						//ニュースコアの大きさ（Y）
#define RANK_NEW_INTERVAL_X	(65.0f)						//ニュースコアの間隔（X）
#define RANK_NEW_INTERVAL_Y	(0.0f)						//ニュースコアの間隔（Y）
#define RANK_NEW_MAX_NUMBER	(999999999)					//ニュースコアの最大数

//**********************************
//* 画面遷移中のランキングスコアの数値関係
//**********************************
#define RANK_FADE_MAX			(5)							//スコアの数
#define	RANK_FADE_NUM_PLACE		(9)							//スコアの桁数
#define RANK_FADE_TEX			(1)							//スコアのテクスチャ
#define RANK_FADE_COLUMN		(1)							//スコアを分ける列
#define RANK_FADE_POS_X			(SCREEN_WIDTH * 0.75f)		//スコアの開始位置（X)
#define RANK_FADE_POS_Y			(SCREEN_HEIGHT * 0.23f)		//スコアの開始位置（Y）	
#define RANK_FADE_SIZE_X		(50.0f)						//スコアの大きさ（X）
#define RANK_FADE_SIZE_Y		(40.0f)						//スコアの大きさ（Y）
#define RANK_FADE_INTERVAL_X	(80.0f)						//スコアの間隔（X）
#define RANK_FADE_INTERVAL_Y	(105.0f)					//スコアの間隔（Y）
#define RANK_FADE_MAX_NUMBER	(999999999)					//スコアの最大数

//**********************************************
//* 列挙型
//**********************************************

//**********************************************
//* 構造体
//**********************************************
//**********************************
//* ランキングのスコア
//**********************************
typedef struct
{
	int nScore;				//スコア
	D3DXVECTOR3 pos;		//位置
	D3DXCOLOR col;			//色
	bool bFlash;			//点滅
	bool bUse;				//使用しているか
}RankingScore;

//***************************************************************
// グローバル変数宣言
//***************************************************************
RankingScore g_aRankScore[RANK_MAX];	//ランキングでのスコアの情報
RankingScore g_aRankNewScore;			//ランキングのニュースコアの情報

FileMode g_fileMode;			//ファイルの入出力の種類
bool g_bRankTrance;				//遷移の有無
int g_nNewScoreNumber = -1;		//ニュースコアの番号

//====================================
//= ランキングの初期化処理
//=====================================
void InitRanking(void)
{
	//スコアを取得
	int pScore = GetScore();

	//変数宣言
	int nCutRank;

	//変数の初期化
	g_bRankTrance = false;

	if (GetTitleState() != TITLESTATE_FADE)
	{//タイトル遷移中じゃないとき
		for (nCutRank = 0; nCutRank < RANK_MAX; nCutRank++)
		{
			g_aRankScore[nCutRank].pos = D3DXVECTOR3(RANK_POS_X, RANK_POS_Y, 0.0f);
			g_aRankScore[nCutRank].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_aRankScore[nCutRank].bFlash = false;
			g_aRankScore[nCutRank].bUse = true;
		}
	}
	else
	{
		//スコアの情報を初期化
		for (nCutRank = 0; nCutRank < RANK_FADE_MAX; nCutRank++)
		{
			g_aRankScore[nCutRank].pos = D3DXVECTOR3(RANK_FADE_POS_X, RANK_FADE_POS_Y, 0.0f);
			g_aRankScore[nCutRank].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_aRankScore[nCutRank].bFlash = false;
			g_aRankScore[nCutRank].bUse = true;
		}
	}

	//ニュースコアの情報を初期化
	{
		g_aRankNewScore.nScore = pScore;
		g_aRankNewScore.pos = D3DXVECTOR3(RANK_NEW_POS_X, RANK_NEW_POS_Y, 0.0f);
		g_aRankNewScore.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aRankScore[nCutRank].bFlash = false;
		g_aRankNewScore.bUse = true;
	}

	// 万能初期化の全体処理（3Dマップ）
	InitAllAroundChunk();

	// アイコンの初期化
	InitIcon();

	// ファイルをロードする全体処理
	LoadFileChunk
	( // 引数
		true,	// 車のカーブ
		true,	// 人間のカーブ
		true,	// ステージ
		false,	// 当たり判定
		true,	// 影
		true,	// オブジェクト
		true	// AI
	);

	if (GetTitleState() != TITLESTATE_FADE)
	{//タイトル遷移中じゃないとき
		
		//スコアのソート
		SortRankingScore();

	}
}

//=====================================
//= ランキングの終了処理
//=====================================
void UninitRanking(void)
{
	//ニュースコアの番号
	g_nNewScoreNumber = -1;

	//ランキングの値をファイルに書き出し
	SaveRanking();

	// 万能終了の全体処理（3Dマップ）
	UninitAllAroundChunk();

	// アイコンの終了
	UninitIcon();

	if (GetTitleState() != TITLESTATE_FADE)
	{//タイトル遷移中じゃないとき
		//サウンドの停止（リザルトからのBGM）
		StopSound();
	}

}

//=====================================
//= ランキングの更新処理
//=====================================
void UpdateRanking(void)
{
	// 天気の更新処理
	UpdateWeather();

	// カメラの更新処理
	UpdateCamera();

	// ライトの更新処理
	UpdateLight();

	// 車の更新処理
	UpdateCar();

	// 人間の更新処理
	UpdateHuman();

	//　パトカーの更新処理
	UpdatePolice();

	//------*****点滅の処理******------

	if (g_nNewScoreNumber != -1)
	{//ランキングが更新されたとき
		//更新されたスコアとニュースコア
		if (g_aRankScore[g_nNewScoreNumber].bFlash == false)
		{
			g_aRankScore[g_nNewScoreNumber].col.a -= RANK_FLASH_INTERVAL;
			g_aRankNewScore.col.a -= RANK_FLASH_INTERVAL;
			if (g_aRankScore[g_nNewScoreNumber].col.a <= RANK_FLASH_MIN)
			{
				g_aRankScore[g_nNewScoreNumber].bFlash = true;
				g_aRankNewScore.bFlash = true;
			}
		}
		else if (g_aRankScore[g_nNewScoreNumber].bFlash == true)
		{
			g_aRankScore[g_nNewScoreNumber].col.a += RANK_FLASH_INTERVAL;
			g_aRankNewScore.col.a += RANK_FLASH_INTERVAL;
			if (g_aRankScore[g_nNewScoreNumber].col.a >= RANK_FLASH_MAX)
			{
				g_aRankScore[g_nNewScoreNumber].bFlash = false;
				g_aRankNewScore.bFlash = false;
			}
		}
	}

	//------*****入力の判定******------

	//フェードの情報を取得
	FADE pFade = GetFade();

	//モード設定
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_SPACE) == true
	 || GetJoyKeyTrigger(JOYKEY_A, 0) == true  || GetJoyKeyTrigger(JOYKEY_B, 0) == true
	 || GetJoyKeyTrigger(JOYKEY_X, 0) == true  || GetJoyKeyTrigger(JOYKEY_Y, 0) == true && pFade == FADE_NONE)
	{//決定キーを押したら
		//モード設定（タイトル画面に移行）
		SetFade(MODE_TITLE);
		//決定の音
		//PlaySound(SOUND_LABEL_SE_ENTER);
	}

	//触っていない時間のときタイトルに
	if (g_bRankTrance == true)
	{
		//モード設定（タイトル画面に移行）
		SetFade(MODE_TITLE);
	}
}

//=====================================
//= ランキングの描画処理
//=====================================
void DrawRanking(void)
{
	// リザルトの描画全体処理
	DrawResultChunk();

	if (GetTitleState() != TITLESTATE_FADE)
	{//タイトル遷移中じゃないとき
	 //ランキングスコアの数値
		for (int nCount = 0; nCount < RANK_MAX; nCount++)
		{ // 順位が更新されている場合

		  //数値を設定
			SetValue(
				D3DXVECTOR3(g_aRankScore[nCount].pos.x, g_aRankScore[nCount].pos.y + (nCount * RANK_INTERVAL_Y), 0.0f),
				g_aRankScore[nCount].nScore,
				RANK_MAX_NUMBER,
				RANK_SIZE_X,
				RANK_SIZE_Y,
				RANK_INTERVAL_X,
				g_aRankScore[nCount].col.a);

			// 数値の描画
			DrawValue(RANK_NUM_PLACE, VALUETYPE_NORMAL);
		}

		//ニュースコアの数値
		{
			//数値を設定
			SetValue(
				D3DXVECTOR3(g_aRankNewScore.pos.x, g_aRankNewScore.pos.y, 0.0f),
				g_aRankNewScore.nScore,
				RANK_MAX_NUMBER,
				RANK_NEW_SIZE_X,
				RANK_NEW_SIZE_Y,
				RANK_NEW_INTERVAL_X,
				g_aRankNewScore.col.a);

			// 数値の描画
			DrawValue(RANK_NUM_PLACE, VALUETYPE_RED);
		}
	}
	else
	{//タイトル遷移中の描画処理

		for (int nCount = 0; nCount < RANK_FADE_MAX; nCount++)
		{ // 順位が更新されている場合

		  //数値を設定
			SetValue(
				D3DXVECTOR3(g_aRankScore[nCount].pos.x, g_aRankScore[nCount].pos.y + (nCount * RANK_FADE_INTERVAL_Y), 0.0f),
				g_aRankScore[nCount].nScore,
				RANK_FADE_MAX_NUMBER,
				RANK_FADE_SIZE_X,
				RANK_FADE_SIZE_Y,
				RANK_FADE_INTERVAL_X,
				g_aRankScore[nCount].col.a);

			// 数値の描画
			DrawValue(RANK_FADE_NUM_PLACE, VALUETYPE_NORMAL);
		}
	}

}

//=====================================
//= ランキングのスコアのソート設定
//=====================================
void SortRankingScore(void)
{
	//変数宣言
	int nCutRank, nCutScoreRank;	//最大数のデータと要素のカウント
	int nTenpData;					//仮のデータ置き場
	int nCountData;					//置き換える番号

	if (g_aRankNewScore.nScore > 0)
	{//ニュースコアが0より下ではないとき
		if (g_aRankNewScore.nScore >= g_aRankScore[(RANK_MAX - 1)].nScore ||
			g_aRankScore[(RANK_MAX - 1)].nScore == 0)
		{//ニュースコアが一番低いスコアより大きいとき
			g_aRankScore[(RANK_MAX - 1)].nScore = g_aRankNewScore.nScore;
			g_nNewScoreNumber = (RANK_MAX - 1);

			//要素1のデータと要素2のデータを比較
			for (nCutRank = 0; nCutRank < (RANK_MAX - 1); nCutRank++)
			{
				nCountData = nCutRank;
				//置き換え番号に要素1のデータ番号を代入
				for (nCutScoreRank = nCutRank; nCutScoreRank < RANK_MAX; nCutScoreRank++)
				{
					//要素2に仮の数値を代入
					if (g_aRankScore[nCutScoreRank].nScore > g_aRankScore[nCountData].nScore)
					{
						if (nCutScoreRank == g_nNewScoreNumber)
						{
							g_nNewScoreNumber = nCountData;
						}
						nCountData = nCutScoreRank;
					}
				}

				//データを置き換え
				nTenpData = g_aRankScore[nCutRank].nScore;
				g_aRankScore[nCutRank].nScore = g_aRankScore[nCountData].nScore;
				g_aRankScore[nCountData].nScore = nTenpData;
			}
		}
	}
}

//=====================================
//= ランキングのリセット処理
//=====================================
void ResetRanking(void)
{
	//ファイルの入出力モードの設定
	g_fileMode = RANK_FILE_MODE;

	//ランキングのスコアの値をファイルから読み込み
	g_aRankScore[0].nScore = {};
	LoadRanking();

	//ニュースコアのリセット
	g_aRankNewScore.nScore = 0;
}

//=======================
//=　ファイルに書き出し
//=======================
void SaveRanking(void)
{
	//ファイルポインタ宣言
	FILE * pFile;

	switch (g_fileMode)
	{
	//**********テキストモード**********
	case FILE_MODE_TXT:
		//ファイルを開く
		pFile = fopen(RANK_FILE_TXT, "w");
		//ファイルに書き出し
		if (pFile != NULL)
		{//ファイルが開けたなら
		 //テキストファイルに書き出し
			for (int nCount = 0; nCount < RANK_MAX; nCount++)
			{
				fprintf(pFile, "%d\n", g_aRankScore[nCount].nScore);
			}
			//ファイルを閉じる
			fclose(pFile);
		}
		else
		{ // ファイルが開けなかった場合

		  // エラーメッセージボックス
			MessageBox(NULL, "ランキングデータがありません", "警告！", MB_ICONWARNING);
		}
		break;
	//**********バイナリモード**********
	case FILE_MODE_BIN:
		//ファイルを開く
		pFile = fopen(RANK_FILE_BIN, "wb");
		//ファイルに書き出し
		if (pFile != NULL)
		{//ファイルが開けたなら
		 //バイナリファイルに書き出し
			for (int nCount = 0; nCount < RANK_MAX; nCount++)
			{
				fwrite(&g_aRankScore[nCount].nScore, sizeof(int), RANK_MAX, pFile);
			}
			//ファイルを閉じる
			fclose(pFile);
		}
		else
		{ // ファイルが開けなかった場合

		  // エラーメッセージボックス
			MessageBox(NULL, "ランキングデータがありません", "警告！", MB_ICONWARNING);
		}
		break;

	}

}

//=======================
//=　ファイルを読み込み
//=======================
void LoadRanking(void)
{
	//ファイルポインタ宣言
	FILE *pFile;

	switch (g_fileMode)
	{
	//**********テキストモード**********
	case FILE_MODE_TXT:
		//ファイルを開く
		pFile = fopen(RANK_FILE_TXT, "r");
		//ファイルを読み込み
		if (pFile != NULL)
		{//ファイルが開けたなら
		 //バイナリファイルに書き出し
			for (int nCount = 0; nCount < RANK_MAX; nCount++)
			{
				fscanf(pFile, "%d", &g_aRankScore[nCount].nScore);
			}
			//ファイルを閉じる
			fclose(pFile);
		}
		else
		{ // ファイルが開けなかった場合

		  // エラーメッセージボックス
			MessageBox(NULL, "ランキングデータがありません", "警告！", MB_ICONWARNING);
		}
		break;
	//**********バイナリモード**********
	case FILE_MODE_BIN:
		//ファイルを開く
		pFile = fopen(RANK_FILE_BIN, "rb");
		//ファイルを読み込み
		if (pFile != NULL)
		{//ファイルが開けたなら
		 //バイナリファイルに書き出し
			for (int nCount = 0; nCount < RANK_MAX; nCount++)
			{
				fread(&g_aRankScore[nCount].nScore, sizeof(int), RANK_MAX, pFile);
			}
			//ファイルを閉じる
			fclose(pFile);
		}
		break;
	}
}

//=======================
//=　道路処理での画面遷移を設定
//=======================
void SetRankingRoadTrance(void)
{
	g_bRankTrance = true;
}