//======================================================================================================================
//
//	�X�^�[�g���� [start.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "game.h"
#include "start.h"
#include "input.h"
#include "2Deffect.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define NONE_START_TEXTURE				(-1)																// �e�N�X�`��������Ȃ��ԍ�

#define START_PREFACE_FIRST_RADIUS		(D3DXVECTOR3(200.0f, 40.0f, 0.0f))									// �O�u��(�O��)�̔��a
#define START_PREFACE_SECOND_RADIUS		(D3DXVECTOR3(200.0f, 40.0f, 0.0f))									// �O�u��(�㔼)�̔��a
#define START_SIGLIGHT_INIT_RADIUS		(D3DXVECTOR3(0.0f, 60.0f, 0.0f))									// �V�O�i���̌����̑S��
#define START_SIGNAL_RADIUS				(D3DXVECTOR3(250.0f, 80.0f, 0.0f))									// �V�O�i���̔��a
#define START_GO_RADIUS					(D3DXVECTOR3(200.0f, 100.0f, 0.0f))									// Go�̔��a

#define START_PREFACE_FIRST_INIT_POS	(D3DXVECTOR3(-250.0f, 250.0f, 0.0f))													// �O�u��(�O��)�̏����ʒu
#define START_PREFACE_SECOND_INIT_POS	(D3DXVECTOR3(SCREEN_WIDTH + 250.0f, 350.0f, 0.0f))										// �O�u��(�㔼)�̏����ʒu
#define START_SIGLIGHT_INIT_POS			(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - START_SIGNAL_RADIUS.x + 5.0f, -80.0f, 0.0f))				// �V�O�i���̌����̏����ʒu
#define START_SIGNAL_INIT_POS			(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, -80.0f, 0.0f))										// �V�O�i���̏����ʒu
#define START_GO_INIT_POS				(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f))							// Go�̏����ʒu

#define START_PREFACE_FIRST_POS			(D3DXVECTOR3(600.0f, 250.0f, 0.0f))									// �O�u��(�O��)�̈ړ�������̈ʒu
#define START_PREFACE_SECOND_POS		(D3DXVECTOR3(750.0f, 250.0f, 0.0f))									// �O�u��(�㔼)�̈ړ�������̈ʒu
#define START_SIGLIGHT_POS				(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - START_SIGNAL_RADIUS.x, SCREEN_HEIGHT * 0.5f, 0.0f))	// �V�O�i���̌����̈ړ�������̈ʒu
#define START_SIGNAL_POS				(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f))							// �V�O�i���̈ړ�������̈ʒu
#define START_SIGLIGHT_RADIUS			(D3DXVECTOR3(490.0f, 60.0f, 0.0f))									// �V�O�i���̌����̏�����̔��a

#define START_PREFACE_MOVE				(5.5f)			// �O�u���̈ړ���
#define START_PREFACE_ALPHA_MOVE		(0.04f)			// �����x�̈ړ���
#define START_PREFACE_WAIT_COUNT		(90)			// �O�u���̑ҋ@�J�E���g��
#define START_PREFACE_WAIT_COUNT		(90)			// �O�u���̑ҋ@�J�E���g��
#define START_SIGNAL_MOVE				(4.0f)			// �V�O�i���̈ړ���
#define START_SIGNAL_COUNT				(40)			// �V�O�i���̃J�E���g
#define START_SIGNAL_LIGHT_MAX			(3)				// �V�O�i���������
#define START_SIGNAL_NORMAL_COL			(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))									// �ʏ�̃V�O�i���̐F
#define START_SIGNAL_START_COL			(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f))									// �X�^�[�g���̃V�O�i���̐F
#define START_GO_ALPHA_MOVE				(0.01f)			// GO!!�̓����x�̈ړ���
#define START_GO_RADIUS_ADD				(2.0f)			// GO!!�̔��a�̒ǉ���
#define START_SIGNAL_PARTICLE_COL		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))									// �p�[�e�B�N���̐F

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTextureStart[] =			// �e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\start000.png",		// �X�^�[�g�R����(�F) (�ʏ�) �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\start001.png",		// �X�^�[�g�R����(�F) (��) �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\StartSignal.png",	// �V�O�i���̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\start001.png",		// �X�^�[�g�R����(�F) (��) �̃e�N�X�`�����΃p�X
};

