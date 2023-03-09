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
#include "icon.h"

//************************************************************
//	�}�N����`
//************************************************************
#define PLAY_WIDTH			(100.0f)	// �v���C���[�̉��� / 2
#define PLAY_HEIGHT			(25.0f)		// �v���C���[�̏c��
#define PLAY_DEPTH			(100.0f)	// �v���C���[�̉��s / 2

#define MAX_FORWARD			(35.0f)						// �O�i���̍ō����x
#define MAX_BOOST			(15.0f)						// �u�[�X�g�̍ő�ړ���
#define MAX_REAL_SPEED		(MAX_FORWARD + MAX_BOOST)	// ���l��̍ō����x

#define PLAY_REV_ROT_MIN	(0.75f)		// �v���C���[�̌����ύX�ʂ̌����W���̍ŏ��l
#define PLAY_LIFE			(100)		// �v���C���[�̗̑�
#define PLAY_DAMAGE			(10)		// �v���C���[�̃_���[�W��
#define BOOST_WAIT_CNT		(180)		// �u�[�X�g�̍Ďg�p�܂ł̎���
#define WIND_OVERHEAT_CNT	(180)		// �E�B���h�̃I�[�o�[�q�[�g�܂ł̎���
#define BOMB_WAIT_CNT		(2400)		// �{���̍Ďg�p�܂ł̎���

#define DAMAGE_TIME_PLAY	(90)						// �_���[�W��Ԃ�ۂ���
#define UNR_TIME_PLAY		(DAMAGE_TIME_PLAY - 10)		// ���G��ԂɕύX���鎞��

//************************************************************
//	�񋓌^��` (ATTACKSTATE)
//************************************************************
typedef enum
{
	ATTACKSTATE_NONE = 0,			// �������Ȃ����
	ATTACKSTATE_WAIT,				// �U���ҋ@���
	ATTACKSTATE_HEAL,				// �Q�[�W�񕜏��
	ATTACKSTATE_MAX					// ���̗񋓌^�̑���
}ATTACKSTATE;

//************************************************************
//	�񋓌^��` (PLAYCAMESTATE)
//************************************************************
typedef enum
{
	PLAYCAMESTATE_NORMAL = 0,		// �ʏ�̃J����
	PLAYCAMESTATE_BACK,				// ��l�̃J����
	PLAYCAMESTATE_MAX				// ���̗񋓌^�̑���
}PLAYCAMESTATE;

//************************************************************
//	�񋓌^��` (BOOSTSTATE)
//************************************************************
typedef enum
{
	BOOSTSTATE_NONE = 0,			// �������Ȃ����
	BOOSTSTATE_UP,					// �������
	BOOSTSTATE_DOWN,				// �������
	BOOSTSTATE_WAIT,				// �g�p�\�̑ҋ@���
	BOOSTSTATE_MAX					// ���̗񋓌^�̑���
}BOOSTSTATE;

//************************************************************
//	�\���̒�` (PlayerBoost)
//************************************************************
typedef struct
{
	D3DXVECTOR3 plusMove;			// �ǉ��ړ���
	BOOSTSTATE  state;				// �������
	int         nCounter;			// �����Ǘ��J�E���^�[
}PlayerBoost;

//************************************************************
//	�\���̒�` (PlayerWind)
//************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// �����o���ʒu
	D3DXVECTOR3	rot;				// ��΂�����
	int			nCount;				// �����o���J�E���g
	int			nCircleCount;		// �����o���ʒu�̃J�E���g
	bool		bUseWind;			// ���̎g�p��
}PlayerWind;

//************************************************************
//	�\���̒�` (PlayerBomb)
//************************************************************
typedef struct
{
	ATTACKSTATE state;				// �U�����
	int         nCounterState;		// �U���Ǘ��J�E���^�[
	int         nCounterControl;	// ����Ǘ��J�E���^�[
	int			nHeal;				// �Q�[�W�̉񕜗�
	bool        bShot;				// ���ˑҋ@��
}PlayerBomb;

//************************************************************
//	�\���̒�` (Player)
//************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// ���݂̈ʒu
	D3DXVECTOR3 oldPos;				// �O��̈ʒu
	D3DXVECTOR3 move;				// �ړ���
	D3DXVECTOR3 rot;				// ����
	D3DXVECTOR3 moveRot;			// �����ύX��
	D3DXMATRIX  mtxWorld;			// ���[���h�}�g���b�N�X
	ACTIONSTATE state;				// �v���C���[�̏��
	Model       modelData;			// ���f�����
	PlayerBoost boost;				// �u�[�X�g�̏��
	PlayerWind  wind;				// ���̏��
	PlayerBomb  bomb;				// ���e�̏��
	int         nLife;				// �̗�
	int         nCounterState;		// ��ԊǗ��J�E���^�[
	int         nShadowID;			// �e�̃C���f�b�N�X
	int			nNumEnterGate;		// �o�Ă���Q�[�g
	IconInfo	icon;				// �A�C�R���̏��
	int			nCameraState;		// �O�����J�����̏��
	bool		bCameraFirst;		// ��l�̃J�����̏�
	bool        bMove;				// �ړ���
	bool        bJump;				// �W�����v��
	bool		bUnrivaled;			// ������
	bool        bUse;				// �g�p��
}Player;

//************************************************************
//	�v���g�^�C�v�錾
//************************************************************
void InitPlayer(void);				// �v���C���[�̏���������
void UninitPlayer(void);			// �v���C���[�̏I������
void UpdateGamePlayer(void);		// �v���C���[�̃Q�[���X�V����
void UpdateTutorialPlayer(void);	// �v���C���[�̃`���[�g���A���X�V����
void DrawPlayer(void);				// �v���C���[�̕`�揈��

void SetPositionPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// �v���C���[�̈ʒu�E�����̐ݒ菈��
void SetPlayerGate(void);									// �v���C���[�̃Q�[�g�̐ݒ菈��

void HealPlayer(Player *pPlayer, int nHeal);	// �v���C���[�̉񕜔���
void HealBarrier(Player *pPlayer, int nHeal);	// �o���A�̉񕜔���
void HitPlayer(Player *pPlayer, int nDamage);	// �v���C���[�̃_���[�W����
Player *GetPlayer(void);						// �v���C���[�̎擾����

//************************************************************
//	�v���g�^�C�v�錾 (�f�o�b�O�p)
//************************************************************

#endif