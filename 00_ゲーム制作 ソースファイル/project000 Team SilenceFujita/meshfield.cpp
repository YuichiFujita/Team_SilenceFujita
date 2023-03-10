//======================================================================================================================
//
//	���b�V���t�B�[���h���� [meshfield.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "calculation.h"
#include "meshfield.h"
#include "player.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAP_COL_AREA_OVER	(D3DXCOLOR(0.35f, 0.35f, 0.35f, 1.0f))	// �}�b�v�̃G���A�O�̐F
#define MAP_COL_ROAD		(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))		// �}�b�v�̓��H�̐F
#define MAP_COL_SIDEWALK	(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f))		// �}�b�v�̕����̐F
#define MAP_COL_FIELD		(D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f))		// �}�b�v�̒n�ʂ̐F

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTextureMeshField[] =		// �e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\road000.png",			// �����̃e�N�X�`���̑��΃p�X
	"data\\TEXTURE\\road001.png",			// �Ȃ���p�x�̃e�N�X�`���̑��΃p�X
	"data\\TEXTURE\\road002.png",			// �����_�̃e�N�X�`���̑��΃p�X
	"data\\TEXTURE\\road003.png",			// �����H�̃e�N�X�`���̑��΃p�X
	"data\\TEXTURE\\sidewalk000.png",		// �n�ʂ̃e�N�X�`���̑��΃p�X
	"data\\TEXTURE\\Tile.jpg",				// �^�C���̃e�N�X�`���̑��΃p�X
	"data\\TEXTURE\\shopping_street.jpg",	// ���X�X�̓��̃e�N�X�`���̑��΃p�X
	"data\\TEXTURE\\red_sidewalk.jpg",		// �����̃e�N�X�`���̑��΃p�X
	"data\\TEXTURE\\sidewalk002.png",		// �n�ʂ̃e�N�X�`���̑��΃p�X
};

const D3DXCOLOR aColorMeshField[] =		// �}�b�v�̒n�ʃJ���[
{
	MAP_COL_ROAD,		// �����̃e�N�X�`���̒n�ʃJ���[
	MAP_COL_ROAD,		// �Ȃ���p�x�̃e�N�X�`���̒n�ʃJ���[
	MAP_COL_ROAD,		// �����_�̃e�N�X�`���̒n�ʃJ���[
	MAP_COL_ROAD,		// �����H�̃e�N�X�`���̒n�ʃJ���[
	MAP_COL_FIELD,		// �n�ʂ̃e�N�X�`���̒n�ʃJ���[
	MAP_COL_FIELD,		// �n�ʂ̃e�N�X�`���̒n�ʃJ���[
	MAP_COL_SIDEWALK,	// �����̃e�N�X�`���̒n�ʃJ���[
	MAP_COL_SIDEWALK,	// �����̃e�N�X�`���̒n�ʃJ���[
	MAP_COL_AREA_OVER,	// �G���A�O�̃e�N�X�`���̒n�ʃJ���[
};

