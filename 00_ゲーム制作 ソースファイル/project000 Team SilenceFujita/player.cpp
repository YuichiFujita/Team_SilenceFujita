//============================================================
//
//	�v���C���[���� [player.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "input.h"
#include "tutorial.h"
#include "game.h"
#include "calculation.h"
#include "sound.h"

#include "camera.h"
#include "Combo.h"
#include "gate.h"
#include "object.h"
#include "particle.h"
#include "player.h"
#include "shadow.h"
#include "sound.h"
#include "life.h"
#include "bomb.h"
#include "wind.h"
#include "weather.h"

#include "meshfield.h"
#include "Police.h"
#include "Car.h"
#include "tiremark.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MOVE_FORWARD		(0.18f)		// �v���C���[�O�i���̈ړ���
#define MOVE_BACKWARD		(0.3f)		// �v���C���[��ގ��̈ړ���
#define MOVE_PLUS_FORWARD	(0.36f)		// �v���C���[�O�i���̒ǉ��̈ړ���
#define MOVE_PLUS_BACKWARD	(0.6f)		// �v���C���[��ގ��̒ǉ��̈ړ���
#define MOVE_ROT			(0.012f)	// �v���C���[�̌����ύX��
#define REV_MOVE_ROT		(0.08f)		// �ړ��ʂɂ������ύX�ʂ̕␳�W��
#define REV_MOVE_BRAKE		(0.1f)		// �u���[�L���̌����W��
#define DEL_MOVE_ABS		(1.9f)		// �ړ��ʂ̍폜�͈͂̐�Βl
#define PLAY_GRAVITY		(0.75f)		// �v���C���[�ɂ�����d��
#define MAX_BACKWARD		(-12.0f)	// ��ގ��̍ō����x
#define REV_MOVE_SUB		(0.08f)		// �ړ��ʂ̌����W��
#define UNRIVALED_CNT		(10)		// ���G���Ƀ`�J�`�J������J�E���g
#define STATE_MOVE			(1.5f)		// ��~�E���񎞂̔���͈�

#define PLAY_CLEAR_MOVE		(4.0f)		// �N���A�������̃v���C���[�̎����ړ���
#define REV_PLAY_CLEAR_MOVE	(0.1f)		// �N���A�������̃v���C���[�̌����W��
#define REV_PLAY_OVER_MOVE	(0.02f)		// �N���A���s���̃v���C���[�̌����W��

//------------------------------------------------------------
//	�j�Ŏ��� (�X�����E�u�[�X�g) �}�N����`
//------------------------------------------------------------
#define BOOST_ADD_MOVE	(0.25f)		// �u�[�X�g�̉�����
#define BOOST_SUB_MOVE	(0.12f)		// �u�[�X�g�̌�����
#define BOOST_UP_CNT	(180)		// �u�[�X�g�̉�����Ԃ̎���
#define BOOST_WAIT_SUB	(5)			// �u�[�X�g�̑ҋ@��Ԃ̌��Z��

#define BOOST_XZ_SUB	(90.0f)		// �u�[�X�g���ˈʒu�� xz���Z��
#define BOOST_Y_ADD		(40.0f)		// �u�[�X�g���ˈʒu�� y���Z��
#define BOOST_SIDE_PULS	(18.0f)		// �u�[�X�g���ˈʒu�̉��ʒu�ύX��
#define BOOST_MIN_MOVE	(1.5f)		// �u�[�X�g���ɕK�v�ȍŒᑬ�x

//------------------------------------------------------------
//	����U�� (�t���C�E�A�E�F�C) �}�N����`
//------------------------------------------------------------
#define FLYAWAY_INTERVAL_CNT	(3)			// ���̏o��Ԋu
#define FLYAWAY_SET_CNT			(10)		// ���̏o���
#define FLYAWAY_SHIFT_WIDTH		(90.0f)		// ���̏o��ʒu�����炷��
#define FLYAWAY_SHIFT_HEIGHT	(50.0f)		// ���̏o��ʒu�����炷����
#define FLYAWAY_OVERHEAT_CNT	(80)		// �����I�[�o�[�q�[�g�����Ƃ��̃N�[���_�E���܂ł̎���
#define FLYAWAY_WAIT_SUB		(5)			// ���̑ҋ@��Ԃ̌��Z��

//------------------------------------------------------------
//	�������E (�T�C�����X�E���[���h) �}�N����`
//------------------------------------------------------------
#define BOMB_CANCEL_CNT			(18)		// �U���L�����Z�����̓�񉟂��̗P�\�t���[��
#define SUB_BOMB_CNT			(4)			// �g�p�ҋ@���̃Q�[�W�̌��Z��
#define TUTO_HEAL_BOMB_CNT		(80)		// �`���[�g���A�����̃{���̃N�[���^�C���̉񕜑ҋ@�t���[��

//************************************************************
//	�񋓌^��` (PLAYMOVESTATE)
//************************************************************
typedef enum
{
	PLAYMOVESTATE_NONE = 0,			// �������Ȃ����
	PLAYMOVESTATE_ACCEL,			// �O�i���
	PLAYMOVESTATE_BACK,				// ��ޏ��
	PLAYMOVESTATE_ROTATE,			// ������
	PLAYMOVESTATE_BRAKE,			// ��~���
	PLAYMOVESTATE_MAX				// ���̗񋓌^�̑���
}PLAYMOVESTATE;

//************************************************************
//	�\���̒�` (TutorialInfo)
//************************************************************
typedef struct
{
	int  nCounterHeal;				// �{���̉񕜊Ǘ��J�E���^�[
	bool bForward;					// �O�����J�����̏�
	bool bFirst;					// ��l�̃J�����̏�
}TutorialInfo;

//************************************************************
//	�v���g�^�C�v�錾
//************************************************************
void UpdateGameStartPlayer(void);	// �Q�[���X�^�[�g���̃v���C���[�X�V����
void UpdateGameNorPlayer(void);		// �Q�[���ʏ펞�̃v���C���[�X�V����
void UpdateTutorialNorPlayer(void);	// �`���[�g���A���ʏ펞�̃v���C���[�X�V����
void UpdateClearPlayer(void);		// �N���A�������̃v���C���[�X�V����
void UpdateOverPlayer(void);		// �N���A���s���̃v���C���[�X�V����

PLAYMOVESTATE MovePlayer(bool bMove, bool bRotate, bool bBrake);		// �v���C���[�̈ړ��ʂ̍X�V����

void PosPlayer(void);				// �v���C���[�̈ʒu�̍X�V����
void RotPlayer(void);				// �v���C���[�̌����̍X�V����
void RevPlayer(void);				// �v���C���[�̕␳�̍X�V����

void CameraChangePlayer(void);		// �v���C���[�̃J�����̏�ԕω�����
void SlumBoostPlayer(void);			// �v���C���[�̉�������
void FlyAwayPlayer(void);			// �v���C���[�̑�������
void SilenceWorldPlayer(void);		// �v���C���[�̔��e����

void UpdateSlumBoost(void);			// �����̍X�V����
void SetSlumBoost(void);			// �����̐ݒ菈��
void UpdateFlyAway(void);			// �����̍X�V����
void UpdateSilenceWorld(void);		// ���e�̍X�V����

void AbiHealPlayer(void);			// �\�̓Q�[�W�̉񕜏���
void CameraChange(void);			// �J������ς����Ƃ��̏���

//************************************************************
//	�O���[�o���ϐ�
//************************************************************
Player       g_player;		// �v���C���[���
TutorialInfo g_tutoInfo;	// �`���[�g���A�����
bool g_bWindSound;			// �v���C���[�̉��̗L��

