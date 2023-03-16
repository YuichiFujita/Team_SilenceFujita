//========================================
//
//�J�߂̃��C������[Praise.cpp]
//Author ��������
//
//========================================
#include "main.h"
#include "praise.h"

#include "tutorial.h"

//===========================================
//�}�N����`
//===========================================
#define MAX_PRAISE			(30)									// �J�߂̍ő吔

#define PRAISE_INIT_POS		(D3DXVECTOR3(	0.0f,	0.0f, 0.0f))	// �J�߂̏����ʒu
#define PRAISE_POS			(D3DXVECTOR3(1100.0f, 500.0f, 0.0f))	// �J�߂̈ʒu
#define PRAISE_SIZE			(D3DXVECTOR3( 162.0f,  30.0f, 0.0f))	// �J�߂̃T�C�Y
#define PRAISE_LAPSE_CNT	(90)									// �J�߂�\�����Ă����J�E���g

//===========================================
//�񋓌^��`(PRAISE_TEXTURE)
//===========================================
typedef enum
{
	PRAISE_TEXTURE_NICE = 0,			// �i�C�X�R���{�e�N�X�`��
	PRAISE_TEXTURE_GREAT,				// �O���[�g�R���{�e�N�X�`��
	PRAISE_TEXTURE_GOOD,				// �O�b�h�R���{�e�N�X�`��
	PRAISE_TEXTURE_MAX					// ���̗񋓌^�̑���
}PRAISE_TEXTURE;

//===========================================
//�v���g�^�C�v�錾
//===========================================

