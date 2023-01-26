//======================================================================================================================
//
//	�l�Ԃ̏��� [Human.cpp]
//	Author�F��������
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "input.h"
#include "model.h"

#include "Human.h"
#include "shadow.h"
#include "sound.h"
#include "player.h"
#include "Police.h"
#include "curve.h"
#include "object.h"
#include "wind.h"

#ifdef _DEBUG	// �f�o�b�O����
#include "game.h"
#endif

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define HUMAN_LIFE					(50)		// �l�̗̑�
#define HUMAN_GRAVITY				(1.0f)		// �l�ɂ�����d��
#define HUMAN_MOVE_FORWARD			(0.1f)		// �l�O�i���̈ړ���
#define HUMAN_MOVE_BACKWARD			(0.2f)		// �l��ގ��̈ړ���
#define HUMAN_MOVE_ROT				(0.012f)	// �l�̌����ύX��
#define REV_HUMAN_MOVE_ROT			(0.1f)		// �ړ��ʂɂ������ύX�ʂ̕␳�W��
#define SUB_HUMAN_MOVE_VALUE		(15.0f)		// �����ύX���̌������s����ړ���
#define SUB_HUMAN_MOVE				(0.05f)		// �����ύX���̌�����
#define MAX_HUMAN_FORWARD			(2.0f)		// �O�i���̍ō����x
#define MAX_HUMAN_BACKWARD			(8.0f)		// ��ގ��̍ō����x
#define REV_HUMAN_MOVE_SUB			(0.04f)		// �ړ��ʂ̌����W��
#define HUMAN_WIDTH					(10.0f)		// �l�̏c��
#define HUMAN_DEPTH					(10.0f)		// �l�̉��s

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void PosHuman(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove);		// �l�Ԃ̈ʒu�̍X�V����
void RevHuman(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos);	// �l�Ԃ̕␳�̍X�V����
void CurveHuman(Human *pHuman);						// �l�Ԃ̃J�[�u����
void CurveRotHuman(Human *pHuman);					// �l�Ԃ̊p�x�X�V����
void StopHuman(Human *pHuman);						// �l�Ԃ̒�~����
void ReactionHuman(Human *pHuman);					// �l�Ԃ̃��A�N�V��������

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
Human g_aHuman[MAX_HUMAN];		// �l�Ԃ̏��

