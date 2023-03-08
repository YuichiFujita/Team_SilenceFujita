//======================================================================================================================
//
//	スタート処理 [start.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "game.h"
#include "start.h"
#include "input.h"
#include "2Deffect.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define NONE_START_TEXTURE				(-1)																// テクスチャがいらない番号

#define START_PREFACE_FIRST_RADIUS		(D3DXVECTOR3(200.0f, 40.0f, 0.0f))									// 前置き(前半)の半径
#define START_PREFACE_SECOND_RADIUS		(D3DXVECTOR3(200.0f, 40.0f, 0.0f))									// 前置き(後半)の半径
#define START_SIGLIGHT_INIT_RADIUS		(D3DXVECTOR3(0.0f, 60.0f, 0.0f))									// シグナルの光源の全長
#define START_SIGNAL_RADIUS				(D3DXVECTOR3(250.0f, 80.0f, 0.0f))									// シグナルの半径
#define START_GO_RADIUS					(D3DXVECTOR3(200.0f, 100.0f, 0.0f))									// Goの半径

#define START_PREFACE_FIRST_INIT_POS	(D3DXVECTOR3(-250.0f, 250.0f, 0.0f))													// 前置き(前半)の初期位置
#define START_PREFACE_SECOND_INIT_POS	(D3DXVECTOR3(SCREEN_WIDTH + 250.0f, 350.0f, 0.0f))										// 前置き(後半)の初期位置
#define START_SIGLIGHT_INIT_POS			(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - START_SIGNAL_RADIUS.x + 5.0f, -80.0f, 0.0f))				// シグナルの光源の初期位置
#define START_SIGNAL_INIT_POS			(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, -80.0f, 0.0f))										// シグナルの初期位置
#define START_GO_INIT_POS				(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f))							// Goの初期位置

#define START_PREFACE_FIRST_POS			(D3DXVECTOR3(600.0f, 250.0f, 0.0f))									// 前置き(前半)の移動した後の位置
#define START_PREFACE_SECOND_POS		(D3DXVECTOR3(750.0f, 250.0f, 0.0f))									// 前置き(後半)の移動した後の位置
#define START_SIGLIGHT_POS				(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - START_SIGNAL_RADIUS.x, SCREEN_HEIGHT * 0.5f, 0.0f))	// シグナルの光源の移動した後の位置
#define START_SIGNAL_POS				(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f))							// シグナルの移動した後の位置
#define START_SIGLIGHT_RADIUS			(D3DXVECTOR3(490.0f, 60.0f, 0.0f))									// シグナルの光源の処理後の半径

#define START_PREFACE_MOVE				(5.5f)			// 前置きの移動量
#define START_PREFACE_ALPHA_MOVE		(0.04f)			// 透明度の移動量
#define START_PREFACE_WAIT_COUNT		(90)			// 前置きの待機カウント数
#define START_PREFACE_WAIT_COUNT		(90)			// 前置きの待機カウント数
#define START_SIGNAL_MOVE				(4.0f)			// シグナルの移動量
#define START_SIGNAL_COUNT				(40)			// シグナルのカウント
#define START_SIGNAL_LIGHT_MAX			(3)				// シグナルが光る回数
#define START_SIGNAL_NORMAL_COL			(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))									// 通常のシグナルの色
#define START_SIGNAL_START_COL			(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f))									// スタート時のシグナルの色
#define START_GO_ALPHA_MOVE				(0.01f)			// GO!!の透明度の移動量
#define START_GO_RADIUS_ADD				(2.0f)			// GO!!の半径の追加量
#define START_SIGNAL_PARTICLE_COL		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))									// パーティクルの色

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTextureStart[] =			// テクスチャの相対パス
{
	"data\\TEXTURE\\start000.png",		// スタートコロン(：) (通常) のテクスチャ相対パス
	"data\\TEXTURE\\start001.png",		// スタートコロン(：) (赤) のテクスチャ相対パス
	"data\\TEXTURE\\StartSignal.png",	// シグナルのテクスチャ相対パス
	"data\\TEXTURE\\start001.png",		// スタートコロン(：) (赤) のテクスチャ相対パス
};

//**********************************************************************************************************************
//	列挙型定義 (TEXTURE_START)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_START_PREFACE_FIRST = 0,		// 前置き前半
	TEXTURE_START_PREFACE_SECOND,			// 前置き後半
	TEXTURE_START_SIGNAL,					// シグナル
	TEXTURE_START_GO,						// Go!!
	TEXTURE_START_MAX,						// この列挙型の総数
} TEXTURE_START;