//============================================================
//	�v���C���[�̏���������
//============================================================
void InitPlayer(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// �v���C���[�̖{�̏��̏�����
	g_player.pos           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���݂̈ʒu
	g_player.oldPos        = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �O��̈ʒu
	g_player.move          = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	g_player.rot           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	g_player.moveRot       = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����ύX��
	g_player.state         = ACTIONSTATE_NORMAL;				// �v���C���[�̏��
	g_player.nLife         = PLAY_LIFE;							// �̗�
	g_player.nCounterState = 0;									// ��ԊǗ��J�E���^�[
	g_player.nShadowID     = NONE_SHADOW;						// �e�̃C���f�b�N�X
	g_player.bMove         = false;								// �ړ���
	g_player.bJump         = false;								// �W�����v��
	g_player.nCameraState  = PLAYCAMESTATE_NORMAL;				// �J�����̏��
	g_player.bCameraFirst  = false;								// ��l�̃J�����̏�
	g_player.bUnrivaled	   = false;								// ������
	g_player.bUse          = false;								// �g�p��

	// ���f�����̏�����
	g_player.modelData.dwNumMat = 0;				// �}�e���A���̐�
	g_player.modelData.pTexture = NULL;				// �e�N�X�`���ւ̃|�C���^
	g_player.modelData.pMesh    = NULL;				// ���b�V�� (���_���) �ւ̃|�C���^
	g_player.modelData.pBuffMat = NULL;				// �}�e���A���ւ̃|�C���^
	g_player.modelData.dwNumMat = 0;				// �}�e���A���̐�
	g_player.modelData.vtxMin   = INIT_VTX_MIN;		// �ŏ��̒��_���W
	g_player.modelData.vtxMax   = INIT_VTX_MAX;		// �ő�̒��_���W
	g_player.modelData.size     = INIT_SIZE;		// �傫��
	g_player.modelData.fRadius  = 0.0f;				// ���a

	// �u�[�X�g�̏��̏�����
	g_player.boost.plusMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ǉ��ړ���
	g_player.boost.state    = BOOSTSTATE_NONE;					// �������
	g_player.boost.nCounter = 0;								// �����Ǘ��J�E���^�[

	// ���̏��̏�����
	g_player.wind.bUseWind     = false;							// ���̎g�p��
	g_player.wind.nCircleCount = 0;								// �ǂ��ɏo����
	g_player.wind.nCount       = 0;								// �����o���J�E���g
	g_player.wind.rot          = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����o������

	// ���e�̏��̏�����
	g_player.bomb.state           = ATTACKSTATE_NONE;	// �U�����
	g_player.bomb.nCounterState   = BOMB_WAIT_CNT;		// �U���Ǘ��J�E���^�[
	g_player.bomb.nCounterControl = 0;					// ����Ǘ��J�E���^�[
	g_player.bomb.nHeal = 0;							// �Q�[�W�̉񕜗�
	g_player.bomb.bShot           = false;				// ���ˑҋ@��

	// �A�C�R���̏��̏�����
	g_player.icon.nIconID = NONE_ICON;			// �A�C�R���̃C���f�b�N�X
	g_player.icon.state   = ICONSTATE_NONE;		// �A�C�R���̏��

	// �`���[�g���A���̏��̏�����
	g_tutoInfo.nCounterHeal = 0;				// �{���̉񕜊Ǘ��J�E���^�[
	g_tutoInfo.bForward     = false;			// �O�����J�����̏�
	g_tutoInfo.bFirst       = false;			// ��l�̃J�����̏�

	//�v���C���[�̉�
	g_bWindSound = false;		//�����@
}

//============================================================
//	�v���C���[�̏I������
//============================================================
void UninitPlayer(void)
{
	// �g�p���Ă��Ȃ���Ԃɂ���
	g_player.bUse = false;
}

//============================================================
//	�v���C���[�̃Q�[���X�V����
//============================================================
void UpdateGamePlayer(void)
{
	if (g_player.bUse == true)
	{ // �v���C���[���g�p����Ă���ꍇ

		if (*GetGameState() == GAMESTATE_START)
		{ // �Q�[�����X�^�[�g��Ԃ̏ꍇ

			// �Q�[���X�^�[�g���̃v���C���[�X�V����
			UpdateGameStartPlayer();
		}
		else if (*GetGameState() == GAMESTATE_NORMAL)
		{ // �Q�[�����ʏ��Ԃ̏ꍇ

			// �Q�[���ʏ펞�̃v���C���[�X�V
			UpdateGameNorPlayer();
		}
		else if (GetResultState() == RESULTSTATE_CLEAR)
		{ // ���U���g���N���A������Ԃ̏ꍇ

			// �N���A�������̃v���C���[�X�V
			UpdateClearPlayer();
		}
		else if (GetResultState() == RESULTSTATE_TIMEOVER)
		{ // ���U���g���N���A���s��Ԃ̏ꍇ

			// �N���A���s���̃v���C���[�X�V
			UpdateOverPlayer();
		}
	}
}

//============================================================
//	�v���C���[�̃`���[�g���A���X�V����
//============================================================
void UpdateTutorialPlayer(void)
{
	if (g_player.bUse == true)
	{ // �v���C���[���g�p����Ă���ꍇ

		if (GetTutorialState() == TUTORIALSTATE_NORMAL)
		{ // �`���[�g���A�����ʏ��Ԃ̏ꍇ

			// �`���[�g���A���ʏ펞�̃v���C���[�X�V
			UpdateTutorialNorPlayer();
		}
		else if (GetTutorialState() == TUTORIALSTATE_END)
		{ // �`���[�g���A�����I����Ԃ̏ꍇ

			// �N���A�������̃v���C���[�X�V
			UpdateClearPlayer();
		}
		else if (GetTutorialState() == TUTORIALSTATE_SKIP)
		{ // �`���[�g���A�����I����Ԃ̏ꍇ

			// �N���A���s���̃v���C���[�X�V
			UpdateOverPlayer();
		}
	}
}

//============================================================
//	�v���C���[�̕`�揈��
//============================================================
void DrawPlayer(void)
{
	// �ϐ���錾
	D3DXMATRIX   mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL      redMat;					// �}�e���A���f�[�^ (��)
	D3DXMATERIAL	  UnrivaledMat;				// �}�e���A���f�[�^ (���G)

	if (g_player.bUse == true)
	{ // �v���C���[���g�p����Ă���ꍇ

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_player.mtxWorld);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
		D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
		D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_player.modelData.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_player.modelData.dwNumMat; nCntMat++)
		{ // �}�e���A���̐����J��Ԃ�

			switch (g_player.state)
			{ // ��Ԃ��Ƃ̏���
			case ACTIONSTATE_DAMAGE:	// �_���[�W���

				// �\���̗̂v�f���N���A
				ZeroMemory(&redMat, sizeof(D3DXMATERIAL));

				// �g�U���E�����E���Ȕ�����Ԃɂ���
				redMat.MatD3D.Diffuse  = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				redMat.MatD3D.Ambient  = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				redMat.MatD3D.Emissive = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&redMat.MatD3D);			// ��

				// ������Ԃ�OFF�ɂ���
				g_player.bUnrivaled = false;

				// �����𔲂���
				break;

			case ACTIONSTATE_UNRIVALED:	// ���G���

				if (g_player.bUnrivaled == true)
				{ // �����󋵂̏ꍇ

					// �}�e���A����������
					UnrivaledMat = pMat[nCntMat];

					// ���Ȕ����𓧖��ɂ���
					UnrivaledMat.MatD3D.Emissive = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&UnrivaledMat.MatD3D);			// ����
				}
				else
				{ // �����󋵂���Ȃ��ꍇ

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// �ʏ�
				}

				break;					// �����o��

			default:					// ����ȊO�̏��

				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// �ʏ�

				// ������Ԃ�OFF�ɂ���
				g_player.bUnrivaled = false;

				// �����𔲂���
				break;
			}

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_player.modelData.pTexture[nCntMat]);

			// ���f���̕`��
			g_player.modelData.pMesh->DrawSubset(nCntMat);
		}

		// �ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//============================================================
//	�v���C���[�̈ʒu�E�����̐ݒ菈��
//============================================================
void SetPositionPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// ��������
	g_player.pos     = pos;		// ���݂̈ʒu
	g_player.oldPos  = pos;		// �O��̈ʒu
	g_player.rot     = rot;		// ����

	if (g_player.bUse == false)
	{ // �v���C���[���g�p����Ă��Ȃ��ꍇ

		// ���f������ݒ�
		g_player.modelData = GetModelData(MODELTYPE_PLAYER_CAR);

		// �e�̃C���f�b�N�X��ݒ�
		g_player.nShadowID = SetModelShadow(g_player.modelData, &g_player.nShadowID, &g_player.bUse);

		// �A�C�R���̃C���f�b�N�X��ݒ�
		g_player.icon.nIconID = SetIcon(g_player.pos, ICONTYPE_PLAY, &g_player.icon.nIconID, &g_player.bUse, &g_player.icon.state);

		// �e�̈ʒu�ݒ�
		SetPositionShadow(g_player.nShadowID, g_player.pos, g_player.rot, NONE_SCALE);

		// �g�p���Ă����Ԃɂ���
		g_player.bUse = true;
	}
}

//============================================================
//	�v���C���[�̉񕜔���
//============================================================
void HealPlayer(Player *pPlayer, int nHeal)
{
	if (pPlayer->state == ACTIONSTATE_NORMAL)
	{ // �v���C���[���ʏ��Ԃ̏ꍇ

		// �̗̓o�[�̐ݒ�
		SetLife(pPlayer->nLife, nHeal);

		// �����̉񕜕���̗͂ɉ��Z
		pPlayer->nLife += nHeal;

		if (pPlayer->nLife > PLAY_LIFE)
		{ // �̗͂��ő�̗͂�葽���Ȃ����ꍇ

			// �̗͂�␳
			pPlayer->nLife = PLAY_LIFE;
		}

		// �T�E���h�̍Đ�
		//PlaySound(SOUND_LABEL_SE_HEAL);		// SE (��)
	}
}

//============================================================
// �o���A�̉񕜔���
//============================================================
void HealBarrier(Player *pPlayer, int nHeal)
{
	// �����̉񕜕����o���A�ɉ��Z����
	pPlayer->bomb.nHeal += nHeal;

	// ��Ԃ�ݒ肷��
	pPlayer->bomb.state = ATTACKSTATE_HEAL;
}

