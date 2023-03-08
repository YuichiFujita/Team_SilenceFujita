//======================================================================================================================
//
//	�T�E���h�w�b�_�[ [sound.h]
//	Author�F���c�E��
//
//======================================================================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"

//**********************************************************************************************************************
//	�񋓌^��` (SOUND_LABEL)
//**********************************************************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_ = 0,				// BGM
	SOUND_LABEL_BGM_GAME_000,			// �Q�[����BGM_000
	SOUND_LABEL_BGM_GAME_001,			// �Q�[����BGM_001
	SOUND_LABEL_BGM_TITLE_000,			// �^�C�g����BGM_000
	SOUND_LABEL_BGM_RESULT_000,			// ���U���g��BGM_000
	SOUND_LABEL_BGM_TUTORIAL_000,		// �`���[�g���A����BGM_000
	SOUND_LABEL_BGM_FIRECAR_000,		// ���h�Ԃ̌��ʉ�BGM_000
	SOUND_LABEL_BGM_YAKIIMO_000,		// �ΏĂ����̌��ʉ�BGM_000
	SOUND_LABEL_BGM_BOUSOUCAR_000,		// �\���Ԃ̌��ʉ�BGM_000
	SOUND_LABEL_BGM_SENKYOCAR_000,		// �I���J�[�̌��ʉ�BGM_000
	SOUND_LABEL_BGM_POLICE_000,			// �x�@�̌��ʉ�BGM_000
	SOUND_LABEL_BGM_ABILITY_WIND_000,	// �v���C���[�̔\�́i�����@�j�̌��ʉ�BGM_000
	SOUND_LABEL_SE_,					// SE
	SOUND_LABEL_SE_SELECT_000,			// �I����SE
	SOUND_LABEL_SE_DECIDE_000,			// �����SE
	SOUND_LABEL_SE_START_COUNT_000,		// �X�^�[�g�̃J�E���g�_�E���i�s�b�I�j��SE_000
	SOUND_LABEL_SE_START_GO_000,		// �J�n���̃X�^�[�g�i�s�[�I�j��SE_000// �����SE
	SOUND_LABEL_SE_ABILITY_BOOST_000,	// �v���C���[�̔\�́i�u�[�X�g�j��BGM_000
	SOUND_LABEL_SE_ABILITY_BOMB_000,	// �v���C���[�̔\�́i�{���j��SE_000
	SOUND_LABEL_SE_BREAK_000,			// �j�󉹂�SE_000
	SOUND_LABEL_SE_DAMAGE_000,			// �v���C���[�_���[�W��SE_000
	SOUND_LABEL_SE_GAREKI_000,			// �j�󉹁i�����n�j��SE_000
	SOUND_LABEL_SE_GAREKI_001,			// �j�󉹁i�����n�j��SE_001
	SOUND_LABEL_SE_SCORE_000,			// �X�R�A�l����SE_000
	SOUND_LABEL_MAX,					// ���̗񋓌^�̑���
} SOUND_LABEL;

//**********************************************************************************************************************
//	�񋓌^��` (�T�E���h�̎��)
//**********************************************************************************************************************
typedef enum
{
	SOUND_TYPE_MAIN_BGM = 0,	//��{BGM�̂�
	SOUND_TYPE_SUB_BGM,			//���ʉ��n��BGM�̂�
	SOUND_TYPE_SE,				//SE�̂�
	SOUND_TYPE_MAX
}SOUND_TYPE;

//**********************************************************************************************************************
//	�񋓌^��` (�T�E���h�̗L���̐ݒ�)
//**********************************************************************************************************************
typedef enum
{
	SOUND_SWITCH_OFF = 0,		//���Ȃ�
	SOUND_SWITCH_ON,			//�S��
	SOUND_SWITCH_NO_BGM,		//BGM�ȊO
	SOUND_SWITCH_MAX
}SOUND_SWITCH;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
HRESULT InitSound(HWND hWnd);			// �T�E���h�̏���������
void UninitSound(void);					// �T�E���h�̏I������
HRESULT PlaySound(SOUND_LABEL label);	// �T�E���h�̍Đ�����
void StopSound(SOUND_LABEL label);
void StopSound(void);
void SetSoundVolume(SOUND_LABEL label, float fdata);	// �T�E���h�̉��ʒ���
float GetSoundVolume(SOUND_LABEL label);				// �T�E���h�̉��ʂ̎擾
bool GetSoundType(SOUND_TYPE);							// �T�E���h�̗L���̎擾

#endif
