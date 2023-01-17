//============================================================
//
//	メイン処理 [main.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "input.h"
#include "model.h"
#include "sound.h"
#include "fade.h"

#include "title.h"
#include "game.h"

#include "billboard.h"
#include "object.h"

#ifdef _DEBUG	// デバッグ処理
#include "camera.h"
#include "effect.h"
#include "particle.h"
#include "Editmain.h"
#include "EditObject.h"
#include "EditBillboard.h"
#include "SoundDJ.h"
#endif

//************************************************************
//	マクロ定義
//************************************************************
#define CLASS_NAME		"WindowClass"		// ウインドウクラスの名前
#define WINDOW_NAME		"None"				// ウインドウの名前 (キャプションに表示)

#ifdef _DEBUG	// デバッグ処理
#define DEBUG_PRINT		(1280)		// デバッグ表示の文字列の最長

#define MAX_DEBUG		(2)			// 使用するポリゴン数
#define DEBUG_WIDTH		(405.0f)	// デバッグ背景の横幅
#define DEBUG_SPACE		(470.0f)	// デバッグ背景の空白
#endif

//************************************************************
//	プロトタイプ宣言
//************************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// ウインドウプロシージャ
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);							// 初期化処理
void Uninit(void);		// 終了処理
void Update(void);		// 更新処理
void Draw(void);		// 描画処理

#ifdef _DEBUG	// デバッグ処理
void InitDebug(void);	// デバッグの初期化処理
void UninitDebug(void);	// デバッグの終了処理
void UpdateDebug(void);	// デバッグの更新処理
void DrawDebug(void);	// デバッグの描画処理

void DrawDebugEditObject(void);			// エディットオブジェクトモードのデバッグ表示
void DrawDebugEditBillboard(void);		// エディットビルボードモードのデバッグ表示
void DrawDebugControlObject(void);		// エディットオブジェクト操作説明
void DrawDebugControlBillboard(void);	// エディットビルボード操作説明
#endif

//************************************************************
//	グローバル変数
//************************************************************
LPDIRECT3D9       g_pD3D = NULL;		// Direct3D オブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	// Direct3D デバイスへのポインタ

MODE       g_mode;			// モード切り替え用
StageLimit g_stageLimit;	// ステージの移動範囲

#ifdef _DEBUG	// デバッグ処理
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDebug = NULL;	// 頂点バッファへのポインタ
LPD3DXFONT g_pFont;			// フォントへのポインタ
int        g_nCountFPS;		// FPSカウンタ
bool       g_bDispDebug;	// デバッグ表記の ON / OFF
bool       g_bLeftBG;		// 左背景表示の ON / OFF
bool       g_bRightBG;		// 右背景表示の ON / OFF
#endif

//============================================================
//	メイン関数
//============================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	// 変数を宣言
	WNDCLASSEX wcex =
	{ // 初期値
		sizeof(WNDCLASSEX),					// WNDCLASSEX のメモリサイズ
		CS_CLASSDC,							// ウインドウのスタイル
		WindowProc,							// ウインドウプロシージャ
		0,									// 0 にする (通常は使用しない)
		0,									// 0 にする (通常は使用しない)
		hInstance,							// インスタンスハンドル
		LoadIcon(NULL, IDI_APPLICATION),	// タスクバーのアイコン
		LoadCursor(NULL, IDC_ARROW),		// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),			// クライアント領域の背景色
		NULL,								// メニューバー
		CLASS_NAME,							// ウインドウクラスの名前
		LoadIcon(NULL, IDI_APPLICATION)		// ファイルのアイコン
	};

	// 画面サイズ
	RECT rect = 
	{ // 初期値
		0,					// ウインドウの左上 X座標
		0,					// ウインドウの左上 Y座標
		SCREEN_WIDTH,		// ウインドウの幅
		SCREEN_HEIGHT		// ウインドウの高さ
	};

	// ウインドウ用
	HWND hWnd;				// ウインドウハンドル (識別子)
	MSG  msg;				// メッセージを格納する変数

	// 処理速度用
	DWORD dwCurrentTime;	// 現在時刻
	DWORD dwExecLastTime;	// 最後に処理した時刻

#ifdef _DEBUG	// デバッグ処理
	DWORD dwFrameCount;		// フレームカウント
	DWORD dwFPSLastTime;	// 最後に FPS を計測した時刻
#endif

	// ウインドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウインドウを生成
	hWnd = CreateWindowEx
	( // 引数
		0,							// 拡張ウインドウスタイル
		CLASS_NAME,					// ウインドウクラスの名前
		WINDOW_NAME,				// ウインドウの名前
		WS_OVERLAPPEDWINDOW,		// ウインドウスタイル
		CW_USEDEFAULT,				// ウインドウの左上 X 座標
		CW_USEDEFAULT,				// ウインドウの左上 Y 座標
		(rect.right - rect.left),	// ウインドウの幅
		(rect.bottom - rect.top),	// ウインドウの高さ
		NULL,						// 親ウインドウのハンドル
		NULL,						// メニューハンドルまたは子ウインドウ ID
		hInstance,					// インスタンスハンドル
		NULL						// ウインドウ作成データ
	);

	// 分解能を設定
	timeBeginPeriod(1);

	//ランダム関数の種を作成
	srand((unsigned int)time(0));

	// 処理速度用
	dwCurrentTime  = 0;
	dwExecLastTime = timeGetTime();	// 現在時刻を取得 (保存)