//============================================================
//	�v���C���[�̃_���[�W����
//============================================================
void HitPlayer(Player *pPlayer, int nDamage)
{
	if (pPlayer->state == ACTIONSTATE_NORMAL)
	{ // �v���C���[���ʏ��Ԃ̏ꍇ

		// �̗̓o�[�̐ݒ�
		SetLife(pPlayer->nLife, -nDamage);

		// �����̃_���[�W����̗͂��猸�Z
		pPlayer->nLife -= nDamage;

		// �R���{�̔{������
		MagnificCombo(COMBO_INTERRUPTION);

		if (pPlayer->nLife > 0)
		{ // �̗͂��c���Ă���ꍇ

			// �_���[�W��Ԃɂ���
			pPlayer->state = ACTIONSTATE_DAMAGE;

			// �_���[�W���ɂ���
			pPlayer->icon.state = ICONSTATE_DAMAGE;

			// �p�[�e�B�N���̐ݒ�
			SetParticle
			( // ����
				pPlayer->pos,						// �ʒu
				D3DXCOLOR(0.8f, 0.0f, 0.3f, 1.0f),	// �F
				PARTICLETYPE_DAMAGE,				// ���
				SPAWN_PARTICLE_DAMAGE,				// �G�t�F�N�g��
				2									// ����
			);

			// �J�E���^�[��ݒ�
			pPlayer->nCounterState = DAMAGE_TIME_PLAY;

			//���ʉ��̍Đ�
			if (GetSoundType(SOUND_TYPE_SE) == true)
			{
				// �T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_DAMAGE_000);			// SE (�_���[�W)
			}
		}
		else
		{ // �̗͂��s�����ꍇ

			// �p�[�e�B�N���̐ݒ�
			SetParticle
			( // ����
				pPlayer->pos,						// �ʒu
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),	// �F
				PARTICLETYPE_EXPLOSION,				// ���
				SPAWN_PARTICLE_EXPLOSION,			// �G�t�F�N�g��
				2									// ����
			);

			// �g�p���Ă��Ȃ���Ԃɂ���
			pPlayer->bUse = false;
		}
	}
}

//============================================================
//	�Q�[���ʏ펞�̃v���C���[�X�V����
//============================================================
void UpdateGameNorPlayer(void)
{
	// �ϐ���錾
	int nTrafficCnt = 0;		// �����ݒ�p
	int nTackleCnt = 0;			// �����ݒ�p
	float fTackleSpeed = 0.0f;	// �����ݒ�p
	POLICESTATE policeState = POLICESTATE_CHASE;	// �x�@�̏��(�I�u�W�F�N�g�Ƃ̓����蔻��Ɏg�����ߖ��Ӗ�)

	// �O��ʒu�̍X�V
	g_player.oldPos = g_player.pos;

	// ��Ԃ̍X�V
	if (g_player.nCounterState > 0)
	{ // �J�E���^�[�� 0���傫���ꍇ

		// �J�E���^�[�����Z
		g_player.nCounterState--;

		if (g_player.nCounterState == UNR_TIME_PLAY)
		{ // �J�E���^�[�����l�̏ꍇ

			// ���G��Ԃɂ���
			g_player.state = ACTIONSTATE_UNRIVALED;

			// ���G��Ԃ̏�Ԃɂ���
			g_player.icon.state = ICONSTATE_UNRIVALED;
		}
		else if (g_player.nCounterState <= 0)
		{ // �J�E���^�[�� 0�ȉ��̏ꍇ

			// �ʏ��Ԃɂ���
			g_player.state = ACTIONSTATE_NORMAL;

			// �����̏�Ԃɂ���
			g_player.icon.state = ICONSTATE_NONE;
		}

		if (g_player.state == ACTIONSTATE_UNRIVALED)
		{ // �v���C���[�̏�Ԃ����G�̏ꍇ

			if (g_player.nCounterState % UNRIVALED_CNT == 0)
			{ // ��莞�Ԍo�ߎ�

				// �����󋵂̓���ւ�
				g_player.bUnrivaled = g_player.bUnrivaled ? false : true;
			}
		}
	}

	// �����̍X�V
	UpdateSlumBoost();

	// �v���C���[�̈ړ��ʂ̍X�V
	MovePlayer(true, true, true);

	// �v���C���[�̈ʒu�̍X�V
	PosPlayer();

	// �v���C���[�̌����̍X�V
	RotPlayer();

	// �v���C���[�̒��n�̍X�V
	LandObject(&g_player.pos, &g_player.move, &g_player.bJump);

	// �v���C���[�̉���
	SlumBoostPlayer();

	// �v���C���[�̑���
	FlyAwayPlayer();

	// �v���C���[�̔��e
	SilenceWorldPlayer();

	// �����̍X�V
	UpdateFlyAway();

	// ���e�̍X�V
	UpdateSilenceWorld();

	// �v���C���[�̃J�����̏�ԕω�
	CameraChangePlayer();

	//--------------------------------------------------------
	//	�����蔻��
	//--------------------------------------------------------
	// �I�u�W�F�N�g�Ƃ̓����蔻��
	CollisionObject
	( // ����
		&g_player.pos,			// ���݂̈ʒu
		&g_player.oldPos,		// �O��̈ʒu
		&g_player.move,			// �ړ���
		PLAY_WIDTH,				// ����
		PLAY_DEPTH,				// ���s
		&nTrafficCnt,			// �a�؃J�E���g
		g_player.boost.state,	// �u�[�X�g�̏��
		&policeState,			// �x�@�̏��
		&nTackleCnt,			// �^�b�N���J�E���g
		&fTackleSpeed			// �^�b�N�����̈ړ���
	);

	// ������ԃI�u�W�F�N�g�Ƃ̓����蔻��
	SmashCollision
	( // ����
		g_player.pos, 
		g_player.modelData.fRadius,
		g_player.move.x
	);				

	// �Q�[�g�Ƃ̓����蔻��
	CollisionGate
	( // ����
		&g_player.pos,		// ���݂̈ʒu
		&g_player.oldPos,	// �O��̈ʒu
		&g_player.move,		// �ړ���
		PLAY_WIDTH,			// ����
		PLAY_DEPTH			// ���s
	);	

	// �Ԃ̒�~����
	CollisionStopCar
	( // ����
		g_player.pos,				//�ʒu
		g_player.rot,				//����
		&g_player.move,				//�ړ���
		g_player.modelData.fRadius,	//���a
		COLLOBJECTTYPE_PLAYER,		//�Ώۂ̃^�C�v
		&nTrafficCnt
	);

	// �ԓ��m�̓����蔻��
	CollisionCarBody
	( // ����
		&g_player.pos,
		&g_player.oldPos,
		g_player.rot,
		&g_player.move,
		PLAY_WIDTH,
		PLAY_DEPTH,
		COLLOBJECTTYPE_PLAYER,
		&nTrafficCnt,
		TACKLESTATE_CHARGE
	);

	//--------------------------------------------------------
	//	�e�̍X�V
	//--------------------------------------------------------
	// �e�̈ʒu�ݒ�
	SetPositionShadow
	( // ����
		g_player.nShadowID,	// �e�̃C���f�b�N�X
		g_player.pos,		// �ʒu
		g_player.rot,		// ����
		NONE_SCALE			// �g�嗦
	);

	//--------------------------------------------------------
	//	�A�C�R���̍X�V
	//--------------------------------------------------------
	// �A�C�R���̈ʒu�ݒ�
	SetPositionIcon
	(
		g_player.icon.nIconID, 
		g_player.pos
	);

	// �v���C���[�̕␳�̍X�V����
	RevPlayer();
}

