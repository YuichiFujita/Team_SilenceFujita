//=======================================================================================================
//
//	�A�C�R���w�b�_�[ [icon.h]
//	Author�F��������
//
//=======================================================================================================
#ifndef _ICON_H_				// ���̃}�N����`������Ă��Ȃ��ꍇ
#define _ICON_H_				// ��d�C���N���[�h�h�~�̃}�N�����`����

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "game.h"
#include "model.h"

//************************************************************
//	�}�N����`
//************************************************************
#define NONE_ICON		(-1)	// �A�C�R���̏����l (�g�p���Ă��Ȃ��ꍇ)

//************************************************************
//	�񋓌^��`�iICONTYPE�j
//************************************************************
typedef enum
{
	ICONTYPE_EVIL_OBJECT = 0,	// �����I�u�W�F�N�g
	ICONTYPE_OBJECT,			// �ǂ��I�u�W�F�N�g
	ICONTYPE_EVIL_HUMAN,		// �����l�Ԃ̃A�C�R��
	ICONTYPE_EVIL_CAR,			// �����Ԃ̃A�C�R��
	ICONTYPE_POLICE,			// �x�@�̃A�C�R��
	ICONTYPE_BARRIER,			// �o���A�̃A�C�R��
	ICONTYPE_ITEM,				// �A�C�e��
	ICONTYPE_GATE_VERT,			// �Q�[�g�̉���O�̃A�C�R��
	ICONTYPE_GATE_HORIZ,		// �Q�[�g�̍��E�̃A�C�R��
	ICONTYPE_PLAY,				// �v���C���[�̃A�C�R��
	ICONTYPE_MAX				// ���̗񋓌^�̑���
}ICONTYPE;

//************************************************************
//	�񋓌^��`�iICONSTATE�j
//************************************************************
typedef enum
{
	ICONSTATE_NONE = 0,			// �����Ȃ�
	ICONSTATE_DISAPPEAR,		// ������r��
	ICONSTATE_REVIVAL,			// ������
	ICONSTATE_DAMAGE,			// �_���[�W��
	ICONSTATE_UNRIVALED,		// ���G���
	ICONSTATE_FLYAWAY,			// �l�����ł�����
	ICONSTATE_ENLARGE,			// �g����
	ICONSTATE_REDUCE,			// �k�����
	ICONSTATE_MAX				// ���̗񋓌^�̑���
}ICONSTATE;

//************************************************************
//	�񋓌^��`�iICONALPHA�j
//************************************************************
typedef enum
{
	ICONALPHA_NONE = 0,			// �s����
	ICONALPHA_CLEAR,			// ����
	ICONALPHA_MAX				// ���̗񋓌^�̒�`
}ICONALPHA;

//************************************************************
//	�\���̒�` (IconInfo)
//************************************************************
typedef struct
{
	int nIconID;					// �A�C�R���̃C���f�b�N�X
	ICONSTATE state;				// �A�C�R���̏��
} IconInfo;

//************************************************************
//	�\���̒�` (Icon)
//************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// �ʒu
	ICONTYPE	type;				// ���
	D3DXCOLOR	col;				// �F
	D3DXCOLOR	colCopy;			// �F�̃R�s�[
	ICONALPHA	alpha;				// �����x�̏��
	D3DXVECTOR3	radius;				// ���a
	D3DXMATRIX  mtxWorld;			// ���[���h�}�g���b�N�X
	int        *pIconIDParent;		// �A�C�R���̐e�̃A�C�R���C���f�b�N�X
	int			nCounter;			// �J�E���^�[
	bool       *pUseParent;			// �A�C�R���̐e�̎g�p��
	ICONSTATE  *pState;				// �A�C�R���̏��
	bool        bUse;				// �g�p��
} Icon;

//************************************************************
//	�v���g�^�C�v�錾
//************************************************************
void InitIcon(void);		// �A�C�R���̏���������
void UninitIcon(void);		// �A�C�R���̏I������
void UpdateIcon(void);		// �A�C�R���̍X�V����
void DrawIcon(void);		// �A�C�R���̕`�揈��
int SetIcon(D3DXVECTOR3 pos, ICONTYPE type, int *pIconID, bool *pUse, ICONSTATE *pState);			// �A�C�R���̐ݒ菈��
int SetIconObject(D3DXVECTOR3 radius, D3DXVECTOR3 pos, JUDGESTATE type, int *pIconID, bool *pUse, ICONSTATE *pState, ROTSTATE rot);	// �I�u�W�F�N�g�̃A�C�R���̐ݒ菈��

void SetPositionIcon(int nIconID, D3DXVECTOR3 pos);			// �A�C�R���̈ʒu�̐ݒ菈��


#endif