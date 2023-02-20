//======================================================================================================================
//
//	���ꂫ���� [junk.cpp]
//	Author�F��������
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "calculation.h"
#include "input.h"
#include "model.h"

#include "junk.h"
#include "player.h"
#include "shadow.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define JUNK_GRAVITY		(-0.8f)		// ���ꂫ�̏d��
#define JUNK_LEV_MAGNI		(0.06f)		// ���ꂫ�̈ړ��ʂ̌����W��
#define JUNK_MOVE_ADD_MAGNI	(0.4f)		// ���ꂫ�ɕt���������̈ړ��ʂ̔{��
#define JUNK_MOVE_LEV		(-0.4f)		// ���ꂫ�̈ړ��ʂ̌����{��
#define JUNK_SCALE_MAGNI	(0.05f)		// ���ꂫ�̊g�嗦�̔{��
#define JUNK_COL_MAGNI		(0.05f)		// ���ꂫ�̐F�̌����{��
#define JUNK_POS_Y_ADD		(40.0f)		// ���ꂫ�̈ʒu�̉��Z��(Y��)

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
Junk g_aJunk[MAX_JUNK];			// ���ꂫ�̏��

//======================================================================================================================
//	���ꂫ�̏���������
//======================================================================================================================
void InitJunk(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

												// ���ꂫ�̏��̏�����
	for (int nCntJunk = 0; nCntJunk < MAX_JUNK; nCntJunk++)
	{ // ���ꂫ�̍ő�\�������J��Ԃ�

		// ��{���̏�����
		g_aJunk[nCntJunk].pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aJunk[nCntJunk].move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
		g_aJunk[nCntJunk].scale		= NONE_SCALE;						// �g�嗦
		g_aJunk[nCntJunk].scaleMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �g�嗦�̈ړ���
		g_aJunk[nCntJunk].rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aJunk[nCntJunk].rotMove	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����̈ړ���
		g_aJunk[nCntJunk].state		= JUNKSTATE_NONE;					// ���ꂫ�̏��
		g_aJunk[nCntJunk].nShadowID = NONE_SHADOW;						// �e�̃C���f�b�N�X
		g_aJunk[nCntJunk].bUse		= false;							// �g�p��

		// ���f�����̏�����
		g_aJunk[nCntJunk].modelData.dwNumMat = 0;					// �}�e���A���̐�
		g_aJunk[nCntJunk].modelData.pTexture = NULL;				// �e�N�X�`���ւ̃|�C���^
		g_aJunk[nCntJunk].modelData.pMesh = NULL;					// ���b�V�� (���_���) �ւ̃|�C���^
		g_aJunk[nCntJunk].modelData.pBuffMat = NULL;				// �}�e���A���ւ̃|�C���^
		g_aJunk[nCntJunk].modelData.vtxMin = INIT_VTX_MIN;			// �ŏ��̒��_���W
		g_aJunk[nCntJunk].modelData.vtxMax = INIT_VTX_MAX;			// �ő�̒��_���W
		g_aJunk[nCntJunk].modelData.size = INIT_SIZE;				// �傫��
		g_aJunk[nCntJunk].modelData.fRadius = 0.0f;					// ���a

		// �}�e���A���̃R�s�[��������
		g_aJunk[nCntJunk].matCopy[MAX_MATERIAL] = {};
	}

	//// �����蔻��̃Z�b�g�A�b�v
	//TxtSetCollision();

	//// �e�̔��a�̃Z�b�g�A�b�v
	//TxtSetShadow();
}

//======================================================================================================================
//	���ꂫ�̏I������
//======================================================================================================================
void UninitJunk(void)
{

}

