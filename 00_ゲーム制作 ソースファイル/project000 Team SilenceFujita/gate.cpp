//======================================================================================================================
//
//	�Q�[�g���� [gate.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "calculation.h"
#include "input.h"
#include "model.h"

#include "gate.h"
#include "player.h"
#include "timer.h"
#include "particle.h"
#include "shadow.h"
#include "sound.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define GATE_GRAVITY			(1.0f)		// �d��
#define GATE_OPEN				(2.5f)		// �Q�[�g�̊J�����x
#define GATE_DOOR_PLUSPOS		(285.0f)	// �h�A�̏����ʒu�� y���W���Z��
#define GATE_EXIT_WIDESIZE_MUL	(0.3f)		// �E�o���̉��ʒu���Z�ʂ̔{��
#define GATE_EXIT_FORWARDPLUS	(150.0f)	// �E�o���̑O�����Z��

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
Gate      g_aGate[MAX_GATE];				// �I�u�W�F�N�g�̏��
Exit      g_exit;							// �E�o�̏��
Collision g_aCollGate[MODEL_GATE_MAX];		// �����蔻��̏��

//======================================================================================================================
//	�I�u�W�F�N�g�̏���������
//======================================================================================================================
void InitGate(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// �I�u�W�F�N�g�̏��̏�����
	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		//--------------------------------------------------------------------------------------------------------------
		//	��̏���������
		//--------------------------------------------------------------------------------------------------------------
		// ��{���̏�����
		g_aGate[nCntGate].pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aGate[nCntGate].rot   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aGate[nCntGate].state = GATESTATE_FLY;					// ���
		g_aGate[nCntGate].bUse  = false;							// �g�p��

		// ���f�����̏�����
		g_aGate[nCntGate].modelData.dwNumMat = 0;					// �}�e���A���̐�
		g_aGate[nCntGate].modelData.pTexture = NULL;				// �e�N�X�`���ւ̃|�C���^
		g_aGate[nCntGate].modelData.pMesh    = NULL;				// ���b�V�� (���_���) �ւ̃|�C���^
		g_aGate[nCntGate].modelData.pBuffMat = NULL;				// �}�e���A���ւ̃|�C���^
		g_aGate[nCntGate].modelData.vtxMin   = INIT_VTX_MIN;		// �ŏ��̒��_���W
		g_aGate[nCntGate].modelData.vtxMax   = INIT_VTX_MAX;		// �ő�̒��_���W
		g_aGate[nCntGate].modelData.size     = INIT_SIZE;			// �傫��
		g_aGate[nCntGate].modelData.fRadius  = 0.0f;				// ���a

		// �����蔻����̏�����
		for (int nCntCollGate = 0; nCntCollGate < MODEL_GATE_MAX; nCntCollGate++)
		{ // �Q�[�g�̎�ނ̑������J��Ԃ�

			for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
			{ // �����蔻��̍ő吔���J��Ԃ�

				g_aGate[nCntGate].collInfo[nCntCollGate].vecPos[nCntColl] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu�x�N�g��
				g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl] = 0.0f;							// ����
				g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl] = 0.0f;							// ���s
			}
		}

		//--------------------------------------------------------------------------------------------------------------
		//	�S�̏���������
		//--------------------------------------------------------------------------------------------------------------
		// ��{���̏�����
		g_aGate[nCntGate].doorData.fPos  = 0.0f;	// �ʒu
		g_aGate[nCntGate].doorData.fMove = 0.0f;	// �ړ���

		// ���f�����̏�����
		g_aGate[nCntGate].doorData.modelData.dwNumMat = 0;					// �}�e���A���̐�
		g_aGate[nCntGate].doorData.modelData.pTexture = NULL;				// �e�N�X�`���ւ̃|�C���^
		g_aGate[nCntGate].doorData.modelData.pMesh    = NULL;				// ���b�V�� (���_���) �ւ̃|�C���^
		g_aGate[nCntGate].doorData.modelData.pBuffMat = NULL;				// �}�e���A���ւ̃|�C���^
		g_aGate[nCntGate].doorData.modelData.vtxMin   = INIT_VTX_MIN;		// �ŏ��̒��_���W
		g_aGate[nCntGate].doorData.modelData.vtxMax   = INIT_VTX_MAX;		// �ő�̒��_���W
		g_aGate[nCntGate].doorData.modelData.size     = INIT_SIZE;			// �傫��
		g_aGate[nCntGate].doorData.modelData.fRadius  = 0.0f;				// ���a
	}

	// �E�o�̏��̏�����
	g_exit.pGate = NULL;	// �E�o�Q�[�g�̃A�h���X
	g_exit.bExit = false;	// �E�o��

	// �����蔻��̏��̏�����
	for (int nCntGate = 0; nCntGate < MODEL_GATE_MAX; nCntGate++)
	{ // �Q�[�g�̎�ނ̑������J��Ԃ�

		// �����蔻��̑�����������
		g_aCollGate[nCntGate].nNumColl = 1;

		for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
		{ // �����蔻��̍ő吔���J��Ԃ�

			g_aCollGate[nCntGate].vecPos[nCntColl] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu�x�N�g��
			g_aCollGate[nCntGate].scale[nCntColl]  = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// �g�嗦
			g_aCollGate[nCntGate].fWidth[nCntColl] = 0.0f;							// ����
			g_aCollGate[nCntGate].fDepth[nCntColl] = 0.0f;							// ���s
		}
	}
}

