//======================================================================================================================
//
//	�|�[�Y��ʏ��� [pause.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

#include "pause.h"

#include "game.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAX_PAUSE		(4)			// �g�p����|���S����

#define PAUSE_POS_X		(640.0f)	// �Z���N�g���j���[�̐�΍��W (x)
#define PAUSE_POS_Y		(210.0f)	// �Z���N�g���j���[�̐�΍��W (y)

#define PAUSE_WIDTH		(220.0f)	// �Z���N�g���j���[�̕� / 2 (��)
#define PAUSE_HEIGHT	(55.0f)		// �Z���N�g���j���[�̕� / 2 (����)
#define PAUSE_SPACE		(150.0f)	// �Z���N�g���j���[�Ԃ̕�

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTexturePause[] =		// �e�N�X�`���̑��΃p�X
{
	NULL,							// NULL
	"data\\TEXTURE\\pause000.png",	// RESUME �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\pause001.png",	// RETRY �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\pause002.png",	// EXIT �̃e�N�X�`�����΃p�X
};

//**********************************************************************************************************************
//	�񋓌^��` (TEXTURE_PAUSE)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_PAUSE_NULL_00 = 0,		// NULL
	TEXTURE_PAUSE_RESUME,			// RESUME
	TEXTURE_PAUSE_RETRY,			// RETRY
	TEXTURE_PAUSE_EXIT,				// EXIT
	TEXTURE_PAUSE_MAX,				// ���̗񋓌^�̑���
} TEXTURE_PAUSE;

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTexturePause[TEXTURE_PAUSE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;						// ���_�o�b�t�@�ւ̃|�C���^

int  g_nSelectPause;				// ���݂̑I�𒆃��j���[
int  g_nOldSelectPause;				// �O��̑I�𒆃��j���[
bool g_bPauseEnd;					// �|�[�Y�I���m�F�p

//======================================================================================================================
//	�|�[�Y��ʂ̏���������
//======================================================================================================================
void InitPause(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�ւ̃|�C���^ (�|�C���^�̐錾���� * �͂���Ȃ�)
	VERTEX_2D *pVtx;				// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntPause = 0; nCntPause < TEXTURE_PAUSE_MAX; nCntPause++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, apTexturePause[nCntPause], &g_apTexturePause[nCntPause]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_2D) * 4 * MAX_PAUSE,		// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL
	);

	// �ϐ��̒l��������
	g_nSelectPause    = PAUSESELECT_RESUME;		// ���݂̑I�𒆃��j���[��������
	g_nOldSelectPause = PAUSESELECT_RESUME;		// �O��̑I�𒆃��j���[��������

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------------------------------------------------------------------------------------------
	//	�w�i (�t�F�[�h) �̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f,         0.0f,          0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f,          0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f,         SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhw �̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);

	//------------------------------------------------------------------------------------------------------------------
	//	�Z���N�g���j���[�̏�����
	//------------------------------------------------------------------------------------------------------------------
	for (int nCntPause = 0; nCntPause < PAUSESELECT_MAX; nCntPause++)
	{ // �|�[�Y�̍��ڐ����J��Ԃ�

		// ���_���W��ݒ�
		pVtx[4].pos = D3DXVECTOR3(PAUSE_POS_X - PAUSE_WIDTH, PAUSE_POS_Y + (nCntPause * PAUSE_SPACE) - PAUSE_HEIGHT, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(PAUSE_POS_X + PAUSE_WIDTH, PAUSE_POS_Y + (nCntPause * PAUSE_SPACE) - PAUSE_HEIGHT, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(PAUSE_POS_X - PAUSE_WIDTH, PAUSE_POS_Y + (nCntPause * PAUSE_SPACE) + PAUSE_HEIGHT, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(PAUSE_POS_X + PAUSE_WIDTH, PAUSE_POS_Y + (nCntPause * PAUSE_SPACE) + PAUSE_HEIGHT, 0.0f);

		// rhw �̐ݒ�
		pVtx[4].rhw = 1.0f;
		pVtx[5].rhw = 1.0f;
		pVtx[6].rhw = 1.0f;
		pVtx[7].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[4].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[5].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[6].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[7].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();
}

//======================================================================================================================
//	�|�[�Y��ʂ̏I������
//======================================================================================================================
void UninitPause(void)
{
	// �e�N�X�`���̔j��
	for (int nCntPause = 0; nCntPause < TEXTURE_PAUSE_MAX; nCntPause++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTexturePause[nCntPause] != NULL)
		{ // �ϐ� (g_apTexturePause) ��NULL�ł͂Ȃ��ꍇ

			g_apTexturePause[nCntPause]->Release();
			g_apTexturePause[nCntPause] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffPause != NULL)
	{ // �ϐ� (g_pVtxBuffPause) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//======================================================================================================================
//	�|�[�Y��ʂ̍X�V����
//======================================================================================================================
void UpdatePause(void)
{
	// �|�C���^��錾
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	//------------------------------------------------------------------------------------------------------------------
	//	����̍X�V
	//------------------------------------------------------------------------------------------------------------------
	if (GetFade() == FADE_NONE)
	{ // �t�F�[�h���Ă��Ȃ��ꍇ

		if (GetKeyboardTrigger(DIK_W) == true || GetKeyboardTrigger(DIK_UP) == true || GetJoyKeyTrigger(JOYKEY_UP, 0) == true)
		{ // ��ړ��̑��삪�s��ꂽ�ꍇ

			// �ϐ� (g_nSelectPause) �� 1�������Ă��� ( 0�̏ꍇ 1�ɖ߂�)
			g_nSelectPause = (g_nSelectPause + (PAUSESELECT_MAX - 1)) % PAUSESELECT_MAX;

			// �T�E���h�̍Đ�
			//PlaySound(SOUND_LABEL_SE_MOVE);			// �ړ��� (�V�X�e��)
		}
		else if (GetKeyboardTrigger(DIK_S) == true || GetKeyboardTrigger(DIK_DOWN) == true || GetJoyKeyTrigger(JOYKEY_DOWN, 0) == true)
		{ // ���ړ��̑��삪�s��ꂽ�ꍇ

			// �ϐ� (g_nSelectPause) �� 1�������Ă��� ( 1�̏ꍇ 0�ɖ߂�)
			g_nSelectPause = (g_nSelectPause + 1) % PAUSESELECT_MAX;

			// �T�E���h�̍Đ�
			//PlaySound(SOUND_LABEL_SE_MOVE);			// �ړ��� (�V�X�e��)
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_SPACE) == true
		||  GetJoyKeyTrigger(JOYKEY_A, 0)  == true || GetJoyKeyTrigger(JOYKEY_B, 0) == true
		||  GetJoyKeyTrigger(JOYKEY_X, 0)  == true || GetJoyKeyTrigger(JOYKEY_Y, 0) == true)
		{ // ����̑��삪�s��ꂽ�ꍇ

			// �T�E���h�̍Đ�
			//PlaySound(SOUND_LABEL_SE_DECISION_01);	// ���艹01 (�V�X�e��)

			switch (g_nSelectPause)
			{ // ��ʂ̈ڍs��̊m�F
			case PAUSESELECT_RESUME:	// �ĊJ�̏ꍇ

				// �|�[�Y���I��������Ԃɂ���
				g_bPauseEnd = true;

				// �|�[�Y��Ԃ̐ݒ�
				SetEnablePause(false);

				// �����𔲂���
				break;

			case PAUSESELECT_RETRY:		// ���g���C�̏ꍇ

				// ���[�h�I�� (�Q�[����ʂɈڍs)
				SetFade(MODE_GAME);

				// �����𔲂���
				break;

			case PAUSESELECT_EXIT:		// �I���̏ꍇ

				// ���[�h�I�� (�^�C�g����ʂɈڍs)
				SetFade(MODE_TITLE);

				// �����𔲂���
				break;
			}
		}
	}

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̍X�V
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�f�[�^�̃|�C���^��O�t���[���ɃZ���N�g���Ă����v�f�̒��_�Ɉړ�
	pVtx += (g_nOldSelectPause * 4);

	// ���_�J���[�̍X�V
	pVtx[4].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	pVtx[5].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	pVtx[6].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	pVtx[7].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

	// ���_�f�[�^�̃|�C���^��擪�A�h���X�ɖ߂�
	pVtx -= (g_nOldSelectPause * 4);

	// ���_�f�[�^�̃|�C���^�����t���[���ŃZ���N�g���Ă���v�f�̒��_�Ɉړ�
	pVtx += (g_nSelectPause * 4);

	// ���_�J���[�̍X�V
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���݃Z���N�g���̗v�f����
	g_nOldSelectPause = g_nSelectPause;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();
}

//======================================================================================================================
//	�|�[�Y��ʂ̕`�揈��
//======================================================================================================================
void DrawPause(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^ (�|�C���^�̐錾���� * �͂���Ȃ�)

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	{ // �g�p����|���S�������J��Ԃ�

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTexturePause[nCntPause]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);
	}
}

//======================================================================================================================
//	�|�[�Y�̐^�U�̐ݒ菈��
//======================================================================================================================
void SetEnablePauseEnd(bool bPauseEnd)
{
	// ���݂̃|�[�Y�I����ԂɈ����̃|�[�Y�I����Ԃ���
	g_bPauseEnd = bPauseEnd;
}

//======================================================================================================================
//	�|�[�Y�̏�Ԃ̎擾����
//======================================================================================================================
bool GetPauseEnd(void)
{
	// �|�[�Y�I����Ԃ̐^�U��Ԃ�
	return g_bPauseEnd;
}

//======================================================================================================================
//	�|�[�Y�̑I�����ڂ̍Đݒ菈��
//======================================================================================================================
void ResetPauseSelect(void)
{
	// �|�[�Y�̑I�����ڂ�������
	g_nSelectPause = PAUSESELECT_RESUME;
}