//============================================================
//
//	�e���� [shadow.cpp]
//	Author�F���c�E��
//
//============================================================
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
	D3DXMATRIX  mtxParent;			// �e�̐e�̃��[���h�}�g���b�N�X
	Model       modelDataParent;	// �e�̐e�̃��f�����
	SHADOWTYPE  type;				// ���
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
		g_aShadow[nCntShadow].type            = SHADOWTYPE_NONE;				// ���
		g_aShadow[nCntShadow].pShadowIDParent = NULL;							// �e�̐e�̉e�C���f�b�N�X
		g_aShadow[nCntShadow].pUseParent      = NULL;							// �e�̐e�̎g�p��
		g_aShadow[nCntShadow].bUse            = false;							// �g�p��

		// ���f�����̏�����
		g_aShadow[nCntShadow].modelDataParent.dwNumMat = 0;				// �}�e���A���̐�
		g_aShadow[nCntShadow].modelDataParent.pTexture = NULL;			// �e�N�X�`���ւ̃|�C���^
		g_aShadow[nCntShadow].modelDataParent.pMesh    = NULL;			// ���b�V�� (���_���) �ւ̃|�C���^
		g_aShadow[nCntShadow].modelDataParent.pBuffMat = NULL;			// �}�e���A���ւ̃|�C���^
		g_aShadow[nCntShadow].modelDataParent.dwNumMat = 0;				// �}�e���A���̐�
		g_aShadow[nCntShadow].modelDataParent.vtxMin   = INIT_VTX_MIN;	// �ŏ��̒��_���W
		g_aShadow[nCntShadow].modelDataParent.vtxMax   = INIT_VTX_MAX;	// �ő�̒��_���W
		g_aShadow[nCntShadow].modelDataParent.size     = INIT_SIZE;		// �傫��
		g_aShadow[nCntShadow].modelDataParent.fRadius  = 0.0f;			// ���a
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
	D3DXMATRIX   mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX   mtxShadow;			// �V���h�E�}�g���b�N�X
	D3DMATERIAL9 matDef;			// �}�e���A���ۑ��p
	D3DLIGHT9    light;				// ���C�g�̏��
	D3DXVECTOR4  posLight;			// ���C�g�̈ʒu
	D3DXVECTOR3  pos, normal;		// ���ʏ�̔C�ӂ̈ʒu�A�@���x�N�g��
	D3DXPLANE    plane;				// ���ʏ��
	D3DXMATERIAL blackMat;			// �}�e���A���f�[�^ (��)

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	// ���C�g�̈ʒu��ݒ�
	pDevice->GetLight(0, &light);
	posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{ // �e�̍ő�\�������J��Ԃ�

		if (g_aShadow[nCntShadow].bUse == true)
		{ // �e���g�p����Ă���ꍇ

			switch (g_aShadow[nCntShadow].type)
			{ // ��ނ��Ƃ̏���
			case SHADOWTYPE_CIRCLE:	// �ۉe

				// ���u�����f�B���O�����Z�����ɐݒ�
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

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

				// ���u�����f�B���O�����ɖ߂�
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

				// �����𔲂���
				break;

			case SHADOWTYPE_MODEL:	// ���f���e

				// ���ʏ��𐶐�
				pos    = g_aShadow[nCntShadow].pos;
				normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				D3DXPlaneFromPointNormal(&plane, &pos, &normal);

				// �V���h�E�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&mtxShadow);

				// �V���h�E�}�g���b�N�X�̐���
				D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
				D3DXMatrixMultiply(&mtxShadow, &g_aShadow[nCntShadow].mtxParent, &mtxShadow);

				// �V���h�E�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

				// ���݂̃}�e���A�����擾
				pDevice->GetMaterial(&matDef);

				for (int nCntMat = 0; nCntMat < (int)g_aShadow[nCntShadow].modelDataParent.dwNumMat; nCntMat++)
				{ // �}�e���A���̐����J��Ԃ�

					// �\���̗̂v�f���N���A
					ZeroMemory(&blackMat, sizeof(D3DXMATERIAL));

					// �g�U���E�����E���Ȕ��������ɂ���
					blackMat.MatD3D.Diffuse  = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					blackMat.MatD3D.Ambient  = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					blackMat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&blackMat.MatD3D);	// ��

					// �e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, NULL);

					// ���f���̕`��
					g_aShadow[nCntShadow].modelDataParent.pMesh->DrawSubset(nCntMat);
				}

				// �ۑ����Ă����}�e���A����߂�
				pDevice->SetMaterial(&matDef);

				// �����𔲂���
				break;
			}
		}
	}

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�
}

//=======================================================================================================
//	�ۉe�̐ݒ菈��
//=======================================================================================================
int SetCircleShadow(float fAlpha, float fRadius, int *pShadowID, bool *pUse)
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

			// ��ނ��ۉe�ɐݒ�
			g_aShadow[nCntShadow].type = SHADOWTYPE_CIRCLE;

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
//	���f���e�̐ݒ菈��
//=======================================================================================================
int SetModelShadow(Model model, int *pShadowID, bool *pUse)
{
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{ // �e�̍ő�\�������J��Ԃ�

		if (g_aShadow[nCntShadow].bUse == false)
		{ // �e���g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aShadow[nCntShadow].modelDataParent = model;		// �e�̐e�̃��f�����

			// �����̃A�h���X����
			g_aShadow[nCntShadow].pShadowIDParent = pShadowID;	// �e�̐e�̉e�C���f�b�N�X
			g_aShadow[nCntShadow].pUseParent      = pUse;		// �e�̐e�̎g�p��

			// ��ނ����f���e�ɐݒ�
			g_aShadow[nCntShadow].type = SHADOWTYPE_MODEL;

			// �g�p���Ă����Ԃɂ���
			g_aShadow[nCntShadow].bUse = true;

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
void SetPositionShadow(int nShadowID, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	// �ϐ���錾
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	if (nShadowID > NONE_SHADOW)
	{ // �g�p�ł���ID�̏ꍇ

		// �����̉e�̈ʒu��ݒ�
		g_aShadow[nShadowID].pos = pos;

		switch (g_aShadow[nShadowID].type)
		{ // ��ނ��Ƃ̏���
		case SHADOWTYPE_CIRCLE:	// �ۉe

			// ����

			// �����𔲂���
			break;

		case SHADOWTYPE_MODEL:	// ���f���e

			// �e�̐e�̃��[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aShadow[nShadowID].mtxParent);

			// �g�嗦�𔽉f
			D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
			D3DXMatrixMultiply(&g_aShadow[nShadowID].mtxParent, &g_aShadow[nShadowID].mtxParent, &mtxScale);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
			D3DXMatrixMultiply(&g_aShadow[nShadowID].mtxParent, &g_aShadow[nShadowID].mtxParent, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&g_aShadow[nShadowID].mtxParent, &g_aShadow[nShadowID].mtxParent, &mtxTrans);

			// �e�̐e�̃��[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nShadowID].mtxParent);

			// �����𔲂���
			break;
		}
	}
}
