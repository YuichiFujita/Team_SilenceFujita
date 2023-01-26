#ifdef _DEBUG	// �f�o�b�O����
//===================================
//
//	�G�f�B�b�g�w�b�_�[ [EditCollision.h]
//	Author�F���c�E��
//
//===================================
#ifndef _EDITCOLLISION_H_
#define _EDITCOLLISION_H_

#include "main.h"
#include "model.h"
#include "object.h"

//�Z�b�g�I�u�W�F�N�g�̏��
typedef struct
{
	D3DXVECTOR3 pos[MAX_COLLISION];		// �ʒu
	D3DXVECTOR3 rot;					// ����
	ROTSTATE    stateRot;				// �������
	Collision   collision;				// �����蔻����
	D3DXMATRIX  mtxWorld;				// ���[���h�}�g���b�N�X
	Model       modelData;				// ���f�����
	int         nSelectColl;			// �I�𒆂̓����蔻��
}EditCollision;

//�v���g�^�C�v�錾
void InitEditCollision(void);			// �����蔻��̏���������
void UninitEditCollision(void);			// �����蔻��̏I������
void UpdateEditCollision(void);			// �����蔻��̍X�V����
void DrawEditCollision(void);			// �����蔻��̕`�揈��
EditCollision *GetEditCollision(void);	// �����蔻��̎擾����

#endif

#endif