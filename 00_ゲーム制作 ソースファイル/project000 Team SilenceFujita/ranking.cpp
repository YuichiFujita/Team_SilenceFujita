
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

#include "score.h"
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
#define RANK_FILE_MODE		(FILE_MODE_BIN)					//�����L���O�t�@�C���̓��o�̓��[�h
#define RANK_FILE_TXT		("data\\TXT\\rank.txt")			//�����L���O�̃X�R�A�̃e�L�X�g�t�@�C��
#define RANK_FILE_BIN		("data\\BIN\\rank.bin")			//�����L���O�̃X�R�A�̃o�C�i���t�@�C��

#define RANK_FLASH_MAX		(1.0f)					//�_�ł̍ő�l
#define RANK_FLASH_MIN		(0.3f)					//�_�ł̍ŏ��l
#define RANK_FLASH_INTERVAL	(0.02f)					//�_�ł̊Ԋu

//**********************************
//* �����L���O�X�R�A�̐��l�֌W
//**********************************
#define RANK_MAX			(5)							//�X�R�A�̐�
#define	RANK_NUM_PLACE		(9)							//�X�R�A�̌���
#define RANK_TEX			(1)							//�X�R�A�̃e�N�X�`��
#define RANK_COLUMN			(1)							//�X�R�A�𕪂����
#define RANK_POS_X			(SCREEN_WIDTH * 0.75f)		//�X�R�A�̊J�n�ʒu�iX)
#define RANK_POS_Y			(SCREEN_HEIGHT * 0.14f)		//�X�R�A�̊J�n�ʒu�iY�j	
#define RANK_SIZE_X			(50.0f)						//�X�R�A�̑傫���iX�j
#define RANK_SIZE_Y			(40.0f)						//�X�R�A�̑傫���iY�j
#define RANK_INTERVAL_X		(80.0f)						//�X�R�A�̊Ԋu�iX�j
#define RANK_INTERVAL_Y		(90.0f)						//�X�R�A�̊Ԋu�iY�j
#define RANK_MAX_NUMBER		(999999999)					//�X�R�A�̍ő吔

//**********************************
//* �����L���O�j���[�X�R�A�̐��l�֌W
//**********************************
#define	RANK_NEW_NUM_PLACE	(9)							//�j���[�X�R�A�̌���
#define RANK_NEW_TEX		(1)							//�j���[�X�R�A�̃e�N�X�`��
#define RANK_NEW_COLUMN		(1)							//�j���[�X�R�A�𕪂����
#define RANK_NEW_POS_X		(SCREEN_WIDTH * 0.70f)		//�j���[�X�R�A�̊J�n�ʒu�iX)
#define RANK_NEW_POS_Y		(SCREEN_HEIGHT * 0.85f)		//�j���[�X�R�A�̊J�n�ʒu�iY�j	
#define RANK_NEW_SIZE_X		(45.0f)						//�j���[�X�R�A�̑傫���iX�j
#define RANK_NEW_SIZE_Y		(35.0f)						//�j���[�X�R�A�̑傫���iY�j
#define RANK_NEW_INTERVAL_X	(65.0f)						//�j���[�X�R�A�̊Ԋu�iX�j
#define RANK_NEW_INTERVAL_Y	(0.0f)						//�j���[�X�R�A�̊Ԋu�iY�j
#define RANK_NEW_MAX_NUMBER	(999999999)					//�j���[�X�R�A�̍ő吔

//**********************************
//* ��ʑJ�ڒ��̃����L���O�X�R�A�̐��l�֌W
//**********************************
#define RANK_FADE_MAX			(5)							//�X�R�A�̐�
#define	RANK_FADE_NUM_PLACE		(9)							//�X�R�A�̌���
#define RANK_FADE_TEX			(1)							//�X�R�A�̃e�N�X�`��
#define RANK_FADE_COLUMN		(1)							//�X�R�A�𕪂����
#define RANK_FADE_POS_X			(SCREEN_WIDTH * 0.75f)		//�X�R�A�̊J�n�ʒu�iX)
#define RANK_FADE_POS_Y			(SCREEN_HEIGHT * 0.23f)		//�X�R�A�̊J�n�ʒu�iY�j	
#define RANK_FADE_SIZE_X		(50.0f)						//�X�R�A�̑傫���iX�j
#define RANK_FADE_SIZE_Y		(40.0f)						//�X�R�A�̑傫���iY�j
#define RANK_FADE_INTERVAL_X	(80.0f)						//�X�R�A�̊Ԋu�iX�j
#define RANK_FADE_INTERVAL_Y	(105.0f)					//�X�R�A�̊Ԋu�iY�j
#define RANK_FADE_MAX_NUMBER	(999999999)					//�X�R�A�̍ő吔

//**********************************************
//* �񋓌^
//**********************************************

//**********************************************
//* �\����
//**********************************************
//**********************************
//* �����L���O�̃X�R�A
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
RankingScore g_aRankScore[RANK_MAX];	//�����L���O�ł̃X�R�A�̏��
RankingScore g_aRankNewScore;			//�����L���O�̃j���[�X�R�A�̏��

