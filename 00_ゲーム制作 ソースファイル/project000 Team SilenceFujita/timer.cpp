//======================================================================================================================
//
//	�^�C�}�[���� [timer.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "fade.h"
#include "game.h"
#include "gate.h"
#include "timer.h"
#include "value.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAX_TIMER		(1)			// �g�p����|���S����
#define MAX_VAL_TIMER	(2)			// �^�C�}�[�̐��l�\���̍ő吔
#define MAX_MIN			(60)		// �^�C�� (��) �̍ő�l
#define MAX_SEC			(60)		// �^�C�� (�b) �̍ő�l
#define NUM_RED_TIMER	(10)		// �^�C�}�[���Ԃ��Ȃ�n�߂鎞��

#define VAL_TIME_WIDTH	(28.0f)		// �^�C�}�[�̐��l�̉��� / 2
#define VAL_TIME_HEIGHT	(28.0f)		// �^�C�}�[�̐��l�̏c�� / 2
#define VAL_TIME_SPACE	(38.0f)		// �^�C�}�[�̐��l�Ԃ̕� (x)

#define TIMER_POS_MIN_X	(1085.0f)	// �^�C�}�[ (��) �̐�΍��W (x)
#define TIMER_POS_SEC_X	(1185.0f)	// �^�C�}�[ (�b) �̐�΍��W (x)
#define TIMER_POS_X		(1115.0f)	// �^�C�}�[ (�w�i) �̐�΍��W (x)
#define TIMER_POS_Y		(320.0f)	// �^�C�}�[�̐�΍��W (y)
#define TIMER_WIDTH		(32.0f)		// �^�C�}�[�̉��� / 2
#define TIMER_HEIGHT	(32.0f)		// �^�C�}�[�̏c�� / 2

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTextureTimer[] =		// �e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\timer000.png",	// �^�C�}�[�R����(�F) (�ʏ�) �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\timer001.png",	// �^�C�}�[�R����(�F) (��) �̃e�N�X�`�����΃p�X
};

//**********************************************************************************************************************
//	�񋓌^��` (TEXTURE_TIMER)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_TIMER_NORMAL = 0,		// �^�C�}�[�R����(�F) (�ʏ�)
	TEXTURE_TIMER_RED,				// �^�C�}�[�R����(�F) (��)
	TEXTURE_TIMER_MAX,				// ���̗񋓌^�̑���
} TEXTURE_TIMER;

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureTimer[TEXTURE_TIMER_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;						// ���_�o�b�t�@�ւ̃|�C���^

TIMERSTATE g_timerState;			// �^�C�}�[�̏��
int        g_nTime;					// ���݂̃^�C��

//======================================================================================================================
//	�^�C�}�[�̏���������
//======================================================================================================================
void InitTimer(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D         *pVtx;					// ���_���ւ̃|�C���^

	for (int nCntTimer = 0; nCntTimer < TEXTURE_TIMER_MAX; nCntTimer++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, apTextureTimer[nCntTimer], &g_apTextureTimer[nCntTimer]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_2D) * 4 * MAX_TIMER,		// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL
	);

	// �O���[�o���ϐ���������
	g_timerState = TIMERSTATE_COUNT;	// �^�C�}�[�̏��
	g_nTime      = NUM_TIME_GAME;		// ���݂̃^�C��

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(TIMER_POS_X - TIMER_WIDTH, TIMER_POS_Y - TIMER_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TIMER_POS_X + TIMER_WIDTH, TIMER_POS_Y - TIMER_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TIMER_POS_X - TIMER_WIDTH, TIMER_POS_Y + TIMER_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TIMER_POS_X + TIMER_WIDTH, TIMER_POS_Y + TIMER_HEIGHT, 0.0f);

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

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTimer->Unlock();
}

//======================================================================================================================
//	�^�C�}�[�̏I������
//======================================================================================================================
void UninitTimer(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTimer = 0; nCntTimer < TEXTURE_TIMER_MAX; nCntTimer++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTextureTimer[nCntTimer] != NULL)
		{ // �ϐ� (g_apTextureTimer) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureTimer[nCntTimer]->Release();
			g_apTextureTimer[nCntTimer] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTimer != NULL)
	{ // �ϐ� (g_pVtxBuffTimer) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}
}