//======================================================================================================================
//	���ꂫ�̍X�V����
//======================================================================================================================
void UpdateJunk(void)
{
	Player *pPlayer = GetPlayer();

	for (int nCntJunk = 0; nCntJunk < MAX_JUNK; nCntJunk++)
	{ // ���ꂫ�̍ő�\�������J��Ԃ�

		if (g_aJunk[nCntJunk].bUse == true)
		{ // ���ꂫ���g�p����Ă���ꍇ

			// �d�͂�������
			g_aJunk[nCntJunk].move.y += JUNK_GRAVITY;

			// �ړ�������
			g_aJunk[nCntJunk].pos += g_aJunk[nCntJunk].move;

			// �ړ��ʂ�����������
			g_aJunk[nCntJunk].move.x += (0.0f - g_aJunk[nCntJunk].move.x) * JUNK_LEV_MAGNI;
			g_aJunk[nCntJunk].move.z += (0.0f - g_aJunk[nCntJunk].move.z) * JUNK_LEV_MAGNI;

			// �p�x���X����
			g_aJunk[nCntJunk].rot += g_aJunk[nCntJunk].rotMove;

			// �����̐��K��
			RotNormalize(&g_aJunk[nCntJunk].rot.x);
			RotNormalize(&g_aJunk[nCntJunk].rot.y);
			RotNormalize(&g_aJunk[nCntJunk].rot.z);

			switch (g_aJunk[nCntJunk].state)
			{
			case JUNKSTATE_NONE:	// ���ꂫ

				// �v���C���[�̈ړ��ʂ�������
				g_aJunk[nCntJunk].pos.x += (pPlayer->pos.x - pPlayer->oldPos.x) * JUNK_MOVE_ADD_MAGNI;
				g_aJunk[nCntJunk].pos.z += (pPlayer->pos.z - pPlayer->oldPos.z) * JUNK_MOVE_ADD_MAGNI;

				if (g_aJunk[nCntJunk].pos.y <= 0.0f)
				{ // �n�ʂ�艺�ɍs�����ꍇ

					// �ʒu��␳����
					g_aJunk[nCntJunk].pos.y = 0.0f;

					// �ړ��ʂ����Z����
					g_aJunk[nCntJunk].move.y *= JUNK_MOVE_LEV;

					// ������Ԃɂ���
					g_aJunk[nCntJunk].state = JUNKSTATE_DELETE;

					// �g�嗦�̌������𑪂�
					g_aJunk[nCntJunk].scaleMove.x = (g_aJunk[nCntJunk].scale.x * JUNK_SCALE_MAGNI);
					g_aJunk[nCntJunk].scaleMove.y = (g_aJunk[nCntJunk].scale.y * JUNK_SCALE_MAGNI);
					g_aJunk[nCntJunk].scaleMove.z = (g_aJunk[nCntJunk].scale.z * JUNK_SCALE_MAGNI);
				}

				break;				// �����o��

			case JUNKSTATE_DELETE:	// �������

				// �g�嗦�������Ă���
				g_aJunk[nCntJunk].scale -= g_aJunk[nCntJunk].scaleMove;

				if (g_aJunk[nCntJunk].pos.y <= 0.0f)
				{ // �n�ʂ�艺�ɍs�����ꍇ

					// �ʒu��␳����
					g_aJunk[nCntJunk].pos.y = 0.0f;

					// �ړ��ʂ����Z����
					g_aJunk[nCntJunk].move.y *= -JUNK_MOVE_LEV;
				}

				if
				(
					g_aJunk[nCntJunk].scale.x <= 0.0f ||
					g_aJunk[nCntJunk].scale.y <= 0.0f ||
					g_aJunk[nCntJunk].scale.z <= 0.0f
				)
				{ // �g�嗦��0.0f�ɂȂ����ꍇ

					// �g�p���Ȃ�
					g_aJunk[nCntJunk].bUse = false;
				}

				break;				// �����o��
			}

			// �e�̈ʒu�ݒ菈��
			SetPositionShadow
			(
				g_aJunk[nCntJunk].nShadowID,
				g_aJunk[nCntJunk].pos,
				g_aJunk[nCntJunk].rot,
				g_aJunk[nCntJunk].scale
			);
		}
	}
}

