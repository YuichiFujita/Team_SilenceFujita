//===================================
//
// �Č��z�^�C�}�[�w�b�_�[[buildtimer.h]
// Author ��������
//
//===================================
#ifndef _BUILDTIMER_H_
#define _BUILDTIMER_H_

#include "main.h"
#include "object.h"

//�}�N����`
#define MAX_BUILDTIMER			(256)	//�Č��z�^�C�}�[�̍ő吔

//�v���g�^�C�v�錾
void InitBuildtimer(void);				// �Č��z�̏���������
void UninitBuildtimer(void);			// �Č��z�̏I������
void UpdateBuildtimer(void);			// �Č��z�̍X�V����
void DrawBuildtimer(void);				// �Č��z�̕`�揈��
void SetBuildtimer(D3DXVECTOR3 pos, int nCount, Object *pObject);	// �Č��z�̐ݒ菈��

#endif