//============================================================
//	�`���[�g���A���ʏ펞�̃v���C���[�X�V����
//============================================================
void UpdateTutorialNorPlayer(void)
{
	// �ϐ���錾
	int   nTrafficCnt  = 0;		// �����ݒ�p
	int   nTackleCnt   = 0;		// �����ݒ�p
	float fTackleSpeed = 0.0f;	// �����ݒ�p
	POLICESTATE   policeState   = POLICESTATE_CHASE;	// �x�@�̏��(�I�u�W�F�N�g�Ƃ̓����蔻��Ɏg�����ߖ��Ӗ�)
	PLAYMOVESTATE currentPlayer = PLAYMOVESTATE_NONE;	// ���݂̃v���C���[�̓���

	// �ϐ��z���錾
	bool aControl[3];			// ����̐����p

	// ����̐錾��ݒ�
	switch (GetLessonState())
	{ // ���b�X�����Ƃ̏���
	case LESSON_00:	// ���b�X��0 (�ړ�)

		// ����̐�����ݒ�
		aControl[0] = true;		// �ړ�
		aControl[1] = false;	// ����
		aControl[2] = true;		// ��~

		// �����𔲂���
		break;

	case LESSON_01:	// ���b�X��1 (����)

		// ����̐�����ݒ�
		aControl[0] = true;		// �ړ�
		aControl[1] = true;		// ����
		aControl[2] = true;		// ��~

		// �����𔲂���
		break;
	}

	// �O��ʒu�̍X�V
	g_player.oldPos = g_player.pos;

	// �����̍X�V
	UpdateSlumBoost();

	// �v���C���[�̈ړ��ʂ̍X�V
	currentPlayer = MovePlayer(aControl[0], aControl[1], aControl[2]);

	// �v���C���[�̈ʒu�̍X�V
	PosPlayer();

	// �v���C���[�̌����̍X�V
	RotPlayer();

	// �v���C���[�̒��n�̍X�V
	LandObject(&g_player.pos, &g_player.move, &g_player.bJump);

	if (GetLessonState() >= LESSON_02)
	{ // ���b�X��2�ɒ��풆�A�܂��̓N���A���Ă���ꍇ

		// �v���C���[�̉���
		SlumBoostPlayer();
	}

	if (GetLessonState() >= LESSON_03)
	{ // ���b�X��3�ɒ��풆�A�܂��̓N���A���Ă���ꍇ

		// �v���C���[�̑���
		FlyAwayPlayer();
	}

	if (GetLessonState() >= LESSON_04)
	{ // ���b�X��4�ɒ��풆�A�܂��̓N���A���Ă���ꍇ

		// �v���C���[�̔��e
		SilenceWorldPlayer();
	}

	// �����̍X�V
	UpdateFlyAway();

	// ���e�̍X�V
	UpdateSilenceWorld();

	// �v���C���[�̃J�����̏�ԕω�
	CameraChangePlayer();

	// �\�̓Q�[�W�̉�
	AbiHealPlayer();

	//--------------------------------------------------------
	//	�����蔻��
	//--------------------------------------------------------
	// �I�u�W�F�N�g�Ƃ̓����蔻��
	CollisionObject
	( // ����
		&g_player.pos,			// ���݂̈ʒu
		&g_player.oldPos,		// �O��̈ʒu
		&g_player.move,			// �ړ���
		PLAY_WIDTH,				// ����
		PLAY_DEPTH,				// ���s
		&nTrafficCnt,			// �a�؃J�E���g
		g_player.boost.state,	// �u�[�X�g�̏��
		&policeState,			// �x�@�̏��
		&nTackleCnt,			// �^�b�N���J�E���g
		&fTackleSpeed			// �^�b�N�����̈ړ���
	);

	// ������ԃI�u�W�F�N�g�Ƃ̓����蔻��
	SmashCollision
	( // ����
		g_player.pos, 
		g_player.modelData.fRadius,
		g_player.move.x
	);				

	// �Q�[�g�Ƃ̓����蔻��
	CollisionGate
	( // ����
		&g_player.pos,		// ���݂̈ʒu
		&g_player.oldPos,	// �O��̈ʒu
		&g_player.move,		// �ړ���
		PLAY_WIDTH,			// ����
		PLAY_DEPTH			// ���s
	);	

	// �Ԃ̒�~����
	CollisionStopCar
	( // ����
		g_player.pos,				//�ʒu
		g_player.rot,				//����
		&g_player.move,				//�ړ���
		g_player.modelData.fRadius,	//���a
		COLLOBJECTTYPE_PLAYER,		//�Ώۂ̃^�C�v
		&nTrafficCnt
	);

	// �ԓ��m�̓����蔻��
	CollisionCarBody
	( // ����
		&g_player.pos,
		&g_player.oldPos,
		g_player.rot,
		&g_player.move,
		PLAY_WIDTH,
		PLAY_DEPTH,
		COLLOBJECTTYPE_PLAYER,
		&nTrafficCnt,
		TACKLESTATE_CHARGE
	);

	//--------------------------------------------------------
	//	�e�̍X�V
	//--------------------------------------------------------
	// �e�̈ʒu�ݒ�
	SetPositionShadow
	( // ����
		g_player.nShadowID,	// �e�̃C���f�b�N�X
		g_player.pos,		// �ʒu
		g_player.rot,		// ����
		NONE_SCALE			// �g�嗦
	);

	// �v���C���[�̕␳�̍X�V����
	RevPlayer();

	//--------------------------------------------------------
	//	�`���[�g���A���̍X�V
	//--------------------------------------------------------
	switch (GetLessonState())
	{ // ���b�X�����Ƃ̏���
	case LESSON_00:	// ���b�X��0 (�ړ�)

		if (currentPlayer == PLAYMOVESTATE_ACCEL
		||  currentPlayer == PLAYMOVESTATE_BACK)
		{ // ���݂̃v���C���[�̓������ړ���Ԃ̏ꍇ

			// ���b�X���̏�Ԃ̉��Z
			AddLessonState();
		}

		// �����𔲂���
		break;

	case LESSON_01:	// ���b�X��1 (����)

		if (currentPlayer == PLAYMOVESTATE_ROTATE)
		{ // ���݂̃v���C���[�̓����������Ԃ̏ꍇ

			// ���b�X���̏�Ԃ̉��Z
			AddLessonState();
		}

		// �����𔲂���
		break;
	}
}

//============================================================
//	�N���A�������̃v���C���[�X�V����
//============================================================
void UpdateClearPlayer(void)
{
	// �J�����̏�Ԃ�ʏ��Ԃɐݒ�
	g_player.bCameraFirst = false;
	g_player.nCameraState = PLAYCAMESTATE_NORMAL;

	// �p�x���Q�[�g�̌����ɐݒ�
	g_player.rot.y = GetExit().pGate->rot.y;

	// �ړ��ʂ�����
	g_player.move.x += (PLAY_CLEAR_MOVE - g_player.move.x) * REV_PLAY_CLEAR_MOVE;

	// �ǉ��ړ��ʂ�����
	g_player.boost.plusMove.x += (0.0f - g_player.boost.plusMove.x) * REV_PLAY_CLEAR_MOVE;

	// �O��ʒu�̍X�V
	g_player.oldPos = g_player.pos;

	// �v���C���[�̈ʒu�̍X�V
	PosPlayer();

	// �v���C���[�̌����̍X�V
	RotPlayer();

	// �v���C���[�̒��n�̍X�V
	LandObject(&g_player.pos, &g_player.move, &g_player.bJump);

	//--------------------------------------------------------
	//	�e�̍X�V
	//--------------------------------------------------------
	// �e�̈ʒu�ݒ�
	SetPositionShadow
	( // ����
		g_player.nShadowID,	// �e�̃C���f�b�N�X
		g_player.pos,		// �ʒu
		g_player.rot,		// ����
		NONE_SCALE			// �g�嗦
	);

	//--------------------------------------------------------
	//	�A�C�R���̍X�V
	//--------------------------------------------------------
	// �A�C�R���̈ʒu�ݒ�
	SetPositionIcon
	(
		g_player.icon.nIconID,
		g_player.pos
	);
}

//============================================================
//	�N���A���s���̃v���C���[�X�V����
//============================================================
void UpdateOverPlayer(void)
{
	int nTackleCnt = 0;			// �����ݒ�p
	float fTackleSpeed = 0.0f;	// �����ݒ�p

	// �ϐ���錾
	int nTrafficCnt = 0;	// �����ݒ�p

	POLICESTATE policeState = POLICESTATE_CHASE;	// �x�@�̏��(�I�u�W�F�N�g�Ƃ̓����蔻��Ɏg�����ߖ��Ӗ�)

	// �J�����̏�Ԃ�ʏ��Ԃɐݒ�
	g_player.bCameraFirst = false;
	g_player.nCameraState = PLAYCAMESTATE_NORMAL;

	// �ړ����Ă��Ȃ���Ԃɂ���
	g_player.bMove = false;

	// �ړ��ʂ�����
	g_player.move.x += (0.0f - g_player.move.x) * REV_PLAY_OVER_MOVE;

	// �ǉ��ړ��ʂ�����
	g_player.boost.plusMove.x += (0.0f - g_player.boost.plusMove.x) * REV_PLAY_OVER_MOVE;

	// �O��ʒu�̍X�V
	g_player.oldPos = g_player.pos;

	// �v���C���[�̈ʒu�̍X�V
	PosPlayer();

	// �v���C���[�̌����̍X�V
	RotPlayer();

	// �v���C���[�̒��n�̍X�V
	LandObject(&g_player.pos, &g_player.move, &g_player.bJump);

	//--------------------------------------------------------
	//	�����蔻��
	//--------------------------------------------------------
	// �I�u�W�F�N�g�Ƃ̓����蔻��
	CollisionObject
	( // ����
		&g_player.pos,			// ���݂̈ʒu
		&g_player.oldPos,		// �O��̈ʒu
		&g_player.move,			// �ړ���
		PLAY_WIDTH,				// ����
		PLAY_DEPTH,				// ���s
		&nTrafficCnt,			// �a�؃J�E���g
		g_player.boost.state,	// �u�[�X�g�̏��
		&policeState,			// �x�@�̏��
		&nTackleCnt,			// �^�b�N���J�E���g
		&fTackleSpeed			// �^�b�N�����̈ړ���
	);

	// ������ԃI�u�W�F�N�g�Ƃ̓����蔻��
	SmashCollision
	( // ����
		g_player.pos, 
		g_player.modelData.fRadius,
		g_player.move.x
	);				

	// �Q�[�g�Ƃ̓����蔻��
	CollisionGate
	( // ����
		&g_player.pos,		// ���݂̈ʒu
		&g_player.oldPos,	// �O��̈ʒu
		&g_player.move,		// �ړ���
		PLAY_WIDTH,			// ����
		PLAY_DEPTH			// ���s
	);	

	// �Ԃ̒�~����
	CollisionStopCar
	( // ����
		g_player.pos,				//�ʒu
		g_player.rot,				//����
		&g_player.move,				//�ړ���
		g_player.modelData.fRadius,	//���a
		COLLOBJECTTYPE_PLAYER,		//�Ώۂ̃^�C�v
		&nTrafficCnt
	);

	// �ԓ��m�̓����蔻��
	CollisionCarBody
	( // ����
		&g_player.pos,
		&g_player.oldPos,
		g_player.rot,
		&g_player.move,
		PLAY_WIDTH,
		PLAY_DEPTH,
		COLLOBJECTTYPE_PLAYER,
		&nTrafficCnt,
		TACKLESTATE_CHARGE
	);

	//--------------------------------------------------------
	//	�e�̍X�V
	//--------------------------------------------------------
	// �e�̈ʒu�ݒ�
	SetPositionShadow
	( // ����
		g_player.nShadowID,	// �e�̃C���f�b�N�X
		g_player.pos,		// �ʒu
		g_player.rot,		// ����
		NONE_SCALE			// �g�嗦
	);

	// �v���C���[�̕␳�̍X�V����
	RevPlayer();

	//--------------------------------------------------------
	//	�A�C�R���̍X�V
	//--------------------------------------------------------
	// �A�C�R���̈ʒu�ݒ�
	SetPositionIcon
	(
		g_player.icon.nIconID,
		g_player.pos
	);
}