//**********************************************************************************************************************
//	列挙型定義 (START_POLIGON)
//**********************************************************************************************************************
typedef enum
{
	START_POLIGON_PREFACE_FIRST = 0,		// 前置き前半
	START_POLIGON_PREFACE_SECOND,			// 前置き後半
	START_POLIGON_SIGLIGHT,					// シグナルの光源
	START_POLIGON_SIGNAL,					// シグナル
	START_POLIGON_GO,						// Go!!
	START_POLIGON_MAX,						// この列挙型の総数
}START_POLIGON;

//**********************************************************************************************************************
//	構造体定義（Start）
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;						// 位置
	D3DXVECTOR3 move;						// 移動量
	D3DXVECTOR3 radius;						// 半径
	D3DXCOLOR	col;						// 色
	int			nNumTex;					// テクスチャの番号
	bool		bUse;						// 使用状況
}Start;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void StartPrefaceFirst(void);				// 前置き(前半)の処理
void StartPrefaceSecond(void);				// 前置き(後半)の処理
void StartPrefaceThird(void);				// 前置き(画面からはける)の処理
void StartSignalMove(void);					// シグナルの移動処理
void StartSignal(void);						// シグナルの処理
void StartGo(void);							// GO!!表示処理

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureStart[TEXTURE_START_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStart = NULL;						// 頂点バッファへのポインタ

Start	   g_aStart[START_POLIGON_MAX];								// それぞれの演出の位置
STARTSTATE g_StartState;											// スタートの状態
int		   g_nActionCount;											// 行動のカウント
int		   g_nSignalCount;											// シグナルの変わった回数

//======================================================================================================================
//	スタートの初期化処理
//======================================================================================================================
void InitStart(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_2D         *pVtx;					// 頂点情報へのポインタ

	for (int nCntStart = 0; nCntStart < TEXTURE_START_MAX; nCntStart++)
	{ // 使用するテクスチャ数分繰り返す

	  // テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, apTextureStart[nCntStart], &g_apTextureStart[nCntStart]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_2D) * 4 * START_POLIGON_MAX,		// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffStart,
		NULL
	);

	// グローバル変数を初期化
	for (int nCntPara = 0; nCntPara < START_POLIGON_MAX; nCntPara++)
	{
		switch (nCntPara)
		{
		case START_POLIGON_PREFACE_FIRST:		// 前置き(前半)

			g_aStart[nCntPara].pos = START_PREFACE_FIRST_INIT_POS;			// 位置

			g_aStart[nCntPara].radius = START_PREFACE_FIRST_RADIUS;			// 半径

			g_aStart[nCntPara].nNumTex = TEXTURE_START_PREFACE_FIRST;		// テクスチャの番号

			g_aStart[nCntPara].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色

			break;

		case START_POLIGON_PREFACE_SECOND:		// 前置き(後半)

			g_aStart[nCntPara].pos = START_PREFACE_SECOND_INIT_POS;			// 位置

			g_aStart[nCntPara].radius = START_PREFACE_SECOND_RADIUS;		// 半径

			g_aStart[nCntPara].nNumTex = TEXTURE_START_PREFACE_SECOND;		// テクスチャの番号

			g_aStart[nCntPara].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色

			break;

		case START_POLIGON_SIGLIGHT:			// シグナルの光源

			g_aStart[nCntPara].pos = START_SIGLIGHT_INIT_POS;				// 位置

			g_aStart[nCntPara].radius = START_SIGLIGHT_INIT_RADIUS;			// 全長

			g_aStart[nCntPara].nNumTex = NONE_START_TEXTURE;				// テクスチャの番号

			g_aStart[nCntPara].col = START_SIGNAL_NORMAL_COL;				// 色

			break;

		case START_POLIGON_SIGNAL:				// シグナル

			g_aStart[nCntPara].pos = START_SIGNAL_INIT_POS;					// 位置

			g_aStart[nCntPara].radius = START_SIGNAL_RADIUS;				// 半径

			g_aStart[nCntPara].nNumTex = TEXTURE_START_SIGNAL;				// テクスチャの番号

			g_aStart[nCntPara].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色

			break;

		case START_POLIGON_GO:					// Go!!

			g_aStart[nCntPara].pos = START_GO_INIT_POS;						// 位置

			g_aStart[nCntPara].radius = START_GO_RADIUS;					// 半径

			g_aStart[nCntPara].nNumTex = TEXTURE_START_GO;					// テクスチャの番号
																			
			g_aStart[nCntPara].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色

			break;

		default:								// 上記以外

			break;
		}

		// 使用状況
		g_aStart[nCntPara].bUse = false;

		// 移動量を初期化する
		g_aStart[nCntPara].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// スタートの状態を設定する
	g_StartState = STARTSTATE_PREFACE_FIRST;

	// 行動カウントを初期化する
	g_nActionCount = 0;

	// シグナルの変わった回数を初期化する
	g_nSignalCount = 0;

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStart = 0; nCntStart < START_POLIGON_MAX; nCntStart++)
	{
		switch (nCntStart)
		{
		case START_POLIGON_SIGLIGHT:			// シグナルの光源

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x, g_aStart[nCntStart].pos.y - g_aStart[nCntStart].radius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x + g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y - g_aStart[nCntStart].radius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x, g_aStart[nCntStart].pos.y + g_aStart[nCntStart].radius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x + g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y + g_aStart[nCntStart].radius.y, 0.0f);

			break;

		default:								// 上記以外

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x - g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y - g_aStart[nCntStart].radius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x + g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y - g_aStart[nCntStart].radius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x - g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y + g_aStart[nCntStart].radius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x + g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y + g_aStart[nCntStart].radius.y, 0.0f);

			break;
		}

		// rhw の設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = g_aStart[nCntStart].col;
		pVtx[1].col = g_aStart[nCntStart].col;
		pVtx[2].col = g_aStart[nCntStart].col;
		pVtx[3].col = g_aStart[nCntStart].col;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;				// 頂点データを4つ分進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffStart->Unlock();
}

