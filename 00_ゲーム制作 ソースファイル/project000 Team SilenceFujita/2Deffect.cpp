//======================================================================================================================
//
//	2D2D�G�t�F�N�g���� [2Deffect.cpp]
//	Author�F��������
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "2Deffect.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAX_EFFECT		(4096)		// �g�p����|���S���� (2D�G�t�F�N�g�̍ő吔)

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTexture2DEffect[] =		// �e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\effect000.jpg",	// 2D�G�t�F�N�g�̃e�N�X�`���̑��΃p�X
};

//**********************************************************************************************************************
//	�񋓌^��` (TEXTURE_EFFECT)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_2DEFFECT_NORMAL = 0,		// 2D�G�t�F�N�g (�ʏ�)
	TEXTURE_2DEFFECT_MAX,				// ���̗񋓌^�̑���
} TEXTURE_2DEFFECT;

//**********************************************************************************************************************
//	�\���̒�` (2DEffect)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 move;				// �ړ���
	D3DXCOLOR   col;				// �F
	int         nLife;				// ����
	float       fRadius;			// ���a
	float       fSub;				// ���Z�� (���a)
	float       fAlpha;				// 1F���Ƃ̔����Ȃ��
	bool        bUse;				// �g�p��
}Effect2D;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTexture2DEffect[TEXTURE_2DEFFECT_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuff2DEffect = NULL;						// ���_�o�b�t�@�ւ̃|�C���^

Effect2D g_a2DEffect[MAX_EFFECT];		// 2D�G�t�F�N�g�̏��

//======================================================================================================================
//	2D�G�t�F�N�g�̏���������
//======================================================================================================================
void Init2DEffect(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;							// ���_���ւ̃|�C���^

	for (int nCnt2DEffect = 0; nCnt2DEffect < TEXTURE_2DEFFECT_MAX; nCnt2DEffect++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, apTexture2DEffect[nCnt2DEffect], &g_apTexture2DEffect[nCnt2DEffect]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_2D) * 4 * MAX_EFFECT,		// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuff2DEffect,
		NULL
	);

	// 2D�G�t�F�N�g�̏��̏�����
	for (int nCnt2DEffect = 0; nCnt2DEffect < MAX_EFFECT; nCnt2DEffect++)
	{ // 2D�G�t�F�N�g�̍ő�\�������J��Ԃ�

		g_a2DEffect[nCnt2DEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_a2DEffect[nCnt2DEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
		g_a2DEffect[nCnt2DEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �F
		g_a2DEffect[nCnt2DEffect].nLife = 0;								// ����
		g_a2DEffect[nCnt2DEffect].fRadius = 0.0f;							// ���a
		g_a2DEffect[nCnt2DEffect].fSub = 0.0f;								// ���Z�� (���a)
		g_a2DEffect[nCnt2DEffect].fAlpha = 0.0f;							// 1F���Ƃ̔����Ȃ��
		g_a2DEffect[nCnt2DEffect].bUse = false;								// �g�p��
	}

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuff2DEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt2DEffect = 0; nCnt2DEffect < MAX_EFFECT; nCnt2DEffect++)
	{ // 2D�G�t�F�N�g�̍ő�\�������J��Ԃ�

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// rhw �̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

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
	g_pVtxBuff2DEffect->Unlock();
}

//======================================================================================================================
//	2D�G�t�F�N�g�̏I������
//======================================================================================================================
void Uninit2DEffect(void)
{
	// �e�N�X�`���̔j��
	for (int nCnt2DEffect = 0; nCnt2DEffect < TEXTURE_2DEFFECT_MAX; nCnt2DEffect++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTexture2DEffect[nCnt2DEffect] != NULL)
		{ // �ϐ� (g_apTexture2DEffect) ��NULL�ł͂Ȃ��ꍇ

			g_apTexture2DEffect[nCnt2DEffect]->Release();
			g_apTexture2DEffect[nCnt2DEffect] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuff2DEffect != NULL)
	{ // �ϐ� (g_pVtxBuff2DEffect) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuff2DEffect->Release();
		g_pVtxBuff2DEffect = NULL;
	}
}

//======================================================================================================================
//	2D�G�t�F�N�g�̍X�V����
//======================================================================================================================
void Update2DEffect(void)
{
	// �|�C���^��錾
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuff2DEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt2DEffect = 0; nCnt2DEffect < MAX_EFFECT; nCnt2DEffect++)
	{ // 2D�G�t�F�N�g�̍ő�\�������J��Ԃ�

		if (g_a2DEffect[nCnt2DEffect].bUse == true)
		{ // 2D�G�t�F�N�g���g�p����Ă���ꍇ

			// �ړ��̍X�V
			g_a2DEffect[nCnt2DEffect].pos += g_a2DEffect[nCnt2DEffect].move;

			// �\���̗̂v�f�����Z����
			g_a2DEffect[nCnt2DEffect].nLife--;									// ���������Z
			g_a2DEffect[nCnt2DEffect].fRadius -= g_a2DEffect[nCnt2DEffect].fSub;	// ���a�����Z
			g_a2DEffect[nCnt2DEffect].col.a -= g_a2DEffect[nCnt2DEffect].fAlpha;	// ���l�����Z

																			// ���a�̕␳
			if (g_a2DEffect[nCnt2DEffect].fRadius < 0.0f)
			{ // ���a�� 0.0f ��菬�����ꍇ

			  // ���a�� 0.0f ����
				g_a2DEffect[nCnt2DEffect].fRadius = 0.0f;
			}

			// �����x�̕␳
			if (g_a2DEffect[nCnt2DEffect].col.a < 0.0f)
			{ // ���l�� 0.0f ��菬�����ꍇ

			  // ���l�� 0.0f ����
				g_a2DEffect[nCnt2DEffect].col.a = 0.0f;
			}

			if (g_a2DEffect[nCnt2DEffect].nLife <= 0)
			{ // �������}�����ꍇ

			  // �g�p���Ă��Ȃ���Ԃɂ���
				g_a2DEffect[nCnt2DEffect].bUse = false;
			}

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_a2DEffect[nCnt2DEffect].pos.x - g_a2DEffect[nCnt2DEffect].fRadius, g_a2DEffect[nCnt2DEffect].pos.y - g_a2DEffect[nCnt2DEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_a2DEffect[nCnt2DEffect].pos.x + g_a2DEffect[nCnt2DEffect].fRadius, g_a2DEffect[nCnt2DEffect].pos.y - g_a2DEffect[nCnt2DEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_a2DEffect[nCnt2DEffect].pos.x - g_a2DEffect[nCnt2DEffect].fRadius, g_a2DEffect[nCnt2DEffect].pos.y + g_a2DEffect[nCnt2DEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_a2DEffect[nCnt2DEffect].pos.x + g_a2DEffect[nCnt2DEffect].fRadius, g_a2DEffect[nCnt2DEffect].pos.y + g_a2DEffect[nCnt2DEffect].fRadius, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_a2DEffect[nCnt2DEffect].col;
			pVtx[1].col = g_a2DEffect[nCnt2DEffect].col;
			pVtx[2].col = g_a2DEffect[nCnt2DEffect].col;
			pVtx[3].col = g_a2DEffect[nCnt2DEffect].col;
		}

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuff2DEffect->Unlock();
}

//======================================================================================================================
//	2D�G�t�F�N�g�̕`�揈��
//======================================================================================================================
void Draw2DEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuff2DEffect,										//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D));										//���_���\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_a2DEffect[nCntEffect].bUse == true)
		{//�G�t�F�N�g���g�p����Ă���

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTexture2DEffect[TEXTURE_2DEFFECT_NORMAL]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
				4 * nCntEffect,									//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);												//�`�悷��v���~�e�B�u��
		}
	}

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//======================================================================================================================
//	2D�G�t�F�N�g�̐ݒ菈��
//======================================================================================================================
void Set2DEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fRadius, float fSub)
{
	// �|�C���^��錾
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuff2DEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt2DEffect = 0; nCnt2DEffect < MAX_EFFECT; nCnt2DEffect++)
	{ // 2D�G�t�F�N�g�̍ő�\�������J��Ԃ�

		if (g_a2DEffect[nCnt2DEffect].bUse == false)
		{ // 2D�G�t�F�N�g���g�p����Ă��Ȃ��ꍇ

			// ��������
			g_a2DEffect[nCnt2DEffect].pos = pos;			// �ʒu
			g_a2DEffect[nCnt2DEffect].move = move;			// �ړ���
			g_a2DEffect[nCnt2DEffect].col = col;			// �F
			g_a2DEffect[nCnt2DEffect].nLife = nLife;		// ����
			g_a2DEffect[nCnt2DEffect].fRadius = fRadius;	// ���a
			g_a2DEffect[nCnt2DEffect].fSub = fSub;			// ���Z�� (���a)

			// 1F ���Ƃ̔����Ȃ�ʂ�ݒ�
			g_a2DEffect[nCnt2DEffect].fAlpha = 1.0f / nLife;

			// �g�p���Ă����Ԃɂ���
			g_a2DEffect[nCnt2DEffect].bUse = true;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_a2DEffect[nCnt2DEffect].pos.x - g_a2DEffect[nCnt2DEffect].fRadius, g_a2DEffect[nCnt2DEffect].pos.y - g_a2DEffect[nCnt2DEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_a2DEffect[nCnt2DEffect].pos.x + g_a2DEffect[nCnt2DEffect].fRadius, g_a2DEffect[nCnt2DEffect].pos.y - g_a2DEffect[nCnt2DEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_a2DEffect[nCnt2DEffect].pos.x - g_a2DEffect[nCnt2DEffect].fRadius, g_a2DEffect[nCnt2DEffect].pos.y + g_a2DEffect[nCnt2DEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_a2DEffect[nCnt2DEffect].pos.x + g_a2DEffect[nCnt2DEffect].fRadius, g_a2DEffect[nCnt2DEffect].pos.y + g_a2DEffect[nCnt2DEffect].fRadius, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_a2DEffect[nCnt2DEffect].col;
			pVtx[1].col = g_a2DEffect[nCnt2DEffect].col;
			pVtx[2].col = g_a2DEffect[nCnt2DEffect].col;
			pVtx[3].col = g_a2DEffect[nCnt2DEffect].col;

			// �����𔲂���
			break;
		}

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuff2DEffect->Unlock();
}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================
//**********************************************************************************************************************
//	2D�G�t�F�N�g�̑����擾����
//**********************************************************************************************************************
int GetNum2DEffect(void)
{
	// �ϐ���錾
	int nNum2DEffect = 0;	// 2D�G�t�F�N�g�̑����̊m�F�p

	for (int nCnt2DEffect = 0; nCnt2DEffect < MAX_EFFECT; nCnt2DEffect++)
	{ // 2D�G�t�F�N�g�̍ő�\�������J��Ԃ�

		if (g_a2DEffect[nCnt2DEffect].bUse == true)
		{ // 2D�G�t�F�N�g���g�p����Ă���ꍇ

		  // �J�E���^�[�����Z
			nNum2DEffect++;
		}
	}

	// �ϐ��̒l��Ԃ�
	return nNum2DEffect;	// 2D�G�t�F�N�g�̑���
}
#endif