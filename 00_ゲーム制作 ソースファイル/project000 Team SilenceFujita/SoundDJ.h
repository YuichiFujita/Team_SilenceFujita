//=============================================================================
//
// サウンドDJ処理 [SoundDJ.h]
// Author 小原立暉
//
//=============================================================================
#ifndef _SOUNDDJ_H_
#define _SOUNDDJ_H_

#include "main.h"

//=============================================================================
// サウンド一覧
//=============================================================================
typedef enum
{
	FUJITA_DJ_LABEL_ONE = 0,
	FUJITA_DJ_LABEL_TWO,
	FUJITA_DJ_LABEL_THREE,
	FUJITA_DJ_LABEL_FOUR,
	FUJITA_DJ_LABEL_FIVE,
	SOUND_DJ_LABEL_MAX,
} SOUND_DJ_LABEL;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitSoundDJ(HWND hWnd);
void UninitSoundDJ(void);
HRESULT PlaySound(int label, bool DJSign);
void StopSoundDJ(SOUND_DJ_LABEL label);
void StopSoundDJ(void);
HRESULT ReplaySoundDJ(SOUND_DJ_LABEL label);

#endif
