
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

//**********************************************
//* �}�N��
//**********************************************
//**********************************
//* �����L���O�X�R�A�̐��l�֌W
//**********************************
#define RANK_MAX			(5)						//�X�R�A�̐�
#define	RANK_NUM_PLACE		(8)						//�X�R�A�̌���
#define RANK_TEX			(1)						//�X�R�A�̃e�N�X�`��
#define RANK_COLUMN			(1)						//�X�R�A�𕪂����
#define RANK_POS_X			(SCREEN_WIDTH * 0.725f)	//�X�R�A�̊J�n�ʒu�iX)
#define RANK_POS_Y			(SCREEN_HEIGHT * 0.15f)	//�X�R�A�̊J�n�ʒu�iY�j	
#define RANK_SIZE_X			(50.0f)					//�X�R�A�̑傫���iX�j
#define RANK_SIZE_Y			(40.0f)					//�X�R�A�̑傫���iY�j
#define RANK_INTERVAL_X		(80.0f)					//�X�R�A�̊Ԋu�iX�j
#define RANK_INTERVAL_Y		(90.0f)					//�X�R�A�̊Ԋu�iY�j

#define RANK_FILE_MODE		(FILE_MODE_BIN)					//�����L���O�t�@�C���̓��o�̓��[�h
#define RANK_FILE_TXT		("data\\TXT\\rank.txt")			//�����L���O�̃X�R�A�̃e�L�X�g�t�@�C��
#define RANK_FILE_BIN		("data\\BIN\\rank.bin")			//�����L���O�̃X�R�A�̃o�C�i���t�@�C��

//**********************************
//* �����L���O�j���[�X�R�A�̐��l�֌W
//**********************************
#define	RANK_NEW_NUM_PLACE	(8)						//�j���[�X�R�A�̌���
#define RANK_NEW_TEX		(1)						//�j���[�X�R�A�̃e�N�X�`��
#define RANK_NEW_COLUMN		(1)						//�j���[�X�R�A�𕪂����
#define RANK_NEW_POS_X		(SCREEN_WIDTH * 0.65f)	//�j���[�X�R�A�̊J�n�ʒu�iX)
#define RANK_NEW_POS_Y		(SCREEN_HEIGHT * 0.85f)	//�j���[�X�R�A�̊J�n�ʒu�iY�j	
#define RANK_NEW_SIZE_X		(40.0f)					//�j���[�X�R�A�̑傫���iX�j
#define RANK_NEW_SIZE_Y		(30.0f)					//�j���[�X�R�A�̑傫���iY�j
#define RANK_NEW_INTERVAL_X	(60.0f)					//�j���[�X�R�A�̊Ԋu�iX�j
#define RANK_NEW_INTERVAL_Y	(0.0f)					//�j���[�X�R�A�̊Ԋu�iY�j

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

//**********************************
//* �����L���O�̃X�R�A
//**********************************
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXCOLOR col;		//�F
	int nScore;			//�X�R�A
	bool bUse;			//�g�p���Ă��邩
}RankingNewScore;

//***************************************************************
// �O���[�o���ϐ��錾
//***************************************************************
RankingScore g_aRankScore[RANK_MAX];		//�����L���O�ł̃X�R�A�̏��
RankingNewScore g_aRankNewScore;			//�����L���O�̃j���[�X�R�A�̏��

FileMode g_fileMode;			//�t�@�C���̓��o�͂̎��
int g_nRankingNoTouchTime;		//�G���ĂȂ�����
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
	g_nRankingNoTouchTime = 0;


	//�X�R�A�̏���������
	for (nCutRank = 0; nCutRank < RANK_MAX; nCutRank++)
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
		g_aRankNewScore.bUse = true;
	}

	SetRankingScore();
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
}

//=====================================
//= �����L���O�̍X�V����
//=====================================
void UpdateRanking(void)
{
	//------*****���͂̔���******------

	//�t�F�[�h�̏����擾
	FADE pFade = GetFade();

	//���[�h�ݒ�
	if (GetKeyboardTrigger(DIK_RETURN) == true && pFade == FADE_NONE)
	{//����L�[����������
		//���[�h�ݒ�i�^�C�g����ʂɈڍs�j
		SetFade(MODE_TITLE);
		//����̉�
		//PlaySound(SOUND_LABEL_SE_ENTER);
	}

	g_nRankingNoTouchTime++;

	//�G���Ă��Ȃ����Ԃ̂Ƃ��^�C�g����
	if (g_nRankingNoTouchTime == 600)
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
	//�����L���O�X�R�A�̐��l
	for (int nCount = 0; nCount < RANK_MAX; nCount++)
	{ // ���ʂ��X�V����Ă���ꍇ
		
		//���l��ݒ�
		SetValue(
			D3DXVECTOR3(g_aRankScore[nCount].pos.x, g_aRankScore[nCount].pos.y + (nCount * RANK_INTERVAL_Y), 0.0f),
			g_aRankScore[nCount].nScore,
			99999999,
			RANK_SIZE_X,
			RANK_SIZE_Y,
			RANK_INTERVAL_X,
			1.0f);

		// ���l�̕`��
		DrawValue(RANK_NUM_PLACE, VALUETYPE_NORMAL);
	}

	//�j���[�X�R�A�̐��l
	{
		//���l��ݒ�
		SetValue(
			D3DXVECTOR3(g_aRankNewScore.pos.x, g_aRankNewScore.pos.y, 0.0f),
			g_aRankNewScore.nScore,
			99999999,
			RANK_NEW_SIZE_X,
			RANK_NEW_SIZE_Y,
			RANK_NEW_INTERVAL_X,
			1.0f);

		// ���l�̕`��
		DrawValue(RANK_NUM_PLACE, VALUETYPE_RED);
	}
}

//=====================================
//= �����L���O�̃X�R�A�̐ݒ�
//=====================================
void SetRankingScore(void)
{
	//�ϐ��錾
	int nCutRank, nCutScoreRank;	//�ő吔�̃f�[�^�Ɨv�f�̃J�E���g
	int nTenpData;					//���̃f�[�^�u����
	int nCountData;					//�u��������ԍ�

	if (g_aRankNewScore.nScore >= g_aRankScore[(RANK_MAX - 1)].nScore || g_aRankScore[(RANK_MAX - 1)].nScore == 0)
	{
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
