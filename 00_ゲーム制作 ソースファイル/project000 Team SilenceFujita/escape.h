//===========================================
//
//�E�o�ʒm�w�b�_�[[escape.h]
//Author ��������
//
//===========================================
#ifndef _ESCAPE_H_
#define _ESCAPE_H_

#include"main.h"

//===============================================
// �񋓌^��`(ESCAPESTATE)
//===============================================
typedef enum
{
	ESCAPESTATE_NONE = 0,			// �����
	ESCAPESTATE_DISP,				// �\�����
	ESCAPESTATE_EMPHASIS,			// �������
	ESCAPESTATE_MAX					// ���̗񋓌^�̑���
}ESCAPESTATE;

//===============================================
// �񋓌^��`(EMPHASISSTATE)
//===============================================
typedef enum
{
	EMPHASISSTATE_BIGGER = 0,		// �g�咆���
	EMPHASISSTATE_SMALLER,			// �k�������
	EMPHASISSTATE_MAX				// ���̗񋓌^�̑���
}EMPHASISSTATE;

//===============================================
// ��Ԋ֌W�̍\����
//===============================================
typedef struct
{
	ESCAPESTATE	  escapeState;			// ���
	EMPHASISSTATE emphaState;			// �������
	int			  nStateCounter;		// ��Ԃ̃J�E���g
}EscapeState;

//===============================================
// �J�E���g�_�E���̍\����
//===============================================
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		length;			// ����
	EscapeState		stateInfo;		// �E�o��Ԋ֌W
	D3DXCOLOR		col;			// �F
	bool			bUse;			// �g�p���Ă��邩�ǂ���
}EscapeInfo;

//�v���g�^�C�v�錾
void InitEscape(void);				// �J�E���g�_�E���̏���������
void UninitEscape(void);			// �J�E���g�_�E���̏I������
void UpdateEscape(void);			// �J�E���g�_�E���̍X�V����
void DrawEscape(void);				// �J�E���g�_�E���̕`�揈��
void SetEscape(void);				// �J�E���g�_�E���̐ݒ菈��

#endif