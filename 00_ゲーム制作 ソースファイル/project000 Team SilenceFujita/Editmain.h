//===================================
//
//�G�f�B�b�g���C����ʃw�b�_�[[Editmain.h]
//Author ��������
//
//===================================
#ifndef _EDITMAIN_H_			//���̃}�N����`������Ă��Ȃ�������
#define _EDITMAIN_H_			//2�d�C���N���[�h�h�~�̃}�N����ݒ肷��

#include"main.h"

//�I�u�W�F�N�g�̎��
typedef enum
{
	EDITSTYLE_OBJECT = 0,								//�I�u�W�F�N�g
	EDITSTYLE_BILLBOARD,								//�r���{�[�h
	EDITSTYLE_MAX
}EDITSTYLE;

//�v���g�^�C�v�錾
void InitEditmain(void);						//�G�f�B�b�g���C����ʂ̏���������
void UninitEditmain(void);						//�G�f�B�b�g���C����ʂ̏I������
void UpdateEditmain(void);						//�G�f�B�b�g���C����ʂ̍X�V����
void DrawEditmain(void);						//�G�f�B�b�g���C����ʂ̕`�揈��
int GetStyle(void);								//�X�^�C���̎擾����
void StyleChange(void);							//�X�^�C���ύX����

#endif