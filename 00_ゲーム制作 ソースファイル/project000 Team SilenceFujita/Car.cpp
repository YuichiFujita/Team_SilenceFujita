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
#include "calculation.h"

#include "Car.h"
#include "shadow.h"
#include "sound.h"
#include "player.h"
#include "Police.h"
#include "object.h"
#include "meshfield.h"

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
#define CAR_NOTMOVE_SLOW		(0.04f)		// �Ԃ̈ړ����Ă��Ȃ��Ƃ��̌����W��
#define CAR_STOP_RADIUS_DIST	(300.0f)	// �Ԃ̎~�܂�w�W�̉~�̂��炷����
#define CAR_STOP_ADD_RADIUS		(50.0f)		// �Ԃ̎~�܂锼�a�̒ǉ���

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void PosCar(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove);		// �Ԃ̈ʒu�̍X�V����
void RevCar(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos);	// �Ԃ̕␳�̍X�V����
void CurveCar(Car *pCar);							// �Ԃ̃J�[�u����
void DashCarAction(Car *pCar);						// �Ԃ̑��s����
void SetCarPosRot(Car *pCar);						// �Ԃ̈ʒu�ƌ����̐ݒ菈��
void CarPosRotCorrect(Car *pCar);					// �Ԃ̈ʒu�̕␳����

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

		g_aCar[nCntCar].pos       = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aCar[nCntCar].posOld    = g_aCar[nCntCar].pos;			// �O��̈ʒu
		g_aCar[nCntCar].move      = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
		g_aCar[nCntCar].rot       = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aCar[nCntCar].nShadowID = NONE_SHADOW;					// �e�̃C���f�b�N�X
		g_aCar[nCntCar].bombState = BOMBSTATE_NONE;					// �{���̏��
		g_aCar[nCntCar].bJump     = false;							// �W�����v���Ă��邩�ǂ���
		g_aCar[nCntCar].bMove     = false;							// �ړ����Ă��邩
		g_aCar[nCntCar].bUse      = false;							// �g�p��

		//�Ȃ���p�̏��̏�����
		g_aCar[nCntCar].carCurveInfo.curveInfo.curveAngle   = CURVE_LEFT;	// ���ɋȂ���
		g_aCar[nCntCar].carCurveInfo.curveInfo.nCurveNumber = 0;			// �J�[�u����Ԓn
		g_aCar[nCntCar].carCurveInfo.nSKipCnt    = 0;						// �X�L�b�v����Ȃ���p�̉�
		g_aCar[nCntCar].carCurveInfo.rotDest     = g_aCar[nCntCar].rot;		// �O��̌���
		g_aCar[nCntCar].carCurveInfo.actionState = CARACT_DASH;				// ���݂̎Ԃ̍s��

		// ���f�����̏�����
		g_aCar[nCntCar].modelData.dwNumMat = 0;						// �}�e���A���̐�
		g_aCar[nCntCar].modelData.pTexture = NULL;					// �e�N�X�`���ւ̃|�C���^
		g_aCar[nCntCar].modelData.pMesh    = NULL;					// ���b�V�� (���_���) �ւ̃|�C���^
		g_aCar[nCntCar].modelData.pBuffMat = NULL;					// �}�e���A���ւ̃|�C���^
		g_aCar[nCntCar].modelData.vtxMin   = INIT_VTX_MIN;			// �ŏ��̒��_���W
		g_aCar[nCntCar].modelData.vtxMax   = INIT_VTX_MAX;			// �ő�̒��_���W
		g_aCar[nCntCar].modelData.size     = INIT_SIZE;				// �傫��
		g_aCar[nCntCar].modelData.fRadius  = 0.0f;					// ���a
	}

	//�Ԃ̐ݒ菈��
	SetCar(D3DXVECTOR3(-3000.0f, 0.0f, 3000.0f));
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

			// �I�u�W�F�N�g�̒��n�̍X�V����
			LandObject(&g_aCar[nCntCar].pos, &g_aCar[nCntCar].move, &g_aCar[nCntCar].bJump);

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

			// �v���C���[�̈ʒu�̍X�V
			PosCar(&g_aCar[nCntCar].move, &g_aCar[nCntCar].pos, &g_aCar[nCntCar].rot, g_aCar[nCntCar].bMove);

			//�Ԃ̃J�[�u����
			CurveCar(&g_aCar[nCntCar]);

			if (g_aCar[nCntCar].pos.y < 0.0f)
			{//Y���̈ʒu��0.0f�������ꍇ
				//�c�ւ̈ړ��ʂ�0.0f�ɂ���
				g_aCar[nCntCar].move.y = 0.0f;

				//�ʒu��0.0f�ɖ߂�
				g_aCar[nCntCar].pos.y = 0.0f;
			}

			//----------------------------------------------------
			//	�����蔻��
			//----------------------------------------------------
			// �I�u�W�F�N�g�Ƃ̓����蔻��
			CollisionObject
			( // ����
				&g_aCar[nCntCar].pos,		// ���݂̈ʒu
				&g_aCar[nCntCar].posOld,	// �O��̈ʒu
				&g_aCar[nCntCar].move,		// �ړ���
				CAR_WIDTH,					// ����
				CAR_DEPTH					// ���s
			);

			// �Ԃ̒�~����
			CollisionStopCar
			( // ����
				g_aCar[nCntCar].pos,		//�ʒu
				g_aCar[nCntCar].rot,		//����
				&g_aCar[nCntCar].move,		//�ړ���
				g_aCar[nCntCar].modelData.fRadius,	//���a
				COLLOBJECTTYPE_CAR			//�Ώۂ̃T�C�Y
			);

			// �ԓ��m�̓����蔻��
			CollisionCarBody
			( // ����
				&g_aCar[nCntCar].pos, 
				&g_aCar[nCntCar].posOld,
				g_aCar[nCntCar].rot,
				&g_aCar[nCntCar].move,
				CAR_WIDTH,
				CAR_DEPTH,
				COLLOBJECTTYPE_CAR
			);

			// �Ԃ̕␳�̍X�V����
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
	Player           *pPlayer = GetPlayer();	// �v���C���[�̏��
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL      bombMat;					// �}�e���A���f�[�^ (�{���p)

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

				if (pPlayer->atkState == ATTACKSTATE_BOMB)
				{ // �U����Ԃ��{���U����Ԃ̏ꍇ

					// �\���̗̂v�f���N���A
					ZeroMemory(&bombMat, sizeof(D3DXMATERIAL));

					switch (g_aCar[nCntCar].bombState)
					{ // �{���̏��
					case BOMBSTATE_RANGE:	// �͈͓����

						// �͈͓����̃}�e���A���̐F��ݒ�
						bombMat.MatD3D.Diffuse = BOMB_RANGE_COL;

						// �����𔲂���
						break;

					case BOMBSTATE_AIM:		// �_�����

						// �_�����̃}�e���A���̐F��ݒ�
						bombMat.MatD3D.Diffuse = BOMB_AIM_COL;

						// �����𔲂���
						break;

					default:				// ��L�ȊO

						// �͈͊O���̃}�e���A���̐F��ݒ�
						bombMat.MatD3D.Diffuse = BOMB_NONE_COL;

						// �����𔲂���
						break;
					}

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&bombMat.MatD3D);
				}
				else
				{ // �U����Ԃ�����ȊO�̏�Ԃ̏ꍇ

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				}

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
void SetCar(D3DXVECTOR3 pos)
{
	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aCar[nCntCar].bUse == false)
		{ // �I�u�W�F�N�g���g�p����Ă��Ȃ��ꍇ
			// ��������
			g_aCar[nCntCar].pos       = pos;							// ���݂̈ʒu
			g_aCar[nCntCar].posOld    = g_aCar[nCntCar].pos;			// �O��̈ʒu
			g_aCar[nCntCar].move      = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
			g_aCar[nCntCar].bombState = BOMBSTATE_NONE;					// �������Ă��Ȃ���Ԃɂ���
			g_aCar[nCntCar].bJump     = false;							// �W�����v���Ă��邩�ǂ���
			g_aCar[nCntCar].bMove     = false;							// �ړ����Ă��Ȃ�

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

			// �Ԃ̈ʒu�ƌ����̐ݒ菈��
			SetCarPosRot(&g_aCar[nCntCar]);
			g_aCar[nCntCar].carCurveInfo.nSKipCnt = 0;																					// �X�L�b�v����Ȃ���p�̉�
			g_aCar[nCntCar].carCurveInfo.rotDest = g_aCar[nCntCar].rot;																	// �O��̌���
			g_aCar[nCntCar].carCurveInfo.actionState = CARACT_DASH;																		// �����Ă�����

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
		move->x += (0.0f - move->x) * CAR_NOTMOVE_SLOW;
	}
}

