//======================================================================================================================
//
//	�l�Ԃ̏��� [Human.cpp]
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

#include "Human.h"
#include "shadow.h"
#include "sound.h"
#include "player.h"
#include "Police.h"
#include "curve.h"
#include "object.h"
#include "wind.h"
#include "Car.h"

#ifdef _DEBUG	// �f�o�b�O����
#include "game.h"
#endif

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define HUMAN_LIFE					(50)		// �l�̗̑�
#define HUMAN_GRAVITY				(1.0f)		// �l�ɂ�����d��
#define HUMAN_MOVE_FORWARD			(0.1f)		// �l�O�i���̈ړ���
#define HUMAN_MOVE_BACKWARD			(0.2f)		// �l��ގ��̈ړ���
#define HUMAN_MOVE_ROT				(0.012f)	// �l�̌����ύX��
#define REV_HUMAN_MOVE_ROT			(0.1f)		// �ړ��ʂɂ������ύX�ʂ̕␳�W��
#define SUB_HUMAN_MOVE_VALUE		(15.0f)		// �����ύX���̌������s����ړ���
#define SUB_HUMAN_MOVE				(0.05f)		// �����ύX���̌�����
#define MAX_HUMAN_BACKWARD			(8.0f)		// ��ގ��̍ō����x
#define REV_HUMAN_MOVE_SUB			(0.04f)		// �ړ��ʂ̌����W��
#define HUMAN_CURVE_ADD				(0.03f)		// �Ȃ���p�ł̌����̉��Z��
#define HUMAN_RANDAM_MOVE			(6)			// �l�Ԃ̈ړ��ʂ̃����_��
#define HUMAN_MOVE_LEAST			(4)			// �l�Ԃ̈ړ��ʂ̍Œ��
#define HUMAN_PASS_SHIFT			(40.0f)		// �����������̂��炷��
#define HUMAN_RADIUS				(30.0f)		// �l�Ԃ̕�
#define HUMAN_PASS_CORRECT			(0.06f)		// �l�Ԃ̂��炷�␳�{��
#define REACTION_HUMAN_RANGE		(170.0f)	// ���A�N�V��������l�Ԃ͈̔�
#define REACTION_CAR_RANGE			(50.0f)		// ���A�N�V��������Ԃ͈̔�

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void PosHuman(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove, float fMaxMove);		// �l�Ԃ̈ʒu�̍X�V����
void RevHuman(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos);	// �l�Ԃ̕␳�̍X�V����
void CurveHuman(Human *pHuman);						// �l�Ԃ̃J�[�u����
void StopHuman(Human *pHuman);						// �l�Ԃ̒�~����
void ReactionHuman(Human *pHuman);					// �l�Ԃ̃��A�N�V��������
void CollisionCarHuman(Human *pHuman);				// �l�ԂƎԂ̓����蔻��
void CurveRotHuman(Human *pHuman);					// �l�Ԃ̊p�x�X�V����
void WalkHuman(Human *pHuman);						// �l�Ԃ̕�������
void PassingHuman(Human *pHuman);					// �l�Ԃ̂���Ⴂ����

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
Human g_aHuman[MAX_HUMAN];		// �l�Ԃ̏��

//======================================================================================================================
//	�l�Ԃ̏���������
//======================================================================================================================
void InitHuman(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// �l�Ԃ̏��̏�����
	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		g_aHuman[nCntHuman].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aHuman[nCntHuman].posOld = g_aHuman[nCntHuman].pos;		// �O��̈ʒu
		g_aHuman[nCntHuman].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
		g_aHuman[nCntHuman].fMaxMove = 0.0f;						// �ړ��ʂ̍ő吔
		g_aHuman[nCntHuman].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aHuman[nCntHuman].nShadowID = NONE_SHADOW;				// �e�̃C���f�b�N�X
		g_aHuman[nCntHuman].bJump = false;							// �W�����v���Ă��邩�ǂ���
		g_aHuman[nCntHuman].bMove = false;							// �ړ����Ă��邩
		g_aHuman[nCntHuman].bUse = false;							// �g�p��
		g_aHuman[nCntHuman].state = HUMANSTATE_WALK;				// ���

		// ���f�����̏�����
		g_aHuman[nCntHuman].modelData.dwNumMat = 0;					// �}�e���A���̐�
		g_aHuman[nCntHuman].modelData.pTexture = NULL;				// �e�N�X�`���ւ̃|�C���^
		g_aHuman[nCntHuman].modelData.pMesh = NULL;					// ���b�V�� (���_���) �ւ̃|�C���^
		g_aHuman[nCntHuman].modelData.pBuffMat = NULL;				// �}�e���A���ւ̃|�C���^
		g_aHuman[nCntHuman].modelData.vtxMin = INIT_VTX_MIN;		// �ŏ��̒��_���W
		g_aHuman[nCntHuman].modelData.vtxMax = INIT_VTX_MAX;		// �ő�̒��_���W
		g_aHuman[nCntHuman].modelData.size = INIT_SIZE;				// �c��
		g_aHuman[nCntHuman].modelData.fRadius = 0.0f;				// ���a

		// �Ȃ���p�֌W�̏�����
		g_aHuman[nCntHuman].curveInfo.actionState = HUMANACT_WALK;	// ���s���
		g_aHuman[nCntHuman].curveInfo.nRandamRoute = 0;				// �i�ރ��[�g�̎��
		g_aHuman[nCntHuman].curveInfo.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ڕW�̈ʒu
		g_aHuman[nCntHuman].curveInfo.curveInfo.nCurveTime = 0;		// �Ȃ���p�̐�
		g_aHuman[nCntHuman].curveInfo.curveInfo.nNowCurve = 0;		// ���݂̋Ȃ���p

		for (int nCntCur = 0; nCntCur < MAX_HUMAN_CURVE; nCntCur++)
		{
			g_aHuman[nCntHuman].curveInfo.curveInfo.curvePoint[nCntCur] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �Ȃ���p�̈ʒu
			g_aHuman[nCntHuman].curveInfo.curveInfo.curveAngle[nCntCur] = CURVE_RIGHT;							// �E�ɋȂ���
			g_aHuman[nCntHuman].curveInfo.curveInfo.dashAngle[nCntCur]  = DASH_RIGHT;							// �E�Ɍ������đ����Ă���
		}
	}
}

