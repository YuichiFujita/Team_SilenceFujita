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

#ifdef _DEBUG	// �f�o�b�O����
#include "game.h"
#endif

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************

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

//======================================================================================================================
//	�J�[�u�̐ݒ菈��
//======================================================================================================================
void SetCurvePoint(CURVE *pCurve, D3DXVECTOR3 *rot, D3DXVECTOR3 *pos)
{
	for (int nCnt = 0; nCnt < pCurve->nCurveTime; nCnt++)
	{ // ���鎲�ƕ��������߂�
		if ((int)pCurve->curvePoint[nCnt].x == (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].x)
		{ // X���W��������������
			// Z���𑖂��Ă���
			pCurve->bCurveX[(nCnt + 1) % pCurve->nCurveTime] = false;

			if ((int)pCurve->curvePoint[nCnt].z < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].z)
			{ // �s���Z���W�����̃|�C���g�������������ꍇ
				// �v���X�����ɑ����Ă���(���ɑ����Ă���)
				pCurve->bCurvePlus[(nCnt + 1) % pCurve->nCurveTime] = true;
			}
			else
			{ // �s���Z���W�����̃|�C���g������O�������ꍇ
				// �}�C�i�X�����ɑ����Ă���(��O�ɑ����Ă���)
				pCurve->bCurvePlus[(nCnt + 1) % pCurve->nCurveTime] = false;
			}
		}
		else
		{ // Z���W��������������
			// X���𑖂��Ă���
			pCurve->bCurveX[(nCnt + 1) % pCurve->nCurveTime] = true;

			if ((int)pCurve->curvePoint[nCnt].x < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].x)
			{ // �s���X���W�����̃|�C���g�����E�������ꍇ
				// �v���X�����ɑ����Ă���
				pCurve->bCurvePlus[(nCnt + 1) % pCurve->nCurveTime] = true;
			}
			else
			{ // �s���X���W�����̃|�C���g�������������ꍇ
				// �}�C�i�X�����ɑ����Ă���
				pCurve->bCurvePlus[(nCnt + 1) % pCurve->nCurveTime] = false;
			}
		}
	}

	for (int nCnt = 0; nCnt < pCurve->nCurveTime; nCnt++)
	{ // ����Ȃ�����������߂�
		if (pCurve->bCurveX[nCnt] == true)
		{//X���𑖂��Ă���ꍇ
			if (pCurve->bCurvePlus[nCnt] == true)
			{//�E�ɑ����Ă���ꍇ
				if ((int)pCurve->curvePoint[nCnt].z < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].z)
				{ // ���̖ړI�n�����������ꍇ
					// ���Ȃ���ɂȂ�
					pCurve->curveAngle[nCnt] = CURVE_LEFT;
				}
				else
				{ // ���̖ړI�n����O�������ꍇ
					// �E�Ȃ���ɂȂ�
					pCurve->curveAngle[nCnt] = CURVE_RIGHT;
				}
			}
			else
			{//���ɑ����Ă���ꍇ
				if ((int)pCurve->curvePoint[nCnt].z < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].z)
				{ // ���̖ړI�n�����������ꍇ
					// ���Ȃ���ɂȂ�
					pCurve->curveAngle[nCnt] = CURVE_RIGHT;
				}
				else
				{ // ���̖ړI�n����O�������ꍇ
					// �E�Ȃ���ɂȂ�
					pCurve->curveAngle[nCnt] = CURVE_LEFT;
				}
			}
		}
		else
		{//Z���𑖂��Ă���ꍇ
			if (pCurve->bCurvePlus[nCnt] == true)
			{//���ɑ����Ă���ꍇ
				if ((int)pCurve->curvePoint[nCnt].x < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].x)
				{ // ���̖ړI�n���E�������ꍇ
					//�E�Ȃ���ɂȂ�
					pCurve->curveAngle[nCnt] = CURVE_RIGHT;
				}
				else
				{ // ���̖ړI�n�����������ꍇ
					//�E�Ȃ���ɂȂ�
					pCurve->curveAngle[nCnt] = CURVE_LEFT;
				}
			}
			else
			{//��O�ɑ����Ă���ꍇ
				if ((int)pCurve->curvePoint[nCnt].x < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].x)
				{ // ���̖ړI�n���E�������ꍇ
					//�E�Ȃ���ɂȂ�
					pCurve->curveAngle[nCnt] = CURVE_LEFT;
				}
				else
				{ // ���̖ړI�n�����������ꍇ
					//�E�Ȃ���ɂȂ�
					pCurve->curveAngle[nCnt] = CURVE_RIGHT;
				}
			}
		}
	}

	for (int nCnt = 0; nCnt < pCurve->nCurveTime; nCnt++)
	{ // �Ȃ���񐔕���
		if (pCurve->bCurveX[nCnt] == true)
		{ // X�����ړ����Ă����ꍇ
			if (pCurve->bCurvePlus[nCnt] == true)
			{ // �E�����Ɉړ����Ă����ꍇ

				//�Ȃ���|�C���g��Z����75.0f�߂�
				pCurve->curvePoint[nCnt].z -= SHIFT_CAR_CURVE;

				if (nCnt == 0)
				{//���ڂ̏ꍇ
					//������90�x�ɐݒ肷��
					rot->y = D3DXToRadian(90);
				}

				if (pCurve->curveAngle[nCnt] == CURVE_RIGHT)
				{ // �E�ɋȂ���ꍇ
					//�Ȃ���|�C���g��X����75.0f�i�߂�
					pCurve->curvePoint[nCnt].x -= SHIFT_CAR_CURVE;
				}
				else if (pCurve->curveAngle[nCnt] == CURVE_LEFT)
				{ // ���ɋȂ���ꍇ
				  //�Ȃ���|�C���g��X����75.0f�߂�
					pCurve->curvePoint[nCnt].x += SHIFT_CAR_CURVE;
				}
			}
			else
			{ // �������Ɉړ����Ă����ꍇ

				//�Ȃ���|�C���g��Z����75.0f�i�߂�
				pCurve->curvePoint[nCnt].z += SHIFT_CAR_CURVE;

				if (nCnt == 0)
				{//���ڂ̏ꍇ
					//������-90�x�ɐݒ肷��
					rot->y = D3DXToRadian(-90);
				}

				if (pCurve->curveAngle[nCnt] == CURVE_RIGHT)
				{ // �E�ɋȂ���ꍇ
				  //�Ȃ���|�C���g��X����75.0f�i�߂�
					pCurve->curvePoint[nCnt].x += SHIFT_CAR_CURVE;
				}
				else if (pCurve->curveAngle[nCnt] == CURVE_LEFT)
				{ // ���ɋȂ���ꍇ
				  //�Ȃ���|�C���g��X����75.0f�߂�
					pCurve->curvePoint[nCnt].x -= SHIFT_CAR_CURVE;
				}
			}
		}
		else
		{ // Z�����ړ����Ă����ꍇ
			if (pCurve->bCurvePlus[nCnt] == true)
			{ // �������Ɉړ����Ă����ꍇ

				//�Ȃ���|�C���g��X����i�߂�
				pCurve->curvePoint[nCnt].x += SHIFT_CAR_CURVE;

				if (nCnt == 0)
				{//���ڂ̏ꍇ
				 //������0�x�ɐݒ肷��
					rot->y = D3DXToRadian(0);
				}

				if (pCurve->curveAngle[nCnt] == CURVE_RIGHT)
				{ // �E�ɋȂ���ꍇ
				  //�Ȃ���|�C���g��Z����75.0f�i�߂�
					pCurve->curvePoint[nCnt].z -= SHIFT_CAR_CURVE;
				}
				else if (pCurve->curveAngle[nCnt] == CURVE_LEFT)
				{ // ���ɋȂ���ꍇ
				  //�Ȃ���|�C���g��Z����75.0f�߂�
					pCurve->curvePoint[nCnt].z += SHIFT_CAR_CURVE;
				}
			}
			else
			{ // ��O�����Ɉړ����Ă����ꍇ

				//�Ȃ���|�C���g��X����75.0f�߂�
				pCurve->curvePoint[nCnt].x -= SHIFT_CAR_CURVE;

				if (pCurve->curveAngle[nCnt] == CURVE_RIGHT)
				{ // �E�ɋȂ���ꍇ
					//�Ȃ���|�C���g��Z����75.0f�i�߂�
					pCurve->curvePoint[nCnt].z += SHIFT_CAR_CURVE;

					if (nCnt == 0)
					{//���ڂ̏ꍇ
						//������-180�x�ɐݒ肷��
						rot->y = D3DXToRadian(-180);
					}
				}
				else if (pCurve->curveAngle[nCnt] == CURVE_LEFT)
				{ // ���ɋȂ���ꍇ
					//�Ȃ���|�C���g��Z����75.0f�߂�
					pCurve->curvePoint[nCnt].z -= SHIFT_CAR_CURVE;

					if (nCnt == 0)
					{//���ڂ̏ꍇ
					 //������180�x�ɐݒ肷��
						rot->y = D3DXToRadian(180);
					}
				}
			}
		}
	}
}

