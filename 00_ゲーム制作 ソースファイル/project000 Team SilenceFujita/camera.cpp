//============================================================
//
//	�J�������� [camera.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "input.h"
#include "camera.h"
#include "player.h"

#include "Human.h"

#ifdef _DEBUG	// �f�o�b�O����
#include "game.h"
#endif

//************************************************************
//	�}�N����`
//************************************************************
// �J�����S��
#define VIEW_ANGLE		(D3DXToRadian(65.0f))	// ����p
#define VIEW_NEAR		(10.0f)					// ���f����������Z���̍ŏ��l
#define VIEW_FAR		(60000.0f)				// ���f����������Z���̍ő�l

// ���C���J����
#define FIRST_ROT		(1.45f)		// �J�n���̎��_�ƒ����_�̊Ԃ̌��� (x)
#define FIRST_DIS		(-240.0f)	// �J�n���̎��_�ƒ����_�̊Ԃ̋���

#define POS_R_PLUS		(25.0f)		// �v���C���[�̈ʒu�ƒ����_�̈ʒu�̋��� (x, z)
#define POS_R_PLUS_Y	(110.0f)	// �v���C���[�̈ʒu�ƒ����_�̈ʒu�̋��� (y)
#define POS_V_Y			(140.0f)	// �Ǐ]���̎��_�̈ʒu (y)
#define REV_ROT_FOLLOW	(0.05f)		// �Ǐ]���̉�荞�݂̕␳�W��
#define REV_POS_V		(0.225f)	// ���_�̈ʒu�̕␳�W�� (x, z)
#define REV_POS_R		(0.25f)		// �����_�̈ʒu�̕␳�W�� (x, z)
#define REV_POS_V_Y		(0.045f)	// ���_�̈ʒu�̕␳�W�� (y)
#define REV_POS_R_Y		(0.05f)		// �����_�̈ʒu�̕␳�W�� (y)

#define MOVE_CAMERA		(4.0f)		// �J�����̈ړ���
#define MOVE_ROT_X		(0.015f)	// �����̕ύX�� (x)
#define MOVE_ROT_Y		(0.03f)		// �����̕ύX�� (y)
#define MOVE_DIS		(2.0f)		// �����̕ύX��
#define REV_DIS			(-1.0f)		// �����̕␳�l

#define REV_DIS_MOUSE	(-0.15f)	// �}�E�X����ł̃J�����̋����̕␳�W��
#define REV_ROT_MOUSE	(0.008f)	// �}�E�X����ł̃J�����̉�]�ʂ̕␳�W��

#define LIMIT_ROT_HIGH	(D3DX_PI - 0.1f)		// ��]�� (x) �̉�]�����l (��)
#define LIMIT_ROT_LOW	(0.1f)					// ��]�� (x) �̉�]�����l (��)

// �}�b�v�J����
#define MAPCAM_X		(1000)		// �}�b�v�J�����̍�����̃s�N�Z�����W (x)
#define MAPCAM_Y		(20)		// �}�b�v�J�����̍�����̃s�N�Z�����W (y)
#define MAPCAM_SIZE_X	(260)		// �}�b�v�J�����̕`�悷���ʂ̉���
#define MAPCAM_SIZE_Y	(260)		// �}�b�v�J�����̕`�悷���ʂ̏c��
#define MAPCAM_POS_R	(0.0f)		// �}�b�v�J�����̒����_�̈ʒu (y)
#define MAPCAM_POS_V	(6000.0f)	// �}�b�v�J�����̎��_�̈ʒu (y)

// ��l�̎��_�J����
#define CAMERA_FORWARD_SHIFT	(18.5f)			// �O�ɃJ���������炷����
#define CAMERA_UP_SHIFT			(80.0f)			// ��ɃJ���������炷����
#define CAMERA_RIGHT_SHIFT		(7.0f)			// �E�ɃJ���������炷����
#define CAMERA_BACK_SHIFT		(35.0f)			// ���ɃJ���������炷����

// ���U���g�J����
#define CAMERA_RSL_ROT_MOVE		(0.003f)		// �J�����̌����̈ړ���
#define TITLE_DISTANCE			(13000.0f)		// �^�C�g�����̃J�����̋���
#define TITLE_INIT_POS_Y		(4500.0f)		// �^�C�g�����̃J�����̏����ʒu(Y��)
	
//************************************************************
//	�v���g�^�C�v�錾
//************************************************************
void MoveFollowCamera(void);		// ���C���J�����̈ʒu�̍X�V���� (�Ǐ])
void MoveCamera(void);				// ���C���J�����̈ʒu�̍X�V���� (����)
void DisCamera(void);				// ���C���J�����̋����̍X�V���� (����)
void RotCamera(void);				// ���C���J�����̌����̍X�V���� (����)

void MoveMiniMap(void);				// �}�b�v�J�����̈ʒu�̍X�V����

void RevRotXCamera(void);			// �J�����̌����̕␳���� (x)
void RevRotYCamera(void);			// �J�����̌����̕␳���� (y)

//void MoveGoodjobCamera(void);		// �~�b�V�����������̃J�����X�V����(�Ǐ])

//************************************************************
//	�O���[�o���ϐ�
//************************************************************
Camera g_aCamera[CAMERATYPE_MAX];	// �J�����̏��
CAMERASTATE g_CameraState;			// �J�����̏��

//============================================================
//	�J�����̏���������
//============================================================
void InitCamera(void)
{
	MODE mode = GetMode();			// ���[�h�̏��

	switch (mode)
	{
	case MODE_GAME:					// �Q�[��

		//--------------------------------------------------------
		//	���C���J�����̏�����
		//--------------------------------------------------------
		// ��{����������
		g_aCamera[CAMERATYPE_MAIN].posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���݂̎��_
		g_aCamera[CAMERATYPE_MAIN].posVOld = g_aCamera[CAMERATYPE_MAIN].posV;	// �O��̎��_
		g_aCamera[CAMERATYPE_MAIN].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���݂̒����_
		g_aCamera[CAMERATYPE_MAIN].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW�̎��_
		g_aCamera[CAMERATYPE_MAIN].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW�̒����_
		g_aCamera[CAMERATYPE_MAIN].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// ������x�N�g��
		g_aCamera[CAMERATYPE_MAIN].rot = D3DXVECTOR3(FIRST_ROT, 0.0f, 0.0f);	// ����
		g_aCamera[CAMERATYPE_MAIN].fDis = FIRST_DIS;							// ���_�ƒ����_�̋���

		// �r���[�|�[�g����������
		g_aCamera[CAMERATYPE_MAIN].viewport.X = 0;					// ������̃s�N�Z�����W (x)
		g_aCamera[CAMERATYPE_MAIN].viewport.Y = 0;					// ������̃s�N�Z�����W (y)
		g_aCamera[CAMERATYPE_MAIN].viewport.Width = SCREEN_WIDTH;	// �`�悷���ʂ̉���
		g_aCamera[CAMERATYPE_MAIN].viewport.Height = SCREEN_HEIGHT;	// �`�悷���ʂ̏c��
		g_aCamera[CAMERATYPE_MAIN].viewport.MinZ = 0.0f;
		g_aCamera[CAMERATYPE_MAIN].viewport.MaxZ = 1.0f;

		//--------------------------------------------------------
		//	�}�b�v�J�����̏�����
		//--------------------------------------------------------
		// ��{����������
		g_aCamera[CAMERATYPE_MAP].posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���݂̎��_
		g_aCamera[CAMERATYPE_MAP].posVOld = g_aCamera[CAMERATYPE_MAP].posV;		// �O��̎��_
		g_aCamera[CAMERATYPE_MAP].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���݂̒����_
		g_aCamera[CAMERATYPE_MAP].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ڕW�̎��_
		g_aCamera[CAMERATYPE_MAP].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ڕW�̒����_
		g_aCamera[CAMERATYPE_MAP].vecU = D3DXVECTOR3(0.0f, 0.0f, 1.0f);			// ������x�N�g��
		g_aCamera[CAMERATYPE_MAP].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ����
		g_aCamera[CAMERATYPE_MAP].fDis = 0.0f;									// ���_�ƒ����_�̋���

		// �r���[�|�[�g����������
		g_aCamera[CAMERATYPE_MAP].viewport.X = MAPCAM_X;			// ������̃s�N�Z�����W (x)
		g_aCamera[CAMERATYPE_MAP].viewport.Y = MAPCAM_Y;			// ������̃s�N�Z�����W (y)
		g_aCamera[CAMERATYPE_MAP].viewport.Width = MAPCAM_SIZE_X;	// �`�悷���ʂ̉���
		g_aCamera[CAMERATYPE_MAP].viewport.Height = MAPCAM_SIZE_Y;	// �`�悷���ʂ̏c��
		g_aCamera[CAMERATYPE_MAP].viewport.MinZ = 0.0f;
		g_aCamera[CAMERATYPE_MAP].viewport.MaxZ = 1.0f;

		//--------------------------------------------------------
		//	UI�J�����̏�����
		//--------------------------------------------------------
		// ��{����������
		g_aCamera[CAMERATYPE_UI].posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���݂̎��_
		g_aCamera[CAMERATYPE_UI].posVOld = g_aCamera[CAMERATYPE_UI].posV;		// �O��̎��_
		g_aCamera[CAMERATYPE_UI].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���݂̒����_
		g_aCamera[CAMERATYPE_UI].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ڕW�̎��_
		g_aCamera[CAMERATYPE_UI].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ڕW�̒����_
		g_aCamera[CAMERATYPE_UI].vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ������x�N�g��
		g_aCamera[CAMERATYPE_UI].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ����
		g_aCamera[CAMERATYPE_UI].fDis = 0.0f;									// ���_�ƒ����_�̋���

		// �r���[�|�[�g����������
		g_aCamera[CAMERATYPE_UI].viewport.X = 0;					// ������̃s�N�Z�����W (x)
		g_aCamera[CAMERATYPE_UI].viewport.Y = 0;					// ������̃s�N�Z�����W (y)
		g_aCamera[CAMERATYPE_UI].viewport.Width = SCREEN_WIDTH;		// �`�悷���ʂ̉���
		g_aCamera[CAMERATYPE_UI].viewport.Height = SCREEN_HEIGHT;	// �`�悷���ʂ̏c��
		g_aCamera[CAMERATYPE_UI].viewport.MinZ = 0.0f;
		g_aCamera[CAMERATYPE_UI].viewport.MaxZ = 1.0f;

		break;						// �����o��

	case MODE_RESULT:				// ���U���g
		
		//--------------------------------------------------------
		//	���C���J�����̏�����
		//--------------------------------------------------------
		g_aCamera[CAMERATYPE_MAIN].fDis = TITLE_DISTANCE;						//���_�ƒ����_�̋���
		g_aCamera[CAMERATYPE_MAIN].posV = D3DXVECTOR3(0.0f, TITLE_INIT_POS_Y, -g_aCamera[CAMERATYPE_MAIN].fDis);	//���_�̈ʒu
		g_aCamera[CAMERATYPE_MAIN].posVOld = g_aCamera[CAMERATYPE_MAIN].posV;	// �O��̎��_
		g_aCamera[CAMERATYPE_MAIN].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����_�̈ʒu
		g_aCamera[CAMERATYPE_MAIN].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW�̎��_
		g_aCamera[CAMERATYPE_MAIN].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW�̒����_
		g_aCamera[CAMERATYPE_MAIN].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// ������x�N�g��
		g_aCamera[CAMERATYPE_MAIN].rot = D3DXVECTOR3(FIRST_ROT, 0.0f, 0.0f);	// ����

		// �r���[�|�[�g����������
		g_aCamera[CAMERATYPE_MAIN].viewport.X = 0;					// ������̃s�N�Z�����W (x)
		g_aCamera[CAMERATYPE_MAIN].viewport.Y = 0;					// ������̃s�N�Z�����W (y)
		g_aCamera[CAMERATYPE_MAIN].viewport.Width = SCREEN_WIDTH;	// �`�悷���ʂ̉���
		g_aCamera[CAMERATYPE_MAIN].viewport.Height = SCREEN_HEIGHT;	// �`�悷���ʂ̏c��
		g_aCamera[CAMERATYPE_MAIN].viewport.MinZ = 0.0f;
		g_aCamera[CAMERATYPE_MAIN].viewport.MaxZ = 1.0f;

		//--------------------------------------------------------
		//	�}�b�v�J�����̏�����
		//--------------------------------------------------------
		// ��{����������
		g_aCamera[CAMERATYPE_MAP].posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���݂̎��_
		g_aCamera[CAMERATYPE_MAP].posVOld = g_aCamera[CAMERATYPE_MAP].posV;		// �O��̎��_
		g_aCamera[CAMERATYPE_MAP].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���݂̒����_
		g_aCamera[CAMERATYPE_MAP].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ڕW�̎��_
		g_aCamera[CAMERATYPE_MAP].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ڕW�̒����_
		g_aCamera[CAMERATYPE_MAP].vecU = D3DXVECTOR3(0.0f, 0.0f, 1.0f);			// ������x�N�g��
		g_aCamera[CAMERATYPE_MAP].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ����
		g_aCamera[CAMERATYPE_MAP].fDis = 0.0f;									// ���_�ƒ����_�̋���

		// �r���[�|�[�g����������
		g_aCamera[CAMERATYPE_MAP].viewport.X = MAPCAM_X;			// ������̃s�N�Z�����W (x)
		g_aCamera[CAMERATYPE_MAP].viewport.Y = MAPCAM_Y;			// ������̃s�N�Z�����W (y)
		g_aCamera[CAMERATYPE_MAP].viewport.Width = MAPCAM_SIZE_X;	// �`�悷���ʂ̉���
		g_aCamera[CAMERATYPE_MAP].viewport.Height = MAPCAM_SIZE_Y;	// �`�悷���ʂ̏c��
		g_aCamera[CAMERATYPE_MAP].viewport.MinZ = 0.0f;
		g_aCamera[CAMERATYPE_MAP].viewport.MaxZ = 1.0f;

		//--------------------------------------------------------
		//	UI�J�����̏�����
		//--------------------------------------------------------
		// ��{����������
		g_aCamera[CAMERATYPE_UI].posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���݂̎��_
		g_aCamera[CAMERATYPE_UI].posVOld = g_aCamera[CAMERATYPE_UI].posV;		// �O��̎��_
		g_aCamera[CAMERATYPE_UI].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���݂̒����_
		g_aCamera[CAMERATYPE_UI].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ڕW�̎��_
		g_aCamera[CAMERATYPE_UI].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ڕW�̒����_
		g_aCamera[CAMERATYPE_UI].vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ������x�N�g��
		g_aCamera[CAMERATYPE_UI].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ����
		g_aCamera[CAMERATYPE_UI].fDis = 0.0f;									// ���_�ƒ����_�̋���

		// �r���[�|�[�g����������
		g_aCamera[CAMERATYPE_UI].viewport.X = 0;					// ������̃s�N�Z�����W (x)
		g_aCamera[CAMERATYPE_UI].viewport.Y = 0;					// ������̃s�N�Z�����W (y)
		g_aCamera[CAMERATYPE_UI].viewport.Width = SCREEN_WIDTH;		// �`�悷���ʂ̉���
		g_aCamera[CAMERATYPE_UI].viewport.Height = SCREEN_HEIGHT;	// �`�悷���ʂ̏c��
		g_aCamera[CAMERATYPE_UI].viewport.MinZ = 0.0f;
		g_aCamera[CAMERATYPE_UI].viewport.MaxZ = 1.0f;

		break;						// �����o��
	}
	
	g_CameraState = CAMERASTATE_NORMAL;							// �J�����̏��
}

