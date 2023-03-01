//============================================================
//
//	モデル処理 [model.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "model.h"

//************************************************************
//	コンスト定義
//************************************************************
const char *apModelData[] =		// モデルの相対パス
{
	// オブジェクトモデル
	"data\\MODEL_OBJECT\\Tree.x",				// 木
	"data\\MODEL_OBJECT\\Rock.x",				// 岩
	"data\\MODEL_OBJECT\\Forest.x",				// 森
	"data\\MODEL_OBJECT\\BigTree.x",			// 大木
	"data\\MODEL_OBJECT\\YellowTree.x",			// 黄色い木
	"data\\MODEL_OBJECT\\Grass.x",				// 草
	"data\\MODEL_OBJECT\\House001.x",			// 家
	"data\\MODEL_OBJECT\\CurryShop.x",			// カレー屋
	"data\\MODEL_OBJECT\\bill001.x",			// ビルver1
	"data\\MODEL_OBJECT\\bill002.x",			// ビルver2
	"data\\MODEL_OBJECT\\ARCS.x",				// スーパー
	"data\\MODEL_OBJECT\\Bakery.x",				// パン屋
	"data\\MODEL_OBJECT\\CityOffice.x",			// 市役所
	"data\\MODEL_OBJECT\\gymnasium.x",			// 体育館
	"data\\MODEL_OBJECT\\Casino000.x",			// カジノ
	"data\\MODEL_OBJECT\\sunaba001.x",			// 公園（砂場）
	"data\\MODEL_OBJECT\\Toilet001.x",			// 公園（トイレ）
	"data\\MODEL_OBJECT\\suidou.x",				// 公園（水道）
	"data\\MODEL_OBJECT\\gomibako001.x",		// 公園（ゴミ箱）
	"data\\MODEL_OBJECT\\benti001.x",			// 公園（ベンチ）
	"data\\MODEL_OBJECT\\gaitou001.x",			// 公園（街灯）
	"data\\MODEL_OBJECT\\tetubou001.x",			// 公園（鉄棒）
	"data\\MODEL_OBJECT\\suberidai001.x",		// 公園（滑り台）
	"data\\MODEL_OBJECT\\Apart001.x",			// アパート
	"data\\MODEL_OBJECT\\gas.x",				// ガソリンスタンド
	"data\\MODEL_OBJECT\\711.x",				// コンビニ
	"data\\MODEL_OBJECT\\JoyFat.x",				// ジム
	"data\\MODEL_OBJECT\\towercrane.x",			// タワーレーン
	"data\\MODEL_OBJECT\\KaikanClub.x",			// ネットカフェ
	"data\\MODEL_OBJECT\\House002.x",			// 家（2）
	"data\\MODEL_OBJECT\\House003.x",			// 家（3）
	"data\\MODEL_OBJECT\\School.x",				// 学校
	"data\\MODEL_OBJECT\\DENTALCLINIC.x",		// 歯医者
	"data\\MODEL_OBJECT\\singou001.x",			// 信号
	"data\\MODEL_OBJECT\\HOSPITAL.x",			// 病院
	"data\\MODEL_OBJECT\\711parking.x",			// コンビニの駐車場
	"data\\MODEL_OBJECT\\FujiCamera.x",			// 家電量販店（フジカメラ）
	"data\\MODEL_OBJECT\\roof.x",				// 商店街（屋根）
	"data\\MODEL_OBJECT\\obarafood.x",			// 商店街（食堂）
	"data\\MODEL_OBJECT\\obamart.x",			// 商店街（半額店）
	"data\\MODEL_OBJECT\\obarastation.x",		// 商店街（ゲームセンター）
	"data\\MODEL_OBJECT\\swing000.x",			// 公園（ブランコ）
	"data\\MODEL_OBJECT\\fujidora.x",			// フジドラ
	"data\\MODEL_OBJECT\\fence000.x",			// 公園（フェンス）
	"data\\MODEL_OBJECT\\obarakurabu000.x",		// 商店街（雑貨店）	
	"data\\MODEL_OBJECT\\obarameat000.x",		// 商店街（精肉店）	
	"data\\MODEL_OBJECT\\obaravegetable000.x",	// 商店街（青果店）	
	"data\\MODEL_OBJECT\\obaraflour000.x",		// 商店街（花屋）	
	"data\\MODEL_OBJECT\\obaraclothes000.x",	// 商店街（服屋）	
	"data\\MODEL_OBJECT\\obarakaraoke000.x",	// 商店街（汎用・カラオケ）
	"data\\MODEL_OBJECT\\obaracleaning000.x",	// 商店街（汎用・クリーニング）
	"data\\MODEL_OBJECT\\obaraclinic000.x",		// 商店街（汎用・クリニック）	
	"data\\MODEL_OBJECT\\obararamen000.x",		// 商店街（汎用・ラーメン）	
	"data\\MODEL_OBJECT\\obaracoffee000.x",		// 商店街（汎用・喫茶店）	
	"data\\MODEL_OBJECT\\obaramobile000.x",		// 商店街（汎用・携帯ショップ）	
	"data\\MODEL_OBJECT\\obarabone000.x",		// 商店街（汎用・整骨院）	
	"data\\MODEL_OBJECT\\obaratempura000.x",	// 商店街（汎用・天ぷら）	
	"data\\MODEL_OBJECT\\obarapharmacy000.x",	// 商店街（汎用・薬局）	
	"data\\MODEL_OBJECT\\obarashutter000.x",	// 商店街（汎用・シャッター）	
	"data\\MODEL_OBJECT\\bill003.x",			// ビルver3
	"data\\MODEL_OBJECT\\hodoukyou.x",			// 歩道橋
	"data\\MODEL_OBJECT\\plant.x",				// 工場
	"data\\MODEL_OBJECT\\net.x",				// 工場現場網
	"data\\MODEL_OBJECT\\koujigenba.x",			// 工場現場ビル
	"data\\MODEL_OBJECT\\TPOtoilet.x",			// 仮設トイレ
	"data\\MODEL_OBJECT\\guardrail001.x",		// ガードレール
	"data\\MODEL_OBJECT\\ramen001.x",			// ラーメン屋
	"data\\MODEL_OBJECT\\funsui.x",				// 公園（噴水）
	"data\\MODEL_OBJECT\\TriCorn.x",			// 三角コーン
	"data\\MODEL_OBJECT\\TriCorn001.x",			// 三角コーン（二つ）

	// プレイヤーモデル
	"data\\MODEL_PLAYER\\car000.x",			// プレイヤー
	"data\\MODEL_PLAYER\\barrier000.x",		// バリア

	// 警察モデル
	"data\\MODEL_OBJECT\\policecar.x",		// パトカー

	// 車モデル
	"data\\MODEL_OBJECT\\car001.x",			// 自動車
	"data\\MODEL_OBJECT\\farecar.x",		// 消防車
	"data\\MODEL_OBJECT\\yakiimocar.x",		// 焼き芋屋
	"data\\MODEL_OBJECT\\senkyoCar000.x",	// 選挙カー
	"data\\MODEL_OBJECT\\BousouCar.x",		// 暴走車

	// 人間モデル
	"data\\MODEL_HUMAN\\body.x",			// 体
	"data\\MODEL_HUMAN\\head.x",			// 頭
	"data\\MODEL_HUMAN\\sleeve_l.x",		// 左腕
	"data\\MODEL_HUMAN\\sleeve_r.x",		// 右腕
	"data\\MODEL_HUMAN\\wrist_l.x",			// 左手首
	"data\\MODEL_HUMAN\\wrist_r.x",			// 右手首
	"data\\MODEL_HUMAN\\hand_l.x",			// 左手
	"data\\MODEL_HUMAN\\hand_r.x",			// 右手
	"data\\MODEL_HUMAN\\pants_l.x",			// 左足
	"data\\MODEL_HUMAN\\pants_r.x",			// 右足
	"data\\MODEL_HUMAN\\leg_l.x",			// 左腿
	"data\\MODEL_HUMAN\\leg_r.x",			// 右腿
	"data\\MODEL_HUMAN\\shoes_l.x",			// 左靴
	"data\\MODEL_HUMAN\\shoes_r.x",			// 右靴

	// ゲートモデル
	"data\\MODEL_GATE\\gate000.x",			// ゲート (建物)
	"data\\MODEL_GATE\\gate001.x",			// ゲート (鉄板)

	// がれきモデル
	"data\\MODEL_JUNK\\Junk.x",				// がれき

	// エディットモデル
	"data\\MODEL_EDIT\\collision000.x",		// 当たり判定
};

