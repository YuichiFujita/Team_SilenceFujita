
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
#define MAX_RANK			(12)						// 使用するポリゴン数

//**********************************
//* ランキングの外部ファイル関連
//**********************************
#define RANK_FILE_MODE		(FILE_MODE_BIN)				//ランキングファイルの入出力モード
#define RANK_FILE_TXT		("data\\TXT\\rank.txt")		//ランキングのスコアのテキストファイル
#define RANK_FILE_BIN		("data\\BIN\\rank.bin")		//ランキングのスコアのバイナリファイル

//**********************************
//* ランキングスコアの数値関係
//**********************************
#define RANK_SCORE_MAX		(5)							//スコアの数
#define	RANK_NUM_PLACE		(9)							//スコアの桁数
#define RANK_COLUMN			(1)							//スコアを分ける列
#define RANK_POS_X			(SCREEN_WIDTH * 0.68f)		//スコアの開始位置（X)
#define RANK_POS_Y			(SCREEN_HEIGHT * 0.23f)		//スコアの開始位置（Y）	
#define RANK_WIDTH			(40.0f)						//スコアの大きさ（X）
#define RANK_HEIGHT			(30.0f)						//スコアの大きさ（Y）
#define RANK_INTERVAL_X		(60.0f)						//スコアの間隔（X）
#define RANK_INTERVAL_Y		(90.0f)						//スコアの間隔（Y）
#define RANK_MAX_NUMBER		(999999999)					//スコアの最大数

#define RANK_FLASH_MAX		(1.0f)						//点滅の最大値
#define RANK_FLASH_MIN		(0.3f)						//点滅の最小値
#define RANK_FLASH_INTERVAL	(0.02f)						//点滅の間隔

//**********************************
//* ランキングニュースコアの数値関係
//**********************************
#define	RANK_NEW_NUM_PLACE	(9)							//ニュースコアの桁数
#define RANK_NEW_COLUMN		(1)							//ニュースコアを分ける列
#define RANK_NEW_POS_X		(SCREEN_WIDTH * 0.70f)		//ニュースコアの開始位置（X)
#define RANK_NEW_POS_Y		(SCREEN_HEIGHT * 0.88f)		//ニュースコアの開始位置（Y）	
#define RANK_NEW_WIDTH		(45.0f)						//ニュースコアの大きさ（X）
#define RANK_NEW_HEIGHT		(35.0f)						//ニュースコアの大きさ（Y）
#define RANK_NEW_INTERVAL_X	(65.0f)						//ニュースコアの間隔（X）
#define RANK_NEW_INTERVAL_Y	(0.0f)						//ニュースコアの間隔（Y）
#define RANK_NEW_MAX_NUMBER	(999999999)					//ニュースコアの最大数

//**********************************
//* ランキング（文字）の2Dポリゴン関係
//**********************************
#define RANK_CHAR_POS_X		(SCREEN_WIDTH * 0.5f)		//ランキング文字の開始位置（X)
#define RANK_CHAR_POS_Y		(SCREEN_HEIGHT * 0.08f)		//ランキング文字の開始位置（Y）	
#define RANK_CHAR_WIDTH		(300.0f)					//ランキング文字の大きさ（X）
#define RANK_CHAR_HEIGHT	(70.0f)						//ランキング文字の大きさ（Y）

//**********************************
//* ランキング（ニュースコア背景）の2Dポリゴン関係
//**********************************
#define RANK_NEW_BG_POS_X	(SCREEN_WIDTH * 0.49f)		//ニュースコア背景の開始位置（X)
#define RANK_NEW_BG_POS_Y	(SCREEN_HEIGHT * 0.9f)		//ニュースコア背景の開始位置（Y）	
#define RANK_NEW_BG_WIDTH	(400.0f)					//ニュースコア背景の大きさ（X）
#define RANK_NEW_BG_HEIGHT	(60.0f)						//ニュースコア背景の大きさ（Y）

//**********************************
//* ランキング（下線）の2Dポリゴン関係
//**********************************
#define RANK_LINE_WIDTH		(70.0f)		//下線の大きさ（X）
#define RANK_LINE_HEIGHT	(20.0f)		//下線の大きさ（Y）
#define RANK_LINE_OVER_X	(70.0f)		//下線を伸ばす距離（X）
#define RANK_LINE_POS_X		(120.0f)	//初期でずらす位置（X）
#define RANK_LINE_POS_Y		(10.0f)		//初期でずらす位置（Y）

