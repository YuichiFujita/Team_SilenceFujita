
//=====================================================================
//=
//= �����L���O����[ranking.cpp]
//= Author Sakai Minato
//=
//=====================================================================

#include <stdio.h>

#include "main.h"
#include "ranking.h"

#include "Input.h"
#include "sound.h"
#include "fade.h"
#include "value.h"

#include "title.h"

#include "calculation.h"
#include "camera.h"
#include "Human.h"
#include "icon.h"
#include "light.h"
#include "score.h"
#include "weather.h"
#include "shadow.h"
#include "Car.h"
#include "Police.h"

//**********************************************
//* �}�N��
//**********************************************
#define MAX_RANK			(7)							// �g�p����|���S����

//**********************************
//* �����L���O�̊O���t�@�C���֘A
//**********************************
#define RANK_FILE_MODE		(FILE_MODE_BIN)				//�����L���O�t�@�C���̓��o�̓��[�h
#define RANK_FILE_TXT		("data\\TXT\\rank.txt")		//�����L���O�̃X�R�A�̃e�L�X�g�t�@�C��
#define RANK_FILE_BIN		("data\\BIN\\rank.bin")		//�����L���O�̃X�R�A�̃o�C�i���t�@�C��

//**********************************
//* �����L���O�X�R�A�̐��l�֌W
//**********************************
#define RANK_SCORE_MAX		(5)							//�X�R�A�̐�
#define	RANK_NUM_PLACE		(9)							//�X�R�A�̌���
#define RANK_COLUMN			(1)							//�X�R�A�𕪂����
#define RANK_POS_X			(SCREEN_WIDTH * 0.68f)		//�X�R�A�̊J�n�ʒu�iX)
#define RANK_POS_Y			(SCREEN_HEIGHT * 0.23f)		//�X�R�A�̊J�n�ʒu�iY�j	
#define RANK_WIDTH			(40.0f)						//�X�R�A�̑傫���iX�j
#define RANK_HEIGHT			(30.0f)						//�X�R�A�̑傫���iY�j
#define RANK_INTERVAL_X		(60.0f)						//�X�R�A�̊Ԋu�iX�j
#define RANK_INTERVAL_Y		(90.0f)						//�X�R�A�̊Ԋu�iY�j
#define RANK_MAX_NUMBER		(999999999)					//�X�R�A�̍ő吔

#define RANK_FLASH_MAX		(1.0f)						//�_�ł̍ő�l
#define RANK_FLASH_MIN		(0.3f)						//�_�ł̍ŏ��l
#define RANK_FLASH_INTERVAL	(0.02f)						//�_�ł̊Ԋu

//**********************************
//* �����L���O�j���[�X�R�A�̐��l�֌W
//**********************************
#define	RANK_NEW_NUM_PLACE	(9)							//�j���[�X�R�A�̌���
#define RANK_NEW_COLUMN		(1)							//�j���[�X�R�A�𕪂����
#define RANK_NEW_POS_X		(SCREEN_WIDTH * 0.70f)		//�j���[�X�R�A�̊J�n�ʒu�iX)
#define RANK_NEW_POS_Y		(SCREEN_HEIGHT * 0.88f)		//�j���[�X�R�A�̊J�n�ʒu�iY�j	
#define RANK_NEW_WIDTH		(45.0f)						//�j���[�X�R�A�̑傫���iX�j
#define RANK_NEW_HEIGHT		(35.0f)						//�j���[�X�R�A�̑傫���iY�j
#define RANK_NEW_INTERVAL_X	(65.0f)						//�j���[�X�R�A�̊Ԋu�iX�j
#define RANK_NEW_INTERVAL_Y	(0.0f)						//�j���[�X�R�A�̊Ԋu�iY�j
#define RANK_NEW_MAX_NUMBER	(999999999)					//�j���[�X�R�A�̍ő吔

//**********************************
//* �����L���O�i�����j��2D�|���S���֌W
//**********************************
#define RANK_CHAR_POS_X		(SCREEN_WIDTH * 0.5f)		//�����L���O�����̊J�n�ʒu�iX)
#define RANK_CHAR_POS_Y		(SCREEN_HEIGHT * 0.08f)		//�����L���O�����̊J�n�ʒu�iY�j	
#define RANK_CHAR_WIDTH		(300.0f)					//�����L���O�����̑傫���iX�j
#define RANK_CHAR_HEIGHT	(70.0f)						//�����L���O�����̑傫���iY�j