//============================================================
//	�v���C���[�̕␳�̍X�V����
//============================================================
void RevCar(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos)
{
	// �����̐��K��
	RotNormalize(rot->y);

	//--------------------------------------------------------
	//	�ړ��͈͂̕␳
	//--------------------------------------------------------
	if (pos->z > GetLimitStage().fNear - (CAR_DEPTH * 2))
	{ // �͈͊O�̏ꍇ (��O)

		// ��O�Ɉʒu��␳
		pos->z = GetLimitStage().fNear - (CAR_DEPTH * 2);
	}
	if (pos->z < GetLimitStage().fFar + (CAR_DEPTH * 2))
	{ // �͈͊O�̏ꍇ (��)

		// ���Ɉʒu��␳
		pos->z = GetLimitStage().fFar + (CAR_DEPTH * 2);
	}
	if (pos->x > GetLimitStage().fRight - (CAR_WIDTH * 2))
	{ // �͈͊O�̏ꍇ (�E)

		// �E�Ɉʒu��␳
		pos->x = GetLimitStage().fRight - (CAR_WIDTH * 2);
	}
	if (pos->x < GetLimitStage().fLeft + (CAR_WIDTH * 2))
	{ // �͈͊O�̏ꍇ (��)

		// ���Ɉʒu��␳
		pos->x = GetLimitStage().fLeft + (CAR_WIDTH * 2);
	}
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

	switch (pCar->carCurveInfo.actionState)
	{
	case CARACT_DASH:		// ���s���

		// �Ԃ̑��s����
		DashCarAction(pCar);

		break;				// �����o��

	case CARACT_CURVE:		// �J�[�u���

		// �Ԃ̊p�x�X�V�E�␳����
		CurveInfoRotCar(&pCar->carCurveInfo, &pCar->rot, &pCar->move, &pCar->pos);

		break;				// �����o��
	}

	if (pCar->move.x > MAX_CAR_FORWARD_PATROL)
	{ // �v���C���[�̈ړ��� (x) �����l�ȏ�̏ꍇ

	  // �v���C���[�̈ړ��� (x) ��␳
		pCar->move.x = MAX_CAR_FORWARD_PATROL;
	}
}

