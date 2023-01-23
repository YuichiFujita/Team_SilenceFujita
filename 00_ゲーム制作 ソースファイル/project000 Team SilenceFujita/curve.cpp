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
#define SHIFT_CAR_CURVE		(75.0f)			//�Ԃ̃J�[�u�ł��炷��
#define SHIFT_HUMAN_CURVE	(100.0f)		//�l�̃J�[�u�ł��炷��

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