//======================================================================================================================
//	�l�Ԃ̏���������
//======================================================================================================================
void InitHuman(void)
{
	CURVE HumanCurveInfo;				//���

	//�Ȃ����
	HumanCurveInfo.nCurveTime = 8;

	for (int nCntInfo = 0; nCntInfo < HumanCurveInfo.nCurveTime; nCntInfo++)
	{
		switch (nCntInfo)
		{
		case 0:

			//���Ȃ���|�C���g
			HumanCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(6500.0f, 0.0f, 2000.0f);

			//�������Ă鎲
			HumanCurveInfo.bCurveX[nCntInfo] = false;

			//�������Ă������
			HumanCurveInfo.bCurvePlus[nCntInfo] = true;

			//���Ȃ������
			HumanCurveInfo.curveAngle[nCntInfo] = CURVE_LEFT;

			//���Ɍ����p�x
			HumanCurveInfo.fCurveRot[nCntInfo] = -90;

			break;				//�����o��

		case 1:

			//���Ȃ���|�C���g
			HumanCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(2000.0f, 0.0f, 2000.0f);

			//�������Ă鎲
			HumanCurveInfo.bCurveX[nCntInfo] = true;

			//�������Ă������
			HumanCurveInfo.bCurvePlus[nCntInfo] = false;

			//���Ȃ������
			HumanCurveInfo.curveAngle[nCntInfo] = CURVE_RIGHT;

			//���Ɍ����p�x
			HumanCurveInfo.fCurveRot[nCntInfo] = 0;

			break;				//�����o��

		case 2:

			//���Ȃ���|�C���g
			HumanCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(2000.0f, 0.0f, 6500.0f);

			//�������Ă鎲
			HumanCurveInfo.bCurveX[nCntInfo] = false;

			//�������Ă������
			HumanCurveInfo.bCurvePlus[nCntInfo] = true;

			//���Ȃ������
			HumanCurveInfo.curveAngle[nCntInfo] = CURVE_LEFT;

			//���Ɍ����p�x
			HumanCurveInfo.fCurveRot[nCntInfo] = -90;

			break;

		case 3:

			//���Ȃ���|�C���g
			HumanCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(-6500.0f, 0.0f, 6500.0f);

			//�������Ă鎲
			HumanCurveInfo.bCurveX[nCntInfo] = true;

			//�������Ă������
			HumanCurveInfo.bCurvePlus[nCntInfo] = false;

			//���Ȃ������
			HumanCurveInfo.curveAngle[nCntInfo] = CURVE_LEFT;

			//���Ɍ����p�x
			HumanCurveInfo.fCurveRot[nCntInfo] = -180;

			break;				//�����o��

		case 4:

			//���Ȃ���|�C���g
			HumanCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(-6500.0f, 0.0f, 2500.0f);

			//�������Ă鎲
			HumanCurveInfo.bCurveX[nCntInfo] = false;

			//�������Ă������
			HumanCurveInfo.bCurvePlus[nCntInfo] = false;

			//���Ȃ������
			HumanCurveInfo.curveAngle[nCntInfo] = CURVE_LEFT;

			//���Ɍ����p�x
			HumanCurveInfo.fCurveRot[nCntInfo] = 90;

			break;				//�����o��

		case 5:

			//���Ȃ���|�C���g
			HumanCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(-2250.0f, 0.0f, 2500.0f);

			//�������Ă鎲
			HumanCurveInfo.bCurveX[nCntInfo] = true;

			//�������Ă������
			HumanCurveInfo.bCurvePlus[nCntInfo] = true;

			//���Ȃ������
			HumanCurveInfo.curveAngle[nCntInfo] = CURVE_RIGHT;

			//���Ɍ����p�x
			HumanCurveInfo.fCurveRot[nCntInfo] = 180;

			break;				//�����o��

		case 6:

			//���Ȃ���|�C���g
			HumanCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(-2250.0f, 0.0f, -2000.0f);

			//�������Ă鎲
			HumanCurveInfo.bCurveX[nCntInfo] = false;

			//�������Ă������
			HumanCurveInfo.bCurvePlus[nCntInfo] = false;

			//���Ȃ������
			HumanCurveInfo.curveAngle[nCntInfo] = CURVE_LEFT;

			//���Ɍ����p�x
			HumanCurveInfo.fCurveRot[nCntInfo] = 90;

			break;				//�����o��

		case 7:

			//���Ȃ���|�C���g
			HumanCurveInfo.curvePoint[nCntInfo] = D3DXVECTOR3(6500.0f, 0.0f, -2000.0f);

			//�������Ă鎲
			HumanCurveInfo.bCurveX[nCntInfo] = true;

			//�������Ă������
			HumanCurveInfo.bCurvePlus[nCntInfo] = true;

			//���Ȃ������
			HumanCurveInfo.curveAngle[nCntInfo] = CURVE_LEFT;

			//���Ɍ����p�x
			HumanCurveInfo.fCurveRot[nCntInfo] = 0;

			break;				//�����o��
		}
	}
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

												// �l�Ԃ̏��̏�����
	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		g_aHuman[nCntHuman].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aHuman[nCntHuman].posOld = g_aHuman[nCntHuman].pos;			// �O��̈ʒu
		g_aHuman[nCntHuman].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
		g_aHuman[nCntHuman].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aHuman[nCntHuman].nShadowID = NONE_SHADOW;				// �e�̃C���f�b�N�X
		g_aHuman[nCntHuman].bMove = false;							// �ړ����Ă��邩
		g_aHuman[nCntHuman].bUse = false;							// �g�p��
		g_aHuman[nCntHuman].state = HUMANSTATE_WALK;				// ���

		//�Ȃ���p�̏���������
		for (int nCntCurve = 0; nCntCurve < MAX_CURVE; nCntCurve++)
		{
			g_aHuman[nCntHuman].humanCurve.curveAngle[nCntCurve] = CURVE_RIGHT;						// �Ȃ������
			g_aHuman[nCntHuman].humanCurve.curvePoint[nCntCurve] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �Ȃ���|�C���g
			g_aHuman[nCntHuman].humanCurve.nNowCurve = 0;												// ���݂̋Ȃ���p
			g_aHuman[nCntHuman].humanCurve.bCurveX[nCntCurve] = false;								// X���𑖂��Ă��邩
			g_aHuman[nCntHuman].humanCurve.fCurveRot[nCntCurve] = 0;									// �ڕW�̌���
		}
		g_aHuman[nCntHuman].humanCurve.nCurveTime = 0;				// �Ȃ����
		g_aHuman[nCntHuman].humanCurve.nNowCurve = 0;					// ���݂̃��[�g

		// ���f�����̏�����
		g_aHuman[nCntHuman].modelData.dwNumMat = 0;					// �}�e���A���̐�
		g_aHuman[nCntHuman].modelData.pTexture = NULL;				// �e�N�X�`���ւ̃|�C���^
		g_aHuman[nCntHuman].modelData.pMesh = NULL;					// ���b�V�� (���_���) �ւ̃|�C���^
		g_aHuman[nCntHuman].modelData.pBuffMat = NULL;				// �}�e���A���ւ̃|�C���^
		g_aHuman[nCntHuman].modelData.vtxMin = INIT_VTX_MIN;		// �ŏ��̒��_���W
		g_aHuman[nCntHuman].modelData.vtxMax = INIT_VTX_MAX;		// �ő�̒��_���W
		g_aHuman[nCntHuman].modelData.size = INIT_SIZE;				// �c��
		g_aHuman[nCntHuman].modelData.fRadius = 0.0f;				// ���a
	}

	//�l�Ԃ̐ݒ菈��
	SetHuman(D3DXVECTOR3(6000.0f, 0.0f, -1000.0f), HumanCurveInfo);
}