//**********************************
//* ランキング（順位）の2Dポリゴン関係
//**********************************
#define RANK_RANK_WIDTH		(60.0f)		//順位の大きさ（X）
#define RANK_RANK_HEIGHT	(40.0f)		//順位の大きさ（Y）
#define RANK_RANK_POS_X		(40.0f)		//順位の位置調整（X）
#define RANK_RANK_POS_Y		(10.0f)		//順位の位置調整（Y）

//**********************************
//* 画面遷移中のランキングスコアの数値関係
//**********************************
#define RANK_FADE_MAX			(5)							//スコアの数
#define	RANK_FADE_NUM_PLACE		(9)							//スコアの桁数
#define RANK_FADE_COLUMN		(1)							//スコアを分ける列
#define RANK_FADE_POS_X			(SCREEN_WIDTH * 0.68f)		//スコアの開始位置（X)
#define RANK_FADE_POS_Y			(SCREEN_HEIGHT * 0.38f)		//スコアの開始位置（Y）	
#define RANK_FADE_WIDTH			(40.0f)						//スコアの大きさ（X）
#define RANK_FADE_HEIGHT		(30.0f)						//スコアの大きさ（Y）
#define RANK_FADE_INTERVAL_X	(60.0f)						//スコアの間隔（X）
#define RANK_FADE_INTERVAL_Y	(90.0f)						//スコアの間隔（Y）
#define RANK_FADE_MAX_NUMBER	(999999999)					//スコアの最大数

//**********************************
//* 画面遷移中のランキング（字）の2Dポリゴン関係
//**********************************
#define RANK_FADE_CHAR_POS_X	(SCREEN_WIDTH * 0.5f)		//スコアの開始位置（X)
#define RANK_FADE_CHAR_POS_Y	(SCREEN_HEIGHT * 0.13f)		//スコアの開始位置（Y）	
#define RANK_FADE_CHAR_WIDTH	(250.0f)					//スコアの大きさ（X）
#define RANK_FADE_CHAR_HEIGHT	(50.0f)						//スコアの大きさ（Y）

//**********************************************************************************************************************
//	ランキングのコンスト定義
//**********************************************************************************************************************
const char *apTextureRank[] =		// ランキングのテクスチャの相対パス
{
	"data\\TEXTURE\\ranking000.tga",	// ランキング文字
	"data\\TEXTURE\\ui001.tga",			// ニュースコア背景
	"data\\TEXTURE\\rankline000.tga",	// スコアの下線
	"data\\TEXTURE\\rank1st000.png",	// スコアの順位（1位）
	"data\\TEXTURE\\rank2nd000.png",	// スコアの順位（2位）
	"data\\TEXTURE\\rank3rd000.png",	// スコアの順位（3位）
	"data\\TEXTURE\\rank4th000.png",	// スコアの順位（4位）
	"data\\TEXTURE\\rank5th000.png",	// スコアの順位（5位）
};

//**********************************************************************************************************************
//	ランキングの列挙型定義 (TEXTURE_TITLE)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_RANK_CHAR = 0,		// ランキング文字
	TEXTURE_RANK_UI_004,		// ニュースコア背景
	TEXTURE_RANK_LINE,			// ランキングの下線
	TEXTURE_RANK_1ST,			// スコアの順位（一位）
	TEXTURE_RANK_2ND,			// スコアの順位（二位）
	TEXTURE_RANK_3RD,			// スコアの順位（三位）
	TEXTURE_RANK_4TH,			// スコアの順位（四位）
	TEXTURE_RANK_5TH,			// スコアの順位（五位）
	TEXTURE_RANK_MAX,			// この列挙型の総数
} TEXTURE_RANKING;

//**********************************************
//* 列挙型
//**********************************************

//**********************************************
//* 構造体
//**********************************************
//**********************************
//* 構造体（ランキングの2Dポリゴン）
//**********************************
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXCOLOR col;			//色
	bool bUse;				//使用の有無
}Ranking2D;