//======================================================================================================================
//	�l�Ԃ̏I������
//======================================================================================================================
void UninitHuman(void)
{

}

//======================================================================================================================
//	�l�Ԃ̍X�V����
//======================================================================================================================
void UpdateHuman(void)
{
	int nCnt = 0;

	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�
		if (g_aHuman[nCntHuman].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// �O��ʒu�̍X�V
			g_aHuman[nCntHuman].posOld = g_aHuman[nCntHuman].pos;

			// �I�u�W�F�N�g�̒��n�̍X�V����
			LandObject(&g_aHuman[nCntHuman].pos, &g_aHuman[nCntHuman].move, &g_aHuman[nCntHuman].bJump);

			//----------------------------------------------------
			//	�e�̍X�V
			//----------------------------------------------------
			// �e�̈ʒu�ݒ�
			SetPositionShadow
			( // ����
				g_aHuman[nCntHuman].nShadowID,		// �e�̃C���f�b�N�X
				g_aHuman[nCntHuman].pos,			// �ʒu
				g_aHuman[nCntHuman].rot,			// ����
				NONE_SCALE							// �g�嗦
			);

			// �v���C���[�̈ʒu�̍X�V
			PosHuman
			(
				&g_aHuman[nCntHuman].move,			// �ړ���
				&g_aHuman[nCntHuman].pos,			// �ʒu
				&g_aHuman[nCntHuman].rot,			// ����
				g_aHuman[nCntHuman].bMove,			// �ړ����Ă��邩
				g_aHuman[nCntHuman].fMaxMove		// �ړ��ʂ̍ő吔
			);

			//�l�Ԃ̃��A�N�V��������
			ReactionHuman(&g_aHuman[nCntHuman]);

			// ���̓����蔻��
			CollisionWind(&g_aHuman[nCntHuman]);

			switch (g_aHuman[nCntHuman].state)
			{
			case HUMANSTATE_WALK:		//�������

				//�l�Ԃ̃J�[�u����
				CurveHuman(&g_aHuman[nCntHuman]);

				if (g_aHuman[nCntHuman].pos.y < 0.0f)
				{//Y���̈ʒu��0.0f�������ꍇ
					//�c�ւ̈ړ��ʂ�0.0f�ɂ���
					g_aHuman[nCntHuman].move.y = 0.0f;

					//�ʒu��0.0f�ɖ߂�
					g_aHuman[nCntHuman].pos.y = 0.0f;
				}

				break;					//�����o��

			case HUMANSTATE_STOP:		//�~�܂������

				// �l�Ԃ̒�~����
				StopHuman(&g_aHuman[nCntHuman]);

				if (g_aHuman[nCntHuman].pos.y < 0.0f)
				{//Y���̈ʒu��0.0f�������ꍇ
					//�c�ւ̈ړ��ʂ�0.0f�ɂ���
					g_aHuman[nCntHuman].move.y = 0.0f;

					//�ʒu��0.0f�ɖ߂�
					g_aHuman[nCntHuman].pos.y = 0.0f;
				}

				break;					//�����o��

			case HUMANSTATE_FLY:		//������񂾏��

				// ��΂�
				g_aHuman[nCntHuman].pos.x += g_aHuman[nCntHuman].move.x;
				g_aHuman[nCntHuman].pos.z += g_aHuman[nCntHuman].move.z;

				if (g_aHuman[nCntHuman].pos.y <= 0.0f)
				{ // �ʒu��0.0f�ȉ��ɂȂ����ꍇ
					//�g�p���Ă��Ȃ�
					g_aHuman[nCntHuman].bUse = false;
				}

				break;					//�����o��
			}

			//----------------------------------------------------
			//	�����蔻��
			//----------------------------------------------------
			// �I�u�W�F�N�g�Ƃ̓����蔻��
			CollisionObject
			( // ����
				&g_aHuman[nCntHuman].pos,		// ���݂̈ʒu
				&g_aHuman[nCntHuman].posOld,	// �O��̈ʒu
				&g_aHuman[nCntHuman].move,		// �ړ���
				HUMAN_WIDTH,					// ����
				HUMAN_DEPTH,					// ���s
				&nCnt							// �a�؃J�E���g
			);

			// �v���C���[�̕␳�̍X�V����
			RevHuman(&g_aHuman[nCntHuman].rot, &g_aHuman[nCntHuman].pos);
		}
	}
}

