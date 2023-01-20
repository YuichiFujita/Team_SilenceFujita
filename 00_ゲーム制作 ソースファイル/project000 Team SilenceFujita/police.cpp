//======================================================================================================================
//
//	�x�@�̏��� [Police.cpp]
//	Author�F��������
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "input.h"
#include "model.h"

#include "Police.h"
#include "shadow.h"
#include "sound.h"
#include "player.h"

#ifdef _DEBUG	// �f�o�b�O����
#include "game.h"
#include "EditObject.h"
#endif

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define POLI_LIFE				(50)		// �I�u�W�F�N�g�̗̑�
#define POLI_GRAVITY			(0.75f)		// �v���C���[�ɂ�����d��
#define POLI_MOVE_FORWARD		(0.1f)		// �v���C���[�O�i���̈ړ���
#define POLI_MOVE_BACKWARD		(0.2f)		// �v���C���[��ގ��̈ړ���
#define POLI_MOVE_ROT			(0.012f)	// �v���C���[�̌����ύX��
#define REV_POLI_MOVE_ROT		(0.1f)		// �ړ��ʂɂ������ύX�ʂ̕␳�W��
#define SUB_POLI_MOVE_VALUE		(15.0f)		// �����ύX���̌������s����ړ���
#define SUB_POLI_MOVE			(0.05f)		// �����ύX���̌�����
#define MAX_POLI_FORWARD		(30.0f)		// �O�i���̍ō����x
#define MAX_POLI_FORWARD_PATROL (15.0f)		// �p�g���[�����̑O�i���̍ō����x
#define MAX_POLI_BACKWARD		(8.0f)		// ��ގ��̍ō����x
#define REV_POLI_MOVE_SUB		(0.04f)		// �ړ��ʂ̌����W��
#define POLICAR_WIDTH			(30.0f)		// �p�g�J�[�̏c��
#define POLICAR_HEIGHT			(30.0f)		// �p�g�J�[�̉��s

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void MovePlayer(bool *bMove, D3DXVECTOR3 *move, D3DXVECTOR3 *rot);	// �v���C���[�̈ړ��ʂ̍X�V����
void PosPlayer(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove);	// �v���C���[�̈ʒu�̍X�V����
void RevPlayer(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos);		// �v���C���[�̕␳�̍X�V����
void PatrolPoliceAct(Police *pPolice);					// �x�@�̃p�g���[���s������
void PatrolCarSearch(Police *pPolice);					// �x�@�ԗ��̒T�m����
void ChasePoliceAct(Police *pPolice);					// �x�@�̒ǐՏ���
void PatrolBackPoliceAct(Police *pPolice);				// �x�@�̃p�g���[���ɖ߂�Ƃ��̏���
void PatorolCarFileLoad(void);							// �p�g�J�[�̃��[�h����
void CollisionStopCar(Police *pPolice);					// �Ԃ̒�~����

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
Police g_aPolice[MAX_POLICE];	// �I�u�W�F�N�g�̏��

//======================================================================================================================
//	�x�@�̏���������
//======================================================================================================================
void InitPolice(void)
{
	// �p�g�J�[�̃��[�h����
	PatorolCarFileLoad();

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// �I�u�W�F�N�g�̏��̏�����
	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		g_aPolice[nCntPolice].pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aPolice[nCntPolice].posOld = g_aPolice[nCntPolice].pos;			// �O��̈ʒu
		g_aPolice[nCntPolice].move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
		g_aPolice[nCntPolice].rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aPolice[nCntPolice].rotDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW�̌���
		g_aPolice[nCntPolice].state		= POLICESTATE_PATROL;				// �x�@�̏��
		g_aPolice[nCntPolice].nLife		= 0;								// �̗�
		g_aPolice[nCntPolice].nShadowID = NONE_SHADOW;						// �e�̃C���f�b�N�X
		g_aPolice[nCntPolice].bUse		= false;							// �g�p��
		g_aPolice[nCntPolice].poliDest = POLICEDESTINATION_RIGHT;			// �x�@�̍s��

		// ���f�����̏�����
		g_aPolice[nCntPolice].modelData.dwNumMat = 0;						// �}�e���A���̐�
		g_aPolice[nCntPolice].modelData.pTexture = NULL;					// �e�N�X�`���ւ̃|�C���^
		g_aPolice[nCntPolice].modelData.pMesh    = NULL;					// ���b�V�� (���_���) �ւ̃|�C���^
		g_aPolice[nCntPolice].modelData.pBuffMat = NULL;					// �}�e���A���ւ̃|�C���^
		g_aPolice[nCntPolice].modelData.dwNumMat = 0;						// �}�e���A���̐�
		g_aPolice[nCntPolice].modelData.vtxMin   = INIT_VTX_MIN;			// �ŏ��̒��_���W
		g_aPolice[nCntPolice].modelData.vtxMax   = INIT_VTX_MAX;			// �ő�̒��_���W
		g_aPolice[nCntPolice].modelData.fHeight  = 0.0f;					// �c��
		g_aPolice[nCntPolice].modelData.fRadius  = 0.0f;					// ���a

		// �Ȃ���p�̈ʒu�̏�����
		g_aPolice[nCntPolice].poliCurve.Far	 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���̋Ȃ���p
		g_aPolice[nCntPolice].poliCurve.Left	 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���̋Ȃ���p
		g_aPolice[nCntPolice].poliCurve.Near	 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��O�̋Ȃ���p
		g_aPolice[nCntPolice].poliCurve.Right	 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �E�̋Ȃ���p
	}

	//�x�@�̐ݒ菈��
	SetPolice(D3DXVECTOR3(7000.0f, 0.0f, 1500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), POLICEDESTINATION_RIGHT);
	SetPolice(D3DXVECTOR3(7000.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), POLICEDESTINATION_RIGHT);
	SetPolice(D3DXVECTOR3(3000.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), POLICEDESTINATION_LEFT);
	//SetPolice(D3DXVECTOR3(7000.0f, 0.0f, 1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//SetPolice(D3DXVECTOR3(7000.0f, 0.0f, 1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//======================================================================================================================
