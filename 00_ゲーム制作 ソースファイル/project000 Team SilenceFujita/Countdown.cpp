//=======================================
//
//�J�E���g�_�E������[Countdown.cpp]
//Author ��������
//
//=======================================
#include"main.h"
#include"Countdown.h"
#include "escape.h"

//�}�N����`
#define COUNTDOWN_APPEAR_CNT	(100)											// �J�E���g�_�E���̏o������̑J�ڃJ�E���g
#define COUNTDOWN_LENGTH_CNT	(40)											// �J�E���g�_�E���̏o������̑J�ڃJ�E���g

#define COUNTDOWN_RADIUS		(D3DXVECTOR3(600.0f, 100.0f, 0.0f))														// �J�E���g�_�E�����a
#define COUNTDOWN_INIT_RADIUS	(D3DXVECTOR3(0.0f, 100.0f, 0.0f))														// �J�E���g�_�E�����a
#define COUNTDOWN_POS			(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - (COUNTDOWN_RADIUS.x * 0.5f), SCREEN_HEIGHT * 0.5f, 0.0f))		// �J�E���g�_�E���̈ʒu
#define COUNTDOWN_LENGTH_MOVE	(COUNTDOWN_RADIUS.x / COUNTDOWN_LENGTH_CNT)		// �J�E���g�_�E���̒����̈ړ���
#define COUNTDOWN_INIT_TEXU		(0.0f)											// �J�E���g�_�E���̃e�N�X�`���̏����l
#define COUNTDOWN_INIT_ALPHA	(0.0f)											// �J�E���g�_�E���̓����x�̏����l
#define COUNTDOWN_ALPHA			(0.5f)											// �J�E���g�_�E���̓����x
#define COUNTDOWN_TEXTURE		"data/TEXTURE/Countdown.png"					// �J�E���g�_�E���̃e�N�X�`��

#define COUNTDOWN_ALPHA_SUB		(0.05f)											// �J�E���g�_�E���̓����x�̌�����

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureCountDown = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCountDown = NULL;		//���_�o�b�t�@�ւ̃|�C���^
CountDown g_CountDown;									//�J�E���g�_�E���̏��

//==========================================
//�J�E���g�_�E���̏���������
//==========================================
void InitCountDown(void)
{
	LPDIRECT3DDEVICE9 pDevice;								//�e���̐������i�[

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		COUNTDOWN_TEXTURE,
		&g_pTextureCountDown);

	// �J�E���g�_�E���̏��̐ݒ�
	g_CountDown.pos		= COUNTDOWN_POS;				// �ʒu
	g_CountDown.length  = COUNTDOWN_INIT_RADIUS;		// ����
	g_CountDown.fAlpha	= COUNTDOWN_INIT_ALPHA;			// �����x
	g_CountDown.fTexU	= COUNTDOWN_INIT_TEXU;			// �e�N�X�`����U���̒l
	g_CountDown.bUse	= false;						// �g�p��

	// ��Ԋ֌W�̐ݒ�
	g_CountDown.stateInfo.state			= CNTDOWNSTATE_NONE;	// ���
	g_CountDown.stateInfo.nStateCounter = 0;					// ��Ԃ̃J�E���g

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCountDown,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffCountDown->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_CountDown.pos.x, g_CountDown.pos.y - g_CountDown.length.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_CountDown.pos.x + g_CountDown.length.x, g_CountDown.pos.y - g_CountDown.length.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_CountDown.pos.x, g_CountDown.pos.y + g_CountDown.length.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_CountDown.pos.x + g_CountDown.length.x, g_CountDown.pos.y + g_CountDown.length.y, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(g_CountDown.fTexU, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(g_CountDown.fTexU, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCountDown->Unlock();
}

//========================================
//�J�E���g�_�E���̏I������
//========================================
void UninitCountDown(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureCountDown != NULL)
	{
		g_pTextureCountDown->Release();
		g_pTextureCountDown = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffCountDown != NULL)
	{
		g_pVtxBuffCountDown->Release();
		g_pVtxBuffCountDown = NULL;
	}
}

