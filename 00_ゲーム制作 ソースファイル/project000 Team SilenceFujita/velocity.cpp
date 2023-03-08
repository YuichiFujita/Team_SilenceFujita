//======================================================================================================================
//
//	���x�o�[���� [velocity.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "velocity.h"
#include "player.h"
#include "value.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAX_VELO		(3)			// �g�p����|���S����

#define VAL_VELO_WIDTH	(28.0f)		// �X�R�A�̐��l�̉��� / 2
#define VAL_VELO_HEIGHT	(28.0f)		// �X�R�A�̐��l�̏c�� / 2
#define VAL_VELO_SPACE	(38.0f)		// �X�R�A�̐��l�Ԃ̕� (x)
#define VAL_VELO_DIGIT	(3)			// �X�R�A�̐��l�Ԃ̕� (x)

#define VELO_POS_VAL_X	(190.0f)	// �X�R�A (�l) �̐�΍��W (x)
#define VELO_POS_VAL_Y	(610.0f)	// �X�R�A�̐�΍��W (y)

#define VELO_POS_X		(72.0f)		// ���x�o�[�̐�΍��W (x)
#define VELO_POS_Y		(652.5f)	// ���x�o�[�̐�΍��W (y)
#define VELO_WIDTH_MUL	(3.9f)		// ���x�o�[�̉����̃v���C���[���x��Z��
#define VELO_HEIGHT		(15.0f)		// ���x�o�[�̏c�� / 2
#define VELO_PULS_Y		(66.0f)		// ���x�o�[�̏c���̒ǉ���

#define VELO_BG_POS_X	(190.0f)	// ���x�o�[�̔w�i�̐�΍��W (x)
#define VELO_BG_POS_Y	(620.0f)	// ���x�o�[�̔w�i�̐�΍��W (x)
#define VELO_BG_WIDTH	(155.0f)	// ���x�o�[�̔w�i�̉��� / 2
#define VELO_BG_HEIGHT	(70.0f)		// ���x�o�[�̔w�i�̏c�� / 2

#define MAX_SPEED		(240)		// �\����̍ō����x

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTextureVelocity[] =	// �e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\ui003.png",		// ���C�t�w�i�̃e�N�X�`���̑��΃p�X
	NULL,							// NULL
	NULL,							// NULL
};

