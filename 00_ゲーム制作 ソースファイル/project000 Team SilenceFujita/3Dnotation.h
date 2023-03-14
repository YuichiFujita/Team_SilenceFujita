//============================================================
//
//	�����\���w�b�_�[ [3Dnotation.h]
//	Author�F���c�E��
//
//============================================================
#ifndef _3D_NOTATION_H_		// ���̃}�N����`������Ă��Ȃ��ꍇ
#define _3D_NOTATION_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�}�N����`
//************************************************************
#define NONE_3D_NOTATION	(-1)		// �����\���̏����l (�g�p���Ă��Ȃ��ꍇ)
#define NOTA_PLUS_POS_OBJ	(250.0f)	// �����̋����\���� y���W���Z��
#define NOTA_PLUS_POS_CAR	(130.0f)	// �Ԃ̋����\���� y���W���Z��
#define NOTA_PLUS_POS_EXIT	(300.0f)	// �E�o�̋����\���� y���W���Z��
#define NOTA_PLUS_POS_HUMAN	(40.0f)	// �l�Ԃ̋����\���� y���W���Z��

//************************************************************
//	�񋓌^��` (NOTATIONTYPE)
//************************************************************
typedef enum
{
	NOTATIONTYPE_BREAK = 0,			// �j��
	NOTATIONTYPE_SHOT,				// �ˏo
	NOTATIONTYPE_FLY,				// ����
	NOTATIONTYPE_EXIT,				// �E�o
	NOTATIONTYPE_MAX,				// ���̗񋓌^�̑���
} NOTATIONTYPE;

//************************************************************
//	�v���g�^�C�v�錾
//************************************************************
void Init3DNotation(void);			// �����\���̏���������
void Uninit3DNotation(void);		// �����\���̏I������
void Update3DNotation(void);		// �����\���̍X�V����
void Draw3DNotation(void);			// �����\���̕`�揈��

int  Set3DNotation(NOTATIONTYPE type, int *p3DNotationID, bool *pUse);	// �����\���̐ݒ菈��
void SetPosition3DNotation(int n3DNotationID, D3DXVECTOR3 pos);			// �����\���̈ʒu�̐ݒ菈��

#endif