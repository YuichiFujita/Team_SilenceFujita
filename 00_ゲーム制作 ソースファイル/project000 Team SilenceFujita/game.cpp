//======================================================================================================================
//
//	�Q�[����ʏ��� [game.cpp]
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

#include "game.h"

#include "billboard.h"
#include "camera.h"
#include "effect.h"
#include "light.h"
#include "meshfield.h"
#include "meshwall.h"
#include "particle.h"
#include "pause.h"
#include "shadow.h"
#include "object.h"
#include "Police.h"

#ifdef _DEBUG	// �f�o�b�O����
#include "Editmain.h"
#include "SoundDJ.h"
#endif

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
GAMESTATE g_gameState;			// �Q�[���̏��
int       g_nCounterGameState;	// ��ԊǗ��J�E���^�[
bool      g_bPause;				// �|�[�Y��Ԃ� ON / OFF
int       g_nGameMode;			// �G�f�B�b�g�� ON / OFF
int		  g_nSoundDJ;			//���ݗ���Ă���T�E���h

//======================================================================================================================
//	�Q�[����ʂ̏���������
//======================================================================================================================
void InitGame(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	�Q�[���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// �O���[�o���ϐ���������
	g_gameState         = GAMESTATE_NORMAL;		// �Q�[���̏��
	g_nCounterGameState = 0;					// ��ԊǗ��J�E���^�[
	g_bPause            = false;				// �|�[�Y��Ԃ� ON / OFF
	g_nGameMode         = GAMEMODE_PLAY;		// �G�f�B�b�g�� ON / OFF
#ifdef _DEBUG	// �f�o�b�O����
	g_nSoundDJ			= FUJITA_DJ_LABEL_ONE;	//�T�E���h������������
#endif

	//------------------------------------------------------------------------------------------------------------------
	//	�g�p����\�[�X�t�@�C���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// �e�̏�����
	InitShadow();

	// �I�u�W�F�N�g�̏�����
	InitObject();

	// �J�����̏�����
	InitCamera();

	// ���C�g�̏�����
	InitLight();

	// ���b�V���t�B�[���h�̏�����
	InitMeshField();

	// ���b�V���E�H�[���̏�����
	InitMeshWall();

	// �r���{�[�h�̏�����
	InitBillboard();

	// �G�t�F�N�g�̏�����
	InitEffect();

	// �p�[�e�B�N���̏�����
	InitParticle();

	// �|�[�Y�̏�����
	InitPause();

	//�x�@�̏�����
	InitPolice();

	// �X�e�[�W�̃Z�b�g�A�b�v
	TxtSetStage();

#ifdef _DEBUG	// �f�o�b�O����
	// �G�f�B�b�g���C���̏�����
	InitEditmain();
#endif
}

//======================================================================================================================
//	�Q�[����ʂ̏I������
//======================================================================================================================
void UninitGame(void)
{
	// �e�̏I��
	UninitShadow();

	// �I�u�W�F�N�g�̏I��
	UninitObject();

	// �J�����̏I��
	UninitCamera();

	// ���C�g�̏I��
	UninitLight();

	// ���b�V���t�B�[���h�̏I��
	UninitMeshField();

	// ���b�V���E�H�[���̏I��
	UninitMeshWall();

	// �r���{�[�h�̏I��
	UninitBillboard();

	// �G�t�F�N�g�̏I��
	UninitEffect();

	// �p�[�e�B�N���̏I��
	UninitParticle();

	// �|�[�Y�̏I��
	UninitPause();

	//�x�@�̏I��
	UninitPolice();

#ifdef _DEBUG	// �f�o�b�O����
	// �G�f�B�b�g���C���̏I��
	UninitEditmain();
#endif
}

//======================================================================================================================
//	�Q�[����ʂ̍X�V����
//======================================================================================================================
void UpdateGame(void)
{
	switch (g_gameState)
	{ // ��Ԃ��Ƃ̏���
	case GAMESTATE_NORMAL:

		if (GetFade() == FADE_NONE)
		{ // �t�F�[�h���Ă��Ȃ��ꍇ

#ifdef _DEBUG	// �f�o�b�O����
			if (g_nGameMode == GAMEMODE_PLAY)
			{ // �Q�[�����[�h�������ꍇ

				if (GetKeyboardTrigger(DIK_P) == true || GetJoyKeyTrigger(JOYKEY_START, 0) == true)
				{ // �|�[�Y�̑��삪�s��ꂽ�ꍇ

					// �|�[�Y�̑I�����ڂ̍Đݒ�
					ResetPauseSelect();

					// �|�[�Y�̊J�n�A����
					g_bPause = (g_bPause == false) ? true : false;
				}
			}
#else
			if (GetKeyboardTrigger(DIK_P) == true || GetJoyKeyTrigger(JOYKEY_START, 0) == true)
			{ // �|�[�Y�̑��삪�s��ꂽ�ꍇ

				// �|�[�Y�̑I�����ڂ̍Đݒ�
				ResetPauseSelect();

				// �|�[�Y�̊J�n�A����
				g_bPause = (g_bPause == false) ? true : false;
			}
#endif
		}

		// �����𔲂���
		break;

	case GAMESTATE_END:

		// ����

		// �����𔲂���
		break;
	}

#ifdef _DEBUG	// �f�o�b�O����
	// ���C�g�̍X�V
	UpdateLight();

	// �J�����̍X�V
	UpdateCamera();

	if (g_nGameMode == GAMEMODE_EDIT)
	{ // �G�f�B�b�g���[�h�������ꍇ

		// �G�f�B�b�g���C���̍X�V
		UpdateEditmain();
	}
	else
	{ // �Q�[�����[�h�������ꍇ

		if (g_bPause == false)
		{ // �|�[�Y��Ԃł͂Ȃ��ꍇ

			// ���b�V���t�B�[���h�̍X�V
			UpdateMeshField();

			// ���b�V���E�H�[���̍X�V
			UpdateMeshWall();

			// �I�u�W�F�N�g�̍X�V
			UpdateObject();

			//�x�@�̍X�V
			UpdatePolice();
		}
		else
		{ // �|�[�Y��Ԃ̏ꍇ

			// �|�[�Y�̍X�V
			UpdatePause();
		}
	}

	if (GetKeyboardTrigger(DIK_F5) == true)
	{ // [F5] �������ꂽ�ꍇ

		// �T�E���h�̒�~
		StopSoundDJ();

		// �T�E���h�𗬂�
		PlaySound(g_nSoundDJ, true);
	}

	if (GetKeyboardTrigger(DIK_F4) == true)
	{ // [F4] �������ꂽ�ꍇ

		// �T�E���h��ς���
		g_nSoundDJ = (g_nSoundDJ + 1) % SOUND_DJ_LABEL_MAX;
	}

	if (GetKeyboardTrigger(DIK_F6) == true)
	{ // [F6]�������ꂽ�ꍇ

		// �T�E���h�̒�~
		StopSoundDJ((SOUND_DJ_LABEL)g_nSoundDJ);
	}

	// �r���{�[�h�̍X�V
	UpdateBillboard();

	// �G�t�F�N�g�̍X�V
	UpdateEffect();

	// �p�[�e�B�N���̍X�V
	UpdateParticle();

	// �e�̍X�V
	UpdateShadow();

	if (GetKeyboardTrigger(DIK_F2) == true)
	{ // [F2] �������ꂽ�ꍇ

		// �Q�[�����[�h�����ւ���
		g_nGameMode = (g_nGameMode + 1) % GAMEMODE_MAX;
	}
	if (GetKeyboardTrigger(DIK_F3) == true)
	{ // [F3] �������ꂽ�ꍇ

		// �X�e�[�W�̕ۑ�
		TxtSaveStage();
	}
#else
	if (g_bPause == false)
	{ // �|�[�Y��Ԃł͂Ȃ��ꍇ

		// ���C�g�̍X�V
		UpdateLight();

		// �J�����̍X�V
		UpdateCamera();

		// ���b�V���t�B�[���h�̍X�V
		UpdateMeshField();

		// ���b�V���E�H�[���̍X�V
		UpdateMeshWall();

		// �I�u�W�F�N�g�̍X�V
		UpdateObject();

		// �r���{�[�h�̍X�V
		UpdateBillboard();

		//�x�@�̍X�V
		UpdatePolice();

		// �G�t�F�N�g�̍X�V
		UpdateEffect();

		// �p�[�e�B�N���̍X�V
		UpdateParticle();

		// �e�̍X�V
		UpdateShadow();
	}
	else
	{ // �|�[�Y��Ԃ̏ꍇ

		// �|�[�Y�̍X�V
		UpdatePause();
	}
#endif
}

//======================================================================================================================
//	�Q�[����ʂ̕`�揈��
//======================================================================================================================
void DrawGame(void)
{
	// �J�����̐ݒ�
	SetCamera();

	// ���b�V���t�B�[���h�̕`��
	DrawMeshField();

	// ���b�V���E�H�[���̕`��
	DrawMeshWall();

	// �e�̕`��
	DrawShadow();

	// �I�u�W�F�N�g�̕`��
	DrawObject();

	// �r���{�[�h�̕`��
	DrawBillboard();

	//�x�@�̕`��
	DrawPolice();

	// �G�t�F�N�g�̕`��
	DrawEffect();

	// �p�[�e�B�N���̕`��
	DrawParticle();

	if (g_bPause == true)
	{ // �|�[�Y��Ԃ̏ꍇ

		// �|�[�Y�̕`��
		DrawPause();
	}

#ifdef _DEBUG	// �f�o�b�O����
	if (g_nGameMode == GAMEMODE_EDIT)
	{ // �G�f�B�b�g���[�h�̏ꍇ

		// �G�f�B�b�g���C���̕`�揈��
		DrawEditmain();
	}
#endif
}

//======================================================================================================================
//	�Q�[����ʂ̏�Ԑݒ菈��
//======================================================================================================================
void SetGameState(GAMESTATE state, int nCounter)
{
	// �����̏�Ԃ���
	g_gameState = state;

	// �����̗]�C����
	g_nCounterGameState = nCounter;
}

//======================================================================================================================
//	�|�[�Y��Ԃ̐ݒ菈��
//======================================================================================================================
void SetEnablePause(bool bPause)
{
	// ���݂̃|�[�Y��ԂɈ����̃|�[�Y��Ԃ���
	g_bPause = bPause;
}

//======================================================================================================================
//	�Q�[����ʂ̏�Ԏ擾����
//======================================================================================================================
GAMESTATE GetGameState(void)
{
	// �Q�[���̏�Ԃ�Ԃ�
	return g_gameState;
}

//======================================================================================================================
//	�|�[�Y��Ԃ̎擾����
//======================================================================================================================
bool GetPause(void)
{
	// �|�[�Y��Ԃ̐^�U��Ԃ�
	return g_bPause;
}

//======================================================================================================================
//	�Q�[�����[�h�̎擾����
//======================================================================================================================
int GetGameMode(void)
{
	// �Q�[�����[�h��Ԃ�
	return g_nGameMode;
}