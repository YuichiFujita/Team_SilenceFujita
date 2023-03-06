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

#include "ranking.h"

#ifdef _DEBUG	// �f�o�b�O����
#include "game.h"
#endif

//************************************************************
//	�}�N����`
//************************************************************
// �J�����S��
#define VIEW_NEAR		(10.0f)		// ���f����������Z���̍ŏ��l
#define VIEW_FAR		(80000.0f)	// ���f����������Z���̍ő�l

// ���C���J����
#define FIRST_ROT		(1.45f)		// �J�n���̎��_�ƒ����_�̊Ԃ̌��� (x)
#define FIRST_DIS		(-340.0f)	// �J�n���̎��_�ƒ����_�̊Ԃ̋���

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

#define MUL_ANGLE_PLUS	(1.8f)		// �u�[�X�g���̎���p�����̏�Z��

#define LIMIT_ROT_HIGH	(D3DX_PI - 0.1f)	// ��]�� (x) �̉�]�����l (��)
#define LIMIT_ROT_LOW	(0.1f)				// ��]�� (x) �̉�]�����l (��)

// ��l�̎��_�J����
#define CAMERA_FORWARD_SHIFT	(18.5f)			// �O�ɃJ���������炷����
#define CAMERA_UP_SHIFT			(100.0f)		// ��ɃJ���������炷����
#define CAMERA_RIGHT_SHIFT		(7.0f)			// �E�ɃJ���������炷����
#define CAMERA_BACK_SHIFT		(35.0f)			// ���ɃJ���������炷����

// �^�C�g���J����
#define TITLE_DISTANCE				(500.0f)			// �����L���O���̃J�����̋���
#define TITLE_POSV_Y				(200.0f)			// �����L���O���̃J�����̎��_�����ʒu(Y��)
#define TITLE_POSR_Y				(350.0f)			// �����L���O���̃J�����̒����_�����ʒu(Y��)

#define TITLE_POS_X_ROAD_ONE		(2500.0f)			// ��ڂ̓��H�̎��_�̈ʒu
#define TITLE_POS_Z_ROAD_ONE		(-7000.0f)			// ��ڂ̓��H�̎��_�̈ʒu
#define TITLE_POS_X_ROAD_TWO		(11000.0f)			// ��ڂ̓��H�̎��_�̈ʒu
#define TITLE_POS_Z_ROAD_TWO		(2250.0f)			// ��ڂ̓��H�̎��_�̈ʒu
#define TITLE_POS_X_ROAD_THREE		(9000.0f)			// �O�ڂ̓��H�̎��_�̈ʒu
#define TITLE_POS_Z_ROAD_THREE		(11000.0f)			// �O�ڂ̓��H�̎��_�̈ʒu

#define TITLE_ROT_ROAD_ONE			(0)					// ��ڂ̓��H�̊p�x
#define TITLE_ROT_ROAD_TWO			(-D3DX_PI * 0.5f)	// ��ڂ̓��H�̊p�x
#define TITLE_ROT_ROAD_THREE		(-D3DX_PI)			// �O�ڂ̓��H�̊p�x

#define TITLE_POS_MOVE_ROAD			(10.0f)				// ���H�̈ړ���
#define TITLE_POS_END_ROAD_ONE		(10000.0f)			// ��ڂ̓��H�̏I���_
#define TITLE_POS_END_ROAD_TWO		(-5000.0f)			// ��ڂ̓��H�̏I���_
#define TITLE_POS_END_ROAD_THREE	(-1000.0f)			// �O�ڂ̓��H�̏I���_

// ���U���g�J����
#define CAMERA_RSL_ROT_MOVE		(0.003f)			// �J�����̌����̈ړ���
#define RESULT_DISTANCE			(13000.0f)			// ���U���g���̃J�����̋���
#define RESULT_INIT_POS_Y		(4500.0f)			// ���U���g���̃J�����̏����ʒu(Y��)
	
// �����L���O�J����
#define RANK_DISTANCE			(500.0f)			// �����L���O���̃J�����̋���
#define RANK_POSV_Y				(200.0f)			// �����L���O���̃J�����̎��_�����ʒu(Y��)
#define RANK_POSR_Y				(350.0f)			// �����L���O���̃J�����̒����_�����ʒu(Y��)

