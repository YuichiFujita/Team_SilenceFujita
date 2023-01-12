//======================================================================================================================
//
//	�I�u�W�F�N�g���� [object.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "input.h"
#include "model.h"

#include "object.h"
#include "particle.h"
#include "shadow.h"
#include "sound.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define OBJ_LIFE	(50)		// �I�u�W�F�N�g�̗̑�

#define DAMAGE_TIME_OBJ		(20)					// �_���[�W��Ԃ�ۂ���
#define UNR_TIME_OBJ		(DAMAGE_TIME_OBJ - 10)	// ���G��ԂɕύX���鎞��

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
Object g_aObject[MAX_OBJECT];	// �I�u�W�F�N�g�̏��

//======================================================================================================================
//	�I�u�W�F�N�g�̏���������
//======================================================================================================================
void InitObject(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// �I�u�W�F�N�g�̏��̏�����
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		g_aObject[nCntObject].pos           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_aObject[nCntObject].rot           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
		g_aObject[nCntObject].scale         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �g�嗦
		g_aObject[nCntObject].pModelData    = NULL;									// ���f�����
		g_aObject[nCntObject].state         = ACTIONSTATE_NONE;						// ���
		g_aObject[nCntObject].nLife         = 0;									// �̗�
		g_aObject[nCntObject].nBreakType    = BREAKTYPE_NONE;						// �����̎��
		g_aObject[nCntObject].nType         = 0;									// �I�u�W�F�N�g�̎��
		g_aObject[nCntObject].nCounterState = 0; 									// ��ԊǗ��J�E���^�[
		g_aObject[nCntObject].nShadowID     = NONE_SHADOW;							// �e�̃C���f�b�N�X
		g_aObject[nCntObject].bUse          = false;								// �g�p��
	}
}

//======================================================================================================================
//	�I�u�W�F�N�g�̏I������
//======================================================================================================================
void UninitObject(void)
{

}

//======================================================================================================================
//	�I�u�W�F�N�g�̍X�V����
//======================================================================================================================
void UpdateObject(void)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aObject[nCntObject].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			switch (g_aObject[nCntObject].nBreakType)
			{ // �����̎�ނ��Ƃ̏���
			case BREAKTYPE_NONE:	// ���Ȃ����

				// ����

				// �����𔲂���
				break;

			case BREAKTYPE_ON:		// ������

				if (g_aObject[nCntObject].nCounterState > 0)
				{ // �J�E���^�[�� 0���傫���ꍇ

					// �J�E���^�[�����Z
					g_aObject[nCntObject].nCounterState--;

					if (g_aObject[nCntObject].nCounterState == UNR_TIME_OBJ)
					{ // �J�E���^�[�����l�̏ꍇ

						// ���G��Ԃɂ���
						g_aObject[nCntObject].state = ACTIONSTATE_UNRIVALED;
					}
					else if (g_aObject[nCntObject].nCounterState <= 0)
					{ // �J�E���^�[�� 0�ȉ��̏ꍇ

						// �ʏ��Ԃɂ���
						g_aObject[nCntObject].state = ACTIONSTATE_NORMAL;
					}
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
void DrawObject(void)
{
	// �ϐ���錾
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL      blackMat;					// �}�e���A���f�[�^ (���{��)

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aObject[nCntObject].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aObject[nCntObject].mtxWorld);

			//�g�嗦�𔽉f
			D3DXMatrixScaling(&mtxScale, g_aObject[nCntObject].scale.x, g_aObject[nCntObject].scale.y, g_aObject[nCntObject].scale.z);
			D3DXMatrixMultiply(&g_aObject[nCntObject].mtxWorld, &g_aObject[nCntObject].mtxWorld, &mtxScale);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aObject[nCntObject].rot.y, g_aObject[nCntObject].rot.x, g_aObject[nCntObject].rot.z);
			D3DXMatrixMultiply(&g_aObject[nCntObject].mtxWorld, &g_aObject[nCntObject].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aObject[nCntObject].pos.x, g_aObject[nCntObject].pos.y, g_aObject[nCntObject].pos.z);
			D3DXMatrixMultiply(&g_aObject[nCntObject].mtxWorld, &g_aObject[nCntObject].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aObject[nCntObject].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aObject[nCntObject].pModelData->pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aObject[nCntObject].pModelData->dwNumMat; nCntMat++)
			{ // �}�e���A���̐����J��Ԃ�

				switch (g_aObject[nCntObject].state)
				{ // ��Ԃ��Ƃ̏���
				case ACTIONSTATE_DAMAGE:	// �_���[�W���

					// �ǂݍ��񂾃}�e���A������
					blackMat.MatD3D = pMat[nCntMat].MatD3D;

					// �g�U���E�����E���Ȕ�����Ԃɂ���
					blackMat.MatD3D.Diffuse  = D3DXCOLOR(0.3f, 0.3f, 0.0f, 1.0f);
					blackMat.MatD3D.Ambient  = D3DXCOLOR(0.3f, 0.3f, 0.0f, 1.0f);
					blackMat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&blackMat.MatD3D);			// ���{��

					// �����𔲂���
					break;

				default:					// ����ȊO�̏��

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// �ʏ�

					// �����𔲂���
					break;
				}

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aObject[nCntObject].pModelData->pTexture[nCntMat]);

				// ���f���̕`��
				g_aObject[nCntObject].pModelData->pMesh->DrawSubset(nCntMat);
			}

			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	�I�u�W�F�N�g�̐ݒ菈��
//======================================================================================================================
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXMATERIAL *pMat, int nBreakType, int nType)
{
	// �|�C���^��錾
	D3DXMATERIAL *pMatModel;		// �}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aObject[nCntObject].bUse == false)
		{ // �I�u�W�F�N�g���g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aObject[nCntObject].pos        = pos;			// �ʒu
			g_aObject[nCntObject].rot        = rot;			// ����
			g_aObject[nCntObject].scale      = scale;		// �g�嗦
			g_aObject[nCntObject].nBreakType = nBreakType;	// �����̎��
			g_aObject[nCntObject].nType      = nType;		// �I�u�W�F�N�g�̎��

			// �I�u�W�F�N�g�̏���������
			g_aObject[nCntObject].state         = ACTIONSTATE_NORMAL;	// ���
			g_aObject[nCntObject].nLife         = OBJ_LIFE;				// �̗�
			g_aObject[nCntObject].nCounterState = 0; 					// ��ԊǗ��J�E���^�[

			// �g�p���Ă����Ԃɂ���
			g_aObject[nCntObject].bUse = true;

			// ���f������ݒ�
			g_aObject[nCntObject].pModelData = GetModelData(nType + FROM_OBJECT);	// ���f�����

			// �ő�l�𔽉f����
			g_aObject[nCntObject].pModelData->vtxMax.x *= g_aObject[nCntObject].scale.x;
			g_aObject[nCntObject].pModelData->vtxMax.y *= g_aObject[nCntObject].scale.y;
			g_aObject[nCntObject].pModelData->vtxMax.z *= g_aObject[nCntObject].scale.z;

			// �ŏ��l�𔽉f����
			g_aObject[nCntObject].pModelData->vtxMin.x *= g_aObject[nCntObject].scale.x;
			g_aObject[nCntObject].pModelData->vtxMin.y *= g_aObject[nCntObject].scale.y;
			g_aObject[nCntObject].pModelData->vtxMin.z *= g_aObject[nCntObject].scale.z;

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMatModel = (D3DXMATERIAL*)g_aObject[nCntObject].pModelData->pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aObject[nCntObject].pModelData->dwNumMat; nCntMat++)
			{ // �}�e���A���̐����J��Ԃ�

				// �����̐F����
				pMatModel[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Ambient;
				pMatModel[nCntMat].MatD3D.Diffuse = pMat[nCntMat].MatD3D.Diffuse;
			}

			// �e�̃C���f�b�N�X��ݒ�
			g_aObject[nCntObject].nShadowID = SetShadow
			( // ����
				0.5f,																																// ���l
				fabsf(g_aObject[g_aObject[nCntObject].nType].pModelData->vtxMax.x - g_aObject[g_aObject[nCntObject].nType].pModelData->vtxMin.x),	// ���a
				&g_aObject[nCntObject].nShadowID,																									// �e�̐e�̉e�C���f�b�N�X
				&g_aObject[nCntObject].bUse																											// �e�̐e�̎g�p��
			);

			// �e�̈ʒu�ݒ�
			SetPositionShadow(g_aObject[nCntObject].nShadowID, g_aObject[nCntObject].pos);

			// �����𔲂���
			break;
		}
	}
}

#if 0
//======================================================================================================================
//	�I�u�W�F�N�g�̃_���[�W����
//======================================================================================================================
void HitObject(Object *pObject, int nDamage)
{
	if (pObject->state == ACTIONSTATE_NORMAL)
	{ // �I�u�W�F�N�g���ʏ��Ԃ̏ꍇ

		// �����̃_���[�W����̗͂��猸�Z
		pObject->nLife -= nDamage;

		if (pObject->nLife > 0)
		{ // �̗͂��c���Ă���ꍇ

			// �_���[�W��Ԃɂ���
			pObject->state = ACTIONSTATE_DAMAGE;

			// �p�[�e�B�N���̐ݒ�
			SetParticle
			( // ����
				pObject->pos,						// �ʒu
				D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f),	// �F
				PARTICLETYPE_DAMAGE,				// ���
				SPAWN_PARTICLE_DAMAGE,				// �G�t�F�N�g��
				2									// ����
			);

			// �J�E���^�[��ݒ�
			pObject->nCounterState = DAMAGE_TIME_OBJ;

			// �T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_DMG);			// SE (�_���[�W)
		}
		else
		{ // �̗͂��s�����ꍇ

			// �����̐ݒ�
			SetExplosion(pObject->pos, SOUNDTYPE_BREAK);

			// �p�[�e�B�N���̐ݒ�
			SetParticle
			( // ����
				pObject->pos,						// �ʒu
				D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f),	// �F
				PARTICLETYPE_EXPLOSION,				// ���
				SPAWN_PARTICLE_EXPLOSION,			// �G�t�F�N�g��
				2									// ����
			);

			// �A�C�e���̐ݒ�
			SetItem(pObject->pos, ITEMTYPE_HEAL);

			// �g�p���Ă��Ȃ���Ԃɂ���
			pObject->bUse = false;
		}
	}
}
#endif

