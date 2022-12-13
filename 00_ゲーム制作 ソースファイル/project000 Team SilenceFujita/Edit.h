//===================================
//
//�G�f�B�b�g�w�b�_�[[Edit.h]
//Author ��������
//
//===================================
#ifndef _EDIT_H_
#define _EDIT_H_

#include "main.h"
#include "game.h"

//�}�N����`
#define MAX_OBJECT					(256)			//�I�u�W�F�N�g�̍ő吔

//�Z�b�g�I�u�W�F�N�g�̏��
typedef struct
{
	D3DXVECTOR3 pos;									//�ʒu
	D3DXVECTOR3 rot;									//����
	D3DXVECTOR3 scale;									//�g�嗦
	bool bSet;											//�I�u�W�F�N�g���Z�b�g��Ԃ��ǂ���
	int nSetNumber;										//�I�u�W�F�N�g�̔ԍ�
	int nType;											//���
	D3DXMATRIX mtx;										//���[���h�}�g���b�N�X
	Doll pDollData;										//���f���̊�{���
}EditObject;

//�v���g�^�C�v�錾
void InitEditObject(void);								//�I�u�W�F�N�g�̏���������
void UninitEditObject(void);							//�I�u�W�F�N�g�̏I������
void UpdateEditObject(void);							//�I�u�W�F�N�g�̍X�V����
void DrawEditObject(void);								//�I�u�W�F�N�g�̕`�揈��
EditObject GetEdit(void);								//�G�f�B�b�g�I�u�W�F�N�g�̎擾����

#endif