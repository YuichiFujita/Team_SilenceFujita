//===================================
//
// ���w�b�_�[[arrow.h]
// Author ��������
//
//===================================
#ifndef _ARROW_H_
#define _ARROW_H_

#include "main.h"

//�}�N����`

//�r���{�[�h�̍\����
typedef struct
{
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 rot;			// ����(���˂̕����ݒ�ɕK�v)
	D3DXMATRIX	mtx;			// ���[���h�}�g���b�N�X
	bool		bUse;			// �g�p��
}Arrow;

//�v���g�^�C�v�錾
void InitArrow(void);			// ���̏���������
void UninitArrow(void);			// ���̏I������
void UpdateArrow(void);			// ���̍X�V����
void DrawArrow(void);			// ���̕`�揈��
void SetArrow(bool bUse);		// ���̐ݒ菈��

#endif