//**********************************
//* �����L���O�i�j���[�X�R�A�w�i�j��2D�|���S���֌W
//**********************************
#define RANK_NEW_BG_POS_X	(SCREEN_WIDTH * 0.49f)		//�j���[�X�R�A�w�i�̊J�n�ʒu�iX)
#define RANK_NEW_BG_POS_Y	(SCREEN_HEIGHT * 0.88f)		//�j���[�X�R�A�w�i�̊J�n�ʒu�iY�j	
#define RANK_NEW_BG_WIDTH	(490.0f)					//�j���[�X�R�A�w�i�̑傫���iX�j
#define RANK_NEW_BG_HEIGHT	(95.0f)						//�j���[�X�R�A�w�i�̑傫���iY�j

//**********************************
//* �����L���O�i�����j��2D�|���S���֌W
//**********************************
#define RANK_LINE_MAX		(RANK_SCORE_MAX)			//�����̐��i�X�R�A�̐��Ɠ����j
#define RANK_LINE_POS_X		(SCREEN_WIDTH * 0.49f)		//�����̊J�n�ʒu�iX)
#define RANK_LINE_POS_Y		(SCREEN_HEIGHT * 0.88f)		//�����̊J�n�ʒu�iY�j	
#define RANK_LINE_WIDTH		(490.0f)					//�����̑傫���iX�j
#define RANK_LINE_HEIGHT	(95.0f)						//�����̑傫���iY�j

//**********************************
//* ��ʑJ�ڒ��̃����L���O�X�R�A�̐��l�֌W
//**********************************
#define RANK_FADE_MAX			(5)							//�X�R�A�̐�
#define	RANK_FADE_NUM_PLACE		(9)							//�X�R�A�̌���
#define RANK_FADE_COLUMN		(1)							//�X�R�A�𕪂����
#define RANK_FADE_POS_X			(SCREEN_WIDTH * 0.68f)		//�X�R�A�̊J�n�ʒu�iX)
#define RANK_FADE_POS_Y			(SCREEN_HEIGHT * 0.38f)		//�X�R�A�̊J�n�ʒu�iY�j	
#define RANK_FADE_WIDTH			(40.0f)						//�X�R�A�̑傫���iX�j
#define RANK_FADE_HEIGHT		(30.0f)						//�X�R�A�̑傫���iY�j
#define RANK_FADE_INTERVAL_X	(60.0f)						//�X�R�A�̊Ԋu�iX�j
#define RANK_FADE_INTERVAL_Y	(90.0f)						//�X�R�A�̊Ԋu�iY�j
#define RANK_FADE_MAX_NUMBER	(999999999)					//�X�R�A�̍ő吔

//**********************************
//* ��ʑJ�ڒ��̃����L���O�i���j��2D�|���S���֌W
//**********************************
#define RANK_FADE_CHAR_POS_X	(SCREEN_WIDTH * 0.5f)		//�X�R�A�̊J�n�ʒu�iX)
#define RANK_FADE_CHAR_POS_Y	(SCREEN_HEIGHT * 0.13f)		//�X�R�A�̊J�n�ʒu�iY�j	
#define RANK_FADE_CHAR_WIDTH	(250.0f)					//�X�R�A�̑傫���iX�j
#define RANK_FADE_CHAR_HEIGHT	(50.0f)						//�X�R�A�̑傫���iY�j

//**********************************************************************************************************************
//	�����L���O�̃R���X�g��`
//**********************************************************************************************************************
const char *apTextureRank[] =		// �����L���O�̃e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\ranking000.png",	// �����L���O����
	"data\\TEXTURE\\ui001.png",			// �j���[�X�R�A�w�i
	"data\\TEXTURE\\ranking001.png",	// �����L���O�̉����i1�ʁj
	"data\\TEXTURE\\ranking002.png",	// �����L���O�̉����i2�ʁj
	"data\\TEXTURE\\ranking003.png",	// �����L���O�̉����i3�ʁj
	"data\\TEXTURE\\ranking004.png",	// �����L���O�̉����i4�ʁj
	"data\\TEXTURE\\ranking005.png",	// �����L���O�̉����i5�ʁj
};