//**********************************************************************************************************************
//	�񋓌^��` (TEXTURE_START)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_START_PREFACE_FIRST = 0,		// �O�u���O��
	TEXTURE_START_PREFACE_SECOND,			// �O�u���㔼
	TEXTURE_START_SIGNAL,					// �V�O�i��
	TEXTURE_START_GO,						// Go!!
	TEXTURE_START_MAX,						// ���̗񋓌^�̑���
} TEXTURE_START;

//**********************************************************************************************************************
//	�񋓌^��` (START_POLIGON)
//**********************************************************************************************************************
typedef enum
{
	START_POLIGON_PREFACE_FIRST = 0,		// �O�u���O��
	START_POLIGON_PREFACE_SECOND,			// �O�u���㔼
	START_POLIGON_SIGLIGHT,					// �V�O�i���̌���
	START_POLIGON_SIGNAL,					// �V�O�i��
	START_POLIGON_GO,						// Go!!
	START_POLIGON_MAX,						// ���̗񋓌^�̑���
}START_POLIGON;

//**********************************************************************************************************************
//	�\���̒�`�iStart�j
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;						// �ʒu
	D3DXVECTOR3 move;						// �ړ���
	D3DXVECTOR3 radius;						// ���a
	D3DXCOLOR	col;						// �F
	int			nNumTex;					// �e�N�X�`���̔ԍ�
	bool		bUse;						// �g�p��
}Start;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void StartPrefaceFirst(void);				// �O�u��(�O��)�̏���
void StartPrefaceSecond(void);				// �O�u��(�㔼)�̏���
void StartPrefaceThird(void);				// �O�u��(��ʂ���͂���)�̏���
void StartSignalMove(void);					// �V�O�i���̈ړ�����
void StartSignal(void);						// �V�O�i���̏���
void StartGo(void);							// GO!!�\������

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureStart[TEXTURE_START_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStart = NULL;						// ���_�o�b�t�@�ւ̃|�C���^

Start	   g_aStart[START_POLIGON_MAX];								// ���ꂼ��̉��o�̈ʒu
STARTSTATE g_StartState;											// �X�^�[�g�̏��
int		   g_nActionCount;											// �s���̃J�E���g
int		   g_nSignalCount;											// �V�O�i���̕ς������

//======================================================================================================================
//	�X�^�[�g�̏���������
//======================================================================================================================
void InitStart(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D         *pVtx;					// ���_���ւ̃|�C���^

	for (int nCntStart = 0; nCntStart < TEXTURE_START_MAX; nCntStart++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

	  // �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, apTextureStart[nCntStart], &g_apTextureStart[nCntStart]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_2D) * 4 * START_POLIGON_MAX,		// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffStart,
		NULL
	);

	// �O���[�o���ϐ���������
	for (int nCntPara = 0; nCntPara < START_POLIGON_MAX; nCntPara++)
	{
		switch (nCntPara)
		{
		case START_POLIGON_PREFACE_FIRST:		// �O�u��(�O��)

			g_aStart[nCntPara].pos = START_PREFACE_FIRST_INIT_POS;			// �ʒu

			g_aStart[nCntPara].radius = START_PREFACE_FIRST_RADIUS;			// ���a

			g_aStart[nCntPara].nNumTex = TEXTURE_START_PREFACE_FIRST;		// �e�N�X�`���̔ԍ�

			g_aStart[nCntPara].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F

			break;

		case START_POLIGON_PREFACE_SECOND:		// �O�u��(�㔼)

			g_aStart[nCntPara].pos = START_PREFACE_SECOND_INIT_POS;			// �ʒu

			g_aStart[nCntPara].radius = START_PREFACE_SECOND_RADIUS;		// ���a

			g_aStart[nCntPara].nNumTex = TEXTURE_START_PREFACE_SECOND;		// �e�N�X�`���̔ԍ�

			g_aStart[nCntPara].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F

			break;

		case START_POLIGON_SIGLIGHT:			// �V�O�i���̌���

			g_aStart[nCntPara].pos = START_SIGLIGHT_INIT_POS;				// �ʒu

			g_aStart[nCntPara].radius = START_SIGLIGHT_INIT_RADIUS;			// �S��

			g_aStart[nCntPara].nNumTex = NONE_START_TEXTURE;				// �e�N�X�`���̔ԍ�

			g_aStart[nCntPara].col = START_SIGNAL_NORMAL_COL;				// �F

			break;

		case START_POLIGON_SIGNAL:				// �V�O�i��

			g_aStart[nCntPara].pos = START_SIGNAL_INIT_POS;					// �ʒu

			g_aStart[nCntPara].radius = START_SIGNAL_RADIUS;				// ���a

			g_aStart[nCntPara].nNumTex = TEXTURE_START_SIGNAL;				// �e�N�X�`���̔ԍ�

			g_aStart[nCntPara].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F

			break;

		case START_POLIGON_GO:					// Go!!

			g_aStart[nCntPara].pos = START_GO_INIT_POS;						// �ʒu

			g_aStart[nCntPara].radius = START_GO_RADIUS;					// ���a

			g_aStart[nCntPara].nNumTex = TEXTURE_START_GO;					// �e�N�X�`���̔ԍ�
																			
			g_aStart[nCntPara].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F

			break;

		default:								// ��L�ȊO

			break;
		}

		// �g�p��
		g_aStart[nCntPara].bUse = false;

		// �ړ��ʂ�����������
		g_aStart[nCntPara].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// �X�^�[�g�̏�Ԃ�ݒ肷��
	g_StartState = STARTSTATE_PREFACE_FIRST;

	// �s���J�E���g������������
	g_nActionCount = 0;

	// �V�O�i���̕ς�����񐔂�����������
	g_nSignalCount = 0;

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStart = 0; nCntStart < START_POLIGON_MAX; nCntStart++)
	{
		switch (nCntStart)
		{
		case START_POLIGON_SIGLIGHT:			// �V�O�i���̌���

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x, g_aStart[nCntStart].pos.y - g_aStart[nCntStart].radius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x + g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y - g_aStart[nCntStart].radius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x, g_aStart[nCntStart].pos.y + g_aStart[nCntStart].radius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x + g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y + g_aStart[nCntStart].radius.y, 0.0f);

			break;

		default:								// ��L�ȊO

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x - g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y - g_aStart[nCntStart].radius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x + g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y - g_aStart[nCntStart].radius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x - g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y + g_aStart[nCntStart].radius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x + g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y + g_aStart[nCntStart].radius.y, 0.0f);

			break;
		}

		// rhw �̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = g_aStart[nCntStart].col;
		pVtx[1].col = g_aStart[nCntStart].col;
		pVtx[2].col = g_aStart[nCntStart].col;
		pVtx[3].col = g_aStart[nCntStart].col;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;				// ���_�f�[�^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffStart->Unlock();
}

