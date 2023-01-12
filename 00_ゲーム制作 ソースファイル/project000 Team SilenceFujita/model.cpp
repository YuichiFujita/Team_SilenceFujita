//============================================================
//
//	���f������ [model.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "model.h"

//************************************************************
//	�}�N����`
//************************************************************
#define INIT_VTX_MIN	(D3DXVECTOR3( 9999.0f,  9999.0f,  9999.0f))		// ���f���̍ŏ��̒��_���W�̏����l
#define INIT_VTX_MAX	(D3DXVECTOR3(-9999.0f, -9999.0f, -9999.0f))		// ���f���̍ŏ��̒��_���W�̏����l

//************************************************************
//	�R���X�g��`
//************************************************************
const char *apModelData[] =		// ���f���̑��΃p�X
{
	// �I�u�W�F�N�g���f��
	"data\\MODEL_OBJECT\\Tree.x",		// ��
	"data\\MODEL_OBJECT\\Rock.x",		// ��
	"data\\MODEL_OBJECT\\Forest.x",		// �X
	"data\\MODEL_OBJECT\\BigTree.x",	// ���
	"data\\MODEL_OBJECT\\YellowTree.x",	// ���F����
	"data\\MODEL_OBJECT\\Grass.x",		// ��

	// �v���C���[���f��
	"data\\MODEL_PLAYER\\head.x",		// ��
};

//************************************************************
//	�v���g�^�C�v�錾
//************************************************************
HRESULT LoadXFileModel(void);	// x�t�@�C���̓ǂݍ���
void SetCollisionModel(void);	// �����蔻��̍쐬
HRESULT LoadTextureModel(void);	// �e�N�X�`���̓ǂݍ���

//************************************************************
//	�O���[�o���ϐ�
//************************************************************
Model g_aModel[MODELTYPE_MAX];	// ���f���̏��

//============================================================
//	���f���̏���������
//============================================================
HRESULT InitModel(void)
{
	// ���f�����̏�����
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{ // ���f���Ɏg�p���郂�f���̍ő吔���J��Ԃ�

		g_aModel[nCntModel].pTexture = NULL;			// �e�N�X�`���ւ̃|�C���^
		g_aModel[nCntModel].pMesh    = NULL;			// ���b�V�� (���_���) �ւ̃|�C���^
		g_aModel[nCntModel].pBuffMat = NULL;			// �}�e���A���ւ̃|�C���^
		g_aModel[nCntModel].dwNumMat = 0;				// �}�e���A���̐�
		g_aModel[nCntModel].vtxMin   = INIT_VTX_MIN;	// �ŏ��̒��_���W
		g_aModel[nCntModel].vtxMax   = INIT_VTX_MAX;	// �ő�̒��_���W
		g_aModel[nCntModel].fHeight  = 0.0f;			// �c��
		g_aModel[nCntModel].fRadius  = 0.0f;			// ���a
	}

	// x�t�@�C���̓ǂݍ���
	if (FAILED(LoadXFileModel()))
	{ // x�t�@�C���̓ǂݍ��݂Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �����蔻��̍쐬
	SetCollisionModel();

	// �e�N�X�`���̓ǂݍ���
	if (FAILED(LoadTextureModel()))
	{ // �e�N�X�`���̓ǂݍ��݂Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	���f���̏I������
//============================================================
void UninitModel(void)
{
	// �e�N�X�`���̔j��
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{ // ���f���̍ő吔���J��Ԃ�

		for (int nCntMat = 0; nCntMat < (int)g_aModel[nCntModel].dwNumMat; nCntMat++)
		{ // �}�e���A���̐����J��Ԃ�

			if (g_aModel[nCntModel].pTexture[nCntMat] != NULL)
			{ // �ϐ� (g_aModel[nCntModel].pTexture) ��NULL�ł͂Ȃ��ꍇ

				g_aModel[nCntModel].pTexture[nCntMat]->Release();
				g_aModel[nCntModel].pTexture[nCntMat] = NULL;
			}
		}

		// ���I�m�ۂ������������������
		free(g_aModel[nCntModel].pTexture);
	}

	// ���b�V���̔j��
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{ // ���f���̍ő吔���J��Ԃ�

		if (g_aModel[nCntModel].pMesh != NULL)
		{ // �ϐ� (g_aModel[nCntModel].pMesh) ��NULL�ł͂Ȃ��ꍇ

			g_aModel[nCntModel].pMesh->Release();
			g_aModel[nCntModel].pMesh = NULL;
		}
	}

	// �}�e���A���̔j��
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{ // ���f���̍ő吔���J��Ԃ�

		if (g_aModel[nCntModel].pBuffMat != NULL)
		{ // �ϐ� (g_aModel[nCntModel].pBuffMat) ��NULL�ł͂Ȃ��ꍇ

			g_aModel[nCntModel].pBuffMat->Release();
			g_aModel[nCntModel].pBuffMat = NULL;
		}
	}
}

//============================================================
//	x�t�@�C���̓ǂݍ���
//============================================================
HRESULT LoadXFileModel(void)
{
	// �ϐ���錾
	HRESULT hr;		// �ُ�I���̊m�F�p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{ // ���f���̍ő吔���J��Ԃ�

		// x�t�@�C���̓ǂݍ���
		hr = D3DXLoadMeshFromX
		( // ����
			apModelData[nCntModel],				// ���f���̑��΃p�X
			D3DXMESH_SYSTEMMEM,					// ���b�V���쐬�p�I�v�V����
			pDevice,							// �f�o�C�X�ւ̃|�C���^
			NULL,								// �אڐ��f�[�^
			&g_aModel[nCntModel].pBuffMat,		// �}�e���A���ւ̃|�C���^
			NULL,								// �G�t�F�N�g�f�[�^
			&g_aModel[nCntModel].dwNumMat,		// �}�e���A���̐�
			&g_aModel[nCntModel].pMesh			// ���b�V�� (���_���) �ւ̃|�C���^
		);

		if (FAILED(hr))
		{ // x�t�@�C���̓ǂݍ��݂Ɏ��s�����ꍇ

			// �G���[���b�Z�[�W�{�b�N�X
			MessageBox(NULL, "x�t�@�C���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// ���s��Ԃ�
			return E_FAIL;
		}

		// �m�ۂ����������̃A�h���X���擾
		g_aModel[nCntModel].pTexture = (LPDIRECT3DTEXTURE9*)malloc(g_aModel[nCntModel].dwNumMat * sizeof(LPDIRECT3DTEXTURE9*));

		if (g_aModel[nCntModel].pTexture == NULL)
		{ // ���I�m�ۂɎ��s�����ꍇ

			// �G���[���b�Z�[�W�{�b�N�X
			MessageBox(NULL, "���I�m�ۂɎ��s�I", "�x���I", MB_ICONWARNING);

			for (int nCntFree = 0; nCntFree < nCntModel; nCntFree++)
			{ // ���I�m�ۂɐ��������񐔕��J��Ԃ�

				// ���I�m�ۂ������������������
				free(g_aModel[nCntFree].pTexture);
			}

			// ���s��Ԃ�
			return E_FAIL;
		}
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�����蔻��̍쐬
//============================================================
void SetCollisionModel(void)
{
	// �ϐ���錾
	int         nNumVtx;		// ���f���̒��_��
	DWORD       dwSizeFVF;		// ���f���̒��_�t�H�[�}�b�g�̃T�C�Y
	BYTE        *pVtxBuff;		// ���f���̒��_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 vtx;			// ���f���̒��_���W
	D3DXVECTOR3 size;			// ���f���̑傫��

	// �����蔻��̍쐬
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{ // ���f���̍ő吔���J��Ԃ�

		// ���f���̒��_�����擾
		nNumVtx = g_aModel[nCntModel].pMesh->GetNumVertices();

		// ���f���̒��_�t�H�[�}�b�g�̃T�C�Y���擾
		dwSizeFVF = D3DXGetFVFVertexSize(g_aModel[nCntModel].pMesh->GetFVF());

		// ���f���̒��_�o�b�t�@�����b�N
		g_aModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{ // ���f���̒��_�����J��Ԃ�

			// ���f���̒��_���W����
			vtx = *(D3DXVECTOR3*)pVtxBuff;

			// ���_���W (x) �̐ݒ�
			if (vtx.x < g_aModel[nCntModel].vtxMin.x)
			{ // ����̒��_���W (x) ���A����̒��_���W (x) �����������ꍇ

				// ����̒��_��� (x) ����
				g_aModel[nCntModel].vtxMin.x = vtx.x;
			}
			else if (vtx.x > g_aModel[nCntModel].vtxMax.x)
			{ // ����̒��_���W (x) ���A����̒��_���W (x) �����傫���ꍇ

				// ����̒��_��� (x) ����
				g_aModel[nCntModel].vtxMax.x = vtx.x;
			}

			// ���_���W (y) �̐ݒ�
			if (vtx.y < g_aModel[nCntModel].vtxMin.y)
			{ // ����̒��_���W (y) ���A����̒��_���W (y) �����������ꍇ

				// ����̒��_��� (y) ����
				g_aModel[nCntModel].vtxMin.y = vtx.y;
			}
			else if (vtx.y > g_aModel[nCntModel].vtxMax.y)
			{ // ����̒��_���W (y) ���A����̒��_���W (y) �����傫���ꍇ

				// ����̒��_��� (y) ����
				g_aModel[nCntModel].vtxMax.y = vtx.y;
			}

			// ���_���W (z) �̐ݒ�
			if (vtx.z < g_aModel[nCntModel].vtxMin.z)
			{ // ����̒��_���W (z) ���A����̒��_���W (z) �����������ꍇ

				// ����̒��_��� (z) ����
				g_aModel[nCntModel].vtxMin.z = vtx.z;
			}
			else if (vtx.z > g_aModel[nCntModel].vtxMax.z)
			{ // ����̒��_���W (z) ���A����̒��_���W (z) �����傫���ꍇ

				// ����̒��_��� (z) ����
				g_aModel[nCntModel].vtxMax.z = vtx.z;
			}

			// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += dwSizeFVF;
		}

		// ���f���̒��_�o�b�t�@���A�����b�N
		g_aModel[nCntModel].pMesh->UnlockVertexBuffer();

		// ���f���T�C�Y�����߂�
		size = g_aModel[nCntModel].vtxMax - g_aModel[nCntModel].vtxMin;

		// ���f���̏c������
		g_aModel[nCntModel].fHeight = size.y;

		// ���f���̉~�̓����蔻����쐬
		g_aModel[nCntModel].fRadius = ((size.x * 0.5f) + (size.z * 0.5f)) * 0.5f;
	}
}

//============================================================
//	�e�N�X�`���̓ǂݍ���
//============================================================
HRESULT LoadTextureModel(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL     *pMat;						// �}�e���A���ւ̃|�C���^

	// �e�N�X�`���̓ǂݍ���
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{ // ���f���Ɏg�p���郂�f���̍ő吔���J��Ԃ�

		// �}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aModel[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aModel[nCntModel].dwNumMat; nCntMat++)
		{ // �}�e���A���̐����J��Ԃ�

			if (pMat[nCntMat].pTextureFilename != NULL)
			{ // �e�N�X�`���t�@�C�������݂���ꍇ

				// �e�N�X�`���̓ǂݍ���
				if (FAILED(D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aModel[nCntModel].pTexture[nCntMat])))
				{ // �e�N�X�`���̓ǂݍ��݂����s�����ꍇ

					// �G���[���b�Z�[�W�{�b�N�X
					MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);

					// ���s��Ԃ�
					return E_FAIL;
				}
			}
			else
			{ // �e�N�X�`���t�@�C�������݂��Ȃ��ꍇ

				// NULL��ݒ�
				g_aModel[nCntModel].pTexture[nCntMat] = NULL;
			}
		}
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	���f�����̎擾����
//============================================================
Model *GetModelData(int nID)
{
	// �����̃��f���̏��A�h���X��Ԃ�
	return &g_aModel[nID];
}