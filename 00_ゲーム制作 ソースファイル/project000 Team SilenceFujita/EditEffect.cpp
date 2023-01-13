//======================================================================================================================
//
//	�G�f�B�b�g�G�t�F�N�g���� [EditEffect.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "EditEffect.h"

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTextureEditEffect[] =		// �e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\effect000.jpg",		// �G�t�F�N�g�̃e�N�X�`���̑��΃p�X
};

//**********************************************************************************************************************
//	�e�N�X�`���̒�` (EditEffect)
//**********************************************************************************************************************
typedef enum
{
	EDITEFFECTTEX_NORMAL = 0,		//���ʂ̃G�t�F�N�g
	EDITEFFECTTEX_MAX
}EditEffectTex;

//**********************************************************************************************************************
//	�\���̒�` (EditEffect)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 move;				// �ړ���
	D3DXMATRIX  mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXCOLOR   col;				// �F
	int         nLife;				// ����
	D3DXVECTOR3 fRadius;			// ���a
	bool        bUse;				// �g�p��
}EditEffect;

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureEditEffect[EDITEFFECTTEX_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEditEffect = NULL;					// ���_�o�b�t�@�ւ̃|�C���^

EditEffect g_aEditEffect[MAX_EDITEFFECT];		// �G�t�F�N�g�̏��

//======================================================================================================================
//	�G�t�F�N�g�̏���������
//======================================================================================================================
void InitEditEffect(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_3D *pVtx;							// ���_���ւ̃|�C���^

	for (int nCntEditEffect = 0; nCntEditEffect < EDITEFFECTTEX_MAX; nCntEditEffect++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, apTextureEditEffect[nCntEditEffect], &g_apTextureEditEffect[nCntEditEffect]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_3D) * 4 * MAX_EDITEFFECT,	// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffEditEffect,
		NULL
	);

	// �G�t�F�N�g�̏��̏�����
	for (int nCntEditEffect = 0; nCntEditEffect < MAX_EDITEFFECT; nCntEditEffect++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		g_aEditEffect[nCntEditEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_aEditEffect[nCntEditEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
		g_aEditEffect[nCntEditEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �F
		g_aEditEffect[nCntEditEffect].nLife = 0;								// ����
		g_aEditEffect[nCntEditEffect].fRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���a
		g_aEditEffect[nCntEditEffect].bUse = false;								// �g�p��
	}

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEditEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEditEffect = 0; nCntEditEffect < MAX_EDITEFFECT; nCntEditEffect++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEditEffect->Unlock();
}

//======================================================================================================================
//	�G�t�F�N�g�̏I������
//======================================================================================================================
void UninitEditEffect(void)
{
	// �e�N�X�`���̔j��
	for (int nCntEditEffect = 0; nCntEditEffect < EDITEFFECTTEX_MAX; nCntEditEffect++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTextureEditEffect[nCntEditEffect] != NULL)
		{ // �ϐ� (g_apTextureEditEffect) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureEditEffect[nCntEditEffect]->Release();
			g_apTextureEditEffect[nCntEditEffect] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffEditEffect != NULL)
	{ // �ϐ� (g_pVtxBuffEditEffect) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffEditEffect->Release();
		g_pVtxBuffEditEffect = NULL;
	}
}

//======================================================================================================================
//	�G�t�F�N�g�̍X�V����
//======================================================================================================================
void UpdateEditEffect(void)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;							// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEditEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEditEffect = 0; nCntEditEffect < MAX_EDITEFFECT; nCntEditEffect++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aEditEffect[nCntEditEffect].bUse == true)
		{ // �G�t�F�N�g���g�p����Ă���ꍇ
			//�ړ��ʂ����Ɍ����Ă���
			g_aEditEffect[nCntEditEffect].move.y -= 1.0f;

			// �ړ��̍X�V
			g_aEditEffect[nCntEditEffect].pos += g_aEditEffect[nCntEditEffect].move;

			// �\���̗̂v�f�����Z����
			g_aEditEffect[nCntEditEffect].nLife--;									// ���������Z

			// ���a�̕␳
			if (g_aEditEffect[nCntEditEffect].fRadius.x < 0.0f)
			{ // ���a�� 0.0f ��菬�����ꍇ
				// ���a�� 0.0f ����
				g_aEditEffect[nCntEditEffect].fRadius.x = 0.0f;
			}

			// ���a�̕␳
			if (g_aEditEffect[nCntEditEffect].fRadius.y < 0.0f)
			{ // ���a�� 0.0f ��菬�����ꍇ
				// ���a�� 0.0f ����
				g_aEditEffect[nCntEditEffect].fRadius.y = 0.0f;
			}

			// �����x�̕␳
			if (g_aEditEffect[nCntEditEffect].col.a < 0.0f)
			{ // ���l�� 0.0f ��菬�����ꍇ
				// ���l�� 0.0f ����
				g_aEditEffect[nCntEditEffect].col.a = 0.0f;
			}

			if (g_aEditEffect[nCntEditEffect].nLife <= 0)
			{ // �������}�����ꍇ
				// �g�p���Ă��Ȃ���Ԃɂ���
				g_aEditEffect[nCntEditEffect].bUse = false;
			}

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aEditEffect[nCntEditEffect].fRadius.x, +g_aEditEffect[nCntEditEffect].fRadius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aEditEffect[nCntEditEffect].fRadius.x, +g_aEditEffect[nCntEditEffect].fRadius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEditEffect[nCntEditEffect].fRadius.x, -g_aEditEffect[nCntEditEffect].fRadius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aEditEffect[nCntEditEffect].fRadius.x, -g_aEditEffect[nCntEditEffect].fRadius.y, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aEditEffect[nCntEditEffect].col;
			pVtx[1].col = g_aEditEffect[nCntEditEffect].col;
			pVtx[2].col = g_aEditEffect[nCntEditEffect].col;
			pVtx[3].col = g_aEditEffect[nCntEditEffect].col;
		}

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEditEffect->Unlock();
}

//======================================================================================================================
//	�G�t�F�N�g�̕`�揈��
//======================================================================================================================
void DrawEditEffect(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxTrans;						// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;							// �r���[�}�g���b�N�X�擾�p

												// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

												// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

																// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntEditEffect = 0; nCntEditEffect < MAX_EDITEFFECT; nCntEditEffect++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�
		if (g_aEditEffect[nCntEditEffect].bUse == true)
		{ // �G�t�F�N�g���g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEditEffect[nCntEditEffect].mtxWorld);

			// �r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aEditEffect[nCntEditEffect].mtxWorld, NULL, &mtxView);	// �t�s������߂�
			g_aEditEffect[nCntEditEffect].mtxWorld._41 = 0.0f;
			g_aEditEffect[nCntEditEffect].mtxWorld._42 = 0.0f;
			g_aEditEffect[nCntEditEffect].mtxWorld._43 = 0.0f;

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEditEffect[nCntEditEffect].pos.x, g_aEditEffect[nCntEditEffect].pos.y, g_aEditEffect[nCntEditEffect].pos.z);
			D3DXMatrixMultiply(&g_aEditEffect[nCntEditEffect].mtxWorld, &g_aEditEffect[nCntEditEffect].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEditEffect[nCntEditEffect].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEditEffect, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureEditEffect[EDITEFFECTTEX_NORMAL]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEditEffect * 4, 2);
		}
	}

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

																// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//======================================================================================================================
//	�G�t�F�N�g�̐ݒ菈��
//======================================================================================================================
void SetEditEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, D3DXVECTOR3 fRadius)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

						// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEditEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEditEffect = 0; nCntEditEffect < MAX_EDITEFFECT; nCntEditEffect++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aEditEffect[nCntEditEffect].bUse == false)
		{ // �G�t�F�N�g���g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aEditEffect[nCntEditEffect].pos = pos;		// �ʒu
			g_aEditEffect[nCntEditEffect].move = move;		// �ړ���
			g_aEditEffect[nCntEditEffect].col = col;		// �F
			g_aEditEffect[nCntEditEffect].nLife = nLife;		// ����
			g_aEditEffect[nCntEditEffect].fRadius = fRadius;	// ���a

			// �g�p���Ă����Ԃɂ���
			g_aEditEffect[nCntEditEffect].bUse = true;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aEditEffect[nCntEditEffect].fRadius.x, +g_aEditEffect[nCntEditEffect].fRadius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aEditEffect[nCntEditEffect].fRadius.x, +g_aEditEffect[nCntEditEffect].fRadius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEditEffect[nCntEditEffect].fRadius.x, -g_aEditEffect[nCntEditEffect].fRadius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aEditEffect[nCntEditEffect].fRadius.x, -g_aEditEffect[nCntEditEffect].fRadius.y, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aEditEffect[nCntEditEffect].col;
			pVtx[1].col = g_aEditEffect[nCntEditEffect].col;
			pVtx[2].col = g_aEditEffect[nCntEditEffect].col;
			pVtx[3].col = g_aEditEffect[nCntEditEffect].col;

			// �����𔲂���
			break;
		}

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEditEffect->Unlock();
}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================

#endif