//========================================
//�J�E���g�_�E���̍X�V����
//========================================
void UpdateCountDown(void)
{
	VERTEX_2D *pVtx;

	if (g_CountDown.bUse == true)
	{ // �g�p���Ă���ꍇ

		switch (g_CountDown.stateInfo.state)
		{
		case CNTDOWNSTATE_NONE:		// �����

			// �����x��ݒ肷��
			g_CountDown.fAlpha = COUNTDOWN_INIT_ALPHA;

			break;

		case CNTDOWNSTATE_APPEAR:	// �o�����

			// ������L�΂�
			g_CountDown.length.x += COUNTDOWN_LENGTH_MOVE;

			// �e�N�X�`����L�΂�
			g_CountDown.fTexU += 1.0f / COUNTDOWN_LENGTH_CNT;

			// �J�E���g�����Z����
			g_CountDown.stateInfo.nStateCounter++;

			if (g_CountDown.stateInfo.nStateCounter >= COUNTDOWN_LENGTH_CNT)
			{ // �J�E���g�����̃J�E���g�ɂȂ����ꍇ

				// ������ݒ肷��
				g_CountDown.length.x = COUNTDOWN_RADIUS.x;

				// �e�N�X�`����ݒ肷��
				g_CountDown.fTexU = 1.0f;
			}

			if (g_CountDown.stateInfo.nStateCounter >= COUNTDOWN_APPEAR_CNT)
			{ // �J�E���g�����̃J�E���g�ɂȂ����ꍇ

				// �J�E���g������������
				g_CountDown.stateInfo.nStateCounter = 0;

				// �g�剻��Ԃɂ���
				g_CountDown.stateInfo.state = CNTDOWNSTATE_TRANS;
			}

			break;

		case CNTDOWNSTATE_TRANS:	// ���������

			// �����x���ړ�������
			g_CountDown.fAlpha -= COUNTDOWN_ALPHA_SUB;

			if (g_CountDown.fAlpha <= 0.0f)
			{ // �����x�����������ꍇ

				// �����x��ݒ肷��
				g_CountDown.fAlpha = 0.0f;

				// ����Ԃɂ���
				g_CountDown.stateInfo.state = CNTDOWNSTATE_NONE;

				// �g�p���Ȃ�
				g_CountDown.bUse = false;

				// �E�o�ʒm�̐ݒ菈��
				SetEscape();
			}

			break;
		}

		//���_�o�b�t�@�����b�N
		g_pVtxBuffCountDown->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_CountDown.pos.x, g_CountDown.pos.y - g_CountDown.length.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_CountDown.pos.x + g_CountDown.length.x, g_CountDown.pos.y - g_CountDown.length.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_CountDown.pos.x, g_CountDown.pos.y + g_CountDown.length.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_CountDown.pos.x + g_CountDown.length.x, g_CountDown.pos.y + g_CountDown.length.y, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_CountDown.fTexU, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_CountDown.fTexU, 1.0f);

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffCountDown->Unlock();
	}
}

//=====================================
//�J�E���g�_�E���̕`�揈��
//=====================================
void DrawCountDown(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffCountDown,							//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D));								//���_���\���̂̃T�C�Y

	if (g_CountDown.bUse == true)
	{//�g�p����Ă����ꍇ

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureCountDown);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			0,											//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);											//�`�悷��v���~�e�B�u��
	}
}

//=====================================
// �J�E���g�_�E���̐ݒ菈��
//=====================================
void SetCountDown(void)
{
	VERTEX_2D *pVtx;

	if (g_CountDown.bUse == false)
	{ // �g�p���Ă��Ȃ��ꍇ

		// ���̐ݒ�
		g_CountDown.pos		= COUNTDOWN_POS;			// �ʒu
		g_CountDown.length	= COUNTDOWN_INIT_RADIUS;	// ����
		g_CountDown.fAlpha	= COUNTDOWN_ALPHA;			// �����x
		g_CountDown.fTexU	= COUNTDOWN_INIT_TEXU;		// �e�N�X�`����U���̒l

		// ��Ԋ֌W�̏��̐ݒ�
		g_CountDown.stateInfo.state			= CNTDOWNSTATE_APPEAR;		// ���
		g_CountDown.stateInfo.nStateCounter = 0;						// ��ԃJ�E���g

		//���_�o�b�t�@�����b�N
		g_pVtxBuffCountDown->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_CountDown.pos.x, g_CountDown.pos.y - g_CountDown.length.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_CountDown.pos.x + g_CountDown.length.x, g_CountDown.pos.y - g_CountDown.length.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_CountDown.pos.x, g_CountDown.pos.y + g_CountDown.length.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_CountDown.pos.x + g_CountDown.length.x, g_CountDown.pos.y + g_CountDown.length.y, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_CountDown.fAlpha);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_CountDown.fTexU, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_CountDown.fTexU, 1.0f);

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffCountDown->Unlock();

		g_CountDown.bUse = true;						// �g�p����
	}
}