//============================================================
//	�v���C���[�̈ړ��ʂ̍X�V����
//============================================================
PLAYMOVESTATE MovePlayer(bool bMove, bool bRotate, bool bBrake)
{
	// �ϐ���錾
	PLAYMOVESTATE currentPlayer = PLAYMOVESTATE_NONE;	// ���݂̃v���C���[�̓���

	if (bMove)
	{ // �ړ��̑��삪�\�ȏꍇ

		if (GetKeyboardPress(DIK_W) == true || GetJoyKeyR2Press(0) == true)
		{ // �O�i�̑��삪�s��ꂽ�ꍇ

			// �O�i��Ԃɂ���
			currentPlayer = PLAYMOVESTATE_ACCEL;

			// �ړ��ʂ��X�V
			g_player.move.x += MOVE_FORWARD;

			if (g_player.move.x < 0)
			{ // �ړ��ʂ� 0��菬�����ꍇ

				// �ړ��ʂ��X�V
				g_player.move.x += MOVE_PLUS_FORWARD;
			}

			// �ړ����Ă����Ԃɂ���
			g_player.bMove = true;
		}
		else if (GetKeyboardPress(DIK_S) == true || GetJoyKeyL2Press(0) == true)
		{ // ��ނ̑��삪�s��ꂽ�ꍇ

			// ��ޏ�Ԃɂ���
			currentPlayer = PLAYMOVESTATE_BACK;

			// �ړ��ʂ��X�V
			g_player.move.x -= MOVE_BACKWARD;

			if (g_player.move.x > 0)
			{ // �ړ��ʂ� 0���傫���ꍇ

				// �ړ��ʂ��X�V
				g_player.move.x -= MOVE_PLUS_BACKWARD;
			}

			// �ړ����Ă����Ԃɂ���
			g_player.bMove = true;
		}
		else
		{ // �ړ����Ă��Ȃ��ꍇ

			// �ړ����Ă��Ȃ���Ԃɂ���
			g_player.bMove = false;
		}
	}

	if (bRotate)
	{ // ����̑��삪�\�ȏꍇ

		if (GetKeyboardPress(DIK_A) == true || GetJoyStickPressLX(0) < 0)
		{ // �������̑��삪�s��ꂽ�ꍇ

			// �������X�V
			g_player.moveRot.y -= MOVE_ROT * ((g_player.move.x + g_player.boost.plusMove.x) * REV_MOVE_ROT);

			if (g_player.move.x <= -STATE_MOVE
			||  g_player.move.x >=  STATE_MOVE)
			{ // �ړ��ʂ����l�͈̔͊O�̏ꍇ

				// �����Ԃɂ���
				currentPlayer = PLAYMOVESTATE_ROTATE;
			}
		}
		else if (GetKeyboardPress(DIK_D) == true || GetJoyStickPressLX(0) > 0)
		{ // �E�����̑��삪�s��ꂽ�ꍇ

			// �������X�V
			g_player.moveRot.y += MOVE_ROT * ((g_player.move.x + g_player.boost.plusMove.x) * REV_MOVE_ROT);

			if (g_player.move.x <= -STATE_MOVE
			||  g_player.move.x >=  STATE_MOVE)
			{ // �ړ��ʂ����l�͈̔͊O�̏ꍇ

				// �����Ԃɂ���
				currentPlayer = PLAYMOVESTATE_ROTATE;
			}
		}
	}

	if (GetKeyboardPress(DIK_LCONTROL) == true || GetJoyKeyPress(JOYKEY_Y, 0) == true)
	{ // �u���[�L�̑��삪�s��ꂽ�ꍇ

		if (bBrake)
		{ // ��~�̑��삪�\�ȏꍇ

			// �ړ��ʂ�����
			g_player.move.x += (0.0f - g_player.move.x) * REV_MOVE_BRAKE;

			if (g_player.move.x <= -STATE_MOVE
			||  g_player.move.x >=  STATE_MOVE)
			{ // �ړ��ʂ����l�͈̔͊O�̏ꍇ

				// ��~��Ԃɂ���
				currentPlayer = PLAYMOVESTATE_BRAKE;
			}

			// �ړ��ʂ̕␳
			if (g_player.move.x <= DEL_MOVE_ABS
			&&  g_player.move.x >= -DEL_MOVE_ABS)
			{ // �ړ��ʂ��폜�͈͓̔��̏ꍇ

				// �ړ��ʂ��폜
				g_player.move.x = 0.0f;
			}
		}
	}

	// ���݂̃v���C���[�̓�����Ԃ�
	return currentPlayer;
}

//============================================================
//	�v���C���[�̈ʒu�̍X�V����
//============================================================
void PosPlayer(void)
{
	//--------------------------------------------------------
	//	�d�͂̍X�V
	//--------------------------------------------------------
	g_player.move.y -= PLAY_GRAVITY;

	//--------------------------------------------------------
	//	�ړ��ʂ̕␳
	//--------------------------------------------------------
	if (g_player.move.x > MAX_FORWARD)
	{ // �v���C���[�̈ړ��� (x) �����l�ȏ�̏ꍇ

		// �v���C���[�̈ړ��� (x) ��␳
		g_player.move.x = MAX_FORWARD;
	}
	else if (g_player.move.x < MAX_BACKWARD)
	{ // �v���C���[�̈ړ��� (x) �����l�ȉ��̏ꍇ

		// �v���C���[�̈ړ��� (x) ��␳
		g_player.move.x = MAX_BACKWARD;
	}

	//--------------------------------------------------------
	//	�ʒu�̍X�V
	//--------------------------------------------------------
	g_player.pos.x += sinf(g_player.rot.y) * (g_player.move.x + g_player.boost.plusMove.x);
	g_player.pos.y += g_player.move.y;
	g_player.pos.z += cosf(g_player.rot.y) * (g_player.move.x + g_player.boost.plusMove.x);

	//--------------------------------------------------------
	//	�ړ��ʂ̌���
	//--------------------------------------------------------
	if (g_player.bMove == false)
	{ // �ړ����Ă��Ȃ���Ԃ̏ꍇ

		// �ړ��ʂ�����
		g_player.move.x += (0.0f - g_player.move.x) * REV_MOVE_SUB;
	}
}

//============================================================
//	�v���C���[�̌����̍X�V����
//============================================================
void RotPlayer(void)
{
	// �ϐ���錾
	float fRevRot = (((fabsf(GetPlayer()->move.x + GetPlayer()->boost.plusMove.x) - MAX_REAL_SPEED) * -1.0f) * ((1.0f - PLAY_REV_ROT_MIN) / MAX_REAL_SPEED)) + PLAY_REV_ROT_MIN;	// �����ύX�ʂ̌����W��

	//--------------------------------------------------------
	//	�����̍X�V
	//--------------------------------------------------------
	g_player.rot.y += g_player.moveRot.y;

	//--------------------------------------------------------
	//	�����̐��K��
	//--------------------------------------------------------
	// �����̐��K��
	RotNormalize(&g_player.rot.y);

	//--------------------------------------------------------
	//	�����ύX�ʂ̌���
	//--------------------------------------------------------
	// �����ύX�ʂ�����
	g_player.moveRot.y += (0.0f - g_player.moveRot.y) * fRevRot;
}

