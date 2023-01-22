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
#include "calculation.h"
#include "input.h"
#include "model.h"

#include "object.h"
#include "particle.h"
#include "shadow.h"
#include "sound.h"

#ifdef _DEBUG	// �f�o�b�O����
#include "game.h"
#endif

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define OBJ_LIFE	(50)		// �I�u�W�F�N�g�̗̑�

#define DAMAGE_TIME_OBJ		(20)					// �_���[�W��Ԃ�ۂ���
#define UNR_TIME_OBJ		(DAMAGE_TIME_OBJ - 10)	// ���G��ԂɕύX���鎞��

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
Object    g_aObject[MAX_OBJECT];		// �I�u�W�F�N�g�̏��
Collision g_aCollision[MODEL_OBJ_MAX];	// �����蔻��̏��


void a(int nCntObject, D3DXVECTOR3 *pPos, D3DXVECTOR3 *pOldPos);


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

		// ��{���̏�����
		g_aObject[nCntObject].pos            = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_aObject[nCntObject].rot            = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
		g_aObject[nCntObject].scale          = D3DXVECTOR3(1.0f, 1.0f, 1.0f);		// �g�嗦
		g_aObject[nCntObject].state          = ACTIONSTATE_NONE;					// ���
		g_aObject[nCntObject].nLife          = 0;									// �̗�
		g_aObject[nCntObject].nCollisionType = COLLISIONTYPE_NONE;					// �����蔻��̎��
		g_aObject[nCntObject].nShadowType    = SHADOWTYPE_NONE;						// �e�̎��
		g_aObject[nCntObject].nBreakType     = BREAKTYPE_NONE;						// �����̎��
		g_aObject[nCntObject].nType          = 0;									// �I�u�W�F�N�g�̎��
		g_aObject[nCntObject].nCounterState  = 0; 									// ��ԊǗ��J�E���^�[
		g_aObject[nCntObject].nShadowID      = NONE_SHADOW;							// �e�̃C���f�b�N�X
		g_aObject[nCntObject].bUse           = false;								// �g�p��

		// ���f�����̏�����
		g_aObject[nCntObject].modelData.dwNumMat = 0;								// �}�e���A���̐�
		g_aObject[nCntObject].modelData.pTexture = NULL;							// �e�N�X�`���ւ̃|�C���^
		g_aObject[nCntObject].modelData.pMesh    = NULL;							// ���b�V�� (���_���) �ւ̃|�C���^
		g_aObject[nCntObject].modelData.pBuffMat = NULL;							// �}�e���A���ւ̃|�C���^
		g_aObject[nCntObject].modelData.dwNumMat = 0;								// �}�e���A���̐�
		g_aObject[nCntObject].modelData.vtxMin   = INIT_VTX_MIN;					// �ŏ��̒��_���W
		g_aObject[nCntObject].modelData.vtxMax   = INIT_VTX_MAX;					// �ő�̒��_���W
		g_aObject[nCntObject].modelData.size     = INIT_SIZE;						// �傫��
		g_aObject[nCntObject].modelData.fRadius  = 0.0f;							// ���a

		// �}�e���A���̃R�s�[��������
		g_aObject[nCntObject].matCopy[MAX_MATERIAL] = {};

#ifdef _DEBUG	// �f�o�b�O����
		// �G�f�B�b�g���̏��
		g_aObject[nCntObject].editState = OBJECTSTATE_NONE;
#endif
	}

	// �����蔻��̏��̏�����
	for (int nCntObject = 0; nCntObject < MODEL_OBJ_MAX; nCntObject++)
	{ // �I�u�W�F�N�g�̎�ނ̑������J��Ԃ�

		g_aCollision[nCntObject].pos      = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aCollision[nCntObject].vecPos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu�x�N�g��
		g_aCollision[nCntObject].rot      = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aCollision[nCntObject].stateRot = ROTSTATE_0;						// �������
		g_aCollision[nCntObject].scale    = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// �g�嗦
		g_aCollision[nCntObject].fWidth   = 0.0f;							// ����
		g_aCollision[nCntObject].fHeight  = 0.0f;							// �c��
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

#ifdef _DEBUG	// �f�o�b�O����
	D3DXMATERIAL deleteMat;						// �폜�}�e���A���f�[�^ (��)
#endif

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aObject[nCntObject].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aObject[nCntObject].mtxWorld);

			// �g�嗦�𔽉f
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
			pMat = &g_aObject[nCntObject].matCopy[0];

			for (int nCntMat = 0; nCntMat < (int)g_aObject[nCntObject].modelData.dwNumMat; nCntMat++)
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

