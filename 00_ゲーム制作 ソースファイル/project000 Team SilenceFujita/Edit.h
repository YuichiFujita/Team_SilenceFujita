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

//�Z�b�g�I�u�W�F�N�g�̏��
typedef struct
{
	D3DXVECTOR3 pos;									//�ʒu
	D3DXVECTOR3 rot;									//����
	D3DXVECTOR3 scale;									//�g�嗦
	bool bUse;											//�I�u�W�F�N�g���g�p�\���ǂ���
	int nSetNumber;										//�I�u�W�F�N�g�̔ԍ�
	int nType;											//���
	D3DXMATRIX mtx;										//���[���h�}�g���b�N�X
	Doll pDollData;										//���f���̊�{���
	int nCntMaterial;									//�}�e���A���̔ԍ�
	int nColorCount;									//�F��ς���Ƃ��̃J�E���g
}EditObject;

//�G�f�B�b�g�}�e���A���̏��
typedef struct
{
	D3DXMATERIAL EditMaterial[MAX_MATERIAL] = {};		//�J�X�^���p�̃}�e���A�����
}EditMaterial;

//�v���g�^�C�v�錾
void InitEditObject(void);								//�I�u�W�F�N�g�̏���������
void UninitEditObject(void);							//�I�u�W�F�N�g�̏I������
void UpdateEditObject(void);							//�I�u�W�F�N�g�̍X�V����
void DrawEditObject(void);								//�I�u�W�F�N�g�̕`�揈��
EditObject *GetEditObject(void);						//�G�f�B�b�g�I�u�W�F�N�g�̎擾����
EditMaterial GetCustomMaterial(void);					//�}�e���A���̏��擾����

#endif