//======================================================================================================================
//	�l�Ԃ̏I������
//======================================================================================================================
void UninitHuman(void)
{

}

//======================================================================================================================
//	�l�Ԃ̍X�V����
//======================================================================================================================
void UpdateHuman(void)
{
	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�
		if (g_aHuman[nCntHuman].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// �O��ʒu�̍X�V
			g_aHuman[nCntHuman].posOld = g_aHuman[nCntHuman].pos;

			//----------------------------------------------------
			//	�e�̍X�V
			//----------------------------------------------------
			// �e�̈ʒu�ݒ�
			SetPositionShadow
			( // ����
				g_aHuman[nCntHuman].nShadowID,	// �e�̃C���f�b�N�X
				g_aHuman[nCntHuman].pos,			// �ʒu
				g_aHuman[nCntHuman].rot,			// ����
				NONE_SCALE							// �g�嗦
			);

			// �v���C���[�̈ʒu�̍X�V
			PosHuman(&g_aHuman[nCntHuman].move, &g_aHuman[nCntHuman].pos, &g_aHuman[nCntHuman].rot, g_aHuman[nCntHuman].bMove);

			//�l�Ԃ̃��A�N�V��������
			ReactionHuman(&g_aHuman[nCntHuman]);

			// ���̓����蔻��
			CollisionWind(&g_aHuman[nCntHuman]);

			switch (g_aHuman[nCntHuman].state)
			{
			case HUMANSTATE_WALK:		//�������

				//�l�Ԃ̃J�[�u����
				CurveHuman(&g_aHuman[nCntHuman]);

				if (g_aHuman[nCntHuman].pos.y < 0.0f)
				{//Y���̈ʒu��0.0f�������ꍇ
					//�c�ւ̈ړ��ʂ�0.0f�ɂ���
					g_aHuman[nCntHuman].move.y = 0.0f;

					//�ʒu��0.0f�ɖ߂�
					g_aHuman[nCntHuman].pos.y = 0.0f;
				}

				break;					//�����o��

			case HUMANSTATE_STOP:		//�~�܂������

				// �l�Ԃ̒�~����
				StopHuman(&g_aHuman[nCntHuman]);

				if (g_aHuman[nCntHuman].pos.y < 0.0f)
				{//Y���̈ʒu��0.0f�������ꍇ
					//�c�ւ̈ړ��ʂ�0.0f�ɂ���
					g_aHuman[nCntHuman].move.y = 0.0f;

					//�ʒu��0.0f�ɖ߂�
					g_aHuman[nCntHuman].pos.y = 0.0f;
				}

				break;					//�����o��

			case HUMANSTATE_FLY:		//������񂾏��

				// ��΂�
				g_aHuman[nCntHuman].pos.x += g_aHuman[nCntHuman].move.x;
				g_aHuman[nCntHuman].pos.z += g_aHuman[nCntHuman].move.z;

				if (g_aHuman[nCntHuman].pos.y <= 0.0f)
				{ // �ʒu��0.0f�ȉ��ɂȂ����ꍇ
					//�g�p���Ă��Ȃ�
					g_aHuman[nCntHuman].bUse = false;
				}

				break;					//�����o��
			}

			//----------------------------------------------------
			//	�����蔻��
			//----------------------------------------------------
			// �I�u�W�F�N�g�Ƃ̓����蔻��
			CollisionObject
			( // ����
				&g_aHuman[nCntHuman].pos,		// ���݂̈ʒu
				&g_aHuman[nCntHuman].posOld,	// �O��̈ʒu
				&g_aHuman[nCntHuman].move,		// �ړ���
				HUMAN_WIDTH,					// ����
				HUMAN_DEPTH						// ���s
			);

			// �v���C���[�̕␳�̍X�V����
			RevHuman(&g_aHuman[nCntHuman].rot, &g_aHuman[nCntHuman].pos);
		}
	}
}