//======================================================================================================================
//	�l�Ԃ̕`�揈��
//======================================================================================================================
void DrawHuman(void)
{
	// �ϐ���錾
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

												// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL	  blueMat;					// ���߃}�e���A���|�C���^

	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aHuman[nCntHuman].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ
		  // ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aHuman[nCntHuman].mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aHuman[nCntHuman].rot.y, g_aHuman[nCntHuman].rot.x, g_aHuman[nCntHuman].rot.z);
			D3DXMatrixMultiply(&g_aHuman[nCntHuman].mtxWorld, &g_aHuman[nCntHuman].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aHuman[nCntHuman].pos.x, g_aHuman[nCntHuman].pos.y, g_aHuman[nCntHuman].pos.z);
			D3DXMatrixMultiply(&g_aHuman[nCntHuman].mtxWorld, &g_aHuman[nCntHuman].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aHuman[nCntHuman].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aHuman[nCntHuman].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aHuman[nCntHuman].modelData.dwNumMat; nCntMat++)
			{ // �}�e���A���̐����J��Ԃ�

				switch (g_aHuman[nCntHuman].state)
				{
				case HUMANSTATE_STOP:		//��~���

					// �\���̗̂v�f���N���A
					ZeroMemory(&blueMat, sizeof(D3DXMATERIAL));

					// �g�U���E�����E���Ȕ�����Ԃɂ���
					blueMat.MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
					blueMat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
					blueMat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&blueMat.MatD3D);			// ��

					break;					//�����o��

				default:		//��L�ȊO

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					break;					//�����o��
				}

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aHuman[nCntHuman].modelData.pTexture[nCntMat]);

				// ���f���̕`��
				g_aHuman[nCntHuman].modelData.pMesh->DrawSubset(nCntMat);
			}

			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	�l�Ԃ̐ݒ菈��
//======================================================================================================================
void SetHuman(D3DXVECTOR3 pos)
{
	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aHuman[nCntHuman].bUse == false)
		{ // �I�u�W�F�N�g���g�p����Ă��Ȃ��ꍇ
		  // ��������
			g_aHuman[nCntHuman].pos = pos;								// ���݂̈ʒu
			g_aHuman[nCntHuman].posOld = g_aHuman[nCntHuman].pos;		// �O��̈ʒu
			g_aHuman[nCntHuman].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
			g_aHuman[nCntHuman].fMaxMove = (float)(rand() % HUMAN_RANDAM_MOVE + HUMAN_MOVE_LEAST);		// �ړ��ʂ̍ő吔
			g_aHuman[nCntHuman].bJump = false;							// �W�����v���Ă��邩�ǂ���
			g_aHuman[nCntHuman].bMove = false;							// �ړ����Ă��Ȃ�
			g_aHuman[nCntHuman].state = HUMANSTATE_WALK;				// �������

			// �g�p���Ă����Ԃɂ���
			g_aHuman[nCntHuman].bUse = true;

			// ���f������ݒ�
			g_aHuman[nCntHuman].modelData = GetModelData(MODELTYPE_OBJECT_BIGTREE + FROM_OBJECT);	// ���f�����

			// �e�̃C���f�b�N�X��ݒ�
			g_aHuman[nCntHuman].nShadowID = SetModelShadow
			( // ����
				g_aHuman[nCntHuman].modelData,	// ���f�����
				&g_aHuman[nCntHuman].nShadowID,	// �e�̐e�̉e�C���f�b�N�X
				&g_aHuman[nCntHuman].bUse		// �e�̐e�̎g�p��
			);

			// �e�̈ʒu�ݒ�
			SetPositionShadow(g_aHuman[nCntHuman].nShadowID, g_aHuman[nCntHuman].pos, g_aHuman[nCntHuman].rot, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

			// �Ȃ���p���̐ݒu
			g_aHuman[nCntHuman].curveInfo.actionState = HUMANACT_WALK;			// ���
			g_aHuman[nCntHuman].curveInfo.nRandamRoute = rand() % MAX_HUMAN_ROUTE;		// ���[�g�̎��
			g_aHuman[nCntHuman].curveInfo.rotDest = g_aHuman[nCntHuman].rot;	// �ڕW�̌���
			g_aHuman[nCntHuman].rot.y = GetDefaultRot(g_aHuman[nCntHuman].curveInfo.nRandamRoute);		// �����̌���
			g_aHuman[nCntHuman].curveInfo.curveInfo = GetHumanRoute(g_aHuman[nCntHuman].curveInfo.nRandamRoute);		// ���[�g

			switch (g_aHuman[nCntHuman].curveInfo.curveInfo.dashAngle[0])
			{
			case DASH_RIGHT:	// �E�Ɍ������đ����Ă���

				// �ʒu��␳����
				g_aHuman[nCntHuman].pos.z = g_aHuman[nCntHuman].curveInfo.curveInfo.curvePoint[0].z - (HUMAN_WIDTH * 2);

				break;			// �����o��

			case DASH_LEFT:		// ���Ɍ������đ����Ă���

				// �ʒu��␳����
				g_aHuman[nCntHuman].pos.z = g_aHuman[nCntHuman].curveInfo.curveInfo.curvePoint[0].z + (HUMAN_WIDTH * 2);

				break;			// �����o��

			case DASH_FAR:		// ���Ɍ������đ����Ă���

				// �ʒu��␳����
				g_aHuman[nCntHuman].pos.x = g_aHuman[nCntHuman].curveInfo.curveInfo.curvePoint[0].x + (HUMAN_WIDTH * 2);

				break;			// �����o��

			case DASH_NEAR:		// ��O�Ɍ������đ����Ă���

				// �ʒu��␳����
				g_aHuman[nCntHuman].pos.x = g_aHuman[nCntHuman].curveInfo.curveInfo.curvePoint[0].x - (HUMAN_WIDTH * 2);

				break;			// �����o��
			}

			// �����𔲂���
			break;
		}
	}
}

