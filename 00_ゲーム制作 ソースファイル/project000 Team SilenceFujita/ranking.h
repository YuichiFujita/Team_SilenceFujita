
//=======================
//=
//= �����L���O�̃w�b�^�[[ranking.h]
//= Author Sakai Minato
//=
//=======================

#ifndef _RANKING_H_			//���̃}�N������`����Ȃ�������
#define _RANKING_H_			//��d�C���N���[�h�X�q�̃}�N�����`

//**********************************************
//* �񋓌^
//**********************************************
//**********************************
//* �t�@�C���̓��o�͂̎��
//**********************************
typedef enum
{
	FILE_MODE_TXT = 0,	//�e�L�X�g���[�h�̓��o��
	FILE_MODE_BIN,		//�o�C�i�����[�h�̓��o��
	FILE_MODE_END
}FileMode;

//�v���g�^�C�v�錾
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void SortRankingScore(void);
void ResetRanking(void);
void SaveRanking(void);
void LoadRanking(void);
void SetRankingRoadTrance(void);

#endif 