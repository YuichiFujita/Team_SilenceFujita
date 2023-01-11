//=============================================================================
//
// �T�E���hDJ���� [SoundDJ.h]
// Author ��������
//
//=============================================================================
#ifndef _SOUNDDJ_H_
#define _SOUNDDJ_H_

#include "main.h"

//=============================================================================
// �T�E���h�ꗗ
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
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitSoundDJ(HWND hWnd);
void UninitSoundDJ(void);
HRESULT PlaySound(int label, bool DJSign);
void StopSoundDJ(SOUND_DJ_LABEL label);
void StopSoundDJ(void);
HRESULT ReplaySoundDJ(SOUND_DJ_LABEL label);

#endif
