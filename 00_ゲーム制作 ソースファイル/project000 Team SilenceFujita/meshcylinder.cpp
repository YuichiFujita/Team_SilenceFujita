//======================================================================================================================
//
//	���b�V���V�����_�[���� [meshcylinder.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "meshcylinder.h"
#include "weather.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAX_MESHCYLINDER	(4)			// ���b�V���V�����_�[�̍ő吔
#define RES_PLUS_POS_Y		(100.0f)	// ���U���g���� y���W�̉��Z��

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTextureMeshCylinder[] =	// �e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\scenery000.png",	// �r���X (���邢) �̃e�N�X�`���̑��΃p�X
	"data\\TEXTURE\\scenery001.png",	// �r���X (�ʏ�) �̃e�N�X�`���̑��΃p�X
	"data\\TEXTURE\\scenery002.png",	// �r���X (�Â�) �̃e�N�X�`���̑��΃p�X
};

//**********************************************************************************************************************
//	�񋓌^��` (TEXTURE_MESHCYLINDER)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_MESHCYLINDER_BRIGHT = 0,	// ���b�V���V�����_�[ (���邢)
	TEXTURE_MESHCYLINDER_NORMAL,		// ���b�V���V�����_�[ (�ʏ�)
	TEXTURE_MESHCYLINDER_DARK,			// ���b�V���V�����_�[ (�Â�)
	TEXTURE_MESHCYLINDER_MAX,			// ���̗񋓌^�̑���
} TEXTURE_MESHCYLINDER;