//============================================================
// �Ԃ̑��s����
//============================================================
void DashCarAction(Car *pCar)
{
	for (int nCnt = 0; nCnt < MAX_CURVEPOINT; nCnt++)
	{
		switch (pCar->carCurveInfo.curveInfo.dashAngle)
		{
		case DASH_RIGHT:		//�E�ɑ����Ă���ꍇ

			//���킹��
			pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->pos.z == GetCurveInfo(nCnt).pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
			{ // ����Z����𑖂��Ă���ꍇ
				if (pCar->pos.x >= GetCurveInfo(nCnt).pos.x - (CAR_WIDTH * 2) &&
					pCar->posOld.x <= GetCurveInfo(nCnt).pos.x - (CAR_WIDTH * 2))
				{ // �ʒu����v�����ꍇ
					if (GetCurveInfo(nCnt).dashAngle == DASH_RIGHT)
					{ // �E�ɑ���ꍇ�̂�
						// �X�L�b�v�J�E���g�����Z����
						pCar->carCurveInfo.nSKipCnt--;

						if (pCar->carCurveInfo.nSKipCnt == 0 || GetCurveInfo(nCnt).bDeadEnd == true)
						{ // �X�L�b�v�񐔂�0�ɂȂ����܂��́A�s���~�܂肾�����ꍇ
							// �X�L�b�v�񐔂�0�ɂ���
							pCar->carCurveInfo.nSKipCnt = 0;

							// �Ȃ���p�̏����X�V����
							pCar->carCurveInfo.curveInfo = GetCurveInfo(nCnt);
						}
					}
				}
			}

			break;				//�����o��

		case DASH_LEFT:			//���𑖂��Ă���ꍇ

			//���킹��
			pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->pos.z == GetCurveInfo(nCnt).pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
			{ // ����Z����𑖂��Ă���ꍇ
				if (pCar->pos.x <= GetCurveInfo(nCnt).pos.x + (CAR_WIDTH * 2) &&
					pCar->posOld.x >= GetCurveInfo(nCnt).pos.x + (CAR_WIDTH * 2))
				{ // �ʒu����v�����ꍇ
					if (GetCurveInfo(nCnt).dashAngle == DASH_LEFT)
					{ // ���ɑ���ꍇ�̂�
						// �X�L�b�v�J�E���g�����Z����
						pCar->carCurveInfo.nSKipCnt--;

						if (pCar->carCurveInfo.nSKipCnt == 0 || GetCurveInfo(nCnt).bDeadEnd == true)
						{ // �X�L�b�v�񐔂�0�ɂȂ����ꍇ
							// �X�L�b�v�񐔂�0�ɂ���
							pCar->carCurveInfo.nSKipCnt = 0;

							// �Ȃ���p�̏����X�V����
							pCar->carCurveInfo.curveInfo = GetCurveInfo(nCnt);
						}
					}
				}
			}

			break;				//�����o��

		case DASH_FAR:			//���ɑ����Ă���ꍇ

			//���킹��
			pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->pos.x == GetCurveInfo(nCnt).pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
			{ // ����Z����𑖂��Ă���ꍇ
				if (pCar->pos.z >= GetCurveInfo(nCnt).pos.z - (CAR_WIDTH * 2) &&
					pCar->posOld.z <= GetCurveInfo(nCnt).pos.z - (CAR_WIDTH * 2))
				{ // �ʒu����v�����ꍇ
					if (GetCurveInfo(nCnt).dashAngle == DASH_FAR)
					{ // ���ɑ���ꍇ�̂�
						// �X�L�b�v�J�E���g�����Z����
						pCar->carCurveInfo.nSKipCnt--;

						if (pCar->carCurveInfo.nSKipCnt == 0 || GetCurveInfo(nCnt).bDeadEnd == true)
						{ // �X�L�b�v�񐔂�0�ɂȂ����ꍇ
							// �X�L�b�v�񐔂�0�ɂ���
							pCar->carCurveInfo.nSKipCnt = 0;

							// �Ȃ���p�̏����X�V����
							pCar->carCurveInfo.curveInfo = GetCurveInfo(nCnt);
						}
					}
				}
			}

			break;				//�����o��

		case DASH_NEAR:			//���ɑ����Ă���ꍇ

			//���킹��
			pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->pos.x == GetCurveInfo(nCnt).pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
			{ // ����Z����𑖂��Ă���ꍇ
				if (pCar->pos.z <= GetCurveInfo(nCnt).pos.z + (CAR_WIDTH * 2) &&
					pCar->posOld.z >= GetCurveInfo(nCnt).pos.z + (CAR_WIDTH * 2))
				{ // �ʒu����v�����ꍇ
					if (GetCurveInfo(nCnt).dashAngle == DASH_NEAR)
					{ // ��O�ɑ���ꍇ�̂�
						// �X�L�b�v�J�E���g�����Z����
						pCar->carCurveInfo.nSKipCnt--;

						if (pCar->carCurveInfo.nSKipCnt == 0 || GetCurveInfo(nCnt).bDeadEnd == true)
						{ // �X�L�b�v�񐔂�0�ɂȂ����ꍇ
							// �X�L�b�v�񐔂�0�ɂ���
							pCar->carCurveInfo.nSKipCnt = 0;

							// �Ȃ���p�̏����X�V����
							pCar->carCurveInfo.curveInfo = GetCurveInfo(nCnt);

							if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_LEFT)
							{ // �Ȃ���������������������ꍇ
							  // �p�x��␳����
								pCar->rot.y = D3DX_PI;
							}
							else
							{ // �Ȃ���������E�����������ꍇ
							  // �p�x��␳����
								pCar->rot.y = -D3DX_PI;
							}
						}
					}
				}
			}

			break;				//�����o��
		}
	}

	if (pCar->carCurveInfo.nSKipCnt == 0)
	{ // �X�L�b�v�J�E���g��0�̏ꍇ
		switch (pCar->carCurveInfo.curveInfo.dashAngle)
		{
		case DASH_RIGHT:			//�E�ɑ����Ă���

			//���킹��
			pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_RIGHT)
			{ // �E�ɋȂ���ꍇ
				if (pCar->pos.x >= pCar->carCurveInfo.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
					// �ʒu��␳����
					pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}
			else if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_LEFT)
			{ // ���ɋȂ���ꍇ
				if (pCar->pos.x >= pCar->carCurveInfo.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
					// �ʒu��␳����
					pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}

			break;					//�����o��

		case DASH_LEFT:				//���ɑ����Ă���

			//���킹��
			pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_RIGHT)
			{ // �E�ɋȂ���ꍇ
				if (pCar->pos.x <= pCar->carCurveInfo.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
					// �ʒu��␳����
					pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}
			else if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_LEFT)
			{ // ���ɋȂ���ꍇ
				if (pCar->pos.x <= pCar->carCurveInfo.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
					// �ʒu��␳����
					pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}

			break;					//�����o��

		case DASH_FAR:				//���ɑ����Ă���

			//���킹��
			pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_RIGHT)
			{ // �E�ɋȂ���ꍇ
				if (pCar->pos.z >= pCar->carCurveInfo.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
					// �ʒu��␳����
					pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}
			else if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_LEFT)
			{ // ���ɋȂ���ꍇ
				if (pCar->pos.z >= pCar->carCurveInfo.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
					// �ʒu��␳����
					pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}

			break;					//�����o��

		case DASH_NEAR:				//��O�ɑ����Ă���

			//���킹��
			pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_RIGHT)
			{ // �E�ɋȂ���ꍇ
				if (pCar->pos.z <= pCar->carCurveInfo.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
					// �ʒu��␳����
					pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}
			else if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_LEFT)
			{ // ���ɋȂ���ꍇ
				if (pCar->pos.z <= pCar->carCurveInfo.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
					// �ʒu��␳����
					pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}

			break;					//�����o��
		}
	}
}