//======================================================================================================================
//	スタートの終了処理
//======================================================================================================================
void UninitStart(void)
{
	// テクスチャの破棄
	for (int nCntStart = 0; nCntStart < TEXTURE_START_MAX; nCntStart++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTextureStart[nCntStart] != NULL)
		{ // 変数 (g_apTextureStart) がNULLではない場合

			g_apTextureStart[nCntStart]->Release();
			g_apTextureStart[nCntStart] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffStart != NULL)
	{ // 変数 (g_pVtxBuffStart) がNULLではない場合

		g_pVtxBuffStart->Release();
		g_pVtxBuffStart = NULL;
	}
}

//======================================================================================================================
//	スタートの更新処理
//======================================================================================================================
void UpdateStart(void)
{
	if (GetKeyboardTrigger(DIK_0) == true)
	{
		// 0を押した場合
		g_StartState = STARTSTATE_PREFACE_FIRST;
	}

	VERTEX_2D         *pVtx;					// 頂点情報へのポインタ

	switch (g_StartState)
	{
	case STARTSTATE_NONE:		// 無状態

		for (int nCntNone = 0; nCntNone < START_POLIGON_MAX; nCntNone++)
		{
			// ポリゴン全てを使用しない
			g_aStart[nCntNone].bUse = false;
		}

		break;

	case STARTSTATE_PREFACE_FIRST:	// 前置き前半

		// 使用する
		g_aStart[START_POLIGON_PREFACE_FIRST].bUse = true;

		// 前置き(前半)の処理
		StartPrefaceFirst();

		break;

	case STARTSTATE_PREFACE_SECOND:	// 前置き後半

		// 前置き(後半)の処理
		StartPrefaceSecond();

		break;

	case STARTSTATE_PREFACE_THIRD:	// 前置き最終(画面からはける)

		// 前置き(画面からはける)の処理
		StartPrefaceThird();

		break;

	case STARTSTATE_SIGNAL_MOVE:	// シグナルの移動状態

		// シグナルの移動処理
		StartSignalMove();

		break;

	case STARTSTATE_SIGNAL:			// シグナル状態

		// シグナルの処理
		StartSignal();

		break;

	case STARTSTATE_GO:				// Go!!状態

		// GO!!表示処理
		StartGo();

		break;
	}

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStart = 0; nCntStart < START_POLIGON_MAX; nCntStart++)
	{
		if (g_aStart[nCntStart].bUse == true)
		{ // 使用している場合

			switch (nCntStart)
			{
			case START_POLIGON_SIGLIGHT:			// シグナルの光源

				// 頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x, g_aStart[nCntStart].pos.y - g_aStart[nCntStart].radius.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x + g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y - g_aStart[nCntStart].radius.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x, g_aStart[nCntStart].pos.y + g_aStart[nCntStart].radius.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x + g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y + g_aStart[nCntStart].radius.y, 0.0f);

				break;

			default:								// 上記以外

				// 頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x - g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y - g_aStart[nCntStart].radius.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x + g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y - g_aStart[nCntStart].radius.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x - g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y + g_aStart[nCntStart].radius.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x + g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y + g_aStart[nCntStart].radius.y, 0.0f);

				break;
			}

			// 頂点カラーの設定
			pVtx[0].col = g_aStart[nCntStart].col;
			pVtx[1].col = g_aStart[nCntStart].col;
			pVtx[2].col = g_aStart[nCntStart].col;
			pVtx[3].col = g_aStart[nCntStart].col;
		}

		pVtx += 4;			// 頂点データを4つ分進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffStart->Unlock();
}

