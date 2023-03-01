//========================================
//
//�v���W�F�N�g��ʂ̃��C������[LogoBy.cpp]
//Author ��������
//
//========================================
#include "main.h"
#include "Logo.h"
#include "input.h"
#include "Fade.h"

//�}�N����`
#define AUTO_LOGO						(90)				//�����ŉ�ʂ��i�ގ���
#define LOGO_X							(450.0f)			//���S�̔��a(X��)
#define LOGO_Y							(250.0f)			//���S�̔��a(Y��)

//�v���W�F�N�g��ʂ̃e�N�X�`��
typedef enum
{
	LOGO_SCREEN = 0,		//�w�i
	LOGO_PROJECT,			//���S
	LOGO_MAX
}LOGO;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureLogo[LOGO_MAX] = {};		//�e�N�X�`��(2����)�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLogo		 = NULL;	//���_�o�b�t�@�ւ̃|�C���^

int g_nLogoCounter = 0;									//�^�C�g���Ɉڍs����Ԃł̃J�E���^�[
D3DXVECTOR3 LogoPos;									// ���S�̒��S

// �e�N�X�`���t�@�C����
const char *c_apFilenameLogo[LOGO_MAX] = 
{
	"data/TEXTURE/Start_Screen.png",
	"data/TEXTURE/logo000.png",
};

//===============================
//���S��ʂ̏���������
//===============================
void InitLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;								//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < LOGO_MAX; nCnt++)
	{
		//�e�N�X�`��1���ڂ̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameLogo[nCnt],
			&g_apTextureLogo[nCnt]);
	}

	//�J�E���^�[������������
	g_nLogoCounter = 0;

	// ���S�}�[�N�̒��S�̐ݒ�
	LogoPos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * LOGO_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLogo,
		NULL);

	VERTEX_2D * pVtx;										//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLogo->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLogo = 0; nCntLogo < LOGO_MAX; nCntLogo++)
	{
		switch (nCntLogo)
		{
		case LOGO_SCREEN:		// ���

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

			break;				// �����o��

		case LOGO_PROJECT:		// ���S�}�[�N

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(LogoPos.x - LOGO_X, LogoPos.y - LOGO_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(LogoPos.x + LOGO_X, LogoPos.y - LOGO_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(LogoPos.x - LOGO_X, LogoPos.y + LOGO_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(LogoPos.x + LOGO_X, LogoPos.y + LOGO_Y, 0.0f);

			break;				// �����o��
		}

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
	g_pVtxBuffLogo->Unlock();
}

//======================================
//���S��ʂ̏I������
//======================================
void UninitLogo(void)
{
	int nLogoCnt;					//�񐔂̕ϐ���錾����

	for (nLogoCnt = 0; nLogoCnt < LOGO_MAX; nLogoCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureLogo[nLogoCnt] != NULL)
		{
			g_apTextureLogo[nLogoCnt]->Release();
			g_apTextureLogo[nLogoCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffLogo != NULL)
	{
		g_pVtxBuffLogo->Release();
		g_pVtxBuffLogo = NULL;
	}
}

//=======================================
//���S��ʂ̍X�V����
//=======================================
void UpdateLogo(void)
{
	FADE pFade = GetFade();							//�t�F�[�h�̏�Ԃ��擾����

	//���U���g��ʂɂȂ����u�ԉ��Z���n�߂�
	g_nLogoCounter++;

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_SPACE) == true
		|| GetJoyKeyTrigger(JOYKEY_A, 0) == true || GetJoyKeyTrigger(JOYKEY_B, 0) == true
		|| GetJoyKeyTrigger(JOYKEY_X, 0) == true || GetJoyKeyTrigger(JOYKEY_Y, 0) == true
		|| g_nLogoCounter >= AUTO_LOGO)
	{//��莞�Ԍo�߂���

		if (pFade == FADE_NONE)
		{//�t�F�[�h�����Ɉڍs���Ă��Ȃ��Ƃ�

			//���[�h�ݒ�(�Q�[����ʂɈڍs)
			SetFade(MODE_TITLE);
		}
	}
}

//=======================================
//���S��ʂ̕`�揈��
//=======================================
void DrawLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nLogoCnt = 0; nLogoCnt < LOGO_MAX; nLogoCnt++)
	{
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0,
			g_pVtxBuffLogo,								//���_�o�b�t�@�ւ̃|�C���^
			0,
			sizeof(VERTEX_2D));							//���_���\���̂̃T�C�Y

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureLogo[nLogoCnt]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
			4 * nLogoCnt,									//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);												//�`�悷��v���~�e�B�u��
	}
}