//	�x�@�̏I������
//======================================================================================================================
void UninitPolice(void)
{

}

//======================================================================================================================
//	�x�@�̍X�V����
//======================================================================================================================
void UpdatePolice(void)
{
#if 0

	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�
		if (g_aPolice[nCntPolice].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// �O��ʒu�̍X�V
			g_aPolice[nCntPolice].posOld = g_aPolice[nCntPolice].pos;

			//// �v���C���[�̈ړ��ʂ̍X�V
			//MovePlayer(&g_aPolice[nCntPolice].bMove, &g_aPolice[nCntPolice].move, &g_aPolice[nCntPolice].rot);

			//----------------------------------------------------
			//	�e�̍X�V
			//----------------------------------------------------
			// �e�̈ʒu�ݒ�
			SetPositionShadow
			( // ����
				g_aPolice[nCntPolice].nShadowID,	// �e�̃C���f�b�N�X
				g_aPolice[nCntPolice].pos,		// �ʒu
				g_aPolice[nCntPolice].rot,		// ����
				NONE_SCALE			// �g�嗦
			);

			switch (g_aPolice[nCntPolice].state)
			{//��ԂŔ��f����
			case POLICESTATE_PATROL:		//�p�g���[�����

				//�x�@�̃p�g���[���s������
				PatrolPoliceAct(&g_aPolice[nCntPolice]);

				break;						//�����o��

			case POLICESTATE_CHASE:			//�ǐՏ���

				//�x�@�̒ǐՏ���
				ChasePoliceAct(&g_aPolice[nCntPolice]);

				break;						//�����o��

			case POLICESTATE_PATBACK:		//�p�g���[���֖߂鏈��

				break;						//�����o��

			case POLICESTATE_STOP:			//���~�ߏ���

				break;						//�����o��
			}

			// �v���C���[�̈ʒu�̍X�V
			PosPlayer(&g_aPolice[nCntPolice].move, &g_aPolice[nCntPolice].pos, &g_aPolice[nCntPolice].rot, g_aPolice[nCntPolice].bMove);

			//----------------------------------------------------
			//	�����蔻��
			//----------------------------------------------------
			// �I�u�W�F�N�g�Ƃ̓����蔻��
			CollisionPolice
			( // ����
				&g_aPolice[nCntPolice].pos,		// ���݂̈ʒu
				&g_aPolice[nCntPolice].posOld,	// �O��̈ʒu
				POLICAR_WIDTH,					// ����
				POLICAR_HEIGHT,					// ���s
				&g_aPolice[nCntPolice].move.x	// �ړ���
			);

			if (g_aPolice[nCntPolice].pos.y < 0.0f)
			{//Y���̈ʒu��0.0f�������ꍇ
				//�c�ւ̈ړ��ʂ�0.0f�ɂ���
				g_aPolice[nCntPolice].move.y = 0.0f;

				//�ʒu��0.0f�ɖ߂�
				g_aPolice[nCntPolice].pos.y = 0.0f;
			}

			// �v���C���[�̕␳�̍X�V����
			RevPlayer(&g_aPolice[nCntPolice].rot, &g_aPolice[nCntPolice].pos);
		}
	}
#endif
}