//======================================================================================================================
//	�l�Ԃ̕`�揈��
//======================================================================================================================
void DrawHuman(void)
{
	// �ϐ���錾
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

												// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL	  blueMat;					// ���߃}�e���A���|�C���^

	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aHuman[nCntHuman].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ
		  // ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aHuman[nCntHuman].mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aHuman[nCntHuman].rot.y, g_aHuman[nCntHuman].rot.x, g_aHuman[nCntHuman].rot.z);
			D3DXMatrixMultiply(&g_aHuman[nCntHuman].mtxWorld, &g_aHuman[nCntHuman].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aHuman[nCntHuman].pos.x, g_aHuman[nCntHuman].pos.y, g_aHuman[nCntHuman].pos.z);
			D3DXMatrixMultiply(&g_aHuman[nCntHuman].mtxWorld, &g_aHuman[nCntHuman].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aHuman[nCntHuman].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aHuman[nCntHuman].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aHuman[nCntHuman].modelData.dwNumMat; nCntMat++)
			{ // �}�e���A���̐����J��Ԃ�

				switch (g_aHuman[nCntHuman].state)
				{
				case HUMANSTATE_STOP:		//��~���

					// �\���̗̂v�f���N���A
					ZeroMemory(&blueMat, sizeof(D3DXMATERIAL));

					// �g�U���E�����E���Ȕ�����Ԃɂ���
					blueMat.MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
					blueMat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
					blueMat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&blueMat.MatD3D);			// ��

					break;					//�����o��

				default:		//��L�ȊO

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					break;					//�����o��
				}

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aHuman[nCntHuman].modelData.pTexture[nCntMat]);

				// ���f���̕`��
				g_aHuman[nCntHuman].modelData.pMesh->DrawSubset(nCntMat);
			}

			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	�l�Ԃ̐ݒ菈��
//======================================================================================================================
void SetHuman(D3DXVECTOR3 pos, CURVE humanCurve)
{
	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aHuman[nCntHuman].bUse == false)
		{ // �I�u�W�F�N�g���g�p����Ă��Ȃ��ꍇ
		  // ��������
			g_aHuman[nCntHuman].pos = pos;								// ���݂̈ʒu
			g_aHuman[nCntHuman].posOld = g_aHuman[nCntHuman].pos;		// �O��̈ʒu
			g_aHuman[nCntHuman].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
			g_aHuman[nCntHuman].bMove = false;							// �ړ����Ă��Ȃ�
			g_aHuman[nCntHuman].state = HUMANSTATE_WALK;				//�������

			// �g�p���Ă����Ԃɂ���
			g_aHuman[nCntHuman].bUse = true;

			// ���f������ݒ�
			g_aHuman[nCntHuman].modelData = GetModelData(MODELTYPE_OBJECT_BIGTREE + FROM_OBJECT);	// ���f�����

																							// �e�̃C���f�b�N�X��ݒ�
			g_aHuman[nCntHuman].nShadowID = SetModelShadow
			( // ����
				g_aHuman[nCntHuman].modelData,	// ���f�����
				&g_aHuman[nCntHuman].nShadowID,	// �e�̐e�̉e�C���f�b�N�X
				&g_aHuman[nCntHuman].bUse		// �e�̐e�̎g�p��
			);

			// �e�̈ʒu�ݒ�
			SetPositionShadow(g_aHuman[nCntHuman].nShadowID, g_aHuman[nCntHuman].pos, g_aHuman[nCntHuman].rot, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

			//�Ȃ���p�֌W�̐ݒ�
			for (int nCntCurve = 0; nCntCurve < humanCurve.nCurveTime; nCntCurve++)
			{
				g_aHuman[nCntHuman].humanCurve.fCurveRot[nCntCurve] = humanCurve.fCurveRot[nCntCurve];	// ���Ȃ������
				g_aHuman[nCntHuman].humanCurve.curvePoint[nCntCurve] = humanCurve.curvePoint[nCntCurve];	// ���Ȃ���|�C���g
			}
			g_aHuman[nCntHuman].humanCurve.nNowCurve = 0;													// ���݂̃��[�g
			g_aHuman[nCntHuman].humanCurve.nCurveTime = humanCurve.nCurveTime;							// �Ȃ����

			//�J�[�u�̐ݒ菈��
			SetCurvePointHuman(&g_aHuman[nCntHuman].humanCurve, &g_aHuman[nCntHuman].rot, &g_aHuman[nCntHuman].pos);

			if (g_aHuman[nCntHuman].humanCurve.bCurveX[0] == true)
			{ // �ŏ�X���𑖂��Ă���ꍇ
				if (g_aHuman[nCntHuman].humanCurve.bCurvePlus[0] == true)
				{ // �E�ɑ����Ă���ꍇ
					// �ʒu��␳����
					g_aHuman[nCntHuman].pos.z = g_aHuman[nCntHuman].humanCurve.curvePoint[0].z - (HUMAN_WIDTH * 2);
				}
				else
				{ // ���ɑ����Ă���ꍇ
					// �ʒu��␳����
					g_aHuman[nCntHuman].pos.z = g_aHuman[nCntHuman].humanCurve.curvePoint[0].z + (HUMAN_WIDTH * 2);
				}
			}
			else
			{ // �ŏ�Z���𑖂��Ă���ꍇ
				if (g_aHuman[nCntHuman].humanCurve.bCurvePlus[0] == true)
				{ // ���ɑ����Ă���ꍇ
				  // �ʒu��␳����
					g_aHuman[nCntHuman].pos.x = g_aHuman[nCntHuman].humanCurve.curvePoint[0].x + (HUMAN_WIDTH * 2);
				}
				else
				{ // ��O�ɑ����Ă���ꍇ
				  // �ʒu��␳����
					g_aHuman[nCntHuman].pos.x = g_aHuman[nCntHuman].humanCurve.curvePoint[0].x - (HUMAN_WIDTH * 2);
				}
			}

			// �����𔲂���
			break;
		}
	}
}