#ifdef _DEBUG	// デバッグ処理
	dwFrameCount  = 0;
	dwFPSLastTime = timeGetTime();	// 現在時刻を取得 (保存)
#endif

	// ウインドウの表示
	ShowWindow(hWnd, nCmdShow);		// ウインドウの表示状態を設定
	UpdateWindow(hWnd);				// クライアント領域を更新

	// 初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{ // 初期化処理が失敗した場合

		// 値を返す
		return -1;
	}

	// メッセージループ
	while (1)
	{ // 無限ループ

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{ // Windows の処理

			if (msg.message == WM_QUIT)
			{ // WM_QUIT メッセージを受け取ったらメッセージループを抜ける

				// 処理を抜ける
				break;
			}
			else
			{ // WM_QUIT メッセージを受け取っていない場合

				// メッセージの設定
				TranslateMessage(&msg);		// 仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);		// ウインドウプロシージャへメッセージを送出
			}
		}
		else
		{ // DirectX の処理

			// 現在時刻を取得
			dwCurrentTime = timeGetTime();

#ifdef _DEBUG	// デバッグ処理
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{ // 0.5秒経過

				// FPS を計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				// FPS を測定した時刻を保存
				dwFPSLastTime = dwCurrentTime;

				// フレームカウントをクリア
				dwFrameCount = 0;
			}
#endif

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{ // 60分の1秒経過

				// 更新処理
				Update();

				// 描画処理
				Draw();

				// 処理開始の時刻 [現在時刻] を保存
				dwExecLastTime = dwCurrentTime;

#ifdef _DEBUG	// デバッグ処理
				// フレームカウントを加算
				dwFrameCount++;
#endif
			}
		}
	}

	// 終了処理
	Uninit();

	// 分解能を戻す
	timeEndPeriod(1);

	// ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 値を返す
	return (int)msg.wParam;
}

