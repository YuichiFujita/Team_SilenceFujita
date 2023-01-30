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

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAX_ABI			(3)			// �g�p����|���S����

#define ABI_POS_X		(60.0f)		// �\�̓o�[�̐�΍��W (x)
#define ABI_POS_Y		(135.0f)	// �\�̓o�[�̐�΍��W (y)
#define ABI_WIDTH_MUL	(1.4f)		// �\�̓o�[�̉����̃v���C���[�\�͏�Z��
#define ABI_HEIGHT		(15.0f)		// �\�̓o�[�̏c�� / 2

#define ABI_BG_POS_X	(190.0f)	// �\�̓o�[�̔w�i�̐�΍��W (x)
#define ABI_BG_POS_Y	(620.0f)	// �\�̓o�[�̔w�i�̐�΍��W (x)
#define ABI_BG_WIDTH	(165.0f)	// �\�̓o�[�̔w�i�̉��� / 2
#define ABI_BG_HEIGHT	(80.0f)		// �\�̓o�[�̔w�i�̏c�� / 2

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTextureAbility[] =	// �e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\ui000.png",		// ���C�t�w�i�̃e�N�X�`���̑��΃p�X
	NULL,							// NULL
	NULL,							// NULL
};

//**********************************************************************************************************************
//	�񋓌^��` (TEXTURE_ABI)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_ABI_NORMAL = 0,			// ���C�t�̔w�i
	TEXTURE_ABI_NULL_00,			// NULL
	TEXTURE_ABI_NULL_01,			// NULL
	TEXTURE_ABI_MAX,				// ���̗񋓌^�̑���
} TEXTURE_ABI;

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureAbility[TEXTURE_ABI_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAbility = NULL;					// ���_�o�b�t�@�ւ̃|�C���^

//======================================================================================================================
//	�\�̓o�[�̏���������
//======================================================================================================================
void InitAbility(void)
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
		sizeof(VERTEX_2D) * 4 * MAX_ABI,		// �K�v���_��
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
	//	�\�̓o�[ (�w�i)
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
	//	�\�̓o�[ (��)
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[4].pos = D3DXVECTOR3(ABI_POS_X,                                    ABI_POS_Y - ABI_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(ABI_POS_X + (BOOST_WAIT_CNT * ABI_WIDTH_MUL), ABI_POS_Y - ABI_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(ABI_POS_X,                                    ABI_POS_Y + ABI_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(ABI_POS_X + (BOOST_WAIT_CNT * ABI_WIDTH_MUL), ABI_POS_Y + ABI_HEIGHT, 0.0f);

	// rhw �̐ݒ�
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[4].col = D3DCOLOR_RGBA(255, 50, 50, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 50, 50, 255);
	pVtx[6].col = D3DCOLOR_RGBA(205,  0,  0, 255);
	pVtx[7].col = D3DCOLOR_RGBA(205,  0,  0, 255);

	//------------------------------------------------------------------------------------------------------------------
	//	�\�̓o�[ (��)
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[8].pos  = D3DXVECTOR3(ABI_POS_X,                                    ABI_POS_Y - ABI_HEIGHT, 0.0f);
	pVtx[9].pos  = D3DXVECTOR3(ABI_POS_X + (BOOST_WAIT_CNT * ABI_WIDTH_MUL), ABI_POS_Y - ABI_HEIGHT, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(ABI_POS_X,                                    ABI_POS_Y + ABI_HEIGHT, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(ABI_POS_X + (BOOST_WAIT_CNT * ABI_WIDTH_MUL), ABI_POS_Y + ABI_HEIGHT, 0.0f);

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

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAbility->Unlock();
}

//======================================================================================================================
//	�\�̓o�[�̏I������
//======================================================================================================================
void UninitAbility(void)
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
//	�\�̓o�[�̍X�V����
//======================================================================================================================
void UpdateAbility(void)
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
	g_pVtxBuffAbility->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W��ݒ�
	pVtx[8].pos  = D3DXVECTOR3(ABI_POS_X,                                   ABI_POS_Y - ABI_HEIGHT, 0.0f);
	pVtx[9].pos  = D3DXVECTOR3(ABI_POS_X + (fCurrentBoost * ABI_WIDTH_MUL), ABI_POS_Y - ABI_HEIGHT, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(ABI_POS_X,                                   ABI_POS_Y + ABI_HEIGHT, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(ABI_POS_X + (fCurrentBoost * ABI_WIDTH_MUL), ABI_POS_Y + ABI_HEIGHT, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAbility->Unlock();
}

//======================================================================================================================
//	�\�̓o�[�̕`�揈��
//======================================================================================================================
void DrawAbility(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	Player           *pPlayer = GetPlayer();	// �v���C���[�̏��

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffAbility, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntAbility = 0; nCntAbility < MAX_ABI; nCntAbility++)
	{ // �g�p����|���S�������J��Ԃ�

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureAbility[nCntAbility]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAbility * 4, 2);
	}
}