#if 0
//======================================================================================================================
//	�I�u�W�F�N�g�̃_���[�W����
//======================================================================================================================
void HitHuman(Human *pHuman, int nDamage)
{
	if (pHuman->state == ACTIONSTATE_NORMAL)
	{ // �I�u�W�F�N�g���ʏ��Ԃ̏ꍇ

	  // �����̃_���[�W����̗͂��猸�Z
		pHuman->nLife -= nDamage;

		if (pHuman->nLife > 0)
		{ // �̗͂��c���Ă���ꍇ

		  // �_���[�W��Ԃɂ���
			pHuman->state = ACTIONSTATE_DAMAGE;

			// �p�[�e�B�N���̐ݒ�
			SetParticle
			( // ����
				pHuman->pos,						// �ʒu
				D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f),	// �F
				PARTICLETYPE_DAMAGE,				// ���
				SPAWN_PARTICLE_DAMAGE,				// �G�t�F�N�g��
				2									// ����
			);

			// �J�E���^�[��ݒ�
			pHuman->nCounterState = DAMAGE_TIME_OBJ;

			// �T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_DMG);			// SE (�_���[�W)
		}
		else
		{ // �̗͂��s�����ꍇ

		  // �����̐ݒ�
			SetExplosion(pHuman->pos, SOUNDTYPE_BREAK);

			// �p�[�e�B�N���̐ݒ�
			SetParticle
			( // ����
				pHuman->pos,						// �ʒu
				D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f),	// �F
				PARTICLETYPE_EXPLOSION,				// ���
				SPAWN_PARTICLE_EXPLOSION,			// �G�t�F�N�g��
				2									// ����
			);

			// �A�C�e���̐ݒ�
			SetItem(pHuman->pos, ITEMTYPE_HEAL);

			// �g�p���Ă��Ȃ���Ԃɂ���
			pHuman->bUse = false;
		}
	}
}
#endif

//======================================================================================================================
//	�I�u�W�F�N�g�̎擾����
//======================================================================================================================
Human *GetHumanData(void)
{
	// �I�u�W�F�N�g�̏��̐擪�A�h���X��Ԃ�
	return &g_aHuman[0];
}

//============================================================
//	�v���C���[�̈ʒu�̍X�V����
//============================================================
void PosHuman(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove)
{
	//--------------------------------------------------------
	//	�d�͂̍X�V
	//--------------------------------------------------------
	move->y -= HUMAN_GRAVITY;

	//--------------------------------------------------------
	//	�ړ��ʂ̕␳
	//--------------------------------------------------------
	if (move->x > MAX_HUMAN_FORWARD)
	{ // �v���C���[�̈ړ��� (x) �����l�ȏ�̏ꍇ

	  // �v���C���[�̈ړ��� (x) ��␳
		move->x = MAX_HUMAN_FORWARD;
	}
	else if (move->x < -MAX_HUMAN_BACKWARD)
	{ // �v���C���[�̈ړ��� (x) �����l�ȉ��̏ꍇ

	  // �v���C���[�̈ړ��� (x) ��␳
		move->x = -MAX_HUMAN_BACKWARD;
	}

	//--------------------------------------------------------
	//	�ʒu�̍X�V
	//--------------------------------------------------------
	pos->x += sinf(rot->y) * move->x;
	pos->y += move->y;
	pos->z += cosf(rot->y) * move->x;

	//--------------------------------------------------------
	//	�ړ��ʂ̌���
	//--------------------------------------------------------
	if (bMove == false)
	{ // �ړ����Ă��Ȃ���Ԃ̏ꍇ

	  // �ړ��ʂ�����
		move->x += (0.0f - move->x) * 0.04f;
	}
}