//======================================================================================================================
//	�x�@�̕`�揈��
//======================================================================================================================
void DrawPolice(void)
{
	// �ϐ���錾
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aPolice[nCntPolice].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aPolice[nCntPolice].mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPolice[nCntPolice].rot.y, g_aPolice[nCntPolice].rot.x, g_aPolice[nCntPolice].rot.z);
			D3DXMatrixMultiply(&g_aPolice[nCntPolice].mtxWorld, &g_aPolice[nCntPolice].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aPolice[nCntPolice].pos.x, g_aPolice[nCntPolice].pos.y, g_aPolice[nCntPolice].pos.z);
			D3DXMatrixMultiply(&g_aPolice[nCntPolice].mtxWorld, &g_aPolice[nCntPolice].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aPolice[nCntPolice].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aPolice[nCntPolice].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aPolice[nCntPolice].modelData.dwNumMat; nCntMat++)
			{ // �}�e���A���̐����J��Ԃ�

				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aPolice[nCntPolice].modelData.pTexture[nCntMat]);

				// ���f���̕`��
				g_aPolice[nCntPolice].modelData.pMesh->DrawSubset(nCntMat);
			}

			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	�x�@�̐ݒ菈��
//======================================================================================================================
void SetPolice(D3DXVECTOR3 pos, D3DXVECTOR3 rot, POLICEDEST poliDest)
{
	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aPolice[nCntPolice].bUse == false)
		{ // �I�u�W�F�N�g���g�p����Ă��Ȃ��ꍇ
			// ��������
			g_aPolice[nCntPolice].pos		= pos;								// ���݂̈ʒu
			g_aPolice[nCntPolice].posOld	= g_aPolice[nCntPolice].pos;		// �O��̈ʒu
			g_aPolice[nCntPolice].rot		= rot;								// ����
			g_aPolice[nCntPolice].rotDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW�̌���
			g_aPolice[nCntPolice].move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
			g_aPolice[nCntPolice].state		= POLICESTATE_PATROL;				// �p�g���[����Ԃɂ���
			g_aPolice[nCntPolice].nLife		= POLI_LIFE;						// �̗�
			g_aPolice[nCntPolice].bMove		= false;							// �ړ����Ă��Ȃ�
			g_aPolice[nCntPolice].poliDest = poliDest;							// �x�@�̌���

			// �g�p���Ă����Ԃɂ���
			g_aPolice[nCntPolice].bUse = true;

			// ���f������ݒ�
			g_aPolice[nCntPolice].modelData = GetModelData(MODELTYPE_PLAYER_CAR + FROM_OBJECT);	// ���f�����

			// �e�̃C���f�b�N�X��ݒ�
			g_aPolice[nCntPolice].nShadowID = SetModelShadow
			( // ����
				g_aPolice[nCntPolice].modelData,	// ���f�����
				&g_aPolice[nCntPolice].nShadowID,	// �e�̐e�̉e�C���f�b�N�X
				&g_aPolice[nCntPolice].bUse			// �e�̐e�̎g�p��
			);

			// �e�̈ʒu�ݒ�
			SetPositionShadow(g_aPolice[nCntPolice].nShadowID, g_aPolice[nCntPolice].pos, g_aPolice[nCntPolice].rot, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

			//�Ȃ���p������������(75.0f���炷)
			g_aPolice[nCntPolice].poliCurve.Far = D3DXVECTOR3(2925.0f, 0.0f, -2075.0f);
			g_aPolice[nCntPolice].poliCurve.Left = D3DXVECTOR3(2925.0f, 0.0f, 2075.0f);
			g_aPolice[nCntPolice].poliCurve.Near = D3DXVECTOR3(6575.0f, 0.0f, 2075.0f);
			g_aPolice[nCntPolice].poliCurve.Right = D3DXVECTOR3(6575.0f, 0.0f, -2075.0f);

			// �����𔲂���
			break;
		}
	}
}