//===========================================
//�O���[�o���ϐ��錾
//===========================================
LPDIRECT3DTEXTURE9      g_apTexturePraise[PRAISE_TEXTURE_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPraise = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

Praise g_aPraise[MAX_PRAISE];						// �J�߂̏��

// �R���X�g��`
const char *c_apPraiseTextureName[PRAISE_TEXTURE_MAX] = 
{
	"data/TEXTURE/praise000.png",		// �i�C�X�R���{�e�N�X�`��
	"data/TEXTURE/praise001.png",		// �O���[�g�R���{�e�N�X�`��
	"data/TEXTURE/praise002.png",		// �O�b�h�R���{�e�N�X�`��
};

//===========================================
//�J�߂̏���������
//===========================================
void InitPraise(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;							// ���_���ւ̃|�C���^

	for (int nCntPra = 0; nCntPra < MAX_PRAISE; nCntPra++)
	{
		// ���̏�����
		g_aPraise[nCntPra].pos	= PRAISE_INIT_POS;					// �ʒu
		g_aPraise[nCntPra].rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aPraise[nCntPra].nCount = 0;								// ���ŃJ�E���g
		g_aPraise[nCntPra].nNumTex = 0;								// 
		g_aPraise[nCntPra].fAngle = atan2f((PRAISE_SIZE.x * 2), (PRAISE_SIZE.y * 2));		// ����
		g_aPraise[nCntPra].fLength = sqrtf((PRAISE_SIZE.x * 2) * (PRAISE_SIZE.x * 2) + (PRAISE_SIZE.y * 2) * (PRAISE_SIZE.y * 2)) * 0.5f;		// ����
		g_aPraise[nCntPra].bUse = false;							// �g�p��
	}

	for (int nCntTex = 0; nCntTex < PRAISE_TEXTURE_MAX; nCntTex++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, c_apPraiseTextureName[nCntTex], &g_apTexturePraise[nCntTex]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_2D) * 4 * MAX_PRAISE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPraise,
		NULL
	);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPraise->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPra = 0; nCntPra < MAX_PRAISE; nCntPra++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + (D3DX_PI + g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
		pVtx[0].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + (D3DX_PI + g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + (D3DX_PI + -g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
		pVtx[1].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + (D3DX_PI + -g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + -g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
		pVtx[2].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + -g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
		pVtx[3].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
		pVtx[3].pos.z = 0.0f;

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;			// ���_�f�[�^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPraise->Unlock();
}

//===========================================
//�J�߂̏I������
//===========================================
void UninitPraise(void)
{
	for (int nCntPra = 0; nCntPra < PRAISE_TEXTURE_MAX; nCntPra++)
	{
		//�e�N�X�`���̔j��
		if (g_apTexturePraise[nCntPra] != NULL)
		{
			g_apTexturePraise[nCntPra]->Release();
			g_apTexturePraise[nCntPra] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPraise != NULL)
	{
		g_pVtxBuffPraise->Release();
		g_pVtxBuffPraise = NULL;
	}
}

//===========================================
//�J�߂̍X�V����
//===========================================
void UpdatePraise(void)
{
	VERTEX_2D * pVtx;											//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPraise->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPra = 0; nCntPra < MAX_PRAISE; nCntPra++)
	{
		if (g_aPraise[nCntPra].bUse == true)
		{//�g�p���Ă����ꍇ

			// ���ŃJ�E���^�[�����Z����
			g_aPraise[nCntPra].nCount++;

			if (g_aPraise[nCntPra].nCount % PRAISE_LAPSE_CNT == 0)
			{ // �J�E���^�[����萔�ȏ�ɂȂ�����

				// �J�E���^�[������������
				g_aPraise[nCntPra].nCount = 0;

				// �g�p���Ȃ�
				g_aPraise[nCntPra].bUse = false;
			}

			// ���_���W�̐ݒ�
			pVtx[0].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + (D3DX_PI + g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
			pVtx[0].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + (D3DX_PI + g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + (D3DX_PI + -g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
			pVtx[1].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + (D3DX_PI + -g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + -g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
			pVtx[2].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + -g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
			pVtx[3].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
			pVtx[3].pos.z = 0.0f;
		}

		pVtx += 4;		// ���_�f�[�^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPraise->Unlock();
}

//===========================================
//�J�߂̕`�揈��
//===========================================
void DrawPraise(void)
{
	// �ϐ���錾
	D3DXVECTOR3 pos;	// �����Ƃ̔{���ʒu�̐ݒ�p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffPraise,									//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D));									//���_���\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPra = 0; nCntPra < MAX_PRAISE; nCntPra++)
	{
		if (g_aPraise[nCntPra].bUse == true)
		{//�g�p����Ă���ꍇ

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTexturePraise[g_aPraise[nCntPra].nNumTex]);

			// �|���S���̕`��
			pDevice->DrawPrimitive
			( // ����
				D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
				nCntPra * 4,			// �`�悷��ŏ��̒��_�C���f�b�N�X
				2						// �`�悷��v���~�e�B�u��
			);
		}
	}
}

//===========================================
// �J�ߌ��t�̐ݒ菈��
//===========================================
void SetPraise(void)
{
	VERTEX_2D * pVtx;											//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPraise->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPra = 0; nCntPra < MAX_PRAISE; nCntPra++)
	{
		if (g_aPraise[nCntPra].bUse == false)
		{//�g�p���Ă��Ȃ������ꍇ

			// ���̐ݒ�
			g_aPraise[nCntPra].pos = PRAISE_POS;		// �ʒu
			g_aPraise[nCntPra].rot = D3DXVECTOR3		// ����
			(
				0.0f,
				0.0f,
				D3DXToRadian((rand() % 35 - 17))
			);
			g_aPraise[nCntPra].nNumTex = rand() % PRAISE_TEXTURE_MAX;	// �e�N�X�`���̔ԍ�
			g_aPraise[nCntPra].nCount = 0;								// ���ŃJ�E���g
			g_aPraise[nCntPra].fAngle = atan2f((PRAISE_SIZE.x * 2), (PRAISE_SIZE.y * 2));		// ����
			g_aPraise[nCntPra].fLength = sqrtf((PRAISE_SIZE.x * 2) * (PRAISE_SIZE.x * 2) + (PRAISE_SIZE.y * 2) * (PRAISE_SIZE.y * 2)) * 0.5f;		// ����

			// ���_���W�̐ݒ�
			pVtx[0].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + (D3DX_PI + g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
			pVtx[0].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + (D3DX_PI + g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + (D3DX_PI + -g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
			pVtx[1].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + (D3DX_PI + -g_aPraise[nCntPra].fAngle)) * g_aPraise[nCntPra].fLength;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + -g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
			pVtx[2].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + -g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_aPraise[nCntPra].pos.x + sinf(g_aPraise[nCntPra].rot.z + g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
			pVtx[3].pos.y = g_aPraise[nCntPra].pos.y + cosf(g_aPraise[nCntPra].rot.z + g_aPraise[nCntPra].fAngle) * g_aPraise[nCntPra].fLength;
			pVtx[3].pos.z = 0.0f;

			g_aPraise[nCntPra].bUse = true;							// �g�p����

			break;			// �����o��
		}

		pVtx += 4;		// ���_�f�[�^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPraise->Unlock();
}