//============================================================
//	�v���C���[�̕␳�̍X�V����
//============================================================
void RevHuman(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos)
{
	//--------------------------------------------------------
	//	�����̐��K��
	//--------------------------------------------------------
	if (rot->y > D3DX_PI) { rot->y -= D3DX_PI * 2; }
	else if (rot->y < -D3DX_PI) { rot->y += D3DX_PI * 2; }

	//--------------------------------------------------------
	//	�ړ��͈͂̕␳
	//--------------------------------------------------------
	if (pos->z > GetLimitStage().fNear - (30.0f * 2))
	{ // �͈͊O�̏ꍇ (��O)

		// ��O�Ɉʒu��␳
		pos->z = GetLimitStage().fNear - (30.0f * 2);
	}
	if (pos->z < GetLimitStage().fFar + (30.0f * 2))
	{ // �͈͊O�̏ꍇ (��)

		// ���Ɉʒu��␳
		pos->z = GetLimitStage().fFar + (30.0f * 2);
	}
	if (pos->x > GetLimitStage().fRight - (30.0f * 2))
	{ // �͈͊O�̏ꍇ (�E)

		// �E�Ɉʒu��␳
		pos->x = GetLimitStage().fRight - (30.0f * 2);
	}
	if (pos->x < GetLimitStage().fLeft + (30.0f * 2))
	{ // �͈͊O�̏ꍇ (��)

		// ���Ɉʒu��␳
		pos->x = GetLimitStage().fLeft + (30.0f * 2);
	}
}

