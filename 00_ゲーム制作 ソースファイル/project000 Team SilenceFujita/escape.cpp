//=======================================
//
//�E�o�ʒm����[Escape.cpp]
//Author ��������
//
//=======================================
#include "main.h"
#include "escape.h"
#include "timer.h"

//-----------------------------------------------------
//�}�N����`
//-----------------------------------------------------
#define ESCAPE_APPEAR_CNT			(100)										// �E�o�ʒm�̏o������̑J�ڃJ�E���g
#define ESCAPE_LENGTH_CNT			(40)										// �E�o�ʒm�̏o������̑J�ڃJ�E���g

#define ESCAPE_POS					(D3DXVECTOR3(1150.0f, 40.0f, 0.0f))			// �E�o�ʒm�̈ʒu
#define ESCAPE_RADIUS				(D3DXVECTOR3(80.0f, 20.0f, 0.0f))			// �E�o�ʒm�̔��a
#define ESCAPE_INIT_ALPHA			(0.0f)										// �E�o�ʒm�̓����x�̏����l
#define ESCAPE_ALPHA				(1.0f)										// �E�o�ʒm�̓����x
#define ESCAPE_ADD_ALPHA			(0.01f)										// �E�o�ʒm�̓����x�̉��Z��
#define ESCAPE_EMPHASIS_TIME		(30)										// �E�o�ʒm��������ԂɂȂ鎞��
#define ESCAPE_RADIUS_MOVE			(D3DXVECTOR3(2.0f, 0.5f, 0.0f))				// �E�o�ʒm�̔��a�̊g�嗦
#define ESCAPE_EMPHASIS_RADIUS		(D3DXVECTOR3(120.0f, 30.0f, 0.0f))			// �E�o�ʒm�̋������̔��a
#define ESCAPE_EMPHASIS_WAIT_CNT	(20)										// �E�o�ʒm�̋������̑ҋ@�J�E���g��

#define ESCAPE_TEXTURE		"data/TEXTURE/Escape.png"							// �E�o�ʒm�̃e�N�X�`��

//-----------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------
void EscapeEmphasis(void);								// �E�o�ʒm�̋�������

//-----------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureEscape = NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEscape = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
EscapeInfo g_Escape;									// �E�o�ʒm�̏��

//==========================================
//�E�o�ʒm�̏���������
//==========================================
void InitEscape(void)
{
	LPDIRECT3DDEVICE9 pDevice;								//�e���̐������i�[

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		ESCAPE_TEXTURE,
		&g_pTextureEscape);

	// �E�o�ʒm�̏��̐ݒ�
	g_Escape.pos = ESCAPE_POS;					// �ʒu
	g_Escape.length = ESCAPE_RADIUS;			// ����
	g_Escape.fAlpha = ESCAPE_INIT_ALPHA;		// �����x
	g_Escape.bUse = false;						// �g�p��

	// �E�o�ʒm�̏�Ԋ֌W�̐ݒ�
	g_Escape.stateInfo.nStateCounter = 0;						// ��ԃJ�E���^�[ 
	g_Escape.stateInfo.escapeState	 = ESCAPESTATE_NONE;		// ���
	g_Escape.stateInfo.emphaState	 = EMPHASISSTATE_BIGGER;	// �g�咆���

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEscape,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffEscape->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Escape.pos.x - g_Escape.length.x, g_Escape.pos.y - g_Escape.length.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Escape.pos.x + g_Escape.length.x, g_Escape.pos.y - g_Escape.length.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Escape.pos.x - g_Escape.length.x, g_Escape.pos.y + g_Escape.length.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Escape.pos.x + g_Escape.length.x, g_Escape.pos.y + g_Escape.length.y, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEscape->Unlock();
}

//========================================
//�E�o�ʒm�̏I������
//========================================
void UninitEscape(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureEscape != NULL)
	{
		g_pTextureEscape->Release();
		g_pTextureEscape = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEscape != NULL)
	{
		g_pVtxBuffEscape->Release();
		g_pVtxBuffEscape = NULL;
	}
}

//========================================
//�E�o�ʒm�̍X�V����
//========================================
void UpdateEscape(void)
{
	VERTEX_2D *pVtx;

	if (g_Escape.bUse == true)
	{ // �g�p���Ă���ꍇ

		switch (g_Escape.stateInfo.escapeState)
		{
		case ESCAPESTATE_NONE:			// �����

			// �����x�����Z����
			g_Escape.fAlpha += ESCAPE_ADD_ALPHA;

			if (g_Escape.fAlpha >= ESCAPE_ALPHA)
			{ // ��萔�l�𒴂����ꍇ

				// �����x��␳����
				g_Escape.fAlpha = ESCAPE_ALPHA;

				// �\����Ԃɂ���
				g_Escape.stateInfo.escapeState = ESCAPESTATE_DISP;
			}

			break;

		case ESCAPESTATE_DISP:			// �\�����

			if (GetTime() == ESCAPE_EMPHASIS_TIME)
			{ // ���Ԃ���萔�ɒB�����ꍇ

				// ������Ԃɂ���
				g_Escape.stateInfo.escapeState = ESCAPESTATE_EMPHASIS;
			}

			break;

		case ESCAPESTATE_EMPHASIS:		// �������

			// �E�o�ʒm�̋�������
			EscapeEmphasis();

			break;

		default:						// ��L�ȊO

			break;
		}

		//���_�o�b�t�@�����b�N
		g_pVtxBuffEscape->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Escape.pos.x - g_Escape.length.x, g_Escape.pos.y - g_Escape.length.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Escape.pos.x + g_Escape.length.x, g_Escape.pos.y - g_Escape.length.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Escape.pos.x - g_Escape.length.x, g_Escape.pos.y + g_Escape.length.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Escape.pos.x + g_Escape.length.x, g_Escape.pos.y + g_Escape.length.y, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffEscape->Unlock();
	}
}

