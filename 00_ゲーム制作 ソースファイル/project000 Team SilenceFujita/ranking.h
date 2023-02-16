
//=======================
//=
//= ランキングのヘッター[ranking.h]
//= Author Sakai Minato
//=
//=======================

#ifndef _RANKING_H_			//このマクロが定義されなかったら
#define _RANKING_H_			//二重インクルード帽子のマクロを定義

//**********************************************
//* 列挙型
//**********************************************
//**********************************
//* ファイルの入出力の種類
//**********************************
typedef enum
{
	FILE_MODE_TXT = 0,	//テキストモードの入出力
	FILE_MODE_BIN,		//バイナリモードの入出力
	FILE_MODE_END
}FileMode;

//プロトタイプ宣言
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void SortRankingScore(void);
void ResetRanking(void);
void SaveRanking(void);
void LoadRanking(void);
void SetRankingRoadTrance(void);

#endif 