//======================================================================================================================
//	�I�u�W�F�N�g�̏I������
//======================================================================================================================
void UninitGate(void)
{

}

//======================================================================================================================
//	�I�u�W�F�N�g�̍X�V����
//======================================================================================================================
void UpdateGate(void)
{
	if (GetKeyboardTrigger(DIK_0) == true)
	{
		AllOpenGate();
	}

	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aGate[nCntGate].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			switch (g_aGate[nCntGate].state)
			{ // ��Ԃ��Ƃ̏���
			case GATESTATE_FLY:		// �����

				// �Q�[�g�̒E�o����
				CollisionExitGate(&g_aGate[nCntGate]);

				// �����𔲂���
				break;

			case GATESTATE_LAND:	// �������

				// �d�͂�������
				g_aGate[nCntGate].doorData.fMove -= GATE_GRAVITY;

				// �ʒu���X�V����
				g_aGate[nCntGate].doorData.fPos += g_aGate[nCntGate].doorData.fMove;

				if (g_aGate[nCntGate].doorData.fPos <= 0.0f)
				{ // �n�ʂɓ��B�����ꍇ

					// �ʒu��␳����
					g_aGate[nCntGate].doorData.fPos = 0.0f;

					// �ړ��ʂ�ݒ肷��
					g_aGate[nCntGate].doorData.fMove = 8.0f;

					// �o�E���h��Ԃɂ���
					g_aGate[nCntGate].state = GATESTATE_BOUND;
				}

				// �����𔲂���
				break;

			case GATESTATE_BOUND:	// �o�E���h���

				// �d�͂�������
				g_aGate[nCntGate].doorData.fMove -= GATE_GRAVITY;

				// �ʒu���X�V����
				g_aGate[nCntGate].doorData.fPos += g_aGate[nCntGate].doorData.fMove;

				if (g_aGate[nCntGate].doorData.fPos <= 0.0f)
				{ // �n�ʂɓ��B�����ꍇ

					// �ʒu��␳����
					g_aGate[nCntGate].doorData.fPos = 0.0f;

					// �ړ��ʂ�0�ɂ���
					g_aGate[nCntGate].doorData.fMove = 0.0f;

					// ��~��Ԃɂ���
					g_aGate[nCntGate].state = GATESTATE_STOP;
				}

				// �����𔲂���
				break;

			case GATESTATE_STOP:	// ��~���

				// ����

				// �����𔲂���
				break;

			case GATESTATE_OPEN:	// �J�����

				// �d�͂�������
				g_aGate[nCntGate].doorData.fMove = GATE_OPEN;

				// �ʒu���X�V����
				g_aGate[nCntGate].doorData.fPos += g_aGate[nCntGate].doorData.fMove;

				if (g_aGate[nCntGate].doorData.fPos >= GATE_DOOR_PLUSPOS)
				{ // �n�ʂɓ��B�����ꍇ

					// �ʒu��␳����
					g_aGate[nCntGate].doorData.fPos = GATE_DOOR_PLUSPOS;

					// �ړ��ʂ�ݒ肷��
					g_aGate[nCntGate].doorData.fMove = 0.0f;

					// �o�E���h��Ԃɂ���
					g_aGate[nCntGate].state = GATESTATE_FLY;
				}

				// �����𔲂���
				break;
			}
		}
	}
}