#if 0
//======================================================================================================================
//	�I�u�W�F�N�g�̃_���[�W����
//======================================================================================================================
void HitHuman(Human *pHuman, int nDamage)
{
	if (pHuman->state == ACTIONSTATE_NORMAL)
	{ // �I�u�W�F�N�g���ʏ��Ԃ̏ꍇ

	  // �����̃_���[�W����̗͂��猸�Z
		pHuman->nLife -= nDamage;

		if (pHuman->nLife > 0)
		{ // �̗͂��c���Ă���ꍇ

		  // �_���[�W��Ԃɂ���
			pHuman->state = ACTIONSTATE_DAMAGE;

			// �p�[�e�B�N���̐ݒ�
			SetParticle
			( // ����
				pHuman->pos,						// �ʒu
				D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f),	// �F
				PARTICLETYPE_DAMAGE,				// ���
				SPAWN_PARTICLE_DAMAGE,				// �G�t�F�N�g��
				2									// ����
			);

			// �J�E���^�[��ݒ�
			pHuman->nCounterState = DAMAGE_TIME_OBJ;

			// �T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_DMG);			// SE (�_���[�W)
		}
		else
		{ // �̗͂��s�����ꍇ

		  // �����̐ݒ�
			SetExplosion(pHuman->pos, SOUNDTYPE_BREAK);

			// �p�[�e�B�N���̐ݒ�
			SetParticle
			( // ����
				pHuman->pos,						// �ʒu
				D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f),	// �F
				PARTICLETYPE_EXPLOSION,				// ���
				SPAWN_PARTICLE_EXPLOSION,			// �G�t�F�N�g��
				2									// ����
			);

			// �A�C�e���̐ݒ�
			SetItem(pHuman->pos, ITEMTYPE_HEAL);

			// �g�p���Ă��Ȃ���Ԃɂ���
			pHuman->bUse = false;
		}
	}
}
#endif

//======================================================================================================================
//	�I�u�W�F�N�g�̎擾����
//======================================================================================================================
Human *GetHumanData(void)
{
	// �I�u�W�F�N�g�̏��̐擪�A�h���X��Ԃ�
	return &g_aHuman[0];
}

