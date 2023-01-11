//=======================================================================================================
//
//	�e���� [shadow.cpp]
//	Author�F���c�E��
//
//=======================================================================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "shadow.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_SHADOW		(512)		// �g�p����|���S���� (�e�̍ő吔)

//************************************************************
//	�R���X�g��`
//************************************************************
const char *apTextureShadow[] =		// �e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\shadow000.jpg",	// �e�̃e�N�X�`���̑��΃p�X
};

//************************************************************
//	�񋓌^��` (TEXTURE_SHADOW)
//************************************************************
typedef enum
{
	TEXTURE_SHADOW_NORMAL = 0,		// �e (�ʏ�)
	TEXTURE_SHADOW_MAX,				// ���̗񋓌^�̑���
} TEXTURE_SHADOW;

//************************************************************
//	�\���̒�` (Shadow)
//************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 rot;				// ����
	D3DXMATRIX  mtxWorld;			// ���[���h�}�g���b�N�X
	int        *pShadowIDParent;	// �e�̐e�̉e�C���f�b�N�X
	bool       *pUseParent;			// �e�̐e�̎g�p��
	bool        bUse;				// �g�p��
} Shadow;

//************************************************************
//	�O���[�o���ϐ�
//************************************************************
LPDIRECT3DTEXTURE9      g_apTextureShadow[TEXTURE_SHADOW_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;					// ���_�o�b�t�@�ւ̃|�C���^

Shadow g_aShadow[MAX_SHADOW];		// �e�̏��

//=======================================================================================================
//	�e�̏���������
//=======================================================================================================
void InitShadow(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_3D *pVtx;							// ���_���ւ̃|�C���^

	// �e�N�X�`���̓ǂݍ���
	for (int nCntShadow = 0; nCntShadow < TEXTURE_SHADOW_MAX; nCntShadow++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		D3DXCreateTextureFromFile(pDevice, apTextureShadow[nCntShadow], &g_apTextureShadow[nCntShadow]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_3D) * 4 * MAX_SHADOW,		// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL
	);

	// �e�̏��̏�����
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{ // �e�̍ő�\�������J��Ԃ�

		g_aShadow[nCntShadow].pos             = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aShadow[nCntShadow].rot             = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aShadow[nCntShadow].pShadowIDParent = NULL;							// �e�̐e�̉e�C���f�b�N�X
		g_aShadow[nCntShadow].pUseParent      = NULL;							// �e�̐e�̎g�p��
		g_aShadow[nCntShadow].bUse            = false;							// �g�p��
	}

	//--------------------------------------------------------
	//	���_���̏�����
	//--------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{ // �e�̍ő�\�������J��Ԃ�

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShadow->Unlock();
}

//=======================================================================================================
//	�e�̏I������
//=======================================================================================================
void UninitShadow(void)
{
	// �e�N�X�`���̔j��
	for (int nCntShadow = 0; nCntShadow < TEXTURE_SHADOW_MAX; nCntShadow++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTextureShadow[nCntShadow] != NULL)
		{ // �ϐ� (g_apTextureShadow) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureShadow[nCntShadow]->Release();
			g_apTextureShadow[nCntShadow] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffShadow != NULL)
	{ // �ϐ� (g_pVtxBuffShadow) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//=======================================================================================================
//	�e�̍X�V����
//=======================================================================================================
void UpdateShadow(void)
{
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{ // �e�̍ő�\�������J��Ԃ�

		if (g_aShadow[nCntShadow].bUse == true && g_aShadow[nCntShadow].pUseParent != NULL)
		{ // �e���g�p����Ă��銎�A�e�̐e�̎g�p�󋵃A�h���X���ݒ肳��Ă���ꍇ

			if (*g_aShadow[nCntShadow].pUseParent == false)
			{ // �e�̐e���g�p����Ă��Ȃ��ꍇ

				// �e���g�p���Ă��Ȃ���Ԃɂ���
				g_aShadow[nCntShadow].bUse = false;
			}
			else
			{ // �e�̐e���g�p����Ă���ꍇ

				if (g_aShadow[nCntShadow].pShadowIDParent != NULL)
				{ // �e�̐e�̉e�C���f�b�N�X�A�h���X���ݒ肳��Ă���ꍇ

					if (*g_aShadow[nCntShadow].pShadowIDParent != nCntShadow)
					{ // �e�̐e�̉e�C���f�b�N�X�ƁA���݂̉e�C���f�b�N�X�̒l���Ⴄ�ꍇ

						// �e���g�p���Ă��Ȃ���Ԃɂ���
						g_aShadow[nCntShadow].bUse = false;

						// [��] �g�p���Ȃ���ԂɂȂ����e�����t���[�����œ����C���f�b�N�X���g����
						//      �܂��g�p���ꂽ�ꍇ�ɉe����̐e�ɓ��������Ă��܂����߁A���̖h�~�B
					}
				}
			}
		}
	}
}

//=======================================================================================================
//	�e�̕`�揈��
//=======================================================================================================
void DrawShadow(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{ // �e�̍ő�\�������J��Ԃ�

		if (g_aShadow[nCntShadow].bUse == true)
		{ // �e���g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureShadow[TEXTURE_SHADOW_NORMAL]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);
		}
	}

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=======================================================================================================
//	�e�̐ݒ菈��
//=======================================================================================================
int SetShadow(float fAlpha, float fRadius, int *pShadowID, bool *pUse)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{ // �e�̍ő�\�������J��Ԃ�

		if (g_aShadow[nCntShadow].bUse == false)
		{ // �e���g�p����Ă��Ȃ��ꍇ

			// �����̃A�h���X����
			g_aShadow[nCntShadow].pShadowIDParent = pShadowID;	// �e�̐e�̉e�C���f�b�N�X
			g_aShadow[nCntShadow].pUseParent      = pUse;		// �e�̐e�̎g�p��

			// �g�p���Ă����Ԃɂ���
			g_aShadow[nCntShadow].bUse = true;

			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

			// ���_�f�[�^�̃|�C���^���g�p����e�̃|�C���^�܂Ői�߂�
			pVtx += nCntShadow * 4;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-fRadius, 0.0f,  fRadius);
			pVtx[1].pos = D3DXVECTOR3( fRadius, 0.0f,  fRadius);
			pVtx[2].pos = D3DXVECTOR3(-fRadius, 0.0f, -fRadius);
			pVtx[3].pos = D3DXVECTOR3( fRadius, 0.0f, -fRadius);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);

			// ���_�o�b�t�@���A�����b�N����
			g_pVtxBuffShadow->Unlock();

			// �g�p�ł���e��ID��Ԃ�
			return nCntShadow;
		}
	}

	// �e�̑���������Ȃ��ꍇ -1��Ԃ�
	return -1;
}

//=======================================================================================================
//	�e�̈ʒu�̐ݒ菈��
//=======================================================================================================
void SetPositionShadow(int nShadowID, D3DXVECTOR3 pos)
{
	if (nShadowID > NONE_SHADOW)
	{ // �g�p�ł���ID�̏ꍇ

		// �����̉e�̈ʒu��ݒ�
		g_aShadow[nShadowID].pos.x = pos.x;
		g_aShadow[nShadowID].pos.z = pos.z;
	}
}