//======================================================================================================================
//	�I�u�W�F�N�g�̕`�揈��
//======================================================================================================================
void DrawGate(void)
{
	// �ϐ���錾
	D3DXMATRIX   mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aGate[nCntGate].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//----------------------------------------------------------------------------------------------------------
			//	��̕`��
			//----------------------------------------------------------------------------------------------------------
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aGate[nCntGate].mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aGate[nCntGate].rot.y, g_aGate[nCntGate].rot.x, g_aGate[nCntGate].rot.z);
			D3DXMatrixMultiply(&g_aGate[nCntGate].mtxWorld, &g_aGate[nCntGate].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aGate[nCntGate].pos.x, g_aGate[nCntGate].pos.y, g_aGate[nCntGate].pos.z);
			D3DXMatrixMultiply(&g_aGate[nCntGate].mtxWorld, &g_aGate[nCntGate].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aGate[nCntGate].mtxWorld);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aGate[nCntGate].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aGate[nCntGate].modelData.dwNumMat; nCntMat++)
			{ // �}�e���A���̐����J��Ԃ�

				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aGate[nCntGate].modelData.pTexture[nCntMat]);

				// ���f���̕`��
				g_aGate[nCntGate].modelData.pMesh->DrawSubset(nCntMat);
			}

			//----------------------------------------------------------------------------------------------------------
			//	�S�̕`��
			//----------------------------------------------------------------------------------------------------------
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aGate[nCntGate].doorData.mtxWorld);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, 0.0f, g_aGate[nCntGate].doorData.fPos, 0.0f);
			D3DXMatrixMultiply(&g_aGate[nCntGate].doorData.mtxWorld, &g_aGate[nCntGate].doorData.mtxWorld, &mtxTrans);

			// �Q�[�g�̃}�g���b�N�X�Ɗ|�����킹��
			D3DXMatrixMultiply
			( // ����
				&g_aGate[nCntGate].mtxWorld,
				&g_aGate[nCntGate].mtxWorld,
				&g_aGate[nCntGate].doorData.mtxWorld
			);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aGate[nCntGate].mtxWorld);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aGate[nCntGate].doorData.modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aGate[nCntGate].doorData.modelData.dwNumMat; nCntMat++)
			{ // �}�e���A���̐����J��Ԃ�

				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aGate[nCntGate].doorData.modelData.pTexture[nCntMat]);

				// ���f���̕`��
				g_aGate[nCntGate].doorData.modelData.pMesh->DrawSubset(nCntMat);
			}

			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	�I�u�W�F�N�g�̐ݒ菈��
//======================================================================================================================
void SetGate(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ROTSTATE stateRot, bool bOpen)
{
	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aGate[nCntGate].bUse == false)
		{ // �I�u�W�F�N�g���g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aGate[nCntGate].pos = pos;	// �ʒu
			g_aGate[nCntGate].rot = rot;	// ����

			if (bOpen == true)
			{ // �I�[�v����Ԃ̏ꍇ
				g_aGate[nCntGate].state = GATESTATE_FLY;				// ���
				g_aGate[nCntGate].doorData.fPos = GATE_DOOR_PLUSPOS;	// �ʒu
			}
			else
			{ // �N���[�Y��Ԃ������ꍇ
				g_aGate[nCntGate].state = GATESTATE_STOP;				// ���
				g_aGate[nCntGate].doorData.fPos = 0.0f;					// �ʒu
			}

			// ����������
			g_aGate[nCntGate].doorData.fMove = 0.0f;				// �ړ���

			// ���f������ݒ�
			g_aGate[nCntGate].modelData          = GetModelData(MODELTYPE_OBJECT_GATE000);	// ��̃��f�����
			g_aGate[nCntGate].doorData.modelData = GetModelData(MODELTYPE_OBJECT_GATE001);	// �S�̃��f�����

			// �g�p���Ă����Ԃɂ���
			g_aGate[nCntGate].bUse = true;

			// �����蔻�����ݒ�
			for (int nCntCollGate = 0; nCntCollGate < MODEL_GATE_MAX; nCntCollGate++)
			{ // �Q�[�g�̎�ނ̑������J��Ԃ�

				// ������Ԃ�ݒ�
				g_aGate[nCntGate].collInfo[nCntCollGate].stateRot = stateRot;

				for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
				{ // �����蔻��̍ő吔���J��Ԃ�

					// �����Əc�����v�Z
					if (stateRot == ROTSTATE_0
					||  stateRot == ROTSTATE_180)
					{ // �p�x��0�x�A�܂���180�x�̏ꍇ

						// ����T�C�Y��ݒ�
						g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl] = g_aCollGate[nCntCollGate].fWidth[nCntColl];
						g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl] = g_aCollGate[nCntCollGate].fDepth[nCntColl];
					}
					else
					{ // �p�x90�x�A�܂���270�x�̏ꍇ

						// ����T�C�Y��ݒ�
						g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl] = g_aCollGate[nCntCollGate].fDepth[nCntColl];
						g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl] = g_aCollGate[nCntCollGate].fWidth[nCntColl];
					}
				}

				// �����蔻���ݒ�
				for (int nCntColl = 0; nCntColl < g_aCollGate[nCntCollGate].nNumColl; nCntColl++)
				{ // �����蔻��̑������J��Ԃ�

					// �ʒu�x�N�g����ݒ�
					g_aGate[nCntGate].collInfo[nCntCollGate].vecPos[nCntColl] = g_aCollGate[nCntCollGate].vecPos[nCntColl];

					for (int nCntRot = ROTSTATE_0; nCntRot < stateRot; nCntRot++)
					{ // �ݒ肳�ꂽ�����ɂȂ�܂ŌJ��Ԃ�

						// �ʒu�x�N�g����90�x��]
						g_aGate[nCntGate].collInfo[nCntCollGate].vecPos[nCntColl] = D3DXVECTOR3
						( // ����
							-g_aGate[nCntGate].collInfo[nCntCollGate].vecPos[nCntColl].z,
							 g_aGate[nCntGate].collInfo[nCntCollGate].vecPos[nCntColl].y,
							 g_aGate[nCntGate].collInfo[nCntCollGate].vecPos[nCntColl].x
						);
					}
				}
			}

			// �����𔲂���
			break;
		}
	}
}

