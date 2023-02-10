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
#include "shadow.h"
#include "sound.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define GATE_GRAVITY		(1.0f)		// �d��

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
Gate      g_aGate[MAX_GATE];			// �I�u�W�F�N�g�̏��
Collision g_aCollGate[MODEL_GATE_MAX];	// �����蔻��̏��

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

		// ��{���̏�����
		g_aGate[nCntGate].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_aGate[nCntGate].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
		g_aGate[nCntGate].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
		g_aGate[nCntGate].state = GATESTATE_FLY;					// ���
		g_aGate[nCntGate].bUse = false;								// �g�p��

		// �����蔻����̏�����
		for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
		{ // �����蔻��̍ő吔���J��Ԃ�

			g_aGate[nCntGate].collInfo.vecPos[nCntColl] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu�x�N�g��
			g_aGate[nCntGate].collInfo.fWidth[nCntColl] = 0.0f;								// ����
			g_aGate[nCntGate].collInfo.fDepth[nCntColl] = 0.0f;								// ���s
		}

		// ���f�����̏�����
		g_aGate[nCntGate].modelData.dwNumMat = 0;					// �}�e���A���̐�
		g_aGate[nCntGate].modelData.pTexture = NULL;				// �e�N�X�`���ւ̃|�C���^
		g_aGate[nCntGate].modelData.pMesh = NULL;					// ���b�V�� (���_���) �ւ̃|�C���^
		g_aGate[nCntGate].modelData.pBuffMat = NULL;				// �}�e���A���ւ̃|�C���^
		g_aGate[nCntGate].modelData.vtxMin = INIT_VTX_MIN;			// �ŏ��̒��_���W
		g_aGate[nCntGate].modelData.vtxMax = INIT_VTX_MAX;			// �ő�̒��_���W
		g_aGate[nCntGate].modelData.size = INIT_SIZE;				// �傫��
		g_aGate[nCntGate].modelData.fRadius = 0.0f;					// ���a
	}

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

	// �Q�[�g�̐ݒ菈��
	SetGate(D3DXVECTOR3(0.0f, 300.0f, 700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
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
	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (GetKeyboardTrigger(DIK_RSHIFT) == true)
		{ // �E�V�t�g�L�[���������ꍇ
			// ������Ԃɂ���
			g_aGate[nCntGate].state = GATESTATE_LAND;
		}

		if (g_aGate[nCntGate].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ
			switch (g_aGate[nCntGate].state)
			{
			case GATESTATE_FLY:		// �����

				// �ړ��ʂ�0�ɂ���
				g_aGate[nCntGate].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				break;				// �����o��

			case GATESTATE_LAND:	// �������

				// �d�͂�������
				g_aGate[nCntGate].move.y -= GATE_GRAVITY;

				// �ʒu���X�V����
				g_aGate[nCntGate].pos.y += g_aGate[nCntGate].move.y;

				if (g_aGate[nCntGate].pos.y <= 0.0f)
				{ // �n�ʂɓ��B�����ꍇ
					// �ʒu��␳����
					g_aGate[nCntGate].pos.y = 0.0f;

					// �ړ��ʂ�ݒ肷��
					g_aGate[nCntGate].move = D3DXVECTOR3(0.0f, 8.0f, 0.0f);

					// �o�E���h��Ԃɂ���
					g_aGate[nCntGate].state = GATESTATE_BOUND;
				}

				break;				// �����o��

			case GATESTATE_BOUND:	// �o�E���h���

				// �d�͂�������
				g_aGate[nCntGate].move.y -= GATE_GRAVITY;

				// �ʒu���X�V����
				g_aGate[nCntGate].pos.y += g_aGate[nCntGate].move.y;

				if (g_aGate[nCntGate].pos.y <= 0.0f)
				{ // �n�ʂɓ��B�����ꍇ
					// �ʒu��␳����
					g_aGate[nCntGate].pos.y = 0.0f;

					// �ړ��ʂ�0�ɂ���
					g_aGate[nCntGate].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					// ��~��Ԃɂ���
					g_aGate[nCntGate].state = GATESTATE_STOP;
				}

				break;				// �����o��

			case GATESTATE_STOP:	// ��~���

				// �ړ��ʂ�0�ɂ���
				g_aGate[nCntGate].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				break;				// �����o��
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

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

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

			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	�I�u�W�F�N�g�̐ݒ菈��
//======================================================================================================================
void SetGate(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aGate[nCntGate].bUse == false)
		{ // �I�u�W�F�N�g���g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aGate[nCntGate].pos = pos;					// �ʒu
			g_aGate[nCntGate].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
			g_aGate[nCntGate].rot = rot;					// ����
			g_aGate[nCntGate].state = GATESTATE_FLY;		// ���

			// �g�p���Ă����Ԃɂ���
			g_aGate[nCntGate].bUse = true;

			// ���f������ݒ�
			g_aGate[nCntGate].modelData = GetModelData(MODELTYPE_OBJECT_BAKERY + FROM_OBJECT);	// ���f�����

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

	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aGate[nCntGate].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ
			if (g_aGate[nCntGate].state != GATESTATE_FLY)
			{ // �󒆏�ԈȊO�̏ꍇ
				// �O��̓����蔻��
				if (pPos->x + fWidth > g_aGate[nCntGate].pos.x + g_aGate[nCntGate].modelData.vtxMin.x
					&&  pPos->x - fWidth < g_aGate[nCntGate].pos.x + g_aGate[nCntGate].modelData.vtxMax.x)
				{ // �u���b�N�̍��E�͈͓̔��̏ꍇ

					if (pPos->z + fDepth >  g_aGate[nCntGate].pos.z + g_aGate[nCntGate].modelData.vtxMin.z
						&&  pOldPos->z + fDepth <= g_aGate[nCntGate].pos.z + g_aGate[nCntGate].modelData.vtxMin.z)
					{ // �O����̓����蔻��

						// �ʒu��␳
						pPos->z = g_aGate[nCntGate].pos.z + g_aGate[nCntGate].modelData.vtxMin.z - fDepth - 0.01f;

						// �ړ��ʂ��폜
						pMove->x *= 0.95f;
					}
					else if (pPos->z - fDepth <  g_aGate[nCntGate].pos.z + g_aGate[nCntGate].modelData.vtxMax.z
						&&  pOldPos->z - fDepth >= g_aGate[nCntGate].pos.z + g_aGate[nCntGate].modelData.vtxMax.z)
					{ // ��납��̓����蔻��

						// �ʒu��␳
						pPos->z = g_aGate[nCntGate].pos.z + g_aGate[nCntGate].modelData.vtxMax.z + fDepth + 0.01f;

						// �ړ��ʂ��폜
						pMove->x *= 0.95f;
					}
				}

				// ���E�̓����蔻��
				if (pPos->z + fDepth > g_aGate[nCntGate].pos.z + g_aGate[nCntGate].modelData.vtxMin.z
					&&  pPos->z - fDepth < g_aGate[nCntGate].pos.z + g_aGate[nCntGate].modelData.vtxMax.z)
				{ // �u���b�N�̑O��͈͓̔��̏ꍇ

					if (pPos->x + fWidth >  g_aGate[nCntGate].pos.x + g_aGate[nCntGate].modelData.vtxMin.x
						&&  pOldPos->x + fWidth <= g_aGate[nCntGate].pos.x + g_aGate[nCntGate].modelData.vtxMin.x)
					{ // ������̓����蔻��

						// �ʒu��␳
						pPos->x = g_aGate[nCntGate].pos.x + g_aGate[nCntGate].modelData.vtxMin.x - fWidth - 0.01f;

						// �ړ��ʂ��폜
						pMove->x *= 0.95f;
					}
					else if (pPos->x - fWidth <  g_aGate[nCntGate].pos.x + g_aGate[nCntGate].modelData.vtxMax.x
						&&  pOldPos->x - fWidth >= g_aGate[nCntGate].pos.x + g_aGate[nCntGate].modelData.vtxMax.x)
					{ // �E����̓����蔻��

						// �ʒu��␳
						pPos->x = g_aGate[nCntGate].pos.x + g_aGate[nCntGate].modelData.vtxMax.x + fWidth + 0.01f;

						// �ړ��ʂ��폜
						pMove->x *= 0.95f;
					}
				}
			}
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