#define RANK_POS_X_ROAD_ONE		(2500.0f)			// ��ڂ̓��H�̎��_�̈ʒu
#define RANK_POS_Z_ROAD_ONE		(-7000.0f)			// ��ڂ̓��H�̎��_�̈ʒu
#define RANK_POS_X_ROAD_TWO		(11000.0f)			// ��ڂ̓��H�̎��_�̈ʒu
#define RANK_POS_Z_ROAD_TWO		(2250.0f)			// ��ڂ̓��H�̎��_�̈ʒu
#define RANK_POS_X_ROAD_THREE	(9000.0f)			// �O�ڂ̓��H�̎��_�̈ʒu
#define RANK_POS_Z_ROAD_THREE	(11000.0f)			// �O�ڂ̓��H�̎��_�̈ʒu

#define RANK_ROT_ROAD_ONE		(0)					// ��ڂ̓��H�̊p�x
#define RANK_ROT_ROAD_TWO		(-D3DX_PI * 0.5f)	// ��ڂ̓��H�̊p�x
#define RANK_ROT_ROAD_THREE		(-D3DX_PI)			// �O�ڂ̓��H�̊p�x

#define RANK_POS_MOVE_ROAD		(10.0f)		// ���H�̈ړ���
#define RANK_POS_END_ROAD_ONE	(10000.0f)	// ��ڂ̓��H�̏I���_
#define RANK_POS_END_ROAD_TWO	(-5000.0f)	// ��ڂ̓��H�̏I���_
#define RANK_POS_END_ROAD_THREE	(-1000.0f)	// �O�ڂ̓��H�̏I���_

//************************************************************
//	�񋓌^
//************************************************************
//**********************************
//* ���H�̗񋓌^
//**********************************
typedef enum
{
	ROAD_CAMERA_TYPE_ONE = 0,		// ��ڂ̓��H�i�^�񒆁j
	ROAD_CAMERA_TYPE_TWO,			// ��ڂ̓��H�i������j
	ROAD_CAMERA_TYPE_THREE,			// �O�ڂ̓��H�i���X�X�j
	ROAD_CAMERA_TYPE_MAX
}ROADTYPE;

//************************************************************
//	�v���g�^�C�v�錾
//************************************************************
void InitMapCamera(void);			// �}�b�v�J�����̏���������
void InitUiCamera(void);			// UI�J�����̏���������

void UpdateResultCamera(void);		// ���U���g���̃J�����̍X�V����
void UpdateTitleCamera(void);		// �^�C�g�����̃J�����̍X�V����
void UpdateRankingCamera(void);		// �����L���O���̃J�����̍X�V����

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
ROADTYPE g_roadType;				// �擾���铹�H�̎��

//============================================================
//	�J�����̏���������
//============================================================
void InitCamera(void)
{
	MODE mode  = GetMode();			// ���[�h�̏��
	g_roadType = ROADTYPE(rand() % int(ROAD_CAMERA_TYPE_MAX));

	// �J�����̏�Ԃ�������
	g_CameraState = CAMERASTATE_NORMAL;

	if (mode == MODE_TUTORIAL || mode == MODE_GAME)
	{ // ���[�h���`���[�g���A���A�܂��̓Q�[���̏ꍇ

		// ��{����������
		g_aCamera[CAMERATYPE_MAIN].posV     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���݂̎��_
		g_aCamera[CAMERATYPE_MAIN].posVOld  = g_aCamera[CAMERATYPE_MAIN].posV;		// �O��̎��_
		g_aCamera[CAMERATYPE_MAIN].posR     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���݂̒����_
		g_aCamera[CAMERATYPE_MAIN].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ڕW�̎��_
		g_aCamera[CAMERATYPE_MAIN].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ڕW�̒����_
		g_aCamera[CAMERATYPE_MAIN].vecU     = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// ������x�N�g��
		g_aCamera[CAMERATYPE_MAIN].rot      = D3DXVECTOR3(FIRST_ROT, 0.0f, 0.0f);	// ����
		g_aCamera[CAMERATYPE_MAIN].fDis     = FIRST_DIS;							// ���_�ƒ����_�̋���

		// �r���[�|�[�g����������
		g_aCamera[CAMERATYPE_MAIN].viewport.X      = 0;				// ������̃s�N�Z�����W (x)
		g_aCamera[CAMERATYPE_MAIN].viewport.Y      = 0;				// ������̃s�N�Z�����W (y)
		g_aCamera[CAMERATYPE_MAIN].viewport.Width  = SCREEN_WIDTH;	// �`�悷���ʂ̉���
		g_aCamera[CAMERATYPE_MAIN].viewport.Height = SCREEN_HEIGHT;	// �`�悷���ʂ̏c��
		g_aCamera[CAMERATYPE_MAIN].viewport.MinZ   = 0.0f;
		g_aCamera[CAMERATYPE_MAIN].viewport.MaxZ   = 1.0f;

		// �}�b�v�J�����̏�����
		InitMapCamera();

		// UI�J�����̏�����
		InitUiCamera();
	}
	else if (mode == MODE_RESULT)
	{ // ���[�h�����U���g�̏ꍇ

		// ���C���J�����̏�����
		g_aCamera[CAMERATYPE_MAIN].fDis     = RESULT_DISTANCE;															// ���_�ƒ����_�̋���
		g_aCamera[CAMERATYPE_MAIN].posV     = D3DXVECTOR3(0.0f, RESULT_INIT_POS_Y, -g_aCamera[CAMERATYPE_MAIN].fDis);	// ���_�̈ʒu

		g_aCamera[CAMERATYPE_MAIN].posVOld  = g_aCamera[CAMERATYPE_MAIN].posV;		// �O��̎��_
		g_aCamera[CAMERATYPE_MAIN].posR     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����_�̈ʒu
		g_aCamera[CAMERATYPE_MAIN].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ڕW�̎��_
		g_aCamera[CAMERATYPE_MAIN].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ڕW�̒����_
		g_aCamera[CAMERATYPE_MAIN].vecU     = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// ������x�N�g��
		g_aCamera[CAMERATYPE_MAIN].rot      = D3DXVECTOR3(FIRST_ROT, 0.0f, 0.0f);	// ����

		// �r���[�|�[�g����������
		g_aCamera[CAMERATYPE_MAIN].viewport.X      = 0;				// ������̃s�N�Z�����W (x)
		g_aCamera[CAMERATYPE_MAIN].viewport.Y      = 0;				// ������̃s�N�Z�����W (y)
		g_aCamera[CAMERATYPE_MAIN].viewport.Width  = SCREEN_WIDTH;	// �`�悷���ʂ̉���
		g_aCamera[CAMERATYPE_MAIN].viewport.Height = SCREEN_HEIGHT;	// �`�悷���ʂ̏c��
		g_aCamera[CAMERATYPE_MAIN].viewport.MinZ   = 0.0f;
		g_aCamera[CAMERATYPE_MAIN].viewport.MaxZ   = 1.0f;

		// �}�b�v�J�����̏�����
		InitMapCamera();

		// UI�J�����̏�����
		InitUiCamera();
	}
	else if (mode == MODE_TITLE)
	{ // ���[�h�����U���g�̏ꍇ

	  // ���C���J�����̏�����
		switch (g_roadType)
		{
		case ROAD_CAMERA_TYPE_ONE:		// ��ڂ̓��H

			g_aCamera[CAMERATYPE_MAIN].fDis = RANK_DISTANCE;																		// ���_�ƒ����_�̋���
			g_aCamera[CAMERATYPE_MAIN].posV = D3DXVECTOR3(RANK_POS_X_ROAD_ONE, RANK_POSV_Y, -g_aCamera[CAMERATYPE_MAIN].fDis);		// ���_�̈ʒu
			g_aCamera[CAMERATYPE_MAIN].posVOld = g_aCamera[CAMERATYPE_MAIN].posV;														// �O��̎��_
			g_aCamera[CAMERATYPE_MAIN].posR = D3DXVECTOR3(RANK_POS_X_ROAD_ONE, RANK_POSR_Y, RANK_POS_Z_ROAD_ONE);					// �����_�̈ʒu
			g_aCamera[CAMERATYPE_MAIN].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// �ڕW�̎��_
			g_aCamera[CAMERATYPE_MAIN].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// �ڕW�̒����_
			g_aCamera[CAMERATYPE_MAIN].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);														// ������x�N�g��
			g_aCamera[CAMERATYPE_MAIN].rot = D3DXVECTOR3(0.0f, RANK_ROT_ROAD_ONE, 0.0f);											// ����

			break;

		case ROAD_CAMERA_TYPE_TWO:		// ��ڂ̓��H

			g_aCamera[CAMERATYPE_MAIN].fDis = RANK_DISTANCE;																		// ���_�ƒ����_�̋���
			g_aCamera[CAMERATYPE_MAIN].posV = D3DXVECTOR3(RANK_POS_X_ROAD_TWO, RANK_POSV_Y, -g_aCamera[CAMERATYPE_MAIN].fDis);		// ���_�̈ʒu
			g_aCamera[CAMERATYPE_MAIN].posVOld = g_aCamera[CAMERATYPE_MAIN].posV;														// �O��̎��_
			g_aCamera[CAMERATYPE_MAIN].posR = D3DXVECTOR3(RANK_POS_X_ROAD_TWO, RANK_POSR_Y, RANK_POS_Z_ROAD_TWO);					// �����_�̈ʒu
			g_aCamera[CAMERATYPE_MAIN].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// �ڕW�̎��_
			g_aCamera[CAMERATYPE_MAIN].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// �ڕW�̒����_
			g_aCamera[CAMERATYPE_MAIN].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);														// ������x�N�g��
			g_aCamera[CAMERATYPE_MAIN].rot = D3DXVECTOR3(0.0f, RANK_ROT_ROAD_TWO, 0.0f);											// ����

			break;

		case ROAD_CAMERA_TYPE_THREE:	// �O�ڂ̓��H

			g_aCamera[CAMERATYPE_MAIN].fDis = RANK_DISTANCE;																		// ���_�ƒ����_�̋���
			g_aCamera[CAMERATYPE_MAIN].posV = D3DXVECTOR3(RANK_POS_X_ROAD_THREE, RANK_POSV_Y, -g_aCamera[CAMERATYPE_MAIN].fDis);	// ���_�̈ʒu
			g_aCamera[CAMERATYPE_MAIN].posVOld = g_aCamera[CAMERATYPE_MAIN].posV;														// �O��̎��_
			g_aCamera[CAMERATYPE_MAIN].posR = D3DXVECTOR3(RANK_POS_X_ROAD_THREE, RANK_POSR_Y, RANK_POS_Z_ROAD_THREE);				// �����_�̈ʒu
			g_aCamera[CAMERATYPE_MAIN].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// �ڕW�̎��_
			g_aCamera[CAMERATYPE_MAIN].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// �ڕW�̒����_
			g_aCamera[CAMERATYPE_MAIN].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);														// ������x�N�g��
			g_aCamera[CAMERATYPE_MAIN].rot = D3DXVECTOR3(0.0f, RANK_ROT_ROAD_THREE, 0.0f);											// ����

			break;
		}

		// �r���[�|�[�g����������
		g_aCamera[CAMERATYPE_MAIN].viewport.X = 0;				// ������̃s�N�Z�����W (x)
		g_aCamera[CAMERATYPE_MAIN].viewport.Y = 0;				// ������̃s�N�Z�����W (y)
		g_aCamera[CAMERATYPE_MAIN].viewport.Width = SCREEN_WIDTH;	// �`�悷���ʂ̉���
		g_aCamera[CAMERATYPE_MAIN].viewport.Height = SCREEN_HEIGHT;	// �`�悷���ʂ̏c��
		g_aCamera[CAMERATYPE_MAIN].viewport.MinZ = 0.0f;
		g_aCamera[CAMERATYPE_MAIN].viewport.MaxZ = 1.0f;

		// �}�b�v�J�����̏�����
		InitMapCamera();

		// UI�J�����̏�����
		InitUiCamera();
	}
	else if (mode == MODE_RANKING)
	{ // ���[�h�����U���g�̏ꍇ

		// ���C���J�����̏�����
		switch (g_roadType)
		{
		case ROAD_CAMERA_TYPE_ONE:		// ��ڂ̓��H

			g_aCamera[CAMERATYPE_MAIN].fDis     = RANK_DISTANCE;																		// ���_�ƒ����_�̋���
			g_aCamera[CAMERATYPE_MAIN].posV     = D3DXVECTOR3(RANK_POS_X_ROAD_ONE, RANK_POSV_Y, -g_aCamera[CAMERATYPE_MAIN].fDis);		// ���_�̈ʒu
			g_aCamera[CAMERATYPE_MAIN].posVOld  = g_aCamera[CAMERATYPE_MAIN].posV;														// �O��̎��_
			g_aCamera[CAMERATYPE_MAIN].posR     = D3DXVECTOR3(RANK_POS_X_ROAD_ONE, RANK_POSR_Y, RANK_POS_Z_ROAD_ONE);					// �����_�̈ʒu
			g_aCamera[CAMERATYPE_MAIN].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// �ڕW�̎��_
			g_aCamera[CAMERATYPE_MAIN].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// �ڕW�̒����_
			g_aCamera[CAMERATYPE_MAIN].vecU     = D3DXVECTOR3(0.0f, 1.0f, 0.0f);														// ������x�N�g��
			g_aCamera[CAMERATYPE_MAIN].rot      = D3DXVECTOR3(0.0f, RANK_ROT_ROAD_ONE, 0.0f);											// ����

			break;

		case ROAD_CAMERA_TYPE_TWO:		// ��ڂ̓��H

			g_aCamera[CAMERATYPE_MAIN].fDis     = RANK_DISTANCE;																		// ���_�ƒ����_�̋���
			g_aCamera[CAMERATYPE_MAIN].posV     = D3DXVECTOR3(RANK_POS_X_ROAD_TWO, RANK_POSV_Y, -g_aCamera[CAMERATYPE_MAIN].fDis);		// ���_�̈ʒu
			g_aCamera[CAMERATYPE_MAIN].posVOld  = g_aCamera[CAMERATYPE_MAIN].posV;														// �O��̎��_
			g_aCamera[CAMERATYPE_MAIN].posR     = D3DXVECTOR3(RANK_POS_X_ROAD_TWO, RANK_POSR_Y, RANK_POS_Z_ROAD_TWO);					// �����_�̈ʒu
			g_aCamera[CAMERATYPE_MAIN].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// �ڕW�̎��_
			g_aCamera[CAMERATYPE_MAIN].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// �ڕW�̒����_
			g_aCamera[CAMERATYPE_MAIN].vecU     = D3DXVECTOR3(0.0f, 1.0f, 0.0f);														// ������x�N�g��
			g_aCamera[CAMERATYPE_MAIN].rot      = D3DXVECTOR3(0.0f, RANK_ROT_ROAD_TWO, 0.0f);											// ����

			break;

		case ROAD_CAMERA_TYPE_THREE:	// �O�ڂ̓��H

			g_aCamera[CAMERATYPE_MAIN].fDis     = RANK_DISTANCE;																		// ���_�ƒ����_�̋���
			g_aCamera[CAMERATYPE_MAIN].posV     = D3DXVECTOR3(RANK_POS_X_ROAD_THREE, RANK_POSV_Y, -g_aCamera[CAMERATYPE_MAIN].fDis);	// ���_�̈ʒu
			g_aCamera[CAMERATYPE_MAIN].posVOld  = g_aCamera[CAMERATYPE_MAIN].posV;														// �O��̎��_
			g_aCamera[CAMERATYPE_MAIN].posR     = D3DXVECTOR3(RANK_POS_X_ROAD_THREE, RANK_POSR_Y, RANK_POS_Z_ROAD_THREE);				// �����_�̈ʒu
			g_aCamera[CAMERATYPE_MAIN].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// �ڕW�̎��_
			g_aCamera[CAMERATYPE_MAIN].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);														// �ڕW�̒����_
			g_aCamera[CAMERATYPE_MAIN].vecU     = D3DXVECTOR3(0.0f, 1.0f, 0.0f);														// ������x�N�g��
			g_aCamera[CAMERATYPE_MAIN].rot      = D3DXVECTOR3(0.0f, RANK_ROT_ROAD_THREE, 0.0f);											// ����

			break;
		}

		// �r���[�|�[�g����������
		g_aCamera[CAMERATYPE_MAIN].viewport.X      = 0;				// ������̃s�N�Z�����W (x)
		g_aCamera[CAMERATYPE_MAIN].viewport.Y      = 0;				// ������̃s�N�Z�����W (y)
		g_aCamera[CAMERATYPE_MAIN].viewport.Width  = SCREEN_WIDTH;	// �`�悷���ʂ̉���
		g_aCamera[CAMERATYPE_MAIN].viewport.Height = SCREEN_HEIGHT;	// �`�悷���ʂ̏c��
		g_aCamera[CAMERATYPE_MAIN].viewport.MinZ   = 0.0f;
		g_aCamera[CAMERATYPE_MAIN].viewport.MaxZ   = 1.0f;

		// �}�b�v�J�����̏�����
		InitMapCamera();

		// UI�J�����̏�����
		InitUiCamera();
	}
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

	switch (mode)
	{ // ���[�h���Ƃ̏���
	case MODE_TUTORIAL:	// �`���[�g���A��

		// �J�����̈ʒu�̍X�V (�Ǐ])
		MoveFollowCamera();

		// �����𔲂���
		break;

	case MODE_GAME:		// �Q�[��

#if 1
		switch (GetGameMode())
		{ // �Q�[�����[�h���Ƃ̏���
		case GAMEMODE_PLAY:

	#if 0
			switch (g_CameraState)
			{
			case CAMERASTATE_NORMAL:		// �ʏ�J����

				// �J�����̈ʒu�̍X�V (�Ǐ])
				MoveFollowCamera();

				break;						// �����o��

			case CAMERASTATE_GOODJOB:		// �~�b�V��������

				// �~�b�V�����������̃J�����X�V����(�Ǐ])
				MoveGoodjobCamera();

				break;						// �����o��
			}
	#else
			// �J�����̈ʒu�̍X�V (�Ǐ])
			MoveFollowCamera();
	#endif
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
		// �}�b�v�J�����̈ʒu�̍X�V
		MoveMiniMap();

		// �����𔲂���
		break;

	case MODE_TITLE:	// �^�C�g��
		
		//�^�C�g�����̃J�����̍X�V
		UpdateTitleCamera();
		
		//�����𔲂���
		break;

	case MODE_RESULT:	// ���U���g

		// ���U���g���̃J�����̍X�V
		UpdateResultCamera();

		// �����𔲂���
		break;

	case MODE_RANKING:	// �����L���O

		// �����L���O���̃J�����̍X�V
		UpdateRankingCamera();

		// �����𔲂���
		break;
	}
}

