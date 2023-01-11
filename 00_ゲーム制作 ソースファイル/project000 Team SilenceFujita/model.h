//============================================================
//
//	モデルヘッダー [model.h]
//	Author：藤田勇一
//
//============================================================
#ifndef _MODEL_H_					// このマクロ定義がされていない場合
#define _MODEL_H_					// 二重インクルード防止のマクロを定義する

//************************************************************
//	列挙型定義 (MODELTYPE)
//************************************************************
typedef enum
{
	MODELTYPE_PLAYER_BODY = 0,		// 体
	MODELTYPE_PLAYER_HEAD,			// 頭
	MODELTYPE_PLAYER_SLEEVE_L,		// 左腕
	MODELTYPE_PLAYER_SLEEVE_R,		// 右腕
	MODELTYPE_PLAYER_WRIST_L,		// 左手首
	MODELTYPE_PLAYER_WRIST_R,		// 右手首
	MODELTYPE_PLAYER_HAND_L,		// 左手
	MODELTYPE_PLAYER_HAND_R,		// 右手
	MODELTYPE_PLAYER_PANTS_L,		// 左足
	MODELTYPE_PLAYER_PANTS_R,		// 右足
	MODELTYPE_PLAYER_LEG_L,			// 左腿
	MODELTYPE_PLAYER_LEG_R,			// 右腿
	MODELTYPE_PLAYER_SHOES_L,		// 左靴
	MODELTYPE_PLAYER_SHOES_R,		// 右靴
	MODELTYPE_PLAYER_KNIFE,			// 武器 (ナイフ)
	MODELTYPE_PLAYER_GUN,			// 武器 (銃)
	MODELTYPE_MAX,					// この列挙型の総数
} MODELTYPE;

//************************************************************
//	マクロ定義
//************************************************************
#define FROM_PLAYER	(MODELTYPE_PLAYER_BODY)		// プレイヤーのモデル列挙の開始地点

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
Model *GetModelData(int nID);		// モデル情報の取得処理

#endif