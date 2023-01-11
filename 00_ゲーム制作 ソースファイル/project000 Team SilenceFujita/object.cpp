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

#include "object.h"
#include "particle.h"
#include "shadow.h"
#include "sound.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define OBJ_LIFE			(50)		// �I�u�W�F�N�g�̗̑�

#define DAMAGE_TIME_OBJ		(PLAY_SLASH_LIFE)		// �_���[�W��Ԃ�ۂ���
#define UNR_TIME_OBJ		(PLAY_SLASH_LIFE - 10)	// ���G��ԂɕύX���鎞��

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apModelObject[] =			// ���f���̑��΃p�X
{
	"data\\MODEL_STAGE\\wood000.x",		// �؂̃��f�����΃p�X
	"data\\MODEL_STAGE\\wood001.x",		// �؂̃��f�����΃p�X
	"data\\MODEL_STAGE\\wood002.x",		// �؂̃��f�����΃p�X
	"data\\MODEL_STAGE\\stone000.x",	// �΂̃��f�����΃p�X
	"data\\MODEL_STAGE\\stone001.x",	// �΂̃��f�����΃p�X
	"data\\MODEL_STAGE\\stone002.x",	// �΂̃��f�����΃p�X
	"data\\MODEL_STAGE\\box000.x",		// ���̃��f�����΃p�X
	"data\\MODEL_STAGE\\box001.x",		// ���̃��f�����΃p�X
};

//**********************************************************************************************************************
//	�񋓌^��` (OBJECTTYPE)
//**********************************************************************************************************************
typedef enum
{
	OBJECTTYPE_WOOD_S = 0,				// �� (��)
	OBJECTTYPE_WOOD_M,					// �� (��)
	OBJECTTYPE_WOOD_L,					// �� (��)
	OBJECTTYPE_STONE_S,					// �� (��)
	OBJECTTYPE_STONE_M,					// �� (��)
	OBJECTTYPE_STONE_L,					// �� (��)
	OBJECTTYPE_BOX_S,					// �� (��)
	OBJECTTYPE_BOX_L,					// �� (��)
	OBJECTTYPE_MAX,						// ���̗񋓌^�̑���
} OBJECTTYPE;

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
Model  g_aModelObject[OBJECTTYPE_MAX];	// �I�u�W�F�N�g�̃��f�����
Object g_aObject[MAX_OBJECT];			// �I�u�W�F�N�g�̏��

//======================================================================================================================
//	�I�u�W�F�N�g�̏���������
//======================================================================================================================
void InitObject(void)
{
	// �ϐ���錾
	int         nNumVtx;		// ���f���̒��_��
	DWORD       dwSizeFVF;		// ���f���̒��_�t�H�[�}�b�g�̃T�C�Y
	BYTE        *pVtxBuff;		// ���f���̒��_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 vtx;			// ���f���̒��_���W
	D3DXVECTOR3 size;			// ���f���̑傫��

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL      *pMat;					// �}�e���A���ւ̃|�C���^

	// �I�u�W�F�N�g�̃��f�����̏�����
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // �I�u�W�F�N�g�̎�ޕ��J��Ԃ�

		g_aModelObject[nCntObject].apTexture[MAX_MODEL_TEXTURE] = {};						// �e�N�X�`���ւ̃|�C���^
		g_aModelObject[nCntObject].pMesh    = NULL;											// ���b�V�� (���_���) �ւ̃|�C���^
		g_aModelObject[nCntObject].pBuffMat = NULL;											// �}�e���A���ւ̃|�C���^
		g_aModelObject[nCntObject].dwNumMat = 0;											// �}�e���A���̐�
		g_aModelObject[nCntObject].vtxMin   = D3DXVECTOR3( 9999.0f,  9999.0f,  9999.0f);	// �ŏ��̒��_���W
		g_aModelObject[nCntObject].vtxMax   = D3DXVECTOR3(-9999.0f, -9999.0f, -9999.0f);	// �ő�̒��_���W
		g_aModelObject[nCntObject].fHeight  = 0.0f;											// �c��
		g_aModelObject[nCntObject].fRadius  = 0.0f;											// ���a
	}

	// �I�u�W�F�N�g�̏��̏�����
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		g_aObject[nCntObject].pos           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_aObject[nCntObject].rot           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
		g_aObject[nCntObject].pModelData    = NULL;									// ���f�����
		g_aObject[nCntObject].state         = ACTIONSTATE_NONE;						// ���
		g_aObject[nCntObject].nLife         = 0;									// �̗�
		g_aObject[nCntObject].nBreakType    = BREAKTYPE_NONE;						// �����̎��
		g_aObject[nCntObject].nType         = 0;									// �I�u�W�F�N�g�̎��
		g_aObject[nCntObject].nCounterState = 0; 									// ��ԊǗ��J�E���^�[
		g_aObject[nCntObject].nShadowID     = NONE_SHADOW;							// �e�̃C���f�b�N�X
		g_aObject[nCntObject].bUse          = false;								// �g�p��
	}

	// x�t�@�C���̓ǂݍ���
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // �I�u�W�F�N�g�̎�ޕ��J��Ԃ�

		D3DXLoadMeshFromX
		( // ����
			apModelObject[nCntObject],				// ���f���̑��΃p�X
			D3DXMESH_SYSTEMMEM,
			pDevice,								// �f�o�C�X�ւ̃|�C���^
			NULL,
			&g_aModelObject[nCntObject].pBuffMat,	// �}�e���A���ւ̃|�C���^
			NULL,
			&g_aModelObject[nCntObject].dwNumMat,	// �}�e���A���̐�
			&g_aModelObject[nCntObject].pMesh		// ���b�V�� (���_���) �ւ̃|�C���^
		);
	}

	// �����蔻��̍쐬
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // �I�u�W�F�N�g�̎�ޕ��J��Ԃ�

		// ���f���̒��_�����擾
		nNumVtx = g_aModelObject[nCntObject].pMesh->GetNumVertices();

		// ���f���̒��_�t�H�[�}�b�g�̃T�C�Y���擾
		dwSizeFVF = D3DXGetFVFVertexSize(g_aModelObject[nCntObject].pMesh->GetFVF());

		// ���f���̒��_�o�b�t�@�����b�N
		g_aModelObject[nCntObject].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{ // ���f���̒��_�����J��Ԃ�

			// ���f���̒��_���W����
			vtx = *(D3DXVECTOR3*)pVtxBuff;

			// ���_���W (x) �̐ݒ�
			if (vtx.x < g_aModelObject[nCntObject].vtxMin.x)
			{ // ����̒��_���W (x) ���A����̒��_���W (x) �����������ꍇ

				// ����̒��_��� (x) ����
				g_aModelObject[nCntObject].vtxMin.x = vtx.x;
			}
			else if (vtx.x > g_aModelObject[nCntObject].vtxMax.x)
			{ // ����̒��_���W (x) ���A����̒��_���W (x) �����傫���ꍇ

				// ����̒��_��� (x) ����
				g_aModelObject[nCntObject].vtxMax.x = vtx.x;
			}

			// ���_���W (y) �̐ݒ�
			if (vtx.y < g_aModelObject[nCntObject].vtxMin.y)
			{ // ����̒��_���W (y) ���A����̒��_���W (y) �����������ꍇ

				// ����̒��_��� (y) ����
				g_aModelObject[nCntObject].vtxMin.y = vtx.y;
			}
			else if (vtx.y > g_aModelObject[nCntObject].vtxMax.y)
			{ // ����̒��_���W (y) ���A����̒��_���W (y) �����傫���ꍇ

				// ����̒��_��� (y) ����
				g_aModelObject[nCntObject].vtxMax.y = vtx.y;
			}

			// ���_���W (z) �̐ݒ�
			if (vtx.z < g_aModelObject[nCntObject].vtxMin.z)
			{ // ����̒��_���W (z) ���A����̒��_���W (z) �����������ꍇ

				// ����̒��_��� (z) ����
				g_aModelObject[nCntObject].vtxMin.z = vtx.z;
			}
			else if (vtx.z > g_aModelObject[nCntObject].vtxMax.z)
			{ // ����̒��_���W (z) ���A����̒��_���W (z) �����傫���ꍇ

				// ����̒��_��� (z) ����
				g_aModelObject[nCntObject].vtxMax.z = vtx.z;
			}

			// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += dwSizeFVF;
		}

		// ���f���̒��_�o�b�t�@���A�����b�N
		g_aModelObject[nCntObject].pMesh->UnlockVertexBuffer();

		// ���f���T�C�Y�����߂�
		size = g_aModelObject[nCntObject].vtxMax - g_aModelObject[nCntObject].vtxMin;

		// ���f���̏c������
		g_aModelObject[nCntObject].fHeight = size.y;

		// ���f���̉~�̓����蔻����쐬
		g_aModelObject[nCntObject].fRadius = ((size.x * 0.5f) + (size.z * 0.5f)) * 0.5f;
	}

	// �e�N�X�`���̓ǂݍ���
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // �I�u�W�F�N�g�̎�ޕ��J��Ԃ�

		// �}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aModelObject[nCntObject].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aModelObject[nCntObject].dwNumMat; nCntMat++)
		{ // �}�e���A���̐����J��Ԃ�

			if (pMat[nCntMat].pTextureFilename != NULL)
			{ // �e�N�X�`���t�@�C�������݂���ꍇ

				// �e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aModelObject[nCntObject].apTexture[nCntMat]);
			}
		}
	}
}

