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
	MODELTYPE_OBJECT_TREE = 0,			// 木
	MODELTYPE_OBJECT_ROCK,				// 岩
	MODELTYPE_OBJECT_FOREST,			// 森
	MODELTYPE_OBJECT_BIGTREE,			// 大木
	MODELTYPE_OBJECT_YELLOWTREE,		// 黄色い木
	MODELTYPE_OBJECT_GRASS,				// 草
	MODELTYPE_OBJECT_HOUSE,				// 家
	MODELTYPE_OBJECT_CURRYSHOP,			// カレー屋
	MODELTYPE_OBJECT_BILL001,			// ビルver1
	MODELTYPE_OBJECT_BILL002,			// ビルver2
	MODELTYPE_OBJECT_ARCS,				// スーパー
	MODELTYPE_OBJECT_BAKERY,			// パン屋
	MODELTYPE_OBJECT_CITYOFFECE,		// 市役所
	MODELTYPE_OBJECT_GYMNASIUM,			// 体育館
	MODELTYPE_OBJECT_CASINO,			// カジノ
	MODELTYPE_OBJECT_SANDBOX,			// 公園（砂場）
	MODELTYPE_OBJECT_TOILET,			// 公園（トイレ）
	MODELTYPE_OBJECT_WATER,				// 公園（水道）
	MODELTYPE_OBJECT_BIN,				// 公園（ゴミ箱）
	MODELTYPE_OBJECT_BENCH,				// 公園（ベンチ）
	MODELTYPE_OBJECT_STREETLIGHT,		// 公園（街灯）
	MODELTYPE_OBJECT_HORIZONTALBAR,		// 公園（鉄棒）
	MODELTYPE_OBJECT_SLIDE,				// 公園（滑り台）
	MODELTYPE_OBJECT_APART,				// アパート
	MODELTYPE_OBJECT_GASSTAND,			// ガソリンスタンド
	MODELTYPE_OBJECT_CONVENI,			// コンビニ
	MODELTYPE_OBJECT_JOYFAT,			// ジム
	MODELTYPE_OBJECT_TOWERCRANE,		// タワーレーン
	MODELTYPE_OBJECT_NETCAFE,			// ネットカフェ
	MODELTYPE_OBJECT_HOUSE002,			// 家（2）
	MODELTYPE_OBJECT_HOUSE003,			// 家（3）
	MODELTYPE_OBJECT_SCHOOL,			// 学校
	MODELTYPE_OBJECT_DENTIST,			// 歯医者
	MODELTYPE_OBJECT_SIGNAL,			// 信号
	MODELTYPE_OBJECT_HOSPITAL,			// 病院

	// プレイヤーモデル
	MODELTYPE_PLAYER_CAR,				// プレイヤー
	MODELTYPE_PLAYER_BARRIER,			// バリア

	// 車モデル
	MODELTYPE_CAR_POLICE,				// パトカー
	MODELTYPE_CAR_FARECAR,				// 消防車
	MODELTYPE_CAR_CAR001,				// 自動車
	MODELTYPE_CAR_YAKIIMOCAR,			// 石焼き芋屋
	MODELTYPE_CAR_ELECTIONCAR,			// 選挙カー

	// エディットモデル
	MODELTYPE_EDIT_COLLISION,			// 当たり判定
	MODELTYPE_MAX,						// 全モデルの総数
} MODELTYPE;

////************************************************************
////	列挙型定義 (MODELTYPE)
////************************************************************
//typedef enum
//{
//	MODELTYPE_OBJECT = 0,	// オブジェクトモデル
//	MODELTYPE_PLAYER,		// プレイヤーモデル
//	MODELTYPE_CAR,			// 車モデル
//	MODELTYPE_EDIT,			// エディットモデル
//	MODELTYPE_MAX,			// モデルの種類の総数
//} MODELTYPE;

//************************************************************
//	マクロ定義
//************************************************************
#define INIT_VTX_MIN	(D3DXVECTOR3( 9999.0f,  9999.0f,  9999.0f))		// モデルの最小の頂点座標の初期値
#define INIT_VTX_MAX	(D3DXVECTOR3(-9999.0f, -9999.0f, -9999.0f))		// モデルの最大の頂点座標の初期値
#define INIT_SIZE		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))					// モデルサイズの初期値

#define FROM_OBJECT		(0)								// オブジェクトのモデル列挙の開始地点
#define MODEL_OBJ_MAX	(int(MODELTYPE_PLAYER_CAR))		// オブジェクトのモデルの総数
#define FROM_PLAYER		(int(MODELTYPE_PLAYER_CAR))		// プレイヤーのモデル列挙の開始地点
#define MODEL_PLAY_MAX	(2)								// プレイヤーのモデルの総数
#define FROM_CAR		(int(MODELTYPE_CAR_POLICE))		// 車のモデル列挙の開始地点
#define MODEL_CAR_MAX	(5)								// 車のモデルの総数
#define FROM_EDIT		(int(MODELTYPE_EDIT_COLLISION))	// エディットのモデル列挙の開始地点
#define MODEL_EDIT_MAX	(1)								// エディットのモデルの総数

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
	D3DXVECTOR3        size;		// 大きさ
	float              fRadius;		// 半径
}Model;

//************************************************************
//	プロトタイプ宣言
//************************************************************
HRESULT InitModel(void);			// モデルの初期化処理
void UninitModel(void);				// モデルの終了処理
Model GetModelData(int nID);		// モデル情報の取得処理

#endif