//============================================================
//	ウインドウプロシージャ
//============================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 変数を宣言
	int nID;	// 返り値を格納

	switch (uMsg)
	{
	case WM_KEYDOWN:	// キー入力検知処理

		switch (wParam)
		{
		case VK_ESCAPE:		// [ESC] キーが押された場合

			// YES / NO のメッセージBOXを表示
			nID = MessageBox(hWnd, "本当に終了しますか？", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{ // YES が押された場合

				// ウインドウを破棄する (WM_DESTROY メッセージを送る)
				DestroyWindow(hWnd);
			}

			// 処理から抜ける
			break;
		}

		// 処理から抜ける
		break;

	case WM_CLOSE:		// ウインドウを終了する確認処理

		// YES / NO のメッセージBOXを表示
		nID = MessageBox(hWnd, "本当に終了しますか？", "終了メッセージ", MB_YESNO);

		if (nID == IDYES)
		{ // YES が押された場合

			// ウインドウを破棄する (WM_DESTROY メッセージを送る)
			DestroyWindow(hWnd);
		}
		else
		{ // NO が押された場合

			// 値を返す
			return 0;
		}

		// 処理から抜ける
		break;

	case WM_DESTROY:	// ウインドウを終了する処理

		// WM_QUIT メッセージを送る
		PostQuitMessage(0);

		// 処理から抜ける
		break;
	}

	// 値を返す
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//============================================================
//	初期化処理
//============================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// 変数を宣言
	D3DDISPLAYMODE        d3ddm;	// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// Direct3D オブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{ // オブジェクトの生成に失敗した場合

		// 失敗の値を返す
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{ // ディスプレイモードの取得に失敗した場合

		// 失敗の値を返す
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	デバイスの設定
	//--------------------------------------------------------
	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// パラメータのゼロクリア
	d3dpp.BackBufferWidth            = SCREEN_WIDTH;				// ゲーム画面サイズ (幅)
	d3dpp.BackBufferHeight           = SCREEN_HEIGHT;				// ゲーム画面サイズ (高さ)
	d3dpp.BackBufferFormat           = d3ddm.Format;				// バックバッファの形式
	d3dpp.BackBufferCount            = 1;							// バックバッファの数
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD;		// ダブルバッファの切り替え (映像信号に同期)
	d3dpp.EnableAutoDepthStencil     = TRUE;						// デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D16;					// デプスバッファとして 16bit を使う
	d3dpp.Windowed                   = bWindow;						// ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	//--------------------------------------------------------
	//	Direct3D デバイスの生成
	//--------------------------------------------------------
	// [※(現在の設定) 描画処理と頂点処理をハードウェアで行う]
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
	{ // DirectX が CPU のスペックを確認し、性能が低かった場合

		// [※ (現在の設定) 描画処理はハードウェア、頂点処理は CPU が行う]
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
		{ // DirectX が ハードウェア のスペックを確認し、性能が低かった場合

			// [※ (現在の設定) 描画処理と頂点処理を CPU で行う]
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
			{ // DirectX が CPU とハードウェアのスペックを確認し、性能が使い物にならなかった場合

				// 失敗の値を返す
				return E_FAIL;
			}
		}
	}

	//--------------------------------------------------------
	//	ステートの設定
	//--------------------------------------------------------
	// レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定 (テクスチャの拡縮補間の設定)
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージステートの設定 (テクスチャのアルファブレンドの設定)
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//--------------------------------------------------------
	//	変数の初期化
	//--------------------------------------------------------
	g_mode = MODE_TITLE;	// モードをタイトルに初期化

	// ステージの移動範囲を初期化
	g_stageLimit.fNear  = 0.0f;		// 移動の制限位置 (手前)
	g_stageLimit.fFar   = 0.0f;		// 移動の制限位置 (奥)
	g_stageLimit.fRight = 0.0f;		// 移動の制限位置 (右)
	g_stageLimit.fLeft  = 0.0f;		// 移動の制限位置 (左)
	g_stageLimit.fField = 0.0f;		// 移動の制限位置 (地面)

	//--------------------------------------------------------
	//	初期化処理
	//--------------------------------------------------------
	// 入力デバイスの初期化
	if (FAILED(InitInput(hInstance, hWnd)))
	{ // 入力デバイスの初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// モデルの初期化
	if (FAILED(InitModel()))
	{ // モデルの初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// サウンドの初期化
	InitSound(hWnd);

	// フェードの初期化
	InitFade(g_mode);

#ifdef _DEBUG	// デバッグ処理
	// サウンドDJの初期化
	InitSoundDJ(hWnd);

	// デバッグの初期化
	InitDebug();
#endif

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void Uninit(void)
{
	// 入力デバイスの終了
	UninitInput();

	// モデルの終了
	UninitModel();

	// サウンドの終了
	UninitSound();

	// フェードの終了
	UninitFade();

	// タイトル画面の終了
	UninitTitle();

	// ゲーム画面の終了
	UninitGame();

	// Direct3D デバイスの破棄
	if (g_pD3DDevice != NULL)
	{ // Direct3D デバイスが NULL ではない場合

		// Direct3D デバイスを破棄する
		g_pD3DDevice->Release();

		// Direct3D デバイスを NULL にする
		g_pD3DDevice = NULL;
	}

	// Direct3D オブジェクトの破棄
	if (g_pD3D != NULL)
	{ // Direct3D オブジェクトが NULL ではない場合

		// Direct3D オブジェクトを破棄する
		g_pD3D->Release();

		// Direct3D オブジェクトを NULL にする
		g_pD3D = NULL;
	}

#ifdef _DEBUG	// デバッグ処理
	// サウンドDJの終了
	UninitSoundDJ();

	// デバッグの終了
	UninitDebug();
#endif
}

//============================================================
//	更新処理
//============================================================
void Update(void)
{
	// 入力デバイスの更新
	UpdateInput();

	// それぞれの画面の更新
	switch (g_mode)
	{ // 選択処理
	case MODE_TITLE:	// タイトル画面

		// タイトル画面の更新
		UpdateTitle();

		// 処理から抜ける
		break;

	case MODE_GAME:		// ゲーム画面

		// ゲーム画面の更新
		UpdateGame();

		// 処理から抜ける
		break;
	}

	// フェードの更新
	UpdateFade();

#ifdef _DEBUG	// デバッグ処理
	// デバッグの更新
	UpdateDebug();
#endif
}

//============================================================
//	描画処理
//============================================================
void Draw(void)
{
	// 画面クリア (バックバッファと Zバッファのクリア)
	g_pD3DDevice->Clear
	( // 引数
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),				// 画面クリア時の色
		1.0f,
		0
	);

	// 描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{ // 描画開始が成功した場合

		switch (g_mode)
		{ // 選択処理
		case MODE_TITLE:	// タイトル画面

			// タイトル画面の描画
			DrawTitle();

			// 処理から抜ける
			break;

		case MODE_GAME:		// ゲーム画面

			// ゲーム画面の描画
			DrawGame();

			// 処理から抜ける
			break;
		}

		// フェードの描画
		DrawFade();

#ifdef _DEBUG	// デバッグ処理
		// デバッグの描画
		DrawDebug();
#endif

		// 描画終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//============================================================
//	モードの設定
//============================================================
void SetMode(MODE mode)
{
	//--------------------------------------------------------
	//	現在の画面 (モード) の終了処理
	//--------------------------------------------------------
	switch (g_mode)
	{
	case MODE_TITLE:	// タイトル画面

		// タイトル画面の終了
		UninitTitle();

		// 処理から抜ける
		break;

	case MODE_GAME:		// ゲーム画面

		// ゲーム画面の終了
		UninitGame();

		// 処理から抜ける
		break;
	}

	//--------------------------------------------------------
	//	新しい画面 (モード) の初期化処理
	//--------------------------------------------------------
	switch (mode)
	{
	case MODE_TITLE:	// タイトル画面

		// タイトル画面の初期化
		InitTitle();

		// 処理から抜ける
		break;

	case MODE_GAME:		// ゲーム画面

		// ゲーム画面の初期化
		InitGame();

		// 処理から抜ける
		break;
	}

	// モードを設定する
	g_mode = mode;		// 現在のモードに新しいモードの値を代入
}

//============================================================
//	モードの取得処理
//============================================================
MODE GetMode(void)
{
	// 現在のモードを返す
	return g_mode;
}

//============================================================
//	ステージの移動範囲の設定処理
//============================================================
void SetLimitStage(StageLimit limit)
{
	// 引数の移動範囲を代入
	g_stageLimit.fNear  = limit.fNear;		// 移動の制限位置 (手前)
	g_stageLimit.fFar   = limit.fFar;		// 移動の制限位置 (奥)
	g_stageLimit.fRight = limit.fRight;		// 移動の制限位置 (右)
	g_stageLimit.fLeft  = limit.fLeft;		// 移動の制限位置 (左)
	g_stageLimit.fField = limit.fField;		// 移動の制限位置 (地面)
}

//============================================================
//	ステージの移動範囲の取得処理
//============================================================
StageLimit GetLimitStage(void)
{
	// ステージの移動範囲を返す
	return g_stageLimit;
}

//============================================================
//	デバイスの取得
//============================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	// デバイスのアドレスを返す
	return g_pD3DDevice;
}

//============================================================
//	ステージのセットアップ処理
//============================================================
void TxtSetStage(void)
{
	// 変数を宣言
	int         nEnd;			// テキスト読み込み終了の確認用
	StageLimit  stageLimit;		// ステージの移動範囲の代入用
	D3DXVECTOR3 pos;			// 位置の代入用
	D3DXVECTOR3 rot;			// 向きの代入用
	D3DXVECTOR3 scale;			// 拡大率の代入用
	D3DXCOLOR   col;			// 色の代入用
	D3DXVECTOR2 radius;			// 半径の代入用
	int         nType;			// 種類の代入用
	int         nBreakType;		// 壊れ方の種類の代入用
	int         nShadowType;	// 影の種類の代入用
	int         nCollisionType;	// 当たり判定の種類の代入用
	int         nNumMat;		// マテリアル数の代入用
	int         nAnimCnt;		// 再生カウントの代入用
	int         nAnimPat;		// 再生パターンの代入用
	int         nAnim;			// アニメーションの ON / OFF の設定用
	bool        bAnim;			// アニメーションの ON / OFF の代入用
	bool        bShadow;		// 影の ON / OFF の代入

	// 変数配列を宣言
	char         aString[MAX_STRING];	// テキストの文字列の代入用
	D3DXMATERIAL aMat[MAX_MATERIAL];	// マテリアルの情報の代入用

	// ポインタを宣言
	FILE *pFile;				// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(STAGE_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			//------------------------------------------------
			//	移動範囲の設定
			//------------------------------------------------
			if (strcmp(&aString[0], "STAGE_LIMITSET") == 0)
			{ // 読み込んだ文字列が STAGE_LIMITSET の場合

				do
				{ // 読み込んだ文字列が END_STAGE_LIMITSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NEAR") == 0)
					{ // 読み込んだ文字列が NEAR の場合
						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &stageLimit.fNear);		// 手前の制限位置を読み込む
					}
					else if (strcmp(&aString[0], "FAR") == 0)
					{ // 読み込んだ文字列が FAR の場合
						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &stageLimit.fFar);		// 奥の制限位置を読み込む
					}
					else if (strcmp(&aString[0], "RIGHT") == 0)
					{ // 読み込んだ文字列が RIGHT の場合
						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &stageLimit.fRight);	// 右の制限位置を読み込む
					}
					else if (strcmp(&aString[0], "LEFT") == 0)
					{ // 読み込んだ文字列が LEFT の場合
						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &stageLimit.fLeft);		// 左の制限位置を読み込む
					}
					else if (strcmp(&aString[0], "FIELD") == 0)
					{ // 読み込んだ文字列が FIELD の場合
						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &stageLimit.fField);	// 地面の制限位置を読み込む
					}
				} while (strcmp(&aString[0], "END_STAGE_LIMITSET") != 0);	// 読み込んだ文字列が END_STAGE_LIMITSET ではない場合ループ

				// ステージの移動範囲の設定
				SetLimitStage(stageLimit);
			}

			//------------------------------------------------
			//	オブジェクトの設定
			//------------------------------------------------
			else if (strcmp(&aString[0], "SETSTAGE_OBJECT") == 0)
			{ // 読み込んだ文字列が SETSTAGE_OBJECT の場合

				do
				{ // 読み込んだ文字列が END_SETSTAGE_OBJECT ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "SET_OBJECT") == 0)
					{ // 読み込んだ文字列が SET_OBJECT の場合

						do
						{ // 読み込んだ文字列が END_SET_OBJECT ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "POS") == 0)
							{ // 読み込んだ文字列が POS の場合
								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);		// 位置を読み込む
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // 読み込んだ文字列が ROT の場合
								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);		// 向きを読み込む
							}
							else if (strcmp(&aString[0], "SCALE") == 0)
							{ // 読み込んだ文字列が SCALE の場合
								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%f%f%f", &scale.x, &scale.y, &scale.z);	// 拡大率を読み込む
							}
							else if (strcmp(&aString[0], "TYPE") == 0)
							{ // 読み込んだ文字列が TYPE の場合
								fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
								fscanf(pFile, "%d", &nType);			// オブジェクトの種類を読み込む
							}
							else if (strcmp(&aString[0], "BREAKTYPE") == 0)
							{ // 読み込んだ文字列が BREAKTYPE の場合
								fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
								fscanf(pFile, "%d", &nBreakType);		// 壊れ方の種類を読み込む
							}
							else if (strcmp(&aString[0], "SHADOWTYPE") == 0)
							{ // 読み込んだ文字列が SHADOWTYPE の場合
								fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
								fscanf(pFile, "%d", &nShadowType);		// 影の種類を読み込む
							}
							else if (strcmp(&aString[0], "COLLTYPE") == 0)
							{ // 読み込んだ文字列が COLLTYPE の場合
								fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
								fscanf(pFile, "%d", &nCollisionType);	// 当たり判定の種類を読み込む
							}
							else if (strcmp(&aString[0], "NUMMAT") == 0)
							{ // 読み込んだ文字列が NUMMAT の場合
								fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
								fscanf(pFile, "%d", &nNumMat);			// マテリアル数を読み込む

								for (int nCntMat = 0; nCntMat < nNumMat; nCntMat++)
								{ // マテリアル数分繰り返す

									fscanf(pFile, "%s", &aString[0]);	// マテリアルの番号を読み込む (不要)
									fscanf(pFile, "%s", &aString[0]);	// マテリアルの要素を読み込む (不要)
									fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)

									// 拡散光を読み込む
									fscanf(pFile, "%f%f%f%f",
									&aMat[nCntMat].MatD3D.Diffuse.r,
									&aMat[nCntMat].MatD3D.Diffuse.g,
									&aMat[nCntMat].MatD3D.Diffuse.b,
									&aMat[nCntMat].MatD3D.Diffuse.a);

									fscanf(pFile, "%s", &aString[0]);	// マテリアルの番号を読み込む (不要)
									fscanf(pFile, "%s", &aString[0]);	// マテリアルの要素を読み込む (不要)
									fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)

									// 環境光を読み込む
									fscanf(pFile, "%f%f%f%f",
									&aMat[nCntMat].MatD3D.Ambient.r,
									&aMat[nCntMat].MatD3D.Ambient.g,
									&aMat[nCntMat].MatD3D.Ambient.b,
									&aMat[nCntMat].MatD3D.Ambient.a);
								}
							}

						} while (strcmp(&aString[0], "END_SET_OBJECT") != 0);	// 読み込んだ文字列が END_SET_OBJECT ではない場合ループ

						// オブジェクトの設定
						SetObject(pos, rot, scale, &aMat[0], nType, nBreakType, nShadowType, nCollisionType);
					}
				} while (strcmp(&aString[0], "END_SETSTAGE_OBJECT") != 0);		// 読み込んだ文字列が END_SETSTAGE_OBJECT ではない場合ループ
			}

			//------------------------------------------------
			//	ビルボードの設定
			//------------------------------------------------
			else if (strcmp(&aString[0], "SETSTAGE_BILLBOARD") == 0)
			{ // 読み込んだ文字列が SETSTAGE_BILLBOARD の場合

				do
				{ // 読み込んだ文字列が END_SETSTAGE_BILLBOARD ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "SET_BILLBOARD") == 0)
					{ // 読み込んだ文字列が SET_BILLBOARD の場合

						do
						{ // 読み込んだ文字列が END_SET_BILLBOARD ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "POS") == 0)
							{ // 読み込んだ文字列が POS の場合
								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);			// 位置を読み込む
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // 読み込んだ文字列が ROT の場合
								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);			// 向きを読み込む
							}
							else if (strcmp(&aString[0], "COL") == 0)
							{ // 読み込んだ文字列が COL の場合
								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%f%f%f%f", &col.r, &col.g, &col.b, &col.a);	// 色を読み込む
							}
							else if (strcmp(&aString[0], "RADIUS") == 0)
							{ // 読み込んだ文字列が RADIUS の場合
								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%f%f", &radius.x, &radius.y);				// 半径を読み込む
							}
							else if (strcmp(&aString[0], "TYPE") == 0)
							{ // 読み込んだ文字列が TYPE の場合
								fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
								fscanf(pFile, "%d", &nType);			// ビルボードの種類を読み込む
							}
							else if (strcmp(&aString[0], "SHADOWTYPE") == 0)
							{ // 読み込んだ文字列が SHADOWTYPE の場合
								fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
								fscanf(pFile, "%d", &nShadowType);		// 影の種類を読み込む

								// 影の ON / OFF を設定
								bShadow = (nShadowType == 0) ? false : true;
							}
							else if (strcmp(&aString[0], "ANIMATION") == 0)
							{ // 読み込んだ文字列が ANIMATION の場合
								fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
								fscanf(pFile, "%d", &nAnim);			// アニメーションの ON / OFF を読み込む

								// アニメーションの ON / OFF を設定
								bAnim = (nAnim == 0) ? false : true;
							}
							else if (strcmp(&aString[0], "ANIMCNT") == 0)
							{ // 読み込んだ文字列が ANIMCNT の場合
								fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
								fscanf(pFile, "%d", &nAnimCnt);			// 再生カウントを読み込む
							}
							else if (strcmp(&aString[0], "ANIMPAT") == 0)
							{ // 読み込んだ文字列が ANIMPAT の場合
								fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
								fscanf(pFile, "%d", &nAnimPat);			// 再生パターンを読み込む
							}
						} while (strcmp(&aString[0], "END_SET_BILLBOARD") != 0);	// 読み込んだ文字列が END_SET_BILLBOARD ではない場合ループ

						// ビルボードの設定
						SetBillboard(rot, pos, nType, radius, col, nAnimCnt, nAnimPat, bAnim, bShadow);
					}
				} while (strcmp(&aString[0], "END_SETSTAGE_BILLBOARD") != 0);		// 読み込んだ文字列が END_SETSTAGE_BILLBOARD ではない場合ループ
			}
		} while (nEnd != EOF);														// 読み込んだ文字列が EOF ではない場合ループ
		
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "ステージファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}