//======================================================================================================================
//	�I�u�W�F�N�g�̏I������
//======================================================================================================================
void UninitObject(void)
{
	// �e�N�X�`���̔j��
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // �I�u�W�F�N�g�̎�ޕ��J��Ԃ�

		for (int nCntMat = 0; nCntMat < MAX_MODEL_TEXTURE; nCntMat++)
		{ // �g�p����e�N�X�`�������J��Ԃ�

			if (g_aModelObject[nCntObject].apTexture[nCntMat] != NULL)
			{ // �ϐ� (g_aModelObject[nCntObject].apTexture) ��NULL�ł͂Ȃ��ꍇ

				g_aModelObject[nCntObject].apTexture[nCntMat]->Release();
				g_aModelObject[nCntObject].apTexture[nCntMat] = NULL;
			}
		}
	}

	// ���b�V���̔j��
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // �I�u�W�F�N�g�̎�ޕ��J��Ԃ�

		if (g_aModelObject[nCntObject].pMesh != NULL)
		{ // �ϐ� (g_aModelObject[nCntObject].pMesh) ��NULL�ł͂Ȃ��ꍇ

			g_aModelObject[nCntObject].pMesh->Release();
			g_aModelObject[nCntObject].pMesh = NULL;
		}
	}

	// �}�e���A���̔j��
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // �I�u�W�F�N�g�̎�ޕ��J��Ԃ�

		if (g_aModelObject[nCntObject].pBuffMat != NULL)
		{ // �ϐ� (g_aModelObject[nCntObject].pBuffMat) ��NULL�ł͂Ȃ��ꍇ

			g_aModelObject[nCntObject].pBuffMat->Release();
			g_aModelObject[nCntObject].pBuffMat = NULL;
		}
	}
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
	D3DXMATRIX   mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p

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
				pDevice->SetTexture(0, g_aObject[nCntObject].pModelData->apTexture[nCntMat]);

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
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nBreakType, int nType)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aObject[nCntObject].bUse == false)
		{ // �I�u�W�F�N�g���g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aObject[nCntObject].pos        = pos;			// �ʒu
			g_aObject[nCntObject].rot        = rot;			// ����
			g_aObject[nCntObject].nBreakType = nBreakType;	// �����̎��
			g_aObject[nCntObject].nType      = nType;		// �I�u�W�F�N�g�̎��

			// �I�u�W�F�N�g�̏���������
			g_aObject[nCntObject].state         = ACTIONSTATE_NORMAL;	// ���
			g_aObject[nCntObject].nLife         = OBJ_LIFE;				// �̗�
			g_aObject[nCntObject].nCounterState = 0; 					// ��ԊǗ��J�E���^�[

			// �g�p���Ă����Ԃɂ���
			g_aObject[nCntObject].bUse = true;

			// ���f������ݒ�
			g_aObject[nCntObject].pModelData = &g_aModelObject[nType];	// ���f�����

			// �e�̃C���f�b�N�X��ݒ�
			g_aObject[nCntObject].nShadowID = SetShadow
			( // ����
				0.5f,																												// ���l
				fabsf(g_aModelObject[g_aObject[nCntObject].nType].vtxMax.x - g_aModelObject[g_aObject[nCntObject].nType].vtxMin.x),	// ���a
				&g_aObject[nCntObject].nShadowID,																					// �e�̐e�̉e�C���f�b�N�X
				&g_aObject[nCntObject].bUse																							// �e�̐e�̎g�p��
			);

			// �e�̈ʒu�ݒ�
			SetPositionShadow(g_aObject[nCntObject].nShadowID, g_aObject[nCntObject].pos);

			// �����𔲂���
			break;
		}
	}
}

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