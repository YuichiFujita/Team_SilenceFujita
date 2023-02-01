//======================================================================================================================
//
//	�J�[�u�̏��� [Curve.cpp]
//	Author�F��������
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "curve.h"
#include "calculation.h"

#ifdef _DEBUG	// �f�o�b�O����
#include "game.h"
#endif

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define CURVE_MOVE_SLOW_MAGNI			(0.3f)				// �J�[�u����Ƃ��̌����W��
#define CURVE_ADD						(0.03f)				// �J�[�u����Ƃ��̌����̒ǉ���

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
CURVEINFO g_aCurveInfo[MAX_CURVEPOINT];		// �Ԃ̃J�[�u�̈ʒu�̏��

//======================================================================================================================
// �J�[�u�̏��̏���������
//======================================================================================================================
void InitCurveInfo(void)
{
	g_aCurveInfo[0].pos = D3DXVECTOR3(-7000.0f, 0.0f, 7000.0f);
	g_aCurveInfo[0].nCurveNumber = 0;
	g_aCurveInfo[0].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[0].nPairNumber = 1;
	g_aCurveInfo[0].dashAngle = DASH_FAR;
	g_aCurveInfo[0].bDeadEnd = true;

	g_aCurveInfo[1].pos = D3DXVECTOR3(-6500.0f, 0.0f, 6500.0f);
	g_aCurveInfo[1].nCurveNumber = 1;
	g_aCurveInfo[1].curveAngle = CURVE_LEFT;
	g_aCurveInfo[1].nPairNumber = 0;
	g_aCurveInfo[1].dashAngle = DASH_LEFT;
	g_aCurveInfo[1].bDeadEnd = true;

	g_aCurveInfo[2].pos = D3DXVECTOR3(-4375.0f, 0.0f, 7000.0f);
	g_aCurveInfo[2].nCurveNumber = 2;
	g_aCurveInfo[2].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[2].nPairNumber = 3;
	g_aCurveInfo[2].dashAngle = DASH_RIGHT;
	g_aCurveInfo[2].bDeadEnd = false;

	g_aCurveInfo[3].pos = D3DXVECTOR3(-4875.0f, 0.0f, 6500.0f);
	g_aCurveInfo[3].nCurveNumber = 3;
	g_aCurveInfo[3].curveAngle = CURVE_LEFT;
	g_aCurveInfo[3].nPairNumber = 2;
	g_aCurveInfo[3].dashAngle = DASH_FAR;
	g_aCurveInfo[3].bDeadEnd = false;

	g_aCurveInfo[4].pos = D3DXVECTOR3(-4375.0f, 0.0f, 6500.0f);
	g_aCurveInfo[4].nCurveNumber = 4;
	g_aCurveInfo[4].curveAngle = CURVE_LEFT;
	g_aCurveInfo[4].nPairNumber = 5;
	g_aCurveInfo[4].dashAngle = DASH_LEFT;
	g_aCurveInfo[4].bDeadEnd = false;

	g_aCurveInfo[5].pos = D3DXVECTOR3(-4875.0f, 0.0f, 7000.0f);
	g_aCurveInfo[5].nCurveNumber = 5;
	g_aCurveInfo[5].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[5].nPairNumber = 4;
	g_aCurveInfo[5].dashAngle = DASH_FAR;
	g_aCurveInfo[5].bDeadEnd = true;

	g_aCurveInfo[6].pos = D3DXVECTOR3(-2250.0f, 0.0f, 7000.0f);
	g_aCurveInfo[6].nCurveNumber = 6;
	g_aCurveInfo[6].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[6].nPairNumber = 7;
	g_aCurveInfo[6].dashAngle = DASH_RIGHT;
	g_aCurveInfo[6].bDeadEnd = true;

	g_aCurveInfo[7].pos = D3DXVECTOR3(-2750.0f, 0.0f, 6500.0f);
	g_aCurveInfo[7].nCurveNumber = 7;
	g_aCurveInfo[7].curveAngle = CURVE_LEFT;
	g_aCurveInfo[7].nPairNumber = 6;
	g_aCurveInfo[7].dashAngle = DASH_FAR;
	g_aCurveInfo[7].bDeadEnd = true;

	g_aCurveInfo[8].pos = D3DXVECTOR3(-2750.0f, 0.0f, 4750.0f);
	g_aCurveInfo[8].nCurveNumber = 8;
	g_aCurveInfo[8].curveAngle = CURVE_LEFT;
	g_aCurveInfo[8].nPairNumber = 9;
	g_aCurveInfo[8].dashAngle = DASH_RIGHT;
	g_aCurveInfo[8].bDeadEnd = false;

	g_aCurveInfo[9].pos = D3DXVECTOR3(-2250.0f, 0.0f, 4250.0f);
	g_aCurveInfo[9].nCurveNumber = 9;
	g_aCurveInfo[9].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[9].nPairNumber = 8;
	g_aCurveInfo[9].dashAngle = DASH_NEAR;
	g_aCurveInfo[9].bDeadEnd = false;

	g_aCurveInfo[10].pos = D3DXVECTOR3(-2750.0f, 0.0f, 4250.0f);
	g_aCurveInfo[10].nCurveNumber = 10;
	g_aCurveInfo[10].curveAngle = CURVE_LEFT;
	g_aCurveInfo[10].nPairNumber = 11;
	g_aCurveInfo[10].dashAngle = DASH_FAR;
	g_aCurveInfo[10].bDeadEnd = false;

	g_aCurveInfo[11].pos = D3DXVECTOR3(-2250.0f, 0.0f, 4750.0f);
	g_aCurveInfo[11].nCurveNumber = 11;
	g_aCurveInfo[11].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[11].nPairNumber = 10;
	g_aCurveInfo[11].dashAngle = DASH_RIGHT;
	g_aCurveInfo[11].bDeadEnd = true;

	g_aCurveInfo[12].pos = D3DXVECTOR3(-4375.0f, 0.0f, 4250.0f);
	g_aCurveInfo[12].nCurveNumber = 12;
	g_aCurveInfo[12].curveAngle = CURVE_LEFT;
	g_aCurveInfo[12].nPairNumber = 13;
	g_aCurveInfo[12].dashAngle = DASH_LEFT;
	g_aCurveInfo[12].bDeadEnd = false;

	g_aCurveInfo[13].pos = D3DXVECTOR3(-4875.0f, 0.0f, 4750.0f);
	g_aCurveInfo[13].nCurveNumber = 13;
	g_aCurveInfo[13].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[13].nPairNumber = 12;
	g_aCurveInfo[13].dashAngle = DASH_FAR;
	g_aCurveInfo[13].bDeadEnd = false;

	g_aCurveInfo[14].pos = D3DXVECTOR3(-4375.0f, 0.0f, 4750.0f);
	g_aCurveInfo[14].nCurveNumber = 14;
	g_aCurveInfo[14].curveAngle = CURVE_LEFT;
	g_aCurveInfo[14].nPairNumber = 15;
	g_aCurveInfo[14].dashAngle = DASH_NEAR;
	g_aCurveInfo[14].bDeadEnd = false;

	g_aCurveInfo[15].pos = D3DXVECTOR3(-4875.0f, 0.0f, 4250.0f);
	g_aCurveInfo[15].nCurveNumber = 15;
	g_aCurveInfo[15].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[15].nPairNumber = 14;
	g_aCurveInfo[15].dashAngle = DASH_LEFT;
	g_aCurveInfo[15].bDeadEnd = true;

	g_aCurveInfo[16].pos = D3DXVECTOR3(-4875.0f, 0.0f, 2500.0f);
	g_aCurveInfo[16].nCurveNumber = 16;
	g_aCurveInfo[16].curveAngle = CURVE_LEFT;
	g_aCurveInfo[16].nPairNumber = 17;
	g_aCurveInfo[16].dashAngle = DASH_RIGHT;
	g_aCurveInfo[16].bDeadEnd = false;

	g_aCurveInfo[17].pos = D3DXVECTOR3(-4375.0f, 0.0f, 2000.0f);
	g_aCurveInfo[17].nCurveNumber = 17;
	g_aCurveInfo[17].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[17].nPairNumber = 16;
	g_aCurveInfo[17].dashAngle = DASH_NEAR;
	g_aCurveInfo[17].bDeadEnd = true;

	g_aCurveInfo[18].pos = D3DXVECTOR3(-4875.0f, 0.0f, 2000.0f);
	g_aCurveInfo[18].nCurveNumber = 18;
	g_aCurveInfo[18].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[18].nPairNumber = 19;
	g_aCurveInfo[18].dashAngle = DASH_LEFT;
	g_aCurveInfo[18].bDeadEnd = false;

	g_aCurveInfo[19].pos = D3DXVECTOR3(-4375.0f, 0.0f, 2500.0f);
	g_aCurveInfo[19].nCurveNumber = 19;
	g_aCurveInfo[19].curveAngle = CURVE_LEFT;
	g_aCurveInfo[19].nPairNumber = 18;
	g_aCurveInfo[19].dashAngle = DASH_NEAR;
	g_aCurveInfo[19].bDeadEnd = false;

	g_aCurveInfo[20].pos = D3DXVECTOR3(-2750.0f, 0.0f, 2500.0f);
	g_aCurveInfo[20].nCurveNumber = 20;
	g_aCurveInfo[20].curveAngle = CURVE_LEFT;
	g_aCurveInfo[20].nPairNumber = 21;
	g_aCurveInfo[20].dashAngle = DASH_RIGHT;
	g_aCurveInfo[20].bDeadEnd = true;

	g_aCurveInfo[21].pos = D3DXVECTOR3(-2250.0f, 0.0f, 2000.0f);
	g_aCurveInfo[21].nCurveNumber = 21;
	g_aCurveInfo[21].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[21].nPairNumber = 20;
	g_aCurveInfo[21].dashAngle = DASH_NEAR;
	g_aCurveInfo[21].bDeadEnd = true;

	g_aCurveInfo[22].pos = D3DXVECTOR3(-6500.0f, 0.0f, 2500.0f);
	g_aCurveInfo[22].nCurveNumber = 22;
	g_aCurveInfo[22].curveAngle = CURVE_LEFT;
	g_aCurveInfo[22].nPairNumber = 23;
	g_aCurveInfo[22].dashAngle = DASH_NEAR;
	g_aCurveInfo[22].bDeadEnd = true;

	g_aCurveInfo[23].pos = D3DXVECTOR3(-7000.0f, 0.0f, 2000.0f);
	g_aCurveInfo[23].nCurveNumber = 23;
	g_aCurveInfo[23].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[23].nPairNumber = 22;
	g_aCurveInfo[23].dashAngle = DASH_LEFT;
	g_aCurveInfo[23].bDeadEnd = true;
}

