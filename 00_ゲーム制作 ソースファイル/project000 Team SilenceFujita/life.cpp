//======================================================================================================================
//
//	�̗̓o�[���� [life.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "life.h"
#include "player.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAX_LIFE		(5)			// �g�p����|���S����

#define LIFE_POS_X		(30.0f)		// �̗̓o�[�̐�΍��W (x)
#define LIFE_POS_Y		(61.0f)		// �̗̓o�[�̐�΍��W (y)
#define LIFE_WIDTH_MUL	(3.58f)		// �̗̓o�[�̉����̃v���C���[�̗͏�Z��
#define LIFE_HEIGHT		(40.0f)		// �̗̓o�[�̏c�� / 2

#define LIFE_BG_POS_X	(230.0f)	// �̗̓o�[�̔w�i�̐�΍��W (x)
#define LIFE_BG_POS_Y	(60.0f)		// �̗̓o�[�̔w�i�̐�΍��W (y)
#define LIFE_BG_WIDTH	(225.0f)	// �̗̓o�[�̔w�i�̉��� / 2
#define LIFE_BG_HEIGHT	(55.0f)		// �̗̓o�[�̔w�i�̏c�� / 2

#define CNT_FRAME		((int)(DAMAGE_TIME_PLAY * 0.5f))	// �̗͂����炵����t���[����

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTextureLife[] =		// �e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\alpha000.png",	// ���C�t���ߌ^�̃e�N�X�`���̑��΃p�X
	NULL,							// NULL
	NULL,							// NULL
	"data\\TEXTURE\\ui000.tga",		// ���C�t�w�i�̃e�N�X�`���̑��΃p�X
	"data\\TEXTURE\\frame000.tga",	// ���C�t�^�̃e�N�X�`���̑��΃p�X
};

//**********************************************************************************************************************
//	�񋓌^��` (TEXTURE_LIFE)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_LIFE_ALPHA = 0,	// ���C�t�̓��ߌ^
	TEXTURE_LIFE_NULL_00,	// NULL
	TEXTURE_LIFE_NULL_01,	// NULL
	TEXTURE_LIFE_BG,		// ���C�t�̔w�i
	TEXTURE_LIFE_FRAME,		// ���C�t�̌^
	TEXTURE_LIFE_MAX,		// ���̗񋓌^�̑���
} TEXTURE_LIFE;

