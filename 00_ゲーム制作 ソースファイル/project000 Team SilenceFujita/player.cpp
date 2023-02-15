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
#include "game.h"
#include "calculation.h"

#include "camera.h"
#include "gate.h"
#include "object.h"
#include "particle.h"
#include "player.h"
#include "shadow.h"
#include "sound.h"
#include "life.h"
#include "bomb.h"
#include "wind.h"

#include "meshfield.h"
#include "Police.h"
#include "Car.h"
#include "tiremark.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MOVE_FORWARD	(0.1f)		// �v���C���[�O�i���̈ړ���
#define MOVE_BACKWARD	(0.2f)		// �v���C���[��ގ��̈ړ���
#define MOVE_ROT		(0.012f)	// �v���C���[�̌����ύX��
#define REV_MOVE_ROT	(0.085f)	// �ړ��ʂɂ������ύX�ʂ̕␳�W��
#define SUB_MOVE_VALUE	(10.0f)		// �����ύX���̌������s����ړ���
#define SUB_MOVE		(0.15f)		// �����ύX���̌�����
#define REV_MOVE_BRAKE	(0.1f)		// �u���[�L���̌����W��
#define DEL_MOVE_ABS	(1.9f)		// �ړ��ʂ̍폜�͈͂̐�Βl
#define PLAY_GRAVITY	(0.75f)		// �v���C���[�ɂ�����d��
#define MAX_BACKWARD	(-10.0f)	// ��ގ��̍ō����x
#define REV_MOVE_SUB	(0.08f)		// �ړ��ʂ̌����W��
#define UNRIVALED_CNT	(10)		// ���G���Ƀ`�J�`�J������J�E���g

#define PLAY_CLEAR_MOVE		(4.0f)	// �N���A�������̃v���C���[�̎����ړ���
#define REV_PLAY_CLEAR_MOVE	(0.1f)	// �N���A�������̃v���C���[�̌����W��
#define REV_PLAY_OVER_MOVE	(0.02f)	// �N���A���s���̃v���C���[�̌����W��

//------------------------------------------------------------
//	�j�Ŏ��� (�X�����E�u�[�X�g) �}�N����`
//------------------------------------------------------------
#define BOOST_OK_MOVE	(15.0f)		// �u�[�X�g�g�p�ɕK�v�ȃv���C���[�̍Œᑬ�x
#define BOOST_ADD_MOVE	(0.25f)		// �u�[�X�g�̉�����
#define BOOST_SUB_MOVE	(0.08f)		// �u�[�X�g�̌�����
#define BOOST_UP_CNT	(180)		// �u�[�X�g�̉�����Ԃ̎���

#define BOOST_XZ_SUB	(90.0f)		// �u�[�X�g���ˈʒu�� xz���Z��
#define BOOST_Y_ADD		(40.0f)		// �u�[�X�g���ˈʒu�� y���Z��
#define BOOST_SIDE_PULS	(18.0f)		// �u�[�X�g���ˈʒu�̉��ʒu�ύX��

//------------------------------------------------------------
//	����U�� (�t���C�E�A�E�F�C) �}�N����`
//------------------------------------------------------------
#define FLYAWAY_INTERVAL_CNT	(3)			// ���̏o��Ԋu
#define FLYAWAY_SET_CNT			(10)		// ���̏o���
#define FLYAWAY_SHIFT_WIDTH		(90.0f)		// ���̏o��ʒu�����炷��
#define FLYAWAY_SHIFT_HEIGHT	(50.0f)		// ���̏o��ʒu�����炷����
#define FLYAWAY_OVERHEAT_CNT	(80)		// �����I�[�o�[�q�[�g�����Ƃ��̃N�[���_�E���܂ł̎���

//------------------------------------------------------------
//	�������E (�T�C�����X�E���[���h) �}�N����`
//------------------------------------------------------------
#define BOMB_CNT	(2400)	// �{���Ďg�p����

//************************************************************
//	�v���g�^�C�v�錾
//************************************************************
void UpdateNormalPlayer(void);		// �ʏ펞�̃v���C���[�X�V����
void UpdateClearPlayer(void);		// �N���A�������̃v���C���[�X�V����
void UpdateOverPlayer(void);		// �N���A���s���̃v���C���[�X�V����

void MovePlayer(void);				// �v���C���[�̈ړ��ʂ̍X�V����
void PosPlayer(void);				// �v���C���[�̈ʒu�̍X�V����
void RevPlayer(void);				// �v���C���[�̕␳�̍X�V����

void CameraChangePlayer(void);		// �v���C���[�̃J�����̏�ԕω�����
void DriftPlayer(void);				// �v���C���[�̃h���t�g����
void SlumBoostPlayer(void);			// �v���C���[�̉�������
void FlyAwayPlayer(void);			// �v���C���[�̑�������
void SilenceWorldPlayer(void);		// �v���C���[�̔��e����

void UpdateBoost(void);				// �����̍X�V����
void SetBoost(void);				// �����̐ݒ菈��
void UpdateFlyAway(void);			// �����̍X�V����
void UpdateSilenceWorld(void);		// ���e�̍X�V����

//************************************************************
//	�O���[�o���ϐ�
//************************************************************
Player g_player;	// �v���C���[���

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
	g_player.rot           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���݂̌���
	g_player.destRot       = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ڕW�̌���
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
	g_player.bomb.state    = ATTACKSTATE_NONE;					// �U�����
	g_player.bomb.nCounter = BOMB_CNT;							// �U���Ǘ��J�E���^�[

	// �h���t�g�̏��̏�����
	g_player.drift.bDrift = false;	// �h���t�g��

	// �v���C���[�̈ʒu�E�����̐ݒ�
	SetPositionPlayer(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//============================================================
//	�v���C���[�̏I������
//============================================================
void UninitPlayer(void)
{

}

//============================================================
//	�v���C���[�̍X�V����
//============================================================
void UpdatePlayer(void)
{
	if (g_player.bUse == true)
	{ // �v���C���[���g�p����Ă���ꍇ

		if (GetGameState() == GAMESTATE_NORMAL)
		{ // �Q�[�����ʏ��Ԃ̏ꍇ

			// �ʏ펞�̃v���C���[�X�V
			UpdateNormalPlayer();
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

				if (g_player.nCounterState % UNRIVALED_CNT == 0)
				{ // ��莞�Ԍo�ߎ�

					// �����󋵂̓���ւ�
					g_player.bUnrivaled = g_player.bUnrivaled ? false : true;
				}

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
	g_player.rot     = rot;		// ���݂̌���
	g_player.destRot = rot;		// �ڕW�̌���

	// ���f������ݒ�
	g_player.modelData = GetModelData(MODELTYPE_PLAYER_CAR);

	// �e�̃C���f�b�N�X��ݒ�
	g_player.nShadowID = SetModelShadow(g_player.modelData, &g_player.nShadowID, &g_player.bUse);

	// �e�̈ʒu�ݒ�
	SetPositionShadow(g_player.nShadowID, g_player.pos, g_player.rot, NONE_SCALE);

	// �g�p���Ă����Ԃɂ���
	g_player.bUse = true;
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

		if (pPlayer->nLife > 0)
		{ // �̗͂��c���Ă���ꍇ

			// �_���[�W��Ԃɂ���
			pPlayer->state = ACTIONSTATE_DAMAGE;

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

			// �T�E���h�̍Đ�
			//PlaySound(SOUND_LABEL_SE_DMG);			// SE (�_���[�W)
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
//	�ʏ펞�̃v���C���[�X�V����
//============================================================
void UpdateNormalPlayer(void)
{
	// �ϐ���錾
	int nTrafficCnt = 0;	// �����ݒ�p
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
		}
		else if (g_player.nCounterState <= 0)
		{ // �J�E���^�[�� 0�ȉ��̏ꍇ

			// �ʏ��Ԃɂ���
			g_player.state = ACTIONSTATE_NORMAL;
		}
	}

	// �����̍X�V
	UpdateBoost();

	// �v���C���[�̈ړ��ʂ̍X�V
	MovePlayer();

	// �v���C���[�̈ʒu�̍X�V
	PosPlayer();

	// �v���C���[�̒��n�̍X�V
	LandObject(&g_player.pos, &g_player.move, &g_player.bJump);
	
	// �v���C���[�̃h���t�g
	DriftPlayer();

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
		&policeState			// �x�@�̏��
	);

	// ������ԃI�u�W�F�N�g�Ƃ̓����蔻��
	SmashCollision
	( // ����
		g_player.pos, 
		g_player.modelData.fRadius
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
		nTrafficCnt
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
}

//============================================================
//	�N���A���s���̃v���C���[�X�V����
//============================================================
void UpdateOverPlayer(void)
{
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
		&policeState			// �x�@�̏��
	);

	// ������ԃI�u�W�F�N�g�Ƃ̓����蔻��
	SmashCollision
	( // ����
		g_player.pos, 
		g_player.modelData.fRadius
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
		nTrafficCnt
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
}

//============================================================
//	�v���C���[�̈ړ��ʂ̍X�V����
//============================================================
void MovePlayer(void)
{
	if (GetKeyboardPress(DIK_W) == true || GetJoyKeyR2Press(0) == true)
	{ // �O�i�̑��삪�s��ꂽ�ꍇ

		// �ړ��ʂ��X�V
		g_player.move.x += MOVE_FORWARD;

		// �ړ����Ă����Ԃɂ���
		g_player.bMove = true;
	}
	else if (GetKeyboardPress(DIK_S) == true || GetJoyKeyL2Press(0) == true)
	{ // ��ނ̑��삪�s��ꂽ�ꍇ

		if (g_player.move.x >= 5.0f)
		{ // �ړ��ʂ����ȏゾ�����ꍇ
		
			// �h���t�g����
			g_player.drift.bDrift = true;
			
			// �ړ����Ă����Ԃɂ���
			g_player.bMove = true;
		}
		else
		{ // �ړ��ʂ����ȉ��������ꍇ
		
			// �ړ��ʂ��X�V
			g_player.move.x -= MOVE_BACKWARD;

			// �ړ����Ă����Ԃɂ���
			g_player.bMove = true;
		}
	}
	else
	{ // �ړ����Ă��Ȃ��ꍇ

		// �ړ����Ă��Ȃ���Ԃɂ���
		g_player.bMove = false;
	}

	if (GetKeyboardPress(DIK_A) == true || GetJoyStickPressLX(0) < 0)
	{ // �������̑��삪�s��ꂽ�ꍇ

		if (g_player.drift.bDrift == true)
		{ // �h���t�g���������ꍇ
		
			// �������X�V
			g_player.rot.y -= 0.04f;
		}
		else
		{ // �h���t�g�����ᖳ�������ꍇ
		
			// �������X�V
			g_player.rot.y -= MOVE_ROT * ((g_player.move.x + g_player.boost.plusMove.x) * REV_MOVE_ROT);

			if (g_player.move.x >= SUB_MOVE_VALUE)
			{ // �ړ��ʂ����l�ȏ�̏ꍇ

				// �ړ��ʂ��X�V
				g_player.move.x -= SUB_MOVE;

				if (g_player.move.x < SUB_MOVE_VALUE)
				{ // �ړ��ʂ����l��菬�����ꍇ

					// �Œ���̈ړ��ʂ���
					g_player.move.x = SUB_MOVE_VALUE;
				}
			}
		}
	}
	else if (GetKeyboardPress(DIK_D) == true || GetJoyStickPressLX(0) > 0)
	{ // �E�����̑��삪�s��ꂽ�ꍇ

		if (g_player.drift.bDrift == true)
		{ // �h���t�g���������ꍇ
		
			// �������X�V
			g_player.rot.y += 0.04f;
		}
		else
		{ // �h���t�g������Ȃ������ꍇ
		
			// �������X�V
			g_player.rot.y += MOVE_ROT * ((g_player.move.x + g_player.boost.plusMove.x) * REV_MOVE_ROT);

			if (g_player.move.x >= SUB_MOVE_VALUE)
			{ // �ړ��ʂ����l�ȏ�̏ꍇ

				// �ړ��ʂ��X�V
				g_player.move.x -= SUB_MOVE;

				if (g_player.move.x < SUB_MOVE_VALUE)
				{ // �ړ��ʂ����l��菬�����ꍇ

					// �Œ���̈ړ��ʂ���
					g_player.move.x = SUB_MOVE_VALUE;
				}
			}
		}
	}
	else
	{ // A�ED�L�[�������Ă��Ȃ��ꍇ
	
		// �h���t�g���Ă��Ȃ���Ԃɂ���
		g_player.drift.bDrift = false;
	}

	if (GetKeyboardPress(DIK_X) == true || GetJoyKeyPress(JOYKEY_X, 0) == true)
	{ // �u���[�L�̑��삪�s��ꂽ�ꍇ

		// �ړ��ʂ�����
		g_player.move.x += (0.0f - g_player.move.x) * REV_MOVE_BRAKE;

		// �ړ��ʂ̕␳
		if (g_player.move.x <=  DEL_MOVE_ABS
		&&  g_player.move.x >= -DEL_MOVE_ABS)
		{ // �ړ��ʂ��폜�͈͓̔��̏ꍇ

			// �ړ��ʂ��폜
			g_player.move.x = 0.0f;
		}
	}
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
		g_player.move.x *= 0.99f;
	}
	if (g_player.pos.z < GetLimitStage().fFar + (PLAY_DEPTH * 2))
	{ // �͈͊O�̏ꍇ (��)

		// ���Ɉʒu��␳
		g_player.pos.z = GetLimitStage().fFar + (PLAY_DEPTH * 2);

		// �ړ��ʂ��폜
		g_player.move.x *= 0.99f;
	}
	if (g_player.pos.x > GetLimitStage().fRight - (PLAY_WIDTH * 2))
	{ // �͈͊O�̏ꍇ (�E)

		// �E�Ɉʒu��␳
		g_player.pos.x = GetLimitStage().fRight - (PLAY_WIDTH * 2);

		// �ړ��ʂ��폜
		g_player.move.x *= 0.99f;
	}
	if (g_player.pos.x < GetLimitStage().fLeft + (PLAY_WIDTH * 2))
	{ // �͈͊O�̏ꍇ (��)

		// ���Ɉʒu��␳
		g_player.pos.x = GetLimitStage().fLeft + (PLAY_WIDTH * 2);

		// �ړ��ʂ��폜
		g_player.move.x *= 0.99f;
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
//	�v���C���[�̃h���t�g����
//============================================================
void DriftPlayer(void)
{
	if (g_player.drift.bDrift == true)
	{ // �h���t�g���Ă���ꍇ

		// �ړ��ʂ�����������
		g_player.move.x += (0.0f - g_player.move.x) * 0.04f;
	}
}

//============================================================
//	�v���C���[�̉�������
//============================================================
void SlumBoostPlayer(void)
{
	if (GetKeyboardPress(DIK_SPACE) == true || GetJoyKeyPress(JOYKEY_A, 0) == true)
	{ // �����̑��삪�s���Ă���ꍇ

		// �����̐ݒ�
		SetBoost();
	}
}

//============================================================
//	�v���C���[�̑�������
//============================================================
void FlyAwayPlayer(void)
{
	if (GetKeyboardPress(DIK_U) == true || GetJoyKeyPress(JOYKEY_Y, 0) == true)
	{ // �����@�̑��삪�s���Ă���ꍇ

		if (GetWindInfo()->state == WIND_USABLE)
		{ // �����g�p�\�������ꍇ

			// �����@���g�p����
			g_player.wind.bUseWind = true;
		}
	}
	else
	{ // �����@�̑��삪�s���Ă��Ȃ��ꍇ
	
		// �����@���g�p���Ȃ�
		g_player.wind.bUseWind = false;
	}
}

//============================================================
//	�v���C���[�̔��e����
//============================================================
void SilenceWorldPlayer(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyKeyTrigger(JOYKEY_B, 0))
	{ // �U�����[�h�̕ύX�̑��삪�s��ꂽ�ꍇ

		if (g_player.bomb.state != ATTACKSTATE_WAIT)
		{ // �U����Ԃ��U���ҋ@��Ԃł͂Ȃ��ꍇ

			// �U�����[�h��ύX
			g_player.bomb.state = (ATTACKSTATE)((g_player.bomb.state + 1) % ATTACKSTATE_MAX);
		}
	}

	if (GetKeyboardTrigger(DIK_BACKSPACE) == true)
	{ // ���e�̔��˂̑��삪�s��ꂽ�ꍇ

		// �o���A�̔���
		ShotBarrier();
	}
}

//============================================================
//	�v���C���[�̃J�����̏�ԕω�����
//============================================================
void CameraChangePlayer(void)
{
	if (GetKeyboardTrigger(DIK_J) == true || GetJoyKeyPress(JOYKEY_UP, 0) == true)
	{ // �J���������̕ύX���삪�s��ꂽ�ꍇ

		// �J�����̏�Ԃ�ς���
		g_player.nCameraState = (g_player.nCameraState + 1) % PLAYCAMESTATE_MAX;
	}

	if (GetKeyboardTrigger(DIK_K) == true || GetJoyKeyPress(JOYKEY_DOWN, 0) == true)
	{ // ��l�̃J������ ON / OFF �̕ύX���삪�s��ꂽ�ꍇ

		// ��l�̃J�����̏󋵂�ς���
		g_player.bCameraFirst = g_player.bCameraFirst ? false : true;
	}
}

//============================================================
//	�����̍X�V����
//============================================================
void UpdateBoost(void)
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
			g_player.boost.nCounter--;
		}
		else
		{ // �J�E���^�[�� 0�ȉ��̏ꍇ

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
void SetBoost(void)
{
	if (g_player.boost.state == BOOSTSTATE_NONE
	&&  g_player.move.x      >= BOOST_OK_MOVE)
	{ // �u�[�X�g���s�����Ԃ̏ꍇ

		// ������Ԃɂ���
		g_player.boost.state = BOOSTSTATE_UP;

		// �J�E���^�[��ݒ�
		g_player.boost.nCounter = BOOST_UP_CNT;
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
		pWindInfo->nUseCounter--;

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

	case ATTACKSTATE_BOMB:	// �{���U�����

		if (g_player.bomb.nCounter > 0)
		{ // �J�E���^�[�� 0���傫���ꍇ

			// �J�E���^�[�����Z
			g_player.bomb.nCounter--;
		}
		else
		{ // �J�E���^�[�� 0�ȉ��̏ꍇ

			// �U���ҋ@��Ԃɂ���
			g_player.bomb.state = ATTACKSTATE_WAIT;
		}

		// �����𔲂���
		break;

	case ATTACKSTATE_WAIT:	// �U���ҋ@���

		if (g_player.bomb.nCounter < BOMB_CNT)
		{ // �J�E���^�[�����l��菬�����ꍇ

			// �J�E���^�[�����Z
			g_player.bomb.nCounter++;
		}
		else
		{ // �J�E���^�[�����l�ȏ�̏ꍇ

			// �������Ȃ���Ԃɂ���
			g_player.bomb.state = ATTACKSTATE_NONE;
		}

		// �����𔲂���
		break;
	}
}

#ifdef _DEBUG	// �f�o�b�O����
#endif