//============================================================
// �Ԃ̊p�x�X�V�E�␳����
//============================================================
void CurveInfoRotCar(CARCURVE *pCurve, D3DXVECTOR3 *rot, D3DXVECTOR3 *move)
{
	//���x�𗎂Ƃ�
	move->x *= CURVE_MOVE_SLOW_MAGNI;

	if (pCurve->curveInfo.curveAngle == CURVE_LEFT)
	{ // �Ȃ�����������������ꍇ
		if (pCurve->curveInfo.dashAngle == DASH_RIGHT)
		{ // �E�ɑ����Ă���ꍇ
			// �ڕW�̌�����ݒ肷��
			pCurve->rotDest.y = 0.0f;

			// ������������
			rot->y -= CURVE_ADD;

			if (rot->y <= pCurve->rotDest.y)
			{ // �������ڕW�l�𒴂����ꍇ

				// ������␳����
				rot->y = pCurve->rotDest.y;

				// ���ɑ���
				pCurve->curveInfo.dashAngle = DASH_FAR;

				// ���s��Ԃɂ���
				pCurve->actionState = CARACT_DASH;

				// �X�L�b�v����񐔂�ݒ肷��
				pCurve->nSKipCnt = rand() % 2 + 1;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_LEFT)
		{ // ���ɑ����Ă���ꍇ
			// �ڕW�̌�����ݒ肷��
			pCurve->rotDest.y = -D3DX_PI;

			// ������������
			rot->y -= CURVE_ADD;

			if (rot->y <= pCurve->rotDest.y)
			{ // �������ڕW�l�𒴂����ꍇ

				// ������␳����
				rot->y = pCurve->rotDest.y;

				// ��O�ɑ���
				pCurve->curveInfo.dashAngle = DASH_NEAR;

				// ���s��Ԃɂ���
				pCurve->actionState = CARACT_DASH;

				// �X�L�b�v����񐔂�ݒ肷��
				pCurve->nSKipCnt = rand() % 2 + 1;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_FAR)
		{ // ���ɑ����Ă���ꍇ
			// �ڕW�̌�����ݒ肷��
			pCurve->rotDest.y = -D3DX_PI * 0.5f;

			// ������������
			rot->y -= CURVE_ADD;

			if (rot->y <= pCurve->rotDest.y)
			{ // �������ڕW�l�𒴂����ꍇ

				// ������␳����
				rot->y = pCurve->rotDest.y;

				// ���ɑ���
				pCurve->curveInfo.dashAngle = DASH_LEFT;

				// ���s��Ԃɂ���
				pCurve->actionState = CARACT_DASH;

				// �X�L�b�v����񐔂�ݒ肷��
				pCurve->nSKipCnt = rand() % 2 + 1;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_NEAR)
		{ // ��O�ɑ����Ă���ꍇ
			// �ڕW�̌�����ݒ肷��
			pCurve->rotDest.y = D3DX_PI * 0.5f;

			// ������������
			rot->y -= CURVE_ADD;

			if (rot->y <= pCurve->rotDest.y)
			{ // �������ڕW�l�𒴂����ꍇ

				// ������␳����
				rot->y = pCurve->rotDest.y;

				// �E�ɑ���
				pCurve->curveInfo.dashAngle = DASH_RIGHT;

				// ���s��Ԃɂ���
				pCurve->actionState = CARACT_DASH;

				// �X�L�b�v����񐔂�ݒ肷��
				pCurve->nSKipCnt = rand() % 2 + 1;
			}
		}
	}
	else
	{ // �Ȃ���������E�������ꍇ
		//���x�𗎂Ƃ�
		move->x *= 0.3f;

		if (pCurve->curveInfo.dashAngle == DASH_RIGHT)
		{ // �E�ɑ����Ă���ꍇ

			// �ڕW�̌�����ݒ肷��
			pCurve->rotDest.y = D3DX_PI;

			// ������������
			rot->y += CURVE_ADD;

			if (rot->y >= pCurve->rotDest.y)
			{ // �������ڕW�l�𒴂����ꍇ

				// ������␳����
				rot->y = pCurve->rotDest.y;

				// ��O�ɑ���
				pCurve->curveInfo.dashAngle = DASH_NEAR;

				// ���s��Ԃɂ���
				pCurve->actionState = CARACT_DASH;

				// �X�L�b�v����񐔂�ݒ肷��
				pCurve->nSKipCnt = rand() % 2 + 1;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_LEFT)
		{ // ���ɑ����Ă���ꍇ

			// �ڕW�̌�����ݒ肷��
			pCurve->rotDest.y = 0.0f;

			// ������������
			rot->y += CURVE_ADD;

			if (rot->y >= pCurve->rotDest.y)
			{ // �������ڕW�l�𒴂����ꍇ

				// ������␳����
				rot->y = pCurve->rotDest.y;

				// ���ɑ���
				pCurve->curveInfo.dashAngle = DASH_FAR;

				// ���s��Ԃɂ���
				pCurve->actionState = CARACT_DASH;

				// �X�L�b�v����񐔂�ݒ肷��
				pCurve->nSKipCnt = rand() % 2 + 1;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_FAR)
		{ // ���ɑ����Ă���ꍇ

			// �ڕW�̌�����ݒ肷��
			pCurve->rotDest.y = D3DX_PI * 0.5f;

			// ������������
			rot->y += CURVE_ADD;

			if (rot->y >= pCurve->rotDest.y)
			{ // �������ڕW�l�𒴂����ꍇ

				// ������␳����
				rot->y = pCurve->rotDest.y;

				// �E�ɑ���
				pCurve->curveInfo.dashAngle = DASH_RIGHT;

				// ���s��Ԃɂ���
				pCurve->actionState = CARACT_DASH;

				// �X�L�b�v����񐔂�ݒ肷��
				pCurve->nSKipCnt = rand() % 2 + 1;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_NEAR)
		{ // ��O�ɑ����Ă���ꍇ

			// �ڕW�̌�����ݒ肷��
			pCurve->rotDest.y = -D3DX_PI * 0.5f;

			// ������������
			rot->y += CURVE_ADD;

			if (rot->y >= pCurve->rotDest.y)
			{ // �������ڕW�l�𒴂����ꍇ

				// ������␳����
				rot->y = pCurve->rotDest.y;

				// ���ɑ���
				pCurve->curveInfo.dashAngle = DASH_LEFT;

				// ���s��Ԃɂ���
				pCurve->actionState = CARACT_DASH;

				// �X�L�b�v����񐔂�ݒ肷��
				pCurve->nSKipCnt = rand() % 2 + 1;
			}
		}
	}

	// �����̐��K��
	RotNormalize(rot->y);
}

//============================================================
//�Ȃ���p�̈ʒu�̎擾����
//============================================================
CURVEINFO GetCurveInfo(int nID)
{
	//�Ȃ���p�̏���Ԃ�
	return g_aCurveInfo[nID];
}