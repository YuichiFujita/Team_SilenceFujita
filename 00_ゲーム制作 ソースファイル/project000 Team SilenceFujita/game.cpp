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

#include "ability.h"
#include "billboard.h"
#include "camera.h"
#include "Car.h"
#include "effect.h"
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
//	�v���g�^�C�v�錾
//**********************************************************************************************************************

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
	g_nSoundDJ = FUJITA_DJ_LABEL_ONE;			// �T�E���h������������
#endif

	//------------------------------------------------------------------------------------------------------------------
	//	�g�p����\�[�X�t�@�C���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// �J�[�u�̏��̏���������
	InitCurveInfo();

	// �J�[�u�e�L�X�g�̃��[�h����
	LoadCurveTxt();

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

	// �^�C�����̏�����
	InitTireMark();

	// �����@�̏�����
	InitWind();

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

	// �X�e�[�W�̃Z�b�g�A�b�v
	TxtSetStage();

#ifdef _DEBUG	// �f�o�b�O����
	// �G�f�B�b�g���C���̏�����
	InitEditmain();
#endif


	// ���e�̏�����
	InitBomb();
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

	// �^�C�����̏I��
	UninitTireMark();

	// �����@�̏I��
	UninitWind();

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


	// ���e�̏I��
	UninitBomb();
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

	if (g_nGameMode == GAMEMODE_EDIT)
	{ // �G�f�B�b�g���[�h�������ꍇ

		// �G�f�B�b�g���C���̍X�V
		UpdateEditmain();

		// �J�����̍X�V
		UpdateCamera();
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

			// ���̍X�V
			UpdateWind();

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
			
			// �G�t�F�N�g�̍X�V
			UpdateEffect();

			// �p�[�e�B�N���̍X�V
			UpdateParticle();
		}
		else
		{ // �|�[�Y��Ԃ̏ꍇ

			// �|�[�Y�̍X�V
			UpdatePause();
		}
	}

	// �J�����̍X�V
	UpdateCamera();

	// �r���{�[�h�̍X�V
	UpdateBillboard();

	// �̗̓o�[�̍X�V
	UpdateLife();

	// �^�C�}�[�̍X�V
	UpdateTimer();

	// �\�̓o�[�̍X�V
	UpdateAbility();

	// ���x�o�[�̍X�V
	UpdateVelocity();

	// �X�R�A�̍X�V
	UpdateScore();

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
#else
	if (g_bPause == false)
	{ // �|�[�Y��Ԃł͂Ȃ��ꍇ

		// ���C�g�̍X�V
		UpdateLight();

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

		// �v���C���[�̍X�V
		UpdatePlayer();

		// �^�C�����̍X�V
		UpdateTireMark();

		// �x�@�̍X�V
		UpdatePolice();

		// �Ԃ̍X�V����
		UpdateCar();

		// �l�Ԃ̍X�V
		UpdateHuman();

		// �I�u�W�F�N�g�̍X�V
		UpdateObject();

		// �r���{�[�h�̍X�V
		UpdateBillboard();

		// ���̍X�V
		UpdateWind();

		// �J�����̍X�V
		UpdateCamera();

		// �G�t�F�N�g�̍X�V
		UpdateEffect();

		// �p�[�e�B�N���̍X�V
		UpdateParticle();

		// �̗̓o�[�̍X�V
		UpdateLife();

		// �^�C�}�[�̍X�V
		UpdateTimer();

		// �\�̓o�[�̍X�V
		UpdateAbility();

		// ���x�o�[�̍X�V
		UpdateVelocity();

		// �X�R�A�̍X�V
		UpdateScore();

		// �e�̍X�V
		UpdateShadow();
	}
	else
	{ // �|�[�Y��Ԃ̏ꍇ

		// �|�[�Y�̍X�V
		UpdatePause();
	}
#endif


	// ���e�̍X�V
	UpdateBomb();
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

	// ���b�V���V�����_�[�̕`��
	DrawMeshCylinder();

	// ���b�V���t�B�[���h�̕`��
	DrawMeshField();

	// ���b�V���E�H�[���̕`��
	DrawMeshWall();

	// �e�̕`��
	DrawShadow();

	// �v���C���[�̕`��
	DrawPlayer();

	// �^�C�����̕`��
	DrawTireMark();

	// �x�@�̕`��
	DrawPolice();

	// �Ԃ̕`�揈��
	DrawCar();

	// �I�u�W�F�N�g�̕`��
	DrawObject();

	// �l�Ԃ̕`��
	DrawHuman();

	// �r���{�[�h�̕`��
	DrawBillboard();

	// ���̕`��
	DrawWind();

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


	// ���e�̕`��
	DrawBomb();

	//------------------------------------------------------------------------------------------------------------------
	//	�}�b�v�J�����̕`��
	//------------------------------------------------------------------------------------------------------------------
	// �J�����̐ݒ�
	SetCamera(CAMERATYPE_MAP);

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);	// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	// ���b�V���t�B�[���h�̕`��
	DrawMeshField();

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

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

//================================
//�O�ς̓����蔻��
//================================
void CollisionOuterProduct(D3DXVECTOR3 *Targetpos, D3DXVECTOR3 *TargetposOld, D3DXVECTOR3 WallRpos, D3DXVECTOR3 WallLpos, bool *bUse, int *nShadow)
{
	D3DXVECTOR3 vecMove, vecLine, vecTopos, posCross;	//�O�ς̕ϐ�
	float fRate;										//�����̕ϐ�

	//�e�̋O�Ղ𑪂�
	vecMove.x = (Targetpos->x - TargetposOld->x);
	vecMove.y = (Targetpos->y - TargetposOld->y);
	vecMove.z = (Targetpos->z - TargetposOld->z);

	//�ǂ̋��E���𑪂�
	vecLine.x = (WallRpos.x - WallLpos.x);
	vecLine.y = (WallRpos.y - WallLpos.y);
	vecLine.z = (WallRpos.z - WallLpos.z);

	//�ǂ̎n�_����e�̈ʒu�̋����𑪂�
	vecTopos.x = (Targetpos->x - WallLpos.x);
	vecTopos.y = (Targetpos->y - WallLpos.y);
	vecTopos.z = (Targetpos->z - WallLpos.z);

	//���������߂�
	fRate = ((vecTopos.z * vecMove.x) - (vecTopos.x * vecMove.z)) / ((vecLine.z * vecMove.x) - (vecLine.x * vecMove.z));

	//��_�����߂�
	posCross.x = vecLine.x * fRate - WallLpos.x;
	posCross.y = vecLine.y * fRate - WallLpos.y;
	posCross.z = vecLine.z * fRate - WallLpos.z;

	if ((vecLine.z * vecTopos.x) - (vecLine.x * vecTopos.z) < 0.0f)
	{//���E���𒴂����ꍇ
		if (fRate >= 0.0f && fRate <= 1.0f)
		{//������0.0f�`1.0f�̊Ԃ�����(���E���𒴂���)�ꍇ
			//�ʒu��ݒ肷��
			Targetpos = &posCross;
		}
	}
}