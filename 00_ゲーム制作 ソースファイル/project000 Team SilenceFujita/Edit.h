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
#include "billboard.h"

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
}EditObject;

//�Z�b�g�r���{�[�h�̏��
typedef struct
{
	D3DXVECTOR3 pos;									//�ʒu
	D3DXVECTOR2 Radius;									//��
	bool bUse;											//�r���{�[�h���g�p�\���ǂ���
	int nSetNumber;										//�r���{�[�h�̔ԍ�
	int nType;											//���
	D3DXMATRIX mtx;										//���[���h�}�g���b�N�X
	LPDIRECT3DTEXTURE9 Texture[BILLBOARD_MAX];			//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 VtxBuff;					//���_�o�b�t�@�ւ̃|�C���^
}EditBillboard;

//�I�u�W�F�N�g�̎��
typedef enum
{
	EDITSTYLE_OBJECT = 0,								//�I�u�W�F�N�g
	EDITSTYLE_BILLBOARD,								//�r���{�[�h
	EDITSTYLE_MAX
}EDITSTYLE;

//�v���g�^�C�v�錾
void InitEditObject(void);								//�I�u�W�F�N�g�̏���������
void InitEditBillboard(void);							//�r���{�[�h�̏���������
void UninitEditObject(void);							//�I�u�W�F�N�g�̏I������
void UpdateEditObject(void);							//�I�u�W�F�N�g�̍X�V����
void DrawEditObject(void);								//�I�u�W�F�N�g�̕`�揈��
void DrawEditBillboard(void);							//�r���{�[�h�̕`�揈��
EditObject GetEditObject(void);							//�G�f�B�b�g�I�u�W�F�N�g�̎擾����
EditBillboard GetEditBillboard(void);					//�G�f�B�b�g�r���{�[�h�̎擾����
int GetStyle(void);										//�G�f�B�b�g�X�^�C���̎擾����

#endif