FileMode g_fileMode;			//�t�@�C���̓��o�͂̎��
bool g_bRankTrance;				//�J�ڂ̗L��
int g_nNewScoreNumber = -1;		//�j���[�X�R�A�̔ԍ�

//====================================
//= �����L���O�̏���������
//=====================================
void InitRanking(void)
{
	//�X�R�A���擾
	int pScore = GetScore();

	//�ϐ��錾
	int nCutRank;

	//�ϐ��̏�����
	g_bRankTrance = false;

	if (GetTitleState() != TITLESTATE_FADE)
	{//�^�C�g���J�ڒ�����Ȃ��Ƃ�
		for (nCutRank = 0; nCutRank < RANK_MAX; nCutRank++)
		{
			g_aRankScore[nCutRank].pos = D3DXVECTOR3(RANK_POS_X, RANK_POS_Y, 0.0f);
			g_aRankScore[nCutRank].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_aRankScore[nCutRank].bFlash = false;
			g_aRankScore[nCutRank].bUse = true;
		}
	}
	else
	{
		//�X�R�A�̏���������
		for (nCutRank = 0; nCutRank < RANK_FADE_MAX; nCutRank++)
		{
			g_aRankScore[nCutRank].pos = D3DXVECTOR3(RANK_FADE_POS_X, RANK_FADE_POS_Y, 0.0f);
			g_aRankScore[nCutRank].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_aRankScore[nCutRank].bFlash = false;
			g_aRankScore[nCutRank].bUse = true;
		}
	}

	//�j���[�X�R�A�̏���������
	{
		g_aRankNewScore.nScore = pScore;
		g_aRankNewScore.pos = D3DXVECTOR3(RANK_NEW_POS_X, RANK_NEW_POS_Y, 0.0f);
		g_aRankNewScore.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aRankScore[nCutRank].bFlash = false;
		g_aRankNewScore.bUse = true;
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
}

//=====================================
//= �����L���O�̏I������
//=====================================
void UninitRanking(void)
{
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
			g_aRankNewScore.col.a -= RANK_FLASH_INTERVAL;
			if (g_aRankScore[g_nNewScoreNumber].col.a <= RANK_FLASH_MIN)
			{
				g_aRankScore[g_nNewScoreNumber].bFlash = true;
				g_aRankNewScore.bFlash = true;
			}
		}
		else if (g_aRankScore[g_nNewScoreNumber].bFlash == true)
		{
			g_aRankScore[g_nNewScoreNumber].col.a += RANK_FLASH_INTERVAL;
			g_aRankNewScore.col.a += RANK_FLASH_INTERVAL;
			if (g_aRankScore[g_nNewScoreNumber].col.a >= RANK_FLASH_MAX)
			{
				g_aRankScore[g_nNewScoreNumber].bFlash = false;
				g_aRankNewScore.bFlash = false;
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
	// ���U���g�̕`��S�̏���
	DrawResultChunk();

	if (GetTitleState() != TITLESTATE_FADE)
	{//�^�C�g���J�ڒ�����Ȃ��Ƃ�
	 //�����L���O�X�R�A�̐��l
		for (int nCount = 0; nCount < RANK_MAX; nCount++)
		{ // ���ʂ��X�V����Ă���ꍇ

		  //���l��ݒ�
			SetValue(
				D3DXVECTOR3(g_aRankScore[nCount].pos.x, g_aRankScore[nCount].pos.y + (nCount * RANK_INTERVAL_Y), 0.0f),
				g_aRankScore[nCount].nScore,
				RANK_MAX_NUMBER,
				RANK_SIZE_X,
				RANK_SIZE_Y,
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
				RANK_NEW_SIZE_X,
				RANK_NEW_SIZE_Y,
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
				RANK_FADE_SIZE_X,
				RANK_FADE_SIZE_Y,
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
		if (g_aRankNewScore.nScore >= g_aRankScore[(RANK_MAX - 1)].nScore ||
			g_aRankScore[(RANK_MAX - 1)].nScore == 0)
		{//�j���[�X�R�A����ԒႢ�X�R�A���傫���Ƃ�
			g_aRankScore[(RANK_MAX - 1)].nScore = g_aRankNewScore.nScore;
			g_nNewScoreNumber = (RANK_MAX - 1);

			//�v�f1�̃f�[�^�Ɨv�f2�̃f�[�^���r
			for (nCutRank = 0; nCutRank < (RANK_MAX - 1); nCutRank++)
			{
				nCountData = nCutRank;
				//�u�������ԍ��ɗv�f1�̃f�[�^�ԍ�����
				for (nCutScoreRank = nCutRank; nCutScoreRank < RANK_MAX; nCutScoreRank++)
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
			for (int nCount = 0; nCount < RANK_MAX; nCount++)
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
			for (int nCount = 0; nCount < RANK_MAX; nCount++)
			{
				fwrite(&g_aRankScore[nCount].nScore, sizeof(int), RANK_MAX, pFile);
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
			for (int nCount = 0; nCount < RANK_MAX; nCount++)
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
			for (int nCount = 0; nCount < RANK_MAX; nCount++)
			{
				fread(&g_aRankScore[nCount].nScore, sizeof(int), RANK_MAX, pFile);
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