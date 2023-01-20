//============================================================
//
//	モデルヘッダー [model.h]
//	Author：藤田勇一
//
//============================================================
#ifndef _MODEL_H_	// このマクロ定義がされていない場合
#define _MODEL_H_	// 二重インクルード防止のマクロを定義する

//************************************************************
//	列挙型定義 (MODELTYPE)
//************************************************************
typedef enum
{
	// オブジェクトモデル
	MODELTYPE_OBJECT_TREE = 0,		// 木
	MODELTYPE_OBJECT_ROCK,			// 岩
	MODELTYPE_OBJECT_FOREST,		// 森
	MODELTYPE_OBJECT_BIGTREE,		// 大木
	MODELTYPE_OBJECT_YELLOWTREE,	// 黄色い木
	MODELTYPE_OBJECT_GRASS,			// 草
	//MODELTYPE_OBJECT_APART,		// アパート
	MODELTYPE_OBJECT_HOUSE,			// 家
	MODELTYPE_OBJECT_CURRYSHOP,		// カレー屋
	MODELTYPE_OBJECT_BILL,			// ビル
	//MODELTYPE_OBJECT_POLICE,		// 仮の警察

	// プレイヤーモデル
	MODELTYPE_PLAYER_CAR,			// 車
	MODELTYPE_MAX,					// 全モデルの総数
} MODELTYPE;

//************************************************************
//	マクロ定義
//************************************************************
#define INIT_VTX_MIN	(D3DXVECTOR3( 9999.0f,  9999.0f,  9999.0f))		// モデルの最小の頂点座標の初期値
#define INIT_VTX_MAX	(D3DXVECTOR3(-9999.0f, -9999.0f, -9999.0f))		// モデルの最大の頂点座標の初期値
#define FROM_OBJECT				(0)	// オブジェクトのモデル列挙の開始地点
#define MODELTYPE_OBJECT_MAX	(9)	// オブジェクトのモデルの総数
#define FROM_PLAYER				(9)	// プレイヤーのモデル列挙の開始地点
#define MODELTYPE_PLAYER_MAX	(1)	// プレイヤーのモデルの総数

//************************************************************
//	構造体定義 (Model)
//************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9 *pTexture;	// テクスチャへのポインタ
	LPD3DXMESH         pMesh;		// メッシュ (頂点情報) へのポインタ
	LPD3DXBUFFER       pBuffMat;	// マテリアルへのポインタ
	DWORD              dwNumMat;	// マテリアルの数
	D3DXVECTOR3        vtxMin;		// 最小の頂点座標
	D3DXVECTOR3        vtxMax;		// 最大の頂点座標
	float              fHeight;		// 縦幅
	float              fRadius;		// 半径
}Model;

//************************************************************
//	プロトタイプ宣言
//************************************************************
HRESULT InitModel(void);			// モデルの初期化処理
void UninitModel(void);				// モデルの終了処理
Model GetModelData(int nID);		// モデル情報の取得処理

#endif