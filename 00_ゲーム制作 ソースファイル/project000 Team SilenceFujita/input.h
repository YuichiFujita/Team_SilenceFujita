//======================================================================================================================
//
//	���̓w�b�_�[ [input.h]
//	Author�F���c�E��
//
//======================================================================================================================
#ifndef _INPUT_H_			// ���̃}�N����`������Ă��Ȃ��ꍇ
#define _INPUT_H_			// ��d�C���N���[�h�h�~�̃}�N�����`����

//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"

//**********************************************************************************************************************
//	�񋓌^��` (MOUKEY)
//**********************************************************************************************************************
typedef enum
{
	MOUKEY_LEFT = 0,		// �}�E�X �E�N���b�N
	MOUKEY_RIGHT,			// �}�E�X ���N���b�N
	MOUKEY_WHEEL,			// �}�E�X�z�C�[�� ��������
	MOUKEY_MAX				// ���̗񋓌^�̑���
}MOUKEY;

//**********************************************************************************************************************
//	�񋓌^��` (JOYKEY)
//**********************************************************************************************************************
typedef enum
{
	JOYKEY_UP = 0,			// �����L�[ ��
	JOYKEY_DOWN,			// �����L�[ ��
	JOYKEY_LEFT,			// �����L�[ ��
	JOYKEY_RIGHT,			// �����L�[ �E
	JOYKEY_START,			// START�{�^��
	JOYKEY_BACK,			// BACK�{�^��
	JOYKEY_LSTICK_PUSH,		// ���X�e�B�b�N��������
	JOYKEY_RSTICK_PUSH,		// �E�X�e�B�b�N��������
	JOYKEY_L1,				// LB�{�^��
	JOYKEY_R1,				// RB�{�^��
	JOYKEY_L2,				// LT�{�^��
	JOYKEY_R2,				// RT�{�^��
	JOYKEY_A,				// A�{�^��
	JOYKEY_B,				// B�{�^��
	JOYKEY_X,				// X�{�^��
	JOYKEY_Y,				// Y�{�^��
	JOYKEY_LSTICK_X,		// ���X�e�B�b�N ���E
	JOYKEY_LSTICK_Y,		// ���X�e�B�b�N �㉺
	JOYKEY_RSTICK_X,		// �E�X�e�B�b�N ���E
	JOYKEY_RSTICK_Y,		// �E�X�e�B�b�N �㉺
	JOYKEY_MAX				// ���̗񋓌^�̑���
}JOYKEY;

//**********************************************************************************************************************
//	�񋓌^��` (VIBSTATE)
//**********************************************************************************************************************
typedef enum
{
	VIBSTATE_NONE = 0,		// �������Ă��Ȃ����
	VIBSTATE_DAMAGE,		// �_���[�W���
	VIBSTATE_BIG_DAMAGE,	// ��_���[�W���
	VIBSTATE_DEATH,			// ���S���
	VIBSTATE_MAX			// ���̗񋓌^�̑���
}VIBSTATE;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
// ���̓f�o�C�X
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);		// ���̓f�o�C�X�̏���������
void UninitInput(void);									// ���̓f�o�C�X�̏I������
void UpdateInput(void);									// ���̓f�o�C�X�̍X�V����

// �L�[�{�[�h
bool GetKeyboardPress(int nKey);						// �L�[�{�[�h�̃v���X�̎擾����
bool GetKeyboardTrigger(int nKey);						// �L�[�{�[�h�̃g���K�[�̎擾����
bool GetKeyboardRelease(int nKey);						// �L�[�{�[�h�̃����[�X�̎擾����

// �}�E�X
bool GetMousePress(int nMouKey);						// �}�E�X�̃v���X�̎擾����
bool GetMouseTrigger(int nMouKey);						// �}�E�X�̃g���K�[�̎擾����
bool GetMouseRelease(int nMouKey);						// �}�E�X�̃����[�X�̎擾����
D3DXVECTOR3 GetMouseMove(void);							// �}�E�X�̈ړ��ʂ̎擾����

// �p�b�h
void  SetVibration(VIBSTATE state, int nPlayer);		// �p�b�h�̃o�C�u�̐ݒ菈��
bool  GetJoyKeyPress(JOYKEY nJoyKey, int nPlayer);		// �p�b�h�̃v���X�̎擾���� (�{�^��)
bool  GetJoyKeyL2Press(int nPlayer);					// �p�b�h�̃v���X�̎擾���� (L2)
bool  GetJoyKeyR2Press(int nPlayer);					// �p�b�h�̃v���X�̎擾���� (R2)
SHORT GetJoyStickPressLX(int nPlayer);					// �p�b�h�̃v���X�̎擾���� (L�X�e�B�b�N X)
SHORT GetJoyStickPressLY(int nPlayer);					// �p�b�h�̃v���X�̎擾���� (L�X�e�B�b�N Y)
SHORT GetJoyStickPressRX(int nPlayer);					// �p�b�h�̃v���X�̎擾���� (L�X�e�B�b�N X)
SHORT GetJoyStickPressRY(int nPlayer);					// �p�b�h�̃v���X�̎擾���� (L�X�e�B�b�N Y)

bool GetJoyKeyTrigger(JOYKEY nJoyKey, int nPlayer);		// �p�b�h�̃g���K�[�̎擾���� (�{�^��)
bool GetJoyKeyL2Trigger(int nPlayer);					// �p�b�h�̃g���K�[�̎擾���� (L2)
bool GetJoyKeyR2Trigger(int nPlayer);					// �p�b�h�̃g���K�[�̎擾���� (R2)
bool GetJoyKeyRelease(JOYKEY nJoyKey, int nPlayer);		// �p�b�h�̃����[�X�̎擾���� (�{�^��)

#endif