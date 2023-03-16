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

#include "3Dnotation.h"
#include "billboard.h"
#include "camera.h"
#include "Car.h"
#include "effect.h"
#include "flash.h"
#include "gate.h"
#include "Human.h"
#include "icon.h"
#include "light.h"
#include "meshdome.h"
#include "meshcylinder.h"
#include "meshwall.h"
#include "particle.h"
#include "shadow.h"
#include "object.h"
#include "Police.h"
#include "weather.h"

#include "bonus.h"
#include "game.h"

//**********************************************************************************
//	マクロ定義
//**********************************************************************************
#define COLL_SLOW_MOVE	(12.5f)	// ぶつかった際減速する移動量
#define COLL_SLOW		(0.95f)	// ぶつかった際の減速倍率

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
//	外積の当たり判定
//==================================================================================
//	境界線にぶつかっていた場合、ぶつかった位置を求める際に使用
//==================================================================================
void CollisionOuterProduct(D3DXVECTOR3 *Targetpos, D3DXVECTOR3 *TargetposOld, D3DXVECTOR3 WallRpos, D3DXVECTOR3 WallLpos, bool *bUse, int *nShadow)
{
	// 変数を宣言
	D3DXVECTOR3 vecMove, vecLine, vecTopos, posCross;	// 外積の変数
	float       fRate;									// 割合の変数

	// 弾の軌跡を測る
	vecMove.x = (Targetpos->x - TargetposOld->x);
	vecMove.y = (Targetpos->y - TargetposOld->y);
	vecMove.z = (Targetpos->z - TargetposOld->z);

	// 壁の境界線を測る
	vecLine.x = (WallRpos.x - WallLpos.x);
	vecLine.y = (WallRpos.y - WallLpos.y);
	vecLine.z = (WallRpos.z - WallLpos.z);

	// 壁の始点から弾の位置の距離を測る
	vecTopos.x = (Targetpos->x - WallLpos.x);
	vecTopos.y = (Targetpos->y - WallLpos.y);
	vecTopos.z = (Targetpos->z - WallLpos.z);

	// 割合を求める
	fRate = ((vecTopos.z * vecMove.x) - (vecTopos.x * vecMove.z)) / ((vecLine.z * vecMove.x) - (vecLine.x * vecMove.z));

	// 交点を求める
	posCross.x = vecLine.x * fRate - WallLpos.x;
	posCross.y = vecLine.y * fRate - WallLpos.y;
	posCross.z = vecLine.z * fRate - WallLpos.z;

	if ((vecLine.z * vecTopos.x) - (vecLine.x * vecTopos.z) < 0.0f)
	{ // 境界線を超えた場合

		if (fRate >= 0.0f && fRate <= 1.0f)
		{ // 割合が0.0f～1.0fの間だった(境界線を超えた)場合

			// 位置を設定する
			Targetpos = &posCross;
		}
	}
}

//==================================================================================
//	扇形の当たり判定
//==================================================================================
//	視界内の判定などに使用
//==================================================================================
bool CollisionSector(D3DXVECTOR3 centerPos, D3DXVECTOR3 targetPos, float fCenterRot, float fRadius, float fAngle)
{
	// 変数を宣言
	float       fLength;		// 中心位置と目標位置の距離
	float       fHalfAngle;		// 引数の角度の半分の値の代入用
	bool        bHit = false;	// 当たり判定の結果
	D3DXVECTOR3 vecToPos;		// 左端と位置のベクトル

	// 変数配列を宣言
	float       fRotEdge[2];	// 扇形の縁の角度     [※] 0：左 1：右
	D3DXVECTOR3 posEdge[2];		// 扇形の縁の先端位置 [※] 0：左 1：右
	D3DXVECTOR3 vecEdge[2];		// 扇形の縁ベクトル   [※] 0：左 1：右

	// 中心位置と目標位置の距離求める
	fLength = (centerPos.x - targetPos.x) * (centerPos.x - targetPos.x)
			+ (centerPos.z - targetPos.z) * (centerPos.z - targetPos.z);

	if (fLength < fRadius * fRadius)
	{ // 円の範囲内の場合

		// 引数の角度の半分の値を求める
		fHalfAngle = fAngle * 0.5f;

		// 扇形の左縁の角度を求める
		fRotEdge[0] = fCenterRot + fHalfAngle;	// 角度を左に傾ける
		RotNormalize(&fRotEdge[0]);				// 向きを正規化

		// 扇形の右縁の角度を求める
		fRotEdge[1] = fCenterRot - fHalfAngle;	// 角度を右に傾ける
		RotNormalize(&fRotEdge[1]);				// 向きを正規化

		// 扇形の左縁の先端位置を求める
		posEdge[0].x = centerPos.x + sinf(fRotEdge[0]) * 1.0f;
		posEdge[0].y = 0.0f;
		posEdge[0].z = centerPos.z + cosf(fRotEdge[0]) * 1.0f;

		// 扇形の右縁の先端位置を求める
		posEdge[1].x = centerPos.x + sinf(fRotEdge[1]) * 1.0f;
		posEdge[1].y = 0.0f;
		posEdge[1].z = centerPos.z + cosf(fRotEdge[1]) * 1.0f;

		// 扇形の左縁のベクトルを求める
		vecEdge[0] = posEdge[0] - centerPos;
		vecEdge[0].y = 0.0f;

		// 扇形の右縁のベクトルを求める
		vecEdge[1] = posEdge[1] - centerPos;
		vecEdge[1].y = 0.0f;

		// 左端と位置のベクトルを求める
		vecToPos = targetPos - centerPos;

		if ((vecEdge[0].z * vecToPos.x) - (vecEdge[0].x * vecToPos.z) < 0
		&&  (vecEdge[1].z * vecToPos.x) - (vecEdge[1].x * vecToPos.z) > 0)
		{ // 扇形の両縁の範囲内の場合

			// 当たっている状態にする
			bHit = true;
		}
	}

	// 当たり判定の結果を返す
	return bHit;
}