#ifdef _DEBUG	// �f�o�b�O����
				if (GetGameMode() == GAMEMODE_EDIT)
				{ // �G�f�B�b�g���[�h�̏ꍇ

					switch (g_aObject[nCntObject].editState)
					{ // ��Ԃ��Ƃ̏���
					case OBJECTSTATE_DELETETARGET:	// �폜�Ώ�

						// �g�U���E�����E���Ȕ�����Ԃɂ���
						deleteMat.MatD3D.Diffuse  = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						deleteMat.MatD3D.Ambient  = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						deleteMat.MatD3D.Emissive = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						// �}�e���A���̐ݒ�
						pDevice->SetMaterial(&deleteMat.MatD3D);		// ��

						// �����𔲂���
						break;

					default:						// ����ȊO�̏��

						// �}�e���A���̐ݒ�
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// �ʏ�

						// �����𔲂���
						break;
					}
				}
#endif

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aObject[nCntObject].modelData.pTexture[nCntMat]);

				if (g_aObject[nCntObject].scale != NONE_SCALE)
				{ // �g�嗦���ύX����Ă���ꍇ

					// ���_�@���̎������K����L���ɂ���
					pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
				}

				// ���f���̕`��
				g_aObject[nCntObject].modelData.pMesh->DrawSubset(nCntMat);

				// ���_�@���̎������K���𖳌��ɂ���
				pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
			}

			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	�I�u�W�F�N�g�̐ݒ菈��
//======================================================================================================================
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXMATERIAL *pMat, int nType, int nBreakType, int nShadowType, int nCollisionType)
{
	// �|�C���^��錾
	D3DXMATERIAL *pMatModel;		// �}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aObject[nCntObject].bUse == false)
		{ // �I�u�W�F�N�g���g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aObject[nCntObject].pos            = pos;					// �ʒu
			g_aObject[nCntObject].rot            = rot;					// ����
			g_aObject[nCntObject].scale          = scale;				// �g�嗦
			g_aObject[nCntObject].nType          = nType;				// �I�u�W�F�N�g�̎��
			g_aObject[nCntObject].nBreakType     = nBreakType;			// �����̎��
			g_aObject[nCntObject].nShadowType    = nShadowType;			// �e�̎��
			g_aObject[nCntObject].nCollisionType = nCollisionType;		// �����蔻��̎��

			// �I�u�W�F�N�g�̏���������
			g_aObject[nCntObject].state         = ACTIONSTATE_NORMAL;	// ���
			g_aObject[nCntObject].nLife         = OBJ_LIFE;				// �̗�
			g_aObject[nCntObject].nCounterState = 0; 					// ��ԊǗ��J�E���^�[

			// �g�p���Ă����Ԃɂ���
			g_aObject[nCntObject].bUse = true;

			// ���f������ݒ�
			g_aObject[nCntObject].modelData = GetModelData(nType + FROM_OBJECT);	// ���f�����

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMatModel = (D3DXMATERIAL*)g_aObject[nCntObject].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aObject[nCntObject].modelData.dwNumMat; nCntMat++, pMatModel++)
			{ // �}�e���A���̐����J��Ԃ�

				// �}�e���A�����R�s�[����
				g_aObject[nCntObject].matCopy[nCntMat] = *pMatModel;
			}

			// �ő�l�𔽉f����
			g_aObject[nCntObject].modelData.vtxMax.x *= g_aObject[nCntObject].scale.x;
			g_aObject[nCntObject].modelData.vtxMax.y *= g_aObject[nCntObject].scale.y;
			g_aObject[nCntObject].modelData.vtxMax.z *= g_aObject[nCntObject].scale.z;

			// �ŏ��l�𔽉f����
			g_aObject[nCntObject].modelData.vtxMin.x *= g_aObject[nCntObject].scale.x;
			g_aObject[nCntObject].modelData.vtxMin.y *= g_aObject[nCntObject].scale.y;
			g_aObject[nCntObject].modelData.vtxMin.z *= g_aObject[nCntObject].scale.z;

			for (int nCntMat = 0; nCntMat < (int)g_aObject[nCntObject].modelData.dwNumMat; nCntMat++)
			{ // �}�e���A���̐����J��Ԃ�

				// �����̐F����
				g_aObject[nCntObject].matCopy[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Ambient;
				g_aObject[nCntObject].matCopy[nCntMat].MatD3D.Diffuse = pMat[nCntMat].MatD3D.Diffuse;
			}

			if (nShadowType == SHADOWTYPE_CIRCLE)
			{ // �ۉe�̏ꍇ

				// �e�̃C���f�b�N�X��ݒ�
				g_aObject[nCntObject].nShadowID = SetCircleShadow
				( // ����
					0.5f,																						// ���l
					fabsf(g_aObject[nCntObject].modelData.vtxMax.x - g_aObject[nCntObject].modelData.vtxMin.x),	// ���a
					&g_aObject[nCntObject].nShadowID,															// �e�̐e�̉e�C���f�b�N�X
					&g_aObject[nCntObject].bUse																	// �e�̐e�̎g�p��
				);
			}
			else if (nShadowType == SHADOWTYPE_MODEL)
			{ // ���f���e�̏ꍇ

				// �e�̃C���f�b�N�X��ݒ�
				g_aObject[nCntObject].nShadowID = SetModelShadow
				( // ����
					g_aObject[nCntObject].modelData,	// ���f�����
					&g_aObject[nCntObject].nShadowID,	// �e�̐e�̉e�C���f�b�N�X
					&g_aObject[nCntObject].bUse			// �e�̐e�̎g�p��
				);
			}
			else
			{ // �e�����̏ꍇ

				// �e�̃C���f�b�N�X��ݒ�
				g_aObject[nCntObject].nShadowID = NONE_SHADOW;	// �e��ݒ肵�Ȃ�
			}

			// �e�̈ʒu�ݒ�
			SetPositionShadow(g_aObject[nCntObject].nShadowID, g_aObject[nCntObject].pos, g_aObject[nCntObject].rot, g_aObject[nCntObject].scale);

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
void CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pOldPos, D3DXVECTOR3 *pMove, float fWidth, float fDepth)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aObject[nCntObject].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			switch (g_aObject[nCntObject].nCollisionType)
			{ // �����蔻��̎�ނ��Ƃ̏���
			case COLLISIONTYPE_NONE:	// �����蔻�薳��

				// ����

				// �����𔲂���
				break;

			case COLLISIONTYPE_MODEL:	// ���f�����_�̓����蔻��

				// �O��̓����蔻��
				if (pPos->x + fWidth > g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMin.x
				&&  pPos->x - fWidth < g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMax.x)
				{ // �u���b�N�̍��E�͈͓̔��̏ꍇ

					if (pPos->z    + fDepth >  g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMin.z
					&&  pOldPos->z + fDepth <= g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMin.z)
					{ // �O����̓����蔻��

						// �ʒu��␳
						pPos->z = g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMin.z - fDepth - 0.01f;

						// �ړ��ʂ��폜
						pMove->x *= 0.95f;
					}
					else if (pPos->z    - fDepth <  g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMax.z
					     &&  pOldPos->z - fDepth >= g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMax.z)
					{ // ��납��̓����蔻��

						// �ʒu��␳
						pPos->z = g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMax.z + fDepth + 0.01f;

						// �ړ��ʂ��폜
						pMove->x *= 0.95f;
					}
				}

				// ���E�̓����蔻��
				if (pPos->z + fDepth > g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMin.z
				&&  pPos->z - fDepth < g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMax.z)
				{ // �u���b�N�̑O��͈͓̔��̏ꍇ

					if (pPos->x    + fWidth >  g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMin.x
					&&  pOldPos->x + fWidth <= g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMin.x)
					{ // ������̓����蔻��

						// �ʒu��␳
						pPos->x = g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMin.x - fWidth - 0.01f;

						// �ړ��ʂ��폜
						pMove->x *= 0.95f;
					}
					else if (pPos->x    - fWidth <  g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMax.x
					     &&  pOldPos->x - fWidth >= g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMax.x)
					{ // �E����̓����蔻��
						
						// �ʒu��␳
						pPos->x = g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMax.x + fWidth + 0.01f;

						// �ړ��ʂ��폜
						pMove->x *= 0.95f;
					}
				}

				// �����𔲂���
				break;

			case COLLISIONTYPE_CREATE:	// �쐬���������蔻�� (�ėp)

				D3DXVECTOR3 pos = g_aObject[nCntObject].pos - g_aCollision[g_aObject[nCntObject].nType].vecPos;

				// �O��̓����蔻��
				if (pPos->x + fWidth > pos.x - g_aCollision[g_aObject[nCntObject].nType].fWidth
				&&  pPos->x - fWidth < pos.x + g_aCollision[g_aObject[nCntObject].nType].fWidth)
				{ // �u���b�N�̍��E�͈͓̔��̏ꍇ

					if (pPos->z    + fDepth >  pos.z - g_aCollision[g_aObject[nCntObject].nType].fHeight
					&&  pOldPos->z + fDepth <= pos.z - g_aCollision[g_aObject[nCntObject].nType].fHeight)
					{ // �O����̓����蔻��

						// �ʒu��␳
						pPos->z = pos.z - g_aCollision[g_aObject[nCntObject].nType].fHeight - fDepth - 0.01f;

						// �ړ��ʂ��폜
						pMove->x *= 0.95f;
					}
					else if (pPos->z    - fDepth <  pos.z + g_aCollision[g_aObject[nCntObject].nType].fHeight
					     &&  pOldPos->z - fDepth >= pos.z + g_aCollision[g_aObject[nCntObject].nType].fHeight)
					{ // ��납��̓����蔻��

						// �ʒu��␳
						pPos->z = pos.z + g_aCollision[g_aObject[nCntObject].nType].fHeight + fDepth + 0.01f;

						// �ړ��ʂ��폜
						pMove->x *= 0.95f;
					}
				}

				// ���E�̓����蔻��
				if (pPos->z + fDepth > pos.z - g_aCollision[g_aObject[nCntObject].nType].fHeight
				&&  pPos->z - fDepth < pos.z + g_aCollision[g_aObject[nCntObject].nType].fHeight)
				{ // �u���b�N�̑O��͈͓̔��̏ꍇ

					if (pPos->x    + fWidth >  pos.x - g_aCollision[g_aObject[nCntObject].nType].fWidth
					&&  pOldPos->x + fWidth <= pos.x - g_aCollision[g_aObject[nCntObject].nType].fWidth)
					{ // ������̓����蔻��

						// �ʒu��␳
						pPos->x = pos.x - g_aCollision[g_aObject[nCntObject].nType].fWidth - fWidth - 0.01f;

						// �ړ��ʂ��폜
						pMove->x *= 0.95f;
					}
					else if (pPos->x    - fWidth <  pos.x + g_aCollision[g_aObject[nCntObject].nType].fWidth
					     &&  pOldPos->x - fWidth >= pos.x + g_aCollision[g_aObject[nCntObject].nType].fWidth)
					{ // �E����̓����蔻��
						
						// �ʒu��␳
						pPos->x = pos.x + g_aCollision[g_aObject[nCntObject].nType].fWidth + fWidth + 0.01f;

						// �ړ��ʂ��폜
						pMove->x *= 0.95f;
					}
				}

				// �����𔲂���
				break;
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

//======================================================================================================================
//	�����蔻��̎擾����
//======================================================================================================================
Collision *GetCollision(void)
{
	// �����蔻��̏��̐擪�A�h���X��Ԃ�
	return &g_aCollision[0];
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