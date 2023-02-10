//==================================================================================
//
//	汎用関数処理 [calculation.cpp]
//	Author：藤田勇一 & 小原立暉
//
//==================================================================================
//**********************************************************************************
//	インクルードファイル
//**********************************************************************************
#include "calculation.h"
#include "meshfield.h"

#include "billboard.h"
#include "camera.h"
#include "Car.h"
#include "effect.h"
#include "gate.h"
#include "Human.h"
#include "light.h"
#include "meshdome.h"
#include "meshcylinder.h"
#include "meshwall.h"
#include "particle.h"
#include "shadow.h"
#include "object.h"
#include "Police.h"
#include "weather.h"

//==================================================================================
//	四頂点の位置の計算処理
//==================================================================================
//	四つ角の頂点座標を計算する際に使用
//==================================================================================
void VecSizePos(D3DXVECTOR3 *sizePos, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight)
{
	// 変数を宣言
	float fAngle, fLength;	// 頂点座標の計算用

	// 対角線の長さ、角度を求める
	fAngle  = atan2f(fWidth, fHeight);
	fLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;
	
	// 頂点座標を求める
	sizePos[0].x = pos.x + sinf(rot.y + (D3DX_PI + fAngle)) * fLength;
	sizePos[0].y = pos.y;
	sizePos[0].z = pos.z + cosf(rot.y + (D3DX_PI + fAngle)) * fLength;
	sizePos[1].x = pos.x + sinf(rot.y + (D3DX_PI - fAngle)) * fLength;
	sizePos[1].y = pos.y;
	sizePos[1].z = pos.z + cosf(rot.y + (D3DX_PI - fAngle)) * fLength;
	sizePos[2].x = pos.x + sinf(rot.y + fAngle) * fLength;
	sizePos[2].y = pos.y;
	sizePos[2].z = pos.z + cosf(rot.y + fAngle) * fLength;
	sizePos[3].x = pos.x + sinf(rot.y - fAngle) * fLength;
	sizePos[3].y = pos.y;
	sizePos[3].z = pos.z + cosf(rot.y - fAngle) * fLength;
}

//==================================================================================
//	外積の左右判断処理
//==================================================================================
//	境界線から見て、その境界線の右にいるか、左にいるかの判定に使用
//==================================================================================
float LineOuterProduct(D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 pos)
{
	// 変数を宣言
	D3DXVECTOR3 vecLine;	// 境界線ベクトル
	D3DXVECTOR3 vecToPos;	// 左端と位置のベクトル

	// 境界線ベクトルを求める
	vecLine = posRight - posLeft;

	// 左端と位置のベクトルを求める
	vecToPos = pos - posLeft;

	// 外積の計算結果を返す
	return (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);
}

//==================================================================================
//	モデルの着地の更新処理
//==================================================================================
//	地面の上に立つかメッシュフィールドの上にいるかの判定に使用
//==================================================================================
void LandObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, bool *pbJump)
{
	// 変数を宣言
	float fLandPosY = CollisionMeshField(*pPos);		// 着地点

	if (pPos->y < fLandPosY)
	{ // 着地点に当たっている場合

		// ジャンプしていない状態にする
		*pbJump = false;

		// 位置を補正
		pPos->y = fLandPosY;

		// 移動量を初期化
		pMove->y = 0.0f;
	}
	else
	{ // 着地点に当たっていない場合

		// ジャンプしている状態にする
		*pbJump = true;
	}
}

//==================================================================================
//	向きの正規化
//==================================================================================
void RotNormalize(float *rot)
{
	if      (*rot >  D3DX_PI) { *rot -= D3DX_PI * 2; }
	else if (*rot < -D3DX_PI) { *rot += D3DX_PI * 2; }
}