//======================================================================================================================
//	�I�u�W�F�N�g�Ƃ̓����蔻��
//======================================================================================================================
void CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pOldPos, float fWidth, float fDepth)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aObject[nCntObject].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// �O��̓����蔻��
			if (pPos->x + fWidth > g_aObject[nCntObject].pos.x + g_aObject[nCntObject].pModelData->vtxMin.x
			&&  pPos->x - fWidth < g_aObject[nCntObject].pos.x + g_aObject[nCntObject].pModelData->vtxMax.x)
			{ // �u���b�N�̍��E�͈͓̔��̏ꍇ

				if (pPos->z    + fDepth >  g_aObject[nCntObject].pos.z + g_aObject[nCntObject].pModelData->vtxMin.z
				&&  pOldPos->z + fDepth <= g_aObject[nCntObject].pos.z + g_aObject[nCntObject].pModelData->vtxMin.z)
				{ // �O����̓����蔻��

					// �ʒu��␳
					pPos->z = g_aObject[nCntObject].pos.z + g_aObject[nCntObject].pModelData->vtxMin.z - fDepth - 0.01f;
				}
				else if (pPos->z    - fDepth <  g_aObject[nCntObject].pos.z + g_aObject[nCntObject].pModelData->vtxMax.z
				     &&  pOldPos->z - fDepth >= g_aObject[nCntObject].pos.z + g_aObject[nCntObject].pModelData->vtxMax.z)
				{ // ��납��̓����蔻��

					// �ʒu��␳
					pPos->z = g_aObject[nCntObject].pos.z + g_aObject[nCntObject].pModelData->vtxMax.z + fDepth + 0.01f;
				}
			}

			// ���E�̓����蔻��
			if (pPos->z + fDepth > g_aObject[nCntObject].pos.z + g_aObject[nCntObject].pModelData->vtxMin.z
			&&  pPos->z - fDepth < g_aObject[nCntObject].pos.z + g_aObject[nCntObject].pModelData->vtxMax.z)
			{ // �u���b�N�̑O��͈͓̔��̏ꍇ

				if (pPos->x    + fWidth >  g_aObject[nCntObject].pos.x + g_aObject[nCntObject].pModelData->vtxMin.x
				&&  pOldPos->x + fWidth <= g_aObject[nCntObject].pos.x + g_aObject[nCntObject].pModelData->vtxMin.x)
				{ // ������̓����蔻��

					// �ʒu��␳
					pPos->x = g_aObject[nCntObject].pos.x + g_aObject[nCntObject].pModelData->vtxMin.x - fWidth - 0.01f;
				}
				else if (pPos->x    - fWidth <  g_aObject[nCntObject].pos.x + g_aObject[nCntObject].pModelData->vtxMax.x
				     &&  pOldPos->x - fWidth >= g_aObject[nCntObject].pos.x + g_aObject[nCntObject].pModelData->vtxMax.x)
				{ // �E����̓����蔻��
					
					// �ʒu��␳
					pPos->x = g_aObject[nCntObject].pos.x + g_aObject[nCntObject].pModelData->vtxMax.x + fWidth + 0.01f;
				}
			}
		}
	}
}

//======================================================================================================================
//	�I�u�W�F�N�g�̎擾����
//======================================================================================================================
Object *GetObjectData(void)
{
	// �I�u�W�F�N�g�̏��̐擪�A�h���X��Ԃ�
	return &g_aObject[0];
}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================
//**********************************************************************************************************************
//	�I�u�W�F�N�g�̑����擾����
//**********************************************************************************************************************
int GetNumObject(void)
{
	// �ϐ���錾
	int nNumObject = 0;	// �I�u�W�F�N�g�̑����̊m�F�p

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aObject[nCntObject].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// �J�E���^�[�����Z
			nNumObject++;
		}
	}

	// �ϐ��̒l��Ԃ�
	return nNumObject;	// �I�u�W�F�N�g�̑���
}
#endif