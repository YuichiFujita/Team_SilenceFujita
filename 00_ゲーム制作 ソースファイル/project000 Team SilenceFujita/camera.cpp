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

#ifdef _DEBUG	// �f�o�b�O����
#include "game.h"
#endif

//************************************************************
//	�}�N����`
//************************************************************
#define VIEW_ANGLE		(D3DXToRadian(65.0f))	// ����p
#define VIEW_NEAR		(10.0f)					// ���f����������Z���̍ŏ��l
#define VIEW_FAR		(10000.0f)				// ���f����������Z���̍ő�l

#define FIRST_ROT		(1.45f)		// �J�n���̎��_�ƒ����_�̊Ԃ̌��� (x)
#define FIRST_DIS		(-280.0f)	// �J�n���̎��_�ƒ����_�̊Ԃ̋���

#define POS_R_PLUS		(25.0f)		// �v���C���[�̈ʒu�ƒ����_�̈ʒu�̋��� (x, z)
#define POS_R_PLUS_Y	(110.0f)		// �v���C���[�̈ʒu�ƒ����_�̈ʒu�̋��� (y)
#define POS_V_Y			(200.0f)	// �Ǐ]���̎��_�̈ʒu (y)
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

//************************************************************
//	�v���g�^�C�v�錾
//************************************************************
void MoveFollowCamera(void);		// �J�����̈ʒu�̍X�V���� (�Ǐ])
void MoveCamera(void);				// �J�����̈ʒu�̍X�V���� (����)
void DisCamera(void);				// �J�����̋����̍X�V���� (����)
void RotCamera(void);				// �J�����̌����̍X�V���� (����)

void RevRotXCamera(void);			// �J�����̌����̕␳���� (x)
void RevRotYCamera(void);			// �J�����̌����̕␳���� (y)

//************************************************************
//	�O���[�o���ϐ�
//************************************************************
Camera g_camera;					// �J�����̏��

//============================================================
//	�J�����̏���������
//============================================================
void InitCamera(void)
{
	// �J�����̏���������
	g_camera.posV     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���݂̎��_
	g_camera.posR     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���݂̒����_
	g_camera.destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ڕW�̎��_
	g_camera.destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ڕW�̒����_
	g_camera.vecU     = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// ������x�N�g��
	g_camera.rot      = D3DXVECTOR3(FIRST_ROT, 0.0f, 0.0f);		// ����
	g_camera.fDis     = FIRST_DIS;								// ���_�ƒ����_�̋���
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
#ifdef _DEBUG	// �f�o�b�O����
	switch (GetGameMode())
	{ // �Q�[�����[�h���Ƃ̏���
	case GAMEMODE_PLAY:

		// �J�����̈ʒu�̍X�V (�Ǐ])
		MoveFollowCamera();

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
#endif

#ifdef NDEBUG	// �����[�X����
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
#endif
}

//============================================================
//	�J�����̐ݒ菈��
//============================================================
void SetCamera(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�ւ̃|�C���^

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH
	( // ����
		&g_camera.mtxProjection,
		VIEW_ANGLE,									// ����p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// ��ʂ̃A�X�y�N�g��
		VIEW_NEAR,									// Z���̍ŏ��l
		VIEW_FAR									// Z���̍ő�l
	);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH
	( // ����
		&g_camera.mtxView,
		&g_camera.posV,		// ���_
		&g_camera.posR,		// �����_
		&g_camera.vecU		// ������x�N�g��
	);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//======================================================================================================================
//	�J�����̈ʒu�̍X�V���� (�Ǐ])
//======================================================================================================================
void MoveFollowCamera(void)
{
	// �ϐ���錾
	D3DXVECTOR3 diffPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �J�����̎��_�̈ʒu�̌v�Z����p
	D3DXVECTOR3 diffPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �J�����̒����_�̈ʒu�̌v�Z����p

	// �|�C���^��錾
	Player *pPlayer = GetPlayer();		// �v���C���[�̏��

	if (pPlayer->nCameraState == PLAYERCAME_NORMAL)
	{ // �ʏ�̃J�����̏ꍇ
		// �ڕW�̒����_�̈ʒu���X�V
		g_camera.destPosR.x = pPlayer->pos.x + sinf(pPlayer->rot.y + D3DX_PI) * POS_R_PLUS;	// �v���C���[�̈ʒu��菭���O
		g_camera.destPosR.y = pPlayer->pos.y + POS_R_PLUS_Y;								// �v���C���[�̈ʒu�Ɠ���
		g_camera.destPosR.z = pPlayer->pos.z + cosf(pPlayer->rot.y + D3DX_PI) * POS_R_PLUS;	// �v���C���[�̈ʒu��菭���O

		// �ڕW�̎��_�̈ʒu���X�V
		g_camera.destPosV.x = g_camera.destPosR.x + ((g_camera.fDis * sinf(g_camera.rot.x)) * sinf(g_camera.rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu
		g_camera.destPosV.y = POS_V_Y;																					// �Œ�̍���
		g_camera.destPosV.z = g_camera.destPosR.z + ((g_camera.fDis * sinf(g_camera.rot.x)) * cosf(g_camera.rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu

		// �ڕW�̈ʒu�܂ł̍������v�Z
		diffPosV = g_camera.destPosV - g_camera.posV;	// ���_
		diffPosR = g_camera.destPosR - g_camera.posR;	// �����_

		// ���_�̈ʒu���X�V
		g_camera.posV.x += diffPosV.x * REV_POS_V;
		g_camera.posV.y += diffPosV.y * REV_POS_V_Y;
		g_camera.posV.z += diffPosV.z * REV_POS_V;

		// �����_�̈ʒu���X�V
		g_camera.posR.x += diffPosR.x * REV_POS_R;
		g_camera.posR.y += diffPosR.y * REV_POS_R_Y;
		g_camera.posR.z += diffPosR.z * REV_POS_R;
	}
	else if (pPlayer->nCameraState == PLAYERCAME_BACK)
	{ // �����ς�J�����������ꍇ
		// �ڕW�̒����_�̈ʒu���X�V
		g_camera.posR.x = pPlayer->pos.x + sinf(pPlayer->rot.y + D3DX_PI) * -80.0f;		// �v���C���[�̈ʒu��菭�����
		g_camera.posR.y = pPlayer->pos.y + POS_R_PLUS_Y;								// �v���C���[�̈ʒu�Ɠ���
		g_camera.posR.z = pPlayer->pos.z + cosf(pPlayer->rot.y + D3DX_PI) * -80.0f;		// �v���C���[�̈ʒu��菭�����

		// �ڕW�̎��_�̈ʒu���X�V
		g_camera.posV.x = g_camera.posR.x - ((g_camera.fDis * sinf(g_camera.rot.x)) * sinf(g_camera.rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu
		g_camera.posV.y = POS_V_Y;																				// �Œ�̍���
		g_camera.posV.z = g_camera.posR.z - ((g_camera.fDis * sinf(g_camera.rot.x)) * cosf(g_camera.rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu
	}
	else if (pPlayer->nCameraState == PLAYERCAME_FIRST)
	{ // ��l�̃J�����������ꍇ
		// �ڕW�̎��_�̈ʒu���X�V
		g_camera.posV.x = pPlayer->pos.x + sinf(pPlayer->rot.y) * 50.0f;				// �ڕW�����_���狗�������ꂽ�ʒu
		g_camera.posV.y = pPlayer->pos.y + 80.0f;		// �Œ�̍���
		g_camera.posV.z = pPlayer->pos.z + cosf(pPlayer->rot.y) * 50.0f;				// �ڕW�����_���狗�������ꂽ�ʒu

		// �ڕW�̒��n�_���X�V����
		g_camera.posR.x = g_camera.posV.x - ((g_camera.fDis * sinf(g_camera.rot.x)) * sinf(g_camera.rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu
		g_camera.posR.y = g_camera.posV.y;				// ���_�Ɠ�������
		g_camera.posR.z = g_camera.posV.z - ((g_camera.fDis * sinf(g_camera.rot.x)) * cosf(g_camera.rot.y));	// �ڕW�����_���狗�������ꂽ�ʒu
	}

	// �J�����̌������v���C���[�̌����ɍ��킹��
	g_camera.rot.y = pPlayer->rot.y;
}

//============================================================
//	�J�����̈ʒu�̍X�V���� (����)
//============================================================
void MoveCamera(void)
{
	//--------------------------------------------------------
	//	�}�E�X����̍X�V
	//--------------------------------------------------------
	if (GetMousePress(MOUKEY_LEFT) == true && GetMousePress(MOUKEY_RIGHT) == true)
	{ // �E�N���b�N�ƍ��N���b�N��������Ă���ꍇ

		// ���_���ړ�
		g_camera.posV.x -= sinf(g_camera.rot.y + (D3DX_PI * 0.5f)) * GetMouseMove().x;
		g_camera.posV.z -= cosf(g_camera.rot.y + (D3DX_PI * 0.5f)) * GetMouseMove().x;

		g_camera.posV.x += sinf(g_camera.rot.y) * GetMouseMove().y;
		g_camera.posV.z += cosf(g_camera.rot.y) * GetMouseMove().y;

		// �����_���ړ�
		g_camera.posR.x -= sinf(g_camera.rot.y + (D3DX_PI * 0.5f)) * GetMouseMove().x;
		g_camera.posR.z -= cosf(g_camera.rot.y + (D3DX_PI * 0.5f)) * GetMouseMove().x;

		g_camera.posR.x += sinf(g_camera.rot.y) * GetMouseMove().y;
		g_camera.posR.z += cosf(g_camera.rot.y) * GetMouseMove().y;
	}
}

//============================================================
//	�J�����̋����̍X�V���� (����)
//============================================================
void DisCamera(void)
{
	//--------------------------------------------------------
	//	�}�E�X����̍X�V
	//--------------------------------------------------------
	if (GetMouseMove().z != 0.0f)
	{ // �}�E�X�z�C�[���̈ړ��ʂ� 0.0f �ł͂Ȃ��ꍇ

		// �}�E�X�� z�ړ��ʂ̕����Ɏ��_���ړ�
		g_camera.fDis += GetMouseMove().z * REV_DIS_MOUSE;
	}

	//--------------------------------------------------------
	//	�����̕␳
	//--------------------------------------------------------
	if (g_camera.fDis > REV_DIS)
	{ // �Œ዗�����������ꍇ

		// �Œ዗���ɕ␳
		g_camera.fDis = REV_DIS;
	}
}

//============================================================
//	�J�����̌����̍X�V���� (����)
//============================================================
void RotCamera(void)
{
	//--------------------------------------------------------
	//	�}�E�X����̍X�V
	//--------------------------------------------------------
	if (GetMousePress(MOUKEY_LEFT) == true && GetMousePress(MOUKEY_RIGHT) == false)
	{ // ���N���b�N������������Ă���ꍇ

		// �}�E�X�� x�ړ��ʂ̕����� y������]
		g_camera.rot.y += GetMouseMove().x * REV_ROT_MOUSE;

		// �}�E�X�� y�ړ��ʂ̕����� x������]
		g_camera.rot.x += GetMouseMove().y * REV_ROT_MOUSE;
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
	g_camera.posV.x = g_camera.posR.x + ((g_camera.fDis * sinf(g_camera.rot.x)) * sinf(g_camera.rot.y));
	g_camera.posV.y = g_camera.posR.y + ((g_camera.fDis * cosf(g_camera.rot.x)));
	g_camera.posV.z = g_camera.posR.z + ((g_camera.fDis * sinf(g_camera.rot.x)) * cosf(g_camera.rot.y));

	//--------------------------------------------------------
	//	�}�E�X����̍X�V
	//--------------------------------------------------------
	if (GetMousePress(MOUKEY_RIGHT) == true && GetMousePress(MOUKEY_LEFT) == false)
	{ // �E�N���b�N������������Ă���ꍇ

		// �}�E�X�� x�ړ��ʂ̕����� y������]
		g_camera.rot.y += GetMouseMove().x * REV_ROT_MOUSE;

		// �}�E�X�� y�ړ��ʂ̕����� x������]
		g_camera.rot.x += GetMouseMove().y * REV_ROT_MOUSE;
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
	g_camera.posR.x = g_camera.posV.x + ((-g_camera.fDis * sinf(g_camera.rot.x)) * sinf(g_camera.rot.y));
	g_camera.posR.y = g_camera.posV.y + ((-g_camera.fDis * cosf(g_camera.rot.x)));
	g_camera.posR.z = g_camera.posV.z + ((-g_camera.fDis * sinf(g_camera.rot.x)) * cosf(g_camera.rot.y));
}

//============================================================
//	�J�����̌����̕␳���� (x)
//============================================================
void RevRotXCamera(void)
{
	if (g_camera.rot.x > LIMIT_ROT_HIGH)
	{ // ��]�� (x) �� LIMIT_ROT_HIGH �𒴂����ꍇ

		// LIMIT_ROT_HIGH ���� (����)
		g_camera.rot.x = LIMIT_ROT_HIGH;
	}
	else if (g_camera.rot.x < LIMIT_ROT_LOW)
	{ // ��]�� (x) �� LIMIT_ROT_LOW �𒴂����ꍇ

		// LIMIT_ROT_LOW ���� (����)
		g_camera.rot.x = LIMIT_ROT_LOW;
	}
}

//============================================================
//	�J�����̌����̕␳���� (y)
//============================================================
void RevRotYCamera(void)
{
	if (g_camera.rot.y > D3DX_PI)
	{ // ��]�� (y) �� D3DX_PI �𒴂����ꍇ

		// D3DX_PI���� (����)
		g_camera.rot.y = D3DX_PI;

		// ��]�� (y) �𔽓]������
		g_camera.rot.y *= -1.0f;
	}
	else if (g_camera.rot.y < -D3DX_PI)
	{ // ��]�� (y) �� -D3DX_PI �𒴂����ꍇ

		// -D3DX_PI���� (����)
		g_camera.rot.y = -D3DX_PI;

		// ��]�� (y) �𔽓]������
		g_camera.rot.y *= -1.0f;
	}
}

//============================================================
//	�J�����̎擾����
//============================================================
Camera *GetCamera(void)
{
	// �J�����̏��A�h���X��Ԃ�
	return &g_camera;
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
	return g_camera.posV;
}

//************************************************************
//	�����_���W�̎擾����
//************************************************************
D3DXVECTOR3 GetCameraPosR(void)
{
	// �����_���W��Ԃ�
	return g_camera.posR;
}

//************************************************************
//	�����̎擾����
//************************************************************
D3DXVECTOR3 GetCameraRot(void)
{
	// ������Ԃ�
	return g_camera.rot;
}

//************************************************************
//	�����̎擾����
//************************************************************
float GetCameraDis(void)
{
	// ������Ԃ�
	return g_camera.fDis;
}

//************************************************************
//	�J�����̍Đݒ菈��
//************************************************************
void ResetCamera(void)
{
	// �\���̗̂v�f���Đݒ�
	g_camera.posV     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���݂̎��_
	g_camera.posR     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���݂̒����_
	g_camera.destPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ڕW�̎��_
	g_camera.destPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ڕW�̒����_
	g_camera.vecU     = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// ������x�N�g��
	g_camera.rot      = D3DXVECTOR3(FIRST_ROT, 0.0f, 0.0f);		// ����
	g_camera.fDis     = FIRST_DIS;								// ���_�ƒ����_�̋���
}
#endif