//**********************************************************************************************************************
//	�񋓌^��` (TEXTURE_MESHFIELD)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_MESHFIELD_ROAD_LINE = 0,	// (���H) ����
	TEXTURE_MESHFIELD_ROAD_TURN,		// (���H) �Ȃ���p�x
	TEXTURE_MESHFIELD_ROAD_CROSS,		// (���H) �����_
	TEXTURE_MESHFIELD_ROAD_TJUNC,		// (���H) �����H
	TEXTURE_MESHFIELD_SIDEWALK,			// ����
	TEXTURE_MESHFIELD_TILE,				// �^�C��
	TEXTURE_MESHFIELD_SHOPPING_STREET,	// ���X�X�̓�
	TEXTURE_MESHFIELD_SIDEWALK2,		// ����
	TEXTURE_MESHFIELD_AREAOVER,			// ����
	TEXTURE_MESHFIELD_MAX,				// ���̗񋓌^�̑���
} TEXTURE_MESHFIELD;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nPartWidth, int nPartHeight, int nType);	// ���b�V���t�B�[���h�̐ݒ菈��
void TxtSetMeshField(void);																										// ���b�V���t�B�[���h�̃Z�b�g�A�b�v����

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureMeshField[TEXTURE_MESHFIELD_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;							// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9  g_pIdxBuffMeshField = NULL;							// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

MeshField g_aMeshField[MAX_MESHFIELD];			// ���b�V���t�B�[���h�̏��
int       g_nNeedVtxField;						// �K�v���_��
int       g_nNeedIdxField;						// �K�v�C���f�b�N�X��

//======================================================================================================================
//	���b�V���t�B�[���h�̏���������
//======================================================================================================================
void InitMeshField(void)
{
	// �ϐ���錾
	int nNumVtx = 0;							// ���_���̌v���p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_3D *pVtx;							// ���_���ւ̃|�C���^
	WORD      *pIdx;							// �C���f�b�N�X���ւ̃|�C���^

	// �O���[�o���ϐ��̏�����
	g_nNeedVtxField = 0;						// �K�v���_�̑���
	g_nNeedIdxField = 0;						// �K�v�C���f�b�N�X�̑���

	// ���b�V���t�B�[���h�̏��̏�����
	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++)
	{ // ���b�V���t�B�[���h�̍ő�\�������J��Ԃ�

		g_aMeshField[nCntMeshField].pos         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aMeshField[nCntMeshField].rot         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aMeshField[nCntMeshField].fWidth      = 0.0f;								// ����
		g_aMeshField[nCntMeshField].fHeight     = 0.0f;								// �c��
		g_aMeshField[nCntMeshField].nPartWidth  = 0;								// ���̕�����
		g_aMeshField[nCntMeshField].nPartHeight = 0;								// �c�̕�����
		g_aMeshField[nCntMeshField].nNumVtx     = 0;								// �K�v���_��
		g_aMeshField[nCntMeshField].nNumIdx     = 0;								// �K�v�C���f�b�N�X��
		g_aMeshField[nCntMeshField].nType       = 0;								// ���
		g_aMeshField[nCntMeshField].bUse        = false;							// �g�p��
	}

	// �e�N�X�`���̓ǂݍ���
	for (int nCntMeshField = 0; nCntMeshField < TEXTURE_MESHFIELD_MAX; nCntMeshField++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		D3DXCreateTextureFromFile(pDevice, apTextureMeshField[nCntMeshField], &g_apTextureMeshField[nCntMeshField]);
	}

	// ���b�V���t�B�[���h�̃Z�b�g�A�b�v
	TxtSetMeshField();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_3D) * g_nNeedVtxField,	// �K�v���_��
		D3DUSAGE_WRITEONLY,						// �g�p���@
		FVF_VERTEX_3D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,						// �������̎w��
		&g_pVtxBuffMeshField,					// ���_�o�b�t�@�ւ̃|�C���^
		NULL
	);

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer
	( // ����
		sizeof(WORD) * g_nNeedIdxField,			// �K�v�C���f�b�N�X��
		D3DUSAGE_WRITEONLY,						// �g�p���@
		D3DFMT_INDEX16,							// �C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g
		D3DPOOL_MANAGED,						// �������̎w��
		&g_pIdxBuffMeshField,					// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++)
	{ // ���b�V���t�B�[���h�̍ő�\�������J��Ԃ�

		if (g_aMeshField[nCntMeshField].bUse == true)
		{ // ���b�V���t�B�[���h���g�p����Ă���ꍇ

			for (int nCntHeight = 0; nCntHeight < g_aMeshField[nCntMeshField].nPartHeight + 1; nCntHeight++)
			{ // �c�̕����� +1��J��Ԃ�

				for (int nCntWidth = 0; nCntWidth < g_aMeshField[nCntMeshField].nPartWidth + 1; nCntWidth++)
				{ // ���̕����� +1��J��Ԃ�

					// ���_���W�̐ݒ�
					pVtx[0].pos = D3DXVECTOR3
					( // ����
						nCntWidth * (g_aMeshField[nCntMeshField].fWidth / (float)g_aMeshField[nCntMeshField].nPartWidth) - (g_aMeshField[nCntMeshField].fWidth * 0.5f),			// x
						0.0f,																																					// y
						-(nCntHeight * (g_aMeshField[nCntMeshField].fHeight / (float)g_aMeshField[nCntMeshField].nPartHeight) - (g_aMeshField[nCntMeshField].fHeight * 0.5f))	// z
					);

					// �@���x�N�g���̐ݒ�
					pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

					// ���_�J���[�̐ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// �e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(1.0f * (nCntWidth % 2), 1.0f * nCntHeight);

					// ���_�f�[�^�̃|�C���^�� 1���i�߂�
					pVtx += 1;
				}
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshField->Unlock();

	//------------------------------------------------------------------------------------------------------------------
	//	�C���f�b�N�X���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// �C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++)
	{ // ���b�V���t�B�[���h�̍ő�\�������J��Ԃ�

		if (g_aMeshField[nCntMeshField].bUse == true)
		{ // ���b�V���t�B�[���h���g�p����Ă���ꍇ

			for (int nCntHeight = 0, nCntWidth = 0; nCntHeight < g_aMeshField[nCntMeshField].nPartHeight; nCntHeight++)
			{ // �c�̕����� +1��J��Ԃ�

				for (nCntWidth = 0; nCntWidth < g_aMeshField[nCntMeshField].nPartWidth + 1; nCntWidth++)
				{ // ���̕����� +1��J��Ԃ�

					pIdx[0] = nNumVtx + ((g_aMeshField[nCntMeshField].nPartWidth + 1) * (nCntHeight + 1) + nCntWidth);
					pIdx[1] = nNumVtx + ((g_aMeshField[nCntMeshField].nPartWidth + 1) * nCntHeight + nCntWidth);

					// �C���f�b�N�X�f�[�^�̃|�C���^�� 2���i�߂�
					pIdx += 2;
				}

				if (nCntHeight != g_aMeshField[nCntMeshField].nPartHeight - 1)
				{ // ��Ԏ�O�̕����ꏊ�ł͂Ȃ��ꍇ

					pIdx[0] = nNumVtx + ((g_aMeshField[nCntMeshField].nPartWidth + 1) * nCntHeight + nCntWidth - 1);
					pIdx[1] = nNumVtx + ((g_aMeshField[nCntMeshField].nPartWidth + 1) * (nCntHeight + 2));

					// �C���f�b�N�X�f�[�^�̃|�C���^�� 2���i�߂�
					pIdx += 2;
				}
			}

			// ���_�o�b�t�@�̊J�n�n�_��K�v�������炷
			nNumVtx += g_aMeshField[nCntMeshField].nNumVtx;
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshField->Unlock();
}

//======================================================================================================================
//	���b�V���t�B�[���h�̏I������
//======================================================================================================================
void UninitMeshField(void)
{
	// �e�N�X�`���̔j��
	for (int nCntMeshField = 0; nCntMeshField < TEXTURE_MESHFIELD_MAX; nCntMeshField++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTextureMeshField[nCntMeshField] != NULL)
		{ // �ϐ� (g_apTextureMeshField) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureMeshField[nCntMeshField]->Release();
			g_apTextureMeshField[nCntMeshField] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshField != NULL)
	{ // �ϐ� (g_pVtxBuffMeshField) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffMeshField != NULL)
	{ // �ϐ� (g_pIdxBuffMeshField) ��NULL�ł͂Ȃ��ꍇ

		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//======================================================================================================================
//	���b�V���t�B�[���h�̍X�V����
//======================================================================================================================
void UpdateMeshField(void)
{

}

//======================================================================================================================
//	���b�V���t�B�[���h�̕`�揈��
//======================================================================================================================
void DrawMeshField(bool bMap)
{
	// �ϐ���錾
	int        nNumIdx = 0;						// �C���f�b�N�X���̌v���p
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_3D        *pVtx;						// ���_���ւ̃|�C���^

	if (bMap == true)
	{ // �}�b�v�\���p�̃��b�V���t�B�[���h�̕`��̏ꍇ

		// ���C�e�B���O�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// Z�e�X�g�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);	// Z�e�X�g�̐ݒ�
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++)
	{ // ���b�V���t�B�[���h�̍ő�\�������J��Ԃ�

		if (g_aMeshField[nCntMeshField].bUse == true)
		{ // ���b�V���t�B�[���h���g�p����Ă���ꍇ

			for (int nCntCol = 0; nCntCol < g_aMeshField[nCntMeshField].nNumVtx; nCntCol++)
			{ // ���b�V���t�B�[���h�̒��_�����J��Ԃ�

				if (bMap == true)
				{ // �}�b�v�\���p�̃��b�V���t�B�[���h�̕`��̏ꍇ

					// ���_�J���[�̐ݒ�
					pVtx[nCntCol].col = aColorMeshField[g_aMeshField[nCntMeshField].nType];
				}
				else
				{ // �ʏ�̃��b�V���t�B�[���h�̕`��̏ꍇ

					// ���_�J���[�̐ݒ�
					pVtx[nCntCol].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
			}
		}

		// ���_�f�[�^�̃|�C���^�̊J�n�n�_��K�v�������炷
		pVtx += g_aMeshField[nCntMeshField].nNumVtx;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshField->Unlock();

	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++)
	{ // ���b�V���t�B�[���h�̍ő�\�������J��Ԃ�

		if (g_aMeshField[nCntMeshField].bUse == true)
		{ // ���b�V���t�B�[���h���g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aMeshField[nCntMeshField].mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshField[nCntMeshField].rot.y, g_aMeshField[nCntMeshField].rot.x, g_aMeshField[nCntMeshField].rot.z);
			D3DXMatrixMultiply(&g_aMeshField[nCntMeshField].mtxWorld, &g_aMeshField[nCntMeshField].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aMeshField[nCntMeshField].pos.x, g_aMeshField[nCntMeshField].pos.y, g_aMeshField[nCntMeshField].pos.z);
			D3DXMatrixMultiply(&g_aMeshField[nCntMeshField].mtxWorld, &g_aMeshField[nCntMeshField].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshField[nCntMeshField].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

			// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffMeshField);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			if (bMap == true)
			{ // �}�b�v�\���p�̃��b�V���t�B�[���h�̕`��̏ꍇ

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, NULL);
			}
			else
			{ // �ʏ�̃��b�V���t�B�[���h�̕`��̏ꍇ

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_apTextureMeshField[g_aMeshField[nCntMeshField].nType]);
			}

			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive
			( // ����
				D3DPT_TRIANGLESTRIP,						// �v���~�e�B�u�̎��
				0,
				0,
				g_aMeshField[nCntMeshField].nNumVtx,		// �g�p���钸�_��
				nNumIdx,									// �C���f�b�N�X�o�b�t�@�̊J�n�n�_
				g_aMeshField[nCntMeshField].nNumIdx - 2		// �v���~�e�B�u (�|���S��) ��
			);

			// �C���f�b�N�X�o�b�t�@�̊J�n�n�_��K�v�������炷
			nNumIdx += g_aMeshField[nCntMeshField].nNumIdx;
		}
	}

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�
}

//======================================================================================================================
//	���b�V���t�B�[���h�̐ݒ菈��
//======================================================================================================================
void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nPartWidth, int nPartHeight, int nType)
{
	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++)
	{ // ���b�V���t�B�[���h�̍ő�\�������J��Ԃ�

		if (g_aMeshField[nCntMeshField].bUse == false)
		{ // ���b�V���t�B�[���h���g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aMeshField[nCntMeshField].pos         = pos;			// �ʒu
			g_aMeshField[nCntMeshField].rot         = rot;			// ����
			g_aMeshField[nCntMeshField].fWidth      = fWidth;		// ����
			g_aMeshField[nCntMeshField].fHeight     = fHeight;		// �c��
			g_aMeshField[nCntMeshField].nPartWidth  = nPartWidth;	// ���̕�����
			g_aMeshField[nCntMeshField].nPartHeight = nPartHeight;	// �c�̕�����
			g_aMeshField[nCntMeshField].nType       = nType;		// ���

			// �g�p���Ă����Ԃɂ���
			g_aMeshField[nCntMeshField].bUse = true;

			// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̕K�v����ݒ�
			g_aMeshField[nCntMeshField].nNumVtx = (nPartWidth + 1) * (nPartHeight + 1);
			g_aMeshField[nCntMeshField].nNumIdx = (nPartWidth + 1) * (((nPartHeight + 1) * 2) - 2) + (nPartHeight * 2) - 2;

			// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̑��������Z
			g_nNeedVtxField += g_aMeshField[nCntMeshField].nNumVtx;
			g_nNeedIdxField += g_aMeshField[nCntMeshField].nNumIdx;

			// �����𔲂���
			break;
		}
	}
}

//======================================================================================================================
//	���b�V���t�B�[���h�Ƃ̓����蔻��
//======================================================================================================================
float CollisionMeshField(D3DXVECTOR3 pos)
{
	// �ϐ���錾
	float fLandPosY = GetLimitStage().fField;		// ���n�\��� y���W

	// �ϐ��z���錾
	D3DXVECTOR3 vecPos[4];	// ���_�ʒu ([��] 0�F�E��@1�F����@2�F�����@3�F�E��)

	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++)
	{ // ���b�V���t�B�[���h�̍ő�\�������J��Ԃ�

		if (g_aMeshField[nCntMeshField].bUse == true)
		{ // ���b�V���t�B�[���h���g�p����Ă���ꍇ

			// �l���_�̈ʒu�̌v�Z
			VecSizePos
			( // ����
				&vecPos[0],
				g_aMeshField[nCntMeshField].pos,	// ��΍��W
				g_aMeshField[nCntMeshField].rot,	// ����
				g_aMeshField[nCntMeshField].fWidth,	// ����
				g_aMeshField[nCntMeshField].fHeight	// �c��
			);

			if (LineOuterProduct(vecPos[0], vecPos[1], pos) < 0
			&&  LineOuterProduct(vecPos[1], vecPos[2], pos) < 0
			&&  LineOuterProduct(vecPos[2], vecPos[3], pos) < 0
			&&  LineOuterProduct(vecPos[3], vecPos[0], pos) < 0)
			{ // �l�ӂ̓����ɂ���ꍇ (�������Ă���ꍇ)

				if (fLandPosY < g_aMeshField[nCntMeshField].pos.y)
				{ // ���݂̒��n�_��胁�b�V���t�B�[���h�̕�����̏ꍇ

					// ���݂̒��n�_�Ƀ��b�V���t�B�[���h�̍��W��ݒ�
					fLandPosY = g_aMeshField[nCntMeshField].pos.y;
				}
			}
		}
	}

	// ���n���� y���W��Ԃ�
	return fLandPosY;
}

//======================================================================================================================
//	���b�V���t�B�[���h�̎擾����
//======================================================================================================================
MeshField *GetMeshField(void)
{
	// ���b�V���t�B�[���h�̏��̐擪�A�h���X��Ԃ�
	return &g_aMeshField[0];
}

//======================================================================================================================
//	���b�V���t�B�[���h�̃Z�b�g�A�b�v����
//======================================================================================================================
void TxtSetMeshField(void)
{
	// �ϐ���錾
	D3DXVECTOR3 pos;			// �ʒu�̑���p
	D3DXVECTOR3 rot;			// �����̑���p
	float       fWidth;			// �����̑���p
	float       fHeight;		// �c���̑���p
	int         nPartWidth;		// ���̕������̑���p
	int         nPartHeight;	// �c�̕������̑���p
	int         nType;			// ��ނ̑���p
	int         nEnd;			// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;				// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = (GetMode() == MODE_TUTORIAL) ? fopen(TUTORIAL_STAGE_SETUP_TXT, "r") : fopen(GAME_STAGE_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (strcmp(&aString[0], "STAGE_MESHFIELDSET") == 0)
			{ // �ǂݍ��񂾕����� STAGE_MESHFIELDSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_STAGE_MESHFIELDSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "MESHFIELDSET") == 0)
					{ // �ǂݍ��񂾕����� MESHFIELDSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_MESHFIELDSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "POS") == 0)
							{ // �ǂݍ��񂾕����� POS �̏ꍇ
								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &pos.x);		// X���W��ǂݍ���
								fscanf(pFile, "%f", &pos.y);		// Y���W��ǂݍ���
								fscanf(pFile, "%f", &pos.z);		// Z���W��ǂݍ���
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // �ǂݍ��񂾕����� ROT �̏ꍇ
								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &rot.x);		// X������ǂݍ���
								fscanf(pFile, "%f", &rot.y);		// Y������ǂݍ���
								fscanf(pFile, "%f", &rot.z);		// Z������ǂݍ���
							}
							else if (strcmp(&aString[0], "WIDTH") == 0)
							{ // �ǂݍ��񂾕����� WIDTH �̏ꍇ
								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &fWidth);		// ������ǂݍ���
							}
							else if (strcmp(&aString[0], "HEIGHT") == 0)
							{ // �ǂݍ��񂾕����� HEIGHT �̏ꍇ
								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &fHeight);		// �c����ǂݍ���
							}
							else if (strcmp(&aString[0], "PARTWIDTH") == 0)
							{ // �ǂݍ��񂾕����� PARTWIDTH �̏ꍇ
								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nPartWidth);	// ���̕�������ǂݍ���
							}
							else if (strcmp(&aString[0], "PARTHEIGHT") == 0)
							{ // �ǂݍ��񂾕����� PARTHEIGHT �̏ꍇ
								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nPartHeight);	// �c�̕�������ǂݍ���
							}
							else if (strcmp(&aString[0], "TYPE") == 0)
							{ // �ǂݍ��񂾕����� TYPE �̏ꍇ
								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nType);		// ��ނ�ǂݍ���
							}

						} while (strcmp(&aString[0], "END_MESHFIELDSET") != 0);	// �ǂݍ��񂾕����� END_MESHFIELDSET �ł͂Ȃ��ꍇ���[�v

						// ���b�V���t�B�[���h�̐ݒ�
						SetMeshField(pos, D3DXToRadian(rot), fWidth, fHeight, nPartWidth, nPartHeight, nType);
					}
				} while (strcmp(&aString[0], "END_STAGE_MESHFIELDSET") != 0);	// �ǂݍ��񂾕����� END_STAGE_MESHFIELDSET �ł͂Ȃ��ꍇ���[�v
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�X�e�[�W�t�@�C���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}