//**********************************************************************************************************************
//	�����L���O�̗񋓌^��` (TEXTURE_TITLE)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_RANK_CHAR = 0,		// �����L���O����
	TEXTURE_RANK_UI_004,		// �j���[�X�R�A�w�i
	TEXTURE_RANK_LINE,			// ����
	TEXTURE_RANK_MAX,			// ���̗񋓌^�̑���
} TEXTURE_RANKING;

//**********************************************
//* �񋓌^
//**********************************************

//**********************************************
//* �\����
//**********************************************
//**********************************
//* �\���́i�����L���O��2D�|���S���j
//**********************************
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXCOLOR col;			//�F
	bool bUse;				//�g�p�̗L��
}Ranking2D;

//**********************************
//* �\���́i�����L���O�̃X�R�A�j
//**********************************
typedef struct
{
	int nScore;				//�X�R�A
	D3DXVECTOR3 pos;		//�ʒu
	D3DXCOLOR col;			//�F
	bool bFlash;			//�_��
	bool bUse;				//�g�p���Ă��邩
}RankingScore;

//***************************************************************
// �O���[�o���ϐ��錾
//***************************************************************
LPDIRECT3DTEXTURE9      g_apTextureRank[TEXTURE_RANK_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank = NULL;					// ���_�o�b�t�@�ւ̃|�C���^

Ranking2D g_rank2DChar;			//�����L���O������2D�|���S���̏��
Ranking2D g_rank2DLine;			//�����L���O������2D�|���S���̏��
Ranking2D g_rank2DNewBg;		//�����L���O�j���[�X�R�A�w�i��2D�|���S���̏��

RankingScore g_aRankScore[RANK_SCORE_MAX];	//�����L���O�̃X�R�A�̏��
RankingScore g_aRankNewScore;				//�����L���O�̃j���[�X�R�A�̏��

FileMode g_fileMode;						//�t�@�C���̓��o�͂̎��
bool g_bRankTrance;							//�J�ڂ̗L��
int g_nNewScoreNumber = -1;					//�j���[�X�R�A�̔ԍ�

//====================================
//= �����L���O�̏���������
//=====================================
void InitRanking(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D         *pVtx;					// ���_���ւ̃|�C���^

	for (int nCntRank = 0; nCntRank < TEXTURE_RANK_MAX; nCntRank++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

	  // �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, apTextureRank[nCntRank], &g_apTextureRank[nCntRank]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_2D) * 4 * MAX_RANK,	// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffRank,
		NULL
	);

	//�X�R�A���擾
	int pScore = GetScore();

	//�ϐ��錾
	int nCutRank;

	//�ϐ��̏�����
	g_bRankTrance = false;

	if (GetTitleState() != TITLESTATE_FADE)
	{//�^�C�g���J�ڒ�����Ȃ��Ƃ�

		//2D�|���S��
		{
			//�����L���O������2D�w�i
			g_rank2DChar.pos = D3DXVECTOR3(RANK_CHAR_POS_X, RANK_CHAR_POS_Y, 0.0f);
			g_rank2DChar.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_rank2DChar.bUse = true;

			//�j���[�X�R�A��2D�w�i
			g_rank2DNewBg.pos = D3DXVECTOR3(RANK_NEW_BG_POS_X, RANK_NEW_BG_POS_Y, 0.0f);
			g_rank2DNewBg.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_rank2DNewBg.bUse = true;	//TRUE�ɕύX
		}
		
		//���l
		{
			//�X�R�A�̏���������
			for (nCutRank = 0; nCutRank < RANK_SCORE_MAX; nCutRank++)
			{
				g_aRankScore[nCutRank].pos = D3DXVECTOR3(RANK_POS_X, RANK_POS_Y, 0.0f);
				g_aRankScore[nCutRank].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				g_aRankScore[nCutRank].bFlash = false;
				g_aRankScore[nCutRank].bUse = true;
			}

			//�j���[�X�R�A�̏���������
			{
				g_aRankNewScore.nScore = pScore;
				g_aRankNewScore.pos = D3DXVECTOR3(RANK_NEW_POS_X, RANK_NEW_POS_Y, 0.0f);
				g_aRankNewScore.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				g_aRankNewScore.bFlash = false;
				g_aRankNewScore.bUse = true;
			}
		}

	}
	else
	{
		//2D�|���S��
		{
			//�����L���O������2D�w�i
			g_rank2DChar.pos = D3DXVECTOR3(RANK_FADE_CHAR_POS_X, RANK_FADE_CHAR_POS_Y, 0.0f);
			g_rank2DChar.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_rank2DChar.bUse = true;

			//�j���[�X�R�A��2D�w�i
			g_rank2DNewBg.pos = D3DXVECTOR3(RANK_NEW_BG_POS_X, RANK_NEW_BG_POS_Y, 0.0f);
			g_rank2DNewBg.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_rank2DNewBg.bUse = false;	//FALSE�ɕύX
		}

		//���l
		{
			//�X�R�A�̏���������
			for (nCutRank = 0; nCutRank < RANK_FADE_MAX; nCutRank++)
			{
				g_aRankScore[nCutRank].pos = D3DXVECTOR3(RANK_FADE_POS_X, RANK_FADE_POS_Y, 0.0f);
				g_aRankScore[nCutRank].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				g_aRankScore[nCutRank].bFlash = false;
				g_aRankScore[nCutRank].bUse = true;
			}

			//�j���[�X�R�A�̏���������
			{
				g_aRankNewScore.nScore = pScore;
				g_aRankNewScore.pos = D3DXVECTOR3(RANK_NEW_POS_X, RANK_NEW_POS_Y, 0.0f);
				g_aRankNewScore.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				g_aRankNewScore.bFlash = false;
				g_aRankNewScore.bUse = false;	//FALSE�ɕύX
			}
		}
	}

	// ���\�������̑S�̏����i3D�}�b�v�j
	InitAllAroundChunk();

	// �A�C�R���̏�����
	InitIcon();

	// �t�@�C�������[�h����S�̏���
	LoadFileChunk
	( // ����
		true,	// �Ԃ̃J�[�u
		true,	// �l�Ԃ̃J�[�u
		true,	// �X�e�[�W
		false,	// �����蔻��
		true,	// �e
		true,	// �I�u�W�F�N�g
		true	// AI
	);

	if (GetTitleState() != TITLESTATE_FADE)
	{//�^�C�g���J�ڒ�����Ȃ��Ƃ�
		
		//�X�R�A�̃\�[�g
		SortRankingScore();

	}

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------------------------------------------------------------------------------------------
	//	�����L���O�i�����j�̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_rank2DChar.pos.x - RANK_CHAR_WIDTH, g_rank2DChar.pos.y - RANK_CHAR_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_rank2DChar.pos.x + RANK_CHAR_WIDTH, g_rank2DChar.pos.y - RANK_CHAR_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_rank2DChar.pos.x - RANK_CHAR_WIDTH, g_rank2DChar.pos.y + RANK_CHAR_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_rank2DChar.pos.x + RANK_CHAR_WIDTH, g_rank2DChar.pos.y + RANK_CHAR_HEIGHT, 0.0f);

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
	//	�����L���O�i�j���[�X�R�A�w�i�j�̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[4].pos = D3DXVECTOR3(g_rank2DNewBg.pos.x - RANK_NEW_BG_WIDTH, g_rank2DNewBg.pos.y - RANK_NEW_BG_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_rank2DNewBg.pos.x + RANK_NEW_BG_WIDTH, g_rank2DNewBg.pos.y - RANK_NEW_BG_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_rank2DNewBg.pos.x - RANK_NEW_BG_WIDTH, g_rank2DNewBg.pos.y + RANK_NEW_BG_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_rank2DNewBg.pos.x + RANK_NEW_BG_WIDTH, g_rank2DNewBg.pos.y + RANK_NEW_BG_HEIGHT, 0.0f);

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

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRank->Unlock();
}

//=====================================
//= �����L���O�̏I������
//=====================================
void UninitRanking(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	�����L���O2D�̏I��
	//------------------------------------------------------------------------------------------------------------------
	// �e�N�X�`���̔j��
	for (int nCntRank = 0; nCntRank < TEXTURE_RANK_MAX; nCntRank++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTextureRank[nCntRank] != NULL)
		{ // �ϐ� (g_apTextureTitle) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureRank[nCntRank]->Release();
			g_apTextureRank[nCntRank] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffRank != NULL)
	{ // �ϐ� (g_pVtxBuffTitle) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffRank->Release();
		g_pVtxBuffRank = NULL;
	}

	//�j���[�X�R�A�̔ԍ�
	g_nNewScoreNumber = -1;

	//�����L���O�̒l���t�@�C���ɏ����o��
	SaveRanking();

	// ���\�I���̑S�̏����i3D�}�b�v�j
	UninitAllAroundChunk();

	// �A�C�R���̏I��
	UninitIcon();

	if (GetTitleState() != TITLESTATE_FADE)
	{//�^�C�g���J�ڒ�����Ȃ��Ƃ�
		//�T�E���h�̒�~�i���U���g�����BGM�j
		StopSound();
	}

}

//=====================================
//= �����L���O�̍X�V����
//=====================================
void UpdateRanking(void)
{
	// �V�C�̍X�V����
	UpdateWeather();

	// �J�����̍X�V����
	UpdateCamera();

	// ���C�g�̍X�V����
	UpdateLight();

	// �Ԃ̍X�V����
	UpdateCar();

	// �l�Ԃ̍X�V����
	UpdateHuman();

	//�@�p�g�J�[�̍X�V����
	UpdatePolice();

	//------*****�_�ł̏���******------

	if (g_nNewScoreNumber != -1)
	{//�����L���O���X�V���ꂽ�Ƃ�
		//�X�V���ꂽ�X�R�A�ƃj���[�X�R�A
		if (g_aRankScore[g_nNewScoreNumber].bFlash == false)
		{
			g_aRankScore[g_nNewScoreNumber].col.a -= RANK_FLASH_INTERVAL;
			if (g_aRankScore[g_nNewScoreNumber].col.a <= RANK_FLASH_MIN)
			{
				g_aRankScore[g_nNewScoreNumber].bFlash = true;
			}
		}
		else if (g_aRankScore[g_nNewScoreNumber].bFlash == true)
		{
			g_aRankScore[g_nNewScoreNumber].col.a += RANK_FLASH_INTERVAL;
			if (g_aRankScore[g_nNewScoreNumber].col.a >= RANK_FLASH_MAX)
			{
				g_aRankScore[g_nNewScoreNumber].bFlash = false;
			}
		}
	}

	//------*****���͂̔���******------

	//�t�F�[�h�̏����擾
	FADE pFade = GetFade();

	//���[�h�ݒ�
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_SPACE) == true
	 || GetJoyKeyTrigger(JOYKEY_A, 0) == true  || GetJoyKeyTrigger(JOYKEY_B, 0) == true
	 || GetJoyKeyTrigger(JOYKEY_X, 0) == true  || GetJoyKeyTrigger(JOYKEY_Y, 0) == true && pFade == FADE_NONE)
	{//����L�[����������
		//���[�h�ݒ�i�^�C�g����ʂɈڍs�j
		SetFade(MODE_TITLE);
		//����̉�
		//PlaySound(SOUND_LABEL_SE_ENTER);
	}

	//�G���Ă��Ȃ����Ԃ̂Ƃ��^�C�g����
	if (g_bRankTrance == true)
	{
		//���[�h�ݒ�i�^�C�g����ʂɈڍs�j
		SetFade(MODE_TITLE);
	}
}