//======================================================================================================================
//	�l�Ԃ̃J�[�u�̐ݒ菈��
//======================================================================================================================
void SetCurvePointHuman(CURVE *pCurve, D3DXVECTOR3 *rot, D3DXVECTOR3 *pos)
{
	for (int nCnt = 0; nCnt < pCurve->nCurveTime; nCnt++)
	{ // ���鎲�ƕ��������߂�
		if ((int)pCurve->curvePoint[nCnt].x == (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].x)
		{ // X���W��������������
			// Z���𑖂��Ă���
			pCurve->bCurveX[(nCnt + 1) % pCurve->nCurveTime] = false;

			if ((int)pCurve->curvePoint[nCnt].z < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].z)
			{ // �s���Z���W�����̃|�C���g�������������ꍇ
				// �v���X�����ɑ����Ă���(���ɑ����Ă���)
				pCurve->bCurvePlus[(nCnt + 1) % pCurve->nCurveTime] = true;
			}
			else
			{ // �s���Z���W�����̃|�C���g������O�������ꍇ
				// �}�C�i�X�����ɑ����Ă���(��O�ɑ����Ă���)
				pCurve->bCurvePlus[(nCnt + 1) % pCurve->nCurveTime] = false;
			}
		}
		else
		{ // Z���W��������������
			// X���𑖂��Ă���
			pCurve->bCurveX[(nCnt + 1) % pCurve->nCurveTime] = true;

			if ((int)pCurve->curvePoint[nCnt].x < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].x)
			{ // �s���X���W�����̃|�C���g�����E�������ꍇ
				// �v���X�����ɑ����Ă���
				pCurve->bCurvePlus[(nCnt + 1) % pCurve->nCurveTime] = true;
			}
			else
			{ // �s���X���W�����̃|�C���g�������������ꍇ
				// �}�C�i�X�����ɑ����Ă���
				pCurve->bCurvePlus[(nCnt + 1) % pCurve->nCurveTime] = false;
			}
		}
	}

	for (int nCnt = 0; nCnt < pCurve->nCurveTime; nCnt++)
	{ // ����Ȃ�����������߂�
		if (pCurve->bCurveX[nCnt] == true)
		{//X���𑖂��Ă���ꍇ
			if (pCurve->bCurvePlus[nCnt] == true)
			{//�E�ɑ����Ă���ꍇ
				if ((int)pCurve->curvePoint[nCnt].z < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].z)
				{ // ���̖ړI�n�����������ꍇ
					// ���Ȃ���ɂȂ�
					pCurve->curveAngle[nCnt] = CURVE_LEFT;
				}
				else
				{ // ���̖ړI�n����O�������ꍇ
					// �E�Ȃ���ɂȂ�
					pCurve->curveAngle[nCnt] = CURVE_RIGHT;
				}
			}
			else
			{//���ɑ����Ă���ꍇ
				if ((int)pCurve->curvePoint[nCnt].z < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].z)
				{ // ���̖ړI�n�����������ꍇ
					// ���Ȃ���ɂȂ�
					pCurve->curveAngle[nCnt] = CURVE_RIGHT;
				}
				else
				{ // ���̖ړI�n����O�������ꍇ
					// �E�Ȃ���ɂȂ�
					pCurve->curveAngle[nCnt] = CURVE_LEFT;
				}
			}
		}
		else
		{//Z���𑖂��Ă���ꍇ
			if (pCurve->bCurvePlus[nCnt] == true)
			{//���ɑ����Ă���ꍇ
				if ((int)pCurve->curvePoint[nCnt].x < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].x)
				{ // ���̖ړI�n���E�������ꍇ
					//�E�Ȃ���ɂȂ�
					pCurve->curveAngle[nCnt] = CURVE_RIGHT;
				}
				else
				{ // ���̖ړI�n�����������ꍇ
					//�E�Ȃ���ɂȂ�
					pCurve->curveAngle[nCnt] = CURVE_LEFT;
				}
			}
			else
			{//��O�ɑ����Ă���ꍇ
				if ((int)pCurve->curvePoint[nCnt].x < (int)pCurve->curvePoint[(nCnt + 1) % pCurve->nCurveTime].x)
				{ // ���̖ړI�n���E�������ꍇ
					//�E�Ȃ���ɂȂ�
					pCurve->curveAngle[nCnt] = CURVE_LEFT;
				}
				else
				{ // ���̖ړI�n�����������ꍇ
					//�E�Ȃ���ɂȂ�
					pCurve->curveAngle[nCnt] = CURVE_RIGHT;
				}
			}
		}
	}

	for (int nCnt = 0; nCnt < pCurve->nCurveTime; nCnt++)
	{ // �Ȃ���񐔕���
		if (pCurve->bCurveX[nCnt] == true)
		{ // X�����ړ����Ă����ꍇ
			if (pCurve->bCurvePlus[nCnt] == true)
			{ // �E�����Ɉړ����Ă����ꍇ

				//�Ȃ���|�C���g��Z����75.0f�߂�
				pCurve->curvePoint[nCnt].z += SHIFT_HUMAN_CURVE;

				if (nCnt == 0)
				{//���ڂ̏ꍇ
					//������90�x�ɐݒ肷��
					rot->y = D3DXToRadian(90);
				}

				if (pCurve->curveAngle[nCnt] == CURVE_RIGHT)
				{ // �E�ɋȂ���ꍇ
					//�Ȃ���|�C���g��X����75.0f�i�߂�
					pCurve->curvePoint[nCnt].x += SHIFT_HUMAN_CURVE;
				}
				else if (pCurve->curveAngle[nCnt] == CURVE_LEFT)
				{ // ���ɋȂ���ꍇ
					//�Ȃ���|�C���g��X����75.0f�߂�
					pCurve->curvePoint[nCnt].x -= SHIFT_HUMAN_CURVE;
				}
			}
			else
			{ // �������Ɉړ����Ă����ꍇ

				//�Ȃ���|�C���g��Z����75.0f�i�߂�
				pCurve->curvePoint[nCnt].z -= SHIFT_HUMAN_CURVE;

				if (nCnt == 0)
				{//���ڂ̏ꍇ
					//������-90�x�ɐݒ肷��
					rot->y = D3DXToRadian(-90);
				}

				if (pCurve->curveAngle[nCnt] == CURVE_RIGHT)
				{ // �E�ɋȂ���ꍇ
					//�Ȃ���|�C���g��X����75.0f�i�߂�
					pCurve->curvePoint[nCnt].x -= SHIFT_HUMAN_CURVE;
				}
				else if (pCurve->curveAngle[nCnt] == CURVE_LEFT)
				{ // ���ɋȂ���ꍇ
					//�Ȃ���|�C���g��X����75.0f�߂�
					pCurve->curvePoint[nCnt].x += SHIFT_HUMAN_CURVE;
				}
			}
		}
		else
		{ // Z�����ړ����Ă����ꍇ
			if (pCurve->bCurvePlus[nCnt] == true)
			{ // �������Ɉړ����Ă����ꍇ

				//�Ȃ���|�C���g��X����i�߂�
				pCurve->curvePoint[nCnt].x -= SHIFT_HUMAN_CURVE;

				if (nCnt == 0)
				{//���ڂ̏ꍇ
					//������0�x�ɐݒ肷��
					rot->y = D3DXToRadian(0);
				}

				if (pCurve->curveAngle[nCnt] == CURVE_RIGHT)
				{ // �E�ɋȂ���ꍇ
					//�Ȃ���|�C���g��Z����75.0f�i�߂�
					pCurve->curvePoint[nCnt].z += SHIFT_HUMAN_CURVE;
				}
				else if (pCurve->curveAngle[nCnt] == CURVE_LEFT)
				{ // ���ɋȂ���ꍇ
					//�Ȃ���|�C���g��Z����75.0f�߂�
					pCurve->curvePoint[nCnt].z -= SHIFT_HUMAN_CURVE;
				}
			}
			else
			{ // ��O�����Ɉړ����Ă����ꍇ

				//�Ȃ���|�C���g��X����75.0f�߂�
				pCurve->curvePoint[nCnt].x += SHIFT_HUMAN_CURVE;

				if (pCurve->curveAngle[nCnt] == CURVE_RIGHT)
				{ // �E�ɋȂ���ꍇ
					//�Ȃ���|�C���g��Z����75.0f�i�߂�
					pCurve->curvePoint[nCnt].z -= SHIFT_HUMAN_CURVE;

					if (nCnt == 0)
					{//���ڂ̏ꍇ
						//������-180�x�ɐݒ肷��
						rot->y = D3DXToRadian(-180);
					}
				}
				else if (pCurve->curveAngle[nCnt] == CURVE_LEFT)
				{ // ���ɋȂ���ꍇ
					//�Ȃ���|�C���g��Z����75.0f�߂�
					pCurve->curvePoint[nCnt].z += SHIFT_HUMAN_CURVE;

					if (nCnt == 0)
					{//���ڂ̏ꍇ
						//������180�x�ɐݒ肷��
						rot->y = D3DXToRadian(180);
					}
				}
			}
		}
	}
}