#if 0
//======================================================================================================================
//	�I�u�W�F�N�g�̃_���[�W����
//======================================================================================================================
void HitPolice(Police *pPolice, int nDamage)
{
	if (pPolice->state == ACTIONSTATE_NORMAL)
	{ // �I�u�W�F�N�g���ʏ��Ԃ̏ꍇ

	  // �����̃_���[�W����̗͂��猸�Z
		pPolice->nLife -= nDamage;

		if (pPolice->nLife > 0)
		{ // �̗͂��c���Ă���ꍇ

		  // �_���[�W��Ԃɂ���
			pPolice->state = ACTIONSTATE_DAMAGE;

			// �p�[�e�B�N���̐ݒ�
			SetParticle
			( // ����
				pPolice->pos,						// �ʒu
				D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f),	// �F
				PARTICLETYPE_DAMAGE,				// ���
				SPAWN_PARTICLE_DAMAGE,				// �G�t�F�N�g��
				2									// ����
			);

			// �J�E���^�[��ݒ�
			pPolice->nCounterState = DAMAGE_TIME_OBJ;

			// �T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_DMG);			// SE (�_���[�W)
		}
		else
		{ // �̗͂��s�����ꍇ

		  // �����̐ݒ�
			SetExplosion(pPolice->pos, SOUNDTYPE_BREAK);

			// �p�[�e�B�N���̐ݒ�
			SetParticle
			( // ����
				pPolice->pos,						// �ʒu
				D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f),	// �F
				PARTICLETYPE_EXPLOSION,				// ���
				SPAWN_PARTICLE_EXPLOSION,			// �G�t�F�N�g��
				2									// ����
			);

			// �A�C�e���̐ݒ�
			SetItem(pPolice->pos, ITEMTYPE_HEAL);

			// �g�p���Ă��Ȃ���Ԃɂ���
			pPolice->bUse = false;
		}
	}
}
#endif

//======================================================================================================================
//	�I�u�W�F�N�g�Ƃ̓����蔻��
//======================================================================================================================
void CollisionPolice(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pOldPos, float fWidth, float fDepth, float *pMove)
{
	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�
		if (g_aPolice[nCntPolice].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// �O��̓����蔻��
			if (pPos->x + fWidth > g_aPolice[nCntPolice].pos.x + g_aPolice[nCntPolice].modelData.vtxMin.x
				&&  pPos->x - fWidth < g_aPolice[nCntPolice].pos.x + g_aPolice[nCntPolice].modelData.vtxMax.x)
			{ // �u���b�N�̍��E�͈͓̔��̏ꍇ

				if (pPos->z + fDepth > g_aPolice[nCntPolice].pos.z + g_aPolice[nCntPolice].modelData.vtxMin.z
					&&  pOldPos->z + fDepth <= g_aPolice[nCntPolice].pos.z + g_aPolice[nCntPolice].modelData.vtxMin.z)
				{ // �O����̓����蔻��
					// �ړ��ʂ�0.0f�ɂ���
					*pMove = 0.0f;
				}
				else if (pPos->z - fDepth < g_aPolice[nCntPolice].pos.z + g_aPolice[nCntPolice].modelData.vtxMax.z
					&&  pOldPos->z - fDepth >= g_aPolice[nCntPolice].pos.z + g_aPolice[nCntPolice].modelData.vtxMax.z)
				{ // ��납��̓����蔻��
					// �ړ��ʂ�0.0f�ɂ���
					*pMove = 0.0f;
				}
			}

			// ���E�̓����蔻��
			if (pPos->z + fDepth > g_aPolice[nCntPolice].pos.z + g_aPolice[nCntPolice].modelData.vtxMin.z
				&&  pPos->z - fDepth < g_aPolice[nCntPolice].pos.z + g_aPolice[nCntPolice].modelData.vtxMax.z)
			{ // �u���b�N�̑O��͈͓̔��̏ꍇ

				if (pPos->x + fWidth > g_aPolice[nCntPolice].pos.x + g_aPolice[nCntPolice].modelData.vtxMin.x
					&&  pOldPos->x + fWidth <= g_aPolice[nCntPolice].pos.x + g_aPolice[nCntPolice].modelData.vtxMin.x)
				{ // ������̓����蔻��
					// �ړ��ʂ�0.0f�ɂ���
					*pMove = 0.0f;
				}
				else if (pPos->x - fWidth < g_aPolice[nCntPolice].pos.x + g_aPolice[nCntPolice].modelData.vtxMax.x
					&&  pOldPos->x - fWidth >= g_aPolice[nCntPolice].pos.x + g_aPolice[nCntPolice].modelData.vtxMax.x)
				{ // �E����̓����蔻��
					// �ړ��ʂ�0.0f�ɂ���
					*pMove = 0.0f;
				}
			}
		}
	}
}

//======================================================================================================================
//	�I�u�W�F�N�g�̎擾����
//======================================================================================================================
Police *GetPoliceData(void)
{
	// �I�u�W�F�N�g�̏��̐擪�A�h���X��Ԃ�
	return &g_aPolice[0];
}