//=====================================
//= �����L���O�̕`�揈��
//=====================================
void DrawRanking(void)
{
	// ���\�`��̑S�̏���
	DrawAllAroundChunk();

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRank, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//2D�|���S���̕`��
	{ // �g�p����|���S�������J��Ԃ�

	  //�����L���O�i�����j
		if (g_rank2DChar.bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureRank[0]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}

		//�j���[�X�R�A�w�i
		if (g_rank2DNewBg.bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureRank[1]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
		}
	}

	if (GetTitleState() != TITLESTATE_FADE)
	{//�^�C�g���J�ڒ�����Ȃ��Ƃ�
	 //�����L���O�X�R�A�̐��l
		for (int nCount = 0; nCount < RANK_SCORE_MAX; nCount++)
		{ // ���ʂ��X�V����Ă���ꍇ

		  //���l��ݒ�
			SetValue(
				D3DXVECTOR3(g_aRankScore[nCount].pos.x, g_aRankScore[nCount].pos.y + (nCount * RANK_INTERVAL_Y), 0.0f),
				g_aRankScore[nCount].nScore,
				RANK_MAX_NUMBER,
				RANK_WIDTH,
				RANK_HEIGHT,
				RANK_INTERVAL_X,
				g_aRankScore[nCount].col.a);

			// ���l�̕`��
			DrawValue(RANK_NUM_PLACE, VALUETYPE_NORMAL);
		}

		//�j���[�X�R�A�̐��l
		{
			//���l��ݒ�
			SetValue(
				D3DXVECTOR3(g_aRankNewScore.pos.x, g_aRankNewScore.pos.y, 0.0f),
				g_aRankNewScore.nScore,
				RANK_MAX_NUMBER,
				RANK_NEW_WIDTH,
				RANK_NEW_HEIGHT,
				RANK_NEW_INTERVAL_X,
				g_aRankNewScore.col.a);

			// ���l�̕`��
			DrawValue(RANK_NUM_PLACE, VALUETYPE_RED);
		}
	}
	else
	{//�^�C�g���J�ڒ��̕`�揈��

		for (int nCount = 0; nCount < RANK_FADE_MAX; nCount++)
		{ // ���ʂ��X�V����Ă���ꍇ

		  //���l��ݒ�
			SetValue(
				D3DXVECTOR3(g_aRankScore[nCount].pos.x, g_aRankScore[nCount].pos.y + (nCount * RANK_FADE_INTERVAL_Y), 0.0f),
				g_aRankScore[nCount].nScore,
				RANK_FADE_MAX_NUMBER,
				RANK_FADE_WIDTH,
				RANK_FADE_HEIGHT,
				RANK_FADE_INTERVAL_X,
				g_aRankScore[nCount].col.a);

			// ���l�̕`��
			DrawValue(RANK_FADE_NUM_PLACE, VALUETYPE_NORMAL);
		}
	}



}

