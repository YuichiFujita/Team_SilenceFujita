//======================================================================================================================
//
//	���b�V���E�H�[������ [meshwall.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "meshwall.h"

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTextureMeshWall[] =		// �e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\wall000.png",		// ���b�V���E�H�[���̃e�N�X�`���̑��΃p�X
};

//**********************************************************************************************************************
//	�񋓌^��` (TEXTURE_MESHWALL)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_MESHWALL_NORMAL = 0,		// ���b�V���E�H�[�� (�ʏ�)
	TEXTURE_MESHWALL_MAX,				// ���̗񋓌^�̑���
} TEXTURE_MESHWALL;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nPartWidth, int nPartHeight);	// ���b�V���E�H�[���̐ݒ菈��

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureMeshWall[TEXTURE_MESHWALL_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshWall = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9  g_pIdxBuffMeshWall = NULL;						// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

MeshWall g_aMeshWall[MAX_MESHWALL];		// ���b�V���E�H�[���̏��
int      g_nNeedVtxWall;				// �K�v���_��
int      g_nNeedIdxWall;				// �K�v�C���f�b�N�X��

//======================================================================================================================
//	���b�V���E�H�[���̏���������
//======================================================================================================================
void InitMeshWall(void)
{
	// �ϐ���錾
	int nNumVtx = 0;							// ���_���̌v���p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_3D *pVtx;							// ���_���ւ̃|�C���^
	WORD      *pIdx;							// �C���f�b�N�X���ւ̃|�C���^

	// �O���[�o���ϐ��̏�����
	g_nNeedVtxWall = 0;							// �K�v���_�̑���
	g_nNeedIdxWall = 0;							// �K�v�C���f�b�N�X�̑���

	// ���b�V���E�H�[���̏��̏�����
	for (int nCntMeshWall = 0; nCntMeshWall < MAX_MESHWALL; nCntMeshWall++)
	{ // ���b�V���E�H�[���̍ő�\�������J��Ԃ�

		g_aMeshWall[nCntMeshWall].pos         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aMeshWall[nCntMeshWall].rot         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aMeshWall[nCntMeshWall].fWidth      = 0.0f;							// ����
		g_aMeshWall[nCntMeshWall].fHeight     = 0.0f;							// �c��
		g_aMeshWall[nCntMeshWall].nPartWidth  = 0;								// ���̕�����
		g_aMeshWall[nCntMeshWall].nPartHeight = 0;								// �c�̕�����
		g_aMeshWall[nCntMeshWall].nNumVtx     = 0;								// �K�v���_��
		g_aMeshWall[nCntMeshWall].nNumIdx     = 0;								// �K�v�C���f�b�N�X��
		g_aMeshWall[nCntMeshWall].bUse        = false;							// �g�p��
	}

	// �e�N�X�`���̓ǂݍ���
	for (int nCntMeshWall = 0; nCntMeshWall < TEXTURE_MESHWALL_MAX; nCntMeshWall++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		D3DXCreateTextureFromFile(pDevice, apTextureMeshWall[nCntMeshWall], &g_apTextureMeshWall[nCntMeshWall]);
	}

	// ���b�V���E�H�[���̐ݒ�
	SetMeshWall(D3DXVECTOR3(0.0f, 0.0f, 800.0f),  D3DXVECTOR3(0.0f, D3DXToRadian(0),   0.0f), 1600.0f, 200.0f, 8, 1);
	SetMeshWall(D3DXVECTOR3(800.0f, 0.0f, 0.0f),  D3DXVECTOR3(0.0f, D3DXToRadian(90),  0.0f), 1600.0f, 200.0f, 8, 1);
	SetMeshWall(D3DXVECTOR3(0.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, D3DXToRadian(180), 0.0f), 1600.0f, 200.0f, 8, 1);
	SetMeshWall(D3DXVECTOR3(-800.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DXToRadian(270), 0.0f), 1600.0f, 200.0f, 8, 1);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_3D) * g_nNeedVtxWall,	// �K�v���_��
		D3DUSAGE_WRITEONLY,					// �g�p���@
		FVF_VERTEX_3D,						// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// �������̎w��
		&g_pVtxBuffMeshWall,				// ���_�o�b�t�@�ւ̃|�C���^
		NULL
	);

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer
	( // ����
		sizeof(WORD) * g_nNeedIdxWall,		// �K�v�C���f�b�N�X��
		D3DUSAGE_WRITEONLY,					// �g�p���@
		D3DFMT_INDEX16,						// �C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g
		D3DPOOL_MANAGED,					// �������̎w��
		&g_pIdxBuffMeshWall,				// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMeshWall = 0; nCntMeshWall < MAX_MESHWALL; nCntMeshWall++)
	{ // ���b�V���E�H�[���̍ő�\�������J��Ԃ�

		if (g_aMeshWall[nCntMeshWall].bUse == true)
		{ // ���b�V���E�H�[�����g�p����Ă���ꍇ

			for (int nCntHeight = 0; nCntHeight < g_aMeshWall[nCntMeshWall].nPartHeight + 1; nCntHeight++)
			{ // �c�̕����� +1��J��Ԃ�

				for (int nCntWidth = 0; nCntWidth < g_aMeshWall[nCntMeshWall].nPartWidth + 1; nCntWidth++)
				{ // ���̕����� +1��J��Ԃ�

					// ���_���W�̐ݒ�
					pVtx[0].pos = D3DXVECTOR3
					( // ����
						nCntWidth * (g_aMeshWall[nCntMeshWall].fWidth / (float)g_aMeshWall[nCntMeshWall].nPartWidth) - (g_aMeshWall[nCntMeshWall].fWidth * 0.5f),	// x
						-(nCntHeight * (g_aMeshWall[nCntMeshWall].fHeight / (float)g_aMeshWall[nCntMeshWall].nPartHeight)) + g_aMeshWall[nCntMeshWall].fHeight,		// y
						0.0f																																		// z
					);

					// �@���x�N�g���̐ݒ�
					pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
	g_pVtxBuffMeshWall->Unlock();

	//------------------------------------------------------------------------------------------------------------------
	//	�C���f�b�N�X���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// �C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshWall->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntMeshWall = 0; nCntMeshWall < MAX_MESHWALL; nCntMeshWall++)
	{ // ���b�V���E�H�[���̍ő�\�������J��Ԃ�

		if (g_aMeshWall[nCntMeshWall].bUse == true)
		{ // ���b�V���E�H�[�����g�p����Ă���ꍇ

			for (int nCntHeight = 0; nCntHeight < g_aMeshWall[nCntMeshWall].nPartHeight; nCntHeight++)
			{ // �c�̕����� +1��J��Ԃ�

				for (int nCntWidth = 0; nCntWidth < g_aMeshWall[nCntMeshWall].nPartWidth + 1; nCntWidth++)
				{ // ���̕����� +1��J��Ԃ�

					pIdx[0] = nNumVtx + ((g_aMeshWall[nCntMeshWall].nPartWidth + 1) * (nCntHeight + 1) + nCntWidth);
					pIdx[1] = nNumVtx + ((g_aMeshWall[nCntMeshWall].nPartWidth + 1) * nCntHeight + nCntWidth);

					// �C���f�b�N�X�f�[�^�̃|�C���^�� 2���i�߂�
					pIdx += 2;
				}

				if (nCntHeight != g_aMeshWall[nCntMeshWall].nPartHeight - 1)
				{ // ��Ԏ�O�̕����ꏊ�ł͂Ȃ��ꍇ

					pIdx[0] = nNumVtx + ((g_aMeshWall[nCntMeshWall].nPartWidth + 1) * nCntHeight + g_aMeshWall[nCntMeshWall].nPartWidth);
					pIdx[1] = nNumVtx + ((g_aMeshWall[nCntMeshWall].nPartWidth + 1) * (nCntHeight + 2));

					// �C���f�b�N�X�f�[�^�̃|�C���^�� 2���i�߂�
					pIdx += 2;
				}
			}

			// ���_�o�b�t�@�̊J�n�n�_��K�v�������炷
			nNumVtx += g_aMeshWall[nCntMeshWall].nNumVtx;
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshWall->Unlock();
}

//======================================================================================================================
//	���b�V���E�H�[���̏I������
//======================================================================================================================
void UninitMeshWall(void)
{
	// �e�N�X�`���̔j��
	for (int nCntMeshWall = 0; nCntMeshWall < TEXTURE_MESHWALL_MAX; nCntMeshWall++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTextureMeshWall[nCntMeshWall] != NULL)
		{ // �ϐ� (g_apTextureMeshWall) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureMeshWall[nCntMeshWall]->Release();
			g_apTextureMeshWall[nCntMeshWall] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshWall != NULL)
	{ // �ϐ� (g_pVtxBuffMeshWall) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffMeshWall->Release();
		g_pVtxBuffMeshWall = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffMeshWall != NULL)
	{ // �ϐ� (g_pIdxBuffMeshWall) ��NULL�ł͂Ȃ��ꍇ

		g_pIdxBuffMeshWall->Release();
		g_pIdxBuffMeshWall = NULL;
	}
}

//======================================================================================================================
//	���b�V���E�H�[���̍X�V����
//======================================================================================================================
void UpdateMeshWall(void)
{

}

//======================================================================================================================
//	���b�V���E�H�[���̕`�揈��
//======================================================================================================================
void DrawMeshWall(void)
{
	// �ϐ���錾
	int        nNumIdx = 0;						// �C���f�b�N�X���̌v���p
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	for (int nCntMeshWall = 0; nCntMeshWall < MAX_MESHWALL; nCntMeshWall++)
	{ // ���b�V���E�H�[���̍ő�\�������J��Ԃ�

		if (g_aMeshWall[nCntMeshWall].bUse == true)
		{ // ���b�V���E�H�[�����g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aMeshWall[nCntMeshWall].mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshWall[nCntMeshWall].rot.y, g_aMeshWall[nCntMeshWall].rot.x, g_aMeshWall[nCntMeshWall].rot.z);
			D3DXMatrixMultiply(&g_aMeshWall[nCntMeshWall].mtxWorld, &g_aMeshWall[nCntMeshWall].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aMeshWall[nCntMeshWall].pos.x, g_aMeshWall[nCntMeshWall].pos.y, g_aMeshWall[nCntMeshWall].pos.z);
			D3DXMatrixMultiply(&g_aMeshWall[nCntMeshWall].mtxWorld, &g_aMeshWall[nCntMeshWall].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshWall[nCntMeshWall].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffMeshWall, 0, sizeof(VERTEX_3D));

			// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffMeshWall);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureMeshWall[TEXTURE_MESHWALL_NORMAL]);

			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive
			( // ����
				D3DPT_TRIANGLESTRIP,					// �v���~�e�B�u�̎��
				0,
				0,
				g_aMeshWall[nCntMeshWall].nNumVtx,		// �g�p���钸�_��
				nNumIdx,								// �C���f�b�N�X�o�b�t�@�̊J�n�n�_
				g_aMeshWall[nCntMeshWall].nNumIdx - 2	// �v���~�e�B�u (�|���S��) ��
			);

			// �C���f�b�N�X�o�b�t�@�̊J�n�n�_��K�v�������炷
			nNumIdx += g_aMeshWall[nCntMeshWall].nNumIdx;
		}
	}
}

//======================================================================================================================
//	���b�V���E�H�[���̐ݒ菈��
//======================================================================================================================
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nPartWidth, int nPartHeight)
{
	for (int nCntMeshWall = 0; nCntMeshWall < MAX_MESHWALL; nCntMeshWall++)
	{ // ���b�V���E�H�[���̍ő�\�������J��Ԃ�

		if (g_aMeshWall[nCntMeshWall].bUse == false)
		{ // ���b�V���E�H�[�����g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aMeshWall[nCntMeshWall].pos         = pos;			// �ʒu
			g_aMeshWall[nCntMeshWall].rot         = rot;			// ����
			g_aMeshWall[nCntMeshWall].fWidth      = fWidth;			// ����
			g_aMeshWall[nCntMeshWall].fHeight     = fHeight;		// �c��
			g_aMeshWall[nCntMeshWall].nPartWidth  = nPartWidth;		// ���̕�����
			g_aMeshWall[nCntMeshWall].nPartHeight = nPartHeight;	// �c�̕�����

			// �g�p���Ă����Ԃɂ���
			g_aMeshWall[nCntMeshWall].bUse = true;

			// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̕K�v����ݒ�
			g_aMeshWall[nCntMeshWall].nNumVtx = (nPartWidth + 1) * (nPartHeight + 1);
			g_aMeshWall[nCntMeshWall].nNumIdx = (nPartWidth + 1) * (((nPartHeight + 1) * 2) - 2) + (nPartHeight * 2) - 2;

			// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̑��������Z
			g_nNeedVtxWall += g_aMeshWall[nCntMeshWall].nNumVtx;
			g_nNeedIdxWall += g_aMeshWall[nCntMeshWall].nNumIdx;

			// �����𔲂���
			break;
		}
	}
}

//======================================================================================================================
//	���b�V���E�H�[���̎擾����
//======================================================================================================================
MeshWall *GetMeshWall(void)
{
	// ���b�V���E�H�[���̏��̐擪�A�h���X��Ԃ�
	return &g_aMeshWall[0];
}