//==================================================================================
//	モデルの着地の更新処理
//==================================================================================
//	地面の上に立つかメッシュフィールドの上にいるかの判定に使用
//==================================================================================
float LandObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, bool *pbJump)
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

	// 着地点を返す
	return fLandPosY;
}

//==================================================================================
//	向きの正規化
//==================================================================================
//	あらゆる向きの正規化に使用
//==================================================================================
void RotNormalize(float *rot)
{
	if      (*rot >  D3DX_PI) { *rot -= D3DX_PI * 2; }
	else if (*rot < -D3DX_PI) { *rot += D3DX_PI * 2; }
}

//==================================================================================
//	ぶつかりの減速
//==================================================================================
//	ぶつかったときの減速に使用
//==================================================================================
void CollisionSlow(float *fMove)
{
	if (*fMove >= COLL_SLOW_MOVE)
	{ // 移動量が一定値以上の場合

		// 移動量を減速
		*fMove *= COLL_SLOW;

		if (*fMove < COLL_SLOW_MOVE)
		{ // 移動量が一定値より小さい場合

			// 移動量を補正
			*fMove = COLL_SLOW_MOVE;
		}
	}
}

//==================================================================================
//	ファイルをロードする全体処理
//==================================================================================
//	ファイルをロードする際に使用。引数で読み込むものをコントロールすることも可
//==================================================================================
void LoadFileChunk(bool bCurve, bool bHumanCurve, bool bStage, bool bCollision, bool bShadow, bool bObject, bool bAI, bool bIcon)
{
	if (bCollision == true)
	{ // 当たり判定を読み込む場合

		// 当たり判定のセットアップ
		TxtSetCollision();
	}

	if (bStage == true)
	{ // ステージを読み込む場合

		// ステージのセットアップ
		TxtSetStage();
	}

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

	if (bShadow == true)
	{ // 影を読み込む場合

		// 影の半径のセットアップ
		TxtSetShadow();
	}

	if (bIcon == true)
	{ // アイコンを読み込む場合

		// アイコンのセットアップ
		TxtSetIcon();
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

	// アイコンの初期化
	InitIcon();

	// 強調表示の初期化
	Init3DNotation();

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
	// 天気の終了
	UninitWeather();

	// 影の終了
	UninitShadow();

	// アイコンの終了
	UninitIcon();

	// 強調表示の終了
	Uninit3DNotation();

	// 警察の終了
	UninitPolice();

	// オブジェクトの終了
	UninitObject();

	// 車の終了
	UninitCar();

	// 人間の終了
	UninitHuman();

	// ゲートの終了
	UninitGate();

	// カメラの終了
	UninitCamera();

	// ライトの終了
	UninitLight();

	// メッシュドームの終了
	UninitMeshDome();

	// メッシュシリンダーの終了
	UninitMeshCylinder();

	// メッシュフィールドの終了
	UninitMeshField();

	// メッシュウォールの終了
	UninitMeshWall();

	// ビルボードの終了
	UninitBillboard();

	// エフェクトの終了
	UninitEffect();

	// パーティクルの終了
	UninitParticle();
}

//==================================================================================
//	万能描画の全体処理
//==================================================================================
void DrawAllAroundChunk(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// カメラの設定
	SetCamera(CAMERATYPE_MAIN);

	// メッシュドームの描画
	DrawMeshDome();

	// 雷の描画処理
	DrawThunder();

	// メッシュシリンダーの描画
	DrawMeshCylinder();

	// メッシュフィールドの描画
	DrawMeshField(false);

	// メッシュウォールの描画
	DrawMeshWall();

	// 影の描画
	DrawShadow();

	// オブジェクトの描画
	DrawObject();

	// 警察の描画
	DrawPolice();

	// 車の描画処理
	DrawCar();

	// 人間の描画
	DrawHuman();

	// ゲートの描画
	DrawGate();

	// ビルボードの描画
	DrawBillboard();

	// 爆弾の描画
	DrawBomb();

	// パーティクルの描画
	DrawParticle();

	// 天気の描画処理
	DrawWeather();
}

//==================================================================================
//	リザルトの初期化全体処理
//==================================================================================
void InitResultChunk(void)
{
	// 万能初期化の全体処理
	InitAllAroundChunk();

	// アイコンの初期化
	InitIcon();

	// フラッシュの初期化
	InitFlash();
}

//==================================================================================
//	リザルトの終了全体処理
//==================================================================================
void UninitResultChunk(void)
{
	// 万能終了の全体処理
	UninitAllAroundChunk();

	// アイコンの終了
	UninitIcon();

	// フラッシュの終了
	UninitFlash();
}

//==================================================================================
//	リザルトの描画全体処理
//==================================================================================
void DrawResultChunk(void)
{
	// 万能描画の全体処理
	DrawAllAroundChunk();
}

//==================================================================================
//　全てのアップデートが終わっているかどうかの判断処理
//==================================================================================
//　未終了のアップデートがないかを判断する処理
//==================================================================================
bool UpdateAllClear(RESULTSTATE state)
{
	// 変数を宣言
	bool bAllClear = true;	// 更新の終了確認用

	// ポインタを宣言
	Player      *pPlayer  = GetPlayer();			// プレイヤーの情報
	Gate        *pGate    = GetGateData();			// ゲートの情報
	Bonus       *pBonus   = GetBonus();				// ボーナスの情報

	//------------------------------------------------------------------------------
	//　未終了の確認
	//------------------------------------------------------------------------------
	switch (state)
	{ // リザルトの状態ごとの処理
	case RESULTSTATE_CLEAR:		// クリア状態

		// 無し

		// 処理を抜ける
		break;

	case RESULTSTATE_TIMEOVER:	// タイムオーバー状態

		if (pPlayer->bUse == true)
		{ // プレイヤーが使用されている場合

			if (fabsf(pPlayer->pos.x - pPlayer->oldPos.x) >= 0.5f
			||  fabsf(pPlayer->pos.z - pPlayer->oldPos.z) >= 0.5f)
			{ // 移動が一定値以上行われている場合

				// 更新を終了していない状態にする
				bAllClear = false;
			}
		}

		for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++, pGate++)
		{ // ゲートの最大表示数分繰り返す

			if (pGate->bUse == true)
			{ // ゲートが使用されている場合

				if (pGate->state != GATESTATE_STOP)
				{ // 停止状態ではない場合

					// 更新を終了していない状態にする
					bAllClear = false;

					// 処理を抜ける
					break;
				}
			}
		}

		// 処理を抜ける
		break;

	case RESULTSTATE_LIFEOVER:	// ライフオーバー状態

		// 無し

		// 処理を抜ける
		break;
	}

	for (int nCntBonus = 0; nCntBonus < MAX_BONUS; nCntBonus++, pBonus++)
	{ // ボーナスの最大表示数分繰り返す

		if (pBonus->bUse == true)
		{ // 使用されている場合

			// 更新を終了していない状態にする
			bAllClear = false;

			// 処理を抜ける
			break;
		}
	}

	//------------------------------------------------------------------------------
	//　リザルトに残さない情報の初期化
	//------------------------------------------------------------------------------
	// プレイヤーの情報の初期化
	pPlayer->boost.state   = BOOSTSTATE_NONE;	// プレイヤーのブースト状態
	pPlayer->bomb.state    = ATTACKSTATE_NONE;	// プレイヤーのボム状態
	pPlayer->wind.bUseWind = false;				// 風の使用状況
	pPlayer->icon.state    = ICONSTATE_NONE;	// アイコンの状態を通常状態にする

	// 更新状況を返す
	return bAllClear;
}

