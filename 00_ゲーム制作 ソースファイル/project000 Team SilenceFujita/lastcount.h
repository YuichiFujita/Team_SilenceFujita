//===========================================
//
// ���X�g�J�E���g�w�b�_�[[lastcount.h]
// Author ��������
//
//===========================================
#ifndef _LASTCOUNT_H_
#define _LASTCOUNT_H_

#include"main.h"

//�J�E���g�_�E���̍\����
typedef struct
{
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 size;				// ���a
	int nCount;						// �J�E���g
	int nTime;						// ����
	int nTimeOld;					// �O��̎���
	int nDigit;						// ����
	float fClear;					// �����x�̒l
	bool bUse;						// �g�p���Ă��邩�ǂ���
}LASTCOUNT;

//�v���g�^�C�v�錾
void InitLastCount(void);			// ���X�g�J�E���g�̏���������
void UninitLastCount(void);			// ���X�g�J�E���g�̏I������
void UpdateLastCount(void);			// ���X�g�J�E���g�̍X�V����
void DrawLastCount(void);			// ���X�g�J�E���g�̕`�揈��

#endif