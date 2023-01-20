//============================================================
//
//	�v���C���[�w�b�_�[ [player.h]
//	Author�F���c�E��
//
//============================================================
#ifndef _PLAYER_H_	// ���̃}�N����`������Ă��Ȃ��ꍇ
#define _PLAYER_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "model.h"

//************************************************************
//	�}�N����`
//************************************************************
#define PLAY_WIDTH			(18.0f)	// �v���C���[�̉��� / 2
#define PLAY_HEIGHT			(55.0f)	// �v���C���[�̏c��
#define PLAY_DEPTH			(18.0f)	// �v���C���[�̉��s / 2

#define PLAY_LIFE			(100)	// �v���C���[�̗̑�
#define MAX_FORWARD			(35.0f)	// �O�i���̍ō����x

#define DAMAGE_TIME_PLAY	(30)					// �_���[�W��Ԃ�ۂ���
#define UNR_TIME_PLAY		(DAMAGE_TIME_PLAY - 10)	// ���G��ԂɕύX���鎞��

//************************************************************
//	�J�����̗� (PlayerCamera)
//************************************************************
typedef enum
{
	PLAYCAMESTATE_NORMAL = 0,		// �ʏ�̃J����
	PLAYCAMESTATE_BACK,				// ��l�̃J����
	PLAYCAMESTATE_MAX				// ���̗񋓌^�̑���
}PLAYCAMESTATE;

//************************************************************
//	�\���̒�` (Player)
//************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// ���݂̈ʒu
	D3DXVECTOR3 oldPos;				// �O��̈ʒu
	D3DXVECTOR3 move;				// �ړ���
	D3DXVECTOR3 rot;				// ���݂̌���
	D3DXVECTOR3 destRot;			// �ڕW�̌���
	D3DXMATRIX  mtxWorld;			// ���[���h�}�g���b�N�X
	ACTIONSTATE state;				// �v���C���[�̏��
	Model       modelData;			// ���f�����
	int         nLife;				// �̗�
	int         nCounterState;		// ��ԊǗ��J�E���^�[
	int         nShadowID;			// �e�̃C���f�b�N�X
	int			nCameraState;		// �O�����J�����̏��
	bool		bCameraFirst;		// ��l�̃J�����̏�
	bool        bMove;				// �ړ���
	bool        bJump;				// �W�����v��
	bool        bUse;				// �g�p��
}Player;

//************************************************************
//	�v���g�^�C�v�錾
//************************************************************
void InitPlayer(void);		// �v���C���[�̏���������
void UninitPlayer(void);	// �v���C���[�̏I������
void UpdatePlayer(void);	// �v���C���[�̍X�V����
void DrawPlayer(void);		// �v���C���[�̕`�揈��

void SetPositionPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// �v���C���[�̈ʒu�E�����̐ݒ菈��
void HealPlayer(Player *pPlayer, int nHeal);				// �v���C���[�̉񕜔���
void HitPlayer(Player *pPlayer, int nDamage);				// �v���C���[�̃_���[�W����
Player *GetPlayer(void);									// �v���C���[�̎擾����

//************************************************************
//	�v���g�^�C�v�錾 (�f�o�b�O�p)
//************************************************************

#endif