//============================================================
// �Ԃ̊p�x�X�V�E�␳����
//============================================================
void CurveRotCar(CURVE *pCurve,D3DXVECTOR3 *rot,D3DXVECTOR3 *move)
{
	if (pCurve->curveAngle[pCurve->nNowCurve] == CURVE_LEFT)
	{ // �Ȃ�����������������ꍇ
		// �������X�V
		rot->y -= 0.05f * (move->x * 0.1f);

		if (rot->y <= (float)D3DXToRadian(pCurve->fCurveRot[pCurve->nNowCurve]))
		{ // ���̌����ɒB�����ꍇ
			// ������␳
			rot->y = (float)D3DXToRadian(pCurve->fCurveRot[pCurve->nNowCurve]);

			if (pCurve->curveAngle[(pCurve->nNowCurve + 1) % pCurve->nCurveTime] == CURVE_LEFT)
			{//���̋Ȃ���p�����������ꍇ
				if (pCurve->fCurveRot[(pCurve->nNowCurve + 1) % pCurve->nCurveTime] == 90)
				{//���̋Ȃ���p���}�C�i�X�����ɋȂ���ꍇ
					//�������v���X�ɖ߂�
					rot->y = (float)D3DXToRadian(180);
				}
			}

			//// �Ԃ̕␳�̍X�V����
			//RevCar(&pCar->rot, &pCar->pos);

			// �x�@�̍s���ݒ肷��
			pCurve->nNowCurve = (pCurve->nNowCurve + 1) % pCurve->nCurveTime;
		}
	}
	else if (pCurve->curveAngle[pCurve->nNowCurve] == CURVE_RIGHT)
	{ // �Ȃ���������E�������ꍇ
		// �������X�V
		rot->y += 0.05f * (move->x * 0.1f);

		if (rot->y >= (float)D3DXToRadian(pCurve->fCurveRot[pCurve->nNowCurve]))
		{ // ���̌����ɒB�����ꍇ
			// ������␳
			rot->y = (float)D3DXToRadian(pCurve->fCurveRot[pCurve->nNowCurve]);

			if (pCurve->curveAngle[(pCurve->nNowCurve + 1) % pCurve->nCurveTime] == CURVE_RIGHT)
			{//���̋Ȃ���p���E�������ꍇ
				if (pCurve->fCurveRot[(pCurve->nNowCurve + 1) % pCurve->nCurveTime] == -90)
				{//���̋Ȃ���p���}�C�i�X�����ɋȂ���ꍇ
					//�������}�C�i�X�ɖ߂�
					rot->y = (float)D3DXToRadian(-180);
				}
			}

			//// �Ԃ̕␳�̍X�V����
			//RevCar(&pCar->rot, &pCar->pos);

			// �x�@�̍s���ݒ肷��
			pCurve->nNowCurve = (pCurve->nNowCurve + 1) % pCurve->nCurveTime;
		}
	}
}