//==================================================================================
//	ファイルをロードする全体処理
//==================================================================================
void LoadFileChunk(bool bCurve, bool bHumanCurve, bool bStage, bool bObject, bool bAI)
{
	// カーブの情報の初期化処理
	InitCurveInfo();

	if (bCurve == true)
	{ // カーブを読み込む場合
		// カーブテキストのロード処理
		LoadCurveTxt();
	}

	if (bHumanCurve == true)
	{ // 人間の曲がり角を読み込む場合
		// 人間のルートのロード処理
		LoadHumanCurveTxt();

		// 曲がり角の設定処理
		SetCurvePoint();
	}

	if (bStage == true)
	{ // ステージを読み込む場合
		// ステージのセットアップ
		TxtSetStage();
	}

	if (bObject == true)
	{ // オブジェクトを読み込む場合
		// オブジェクトのセットアップ
		TxtSetObject();
	}

	if (bAI == true)
	{ // AIを読み込む場合
		// AI系のセットアップ
		TxtSetAI();
	}
}

//==================================================================================
//	万能初期化の全体処理
//==================================================================================
void InitAllAroundChunk(void)
{
	// 天気の初期化処理
	InitWeather();

	// 影の初期化
	InitShadow();

	// 警察の初期化
	InitPolice();

	// オブジェクトの初期化
	InitObject();

	// 車の初期化
	InitCar();

	// 人間の初期化
	InitHuman();

	// ゲートの初期化
	InitGate();

	// カメラの初期化
	InitCamera();

	// ライトの初期化
	InitLight();

	// メッシュドームの初期化
	InitMeshDome();

	// メッシュシリンダーの初期化
	InitMeshCylinder();

	// メッシュフィールドの初期化
	InitMeshField();

	// メッシュウォールの初期化
	InitMeshWall();

	// ビルボードの初期化
	InitBillboard();

	// エフェクトの初期化
	InitEffect();

	// パーティクルの初期化
	InitParticle();
}

//==================================================================================
//	万能終了の全体処理
//==================================================================================
void UninitAllAroundChunk(void)
{
	// 天気の初期化処理
	UninitWeather();

	// 影の初期化
	UninitShadow();

	// 警察の初期化
	UninitPolice();

	// オブジェクトの初期化
	UninitObject();

	// 車の初期化
	UninitCar();

	// 人間の初期化
	UninitHuman();

	// ゲートの初期化
	UninitGate();

	// カメラの初期化
	UninitCamera();

	// ライトの初期化
	UninitLight();

	// メッシュドームの初期化
	UninitMeshDome();

	// メッシュシリンダーの初期化
	UninitMeshCylinder();

	// メッシュフィールドの初期化
	UninitMeshField();

	// メッシュウォールの初期化
	UninitMeshWall();

	// ビルボードの初期化
	UninitBillboard();

	// エフェクトの初期化
	UninitEffect();

	// パーティクルの初期化
	UninitParticle();
}

//==================================================================================
//	万能描画の全体処理
//==================================================================================
void DrawAllAroundChunk(void)
{
	// 天気の初期化処理
	DrawWeather();

	// 影の初期化
	DrawShadow();

	// 警察の初期化
	DrawPolice();

	// オブジェクトの初期化
	DrawObject();

	// 車の初期化
	DrawCar();

	// 人間の初期化
	DrawHuman();

	// ゲートの初期化
	DrawGate();

	// カメラの初期化
	SetCamera(CAMERATYPE_MAIN);

	// メッシュドームの初期化
	DrawMeshDome();

	// メッシュシリンダーの初期化
	DrawMeshCylinder();

	// メッシュフィールドの初期化
	DrawMeshField(false);

	// メッシュウォールの初期化
	DrawMeshWall();

	// ビルボードの初期化
	DrawBillboard();

	// エフェクトの初期化
	DrawEffect();

	// パーティクルの初期化
	DrawParticle();
}

//==================================================================================
//	リザルトの初期化全体処理
//==================================================================================
void InitResultChunk(void)
{
	// 万能初期化の全体処理
	InitAllAroundChunk();
}

//==================================================================================
//	リザルトの終了全体処理
//==================================================================================
void UninitResultChunk(void)
{
	// 万能終了の全体処理
	UninitAllAroundChunk();
}

//==================================================================================
//	リザルトの描画全体処理
//==================================================================================
void DrawResultChunk(void)
{
	// 万能描画の全体処理
	DrawAllAroundChunk();
}