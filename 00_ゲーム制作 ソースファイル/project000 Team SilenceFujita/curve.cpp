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
#include "Human.h"

#ifdef _DEBUG	// �f�o�b�O����
#include "game.h"
#endif

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define CURVE_MOVE_SLOW_MAGNI			(0.3f)				// �J�[�u����Ƃ��̌����W��
#define CURVE_ADD						(0.03f)				// �J�[�u����Ƃ��̌����̒ǉ���
#define CURVE_TXT						"data\\TXT\\Curve.txt"		// �J�[�u�ݒ�p�̃e�L�X�g�t�@�C���̑��΃p�X
#define HUMAN_CURVE_TXT					"data\\TXT\\HumanCurve.txt"	// �l�Ԃ̋Ȃ���p(���[�g)�̕ۑ��p�e�L�X�g�t�@�C���̑��΃p�X
#define CURVE_SKIP_RANDOM				(3)					// �X�L�b�v����񐔂̃����_���͈̔�
#define CURVE_SKIP_LEAST				(2)					// �X�L�b�v����񐔂̍Œᐔ

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
CURVEINFO g_aCurveInfo[MAX_CURVEPOINT];						// �Ԃ̃J�[�u�̏��
HUMANCURVEINFO g_aCurveHuman[MAX_HUMAN_ROUTE];				// �l�Ԃ̃J�[�u�̏��
float g_aDefaultRot[MAX_HUMAN_ROUTE];						// �ŏ��̋Ȃ���p

//======================================================================================================================
// �J�[�u�̏��̏���������
//======================================================================================================================
void InitCurveInfo(void)
{
	for (int nCnt = 0; nCnt < MAX_CURVEPOINT; nCnt++)
	{ // �Ԃ̋Ȃ���p�̏��̏�����
		g_aCurveInfo[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_aCurveInfo[nCnt].nCurveNumber = nCnt;						// �ԍ�
		g_aCurveInfo[nCnt].curveAngle = CURVE_RIGHT;				// �Ȃ������
		g_aCurveInfo[nCnt].dashAngle = DASH_RIGHT;					// �Ȃ���p�ɂ��ǂ蒅���܂ł̑����Ă������
		g_aCurveInfo[nCnt].bDeadEnd = false;						// �s���~�܂肩�ǂ���
	}

	for (int nCnt = 0; nCnt < MAX_HUMAN_ROUTE; nCnt++)
	{ // �l�Ԃ̃��[�g�̏��̏�����
		for (int nCntCur = 0; nCntCur < MAX_HUMAN_CURVE; nCntCur++)
		{
			g_aCurveHuman[nCnt].curvePoint[nCntCur] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
			g_aCurveHuman[nCnt].curveAngle[nCntCur] = CURVE_LEFT;						// �Ȃ������
			g_aCurveHuman[nCnt].dashAngle[nCntCur] = DASH_RIGHT;						// �����Ă������
			g_aCurveHuman[nCnt].roadPos[nCntCur] = ROADPOS_RIGHT;						// �Ȃ���p���ǂ������ɕ����Ă��邩
			g_aCurveHuman[nCnt].nNowCurve = 0;											// ���݂̋Ȃ���p�̔ԍ�
			g_aCurveHuman[nCnt].nCurveTime = 1;											// �Ȃ����
		}
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
	RotNormalize(&rot->y);
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

//============================================================
// �l�Ԃ̃��[�g�̃��[�h����
//============================================================
void LoadHumanCurveTxt(void)
{
	// �ϐ��z���錾
	int			 nEnd;					// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	char         aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p
	int			 nRoute = 0;			// ���ڂ̃��[�g��

	// �|�C���^��錾
	FILE *pFile;				// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(HUMAN_CURVE_TXT, "r");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			//------------------------------------------------
			//	�J�[�u�̐ݒ�
			//------------------------------------------------

			if (strcmp(&aString[0], "CURVE_INFO") == 0)
			{ // �ǂݍ��񂾕����� CURVE_INFO �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_CURVE_INFO �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "TIME") == 0)
					{ // �ǂݍ��񂾕����� TIME �̏ꍇ
						fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &g_aCurveHuman[nRoute].nCurveTime);		// �Ȃ���񐔂�ǂݍ���
					}
					else if (strcmp(&aString[0], "POINT") == 0)
					{ // �ǂݍ��񂾕����� POINT �̏ꍇ
						for (int nCnt = 0; nCnt < g_aCurveHuman[nRoute].nCurveTime; nCnt++)
						{
							fscanf(pFile, "%s", &aString[0]);						// POS ��ǂݍ��� (�s�v)
							fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
							fscanf(pFile, "%f%f%f",
								&g_aCurveHuman[nRoute].curvePoint[nCnt].x,			// �ʒu��ǂݍ���
								&g_aCurveHuman[nRoute].curvePoint[nCnt].y,
								&g_aCurveHuman[nRoute].curvePoint[nCnt].z);

							fscanf(pFile, "%s", &aString[0]);						// ROADPOS ��ǂݍ��� (�s�v)
							fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
							fscanf(pFile, "%s", &aString[0]);						// ������Ă��������ǂݍ���

							if (strcmp(&aString[0], "RIGHT") == 0)
							{ // �E�Ə����Ă������ꍇ
								// �Ȃ���p���E�ɐݒ肷��
								g_aCurveHuman[nRoute].roadPos[nCnt] = ROADPOS_RIGHT;
							}
							else if (strcmp(&aString[0], "LEFT") == 0)
							{ // ���Ə����Ă������ꍇ
								// �Ȃ���p��荶�ɐݒ肷��
								g_aCurveHuman[nRoute].roadPos[nCnt] = ROADPOS_LEFT;
							}
						}
					}
				} while (strcmp(&aString[0], "END_CURVE_INFO") != 0);	// �ǂݍ��񂾕����� END_CURVE_INFO �ł͂Ȃ��ꍇ���[�v

				// ���[�g�̐������Z����
				nRoute++;
			}
		} while (nEnd != EOF);											// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

	  // �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�J�[�u�t�@�C���̓ǂݍ��݂Ɏ��s�ł�", "���܂��݂�www", MB_ICONWARNING);
	}
}

