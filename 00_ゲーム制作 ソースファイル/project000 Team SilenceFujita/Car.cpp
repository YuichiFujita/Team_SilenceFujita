//======================================================================================================================
//
//	�Ԃ̏��� [Car.cpp]
//	Author�F��������
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "input.h"
#include "model.h"

#include "Car.h"
#include "shadow.h"
#include "sound.h"
#include "player.h"
#include "Police.h"

#ifdef _DEBUG	// �f�o�b�O����
#include "game.h"
#endif

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define CAR_LIFE				(50)		// �I�u�W�F�N�g�̗̑�
#define CAR_GRAVITY				(0.75f)		// �v���C���[�ɂ�����d��
#define CAR_MOVE_FORWARD		(0.1f)		// �v���C���[�O�i���̈ړ���
#define CAR_MOVE_BACKWARD		(0.2f)		// �v���C���[��ގ��̈ړ���
#define CAR_MOVE_ROT			(0.012f)	// �v���C���[�̌����ύX��
#define REV_CAR_MOVE_ROT		(0.1f)		// �ړ��ʂɂ������ύX�ʂ̕␳�W��
#define SUB_CAR_MOVE_VALUE		(15.0f)		// �����ύX���̌������s����ړ���
#define SUB_CAR_MOVE			(0.05f)		// �����ύX���̌�����
#define MAX_CAR_FORWARD			(30.0f)		// �O�i���̍ō����x
#define MAX_CAR_FORWARD_PATROL  (15.0f)		// �p�g���[�����̑O�i���̍ō����x
#define MAX_CAR_BACKWARD		(8.0f)		// ��ގ��̍ō����x
#define REV_CAR_MOVE_SUB		(0.04f)		// �ړ��ʂ̌����W��
#define CAR_WIDTH				(30.0f)		// �p�g�J�[�̏c��
#define CAR_HEIGHT				(30.0f)		// �p�g�J�[�̉��s

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void PosCar(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove);		// �Ԃ̈ʒu�̍X�V����
void RevCar(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos);	// �Ԃ̕␳�̍X�V����
void CurveCar(Car *pCar);							// �Ԃ̃J�[�u����

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
Car g_aCar[MAX_CAR];		// �Ԃ̏��

//======================================================================================================================
//	�Ԃ̏���������
//======================================================================================================================
void InitCar(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�ւ̃|�C���^

	// �Ԃ̏��̏�����
	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		g_aCar[nCntCar].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aCar[nCntCar].posOld = g_aCar[nCntCar].pos;			// �O��̈ʒu
		g_aCar[nCntCar].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
		g_aCar[nCntCar].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aCar[nCntCar].nShadowID = NONE_SHADOW;				// �e�̃C���f�b�N�X
		g_aCar[nCntCar].bMove = false;							// �ړ����Ă��邩
		g_aCar[nCntCar].bUse = false;							// �g�p��

		//�Ȃ���p�̏���������
		for (int nCntCurve = 0; nCntCurve < MAX_CURVE; nCntCurve++)
		{
			g_aCar[nCntCar].carCurve.curveAngle[nCntCurve] = CURVE_RIGHT;						// �Ȃ������
			g_aCar[nCntCar].carCurve.curvePoint[nCntCurve] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �Ȃ���|�C���g
			g_aCar[nCntCar].carCurve.nNowCurve = 0;												// ���݂̋Ȃ���p
			g_aCar[nCntCar].carCurve.bCurveX[nCntCurve] = false;								// X���𑖂��Ă��邩
			g_aCar[nCntCar].carCurve.fCurveRot[nCntCurve] = 0;									// �ڕW�̌���
		}
		g_aCar[nCntCar].carCurve.nCurveTime = 0;				// �Ȃ����
		g_aCar[nCntCar].carCurve.nNowCurve = 0;					// ���݂̃��[�g

		// ���f�����̏�����
		g_aCar[nCntCar].modelData.dwNumMat = 0;					// �}�e���A���̐�
		g_aCar[nCntCar].modelData.pTexture = NULL;				// �e�N�X�`���ւ̃|�C���^
		g_aCar[nCntCar].modelData.pMesh = NULL;					// ���b�V�� (���_���) �ւ̃|�C���^
		g_aCar[nCntCar].modelData.pBuffMat = NULL;				// �}�e���A���ւ̃|�C���^
		g_aCar[nCntCar].modelData.vtxMin = INIT_VTX_MIN;		// �ŏ��̒��_���W
		g_aCar[nCntCar].modelData.vtxMax = INIT_VTX_MAX;		// �ő�̒��_���W
		g_aCar[nCntCar].modelData.size = INIT_SIZE;				// �傫��
		g_aCar[nCntCar].modelData.fRadius = 0.0f;				// ���a
	}
}

