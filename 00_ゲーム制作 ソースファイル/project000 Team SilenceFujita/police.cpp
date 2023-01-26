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
 
#include "Car.h"
#include "Police.h"
#include "shadow.h"
#include "sound.h"
#include "player.h"
#include "object.h"

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
#define POLICAR_WIDTH			(45.0f)		// �p�g�J�[�̏c��
#define POLICAR_DEPTH			(45.0f)		// �p�g�J�[�̉��s

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void PosPolice(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove);	// �v���C���[�̈ʒu�̍X�V����
void RevPolice(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos);		// �v���C���[�̕␳�̍X�V����
void PatrolPoliceAct(Police *pPolice);					// �x�@�̃p�g���[���s������
void PatrolCarSearch(Police *pPolice);					// �x�@�ԗ��̒T�m����
void ChasePoliceAct(Police *pPolice);					// �x�@�̒ǐՏ���
void PatrolBackAct(Police *pPolice);					// �p�g���[���ɖ߂鏈��
void ReturnPoliceRot(Police *pPolice);					// �x�@�̌����𒼂�����

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
Police g_aPolice[MAX_POLICE];	// �I�u�W�F�N�g�̏��

//======================================================================================================================
//	�x�@�̏���������
//======================================================================================================================
void InitPolice(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// �I�u�W�F�N�g�̏��̏�����
	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		g_aPolice[nCntPolice].pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aPolice[nCntPolice].posOld	= g_aPolice[nCntPolice].pos;		// �O��̈ʒu
		g_aPolice[nCntPolice].posCopy	= g_aPolice[nCntPolice].pos;		// �ŏ��̈ʒu
		g_aPolice[nCntPolice].move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
		g_aPolice[nCntPolice].rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aPolice[nCntPolice].rotDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW�̌���
		g_aPolice[nCntPolice].state		= POLICESTATE_PATROL;				// �x�@�̏��
		g_aPolice[nCntPolice].nLife		= 0;								// �̗�
		g_aPolice[nCntPolice].nShadowID = NONE_SHADOW;						// �e�̃C���f�b�N�X
		g_aPolice[nCntPolice].bUse		= false;							// �g�p��

		// ���f�����̏�����
		g_aPolice[nCntPolice].modelData.dwNumMat = 0;						// �}�e���A���̐�
		g_aPolice[nCntPolice].modelData.pTexture = NULL;					// �e�N�X�`���ւ̃|�C���^
		g_aPolice[nCntPolice].modelData.pMesh    = NULL;					// ���b�V�� (���_���) �ւ̃|�C���^
		g_aPolice[nCntPolice].modelData.pBuffMat = NULL;					// �}�e���A���ւ̃|�C���^
		g_aPolice[nCntPolice].modelData.dwNumMat = 0;						// �}�e���A���̐�
		g_aPolice[nCntPolice].modelData.vtxMin   = INIT_VTX_MIN;			// �ŏ��̒��_���W
		g_aPolice[nCntPolice].modelData.vtxMax   = INIT_VTX_MAX;			// �ő�̒��_���W
		g_aPolice[nCntPolice].modelData.size     = INIT_SIZE;				// �傫��
		g_aPolice[nCntPolice].modelData.fRadius  = 0.0f;					// ���a

		// �Ȃ���p�̈ʒu�̏�����
		for (int nCnt = 0; nCnt < MAX_CURVE; nCnt++)
		{
			g_aPolice[nCntPolice].policeCurve.bCurvePlus[nCnt] = false;			// �{�����ɑ��邩
			g_aPolice[nCntPolice].policeCurve.bCurveX[nCnt] = false;			// X���𑖂邩
			g_aPolice[nCntPolice].policeCurve.curveAngle[nCnt] = CURVE_LEFT;	// �ǂ����ɋȂ��邩
			g_aPolice[nCntPolice].policeCurve.curvePoint[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ǂ��ŋȂ��邩
			g_aPolice[nCntPolice].policeCurve.fCurveRot[nCnt] = 0;				// �������p�x
		}
		g_aPolice[nCntPolice].policeCurve.nCurveTime = 0;						// �Ȃ���p�̉�
		g_aPolice[nCntPolice].policeCurve.nNowCurve = 0;						// ���݂̋Ȃ���p
	}
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
	if (GetKeyboardTrigger(DIK_RSHIFT) == true)
	{
		g_aPolice[0].state = POLICESTATE_PATBACK;
	}

	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�
		if (g_aPolice[nCntPolice].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// �O��ʒu�̍X�V
			g_aPolice[nCntPolice].posOld = g_aPolice[nCntPolice].pos;

			//----------------------------------------------------
			//	�e�̍X�V
			//----------------------------------------------------
			// �e�̈ʒu�ݒ�
			SetPositionShadow
			( // ����
				g_aPolice[nCntPolice].nShadowID,	// �e�̃C���f�b�N�X
				g_aPolice[nCntPolice].pos,			// �ʒu
				g_aPolice[nCntPolice].rot,			// ����
				NONE_SCALE							// �g�嗦
			);

			switch (g_aPolice[nCntPolice].state)
			{//��ԂŔ��f����
			case POLICESTATE_PATROL:				//�p�g���[�����

				// �x�@�̃p�g���[���s������
				PatrolPoliceAct(&g_aPolice[nCntPolice]);

				break;						//�����o��

			case POLICESTATE_CHASE:			//�ǐՏ���

				// �x�@�̒ǐՏ���
				ChasePoliceAct(&g_aPolice[nCntPolice]);

				break;						//�����o��

			case POLICESTATE_PATBACK:		//�p�g���[���֖߂鏈��			

				break;						//�����o��

			case POLICESTATE_POSBACK:		//�ŏ��̍��W�ɖ߂鏈��

				break;						//�����o��
			}

			if (g_aPolice[nCntPolice].pos.y < 0.0f)
			{//Y���̈ʒu��0.0f�������ꍇ
				//�c�ւ̈ړ��ʂ�0.0f�ɂ���
				g_aPolice[nCntPolice].move.y = 0.0f;

				//�ʒu��0.0f�ɖ߂�
				g_aPolice[nCntPolice].pos.y = 0.0f;
			}

			// �v���C���[�̕␳�̍X�V����
			RevPolice(&g_aPolice[nCntPolice].rot, &g_aPolice[nCntPolice].pos);

			// �v���C���[�̈ʒu�̍X�V
			PosPolice(&g_aPolice[nCntPolice].move, &g_aPolice[nCntPolice].pos, &g_aPolice[nCntPolice].rot, g_aPolice[nCntPolice].bMove);

			//----------------------------------------------------
			//	�����蔻��
			//----------------------------------------------------
			// �I�u�W�F�N�g�Ƃ̓����蔻��
			CollisionObject
			( // ����
				&g_aPolice[nCntPolice].pos,		// ���݂̈ʒu
				&g_aPolice[nCntPolice].posOld,	// �O��̈ʒu
				&g_aPolice[nCntPolice].move,	// �ړ���
				POLICAR_WIDTH,			// ����
				POLICAR_DEPTH			// ���s
			);

			// �Ԃ̒�~����
			CollisionStopCar
			( // ����
				g_aPolice[nCntPolice].pos,		//�ʒu
				g_aPolice[nCntPolice].rot,		//����
				&g_aPolice[nCntPolice].move,	//�ړ���
				g_aPolice[nCntPolice].modelData.fRadius,	//���a
				COLLOBJECTTYPE_POLICE			//�Ώۂ̃T�C�Y
			);

			// �ԓ��m�̓����蔻��
			CollisionCarBody
			( // ����
				&g_aPolice[nCntPolice].pos,
				&g_aPolice[nCntPolice].posOld,
				g_aPolice[nCntPolice].rot,
				&g_aPolice[nCntPolice].move,
				g_aPolice[nCntPolice].modelData,
				COLLOBJECTTYPE_POLICE
			);
		}
	}
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
			pMat = &g_aPolice[nCntPolice].MatCopy[0];

			for (int nCntMat = 0; nCntMat < (int)g_aPolice[nCntPolice].modelData.dwNumMat; nCntMat++)
			{ // �}�e���A���̐����J��Ԃ�

				switch (g_aPolice[nCntPolice].state)
				{
				case POLICESTATE_PATBACK:	//�p�g���[���ɖ߂��Ă�����

					// �����x��������
					pMat[nCntMat].MatD3D.Diffuse.a -= 0.005f;
					pMat[nCntMat].MatD3D.Ambient.a -= 0.005f;
					pMat[nCntMat].MatD3D.Emissive.a -= 0.005f;

					if (pMat->MatD3D.Emissive.a <= 0.0f)
					{ // �����x��0.0f�ȉ��ɂȂ����ꍇ
						// �����x��������
						pMat[nCntMat].MatD3D.Diffuse.a = 0.0f;
						pMat[nCntMat].MatD3D.Ambient.a = 0.0f;
						pMat[nCntMat].MatD3D.Emissive.a = 0.0f;

						// �p�g���[���ɖ߂鏈��
						PatrolBackAct(&g_aPolice[nCntPolice]);

						//�ŏ��̈ʒu�ɖ߂�����
						g_aPolice[nCntPolice].state = POLICESTATE_POSBACK;
					}

					break;					//�����o��

				case POLICESTATE_POSBACK:	//�ŏ��̈ʒu�ɖ߂���

					// �����x��������
					pMat[nCntMat].MatD3D.Diffuse.a += 0.005f;
					pMat[nCntMat].MatD3D.Ambient.a += 0.005f;
					pMat[nCntMat].MatD3D.Emissive.a += 0.005f;

					if (pMat->MatD3D.Emissive.a >= 1.0f)
					{ // �����x��0.0f�ȉ��ɂȂ����ꍇ
					  // �����x��������
						pMat[nCntMat].MatD3D.Diffuse.a = 1.0f;
						pMat[nCntMat].MatD3D.Ambient.a = 1.0f;
						pMat[nCntMat].MatD3D.Emissive.a = 1.0f;

						// �ŏ��̈ړ��ʂ����ɖ߂�
						g_aPolice[nCntPolice].move.x = 0.0f;

						//�ŏ��̈ʒu�ɖ߂�����
						g_aPolice[nCntPolice].state = POLICESTATE_PATROL;
					}

					break;					//�����o��

				default:					//��L�ȊO

					//���X�̃}�e���A����������
					pMat = (D3DXMATERIAL*)g_aPolice[nCntPolice].modelData.pBuffMat->GetBufferPointer();

					break;					//�����o��
				}

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
void SetPolice(D3DXVECTOR3 pos, CURVE poliCurve)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aPolice[nCntPolice].bUse == false)
		{ // �I�u�W�F�N�g���g�p����Ă��Ȃ��ꍇ
			// ��������
			g_aPolice[nCntPolice].pos		= pos;								// ���݂̈ʒu
			g_aPolice[nCntPolice].posCopy   = g_aPolice[nCntPolice].pos;		// �ŏ��̈ʒu
			g_aPolice[nCntPolice].posOld	= g_aPolice[nCntPolice].pos;		// �O��̈ʒu
			g_aPolice[nCntPolice].rotDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW�̌���
			g_aPolice[nCntPolice].move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
			g_aPolice[nCntPolice].state		= POLICESTATE_PATROL;				// �p�g���[����Ԃɂ���
			g_aPolice[nCntPolice].nLife		= POLI_LIFE;						// �̗�
			g_aPolice[nCntPolice].bMove		= false;							// �ړ����Ă��Ȃ�

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

			D3DXMATERIAL *pMat;					//�}�e���A���ւ̃|�C���^

			//�}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aPolice[nCntPolice].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aPolice[nCntPolice].modelData.dwNumMat; nCntMat++)
			{
				//�}�e���A���̏����擾����
				g_aPolice[nCntPolice].MatCopy[nCntMat] = pMat[nCntMat];
			}

			g_aPolice[nCntPolice].policeCurve.nCurveTime = poliCurve.nCurveTime;				// �Ȃ����

			for (int nCnt = 0; nCnt < g_aPolice[nCntPolice].policeCurve.nCurveTime; nCnt++)
			{
				g_aPolice[nCntPolice].policeCurve.curvePoint[nCnt] = poliCurve.curvePoint[nCnt];	// �Ȃ���|�C���g
				g_aPolice[nCntPolice].policeCurve.fCurveRot[nCnt] = poliCurve.fCurveRot[nCnt];		// ���Ȃ���p�x
			}

			g_aPolice[nCntPolice].policeCurve.nNowCurve = 0;									// ���݂̋Ȃ���p

			// �Ȃ���ʒu�̐ݒ菈��
			SetCurvePoint(&g_aPolice[nCntPolice].policeCurve, &g_aPolice[nCntPolice].rot, &g_aPolice[nCntPolice].pos);

			if (g_aPolice[nCntPolice].policeCurve.bCurveX[0] == true)
			{ // �ŏ�X���𑖂��Ă���ꍇ
				if (g_aPolice[nCntPolice].policeCurve.bCurvePlus[0] == true)
				{ // �E�ɑ����Ă���ꍇ
					// �ʒu��␳����
					g_aPolice[nCntPolice].pos.z = g_aPolice[nCntPolice].policeCurve.curvePoint[0].z - (POLICAR_WIDTH * 2);
					g_aPolice[nCntPolice].posCopy = g_aPolice[nCntPolice].pos;
				}
				else
				{ // ���ɑ����Ă���ꍇ
					// �ʒu��␳����
					g_aPolice[nCntPolice].pos.z = g_aPolice[nCntPolice].policeCurve.curvePoint[0].z + (POLICAR_WIDTH * 2);
					g_aPolice[nCntPolice].posCopy = g_aPolice[nCntPolice].pos;
				}
			}
			else
			{ // �ŏ�Z���𑖂��Ă���ꍇ
				if (g_aPolice[nCntPolice].policeCurve.bCurvePlus[0] == true)
				{ // ���ɑ����Ă���ꍇ
					// �ʒu��␳����
					g_aPolice[nCntPolice].pos.x = g_aPolice[nCntPolice].policeCurve.curvePoint[0].x + (POLICAR_WIDTH * 2);
					g_aPolice[nCntPolice].posCopy = g_aPolice[nCntPolice].pos;
				}
				else
				{ // ��O�ɑ����Ă���ꍇ
					// �ʒu��␳����
					g_aPolice[nCntPolice].pos.x = g_aPolice[nCntPolice].policeCurve.curvePoint[0].x - (POLICAR_WIDTH * 2);
					g_aPolice[nCntPolice].posCopy = g_aPolice[nCntPolice].pos;
				}
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
//	�I�u�W�F�N�g�̎擾����
//======================================================================================================================
Police *GetPoliceData(void)
{
	// �I�u�W�F�N�g�̏��̐擪�A�h���X��Ԃ�
	return &g_aPolice[0];
}

//============================================================
//	�v���C���[�̈ʒu�̍X�V����
//============================================================
void PosPolice(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove)
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
void RevPolice(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos)
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

	if (pPolice->policeCurve.bCurveX[pPolice->policeCurve.nNowCurve] == true)
	{//X���𑖂��Ă����ꍇ
		if (pPolice->policeCurve.bCurvePlus[pPolice->policeCurve.nNowCurve] == true)
		{ // �E�ɑ����Ă���ꍇ
			if (pPolice->policeCurve.curveAngle[pPolice->policeCurve.nNowCurve] == CURVE_RIGHT)
			{//�E�ɋȂ���ꍇ
				if (pPolice->pos.x >= pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x - (POLICAR_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

					// �Ԃ̊p�x�X�V�E�␳����
					CurveRotCar(&pPolice->policeCurve, &pPolice->rot, &pPolice->move);

					// �ړ��ʂ�����
					pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

					// �E�̕ǂɔ��킹��
					pPolice->pos.x = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x - (POLICAR_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
					// ���̕ǂɔ��킹��
					pPolice->pos.z = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z - (POLICAR_WIDTH * 2);
				}
			}
			else if (pPolice->policeCurve.curveAngle[pPolice->policeCurve.nNowCurve] == CURVE_LEFT)
			{//���ɋȂ���ꍇ
				if (pPolice->pos.x >= pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x + (POLICAR_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

					// �Ԃ̊p�x�X�V�E�␳����
					CurveRotCar(&pPolice->policeCurve, &pPolice->rot, &pPolice->move);

					// �ړ��ʂ�����
					pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

					// �E�̕ǂɔ��킹��
					pPolice->pos.x = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x + (POLICAR_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
					// ���̕ǂɔ��킹��
					pPolice->pos.z = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z - (POLICAR_WIDTH * 2);
				}
			}
		}
		else
		{ // ���ɑ����Ă���ꍇ
			if (pPolice->policeCurve.curveAngle[pPolice->policeCurve.nNowCurve] == CURVE_RIGHT)
			{//�E�ɋȂ���ꍇ
				if (pPolice->pos.x <= pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x + (POLICAR_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

					// �Ԃ̊p�x�X�V�E�␳����
					CurveRotCar(&pPolice->policeCurve, &pPolice->rot, &pPolice->move);

					// �ړ��ʂ�����
					pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

					// ���̕ǂɔ��킹��
					pPolice->pos.x = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x + (POLICAR_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
					// ��O�̕ǂɔ��킹��
					pPolice->pos.z = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z + (POLICAR_WIDTH * 2);
				}
			}
			else if (pPolice->policeCurve.curveAngle[pPolice->policeCurve.nNowCurve] == CURVE_LEFT)
			{//���ɋȂ���ꍇ
				if (pPolice->pos.x <= pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x - (POLICAR_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

					// �Ԃ̊p�x�X�V�E�␳����
					CurveRotCar(&pPolice->policeCurve, &pPolice->rot, &pPolice->move);

					// �ړ��ʂ�����
					pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

					// ���̕ǂɔ��킹��
					pPolice->pos.x = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x - (POLICAR_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
				  // ��O�̕ǂɔ��킹��
					pPolice->pos.z = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z + (POLICAR_WIDTH * 2);
				}
			}
		}
	}
	else
	{//Z���𑖂��Ă����ꍇ
		if (pPolice->policeCurve.bCurvePlus[pPolice->policeCurve.nNowCurve] == true)
		{ // ���ɑ����Ă���ꍇ
			if (pPolice->policeCurve.curveAngle[pPolice->policeCurve.nNowCurve] == CURVE_RIGHT)
			{//�E�ɋȂ���ꍇ
				if (pPolice->pos.z >= pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z - (POLICAR_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

					// �Ԃ̊p�x�X�V�E�␳����
					CurveRotCar(&pPolice->policeCurve, &pPolice->rot, &pPolice->move);

					// �ړ��ʂ�����
					pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

					// ��O�̕ǂɔ��킹��
					pPolice->pos.z = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z - (POLICAR_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
					// �E�̕ǂɔ��킹��
					pPolice->pos.x = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x + (POLICAR_WIDTH * 2);
				}
			}
			else if (pPolice->policeCurve.curveAngle[pPolice->policeCurve.nNowCurve] == CURVE_LEFT)
			{ // ���ɋȂ���ꍇ
				if (pPolice->pos.z >= pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z + (POLICAR_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

					// �Ԃ̊p�x�X�V�E�␳����
					CurveRotCar(&pPolice->policeCurve, &pPolice->rot, &pPolice->move);

					// �ړ��ʂ�����
					pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

					// ��O�̕ǂɔ��킹��
					pPolice->pos.z = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z + (POLICAR_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
					// �E�̕ǂɔ��킹��
					pPolice->pos.x = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x + (POLICAR_WIDTH * 2);
				}
			}
		}
		else
		{ // ��O�ɑ����Ă���ꍇ
			if (pPolice->policeCurve.curveAngle[pPolice->policeCurve.nNowCurve] == CURVE_RIGHT)
			{//�E�ɋȂ���ꍇ
				if (pPolice->pos.z <= pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z + (POLICAR_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

					// �Ԃ̊p�x�X�V�E�␳����
					CurveRotCar(&pPolice->policeCurve, &pPolice->rot, &pPolice->move);

					// �ړ��ʂ�����
					pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

					// ���̕ǂɔ��킹��
					pPolice->pos.z = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z + (POLICAR_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
					// ���̕ǂɔ��킹��
					pPolice->pos.x = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x - (POLICAR_WIDTH * 2);
				}
			}
			else if (pPolice->policeCurve.curveAngle[pPolice->policeCurve.nNowCurve] == CURVE_LEFT)
			{
				if (pPolice->pos.z <= pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z - (POLICAR_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

					// �Ԃ̊p�x�X�V�E�␳����
					CurveRotCar(&pPolice->policeCurve, &pPolice->rot, &pPolice->move);

					// �ړ��ʂ�����
					pPolice->move.x += (0.0f - pPolice->move.x) * REV_POLI_MOVE_SUB;

					// ���̕ǂɔ��킹��
					pPolice->pos.z = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].z - (POLICAR_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
					// ���̕ǂɔ��킹��
					pPolice->pos.x = pPolice->policeCurve.curvePoint[pPolice->policeCurve.nNowCurve].x - (POLICAR_WIDTH * 2);
				}
			}
		}
	}

	if (pPolice->move.x > MAX_POLI_FORWARD_PATROL)
	{ // �v���C���[�̈ړ��� (x) �����l�ȏ�̏ꍇ

	  // �v���C���[�̈ړ��� (x) ��␳
		pPolice->move.x = MAX_POLI_FORWARD_PATROL;
	}

	//�ړI�̋�����ݒ肷��
	fDist = fabsf(sqrtf((pPlayer->pos.x - pPolice->pos.x) * (pPlayer->pos.x - pPolice->pos.x) + (pPlayer->pos.z - pPolice->pos.z) * (pPlayer->pos.z - pPolice->pos.z)));

	if (fDist <= 1000.0f)
	{ // �ړI�̋��������ȓ��ɓ�������
		// �ǐՏ�ԂɈڍs����
		pPolice->state = POLICESTATE_CHASE;
	}

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

	if (fDist <= 3000.0f)
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
	// �ړ��ʂ��X�V
	pPolice->move.x += POLI_MOVE_FORWARD;

	// �ړ����Ă����Ԃɂ���
	pPolice->bMove = true;

	//�x�@�ԗ��̒T�m����
	PatrolCarSearch(pPolice);

	if (pPolice->move.x >= 25.0f)
	{ // �ړ��ʂ����l�ȏ�̏ꍇ
		// �ړ��ʂ��X�V
		pPolice->move.x = 25.0f;
	}
}

//============================================================
// �p�g���[���ɖ߂鏈��
//============================================================
void PatrolBackAct(Police *pPolice)
{
	//�ŏ��̈ʒu�ƌ����ƈړ��ʁA���i�ނׂ����������Z�b�g����
	pPolice->pos = pPolice->posCopy;
	ReturnPoliceRot(pPolice);				//�ړ��ʂ̐ݒ菈��
	pPolice->move.x = 0.0f;
	pPolice->policeCurve.nNowCurve = 0;
}

//============================================================
// �x�@�̌����𒼂�����
//============================================================
void ReturnPoliceRot(Police *pPolice)
{
	if (pPolice->policeCurve.bCurveX[0] == true)
	{ // X�����ړ����Ă����ꍇ
		if (pPolice->policeCurve.bCurvePlus[0] == true)
		{ // �E�����Ɉړ����Ă����ꍇ
			// ������90�x�ɐݒ肷��
			pPolice->rot.y = D3DXToRadian(90);
		}
		else
		{ // �������Ɉړ����Ă����ꍇ
			// ������-90�x�ɐݒ肷��
			pPolice->rot.y = D3DXToRadian(-90);
		}
	}
	else
	{ // Z�����ړ����Ă����ꍇ
		if (pPolice->policeCurve.bCurvePlus[0] == true)
		{ // �������Ɉړ����Ă����ꍇ
			// ������0�x�ɐݒ肷��
			pPolice->rot.y = D3DXToRadian(0);
		}
		else
		{ // ��O�����Ɉړ����Ă����ꍇ
			if (pPolice->policeCurve.curveAngle[0] == CURVE_RIGHT)
			{ // �E�ɋȂ���ꍇ
				// ������-180�x�ɐݒ肷��
				pPolice->rot.y = D3DXToRadian(-180);
			}
			else if (pPolice->policeCurve.curveAngle[0] == CURVE_LEFT)
			{ // ���ɋȂ���ꍇ
				// ������180�x�ɐݒ肷��
				pPolice->rot.y = D3DXToRadian(180);

			}
		}
	}
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