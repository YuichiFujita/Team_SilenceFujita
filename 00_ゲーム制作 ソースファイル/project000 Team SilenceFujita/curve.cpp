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
#define CURVE_TXT						"data\\TXT\\Curve.txt"		// �J�[�u�ݒ�p�̃e�L�X�g�t�@�C���̑��΃p�X
#define CURVE_SKIP_RANDOM				(2)					// �X�L�b�v����񐔂̃����_���͈̔�

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
		g_aCurveInfo[nCnt].bHumanDeadEnd = false;					// �l�Ԃ̍s���~�܂肩�ǂ���
	}

	/*g_aCurveInfo[0].pos = D3DXVECTOR3(-7000.0f, 0.0f, 7000.0f);
	g_aCurveInfo[0].nCurveNumber = 0;
	g_aCurveInfo[0].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[0].dashAngle = DASH_FAR;
	g_aCurveInfo[0].bDeadEnd = true;

	g_aCurveInfo[1].pos = D3DXVECTOR3(-6500.0f, 0.0f, 6500.0f);
	g_aCurveInfo[1].nCurveNumber = 1;
	g_aCurveInfo[1].curveAngle = CURVE_LEFT;
	g_aCurveInfo[1].dashAngle = DASH_LEFT;
	g_aCurveInfo[1].bDeadEnd = true;

	g_aCurveInfo[2].pos = D3DXVECTOR3(-4375.0f, 0.0f, 7000.0f);
	g_aCurveInfo[2].nCurveNumber = 2;
	g_aCurveInfo[2].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[2].dashAngle = DASH_RIGHT;
	g_aCurveInfo[2].bDeadEnd = false;

	g_aCurveInfo[3].pos = D3DXVECTOR3(-4875.0f, 0.0f, 6500.0f);
	g_aCurveInfo[3].nCurveNumber = 3;
	g_aCurveInfo[3].curveAngle = CURVE_LEFT;
	g_aCurveInfo[3].dashAngle = DASH_FAR;
	g_aCurveInfo[3].bDeadEnd = false;

	g_aCurveInfo[4].pos = D3DXVECTOR3(-4375.0f, 0.0f, 6500.0f);
	g_aCurveInfo[4].nCurveNumber = 4;
	g_aCurveInfo[4].curveAngle = CURVE_LEFT;
	g_aCurveInfo[4].dashAngle = DASH_LEFT;
	g_aCurveInfo[4].bDeadEnd = false;

	g_aCurveInfo[5].pos = D3DXVECTOR3(-4875.0f, 0.0f, 7000.0f);
	g_aCurveInfo[5].nCurveNumber = 5;
	g_aCurveInfo[5].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[5].dashAngle = DASH_FAR;
	g_aCurveInfo[5].bDeadEnd = true;

	g_aCurveInfo[6].pos = D3DXVECTOR3(-2250.0f, 0.0f, 7000.0f);
	g_aCurveInfo[6].nCurveNumber = 6;
	g_aCurveInfo[6].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[6].dashAngle = DASH_RIGHT;
	g_aCurveInfo[6].bDeadEnd = true;

	g_aCurveInfo[7].pos = D3DXVECTOR3(-2750.0f, 0.0f, 6500.0f);
	g_aCurveInfo[7].nCurveNumber = 7;
	g_aCurveInfo[7].curveAngle = CURVE_LEFT;
	g_aCurveInfo[7].dashAngle = DASH_FAR;
	g_aCurveInfo[7].bDeadEnd = true;

	g_aCurveInfo[8].pos = D3DXVECTOR3(-2750.0f, 0.0f, 4750.0f);
	g_aCurveInfo[8].nCurveNumber = 8;
	g_aCurveInfo[8].curveAngle = CURVE_LEFT;
	g_aCurveInfo[8].dashAngle = DASH_RIGHT;
	g_aCurveInfo[8].bDeadEnd = false;

	g_aCurveInfo[9].pos = D3DXVECTOR3(-2250.0f, 0.0f, 4250.0f);
	g_aCurveInfo[9].nCurveNumber = 9;
	g_aCurveInfo[9].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[9].dashAngle = DASH_NEAR;
	g_aCurveInfo[9].bDeadEnd = false;

	g_aCurveInfo[10].pos = D3DXVECTOR3(-2750.0f, 0.0f, 4250.0f);
	g_aCurveInfo[10].nCurveNumber = 10;
	g_aCurveInfo[10].curveAngle = CURVE_LEFT;
	g_aCurveInfo[10].dashAngle = DASH_FAR;
	g_aCurveInfo[10].bDeadEnd = false;

	g_aCurveInfo[11].pos = D3DXVECTOR3(-2250.0f, 0.0f, 4750.0f);
	g_aCurveInfo[11].nCurveNumber = 11;
	g_aCurveInfo[11].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[11].dashAngle = DASH_RIGHT;
	g_aCurveInfo[11].bDeadEnd = true;

	g_aCurveInfo[12].pos = D3DXVECTOR3(-4375.0f, 0.0f, 4250.0f);
	g_aCurveInfo[12].nCurveNumber = 12;
	g_aCurveInfo[12].curveAngle = CURVE_LEFT;
	g_aCurveInfo[12].dashAngle = DASH_LEFT;
	g_aCurveInfo[12].bDeadEnd = false;

	g_aCurveInfo[13].pos = D3DXVECTOR3(-4875.0f, 0.0f, 4750.0f);
	g_aCurveInfo[13].nCurveNumber = 13;
	g_aCurveInfo[13].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[13].dashAngle = DASH_FAR;
	g_aCurveInfo[13].bDeadEnd = false;

	g_aCurveInfo[14].pos = D3DXVECTOR3(-4375.0f, 0.0f, 4750.0f);
	g_aCurveInfo[14].nCurveNumber = 14;
	g_aCurveInfo[14].curveAngle = CURVE_LEFT;
	g_aCurveInfo[14].dashAngle = DASH_NEAR;
	g_aCurveInfo[14].bDeadEnd = false;

	g_aCurveInfo[15].pos = D3DXVECTOR3(-4875.0f, 0.0f, 4250.0f);
	g_aCurveInfo[15].nCurveNumber = 15;
	g_aCurveInfo[15].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[15].dashAngle = DASH_LEFT;
	g_aCurveInfo[15].bDeadEnd = true;

	g_aCurveInfo[16].pos = D3DXVECTOR3(-4875.0f, 0.0f, 2500.0f);
	g_aCurveInfo[16].nCurveNumber = 16;
	g_aCurveInfo[16].curveAngle = CURVE_LEFT;
	g_aCurveInfo[16].dashAngle = DASH_RIGHT;
	g_aCurveInfo[16].bDeadEnd = false;

	g_aCurveInfo[17].pos = D3DXVECTOR3(-4375.0f, 0.0f, 2000.0f);
	g_aCurveInfo[17].nCurveNumber = 17;
	g_aCurveInfo[17].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[17].dashAngle = DASH_NEAR;
	g_aCurveInfo[17].bDeadEnd = true;

	g_aCurveInfo[18].pos = D3DXVECTOR3(-4875.0f, 0.0f, 2000.0f);
	g_aCurveInfo[18].nCurveNumber = 18;
	g_aCurveInfo[18].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[18].dashAngle = DASH_LEFT;
	g_aCurveInfo[18].bDeadEnd = false;

	g_aCurveInfo[19].pos = D3DXVECTOR3(-4375.0f, 0.0f, 2500.0f);
	g_aCurveInfo[19].nCurveNumber = 19;
	g_aCurveInfo[19].curveAngle = CURVE_LEFT;
	g_aCurveInfo[19].dashAngle = DASH_NEAR;
	g_aCurveInfo[19].bDeadEnd = false;

	g_aCurveInfo[20].pos = D3DXVECTOR3(-2750.0f, 0.0f, 2500.0f);
	g_aCurveInfo[20].nCurveNumber = 20;
	g_aCurveInfo[20].curveAngle = CURVE_LEFT;
	g_aCurveInfo[20].dashAngle = DASH_RIGHT;
	g_aCurveInfo[20].bDeadEnd = true;

	g_aCurveInfo[21].pos = D3DXVECTOR3(-2250.0f, 0.0f, 2000.0f);
	g_aCurveInfo[21].nCurveNumber = 21;
	g_aCurveInfo[21].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[21].dashAngle = DASH_NEAR;
	g_aCurveInfo[21].bDeadEnd = true;

	g_aCurveInfo[22].pos = D3DXVECTOR3(-6500.0f, 0.0f, 2500.0f);
	g_aCurveInfo[22].nCurveNumber = 22;
	g_aCurveInfo[22].curveAngle = CURVE_LEFT;
	g_aCurveInfo[22].dashAngle = DASH_NEAR;
	g_aCurveInfo[22].bDeadEnd = true;

	g_aCurveInfo[23].pos = D3DXVECTOR3(-7000.0f, 0.0f, 2000.0f);
	g_aCurveInfo[23].nCurveNumber = 23;
	g_aCurveInfo[23].curveAngle = CURVE_RIGHT;
	g_aCurveInfo[23].dashAngle = DASH_LEFT;
	g_aCurveInfo[23].bDeadEnd = true;*/
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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + 1;
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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + 1;
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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + 1;
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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + 1;
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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + 1;
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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + 1;
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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + 1;
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
				pCurve->nSKipCnt = rand() % CURVE_SKIP_RANDOM + 1;
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