//============================================================
//	�v���C���[�̈ʒu�̍X�V����
//============================================================
void PosHuman(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove, float fMaxMove)
{
	//--------------------------------------------------------
	//	�d�͂̍X�V
	//--------------------------------------------------------
	move->y -= HUMAN_GRAVITY;

	//--------------------------------------------------------
	//	�ړ��ʂ̕␳
	//--------------------------------------------------------
	if (move->x > fMaxMove)
	{ // �v���C���[�̈ړ��� (x) �����l�ȏ�̏ꍇ

	  // �v���C���[�̈ړ��� (x) ��␳
		move->x = fMaxMove;
	}
	else if (move->x < -MAX_HUMAN_BACKWARD)
	{ // �v���C���[�̈ړ��� (x) �����l�ȉ��̏ꍇ

	  // �v���C���[�̈ړ��� (x) ��␳
		move->x = -MAX_HUMAN_BACKWARD;
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
void RevHuman(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos)
{
	//--------------------------------------------------------
	//	�����̐��K��
	//--------------------------------------------------------
	if (rot->y > D3DX_PI) { rot->y -= D3DX_PI * 2; }
	else if (rot->y < -D3DX_PI) { rot->y += D3DX_PI * 2; }

	//--------------------------------------------------------
	//	�ړ��͈͂̕␳
	//--------------------------------------------------------
	if (pos->z > GetLimitStage().fNear - (30.0f * 2))
	{ // �͈͊O�̏ꍇ (��O)

		// ��O�Ɉʒu��␳
		pos->z = GetLimitStage().fNear - (30.0f * 2);
	}
	if (pos->z < GetLimitStage().fFar + (30.0f * 2))
	{ // �͈͊O�̏ꍇ (��)

		// ���Ɉʒu��␳
		pos->z = GetLimitStage().fFar + (30.0f * 2);
	}
	if (pos->x > GetLimitStage().fRight - (30.0f * 2))
	{ // �͈͊O�̏ꍇ (�E)

		// �E�Ɉʒu��␳
		pos->x = GetLimitStage().fRight - (30.0f * 2);
	}
	if (pos->x < GetLimitStage().fLeft + (30.0f * 2))
	{ // �͈͊O�̏ꍇ (��)

		// ���Ɉʒu��␳
		pos->x = GetLimitStage().fLeft + (30.0f * 2);
	}
}

//============================================================
// �l�Ԃ̋Ȃ��鏈��
//============================================================
void CurveHuman(Human *pHuman)
{
	// �ړ��ʂ��X�V
	pHuman->move.x += HUMAN_MOVE_FORWARD;

	// �ړ����Ă����Ԃɂ���
	pHuman->bMove = true;

	switch (pHuman->curveInfo.actionState)
	{
	case HUMANACT_WALK:		// �������

		// �l�Ԃ̂���Ⴂ����
		PassingHuman(pHuman);

		break;				// �����o��

	case HUMANACT_CURVE:	// �J�[�u���

		// �l�Ԃ̊p�x�X�V�E�␳����
		CurveRotHuman(pHuman);

		break;				// �����o��
	}

	if (pHuman->move.x > pHuman->fMaxMove)
	{ // �v���C���[�̈ړ��� (x) �����l�ȏ�̏ꍇ

		// �v���C���[�̈ړ��� (x) ��␳
		pHuman->move.x = pHuman->fMaxMove;
	}
}

//============================================================
// �l�Ԃ̕�������
//============================================================
void WalkHuman(Human *pHuman)
{
	if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_RIGHT)
	{ // �E�ɑ����Ă���ꍇ

		if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_RIGHT)
		{ // �E�ɋȂ���ꍇ

			if (pHuman->pos.x >= pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x)
			{ // �ړI�n�ɒ�������

				// �J�[�u��ԂɂȂ�
				pHuman->curveInfo.actionState = HUMANACT_CURVE;
			}
			else
			{ // ���ɂ���ǂ��܂���������

				// ���̕ǂɔ��킹��
				pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;
			}
		}
		else if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_LEFT)
		{ // ���ɋȂ���ꍇ
			if (pHuman->pos.x >= pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x)
			{ // �ړI�n�ɒ�������

				// �J�[�u��ԂɂȂ�
				pHuman->curveInfo.actionState = HUMANACT_CURVE;
			}
			else
			{ // ���ɂ���ǂ��܂���������

				// ���̕ǂɔ��킹��
				pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;
			}
		}
	}
	else if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_LEFT)
	{ // ���ɑ����Ă���ꍇ
		if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_RIGHT)
		{//�E�ɋȂ���ꍇ
			if (pHuman->pos.x <= pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x)
			{ // �ړI�n�ɒ�������

				// �J�[�u��ԂɂȂ�
				pHuman->curveInfo.actionState = HUMANACT_CURVE;
			}
			else
			{ // ���ɂ���ǂ��܂���������

				// ��O�̕ǂɔ��킹��
				pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;
			}
		}
		else if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_LEFT)
		{//���ɋȂ���ꍇ
			if (pHuman->pos.x <= pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x)
			{ // �ړI�n�ɒ�������

				// �J�[�u��ԂɂȂ�
				pHuman->curveInfo.actionState = HUMANACT_CURVE;
			}
			else
			{ // ���ɂ���ǂ��܂���������

				// ��O�̕ǂɔ��킹��
				pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;
			}
		}
	}
	else if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_FAR)
	{ // ���ɑ����Ă���ꍇ
		if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_RIGHT)
		{//�E�ɋȂ���ꍇ
			if (pHuman->pos.z >= pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z)
			{ // �ړI�n�ɒ�������

				// �J�[�u��ԂɂȂ�
				pHuman->curveInfo.actionState = HUMANACT_CURVE;
			}
			else
			{ // ���ɂ���ǂ��܂���������

				// �E�̕ǂɔ��킹��
				pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;
			}
		}
		else if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_LEFT)
		{ // ���ɋȂ���ꍇ
			if (pHuman->pos.z >= pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z)
			{ // �ړI�n�ɒ�������

				// �J�[�u��ԂɂȂ�
				pHuman->curveInfo.actionState = HUMANACT_CURVE;
			}
			else
			{ // ���ɂ���ǂ��܂���������

				// �E�̕ǂɔ��킹��
				pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;
			}
		}
	}
	else if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_NEAR)
	{ // ��O�ɑ����Ă���ꍇ
		if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_RIGHT)
		{ // �E�ɋȂ���ꍇ
			if (pHuman->pos.z <= pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z)
			{ // ���ɂ���ǂ��r�؂ꂽ��

				// �J�[�u��ԂɂȂ�
				pHuman->curveInfo.actionState = HUMANACT_CURVE;
			}
			else
			{ // ���ɂ���ǂ��܂���������

				// ���̕ǂɔ��킹��
				pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;
			}
		}
		else if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_LEFT)
		{ // ���ɋȂ���ꍇ
			if (pHuman->pos.z <= pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z)
			{ // ���ɂ���ǂ��r�؂ꂽ��

				// �J�[�u��ԂɂȂ�
				pHuman->curveInfo.actionState = HUMANACT_CURVE;
			}
			else
			{ // ���ɂ���ǂ��܂���������

				// ���̕ǂɔ��킹��
				pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;
			}
		}
	}
}

//============================================================
// �l�Ԃ̒�~����
//============================================================
void StopHuman(Human *pHuman)
{
	//�ړ��ʂ�0�ɂ���
	pHuman->move.x = 0.0f;
}

