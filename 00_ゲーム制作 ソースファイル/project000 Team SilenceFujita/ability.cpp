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
#define MAX_ABI_BOOST		(2)			// �u�[�X�g�o�[�Ɏg�p����|���S����

#define ABI_BOOST_POS_X		(60.0f)		// �\�̓o�[�̐�΍��W (x)
#define ABI_BOOST_POS_Y		(135.0f)	// �\�̓o�[�̐�΍��W (y)
#define ABI_BOOST_WIDTH_MUL	(1.4f)		// �\�̓o�[�̉����̃v���C���[�\�͏�Z��
#define ABI_BOOST_HEIGHT	(15.0f)		// �\�̓o�[�̏c�� / 2

#define MAX_ABI_WIND		(2)			// �E�B���h�o�[�Ɏg�p����|���S����

#define ABI_WIND_POS_X		(60.0f)		// �\�̓o�[�̐�΍��W (x)
#define ABI_WIND_POS_Y		(175.0f)	// �\�̓o�[�̐�΍��W (y)
#define ABI_WIND_WIDTH_MUL	(1.4f)		// �\�̓o�[�̉����̃v���C���[�\�͏�Z��
#define ABI_WIND_HEIGHT		(15.0f)		// �\�̓o�[�̏c�� / 2

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTextureAbility[] =	// �e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\ui000.png",		// �\�̓o�[�w�i�̃e�N�X�`���̑��΃p�X
};

//**********************************************************************************************************************
//	�񋓌^��` (TEXTURE_ABI)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_ABI_NORMAL = 0,			// �\�̓o�[�̔w�i
	TEXTURE_ABI_MAX,				// ���̗񋓌^�̑���
} TEXTURE_ABI;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void InitAbiBoost(void);			// �u�[�X�g�Q�[�W�̏�����
void UninitAbiBoost(void);			// �u�[�X�g�Q�[�W�̏I��
void UpdateAbiBoost(void);			// �u�[�X�g�Q�[�W�̍X�V
void DrawAbiBoost(void);			// �u�[�X�g�Q�[�W�̕`��

void InitAbiWind(void);				// �E�B���h�Q�[�W�̏�����
void UninitAbiWind(void);			// �E�B���h�Q�[�W�̏I��
void UpdateAbiWind(void);			// �E�B���h�Q�[�W�̍X�V
void DrawAbiWind(void);				// �E�B���h�Q�[�W�̕`��

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureAbility[TEXTURE_ABI_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAbiBoost = NULL;					// �u�[�X�g�Q�[�W�̒��_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAbiWind  = NULL;					// �E�B���h�Q�[�W�̒��_�o�b�t�@�ւ̃|�C���^

//======================================================================================================================
//	�\�̓o�[�̏���������
//======================================================================================================================
void InitAbility(void)
{
	// �u�[�X�g�Q�[�W�̏�����
	InitAbiBoost();

	// �E�B���h�Q�[�W�̏�����
	InitAbiWind();
}

//======================================================================================================================
//	�\�̓o�[�̏I������
//======================================================================================================================
void UninitAbility(void)
{
	// �u�[�X�g�Q�[�W�̏I��
	UninitAbiBoost();
	
	// �E�B���h�Q�[�W�̏I��
	UninitAbiWind();
}

//======================================================================================================================
//	�\�̓o�[�̍X�V����
//======================================================================================================================
void UpdateAbility(void)
{
	// �u�[�X�g�Q�[�W�̍X�V
	UpdateAbiBoost();

	// �E�B���h�Q�[�W�̍X�V
	UpdateAbiWind();
}

//======================================================================================================================
//	�\�̓o�[�̕`�揈��
//======================================================================================================================
void DrawAbility(void)
{
	// �u�[�X�g�Q�[�W�̕`��
	DrawAbiBoost();

	// �E�B���h�Q�[�W�̕`��
	DrawAbiWind();
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
	//	�\�̓o�[ (��)
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
	pVtx[4].col = D3DCOLOR_RGBA(255, 250, 50, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 250, 50, 255);
	pVtx[6].col = D3DCOLOR_RGBA(205, 160,  0, 255);
	pVtx[7].col = D3DCOLOR_RGBA(205, 160,  0, 255);

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

	for (int nCntAbility = 0; nCntAbility < MAX_ABI_BOOST; nCntAbility++)
	{ // �g�p����|���S�������J��Ԃ�

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAbility * 4, 2);
	}
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
	pVtx[0].pos = D3DXVECTOR3(ABI_WIND_POS_X,                                         ABI_WIND_POS_Y - ABI_WIND_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(ABI_WIND_POS_X + (BOOST_WAIT_CNT * ABI_WIND_WIDTH_MUL), ABI_WIND_POS_Y - ABI_WIND_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(ABI_WIND_POS_X,                                         ABI_WIND_POS_Y + ABI_WIND_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(ABI_WIND_POS_X + (BOOST_WAIT_CNT * ABI_WIND_WIDTH_MUL), ABI_WIND_POS_Y + ABI_WIND_HEIGHT, 0.0f);

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
	pVtx[4].pos = D3DXVECTOR3(ABI_WIND_POS_X,                                         ABI_WIND_POS_Y - ABI_WIND_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(ABI_WIND_POS_X + (BOOST_WAIT_CNT * ABI_WIND_WIDTH_MUL), ABI_WIND_POS_Y - ABI_WIND_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(ABI_WIND_POS_X,                                         ABI_WIND_POS_Y + ABI_WIND_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(ABI_WIND_POS_X + (BOOST_WAIT_CNT * ABI_WIND_WIDTH_MUL), ABI_WIND_POS_Y + ABI_WIND_HEIGHT, 0.0f);

	// rhw �̐ݒ�
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[4].col = D3DCOLOR_RGBA(255, 250, 50, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 250, 50, 255);
	pVtx[6].col = D3DCOLOR_RGBA(205, 160,  0, 255);
	pVtx[7].col = D3DCOLOR_RGBA(205, 160,  0, 255);

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

	for (int nCntAbility = 0; nCntAbility < MAX_ABI_WIND; nCntAbility++)
	{ // �g�p����|���S�������J��Ԃ�

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAbility * 4, 2);
	}
}