//======================================================================================================================
//
//	�\�̓o�[���� [ability.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "ability.h"
#include "player.h"
#include "wind.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAX_ABILITY			(3)			// �\�͕\���Ɏg�p����|���S����

#define ABI_BG_POS_X		(245.0f)	// �̗̓o�[�̔w�i�̐�΍��W (x)
#define ABI_BG_POS_Y		(145.0f)	// �̗̓o�[�̔w�i�̐�΍��W (y)
#define ABI_BG_WIDTH		(240.0f)	// �̗̓o�[�̔w�i�̉��� / 2
#define ABI_BG_HEIGHT		(46.0f)		// �̗̓o�[�̔w�i�̏c�� / 2

#define ABI_INIT_BG			(0)			// �w�i���̏����l
#define ABI_LOOP_BG			(2)			// �w�i���̌J��Ԃ���
#define ABI_INIT_FRAME		(2)			// �g���̏����l
#define ABI_LOOP_FRAME		(3)			// �g���̌J��Ԃ���

//----------------------------------------------------------------------------------------------------------------------
//	�j�Ŏ��� (�X�����E�u�[�X�g) �}�N����`
//----------------------------------------------------------------------------------------------------------------------
#define MAX_ABI_BOOST		(2)			// �u�[�X�g�o�[�Ɏg�p����|���S����

#define ABI_BOOST_POS_X		(65.0f)		// �\�̓o�[�̐�΍��W (x)
#define ABI_BOOST_POS_Y		(138.0f)	// �\�̓o�[�̐�΍��W (y)
#define ABI_BOOST_WIDTH_MUL	(0.66f)		// �\�̓o�[�̉����̃v���C���[�\�͏�Z��
#define ABI_BOOST_HEIGHT	(35.0f)		// �\�̓o�[�̏c�� / 2

//----------------------------------------------------------------------------------------------------------------------
//	����U�� (�t���C�E�A�E�F�C) �}�N����`
//----------------------------------------------------------------------------------------------------------------------
#define MAX_ABI_WIND		(2)			// �E�B���h�o�[�Ɏg�p����|���S����

#define ABI_WIND_POS_X		(185.0f)	// �\�̓o�[�̐�΍��W (x)
#define ABI_WIND_POS_Y		(138.0f)	// �\�̓o�[�̐�΍��W (y)
#define ABI_WIND_WIDTH_MUL	(0.655f)	// �\�̓o�[�̉����̃v���C���[�\�͏�Z��
#define ABI_WIND_HEIGHT		(35.0f)		// �\�̓o�[�̏c�� / 2

//----------------------------------------------------------------------------------------------------------------------
//	�������E (�T�C�����X�E���[���h) �}�N����`
//----------------------------------------------------------------------------------------------------------------------
#define MAX_ABI_BOMB		(2)			// �{���o�[�Ɏg�p����|���S����

#define ABI_BOMB_POS_X		(305.0f)	// �\�̓o�[�̐�΍��W (x)
#define ABI_BOMB_POS_Y		(138.0f)	// �\�̓o�[�̐�΍��W (y)
#define ABI_BOMB_WIDTH_MUL	(0.062f)	// �\�̓o�[�̉����̃v���C���[�\�͏�Z��
#define ABI_BOMB_HEIGHT		(35.0f)		// �\�̓o�[�̏c�� / 2

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTextureAbility[] =	// �e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\alpha001.png",	// �\�͕\���̓��ߌ^�̃e�N�X�`���̑��΃p�X
	"data\\TEXTURE\\ui001.png",		// �\�͕\���̔w�i�̃e�N�X�`���̑��΃p�X
	"data\\TEXTURE\\frame001.png",	// �\�͕\���̌^�̃e�N�X�`���̑��΃p�X
};

//**********************************************************************************************************************
//	�񋓌^��` (TEXTURE_ABI)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_ABI_ALPHA = 0,			// �\�͕\���̓��ߌ^
	TEXTURE_ABI_BG,					// �\�͕\���̔w�i
	TEXTURE_ABI_FRAME,				// �\�͕\���̌^
	TEXTURE_ABI_MAX,				// ���̗񋓌^�̑���
} TEXTURE_ABI;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void InitAbi(void);					// �\�͕\���̏�����
void UninitAbi(void);				// �\�͕\���̏I��
void UpdateAbi(void);				// �\�͕\���̍X�V
void DrawAbi(bool bBG);				// �\�͕\���̕`��

void InitAbiBoost(void);			// �u�[�X�g�Q�[�W�̏�����
void UninitAbiBoost(void);			// �u�[�X�g�Q�[�W�̏I��
void UpdateAbiBoost(void);			// �u�[�X�g�Q�[�W�̍X�V
void DrawAbiBoost(void);			// �u�[�X�g�Q�[�W�̕`��

void InitAbiWind(void);				// �E�B���h�Q�[�W�̏�����
void UninitAbiWind(void);			// �E�B���h�Q�[�W�̏I��
void UpdateAbiWind(void);			// �E�B���h�Q�[�W�̍X�V
void DrawAbiWind(void);				// �E�B���h�Q�[�W�̕`��

void InitAbiBomb(void);				// �{���Q�[�W�̏�����
void UninitAbiBomb(void);			// �{���Q�[�W�̏I��
void UpdateAbiBomb(void);			// �{���Q�[�W�̍X�V
void DrawAbiBomb(void);				// �{���Q�[�W�̕`��

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureAbility[TEXTURE_ABI_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAbility  = NULL;					// �\�͕\���̒��_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAbiBoost = NULL;					// �u�[�X�g�Q�[�W�̒��_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAbiWind  = NULL;					// �E�B���h�Q�[�W�̒��_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAbiBomb  = NULL;					// �{���Q�[�W�̒��_�o�b�t�@�ւ̃|�C���^

//======================================================================================================================
//	�\�̓o�[�̏���������
//======================================================================================================================
void InitAbility(void)
{
	// �\�͕\���̏�����
	InitAbi();

	// �u�[�X�g�Q�[�W�̏�����
	InitAbiBoost();

	// �E�B���h�Q�[�W�̏�����
	InitAbiWind();

	// �{���Q�[�W�̏�����
	InitAbiBomb();
}

//======================================================================================================================
//	�\�̓o�[�̏I������
//======================================================================================================================
void UninitAbility(void)
{
	// �\�͕\���̏I��
	UninitAbi();

	// �u�[�X�g�Q�[�W�̏I��
	UninitAbiBoost();
	
	// �E�B���h�Q�[�W�̏I��
	UninitAbiWind();

	// �{���Q�[�W�̏I��
	UninitAbiBomb();
}

//======================================================================================================================
//	�\�̓o�[�̍X�V����
//======================================================================================================================
void UpdateAbility(void)
{
	// �\�͕\���̍X�V
	UpdateAbi();

	// �u�[�X�g�Q�[�W�̍X�V
	UpdateAbiBoost();

	// �E�B���h�Q�[�W�̍X�V
	UpdateAbiWind();

	// �{���Q�[�W�̍X�V
	UpdateAbiBomb();
}

//======================================================================================================================
//	�\�̓o�[�̕`�揈��
//======================================================================================================================
void DrawAbility(void)
{
	// �\�͕\���̕`�� (�w�i)
	DrawAbi(true);

	// �u�[�X�g�Q�[�W�̕`��
	DrawAbiBoost();

	// �E�B���h�Q�[�W�̕`��
	DrawAbiWind();

	// �{���Q�[�W�̕`��
	DrawAbiBomb();

	// �\�͕\���̕`�� (�g)
	DrawAbi(false);
}

//======================================================================================================================
//	�\�͕\���̏���������
//======================================================================================================================
void InitAbi(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;							// ���_���ւ̃|�C���^

	for (int nCntAbility = 0; nCntAbility < TEXTURE_ABI_MAX; nCntAbility++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, apTextureAbility[nCntAbility], &g_apTextureAbility[nCntAbility]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_2D) * 4 * MAX_ABILITY,	// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffAbility,
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAbility->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------------------------------------------------------------------------------------------
	//	���ߌ^
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[0].pos = D3DXVECTOR3(ABI_BG_POS_X - ABI_BG_WIDTH, ABI_BG_POS_Y - ABI_BG_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(ABI_BG_POS_X + ABI_BG_WIDTH, ABI_BG_POS_Y - ABI_BG_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(ABI_BG_POS_X - ABI_BG_WIDTH, ABI_BG_POS_Y + ABI_BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(ABI_BG_POS_X + ABI_BG_WIDTH, ABI_BG_POS_Y + ABI_BG_HEIGHT, 0.0f);

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
	//	�w�i
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[4].pos = D3DXVECTOR3(ABI_BG_POS_X - ABI_BG_WIDTH, ABI_BG_POS_Y - ABI_BG_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(ABI_BG_POS_X + ABI_BG_WIDTH, ABI_BG_POS_Y - ABI_BG_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(ABI_BG_POS_X - ABI_BG_WIDTH, ABI_BG_POS_Y + ABI_BG_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(ABI_BG_POS_X + ABI_BG_WIDTH, ABI_BG_POS_Y + ABI_BG_HEIGHT, 0.0f);

	// rhw �̐ݒ�
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//------------------------------------------------------------------------------------------------------------------
	//	�^
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[8].pos  = D3DXVECTOR3(ABI_BG_POS_X - ABI_BG_WIDTH, ABI_BG_POS_Y - ABI_BG_HEIGHT, 0.0f);
	pVtx[9].pos  = D3DXVECTOR3(ABI_BG_POS_X + ABI_BG_WIDTH, ABI_BG_POS_Y - ABI_BG_HEIGHT, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(ABI_BG_POS_X - ABI_BG_WIDTH, ABI_BG_POS_Y + ABI_BG_HEIGHT, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(ABI_BG_POS_X + ABI_BG_WIDTH, ABI_BG_POS_Y + ABI_BG_HEIGHT, 0.0f);

	// rhw �̐ݒ�
	pVtx[8].rhw  = 1.0f;
	pVtx[9].rhw  = 1.0f;
	pVtx[10].rhw = 1.0f;
	pVtx[11].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[8].col  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[9].col  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[8].tex  = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex  = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAbility->Unlock();
}

//======================================================================================================================
//	�\�͕\���̏I������
//======================================================================================================================
void UninitAbi(void)
{
	// �e�N�X�`���̔j��
	for (int nCntAbility = 0; nCntAbility < TEXTURE_ABI_MAX; nCntAbility++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTextureAbility[nCntAbility] != NULL)
		{ // �ϐ� (g_apTextureAbility) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureAbility[nCntAbility]->Release();
			g_apTextureAbility[nCntAbility] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffAbility != NULL)
	{ // �ϐ� (g_pVtxBuffAbility) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffAbility->Release();
		g_pVtxBuffAbility = NULL;
	}
}

//======================================================================================================================
//	�\�͕\���̍X�V����
//======================================================================================================================
void UpdateAbi(void)
{

}

//======================================================================================================================
//	�\�͕\���̕`�揈��
//======================================================================================================================
void DrawAbi(bool bBG)
{
	// �ϐ���錾
	int nInit, nLoop;	// �J��Ԃ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffAbility, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (bBG)
	{ // �w�i�̏ꍇ

		// �J��Ԃ��̐ݒ�
		nInit = ABI_INIT_BG;	// �����l
		nLoop = ABI_LOOP_BG;	// �J��Ԃ���
	}
	else
	{ // �g�̏ꍇ

		// �J��Ԃ��̐ݒ�
		nInit = ABI_INIT_FRAME;	// �����l
		nLoop = ABI_LOOP_FRAME;	// �J��Ԃ���
	}

	for (int nCntAbility = nInit; nCntAbility < nLoop; nCntAbility++)
	{ // �g�p����|���S�������J��Ԃ�

		switch (nCntAbility)
		{ // �`�悷��|���S�����Ƃ̏���
		case TEXTURE_ABI_ALPHA:

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

		case TEXTURE_ABI_BG:

			// Z�e�X�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL);		// Z�e�X�g�̐ݒ�
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

			// �����𔲂���
			break;

		case TEXTURE_ABI_FRAME:

			// ����

			// �����𔲂���
			break;
		}

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureAbility[nCntAbility]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAbility * 4, 2);

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
//	�u�[�X�g�Q�[�W�̏���������
//======================================================================================================================
void InitAbiBoost(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;							// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_2D) * 4 * MAX_ABI_BOOST,	// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffAbiBoost,
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAbiBoost->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------------------------------------------------------------------------------------------
	//	�\�̓o�[ (��)
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[0].pos = D3DXVECTOR3(ABI_BOOST_POS_X,                                          ABI_BOOST_POS_Y - ABI_BOOST_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(ABI_BOOST_POS_X + (BOOST_WAIT_CNT * ABI_BOOST_WIDTH_MUL), ABI_BOOST_POS_Y - ABI_BOOST_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(ABI_BOOST_POS_X,                                          ABI_BOOST_POS_Y + ABI_BOOST_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(ABI_BOOST_POS_X + (BOOST_WAIT_CNT * ABI_BOOST_WIDTH_MUL), ABI_BOOST_POS_Y + ABI_BOOST_HEIGHT, 0.0f);

	// rhw �̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 50, 50, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 50, 50, 255);
	pVtx[2].col = D3DCOLOR_RGBA(205,  0,  0, 255);
	pVtx[3].col = D3DCOLOR_RGBA(205,  0,  0, 255);

	//------------------------------------------------------------------------------------------------------------------
	//	�\�̓o�[ (�I�����W)
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[4].pos = D3DXVECTOR3(ABI_BOOST_POS_X,                                          ABI_BOOST_POS_Y - ABI_BOOST_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(ABI_BOOST_POS_X + (BOOST_WAIT_CNT * ABI_BOOST_WIDTH_MUL), ABI_BOOST_POS_Y - ABI_BOOST_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(ABI_BOOST_POS_X,                                          ABI_BOOST_POS_Y + ABI_BOOST_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(ABI_BOOST_POS_X + (BOOST_WAIT_CNT * ABI_BOOST_WIDTH_MUL), ABI_BOOST_POS_Y + ABI_BOOST_HEIGHT, 0.0f);

	// rhw �̐ݒ�
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[4].col = D3DCOLOR_RGBA(255, 160, 0, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 160, 0, 255);
	pVtx[6].col = D3DCOLOR_RGBA(205, 110, 0, 255);
	pVtx[7].col = D3DCOLOR_RGBA(205, 110, 0, 255);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAbiBoost->Unlock();
}

//======================================================================================================================
//	�u�[�X�g�Q�[�W�̏I������
//======================================================================================================================
void UninitAbiBoost(void)
{
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffAbiBoost != NULL)
	{ // �ϐ� (g_pVtxBuffAbiBoost) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffAbiBoost->Release();
		g_pVtxBuffAbiBoost = NULL;
	}
}

//======================================================================================================================
//	�u�[�X�g�Q�[�W�̍X�V����
//======================================================================================================================
void UpdateAbiBoost(void)
{
	// �ϐ���錾
	float fCurrentBoost = 0.0f;			// ���݂̃u�[�X�g�̑ҋ@�A�g�p����

	// �|�C���^��錾
	VERTEX_2D *pVtx;					// ���_���ւ̃|�C���^
	Player    *pPlayer = GetPlayer();	// �v���C���[�̏��

	// ���݂̃u�[�X�g�̑ҋ@�A�g�p���Ԃ����߂�
	if (pPlayer->boost.state == BOOSTSTATE_NONE
	||  pPlayer->boost.state == BOOSTSTATE_WAIT)
	{ // �u�[�X�g���������Ȃ���ԁA�܂��͑ҋ@��Ԃ̏ꍇ

		// �u�[�X�g�̑ҋ@���Ԃ��v�Z
		fCurrentBoost = (float)(BOOST_WAIT_CNT - pPlayer->boost.nCounter);
	}
	else if (pPlayer->boost.state == BOOSTSTATE_UP)
	{ // �u�[�X�g��������Ԃ̏ꍇ

		// �u�[�X�g�̎g�p���Ԃ��v�Z
		fCurrentBoost = (float)pPlayer->boost.nCounter;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAbiBoost->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W��ݒ�
	pVtx[4].pos = D3DXVECTOR3(ABI_BOOST_POS_X,                                         ABI_BOOST_POS_Y - ABI_BOOST_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(ABI_BOOST_POS_X + (fCurrentBoost * ABI_BOOST_WIDTH_MUL), ABI_BOOST_POS_Y - ABI_BOOST_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(ABI_BOOST_POS_X,                                         ABI_BOOST_POS_Y + ABI_BOOST_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(ABI_BOOST_POS_X + (fCurrentBoost * ABI_BOOST_WIDTH_MUL), ABI_BOOST_POS_Y + ABI_BOOST_HEIGHT, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAbiBoost->Unlock();
}

//======================================================================================================================
//	�u�[�X�g�Q�[�W�̕`�揈��
//======================================================================================================================
void DrawAbiBoost(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffAbiBoost, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	for (int nCntAbility = 0; nCntAbility < MAX_ABI_BOOST; nCntAbility++)
	{ // �g�p����|���S�������J��Ԃ�

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAbility * 4, 2);
	}

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�
}

//======================================================================================================================
//	�E�B���h�Q�[�W�̏���������
//======================================================================================================================
void InitAbiWind(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;							// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_2D) * 4 * MAX_ABI_WIND,	// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffAbiWind,
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAbiWind->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------------------------------------------------------------------------------------------
	//	�\�̓o�[ (��)
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[0].pos = D3DXVECTOR3(ABI_WIND_POS_X,                                            ABI_WIND_POS_Y - ABI_WIND_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(ABI_WIND_POS_X + (WIND_OVERHEAT_CNT * ABI_WIND_WIDTH_MUL), ABI_WIND_POS_Y - ABI_WIND_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(ABI_WIND_POS_X,                                            ABI_WIND_POS_Y + ABI_WIND_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(ABI_WIND_POS_X + (WIND_OVERHEAT_CNT * ABI_WIND_WIDTH_MUL), ABI_WIND_POS_Y + ABI_WIND_HEIGHT, 0.0f);

	// rhw �̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 50, 50, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 50, 50, 255);
	pVtx[2].col = D3DCOLOR_RGBA(205,  0,  0, 255);
	pVtx[3].col = D3DCOLOR_RGBA(205,  0,  0, 255);

	//------------------------------------------------------------------------------------------------------------------
	//	�\�̓o�[ (��)
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[4].pos = D3DXVECTOR3(ABI_WIND_POS_X,                                            ABI_WIND_POS_Y - ABI_WIND_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(ABI_WIND_POS_X + (WIND_OVERHEAT_CNT * ABI_WIND_WIDTH_MUL), ABI_WIND_POS_Y - ABI_WIND_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(ABI_WIND_POS_X,                                            ABI_WIND_POS_Y + ABI_WIND_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(ABI_WIND_POS_X + (WIND_OVERHEAT_CNT * ABI_WIND_WIDTH_MUL), ABI_WIND_POS_Y + ABI_WIND_HEIGHT, 0.0f);

	// rhw �̐ݒ�
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[4].col = D3DCOLOR_RGBA(120, 255, 110, 255);
	pVtx[5].col = D3DCOLOR_RGBA(120, 255, 110, 255);
	pVtx[6].col = D3DCOLOR_RGBA( 70, 205,  60, 255);
	pVtx[7].col = D3DCOLOR_RGBA( 70, 205,  60, 255);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAbiWind->Unlock();
}

//======================================================================================================================
//	�E�B���h�Q�[�W�̏I������
//======================================================================================================================
void UninitAbiWind(void)
{
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffAbiWind != NULL)
	{ // �ϐ� (g_pVtxBuffAbiWind) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffAbiWind->Release();
		g_pVtxBuffAbiWind = NULL;
	}
}

//======================================================================================================================
//	�E�B���h�Q�[�W�̍X�V����
//======================================================================================================================
void UpdateAbiWind(void)
{
	// �ϐ���錾
	float fCurrentWind = 0.0f;			// ���݂̃E�B���h�̑ҋ@�A�g�p����

	// �|�C���^��錾
	VERTEX_2D *pVtx;					// ���_���ւ̃|�C���^
	WindInfo  *pWind = GetWindInfo();	// �E�B���h�̏��

	// ���݂̃E�B���h�̑ҋ@�A�g�p���Ԃ����߂�
	fCurrentWind = (float)(WIND_OVERHEAT_CNT - pWind->nUseCounter);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAbiWind->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W��ݒ�
	pVtx[4].pos = D3DXVECTOR3(ABI_WIND_POS_X,                                       ABI_WIND_POS_Y - ABI_WIND_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(ABI_WIND_POS_X + (fCurrentWind * ABI_WIND_WIDTH_MUL), ABI_WIND_POS_Y - ABI_WIND_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(ABI_WIND_POS_X,                                       ABI_WIND_POS_Y + ABI_WIND_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(ABI_WIND_POS_X + (fCurrentWind * ABI_WIND_WIDTH_MUL), ABI_WIND_POS_Y + ABI_WIND_HEIGHT, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAbiWind->Unlock();
}

//======================================================================================================================
//	�E�B���h�Q�[�W�̕`�揈��
//======================================================================================================================
void DrawAbiWind(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffAbiWind, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	for (int nCntAbility = 0; nCntAbility < MAX_ABI_WIND; nCntAbility++)
	{ // �g�p����|���S�������J��Ԃ�

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAbility * 4, 2);
	}

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�
}

//======================================================================================================================
//	�{���Q�[�W�̏���������
//======================================================================================================================
void InitAbiBomb(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;							// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_2D) * 4 * MAX_ABI_BOMB,	// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffAbiBomb,
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAbiBomb->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------------------------------------------------------------------------------------------
	//	�\�̓o�[ (��)
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[0].pos = D3DXVECTOR3(ABI_BOMB_POS_X,                                        ABI_BOMB_POS_Y - ABI_BOMB_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(ABI_BOMB_POS_X + (BOMB_WAIT_CNT * ABI_BOMB_WIDTH_MUL), ABI_BOMB_POS_Y - ABI_BOMB_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(ABI_BOMB_POS_X,                                        ABI_BOMB_POS_Y + ABI_BOMB_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(ABI_BOMB_POS_X + (BOMB_WAIT_CNT * ABI_BOMB_WIDTH_MUL), ABI_BOMB_POS_Y + ABI_BOMB_HEIGHT, 0.0f);

	// rhw �̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 50, 50, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 50, 50, 255);
	pVtx[2].col = D3DCOLOR_RGBA(205,  0,  0, 255);
	pVtx[3].col = D3DCOLOR_RGBA(205,  0,  0, 255);

	//------------------------------------------------------------------------------------------------------------------
	//	�\�̓o�[ (��)
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[4].pos = D3DXVECTOR3(ABI_BOMB_POS_X,                                         ABI_BOMB_POS_Y - ABI_BOMB_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(ABI_BOMB_POS_X + (BOOST_WAIT_CNT * ABI_BOMB_WIDTH_MUL), ABI_BOMB_POS_Y - ABI_BOMB_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(ABI_BOMB_POS_X,                                         ABI_BOMB_POS_Y + ABI_BOMB_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(ABI_BOMB_POS_X + (BOOST_WAIT_CNT * ABI_BOMB_WIDTH_MUL), ABI_BOMB_POS_Y + ABI_BOMB_HEIGHT, 0.0f);

	// rhw �̐ݒ�
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[4].col = D3DCOLOR_RGBA(110, 210, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(110, 210, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA( 60, 160, 205, 255);
	pVtx[7].col = D3DCOLOR_RGBA( 60, 160, 205, 255);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAbiBomb->Unlock();
}

//======================================================================================================================
//	�{���Q�[�W�̏I������
//======================================================================================================================
void UninitAbiBomb(void)
{
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffAbiBomb != NULL)
	{ // �ϐ� (g_pVtxBuffAbiBomb) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffAbiBomb->Release();
		g_pVtxBuffAbiBomb = NULL;
	}
}

//======================================================================================================================
//	�{���Q�[�W�̍X�V����
//======================================================================================================================
void UpdateAbiBomb(void)
{
	// �ϐ���錾
	float fCurrentBomb = 0.0f;			// ���݂̃{���̑ҋ@�A�g�p����

	// �|�C���^��錾
	VERTEX_2D *pVtx;					// ���_���ւ̃|�C���^
	Player    *pPlayer = GetPlayer();	// �v���C���[�̏��

	// ���݂̃{���̑ҋ@�A�g�p���Ԃ����߂�
	//if (pPlayer->boost.state == BOOSTSTATE_UP)
	{ // �{����������Ԃ̏ꍇ

		// �{���̎g�p���Ԃ��v�Z
		fCurrentBomb = (float)pPlayer->bomb.nCounterState;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAbiBomb->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W��ݒ�
	pVtx[4].pos = D3DXVECTOR3(ABI_BOMB_POS_X,                                       ABI_BOMB_POS_Y - ABI_BOMB_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(ABI_BOMB_POS_X + (fCurrentBomb * ABI_BOMB_WIDTH_MUL), ABI_BOMB_POS_Y - ABI_BOMB_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(ABI_BOMB_POS_X,                                       ABI_BOMB_POS_Y + ABI_BOMB_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(ABI_BOMB_POS_X + (fCurrentBomb * ABI_BOMB_WIDTH_MUL), ABI_BOMB_POS_Y + ABI_BOMB_HEIGHT, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAbiBomb->Unlock();
}

//======================================================================================================================
//	�{���Q�[�W�̕`�揈��
//======================================================================================================================
void DrawAbiBomb(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffAbiBomb, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	for (int nCntAbility = 0; nCntAbility < MAX_ABI_BOMB; nCntAbility++)
	{ // �g�p����|���S�������J��Ԃ�

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAbility * 4, 2);
	}

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�
}