//=====================================
//�E�o�ʒm�̕`�揈��
//=====================================
void DrawEscape(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffEscape,							//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D));							//���_���\���̂̃T�C�Y

	if (g_Escape.bUse == true)
	{//�g�p����Ă����ꍇ

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureEscape);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			0,											//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);											//�`�悷��v���~�e�B�u��
	}
}

//=====================================
// �E�o�ʒm�̐ݒ菈��
//=====================================
void SetEscape(void)
{
	VERTEX_2D *pVtx;

	if (g_Escape.bUse == false)
	{ // �g�p���Ă��Ȃ��ꍇ

		// ���̐ݒ�
		g_Escape.pos = ESCAPE_POS;				// �ʒu
		g_Escape.length = ESCAPE_RADIUS;		// ����
		g_Escape.fAlpha = ESCAPE_INIT_ALPHA;	// �����x

		// ��Ԋ֌W�̏��̐ݒ�
		g_Escape.stateInfo.escapeState	 = ESCAPESTATE_NONE;		// ���
		g_Escape.stateInfo.nStateCounter = 0;						// ��ԃJ�E���g
		g_Escape.stateInfo.emphaState	 = EMPHASISSTATE_BIGGER;	// �g�咆���

		//���_�o�b�t�@�����b�N
		g_pVtxBuffEscape->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Escape.pos.x - g_Escape.length.x, g_Escape.pos.y - g_Escape.length.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Escape.pos.x + g_Escape.length.x, g_Escape.pos.y - g_Escape.length.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Escape.pos.x - g_Escape.length.x, g_Escape.pos.y + g_Escape.length.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Escape.pos.x + g_Escape.length.x, g_Escape.pos.y + g_Escape.length.y, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Escape.fAlpha);

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffEscape->Unlock();

		g_Escape.bUse = true;						// �g�p����
	}
}

//=====================================
// �E�o�ʒm�̋�������
//=====================================
void EscapeEmphasis(void)
{
	switch (g_Escape.stateInfo.emphaState)
	{
	case EMPHASISSTATE_BIGGER:		// �g�咆���

		if (g_Escape.stateInfo.nStateCounter <= 0)
		{ // �J�E���^�[��0�ȉ��̏ꍇ

			// ���a���g�傷��
			g_Escape.length.x += ESCAPE_RADIUS_MOVE.x;
			g_Escape.length.y += ESCAPE_RADIUS_MOVE.y;

			if (g_Escape.length.x >= ESCAPE_EMPHASIS_RADIUS.x)
			{ // ���̔��a�𒴂����ꍇ

				// ���a��␳����
				g_Escape.length = ESCAPE_EMPHASIS_RADIUS;

				// ��ԃJ�E���^�[�����Z����
				g_Escape.stateInfo.nStateCounter++;
			}
		}
		else
		{ // �J�E���^�[��0�𒴉߂��Ă����ꍇ

			// ��ԃJ�E���^�[�����Z����
			g_Escape.stateInfo.nStateCounter++;

			if (g_Escape.stateInfo.nStateCounter >= ESCAPE_EMPHASIS_WAIT_CNT)
			{ // �ҋ@�J�E���g�ɒB�����ꍇ

				// �J�E���g������������
				g_Escape.stateInfo.nStateCounter = 0;

				// �k����Ԃɂ���
				g_Escape.stateInfo.emphaState = EMPHASISSTATE_SMALLER;
			}
		}

		break;

	case EMPHASISSTATE_SMALLER:		// �k�������

		if (g_Escape.stateInfo.nStateCounter <= 0)
		{ // �J�E���^�[��0�ȉ��̏ꍇ

			// ���a���g�傷��
			g_Escape.length.x -= ESCAPE_RADIUS_MOVE.x;
			g_Escape.length.y -= ESCAPE_RADIUS_MOVE.y;

			if (g_Escape.length.x <= ESCAPE_RADIUS.x)
			{ // ���̔��a�𒴂����ꍇ

				// ���a��␳����
				g_Escape.length = ESCAPE_RADIUS;

				// ��ԃJ�E���^�[�����Z����
				g_Escape.stateInfo.nStateCounter++;
			}
		}
		else
		{ // �J�E���^�[��0�𒴉߂��Ă����ꍇ

			// ��ԃJ�E���^�[�����Z����
			g_Escape.stateInfo.nStateCounter++;

			if (g_Escape.stateInfo.nStateCounter >= ESCAPE_EMPHASIS_WAIT_CNT)
			{ // �ҋ@�J�E���g�ɒB�����ꍇ

				// �J�E���g������������
				g_Escape.stateInfo.nStateCounter = 0;

				// �g���Ԃɂ���
				g_Escape.stateInfo.emphaState = EMPHASISSTATE_BIGGER;
			}
		}

		break;

	default:						// ��L�ȊO

		break;
	}
}