//============================================================
//	�v���C���[�̕␳�̍X�V����
//============================================================
void RevPlayer(void)
{
	// �����̐��K��
	RotNormalize(&g_player.rot.y);

	//--------------------------------------------------------
	//	�ړ��͈͂̕␳
	//--------------------------------------------------------
	if (g_player.pos.z > GetLimitStage().fNear - (PLAY_DEPTH * 2))
	{ // �͈͊O�̏ꍇ (��O)

		// ��O�Ɉʒu��␳
		g_player.pos.z = GetLimitStage().fNear - (PLAY_DEPTH * 2);

		// �ړ��ʂ��폜
		CollisionSlow(&g_player.move.x);
	}
	if (g_player.pos.z < GetLimitStage().fFar + (PLAY_DEPTH * 2))
	{ // �͈͊O�̏ꍇ (��)

		// ���Ɉʒu��␳
		g_player.pos.z = GetLimitStage().fFar + (PLAY_DEPTH * 2);

		// �ړ��ʂ��폜
		CollisionSlow(&g_player.move.x);
	}
	if (g_player.pos.x > GetLimitStage().fRight - (PLAY_WIDTH * 2))
	{ // �͈͊O�̏ꍇ (�E)

		// �E�Ɉʒu��␳
		g_player.pos.x = GetLimitStage().fRight - (PLAY_WIDTH * 2);

		// �ړ��ʂ��폜
		CollisionSlow(&g_player.move.x);
	}
	if (g_player.pos.x < GetLimitStage().fLeft + (PLAY_WIDTH * 2))
	{ // �͈͊O�̏ꍇ (��)

		// ���Ɉʒu��␳
		g_player.pos.x = GetLimitStage().fLeft + (PLAY_WIDTH * 2);

		// �ړ��ʂ��폜
		CollisionSlow(&g_player.move.x);
	}
}

//============================================================
//	�v���C���[�̎擾����
//============================================================
Player *GetPlayer(void)
{
	// �v���C���[�{�̂̏��A�h���X��Ԃ�
	return &g_player;
}

//============================================================
//	�v���C���[�̉�������
//============================================================
void SlumBoostPlayer(void)
{
	if (GetKeyboardPress(DIK_LSHIFT) == true || GetJoyKeyPress(JOYKEY_B, 0) == true)
	{ // �����̑��삪�s���Ă���ꍇ

		// �����̐ݒ�
		SetSlumBoost();
	}
}

//============================================================
//	�v���C���[�̑�������
//============================================================
void FlyAwayPlayer(void)
{
	if (GetKeyboardPress(DIK_U) == true || GetJoyKeyPress(JOYKEY_A, 0) == true)
	{ // �����@�̑��삪�s���Ă���ꍇ

		if (GetWindInfo()->state == WIND_USABLE)
		{ // �����g�p�\�������ꍇ

			// �����@���g�p����
			g_player.wind.bUseWind = true;

			//���ʉ��nBGM�̍Đ�
			if (GetSoundType(SOUND_TYPE_SUB_BGM) == true)
			{
				//�T�E���h�̐ݒ�
				if (g_bWindSound == false)
				{//�����@�̃T�E���h������Ă��Ȃ��Ƃ�

					//�����@�̃T�E���h�̃I���ɐݒ�
					g_bWindSound = true;

					//�����@�̃T�E���h�iBGM�j�̍Đ�
					PlaySound(SOUND_LABEL_BGM_ABILITY_WIND_000);
				}
			}
		}
		else
		{//�����g�p�s�̂Ƃ�

			//���ʉ��nBGM�̒�~
			if (GetSoundType(SOUND_TYPE_SUB_BGM) == true)
			{
				//�T�E���h�̐ݒ�
				if (g_bWindSound == true)
				{//�����@�̃T�E���h������Ă���Ƃ�

				 //�����@�̃T�E���h�iBGM�j�̒�~
					StopSound(SOUND_LABEL_BGM_ABILITY_WIND_000);

					//�����@�̃T�E���h�̃I�t�ɐݒ�
					g_bWindSound = false;
				}
			}
		}
	}
	else
	{ // �����@�̑��삪�s���Ă��Ȃ��ꍇ
	
		// �����@���g�p���Ȃ�
		g_player.wind.bUseWind = false;

		//���ʉ��nBGM�̒�~
		if (GetSoundType(SOUND_TYPE_SUB_BGM) == true)
		{
			//�T�E���h�̐ݒ�
			if (g_bWindSound == true)
			{//�����@�̃T�E���h������Ă���Ƃ�

				//�����@�̃T�E���h�iBGM�j�̒�~
				StopSound(SOUND_LABEL_BGM_ABILITY_WIND_000);

				//�����@�̃T�E���h�̃I�t�ɐݒ�
				g_bWindSound = false;


			}
		}
	}
}

//============================================================
//	�v���C���[�̔��e����
//============================================================
void SilenceWorldPlayer(void)
{
	if (GetKeyboardTrigger(DIK_SPACE) == true || GetJoyKeyTrigger(JOYKEY_X, 0))
	{ // �U�����[�h�̕ύX�̑��삪�s��ꂽ�ꍇ

		if (g_player.bomb.state == ATTACKSTATE_NONE)
		{ // �������Ȃ���Ԃ̏ꍇ

			// �o���A�̔���
			ShotBarrier();
		}
	}
}

//============================================================
//	�v���C���[�̃J�����̏�ԕω�����
//============================================================
void CameraChangePlayer(void)
{
	if (GetKeyboardTrigger(DIK_J) == true || GetJoyKeyTrigger(JOYKEY_UP, 0) == true)
	{ // �J���������̕ύX���삪�s��ꂽ�ꍇ

		// �J�����̏�Ԃ��o�b�N�J�����ɂ���
		g_player.nCameraState = (g_player.nCameraState + 1) % PLAYCAMESTATE_MAX;

		// �O�����J������ύX���������c��
		g_tutoInfo.bForward = true;

		// �J������ς����Ƃ��̏���
		CameraChange();
	}

	if (GetKeyboardTrigger(DIK_K) == true || GetJoyKeyTrigger(JOYKEY_DOWN, 0) == true)
	{ // ��l�̃J������ ON / OFF �̕ύX���삪�s��ꂽ�ꍇ

		// ��l�̃J�����̏󋵂�true�ɂ���
		g_player.bCameraFirst = g_player.bCameraFirst ? false : true;

		// ��l�̃J������ύX���������c��
		g_tutoInfo.bFirst = true;

		// �J������ς����Ƃ��̏���
		CameraChange();
	}
}