//======================================================================================================================
//	スタートの描画処理
//======================================================================================================================
void DrawStart(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffStart, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntDraw = 0; nCntDraw < START_POLIGON_MAX; nCntDraw++)
	{
		if (g_aStart[nCntDraw].bUse == true)
		{ // 使用している場合

			if (g_aStart[nCntDraw].nNumTex == NONE_START_TEXTURE)
			{ // テクスチャ番号がない場合

				// テクスチャの設定
				pDevice->SetTexture(0, NULL);
			}
			else
			{ // テクスチャ番号がある場合

				// テクスチャの設定
				pDevice->SetTexture(0, g_apTextureStart[g_aStart[nCntDraw].nNumTex]);
			}

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntDraw, 2);
		}
	}
}

//======================================================================================================================
// 前置き(前半)の処理
//======================================================================================================================
void StartPrefaceFirst(void)
{
	// 移動量を更新する
	g_aStart[START_POLIGON_PREFACE_FIRST].move.x += START_PREFACE_MOVE;
	
	// 位置を更新する
	g_aStart[START_POLIGON_PREFACE_FIRST].pos.x += g_aStart[START_POLIGON_PREFACE_FIRST].move.x;

	if (g_aStart[START_POLIGON_PREFACE_FIRST].pos.x >= START_PREFACE_FIRST_POS.x)
	{ // 前置き(前半)が一定の位置まで移動した場合

		// 前置きの位置を補正する
		g_aStart[START_POLIGON_PREFACE_FIRST].pos.x = START_PREFACE_FIRST_POS.x;

		// 前置き(後半)を使用可能にする
		g_aStart[START_POLIGON_PREFACE_SECOND].bUse = true;

		// 前置き(後半)状態にする
		g_StartState = STARTSTATE_PREFACE_SECOND;
	}
}

//======================================================================================================================
// 前置き(後半)の処理
//======================================================================================================================
void StartPrefaceSecond(void)
{
	// 移動量を更新する
	g_aStart[START_POLIGON_PREFACE_SECOND].move.x -= START_PREFACE_MOVE;

	// 位置を更新する
	g_aStart[START_POLIGON_PREFACE_SECOND].pos.x += g_aStart[START_POLIGON_PREFACE_SECOND].move.x;

	if (g_aStart[START_POLIGON_PREFACE_SECOND].pos.x <= START_PREFACE_SECOND_POS.x)
	{ // 前置き(後半)が一定の位置の位置まで移動した場合

		// 前置きの位置を補正する
		g_aStart[START_POLIGON_PREFACE_SECOND].pos.x = START_PREFACE_SECOND_POS.x;

		// 状態を画面からはける状態にする
		g_StartState = STARTSTATE_PREFACE_THIRD;

		// 行動カウントを初期化する
		g_nActionCount = 0;
	}
}

//======================================================================================================================
// 前置き(画面からはける)の処理
//======================================================================================================================
void StartPrefaceThird(void)
{
	if (g_nActionCount >= START_PREFACE_WAIT_COUNT)
	{ // 待機カウントになった場合

		// 透明度を下げる
		g_aStart[START_POLIGON_PREFACE_FIRST].col.a -= START_PREFACE_ALPHA_MOVE;
		g_aStart[START_POLIGON_PREFACE_SECOND].col.a -= START_PREFACE_ALPHA_MOVE;

		if (g_aStart[START_POLIGON_PREFACE_FIRST].col.a <= 0.0f)
		{ // 透明度が0.0f以下になったら

			// 透明度を0.0fにする
			g_aStart[START_POLIGON_PREFACE_FIRST].col.a = 0.0f;
			g_aStart[START_POLIGON_PREFACE_SECOND].col.a = 0.0f;

			// 使用しない
			g_aStart[START_POLIGON_PREFACE_FIRST].bUse = false;
			g_aStart[START_POLIGON_PREFACE_SECOND].bUse = false;

			// 状態をシグナルの移動状態にする
			g_StartState = STARTSTATE_SIGNAL_MOVE;

			// シグナル系を使用する
			g_aStart[START_POLIGON_SIGLIGHT].bUse = true;
			g_aStart[START_POLIGON_SIGNAL].bUse = true;

			// 行動カウントを初期化する
			g_nActionCount = 0;
		}
	}
	else
	{ // 待機カウント以内だった場合

		// 行動カウントを加算する
		g_nActionCount++;
	}
}

