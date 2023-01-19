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

#include "camera.h"
#include "object.h"
#include "particle.h"
#include "player.h"
#include "shadow.h"
#include "sound.h"

#include "meshfield.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MOVE_FORWARD	(0.1f)		// �v���C���[�O�i���̈ړ���
#define MOVE_BACKWARD	(0.2f)		// �v���C���[��ގ��̈ړ���
#define MOVE_ROT		(0.013f)	// �v���C���[�̌����ύX��
#define REV_MOVE_ROT	(0.09f)		// �ړ��ʂɂ������ύX�ʂ̕␳�W��
#define SUB_MOVE_VALUE	(10.0f)		// �����ύX���̌������s����ړ���
#define SUB_MOVE		(0.2f)		// �����ύX���̌�����

#define PLAY_GRAVITY	(0.75f)		// �v���C���[�ɂ�����d��
#define MAX_BACKWARD	(-8.0f)		// ��ގ��̍ō����x
#define REV_MOVE_SUB	(0.02f)		// �ړ��ʂ̌����W��

//************************************************************
//	�v���g�^�C�v�錾
//************************************************************
void MovePlayer(void);	// �v���C���[�̈ړ��ʂ̍X�V����
void PosPlayer(void);	// �v���C���[�̈ʒu�̍X�V����
void RevPlayer(void);	// �v���C���[�̕␳�̍X�V����
void LandPlayer(void);	// �v���C���[�̒��n�̍X�V����
void CameraChangePlayer(void);		//�v���C���[�̃J�����̏�ԕω�����

//************************************************************
//	�O���[�o���ϐ�
//************************************************************
Player g_player;		// �v���C���[���

//============================================================
//	�v���C���[�̏���������
//============================================================
void InitPlayer(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// �v���C���[�̖{�̏��̏�����
	g_player.pos           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���݂̈ʒu
	g_player.oldPos        = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �O��̈ʒu
	g_player.move          = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
	g_player.rot           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���݂̌���
	g_player.destRot       = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW�̌���
	g_player.state         = ACTIONSTATE_NORMAL;			// �v���C���[�̏��
	g_player.nLife         = PLAY_LIFE;						// �̗�
	g_player.nCounterState = 0;								// ��ԊǗ��J�E���^�[
	g_player.nShadowID     = NONE_SHADOW;					// �e�̃C���f�b�N�X
	g_player.bMove         = false;							// �ړ���
	g_player.bJump         = false;							// �W�����v��
	g_player.nCameraState  = PLAYERCAME_NORMAL;				//�v���C���[�̃J����
	g_player.bUse          = false;							// �g�p��

	// ���f�����̏�����
	g_player.modelData.dwNumMat = 0;			// �}�e���A���̐�
	g_player.modelData.pTexture = NULL;			// �e�N�X�`���ւ̃|�C���^
	g_player.modelData.pMesh    = NULL;			// ���b�V�� (���_���) �ւ̃|�C���^
	g_player.modelData.pBuffMat = NULL;			// �}�e���A���ւ̃|�C���^
	g_player.modelData.dwNumMat = 0;			// �}�e���A���̐�
	g_player.modelData.vtxMin   = INIT_VTX_MIN;	// �ŏ��̒��_���W
	g_player.modelData.vtxMax   = INIT_VTX_MAX;	// �ő�̒��_���W
	g_player.modelData.fHeight  = 0.0f;			// �c��
	g_player.modelData.fRadius  = 0.0f;			// ���a

	// �v���C���[�̈ʒu�E�����̐ݒ�
	SetPositionPlayer(D3DXVECTOR3(2000.0f, 0.0f, 2000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
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

		// �v���C���[�̈ړ��ʂ̍X�V
		MovePlayer();

		// �v���C���[�̈ʒu�̍X�V
		PosPlayer();

		// �v���C���[�̕␳�̍X�V����
		RevPlayer();

		// �v���C���[�̒��n�̍X�V����
		LandPlayer();

		//�v���C���[�̃J�����̏�ԕω�����
		CameraChangePlayer();

		//----------------------------------------------------
		//	�����蔻��
		//----------------------------------------------------
		// �I�u�W�F�N�g�Ƃ̓����蔻��
		CollisionObject
		( // ����
			&g_player.pos,		// ���݂̈ʒu
			&g_player.oldPos,	// �O��̈ʒu
			PLAY_WIDTH,			// ����
			PLAY_DEPTH			// ���s
		);

		//----------------------------------------------------
		//	�e�̍X�V
		//----------------------------------------------------
		// �e�̈ʒu�ݒ�
		SetPositionShadow
		( // ����
			g_player.nShadowID,	// �e�̃C���f�b�N�X
			g_player.pos,		// �ʒu
			g_player.rot,		// ����
			NONE_SCALE			// �g�嗦
		);
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

				// �����𔲂���
				break;

			default:					// ����ȊO�̏��

				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// �ʏ�

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

#if 0
//============================================================
//	�v���C���[�̉񕜔���
//============================================================
void HealPlayer(MainPlayer *pPlayer, int nHeal)
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
		PlaySound(SOUND_LABEL_SE_HEAL);		// SE (��)
	}
}

//============================================================
//	�v���C���[�̃_���[�W����
//============================================================
void HitPlayer(MainPlayer *pPlayer, int nDamage)
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
			PlaySound(SOUND_LABEL_SE_DMG);			// SE (�_���[�W)
		}
		else
		{ // �̗͂��s�����ꍇ

			// �����̐ݒ�
			SetExplosion(pPlayer->pos, SOUNDTYPE_EXPLOSION);

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
#endif

//============================================================
//	�v���C���[�̈ړ��ʂ̍X�V����
//============================================================
void MovePlayer(void)
{
	if (GetKeyboardPress(DIK_W) == true || GetJoyKeyPress(JOYKEY_UP, 0) == true || GetJoyStickPressLY(0) > 0)
	{ // �O�i�̑��삪�s��ꂽ�ꍇ

		// �ړ��ʂ��X�V
		g_player.move.x += MOVE_FORWARD;

		// �ړ����Ă����Ԃɂ���
		g_player.bMove = true;
	}
	else if (GetKeyboardPress(DIK_S) == true || GetJoyKeyPress(JOYKEY_DOWN, 0) == true || GetJoyStickPressLY(0) < 0)
	{ // ��ނ̑��삪�s��ꂽ�ꍇ

		// �ړ��ʂ��X�V
		g_player.move.x -= MOVE_BACKWARD;

		// �ړ����Ă����Ԃɂ���
		g_player.bMove = true;
	}
	else
	{ // �ړ����Ă��Ȃ��ꍇ

		// �ړ����Ă��Ȃ���Ԃɂ���
		g_player.bMove = false;
	}

	if (GetKeyboardPress(DIK_A) == true)
	{ // �������̑��삪�s��ꂽ�ꍇ

		// �������X�V
		g_player.rot.y -= MOVE_ROT * (g_player.move.x * REV_MOVE_ROT);

		if (g_player.move.x >= SUB_MOVE_VALUE)
		{ // �ړ��ʂ����l�ȏ�̏ꍇ

			// �ړ��ʂ��X�V
			g_player.move.x -= SUB_MOVE;
		}
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{ // �E�����̑��삪�s��ꂽ�ꍇ

		// �������X�V
		g_player.rot.y += MOVE_ROT * (g_player.move.x * REV_MOVE_ROT);

		if (g_player.move.x >= SUB_MOVE_VALUE)
		{ // �ړ��ʂ����l�ȏ�̏ꍇ

			// �ړ��ʂ��X�V
			g_player.move.x -= SUB_MOVE;
		}
	}

	//if (GetKeyboardPress(DIK_SPACE) == true)
	//{ // �����̑��삪�s��ꂽ�ꍇ

	//	// �ړ��ʂ��X�V
	//	g_player.move.x += 20.0f;
	//}
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
	g_player.pos.x += sinf(g_player.rot.y) * g_player.move.x;
	g_player.pos.y += g_player.move.y;
	g_player.pos.z += cosf(g_player.rot.y) * g_player.move.x;

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
	//--------------------------------------------------------
	//	�����̐��K��
	//--------------------------------------------------------
	if      (g_player.rot.y >  D3DX_PI) { g_player.rot.y -= D3DX_PI * 2; }
	else if (g_player.rot.y < -D3DX_PI) { g_player.rot.y += D3DX_PI * 2; }

	//--------------------------------------------------------
	//	�ړ��͈͂̕␳
	//--------------------------------------------------------
	if (g_player.pos.z > GetLimitStage().fNear - (PLAY_DEPTH * 2))
	{ // �͈͊O�̏ꍇ (��O)

		// ��O�Ɉʒu��␳
		g_player.pos.z = GetLimitStage().fNear - (PLAY_DEPTH * 2);
	}
	if (g_player.pos.z < GetLimitStage().fFar + (PLAY_DEPTH * 2))
	{ // �͈͊O�̏ꍇ (��)

		// ���Ɉʒu��␳
		g_player.pos.z = GetLimitStage().fFar + (PLAY_DEPTH * 2);
	}
	if (g_player.pos.x > GetLimitStage().fRight - (PLAY_WIDTH * 2))
	{ // �͈͊O�̏ꍇ (�E)

		// �E�Ɉʒu��␳
		g_player.pos.x = GetLimitStage().fRight - (PLAY_WIDTH * 2);
	}
	if (g_player.pos.x < GetLimitStage().fLeft + (PLAY_WIDTH * 2))
	{ // �͈͊O�̏ꍇ (��)

		// ���Ɉʒu��␳
		g_player.pos.x = GetLimitStage().fLeft + (PLAY_WIDTH * 2);
	}
}

//============================================================
//	�v���C���[�̒��n�̍X�V����
//============================================================
void LandPlayer(void)
{
	// �ϐ���錾
	float fLandPosY = CollisionMeshField(g_player.pos);		// ���n�_

	if (g_player.pos.y < fLandPosY)
	{ // ���n�_�ɓ������Ă���ꍇ

		// �W�����v���Ă��Ȃ���Ԃɂ���
		g_player.bJump = false;

		// �ʒu��␳
		g_player.pos.y = fLandPosY;

		// �ړ��ʂ�������
		g_player.move.y = 0.0f;
	}
	else
	{ // ���n�_�ɓ������Ă��Ȃ��ꍇ

		// �W�����v���Ă����Ԃɂ���
		g_player.bJump = true;
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
//	�v���C���[�̃J�����̏�ԕω�����
//============================================================
void CameraChangePlayer(void)
{
	// �|�C���^��錾
	Camera *pCamera = GetCamera();	// �J�����̏��

	if (GetKeyboardTrigger(DIK_J) == true)
	{ // J�L�[���������ꍇ

		// �J�����̏�Ԃ�ς���
		g_player.nCameraState = (g_player.nCameraState + 1) % PLAYERCAME_MAX;

		if (g_player.nCameraState == PLAYERCAME_NORMAL)
		{

			// �ڕW�̒����_�̈ʒu���X�V
			pCamera->posR.x = g_player.pos.x + sinf(g_player.rot.y + D3DX_PI) * 25.0f;	// �v���C���[�̈ʒu��菭���O
			pCamera->posR.y = g_player.pos.y + 110.0f;									// �v���C���[�̈ʒu�Ɠ���
			pCamera->posR.z = g_player.pos.z + cosf(g_player.rot.y + D3DX_PI) * 25.0f;	// �v���C���[�̈ʒu��菭���O

			// �ڕW�̎��_�̈ʒu���X�V
			pCamera->posV.x = pCamera->posR.x + ((pCamera->fDis * sinf(pCamera->rot.x)) * sinf(pCamera->rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu
			pCamera->posV.y = 200.0f;																				// �Œ�̍���
			pCamera->posV.z = pCamera->posR.z + ((pCamera->fDis * sinf(pCamera->rot.x)) * cosf(pCamera->rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu
		}
	}
}

#ifdef _DEBUG	// �f�o�b�O����
#endif