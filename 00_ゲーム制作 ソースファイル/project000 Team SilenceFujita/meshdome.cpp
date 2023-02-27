//======================================================================================================================
//
//	���b�V���h�[������ [meshdome.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "meshdome.h"
#include "weather.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAX_MESHDOME		(1)		// ���b�V���h�[���̍ő吔

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTextureMeshDome[] =	// �e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\sky000.png",	// ���b�V���h�[���̃e�N�X�`���̑��΃p�X
};

//**********************************************************************************************************************
//	�񋓌^��` (TEXTURE_MESHDOME)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_MESHDOME_NORMAL = 0,	// ���b�V���h�[�� (�ʏ�)
	TEXTURE_MESHDOME_MAX,			// ���̗񋓌^�̑���
} TEXTURE_MESHDOME;

//**********************************************************************************************************************
//	�\���̒�` (MeshDome)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 rot;				// ����
	D3DXMATRIX  mtxWorld;			// ���[���h�}�g���b�N�X
	float       fRadius;			// ���a
	int         nPartWidth;			// ���̕�����
	int         nPartHeight;		// �c�̕�����
	int         nNumVtx;			// �K�v���_��
	int         nNumIdx;			// �K�v�C���f�b�N�X��
	bool        bUse;				// �g�p��
} MeshDome;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void SetMeshDome(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, int nPartWidth, int nPartHeight);	// ���b�V���h�[���̐ݒ菈��
void TxtSetMeshDome(void);																			// ���b�V���h�[���̃Z�b�g�A�b�v����

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureMeshDome[TEXTURE_MESHDOME_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshDome = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9  g_pIdxBuffMeshDome = NULL;						// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

MeshDome g_aMeshDome[MAX_MESHDOME];	// ���b�V���h�[���̏��
int      g_nNeedVtxDome;			// �K�v���_��
int      g_nNeedIdxDome;			// �K�v�C���f�b�N�X��

//======================================================================================================================
//	���b�V���h�[���̏���������
//======================================================================================================================
void InitMeshDome(void)
{
	// �ϐ���錾
	D3DXVECTOR3 vecPos;							// ���_�ʒu�̌v�Z�p
	D3DXVECTOR3 vecNor;							// �@���x�N�g���̌v�Z�p
	float       fRotWidth, fRotHeight;			// ���_�ʒu�̕����̌v�Z�p
	int         nNumVtx = 0;					// ���_���̌v���p
	WEATHERTYPE weather = GetWeather();			// �V�C

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_3D *pVtx;							// ���_���ւ̃|�C���^
	WORD      *pIdx;							// �C���f�b�N�X���ւ̃|�C���^
	
	// �O���[�o���ϐ��̏�����
	g_nNeedVtxDome = 0;							// �K�v���_�̑���
	g_nNeedIdxDome = 0;							// �K�v�C���f�b�N�X�̑���

	// ���b�V���h�[���̏��̏�����
	for (int nCntMeshDome = 0; nCntMeshDome < MAX_MESHDOME; nCntMeshDome++)
	{ // ���b�V���h�[���̍ő�\�������J��Ԃ�

		g_aMeshDome[nCntMeshDome].pos         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aMeshDome[nCntMeshDome].rot         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aMeshDome[nCntMeshDome].fRadius     = 0.0f;							// ���a
		g_aMeshDome[nCntMeshDome].nPartWidth  = 0;								// ���̕�����
		g_aMeshDome[nCntMeshDome].nPartHeight = 0;								// �c�̕�����
		g_aMeshDome[nCntMeshDome].nNumVtx     = 0;								// �K�v���_��
		g_aMeshDome[nCntMeshDome].nNumIdx     = 0;								// �K�v�C���f�b�N�X��
		g_aMeshDome[nCntMeshDome].bUse        = false;							// �g�p��
	}

	// �e�N�X�`���̓ǂݍ���
	for (int nCntMeshDome = 0; nCntMeshDome < TEXTURE_MESHDOME_MAX; nCntMeshDome++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		D3DXCreateTextureFromFile(pDevice, apTextureMeshDome[nCntMeshDome], &g_apTextureMeshDome[nCntMeshDome]);
	}

	// ���b�V���h�[���̃Z�b�g�A�b�v
	TxtSetMeshDome();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_3D) * g_nNeedVtxDome,	// �K�v���_��
		D3DUSAGE_WRITEONLY,					// �g�p���@
		FVF_VERTEX_3D,						// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// �������̎w��
		&g_pVtxBuffMeshDome,				// ���_�o�b�t�@�ւ̃|�C���^
		NULL
	);

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer
	( // ����
		sizeof(WORD) * g_nNeedIdxDome,		// �K�v�C���f�b�N�X��
		D3DUSAGE_WRITEONLY,					// �g�p���@
		D3DFMT_INDEX16,						// �C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g
		D3DPOOL_MANAGED,					// �������̎w��
		&g_pIdxBuffMeshDome,				// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshDome->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMeshDome = 0; nCntMeshDome < MAX_MESHDOME; nCntMeshDome++)
	{ // ���b�V���h�[���̍ő�\�������J��Ԃ�

		if (g_aMeshDome[nCntMeshDome].bUse == true)
		{ // ���b�V���h�[�����g�p����Ă���ꍇ

			for (int nCntHeight = 0; nCntHeight < g_aMeshDome[nCntMeshDome].nPartHeight + 1; nCntHeight++)
			{ // �c�̕����� +1��J��Ԃ�

				for (int nCntWidth = 0; nCntWidth < g_aMeshDome[nCntMeshDome].nPartWidth + 1; nCntWidth++)
				{ // ���̕����� +1��J��Ԃ�

					// ���_�̌������v�Z
					fRotWidth  = nCntWidth  * ((D3DX_PI * 2.0f) / g_aMeshDome[nCntMeshDome].nPartWidth);
					fRotHeight = nCntHeight * ((D3DX_PI * 0.5f) / g_aMeshDome[nCntMeshDome].nPartHeight);

					// ���_���W�̕�����ݒ�
					vecPos = D3DXVECTOR3
					( // ����
						g_aMeshDome[nCntMeshDome].fRadius * sinf(fRotHeight) * sinf(fRotWidth),	// x
						g_aMeshDome[nCntMeshDome].fRadius * cosf(fRotHeight),					// y
						g_aMeshDome[nCntMeshDome].fRadius * sinf(fRotHeight) * cosf(fRotWidth)	// z
					);

					// ���_���W�̐ݒ�
					pVtx[0].pos = g_aMeshDome[nCntMeshDome].pos + vecPos;

					// �@���x�N�g���̕�����ݒ�
					vecNor = -vecPos;

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
					pVtx[0].tex = D3DXVECTOR2
					( // ����
						nCntWidth  * 1.0f / (float)g_aMeshDome[nCntMeshDome].nPartWidth,	// u
						nCntHeight * 1.0f / (float)g_aMeshDome[nCntMeshDome].nPartHeight	// v
					);

					// ���_�f�[�^�̃|�C���^�� 1���i�߂�
					pVtx += 1;
				}
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshDome->Unlock();

	//------------------------------------------------------------------------------------------------------------------
	//	�C���f�b�N�X���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// �C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshDome->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntMeshDome = 0; nCntMeshDome < MAX_MESHDOME; nCntMeshDome++)
	{ // ���b�V���h�[���̍ő�\�������J��Ԃ�

		if (g_aMeshDome[nCntMeshDome].bUse == true)
		{ // ���b�V���h�[�����g�p����Ă���ꍇ

			for (int nCntHeight = 0, nCntWidth = 0; nCntHeight < g_aMeshDome[nCntMeshDome].nPartHeight; nCntHeight++)
			{ // �c�̕����� +1��J��Ԃ�

				for (nCntWidth = 0; nCntWidth < g_aMeshDome[nCntMeshDome].nPartWidth + 1; nCntWidth++)
				{ // ���̕����� +1��J��Ԃ�

					pIdx[0] = nNumVtx + ((g_aMeshDome[nCntMeshDome].nPartWidth + 1) * (nCntHeight + 1) + nCntWidth);
					pIdx[1] = nNumVtx + ((g_aMeshDome[nCntMeshDome].nPartWidth + 1) * nCntHeight + nCntWidth);

					// �C���f�b�N�X�f�[�^�̃|�C���^�� 2���i�߂�
					pIdx += 2;
				}

				if (nCntHeight != g_aMeshDome[nCntMeshDome].nPartHeight - 1)
				{ // ��Ԏ�O�̕����ꏊ�ł͂Ȃ��ꍇ

					pIdx[0] = nNumVtx + ((g_aMeshDome[nCntMeshDome].nPartWidth + 1) * nCntHeight + nCntWidth - 1);
					pIdx[1] = nNumVtx + ((g_aMeshDome[nCntMeshDome].nPartWidth + 1) * (nCntHeight + 2));

					// �C���f�b�N�X�f�[�^�̃|�C���^�� 2���i�߂�
					pIdx += 2;
				}
			}

			// ���_�o�b�t�@�̊J�n�n�_��K�v�������炷
			nNumVtx += g_aMeshDome[nCntMeshDome].nNumVtx;
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshDome->Unlock();
}

//======================================================================================================================
//	���b�V���h�[���̏I������
//======================================================================================================================
void UninitMeshDome(void)
{
	// �e�N�X�`���̔j��
	for (int nCntMeshDome = 0; nCntMeshDome < TEXTURE_MESHDOME_MAX; nCntMeshDome++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTextureMeshDome[nCntMeshDome] != NULL)
		{ // �ϐ� (g_apTextureMeshDome) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureMeshDome[nCntMeshDome]->Release();
			g_apTextureMeshDome[nCntMeshDome] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshDome != NULL)
	{ // �ϐ� (g_pVtxBuffMeshDome) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffMeshDome->Release();
		g_pVtxBuffMeshDome = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffMeshDome != NULL)
	{ // �ϐ� (g_pIdxBuffMeshDome) ��NULL�ł͂Ȃ��ꍇ

		g_pIdxBuffMeshDome->Release();
		g_pIdxBuffMeshDome = NULL;
	}
}

//======================================================================================================================
//	���b�V���h�[���̍X�V����
//======================================================================================================================
void UpdateMeshDome(void)
{

}

//======================================================================================================================
//	���b�V���h�[���̕`�揈��
//======================================================================================================================
void DrawMeshDome(void)
{
	// �ϐ���錾
	int        nNumIdx = 0;						// �C���f�b�N�X���̌v���p
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntMeshDome = 0; nCntMeshDome < MAX_MESHDOME; nCntMeshDome++)
	{ // ���b�V���h�[���̍ő�\�������J��Ԃ�

		if (g_aMeshDome[nCntMeshDome].bUse == true)
		{ // ���b�V���h�[�����g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aMeshDome[nCntMeshDome].mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshDome[nCntMeshDome].rot.y, g_aMeshDome[nCntMeshDome].rot.x, g_aMeshDome[nCntMeshDome].rot.z);
			D3DXMatrixMultiply(&g_aMeshDome[nCntMeshDome].mtxWorld, &g_aMeshDome[nCntMeshDome].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aMeshDome[nCntMeshDome].pos.x, g_aMeshDome[nCntMeshDome].pos.y, g_aMeshDome[nCntMeshDome].pos.z);
			D3DXMatrixMultiply(&g_aMeshDome[nCntMeshDome].mtxWorld, &g_aMeshDome[nCntMeshDome].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshDome[nCntMeshDome].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffMeshDome, 0, sizeof(VERTEX_3D));

			// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffMeshDome);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureMeshDome[TEXTURE_MESHDOME_NORMAL]);

			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive
			( // ����
				D3DPT_TRIANGLESTRIP,					// �v���~�e�B�u�̎��
				0,
				0,
				g_aMeshDome[nCntMeshDome].nNumVtx,		// �g�p���钸�_��
				nNumIdx,								// �C���f�b�N�X�o�b�t�@�̊J�n�n�_
				g_aMeshDome[nCntMeshDome].nNumIdx - 2	// �v���~�e�B�u (�|���S��) ��
			);

			// �C���f�b�N�X�o�b�t�@�̊J�n�n�_��K�v�������炷
			nNumIdx += g_aMeshDome[nCntMeshDome].nNumIdx;
		}
	}

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//======================================================================================================================
//	���b�V���h�[���̐ݒ菈��
//======================================================================================================================
void SetMeshDome(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, int nPartWidth, int nPartHeight)
{
	for (int nCntMeshDome = 0; nCntMeshDome < MAX_MESHDOME; nCntMeshDome++)
	{ // ���b�V���h�[���̍ő�\�������J��Ԃ�

		if (g_aMeshDome[nCntMeshDome].bUse == false)
		{ // ���b�V���h�[�����g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aMeshDome[nCntMeshDome].pos         = pos;			// �ʒu
			g_aMeshDome[nCntMeshDome].rot         = rot;			// ����
			g_aMeshDome[nCntMeshDome].fRadius     = fRadius;		// ���a
			g_aMeshDome[nCntMeshDome].nPartWidth  = nPartWidth;		// ���̕�����
			g_aMeshDome[nCntMeshDome].nPartHeight = nPartHeight;	// �c�̕�����

			// �g�p���Ă����Ԃɂ���
			g_aMeshDome[nCntMeshDome].bUse = true;

			// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̕K�v����ݒ�
			g_aMeshDome[nCntMeshDome].nNumVtx = (nPartWidth + 1) * (nPartHeight + 1);
			g_aMeshDome[nCntMeshDome].nNumIdx = (nPartWidth + 1) * (((nPartHeight + 1) * 2) - 2) + (nPartHeight * 2) - 2;

			// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̑��������Z
			g_nNeedVtxDome += g_aMeshDome[nCntMeshDome].nNumVtx;
			g_nNeedIdxDome += g_aMeshDome[nCntMeshDome].nNumIdx;

			// �����𔲂���
			break;
		}
	}
}

//======================================================================================================================
//	���b�V���h�[���̃Z�b�g�A�b�v����
//======================================================================================================================
void TxtSetMeshDome(void)
{
	// �ϐ���錾
	D3DXVECTOR3 pos;			// �ʒu�̑���p
	D3DXVECTOR3 rot;			// �����̑���p
	float       fRadius;		// ���a�̑���p
	int         nPartWidth;		// ���̕������̑���p
	int         nPartHeight;	// �c�̕������̑���p
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

			if (strcmp(&aString[0], "STAGE_MESHDOMESET") == 0)
			{ // �ǂݍ��񂾕����� STAGE_MESHDOMESET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_STAGE_MESHDOMESET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "MESHDOMESET") == 0)
					{ // �ǂݍ��񂾕����� MESHDOMESET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_MESHDOMESET �ł͂Ȃ��ꍇ���[�v

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

						} while (strcmp(&aString[0], "END_MESHDOMESET") != 0);	// �ǂݍ��񂾕����� END_MESHDOMESET �ł͂Ȃ��ꍇ���[�v

						// ���b�V���t�B�[���h�̐ݒ�
						SetMeshDome(pos, D3DXToRadian(rot), fRadius, nPartWidth, nPartHeight);
					}
				} while (strcmp(&aString[0], "END_STAGE_MESHDOMESET") != 0);	// �ǂݍ��񂾕����� END_STAGE_MESHDOMESET �ł͂Ȃ��ꍇ���[�v
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