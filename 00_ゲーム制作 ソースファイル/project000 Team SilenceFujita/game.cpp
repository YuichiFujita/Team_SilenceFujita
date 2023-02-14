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
#include "calculation.h"

#include "game.h"

#include "2Deffect.h"
#include "2Dparticle.h"
#include "ability.h"
#include "billboard.h"
#include "bonus.h"
#include "camera.h"
#include "Car.h"
#include "effect.h"
#include "gate.h"
#include "life.h"
#include "light.h"
#include "meshdome.h"
#include "meshcylinder.h"
#include "meshfield.h"
#include "meshwall.h"
#include "particle.h"
#include "pause.h"
#include "shadow.h"
#include "object.h"
#include "player.h"
#include "Police.h"
#include "score.h"
#include "timer.h"
#include "tiremark.h"
#include "velocity.h"
#include "weather.h"
#include "wind.h"

#ifdef _DEBUG	// �f�o�b�O����
#include "Editmain.h"
#include "SoundDJ.h"
#endif

#include "bomb.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define RESULT_TIME		(120)		// ���U���g�܂ł̗]�C�t���[��

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
GAMESTATE   g_gameState;			// �Q�[���̏��
RESULTSTATE g_resultState;			// ���U���g�̏��
int         g_nCounterGameState;	// ��ԊǗ��J�E���^�[
bool        g_bPause;				// �|�[�Y��Ԃ� ON / OFF
bool        g_bGameEnd;				// ���[�h�̑J�ڏ�
int         g_nGameMode;			// �G�f�B�b�g�� ON / OFF
int		    g_nSoundDJ;				// ���ݗ���Ă���T�E���h

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
	g_resultState       = RESULTSTATE_NONE;		// ���U���g�̏��
	g_nCounterGameState = 0;					// ��ԊǗ��J�E���^�[
	g_bPause            = false;				// �|�[�Y��Ԃ� ON / OFF
	g_bGameEnd          = false;				// ���[�h�̑J�ڏ�
	g_nGameMode         = GAMEMODE_PLAY;		// �G�f�B�b�g�� ON / OFF

#ifdef _DEBUG	// �f�o�b�O����
	g_nSoundDJ = FUJITA_DJ_LABEL_ONE;			// �T�E���h������������
#endif

	//------------------------------------------------------------------------------------------------------------------
	//	�g�p����\�[�X�t�@�C���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// �V�C�̏���������
	InitWeather();

	// �e�̏�����
	InitShadow();

	// �v���C���[�̏�����
	InitPlayer();

	// �x�@�̏�����
	InitPolice();

	// �I�u�W�F�N�g�̏�����
	InitObject();

	// �Ԃ̏�����
	InitCar();

	// �l�Ԃ̏�����
	InitHuman();

	// �Q�[�g�̏�����
	InitGate();

	// �J�����̏�����
	InitCamera();

	// ���C�g�̏�����
	InitLight();

	// ���b�V���h�[���̏�����
	InitMeshDome();

	// ���b�V���V�����_�[�̏�����
	InitMeshCylinder();

	// ���b�V���t�B�[���h�̏�����
	InitMeshField();

	// ���b�V���E�H�[���̏�����
	InitMeshWall();

	// �r���{�[�h�̏�����
	InitBillboard();

	// �{�[�i�X�̏�����
	InitBonus();

	// 2D�G�t�F�N�g�̏�����
	Init2DEffect();

	// 2D�p�[�e�B�N���̏�����
	Init2DParticle();

	// �^�C�����̏�����
	InitTireMark();

	// �����@�̏�����
	InitWind();

	// ���e�̏�����
	InitBomb();

	// �G�t�F�N�g�̏�����
	InitEffect();

	// �p�[�e�B�N���̏�����
	InitParticle();

	// �̗̓o�[�̏�����
	InitLife();

	// �^�C�}�[�̏�����
	InitTimer();

	// �\�̓o�[�̏�����
	InitAbility();

	// ���x�o�[�̏�����
	InitVelocity();

	// �X�R�A�̏�����
	InitScore();

	// �|�[�Y�̏�����
	InitPause();

	// �t�@�C�������[�h����S�̏���
	LoadFileChunk
	( // ����
		true,	// �Ԃ̃J�[�u
		true,	// �l�Ԃ̃J�[�u
		true,	// �X�e�[�W
		true,	// �����蔻��
		true,	// �e
		true,	// �I�u�W�F�N�g
		true	// AI
	);

	// �Q�[�g�̐ݒ菈��
	SetGate(D3DXVECTOR3(1500.0f, 10.0f, 11500.0f), D3DXVECTOR3(0.0f, D3DXToRadian(0), 0.0f), ROTSTATE_0);

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
	// �V�C�̏I������
	UninitWeather();

	// �e�̏I��
	UninitShadow();

	// �v���C���[�̏I��
	UninitPlayer();

	// �x�@�̏I��
	UninitPolice();

	// �I�u�W�F�N�g�̏I��
	UninitObject();

	// �Ԃ̏I��
	UninitCar();

	// �l�Ԃ̏I��
	UninitHuman();

	// �Q�[�g�̏I��
	UninitGate();

	// �J�����̏I��
	UninitCamera();

	// ���C�g�̏I��
	UninitLight();

	// ���b�V���h�[���̏I��
	UninitMeshDome();

	// ���b�V���V�����_�[�̏I��
	UninitMeshCylinder();

	// ���b�V���t�B�[���h�̏I��
	UninitMeshField();

	// ���b�V���E�H�[���̏I��
	UninitMeshWall();

	// �r���{�[�h�̏I��
	UninitBillboard();

	// �{�[�i�X�̏I��
	UninitBonus();

	// 2D�G�t�F�N�g�̏I��
	Uninit2DEffect();

	// 2D�p�[�e�B�N���̏I��
	Uninit2DParticle();

	// �^�C�����̏I��
	UninitTireMark();

	// �����@�̏I��
	UninitWind();

	// ���e�̏I��
	UninitBomb();

	// �G�t�F�N�g�̏I��
	UninitEffect();

	// �p�[�e�B�N���̏I��
	UninitParticle();

	// �̗̓o�[�̏I��
	UninitLife();

	// �^�C�}�[�̏I��
	UninitTimer();

	// �\�̓o�[�̏I��
	UninitAbility();

	// ���x�o�[�̏I��
	UninitVelocity();

	// �X�R�A�̏I��
	UninitScore();

	// �|�[�Y�̏I��
	UninitPause();

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
	if (g_bGameEnd == false)
	{ // �J�ڐݒ肪����Ă��Ȃ��ꍇ

		if (GetExit().bExit == true || GetTimerState() == TIMERSTATE_END || GetPlayer()->bUse == false)
		{ // ���U���g�ɑJ�ڂ���������������ꍇ

			// �J�ڐݒ肪���ꂽ��Ԃɂ���
			g_bGameEnd = true;

			// �Q�[����ʂ̏�Ԑݒ�
			SetGameState(GAMESTATE_END, RESULT_TIME);	// �I�����

			if (GetExit().bExit == true)
			{ // �N���A�ɐ��������ꍇ

				// ���U���g���N���A������Ԃɂ���
				g_resultState = RESULTSTATE_CLEAR;

				// �T�E���h�̍Đ�
				//PlaySound(SOUND_LABEL_SE_RES_00);		// SE (���U���g�ڍs00)
			}
			else if (GetTimerState() == TIMERSTATE_END)
			{ // �^�C���A�E�g���Ă���ꍇ

				// ���U���g���N���A���s��Ԃɂ���
				g_resultState = RESULTSTATE_TIMEOVER;

				// �T�E���h�̍Đ�
				//PlaySound(SOUND_LABEL_SE_RES_01);		// SE (���U���g�ڍs01)
			}
			else if (GetPlayer()->bUse == false)
			{ // �v���C���[�����S���Ă���ꍇ

				// ���U���g���N���A���s��Ԃɂ���
				g_resultState = RESULTSTATE_LIFEOVER;

				// �T�E���h�̍Đ�
				//PlaySound(SOUND_LABEL_SE_RES_01);		// SE (���U���g�ڍs01)
			}
		}
	}

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

		if (UpdateAllClear(g_resultState) == true)
		{ // �S�ẴA�b�v�f�[�g���I����Ă����ꍇ

			if (g_nCounterGameState > 0)
			{ // �J�E���^�[�� 0���傫���ꍇ

				// �J�E���^�[�����Z
				g_nCounterGameState--;
			}
			else
			{ // �J�E���^�[�� 0�ȉ��̏ꍇ

				// ���[�h�I�� (���U���g��ʂɈڍs)
				SetFade(MODE_RESULT);
			}
		}

		// �����𔲂���
		break;
	}

	// ���C�g�̍X�V
	UpdateLight();

	// �J�����̍X�V
	UpdateCamera();

	if (g_nGameMode == GAMEMODE_EDIT)
	{ // �G�f�B�b�g���[�h�������ꍇ

#ifdef _DEBUG	// �f�o�b�O����
		// �G�f�B�b�g���C���̍X�V
		UpdateEditmain();
#endif
	}
	else
	{ // �Q�[�����[�h�������ꍇ

		if (g_bPause == false)
		{ // �|�[�Y��Ԃł͂Ȃ��ꍇ

			// �V�C�̐ݒ菈��
			SetWeather();

			// �V�C�̍X�V����
			UpdateWeather();

			// ���b�V���h�[���̍X�V
			UpdateMeshDome();

			// ���b�V���V�����_�[�̍X�V
			UpdateMeshCylinder();

			// ���b�V���t�B�[���h�̍X�V
			UpdateMeshField();

			// ���b�V���E�H�[���̍X�V
			UpdateMeshWall();

			// �����@�̍X�V
			UpdateWind();

			// ���e�̍X�V
			UpdateBomb();

			// �v���C���[�̍X�V
			UpdatePlayer();

			// �^�C�����̍X�V
			UpdateTireMark();

			// �x�@�̍X�V
			UpdatePolice();

			// �I�u�W�F�N�g�̍X�V
			UpdateObject();

			// �Ԃ̍X�V����
			UpdateCar();

			// �l�Ԃ̍X�V
			UpdateHuman();

			// �Q�[�g�̍X�V
			UpdateGate();
			
			// �G�t�F�N�g�̍X�V
			UpdateEffect();

			// �p�[�e�B�N���̍X�V
			UpdateParticle();

			// 2D�G�t�F�N�g�̍X�V
			Update2DEffect();

			// 2D�p�[�e�B�N���̍X�V
			Update2DParticle();

			// �r���{�[�h�̍X�V
			UpdateBillboard();

			// �̗̓o�[�̍X�V
			UpdateLife();

#if 1
			// �^�C�}�[�̍X�V
			UpdateTimer();
#endif

			// �\�̓o�[�̍X�V
			UpdateAbility();

			// ���x�o�[�̍X�V
			UpdateVelocity();

			// �X�R�A�̍X�V
			UpdateScore();

			// �{�[�i�X�̍X�V����
			UpdateBonus();

			// �e�̍X�V
			UpdateShadow();
		}
		else
		{ // �|�[�Y��Ԃ̏ꍇ

			// �|�[�Y�̍X�V
			UpdatePause();
		}
	}