//======================================================================================================================
//	�X�^�[�g�̏I������
//======================================================================================================================
void UninitStart(void)
{
	// �e�N�X�`���̔j��
	for (int nCntStart = 0; nCntStart < TEXTURE_START_MAX; nCntStart++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTextureStart[nCntStart] != NULL)
		{ // �ϐ� (g_apTextureStart) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureStart[nCntStart]->Release();
			g_apTextureStart[nCntStart] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffStart != NULL)
	{ // �ϐ� (g_pVtxBuffStart) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffStart->Release();
		g_pVtxBuffStart = NULL;
	}
}

//======================================================================================================================
//	�X�^�[�g�̍X�V����
//======================================================================================================================
void UpdateStart(void)
{
	if (GetKeyboardTrigger(DIK_0) == true)
	{
		// 0���������ꍇ
		g_StartState = STARTSTATE_PREFACE_FIRST;
	}

	VERTEX_2D         *pVtx;					// ���_���ւ̃|�C���^

	switch (g_StartState)
	{
	case STARTSTATE_NONE:		// �����

		for (int nCntNone = 0; nCntNone < START_POLIGON_MAX; nCntNone++)
		{
			// �|���S���S�Ă��g�p���Ȃ�
			g_aStart[nCntNone].bUse = false;
		}

		break;

	case STARTSTATE_PREFACE_FIRST:	// �O�u���O��

		// �g�p����
		g_aStart[START_POLIGON_PREFACE_FIRST].bUse = true;

		// �O�u��(�O��)�̏���
		StartPrefaceFirst();

		break;

	case STARTSTATE_PREFACE_SECOND:	// �O�u���㔼

		// �O�u��(�㔼)�̏���
		StartPrefaceSecond();

		break;

	case STARTSTATE_PREFACE_THIRD:	// �O�u���ŏI(��ʂ���͂���)

		// �O�u��(��ʂ���͂���)�̏���
		StartPrefaceThird();

		break;

	case STARTSTATE_SIGNAL_MOVE:	// �V�O�i���̈ړ����

		// �V�O�i���̈ړ�����
		StartSignalMove();

		break;

	case STARTSTATE_SIGNAL:			// �V�O�i�����

		// �V�O�i���̏���
		StartSignal();

		break;

	case STARTSTATE_GO:				// Go!!���

		// GO!!�\������
		StartGo();

		break;
	}

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStart = 0; nCntStart < START_POLIGON_MAX; nCntStart++)
	{
		if (g_aStart[nCntStart].bUse == true)
		{ // �g�p���Ă���ꍇ

			switch (nCntStart)
			{
			case START_POLIGON_SIGLIGHT:			// �V�O�i���̌���

				// ���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x, g_aStart[nCntStart].pos.y - g_aStart[nCntStart].radius.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x + g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y - g_aStart[nCntStart].radius.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x, g_aStart[nCntStart].pos.y + g_aStart[nCntStart].radius.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x + g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y + g_aStart[nCntStart].radius.y, 0.0f);

				break;

			default:								// ��L�ȊO

				// ���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x - g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y - g_aStart[nCntStart].radius.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x + g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y - g_aStart[nCntStart].radius.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x - g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y + g_aStart[nCntStart].radius.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aStart[nCntStart].pos.x + g_aStart[nCntStart].radius.x, g_aStart[nCntStart].pos.y + g_aStart[nCntStart].radius.y, 0.0f);

				break;
			}

			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aStart[nCntStart].col;
			pVtx[1].col = g_aStart[nCntStart].col;
			pVtx[2].col = g_aStart[nCntStart].col;
			pVtx[3].col = g_aStart[nCntStart].col;
		}

		pVtx += 4;			// ���_�f�[�^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffStart->Unlock();
}