//============================================================
// �Ȃ���p�̐ݒ菈��
//============================================================
void SetCurvePoint(void)
{
	for (int nCntCur = 0; nCntCur < MAX_HUMAN_ROUTE; nCntCur++)
	{ // ���[�g���ݒ肷��
		for (int nCnt = 0; nCnt < g_aCurveHuman[nCntCur].nCurveTime; nCnt++)
		{ // ���鎲�ƕ��������߂�

			if ((int)g_aCurveHuman[nCntCur].curvePoint[nCnt].x == (int)g_aCurveHuman[nCntCur].curvePoint[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime].x)
			{ // X���W��������������

				if ((int)g_aCurveHuman[nCntCur].curvePoint[nCnt].z < (int)g_aCurveHuman[nCntCur].curvePoint[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime].z)
				{ // �s���Z���W�����̃|�C���g�������������ꍇ

					// ���ɑ����Ă���
					g_aCurveHuman[nCntCur].dashAngle[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] = DASH_FAR;
				}
				else
				{ // �s���Z���W�����̃|�C���g������O�������ꍇ

					// ��O�ɑ����Ă���
					g_aCurveHuman[nCntCur].dashAngle[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] = DASH_NEAR;
				}
			}
			else
			{ // Z���W��������������
				if ((int)g_aCurveHuman[nCntCur].curvePoint[nCnt].x < (int)g_aCurveHuman[nCntCur].curvePoint[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime].x)
				{ // �s���X���W�����̃|�C���g�����E�������ꍇ

					// �E�ɑ����Ă���
					g_aCurveHuman[nCntCur].dashAngle[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] = DASH_RIGHT;
				}
				else
				{ // �s���X���W�����̃|�C���g�������������ꍇ

					// �������ɑ����Ă���
					g_aCurveHuman[nCntCur].dashAngle[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] = DASH_LEFT;
				}
			}
		}

		for (int nCnt = 0; nCnt < g_aCurveHuman[nCntCur].nCurveTime; nCnt++)
		{ // ����Ȃ�����������߂�

			if (g_aCurveHuman[nCntCur].dashAngle[nCnt] == DASH_RIGHT)
			{ // �E�ɑ����Ă���ꍇ
				if ((int)g_aCurveHuman[nCntCur].curvePoint[nCnt].z < (int)g_aCurveHuman[nCntCur].curvePoint[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime].z)
				{ // ���̖ړI�n�����������ꍇ

					// ���Ȃ���ɂȂ�
					g_aCurveHuman[nCntCur].curveAngle[nCnt] = CURVE_LEFT;
				}
				else
				{ // ���̖ړI�n����O�������ꍇ

					// �E�Ȃ���ɂȂ�
					g_aCurveHuman[nCntCur].curveAngle[nCnt] = CURVE_RIGHT;
				}
			}
			else if (g_aCurveHuman[nCntCur].dashAngle[nCnt] == DASH_LEFT)
			{ // ���ɑ����Ă���ꍇ

				if ((int)g_aCurveHuman[nCntCur].curvePoint[nCnt].z < (int)g_aCurveHuman[nCntCur].curvePoint[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime].z)
				{ // ���̖ړI�n�����������ꍇ

					// �E�Ȃ���ɂȂ�
					g_aCurveHuman[nCntCur].curveAngle[nCnt] = CURVE_RIGHT;
				}
				else
				{ // ���̖ړI�n����O�������ꍇ

					// ���Ȃ���ɂȂ�
					g_aCurveHuman[nCntCur].curveAngle[nCnt] = CURVE_LEFT;
				}
			}
			else if (g_aCurveHuman[nCntCur].dashAngle[nCnt] == DASH_FAR)
			{ // ���ɑ����Ă���ꍇ

				if ((int)g_aCurveHuman[nCntCur].curvePoint[nCnt].x < (int)g_aCurveHuman[nCntCur].curvePoint[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime].x)
				{ // ���̖ړI�n���E�������ꍇ

					// �E�Ȃ���ɂȂ�
					g_aCurveHuman[nCntCur].curveAngle[nCnt] = CURVE_RIGHT;
				}
				else
				{ // ���̖ړI�n�����������ꍇ

					// ���Ȃ���ɂȂ�
					g_aCurveHuman[nCntCur].curveAngle[nCnt] = CURVE_LEFT;
				}
			}
			else if (g_aCurveHuman[nCntCur].dashAngle[nCnt] == DASH_NEAR)
			{ // ��O�ɑ����Ă���ꍇ

				if ((int)g_aCurveHuman[nCntCur].curvePoint[nCnt].x < (int)g_aCurveHuman[nCntCur].curvePoint[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime].x)
				{ // ���̖ړI�n���E�������ꍇ

					// ���Ȃ���ɂȂ�
					g_aCurveHuman[nCntCur].curveAngle[nCnt] = CURVE_LEFT;
				}
				else
				{ // ���̖ړI�n�����������ꍇ

					// �E�Ȃ���ɂȂ�
					g_aCurveHuman[nCntCur].curveAngle[nCnt] = CURVE_RIGHT;
				}
			}
		}

		for (int nCnt = 0; nCnt < g_aCurveHuman[nCntCur].nCurveTime; nCnt++)
		{ // �Ȃ���񐔕���
			if (g_aCurveHuman[nCntCur].dashAngle[nCnt] == DASH_RIGHT)
			{ // �E�����Ɉړ����Ă����ꍇ

				if (nCnt == 0)
				{ // ���ڂ̏ꍇ
					// ������90�x�ɐݒ肷��
					g_aDefaultRot[nCntCur] = D3DXToRadian(90);
				}

				if (g_aCurveHuman[nCntCur].roadPos[nCnt] == ROADPOS_RIGHT)
				{ // �Ȃ���p���E����ʂ�ꍇ
					// �Ȃ���|�C���g��Z����75.0f�߂�
					g_aCurveHuman[nCntCur].curvePoint[nCnt].z -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
				}
				else if (g_aCurveHuman[nCntCur].roadPos[nCnt] == ROADPOS_LEFT)
				{ // �Ȃ���p��荶����ʂ�ꍇ
					// �Ȃ���|�C���g��Z����75.0f�߂�
					g_aCurveHuman[nCntCur].curvePoint[nCnt].z += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
				}

				switch (g_aCurveHuman[nCntCur].curveAngle[nCnt])
				{
				case CURVE_RIGHT:	// �E�ɋȂ���

					if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_RIGHT)
					{ // ���̒ʂ�Ƃ��낪�Ȃ���p���E��������
						// �Ȃ���|�C���g��X����75.0f���炷
						g_aCurveHuman[nCntCur].curvePoint[nCnt].x -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}
					else if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_LEFT)
					{ // ���̒ʂ�Ƃ��낪�Ȃ���p��荶��������
						// �Ȃ���|�C���g��X����75.0f���炷
						g_aCurveHuman[nCntCur].curvePoint[nCnt].x += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}

					break;			// �����o��

				case CURVE_LEFT:	// ���ɋȂ���

					if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_RIGHT)
					{ // ���̒ʂ�Ƃ��낪�Ȃ���p���E��������
						// �Ȃ���|�C���g��X����75.0f���炷
						g_aCurveHuman[nCntCur].curvePoint[nCnt].x += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}
					else if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_LEFT)
					{ // ���̒ʂ�Ƃ��낪�Ȃ���p��荶��������
						// �Ȃ���|�C���g��X����75.0f���炷
						g_aCurveHuman[nCntCur].curvePoint[nCnt].x -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}

					break;			// �����o��
				}
			}
			else if (g_aCurveHuman[nCntCur].dashAngle[nCnt] == DASH_LEFT)
			{ // �������Ɉړ����Ă����ꍇ

				if (nCnt == 0)
				{ // ���ڂ̏ꍇ
					// ������-90�x�ɐݒ肷��
					g_aDefaultRot[nCntCur] = D3DXToRadian(-90);
				}

				if (g_aCurveHuman[nCntCur].roadPos[nCnt] == ROADPOS_RIGHT)
				{ // �Ȃ���p���E����ʂ�ꍇ
					// �Ȃ���|�C���g��Z����75.0f�i�߂�
					g_aCurveHuman[nCntCur].curvePoint[nCnt].z += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
				}
				else if (g_aCurveHuman[nCntCur].roadPos[nCnt] == ROADPOS_LEFT)
				{ // �Ȃ���p��荶����ʂ�ꍇ
					// �Ȃ���|�C���g��Z����75.0f�i�߂�
					g_aCurveHuman[nCntCur].curvePoint[nCnt].z -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
				}

				switch (g_aCurveHuman[nCntCur].curveAngle[nCnt])
				{
				case CURVE_RIGHT:	// �E�ɋȂ���

					if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_RIGHT)
					{ // ���̒ʂ�Ƃ��낪�Ȃ���p���E��������
						// �Ȃ���|�C���g��X����75.0f�i�߂�
						g_aCurveHuman[nCntCur].curvePoint[nCnt].x += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}
					else if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_LEFT)
					{ // ���̒ʂ�Ƃ��낪�Ȃ���p��荶��������
						// �Ȃ���|�C���g��X����75.0f�i�߂�
						g_aCurveHuman[nCntCur].curvePoint[nCnt].x -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}

					break;			// �����o��

				case CURVE_LEFT:	// ���ɋȂ���

					if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_RIGHT)
					{ // ���̒ʂ�Ƃ��낪�Ȃ���p���E��������
						// �Ȃ���|�C���g��X����75.0f�i�߂�
						g_aCurveHuman[nCntCur].curvePoint[nCnt].x -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}
					else if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_LEFT)
					{ // ���̒ʂ�Ƃ��낪�Ȃ���p��荶��������
						// �Ȃ���|�C���g��X����75.0f�i�߂�
						g_aCurveHuman[nCntCur].curvePoint[nCnt].x += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}

					break;			// �����o��
				}
			}
			else if (g_aCurveHuman[nCntCur].dashAngle[nCnt] == DASH_FAR)
			{ // �������Ɉړ����Ă����ꍇ

				if (nCnt == 0)
				{ // ���ڂ̏ꍇ
					// ������0�x�ɐݒ肷��
					g_aDefaultRot[nCntCur] = D3DXToRadian(0);
				}

				if (g_aCurveHuman[nCntCur].roadPos[nCnt] == ROADPOS_RIGHT)
				{ // �Ȃ���p���E����ʂ�ꍇ
					// �Ȃ���|�C���g��X����i�߂�
					g_aCurveHuman[nCntCur].curvePoint[nCnt].x += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
				}
				else if (g_aCurveHuman[nCntCur].roadPos[nCnt] == ROADPOS_LEFT)
				{ // �Ȃ���p��荶����ʂ�ꍇ
					// �Ȃ���|�C���g��X����i�߂�
					g_aCurveHuman[nCntCur].curvePoint[nCnt].x -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
				}

				switch (g_aCurveHuman[nCntCur].curveAngle[nCnt])
				{
				case CURVE_RIGHT:	// �E�ɋȂ���

					if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_RIGHT)
					{ // ���̒ʂ�Ƃ��낪�Ȃ���p���E��������
						// �Ȃ���|�C���g��Z����75.0f�i�߂�
						g_aCurveHuman[nCntCur].curvePoint[nCnt].z -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}
					else if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_LEFT)
					{ // ���̒ʂ�Ƃ��낪�Ȃ���p��荶��������
						// �Ȃ���|�C���g��Z����75.0f�i�߂�
						g_aCurveHuman[nCntCur].curvePoint[nCnt].z += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}

					break;			// �����o��

				case CURVE_LEFT:	// ���ɋȂ���

					if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_RIGHT)
					{ // ���̒ʂ�Ƃ��낪�Ȃ���p���E��������
						// �Ȃ���|�C���g��Z����75.0f�i�߂�
						g_aCurveHuman[nCntCur].curvePoint[nCnt].z += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}
					else if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_LEFT)
					{ // ���̒ʂ�Ƃ��낪�Ȃ���p��荶��������
						// �Ȃ���|�C���g��Z����75.0f�i�߂�
						g_aCurveHuman[nCntCur].curvePoint[nCnt].z -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}

					break;			// �����o��
				}
			}
			else if(g_aCurveHuman[nCntCur].dashAngle[nCnt] == DASH_NEAR)
			{ // ��O�����Ɉړ����Ă����ꍇ

				if (g_aCurveHuman[nCntCur].roadPos[nCnt] == ROADPOS_RIGHT)
				{ // �Ȃ���p���E����ʂ�ꍇ
					// �Ȃ���|�C���g��X����75.0f�߂�
					g_aCurveHuman[nCntCur].curvePoint[nCnt].x -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
				}
				else if (g_aCurveHuman[nCntCur].roadPos[nCnt] == ROADPOS_LEFT)
				{ // �Ȃ���p��荶����ʂ�ꍇ
					// �Ȃ���|�C���g��X����75.0f�߂�
					g_aCurveHuman[nCntCur].curvePoint[nCnt].x += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
				}

				switch (g_aCurveHuman[nCntCur].curveAngle[nCnt])
				{
				case CURVE_RIGHT:		// �E�ɋȂ���

					if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_RIGHT)
					{ // ���̒ʂ�Ƃ��낪�Ȃ���p���E��������
						// �Ȃ���|�C���g��Z����75.0f�i�߂�
						g_aCurveHuman[nCntCur].curvePoint[nCnt].z += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}
					else if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_LEFT)
					{ // ���̒ʂ�Ƃ��낪�Ȃ���p��荶��������
						// �Ȃ���|�C���g��Z����75.0f�i�߂�
						g_aCurveHuman[nCntCur].curvePoint[nCnt].z -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}

					if (nCnt == 0)
					{ // ���ڂ̏ꍇ
						// ������-180�x�ɐݒ肷��
						g_aDefaultRot[nCntCur] = D3DXToRadian(-180);
					}

					break;				// �����o��

				case CURVE_LEFT:		// ���ɋȂ���

					if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_RIGHT)
					{ // ���̒ʂ�Ƃ��낪�Ȃ���p���E��������
						// �Ȃ���|�C���g��Z����75.0f�i�߂�
						g_aCurveHuman[nCntCur].curvePoint[nCnt].z -= (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}
					else if (g_aCurveHuman[nCntCur].roadPos[(nCnt + 1) % g_aCurveHuman[nCntCur].nCurveTime] == ROADPOS_LEFT)
					{ // ���̒ʂ�Ƃ��낪�Ȃ���p��荶��������
						// �Ȃ���|�C���g��Z����75.0f�i�߂�
						g_aCurveHuman[nCntCur].curvePoint[nCnt].z += (SHIFT_HUMAN_CURVE + (HUMAN_WIDTH * 2));
					}

					if (nCnt == 0)
					{ // ���ڂ̏ꍇ
						// ������180�x�ɐݒ肷��
						g_aDefaultRot[nCntCur] = D3DXToRadian(180);
					}

					break;				// �����o��
				}
			}
		}
	}
}

//============================================================
// �l�Ԃ̃��[�g�̎擾����
//============================================================
HUMANCURVEINFO GetHumanRoute(int nID)
{
	// �J�[�u�̏���Ԃ�
	return g_aCurveHuman[nID];
}

//============================================================
// �l�Ԃ̏��������̎擾����
//============================================================
float GetDefaultRot(int nID)
{
	// ����������Ԃ�
	return g_aDefaultRot[nID];
}