//=====================================
//= �����L���O�̃X�R�A�̃\�[�g�ݒ�
//=====================================
void SortRankingScore(void)
{
	//�ϐ��錾
	int nCutRank, nCutScoreRank;	//�ő吔�̃f�[�^�Ɨv�f�̃J�E���g
	int nTenpData;					//���̃f�[�^�u����
	int nCountData;					//�u��������ԍ�

	if (g_aRankNewScore.nScore > 0)
	{//�j���[�X�R�A��0��艺�ł͂Ȃ��Ƃ�
		if (g_aRankNewScore.nScore >= g_aRankScore[(RANK_SCORE_MAX - 1)].nScore ||
			g_aRankScore[(RANK_SCORE_MAX - 1)].nScore == 0)
		{//�j���[�X�R�A����ԒႢ�X�R�A���傫���Ƃ�
			g_aRankScore[(RANK_SCORE_MAX - 1)].nScore = g_aRankNewScore.nScore;
			g_nNewScoreNumber = (RANK_SCORE_MAX - 1);

			//�v�f1�̃f�[�^�Ɨv�f2�̃f�[�^���r
			for (nCutRank = 0; nCutRank < (RANK_SCORE_MAX - 1); nCutRank++)
			{
				nCountData = nCutRank;
				//�u�������ԍ��ɗv�f1�̃f�[�^�ԍ�����
				for (nCutScoreRank = nCutRank; nCutScoreRank < RANK_SCORE_MAX; nCutScoreRank++)
				{
					//�v�f2�ɉ��̐��l����
					if (g_aRankScore[nCutScoreRank].nScore > g_aRankScore[nCountData].nScore)
					{
						if (nCutScoreRank == g_nNewScoreNumber)
						{
							g_nNewScoreNumber = nCountData;
						}
						nCountData = nCutScoreRank;
					}
				}

				//�f�[�^��u������
				nTenpData = g_aRankScore[nCutRank].nScore;
				g_aRankScore[nCutRank].nScore = g_aRankScore[nCountData].nScore;
				g_aRankScore[nCountData].nScore = nTenpData;
			}
		}
	}
}