//======================================================================================================================
//	�X�^�[�g�̕`�揈��
//======================================================================================================================
void DrawStart(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffStart, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntDraw = 0; nCntDraw < START_POLIGON_MAX; nCntDraw++)
	{
		if (g_aStart[nCntDraw].bUse == true)
		{ // �g�p���Ă���ꍇ

			if (g_aStart[nCntDraw].nNumTex == NONE_START_TEXTURE)
			{ // �e�N�X�`���ԍ����Ȃ��ꍇ

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, NULL);
			}
			else
			{ // �e�N�X�`���ԍ�������ꍇ

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_apTextureStart[g_aStart[nCntDraw].nNumTex]);
			}

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntDraw, 2);
		}
	}
}

//======================================================================================================================
// �O�u��(�O��)�̏���
//======================================================================================================================
void StartPrefaceFirst(void)
{
	// �ړ��ʂ��X�V����
	g_aStart[START_POLIGON_PREFACE_FIRST].move.x += START_PREFACE_MOVE;
	
	// �ʒu���X�V����
	g_aStart[START_POLIGON_PREFACE_FIRST].pos.x += g_aStart[START_POLIGON_PREFACE_FIRST].move.x;

	if (g_aStart[START_POLIGON_PREFACE_FIRST].pos.x >= START_PREFACE_FIRST_POS.x)
	{ // �O�u��(�O��)�����̈ʒu�܂ňړ������ꍇ

		// �O�u���̈ʒu��␳����
		g_aStart[START_POLIGON_PREFACE_FIRST].pos.x = START_PREFACE_FIRST_POS.x;

		// �O�u��(�㔼)���g�p�\�ɂ���
		g_aStart[START_POLIGON_PREFACE_SECOND].bUse = true;

		// �O�u��(�㔼)��Ԃɂ���
		g_StartState = STARTSTATE_PREFACE_SECOND;
	}
}

