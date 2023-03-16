//===========================================
//
//�J�E���g�_�E���w�b�_�[[Countdown.h]
//Author ��������
//
//===========================================
#ifndef _COUNTDOWN_H_
#define _COUNTDOWN_H_

#include"main.h"

//===============================================
// �񋓌^��`(CNTDOWNSTATE)
//===============================================
typedef enum
{
	CNTDOWNSTATE_NONE = 0,				// �����
	CNTDOWNSTATE_APPEAR,				// �o�����
	CNTDOWNSTATE_TRANS,					// ���������
	CNTDOWNSTATE_MAX					// ���̗񋓌^�̑���
}CNTDOWNSTATE;

//===============================================
// �񋓌^��`(CNTDOWNTYPE)
//===============================================
typedef enum
{
	CNTDOWNTYPE_1MIN = 0,				// �c��1��
	CNTDOWNTYPE_30SEC,					// �c��30�b
	CNTDOWNTYPE_MAX						// ���̗񋓌^�̑���
}CNTDOWNTYPE;

//===============================================
// ��Ԋ֌W�̍\����
//===============================================
typedef struct
{
	CNTDOWNSTATE state;					// ���
	int			 nStateCounter;			// ��Ԃ̃J�E���g
}CountDownState;

//===============================================
// �J�E���g�_�E���̍\����
//===============================================
typedef struct
{
	D3DXVECTOR3		pos;				// �ʒu
	D3DXVECTOR3		length;				// ����
	float			fTexU;				// �e�N�X�`����U���̒l
	float			fAlpha;				// �����x
	CountDownState	stateInfo;			// ��Ԋ֌W�̏��
	CNTDOWNTYPE		type;				// ���
	bool			bUse;				// �g�p���Ă��邩�ǂ���
}CountDown;

//�v���g�^�C�v�錾
void InitCountDown(void);				// �J�E���g�_�E���̏���������
void UninitCountDown(void);				// �J�E���g�_�E���̏I������
void UpdateCountDown(void);				// �J�E���g�_�E���̍X�V����
void DrawCountDown(void);				// �J�E���g�_�E���̕`�揈��
void SetCountDown(CNTDOWNTYPE type);	// �J�E���g�_�E���̐ݒ菈��

#endif