//=====================================
//= �����L���O�̃��Z�b�g����
//=====================================
void ResetRanking(void)
{
	//�t�@�C���̓��o�̓��[�h�̐ݒ�
	g_fileMode = RANK_FILE_MODE;

	//�����L���O�̃X�R�A�̒l���t�@�C������ǂݍ���
	g_aRankScore[0].nScore = {};
	LoadRanking();

	//�j���[�X�R�A�̃��Z�b�g
	g_aRankNewScore.nScore = 0;
}

//=======================
//=�@�t�@�C���ɏ����o��
//=======================
void SaveRanking(void)
{
	//�t�@�C���|�C���^�錾
	FILE * pFile;

	switch (g_fileMode)
	{
	//**********�e�L�X�g���[�h**********
	case FILE_MODE_TXT:
		//�t�@�C�����J��
		pFile = fopen(RANK_FILE_TXT, "w");
		//�t�@�C���ɏ����o��
		if (pFile != NULL)
		{//�t�@�C�����J�����Ȃ�
		 //�e�L�X�g�t�@�C���ɏ����o��
			for (int nCount = 0; nCount < RANK_SCORE_MAX; nCount++)
			{
				fprintf(pFile, "%d\n", g_aRankScore[nCount].nScore);
			}
			//�t�@�C�������
			fclose(pFile);
		}
		else
		{ // �t�@�C�����J���Ȃ������ꍇ

		  // �G���[���b�Z�[�W�{�b�N�X
			MessageBox(NULL, "�����L���O�f�[�^������܂���", "�x���I", MB_ICONWARNING);
		}
		break;
	//**********�o�C�i�����[�h**********
	case FILE_MODE_BIN:
		//�t�@�C�����J��
		pFile = fopen(RANK_FILE_BIN, "wb");
		//�t�@�C���ɏ����o��
		if (pFile != NULL)
		{//�t�@�C�����J�����Ȃ�
		 //�o�C�i���t�@�C���ɏ����o��
			for (int nCount = 0; nCount < RANK_SCORE_MAX; nCount++)
			{
				fwrite(&g_aRankScore[nCount].nScore, sizeof(int), RANK_SCORE_MAX, pFile);
			}
			//�t�@�C�������
			fclose(pFile);
		}
		else
		{ // �t�@�C�����J���Ȃ������ꍇ

		  // �G���[���b�Z�[�W�{�b�N�X
			MessageBox(NULL, "�����L���O�f�[�^������܂���", "�x���I", MB_ICONWARNING);
		}
		break;

	}

}