//======================================================================================================================
//	�I�u�W�F�N�g�Ƃ̓����蔻��
//======================================================================================================================
void CollisionGate(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pOldPos, D3DXVECTOR3 *pMove, float fWidth, float fDepth)
{
	// �ϐ���錾
	D3DXVECTOR3 collPos;	// �����蔻��̒��S���W
	int         nNumLoop;	// �J��Ԃ����̐ݒ�p

	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aGate[nCntGate].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// �J��Ԃ�����ݒ�
			nNumLoop = (g_aGate[nCntGate].state != GATESTATE_FLY) ? MODEL_GATE_MAX : 1;

			for (int nCntCollGate = 0; nCntCollGate < nNumLoop; nCntCollGate++)
			{ // �ݒ肳�ꂽ�J��Ԃ��������[�v

				for (int nCntColl = 0; nCntColl < g_aCollGate[nCntCollGate].nNumColl; nCntColl++)
				{ // �����蔻�萔���J��Ԃ�

					// �����蔻��̒��S���W��ݒ�
					collPos = g_aGate[nCntGate].pos - g_aGate[nCntGate].collInfo[nCntCollGate].vecPos[nCntColl];

					// �O��̓����蔻��
					if (pPos->x + fWidth > collPos.x - g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl]
					&&  pPos->x - fWidth < collPos.x + g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl])
					{ // �u���b�N�̍��E�͈͓̔��̏ꍇ

						if (pPos->z    + fDepth >  collPos.z - g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl]
						&&  pOldPos->z + fDepth <= collPos.z - g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl])
						{ // �O����̓����蔻��

							// �ʒu��␳
							pPos->z = collPos.z - g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl] - fDepth - 0.01f;

							if (pMove->x >= 13.0f)
							{ // �ړ��ʂ����ȏ�̏ꍇ

								// �p�[�e�B�N���̐ݒ菈��
								SetParticle
								( // ����
									D3DXVECTOR3(pPos->x, pPos->y + 50.0f, collPos.z - g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl]),	// �ʒu
									D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),	// �F
									PARTICLETYPE_SPARK,					// �p�[�e�B�N���̎��
									10,									// ������
									2									// ����
								);
							}

							// �ړ��ʂ��폜
							pMove->x *= 0.95f;
						}
						else if (pPos->z    - fDepth <  collPos.z + g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl]
						     &&  pOldPos->z - fDepth >= collPos.z + g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl])
						{ // ��납��̓����蔻��

							// �ʒu��␳
							pPos->z = collPos.z + g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl] + fDepth + 0.01f;

							if (pMove->x >= 13.0f)
							{ // �ړ��ʂ����ȏ�̏ꍇ

								// �p�[�e�B�N���̐ݒ菈��
								SetParticle
								( // ����
									D3DXVECTOR3(pPos->x, pPos->y + 50.0f, collPos.z + g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl]),	// �ʒu
									D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),	// �F
									PARTICLETYPE_SPARK,					// �p�[�e�B�N���̎��
									10,									// ������
									2									// ����
								);
							}

							// �ړ��ʂ��폜
							pMove->x *= 0.95f;
						}
					}

					// ���E�̓����蔻��
					if (pPos->z + fDepth > collPos.z - g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl]
					&&  pPos->z - fDepth < collPos.z + g_aGate[nCntGate].collInfo[nCntCollGate].fDepth[nCntColl])
					{ // �u���b�N�̑O��͈͓̔��̏ꍇ

						if (pPos->x    + fWidth >  collPos.x - g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl]
						&&  pOldPos->x + fWidth <= collPos.x - g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl])
						{ // ������̓����蔻��

							// �ʒu��␳
							pPos->x = collPos.x - g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl] - fWidth - 0.01f;

							if (pMove->x >= 13.0f)
							{ // �ړ��ʂ����ȏ�̏ꍇ

								// �p�[�e�B�N���̐ݒ菈��
								SetParticle
								( // ����
									D3DXVECTOR3(collPos.x - g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl], pPos->y + 50.0f, pPos->z),	// �ʒu
									D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),	// �F
									PARTICLETYPE_SPARK,					// �p�[�e�B�N���̎��
									10,									// ������
									2									// ����
								);
							}

							// �ړ��ʂ��폜
							pMove->x *= 0.95f;
						}
						else if (pPos->x    - fWidth <  collPos.x + g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl]
						     &&  pOldPos->x - fWidth >= collPos.x + g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl])
						{ // �E����̓����蔻��
							
							// �ʒu��␳
							pPos->x = collPos.x + g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl] + fWidth + 0.01f;

							if (pMove->x >= 13.0f)
							{ // �ړ��ʂ����ȏ�̏ꍇ

								// �p�[�e�B�N���̐ݒ菈��
								SetParticle
								( // ����
									D3DXVECTOR3(collPos.x + g_aGate[nCntGate].collInfo[nCntCollGate].fWidth[nCntColl], pPos->y + 50.0f, pPos->z),	// �ʒu
									D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),	// �F
									PARTICLETYPE_SPARK,					// �p�[�e�B�N���̎��
									10,									// ������
									2									// ����
								);
							}

							// �ړ��ʂ��폜
							pMove->x *= 0.95f;
						}
					}
				}
			}
		}
	}
}