//============================================================
//�l�Ԃ̃��A�N�V��������
//============================================================
void ReactionHuman(Human *pHuman)
{
	float fLength;										// �����̕ϐ�

	Player *pPlayer = GetPlayer();

	if (pHuman->state != HUMANSTATE_FLY)
	{ // ������я�Ԃ���Ȃ��ꍇ
		if (pPlayer->bUse == true)
		{ // �g�p���Ă���ꍇ
			// �����𑪂�
			fLength = (pPlayer->pos.x - pHuman->pos.x) * (pPlayer->pos.x - pHuman->pos.x)
				+ (pPlayer->pos.z - pHuman->pos.z) * (pPlayer->pos.z - pHuman->pos.z);

			if (fLength <= (pPlayer->modelData.fRadius + REACTION_CAR_RANGE) * REACTION_HUMAN_RANGE)
			{ // �v���C���[���߂��ɗ����ꍇ
				//��~�����Ɉڍs
				pHuman->state = HUMANSTATE_STOP;
			}
			else
			{ // �v���C���[�ɋ߂��ɂ��Ȃ��ꍇ
				//��~�����Ɉڍs
				pHuman->state = HUMANSTATE_WALK;
			}
		}
	}
}

//============================================================
// �l�ԂƎԂ̓����蔻��
//============================================================
void CollisionCarHuman(Human *pHuman)
{
	{ // �Ԃ̓����蔻��
		Car *pCar = GetCarData();					// �Ԃ̏����擾����

		for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
		{
			if (pCar[nCntCar].bUse == true)
			{ // �Ԃ��g�p����Ă����ꍇ
				if (pHuman->pos.x + pHuman->modelData.vtxMin.x <= pCar[nCntCar].pos.x + pCar[nCntCar].modelData.vtxMax.x 
					&& pHuman->pos.x + pHuman->modelData.vtxMax.x >= pCar[nCntCar].pos.x + pCar[nCntCar].modelData.vtxMin.x)
				{ // �Ԃ�X���̒��ɂ����ꍇ
					if (pHuman->posOld.z + pHuman->modelData.vtxMax.z <= pCar[nCntCar].posOld.z + pCar[nCntCar].modelData.vtxMin.z
						&& pHuman->pos.z + pHuman->modelData.vtxMax.z >= pCar[nCntCar].pos.z + pCar[nCntCar].modelData.vtxMin.z)
					{//�O��̈ʒu���u���b�N����O���A���݂̈ʒu���u���b�N���������������ꍇ(��O�Ŏ~�߂��鏈��)

						// �ړ��ʂ�0�ɂ���
						pCar[nCntCar].move.x = 0.0f;

					}							//��O�Ŏ~�߂��鏈��
					else if (pHuman->posOld.z + pHuman->modelData.vtxMin.z >= pCar[nCntCar].posOld.z + pCar[nCntCar].modelData.vtxMax.z
						&& pHuman->pos.z + pHuman->modelData.vtxMin.z <= pCar[nCntCar].pos.z + pCar[nCntCar].modelData.vtxMax.z)
					{//�O��̈ʒu�����̈ʒu���������A���݂̈ʒu�����̈ʒu������O�������ꍇ(���Ŏ~�߂��鏈��)
					
						// �ړ��ʂ�0�ɂ���
						pCar[nCntCar].move.x = 0.0f;

					}							//���Ŏ~�߂��鏈��
				}

				if (pHuman->pos.z + pHuman->modelData.vtxMin.z <= pCar[nCntCar].pos.z + pCar[nCntCar].modelData.vtxMax.z
					&& pHuman->pos.z + pHuman->modelData.vtxMax.z >= pCar[nCntCar].pos.z + pCar[nCntCar].modelData.vtxMin.z)
				{//����Z���̒��ɂ����ꍇ
					if (pHuman->posOld.x + pHuman->modelData.vtxMax.x <= pCar[nCntCar].posOld.x + pCar[nCntCar].modelData.vtxMin.x
						&& pHuman->pos.x + pHuman->modelData.vtxMax.x >= pCar[nCntCar].pos.x + pCar[nCntCar].modelData.vtxMin.x)
					{//�O��̈ʒu���u���b�N�̍��[��荶���A���݂̈ʒu���u���b�N�̍������E�������ꍇ(���̏���)
						
						//�ړ��ʂ�0�ɂ���
						pCar[nCntCar].move.x = 0.0f;

					}							//���[�̏���
					else if (pHuman->posOld.x + pHuman->modelData.vtxMin.x >= pCar[nCntCar].posOld.x + pCar[nCntCar].modelData.vtxMax.x
						&& pHuman->pos.x + pHuman->modelData.vtxMin.x <= pCar[nCntCar].pos.x + pCar[nCntCar].modelData.vtxMax.x)
					{//�O��̈ʒu���u���b�N�̉E�[���E���A���݂̈ʒu���u���b�N�̍������E�������ꍇ(�E�̏���)
						
						// �ړ��ʂ�0�ɂ���
						pCar[nCntCar].move.x = 0.0f;

					}							//�E�[�̏���
				}
			}
		}
	}

	{ // �x�@�̓����蔻��
		Police *pPolice = GetPoliceData();					// �x�@�̏����擾����

		for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
		{
			if (pPolice[nCntPolice].bUse == true)
			{ // �Ԃ��g�p����Ă����ꍇ
				if (pHuman->pos.x + pHuman->modelData.vtxMin.x <= pPolice[nCntPolice].pos.x + pPolice[nCntPolice].modelData.vtxMax.x 
					&& pHuman->pos.x + pHuman->modelData.vtxMax.x >= pPolice[nCntPolice].pos.x + pPolice[nCntPolice].modelData.vtxMin.x)
				{ // �Ԃ�X���̒��ɂ����ꍇ
					if (pHuman->posOld.z + pHuman->modelData.vtxMax.z <= pPolice[nCntPolice].posOld.z + pPolice[nCntPolice].modelData.vtxMin.z
						&& pHuman->pos.z + pHuman->modelData.vtxMax.z >= pPolice[nCntPolice].pos.z + pPolice[nCntPolice].modelData.vtxMin.z)
					{//�O��̈ʒu���u���b�N����O���A���݂̈ʒu���u���b�N���������������ꍇ(��O�Ŏ~�߂��鏈��)

						// �ړ��ʂ�0.0f�ɂ���
						pPolice[nCntPolice].move.x = 0.0f;

					}							//��O�Ŏ~�߂��鏈��
					else if (pHuman->posOld.z + pHuman->modelData.vtxMin.z >= pPolice[nCntPolice].posOld.z + pPolice[nCntPolice].modelData.vtxMax.z
						&& pHuman->pos.z + pHuman->modelData.vtxMin.z <= pPolice[nCntPolice].pos.z + pPolice[nCntPolice].modelData.vtxMax.z)
					{//�O��̈ʒu�����̈ʒu���������A���݂̈ʒu�����̈ʒu������O�������ꍇ(���Ŏ~�߂��鏈��)

						// �ړ��ʂ�0.0f�ɂ���
						pPolice[nCntPolice].move.x = 0.0f;

					}							//���Ŏ~�߂��鏈��
				}

				if (pHuman->pos.z + pHuman->modelData.vtxMin.z <= pPolice[nCntPolice].pos.z + pPolice[nCntPolice].modelData.vtxMax.z
					&& pHuman->pos.z + pHuman->modelData.vtxMax.z >= pPolice[nCntPolice].pos.z + pPolice[nCntPolice].modelData.vtxMin.z)
				{//����Z���̒��ɂ����ꍇ
					if (pHuman->posOld.x + pHuman->modelData.vtxMax.x <= pPolice[nCntPolice].posOld.x + pPolice[nCntPolice].modelData.vtxMin.x
						&& pHuman->pos.x + pHuman->modelData.vtxMax.x >= pPolice[nCntPolice].pos.x + pPolice[nCntPolice].modelData.vtxMin.x)
					{//�O��̈ʒu���u���b�N�̍��[��荶���A���݂̈ʒu���u���b�N�̍������E�������ꍇ(���̏���)
						
						// �ړ��ʂ�0.0f�ɂ���
						pPolice[nCntPolice].move.x = 0.0f;

					}							//���[�̏���
					else if (pHuman->posOld.x + pHuman->modelData.vtxMin.x >= pPolice[nCntPolice].posOld.x + pPolice[nCntPolice].modelData.vtxMax.x
						&& pHuman->pos.x + pHuman->modelData.vtxMin.x <= pPolice[nCntPolice].pos.x + pPolice[nCntPolice].modelData.vtxMax.x)
					{//�O��̈ʒu���u���b�N�̉E�[���E���A���݂̈ʒu���u���b�N�̍������E�������ꍇ(�E�̏���)
						
						// �ړ��ʂ�0.0f�ɂ���
						pPolice[nCntPolice].move.x = 0.0f;

					}							//�E�[�̏���
				}
			}
		}
	}
}