//************************************************************
//	プロトタイプ宣言
//************************************************************
HRESULT LoadXFileModel(void);	// xファイルの読み込み
void SetCollisionModel(void);	// 当たり判定の作成
HRESULT LoadTextureModel(void);	// テクスチャの読み込み

//************************************************************
//	グローバル変数
//************************************************************
Model g_aModel[MODELTYPE_MAX];	// モデルの情報

//============================================================
//	モデルの初期化処理
//============================================================
HRESULT InitModel(void)
{
	// モデル情報の初期化
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{ // モデルに使用するモデルの最大数分繰り返す

		g_aModel[nCntModel].pTexture = NULL;			// テクスチャへのポインタ
		g_aModel[nCntModel].pMesh    = NULL;			// メッシュ (頂点情報) へのポインタ
		g_aModel[nCntModel].pBuffMat = NULL;			// マテリアルへのポインタ
		g_aModel[nCntModel].dwNumMat = 0;				// マテリアルの数
		g_aModel[nCntModel].vtxMin   = INIT_VTX_MIN;	// 最小の頂点座標
		g_aModel[nCntModel].vtxMax   = INIT_VTX_MAX;	// 最大の頂点座標
		g_aModel[nCntModel].size     = INIT_SIZE;		// 大きさ
		g_aModel[nCntModel].fRadius  = 0.0f;			// 半径
	}

	// xファイルの読み込み
	if (FAILED(LoadXFileModel()))
	{ // xファイルの読み込みに失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 当たり判定の作成
	SetCollisionModel();

	// テクスチャの読み込み
	if (FAILED(LoadTextureModel()))
	{ // テクスチャの読み込みに失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	モデルの終了処理
//============================================================
void UninitModel(void)
{
	// テクスチャの破棄
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{ // モデルの最大数分繰り返す

		for (int nCntMat = 0; nCntMat < (int)g_aModel[nCntModel].dwNumMat; nCntMat++)
		{ // マテリアルの数分繰り返す

			if (g_aModel[nCntModel].pTexture[nCntMat] != NULL)
			{ // 変数 (g_aModel[nCntModel].pTexture) がNULLではない場合

				g_aModel[nCntModel].pTexture[nCntMat]->Release();
				g_aModel[nCntModel].pTexture[nCntMat] = NULL;
			}
		}

		// 動的確保したメモリを解放する
		free(g_aModel[nCntModel].pTexture);
	}

	// メッシュの破棄
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{ // モデルの最大数分繰り返す

		if (g_aModel[nCntModel].pMesh != NULL)
		{ // 変数 (g_aModel[nCntModel].pMesh) がNULLではない場合

			g_aModel[nCntModel].pMesh->Release();
			g_aModel[nCntModel].pMesh = NULL;
		}
	}

	// マテリアルの破棄
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{ // モデルの最大数分繰り返す

		if (g_aModel[nCntModel].pBuffMat != NULL)
		{ // 変数 (g_aModel[nCntModel].pBuffMat) がNULLではない場合

			g_aModel[nCntModel].pBuffMat->Release();
			g_aModel[nCntModel].pBuffMat = NULL;
		}
	}
}

//============================================================
//	xファイルの読み込み
//============================================================
HRESULT LoadXFileModel(void)
{
	// 変数を宣言
	HRESULT hr;		// 異常終了の確認用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{ // モデルの最大数分繰り返す

		// xファイルの読み込み
		hr = D3DXLoadMeshFromX
		( // 引数
			apModelData[nCntModel],				// モデルの相対パス
			D3DXMESH_SYSTEMMEM,					// メッシュ作成用オプション
			pDevice,							// デバイスへのポインタ
			NULL,								// 隣接性データ
			&g_aModel[nCntModel].pBuffMat,		// マテリアルへのポインタ
			NULL,								// エフェクトデータ
			&g_aModel[nCntModel].dwNumMat,		// マテリアルの数
			&g_aModel[nCntModel].pMesh			// メッシュ (頂点情報) へのポインタ
		);

		if (FAILED(hr))
		{ // xファイルの読み込みに失敗した場合

			// エラーメッセージボックス
			MessageBox(NULL, "xファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);

			// 失敗を返す
			return E_FAIL;
		}

		// 確保したメモリのアドレスを取得
		g_aModel[nCntModel].pTexture = (LPDIRECT3DTEXTURE9*)malloc(g_aModel[nCntModel].dwNumMat * sizeof(LPDIRECT3DTEXTURE9*));

		if (g_aModel[nCntModel].pTexture == NULL)
		{ // 動的確保に失敗した場合

			// エラーメッセージボックス
			MessageBox(NULL, "動的確保に失敗！", "警告！", MB_ICONWARNING);

			for (int nCntFree = 0; nCntFree < nCntModel; nCntFree++)
			{ // 動的確保に成功した回数分繰り返す

				// 動的確保したメモリを解放する
				free(g_aModel[nCntFree].pTexture);
			}

			// 失敗を返す
			return E_FAIL;
		}
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	当たり判定の作成
//============================================================
void SetCollisionModel(void)
{
	// 変数を宣言
	int         nNumVtx;		// モデルの頂点数
	DWORD       dwSizeFVF;		// モデルの頂点フォーマットのサイズ
	BYTE        *pVtxBuff;		// モデルの頂点バッファへのポインタ
	D3DXVECTOR3 vtx;			// モデルの頂点座標

	// 当たり判定の作成
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{ // モデルの最大数分繰り返す

		// モデルの頂点数を取得
		nNumVtx = g_aModel[nCntModel].pMesh->GetNumVertices();

		// モデルの頂点フォーマットのサイズを取得
		dwSizeFVF = D3DXGetFVFVertexSize(g_aModel[nCntModel].pMesh->GetFVF());

		// モデルの頂点バッファをロック
		g_aModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{ // モデルの頂点数分繰り返す

			// モデルの頂点座標を代入
			vtx = *(D3DXVECTOR3*)pVtxBuff;

			// 頂点座標 (x) の設定
			if (vtx.x < g_aModel[nCntModel].vtxMin.x)
			{ // 今回の頂点座標 (x) が、現状の頂点座標 (x) よりも小さい場合

				// 今回の頂点情報 (x) を代入
				g_aModel[nCntModel].vtxMin.x = vtx.x;
			}
			else if (vtx.x > g_aModel[nCntModel].vtxMax.x)
			{ // 今回の頂点座標 (x) が、現状の頂点座標 (x) よりも大きい場合

				// 今回の頂点情報 (x) を代入
				g_aModel[nCntModel].vtxMax.x = vtx.x;
			}

			// 頂点座標 (y) の設定
			if (vtx.y < g_aModel[nCntModel].vtxMin.y)
			{ // 今回の頂点座標 (y) が、現状の頂点座標 (y) よりも小さい場合

				// 今回の頂点情報 (y) を代入
				g_aModel[nCntModel].vtxMin.y = vtx.y;
			}
			else if (vtx.y > g_aModel[nCntModel].vtxMax.y)
			{ // 今回の頂点座標 (y) が、現状の頂点座標 (y) よりも大きい場合

				// 今回の頂点情報 (y) を代入
				g_aModel[nCntModel].vtxMax.y = vtx.y;
			}

			// 頂点座標 (z) の設定
			if (vtx.z < g_aModel[nCntModel].vtxMin.z)
			{ // 今回の頂点座標 (z) が、現状の頂点座標 (z) よりも小さい場合

				// 今回の頂点情報 (z) を代入
				g_aModel[nCntModel].vtxMin.z = vtx.z;
			}
			else if (vtx.z > g_aModel[nCntModel].vtxMax.z)
			{ // 今回の頂点座標 (z) が、現状の頂点座標 (z) よりも大きい場合

				// 今回の頂点情報 (z) を代入
				g_aModel[nCntModel].vtxMax.z = vtx.z;
			}

			// 頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += dwSizeFVF;
		}

		// モデルの頂点バッファをアンロック
		g_aModel[nCntModel].pMesh->UnlockVertexBuffer();

		// モデルサイズを求める
		g_aModel[nCntModel].size = g_aModel[nCntModel].vtxMax - g_aModel[nCntModel].vtxMin;

		// モデルの円の当たり判定を作成
		g_aModel[nCntModel].fRadius = ((g_aModel[nCntModel].size.x * 0.5f) + (g_aModel[nCntModel].size.z * 0.5f)) * 0.5f;
	}
}

//============================================================
//	テクスチャの読み込み
//============================================================
HRESULT LoadTextureModel(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルへのポインタ

	// テクスチャの読み込み
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{ // モデルに使用するモデルの最大数分繰り返す

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_aModel[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aModel[nCntModel].dwNumMat; nCntMat++)
		{ // マテリアルの数分繰り返す

			if (pMat[nCntMat].pTextureFilename != NULL)
			{ // テクスチャファイルが存在する場合

				// テクスチャの読み込み
				if (FAILED(D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aModel[nCntModel].pTexture[nCntMat])))
				{ // テクスチャの読み込みが失敗した場合

					// エラーメッセージボックス
					MessageBox(NULL, "テクスチャの読み込みに失敗！", "警告！", MB_ICONWARNING);

					// 失敗を返す
					return E_FAIL;
				}
			}
			else
			{ // テクスチャファイルが存在しない場合

				// NULLを設定
				g_aModel[nCntModel].pTexture[nCntMat] = NULL;
			}
		}
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	モデル情報の取得処理
//============================================================
Model GetModelData(int nID)
{
	// 引数のモデルの情報アドレスを返す
	return g_aModel[nID];
}