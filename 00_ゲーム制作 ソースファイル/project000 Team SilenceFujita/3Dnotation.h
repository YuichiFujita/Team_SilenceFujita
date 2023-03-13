//=======================================================================================================
//
//	�e�w�b�_�[ [shadow.h]
//	Author�F���c�E��
//
//=======================================================================================================
#ifndef _SHADOW_H_				// ���̃}�N����`������Ă��Ȃ��ꍇ
#define _SHADOW_H_				// ��d�C���N���[�h�h�~�̃}�N�����`����

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "model.h"

//************************************************************
//	�}�N����`
//************************************************************
#define NONE_SHADOW		(-1)	// �e�̏����l (�g�p���Ă��Ȃ��ꍇ)

#define NONE_ROT		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		// �����̏����l
#define NONE_SCALE		(D3DXVECTOR3(1.0f, 1.0f, 1.0f))		// �g�嗦�̏����l

#define FIRST_RADIUS	(60.0f)	// ���������̃I�u�W�F�N�g�̉e�̔��a

//************************************************************
//	�񋓌^��` (SHADOWTYPE)
//************************************************************
typedef enum
{
	SHADOWTYPE_NONE = 0,		// �e����
	SHADOWTYPE_CIRCLE,			// �ۉe
	SHADOWTYPE_MODEL,			// ���f���e
	SHADOWTYPE_MAX,				// ���̗񋓌^�̑���
} SHADOWTYPE;

//************************************************************
//	�v���g�^�C�v�錾
//************************************************************
void InitShadow(void);			// �e�̏���������
void UninitShadow(void);		// �e�̏I������
void UpdateShadow(void);		// �e�̍X�V����
void DrawShadow(void);			// �e�̕`�揈��

int  SetCircleShadow(float fAlpha, float fRadius, int *pShadowID, bool *pUse);				// �ۉe�̐ݒ菈��
int  SetModelShadow(Model model, int *pShadowID, bool *pUse);								// ���f���e�̐ݒ菈��
void SetPositionShadow(int nShadowID, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale);	// �e�̈ʒu�̐ݒ菈��


#endif