//**********************************************************************************************************************
//	�\���̂��` (Life)
//**********************************************************************************************************************
typedef struct
{
	LIFESTATE state;			// ���
	float     fChange;			// 1�t���[���̗͕̑ϓ���
	float     fNowLife;			// ���݂̗̑�
	int       nCounterState;	// ��ԊǗ��J�E���^�[
} Life;

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureLife[TEXTURE_LIFE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;					// ���_�o�b�t�@�ւ̃|�C���^

Life g_life;	// �̗͂̏��

//======================================================================================================================
//	�̗̓o�[�̏���������
//======================================================================================================================
void InitLife(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;							// ���_���ւ̃|�C���^

	for (int nCntLife = 0; nCntLife < TEXTURE_LIFE_MAX; nCntLife++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, apTextureLife[nCntLife], &g_apTextureLife[nCntLife]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_2D) * 4 * MAX_LIFE,		// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife,
		NULL
	);

	// �̗͂̏��̏�����
	g_life.state         = LIFESTATE_NORMAL;	// ���
	g_life.fChange       = 0.0f;				// 1�t���[���̗͕̑ϓ���
	g_life.fNowLife      = (float)PLAY_LIFE;	// ���݂̗̑�
	g_life.nCounterState = 0;					// ��ԊǗ��J�E���^�[

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------------------------------------------------------------------------------------------
	//	�̗̓o�[ (���ߌ^)
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[0].pos = D3DXVECTOR3(LIFE_BG_POS_X - LIFE_BG_WIDTH, LIFE_BG_POS_Y - LIFE_BG_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(LIFE_BG_POS_X + LIFE_BG_WIDTH, LIFE_BG_POS_Y - LIFE_BG_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(LIFE_BG_POS_X - LIFE_BG_WIDTH, LIFE_BG_POS_Y + LIFE_BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(LIFE_BG_POS_X + LIFE_BG_WIDTH, LIFE_BG_POS_Y + LIFE_BG_HEIGHT, 0.0f);

	// rhw �̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//------------------------------------------------------------------------------------------------------------------
	//	�̗̓o�[ (��)
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[4].pos = D3DXVECTOR3(LIFE_POS_X,                                       LIFE_POS_Y - LIFE_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(LIFE_POS_X + (float)(PLAY_LIFE * LIFE_WIDTH_MUL), LIFE_POS_Y - LIFE_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(LIFE_POS_X,                                       LIFE_POS_Y + LIFE_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(LIFE_POS_X + (float)(PLAY_LIFE * LIFE_WIDTH_MUL), LIFE_POS_Y + LIFE_HEIGHT, 0.0f);

	// rhw �̐ݒ�
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[4].col = D3DCOLOR_RGBA(140, 50, 50, 255);
	pVtx[5].col = D3DCOLOR_RGBA(140, 50, 50, 255);
	pVtx[6].col = D3DCOLOR_RGBA(90,  0,  0,  255);
	pVtx[7].col = D3DCOLOR_RGBA(90,  0,  0,  255);

	//------------------------------------------------------------------------------------------------------------------
	//	�̗̓o�[ (��)
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[8].pos  = D3DXVECTOR3(LIFE_POS_X,                                       LIFE_POS_Y - LIFE_HEIGHT, 0.0f);
	pVtx[9].pos  = D3DXVECTOR3(LIFE_POS_X + (float)(PLAY_LIFE * LIFE_WIDTH_MUL), LIFE_POS_Y - LIFE_HEIGHT, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(LIFE_POS_X,                                       LIFE_POS_Y + LIFE_HEIGHT, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(LIFE_POS_X + (float)(PLAY_LIFE * LIFE_WIDTH_MUL), LIFE_POS_Y + LIFE_HEIGHT, 0.0f);

	// rhw �̐ݒ�
	pVtx[8].rhw  = 1.0f;
	pVtx[9].rhw  = 1.0f;
	pVtx[10].rhw = 1.0f;
	pVtx[11].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[8].col  = D3DCOLOR_RGBA(255, 250, 50, 255);
	pVtx[9].col  = D3DCOLOR_RGBA(255, 250, 50, 255);
	pVtx[10].col = D3DCOLOR_RGBA(205, 160,  0, 255);
	pVtx[11].col = D3DCOLOR_RGBA(205, 160,  0, 255);

	//------------------------------------------------------------------------------------------------------------------
	//	�̗̓o�[ (�w�i)
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[12].pos = D3DXVECTOR3(LIFE_BG_POS_X - LIFE_BG_WIDTH, LIFE_BG_POS_Y - LIFE_BG_HEIGHT, 0.0f);
	pVtx[13].pos = D3DXVECTOR3(LIFE_BG_POS_X + LIFE_BG_WIDTH, LIFE_BG_POS_Y - LIFE_BG_HEIGHT, 0.0f);
	pVtx[14].pos = D3DXVECTOR3(LIFE_BG_POS_X - LIFE_BG_WIDTH, LIFE_BG_POS_Y + LIFE_BG_HEIGHT, 0.0f);
	pVtx[15].pos = D3DXVECTOR3(LIFE_BG_POS_X + LIFE_BG_WIDTH, LIFE_BG_POS_Y + LIFE_BG_HEIGHT, 0.0f);

	// rhw �̐ݒ�
	pVtx[12].rhw = 1.0f;
	pVtx[13].rhw = 1.0f;
	pVtx[14].rhw = 1.0f;
	pVtx[15].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[12].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[13].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[14].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[15].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[12].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[13].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[14].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[15].tex = D3DXVECTOR2(1.0f, 1.0f);

	//------------------------------------------------------------------------------------------------------------------
	//	�̗̓o�[ (�^)
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[16].pos = D3DXVECTOR3(LIFE_BG_POS_X - LIFE_BG_WIDTH, LIFE_BG_POS_Y - LIFE_BG_HEIGHT, 0.0f);
	pVtx[17].pos = D3DXVECTOR3(LIFE_BG_POS_X + LIFE_BG_WIDTH, LIFE_BG_POS_Y - LIFE_BG_HEIGHT, 0.0f);
	pVtx[18].pos = D3DXVECTOR3(LIFE_BG_POS_X - LIFE_BG_WIDTH, LIFE_BG_POS_Y + LIFE_BG_HEIGHT, 0.0f);
	pVtx[19].pos = D3DXVECTOR3(LIFE_BG_POS_X + LIFE_BG_WIDTH, LIFE_BG_POS_Y + LIFE_BG_HEIGHT, 0.0f);

	// rhw �̐ݒ�
	pVtx[16].rhw = 1.0f;
	pVtx[17].rhw = 1.0f;
	pVtx[18].rhw = 1.0f;
	pVtx[19].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[16].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[17].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[18].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[19].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[16].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[17].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[18].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[19].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLife->Unlock();
}

//======================================================================================================================
//	�̗̓o�[�̏I������
//======================================================================================================================
void UninitLife(void)
{
	// �e�N�X�`���̔j��
	for (int nCntLife = 0; nCntLife < TEXTURE_LIFE_MAX; nCntLife++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTextureLife[nCntLife] != NULL)
		{ // �ϐ� (g_apTextureLife) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureLife[nCntLife]->Release();
			g_apTextureLife[nCntLife] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffLife != NULL)
	{ // �ϐ� (g_pVtxBuffLife) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}

//======================================================================================================================
//	�̗̓o�[�̍X�V����
//======================================================================================================================
void UpdateLife(void)
{
	// �|�C���^��錾
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	if (g_life.state == LIFESTATE_CHANGE)
	{ // �̗͕ϓ���Ԃ̏ꍇ

		// �J�E���^�[�����Z
		g_life.nCounterState--;

		// ���݂̗̑̓o�[����󂯂��_���[�W�������Z
		g_life.fNowLife += g_life.fChange;

		// ���_���W��ݒ�
		pVtx[8].pos  = D3DXVECTOR3(LIFE_POS_X,                                      LIFE_POS_Y - LIFE_HEIGHT, 0.0f);
		pVtx[9].pos  = D3DXVECTOR3(LIFE_POS_X + (g_life.fNowLife * LIFE_WIDTH_MUL), LIFE_POS_Y - LIFE_HEIGHT, 0.0f);
		pVtx[10].pos = D3DXVECTOR3(LIFE_POS_X,                                      LIFE_POS_Y + LIFE_HEIGHT, 0.0f);
		pVtx[11].pos = D3DXVECTOR3(LIFE_POS_X + (g_life.fNowLife * LIFE_WIDTH_MUL), LIFE_POS_Y + LIFE_HEIGHT, 0.0f);

		if (g_life.nCounterState <= 0)
		{ // �J�E���^�[�� 0�ȉ��ɂȂ����ꍇ

			// �ʏ��Ԃɂ���
			g_life.state = LIFESTATE_NORMAL;

			// ���_���W���Đݒ�
			pVtx[8].pos  = D3DXVECTOR3(LIFE_POS_X,                                           LIFE_POS_Y - LIFE_HEIGHT, 0.0f);
			pVtx[9].pos  = D3DXVECTOR3(LIFE_POS_X + (int)(g_life.fNowLife * LIFE_WIDTH_MUL), LIFE_POS_Y - LIFE_HEIGHT, 0.0f);
			pVtx[10].pos = D3DXVECTOR3(LIFE_POS_X,                                           LIFE_POS_Y + LIFE_HEIGHT, 0.0f);
			pVtx[11].pos = D3DXVECTOR3(LIFE_POS_X + (int)(g_life.fNowLife * LIFE_WIDTH_MUL), LIFE_POS_Y + LIFE_HEIGHT, 0.0f);
		}

		// �̗̓Q�[�W�̕␳
		if (pVtx[9].pos.x < LIFE_POS_X)
		{ // �Q�[�W�����肷�����ꍇ

			// �̗̓Q�[�W�� 0�ɕ␳
			pVtx[9].pos  = D3DXVECTOR3(LIFE_POS_X, LIFE_POS_Y - LIFE_HEIGHT, 0.0f);
			pVtx[11].pos = D3DXVECTOR3(LIFE_POS_X, LIFE_POS_Y + LIFE_HEIGHT, 0.0f);
		}
		else if (pVtx[9].pos.x > LIFE_POS_X + (float)(PLAY_LIFE * LIFE_WIDTH_MUL))
		{ // �Q�[�W�������������ꍇ

			// �̗̓Q�[�W�� 100�ɕ␳
			pVtx[9].pos  = D3DXVECTOR3(LIFE_POS_X + (float)(PLAY_LIFE * LIFE_WIDTH_MUL), LIFE_POS_Y - LIFE_HEIGHT, 0.0f);
			pVtx[11].pos = D3DXVECTOR3(LIFE_POS_X + (float)(PLAY_LIFE * LIFE_WIDTH_MUL), LIFE_POS_Y + LIFE_HEIGHT, 0.0f);
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLife->Unlock();
}

//======================================================================================================================
//	�̗̓o�[�̕`�揈��
//======================================================================================================================
void DrawLife(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{ // �g�p����|���S�������J��Ԃ�

		switch (nCntLife)
		{ // �`�悷��|���S�����Ƃ̏���
		case TEXTURE_LIFE_ALPHA:

			// ���u�����f�B���O�����Z�����ɐݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			// ���e�X�g��L���ɂ���
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);	// ���e�X�g�̗L�� / �����̐ݒ�
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_LESS);	// ���e�X�g�̐ݒ�
			pDevice->SetRenderState(D3DRS_ALPHAREF, 255);		 	// ���e�X�g�̎Q�ƒl�ݒ�

			// �����𔲂���
			break;

		case TEXTURE_LIFE_NULL_00:

			// Z�e�X�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);		// Z�e�X�g�̐ݒ�
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

			// �����𔲂���
			break;

		case TEXTURE_LIFE_NULL_01:

			// Z�e�X�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);		// Z�e�X�g�̐ݒ�
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

			// �����𔲂���
			break;

		case TEXTURE_LIFE_BG:

			// Z�e�X�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL);		// Z�e�X�g�̐ݒ�
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

			// �����𔲂���
			break;

		case TEXTURE_LIFE_FRAME:

			// ����

			// �����𔲂���
			break;
		}

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureLife[nCntLife]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLife * 4, 2);

		// ���u�����f�B���O�����ɖ߂�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// ���e�X�g�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g�̗L�� / �����̐ݒ�
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// ���e�X�g�̐ݒ�
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// ���e�X�g�̎Q�ƒl�ݒ�

		// Z�e�X�g��L���ɂ���
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Z�e�X�g�̐ݒ�
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�
	}
}

//======================================================================================================================
//	�̗̓o�[�̐ݒ菈��
//======================================================================================================================
void SetLife(int nLife, int nDamage)
{
	// ��������
	g_life.fNowLife = (float)nLife;		// ���݂̗̑�

	// 1�t���[���̗͕̑ϓ��ʂ����߂�
	g_life.fChange = ((float)nDamage / (float)CNT_FRAME);

	// ��Ԃ�ݒ�
	g_life.state = LIFESTATE_CHANGE;	// ���l�ϓ����

	// �J�E���^�[��ݒ�
	g_life.nCounterState = CNT_FRAME;	// ��ԊǗ��J�E���^�[
}