//============================================================
//	�J�����̏I������
//============================================================
void UninitCamera(void)
{

}

//============================================================
//	�J�����̍X�V����
//============================================================
void UpdateCamera(void)
{
	MODE mode = GetMode();			// ���[�h�̏��

#ifdef _DEBUG	// �f�o�b�O����
	switch (mode)
	{
	case MODE_GAME:					// �Q�[��
#if 1

		switch (GetGameMode())
		{ // �Q�[�����[�h���Ƃ̏���
		case GAMEMODE_PLAY:

			//switch (g_CameraState)
			//{
			//case CAMERASTATE_NORMAL:		// �ʏ�J����

			// �J�����̈ʒu�̍X�V (�Ǐ])
			MoveFollowCamera();

			//	break;						// �����o��

			//case CAMERASTATE_GOODJOB:		// �~�b�V��������

			//	// �~�b�V�����������̃J�����X�V����(�Ǐ])
			//	MoveGoodjobCamera();

			//	break;						// �����o��
			//}

			// �����𔲂���
			break;

		case GAMEMODE_EDIT:

			// �J�����̈ʒu�̍X�V (����)
			MoveCamera();

			// �J�����̋����̍X�V (����)
			DisCamera();

			// �J�����̌����̍X�V (����)
			RotCamera();

			// �����𔲂���
			break;
		}
#else
		// �J�����̈ʒu�̍X�V (����)
		MoveCamera();

		// �J�����̋����̍X�V (����)
		DisCamera();

		// �J�����̌����̍X�V (����)
		RotCamera();
#endif

		break;						// �����o��

	case MODE_RESULT:				// ���U���g

		//�J�����̒��n�_�̈ʒu��ݒ肷��
		g_aCamera[CAMERATYPE_MAIN].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �O��̈ʒu���L�^����
		g_aCamera[CAMERATYPE_MAIN].posVOld = g_aCamera[CAMERATYPE_MAIN].posV;

		//���_���E�ɉ�
		g_aCamera[CAMERATYPE_MAIN].rot.y += CAMERA_RSL_ROT_MOVE;

		if (g_aCamera[CAMERATYPE_MAIN].rot.y > D3DX_PI)
		{//�p�x��3.14f���傫�������ꍇ
			//�p�x����1�������炷
			g_aCamera[CAMERATYPE_MAIN].rot.y -= (2 * D3DX_PI);
		}
		else if (g_aCamera[CAMERATYPE_MAIN].rot.y < -D3DX_PI)
		{//�p�x��-3.14f��菬���������ꍇ
			//�p�x��1����������
			g_aCamera[CAMERATYPE_MAIN].rot.y += (2 * D3DX_PI);
		}

		//�J�����̎��_���X�V����
		g_aCamera[CAMERATYPE_MAIN].posV.x = g_aCamera[CAMERATYPE_MAIN].posR.x + sinf(g_aCamera[CAMERATYPE_MAIN].rot.y) * -g_aCamera[CAMERATYPE_MAIN].fDis;
		g_aCamera[CAMERATYPE_MAIN].posV.z = g_aCamera[CAMERATYPE_MAIN].posR.z + cosf(g_aCamera[CAMERATYPE_MAIN].rot.y) * -g_aCamera[CAMERATYPE_MAIN].fDis;

		//�J�����̒����_���X�V����
		g_aCamera[CAMERATYPE_MAIN].posR.x = g_aCamera[CAMERATYPE_MAIN].posV.x + sinf(g_aCamera[CAMERATYPE_MAIN].rot.y) * g_aCamera[CAMERATYPE_MAIN].fDis;
		g_aCamera[CAMERATYPE_MAIN].posR.z = g_aCamera[CAMERATYPE_MAIN].posV.z + cosf(g_aCamera[CAMERATYPE_MAIN].rot.y) * g_aCamera[CAMERATYPE_MAIN].fDis;

		break;						// �����o��
	}

	// �}�b�v�J�����̈ʒu�̍X�V
	MoveMiniMap();
#endif

#ifdef NDEBUG	// �����[�X����

	switch (mode)
	{
	case MODE_GAME:					// �Q�[��

#if 1
		// �J�����̈ʒu�̍X�V (�Ǐ])
		MoveFollowCamera();
#else
		// �J�����̈ʒu�̍X�V (����)
		MoveCamera();

		// �J�����̋����̍X�V (����)
		DisCamera();

		// �J�����̌����̍X�V (����)
		RotCamera();
#endif

		break;						// �����o��

	case MODE_RESULT:				// ���U���g

		//�J�����̒��n�_�̈ʒu��ݒ肷��
		g_aCamera[CAMERATYPE_MAIN].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//���_���E�ɉ�
		g_aCamera[CAMERATYPE_MAIN].rot.y += CAMERA_RSL_ROT_MOVE;

		if (g_aCamera[CAMERATYPE_MAIN].rot.y > D3DX_PI)
		{//�p�x��3.14f���傫�������ꍇ
			//�p�x����1�������炷
			g_aCamera[CAMERATYPE_MAIN].rot.y -= (2 * D3DX_PI);
		}
		else if (g_aCamera[CAMERATYPE_MAIN].rot.y < -D3DX_PI)
		{//�p�x��-3.14f��菬���������ꍇ
			//�p�x��1����������
			g_aCamera[CAMERATYPE_MAIN].rot.y += (2 * D3DX_PI);
		}

		//�J�����̎��_���X�V����
		g_aCamera[CAMERATYPE_MAIN].posV.x = g_aCamera[CAMERATYPE_MAIN].posR.x + sinf(g_aCamera[CAMERATYPE_MAIN].rot.y) * -g_aCamera[CAMERATYPE_MAIN].fDis;
		g_aCamera[CAMERATYPE_MAIN].posV.z = g_aCamera[CAMERATYPE_MAIN].posR.z + cosf(g_aCamera[CAMERATYPE_MAIN].rot.y) * -g_aCamera[CAMERATYPE_MAIN].fDis;

		//�J�����̒����_���X�V����
		g_aCamera[CAMERATYPE_MAIN].posR.x = g_aCamera[CAMERATYPE_MAIN].posV.x + sinf(g_aCamera[CAMERATYPE_MAIN].rot.y) * g_aCamera[CAMERATYPE_MAIN].fDis;
		g_aCamera[CAMERATYPE_MAIN].posR.z = g_aCamera[CAMERATYPE_MAIN].posV.z + cosf(g_aCamera[CAMERATYPE_MAIN].rot.y) * g_aCamera[CAMERATYPE_MAIN].fDis;

		break;						// �����o��
}

	// �}�b�v�J�����̈ʒu�̍X�V
	MoveMiniMap();
#endif
}

