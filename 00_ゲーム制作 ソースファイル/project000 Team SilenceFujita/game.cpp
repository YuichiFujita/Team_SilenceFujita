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
#include "bomb.h"
#include "bonus.h"
#include "buildtimer.h"
#include "camera.h"
#include "Car.h"
#include "Combo.h"
#include "Countdown.h"
#include "effect.h"
#include "flash.h"
#include "Human.h"
#include "gate.h"
#include "icon.h"
#include "item.h"
#include "junk.h"
#include "life.h"
#include "light.h"
#include "map.h"
#include "meshdome.h"
#include "meshcylinder.h"
#include "meshfield.h"
#include "meshwall.h"
#include "particle.h"
#include "pause.h"
#include "shadow.h"
#include "start.h"
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
#endif

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define END_GAME_TIME	(120)		// �Q�[���I���܂ł̗]�C�t���[��

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
GAMESTATE   g_gameState;			// �Q�[���̏��
RESULTSTATE g_resultState;			// ���U���g�̏��
int         g_nCounterGameState;	// ��ԊǗ��J�E���^�[
bool        g_bPause;				// �|�[�Y��Ԃ� ON / OFF
bool        g_bGameEnd;				// ���[�h�̑J�ڏ�
int         g_nGameMode;			// �G�f�B�b�g�� ON / OFF