//======================================================================================================================
//	���ꂫ�̕`�揈��
//======================================================================================================================
void DrawJunk(void)
{
	// �ϐ���錾
	D3DXMATRIX   mtxRot, mtxTrans, mtxScale;				// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntJunk = 0; nCntJunk < MAX_JUNK; nCntJunk++)
	{ // ���ꂫ�̍ő�\�������J��Ԃ�

		if (g_aJunk[nCntJunk].bUse == true)
		{ // ���ꂫ���g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aJunk[nCntJunk].mtxWorld);

			// �g�嗦�𔽉f
			D3DXMatrixScaling(&mtxScale, g_aJunk[nCntJunk].scale.x, g_aJunk[nCntJunk].scale.y, g_aJunk[nCntJunk].scale.z);
			D3DXMatrixMultiply(&g_aJunk[nCntJunk].mtxWorld, &g_aJunk[nCntJunk].mtxWorld, &mtxScale);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aJunk[nCntJunk].rot.y, g_aJunk[nCntJunk].rot.x, g_aJunk[nCntJunk].rot.z);
			D3DXMatrixMultiply(&g_aJunk[nCntJunk].mtxWorld, &g_aJunk[nCntJunk].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aJunk[nCntJunk].pos.x, g_aJunk[nCntJunk].pos.y, g_aJunk[nCntJunk].pos.z);
			D3DXMatrixMultiply(&g_aJunk[nCntJunk].mtxWorld, &g_aJunk[nCntJunk].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aJunk[nCntJunk].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = &g_aJunk[nCntJunk].matCopy[0];

			for (int nCntMat = 0; nCntMat < (int)g_aJunk[nCntJunk].modelData.dwNumMat; nCntMat++)
			{ // �}�e���A���̐����J��Ԃ�

				if (g_aJunk[nCntJunk].state == JUNKSTATE_DELETE)
				{ // ������Ԃ������ꍇ

					// �����x�����Z����
					g_aJunk[nCntJunk].matCopy[nCntMat].MatD3D.Diffuse.a -= JUNK_COL_MAGNI;

					if (g_aJunk[nCntJunk].matCopy[nCntMat].MatD3D.Diffuse.a <= 0.0f)
					{ // �����x��0.0f�ȉ��ɂȂ����ꍇ

						// �����x��0.0f�ɂ���
						g_aJunk[nCntJunk].matCopy[nCntJunk].MatD3D.Diffuse.a = 0.0f;
					}
				}

				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// �ʏ�

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aJunk[nCntJunk].modelData.pTexture[nCntMat]);

				if (g_aJunk[nCntJunk].scale != NONE_SCALE)
				{ // �g�嗦���ύX����Ă���ꍇ

					// ���_�@���̎������K����L���ɂ���
					pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
				}

				// ���f���̕`��
				g_aJunk[nCntJunk].modelData.pMesh->DrawSubset(nCntMat);

				// ���_�@���̎������K���𖳌��ɂ���
				pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
			}

			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	���ꂫ�̐ݒ菈��
//======================================================================================================================
void SetJunk(D3DXVECTOR3 pos, D3DXVECTOR3 rot, SCALETYPE scale, D3DMATERIAL9 col)
{
	// �|�C���^��錾
	D3DXMATERIAL *pMatModel;	// �}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntJunk = 0; nCntJunk < MAX_JUNK; nCntJunk++)
	{ // ���ꂫ�̍ő�\�������J��Ԃ�

		if (g_aJunk[nCntJunk].bUse == false)
		{ // ���ꂫ���g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aJunk[nCntJunk].pos = D3DXVECTOR3(pos.x, pos.y + JUNK_POS_Y_ADD, pos.z);		// �ʒu
			g_aJunk[nCntJunk].rot = rot;					// ����
			g_aJunk[nCntJunk].scale = NONE_SCALE;			// �g�嗦
			g_aJunk[nCntJunk].scaleMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �g�嗦�̈ړ���
			g_aJunk[nCntJunk].state = JUNKSTATE_NONE;		// ���ꂫ�̏��
			g_aJunk[nCntJunk].bUse = true;					// �g�p���Ă����Ԃɂ���

			// �ړ��ʂ�ݒ肷��
			g_aJunk[nCntJunk].move.x = (float)(rand() % 51 - 25);
			g_aJunk[nCntJunk].move.y = 0.0f;
			g_aJunk[nCntJunk].move.z = (float)(rand() % 51 - 25);

			// �����̈ړ��ʂ�ݒ肷��
			g_aJunk[nCntJunk].rotMove.x = (float)(rand() % 7 - 3) * 0.1f;
			g_aJunk[nCntJunk].rotMove.y = (float)(rand() % 7 - 3) * 0.1f;
			g_aJunk[nCntJunk].rotMove.z = (float)(rand() % 7 - 3) * 0.1f;

			switch (scale)
			{
			case SCALETYPE_SMALL:	// ���������ꂫ

				// �g�嗦��ݒ肷��
				g_aJunk[nCntJunk].scale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

				break;				// �����o��

			case SCALETYPE_NORMAL:	// ���ʂ̂��ꂫ

				// �g�嗦��ݒ肷��
				g_aJunk[nCntJunk].scale = D3DXVECTOR3(0.75f, 0.75f, 0.75f);

				break;				// �����o��

			case SCALETYPE_BIG:		// �傫�����ꂫ

				// �g�嗦��ݒ肷��
				g_aJunk[nCntJunk].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

				break;				// �����o��
			}

			// ���f������ݒ�
			g_aJunk[nCntJunk].modelData = GetModelData(MODELTYPE_OBJECT_JUNK + FROM_OBJECT);	// ���f�����

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMatModel = (D3DXMATERIAL*)g_aJunk[nCntJunk].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aJunk[nCntJunk].modelData.dwNumMat; nCntMat++, pMatModel++)
			{ // �}�e���A���̐����J��Ԃ�

				// �}�e���A�����R�s�[����
				g_aJunk[nCntJunk].matCopy[nCntMat] = *pMatModel;
			}

			// �F��ݒ肷��
			g_aJunk[nCntJunk].matCopy[0].MatD3D = col;

			// �e�̃C���f�b�N�X��ݒ�
			g_aJunk[nCntJunk].nShadowID = SetCircleShadow
			( // ����
				0.5f,									// ���l
				g_aJunk[nCntJunk].modelData.fRadius,	// ���a
				&g_aJunk[nCntJunk].nShadowID,			// �e�̐e�̉e�C���f�b�N�X
				&g_aJunk[nCntJunk].bUse					// �e�̐e�̎g�p��
			);

			// �e�̈ʒu�ݒ�
			SetPositionShadow(g_aJunk[nCntJunk].nShadowID, g_aJunk[nCntJunk].pos, g_aJunk[nCntJunk].rot, NONE_SCALE);

			// �����𔲂���
			break;
		}
	}
}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================
//**********************************************************************************************************************
//	���ꂫ�̑����擾����
//**********************************************************************************************************************
int GetNumJunk(void)
{
	// �ϐ���錾
	int nNumJunk = 0;	// ���ꂫ�̑����̊m�F�p

	for (int nCntJunk = 0; nCntJunk < MAX_JUNK; nCntJunk++)
	{ // ���ꂫ�̍ő�\�������J��Ԃ�

		if (g_aJunk[nCntJunk].bUse == true)
		{ // ���ꂫ���g�p����Ă���ꍇ

			// �J�E���^�[�����Z
			nNumJunk++;
		}
	}

	// �ϐ��̒l��Ԃ�
	return nNumJunk;	// ���ꂫ�̑���
}
#endif