#ifdef _DEBUG	// デバッグ処理
//============================================================
//	デバッグの初期化処理
//============================================================
void InitDebug(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_2D         *pVtx;					// 頂点情報へのポインタ

	// グローバル変数の初期化
	g_pFont      = NULL;	// フォントへのポインタ
	g_nCountFPS  = 0;		// FPSカウンタ
	g_bDispDebug = true;	// デバッグ表記の ON / OFF
	g_bLeftBG    = true;	// 左背景表示の ON / OFF
	g_bRightBG   = false;	// 右背景表示の ON / OFF

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_2D) * 4 * MAX_DEBUG,		// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffDebug,
		NULL
	);

	// デバッグ表示用フォントの生成
	D3DXCreateFont
	( // 引数
		g_pD3DDevice,			// デバイス
		18,						// 高さ
		0,						// 幅
		0,						// 太さ
		0,						// ミニマップ数
		FALSE,					// 斜体
		SHIFTJIS_CHARSET,		// フォントの文字セット
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal",
		&g_pFont				// フォントポインタ
	);

	//--------------------------------------------------------
	//	頂点情報の初期化
	//--------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffDebug->Lock(0, 0, (void**)&pVtx, 0);

	//--------------------------------------------------------
	//	背景の初期化
	//--------------------------------------------------------
	for (int nCntDebug = 0; nCntDebug < MAX_DEBUG; nCntDebug++)
	{ // 使用するポリゴン数分繰り返す

		// 頂点座標を設定
		pVtx[0].pos = D3DXVECTOR3(0.0f        + (nCntDebug * (DEBUG_WIDTH + DEBUG_SPACE)), 0.0f,          0.0f);
		pVtx[1].pos = D3DXVECTOR3(DEBUG_WIDTH + (nCntDebug * (DEBUG_WIDTH + DEBUG_SPACE)), 0.0f,          0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f        + (nCntDebug * (DEBUG_WIDTH + DEBUG_SPACE)), SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(DEBUG_WIDTH + (nCntDebug * (DEBUG_WIDTH + DEBUG_SPACE)), SCREEN_HEIGHT, 0.0f);

		// rhw の設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}
}

//============================================================
//	デバッグの終了処理
//============================================================
void UninitDebug(void)
{
	// 頂点バッファの破棄
	if (g_pVtxBuffDebug != NULL)
	{ // 変数 (g_pVtxBuffDebug) がNULLではない場合

		g_pVtxBuffDebug->Release();
		g_pVtxBuffDebug = NULL;
	}

	// デバッグ表示用フォントの破棄
	if (g_pFont != NULL)
	{ // フォントのポインタが NULL ではない場合

		// フォントのポインタを破棄する
		g_pFont->Release();

		// フォントのポインタを NULL にする
		g_pFont = NULL;
	}
}

//============================================================
//	デバッグの更新処理
//============================================================
void UpdateDebug(void)
{
	if (GetKeyboardTrigger(DIK_F1) == true)
	{ // [F1] が押された場合

		// 真偽を反転させる
		g_bDispDebug = (g_bDispDebug == false) ? true : false;
	}

	if (GetKeyboardTrigger(DIK_F7) == true)
	{ // [F7] が押された場合

		// 真偽を反転させる
		g_bLeftBG = (g_bLeftBG == false) ? true : false;
	}

	if (GetKeyboardTrigger(DIK_F8) == true)
	{ // [F8] が押された場合

		// 真偽を反転させる
		g_bRightBG = (g_bRightBG == false) ? true : false;
	}
}

//============================================================
//	デバッグの描画処理
//============================================================
void DrawDebug(void)
{
	// 変数を宣言
	RECT rect =
	{ // 初期値
		0,					// ウインドウの左上 X座標
		0,					// ウインドウの左上 Y座標
		SCREEN_WIDTH,		// ウインドウの幅
		SCREEN_HEIGHT		// ウインドウの高さ
	};

	D3DXVECTOR3 cameraPosV   = GetCameraPosV();		// カメラの視点
	D3DXVECTOR3 cameraPosR   = GetCameraPosR();		// カメラの注視点
	D3DXVECTOR3 cameraRot    = GetCameraRot();		// カメラの向き
	float       cameraDis    = GetCameraDis();		// カメラの距離
	int         nNumEffect   = GetNumEffect();		// エフェクトの総数
	int         nNumParticle = GetNumParticle();	// パーティクルの総数

	// 変数配列を宣言
	char aDeb[DEBUG_PRINT];	// デバッグ情報の表示用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// 文字列に代入
	sprintf
	( // 引数
		&aDeb[0],			// 代入する文字列
		"　 \n"
		"　 [F1] デバッグ表示 ON / OFF\n"
		"　 [F2] モードの切り替え\n"
		"　 [F3] ステージのセーブ\n"
		"　 [F4] 曲の変更\n"
		"　 [F5] 曲の再生\n"
		"　 [F6] 曲の停止\n"
		"　 [F7] 左背景の ON / OFF\n"
		"　 [F8] 右背景の ON / OFF\n"
		"　 ---------------------------------------------　\n"
		"　 [FPS] %d\n"
		"　 [ カメラ視点 ] %.1f, %.1f, %.1f\n"
		"　 [カメラ注視点] %.1f, %.1f, %.1f\n"
		"　 [ カメラ向き ] %.1f, %.1f, %.1f\n"
		"　 [ カメラ距離 ] %.1f\n"
		"　 [エフェクト数] %-4d　 [パーティクル数] %-3d\n",
		g_nCountFPS,		// FPS
		cameraPosV.x,		// カメラの視点の位置 (x)
		cameraPosV.y,		// カメラの視点の位置 (y)
		cameraPosV.z,		// カメラの視点の位置 (z)
		cameraPosR.x,		// カメラの注視点の位置 (x)
		cameraPosR.y,		// カメラの注視点の位置 (y)
		cameraPosR.z,		// カメラの注視点の位置 (z)
		cameraRot.x,		// カメラの向き (x)
		cameraRot.y,		// カメラの向き (y)
		cameraRot.z,		// カメラの向き (z)
		cameraDis,			// カメラの距離
		nNumEffect,			// エフェクトの総数
		nNumParticle		// パーティクルの総数
	);

	//--------------------------------------------------------
	//	背景の描画
	//--------------------------------------------------------
	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffDebug, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_bLeftBG == true)
	{ // 左の背景表示が ON の場合

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	if (g_bRightBG == true)
	{ // 右の背景表示が ON の場合

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
	}

	//--------------------------------------------------------
	//	デバッグ表示の描画
	//--------------------------------------------------------
	if (g_bDispDebug == true)
	{ // デバッグの表示が ON の場合

		// テキストの描画
		g_pFont->DrawText
		( // 引数
			NULL,
			&aDeb[0],
			-1,
			&rect,
			DT_LEFT,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
		);
	}

	//--------------------------------------------------------
	//	エディット表示の描画
	//--------------------------------------------------------
	if (g_mode == MODE_GAME)
	{ // ゲームモードの場合

		if (GetGameMode() == GAMEMODE_EDIT)
		{ // エディットモードだった場合

			if (GetStyle() == EDITSTYLE_OBJECT)
			{ // オブジェクトスタイルだった場合

				// オブジェクトの操作説明
				DrawDebugControlObject();

				// エディットモードのデバッグ表示
				DrawDebugEditObject();
			}
			else if (GetStyle() == EDITSTYLE_BILLBOARD)
			{ // ビルボードスタイルだった場合

				// ビルボードの操作説明
				DrawDebugControlBillboard();

				// エディットモードのデバッグ表示
				DrawDebugEditBillboard();
			}
		}
	}
}

//==============================================
//	エディットオブジェクトモードのデバッグ表示
//==============================================
void DrawDebugEditObject(void)
{
	// 変数を宣言
	RECT rect =
	{ // 初期値
		0,					// ウインドウの左上 X座標
		0,					// ウインドウの左上 Y座標
		SCREEN_WIDTH,		// ウインドウの幅
		SCREEN_HEIGHT		// ウインドウの高さ
	};

	// 変数配列を宣言
	char aDeb[DEBUG_PRINT];	// デバッグ情報の表示用

	// ポインタを宣言
	EditObject *edit = GetEditObject();		// エディットオブジェクトの情報を取得する

	// 文字列に代入
	sprintf
	( // 引数
		&aDeb[0],
		"\n　 位置　 [%.4f, %.4f, %.4f]"
		"\n　 拡大率 [%.4f, %.4f, %.4f]"
		"\n　 向き　 [%d]"
		"\n　 種類　 [%d]"
		"\n　 色　　 [%.2f, %.2f, %.2f]"
		"\n   影     [%s]"
		"\n   壊れ方 [%s]"
		"\n   判定　 [%s]"
		"\n\n",
		edit->pos.x, edit->pos.y, edit->pos.z,
		edit->scale.x, edit->scale.y, edit->scale.z,
		(int)D3DXToDegree(edit->rot.y), edit->nType,
		edit->EditMaterial[edit->nType][edit->nCntMaterial].MatD3D.Diffuse.r,
		edit->EditMaterial[edit->nType][edit->nCntMaterial].MatD3D.Diffuse.g,
		edit->EditMaterial[edit->nType][edit->nCntMaterial].MatD3D.Diffuse.b,
		edit->Shadowtype.pShadowMode[edit->Shadowtype.Shadowtype],
		edit->Break.pBreakMode[edit->Break.Breaktype],
		edit->Collisiontype.pCollisionMode[edit->Collisiontype.Collisiontype]
	);

	// テキストの描画
	g_pFont->DrawText(NULL, &aDeb[0], -1, &rect, DT_BOTTOM, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//==============================================
//	エディットビルボードモードのデバッグ表示
//==============================================
void DrawDebugEditBillboard(void)
{
	// 変数を宣言
	RECT rect =
	{ // 初期値
		0,					// ウインドウの左上 X座標
		0,					// ウインドウの左上 Y座標
		SCREEN_WIDTH,		// ウインドウの幅
		SCREEN_HEIGHT		// ウインドウの高さ
	};

	// 変数配列を宣言
	char aDeb[DEBUG_PRINT];	// デバッグ情報の表示用

	// ポインタを宣言
	EditBillboard *Editbillboard = GetEditBillboard();	//エディットビルボードの情報を取得する

	// 文字列に代入
	sprintf
	( // 引数
		&aDeb[0],
		"\n　 位置　 [%.4f, %.4f, %.4f]"
		"\n　 拡大率 [%.4f, %.4f]"
		"\n　 種類　 [%d]"
		"\n　 色　　 [%.2f,%.2f,%.2f]"
		"\n   影     [%d]  (0：OFF,  1：ON)"
		"\n　 ---------------------------------------------"
		"\n　 アニメーション　　　：%d  (0：OFF,  1：ON)"
		"\n　 アニメーションプレイ：%d　(0：STOP, 1：PLAY)"
		"\n　 ---------------------------------------------"
		"\n　 アニメーションパターン：%d"
		"\n　 アニメーションカウンタ：%d"
		"\n\n",
		Editbillboard->pos.x, Editbillboard->pos.y, Editbillboard->pos.z,
		Editbillboard->Radius.x, Editbillboard->Radius.y,
		Editbillboard->nType,
		Editbillboard->col.r, Editbillboard->col.g, Editbillboard->col.b,
		Editbillboard->bShadow,
		Editbillboard->EditAnim.bAnim,
		Editbillboard->bAnimReplay,
		Editbillboard->EditAnim.nAnimPattern,
		Editbillboard->EditAnim.nAnimCounter
	);

	// テキストの描画
	g_pFont->DrawText(NULL, &aDeb[0], -1, &rect, DT_BOTTOM, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//==============================================
//	エディットオブジェクト操作説明
//==============================================
void DrawDebugControlObject(void)
{
	// 変数を宣言
	RECT rect =
	{ // 初期値
		0,					// ウインドウの左上 X座標
		0,					// ウインドウの左上 Y座標
		SCREEN_WIDTH,		// ウインドウの幅
		SCREEN_HEIGHT		// ウインドウの高さ
	};

	// 変数配列を宣言
	char aDeb[DEBUG_PRINT];	// デバッグ情報の表示用

	// 文字列に代入
	sprintf
	( // 引数
		&aDeb[0],
		"\n種類の変更：[1] 　"
		"\n向きのリセット：[2] 　"
		"\n拡大率のリセット：[3] 　"
		"\n全体の拡大：[4] 　"
		"\n全体の縮小：[5] 　"
		"\n設置スタイルの変更：[6] 　"
		"\n破壊の種類の変更：[7] 　"
		"\n影の種類の変更：[8] 　"
		"\nオブジェクトの削除：[9] 　"
		"\nオブジェクトの設置：[0] 　"
		"\nマテリアルの変更：[SPACE] 　"
		"\n当たり判定の種類の変更：[BACKSPACE] 　"
		"\n--------------------------------------------- 　"
		"\nオブジェクトの移動：[W/A/S/D] 　"
		"\nオブジェクトの向き変更：[Q/E] 　"
		"\nオブジェクトのX軸の拡大縮小：[U/J] 　"
		"\nオブジェクトのY軸の拡大縮小：[I/K] 　"
		"\nオブジェクトのZ軸の拡大縮小：[O/L] 　"
		"\n--------------------------------------------- 　"
		"\nマテリアルのR値の変更：[LSHIFT+R/F] 　"
		"\nマテリアルのG値の変更：[LSHIFT+T/G] 　"
		"\nマテリアルのB値の変更：[LSHIFT+Y/H] 　"
		"\nマテリアルのリセットR値：[LSHIFT+V] 　"
		"\nマテリアルのリセットG値：[LSHIFT+B] 　"
		"\nマテリアルのリセットB値：[LSHIFT+N] 　"
		"\nオブジェクトの縦の移動：[LSHIFT+W/S] 　"
		"\nオブジェクトの縦の位置の初期化：[LSHIFT+A/D] 　"
		"\nオブジェクトの15度回転：[LSHIFT+Q/E] 　"
	);

	// テキストの描画
	g_pFont->DrawText(NULL, &aDeb[0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//==============================================
//	エディットビルボード操作説明
//==============================================
void DrawDebugControlBillboard(void)
{
	// 変数を宣言
	RECT rect =
	{ // 初期値
		0,					// ウインドウの左上 X座標
		0,					// ウインドウの左上 Y座標
		SCREEN_WIDTH,		// ウインドウの幅
		SCREEN_HEIGHT		// ウインドウの高さ
	};

	// 変数配列を宣言
	char aDeb[DEBUG_PRINT];	// デバッグ情報の表示用

	// 文字列に代入
	sprintf
	( // 引数
		&aDeb[0],
		"\n種類変更：[1] 　"
		"\n影の使用：[2] 　"
		"\n拡大率のリセット：[3] 　"
		"\n全体の拡大：[4] 　"
		"\n全体の縮小：[5] 　"
		"\n設置スタイルの変更：[6] 　"
		"\nアニメーション化：[7] 　"
		"\nアニメーション再生：[8] 　"
		"\nビルボードの削除：[9] 　"
		"\nビルボードの設置：[0] 　"
		"\n--------------------------------------------- 　"
		"\nビルボードの移動：[W/A/S/D] 　"
		"\nビルボードのX軸の拡大縮小：[U/J] 　"
		"\nビルボードのY軸の拡大縮小：[I/K] 　"
		"\n--------------------------------------------- 　"
		"\n色のR値の変更：[LSHIFT+R/F] 　"
		"\n色のG値の変更：[LSHIFT+T/G] 　"
		"\n色のB値の変更：[LSHIFT+Y/H] 　"
		"\n色のリセットR値：[LSHIFT+V] 　"
		"\n色のリセットG値：[LSHIFT+B] 　"
		"\n色のリセットB値：[LSHIFT+N] 　"
		"\n--------------------------------------------- 　"
		"\nアニメーションのカウンタ：[LSHIFT+←/→] 　"
		"\nアニメーションのパターン：[LSHIFT+↑/↓] 　"
		"\nビルボードの縦の移動：[LSHIFT+W/S] 　"
		"\nビルボードの縦の位置の初期化：[LSHIFT+A/D] 　"
	);

	// テキストの描画
	g_pFont->DrawText(NULL, &aDeb[0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(255, 255, 255, 255));
}
#endif