//======================================================================================================================
//	�Q�[����ʂ̏���������
//======================================================================================================================
void InitGame(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	�Q�[���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// �O���[�o���ϐ���������
	g_gameState         = GAMESTATE_START;		// �Q�[���̏��
	g_resultState       = RESULTSTATE_NONE;		// ���U���g�̏��
	g_nCounterGameState = 0;					// ��ԊǗ��J�E���^�[
	g_bPause            = false;				// �|�[�Y��Ԃ� ON / OFF
	g_bGameEnd          = false;				// ���[�h�̑J�ڏ�
	g_nGameMode         = GAMEMODE_PLAY;		// �G�f�B�b�g�� ON / OFF

	//------------------------------------------------------------------------------------------------------------------
	//	�g�p����\�[�X�t�@�C���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// �V�C�̏���������
	InitWeather();

	// �e�̏�����
	InitShadow();

	// �A�C�R���̏�����
	InitIcon();

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

	// ���ꂫ�̏�����
	InitJunk();

	// �A�C�e���̏�����
	InitItem();

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

	// �Č��z�^�C�}�[�̏�����
	InitBuildtimer();

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

	// �R���{�̏�����
	InitCombo();

	// �J�E���g�_�E���̏�����
	InitCountDown();

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

	// �}�b�v�̏�����
	InitMap();

	// ���x�o�[�̏�����
	InitVelocity();

	// �X�R�A�̏�����
	InitScore();

	// �|�[�Y�̏�����
	InitPause();

	// �t���b�V���̏�����
	InitFlash();

	// �X�^�[�g�̏�����
	InitStart();

	// �t�@�C�������[�h����S�̏���
	LoadFileChunk
	( // ����
		true,	// �Ԃ̃J�[�u
		true,	// �l�Ԃ̃J�[�u
		true,	// �X�e�[�W
		true,	// �����蔻��
		true,	// �e
		true,	// �I�u�W�F�N�g
		true,	// AI
		true	// �A�C�R��
	);

	// �v���C���[�̈ʒu�E�����̐ݒ�
	SetPositionPlayer(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// �v���C���[�̃Q�[�g�̐ݒ菈��
	SetPlayerGate();

	//���C��BGM�̍Đ�
	if (GetSoundType(SOUND_TYPE_MAIN_BGM) == true)
	{
		//�T�E���h�i�Q�[��BGM�j�̍Đ�
		PlaySound(SOUND_LABEL_BGM_GAME_000);
	}

	//���ʉ�BGM�̍Đ�
	if (GetSoundType(SOUND_TYPE_SUB_BGM) == true)
	{
		//���ʉ��T�E���h�i���h�ԁj�̍Đ�
		PlaySound(SOUND_LABEL_BGM_FIRECAR_000);

		//���ʉ��T�E���h�i�Ă����j�̍Đ�
		PlaySound(SOUND_LABEL_BGM_YAKIIMO_000);

		//���ʉ��T�E���h�i�\���ԁj�̍Đ�
		PlaySound(SOUND_LABEL_BGM_BOUSOUCAR_000);

		//���ʉ��T�E���h�i�I���J�[�j�̍Đ�
		PlaySound(SOUND_LABEL_BGM_SENKYOCAR_000);

		//���ʉ��T�E���h�i�x�@�j�̍Đ�
		PlaySound(SOUND_LABEL_BGM_POLICE_000);

		//���ʉ��T�E���h�i���s���j�̍Đ�
		PlaySound(SOUND_LABEL_BGM_CAR_000);

		//�Ԃ̌��ʉ��T�E���h�̉��ʂ�0�ɕύX
		SetSoundVolume(SOUND_LABEL_BGM_FIRECAR_000, 0.0f);
		SetSoundVolume(SOUND_LABEL_BGM_YAKIIMO_000, 0.0f);
		SetSoundVolume(SOUND_LABEL_BGM_BOUSOUCAR_000, 0.0f);
		SetSoundVolume(SOUND_LABEL_BGM_SENKYOCAR_000, 0.0f);
		SetSoundVolume(SOUND_LABEL_BGM_POLICE_000, 0.0f);
		SetSoundVolume(SOUND_LABEL_BGM_CAR_000, 1.0f);
	}

#ifdef _DEBUG	// �f�o�b�O����
	// �G�f�B�b�g���C���̏�����6
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

	// �A�C�R���̏I��
	UninitIcon();

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

	// ���ꂫ�̏I��
	UninitJunk();

	// �A�C�e���̏I��
	UninitItem();

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

	// �Č��z�^�C�}�[�̏I��
	UninitBuildtimer();

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

	// �R���{�̏I��
	UninitCombo();

	// �J�E���g�_�E���̏I��
	UninitCountDown();

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

	// �}�b�v�̏I��
	UninitMap();

	// ���x�o�[�̏I��
	UninitVelocity();

	// �X�R�A�̏I��
	UninitScore();

	// �|�[�Y�̏I��
	UninitPause();

	// �t���b�V���̏I��
	UninitFlash();

	// �X�^�[�g�̏I��
	UninitStart();

	// BGM�̒�~
	StopSound();

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
			SetGameState(GAMESTATE_END, END_GAME_TIME);	// �I�����

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

				// �R���{�̔{������(���_�̑������)
				MagnificCombo(COMBO_INTERRUPTION);

				// ���[�h�I�� (���U���g��ʂɈڍs)
				SetFade(MODE_RESULT);
			}
		}

		// �����𔲂���
		break;
	}

	// ���C�g�̍X�V
	UpdateLight();

	if (g_nGameMode == GAMEMODE_EDIT)
	{ // �G�f�B�b�g���[�h�������ꍇ

#ifdef _DEBUG	// �f�o�b�O����

		// �J�����̍X�V
		UpdateCamera();

		// �G�f�B�b�g���C���̍X�V
		UpdateEditmain();
#endif
	}
	else
	{ // �Q�[�����[�h�������ꍇ

		if (g_bPause == false)
		{ // �|�[�Y��Ԃł͂Ȃ��ꍇ

			// �X�^�[�g�̍X�V
			UpdateStart();

			// �V�C�̐ݒ菈��
			SetWeather();

			// �V�C�̍X�V����
			UpdateWeather();

			// �t���b�V���̍X�V����
			UpdateFlash();

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

			// �v���C���[�̃Q�[���X�V
			UpdateGamePlayer();

			// �J�����̍X�V
			UpdateCamera();

			// �^�C�����̍X�V
			UpdateTireMark();

			// �x�@�̍X�V
			UpdatePolice();

			// �I�u�W�F�N�g�̍X�V
			UpdateObject();

			// ���ꂫ�̍X�V
			UpdateJunk();

			// �A�C�e���̍X�V
			UpdateItem();

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

			// �Č��z�^�C�}�[�̍X�V
			UpdateBuildtimer();

			// �̗̓o�[�̍X�V
			UpdateLife();
#if 0
			// �^�C�}�[�̍X�V
			UpdateTimer();
#endif
			// �J�E���g�_�E���̍X�V
			UpdateCountDown();

			// �\�̓o�[�̍X�V
			UpdateAbility();

			// �}�b�v�̍X�V
			UpdateMap();

			// ���x�o�[�̍X�V
			UpdateVelocity();

			// �X�R�A�̍X�V
			UpdateScore();

			// �R���{�̍X�V
			UpdateCombo();

			// �{�[�i�X�̍X�V����
			UpdateBonus();

			// �A�C�R���̍X�V
			UpdateIcon();

			// �e�̍X�V
			UpdateShadow();

			// �x�@�̒ǉ�����
			AddPolice();
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

	// �Č��z�^�C�}�[�̕`��
	DrawBuildtimer();

	// �I�u�W�F�N�g�̕`��
	DrawObject();

	// ���ꂫ�̕`��
	DrawJunk();

	// �A�C�e���̕`��
	DrawItem();

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

#if 1
	//------------------------------------------------------------------------------------------------------------------
	//	UI�̕`��
	//------------------------------------------------------------------------------------------------------------------
	// �J�����̐ݒ�
	SetCamera(CAMERATYPE_UI);

	// �}�b�v�̕`��
	DrawMap();

	//------------------------------------------------------------------------------------------------------------------
	//	�}�b�v�J�����̕`��
	//------------------------------------------------------------------------------------------------------------------
	// �J�����̐ݒ�
	SetCamera(CAMERATYPE_MAP);

	// ���b�V���t�B�[���h�̕`��
	DrawMeshField(true);

	// �A�C�R���̕`��
	DrawIcon();

	//------------------------------------------------------------------------------------------------------------------
	//	UI�̕`��
	//------------------------------------------------------------------------------------------------------------------
	// �J�����̐ݒ�
	SetCamera(CAMERATYPE_UI);

	// �̗̓o�[�̕`��
	DrawLife();

	// �^�C�}�[�̕`��
	DrawTimer();

	// �\�̓o�[�̕`��
	DrawAbility();

	// ���x�o�[�̕`��
	DrawVelocity();

	// �R���{�̕`��
	DrawCombo();

	// �X�R�A�̕`��
	DrawScore();

	// �{�[�i�X�̕`��
	DrawBonus();

	// 2D�G�t�F�N�g�̕`��
	Draw2DEffect();

	// 2D�p�[�e�B�N���̕`��
	Draw2DParticle();
#endif

	// �t���b�V���̕`��
	DrawFlash();

	// �X�^�[�g�̕`��
	DrawStart();

	// �J�E���g�_�E���̕`��
	DrawCountDown();

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
GAMESTATE *GetGameState(void)
{
	// �Q�[���̏�Ԃ�Ԃ�
	return &g_gameState;
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