//======================================================================================================================
//	�Q�[�g�̒E�o����
//======================================================================================================================
void CollisionExitGate(Gate *pGate)
{
	// �ϐ���錾
	D3DXVECTOR3 leftPos;			// �Q�[�g�̍��ʒu
	D3DXVECTOR3 rightPos;			// �Q�[�g�̉E�ʒu
	D3DXVECTOR3 leftForwardPos;		// �Q�[�g�̍��̑O���ʒu
	D3DXVECTOR3 rightForwardPos;	// �Q�[�g�̉E�̑O���ʒu
	float       fSideSize;			// �Q�[�g�̉���

	// �v���C���[��錾
	Player *pPlayer = GetPlayer();	// �v���C���[��錾

	if (GetTimerState() != TIMERSTATE_END)
	{ // �^�C�}�[���J�E���g�I����Ԃł͂Ȃ��ꍇ

		// �Q�[�g�̉������Z�ʂ�ݒ�
		fSideSize = pGate->modelData.size.x * GATE_EXIT_WIDESIZE_MUL;

		// ���ʒu�����߂�
		leftPos.x = pGate->pos.x - sinf(pGate->rot.y + (D3DX_PI * 0.5f)) * fSideSize;	// x
		leftPos.y = 0.0f;																// y
		leftPos.z = pGate->pos.z - cosf(pGate->rot.y + (D3DX_PI * 0.5f)) * fSideSize;	// z
		leftPos.x -= sinf(pGate->rot.y) * GATE_EXIT_FORWARDPLUS;	// x
		leftPos.z -= cosf(pGate->rot.y) * GATE_EXIT_FORWARDPLUS;	// z

		// ���̑O���ʒu�����߂�
		leftForwardPos = leftPos;
		leftForwardPos.x -= sinf(pGate->rot.y) * 1.0f;	// x
		leftForwardPos.z -= cosf(pGate->rot.y) * 1.0f;	// z

		// �E�ʒu�����߂�
		rightPos.x = pGate->pos.x + sinf(pGate->rot.y + (D3DX_PI * 0.5f)) * fSideSize;	// x
		rightPos.y = 0.0f;																// y
		rightPos.z = pGate->pos.z + cosf(pGate->rot.y + (D3DX_PI * 0.5f)) * fSideSize;	// z
		rightPos.x -= sinf(pGate->rot.y) * GATE_EXIT_FORWARDPLUS;	// x
		rightPos.z -= cosf(pGate->rot.y) * GATE_EXIT_FORWARDPLUS;	// z

		// �E�̑O���ʒu�����߂�
		rightForwardPos = rightPos;
		rightForwardPos.x -= sinf(pGate->rot.y) * 1.0f;	// x
		rightForwardPos.z -= cosf(pGate->rot.y) * 1.0f;	// z

		if (LineOuterProduct(leftPos,  rightPos,        pPlayer->pos) < 0
		&&  LineOuterProduct(leftPos,  leftForwardPos,  pPlayer->pos) < 0
		&&  LineOuterProduct(rightPos, rightForwardPos, pPlayer->pos) > 0)
		{ // �Q�[�g��ʂ����ꍇ

			// �E�o�Q�[�g�̃A�h���X����
			g_exit.pGate = pGate;

			// �E�o������Ԃɂ���
			g_exit.bExit = true;
		}
	}
}

//======================================================================================================================
//	�Q�[�g�̑S�ߏ���
//======================================================================================================================
void AllShutOutGate(void)
{
	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aGate[nCntGate].bUse == true && g_aGate[nCntGate].state != GATESTATE_STOP && g_aGate[nCntGate].state != GATESTATE_OPEN)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// ������Ԃɂ���
			g_aGate[nCntGate].state = GATESTATE_LAND;
		}
	}
}

//======================================================================================================================
//	�Q�[�g�̑S�J������
//======================================================================================================================
void AllOpenGate(void)
{
	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aGate[nCntGate].bUse == true && g_aGate[nCntGate].state != GATESTATE_FLY)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// �J����Ԃɂ���
			g_aGate[nCntGate].state = GATESTATE_OPEN;
		}
	}
}

//======================================================================================================================
//	�I�u�W�F�N�g�̎擾����
//======================================================================================================================
Gate *GetGateData(void)
{
	// �I�u�W�F�N�g�̏��̐擪�A�h���X��Ԃ�
	return &g_aGate[0];
}

//======================================================================================================================
//	�E�o�̏��̎擾����
//======================================================================================================================
Exit GetExit(void)
{
	// �E�o�̏���Ԃ�
	return g_exit;
}

//======================================================================================================================
//	�Q�[�g�̓����蔻��̎擾����
//======================================================================================================================
Collision *GetCollGate(void)
{
	// �Q�[�g�̓����蔻��̏��̐擪�A�h���X��Ԃ�
	return &g_aCollGate[0];
}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================
//**********************************************************************************************************************
//	�I�u�W�F�N�g�̑����擾����
//**********************************************************************************************************************
int GetNumGate(void)
{
	// �ϐ���錾
	int nNumGate = 0;	// �I�u�W�F�N�g�̑����̊m�F�p

	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aGate[nCntGate].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// �J�E���^�[�����Z
			nNumGate++;
		}
	}

	// �ϐ��̒l��Ԃ�
	return nNumGate;	// �I�u�W�F�N�g�̑���
}
#endif