//=======================
//=�@�t�@�C����ǂݍ���
//=======================
void LoadRanking(void)
{
	//�t�@�C���|�C���^�錾
	FILE *pFile;

	switch (g_fileMode)
	{
	//**********�e�L�X�g���[�h**********
	case FILE_MODE_TXT:
		//�t�@�C�����J��
		pFile = fopen(RANK_FILE_TXT, "r");
		//�t�@�C����ǂݍ���
		if (pFile != NULL)
		{//�t�@�C�����J�����Ȃ�
		 //�o�C�i���t�@�C���ɏ����o��
			for (int nCount = 0; nCount < RANK_SCORE_MAX; nCount++)
			{
				fscanf(pFile, "%d", &g_aRankScore[nCount].nScore);
			}
			//�t�@�C�������
			fclose(pFile);
		}
		else
		{ // �t�@�C�����J���Ȃ������ꍇ

		  // �G���[���b�Z�[�W�{�b�N�X
			MessageBox(NULL, "�����L���O�f�[�^������܂���", "�x���I", MB_ICONWARNING);
		}
		break;
	//**********�o�C�i�����[�h**********
	case FILE_MODE_BIN:
		//�t�@�C�����J��
		pFile = fopen(RANK_FILE_BIN, "rb");
		//�t�@�C����ǂݍ���
		if (pFile != NULL)
		{//�t�@�C�����J�����Ȃ�
		 //�o�C�i���t�@�C���ɏ����o��
			for (int nCount = 0; nCount < RANK_SCORE_MAX; nCount++)
			{
				fread(&g_aRankScore[nCount].nScore, sizeof(int), RANK_SCORE_MAX, pFile);
			}
			//�t�@�C�������
			fclose(pFile);
		}
		break;
	}
}

//=======================
//=�@���H�����ł̉�ʑJ�ڂ�ݒ�
//=======================
void SetRankingRoadTrance(void)
{
	g_bRankTrance = true;
}