#ifdef _DEBUG	// �f�o�b�O����
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
	if (GetKeyboardTrigger(DIK_F4) == true)
	{ // [F4] �������ꂽ�ꍇ

		// �T�E���h��ς���
		g_nSoundDJ = (g_nSoundDJ + 1) % SOUND_DJ_LABEL_MAX;
	}
	if (GetKeyboardTrigger(DIK_F5) == true)
	{ // [F5] �������ꂽ�ꍇ

		// �T�E���h�̒�~
		StopSoundDJ();

		// �T�E���h�𗬂�
		PlaySound(g_nSoundDJ, true);
	}
	if (GetKeyboardTrigger(DIK_F6) == true)
	{ // [F6]�������ꂽ�ꍇ

		// �T�E���h�̒�~
		StopSoundDJ((SOUND_DJ_LABEL)g_nSoundDJ);
	}
	if (GetKeyboardTrigger(DIK_F9) == true)
	{ // [F9] �������ꂽ�ꍇ

		// �����蔻��̕ۑ�
		TxtSaveCollision();
	}
#endif
}

//======================================================================================================================
//	�Q�[����ʂ̕`�揈��
//======================================================================================================================
void DrawGame(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	//------------------------------------------------------------------------------------------------------------------
	//	���C���J�����̕`��
	//------------------------------------------------------------------------------------------------------------------
	// �J�����̐ݒ�
	SetCamera(CAMERATYPE_MAIN);

	// ���b�V���h�[���̕`��
	DrawMeshDome();

	// ���̕`�揈��
	DrawThunder();

	// ���b�V���V�����_�[�̕`��
	DrawMeshCylinder();

	// ���b�V���t�B�[���h�̕`��
	DrawMeshField(false);

	// ���b�V���E�H�[���̕`��
	DrawMeshWall();

	// �e�̕`��
	DrawShadow();

	// �v���C���[�̕`��
	DrawPlayer();

	// �^�C�����̕`��
	DrawTireMark();

	// �I�u�W�F�N�g�̕`��
	DrawObject();

	// �x�@�̕`��
	DrawPolice();

	// �Ԃ̕`�揈��
	DrawCar();

	// �l�Ԃ̕`��
	DrawHuman();

	// �Q�[�g�̕`��
	DrawGate();

	// �r���{�[�h�̕`��
	DrawBillboard();

	// �����@�̕`��
	DrawWind();

	// ���e�̕`��
	DrawBomb();

	// �G�t�F�N�g�̕`��
	DrawEffect();

	// �p�[�e�B�N���̕`��
	DrawParticle();

	// �V�C�̕`�揈��
	DrawWeather();
	
#ifdef _DEBUG	// �f�o�b�O����
	if (g_nGameMode == GAMEMODE_EDIT)
	{ // �G�f�B�b�g���[�h�̏ꍇ

		// �G�f�B�b�g���C���̕`�揈��
		DrawEditmain();
	}
#endif

	//------------------------------------------------------------------------------------------------------------------
	//	�}�b�v�J�����̕`��
	//------------------------------------------------------------------------------------------------------------------
	// �J�����̐ݒ�
	SetCamera(CAMERATYPE_MAP);

	// ���b�V���t�B�[���h�̕`��
	DrawMeshField(true);

	//------------------------------------------------------------------------------------------------------------------
	//	UI�̕`��
	//------------------------------------------------------------------------------------------------------------------
	// �J�����̐ݒ�
	SetCamera(CAMERATYPE_UI);

#if 1
	// �̗̓o�[�̕`��
	DrawLife();

	// �^�C�}�[�̕`��
	DrawTimer();

	// �\�̓o�[�̕`��
	DrawAbility();

	// ���x�o�[�̕`��
	DrawVelocity();

	// �X�R�A�̕`��
	DrawScore();

	// �{�[�i�X�̕`��
	DrawBonus();

	// 2D�G�t�F�N�g�̕`��
	Draw2DEffect();

	// 2D�p�[�e�B�N���̕`��
	Draw2DParticle();
#endif

	if (g_bPause == true)
	{ // �|�[�Y��Ԃ̏ꍇ

		// �|�[�Y�̕`��
		DrawPause();
	}
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
//	�Q�[����Ԃ̎擾����
//======================================================================================================================
RESULTSTATE GetResultState(void)
{
	// �Q�[���I�����̏�Ԃ�Ԃ�
	return g_resultState;
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