//============================================================
// �l�Ԃ̋Ȃ��鏈��
//============================================================
void CurveHuman(Human *pHuman)
{
	// �ړ��ʂ��X�V
	pHuman->move.x += HUMAN_MOVE_FORWARD;

	// �ړ����Ă����Ԃɂ���
	pHuman->bMove = true;

	if (pHuman->humanCurve.bCurveX[pHuman->humanCurve.nNowCurve] == true)
	{//X���𑖂��Ă����ꍇ
		if (pHuman->humanCurve.bCurvePlus[pHuman->humanCurve.nNowCurve] == true)
		{ // �E�ɑ����Ă���ꍇ
			if (pHuman->humanCurve.curveAngle[pHuman->humanCurve.nNowCurve] == CURVE_RIGHT)
			{//�E�ɋȂ���ꍇ
				if (pHuman->pos.x >= pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x - (HUMAN_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

				  // �l�Ԃ̊p�x�X�V�E�␳����
					CurveRotHuman(pHuman);

					// �ړ��ʂ�����
					pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

					// �E�̕ǂɔ��킹��
					pHuman->pos.x = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x - (HUMAN_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
				  // ���̕ǂɔ��킹��
					pHuman->pos.z = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z - (HUMAN_WIDTH * 2);
				}
			}
			else if (pHuman->humanCurve.curveAngle[pHuman->humanCurve.nNowCurve] == CURVE_LEFT)
			{//���ɋȂ���ꍇ
				if (pHuman->pos.x >= pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x + (HUMAN_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

				  // �l�Ԃ̊p�x�X�V�E�␳����
					CurveRotHuman(pHuman);

					// �ړ��ʂ�����
					pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

					// �E�̕ǂɔ��킹��
					pHuman->pos.x = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x + (HUMAN_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
				  // ���̕ǂɔ��킹��
					pHuman->pos.z = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z - (HUMAN_WIDTH * 2);
				}
			}
		}
		else
		{ // ���ɑ����Ă���ꍇ
			if (pHuman->humanCurve.curveAngle[pHuman->humanCurve.nNowCurve] == CURVE_RIGHT)
			{//�E�ɋȂ���ꍇ
				if (pHuman->pos.x <= pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x + (HUMAN_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

				  // �l�Ԃ̊p�x�X�V�E�␳����
					CurveRotHuman(pHuman);

					// �ړ��ʂ�����
					pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

					// ���̕ǂɔ��킹��
					pHuman->pos.x = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x + (HUMAN_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
				  // ��O�̕ǂɔ��킹��
					pHuman->pos.z = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z + (HUMAN_WIDTH * 2);
				}
			}
			else if (pHuman->humanCurve.curveAngle[pHuman->humanCurve.nNowCurve] == CURVE_LEFT)
			{//���ɋȂ���ꍇ
				if (pHuman->pos.x <= pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x - (HUMAN_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

				  // �l�Ԃ̊p�x�X�V�E�␳����
					CurveRotHuman(pHuman);

					// �ړ��ʂ�����
					pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

					// ���̕ǂɔ��킹��
					pHuman->pos.x = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x - (HUMAN_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
				  // ��O�̕ǂɔ��킹��
					pHuman->pos.z = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z + (HUMAN_WIDTH * 2);
				}
			}
		}
	}
	else
	{//Z���𑖂��Ă����ꍇ
		if (pHuman->humanCurve.bCurvePlus[pHuman->humanCurve.nNowCurve] == true)
		{ // �E�̕ǂ��x�@��荶���ɂ���ꍇ
			if (pHuman->humanCurve.curveAngle[pHuman->humanCurve.nNowCurve] == CURVE_RIGHT)
			{//�E�ɋȂ���ꍇ
				if (pHuman->pos.z >= pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z - (HUMAN_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

				  // �l�Ԃ̊p�x�X�V�E�␳����
					CurveRotHuman(pHuman);

					// �ړ��ʂ�����
					pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

					// ��O�̕ǂɔ��킹��
					pHuman->pos.z = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z - (HUMAN_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
				  // �E�̕ǂɔ��킹��
					pHuman->pos.x = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x + (HUMAN_WIDTH * 2);
				}
			}
			else if (pHuman->humanCurve.curveAngle[pHuman->humanCurve.nNowCurve] == CURVE_LEFT)
			{ // ���ɋȂ���ꍇ
				if (pHuman->pos.z >= pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z + (HUMAN_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

				  // �l�Ԃ̊p�x�X�V�E�␳����
					CurveRotHuman(pHuman);

					// �ړ��ʂ�����
					pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

					// ��O�̕ǂɔ��킹��
					pHuman->pos.z = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z + (HUMAN_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
				  // �E�̕ǂɔ��킹��
					pHuman->pos.x = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x + (HUMAN_WIDTH * 2);
				}
			}
		}
		else
		{//���̕ǂ��x�@���E�ɂ���ꍇ
			if (pHuman->humanCurve.curveAngle[pHuman->humanCurve.nNowCurve] == CURVE_RIGHT)
			{//�E�ɋȂ���ꍇ
				if (pHuman->pos.z <= pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z + (HUMAN_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

				  // �l�Ԃ̊p�x�X�V�E�␳����
					CurveRotHuman(pHuman);

					// �ړ��ʂ�����
					pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

					// ���̕ǂɔ��킹��
					pHuman->pos.z = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z + (HUMAN_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
				  // ���̕ǂɔ��킹��
					pHuman->pos.x = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x - (HUMAN_WIDTH * 2);
				}
			}
			else if (pHuman->humanCurve.curveAngle[pHuman->humanCurve.nNowCurve] == CURVE_LEFT)
			{
				if (pHuman->pos.z <= pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z - (HUMAN_WIDTH * 2))
				{ // ���ɂ���ǂ��r�؂ꂽ��

				  // �l�Ԃ̊p�x�X�V�E�␳����
					CurveRotHuman(pHuman);

					// �ړ��ʂ�����
					pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

					// ���̕ǂɔ��킹��
					pHuman->pos.z = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].z - (HUMAN_WIDTH * 2);
				}
				else
				{ // ���ɂ���ǂ��܂���������
				  // ���̕ǂɔ��킹��
					pHuman->pos.x = pHuman->humanCurve.curvePoint[pHuman->humanCurve.nNowCurve].x - (HUMAN_WIDTH * 2);
				}
			}
		}
	}

	if (pHuman->move.x > MAX_HUMAN_FORWARD)
	{ // �v���C���[�̈ړ��� (x) �����l�ȏ�̏ꍇ

	  // �v���C���[�̈ړ��� (x) ��␳
		pHuman->move.x = MAX_HUMAN_FORWARD;
	}
}

//============================================================
// �l�Ԃ̊p�x�X�V�E�␳����
//============================================================
void CurveRotHuman(Human *pHuman)
{
	if (pHuman->humanCurve.curveAngle[pHuman->humanCurve.nNowCurve] == CURVE_LEFT)
	{ // �Ȃ�����������������ꍇ
	  // �������X�V
		pHuman->rot.y -= 0.05f * (pHuman->move.x * 0.1f);

		if (pHuman->rot.y <= (float)D3DXToRadian(pHuman->humanCurve.fCurveRot[pHuman->humanCurve.nNowCurve]))
		{ // ���̌����ɒB�����ꍇ
		  // ������␳
			pHuman->rot.y = (float)D3DXToRadian(pHuman->humanCurve.fCurveRot[pHuman->humanCurve.nNowCurve]);

			if (pHuman->humanCurve.curveAngle[(pHuman->humanCurve.nNowCurve + 1) % pHuman->humanCurve.nCurveTime] == CURVE_LEFT)
			{//���̋Ȃ���p�����������ꍇ
				if (pHuman->humanCurve.fCurveRot[(pHuman->humanCurve.nNowCurve + 1) % pHuman->humanCurve.nCurveTime] == 90)
				{//���̋Ȃ���p���}�C�i�X�����ɋȂ���ꍇ
				 //�������v���X�ɖ߂�
					pHuman->rot.y = (float)D3DXToRadian(180);
				}
			}

			//// �l�Ԃ̕␳�̍X�V����
			//RevHuman(&pHuman->rot, &pHuman->pos);

			// �x�@�̍s���ݒ肷��
			pHuman->humanCurve.nNowCurve = (pHuman->humanCurve.nNowCurve + 1) % pHuman->humanCurve.nCurveTime;
		}
	}
	else if (pHuman->humanCurve.curveAngle[pHuman->humanCurve.nNowCurve] == CURVE_RIGHT)
	{ // �Ȃ���������E�������ꍇ
	  // �������X�V
		pHuman->rot.y += 0.05f * (pHuman->move.x * 0.1f);

		if (pHuman->rot.y >= (float)D3DXToRadian(pHuman->humanCurve.fCurveRot[pHuman->humanCurve.nNowCurve]))
		{ // ���̌����ɒB�����ꍇ
		  // ������␳
			pHuman->rot.y = (float)D3DXToRadian(pHuman->humanCurve.fCurveRot[pHuman->humanCurve.nNowCurve]);

			if (pHuman->humanCurve.curveAngle[(pHuman->humanCurve.nNowCurve + 1) % pHuman->humanCurve.nCurveTime] == CURVE_RIGHT)
			{//���̋Ȃ���p���E�������ꍇ
				if (pHuman->humanCurve.fCurveRot[(pHuman->humanCurve.nNowCurve + 1) % pHuman->humanCurve.nCurveTime] == -90)
				{//���̋Ȃ���p���}�C�i�X�����ɋȂ���ꍇ
				 //�������v���X�ɖ߂�
					pHuman->rot.y = (float)D3DXToRadian(-180);
				}
			}

			//// �l�Ԃ̕␳�̍X�V����
			//RevHuman(&pHuman->rot, &pHuman->pos);

			// �x�@�̍s���ݒ肷��
			pHuman->humanCurve.nNowCurve = (pHuman->humanCurve.nNowCurve + 1) % pHuman->humanCurve.nCurveTime;
		}
	}
}

//============================================================
// �l�Ԃ̒�~����
//============================================================
void StopHuman(Human *pHuman)
{
	//�ړ��ʂ�0�ɂ���
	pHuman->move.x = 0.0f;
}

//============================================================
//�l�Ԃ̃��A�N�V��������
//============================================================
void ReactionHuman(Human *pHuman)
{
	float fLength;										// �����̕ϐ�

	Player *pPlayer = GetPlayer();

	if (pHuman->state != HUMANSTATE_FLY)
	{ // ������я�Ԃ���Ȃ��ꍇ
		if (pPlayer->bUse == true)
		{ // �g�p���Ă���ꍇ
			// �����𑪂�
			fLength = (pPlayer->pos.x - pHuman->pos.x) * (pPlayer->pos.x - pHuman->pos.x)
				+ (pPlayer->pos.z - pHuman->pos.z) * (pPlayer->pos.z - pHuman->pos.z);

			if (fLength <= (pPlayer->modelData.fRadius + 50.0f) * 170.0f)
			{ // �v���C���[���߂��ɗ����ꍇ
				//��~�����Ɉڍs
				pHuman->state = HUMANSTATE_STOP;
			}
			else
			{ // �v���C���[�ɋ߂��ɂ��Ȃ��ꍇ
				//��~�����Ɉڍs
				pHuman->state = HUMANSTATE_WALK;
			}
		}
	}
}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================
#endif