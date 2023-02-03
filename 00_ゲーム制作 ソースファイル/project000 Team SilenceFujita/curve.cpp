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
#include "Car.h"

#ifdef _DEBUG	// �f�o�b�O����
#include "game.h"
#endif

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define CURVE_MOVE_SLOW_MAGNI			(0.3f)				// �J�[�u����Ƃ��̌����W��
#define CURVE_ADD						(0.03f)				// �J�[�u����Ƃ��̌����̒ǉ���
#define CURVE_TXT						"data\\TXT\\Curve.txt"		// �J�[�u�ݒ�p�̃e�L�X�g�t�@�C���̑��΃p�X
#define CURVE_SKIP_RANDOM				(3)					// �X�L�b�v����񐔂̃����_���͈̔�
#define CURVE_SKIP_LEAST				(2)					// �X�L�b�v����񐔂̍Œᐔ

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
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
	for (int nCnt = 0; nCnt < MAX_CURVEPOINT; nCnt++)
	{ // �Ȃ���p�̏��̏�����
		g_aCurveInfo[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_aCurveInfo[nCnt].nCurveNumber = nCnt;						// �ԍ�
		g_aCurveInfo[nCnt].curveAngle = CURVE_RIGHT;				// �Ȃ������
		g_aCurveInfo[nCnt].dashAngle = DASH_RIGHT;					// �Ȃ���p�ɂ��ǂ蒅���܂ł̑����Ă������
		g_aCurveInfo[nCnt].bDeadEnd = false;						// �s���~�܂肩�ǂ���
	}
}