//============================================================
//	�J�����̐ݒ菈��
//============================================================
void SetCamera(int nID)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// �r���[�|�[�g�̐ݒ�
	pDevice->SetViewport(&g_aCamera[nID].viewport);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aCamera[nID].mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH
	( // ����
		&g_aCamera[nID].mtxProjection,													// �v���W�F�N�V�����}�g���b�N�X
		VIEW_ANGLE,																		// ����p
		(float)g_aCamera[nID].viewport.Width / (float)g_aCamera[nID].viewport.Height,	// ��ʂ̃A�X�y�N�g��
		VIEW_NEAR,																		// Z���̍ŏ��l
		VIEW_FAR																		// Z���̍ő�l
	);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_aCamera[nID].mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aCamera[nID].mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH
	( // ����
		&g_aCamera[nID].mtxView,	// �r���[�}�g���b�N�X
		&g_aCamera[nID].posV,		// ���_
		&g_aCamera[nID].posR,		// �����_
		&g_aCamera[nID].vecU		// ������x�N�g��
	);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_aCamera[nID].mtxView);
}

//======================================================================================================================
//	���C���J�����̈ʒu�̍X�V���� (�Ǐ])
//======================================================================================================================
void MoveFollowCamera(void)
{
	// �ϐ���錾
	D3DXVECTOR3 diffPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �J�����̎��_�̈ʒu�̌v�Z����p
	D3DXVECTOR3 diffPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �J�����̒����_�̈ʒu�̌v�Z����p

	// �|�C���^��錾
	Player *pPlayer = GetPlayer();		// �v���C���[�̏��

	switch (pPlayer->nCameraState)
	{
	case PLAYCAMESTATE_NORMAL:			// �ʏ�̃J�������

		if (pPlayer->bCameraFirst == true)
		{ // ��l�̂�ON�̏ꍇ

			// �ڕW�̎��_�̈ʒu���X�V
			g_aCamera[CAMERATYPE_MAIN].posV.x = pPlayer->pos.x + sinf(pPlayer->rot.y) * 18.5f;				// �ڕW�����_���狗�������ꂽ�ʒu
			g_aCamera[CAMERATYPE_MAIN].posV.y = pPlayer->pos.y + 80.0f;										// �Œ�̍���
			g_aCamera[CAMERATYPE_MAIN].posV.z = pPlayer->pos.z + cosf(pPlayer->rot.y) * 18.5f;				// �ڕW�����_���狗�������ꂽ�ʒu

			// �ڕW�̒��n�_���X�V����
			g_aCamera[CAMERATYPE_MAIN].posR.x = g_aCamera[CAMERATYPE_MAIN].posV.x - ((g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * sinf(g_aCamera[CAMERATYPE_MAIN].rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu
			g_aCamera[CAMERATYPE_MAIN].posR.y = g_aCamera[CAMERATYPE_MAIN].posV.y;																															// ���_�Ɠ�������
			g_aCamera[CAMERATYPE_MAIN].posR.z = g_aCamera[CAMERATYPE_MAIN].posV.z - ((g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * cosf(g_aCamera[CAMERATYPE_MAIN].rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu
		}
		else
		{ // ��l�̂�OFF�̏ꍇ

			// �ڕW�̒����_�̈ʒu���X�V
			g_aCamera[CAMERATYPE_MAIN].destPosR.x = pPlayer->pos.x + sinf(pPlayer->rot.y + D3DX_PI) * POS_R_PLUS;	// �v���C���[�̈ʒu��菭���O
			g_aCamera[CAMERATYPE_MAIN].destPosR.y = pPlayer->pos.y + POS_R_PLUS_Y;									// �v���C���[�̈ʒu�Ɠ���
			g_aCamera[CAMERATYPE_MAIN].destPosR.z = pPlayer->pos.z + cosf(pPlayer->rot.y + D3DX_PI) * POS_R_PLUS;	// �v���C���[�̈ʒu��菭���O

			// �ڕW�̎��_�̈ʒu���X�V
			g_aCamera[CAMERATYPE_MAIN].destPosV.x = g_aCamera[CAMERATYPE_MAIN].destPosR.x + ((g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * sinf(g_aCamera[CAMERATYPE_MAIN].rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu
			g_aCamera[CAMERATYPE_MAIN].destPosV.y = POS_V_Y;																																						// �Œ�̍���
			g_aCamera[CAMERATYPE_MAIN].destPosV.z = g_aCamera[CAMERATYPE_MAIN].destPosR.z + ((g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * cosf(g_aCamera[CAMERATYPE_MAIN].rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu

																																																					// �ڕW�̈ʒu�܂ł̍������v�Z
			diffPosV = g_aCamera[CAMERATYPE_MAIN].destPosV - g_aCamera[CAMERATYPE_MAIN].posV;	// ���_
			diffPosR = g_aCamera[CAMERATYPE_MAIN].destPosR - g_aCamera[CAMERATYPE_MAIN].posR;	// �����_

			// ���_�̈ʒu���X�V
			g_aCamera[CAMERATYPE_MAIN].posV.x += diffPosV.x * REV_POS_V;
			g_aCamera[CAMERATYPE_MAIN].posV.y += diffPosV.y * REV_POS_V_Y;
			g_aCamera[CAMERATYPE_MAIN].posV.z += diffPosV.z * REV_POS_V;

			// �����_�̈ʒu���X�V
			g_aCamera[CAMERATYPE_MAIN].posR.x += diffPosR.x * REV_POS_R;
			g_aCamera[CAMERATYPE_MAIN].posR.y += diffPosR.y * REV_POS_R_Y;
			g_aCamera[CAMERATYPE_MAIN].posR.z += diffPosR.z * REV_POS_R;
		}

		break;							// �����o��

	case PLAYCAMESTATE_BACK:			// ��������J�������

		if (pPlayer->bCameraFirst == true)
		{ // ��l�̂�ON�̏ꍇ

			// �ڕW�̎��_�̈ʒu���X�V
			g_aCamera[CAMERATYPE_MAIN].posV.x = pPlayer->pos.x - sinf(pPlayer->rot.y) * 35.0f;				// �ڕW�����_���狗�������ꂽ�ʒu
			g_aCamera[CAMERATYPE_MAIN].posV.y = pPlayer->pos.y + 80.0f;										// �Œ�̍���
			g_aCamera[CAMERATYPE_MAIN].posV.z = pPlayer->pos.z - cosf(pPlayer->rot.y) * 35.0f;				// �ڕW�����_���狗�������ꂽ�ʒu

			// �ڕW�̒��n�_���X�V����
			g_aCamera[CAMERATYPE_MAIN].posR.x = g_aCamera[CAMERATYPE_MAIN].posV.x + ((g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * sinf(g_aCamera[CAMERATYPE_MAIN].rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu
			g_aCamera[CAMERATYPE_MAIN].posR.y = g_aCamera[CAMERATYPE_MAIN].posV.y;																															// ���_�Ɠ�������
			g_aCamera[CAMERATYPE_MAIN].posR.z = g_aCamera[CAMERATYPE_MAIN].posV.z + ((g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * cosf(g_aCamera[CAMERATYPE_MAIN].rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu
		}
		else
		{ // ��l�̂�OFF�̏ꍇ

			// �ڕW�̒����_�̈ʒu���X�V
			g_aCamera[CAMERATYPE_MAIN].destPosR.x = pPlayer->pos.x + sinf(pPlayer->rot.y + D3DX_PI) * -POS_R_PLUS;	// �v���C���[�̈ʒu��菭���O
			g_aCamera[CAMERATYPE_MAIN].destPosR.y = pPlayer->pos.y + POS_R_PLUS_Y;									// �v���C���[�̈ʒu�Ɠ���
			g_aCamera[CAMERATYPE_MAIN].destPosR.z = pPlayer->pos.z + cosf(pPlayer->rot.y + D3DX_PI) * -POS_R_PLUS;	// �v���C���[�̈ʒu��菭���O

			// �ڕW�̎��_�̈ʒu���X�V
			g_aCamera[CAMERATYPE_MAIN].destPosV.x = g_aCamera[CAMERATYPE_MAIN].destPosR.x - ((g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * sinf(g_aCamera[CAMERATYPE_MAIN].rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu
			g_aCamera[CAMERATYPE_MAIN].destPosV.y = POS_V_Y;																																						// �Œ�̍���
			g_aCamera[CAMERATYPE_MAIN].destPosV.z = g_aCamera[CAMERATYPE_MAIN].destPosR.z - ((g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * cosf(g_aCamera[CAMERATYPE_MAIN].rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu

																																																					// �ڕW�̈ʒu�܂ł̍������v�Z
			diffPosV = g_aCamera[CAMERATYPE_MAIN].destPosV - g_aCamera[CAMERATYPE_MAIN].posV;	// ���_
			diffPosR = g_aCamera[CAMERATYPE_MAIN].destPosR - g_aCamera[CAMERATYPE_MAIN].posR;	// �����_

			// ���_�̈ʒu���X�V
			g_aCamera[CAMERATYPE_MAIN].posV.x += diffPosV.x * REV_POS_V;
			g_aCamera[CAMERATYPE_MAIN].posV.y += diffPosV.y * REV_POS_V_Y;
			g_aCamera[CAMERATYPE_MAIN].posV.z += diffPosV.z * REV_POS_V;

			// �����_�̈ʒu���X�V
			g_aCamera[CAMERATYPE_MAIN].posR.x += diffPosR.x * REV_POS_R;
			g_aCamera[CAMERATYPE_MAIN].posR.y += diffPosR.y * REV_POS_R_Y;
			g_aCamera[CAMERATYPE_MAIN].posR.z += diffPosR.z * REV_POS_R;
		}

		break;							//�����o��
	}

	// �J�����̌������v���C���[�̌����ɍ��킹��
	g_aCamera[CAMERATYPE_MAIN].rot.y = pPlayer->rot.y;
}

////======================================================================================================================
//// �~�b�V�����������̃J�����X�V����(�Ǐ])
////======================================================================================================================
//void MoveGoodjobCamera(void)
//{
//	// �ϐ���錾
//	D3DXVECTOR3 diffPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �J�����̎��_�̈ʒu�̌v�Z����p
//	D3DXVECTOR3 diffPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �J�����̒����_�̈ʒu�̌v�Z����p
//
//	// �|�C���^��錾
//	Player *pPlayer = GetPlayer();		// �v���C���[�̏��
//
//	Human *pHuman = GetHumanData();		// �l�Ԃ̏��
//
//	// ���������
//	float fRot = atan2f(pHuman->pos.x - pPlayer->pos.x, pHuman->pos.z - pPlayer->pos.z);
//
//	// �ڕW�̒����_�̈ʒu���X�V
//	g_aCamera[CAMERATYPE_MAIN].destPosR.x = pPlayer->pos.x + sinf(fRot) * POS_R_PLUS;	// �v���C���[�̈ʒu��菭���O
//	g_aCamera[CAMERATYPE_MAIN].destPosR.y = pPlayer->pos.y + POS_R_PLUS_Y;									// �v���C���[�̈ʒu�Ɠ���
//	g_aCamera[CAMERATYPE_MAIN].destPosR.z = pPlayer->pos.z + cosf(fRot) * POS_R_PLUS;	// �v���C���[�̈ʒu��菭���O
//
//	// �ڕW�̎��_�̈ʒu���X�V
//	g_aCamera[CAMERATYPE_MAIN].destPosV.x = g_aCamera[CAMERATYPE_MAIN].destPosR.x + (g_aCamera[CAMERATYPE_MAIN].fDis * sinf(fRot));	// �ڕW�����_���狗�������ꂽ�ʒu
//	g_aCamera[CAMERATYPE_MAIN].destPosV.y = POS_V_Y;																				// �Œ�̍���
//	g_aCamera[CAMERATYPE_MAIN].destPosV.z = g_aCamera[CAMERATYPE_MAIN].destPosR.z + (g_aCamera[CAMERATYPE_MAIN].fDis * cosf(fRot));	// �ڕW�����_���狗�������ꂽ�ʒu
//
//	// �ڕW�̈ʒu�܂ł̍������v�Z
//	diffPosV = g_aCamera[CAMERATYPE_MAIN].destPosV - g_aCamera[CAMERATYPE_MAIN].posV;	// ���_
//	diffPosR = g_aCamera[CAMERATYPE_MAIN].destPosR - g_aCamera[CAMERATYPE_MAIN].posR;	// �����_
//
//	// ���_�̈ʒu���X�V
//	g_aCamera[CAMERATYPE_MAIN].posV.x += diffPosV.x * REV_POS_V;
//	g_aCamera[CAMERATYPE_MAIN].posV.y += diffPosV.y * REV_POS_V_Y;
//	g_aCamera[CAMERATYPE_MAIN].posV.z += diffPosV.z * REV_POS_V;
//
//	// �����_�̈ʒu���X�V
//	g_aCamera[CAMERATYPE_MAIN].posR.x += diffPosR.x * REV_POS_R;
//	g_aCamera[CAMERATYPE_MAIN].posR.y += diffPosR.y * REV_POS_R_Y;
//	g_aCamera[CAMERATYPE_MAIN].posR.z += diffPosR.z * REV_POS_R;
//}

//============================================================
//	���C���J�����̈ʒu�̍X�V���� (����)
//============================================================
void MoveCamera(void)
{
	//--------------------------------------------------------
	//	�}�E�X����̍X�V
	//--------------------------------------------------------
	if (GetMousePress(MOUKEY_LEFT) == true && GetMousePress(MOUKEY_RIGHT) == true)
	{ // �E�N���b�N�ƍ��N���b�N��������Ă���ꍇ

		// ���_���ړ�
		g_aCamera[CAMERATYPE_MAIN].posV.x -= sinf(g_aCamera[CAMERATYPE_MAIN].rot.y + (D3DX_PI * 0.5f)) * GetMouseMove().x;
		g_aCamera[CAMERATYPE_MAIN].posV.z -= cosf(g_aCamera[CAMERATYPE_MAIN].rot.y + (D3DX_PI * 0.5f)) * GetMouseMove().x;

		g_aCamera[CAMERATYPE_MAIN].posV.x += sinf(g_aCamera[CAMERATYPE_MAIN].rot.y) * GetMouseMove().y;
		g_aCamera[CAMERATYPE_MAIN].posV.z += cosf(g_aCamera[CAMERATYPE_MAIN].rot.y) * GetMouseMove().y;

		// �����_���ړ�
		g_aCamera[CAMERATYPE_MAIN].posR.x -= sinf(g_aCamera[CAMERATYPE_MAIN].rot.y + (D3DX_PI * 0.5f)) * GetMouseMove().x;
		g_aCamera[CAMERATYPE_MAIN].posR.z -= cosf(g_aCamera[CAMERATYPE_MAIN].rot.y + (D3DX_PI * 0.5f)) * GetMouseMove().x;

		g_aCamera[CAMERATYPE_MAIN].posR.x += sinf(g_aCamera[CAMERATYPE_MAIN].rot.y) * GetMouseMove().y;
		g_aCamera[CAMERATYPE_MAIN].posR.z += cosf(g_aCamera[CAMERATYPE_MAIN].rot.y) * GetMouseMove().y;
	}
}

//============================================================
//	���C���J�����̋����̍X�V���� (����)
//============================================================
void DisCamera(void)
{
	//--------------------------------------------------------
	//	�}�E�X����̍X�V
	//--------------------------------------------------------
	if (GetMouseMove().z != 0.0f)
	{ // �}�E�X�z�C�[���̈ړ��ʂ� 0.0f �ł͂Ȃ��ꍇ

		// �}�E�X�� z�ړ��ʂ̕����Ɏ��_���ړ�
		g_aCamera[CAMERATYPE_MAIN].fDis += GetMouseMove().z * REV_DIS_MOUSE;
	}

	//--------------------------------------------------------
	//	�����̕␳
	//--------------------------------------------------------
	if (g_aCamera[CAMERATYPE_MAIN].fDis > REV_DIS)
	{ // �Œ዗�����������ꍇ

		// �Œ዗���ɕ␳
		g_aCamera[CAMERATYPE_MAIN].fDis = REV_DIS;
	}
}

//============================================================
//	���C���J�����̌����̍X�V���� (����)
//============================================================
void RotCamera(void)
{
	//--------------------------------------------------------
	//	�}�E�X����̍X�V
	//--------------------------------------------------------
	if (GetMousePress(MOUKEY_LEFT) == true && GetMousePress(MOUKEY_RIGHT) == false)
	{ // ���N���b�N������������Ă���ꍇ

		// �}�E�X�� x�ړ��ʂ̕����� y������]
		g_aCamera[CAMERATYPE_MAIN].rot.y += GetMouseMove().x * REV_ROT_MOUSE;

		// �}�E�X�� y�ړ��ʂ̕����� x������]
		g_aCamera[CAMERATYPE_MAIN].rot.x += GetMouseMove().y * REV_ROT_MOUSE;
	}

	//--------------------------------------------------------
	//	�����̕␳
	//--------------------------------------------------------
	// �����̕␳ (x)
	RevRotXCamera();

	// �����̕␳ (y)
	RevRotYCamera();

	//--------------------------------------------------------
	//	���_�̍X�V
	//--------------------------------------------------------
	g_aCamera[CAMERATYPE_MAIN].posV.x = g_aCamera[CAMERATYPE_MAIN].posR.x + ((g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * sinf(g_aCamera[CAMERATYPE_MAIN].rot.y));
	g_aCamera[CAMERATYPE_MAIN].posV.y = g_aCamera[CAMERATYPE_MAIN].posR.y + ((g_aCamera[CAMERATYPE_MAIN].fDis * cosf(g_aCamera[CAMERATYPE_MAIN].rot.x)));
	g_aCamera[CAMERATYPE_MAIN].posV.z = g_aCamera[CAMERATYPE_MAIN].posR.z + ((g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * cosf(g_aCamera[CAMERATYPE_MAIN].rot.y));

	//--------------------------------------------------------
	//	�}�E�X����̍X�V
	//--------------------------------------------------------
	if (GetMousePress(MOUKEY_RIGHT) == true && GetMousePress(MOUKEY_LEFT) == false)
	{ // �E�N���b�N������������Ă���ꍇ

		// �}�E�X�� x�ړ��ʂ̕����� y������]
		g_aCamera[CAMERATYPE_MAIN].rot.y += GetMouseMove().x * REV_ROT_MOUSE;

		// �}�E�X�� y�ړ��ʂ̕����� x������]
		g_aCamera[CAMERATYPE_MAIN].rot.x += GetMouseMove().y * REV_ROT_MOUSE;
	}

	//--------------------------------------------------------
	//	�����̕␳
	//--------------------------------------------------------
	// �����̕␳ (x)
	RevRotXCamera();

	// �����̕␳ (y)
	RevRotYCamera();

	//--------------------------------------------------------
	//	�����_�̍X�V
	//--------------------------------------------------------
	g_aCamera[CAMERATYPE_MAIN].posR.x = g_aCamera[CAMERATYPE_MAIN].posV.x + ((-g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * sinf(g_aCamera[CAMERATYPE_MAIN].rot.y));
	g_aCamera[CAMERATYPE_MAIN].posR.y = g_aCamera[CAMERATYPE_MAIN].posV.y + ((-g_aCamera[CAMERATYPE_MAIN].fDis * cosf(g_aCamera[CAMERATYPE_MAIN].rot.x)));
	g_aCamera[CAMERATYPE_MAIN].posR.z = g_aCamera[CAMERATYPE_MAIN].posV.z + ((-g_aCamera[CAMERATYPE_MAIN].fDis * sinf(g_aCamera[CAMERATYPE_MAIN].rot.x)) * cosf(g_aCamera[CAMERATYPE_MAIN].rot.y));
}

//============================================================
//	�}�b�v�J�����̈ʒu�̍X�V����
//============================================================
void MoveMiniMap(void)
{
	// �|�C���^��錾
	Player *pPlayer = GetPlayer();	// �v���C���[�̏��

	// �����_�̈ʒu���X�V
	g_aCamera[CAMERATYPE_MAP].posR.x = pPlayer->pos.x;	// �v���C���[�̈ʒu
	g_aCamera[CAMERATYPE_MAP].posR.y = MAPCAM_POS_R;	// �Œ�̍���
	g_aCamera[CAMERATYPE_MAP].posR.z = pPlayer->pos.z;	// �v���C���[�̈ʒu

	// ���_�̈ʒu���X�V
	g_aCamera[CAMERATYPE_MAP].posV.x = pPlayer->pos.x;	// �v���C���[�̈ʒu
	g_aCamera[CAMERATYPE_MAP].posV.y = MAPCAM_POS_V;	// �Œ�̍���
	g_aCamera[CAMERATYPE_MAP].posV.z = pPlayer->pos.z;	// �v���C���[�̈ʒu
}

//============================================================
//	�J�����̌����̕␳���� (x)
//============================================================
void RevRotXCamera(void)
{
	if (g_aCamera[CAMERATYPE_MAIN].rot.x > LIMIT_ROT_HIGH)
	{ // ��]�� (x) �� LIMIT_ROT_HIGH �𒴂����ꍇ

		// LIMIT_ROT_HIGH ���� (����)
		g_aCamera[CAMERATYPE_MAIN].rot.x = LIMIT_ROT_HIGH;
	}
	else if (g_aCamera[CAMERATYPE_MAIN].rot.x < LIMIT_ROT_LOW)
	{ // ��]�� (x) �� LIMIT_ROT_LOW �𒴂����ꍇ

		// LIMIT_ROT_LOW ���� (����)
		g_aCamera[CAMERATYPE_MAIN].rot.x = LIMIT_ROT_LOW;
	}
}

//============================================================
//	�J�����̌����̕␳���� (y)
//============================================================
void RevRotYCamera(void)
{
	if (g_aCamera[CAMERATYPE_MAIN].rot.y > D3DX_PI)
	{ // ��]�� (y) �� D3DX_PI �𒴂����ꍇ

		// D3DX_PI���� (����)
		g_aCamera[CAMERATYPE_MAIN].rot.y = D3DX_PI;

		// ��]�� (y) �𔽓]������
		g_aCamera[CAMERATYPE_MAIN].rot.y *= -1.0f;
	}
	else if (g_aCamera[CAMERATYPE_MAIN].rot.y < -D3DX_PI)
	{ // ��]�� (y) �� -D3DX_PI �𒴂����ꍇ

		// -D3DX_PI���� (����)
		g_aCamera[CAMERATYPE_MAIN].rot.y = -D3DX_PI;

		// ��]�� (y) �𔽓]������
		g_aCamera[CAMERATYPE_MAIN].rot.y *= -1.0f;
	}
}

//============================================================
//	�J�����̎擾����
//============================================================
Camera *GetCamera(void)
{
	// �J�����̏��̐擪�A�h���X��Ԃ�
	return &g_aCamera[0];
}

//============================================================
// �J�����̏�Ԃ̎擾����
//============================================================
CAMERASTATE *GetCameraState(void)
{
	// �J�����̏�Ԃ�Ԃ�
	return &g_CameraState;
}

#ifdef _DEBUG	// �f�o�b�O����
//============================================================
//	�f�o�b�O�����ꗗ
//============================================================
//************************************************************
//	���_���W�̎擾����
//************************************************************
D3DXVECTOR3 GetCameraPosV(void)
{
	// ���_���W��Ԃ�
	return g_aCamera[CAMERATYPE_MAIN].posV;
}

//************************************************************
//	�����_���W�̎擾����
//************************************************************
D3DXVECTOR3 GetCameraPosR(void)
{
	// �����_���W��Ԃ�
	return g_aCamera[CAMERATYPE_MAIN].posR;
}

//************************************************************
//	�����̎擾����
//************************************************************
D3DXVECTOR3 GetCameraRot(void)
{
	// ������Ԃ�
	return g_aCamera[CAMERATYPE_MAIN].rot;
}

//************************************************************
//	�����̎擾����
//************************************************************
float GetCameraDis(void)
{
	// ������Ԃ�
	return g_aCamera[CAMERATYPE_MAIN].fDis;
}
#endif