//======================================================================================================================
//	�Ԃ̏I������
//======================================================================================================================
void UninitCar(void)
{

}

//======================================================================================================================
//	�Ԃ̍X�V����
//======================================================================================================================
void UpdateCar(void)
{
	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�
		if (g_aCar[nCntCar].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// �O��ʒu�̍X�V
			g_aCar[nCntCar].posOld = g_aCar[nCntCar].pos;

			//----------------------------------------------------
			//	�e�̍X�V
			//----------------------------------------------------
			// �e�̈ʒu�ݒ�
			SetPositionShadow
			( // ����
				g_aCar[nCntCar].nShadowID,	// �e�̃C���f�b�N�X
				g_aCar[nCntCar].pos,			// �ʒu
				g_aCar[nCntCar].rot,			// ����
				NONE_SCALE							// �g�嗦
			);

			//�Ԃ̃J�[�u����
			CurveCar(&g_aCar[nCntCar]);

			// �Ԃ̒�~����
			CollisionStopCar
			( // ����
				g_aCar[nCntCar].pos,		//�ʒu
				g_aCar[nCntCar].rot,		//����
				&g_aCar[nCntCar].move,		//�ړ���
				g_aCar[nCntCar].modelData.fRadius,	//���a
				COLLOBJECTTYPE_CAR			//�Ώۂ̃T�C�Y
			);

			// �v���C���[�̈ʒu�̍X�V
			PosCar(&g_aCar[nCntCar].move, &g_aCar[nCntCar].pos, &g_aCar[nCntCar].rot, g_aCar[nCntCar].bMove);

			//----------------------------------------------------
			//	�����蔻��
			//----------------------------------------------------
			// �I�u�W�F�N�g�Ƃ̓����蔻��
			CollisionCar
			( // ����
				&g_aCar[nCntCar].pos,		// ���݂̈ʒu
				&g_aCar[nCntCar].posOld,	// �O��̈ʒu
				CAR_WIDTH,					// ����
				CAR_HEIGHT					// ���s	
			);

			if (g_aCar[nCntCar].pos.y < 0.0f)
			{//Y���̈ʒu��0.0f�������ꍇ
				//�c�ւ̈ړ��ʂ�0.0f�ɂ���
				g_aCar[nCntCar].move.y = 0.0f;

				//�ʒu��0.0f�ɖ߂�
				g_aCar[nCntCar].pos.y = 0.0f;
			}

			// �v���C���[�̕␳�̍X�V����
			RevCar(&g_aCar[nCntCar].rot, &g_aCar[nCntCar].pos);
		}
	}
}

//======================================================================================================================
//	�Ԃ̕`�揈��
//======================================================================================================================
void DrawCar(void)
{
	// �ϐ���錾
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aCar[nCntCar].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aCar[nCntCar].mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aCar[nCntCar].rot.y, g_aCar[nCntCar].rot.x, g_aCar[nCntCar].rot.z);
			D3DXMatrixMultiply(&g_aCar[nCntCar].mtxWorld, &g_aCar[nCntCar].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aCar[nCntCar].pos.x, g_aCar[nCntCar].pos.y, g_aCar[nCntCar].pos.z);
			D3DXMatrixMultiply(&g_aCar[nCntCar].mtxWorld, &g_aCar[nCntCar].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aCar[nCntCar].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aCar[nCntCar].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aCar[nCntCar].modelData.dwNumMat; nCntMat++)
			{ // �}�e���A���̐����J��Ԃ�

				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aCar[nCntCar].modelData.pTexture[nCntMat]);

				// ���f���̕`��
				g_aCar[nCntCar].modelData.pMesh->DrawSubset(nCntMat);
			}

			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	�Ԃ̐ݒ菈��
//======================================================================================================================
void SetCar(D3DXVECTOR3 pos, CURVE carCurve)
{
	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aCar[nCntCar].bUse == false)
		{ // �I�u�W�F�N�g���g�p����Ă��Ȃ��ꍇ
			// ��������
			g_aCar[nCntCar].pos = pos;								// ���݂̈ʒu
			g_aCar[nCntCar].posOld = g_aCar[nCntCar].pos;			// �O��̈ʒu
			g_aCar[nCntCar].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
			g_aCar[nCntCar].bMove = false;							// �ړ����Ă��Ȃ�

			// �g�p���Ă����Ԃɂ���
			g_aCar[nCntCar].bUse = true;

			// ���f������ݒ�
			g_aCar[nCntCar].modelData = GetModelData(MODELTYPE_PLAYER_CAR + FROM_OBJECT);	// ���f�����

			// �e�̃C���f�b�N�X��ݒ�
			g_aCar[nCntCar].nShadowID = SetModelShadow
			( // ����
				g_aCar[nCntCar].modelData,	// ���f�����
				&g_aCar[nCntCar].nShadowID,	// �e�̐e�̉e�C���f�b�N�X
				&g_aCar[nCntCar].bUse		// �e�̐e�̎g�p��
			);

			// �e�̈ʒu�ݒ�
			SetPositionShadow(g_aCar[nCntCar].nShadowID, g_aCar[nCntCar].pos, g_aCar[nCntCar].rot, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

			//�Ȃ���p�֌W�̐ݒ�
			for (int nCntCurve = 0; nCntCurve < carCurve.nCurveTime; nCntCurve++)
			{
				g_aCar[nCntCar].carCurve.fCurveRot[nCntCurve] = carCurve.fCurveRot[nCntCurve];		// ���Ȃ���ڕW�̕���
				g_aCar[nCntCar].carCurve.curvePoint[nCntCurve] = carCurve.curvePoint[nCntCurve];	// ���Ȃ���ʒu
			}
			g_aCar[nCntCar].carCurve.nNowCurve = 0;													// ���݂̃��[�g
			g_aCar[nCntCar].carCurve.nCurveTime = carCurve.nCurveTime;								// �Ȃ����

			//�J�[�u�|�C���g�̐ݒ菈��
			SetCurvePoint(&g_aCar[nCntCar].carCurve, &g_aCar[nCntCar].rot, &g_aCar[nCntCar].pos);

			// �����𔲂���
			break;
		}
	}
}

#if 0
//======================================================================================================================
//	�I�u�W�F�N�g�̃_���[�W����
//======================================================================================================================
void HitCar(Car *pCar, int nDamage)
{
	if (pCar->state == ACTIONSTATE_NORMAL)
	{ // �I�u�W�F�N�g���ʏ��Ԃ̏ꍇ

	  // �����̃_���[�W����̗͂��猸�Z
		pCar->nLife -= nDamage;

		if (pCar->nLife > 0)
		{ // �̗͂��c���Ă���ꍇ

		  // �_���[�W��Ԃɂ���
			pCar->state = ACTIONSTATE_DAMAGE;

			// �p�[�e�B�N���̐ݒ�
			SetParticle
			( // ����
				pCar->pos,						// �ʒu
				D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f),	// �F
				PARTICLETYPE_DAMAGE,				// ���
				SPAWN_PARTICLE_DAMAGE,				// �G�t�F�N�g��
				2									// ����
			);

			// �J�E���^�[��ݒ�
			pCar->nCounterState = DAMAGE_TIME_OBJ;

			// �T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_DMG);			// SE (�_���[�W)
		}
		else
		{ // �̗͂��s�����ꍇ

		  // �����̐ݒ�
			SetExplosion(pCar->pos, SOUNDTYPE_BREAK);

			// �p�[�e�B�N���̐ݒ�
			SetParticle
			( // ����
				pCar->pos,						// �ʒu
				D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f),	// �F
				PARTICLETYPE_EXPLOSION,				// ���
				SPAWN_PARTICLE_EXPLOSION,			// �G�t�F�N�g��
				2									// ����
			);

			// �A�C�e���̐ݒ�
			SetItem(pCar->pos, ITEMTYPE_HEAL);

			// �g�p���Ă��Ȃ���Ԃɂ���
			pCar->bUse = false;
		}
	}
}
#endif