//============================================================
//	�J�����̐ݒ菈��
//============================================================
void SetCamera(int nID)
{
	// �ϐ���錾
	float viewAngle;	// ����p�̐ݒ�p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// �r���[�|�[�g�̐ݒ�
	pDevice->SetViewport(&g_aCamera[nID].viewport);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aCamera[nID].mtxProjection);

	if (GetMode() == MODE_TUTORIAL || GetMode() == MODE_GAME)
	{ // �v���C���[�h�̏ꍇ

		if (nID == CAMERATYPE_MAIN && GetPlayer()->bCameraFirst == false)
		{ // ���C���J�������A��l�̂ł͂Ȃ��ꍇ

			// �u�[�X�g���x�ɉ����Ď���p��ϓ�
			viewAngle = VIEW_ANGLE + D3DXToRadian(GetPlayer()->boost.plusMove.x * MUL_ANGLE_PLUS);
		}
		else
		{ // �񃁃C���J�����A�������͈�l�̂̏ꍇ

			// �ʏ�̎���p��ݒ�
			viewAngle = VIEW_ANGLE;
		}
	}
	else
	{ // ��v���C���[�h�̏ꍇ

		// �ʏ�̎���p��ݒ�
		viewAngle = VIEW_ANGLE;
	}

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH
	( // ����
		&g_aCamera[nID].mtxProjection,													// �v���W�F�N�V�����}�g���b�N�X
		viewAngle,																		// ����p
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
//	�}�b�v�J�����̏���������
//======================================================================================================================
void InitMapCamera(void)
{
	// ��{����������
	g_aCamera[CAMERATYPE_MAP].posV     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���݂̎��_
	g_aCamera[CAMERATYPE_MAP].posVOld  = g_aCamera[CAMERATYPE_MAP].posV;		// �O��̎��_
	g_aCamera[CAMERATYPE_MAP].posR     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���݂̒����_
	g_aCamera[CAMERATYPE_MAP].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ڕW�̎��_
	g_aCamera[CAMERATYPE_MAP].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ڕW�̒����_
	g_aCamera[CAMERATYPE_MAP].vecU     = D3DXVECTOR3(0.0f, 0.0f, 1.0f);			// ������x�N�g��
	g_aCamera[CAMERATYPE_MAP].rot      = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ����
	g_aCamera[CAMERATYPE_MAP].fDis     = 0.0f;									// ���_�ƒ����_�̋���

	// �r���[�|�[�g����������
	g_aCamera[CAMERATYPE_MAP].viewport.X      = MAPCAM_X;		// ������̃s�N�Z�����W (x)
	g_aCamera[CAMERATYPE_MAP].viewport.Y      = MAPCAM_Y;		// ������̃s�N�Z�����W (y)
	g_aCamera[CAMERATYPE_MAP].viewport.Width  = MAPCAM_SIZE_X;	// �`�悷���ʂ̉���
	g_aCamera[CAMERATYPE_MAP].viewport.Height = MAPCAM_SIZE_Y;	// �`�悷���ʂ̏c��
	g_aCamera[CAMERATYPE_MAP].viewport.MinZ   = 0.0f;
	g_aCamera[CAMERATYPE_MAP].viewport.MaxZ   = 1.0f;
}

//======================================================================================================================
//	UI�J�����̏���������
//======================================================================================================================
void InitUiCamera(void)
{
	// ��{����������
	g_aCamera[CAMERATYPE_UI].posV     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���݂̎��_
	g_aCamera[CAMERATYPE_UI].posVOld  = g_aCamera[CAMERATYPE_UI].posV;		// �O��̎��_
	g_aCamera[CAMERATYPE_UI].posR     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���݂̒����_
	g_aCamera[CAMERATYPE_UI].destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ڕW�̎��_
	g_aCamera[CAMERATYPE_UI].destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ڕW�̒����_
	g_aCamera[CAMERATYPE_UI].vecU     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ������x�N�g��
	g_aCamera[CAMERATYPE_UI].rot      = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	g_aCamera[CAMERATYPE_UI].fDis     = 0.0f;								// ���_�ƒ����_�̋���

	// �r���[�|�[�g����������
	g_aCamera[CAMERATYPE_UI].viewport.X      = 0;				// ������̃s�N�Z�����W (x)
	g_aCamera[CAMERATYPE_UI].viewport.Y      = 0;				// ������̃s�N�Z�����W (y)
	g_aCamera[CAMERATYPE_UI].viewport.Width  = SCREEN_WIDTH;	// �`�悷���ʂ̉���
	g_aCamera[CAMERATYPE_UI].viewport.Height = SCREEN_HEIGHT;	// �`�悷���ʂ̏c��
	g_aCamera[CAMERATYPE_UI].viewport.MinZ   = 0.0f;
	g_aCamera[CAMERATYPE_UI].viewport.MaxZ   = 1.0f;
}

//======================================================================================================================
//	�^�C�g�����̃J�����̍X�V����
//======================================================================================================================
void UpdateTitleCamera(void)
{
	//���H���Ƃ̈ړ��ʂ�ݒ�
	switch (g_roadType)
	{
	case ROAD_CAMERA_TYPE_ONE:	//��ڂ̓��H

		g_aCamera[CAMERATYPE_MAIN].posV.z += TITLE_POS_MOVE_ROAD;
		g_aCamera[CAMERATYPE_MAIN].posR.z += TITLE_POS_MOVE_ROAD;

		break;

	case ROAD_CAMERA_TYPE_TWO:	//��ڂ̓��H

		g_aCamera[CAMERATYPE_MAIN].posV.x -= TITLE_POS_MOVE_ROAD;
		g_aCamera[CAMERATYPE_MAIN].posR.x -= TITLE_POS_MOVE_ROAD;

		break;

	case ROAD_CAMERA_TYPE_THREE://�O�ڂ̓��H

		g_aCamera[CAMERATYPE_MAIN].posV.z -= TITLE_POS_MOVE_ROAD;
		g_aCamera[CAMERATYPE_MAIN].posR.z -= TITLE_POS_MOVE_ROAD;

		break;
	}

	// �O��̈ʒu���L�^����
	g_aCamera[CAMERATYPE_MAIN].posVOld = g_aCamera[CAMERATYPE_MAIN].posV;

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
}

//======================================================================================================================
//	���U���g���̃J�����̍X�V����
//======================================================================================================================
void UpdateResultCamera(void)
{
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
}

//======================================================================================================================
//	�����L���O���̃J�����̍X�V����
//======================================================================================================================
void UpdateRankingCamera(void)
{
	//���H���Ƃ̈ړ��ʂ�ݒ�
	switch (g_roadType)
	{
	case ROAD_CAMERA_TYPE_ONE:	//��ڂ̓��H

		g_aCamera[CAMERATYPE_MAIN].posV.z += RANK_POS_MOVE_ROAD;
		g_aCamera[CAMERATYPE_MAIN].posR.z += RANK_POS_MOVE_ROAD;

		if (g_aCamera[CAMERATYPE_MAIN].posV.z >= RANK_POS_END_ROAD_ONE)
		{
			//��ʂ�J�ڂ�����
			SetRankingRoadTrance();
		}

		break;

	case ROAD_CAMERA_TYPE_TWO:	//��ڂ̓��H

		g_aCamera[CAMERATYPE_MAIN].posV.x -= RANK_POS_MOVE_ROAD;
		g_aCamera[CAMERATYPE_MAIN].posR.x -= RANK_POS_MOVE_ROAD;

		//���H�̏I���_�𔻒�
		if (g_aCamera[CAMERATYPE_MAIN].posV.x <= RANK_POS_END_ROAD_TWO)
		{
			//��ʂ�J�ڂ�����
			SetRankingRoadTrance();
		}

		break;

	case ROAD_CAMERA_TYPE_THREE://�O�ڂ̓��H

		g_aCamera[CAMERATYPE_MAIN].posV.z -= RANK_POS_MOVE_ROAD;
		g_aCamera[CAMERATYPE_MAIN].posR.z -= RANK_POS_MOVE_ROAD;

		//���H�̏I���_�𔻒�
		if (g_aCamera[CAMERATYPE_MAIN].posV.z <= RANK_POS_END_ROAD_THREE)
		{
			//��ʂ�J�ڂ�����
			SetRankingRoadTrance();
		}

		break;
	}

	// �O��̈ʒu���L�^����
	g_aCamera[CAMERATYPE_MAIN].posVOld = g_aCamera[CAMERATYPE_MAIN].posV;

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
			g_aCamera[CAMERATYPE_MAIN].posV.x = pPlayer->pos.x + sinf(pPlayer->rot.y) * CAMERA_FORWARD_SHIFT;	// �ڕW�����_���狗�������ꂽ�ʒu
			g_aCamera[CAMERATYPE_MAIN].posV.y = pPlayer->pos.y + CAMERA_UP_SHIFT;								// �Œ�̍���
			g_aCamera[CAMERATYPE_MAIN].posV.z = pPlayer->pos.z + cosf(pPlayer->rot.y) * CAMERA_FORWARD_SHIFT;	// �ڕW�����_���狗�������ꂽ�ʒu

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
			g_aCamera[CAMERATYPE_MAIN].posV.x = pPlayer->pos.x - sinf(pPlayer->rot.y) * CAMERA_BACK_SHIFT;	// �ڕW�����_���狗�������ꂽ�ʒu
			g_aCamera[CAMERATYPE_MAIN].posV.y = pPlayer->pos.y + CAMERA_UP_SHIFT;							// �Œ�̍���
			g_aCamera[CAMERATYPE_MAIN].posV.z = pPlayer->pos.z - cosf(pPlayer->rot.y) * CAMERA_BACK_SHIFT;	// �ڕW�����_���狗�������ꂽ�ʒu

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
Camera *GetCamera(int nID)
{
	// �J�����̏��̈����̗v�f�̃A�h���X��Ԃ�
	return &g_aCamera[nID];
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