//============================================================
//	�����̍X�V����
//============================================================
void UpdateSlumBoost(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posLeft, posRight;	// �u�[�X�g�̕��o�ʒu

	switch (g_player.boost.state)
	{ // ������Ԃ��Ƃ̏���
	case BOOSTSTATE_NONE:	// �������Ȃ����

		// ����

		// �����𔲂���
		break;

	case BOOSTSTATE_UP:		// �������

		// �ǉ����x�����Z
		g_player.boost.plusMove.x += BOOST_ADD_MOVE;

		if (g_player.boost.plusMove.x > MAX_BOOST)
		{ // �����x�����l�ȏ�̏ꍇ

			// �����x�̕␳
			g_player.boost.plusMove.x = MAX_BOOST;
		}

		if (g_player.boost.nCounter > 0)
		{ // �J�E���^�[�� 0���傫���ꍇ

			// �J�E���^�[�����Z
			g_player.boost.nCounter--;

			// �^�C�������o��
			SetTireMark(D3DXVECTOR3(g_player.pos.x + sinf(g_player.rot.y + D3DX_PI * 0.5f) * 55.0f, g_player.pos.y + 0.01f,
				g_player.pos.z + cosf(g_player.rot.y + D3DX_PI * 0.5f) * 55.0f), g_player.rot);

			// �^�C�������o��
			SetTireMark(D3DXVECTOR3(g_player.pos.x - sinf(g_player.rot.y + D3DX_PI * 0.5f) * 55.0f, g_player.pos.y + 0.01f,
				g_player.pos.z - cosf(g_player.rot.y + D3DX_PI * 0.5f) * 55.0f), g_player.rot);

			if (GetWeather() == WEATHERTYPE_RAIN || GetWeather() == WEATHERTYPE_THUNDER)
			{ // �J�������͗��J�̏ꍇ

				// �J�̐����Ԃ��̃G�t�F�N�g
				SetParticle
				(
					D3DXVECTOR3(g_player.pos.x + sinf(g_player.rot.y + D3DX_PI * 0.5f) * 55.0f, g_player.pos.y, g_player.pos.z + cosf(g_player.rot.y + D3DX_PI * 0.5f) * 55.0f),
					D3DXCOLOR(0.3f, 0.3f, 1.0f, 1.0f),
					PARTICLETYPE_RAINSPRAY,
					SPAWN_PARTICLE_RAINSPRAY,
					2
				);

				// �J�̐����Ԃ��̃G�t�F�N�g
				SetParticle
				(
					D3DXVECTOR3(g_player.pos.x - sinf(g_player.rot.y + D3DX_PI * 0.5f) * 55.0f, g_player.pos.y, g_player.pos.z - cosf(g_player.rot.y + D3DX_PI * 0.5f) * 55.0f),
					D3DXCOLOR(0.3f, 0.3f, 1.0f, 1.0f),
					PARTICLETYPE_RAINSPRAY,
					SPAWN_PARTICLE_RAINSPRAY,
					2
				);
			}
		}
		else
		{ // �J�E���^�[�� 0�ȉ��̏ꍇ

			// ������Ԃɂ���
			g_player.boost.state = BOOSTSTATE_DOWN;
		}

		// ���u�[�X�g�̕��o�ʒu�����߂�
		posLeft.x = g_player.pos.x - sinf(g_player.rot.y) * BOOST_XZ_SUB + sinf(g_player.rot.y - (D3DX_PI * 0.5f)) * BOOST_SIDE_PULS;
		posLeft.y = g_player.pos.y + BOOST_Y_ADD;
		posLeft.z = g_player.pos.z - cosf(g_player.rot.y) * BOOST_XZ_SUB + cosf(g_player.rot.y - (D3DX_PI * 0.5f)) * BOOST_SIDE_PULS;

		// �E�u�[�X�g�̕��o�ʒu�����߂�
		posRight.x = g_player.pos.x - sinf(g_player.rot.y) * BOOST_XZ_SUB + sinf(g_player.rot.y + (D3DX_PI * 0.5f)) * BOOST_SIDE_PULS;
		posRight.y = g_player.pos.y + BOOST_Y_ADD;
		posRight.z = g_player.pos.z - cosf(g_player.rot.y) * BOOST_XZ_SUB + cosf(g_player.rot.y + (D3DX_PI * 0.5f)) * BOOST_SIDE_PULS;

		// �p�[�e�B�N���̐ݒ�
		SetParticle
		( // ����
			posLeft,							// �ʒu
			D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f),	// �F
			PARTICLETYPE_BOOST,					// ���
			3,									// �G�t�F�N�g��
			2									// ����
		);

		// �p�[�e�B�N���̐ݒ�
		SetParticle
		( // ����
			posRight,							// �ʒu
			D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f),	// �F
			PARTICLETYPE_BOOST,					// ���
			3,									// �G�t�F�N�g��
			2									// ����
		);

		// �����𔲂���
		break;

	case BOOSTSTATE_DOWN:	// �������

		// �ǉ����x������
		g_player.boost.plusMove.x -= BOOST_SUB_MOVE;

		if (g_player.boost.plusMove.x <= 0.0f)
		{ // �ړ��ʂ��Ȃ��Ȃ�؂����ꍇ

			// �ړ��ʂ�␳
			g_player.boost.plusMove.x = 0.0f;

			// �J�E���^�[��ݒ�
			g_player.boost.nCounter = BOOST_WAIT_CNT;

			// �g�p�\�̑ҋ@��Ԃɂ���
			g_player.boost.state = BOOSTSTATE_WAIT;
		}

		// �����𔲂���
		break;

	case BOOSTSTATE_WAIT:	// �g�p�\�̑ҋ@���

		if (g_player.boost.nCounter > 0)
		{ // �J�E���^�[�� 0���傫���ꍇ

			// �J�E���^�[�����Z
			g_player.boost.nCounter -= BOOST_WAIT_SUB;
		}
		else
		{ // �J�E���^�[�� 0�ȉ��̏ꍇ

			// �J�E���^�[��␳
			g_player.boost.nCounter = 0;

			// �������Ȃ���Ԃɂ���
			g_player.boost.state = BOOSTSTATE_NONE;
		}

		// �����𔲂���
		break;
	}
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void SetSlumBoost(void)
{
	if (g_player.boost.state == BOOSTSTATE_NONE
	&&  g_player.move.x > BOOST_MIN_MOVE)
	{ // �u�[�X�g���s�����Ԃ̏ꍇ

		// ������Ԃɂ���
		g_player.boost.state = BOOSTSTATE_UP;

		// �J�E���^�[��ݒ�
		g_player.boost.nCounter = BOOST_UP_CNT;

		//���ʉ��nBGM���Đ�
		if (GetSoundType(SOUND_TYPE_SUB_BGM) == true)
		{
			//�u�[�X�g�̃T�E���h�iBGM�j�̍Đ�
			PlaySound(SOUND_LABEL_SE_ABILITY_BOOST_000);
		}
	}
}

//============================================================
//	�����̍X�V����
//============================================================
void UpdateFlyAway(void)
{
	// �|�C���^��錾
	WindInfo *pWindInfo = GetWindInfo();

	switch (pWindInfo->state)
	{
	case WIND_USABLE:			// �����g����ꍇ

		if (g_player.wind.bUseWind == true)
		{ // �����@���g�p�����ꍇ

			// �����o���J�E���g�����Z����
			g_player.wind.nCount++;

			// �J�E���^�[�����Z����
			pWindInfo->nUseCounter++;

			if (pWindInfo->nUseCounter >= WIND_OVERHEAT_CNT)
			{ // 3�b�𒴂����ꍇ

				// �����g�p���Ȃ�
				g_player.wind.bUseWind = false;

				// �I�[�o�[�q�[�g��Ԃɂ���
				pWindInfo->state = WIND_OVERHEAT;
			}

			if (g_player.wind.nCount % FLYAWAY_INTERVAL_CNT == 0)
			{ // ���̃J�E���g����萔�ɂȂ�����

				// �ϐ���錾
				float fRotAdd;	// �����̒ǉ���

				for (int nCnt = 0; nCnt < FLYAWAY_SET_CNT; nCnt++)
				{

					{ // �E�̕�

						// �����̒ǉ������Z�o����
						fRotAdd = -(float)((rand() % 240 + 52) - 157) / 100;

						// ���̈ʒu��ݒ肷��
						g_player.wind.pos = D3DXVECTOR3
						(
							g_player.pos.x + sinf(g_player.rot.y + D3DX_PI* 0.5f) * FLYAWAY_SHIFT_WIDTH,		// X���W
							g_player.pos.y + FLYAWAY_SHIFT_HEIGHT,												// Y���W
							g_player.pos.z + cosf(g_player.rot.y + D3DX_PI * 0.5f) * FLYAWAY_SHIFT_WIDTH		// Z���W
						);

						//���̌�����ݒ肷��
						g_player.wind.rot = D3DXVECTOR3(0.0f, g_player.rot.y + D3DX_PI * 0.5f + fRotAdd, 0.0f);

						// ���̐ݒ菈��
						SetWind(g_player.wind.pos, g_player.wind.rot);
					}

					{ // ���̕�

						// �����̒ǉ������Z�o����
						fRotAdd = (float)((rand() % 240 + 52) - 157) / 100;

						// ���̈ʒu��ݒ肷��
						g_player.wind.pos = D3DXVECTOR3
						(
							g_player.pos.x - sinf(g_player.rot.y + D3DX_PI * 0.5f) * FLYAWAY_SHIFT_WIDTH,		// X���W
							g_player.pos.y + FLYAWAY_SHIFT_HEIGHT, 												// Y���W
							g_player.pos.z - cosf(g_player.rot.y + D3DX_PI * 0.5f) * FLYAWAY_SHIFT_WIDTH		// Z���W
						);

						//���̌�����ݒ肷��
						g_player.wind.rot = D3DXVECTOR3(0.0f, g_player.rot.y - D3DX_PI * 0.5f + fRotAdd, 0.0f);

						// ���̐ݒ菈��
						SetWind(g_player.wind.pos, g_player.wind.rot);
					}
				}
			}
		}
		else
		{ // �����@���g�p���Ă��Ȃ��ꍇ

			// �J�E���g������������
			g_player.wind.nCount = 0;

			// �J�E���^�[�����Z����
			pWindInfo->nUseCounter--;

			if (pWindInfo->nUseCounter <= 0)
			{ // �J�E���^�[��0�ȉ��ɂȂ����ꍇ

				// �J�E���^�[��␳����
				pWindInfo->nUseCounter = 0;

				// �g�p�\�ɂ���
				pWindInfo->state = WIND_USABLE;
			}
		}

		break;					// �����o��

	case WIND_OVERHEAT:			// �I�[�o�[�q�[�g���

		// ���̃J�E���g��180�ɌŒ肷��
		pWindInfo->nUseCounter = WIND_OVERHEAT_CNT;

		// �I�[�o�[�q�[�g�J�E���^�[�����Z����
		pWindInfo->nOverHeatCounter++;

		if (pWindInfo->nOverHeatCounter >= FLYAWAY_OVERHEAT_CNT)
		{ // �I�[�o�[�q�[�g�J�E���^�[����萔�ɒB������

			// �I�[�o�[�q�[�g�J�E���^�[������������
			pWindInfo->nOverHeatCounter = 0;

			// �N�[���_�E����Ԃɂ���
			pWindInfo->state = WIND_COOLDOWN;
		}

		// �����g�p���Ȃ�
		g_player.wind.bUseWind = false;

		break;					// �����o��

	case WIND_COOLDOWN:			// �N�[���_�E�����

		// �J�E���g������������
		g_player.wind.nCount = 0;

		// �J�E���^�[�����Z����
		pWindInfo->nUseCounter -= FLYAWAY_WAIT_SUB;

		if (pWindInfo->nUseCounter <= 0)
		{ // �J�E���^�[��0�ȉ��ɂȂ����ꍇ

			// �J�E���^�[��␳����
			pWindInfo->nUseCounter = 0;

			// �g�p�\�ɂ���
			pWindInfo->state = WIND_USABLE;
		}

		break;					// �����o��
	}
}