//============================================================
// �Ԃ̒�~����
//============================================================
void CollisionStopCar(D3DXVECTOR3 targetpos, D3DXVECTOR3 targetrot, D3DXVECTOR3 *move, float fTargetRadius, COLLOBJECTTYPE collObject)
{
	D3DXVECTOR3 stopCarpos =
		D3DXVECTOR3(
			targetpos.x + sinf(targetrot.y) * CAR_STOP_RADIUS_DIST,
			0.0f,
			targetpos.z + cosf(targetrot.y) * CAR_STOP_RADIUS_DIST
		);				// �~�܂�Ԃ̈ʒu

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

				if (fLength <= (pPolice->modelData.fRadius + CAR_STOP_ADD_RADIUS) * (fTargetRadius + CAR_STOP_ADD_RADIUS))
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

			if (fLength <= (pPlayer->modelData.fRadius + CAR_STOP_ADD_RADIUS) * (fTargetRadius + CAR_STOP_ADD_RADIUS))
			{ // �I�u�W�F�N�g���������Ă���

				switch (collObject)
				{
				case COLLOBJECTTYPE_PLAYER:		//�v���C���[�̏ꍇ

					break;						//�����o��

				case COLLOBJECTTYPE_POLICE:		//�Ԃ̏ꍇ

					// �ڕW�̈ړ��ʂ��Z�[�u����
					move->x = move->x;

					break;						//�����o��

				case COLLOBJECTTYPE_CAR:		//�Ԃ̏ꍇ

					// �ڕW�̈ړ��ʂ��Z�[�u����
					move->x = sinf(targetrot.y) * -8.0f;

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

				if (fLength <= (pCar->modelData.fRadius + CAR_STOP_ADD_RADIUS) * (fTargetRadius + CAR_STOP_ADD_RADIUS))
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
}

//============================================================
// �ԓ��m�̓����蔻��
//============================================================
void CollisionCarBody(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 rot, D3DXVECTOR3 *pMove, float fWidth, float fDepth, COLLOBJECTTYPE collObject)
{
	//{ // �Ԃ̓����蔻��
	//	Car *pCar = GetCarData();

	//	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
	//	{
	//		// �O��̓����蔻��
	//		if (pPos->x + fWidth > pCar[nCntCar].pos.x - CAR_WIDTH
	//			&&  pPosOld->x - fWidth < pCar[nCntCar].posOld.x + CAR_WIDTH)
	//		{ // �Ԃ̍��E�͈͓̔��̏ꍇ

	//			if (pPos->z + fDepth > pCar[nCntCar].pos.z - CAR_DEPTH
	//				&&  pPosOld->z + fDepth <= pCar[nCntCar].pos.z - CAR_DEPTH)
	//			{ // �O����̓����蔻��

	//				// �ʒu��␳
	//				pPos->z = pCar[nCntCar].pos.z + (fDepth + CAR_DEPTH) + 0.01f;

	//				switch (collObject)
	//				{
	//				case COLLOBJECTTYPE_PLAYER:	// �v���C���[�̏ꍇ

	//					// �ړ��ʂ��폜
	//					pMove->x *= 0.95f;

	//					break;					// �����o��

	//				case COLLOBJECTTYPE_CAR:	// �Ԃ̏ꍇ

	//					// �ړ��ʂ��폜
	//					pMove->x *= 0.95f;

	//					break;					// �����o��
	//				}
	//			}
	//			else if (pPos->z - fDepth < pCar[nCntCar].pos.z + CAR_DEPTH
	//				&&  pPosOld->z - fDepth >= pCar[nCntCar].pos.z + CAR_DEPTH)
	//			{ // ��납��̓����蔻��

	//				// �ʒu��␳
	//				pPos->z = pCar[nCntCar].pos.z - (fDepth + CAR_DEPTH) - 0.01f;

	//				switch (collObject)
	//				{
	//				case COLLOBJECTTYPE_PLAYER:	// �v���C���[�̏ꍇ

	//					// �ړ��ʂ��폜
	//					pMove->x *= 0.95f;

	//					break;					// �����o��

	//				case COLLOBJECTTYPE_CAR:	// �Ԃ̏ꍇ

	//					// �ړ��ʂ��폜
	//					pMove->x *= 0.95f;

	//					break;					// �����o��
	//				}
	//			}
	//		}

	//		// ���E�̓����蔻��
	//		if (pPos->z + fDepth > pCar[nCntCar].pos.z - CAR_DEPTH
	//			&&  pPosOld->z - fDepth < pCar[nCntCar].pos.z + CAR_DEPTH)
	//		{ // �u���b�N�̑O��͈͓̔��̏ꍇ

	//			if (pPos->x + fWidth > pCar[nCntCar].pos.x - CAR_WIDTH
	//				&&  pPosOld->x + fWidth <= pCar[nCntCar].pos.x - CAR_WIDTH)
	//			{ // ������̓����蔻��

	//				// �ʒu��␳
	//				pPos->x = pCar[nCntCar].pos.x - (fWidth + CAR_WIDTH) - 0.01f;

	//				switch (collObject)
	//				{
	//				case COLLOBJECTTYPE_PLAYER:	// �v���C���[�̏ꍇ

	//					// �ړ��ʂ��폜
	//					pMove->x *= 0.95f;

	//					break;					// �����o��

	//				case COLLOBJECTTYPE_CAR:	// �Ԃ̏ꍇ

	//					// �ړ��ʂ��폜
	//					pMove->x *= 0.95f;

	//					break;					// �����o��
	//				}
	//			}
	//			else if (pPos->x - fWidth < pCar[nCntCar].pos.x + CAR_WIDTH
	//				&&  pPosOld->x - fWidth >= pCar[nCntCar].pos.x + CAR_WIDTH)
	//			{ // �E����̓����蔻��

	//				// �ʒu��␳
	//				pPos->x = pCar[nCntCar].pos.x + (fWidth + CAR_WIDTH) + 0.01f;

	//				switch (collObject)
	//				{
	//				case COLLOBJECTTYPE_PLAYER:	// �v���C���[�̏ꍇ

	//					// �ړ��ʂ��폜
	//					pMove->x *= 0.95f;

	//					break;					// �����o��

	//				case COLLOBJECTTYPE_CAR:	// �Ԃ̏ꍇ

	//					// �ړ��ʂ��폜
	//					pMove->x *= 0.95f;

	//					break;					// �����o��
	//				}
	//			}
	//		}
	//	}
	//}
}

//============================================================
// �Ԃ̈ʒu�ƌ����̐ݒ菈��
//============================================================
void SetCarPosRot(Car *pCar)
{
	float fCurveDist;			// �ŋ߂̋Ȃ���p�Ƃ̋���
	int nCurveNumber = 0;		// �ŋ߂̋Ȃ���p�̔ԍ�

	fCurveDist = fabsf(sqrtf((pCar->pos.x - GetCurveInfo(0).pos.x) * (pCar->pos.x - GetCurveInfo(0).pos.x) +
		(pCar->pos.z - GetCurveInfo(0).pos.z) * (pCar->pos.z - GetCurveInfo(0).pos.z)));

	for (int nCnt = 1; nCnt < MAX_CURVEPOINT; nCnt++)
	{ // �S�Ă̋Ȃ���p�ƎQ��

		float fCurvePoint;		// �Ȃ���p�̒l

		// �J�[�u�̋���
		fCurvePoint = fabsf(sqrtf((pCar->pos.x - GetCurveInfo(nCnt).pos.x) * (pCar->pos.x - GetCurveInfo(nCnt).pos.x) +
			(pCar->pos.z - GetCurveInfo(nCnt).pos.z) * (pCar->pos.z - GetCurveInfo(nCnt).pos.z)));

		if (fCurvePoint <= fCurveDist)
		{ // �����̋߂����X�V���ꂽ�ꍇ
			// �ŋߒl���X�V����
			fCurveDist = fCurvePoint;

			// �ԍ����X�V����
			nCurveNumber = nCnt;
		}
	}

	// �Ȃ���p�̏���������
	pCar->carCurveInfo.curveInfo = GetCurveInfo(nCurveNumber);

	// �Ԃ̈ʒu�̕␳����
	CarPosRotCorrect(pCar);
}

//============================================================
// �Ԃ̈ʒu�̕␳����
//============================================================
void CarPosRotCorrect(Car *pCar)
{
	switch (pCar->carCurveInfo.curveInfo.dashAngle)
	{
	case DASH_RIGHT:		// �E�Ɍ������đ���

		//���킹��
		pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

		// ������ς���
		pCar->rot.y = D3DX_PI * 0.5f;

		break;				// �����o��

	case DASH_LEFT:			// ���Ɍ������đ���

		//���킹��
		pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

		// ������ς���
		pCar->rot.y = -D3DX_PI * 0.5f;

		break;				// �����o��

	case DASH_FAR:			// ���Ɍ������đ���

		//���킹��
		pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

		// ������ς���
		pCar->rot.y = 0.0f;

		break;				// �����o��

	case DASH_NEAR:			// ��O�Ɍ������đ���

		//���킹��
		pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

		switch (pCar->carCurveInfo.curveInfo.curveAngle)
		{
		case CURVE_RIGHT:	// �E�ɋȂ���

			// ������ς���
			pCar->rot.y = -D3DX_PI;

			break;			// �����o��

		case CURVE_LEFT:	// ���ɋȂ���

			// ������ς���
			pCar->rot.y = D3DX_PI;

			break;			// �����o��
		}

		break;				// �����o��
	}
}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================
#endif