//**********************************************************************************************************************
//	�\���̒�` (MeshCylinder)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;					// �ʒu
	D3DXVECTOR3 rot;					// ����
	D3DXMATRIX  mtxWorld;				// ���[���h�}�g���b�N�X
	float       fRadius;				// ���a
	float       fHeight;				// �c��
	int         nPartWidth;				// ���̕�����
	int         nPartHeight;			// �c�̕�����
	int         nNumVtx;				// �K�v���_��
	int         nNumIdx;				// �K�v�C���f�b�N�X��
	int         nType;					// ���
	bool        bUse;					// �g�p��
} MeshCylinder;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void SetMeshCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fHeight, int nPartWidth, int nPartHeight, int nType);	// ���b�V���V�����_�[�̐ݒ菈��
void TxtSetMeshCylinder(void);																										// ���b�V���V�����_�[�̃Z�b�g�A�b�v����

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureMeshCylinder[TEXTURE_MESHCYLINDER_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;							// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9  g_pIdxBuffMeshCylinder = NULL;							// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

MeshCylinder g_aMeshCylinder[MAX_MESHCYLINDER];	// ���b�V���V�����_�[�̏��
int          g_nNeedVtxCylinder;				// �K�v���_��
int          g_nNeedIdxCylinder;				// �K�v�C���f�b�N�X��

//======================================================================================================================
//	���b�V���V�����_�[�̏���������
//======================================================================================================================
void InitMeshCylinder(void)
{
	// �ϐ���錾
	D3DXVECTOR3 vecPos;							// ���_�ʒu�̌v�Z�p
	D3DXVECTOR3 vecNor;							// �@���x�N�g���̌v�Z�p
	int         nNumVtx = 0;					// ���_���̌v���p
	WEATHERTYPE weather = GetWeather();			// �V�C

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_3D *pVtx;							// ���_���ւ̃|�C���^
	WORD      *pIdx;							// �C���f�b�N�X���ւ̃|�C���^
	
	// �O���[�o���ϐ��̏�����
	g_nNeedVtxCylinder = 0;						// �K�v���_�̑���
	g_nNeedIdxCylinder = 0;						// �K�v�C���f�b�N�X�̑���

	// ���b�V���V�����_�[�̏��̏�����
	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{ // ���b�V���V�����_�[�̍ő�\�������J��Ԃ�

		g_aMeshCylinder[nCntMeshCylinder].pos         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aMeshCylinder[nCntMeshCylinder].rot         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aMeshCylinder[nCntMeshCylinder].fRadius     = 0.0f;							// ���a
		g_aMeshCylinder[nCntMeshCylinder].fHeight     = 0.0f;							// �c��
		g_aMeshCylinder[nCntMeshCylinder].nPartWidth  = 0;								// ���̕�����
		g_aMeshCylinder[nCntMeshCylinder].nPartHeight = 0;								// �c�̕�����
		g_aMeshCylinder[nCntMeshCylinder].nNumVtx     = 0;								// �K�v���_��
		g_aMeshCylinder[nCntMeshCylinder].nNumIdx     = 0;								// �K�v�C���f�b�N�X��
		g_aMeshCylinder[nCntMeshCylinder].nType       = 0;								// ���
		g_aMeshCylinder[nCntMeshCylinder].bUse        = false;							// �g�p��
	}

	// �e�N�X�`���̓ǂݍ���
	for (int nCntMeshCylinder = 0; nCntMeshCylinder < TEXTURE_MESHCYLINDER_MAX; nCntMeshCylinder++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		D3DXCreateTextureFromFile(pDevice, apTextureMeshCylinder[nCntMeshCylinder], &g_apTextureMeshCylinder[nCntMeshCylinder]);
	}

	// ���b�V���V�����_�[�̃Z�b�g�A�b�v
	TxtSetMeshCylinder();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_3D) * g_nNeedVtxCylinder,	// �K�v���_��
		D3DUSAGE_WRITEONLY,						// �g�p���@
		FVF_VERTEX_3D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,						// �������̎w��
		&g_pVtxBuffMeshCylinder,				// ���_�o�b�t�@�ւ̃|�C���^
		NULL
	);

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer
	( // ����
		sizeof(WORD) * g_nNeedIdxCylinder,		// �K�v�C���f�b�N�X��
		D3DUSAGE_WRITEONLY,						// �g�p���@
		D3DFMT_INDEX16,							// �C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g
		D3DPOOL_MANAGED,						// �������̎w��
		&g_pIdxBuffMeshCylinder,				// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{ // ���b�V���V�����_�[�̍ő�\�������J��Ԃ�

		if (g_aMeshCylinder[nCntMeshCylinder].bUse == true)
		{ // ���b�V���V�����_�[���g�p����Ă���ꍇ

			for (int nCntHeight = 0; nCntHeight < g_aMeshCylinder[nCntMeshCylinder].nPartHeight + 1; nCntHeight++)
			{ // �c�̕����� +1��J��Ԃ�

				for (int nCntWidth = 0; nCntWidth < g_aMeshCylinder[nCntMeshCylinder].nPartWidth + 1; nCntWidth++)
				{ // ���̕����� +1��J��Ԃ�

					// ���_���W�̕�����ݒ�
					vecPos = D3DXVECTOR3
					( // ����
						sinf(D3DXToRadian(nCntWidth * (360 / (float)g_aMeshCylinder[nCntMeshCylinder].nPartWidth))) * g_aMeshCylinder[nCntMeshCylinder].fRadius,						// x
						-(nCntHeight * (g_aMeshCylinder[nCntMeshCylinder].fHeight / (float)g_aMeshCylinder[nCntMeshCylinder].nPartHeight)) + g_aMeshCylinder[nCntMeshCylinder].fHeight,	// y
						cosf(D3DXToRadian(nCntWidth * (360 / (float)g_aMeshCylinder[nCntMeshCylinder].nPartWidth))) * g_aMeshCylinder[nCntMeshCylinder].fRadius							// z
					);

					// ���_���W�̐ݒ�
					pVtx[0].pos = g_aMeshCylinder[nCntMeshCylinder].pos + vecPos;

					// �@���x�N�g���̕�����ݒ�
					vecNor = D3DXVECTOR3(-vecPos.x, 0.0f, -vecPos.z);

					// �x�N�g���𐳋K��
					D3DXVec3Normalize(&vecNor, &vecNor);

					// �@���x�N�g���̐ݒ�
					pVtx[0].nor = vecNor;

					switch (weather)
					{
					case WEATHERTYPE_SUNNY:		// ����

						// ���_�J���[�̐ݒ�
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						break;					// �����o��

					case WEATHERTYPE_RAIN:		// �J

						// ���_�J���[�̐ݒ�
						pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

						break;					// �����o��

					case WEATHERTYPE_SNOW:		// ��

						// ���_�J���[�̐ݒ�
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						break;					// �����o��

					case WEATHERTYPE_THUNDER:	// ���J

						// ���_�J���[�̐ݒ�
						pVtx[0].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

						break;					// �����o��
					}

					// �e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(1.0f * (nCntWidth % 2), 1.0f * nCntHeight);

					// ���_�f�[�^�̃|�C���^�� 1���i�߂�
					pVtx += 1;
				}
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshCylinder->Unlock();

	//------------------------------------------------------------------------------------------------------------------
	//	�C���f�b�N�X���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// �C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{ // ���b�V���V�����_�[�̍ő�\�������J��Ԃ�

		if (g_aMeshCylinder[nCntMeshCylinder].bUse == true)
		{ // ���b�V���V�����_�[���g�p����Ă���ꍇ

			for (int nCntHeight = 0, nCntWidth = 0; nCntHeight < g_aMeshCylinder[nCntMeshCylinder].nPartHeight; nCntHeight++)
			{ // �c�̕����� +1��J��Ԃ�

				for (nCntWidth = 0; nCntWidth < g_aMeshCylinder[nCntMeshCylinder].nPartWidth + 1; nCntWidth++)
				{ // ���̕����� +1��J��Ԃ�

					pIdx[0] = nNumVtx + ((g_aMeshCylinder[nCntMeshCylinder].nPartWidth + 1) * (nCntHeight + 1) + nCntWidth);
					pIdx[1] = nNumVtx + ((g_aMeshCylinder[nCntMeshCylinder].nPartWidth + 1) * nCntHeight + nCntWidth);

					// �C���f�b�N�X�f�[�^�̃|�C���^�� 2���i�߂�
					pIdx += 2;
				}

				if (nCntHeight != g_aMeshCylinder[nCntMeshCylinder].nPartHeight - 1)
				{ // ��Ԏ�O�̕����ꏊ�ł͂Ȃ��ꍇ

					pIdx[0] = nNumVtx + ((g_aMeshCylinder[nCntMeshCylinder].nPartWidth + 1) * nCntHeight + nCntWidth - 1);
					pIdx[1] = nNumVtx + ((g_aMeshCylinder[nCntMeshCylinder].nPartWidth + 1) * (nCntHeight + 2));

					// �C���f�b�N�X�f�[�^�̃|�C���^�� 2���i�߂�
					pIdx += 2;
				}
			}

			// ���_�o�b�t�@�̊J�n�n�_��K�v�������炷
			nNumVtx += g_aMeshCylinder[nCntMeshCylinder].nNumVtx;
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshCylinder->Unlock();
}

//======================================================================================================================
//	���b�V���V�����_�[�̏I������
//======================================================================================================================
void UninitMeshCylinder(void)
{
	// �e�N�X�`���̔j��
	for (int nCntMeshCylinder = 0; nCntMeshCylinder < TEXTURE_MESHCYLINDER_MAX; nCntMeshCylinder++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTextureMeshCylinder[nCntMeshCylinder] != NULL)
		{ // �ϐ� (g_apTextureMeshCylinder) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureMeshCylinder[nCntMeshCylinder]->Release();
			g_apTextureMeshCylinder[nCntMeshCylinder] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshCylinder != NULL)
	{ // �ϐ� (g_pVtxBuffMeshCylinder) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffMeshCylinder != NULL)
	{ // �ϐ� (g_pIdxBuffMeshCylinder) ��NULL�ł͂Ȃ��ꍇ

		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}
}

//======================================================================================================================
//	���b�V���V�����_�[�̍X�V����
//======================================================================================================================
void UpdateMeshCylinder(void)
{

}

//======================================================================================================================
//	���b�V���V�����_�[�̕`�揈��
//======================================================================================================================
void DrawMeshCylinder(void)
{
	// �ϐ���錾
	int        nNumIdx = 0;			// �C���f�b�N�X���̌v���p
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// ���e�X�g�̗L�� / �����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 200);				// ���e�X�g�̎Q�ƒl�ݒ�

	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{ // ���b�V���V�����_�[�̍ő�\�������J��Ԃ�

		if (g_aMeshCylinder[nCntMeshCylinder].bUse == true)
		{ // ���b�V���V�����_�[���g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aMeshCylinder[nCntMeshCylinder].mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshCylinder[nCntMeshCylinder].rot.y, g_aMeshCylinder[nCntMeshCylinder].rot.x, g_aMeshCylinder[nCntMeshCylinder].rot.z);
			D3DXMatrixMultiply(&g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aMeshCylinder[nCntMeshCylinder].pos.x, g_aMeshCylinder[nCntMeshCylinder].pos.y, g_aMeshCylinder[nCntMeshCylinder].pos.z);
			D3DXMatrixMultiply(&g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshCylinder[nCntMeshCylinder].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));

			// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffMeshCylinder);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureMeshCylinder[g_aMeshCylinder[nCntMeshCylinder].nType]);

			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive
			( // ����
				D3DPT_TRIANGLESTRIP,							// �v���~�e�B�u�̎��
				0,
				0,
				g_aMeshCylinder[nCntMeshCylinder].nNumVtx,		// �g�p���钸�_��
				nNumIdx,										// �C���f�b�N�X�o�b�t�@�̊J�n�n�_
				g_aMeshCylinder[nCntMeshCylinder].nNumIdx - 2	// �v���~�e�B�u (�|���S��) ��
			);

			// �C���f�b�N�X�o�b�t�@�̊J�n�n�_��K�v�������炷
			nNumIdx += g_aMeshCylinder[nCntMeshCylinder].nNumIdx;
		}
	}

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g�̗L�� / �����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// ���e�X�g�̎Q�ƒl�ݒ�
}

//======================================================================================================================
//	���b�V���V�����_�[�̐ݒ菈��
//======================================================================================================================
void SetMeshCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fHeight, int nPartWidth, int nPartHeight, int nType)
{
	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{ // ���b�V���V�����_�[�̍ő�\�������J��Ԃ�

		if (g_aMeshCylinder[nCntMeshCylinder].bUse == false)
		{ // ���b�V���V�����_�[���g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aMeshCylinder[nCntMeshCylinder].pos         = pos;			// �ʒu
			g_aMeshCylinder[nCntMeshCylinder].rot         = rot;			// ����
			g_aMeshCylinder[nCntMeshCylinder].fRadius     = fRadius;		// ���a
			g_aMeshCylinder[nCntMeshCylinder].fHeight     = fHeight;		// �c��
			g_aMeshCylinder[nCntMeshCylinder].nPartWidth  = nPartWidth;		// ���̕�����
			g_aMeshCylinder[nCntMeshCylinder].nPartHeight = nPartHeight;	// �c�̕�����
			g_aMeshCylinder[nCntMeshCylinder].nType       = nType;			// ���]

			// ���U���g�̏ꍇ y���W�����Z
			g_aMeshCylinder[nCntMeshCylinder].pos.y += (GetMode() == MODE_RESULT) ? RES_PLUS_POS_Y : 0;

			// �g�p���Ă����Ԃɂ���
			g_aMeshCylinder[nCntMeshCylinder].bUse = true;

			// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̕K�v����ݒ�
			g_aMeshCylinder[nCntMeshCylinder].nNumVtx = (nPartWidth + 1) * (nPartHeight + 1);
			g_aMeshCylinder[nCntMeshCylinder].nNumIdx = (nPartWidth + 1) * (((nPartHeight + 1) * 2) - 2) + (nPartHeight * 2) - 2;

			// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̑��������Z
			g_nNeedVtxCylinder += g_aMeshCylinder[nCntMeshCylinder].nNumVtx;
			g_nNeedIdxCylinder += g_aMeshCylinder[nCntMeshCylinder].nNumIdx;

			// �����𔲂���
			break;
		}
	}
}

//======================================================================================================================
//	���b�V���V�����_�[�̃Z�b�g�A�b�v����
//======================================================================================================================
void TxtSetMeshCylinder(void)
{
	// �ϐ���錾
	D3DXVECTOR3 pos;			// �ʒu�̑���p
	D3DXVECTOR3 rot;			// �����̑���p
	float       fRadius;		// ���a�̑���p
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

			if (strcmp(&aString[0], "STAGE_MESHCYLINDERSET") == 0)
			{ // �ǂݍ��񂾕����� STAGE_MESHCYLINDERSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_STAGE_MESHCYLINDERSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "MESHCYLINDERSET") == 0)
					{ // �ǂݍ��񂾕����� MESHCYLINDERSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_MESHCYLINDERSET �ł͂Ȃ��ꍇ���[�v

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
							else if (strcmp(&aString[0], "RADIUS") == 0)
							{ // �ǂݍ��񂾕����� RADIUS �̏ꍇ
								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &fRadius);		// ���a��ǂݍ���
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

						} while (strcmp(&aString[0], "END_MESHCYLINDERSET") != 0);	// �ǂݍ��񂾕����� END_MESHCYLINDERSET �ł͂Ȃ��ꍇ���[�v

						// ���b�V���t�B�[���h�̐ݒ�
						SetMeshCylinder(pos, D3DXToRadian(rot), fRadius, fHeight, nPartWidth, nPartHeight, nType);
					}
				} while (strcmp(&aString[0], "END_STAGE_MESHCYLINDERSET") != 0);	// �ǂݍ��񂾕����� END_STAGE_MESHCYLINDERSET �ł͂Ȃ��ꍇ���[�v
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