//============================================================
//	���e�̍X�V����
//============================================================
void UpdateSilenceWorld(void)
{
	switch (g_player.bomb.state)
	{ // �U����Ԃ��Ƃ̏���
	case ATTACKSTATE_NONE:	// �������Ȃ����

		// ����

		// �����𔲂���
		break;

	case ATTACKSTATE_WAIT:	// �U���ҋ@���

		if (g_player.bomb.nCounterState < BOMB_WAIT_CNT)
		{ // �J�E���^�[�����l��菬�����ꍇ

			// �J�E���^�[�����Z
			g_player.bomb.nCounterState++;
		}
		else
		{ // �J�E���^�[�����l�ȏ�̏ꍇ

			// ��ԃJ�E���g��␳����
			g_player.bomb.nCounterState = BOMB_WAIT_CNT;

			// �������Ȃ���Ԃɂ���
			g_player.bomb.state = ATTACKSTATE_NONE;
		}

		// �����𔲂���
		break;

	case ATTACKSTATE_HEAL:	// �Q�[�W�񕜏��

		if (g_player.bomb.nCounterState < BOMB_WAIT_CNT)
		{ // �J�E���^�[�����l��菬�����ꍇ

			// �Q�[�W����
			g_player.bomb.nCounterState += g_player.bomb.nHeal;
		}
		else
		{ // �J�E���^�[�����l�ȏ�̏ꍇ

			// ��ԃJ�E���g��␳����
			g_player.bomb.nCounterState = BOMB_WAIT_CNT;

			// �Q�[�W�񕜗ʂ�����������
			g_player.bomb.nHeal = 0;

			// �������Ȃ���Ԃɂ���
			g_player.bomb.state = ATTACKSTATE_NONE;
		}

		// �����𔲂���
		break;
	}
}

//============================================================
//	�\�̓Q�[�W�̉񕜏���
//============================================================
void AbiHealPlayer(void)
{
	// �|�C���^��錾
	WindInfo *pWindInfo = GetWindInfo();	// ���̏��

	if (g_player.boost.state == BOOSTSTATE_WAIT)
	{ // �X�����u�[�X�g���ҋ@��Ԃ̏ꍇ

		// �J�E���^�[��������
		g_player.boost.nCounter = 0;

		// �������Ȃ���Ԃɂ���
		g_player.boost.state = BOOSTSTATE_NONE;
	}

	if (pWindInfo->state == WIND_COOLDOWN)
	{ // �t���C�A�E�F�C���N�[���_�E����Ԃ̏ꍇ

		// �J�E���^�[��␳����
		pWindInfo->nUseCounter = 0;

		// �g�p�\�ɂ���
		pWindInfo->state = WIND_USABLE;
	}

	if (g_player.bomb.state == ATTACKSTATE_WAIT)
	{ // �T�C�����X���[���h���ҋ@��Ԃ̏ꍇ

		// �J�E���^�[�����Z
		g_tutoInfo.nCounterHeal++;

		if (g_tutoInfo.nCounterHeal >= TUTO_HEAL_BOMB_CNT)
		{ // �J�E���^�[�����l�ȏ�̏ꍇ

			// �J�E���^�[��ݒ�
			g_player.bomb.nCounterState = BOMB_WAIT_CNT;

			// �������Ȃ���Ԃɂ���
			g_player.bomb.state = ATTACKSTATE_NONE;
		}
	}
	else
	{ // �T�C�����X���[���h���ҋ@��Ԃł͂Ȃ��ꍇ

		// �J�E���^�[��������
		g_tutoInfo.nCounterHeal = 0;
	}
}

//============================================================
// �J������ς����Ƃ��̏���
//============================================================
void CameraChange(void)
{
	Camera *pCamera = GetCamera(CAMERATYPE_MAIN);		// ���C���J�����̎擾����

	if (g_player.bCameraFirst == false)
	{ // 1�l�̂���Ȃ��ꍇ

		switch (g_player.nCameraState)
		{
		case PLAYCAMESTATE_NORMAL:			// �ʏ�̃J�������

			// �����_�̈ʒu���X�V
			pCamera->posR.x = g_player.pos.x + sinf(g_player.rot.y + D3DX_PI) * POS_R_PLUS;	// �v���C���[�̈ʒu��菭���O
			pCamera->posR.y = g_player.pos.y + POS_R_PLUS_Y;								// �v���C���[�̈ʒu�Ɠ���
			pCamera->posR.z = g_player.pos.z + cosf(g_player.rot.y + D3DX_PI) * POS_R_PLUS;	// �v���C���[�̈ʒu��菭���O

			// ���_�̈ʒu���X�V
			pCamera->posV.x = pCamera->posR.x + ((pCamera->fDis * sinf(pCamera->rot.x)) * sinf(pCamera->rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu
			pCamera->posV.y = POS_V_Y;																				// �Œ�̍���
			pCamera->posV.z = pCamera->posR.z + ((pCamera->fDis * sinf(pCamera->rot.x)) * cosf(pCamera->rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu

			break;							// �����o��

		case PLAYCAMESTATE_BACK:			// ��������J�������

			// �����_�̈ʒu���X�V
			pCamera->posR.x = g_player.pos.x + sinf(g_player.rot.y + D3DX_PI) * -POS_R_PLUS;	// �v���C���[�̈ʒu��菭���O
			pCamera->posR.y = g_player.pos.y + POS_R_PLUS_Y;									// �v���C���[�̈ʒu�Ɠ���
			pCamera->posR.z = g_player.pos.z + cosf(g_player.rot.y + D3DX_PI) * -POS_R_PLUS;	// �v���C���[�̈ʒu��菭���O

			// ���_�̈ʒu���X�V
			pCamera->posV.x = pCamera->posR.x - ((pCamera->fDis * sinf(pCamera->rot.x)) * sinf(pCamera->rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu
			pCamera->posV.y = POS_V_Y;																				// �Œ�̍���
			pCamera->posV.z = pCamera->posR.z - ((pCamera->fDis * sinf(pCamera->rot.x)) * cosf(pCamera->rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu

			break;							//�����o��
		}
	}
}

//============================================================
// �Q�[���X�^�[�g���̃v���C���[�X�V����
//============================================================
void UpdateGameStartPlayer(void)
{
	Gate *pGate = GetGateData();		// �Q�[�g�̏��

	// �O��ʒu�̍X�V
	g_player.oldPos = g_player.pos;

	// �ړ��ʂ�ݒ肷��
	g_player.move.x = 2.0f;

	// �v���C���[�̈ʒu�̍X�V
	PosPlayer();

	// �v���C���[�̌����̍X�V
	RotPlayer();

	// �v���C���[�̒��n�̍X�V
	LandObject(&g_player.pos, &g_player.move, &g_player.bJump);

	//--------------------------------------------------------
	//	�e�̍X�V
	//--------------------------------------------------------
	// �e�̈ʒu�ݒ�
	SetPositionShadow
	( // ����
		g_player.nShadowID,	// �e�̃C���f�b�N�X
		g_player.pos,		// �ʒu
		g_player.rot,		// ����
		NONE_SCALE			// �g�嗦
	);

	//--------------------------------------------------------
	//	�A�C�R���̍X�V
	//--------------------------------------------------------
	// �A�C�R���̈ʒu�ݒ�
	SetPositionIcon
	(
		g_player.icon.nIconID,
		g_player.pos
	);
}

//============================================================
// �v���C���[�̃Q�[�g�̐ݒ菈��
//============================================================
void SetPlayerGate(void)
{
	int nGateNum = GetGateNum();		// �Q�[�g�̐����擾����
	int nSpawnGateNum;					// �Q�[�g�̔ԍ�
	Gate *pGate = GetGateData();		// �Q�[�g�̏��

	// �o�Ă���Q�[�g�̔ԍ��������_���ŏo��
	nSpawnGateNum = rand() % nGateNum;

	switch (pGate[nSpawnGateNum].collInfo->stateRot)
	{
	case ROTSTATE_0:		// 0�x

		// ������ݒ肷��
		g_player.rot.y = D3DX_PI;

		// �Q�[�g�̈ʒu
		g_player.pos.x = pGate[nSpawnGateNum].pos.x;
		g_player.pos.z = pGate[nSpawnGateNum].pos.z + 300.0f;

		break;

	case ROTSTATE_180:		// 180�x

		// ������ݒ肷��
		g_player.rot.y = 0.0f;

		// �Q�[�g�̈ʒu
		g_player.pos.x = pGate[nSpawnGateNum].pos.x;
		g_player.pos.z = pGate[nSpawnGateNum].pos.z - 300.0f;

		break;

	case ROTSTATE_90:		// 90�x

		// ������ݒ肷��
		g_player.rot.y = -D3DX_PI * 0.5f;

		// �Q�[�g�̈ʒu
		g_player.pos.x = pGate[nSpawnGateNum].pos.x + 300.0f;
		g_player.pos.z = pGate[nSpawnGateNum].pos.z;

		break;

	case ROTSTATE_270:		// 270�x

		// ������ݒ肷��
		g_player.rot.y = D3DX_PI * 0.5f;

		// �Q�[�g�̈ʒu
		g_player.pos.x = pGate[nSpawnGateNum].pos.x - 300.0f;
		g_player.pos.z = pGate[nSpawnGateNum].pos.z;

		break;

	default:				// ��L�ȊO

		break;
	}

	// �����̐��K��
	RotNormalize(&g_player.rot.y);

	// �o�Ă���Q�[�g�̈ʒu��ݒ肷��
	g_player.nNumEnterGate = nSpawnGateNum;
}

#ifdef _DEBUG	// �f�o�b�O����
#endif