//======================================================================================================================
//	�I�u�W�F�N�g�Ƃ̓����蔻��
//======================================================================================================================
void CollisionCar(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pOldPos, float fWidth, float fDepth)
{
	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�
		if (g_aCar[nCntCar].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

		  // �O��̓����蔻��
			if (pPos->x + fWidth > g_aCar[nCntCar].pos.x + g_aCar[nCntCar].modelData.vtxMin.x
				&&  pPos->x - fWidth < g_aCar[nCntCar].pos.x + g_aCar[nCntCar].modelData.vtxMax.x)
			{ // �u���b�N�̍��E�͈͓̔��̏ꍇ

				if (pPos->z + fDepth > g_aCar[nCntCar].pos.z + g_aCar[nCntCar].modelData.vtxMin.z
					&&  pOldPos->z + fDepth <= g_aCar[nCntCar].pos.z + g_aCar[nCntCar].modelData.vtxMin.z)
				{ // �O����̓����蔻��
				  // �ʒu��␳
					pPos->z = g_aCar[nCntCar].pos.z + g_aCar[nCntCar].modelData.vtxMin.z - fDepth - 0.01f;
				}
				else if (pPos->z - fDepth < g_aCar[nCntCar].pos.z + g_aCar[nCntCar].modelData.vtxMax.z
					&&  pOldPos->z - fDepth >= g_aCar[nCntCar].pos.z + g_aCar[nCntCar].modelData.vtxMax.z)
				{ // ��납��̓����蔻��
				  // �ʒu��␳
					pPos->z = g_aCar[nCntCar].pos.z + g_aCar[nCntCar].modelData.vtxMax.z + fDepth + 0.01f;
				}
			}

			// ���E�̓����蔻��
			if (pPos->z + fDepth > g_aCar[nCntCar].pos.z + g_aCar[nCntCar].modelData.vtxMin.z
				&&  pPos->z - fDepth < g_aCar[nCntCar].pos.z + g_aCar[nCntCar].modelData.vtxMax.z)
			{ // �u���b�N�̑O��͈͓̔��̏ꍇ

				if (pPos->x + fWidth > g_aCar[nCntCar].pos.x + g_aCar[nCntCar].modelData.vtxMin.x
					&&  pOldPos->x + fWidth <= g_aCar[nCntCar].pos.x + g_aCar[nCntCar].modelData.vtxMin.x)
				{ // ������̓����蔻��
				  // �ʒu��␳
					pPos->x = g_aCar[nCntCar].pos.x + g_aCar[nCntCar].modelData.vtxMin.x - fWidth - 0.01f;
				}
				else if (pPos->x - fWidth < g_aCar[nCntCar].pos.x + g_aCar[nCntCar].modelData.vtxMax.x
					&&  pOldPos->x - fWidth >= g_aCar[nCntCar].pos.x + g_aCar[nCntCar].modelData.vtxMax.x)
				{ // �E����̓����蔻��
				  // �ʒu��␳
					pPos->x = g_aCar[nCntCar].pos.x + g_aCar[nCntCar].modelData.vtxMax.x + fWidth + 0.01f;
				}
			}
		}
	}
}

//======================================================================================================================
//	�I�u�W�F�N�g�̎擾����
//======================================================================================================================
Car *GetCarData(void)
{
	// �I�u�W�F�N�g�̏��̐擪�A�h���X��Ԃ�
	return &g_aCar[0];
}