//============================================================
// �Ԃ̊p�x�X�V�E�␳����
//============================================================
void CurveInfoRotCar(CARCURVE *pCurve, D3DXVECTOR3 *rot, D3DXVECTOR3 *move, D3DXVECTOR3 *pPos)
{
	//���x�𗎂Ƃ�
	move->x *= CURVE_MOVE_SLOW_MAGNI;

	if (pCurve->curveInfo.curveAngle == CURVE_LEFT)
	{ // �Ȃ�����������������ꍇ
		if (pCurve->curveInfo.dashAngle == DASH_RIGHT)
		{ // �E�ɑ����Ă���ꍇ

			// �ʒu��␳����
			pPos->x = pCurve->curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + CURVE_SKIP_LEAST;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_LEFT)
		{ // ���ɑ����Ă���ꍇ

			// �ʒu��␳����
			pPos->x = pCurve->curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + CURVE_SKIP_LEAST;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_FAR)
		{ // ���ɑ����Ă���ꍇ

			// �ʒu��␳����
			pPos->z = pCurve->curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + CURVE_SKIP_LEAST;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_NEAR)
		{ // ��O�ɑ����Ă���ꍇ

			// �ʒu��␳����
			pPos->z = pCurve->curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + CURVE_SKIP_LEAST;
			}
		}
	}
	else
	{ // �Ȃ���������E�������ꍇ
		//���x�𗎂Ƃ�
		move->x *= 0.3f;

		if (pCurve->curveInfo.dashAngle == DASH_RIGHT)
		{ // �E�ɑ����Ă���ꍇ

			// �ʒu��␳����
			pPos->x = pCurve->curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + CURVE_SKIP_LEAST;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_LEFT)
		{ // ���ɑ����Ă���ꍇ

			// �ʒu��␳����
			pPos->x = pCurve->curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + CURVE_SKIP_LEAST;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_FAR)
		{ // ���ɑ����Ă���ꍇ

			// �ʒu��␳����
			pPos->z = pCurve->curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + CURVE_SKIP_LEAST;
			}
		}
		else if (pCurve->curveInfo.dashAngle == DASH_NEAR)
		{ // ��O�ɑ����Ă���ꍇ

			// �ʒu��␳����
			pPos->z = pCurve->curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + CURVE_SKIP_LEAST;
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

//============================================================
// �J�[�u�e�L�X�g�̃��[�h����
//============================================================
void LoadCurveTxt(void)
{
	// �ϐ��z���錾
	int			 nEnd;					// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	char         aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p
	int			 nCurveNumber = 0;		// �Ȃ���p�̔ԍ�
	char		 aCurveAngle[10];		// �Ȃ������
	char		 aDashAngle[10];		// �i��ł������
	char		 aDeadEnd[10];			// �s���~�܂肩�ǂ����̑���p

	// �|�C���^��錾
	FILE *pFile;				// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(CURVE_TXT, "r");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			//------------------------------------------------
			//	�J�[�u�̐ݒ�
			//------------------------------------------------
			if (strcmp(&aString[0], "SET_CURVE") == 0)
			{ // �ǂݍ��񂾕����� SET_CURVE �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_SET_CURVE �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "CURVE_INFO") == 0)
					{ // �ǂݍ��񂾕����� CURVE_INFO �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// NUMBER ��ǂݍ��� (�s�v)
						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nCurveNumber);			// �ԍ���ǂݍ���

						// �ԍ���������
						g_aCurveInfo[nCurveNumber].nCurveNumber = nCurveNumber;

						do
						{ // �ǂݍ��񂾕����� END_CURVE_INFO �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "POS") == 0)
							{ // �ǂݍ��񂾕����� POS �̏ꍇ
								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f%f%f", 
									&g_aCurveInfo[nCurveNumber].pos.x, 
									&g_aCurveInfo[nCurveNumber].pos.y, 
									&g_aCurveInfo[nCurveNumber].pos.z);						// �ʒu��ǂݍ���
							}
							else if (strcmp(&aString[0], "CURVEANGLE") == 0)
							{ // �ǂݍ��񂾕����� CURVEANGLE �̏ꍇ
								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%s", &aCurveAngle[0]);						// �Ȃ��������ǂݍ���

								if (strcmp(&aCurveAngle[0], "RIGHT") == 0)
								{ // �E�������Ă����ꍇ
									// �E�ɋȂ���ݒ������
									g_aCurveInfo[nCurveNumber].curveAngle = CURVE_RIGHT;
								}
								else if (strcmp(&aCurveAngle[0], "LEFT") == 0)
								{ // ���������Ă����ꍇ
									// ���ɋȂ���ݒ������
									g_aCurveInfo[nCurveNumber].curveAngle = CURVE_LEFT;
								}
							}
							else if (strcmp(&aString[0], "DASHANGLE") == 0)
							{ // �ǂݍ��񂾕����� SCALE �̏ꍇ
								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%s", &aDashAngle[0]);						// �i��ł��������ǂݍ���

								if (strcmp(&aDashAngle[0], "RIGHT") == 0)
								{ // �E�������Ă����ꍇ
									// �E�ɐi��ł����ݒ������
									g_aCurveInfo[nCurveNumber].dashAngle = DASH_RIGHT;
								}
								else if (strcmp(&aDashAngle[0], "LEFT") == 0)
								{ // ���������Ă����ꍇ
									// ���ɐi��ł����ݒ������
									g_aCurveInfo[nCurveNumber].dashAngle = DASH_LEFT;
								}
								else if (strcmp(&aDashAngle[0], "FAR") == 0)
								{ // ���������Ă����ꍇ
									// ���ɐi��ł����ݒ������
									g_aCurveInfo[nCurveNumber].dashAngle = DASH_FAR;
								}
								else if (strcmp(&aDashAngle[0], "NEAR") == 0)
								{ // ��O�������Ă����ꍇ
									// ��O�ɐi��ł����ݒ������
									g_aCurveInfo[nCurveNumber].dashAngle = DASH_NEAR;
								}
							}
							else if (strcmp(&aString[0], "DEADEND") == 0)
							{ // �ǂݍ��񂾕����� TYPE �̏ꍇ
								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%s", &aDeadEnd[0]);							// �s���~�܂肩�ǂ�����ǂݍ���

								if (strcmp(&aDeadEnd[0], "true") == 0)
								{ // true�Ə�����Ă����ꍇ
									// �s���~�܂�ɂ���
									g_aCurveInfo[nCurveNumber].bDeadEnd = true;
								}
								else
								{ // false(��L�ȊO�̌��t)�Ə�����Ă����ꍇ
									// �s���~�܂�ɂ��Ȃ�
									g_aCurveInfo[nCurveNumber].bDeadEnd = false;
								}
							}
						} while (strcmp(&aString[0], "END_CURVE_INFO") != 0);	// �ǂݍ��񂾕����� END_CURVE_INFO �ł͂Ȃ��ꍇ���[�v
					}
				} while (strcmp(&aString[0], "END_SET_CURVE") != 0);			// �ǂݍ��񂾕����� END_SET_CURVE �ł͂Ȃ��ꍇ���[�v
			}
		} while (nEnd != EOF);													// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�J�[�u�t�@�C���̓ǂݍ��݂Ɏ��s�ł�", "���܂��݂�www", MB_ICONWARNING);
	}
}