//======================================================================================================================
// シグナルの処理
//======================================================================================================================
void StartSignalMove(void)
{
	// 移動量を更新する
	g_aStart[START_POLIGON_SIGLIGHT].move.y += START_SIGNAL_MOVE;
	g_aStart[START_POLIGON_SIGNAL].move.y += START_SIGNAL_MOVE;

	// 位置を更新する
	g_aStart[START_POLIGON_SIGLIGHT].pos.y += g_aStart[START_POLIGON_SIGLIGHT].move.y;
	g_aStart[START_POLIGON_SIGNAL].pos.y += g_aStart[START_POLIGON_SIGNAL].move.y;

	if (g_aStart[START_POLIGON_SIGLIGHT].pos.y >= START_SIGLIGHT_POS.y
	 || g_aStart[START_POLIGON_SIGNAL].pos.y >= START_SIGNAL_POS.y)
	{ // 一定の位置以上になった場合

		// 位置を補正する
		g_aStart[START_POLIGON_SIGLIGHT].pos.y = START_SIGLIGHT_POS.y;
		g_aStart[START_POLIGON_SIGNAL].pos.y = START_SIGNAL_POS.y;

		// 移動量を初期化する
		g_aStart[START_POLIGON_SIGLIGHT].move.y = 0.0f;
		g_aStart[START_POLIGON_SIGNAL].move.y = 0.0f;

		// 状態をシグナルの状態にする
		g_StartState = STARTSTATE_SIGNAL;
	}
}

//======================================================================================================================
// シグナルの処理
//======================================================================================================================
void StartSignal(void)
{
	// 行動カウントを加算する
	g_nActionCount++;

	if (g_nActionCount % START_SIGNAL_COUNT == 0)
	{ // 一定数経った場合

		if (g_nSignalCount < START_SIGNAL_LIGHT_MAX)
		{ // シグナルが光った回数が一定数以内だった場合

			// シグナルカウントを加算する
			g_nSignalCount++;

			//　半径を設定する
			g_aStart[START_POLIGON_SIGLIGHT].radius.x = (START_SIGLIGHT_RADIUS.x / START_SIGNAL_LIGHT_MAX) * g_nSignalCount;
		}
		else
		{ // 上記以外

			// 色を設定する
			g_aStart[START_POLIGON_SIGLIGHT].col = START_SIGNAL_START_COL;

			// 状態をGo!!状態にする
			g_StartState = STARTSTATE_GO;

			// 使用する
			g_aStart[START_POLIGON_GO].bUse = true;

			// ゲームの状態を通常にする
			*GetGameState() = GAMESTATE_NORMAL;

			// 行動カウントを初期化する
			g_nActionCount = 0;
		}
	}
}

//======================================================================================================================
// GO!!表示処理
//======================================================================================================================
void StartGo(void)
{
	// 透明度を更新する
	g_aStart[START_POLIGON_GO].col.a -= START_GO_ALPHA_MOVE;

	// 半径を追加する
	g_aStart[START_POLIGON_GO].radius.x += START_GO_RADIUS_ADD;
	g_aStart[START_POLIGON_GO].radius.y += START_GO_RADIUS_ADD;

	// 行動カウントを設定する
	g_nActionCount++;

	if (g_nActionCount >= 60)
	{ // 行動カウントが一定以上だった場合

		// シグナルの移動量を増やす
		g_aStart[START_POLIGON_SIGLIGHT].move.y -= START_SIGNAL_MOVE;
		g_aStart[START_POLIGON_SIGNAL].move.y -= START_SIGNAL_MOVE;

		// シグナルを移動させる
		g_aStart[START_POLIGON_SIGLIGHT].pos.y += g_aStart[START_POLIGON_SIGLIGHT].move.y;
		g_aStart[START_POLIGON_SIGNAL].pos.y += g_aStart[START_POLIGON_SIGLIGHT].move.y;
	}

	if (g_aStart[START_POLIGON_GO].col.a <= 0.0f)
	{ // Go!!の透明度が0.0fいかになった場合

		// 透明度を補正する
		g_aStart[START_POLIGON_SIGNAL].col.a = 0.0f;
		g_aStart[START_POLIGON_SIGLIGHT].col.a = 0.0f;
		g_aStart[START_POLIGON_GO].col.a = 0.0f;

		// 無状態にする
		g_StartState = STARTSTATE_NONE;
	}
}

//======================================================================================================================
//	スタートの状態の取得処理
//======================================================================================================================
STARTSTATE GetStartState(void)
{
	// スタートの状態を返す
	return g_StartState;
}