//**********************************************************************************************************************
//	�񋓌^��` (TEXTURE_VELO)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_VELO_NORMAL = 0,		// ���C�t�̔w�i
	TEXTURE_VELO_NULL_00,			// NULL
	TEXTURE_VELO_NULL_01,			// NULL
	TEXTURE_VELO_MAX,				// ���̗񋓌^�̑���
} TEXTURE_VELO;

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureVelocity[TEXTURE_VELO_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffVelocity = NULL;					// ���_�o�b�t�@�ւ̃|�C���^

//======================================================================================================================
//	���x�o�[�̏���������
//======================================================================================================================
void InitVelocity(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;							// ���_���ւ̃|�C���^

	for (int nCntVelocity = 0; nCntVelocity < TEXTURE_VELO_MAX; nCntVelocity++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, apTextureVelocity[nCntVelocity], &g_apTextureVelocity[nCntVelocity]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_2D) * 4 * MAX_VELO,		// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffVelocity,
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffVelocity->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------------------------------------------------------------------------------------------
	//	���x�o�[ (�w�i)
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[0].pos = D3DXVECTOR3(VELO_BG_POS_X - VELO_BG_WIDTH, VELO_BG_POS_Y - VELO_BG_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(VELO_BG_POS_X + VELO_BG_WIDTH, VELO_BG_POS_Y - VELO_BG_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(VELO_BG_POS_X - VELO_BG_WIDTH, VELO_BG_POS_Y + VELO_BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(VELO_BG_POS_X + VELO_BG_WIDTH, VELO_BG_POS_Y + VELO_BG_HEIGHT, 0.0f);

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
	//	���x�o�[ (��)
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[4].pos = D3DXVECTOR3(VELO_POS_X, VELO_POS_Y - VELO_HEIGHT, 0.0f);

	pVtx[5].pos.x = VELO_POS_X + (MAX_REAL_SPEED * VELO_WIDTH_MUL);
	pVtx[5].pos.y = (VELO_POS_Y - VELO_HEIGHT) - VELO_PULS_Y;
	pVtx[5].pos.z = 0.0f;

	pVtx[6].pos = D3DXVECTOR3(VELO_POS_X,                                     VELO_POS_Y + VELO_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(VELO_POS_X + (MAX_REAL_SPEED * VELO_WIDTH_MUL), VELO_POS_Y + VELO_HEIGHT, 0.0f);

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
	//	���x�o�[ (��)
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[8].pos = D3DXVECTOR3(VELO_POS_X, VELO_POS_Y - VELO_HEIGHT, 0.0f);

	pVtx[9].pos.x = VELO_POS_X;
	pVtx[9].pos.y = VELO_POS_Y - VELO_HEIGHT;
	pVtx[9].pos.z = 0.0f;

	pVtx[10].pos = D3DXVECTOR3(VELO_POS_X, VELO_POS_Y + VELO_HEIGHT, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(VELO_POS_X, VELO_POS_Y + VELO_HEIGHT, 0.0f);

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
	g_pVtxBuffVelocity->Unlock();
}

//======================================================================================================================
//	���x�o�[�̏I������
//======================================================================================================================
void UninitVelocity(void)
{
	// �e�N�X�`���̔j��
	for (int nCntVelocity = 0; nCntVelocity < TEXTURE_VELO_MAX; nCntVelocity++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTextureVelocity[nCntVelocity] != NULL)
		{ // �ϐ� (g_apTextureVelocity) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureVelocity[nCntVelocity]->Release();
			g_apTextureVelocity[nCntVelocity] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffVelocity != NULL)
	{ // �ϐ� (g_pVtxBuffVelocity) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffVelocity->Release();
		g_pVtxBuffVelocity = NULL;
	}
}

//======================================================================================================================
//	���x�o�[�̍X�V����
//======================================================================================================================
void UpdateVelocity(void)
{
	// �ϐ���錾
	float fCurrentSpeed;	// ���݂̃v���C���[���x�̐�Βl

	// �|�C���^��錾
	VERTEX_2D *pVtx;					// ���_���ւ̃|�C���^
	Player    *pPlayer = GetPlayer();	// �v���C���[�̏��

	// ���݂̃v���C���[���x�̐�Βl�����߂�
	fCurrentSpeed = fabsf(pPlayer->move.x + pPlayer->boost.plusMove.x);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffVelocity->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W��ݒ�
	pVtx[8].pos = D3DXVECTOR3(VELO_POS_X, VELO_POS_Y - VELO_HEIGHT, 0.0f);

	pVtx[9].pos.x = VELO_POS_X + (fCurrentSpeed * VELO_WIDTH_MUL);
	pVtx[9].pos.y = (VELO_POS_Y - VELO_HEIGHT) - (fCurrentSpeed * (VELO_PULS_Y / MAX_REAL_SPEED));
	pVtx[9].pos.z = 0.0f;

	pVtx[10].pos = D3DXVECTOR3(VELO_POS_X,                                    VELO_POS_Y + VELO_HEIGHT, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(VELO_POS_X + (fCurrentSpeed * VELO_WIDTH_MUL), VELO_POS_Y + VELO_HEIGHT, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffVelocity->Unlock();
}

//======================================================================================================================
//	���x�o�[�̕`�揈��
//======================================================================================================================
void DrawVelocity(void)
{
	// �ϐ���錾
	int nCurrentSpeed;		// ���݂̃X�s�[�h

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	Player           *pPlayer = GetPlayer();	// �v���C���[�̏��

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffVelocity, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntVelocity = 0; nCntVelocity < MAX_VELO; nCntVelocity++)
	{ // �g�p����|���S�������J��Ԃ�

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureVelocity[nCntVelocity]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntVelocity * 4, 2);
	}

	//------------------------------------------------------------------------------------------------------------------
	//	���l�̕`��
	//------------------------------------------------------------------------------------------------------------------
	// ���݂̃X�s�[�h�����߂�
	nCurrentSpeed = (int)(fabsf(pPlayer->move.x + pPlayer->boost.plusMove.x) * (MAX_SPEED / MAX_REAL_SPEED));

	// ���l�̐ݒ�
	SetValue
	( // ����
		D3DXVECTOR3
		( // ����
			VELO_POS_VAL_X,	// �ʒu (x)
			VELO_POS_VAL_Y,	// �ʒu (y)
			0.0f			// �ʒu (z)
		),
		nCurrentSpeed,		// �l
		MAX_SPEED,			// �l�̍ő�l
		VAL_VELO_WIDTH,		// ����
		VAL_VELO_HEIGHT,	// �c��
		VAL_VELO_SPACE,		// ���l�Ԃ̕�
		1.0f				// ���l
	);

	// ���l�̕`��
	DrawValue(VAL_VELO_DIGIT, VALUETYPE_NORMAL);
}