//**********************************
//* 構造体（ランキングのスコア）
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
LPDIRECT3DTEXTURE9      g_apTextureRank[TEXTURE_RANK_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank = NULL;					// 頂点バッファへのポインタ

Ranking2D g_rank2DChar;						//ランキング文字の2Dポリゴンの情報
Ranking2D g_rank2DNewBg;					//ランキングニュースコア背景の2Dポリゴンの情報
Ranking2D g_rank2DLine[RANK_SCORE_MAX];		//ランキング下線の2Dポリゴンの情報
Ranking2D g_rank2DRank[RANK_SCORE_MAX];		//ランキング順位の2Dポリゴン

RankingScore g_aRankScore[RANK_SCORE_MAX];	//ランキングのスコアの情報
RankingScore g_aRankNewScore;				//ランキングのニュースコアの情報

FileMode g_fileMode;						//ファイルの入出力の種類
bool g_bRankTrance;							//遷移の有無
int g_nNewScoreNumber = -1;					//ニュースコアの番号

//====================================
//= ランキングの初期化処理
//=====================================
void InitRanking(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_2D         *pVtx;					// 頂点情報へのポインタ

	for (int nCount = 0; nCount < TEXTURE_RANK_MAX; nCount++)
	{ // 使用するテクスチャ数分繰り返す

	  // テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, apTextureRank[nCount], &g_apTextureRank[nCount]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_2D) * 4 * MAX_RANK,	// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffRank,
		NULL
	);

	//スコアを取得
	int pScore = GetScore();

	//変数の初期化
	g_bRankTrance = false;

	if (GetTitleState() != TITLESTATE_FADE)
	{//タイトル遷移中じゃないとき

		//2Dポリゴン
		{
			//ランキング文字
			g_rank2DChar.pos = D3DXVECTOR3(RANK_CHAR_POS_X, RANK_CHAR_POS_Y, 0.0f);
			g_rank2DChar.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_rank2DChar.bUse = true;

			//ニュースコアの背景
			g_rank2DNewBg.pos = D3DXVECTOR3(RANK_NEW_BG_POS_X, RANK_NEW_BG_POS_Y, 0.0f);
			g_rank2DNewBg.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_rank2DNewBg.bUse = true;	//TRUEに変更

			//下線
			for (int nCount = 0; nCount < RANK_SCORE_MAX; nCount++)
			{
				g_rank2DLine[nCount].pos = D3DXVECTOR3(RANK_POS_X,
													   (RANK_POS_Y - RANK_LINE_POS_Y) + ((nCount * RANK_INTERVAL_Y) + RANK_LINE_HEIGHT), 0.0f);

				g_rank2DLine[nCount].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				g_rank2DLine[nCount].bUse = true;
			}

			//順位
			for (int nCount = 0; nCount < RANK_SCORE_MAX; nCount++)
			{
				g_rank2DRank[nCount].pos = D3DXVECTOR3(((RANK_POS_X - RANK_RANK_POS_X) - ((RANK_NUM_PLACE * RANK_INTERVAL_X))), (RANK_POS_Y + RANK_RANK_POS_Y) + ((nCount * RANK_INTERVAL_Y)), 0.0f);
				g_rank2DRank[nCount].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				g_rank2DRank[nCount].bUse = true;
			}
		}
		
		//数値
		{
			//スコアの情報を初期化
			for (int nCount = 0; nCount < RANK_SCORE_MAX; nCount++)
			{
				g_aRankScore[nCount].pos = D3DXVECTOR3(RANK_POS_X, RANK_POS_Y, 0.0f);
				g_aRankScore[nCount].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				g_aRankScore[nCount].bFlash = false;
				g_aRankScore[nCount].bUse = true;
			}

			//ニュースコアの情報を初期化
			{
				g_aRankNewScore.nScore = pScore;
				g_aRankNewScore.pos = D3DXVECTOR3(RANK_NEW_POS_X, RANK_NEW_POS_Y, 0.0f);
				g_aRankNewScore.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				g_aRankNewScore.bFlash = false;
				g_aRankNewScore.bUse = true;
			}
		}

	}
	else
	{
		//2Dポリゴン
		{
			//ランキング文字
			g_rank2DChar.pos = D3DXVECTOR3(RANK_FADE_CHAR_POS_X, RANK_FADE_CHAR_POS_Y, 0.0f);
			g_rank2DChar.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_rank2DChar.bUse = true;

			//ニュースコアの背景
			g_rank2DNewBg.pos = D3DXVECTOR3(RANK_NEW_BG_POS_X, RANK_NEW_BG_POS_Y, 0.0f);
			g_rank2DNewBg.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_rank2DNewBg.bUse = false;	//FALSEに変更

			//下線
			for (int nCount = 0; nCount < RANK_SCORE_MAX; nCount++)
			{

				g_rank2DLine[nCount].pos = D3DXVECTOR3(RANK_FADE_POS_X, 
													   (RANK_FADE_POS_Y - RANK_LINE_POS_Y) + ((nCount * RANK_INTERVAL_Y) + RANK_LINE_HEIGHT), 0.0f);

				g_rank2DLine[nCount].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				g_rank2DLine[nCount].bUse = true;
			}

			//順位
			for (int nCount = 0; nCount < RANK_SCORE_MAX; nCount++)
			{
				g_rank2DRank[nCount].pos = D3DXVECTOR3(((RANK_FADE_POS_X - RANK_RANK_POS_X) - ((RANK_NUM_PLACE * RANK_INTERVAL_X))),
													   (RANK_FADE_POS_Y + RANK_RANK_POS_Y) + ((nCount * RANK_INTERVAL_Y)), 0.0f);
				g_rank2DRank[nCount].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				g_rank2DRank[nCount].bUse = true;
			}
		}

		//数値
		{
			//スコアの情報を初期化
			for (int nCount = 0; nCount < RANK_SCORE_MAX; nCount++)
			{
				g_aRankScore[nCount].pos = D3DXVECTOR3(RANK_FADE_POS_X, RANK_FADE_POS_Y, 0.0f);
				g_aRankScore[nCount].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				g_aRankScore[nCount].bFlash = false;
				g_aRankScore[nCount].bUse = true;
			}

			//ニュースコアの情報を初期化
			{
				g_aRankNewScore.nScore = pScore;
				g_aRankNewScore.pos = D3DXVECTOR3(RANK_NEW_POS_X, RANK_NEW_POS_Y, 0.0f);
				g_aRankNewScore.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				g_aRankNewScore.bFlash = false;
				g_aRankNewScore.bUse = false;	//FALSEに変更
			}
		}
	}

	// 万能初期化の全体処理（3Dマップ）
	InitAllAroundChunk();

	// ファイルをロードする全体処理
	LoadFileChunk
	( // 引数
		true,	// 車のカーブ
		true,	// 人間のカーブ
		true,	// ステージ
		false,	// 当たり判定
		true,	// 影
		true,	// オブジェクト
		true,	// AI
		false	// アイコン
	);

	if (GetTitleState() != TITLESTATE_FADE)
	{//タイトル遷移中じゃないとき
		
		//スコアのソート
		SortRankingScore();

	}

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------------------------------------------------------------------------------------------
	//	ランキング（文字）の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[0].pos = D3DXVECTOR3(g_rank2DChar.pos.x - RANK_CHAR_WIDTH, g_rank2DChar.pos.y - RANK_CHAR_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_rank2DChar.pos.x + RANK_CHAR_WIDTH, g_rank2DChar.pos.y - RANK_CHAR_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_rank2DChar.pos.x - RANK_CHAR_WIDTH, g_rank2DChar.pos.y + RANK_CHAR_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_rank2DChar.pos.x + RANK_CHAR_WIDTH, g_rank2DChar.pos.y + RANK_CHAR_HEIGHT, 0.0f);

	// rhw の設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

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

	//------------------------------------------------------------------------------------------------------------------
	//	ランキング（ニュースコア背景）の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[4].pos = D3DXVECTOR3(g_rank2DNewBg.pos.x - RANK_NEW_BG_WIDTH, g_rank2DNewBg.pos.y - RANK_NEW_BG_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_rank2DNewBg.pos.x + RANK_NEW_BG_WIDTH, g_rank2DNewBg.pos.y - RANK_NEW_BG_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_rank2DNewBg.pos.x - RANK_NEW_BG_WIDTH, g_rank2DNewBg.pos.y + RANK_NEW_BG_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_rank2DNewBg.pos.x + RANK_NEW_BG_WIDTH, g_rank2DNewBg.pos.y + RANK_NEW_BG_HEIGHT, 0.0f);

	// rhw の設定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//------------------------------------------------------------------------------------------------------------------
	//	ランキング（下線）の初期化
	//------------------------------------------------------------------------------------------------------------------
	for (int nCount = 0; nCount < RANK_SCORE_MAX; nCount++)
	{ // スコア分回す
	  // 頂点座標を設定
		pVtx[8  + (nCount * 4)].pos = D3DXVECTOR3((g_rank2DLine[nCount].pos.x - RANK_WIDTH) - ((RANK_NUM_PLACE * RANK_INTERVAL_X) + RANK_LINE_WIDTH), (g_rank2DLine[nCount].pos.y - RANK_HEIGHT), 0.0f);
		pVtx[9  + (nCount * 4)].pos = D3DXVECTOR3((g_rank2DLine[nCount].pos.x + RANK_WIDTH + RANK_LINE_OVER_X),										  (g_rank2DLine[nCount].pos.y - RANK_HEIGHT), 0.0f);
		pVtx[10 + (nCount * 4)].pos = D3DXVECTOR3((g_rank2DLine[nCount].pos.x - RANK_WIDTH) - ((RANK_NUM_PLACE * RANK_INTERVAL_X) + RANK_LINE_WIDTH), (g_rank2DLine[nCount].pos.y + RANK_HEIGHT), 0.0f);
		pVtx[11 + (nCount * 4)].pos = D3DXVECTOR3((g_rank2DLine[nCount].pos.x + RANK_WIDTH + RANK_LINE_OVER_X),										  (g_rank2DLine[nCount].pos.y + RANK_HEIGHT), 0.0f);

		// rhw の設定
		pVtx[ 8 + (nCount * 4)].rhw = 1.0f;
		pVtx[ 9 + (nCount * 4)].rhw = 1.0f;
		pVtx[10 + (nCount * 4)].rhw = 1.0f;
		pVtx[11 + (nCount * 4)].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[ 8 + (nCount * 4)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[ 9 + (nCount * 4)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[10 + (nCount * 4)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[11 + (nCount * 4)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[ 8 + (nCount * 4)].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[ 9 + (nCount * 4)].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[10 + (nCount * 4)].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[11 + (nCount * 4)].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//------------------------------------------------------------------------------------------------------------------
	//	ランキング（順位）の初期化
	//------------------------------------------------------------------------------------------------------------------
	for (int nCount = 0; nCount < RANK_SCORE_MAX; nCount++)
	{ // スコア分回す
	  // 頂点座標を設定
		pVtx[28 + (nCount * 4)].pos = D3DXVECTOR3((g_rank2DRank[nCount].pos.x - RANK_RANK_WIDTH), (g_rank2DRank[nCount].pos.y - RANK_RANK_HEIGHT), 0.0f);
		pVtx[29 + (nCount * 4)].pos = D3DXVECTOR3((g_rank2DRank[nCount].pos.x + RANK_RANK_WIDTH), (g_rank2DRank[nCount].pos.y - RANK_RANK_HEIGHT), 0.0f);
		pVtx[30 + (nCount * 4)].pos = D3DXVECTOR3((g_rank2DRank[nCount].pos.x - RANK_RANK_WIDTH), (g_rank2DRank[nCount].pos.y + RANK_RANK_HEIGHT), 0.0f);
		pVtx[31 + (nCount * 4)].pos = D3DXVECTOR3((g_rank2DRank[nCount].pos.x + RANK_RANK_WIDTH), (g_rank2DRank[nCount].pos.y + RANK_RANK_HEIGHT), 0.0f);

		// rhw の設定
		pVtx[28 + (nCount * 4)].rhw = 1.0f;
		pVtx[29 + (nCount * 4)].rhw = 1.0f;
		pVtx[30 + (nCount * 4)].rhw = 1.0f;
		pVtx[31 + (nCount * 4)].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[28 + (nCount * 4)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[29 + (nCount * 4)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[30 + (nCount * 4)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[31 + (nCount * 4)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[28 + (nCount * 4)].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[29 + (nCount * 4)].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[30 + (nCount * 4)].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[31 + (nCount * 4)].tex = D3DXVECTOR2(1.0f, 1.0f);
	}


	// 頂点バッファをアンロックする
	g_pVtxBuffRank->Unlock();
}

//=====================================
//= ランキングの終了処理
//=====================================
void UninitRanking(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	ランキング2Dの終了
	//------------------------------------------------------------------------------------------------------------------
	// テクスチャの破棄
	for (int nCount = 0; nCount < TEXTURE_RANK_MAX; nCount++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTextureRank[nCount] != NULL)
		{ // 変数 (g_apTextureTitle) がNULLではない場合

			g_apTextureRank[nCount]->Release();
			g_apTextureRank[nCount] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffRank != NULL)
	{ // 変数 (g_pVtxBuffTitle) がNULLではない場合

		g_pVtxBuffRank->Release();
		g_pVtxBuffRank = NULL;
	}

	//ニュースコアの番号
	g_nNewScoreNumber = -1;

	//ランキングの値をファイルに書き出し
	SaveRanking();

	// 万能終了の全体処理（3Dマップ）
	UninitAllAroundChunk();

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
			if (g_aRankScore[g_nNewScoreNumber].col.a <= RANK_FLASH_MIN)
			{
				g_aRankScore[g_nNewScoreNumber].bFlash = true;
			}
		}
		else if (g_aRankScore[g_nNewScoreNumber].bFlash == true)
		{
			g_aRankScore[g_nNewScoreNumber].col.a += RANK_FLASH_INTERVAL;
			if (g_aRankScore[g_nNewScoreNumber].col.a >= RANK_FLASH_MAX)
			{
				g_aRankScore[g_nNewScoreNumber].bFlash = false;
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
	// 万能描画の全体処理
	DrawAllAroundChunk();

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRank, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//2Dポリゴンの描画
	{ // 使用するポリゴン数分繰り返す

		//ランキング（文字）
		if (g_rank2DChar.bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureRank[TEXTURE_RANK_CHAR]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}

		//ニュースコア背景
		if (g_rank2DNewBg.bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureRank[TEXTURE_RANK_UI_004]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
		}

		//下線
		for (int nCount = 0; nCount < RANK_SCORE_MAX; nCount++)
		{
			if (g_rank2DLine[nCount].bUse == true)
			{
				// テクスチャの設定
				pDevice->SetTexture(0, g_apTextureRank[TEXTURE_RANK_LINE]);

				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8 + (4 * nCount), 2);
			}
		}

		//順位
		for (int nCount = 0; nCount < RANK_SCORE_MAX; nCount++)
		{
			if (g_rank2DRank[nCount].bUse == true)
			{
				// テクスチャの設定
				pDevice->SetTexture(0, g_apTextureRank[TEXTURE_RANK_1ST + nCount]);

				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 28 + (4 * nCount), 2);
			}
		}
	}

	if (GetTitleState() != TITLESTATE_FADE)
	{//タイトル遷移中じゃないとき
	 //ランキングスコアの数値
		for (int nCount = 0; nCount < RANK_SCORE_MAX; nCount++)
		{ // 順位が更新されている場合

		  //数値を設定
			SetValue(
				D3DXVECTOR3(g_aRankScore[nCount].pos.x, g_aRankScore[nCount].pos.y + (nCount * RANK_INTERVAL_Y), 0.0f),
				g_aRankScore[nCount].nScore,
				RANK_MAX_NUMBER,
				RANK_WIDTH,
				RANK_HEIGHT,
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
				RANK_NEW_WIDTH,
				RANK_NEW_HEIGHT,
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
				RANK_FADE_WIDTH,
				RANK_FADE_HEIGHT,
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
		if (g_aRankNewScore.nScore >= g_aRankScore[(RANK_SCORE_MAX - 1)].nScore ||
			g_aRankScore[(RANK_SCORE_MAX - 1)].nScore == 0)
		{//ニュースコアが一番低いスコアより大きいとき
			g_aRankScore[(RANK_SCORE_MAX - 1)].nScore = g_aRankNewScore.nScore;
			g_nNewScoreNumber = (RANK_SCORE_MAX - 1);

			//要素1のデータと要素2のデータを比較
			for (nCutRank = 0; nCutRank < (RANK_SCORE_MAX - 1); nCutRank++)
			{
				nCountData = nCutRank;
				//置き換え番号に要素1のデータ番号を代入
				for (nCutScoreRank = nCutRank; nCutScoreRank < RANK_SCORE_MAX; nCutScoreRank++)
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

	for (int nCntScore = 0; nCntScore < RANK_SCORE_MAX; nCntScore++)
	{
		// スコアを初期化する
		g_aRankScore[nCntScore].nScore = 0;
	}

	//ランキングのスコアの値をファイルから読み込み
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
			for (int nCount = 0; nCount < RANK_SCORE_MAX; nCount++)
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
			for (int nCount = 0; nCount < RANK_SCORE_MAX; nCount++)
			{
				fwrite(&g_aRankScore[nCount].nScore, sizeof(int), RANK_SCORE_MAX, pFile);
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
			for (int nCount = 0; nCount < RANK_SCORE_MAX; nCount++)
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
			for (int nCount = 0; nCount < RANK_SCORE_MAX; nCount++)
			{
				fread(&g_aRankScore[nCount].nScore, sizeof(int), RANK_SCORE_MAX, pFile);
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