//============================================================
// �l�Ԃ̊p�x�X�V�E�␳����
//============================================================
void CurveRotHuman(Human *pHuman)
{
	if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_LEFT)
	{ // �Ȃ�����������������ꍇ

		// �ړ��ʂ�����
		pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

		// �������X�V
		pHuman->rot.y -= 0.05f * (pHuman->move.x * 0.1f);

		if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_RIGHT)
		{ // �E�ɑ����Ă���ꍇ

			// �ʒu��␳����
			pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;

			// �ڕW�̌�����ݒ肷��
			pHuman->curveInfo.rotDest.y = 0.0f;
		}
		else if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_LEFT)
		{ // ���ɑ����Ă���ꍇ

			// �ʒu��␳����
			pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;

			// �ڕW�̌�����ݒ肷��
			pHuman->curveInfo.rotDest.y = -D3DX_PI;
		}
		else if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_FAR)
		{ // ���ɑ����Ă���ꍇ

			// �ʒu��␳����
			pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;

			// �ڕW�̌�����ݒ肷��
			pHuman->curveInfo.rotDest.y = -D3DX_PI * 0.5f;
		}
		else if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_NEAR)
		{ // ��O�ɑ����Ă���ꍇ

			// �ʒu��␳����
			pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;

			// �ڕW�̌�����ݒ肷��
			pHuman->curveInfo.rotDest.y = D3DX_PI * 0.5f;
		}

		// ������������
		pHuman->rot.y -= HUMAN_CURVE_ADD;

		if (pHuman->rot.y <= pHuman->curveInfo.rotDest.y)
		{ // �������ڕW�l�𒴂����ꍇ

			// ������␳����
			pHuman->rot.y = pHuman->curveInfo.rotDest.y;

			// ���s��Ԃɂ���
			pHuman->curveInfo.actionState = HUMANACT_WALK;

			// �x�@�̍s���ݒ肷��
			pHuman->curveInfo.curveInfo.nNowCurve = (pHuman->curveInfo.curveInfo.nNowCurve + 1) % pHuman->curveInfo.curveInfo.nCurveTime;

			if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_NEAR)
			{ // ���ɑ����Ă���ꍇ

				if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_LEFT)
				{ // ���̋Ȃ���p�����������ꍇ

					// �������}�C�i�X�ɂ���
					pHuman->rot.y = (float)D3DXToRadian(180);
				}
			}
		}
	}
	else
	{ // �Ȃ���������E�������ꍇ

		// �ړ��ʂ�����
		pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

		// �������X�V
		pHuman->rot.y += 0.05f * (pHuman->move.x * 0.1f);

		if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_RIGHT)
		{ // �E�ɑ����Ă���ꍇ

			//�ʒu��␳����
			pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;

			// �ڕW�̌�����ݒ肷��
			pHuman->curveInfo.rotDest.y = D3DX_PI;
		}
		else if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_LEFT)
		{ // ���ɑ����Ă���ꍇ

			// �ʒu��␳����
			pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;

			// �ڕW�̌�����ݒ肷��
			pHuman->curveInfo.rotDest.y = 0.0f;
		}
		else if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_FAR)
		{ // ���ɑ����Ă���ꍇ

			// �ʒu��␳����
			pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;

			// �ڕW�̌�����ݒ肷��
			pHuman->curveInfo.rotDest.y = D3DX_PI * 0.5f;
		}
		else if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_NEAR)
		{ // ��O�ɑ����Ă���ꍇ

			// �ʒu��␳����
			pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;

			// �ڕW�̌�����ݒ肷��
			pHuman->curveInfo.rotDest.y = -D3DX_PI * 0.5f;
		}

		if (pHuman->rot.y >= pHuman->curveInfo.rotDest.y)
		{ // �������ڕW�l�𒴂����ꍇ

			// ������␳����
			pHuman->rot.y = pHuman->curveInfo.rotDest.y;

			// ���s��Ԃɂ���
			pHuman->curveInfo.actionState = HUMANACT_WALK;

			// �x�@�̍s���ݒ肷��
			pHuman->curveInfo.curveInfo.nNowCurve = (pHuman->curveInfo.curveInfo.nNowCurve + 1) % pHuman->curveInfo.curveInfo.nCurveTime;

			if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_NEAR)
			{ // ���ɑ����Ă���ꍇ

				if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_RIGHT)
				{ // ���̋Ȃ���p���}�C�i�X�����ɋȂ���ꍇ

					// �������}�C�i�X�ɂ���
					pHuman->rot.y = (float)D3DXToRadian(-180);
				}
			}
		}
	}

	// �����̐��K��
	RotNormalize(&pHuman->rot.y);
}

