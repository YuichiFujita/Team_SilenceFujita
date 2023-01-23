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

//�񋓌^��` (ROTSTATE)
typedef enum
{
	ROTSTATE_0 = 0,						// ���� 0�x (360�x)
	ROTSTATE_90,						// ���� 90�x
	ROTSTATE_180,						// ���� 180�x
	ROTSTATE_270,						// ���� 270�x
	ROTSTATE_MAX,						// ���̗񋓌^�̑���
} ROTSTATE;

//�Z�b�g�I�u�W�F�N�g�̏��
typedef struct
{
	D3DXVECTOR3 pos;					// �ʒu
	D3DXVECTOR3 rot;					// ����
	ROTSTATE    stateRot;				// �������
	D3DXVECTOR3 scale;					// �g�嗦
	Collision   collision;				// �����蔻����
	D3DXMATRIX  mtxWorld;				// ���[���h�}�g���b�N�X
	Model       modelData;				// ���f�����
	int         nType;					// �I�u�W�F�N�g�̎��
}EditCollision;

//�v���g�^�C�v�錾
void InitEditCollision(void);			// �����蔻��̏���������
void UninitEditCollision(void);			// �����蔻��̏I������
void UpdateEditCollision(void);			// �����蔻��̍X�V����
void DrawEditCollision(void);			// �����蔻��̕`�揈��
EditCollision *GetEditCollision(void);	// �����蔻��̎擾����

#endif

#endif