//======================================================================================================================
//
//	���͏��� [input.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "input.h"
#include "effect.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
// �L�[�{�[�h�p
#define NUM_KEY_MAX			(256)	// �L�[�̍ő吔

// �}�E�X�p
#define NUM_MOUSEKEY_MAX	(4)		// �}�E�X�L�[�̍ő吔

// �p�b�h�p
#define MAX_PLAYER			(4)		// �v���C���[�̍ő吔
#define VIB_TIME			(20)	// �o�C�u�̌p������
#define VIB_TIME_DEATH		(100)	// ���S���̃o�C�u�p������

#define VIB_DAMAGE_LV		((short)(USHRT_MAX * 0.6f))		// �_���[�W���̐U�����x��
#define VIB_BIG_DAMAGE_LV	((short)(USHRT_MAX * 0.8f))		// ��_���[�W���̐U�����x��
#define VIB_DEATH_LV		(USHRT_MAX)						// ���S���̐U�����x��

//**********************************************************************************************************************
//	�\���̒�` (VIBRATION)
//**********************************************************************************************************************
typedef struct
{
	_XINPUT_VIBRATION vibration;	// �p�b�h�̃o�C�u���
	VIBSTATE          state;		// �o�C�u�g�p��
	int               nTime;		// �o�C�u�g�p����
}VIBRATION;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
// �L�[�{�[�h
HRESULT InitKeyboard(HWND hWnd);	// �L�[�{�[�h�̏���������
void UninitKeyboard(void);			// �L�[�{�[�h�̏I������
void UpdateKeyboard(void);			// �L�[�{�[�h�̍X�V����

// �}�E�X
HRESULT InitMouse(HWND hWnd);		// �}�E�X�̏���������
void UninitMouse(void);				// �}�E�X�̏I������
void UpdateMouse(void);				// �}�E�X�̍X�V����

// �p�b�h
HRESULT InitJoyPad(void);			// �p�b�h�̏���������
void UninitJoyPad(void);			// �p�b�h�̏I������
void UpdateJoyPad(void);			// �p�b�h�̍X�V����

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
// ���̓f�o�C�X�p
LPDIRECTINPUT8 g_pInput = NULL;								// DirectInput �I�u�W�F�N�g�ւ̃|�C���^

// �L�[�{�[�h�p
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;					// ���̓f�o�C�X (�L�[�{�[�h) �ւ̃|�C���^
BYTE                 g_aKeyState[NUM_KEY_MAX];				// �L�[�{�[�h�̃v���X���
BYTE                 g_aKeyStateTrigger[NUM_KEY_MAX];		// �L�[�{�[�h�̃g���K�[���
BYTE                 g_aKeyStateRelease[NUM_KEY_MAX];		// �L�[�{�[�h�̃����[�X���

// �}�E�X�p
LPDIRECTINPUTDEVICE8 g_pDevMouse = NULL;					// ���̓f�o�C�X (�}�E�X) �ւ̃|�C���^
DIMOUSESTATE         g_mouseState;							// �}�E�X�̃v���X���
DIMOUSESTATE         g_mouseStateTrigger;					// �}�E�X�̃g���K�[���
DIMOUSESTATE         g_mouseStateRelease;					// �}�E�X�̃����[�X���

// �p�b�h�p
VIBRATION    g_aVibration[MAX_PLAYER];						// �p�b�h�̃o�C�u���
XINPUT_STATE g_aJoyKeyState[MAX_PLAYER];					// �p�b�h�̃v���X���
XINPUT_STATE g_aJoyKeyStateTrigger[MAX_PLAYER];				// �p�b�h�̃g���K�[���
XINPUT_STATE g_aJoyKeyStateRelease[MAX_PLAYER];				// �p�b�h�̃����[�X���

//**********************************************************************************************************************
//	���̓f�o�C�X����
//**********************************************************************************************************************
//======================================================================================================================
//	���̓f�o�C�X�̏���������
//======================================================================================================================
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInput �I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �L�[�{�[�h�̏�����
	if (FAILED(InitKeyboard(hWnd)))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �}�E�X�̏�����
	if (FAILED(InitMouse(hWnd)))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �p�b�h�̏�����
	InitJoyPad();

	// ������Ԃ�
	return S_OK;
}

//======================================================================================================================
//	���̓f�o�C�X�̏I������
//======================================================================================================================
void UninitInput(void)
{
	// �L�[�{�[�h�̏I��
	UninitKeyboard();

	// �}�E�X�̏I��
	UninitMouse();

	// �p�b�h�̏I��
	UninitJoyPad();

	// DirectInput �I�u�W�F�N�g�̔j��
	if (g_pInput != NULL)
	{ // DirectInput �I�u�W�F�N�g�� NULL�ł͂Ȃ��ꍇ

		g_pInput->Release();
		g_pInput = NULL;
	}
}

//======================================================================================================================
//	���̓f�o�C�X�̍X�V����
//======================================================================================================================
void UpdateInput(void)
{
	// �L�[�{�[�h�̍X�V
	UpdateKeyboard();

	// �}�E�X�̍X�V
	UpdateMouse();

	// �p�b�h�̍X�V
	UpdateJoyPad();
}

//**********************************************************************************************************************
//	�L�[�{�[�h����
//**********************************************************************************************************************
//======================================================================================================================
//	�L�[�{�[�h�̏���������
//======================================================================================================================
HRESULT InitKeyboard(HWND hWnd)
{
	// ���̓f�o�C�X (�L�[�{�[�h) �̐���
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{ // �ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{ // �ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
	g_pDevKeyboard->Acquire();

	// ������Ԃ�
	return S_OK;
}

//======================================================================================================================
//	�L�[�{�[�h�̏I������
//======================================================================================================================
void UninitKeyboard(void)
{
	// ���̓f�o�C�X (�L�[�{�[�h) �̔j��
	if (g_pDevKeyboard != NULL)
	{ // ���̓f�o�C�X�� NULL �łȂ��ꍇ

		g_pDevKeyboard->Unacquire();	// �L�[�{�[�h�ւ̃A�N�Z�X�������
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}
}

//======================================================================================================================
//	�L�[�{�[�h�̍X�V����
//======================================================================================================================
void UpdateKeyboard(void)
{
	// �ϐ��z���錾
	BYTE aKeyState[NUM_KEY_MAX];		// �L�[�{�[�h�̓��͏��

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{ // �擾�ɐ��������ꍇ

		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{ // �L�[�̍ő吔���J��Ԃ�

			// �L�[�{�[�h�̃����[�X����ۑ�
			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] | aKeyState[nCntKey]) ^ aKeyState[nCntKey];

			// �L�[�{�[�h�̃g���K�[����ۑ�
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			// �L�[�{�[�h�̃v���X����ۑ�
			g_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{ // �擾�Ɏ��s�����ꍇ

		// �L�[�{�[�h�ւ̃A�N�Z�X�����ēx�l��
		g_pDevKeyboard->Acquire();
	}
}

//======================================================================================================================
//	�L�[�{�[�h�̃v���X�̎擾����
//======================================================================================================================
bool GetKeyboardPress(int nKey)
{
	// �^�U��Ԃ� (�O�����Z�q)									// ���������^�Ȃ� true �A�U�Ȃ� false ��Ԃ�
	return (g_aKeyState[nKey] & 0x80) ? true : false;			// �� �������F() �̒�
}

//======================================================================================================================
//	�L�[�{�[�h�̃g���K�[�̎擾����
//======================================================================================================================
bool GetKeyboardTrigger(int nKey)
{
	// �^�U��Ԃ� (�O�����Z�q)									// ���������^�Ȃ� true �A�U�Ȃ� false ��Ԃ�
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;	// �� �������F() �̒�
}

//======================================================================================================================
//	�L�[�{�[�h�̃����[�X�̎擾����
//======================================================================================================================
bool GetKeyboardRelease(int nKey)
{
	// �^�U��Ԃ� (�O�����Z�q)									// ���������^�Ȃ� true �A�U�Ȃ� false ��Ԃ�
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;	// �� �������F() �̒�
}

//**********************************************************************************************************************
//	�}�E�X����
//**********************************************************************************************************************
//======================================================================================================================
//	�}�E�X�̏���������
//======================================================================================================================
HRESULT InitMouse(HWND hWnd)
{
	// �ϐ���錾
	DIPROPDWORD diprop;		// �f�o�C�X�̐ݒ�p

	// ���̓f�o�C�X (�}�E�X) �̐���
	if (FAILED(g_pInput->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL)))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse)))
	{ // �ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	if (FAILED(g_pDevMouse->SetCooperativeLevel(hWnd, (DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))))
	{ // �ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �f�o�C�X�̐ݒ�
	diprop.diph.dwSize       = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj        = 0;
	diprop.diph.dwHow        = DIPH_DEVICE;
	diprop.dwData            = DIPROPAXISMODE_REL;		// ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j

	if (FAILED(g_pDevMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{ // �f�o�C�X�̐ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �}�E�X�ւ̃A�N�Z�X�����l��
	g_pDevMouse->Acquire();

	// ������Ԃ�
	return S_OK;
}

//======================================================================================================================
//	�}�E�X�̏I������
//======================================================================================================================
void UninitMouse(void)
{
	// ���̓f�o�C�X (�}�E�X) �̔j��
	if (g_pDevMouse != NULL)
	{ // ���̓f�o�C�X�� NULL �łȂ��ꍇ

		g_pDevMouse->Unacquire();	// �}�E�X�ւ̃A�N�Z�X�������
		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}
}

//======================================================================================================================
//	�}�E�X�̍X�V����
//======================================================================================================================
void UpdateMouse(void)
{
	// �ϐ���錾
	DIMOUSESTATE mouseState;		// �}�E�X�̓��͏��

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(mouseState), &mouseState)))
	{ // �擾�ɐ��������ꍇ

		for (int nCntMouKey = 0; nCntMouKey < NUM_MOUSEKEY_MAX; nCntMouKey++)
		{ // �}�E�X�L�[�̍ő吔���J��Ԃ�

			// �}�E�X�̃����[�X����ۑ�
			g_mouseStateRelease.rgbButtons[nCntMouKey] = (g_mouseState.rgbButtons[nCntMouKey] | mouseState.rgbButtons[nCntMouKey]) ^ mouseState.rgbButtons[nCntMouKey];

			// �}�E�X�̃g���K�[����ۑ�
			g_mouseStateTrigger.rgbButtons[nCntMouKey] = (g_mouseState.rgbButtons[nCntMouKey] ^ mouseState.rgbButtons[nCntMouKey]) & mouseState.rgbButtons[nCntMouKey];
		}

		// �}�E�X�̃v���X����ۑ�
		g_mouseState = mouseState;
	}
	else
	{ // �擾�Ɏ��s�����ꍇ

		// �}�E�X�ւ̃A�N�Z�X�����ēx�l��
		g_pDevMouse->Acquire();
	}
}

//======================================================================================================================
//	�}�E�X�̃v���X�̎擾����
//======================================================================================================================
bool GetMousePress(int nMouKey)
{
	// �^�U��Ԃ� (�O�����Z�q)												// ���������^�Ȃ� true �A�U�Ȃ� false ��Ԃ�
	return (g_mouseState.rgbButtons[nMouKey] & 0x80) ? true : false;		// �� �������F() �̒�
}

//======================================================================================================================
//	�}�E�X�̃g���K�[�̎擾����
//======================================================================================================================
bool GetMouseTrigger(int nMouKey)
{
	// �^�U��Ԃ� (�O�����Z�q)												// ���������^�Ȃ� true �A�U�Ȃ� false ��Ԃ�
	return (g_mouseStateTrigger.rgbButtons[nMouKey] & 0x80) ? true : false;	// �� �������F() �̒�
}

//======================================================================================================================
//	�}�E�X�̃����[�X�̎擾����
//======================================================================================================================
bool GetMouseRelease(int nMouKey)
{
	// �^�U��Ԃ� (�O�����Z�q)												// ���������^�Ȃ� true �A�U�Ȃ� false ��Ԃ�
	return (g_mouseStateRelease.rgbButtons[nMouKey] & 0x80) ? true : false;	// �� �������F() �̒�
}

//======================================================================================================================
//	�}�E�X�̈ړ��ʂ̎擾����
//======================================================================================================================
D3DXVECTOR3 GetMouseMove(void)
{
	// �}�E�X�̈ړ��ʂ�Ԃ�
	return D3DXVECTOR3((float)g_mouseState.lX, (float)g_mouseState.lY, (float)g_mouseState.lZ);
}

//**********************************************************************************************************************
//	�p�b�h����
//**********************************************************************************************************************
//======================================================================================================================
//	�p�b�h�̏���������
//======================================================================================================================
HRESULT InitJoyPad(void)
{
	// XInput�̃X�e�[�g��L����
	XInputEnable(true);

	for (int nCntJoyKey = 0; nCntJoyKey < MAX_PLAYER; nCntJoyKey++)
	{ // �v���C���[�̍ő�l�������[�v

		// �������̏�����
		memset(&g_aJoyKeyState[nCntJoyKey], 0, sizeof(XINPUT_STATE));
		memset(&g_aJoyKeyStateTrigger[nCntJoyKey], 0, sizeof(XINPUT_STATE));
		memset(&g_aVibration[nCntJoyKey].vibration, 0, sizeof(_XINPUT_VIBRATION));

		// �\���̗̂v�f�̏�����
		g_aVibration[nCntJoyKey].state = VIBSTATE_NONE;	// �������Ă��Ȃ���Ԃɏ�����
		g_aVibration[nCntJoyKey].nTime = 0;				// �^�C�}�[��������
	}

	// ������Ԃ�
	return S_OK;
}

//======================================================================================================================
//	�p�b�h�̏I������
//======================================================================================================================
void UninitJoyPad(void)
{
	// XInput�̃X�e�[�g�𖳌���
	XInputEnable(false);
}

//======================================================================================================================
//	�p�b�h�̍X�V����
//======================================================================================================================
void UpdateJoyPad(void)
{
	// �ϐ��z���錾
	XINPUT_STATE aJoyKeyState[MAX_PLAYER];	// �p�b�h�̓��͏��

	for (int nCntJoyKey = 0; nCntJoyKey < MAX_PLAYER; nCntJoyKey++)
	{ // �v���C���[�̍ő�l�������[�v

		if (XInputGetState(nCntJoyKey, &aJoyKeyState[nCntJoyKey]) == ERROR_SUCCESS)
		{

			// �p�b�h�̃����[�X����ۑ�
			g_aJoyKeyStateRelease[nCntJoyKey].Gamepad.wButtons =
			(g_aJoyKeyState[nCntJoyKey].Gamepad.wButtons
			|  aJoyKeyState[nCntJoyKey].Gamepad.wButtons)
			^  aJoyKeyState[nCntJoyKey].Gamepad.wButtons;

			// �p�b�h�̃g���K�[����ۑ� (�{�^��)
			g_aJoyKeyStateTrigger[nCntJoyKey].Gamepad.wButtons =
			(g_aJoyKeyState[nCntJoyKey].Gamepad.wButtons
			^  aJoyKeyState[nCntJoyKey].Gamepad.wButtons)
			&  aJoyKeyState[nCntJoyKey].Gamepad.wButtons;

			// �p�b�h�̃g���K�[����ۑ� (L2)
			g_aJoyKeyStateTrigger[nCntJoyKey].Gamepad.bLeftTrigger =
			(g_aJoyKeyState[nCntJoyKey].Gamepad.bLeftTrigger
			^  aJoyKeyState[nCntJoyKey].Gamepad.bLeftTrigger)
			&  aJoyKeyState[nCntJoyKey].Gamepad.bLeftTrigger;

			// �p�b�h�̃g���K�[����ۑ� (R2)
			g_aJoyKeyStateTrigger[nCntJoyKey].Gamepad.bRightTrigger =
			(g_aJoyKeyState[nCntJoyKey].Gamepad.bRightTrigger
			^  aJoyKeyState[nCntJoyKey].Gamepad.bRightTrigger)
			&  aJoyKeyState[nCntJoyKey].Gamepad.bRightTrigger;

			// �p�b�h�̃v���X����ۑ�
			g_aJoyKeyState[nCntJoyKey] = aJoyKeyState[nCntJoyKey];
		}

		switch (g_aVibration[nCntJoyKey].state)
		{ // �U���̍X�V
		case VIBSTATE_DEATH:	// ���S��Ԃ̏ꍇ

			// �J�E���g����
			if (g_aVibration[nCntJoyKey].nTime > 0)
			{ // �o�C�u�\���̗̂v�f (nTime) �� 0���傫���ꍇ

				// �o�C�u�\���̗̂v�f (nTime) �����Z
				g_aVibration[nCntJoyKey].nTime--;

				// �U�����x���̐ݒ�
				g_aVibration[nCntJoyKey].vibration.wLeftMotorSpeed -= (short)(USHRT_MAX / VIB_TIME_DEATH);	// ��
				g_aVibration[nCntJoyKey].vibration.wRightMotorSpeed -= (short)(USHRT_MAX / VIB_TIME_DEATH);	// �E
			}
			else
			{ // �o�C�u�\���̗̂v�f (nTime) �� 0�ȉ��ɂȂ����ꍇ

				// �������̏�����
				memset(&g_aVibration[nCntJoyKey].vibration, 0, sizeof(_XINPUT_VIBRATION));
			}

			// �����𔲂���
			break;

		default:				// ����ȊO�̏ꍇ

			// �J�E���g����
			if (g_aVibration[nCntJoyKey].nTime >= 0)
			{ // �o�C�u�\���̗̂v�f (nTime) �� 0���傫���ꍇ

				// �o�C�u�\���̗̂v�f (nTime) �����Z
				g_aVibration[nCntJoyKey].nTime--;
			}
			else
			{ // �o�C�u�\���̗̂v�f (nTime) �� 0�ȉ��ɂȂ����ꍇ

				// �������Ȃ���Ԃɐݒ�
				g_aVibration[nCntJoyKey].state = VIBSTATE_NONE;

				// �������̏�����
				memset(&g_aVibration[nCntJoyKey].vibration, 0, sizeof(_XINPUT_VIBRATION));
			}

			// �����𔲂���
			break;
		}

		// ���[�^�[�̕␳
		if (g_aVibration[nCntJoyKey].vibration.wLeftMotorSpeed < 0)
		{ // ���̐U���� 0��菬�����ꍇ

			// ���̃��[�^�[�̕␳
			g_aVibration[nCntJoyKey].vibration.wLeftMotorSpeed = 0;
		}
		if (g_aVibration[nCntJoyKey].vibration.wRightMotorSpeed < 0)
		{ // �E�̐U���� 0��菬�����ꍇ

			// �E�̃��[�^�[�̕␳
			g_aVibration[nCntJoyKey].vibration.wRightMotorSpeed = 0;
		}

		// �o�C�u����ݒ�
		XInputSetState(nCntJoyKey, &g_aVibration[nCntJoyKey].vibration);
	}
}

//======================================================================================================================
//	�p�b�h�̃o�C�u�̐ݒ菈��
//======================================================================================================================
void SetVibration(VIBSTATE state, int nPlayer)
{
	// �o�C�u�̏�Ԃ������̂��̂ɐݒ�
	g_aVibration[nPlayer].state = state;

	switch (g_aVibration[nPlayer].state)
	{ // �o�C�u�̎��
	case VIBSTATE_DAMAGE:		// �_���[�W���

		// �U�����x���̐ݒ�
		g_aVibration[nPlayer].vibration.wLeftMotorSpeed = VIB_DAMAGE_LV;		// ��
		g_aVibration[nPlayer].vibration.wRightMotorSpeed = VIB_DAMAGE_LV;		// �E

		// �U�����Ԃ̐ݒ�
		g_aVibration[nPlayer].nTime = VIB_TIME;

		// �����𔲂���
		break;

	case VIBSTATE_BIG_DAMAGE:	// ��_���[�W���

		// �U�����x���̐ݒ�
		g_aVibration[nPlayer].vibration.wLeftMotorSpeed = VIB_BIG_DAMAGE_LV;	// ��
		g_aVibration[nPlayer].vibration.wRightMotorSpeed = VIB_BIG_DAMAGE_LV;	// �E

		// �U�����Ԃ̐ݒ�
		g_aVibration[nPlayer].nTime = VIB_TIME;

		// �����𔲂���
		break;

	case VIBSTATE_DEATH:		// ���S���

		// �U�����x���̐ݒ�
		g_aVibration[nPlayer].vibration.wLeftMotorSpeed = VIB_DEATH_LV;			// ��
		g_aVibration[nPlayer].vibration.wRightMotorSpeed = VIB_DEATH_LV;		// �E

		// �U�����Ԃ̐ݒ�
		g_aVibration[nPlayer].nTime = VIB_TIME_DEATH;

		// �����𔲂���
		break;
	}

	// �o�C�u����ݒ�
	XInputSetState(nPlayer, &g_aVibration[nPlayer].vibration);
}

//======================================================================================================================
//	�p�b�h�̃v���X�̎擾���� (�{�^��)
//======================================================================================================================
bool GetJoyKeyPress(JOYKEY nJoyKey, int nPlayer)
{
	// �^�U��Ԃ� (�O�����Z�q)															// ���������^�Ȃ� true �A�U�Ȃ� false ��Ԃ�
	return (g_aJoyKeyState[nPlayer].Gamepad.wButtons & (1 << nJoyKey)) ? true : false;	// �� �������F() �̒�
}

//======================================================================================================================
//	�p�b�h�̃v���X�̎擾���� (L2)
//======================================================================================================================
bool GetJoyKeyL2Press(int nPlayer)
{
	// �^�U��Ԃ� (�O�����Z�q)															// ���������^�Ȃ� true �A�U�Ȃ� false ��Ԃ�
	return (g_aJoyKeyState[nPlayer].Gamepad.bLeftTrigger & 'l') ? true : false;			// �� �������F() �̒�
}

//======================================================================================================================
//	�p�b�h�̃v���X�̎擾���� (R2)
//======================================================================================================================
bool GetJoyKeyR2Press(int nPlayer)
{
	// �^�U��Ԃ� (�O�����Z�q)															// ���������^�Ȃ� true �A�U�Ȃ� false ��Ԃ�
	return (g_aJoyKeyState[nPlayer].Gamepad.bRightTrigger & 'r') ? true : false;		// �� �������F() �̒�
}

//======================================================================================================================
//	�p�b�h�̃v���X�̎擾���� (LSTICK �E��)
//======================================================================================================================
SHORT GetJoyStickPressLX(int nPlayer)
{
	// �X�e�B�b�N�̌X���̒l��Ԃ� (�E��)
	return g_aJoyKeyState[nPlayer].Gamepad.sThumbLX;
}

//======================================================================================================================
//	�p�b�h�̃v���X�̎擾���� (LSTICK �㉺)
//======================================================================================================================
SHORT GetJoyStickPressLY(int nPlayer)
{
	// �X�e�B�b�N�̌X���̒l��Ԃ� (�㉺)
	return g_aJoyKeyState[nPlayer].Gamepad.sThumbLY;
}

//======================================================================================================================
//	�p�b�h�̃v���X�̎擾���� (RSTICK �E��)
//======================================================================================================================
SHORT GetJoyStickPressRX(int nPlayer)
{
	// �X�e�B�b�N�̌X���̒l��Ԃ� (�E��)
	return g_aJoyKeyState[nPlayer].Gamepad.sThumbRX;
}

//======================================================================================================================
//	�p�b�h�̃v���X�̎擾���� (RSTICK �㉺)
//======================================================================================================================
SHORT GetJoyStickPressRY(int nPlayer)
{
	// �X�e�B�b�N�̌X���̒l��Ԃ� (�㉺)
	return g_aJoyKeyState[nPlayer].Gamepad.sThumbRY;
}

//======================================================================================================================
//	�p�b�h�̃g���K�[�̎擾���� (�{�^��)
//======================================================================================================================
bool GetJoyKeyTrigger(JOYKEY nJoyKey, int nPlayer)
{
	// �^�U��Ԃ� (�O�����Z�q)																	// ���������^�Ȃ� true �A�U�Ȃ� false ��Ԃ�
	return (g_aJoyKeyStateTrigger[nPlayer].Gamepad.wButtons & (1 << nJoyKey)) ? true : false;	// �� �������F() �̒�
}

//======================================================================================================================
//	�p�b�h�̃g���K�[�̎擾���� (L2)
//======================================================================================================================
bool GetJoyKeyL2Trigger(int nPlayer)
{
	// �^�U��Ԃ� (�O�����Z�q)																	// ���������^�Ȃ� true �A�U�Ȃ� false ��Ԃ�
	return (g_aJoyKeyStateTrigger[nPlayer].Gamepad.bLeftTrigger & 'l') ? true : false;			// �� �������F() �̒�
}

//======================================================================================================================
//	�p�b�h�̃g���K�[�̎擾���� (R2)
//======================================================================================================================
bool GetJoyKeyR2Trigger(int nPlayer)
{
	// �^�U��Ԃ� (�O�����Z�q)																	// ���������^�Ȃ� true �A�U�Ȃ� false ��Ԃ�
	return (g_aJoyKeyStateTrigger[nPlayer].Gamepad.bRightTrigger & 'r') ? true : false;			// �� �������F() �̒�
}

//======================================================================================================================
//	�p�b�h�̃����[�X�̎擾���� (�{�^��)
//======================================================================================================================
bool GetJoyKeyRelease(JOYKEY nJoyKey, int nPlayer)
{
	// �^�U��Ԃ� (�O�����Z�q)																	// ���������^�Ȃ� true �A�U�Ȃ� false ��Ԃ�
	return (g_aJoyKeyStateRelease[nPlayer].Gamepad.wButtons & (1 << nJoyKey)) ? true : false;	// �� �������F() �̒�
}