//============================================================
// �l�Ԃ̂���Ⴂ����
//============================================================
void PassingHuman(Human *pHuman)
{
	Human *pPassHuman = GetHumanData();		// �l�Ԃ̏��
	float fLength;							// ����
	float posDest;							// �ڕW�̈ʒu
	float posDiff;							// �ʒu�̍���

	for (int nCnt = 0; nCnt < MAX_HUMAN; nCnt++,pPassHuman++)
	{
		if (pPassHuman != pHuman)
		{ // ��r����l�Ԃ��������g����Ȃ��ꍇ
			if (pPassHuman->bUse == true)
			{ // �l�Ԃ��g�p����Ă����ꍇ
				//�����𑪂�
				fLength = (pPassHuman->pos.x - pHuman->pos.x) * (pPassHuman->pos.x - pHuman->pos.x)
					+ (pPassHuman->pos.z - pHuman->pos.z) * (pPassHuman->pos.z - pHuman->pos.z);

				if (fLength <= (HUMAN_RADIUS * HUMAN_RADIUS))
				{ // �I�u�W�F�N�g���������Ă���
					if (pHuman->move.x >= pPassHuman->move.x)
					{ // �ړ��ʂ������ꍇ
						switch (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve])
						{
						case DASH_RIGHT:		// �E

							// �ڕW�̈ʒu��ݒ�
							posDest = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z - HUMAN_PASS_SHIFT;

							// �ʒu�̍��������߂�
							posDiff = posDest - pHuman->pos.z;

							// �������炷
							pHuman->pos.z += posDiff * HUMAN_PASS_CORRECT;

							break;				// �����o��

						case DASH_LEFT:			// ��

							// �ڕW�̈ʒu��ݒ�
							posDest = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z + HUMAN_PASS_SHIFT;

							// �ʒu�̍��������߂�
							posDiff = posDest - pHuman->pos.z;

							// �������炷
							pHuman->pos.z += posDiff * HUMAN_PASS_CORRECT;

							break;				// �����o��

						case DASH_FAR:			// ��

							// �ڕW�̈ʒu��ݒ�
							posDest = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x + HUMAN_PASS_SHIFT;

							// �ʒu�̍��������߂�
							posDiff = posDest - pHuman->pos.x;

							// �������炷
							pHuman->pos.x += posDiff * HUMAN_PASS_CORRECT;

							break;				// �����o��

						case DASH_NEAR:			// ��O

							// �ڕW�̈ʒu��ݒ�
							posDest = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x - HUMAN_PASS_SHIFT;

							// �ʒu�̍��������߂�
							posDiff = posDest - pHuman->pos.x;

							// �������炷
							pHuman->pos.x += posDiff * HUMAN_PASS_CORRECT;

							break;				// �����o��
						}
					}
				}
				else
				{ // �I�u�W�F�N�g���������Ă���

					// �l�Ԃ̕�������
					WalkHuman(pHuman);
				}
			}
		}
		else
		{ // ��L�ȊO

			// �l�Ԃ̕�������
			WalkHuman(pHuman);
		}
	}
}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================
#endif