//============================================================
// �Ԃ̊p�x�X�V�E�␳����
//============================================================
void CurveInfoRotCar(CURVEINFO *pCurve, D3DXVECTOR3 *rot, D3DXVECTOR3 *move, int *pCnt)
{
	if (pCurve->curveAngle == CURVE_LEFT)
	{ // �Ȃ�����������������ꍇ
		// �������X�V
		rot->y -= D3DX_PI * 0.5f;

		//���x�𗎂Ƃ�
		move->x *= 0.3f;

		if (pCurve->dashAngle == DASH_RIGHT)
		{ // �E�ɑ����Ă���ꍇ
			// ���ɑ���
			pCurve->dashAngle = DASH_FAR;
		}
		else if (pCurve->dashAngle == DASH_LEFT)
		{ // ���ɑ����Ă���ꍇ
			// ��O�ɑ���
			pCurve->dashAngle = DASH_NEAR;
		}
		else if (pCurve->dashAngle == DASH_FAR)
		{ // ���ɑ����Ă���ꍇ
			// ���ɑ���
			pCurve->dashAngle = DASH_LEFT;
		}
		else if (pCurve->dashAngle == DASH_NEAR)
		{ // ��O�ɑ����Ă���ꍇ
			// �E�ɑ���
			pCurve->dashAngle = DASH_RIGHT;
		}

		*pCnt = rand() % 2 + 1;
	}
	else
	{ // �Ȃ���������E�������ꍇ
		// �������X�V
		rot->y += D3DX_PI * 0.5f;

		//���x�𗎂Ƃ�
		move->x *= 0.3f;

		if (pCurve->dashAngle == DASH_RIGHT)
		{ // �E�ɑ����Ă���ꍇ
			// ���ɑ���
			pCurve->dashAngle = DASH_NEAR;
		}
		else if (pCurve->dashAngle == DASH_LEFT)
		{ // ���ɑ����Ă���ꍇ
			// ��O�ɑ���
			pCurve->dashAngle = DASH_FAR;
		}
		else if (pCurve->dashAngle == DASH_FAR)
		{ // ���ɑ����Ă���ꍇ
			// ���ɑ���
			pCurve->dashAngle = DASH_RIGHT;
		}
		else if (pCurve->dashAngle == DASH_NEAR)
		{ // ��O�ɑ����Ă���ꍇ
			// �E�ɑ���
			pCurve->dashAngle = DASH_LEFT;
		}

		*pCnt = rand() % 2 + 1;
	}
}

//============================================================
//�Ȃ���p�̈ʒu�̎擾����
//============================================================
CURVEINFO GetCurveInfo(int nID)
{
	//�Ȃ���p�̏���Ԃ�
	return g_aCurveInfo[nID];
}