//============================================================
//	�v���C���[�̈ʒu�̍X�V����
//============================================================
void PosCar(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove)
{
	//--------------------------------------------------------
	//	�d�͂̍X�V
	//--------------------------------------------------------
	move->y -= CAR_GRAVITY;

	//--------------------------------------------------------
	//	�ړ��ʂ̕␳
	//--------------------------------------------------------
	if (move->x > MAX_CAR_FORWARD)
	{ // �v���C���[�̈ړ��� (x) �����l�ȏ�̏ꍇ

	  // �v���C���[�̈ړ��� (x) ��␳
		move->x = MAX_CAR_FORWARD;
	}
	else if (move->x < -MAX_CAR_BACKWARD)
	{ // �v���C���[�̈ړ��� (x) �����l�ȉ��̏ꍇ

	  // �v���C���[�̈ړ��� (x) ��␳
		move->x = -MAX_CAR_BACKWARD;
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
void RevCar(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos)
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
}

//============================================================
// �Ԃ̋Ȃ��鏈��
//============================================================
void CurveCar(Car *pCar)
{
	// �ړ��ʂ��X�V
	pCar->move.x += CAR_MOVE_FORWARD;

	// �ړ����Ă����Ԃɂ���
	pCar->bMove = true;

	if (pCar->carCurve.bCurveX[pCar->carCurve.nNowCurve] == true)
	{//X���𑖂��Ă����ꍇ
		if (pCar->carCurve.bCurvePlus[pCar->carCurve.nNowCurve] == true)
		{ // �E�ɑ����Ă���ꍇ
			if (pCar->carCurve.curveAngle[pCar->carCurve.nNowCurve] == CURVE_RIGHT)
			{//�E�ɋȂ���ꍇ
				if (pCar->pos.x >= pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x - (CAR_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

					// �Ԃ̊p�x�X�V�E�␳����
					CurveRotCar(&pCar->carCurve, &pCar->rot, &pCar->move);

					// �ړ��ʂ�����
					pCar->move.x += (0.0f - pCar->move.x) * REV_CAR_MOVE_SUB;

					// �E�̕ǂɔ��킹��
					pCar->pos.x = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x - (CAR_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
					// ���̕ǂɔ��킹��
					pCar->pos.z = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z - (CAR_WIDTH * 2);
				}
			}
			else if (pCar->carCurve.curveAngle[pCar->carCurve.nNowCurve] == CURVE_LEFT)
			{//���ɋȂ���ꍇ
				if (pCar->pos.x >= pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x + (CAR_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

					// �Ԃ̊p�x�X�V�E�␳����
					CurveRotCar(&pCar->carCurve, &pCar->rot, &pCar->move);

					// �ړ��ʂ�����
					pCar->move.x += (0.0f - pCar->move.x) * REV_CAR_MOVE_SUB;

					// �E�̕ǂɔ��킹��
					pCar->pos.x = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x + (CAR_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
				  // ���̕ǂɔ��킹��
					pCar->pos.z = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z - (CAR_WIDTH * 2);
				}
			}
		}
		else
		{ // ���ɑ����Ă���ꍇ
			if (pCar->carCurve.curveAngle[pCar->carCurve.nNowCurve] == CURVE_RIGHT)
			{//�E�ɋȂ���ꍇ
				if (pCar->pos.x <= pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x + (CAR_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

					// �Ԃ̊p�x�X�V�E�␳����
					CurveRotCar(&pCar->carCurve, &pCar->rot, &pCar->move);

					// �ړ��ʂ�����
					pCar->move.x += (0.0f - pCar->move.x) * REV_CAR_MOVE_SUB;

					// ���̕ǂɔ��킹��
					pCar->pos.x = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x + (CAR_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
					// ��O�̕ǂɔ��킹��
					pCar->pos.z = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z + (CAR_WIDTH * 2);
				}
			}
			else if (pCar->carCurve.curveAngle[pCar->carCurve.nNowCurve] == CURVE_LEFT)
			{//���ɋȂ���ꍇ
				if (pCar->pos.x <= pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x - (CAR_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

					// �Ԃ̊p�x�X�V�E�␳����
					CurveRotCar(&pCar->carCurve, &pCar->rot, &pCar->move);

					// �ړ��ʂ�����
					pCar->move.x += (0.0f - pCar->move.x) * REV_CAR_MOVE_SUB;

					// ���̕ǂɔ��킹��
					pCar->pos.x = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x - (CAR_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
					// ��O�̕ǂɔ��킹��
					pCar->pos.z = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z + (CAR_WIDTH * 2);
				}
			}
		}
	}
	else
	{//Z���𑖂��Ă����ꍇ
		if (pCar->carCurve.bCurvePlus[pCar->carCurve.nNowCurve] == true)
		{ // �E�̕ǂ��x�@��荶���ɂ���ꍇ
			if (pCar->carCurve.curveAngle[pCar->carCurve.nNowCurve] == CURVE_RIGHT)
			{//�E�ɋȂ���ꍇ
				if (pCar->pos.z >= pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z - (CAR_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

					// �Ԃ̊p�x�X�V�E�␳����
					CurveRotCar(&pCar->carCurve, &pCar->rot, &pCar->move);

					// �ړ��ʂ�����
					pCar->move.x += (0.0f - pCar->move.x) * REV_CAR_MOVE_SUB;

					// ��O�̕ǂɔ��킹��
					pCar->pos.z = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z - (CAR_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
					// �E�̕ǂɔ��킹��
					pCar->pos.x = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x + (CAR_WIDTH * 2);
				}
			}
			else if (pCar->carCurve.curveAngle[pCar->carCurve.nNowCurve] == CURVE_LEFT)
			{ // ���ɋȂ���ꍇ
				if (pCar->pos.z >= pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z + (CAR_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

					// �Ԃ̊p�x�X�V�E�␳����
					CurveRotCar(&pCar->carCurve, &pCar->rot, &pCar->move);

					// �ړ��ʂ�����
					pCar->move.x += (0.0f - pCar->move.x) * REV_CAR_MOVE_SUB;

					// ��O�̕ǂɔ��킹��
					pCar->pos.z = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z + (CAR_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
					// �E�̕ǂɔ��킹��
					pCar->pos.x = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x + (CAR_WIDTH * 2);
				}
			}
		}
		else
		{//���̕ǂ��x�@���E�ɂ���ꍇ
			if (pCar->carCurve.curveAngle[pCar->carCurve.nNowCurve] == CURVE_RIGHT)
			{//�E�ɋȂ���ꍇ
				if (pCar->pos.z <= pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z + (CAR_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

					// �Ԃ̊p�x�X�V�E�␳����
					CurveRotCar(&pCar->carCurve, &pCar->rot, &pCar->move);

					// �ړ��ʂ�����
					pCar->move.x += (0.0f - pCar->move.x) * REV_CAR_MOVE_SUB;

					// ���̕ǂɔ��킹��
					pCar->pos.z = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z + (CAR_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
				  // ���̕ǂɔ��킹��
					pCar->pos.x = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x - (CAR_WIDTH * 2);
				}
			}
			else if (pCar->carCurve.curveAngle[pCar->carCurve.nNowCurve] == CURVE_LEFT)
			{
				if (pCar->pos.z <= pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z - (CAR_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

					// �Ԃ̊p�x�X�V�E�␳����
					CurveRotCar(&pCar->carCurve, &pCar->rot, &pCar->move);

					// �ړ��ʂ�����
					pCar->move.x += (0.0f - pCar->move.x) * REV_CAR_MOVE_SUB;

					// ���̕ǂɔ��킹��
					pCar->pos.z = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].z - (CAR_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
				  // ���̕ǂɔ��킹��
					pCar->pos.x = pCar->carCurve.curvePoint[pCar->carCurve.nNowCurve].x - (CAR_WIDTH * 2);
				}
			}
		}
	}

	if (pCar->move.x > MAX_CAR_FORWARD_PATROL)
	{ // �v���C���[�̈ړ��� (x) �����l�ȏ�̏ꍇ

	  // �v���C���[�̈ړ��� (x) ��␳
		pCar->move.x = MAX_CAR_FORWARD_PATROL;
	}
}

//============================================================
// �Ԃ̒�~����
//============================================================
void CollisionStopCar(D3DXVECTOR3 targetpos, D3DXVECTOR3 targetrot, D3DXVECTOR3 *move, float fTargetRadius, COLLOBJECTTYPE collObject)
{
	D3DXVECTOR3 stopCarpos = D3DXVECTOR3(targetpos.x + sinf(targetrot.y) * 300.0f, 0.0f, targetpos.z + cosf(targetrot.y) * 300.0f);				// �~�܂�Ԃ̈ʒu

	float fLength;										// �����̕ϐ�

	{// �x�@�Ƃ̓����蔻��
		Police *pPolice = GetPoliceData();					// �x�@�̏����擾����

		for (int nCntPoli = 0; nCntPoli < MAX_POLICE; nCntPoli++, pPolice++)
		{
			if (pPolice->bUse == true)
			{ // �g�p���Ă���ꍇ
				// �����𑪂�
				fLength = (pPolice->pos.x - stopCarpos.x) * (pPolice->pos.x - stopCarpos.x)
					+ (pPolice->pos.z - stopCarpos.z) * (pPolice->pos.z - stopCarpos.z);

				if (fLength <= (pPolice->modelData.fRadius + 50.0f) * (fTargetRadius + 50.0f))
				{ // �I�u�W�F�N�g���������Ă���
					switch (collObject)
					{
					case COLLOBJECTTYPE_PLAYER:		//�v���C���[�̏ꍇ

						break;						//�����o��

					case COLLOBJECTTYPE_POLICE:		//�Ԃ̏ꍇ

						// �ڕW�̈ړ��ʂ��Z�[�u����
						move->x += (0.0f - move->x) * 0.5f;

						break;						//�����o��

					case COLLOBJECTTYPE_CAR:		//�Ԃ̏ꍇ

						// �ڕW�̈ړ��ʂ��Z�[�u����
						move->x = 0.0f;

						break;						//�����o��
					}
				}
			}
		}
	}

	{//�v���C���[�Ƃ̓����蔻��
		Player *pPlayer = GetPlayer();				//�v���C���[�̏����擾����

		if (pPlayer->bUse == true)
		{ // �g�p���Ă���ꍇ
		  // �����𑪂�
			fLength = (pPlayer->pos.x - stopCarpos.x) * (pPlayer->pos.x - stopCarpos.x)
				+ (pPlayer->pos.z - stopCarpos.z) * (pPlayer->pos.z - stopCarpos.z);

			if (fLength <= (pPlayer->modelData.fRadius + 50.0f) * (fTargetRadius + 50.0f))
			{ // �I�u�W�F�N�g���������Ă���

				switch (collObject)
				{
				case COLLOBJECTTYPE_PLAYER:		//�v���C���[�̏ꍇ

					// �ړ��ʂ�ݒ肷��
					move->x = sinf(targetrot.y) * -3.0f;

					break;						//�����o��

				case COLLOBJECTTYPE_POLICE:		//�Ԃ̏ꍇ

					//// �ڕW�̈ړ��ʂ��Z�[�u����
					//move->x += (0.0f - move->x) * 0.5f;

					break;						//�����o��

				case COLLOBJECTTYPE_CAR:		//�Ԃ̏ꍇ

					// �ڕW�̈ړ��ʂ��Z�[�u����
					move->x = 0.0f;

					break;						//�����o��
				}
			}
		}
	}

	{//�ԂƂ̓����蔻��
		Car *pCar = GetCarData();				//�Ԃ̏����擾����

		for (int nCntPoli = 0; nCntPoli < MAX_CAR; nCntPoli++, pCar++)
		{
			if (pCar->bUse == true)
			{ // �g�p���Ă���ꍇ
				// �����𑪂�
				fLength = (pCar->pos.x - stopCarpos.x) * (pCar->pos.x - stopCarpos.x)
					+ (pCar->pos.z - stopCarpos.z) * (pCar->pos.z - stopCarpos.z);

				if (fLength <= (pCar->modelData.fRadius + 50.0f) * (fTargetRadius + 50.0f))
				{ // �I�u�W�F�N�g���������Ă���
					switch (collObject)
					{
					case COLLOBJECTTYPE_PLAYER:		//�v���C���[�̏ꍇ

						// �ړ��ʂ�ݒ肷��
						move->x = sinf(targetrot.y) * -3.0f;

						break;						//�����o��

					case COLLOBJECTTYPE_POLICE:		//�Ԃ̏ꍇ

						// �ڕW�̈ړ��ʂ��Z�[�u����
						move->x += (0.0f - move->x) * 0.5f;

						break;						//�����o��

					case COLLOBJECTTYPE_CAR:		//�Ԃ̏ꍇ

						// �ڕW�̈ړ��ʂ��Z�[�u����
						move->x = 0.0f;

						break;						//�����o��
					}
				}
			}
		}
	}
}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================
#endif