//==================================================================================
// ジャッジの更新処理
//==================================================================================
// 悪い奴の自己発光を変化させる処理
//==================================================================================
void UpdateJudge(Judge *pJudge)
{
	switch (pJudge->ticatica)
	{
	case CHICASTATE_BLACKOUT:		// 暗転状態

		// 暗転させていく
		pJudge->col.r -= JUDGE_FLASH;
		pJudge->col.g -= JUDGE_FLASH;
		pJudge->col.b -= JUDGE_FLASH;

		if (pJudge->col.r <= JUDGE_BLACK.r)
		{ // 色が一定数を超えた場合

			// 色を補正する
			pJudge->col = JUDGE_BLACK;

			// 明転状態にする
			pJudge->ticatica = CHICASTATE_WHITEOUT;
		}

		break;						// 抜け出す

	case CHICASTATE_WHITEOUT:		// 明転状態

		// 明転させていく
		pJudge->col.r += JUDGE_FLASH;
		pJudge->col.g += JUDGE_FLASH;
		pJudge->col.b += JUDGE_FLASH;

		if (pJudge->col.r >= JUDGE_WHITE.r)
		{ // 色が一定数を超えた場合

			// 色を補正する
			pJudge->col = JUDGE_WHITE;

			// 暗転状態にする
			pJudge->ticatica = CHICASTATE_BLACKOUT;
		}

		break;						// 抜け出す
	}
}