//======================================================================================================================
//	�^�C�}�[�̍X�V����
//======================================================================================================================
void UpdateTimer(void)
{
	switch (g_timerState)
	{ // ��Ԃ��Ƃ̏���
	case TIMERSTATE_COUNT:	// �J�E���g��Ԃ̏ꍇ

		if (GetGameState() == GAMESTATE_NORMAL
		&&  GetFade()      == FADE_NONE)
		{ // �Q�[�����ʏ��Ԋ��A�t�F�[�h���Ă��Ȃ��ꍇ

			if (g_nTime > 0)
			{ // �^�C�}�[�� 0���傫���ꍇ

				// �^�C�}�[�����Z
				g_nTime--;
			}
			else
			{ // �J�E���^�[�� 0�ȉ��̏ꍇ

				// �Q�[�g�̑S�ߏ���
				AllShutOutGate();

				// �J�E���g�I����Ԃɂ���
				g_timerState = TIMERSTATE_END;
			}
		}

		// �����𔲂���
		break;

	case TIMERSTATE_END:	// �J�E���g�I����Ԃ̏ꍇ

		// ����

		// �����𔲂���
		break;
	}
}

//======================================================================================================================
//	�^�C�}�[�̕`�揈��
//======================================================================================================================
void DrawTimer(void)
{
	// �ϐ���錾
	TEXTURE_TIMER texBG;					// �^�C�}�[ (�w�i) �̃e�N�X�`��
	VALUETYPE     texValue;					// �^�C�}�[ (�l) �̃e�N�X�`��

	int nMin =  g_nTime / 3600;				// �^�C�}�[ (��)
	int nSec = (g_nTime % 3600) / 60;		// �^�C�}�[ (�b)

	if (GetTime() > NUM_RED_TIMER)
	{ // �^�C�}�[�� 10�b���傫���ꍇ

		texBG    = TEXTURE_TIMER_NORMAL;	// �ʏ�̃e�N�X�`����ݒ�
		texValue = VALUETYPE_NORMAL;		// �ʏ�̃e�N�X�`����ݒ�
	}
	else
	{ // �^�C�}�[�� 10�b�ȉ��̏ꍇ

		texBG    = TEXTURE_TIMER_RED;		// �Ԃ̃e�N�X�`����ݒ�
		texValue = VALUETYPE_RED;			// �Ԃ̃e�N�X�`����ݒ�
	}

	// �ϐ��z���錾
	D3DXVECTOR3 aPosValue[MAX_VAL_TIMER] =					// �^�C�}�[�̈ʒu�p
	{ // �����l
		D3DXVECTOR3(TIMER_POS_MIN_X, TIMER_POS_Y, 0.0f),	// �^�C�}�[ (��) �̈ʒu
		D3DXVECTOR3(TIMER_POS_SEC_X, TIMER_POS_Y, 0.0f)		// �^�C�}�[ (�b) �̈ʒu
	};

	int aValue[MAX_VAL_TIMER]    = { nMin,    nSec    };	// �^�C�}�[�\���p
	int aMaxValue[MAX_VAL_TIMER] = { MAX_MIN, MAX_SEC };	// �^�C�}�[�\���̒l�̐����p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTimer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//------------------------------------------------------------------------------------------------------------------
	//	�w�i�̕`��
	//------------------------------------------------------------------------------------------------------------------
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_apTextureTimer[texBG]);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//------------------------------------------------------------------------------------------------------------------
	//	���l�̕`��
	//------------------------------------------------------------------------------------------------------------------
	for (int nCntTimer = 0; nCntTimer < MAX_VAL_TIMER; nCntTimer++)
	{ // �^�C�}�[�̐��l�\���̍ő吔���J��Ԃ�

		// ���l�̐ݒ�
		SetValue
		( // ����
			aPosValue[nCntTimer],	// �ʒu
			aValue[nCntTimer],		// �l
			aMaxValue[nCntTimer],	// �l�̍ő�l
			VAL_TIME_WIDTH,			// ����
			VAL_TIME_HEIGHT,		// �c��
			VAL_TIME_SPACE,			// ���l�Ԃ̕�
			1.0f					// ���l
		);

		// ���l�̕`��
		DrawValue(VAL_TIME_DIGIT, texValue);
	}
}

//======================================================================================================================
//	�^�C���̎擾����
//======================================================================================================================
int GetTime(void)
{
	// �^�C���̒l (�b���Z) ��Ԃ�
	return g_nTime / 60;
}

//======================================================================================================================
//	�^�C�}�[�̏�Ԃ̎擾����
//======================================================================================================================
TIMERSTATE GetTimerState(void)
{
	// �^�C�}�[�̏�Ԃ�Ԃ�
	return g_timerState;
}