//======================================================================================================================
// �O�u��(�㔼)�̏���
//======================================================================================================================
void StartPrefaceSecond(void)
{
	// �ړ��ʂ��X�V����
	g_aStart[START_POLIGON_PREFACE_SECOND].move.x -= START_PREFACE_MOVE;

	// �ʒu���X�V����
	g_aStart[START_POLIGON_PREFACE_SECOND].pos.x += g_aStart[START_POLIGON_PREFACE_SECOND].move.x;

	if (g_aStart[START_POLIGON_PREFACE_SECOND].pos.x <= START_PREFACE_SECOND_POS.x)
	{ // �O�u��(�㔼)�����̈ʒu�̈ʒu�܂ňړ������ꍇ

		// �O�u���̈ʒu��␳����
		g_aStart[START_POLIGON_PREFACE_SECOND].pos.x = START_PREFACE_SECOND_POS.x;

		// ��Ԃ���ʂ���͂����Ԃɂ���
		g_StartState = STARTSTATE_PREFACE_THIRD;

		// �s���J�E���g������������
		g_nActionCount = 0;
	}
}

//======================================================================================================================
// �O�u��(��ʂ���͂���)�̏���
//======================================================================================================================
void StartPrefaceThird(void)
{
	if (g_nActionCount >= START_PREFACE_WAIT_COUNT)
	{ // �ҋ@�J�E���g�ɂȂ����ꍇ

		// �����x��������
		g_aStart[START_POLIGON_PREFACE_FIRST].col.a -= START_PREFACE_ALPHA_MOVE;
		g_aStart[START_POLIGON_PREFACE_SECOND].col.a -= START_PREFACE_ALPHA_MOVE;

		if (g_aStart[START_POLIGON_PREFACE_FIRST].col.a <= 0.0f)
		{ // �����x��0.0f�ȉ��ɂȂ�����

			// �����x��0.0f�ɂ���
			g_aStart[START_POLIGON_PREFACE_FIRST].col.a = 0.0f;
			g_aStart[START_POLIGON_PREFACE_SECOND].col.a = 0.0f;

			// �g�p���Ȃ�
			g_aStart[START_POLIGON_PREFACE_FIRST].bUse = false;
			g_aStart[START_POLIGON_PREFACE_SECOND].bUse = false;

			// ��Ԃ��V�O�i���̈ړ���Ԃɂ���
			g_StartState = STARTSTATE_SIGNAL_MOVE;

			// �V�O�i���n���g�p����
			g_aStart[START_POLIGON_SIGLIGHT].bUse = true;
			g_aStart[START_POLIGON_SIGNAL].bUse = true;

			// �s���J�E���g������������
			g_nActionCount = 0;
		}
	}
	else
	{ // �ҋ@�J�E���g�ȓ��������ꍇ

		// �s���J�E���g�����Z����
		g_nActionCount++;
	}
}

//======================================================================================================================
// �V�O�i���̏���
//======================================================================================================================
void StartSignalMove(void)
{
	// �ړ��ʂ��X�V����
	g_aStart[START_POLIGON_SIGLIGHT].move.y += START_SIGNAL_MOVE;
	g_aStart[START_POLIGON_SIGNAL].move.y += START_SIGNAL_MOVE;

	// �ʒu���X�V����
	g_aStart[START_POLIGON_SIGLIGHT].pos.y += g_aStart[START_POLIGON_SIGLIGHT].move.y;
	g_aStart[START_POLIGON_SIGNAL].pos.y += g_aStart[START_POLIGON_SIGNAL].move.y;

	if (g_aStart[START_POLIGON_SIGLIGHT].pos.y >= START_SIGLIGHT_POS.y
	 || g_aStart[START_POLIGON_SIGNAL].pos.y >= START_SIGNAL_POS.y)
	{ // ���̈ʒu�ȏ�ɂȂ����ꍇ

		// �ʒu��␳����
		g_aStart[START_POLIGON_SIGLIGHT].pos.y = START_SIGLIGHT_POS.y;
		g_aStart[START_POLIGON_SIGNAL].pos.y = START_SIGNAL_POS.y;

		// �ړ��ʂ�����������
		g_aStart[START_POLIGON_SIGLIGHT].move.y = 0.0f;
		g_aStart[START_POLIGON_SIGNAL].move.y = 0.0f;

		// ��Ԃ��V�O�i���̏�Ԃɂ���
		g_StartState = STARTSTATE_SIGNAL;
	}
}