//============================================================
//	�v���C���[�̈ړ��ʂ̍X�V����
//============================================================
void MovePlayer(bool *bMove, D3DXVECTOR3 *move, D3DXVECTOR3 *rot)
{
	if (GetKeyboardPress(DIK_W) == true || GetJoyKeyPress(JOYKEY_UP, 0) == true || GetJoyStickPressLY(0) > 0)
	{ // �O�i�̑��삪�s��ꂽ�ꍇ

		// �ړ��ʂ��X�V
		move->x += 0.1f;

		// �ړ����Ă����Ԃɂ���
		*bMove = true;
	}
	else if (GetKeyboardPress(DIK_S) == true || GetJoyKeyPress(JOYKEY_DOWN, 0) == true || GetJoyStickPressLY(0) < 0)
	{ // ��ނ̑��삪�s��ꂽ�ꍇ

		// �ړ��ʂ��X�V
		move->x -= 0.2f;

		// �ړ����Ă����Ԃɂ���
		*bMove = true;
	}
	else
	{ // �ړ����Ă��Ȃ��ꍇ

		// �ړ����Ă��Ȃ���Ԃɂ���
		*bMove = false;
	}

	if (GetKeyboardPress(DIK_A) == true)
	{ // �������̑��삪�s��ꂽ�ꍇ

		// �������X�V
		rot->y -= 0.012f * (move->x * 0.1f);

		if (move->x >= 15.0f)
		{ // �ړ��ʂ����l�ȏ�̏ꍇ

			// �ړ��ʂ��X�V
			move->x -= 0.05f;
		}
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{ // �E�����̑��삪�s��ꂽ�ꍇ

		// �������X�V
		rot->y += 0.012f * (move->x * 0.1f);

		if (move->x >= 15.0f)
		{ // �ړ��ʂ����l�ȏ�̏ꍇ

			// �ړ��ʂ��X�V
			move->x -= 0.05f;
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
void PosPlayer(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove)
{

	//--------------------------------------------------------
	//	�d�͂̍X�V
	//--------------------------------------------------------
	move->y -= POLI_GRAVITY;

	//--------------------------------------------------------
	//	�ړ��ʂ̕␳
	//--------------------------------------------------------
	if (move->x > MAX_POLI_FORWARD)
	{ // �v���C���[�̈ړ��� (x) �����l�ȏ�̏ꍇ

		// �v���C���[�̈ړ��� (x) ��␳
		move->x = MAX_POLI_FORWARD;
	}
	else if (move->x < -MAX_POLI_BACKWARD)
	{ // �v���C���[�̈ړ��� (x) �����l�ȉ��̏ꍇ

		// �v���C���[�̈ړ��� (x) ��␳
		move->x = -MAX_POLI_BACKWARD;
	}

	//--------------------------------------------------------
	//	�ʒu�̍X�V
	//--------------------------------------------------------
	pos->x += sinf(rot->y) * move->x;
	pos->y += move->y;
	pos->z += cosf(rot->y) * move->x;

	//--------------------------------------------------------
	//	�ړ��ʂ̌���
	//--------------------------------------------------------
	if (bMove == false)
	{ // �ړ����Ă��Ȃ���Ԃ̏ꍇ

		// �ړ��ʂ�����
		move->x += (0.0f - move->x) * 0.04f;
	}
}

//============================================================
//	�v���C���[�̕␳�̍X�V����
//============================================================
void RevPlayer(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos)
{
	//--------------------------------------------------------
	//	�����̐��K��
	//--------------------------------------------------------
	if (rot->y > D3DX_PI) { rot->y -= D3DX_PI * 2; }
	else if (rot->y < -D3DX_PI) { rot->y += D3DX_PI * 2; }

	////--------------------------------------------------------
	////	�ړ��͈͂̕␳
	////--------------------------------------------------------
	//if (pos->z > GetLimitStage().fNear - (30.0f * 2))
	//{ // �͈͊O�̏ꍇ (��O)

	//	// ��O�Ɉʒu��␳
	//	pos->z = GetLimitStage().fNear - (30.0f * 2);
	//}
	//if (pos->z < GetLimitStage().fFar + (30.0f * 2))
	//{ // �͈͊O�̏ꍇ (��)

	//	// ���Ɉʒu��␳
	//	pos->z = GetLimitStage().fFar + (30.0f * 2);
	//}
	//if (pos->x > GetLimitStage().fRight - (30.0f * 2))
	//{ // �͈͊O�̏ꍇ (�E)

	//	// �E�Ɉʒu��␳
	//	pos->x = GetLimitStage().fRight - (30.0f * 2);
	//}
	//if (pos->x < GetLimitStage().fLeft + (30.0f * 2))
	//{ // �͈͊O�̏ꍇ (��)

	//	// ���Ɉʒu��␳
	//	pos->x = GetLimitStage().fLeft + (30.0f * 2);
	//}

	////--------------------------------------------------------
	////	�W�����v����
	////--------------------------------------------------------
	//if (g_player.pos.y < GetLimitStage().fField)
	//{ // �n�ʂɓ������Ă���ꍇ

	//	// �W�����v���Ă��Ȃ���Ԃɂ���
	//	g_player.bJump = false;

	//	// �n�ʂɈʒu��␳
	//	g_player.pos.y = GetLimitStage().fField;

	//	// �ړ��ʂ�������
	//	g_player.move.y = 0.0f;
	//}
	//else
	//{ // �n�ʂɓ������Ă��Ȃ��ꍇ

	//	// �W�����v���Ă����Ԃɂ���
	//	g_player.bJump = true;
	//}
}

//============================================================
//�x�@�̃p�g���[���s������
//============================================================
void PatrolPoliceAct(Police *pPolice)
{
	Player *pPlayer = GetPlayer();					//�v���C���[�̏����擾����

	float fDist;									//�ڕW�̋���

	// �ړ��ʂ��X�V
	pPolice->move.x += POLI_MOVE_FORWARD;

	// �ړ����Ă����Ԃɂ���
	pPolice->bMove = true;

	if (/*pPolice->pos.x >= GetLimitStage().fRight + (POLICAR_WIDTH * 2) || */pPolice->poliDest == POLICEDESTINATION_RIGHT)
	{ // �E�̕ǂ��x�@��荶���ɂ���ꍇ
		if (pPolice->pos.z >= pPolice->poliCurve.Near.z + (POLICAR_WIDTH * 2))
		{ // ���ɂ���ǂ��r�؂ꂽ��

			// �������X�V
			pPolice->rot.y -= 0.05f * (pPolice->move.x * 0.1f);

			if (pPolice->rot.y <= D3DXToRadian(-90))
			{//���̌����ɒB�����ꍇ
				// ������␳
				pPolice->rot.y = D3DXToRadian(-90);

				//�x�@�̍s���ݒ肷��
				pPolice->poliDest = POLICEDESTINATION_NEAR;
			}

			// �ړ��ʂ�����
			pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

			// ��O�̕ǂɔ��킹��
			pPolice->pos.z = pPolice->poliCurve.Near.z + (POLICAR_WIDTH * 2);
		}
		else
		{ // ���ɂ���ǂ��܂���������
			// �E�̕ǂɔ��킹��
			pPolice->pos.x = pPolice->poliCurve.Near.x + (POLICAR_WIDTH * 2);
		}
	}

	if (/*pPolice->pos.z >= GetLimitStage().fNear + (POLICAR_WIDTH * 2) || */pPolice->poliDest == POLICEDESTINATION_NEAR)
	{ // ��O�̕ǂ��x�@��艜�ɂ���ꍇ
		if (pPolice->pos.x <= pPolice->poliCurve.Left.x - (POLICAR_WIDTH * 2))
		{ // ���ɂ���ǂ��r�؂ꂽ��

			// �������X�V
			pPolice->rot.y -= 0.05f * (pPolice->move.x * 0.1f);

			if (pPolice->rot.y <= D3DXToRadian(-180))
			{ // ���̌����ɒB�����ꍇ
				// ������␳
				pPolice->rot.y = D3DXToRadian(180);

				//�x�@�̍s���ݒ肷��
				pPolice->poliDest = POLICEDESTINATION_LEFT;
			}

			// �ړ��ʂ�����
			pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

			// ���̕ǂɔ��킹��
			pPolice->pos.x = pPolice->poliCurve.Left.x - (POLICAR_WIDTH * 2);
		}
		else
		{ // ���ɂ���ǂ��܂���������
			// ��O�̕ǂɔ��킹��
			pPolice->pos.z = pPolice->poliCurve.Left.z + (POLICAR_WIDTH * 2);
		}
	}

	if (/*pPolice->pos.x <= GetLimitStage().fLeft - (POLICAR_WIDTH * 2) || */pPolice->poliDest == POLICEDESTINATION_LEFT)
	{//���̕ǂ��x�@���E�ɂ���ꍇ
		if (pPolice->pos.z <= pPolice->poliCurve.Far.z - (POLICAR_WIDTH * 2))
		{ // ���ɂ���ǂ��r�؂ꂽ��

			// �������X�V
			pPolice->rot.y -= 0.05f * (pPolice->move.x * 0.1f);

			if (pPolice->rot.y <= D3DXToRadian(90))
			{ // ���̌����ɒB�����ꍇ
				// ������␳
				pPolice->rot.y = D3DXToRadian(90);

				//�x�@�̍s���ݒ肷��
				pPolice->poliDest = POLICEDESTINATION_FAR;
			}

			// �ړ��ʂ�����
			pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

			// ���̕ǂɔ��킹��
			pPolice->pos.z = pPolice->poliCurve.Far.z - (POLICAR_WIDTH * 2);
		}
		else
		{ // ���ɂ���ǂ��܂���������
			// ���̕ǂɔ��킹��
			pPolice->pos.x = pPolice->poliCurve.Far.x - (POLICAR_WIDTH * 2);
		}
	}

	if (/*pPolice->pos.z <= GetLimitStage().fFar - (POLICAR_WIDTH * 2) || */pPolice->poliDest == POLICEDESTINATION_FAR)
	{//���̕ǂ��x�@����O�ɂ���ꍇ
		if (pPolice->pos.x >= pPolice->poliCurve.Right.x + (POLICAR_WIDTH * 2))
		{ // ���ɂ���ǂ��r�؂ꂽ��

			// �������X�V
			pPolice->rot.y -= 0.05f * (pPolice->move.x * 0.1f);

			if (pPolice->rot.y <= D3DXToRadian(0))
			{ // ���̌����ɒB�����ꍇ
				// ������␳
				pPolice->rot.y = D3DXToRadian(0);

				//�x�@�̍s���ݒ肷��
				pPolice->poliDest = POLICEDESTINATION_RIGHT;
			}

			// �ړ��ʂ�����
			pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

			// �E�̕ǂɔ��킹��
			pPolice->pos.x = pPolice->poliCurve.Right.x + (POLICAR_WIDTH * 2);
		}
		else
		{ // ���ɂ���ǂ��܂���������
			// ���̕ǂɔ��킹��
			pPolice->pos.z = pPolice->poliCurve.Right.z - (POLICAR_WIDTH * 2);
		}
	}

	if (pPolice->move.x > MAX_POLI_FORWARD_PATROL)
	{ // �v���C���[�̈ړ��� (x) �����l�ȏ�̏ꍇ

		// �v���C���[�̈ړ��� (x) ��␳
		pPolice->move.x = MAX_POLI_FORWARD_PATROL;
	}

	////�ړI�̋�����ݒ肷��
	//fDist = fabsf(sqrtf((pPlayer->pos.x - pPolice->pos.x) * (pPlayer->pos.x - pPolice->pos.x) + (pPlayer->pos.z - pPolice->pos.z) * (pPlayer->pos.z - pPolice->pos.z)));

	//if (fDist <= 700.0f)
	//{ // �ړI�̋��������ȓ��ɓ�������
	//	// �ǐՏ�ԂɈڍs����
	//	pPolice->State = POLICESTATE_CHASE;
	//}

	// �Ԃ̒�~����
	CollisionStopCar(pPolice);

	//if (GetLimitStage().fFar > pPolice->pos.z)
	//{ // �E�̕ǂɓ����肻���ȏꍇ
	//	// �������X�V
	//	pPolice->rot.y -= 0.012f * (pPolice->move.x * 0.5f);

	//	if (pPolice->move.x >= 15.0f)
	//	{ // �ړ��ʂ����l�ȏ�̏ꍇ

	//		// �ړ��ʂ��X�V
	//		pPolice->move.x -= 0.05f;
	//	}

	//	// �ړ��ʂ�����
	//	pPolice->move.x += (0.0f - pPolice->move.x) * 0.04f;
	//}

	//if (GetLimitStage().fLeft > pPolice->pos.x)
	//{ // �E�̕ǂɓ����肻���ȏꍇ
	//	// �������X�V
	//	pPolice->rot.y -= 0.012f * (pPolice->move.x * 0.5f);

	//	if (pPolice->move.x >= 15.0f)
	//	{ // �ړ��ʂ����l�ȏ�̏ꍇ

	//		// �ړ��ʂ��X�V
	//		pPolice->move.x -= 0.05f;
	//	}

	//	// �ړ��ʂ�����
	//	pPolice->move.x += (0.0f - pPolice->move.x) * 0.04f;
	//}

	//if (GetLimitStage().fRight < pPolice->pos.x)
	//{ // �E�̕ǂɓ����肻���ȏꍇ
	//	// �������X�V
	//	pPolice->rot.y -= 0.012f * (pPolice->move.x * 0.5f);

	//	if (pPolice->move.x >= 15.0f)
	//	{ // �ړ��ʂ����l�ȏ�̏ꍇ

	//		// �ړ��ʂ��X�V
	//		pPolice->move.x -= 0.05f;
	//	}

	//	// �ړ��ʂ�����
	//	pPolice->move.x += (0.0f - pPolice->move.x) * 0.04f;
	//}
}

//============================================================
//�x�@�ԗ��̒T�m����
//============================================================
void PatrolCarSearch(Police *pPolice)
{
	Player *pPlayer = GetPlayer();					// �v���C���[�̏����擾����

	float fDist, fRotDest, fRotDiff;				// �ڕW�̋���,�p�x

	// �ړI�̋�����ݒ肷��
	fDist = fabsf(sqrtf((pPlayer->pos.x - pPolice->pos.x) * (pPlayer->pos.x - pPolice->pos.x) + (pPlayer->pos.z - pPolice->pos.z) * (pPlayer->pos.z - pPolice->pos.z)));

	// �ړI�̌�����ݒ肷��
	fRotDest = atan2f(pPlayer->pos.x - pPolice->pos.x, pPlayer->pos.z - pPolice->pos.z);

	if (fDist <= 700.0f)
	{ // �ړI�̋��������ȓ��ɓ�������
		// �ǐՏ�ԂɈڍs����
		pPolice->state = POLICESTATE_CHASE;

		// �����̍��������߂�
		fRotDiff = fRotDest - pPolice->rot.y * 0.8f;

		if (fRotDiff > D3DX_PI)
		{ // �p�x��3.14f���傫�������ꍇ
			// �p�x����1�������炷
			fRotDiff = -D3DX_PI;
		}
		else if (fRotDiff < -D3DX_PI)
		{ // �p�x��-3.14f��菬���������ꍇ
			// �p�x��1����������
			fRotDiff = D3DX_PI;
		}

		// �p�x��␳����
		pPolice->rot.y += fRotDiff;

		if (pPolice->rot.y > D3DX_PI)
		{ // �p�x��3.14f���傫�������ꍇ
			// �p�x����1�������炷
			pPolice->rot.y = -D3DX_PI;
		}
		else if (pPolice->rot.y < -D3DX_PI)
		{ // �p�x��-3.14f��菬���������ꍇ
			// �p�x��1����������
			pPolice->rot.y = D3DX_PI;
		}
	}
	else
	{ // �͈͓��ɓ����Ă��Ȃ��ꍇ
		//����ɖ߂��Ԃɂ���
		pPolice->state = POLICESTATE_PATBACK;
	}
}

//============================================================
//�x�@�̒ǐՏ���
//============================================================
void ChasePoliceAct(Police *pPolice)
{
	//�x�@�ԗ��̒T�m����
	PatrolCarSearch(pPolice);

	if (pPolice->move.x >= 28.0f)
	{ // �ړ��ʂ����l�ȏ�̏ꍇ
		// �ړ��ʂ��X�V
		pPolice->move.x -= 0.05f;
	}
}

//============================================================
// �Ԃ̒�~����
//============================================================
void CollisionStopCar(Police *pPolice)
{
	D3DXVECTOR3 Policepos = D3DXVECTOR3(pPolice->pos.x + sinf(pPolice->rot.y) * 300.0f, 0.0f, pPolice->pos.z + cosf(pPolice->rot.y) * 300.0f);				// �x�@�̈ʒu
	Police *pComparePoli = GetPoliceData();			// �x�@�̏����擾����

	float fLength;									// �����̕ϐ�

	for (int nCntPoli = 0; nCntPoli < MAX_POLICE; nCntPoli++, pComparePoli++)
	{
		if (pComparePoli->bUse == true)
		{ // �g�p���Ă���ꍇ
			// �����𑪂�
			fLength = (pComparePoli->pos.x - Policepos.x) * (pComparePoli->pos.x - Policepos.x)
				+ (pComparePoli->pos.z - Policepos.z) * (pComparePoli->pos.z - Policepos.z);

			if (fLength <= (pComparePoli->modelData.fRadius * pPolice->modelData.fRadius))
			{ // �I�u�W�F�N�g���������Ă���
				// �ړ��ʂ�0.0f�ɂ���
				pPolice->move.x = 0.0f;
			}
		}
	}
}

//============================================================
//�x�@�̃p�g���[���ɖ߂�Ƃ��̏���
//============================================================
void PatrolBackPoliceAct(Police *pPolice)
{

}

//============================================================
// �p�g�J�[�̃��[�h����
//============================================================
void PatorolCarFileLoad(void)
{

}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================
//**********************************************************************************************************************
//	�I�u�W�F�N�g�̑����擾����
//**********************************************************************************************************************
int GetNumPolice(void)
{
	// �ϐ���錾
	int nNumPolice = 0;	// �I�u�W�F�N�g�̑����̊m�F�p

	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aPolice[nCntPolice].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

		  // �J�E���^�[�����Z
			nNumPolice++;
		}
	}

	// �ϐ��̒l��Ԃ�
	return nNumPolice;	// �I�u�W�F�N�g�̑���
}
#endif