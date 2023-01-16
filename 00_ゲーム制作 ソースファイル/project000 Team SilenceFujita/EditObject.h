#ifdef _DEBUG	// �f�o�b�O����
//===================================
//
//�G�f�B�b�g�w�b�_�[[Edit.h]
//Author ��������
//
//===================================
#ifndef _EDIT_H_
#define _EDIT_H_

#include "main.h"
#include "object.h"
#include "game.h"
#include "shadow.h"

//�j��֌W�̍\����
typedef struct
{
	char *pBreakMode[BREAKTYPE_MAX];					//�j�󃂁[�h�̕\�L
	int Breaktype;										//�j��̏��
}BREAK;

//�e�֌W�̍\����
typedef struct
{
	char *pShadowMode[SHADOWTYPE_MAX];					//�e�̎�ނ̕\�L
	int Shadowtype;										//�e�̎��
}EDITSHADOW;

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
	Model modelData;									//���f���̊�{���
	int nCntMaterial;									//�}�e���A���̔ԍ�
	int nColorCount;									//�F��ς���Ƃ��̃J�E���g
	D3DXMATERIAL MatCopy[MODELTYPE_OBJECT_MAX][MAX_MATERIAL];			//�}�e���A���̃R�s�[
	D3DXMATERIAL EditMaterial[MODELTYPE_OBJECT_MAX][MAX_MATERIAL];		//�J�X�^���p�̃}�e���A�����
	BREAK Break;										//�j��̕ϐ�
	EDITSHADOW Shadowtype;								//�e�̕ϐ�
}EditObject;

//�v���g�^�C�v�錾
void InitEditObject(void);								//�I�u�W�F�N�g�̏���������
void UninitEditObject(void);							//�I�u�W�F�N�g�̏I������
void UpdateEditObject(void);							//�I�u�W�F�N�g�̍X�V����
void DrawEditObject(void);								//�I�u�W�F�N�g�̕`�揈��
EditObject *GetEditObject(void);						//�G�f�B�b�g�I�u�W�F�N�g�̎擾����

#endif

#endif