//======================================================================================================================
// �V�O�i���̏���
//======================================================================================================================
void StartSignal(void)
{
	// �s���J�E���g�����Z����
	g_nActionCount++;

	if (g_nActionCount % START_SIGNAL_COUNT == 0)
	{ // ��萔�o�����ꍇ

		if (g_nSignalCount < START_SIGNAL_LIGHT_MAX)
		{ // �V�O�i�����������񐔂���萔�ȓ��������ꍇ

			// �V�O�i���J�E���g�����Z����
			g_nSignalCount++;

			//�@���a��ݒ肷��
			g_aStart[START_POLIGON_SIGLIGHT].radius.x = (START_SIGLIGHT_RADIUS.x / START_SIGNAL_LIGHT_MAX) * g_nSignalCount;
		}
		else
		{ // ��L�ȊO

			// �F��ݒ肷��
			g_aStart[START_POLIGON_SIGLIGHT].col = START_SIGNAL_START_COL;

			// ��Ԃ�Go!!��Ԃɂ���
			g_StartState = STARTSTATE_GO;

			// �g�p����
			g_aStart[START_POLIGON_GO].bUse = true;

			// �Q�[���̏�Ԃ�ʏ�ɂ���
			*GetGameState() = GAMESTATE_NORMAL;

			// �s���J�E���g������������
			g_nActionCount = 0;
		}
	}
}

//======================================================================================================================
// GO!!�\������
//======================================================================================================================
void StartGo(void)
{
	// �����x���X�V����
	g_aStart[START_POLIGON_GO].col.a -= START_GO_ALPHA_MOVE;

	// ���a��ǉ�����
	g_aStart[START_POLIGON_GO].radius.x += START_GO_RADIUS_ADD;
	g_aStart[START_POLIGON_GO].radius.y += START_GO_RADIUS_ADD;

	// �s���J�E���g��ݒ肷��
	g_nActionCount++;

	if (g_nActionCount >= 60)
	{ // �s���J�E���g�����ȏゾ�����ꍇ

		// �V�O�i���̈ړ��ʂ𑝂₷
		g_aStart[START_POLIGON_SIGLIGHT].move.y -= START_SIGNAL_MOVE;
		g_aStart[START_POLIGON_SIGNAL].move.y -= START_SIGNAL_MOVE;

		// �V�O�i�����ړ�������
		g_aStart[START_POLIGON_SIGLIGHT].pos.y += g_aStart[START_POLIGON_SIGLIGHT].move.y;
		g_aStart[START_POLIGON_SIGNAL].pos.y += g_aStart[START_POLIGON_SIGLIGHT].move.y;
	}

	if (g_aStart[START_POLIGON_GO].col.a <= 0.0f)
	{ // Go!!�̓����x��0.0f�����ɂȂ����ꍇ

		// �����x��␳����
		g_aStart[START_POLIGON_SIGNAL].col.a = 0.0f;
		g_aStart[START_POLIGON_SIGLIGHT].col.a = 0.0f;
		g_aStart[START_POLIGON_GO].col.a = 0.0f;

		// ����Ԃɂ���
		g_StartState = STARTSTATE_NONE;
	}
}

//======================================================================================================================
//	�X�^�[�g�̏�Ԃ̎擾����
//======================================================================================================================
STARTSTATE GetStartState(void)
{
	// �X�^�[�g�̏�Ԃ�Ԃ�
	return g_StartState;
}