//======================================================================================================================
//
//	�Ԃ̏��� [Car.cpp]
//	Author�F��������
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "input.h"
#include "model.h"
#include "calculation.h"

#include "bonus.h"
#include "Car.h"
#include "Combo.h"
#include "shadow.h"
#include "sound.h"
#include "player.h"
#include "Police.h"
#include "object.h"
#include "particle.h"
#include "meshfield.h"

#ifdef _DEBUG	// �f�o�b�O����
#include "game.h"
#endif

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define CAR_LIFE				(50)		// �I�u�W�F�N�g�̗̑�
#define CAR_GRAVITY				(0.75f)		// �v���C���[�ɂ�����d��
#define CAR_MOVE_FORWARD		(0.1f)		// �v���C���[�O�i���̈ړ���
#define CAR_MOVE_BACKWARD		(0.2f)		// �v���C���[��ގ��̈ړ���
#define CAR_MOVE_ROT			(0.012f)	// �v���C���[�̌����ύX��
#define REV_CAR_MOVE_ROT		(0.1f)		// �ړ��ʂɂ������ύX�ʂ̕␳�W��
#define SUB_CAR_MOVE_VALUE		(15.0f)		// �����ύX���̌������s����ړ���
#define SUB_CAR_MOVE			(0.05f)		// �����ύX���̌�����
#define MAX_CAR_FORWARD			(30.0f)		// �O�i���̍ō����x
#define MAX_CAR_FORWARD_PATROL  (15.0f)		// �p�g���[�����̑O�i���̍ō����x
#define MAX_CAR_BACKWARD		(8.0f)		// ��ގ��̍ō����x
#define REV_CAR_MOVE_SUB		(0.04f)		// �ړ��ʂ̌����W��
#define CAR_NOTMOVE_SLOW		(0.04f)		// �Ԃ̈ړ����Ă��Ȃ��Ƃ��̌����W��
#define CAR_STOP_RADIUS_DIST	(300.0f)	// �Ԃ̎~�܂�w�W�̉~�̂��炷����
#define CAR_STOP_ADD_RADIUS		(50.0f)		// �Ԃ̎~�܂锼�a�̒ǉ���

#define CAR_TRAFFIC_CNT			(400)		// �a�؂��N�����Ƃ��ɉ��P����p�̃J�E���g
#define CAR_TRAFFIC_IMPROVE_CNT	(540)		// �a�؏�Ԃ̉����̃J�E���g
#define CAR_TRAFFIC_ALPHA		(0.5f)		// �a�؎��̓����x

#define CAR_BOMB_BONUS_CNT		(180)		// �{����Ԃ̎��{�[�i�X������J�E���g��

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void PosCar(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove);		// �Ԃ̈ʒu�̍X�V����
void RevCar(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos);	// �Ԃ̕␳�̍X�V����
void CurveCar(Car *pCar);							// �Ԃ̃J�[�u����
void DashCarAction(Car *pCar);						// �Ԃ̑��s����
void SetCarPosRot(Car *pCar);						// �Ԃ̈ʒu�ƌ����̐ݒ菈��
void CarPosRotCorrect(Car *pCar);					// �Ԃ̈ʒu�̕␳����
void CarBodyStopPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 rot, D3DXVECTOR3 *pMove, float fWidth, float fDepth, COLLOBJECTTYPE collObject, int *pTraCnt, TACKLESTATE state);		// �v���C���[�Ƃ̓����蔻��
void CarBodyStopCar(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 rot, D3DXVECTOR3 *pMove, float fWidth, float fDepth, COLLOBJECTTYPE collObject, int *pTraCnt);			// �ԂƂ̓����蔻��
void CarBodyStopPolice(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 rot, D3DXVECTOR3 *pMove, float fWidth, float fDepth, COLLOBJECTTYPE collObject, int *pTraCnt);		// �x�@�Ƃ̓����蔻��

void CarTrafficImprove(Car *pCar);					// �Ԃ̏a�؉��P����

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
Car g_aCar[MAX_CAR];		// �Ԃ̏��

//======================================================================================================================
//	�Ԃ̏���������
//======================================================================================================================
void InitCar(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�ւ̃|�C���^

	// �Ԃ̏��̏�����
	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		g_aCar[nCntCar].pos         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aCar[nCntCar].posOld      = g_aCar[nCntCar].pos;				// �O��̈ʒu
		g_aCar[nCntCar].move        = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
		g_aCar[nCntCar].rot         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aCar[nCntCar].nShadowID   = NONE_SHADOW;						// �e�̃C���f�b�N�X
		g_aCar[nCntCar].type		= CARTYPE_CAR001;					// ���
		g_aCar[nCntCar].bombState   = BOMBSTATE_NONE;					// �{���̏��
		g_aCar[nCntCar].typeMove	= MOVETYPE_MOVE;					// �ړ��̃^�C�v
		g_aCar[nCntCar].nBombCount  = 0;								// �{�����̃J�E���g
		g_aCar[nCntCar].bJump       = false;							// �W�����v���Ă��邩�ǂ���
		g_aCar[nCntCar].bMove       = false;							// �ړ����Ă��邩
		g_aCar[nCntCar].nTrafficCnt = 0;								// �a�؃J�E���g
		g_aCar[nCntCar].state		= CARSTATE_NORMAL;					// ���
		g_aCar[nCntCar].bUse		= false;							// �g�p��

		// �Ȃ���p�̏��̏�����
		g_aCar[nCntCar].carCurveInfo.curveInfo.curveAngle   = CURVE_LEFT;	// ���ɋȂ���
		g_aCar[nCntCar].carCurveInfo.curveInfo.nCurveNumber = 0;			// �J�[�u����Ԓn
		g_aCar[nCntCar].carCurveInfo.nSKipCnt    = 0;						// �X�L�b�v����Ȃ���p�̉�
		g_aCar[nCntCar].carCurveInfo.rotDest     = g_aCar[nCntCar].rot;		// �O��̌���
		g_aCar[nCntCar].carCurveInfo.actionState = CARACT_DASH;				// ���݂̎Ԃ̍s��

		//�W���b�W�̏��̏�����
		g_aCar[nCntCar].judge.state = JUDGESTATE_JUSTICE;			// �P��
		g_aCar[nCntCar].judge.ticatica = CHICASTATE_BLACKOUT;		// �`�J�`�J���
		g_aCar[nCntCar].judge.col = JUDGE_WHITE;					// �s�J�s�J�̐F

		// ���f�����̏�����
		g_aCar[nCntCar].modelData.dwNumMat = 0;						// �}�e���A���̐�
		g_aCar[nCntCar].modelData.pTexture = NULL;					// �e�N�X�`���ւ̃|�C���^
		g_aCar[nCntCar].modelData.pMesh    = NULL;					// ���b�V�� (���_���) �ւ̃|�C���^
		g_aCar[nCntCar].modelData.pBuffMat = NULL;					// �}�e���A���ւ̃|�C���^
		g_aCar[nCntCar].modelData.vtxMin   = INIT_VTX_MIN;			// �ŏ��̒��_���W
		g_aCar[nCntCar].modelData.vtxMax   = INIT_VTX_MAX;			// �ő�̒��_���W
		g_aCar[nCntCar].modelData.size     = INIT_SIZE;				// �傫��
		g_aCar[nCntCar].modelData.fRadius  = 0.0f;					// ���a

		// �A�C�R���̏��̏�����
		g_aCar[nCntCar].icon.nIconID = NONE_ICON;					// �A�C�R���̃C���f�b�N�X
		g_aCar[nCntCar].icon.state = ICONSTATE_NONE;				// �A�C�R���̏��
	}
}

//======================================================================================================================
//	�Ԃ̏I������
//======================================================================================================================
void UninitCar(void)
{

}

//======================================================================================================================
//	�Ԃ̍X�V����
//======================================================================================================================
void UpdateCar(void)
{
	int nTackleCnt = 0;			// �����ݒ�p
	float fTackleMove = 0.0f;	// �����ݒ�p
	POLICESTATE policeState = POLICESTATE_CHASE;	// �x�@�̏��(�I�u�W�F�N�g�Ƃ̓����蔻��Ɏg�����ߖ��Ӗ�)

	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�
		if (g_aCar[nCntCar].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ
			if (g_aCar[nCntCar].bombState != BOMBSTATE_BAR_IN)
			{ // �o���A����Ԃł͂Ȃ��ꍇ
				// �O��ʒu�̍X�V
				g_aCar[nCntCar].posOld = g_aCar[nCntCar].pos;

				//----------------------------------------------------
				//	�e�̍X�V
				//----------------------------------------------------
				// �e�̈ʒu�ݒ�
				SetPositionShadow
				( // ����
					g_aCar[nCntCar].nShadowID,	// �e�̃C���f�b�N�X
					g_aCar[nCntCar].pos,		// �ʒu
					g_aCar[nCntCar].rot,		// ����
					NONE_SCALE					// �g�嗦
				);

				// �v���C���[�̈ʒu�̍X�V
				PosCar(&g_aCar[nCntCar].move, &g_aCar[nCntCar].pos, &g_aCar[nCntCar].rot, g_aCar[nCntCar].bMove);

				if (g_aCar[nCntCar].typeMove == MOVETYPE_MOVE)
				{ // �ړ���Ԃ̏ꍇ

					//�Ԃ̃J�[�u����
					CurveCar(&g_aCar[nCntCar]);
				}

				if (g_aCar[nCntCar].state == CARSTATE_TRAFFIC)
				{ // �a�؏�Ԃ������ꍇ
					// �Ԃ̏a�؉��P����
					CarTrafficImprove(&g_aCar[nCntCar]);
				}

				if (g_aCar[nCntCar].pos.y < 0.0f)
				{//Y���̈ʒu��0.0f�������ꍇ
					//�c�ւ̈ړ��ʂ�0.0f�ɂ���
					g_aCar[nCntCar].move.y = 0.0f;

					//�ʒu��0.0f�ɖ߂�
					g_aCar[nCntCar].pos.y = 0.0f;
				}
			}

			if (GetBarrierState(&g_aCar[nCntCar]) == BARRIERSTATE_NONE	 ||
				GetBarrierState(&g_aCar[nCntCar]) == BARRIERSTATE_FLY	 ||
				GetBarrierState(&g_aCar[nCntCar]) == BARRIERSTATE_HOMING ||
				GetBarrierState(&g_aCar[nCntCar]) == BARRIERSTATE_LAND)
			{ // �o���A���W�J����ĂȂ������ꍇ

				// �{�����̃J�E���g������������
				g_aCar[nCntCar].nBombCount = 0;

				if (g_aCar[nCntCar].state != CARSTATE_TRAFFIC)
				{ // �a�؏�Ԃ���Ȃ��ꍇ
					//----------------------------------------------------
					//	�����蔻��
					//----------------------------------------------------
					// �I�u�W�F�N�g�Ƃ̓����蔻��
					CollisionObject
					( // ����
						&g_aCar[nCntCar].pos,			// ���݂̈ʒu
						&g_aCar[nCntCar].posOld,		// �O��̈ʒu
						&g_aCar[nCntCar].move,			// �ړ���
						CAR_WIDTH,						// ����
						CAR_DEPTH,						// ���s
						&g_aCar[nCntCar].nTrafficCnt,	// �a�؃J�E���g
						BOOSTSTATE_NONE,				// �u�[�X�g���
						&policeState,					// �x�@�̏��
						&nTackleCnt,					// �^�b�N���J�E���g
						&fTackleMove					// �^�b�N�����̈ړ���
					);

					// �Ԃ̒�~����
					CollisionStopCar
					( // ����
						g_aCar[nCntCar].pos,				// �ʒu
						g_aCar[nCntCar].rot,				// ����
						&g_aCar[nCntCar].move,				// �ړ���
						g_aCar[nCntCar].modelData.fRadius,	// ���a
						COLLOBJECTTYPE_CAR,					// �Ώۂ̃T�C�Y
						&g_aCar[nCntCar].nTrafficCnt		// �a�؃J�E���g
					);

					if (g_aCar[nCntCar].nTrafficCnt >= CAR_TRAFFIC_CNT)
					{ // �a�؂Ɋ������܂ꂽ�ꍇ
						// �a�؏�Ԃɂ���
						g_aCar[nCntCar].state = CARSTATE_TRAFFIC;
					}
				}

				// �ԓ��m�̓����蔻��
				CollisionCarBody
				( // ����
					&g_aCar[nCntCar].pos,				// ���݂̈ʒu
					&g_aCar[nCntCar].posOld,			// �O��̈ʒu
					g_aCar[nCntCar].rot,				// ����
					&g_aCar[nCntCar].move,				// �ړ���
					CAR_WIDTH,							// ����
					CAR_DEPTH,							// ���s
					COLLOBJECTTYPE_CAR,					// �Ώ�
					&g_aCar[nCntCar].nTrafficCnt,		// �a�؃J�E���g
					TACKLESTATE_CHARGE					// ���
				);
			}
			else
			{ // �o���A�Z�b�g��Ԃ̏ꍇ

				if (g_aCar[nCntCar].judge.state == JUDGESTATE_EVIL)
				{ // �����z�̏ꍇ

					// �{�����̃J�E���g�����Z����
					g_aCar[nCntCar].nBombCount++;

					if (g_aCar[nCntCar].nBombCount % CAR_BOMB_BONUS_CNT == 0)
					{ // �{���J�E���g����萔�ɂȂ����ꍇ

						// �{�[�i�X�̐ݒ菈��
						SetBonus(ADDSCORE_CAR);
					}
				}
			}

			if (g_aCar[nCntCar].judge.state == JUDGESTATE_EVIL)
			{ // ���҂������ꍇ

				// �W���b�W�̍X�V����
				UpdateJudge(&g_aCar[nCntCar].judge);

				// �A�C�R���̈ʒu�ݒ菈��
				SetPositionIcon(g_aCar[nCntCar].icon.nIconID, g_aCar[nCntCar].pos);
			}

			if (g_aCar[nCntCar].bombState != BOMBSTATE_BAR_IN)
			{ // �o���A����Ԃł͂Ȃ��ꍇ

				// �Ԃ̕␳�̍X�V����
				RevCar(&g_aCar[nCntCar].rot, &g_aCar[nCntCar].pos);
			}
		}
	}
}

//======================================================================================================================
//	�Ԃ̕`�揈��
//======================================================================================================================
void DrawCar(void)
{
	// �ϐ���錾
	float        carRot;						// �Ԃ̌����̌v�Z�p
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	Player           *pPlayer = GetPlayer();	// �v���C���[�̏��
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL      bombMat;					// �}�e���A���f�[�^ (�{���p)

	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aCar[nCntCar].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aCar[nCntCar].mtxWorld);

			// �Ԃ̌�����ݒ�
			carRot = g_aCar[nCntCar].rot.y + D3DX_PI;
			RotNormalize(&carRot);	// �����𐳋K��

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, carRot, g_aCar[nCntCar].rot.x, g_aCar[nCntCar].rot.z);
			D3DXMatrixMultiply(&g_aCar[nCntCar].mtxWorld, &g_aCar[nCntCar].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aCar[nCntCar].pos.x, g_aCar[nCntCar].pos.y, g_aCar[nCntCar].pos.z);
			D3DXMatrixMultiply(&g_aCar[nCntCar].mtxWorld, &g_aCar[nCntCar].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aCar[nCntCar].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aCar[nCntCar].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aCar[nCntCar].modelData.dwNumMat; nCntMat++)
			{ // �}�e���A���̐����J��Ԃ�

				if (pPlayer->bomb.state == ATTACKSTATE_BOMB)
				{ // �U����Ԃ��{���U����Ԃ̏ꍇ

					// �\���̗̂v�f���N���A
					ZeroMemory(&bombMat, sizeof(D3DXMATERIAL));

					switch (g_aCar[nCntCar].bombState)
					{ // �{���̏��
					case BOMBSTATE_RANGE:	// �͈͓����

						// �͈͓����̃}�e���A���̐F��ݒ�
						bombMat.MatD3D.Diffuse = BOMB_RANGE_COL;

						// �����𔲂���
						break;

					case BOMBSTATE_AIM:		// �_�����

						// �_�����̃}�e���A���̐F��ݒ�
						bombMat.MatD3D.Diffuse = BOMB_AIM_COL;

						// �����𔲂���
						break;

					default:				// ��L�ȊO

						// �͈͊O���̃}�e���A���̐F��ݒ�
						bombMat.MatD3D.Diffuse = BOMB_NONE_COL;

						// �����𔲂���
						break;
					}

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&bombMat.MatD3D);
				}
				else if (g_aCar[nCntCar].state == CARSTATE_TRAFFIC)
				{ // �a�؏�Ԃ̏ꍇ

					// �Ԃ𔖂�����
					g_aCar[nCntCar].MatCopy[nCntMat].MatD3D.Diffuse.a = CAR_TRAFFIC_ALPHA;

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&g_aCar[nCntCar].MatCopy[nCntMat].MatD3D);

				}
				else
				{ // �U����Ԃ�����ȊO�̏�Ԃ̏ꍇ

					// �}�e���A���̃R�s�[�ɑ������
					g_aCar[nCntCar].MatCopy[nCntMat] = pMat[nCntMat];

					if (g_aCar[nCntCar].judge.state == JUDGESTATE_JUSTICE)
					{ // �ǂ��z�̏ꍇ
						// �}�e���A���̐ݒ�
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
					}
					else
					{ // �����z�̏ꍇ

						// ���Ȕ�����������
						g_aCar[nCntCar].MatCopy[nCntMat].MatD3D.Emissive = g_aCar[nCntCar].judge.col;

						// �}�e���A���̐ݒ�
						pDevice->SetMaterial(&g_aCar[nCntCar].MatCopy[nCntMat].MatD3D);
					}
				}

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aCar[nCntCar].modelData.pTexture[nCntMat]);

				// ���f���̕`��
				g_aCar[nCntCar].modelData.pMesh->DrawSubset(nCntMat);
			}

			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	�Ԃ̐ݒ菈��
//======================================================================================================================
void SetCar(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nWalk, int nType)
{
	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aCar[nCntCar].bUse == false)
		{ // �I�u�W�F�N�g���g�p����Ă��Ȃ��ꍇ
			// ��������
			g_aCar[nCntCar].pos			= pos;								// ���݂̈ʒu
			g_aCar[nCntCar].posOld		= g_aCar[nCntCar].pos;				// �O��̈ʒu				
			g_aCar[nCntCar].typeMove	= (MOVETYPE)nWalk;					// �ړ��̃^�C�v
			g_aCar[nCntCar].rot			= rot;								// ����
			g_aCar[nCntCar].type		= (CARTYPE)(nType);					// �Ԃ̎��

			// ���̐ݒ�
			g_aCar[nCntCar].move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
			g_aCar[nCntCar].bombState	= BOMBSTATE_NONE;					// �������Ă��Ȃ���Ԃɂ���
			g_aCar[nCntCar].nBombCount	= 0;								// �{�����̃J�E���g
			g_aCar[nCntCar].bJump		= false;							// �W�����v���Ă��邩�ǂ���
			g_aCar[nCntCar].nTrafficCnt = 0;								// �a�؃J�E���g
			g_aCar[nCntCar].state		= CARSTATE_NORMAL;					// ���
			g_aCar[nCntCar].bMove		= false;							// �ړ����Ă��Ȃ�

			// �g�p���Ă����Ԃɂ���
			g_aCar[nCntCar].bUse = true;

			// ���f������ݒ�
			g_aCar[nCntCar].modelData = GetModelData(g_aCar[nCntCar].type + FROM_CAR);	// ���f�����

			D3DXMATERIAL *pMat;					//�}�e���A���ւ̃|�C���^

			//�}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aCar[nCntCar].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aCar[nCntCar].modelData.dwNumMat; nCntMat++)
			{
				//�}�e���A���̏����擾����
				g_aCar[nCntCar].MatCopy[nCntMat] = pMat[nCntMat];
			}

			// �e�̃C���f�b�N�X��ݒ�
			g_aCar[nCntCar].nShadowID = SetModelShadow
			( // ����
				g_aCar[nCntCar].modelData,	// ���f�����
				&g_aCar[nCntCar].nShadowID,	// �e�̐e�̉e�C���f�b�N�X
				&g_aCar[nCntCar].bUse		// �e�̐e�̎g�p��
			);

			// �e�̈ʒu�ݒ�
			SetPositionShadow(g_aCar[nCntCar].nShadowID, g_aCar[nCntCar].pos, g_aCar[nCntCar].rot, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

			// �A�C�R���̏��̏�����
			g_aCar[nCntCar].icon.nIconID = NONE_ICON;					// �A�C�R���̃C���f�b�N�X
			g_aCar[nCntCar].icon.state = ICONSTATE_NONE;				// �A�C�R���̏��

			if (g_aCar[nCntCar].typeMove == MOVETYPE_MOVE)
			{ // �ړ���Ԃ̏ꍇ

				// �Ԃ̈ʒu�ƌ����̐ݒ菈��
				SetCarPosRot(&g_aCar[nCntCar]);
				g_aCar[nCntCar].carCurveInfo.nSKipCnt = 0;						// �X�L�b�v����Ȃ���p�̉�
				g_aCar[nCntCar].carCurveInfo.rotDest = g_aCar[nCntCar].rot;		// �O��̌���
				g_aCar[nCntCar].carCurveInfo.actionState = CARACT_DASH;			// �����Ă�����
			}

			// �W���b�W�̏��̐ݒ�
			g_aCar[nCntCar].judge.col = JUDGE_WHITE;			// �s�J�s�J�̐F

			if (g_aCar[nCntCar].type == CARTYPE_ELECTIONCAR)
			{ // ������������ꍇ

				g_aCar[nCntCar].judge.state = JUDGESTATE_EVIL;					// �P��
				g_aCar[nCntCar].judge.ticatica = CHICASTATE_BLACKOUT;			// �`�J�`�J���
			}
			else
			{ // �ǂ��z�������ꍇ

				g_aCar[nCntCar].judge.state = JUDGESTATE_JUSTICE;				// �P��
				g_aCar[nCntCar].judge.ticatica = CHICASTATE_BLACKOUT;			// �`�J�`�J���
			}

			if (g_aCar[nCntCar].judge.state == JUDGESTATE_EVIL)
			{ // �����z�̏ꍇ

				// �A�C�R���̃C���f�b�N�X��ݒ�
				g_aCar[nCntCar].icon.nIconID = SetIcon
				( // ����
					g_aCar[nCntCar].pos,
					ICONTYPE_EVIL,
					&g_aCar[nCntCar].icon.nIconID,
					&g_aCar[nCntCar].bUse,
					&g_aCar[nCntCar].icon.state
				);
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
void HitCar(Car *pCar, int nDamage)
{
	if (pCar->state == ACTIONSTATE_NORMAL)
	{ // �I�u�W�F�N�g���ʏ��Ԃ̏ꍇ

	  // �����̃_���[�W����̗͂��猸�Z
		pCar->nLife -= nDamage;

		if (pCar->nLife > 0)
		{ // �̗͂��c���Ă���ꍇ

		  // �_���[�W��Ԃɂ���
			pCar->state = ACTIONSTATE_DAMAGE;

			// �p�[�e�B�N���̐ݒ�
			SetParticle
			( // ����
				pCar->pos,						// �ʒu
				D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f),	// �F
				PARTICLETYPE_DAMAGE,				// ���
				SPAWN_PARTICLE_DAMAGE,				// �G�t�F�N�g��
				2									// ����
			);

			// �J�E���^�[��ݒ�
			pCar->nCounterState = DAMAGE_TIME_OBJ;

			// �T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_DMG);			// SE (�_���[�W)
		}
		else
		{ // �̗͂��s�����ꍇ

		  // �����̐ݒ�
			SetExplosion(pCar->pos, SOUNDTYPE_BREAK);

			// �p�[�e�B�N���̐ݒ�
			SetParticle
			( // ����
				pCar->pos,						// �ʒu
				D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f),	// �F
				PARTICLETYPE_EXPLOSION,				// ���
				SPAWN_PARTICLE_EXPLOSION,			// �G�t�F�N�g��
				2									// ����
			);

			// �A�C�e���̐ݒ�
			SetItem(pCar->pos, ITEMTYPE_HEAL);

			// �g�p���Ă��Ȃ���Ԃɂ���
			pCar->bUse = false;
		}
	}
}
#endif

//======================================================================================================================
//	�I�u�W�F�N�g�̎擾����
//======================================================================================================================
Car *GetCarData(void)
{
	// �I�u�W�F�N�g�̏��̐擪�A�h���X��Ԃ�
	return &g_aCar[0];
}

//============================================================
//	�v���C���[�̈ʒu�̍X�V����
//============================================================
void PosCar(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove)
{
	//--------------------------------------------------------
	//	�d�͂̍X�V
	//--------------------------------------------------------
	move->y -= CAR_GRAVITY;

	//--------------------------------------------------------
	//	�ړ��ʂ̕␳
	//--------------------------------------------------------
	if (move->x > MAX_CAR_FORWARD)
	{ // �v���C���[�̈ړ��� (x) �����l�ȏ�̏ꍇ

	  // �v���C���[�̈ړ��� (x) ��␳
		move->x = MAX_CAR_FORWARD;
	}
	else if (move->x < -MAX_CAR_BACKWARD)
	{ // �v���C���[�̈ړ��� (x) �����l�ȉ��̏ꍇ

	  // �v���C���[�̈ړ��� (x) ��␳
		move->x = -MAX_CAR_BACKWARD;
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
		move->x += (0.0f - move->x) * CAR_NOTMOVE_SLOW;
	}
}

//============================================================
//	�v���C���[�̕␳�̍X�V����
//============================================================
void RevCar(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos)
{
	// �����̐��K��
	RotNormalize(&rot->y);

	//--------------------------------------------------------
	//	�ړ��͈͂̕␳
	//--------------------------------------------------------
	if (pos->z > GetLimitStage().fNear - (CAR_DEPTH * 2))
	{ // �͈͊O�̏ꍇ (��O)

		// ��O�Ɉʒu��␳
		pos->z = GetLimitStage().fNear - (CAR_DEPTH * 2);
	}
	if (pos->z < GetLimitStage().fFar + (CAR_DEPTH * 2))
	{ // �͈͊O�̏ꍇ (��)

		// ���Ɉʒu��␳
		pos->z = GetLimitStage().fFar + (CAR_DEPTH * 2);
	}
	if (pos->x > GetLimitStage().fRight - (CAR_WIDTH * 2))
	{ // �͈͊O�̏ꍇ (�E)

		// �E�Ɉʒu��␳
		pos->x = GetLimitStage().fRight - (CAR_WIDTH * 2);
	}
	if (pos->x < GetLimitStage().fLeft + (CAR_WIDTH * 2))
	{ // �͈͊O�̏ꍇ (��)

		// ���Ɉʒu��␳
		pos->x = GetLimitStage().fLeft + (CAR_WIDTH * 2);
	}
}

//============================================================
// �Ԃ̋Ȃ��鏈��
//============================================================
void CurveCar(Car *pCar)
{
	switch (pCar->carCurveInfo.actionState)
	{
	case CARACT_DASH:		// ���s���

		// �Ԃ̑��s����
		DashCarAction(pCar);

		break;				// �����o��

	case CARACT_CURVE:		// �J�[�u���

		// �ړ����Ă����Ԃɂ���
		pCar->bMove = false;

		// �Ԃ̊p�x�X�V�E�␳����
		CurveInfoRotCar(&pCar->carCurveInfo, &pCar->rot, &pCar->move, &pCar->pos);

		break;				// �����o��
	}

	if (pCar->move.x > MAX_CAR_FORWARD_PATROL)
	{ // �v���C���[�̈ړ��� (x) �����l�ȏ�̏ꍇ

	  // �v���C���[�̈ړ��� (x) ��␳
		pCar->move.x = MAX_CAR_FORWARD_PATROL;
	}
}

//============================================================
// �Ԃ̑��s����
//============================================================
void DashCarAction(Car *pCar)
{
	// �ړ��ʂ��X�V
	pCar->move.x += CAR_MOVE_FORWARD;

	// �ړ����Ă����Ԃɂ���
	pCar->bMove = true;

	for (int nCnt = 0; nCnt < MAX_CURVEPOINT; nCnt++)
	{
		switch (pCar->carCurveInfo.curveInfo.dashAngle)
		{
		case DASH_RIGHT:		//�E�ɑ����Ă���ꍇ

			//���킹��
			pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->pos.z == GetCurveInfo(nCnt).pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
			{ // ����Z����𑖂��Ă���ꍇ
				if (pCar->pos.x >= GetCurveInfo(nCnt).pos.x - (CAR_WIDTH * 2) &&
					pCar->posOld.x <= GetCurveInfo(nCnt).pos.x - (CAR_WIDTH * 2))
				{ // �ʒu����v�����ꍇ
					if (GetCurveInfo(nCnt).dashAngle == DASH_RIGHT)
					{ // �E�ɑ���ꍇ�̂�
						// �X�L�b�v�J�E���g�����Z����
						pCar->carCurveInfo.nSKipCnt--;

						if (pCar->carCurveInfo.nSKipCnt == 0 || GetCurveInfo(nCnt).bDeadEnd == true)
						{ // �X�L�b�v�񐔂�0�ɂȂ����܂��́A�s���~�܂肾�����ꍇ
							// �X�L�b�v�񐔂�0�ɂ���
							pCar->carCurveInfo.nSKipCnt = 0;

							// �Ȃ���p�̏����X�V����
							pCar->carCurveInfo.curveInfo = GetCurveInfo(nCnt);
						}
					}
				}
			}

			break;				//�����o��

		case DASH_LEFT:			//���𑖂��Ă���ꍇ

			//���킹��
			pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->pos.z == GetCurveInfo(nCnt).pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
			{ // ����Z����𑖂��Ă���ꍇ
				if (pCar->pos.x <= GetCurveInfo(nCnt).pos.x + (CAR_WIDTH * 2) &&
					pCar->posOld.x >= GetCurveInfo(nCnt).pos.x + (CAR_WIDTH * 2))
				{ // �ʒu����v�����ꍇ
					if (GetCurveInfo(nCnt).dashAngle == DASH_LEFT)
					{ // ���ɑ���ꍇ�̂�
						// �X�L�b�v�J�E���g�����Z����
						pCar->carCurveInfo.nSKipCnt--;

						if (pCar->carCurveInfo.nSKipCnt == 0 || GetCurveInfo(nCnt).bDeadEnd == true)
						{ // �X�L�b�v�񐔂�0�ɂȂ����ꍇ
							// �X�L�b�v�񐔂�0�ɂ���
							pCar->carCurveInfo.nSKipCnt = 0;

							// �Ȃ���p�̏����X�V����
							pCar->carCurveInfo.curveInfo = GetCurveInfo(nCnt);
						}
					}
				}
			}

			break;				//�����o��

		case DASH_FAR:			//���ɑ����Ă���ꍇ

			//���킹��
			pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->pos.x == GetCurveInfo(nCnt).pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
			{ // ����Z����𑖂��Ă���ꍇ
				if (pCar->pos.z >= GetCurveInfo(nCnt).pos.z - (CAR_WIDTH * 2) &&
					pCar->posOld.z <= GetCurveInfo(nCnt).pos.z - (CAR_WIDTH * 2))
				{ // �ʒu����v�����ꍇ
					if (GetCurveInfo(nCnt).dashAngle == DASH_FAR)
					{ // ���ɑ���ꍇ�̂�
						// �X�L�b�v�J�E���g�����Z����
						pCar->carCurveInfo.nSKipCnt--;

						if (pCar->carCurveInfo.nSKipCnt == 0 || GetCurveInfo(nCnt).bDeadEnd == true)
						{ // �X�L�b�v�񐔂�0�ɂȂ����ꍇ
							// �X�L�b�v�񐔂�0�ɂ���
							pCar->carCurveInfo.nSKipCnt = 0;

							// �Ȃ���p�̏����X�V����
							pCar->carCurveInfo.curveInfo = GetCurveInfo(nCnt);
						}
					}
				}
			}

			break;				//�����o��

		case DASH_NEAR:			//���ɑ����Ă���ꍇ

			//���킹��
			pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->pos.x == GetCurveInfo(nCnt).pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
			{ // ����Z����𑖂��Ă���ꍇ
				if (pCar->pos.z <= GetCurveInfo(nCnt).pos.z + (CAR_WIDTH * 2) &&
					pCar->posOld.z >= GetCurveInfo(nCnt).pos.z + (CAR_WIDTH * 2))
				{ // �ʒu����v�����ꍇ
					if (GetCurveInfo(nCnt).dashAngle == DASH_NEAR)
					{ // ��O�ɑ���ꍇ�̂�
						// �X�L�b�v�J�E���g�����Z����
						pCar->carCurveInfo.nSKipCnt--;

						if (pCar->carCurveInfo.nSKipCnt == 0 || GetCurveInfo(nCnt).bDeadEnd == true)
						{ // �X�L�b�v�񐔂�0�ɂȂ����ꍇ
							// �X�L�b�v�񐔂�0�ɂ���
							pCar->carCurveInfo.nSKipCnt = 0;

							// �Ȃ���p�̏����X�V����
							pCar->carCurveInfo.curveInfo = GetCurveInfo(nCnt);

							if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_LEFT)
							{ // �Ȃ���������������������ꍇ
							  // �p�x��␳����
								pCar->rot.y = D3DX_PI;
							}
							else
							{ // �Ȃ���������E�����������ꍇ
							  // �p�x��␳����
								pCar->rot.y = -D3DX_PI;
							}
						}
					}
				}
			}

			break;				//�����o��
		}
	}

	if (pCar->carCurveInfo.nSKipCnt == 0)
	{ // �X�L�b�v�J�E���g��0�̏ꍇ
		switch (pCar->carCurveInfo.curveInfo.dashAngle)
		{
		case DASH_RIGHT:			//�E�ɑ����Ă���

			//���킹��
			pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_RIGHT)
			{ // �E�ɋȂ���ꍇ
				if (pCar->pos.x >= pCar->carCurveInfo.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
					// �ʒu��␳����
					pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}
			else if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_LEFT)
			{ // ���ɋȂ���ꍇ
				if (pCar->pos.x >= pCar->carCurveInfo.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
					// �ʒu��␳����
					pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}

			break;					//�����o��

		case DASH_LEFT:				//���ɑ����Ă���

			//���킹��
			pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_RIGHT)
			{ // �E�ɋȂ���ꍇ
				if (pCar->pos.x <= pCar->carCurveInfo.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
					// �ʒu��␳����
					pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}
			else if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_LEFT)
			{ // ���ɋȂ���ꍇ
				if (pCar->pos.x <= pCar->carCurveInfo.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
					// �ʒu��␳����
					pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}

			break;					//�����o��

		case DASH_FAR:				//���ɑ����Ă���

			//���킹��
			pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_RIGHT)
			{ // �E�ɋȂ���ꍇ
				if (pCar->pos.z >= pCar->carCurveInfo.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
					// �ʒu��␳����
					pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}
			else if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_LEFT)
			{ // ���ɋȂ���ꍇ
				if (pCar->pos.z >= pCar->carCurveInfo.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
					// �ʒu��␳����
					pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}

			break;					//�����o��

		case DASH_NEAR:				//��O�ɑ����Ă���

			//���킹��
			pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_RIGHT)
			{ // �E�ɋȂ���ꍇ
				if (pCar->pos.z <= pCar->carCurveInfo.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
					// �ʒu��␳����
					pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}
			else if (pCar->carCurveInfo.curveInfo.curveAngle == CURVE_LEFT)
			{ // ���ɋȂ���ꍇ
				if (pCar->pos.z <= pCar->carCurveInfo.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
					// �ʒu��␳����
					pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pCar->carCurveInfo.actionState = CARACT_CURVE;
				}
			}

			break;					//�����o��
		}
	}
}

//============================================================
// �Ԃ̒�~����
//============================================================
void CollisionStopCar(D3DXVECTOR3 targetpos, D3DXVECTOR3 targetrot, D3DXVECTOR3 *move, float fTargetRadius, COLLOBJECTTYPE collObject, int *pTraCnt)
{
	D3DXVECTOR3 stopCarpos =
		D3DXVECTOR3(
			targetpos.x + sinf(targetrot.y) * CAR_STOP_RADIUS_DIST,
			0.0f,
			targetpos.z + cosf(targetrot.y) * CAR_STOP_RADIUS_DIST
		);				// �~�܂�Ԃ̈ʒu

	float fLength;										// �����̕ϐ�

	{// �x�@�Ƃ̓����蔻��
		Police *pPolice = GetPoliceData();					// �x�@�̏����擾����

		for (int nCntPoli = 0; nCntPoli < MAX_POLICE; nCntPoli++, pPolice++)
		{
			if (pPolice->bUse == true && GetBarrierState(pPolice) != BARRIERSTATE_SET)
			{ // �g�p���Ă���ꍇ
				// �����𑪂�
				fLength = (pPolice->pos.x - stopCarpos.x) * (pPolice->pos.x - stopCarpos.x)
					+ (pPolice->pos.z - stopCarpos.z) * (pPolice->pos.z - stopCarpos.z);

				if (fLength <= (pPolice->modelData.fRadius + CAR_STOP_ADD_RADIUS) * (fTargetRadius + CAR_STOP_ADD_RADIUS))
				{ // �I�u�W�F�N�g���������Ă���
					switch (collObject)
					{
					case COLLOBJECTTYPE_PLAYER:		//�v���C���[�̏ꍇ

						break;						//�����o��

					case COLLOBJECTTYPE_POLICE:		//�Ԃ̏ꍇ

						// �ڕW�̈ړ��ʂ��Z�[�u����
						move->x += (0.0f - move->x) * 0.5f;

						// �J�E���g�����Z����
						*pTraCnt += 1;

						break;						//�����o��

					case COLLOBJECTTYPE_CAR:		//�Ԃ̏ꍇ

						// �ڕW�̈ړ��ʂ��Z�[�u����
						move->x = 0.0f;

						// �J�E���g�����Z����
						*pTraCnt += 1;

						break;						//�����o��
					}
				}
			}
		}
	}

	{//�v���C���[�Ƃ̓����蔻��
		Player *pPlayer = GetPlayer();				//�v���C���[�̏����擾����

		if (pPlayer->bUse == true)
		{ // �g�p���Ă���ꍇ
		  // �����𑪂�
			fLength = (pPlayer->pos.x - stopCarpos.x) * (pPlayer->pos.x - stopCarpos.x)
				+ (pPlayer->pos.z - stopCarpos.z) * (pPlayer->pos.z - stopCarpos.z);

			if (fLength <= (pPlayer->modelData.fRadius + CAR_STOP_ADD_RADIUS) * (fTargetRadius + CAR_STOP_ADD_RADIUS))
			{ // �I�u�W�F�N�g���������Ă���

				switch (collObject)
				{
				case COLLOBJECTTYPE_PLAYER:		//�v���C���[�̏ꍇ

					break;						//�����o��

				case COLLOBJECTTYPE_POLICE:		//�Ԃ̏ꍇ

					// �ڕW�̈ړ��ʂ��Z�[�u����
					move->x = move->x;

					break;						//�����o��

				case COLLOBJECTTYPE_CAR:		//�Ԃ̏ꍇ

					// �ڕW�̈ړ��ʂ��Z�[�u����
					move->x = sinf(targetrot.y) * -8.0f;

					break;						//�����o��
				}
			}
		}
	}

	{//�ԂƂ̓����蔻��
		Car *pCar = GetCarData();				//�Ԃ̏����擾����

		for (int nCntPoli = 0; nCntPoli < MAX_CAR; nCntPoli++, pCar++)
		{
			if (pCar->bUse == true && GetBarrierState(pCar) != BARRIERSTATE_SET)
			{ // �g�p���Ă���ꍇ
				// �����𑪂�
				fLength = (pCar->pos.x - stopCarpos.x) * (pCar->pos.x - stopCarpos.x)
					+ (pCar->pos.z - stopCarpos.z) * (pCar->pos.z - stopCarpos.z);

				if (fLength <= (pCar->modelData.fRadius + CAR_STOP_ADD_RADIUS) * (fTargetRadius + CAR_STOP_ADD_RADIUS))
				{ // �I�u�W�F�N�g���������Ă���
					switch (collObject)
					{
					case COLLOBJECTTYPE_PLAYER:		//�v���C���[�̏ꍇ

						break;						//�����o��

					case COLLOBJECTTYPE_POLICE:		//�Ԃ̏ꍇ

						// �ڕW�̈ړ��ʂ��Z�[�u����
						move->x += (0.0f - move->x) * 0.5f;

						// �J�E���g�����Z����
						*pTraCnt += 1;

						break;						//�����o��

					case COLLOBJECTTYPE_CAR:		//�Ԃ̏ꍇ

						// �ڕW�̈ړ��ʂ��Z�[�u����
						move->x = 0.0f;

						// �J�E���g�����Z����
						*pTraCnt += 1;

						break;						//�����o��
					}
				}
			}
		}
	}
}

//============================================================
// �ԓ��m�̓����蔻��
//============================================================
void CollisionCarBody(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 rot, D3DXVECTOR3 *pMove, float fWidth, float fDepth, COLLOBJECTTYPE collObject, int *pTraCnt, TACKLESTATE state)
{
	switch (collObject)
	{
	case COLLOBJECTTYPE_PLAYER:		// �v���C���[

		break;						// �����o��

	case COLLOBJECTTYPE_CAR:		// ��

		if (state != CARSTATE_TRAFFIC)
		{ // �a�؏�Ԃ���Ȃ������ꍇ

			// �v���C���[�Ƃ̓����蔻��
			CarBodyStopPlayer(pPos, pPosOld, rot, pMove, fWidth, fDepth, collObject, pTraCnt, state);

			// �ԂƂ̓����蔻��
			CarBodyStopCar(pPos, pPosOld, rot, pMove, fWidth, fDepth, collObject, pTraCnt);

			// �x�@�Ƃ̓����蔻��
			CarBodyStopPolice(pPos, pPosOld, rot, pMove, fWidth, fDepth, collObject, pTraCnt);
		}

		break;						// �����o��

	case COLLOBJECTTYPE_POLICE:		// �x�@

		// �v���C���[�Ƃ̓����蔻��
		CarBodyStopPlayer(pPos, pPosOld, rot, pMove, fWidth, fDepth, collObject, pTraCnt, state);

		if (state != POLICESTATE_TRAFFIC)
		{ // �a�؏�Ԃ���Ȃ������ꍇ

			// �ԂƂ̓����蔻��
			CarBodyStopCar(pPos, pPosOld, rot, pMove, fWidth, fDepth, collObject, pTraCnt);

			// �x�@�Ƃ̓����蔻��
			CarBodyStopPolice(pPos, pPosOld, rot, pMove, fWidth, fDepth, collObject, pTraCnt);
		}

		break;						// �����o��
	}
}

//============================================================
// �Ԃ̈ʒu�ƌ����̐ݒ菈��
//============================================================
void SetCarPosRot(Car *pCar)
{
	float fCurveDist;			// �ŋ߂̋Ȃ���p�Ƃ̋���
	int nCurveNumber = 0;		// �ŋ߂̋Ȃ���p�̔ԍ�

	fCurveDist = fabsf(sqrtf((pCar->pos.x - GetCurveInfo(0).pos.x) * (pCar->pos.x - GetCurveInfo(0).pos.x) +
		(pCar->pos.z - GetCurveInfo(0).pos.z) * (pCar->pos.z - GetCurveInfo(0).pos.z)));

	for (int nCnt = 1; nCnt < MAX_CURVEPOINT; nCnt++)
	{ // �S�Ă̋Ȃ���p�ƎQ��

		float fCurvePoint;		// �Ȃ���p�̒l

		// �J�[�u�̋���
		fCurvePoint = fabsf(sqrtf((pCar->pos.x - GetCurveInfo(nCnt).pos.x) * (pCar->pos.x - GetCurveInfo(nCnt).pos.x) +
			(pCar->pos.z - GetCurveInfo(nCnt).pos.z) * (pCar->pos.z - GetCurveInfo(nCnt).pos.z)));

		if (fCurvePoint <= fCurveDist)
		{ // �����̋߂����X�V���ꂽ�ꍇ
			// �ŋߒl���X�V����
			fCurveDist = fCurvePoint;

			// �ԍ����X�V����
			nCurveNumber = nCnt;
		}
	}

	// �Ȃ���p�̏���������
	pCar->carCurveInfo.curveInfo = GetCurveInfo(nCurveNumber);

	// �Ԃ̈ʒu�̕␳����
	CarPosRotCorrect(pCar);
}

//============================================================
// �Ԃ̈ʒu�̕␳����
//============================================================
void CarPosRotCorrect(Car *pCar)
{
	switch (pCar->carCurveInfo.curveInfo.dashAngle)
	{
	case DASH_RIGHT:		// �E�Ɍ������đ���

		//���킹��
		pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

		// ������ς���
		pCar->rot.y = D3DX_PI * 0.5f;

		break;				// �����o��

	case DASH_LEFT:			// ���Ɍ������đ���

		//���킹��
		pCar->pos.z = pCar->carCurveInfo.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

		// ������ς���
		pCar->rot.y = -D3DX_PI * 0.5f;

		break;				// �����o��

	case DASH_FAR:			// ���Ɍ������đ���

		//���킹��
		pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

		// ������ς���
		pCar->rot.y = 0.0f;

		break;				// �����o��

	case DASH_NEAR:			// ��O�Ɍ������đ���

		//���킹��
		pCar->pos.x = pCar->carCurveInfo.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

		switch (pCar->carCurveInfo.curveInfo.curveAngle)
		{
		case CURVE_RIGHT:	// �E�ɋȂ���

			// ������ς���
			pCar->rot.y = -D3DX_PI;

			break;			// �����o��

		case CURVE_LEFT:	// ���ɋȂ���

			// ������ς���
			pCar->rot.y = D3DX_PI;

			break;			// �����o��
		}

		break;				// �����o��
	}
}

//============================================================
// �v���C���[�Ƃ̓����蔻��
//============================================================
void CarBodyStopPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 rot, D3DXVECTOR3 *pMove, float fWidth, float fDepth, COLLOBJECTTYPE collObject, int *pTraCnt, TACKLESTATE state)
{
	Player *pPlayer = GetPlayer();				// �Ԃ̏����擾����

	if (pPlayer->bUse == true)
	{ // �v���C���[���g�p����Ă����ꍇ
		if (pPos->x - fWidth <= pPlayer->pos.x + PLAY_WIDTH
			&& pPos->x + fWidth >= pPlayer->pos.x - PLAY_WIDTH)
		{ // �Ԃ�X���̒��ɂ����ꍇ
			if (pPosOld->z + fDepth <= pPlayer->oldPos.z - PLAY_DEPTH
				&& pPos->z + fDepth >= pPlayer->pos.z - PLAY_DEPTH)
			{//�O��̈ʒu���u���b�N����O���A���݂̈ʒu���u���b�N���������������ꍇ(��O�Ŏ~�߂��鏈��)
				switch (collObject)
				{
				case COLLOBJECTTYPE_POLICE:		// �x�@�̏ꍇ

					if (pPlayer->state != ACTIONSTATE_DAMAGE && pPlayer->state != ACTIONSTATE_UNRIVALED)
					{ // �_���[�W��Ԃł͂Ȃ����A���G��Ԃł͂Ȃ��ꍇ

						// �ʒu�����炷
						//pPlayer->pos.z = pPos->z + (PLAY_DEPTH + fDepth);

						// �v���C���[�̈ړ��ʂ��폜
						CollisionSlow(&pPlayer->move.x);

						// �ړ��ʂ��폜
						CollisionSlow(&pMove->x);

						if (state == TACKLESTATE_BOOST)
						{ // �^�b�N����
							// �v���C���[�̃_���[�W����
							HitPlayer
							(
								pPlayer,		// �v���C���[�̃A�h���X
								PLAY_DAMAGE		// �v���C���[�̃_���[�W��
							);
						}
					}

					break;						// �����o��

				case COLLOBJECTTYPE_CAR:		// �Ԃ̏ꍇ

					//�ʒu�����炷
					pPlayer->pos.z = pPos->z + (PLAY_DEPTH + fDepth);

					//�v���C���[�̈ړ��ʂ��폜
					CollisionSlow(&pPlayer->move.x);

					// �ړ��ʂ��폜
					pMove->x = 0.0f;

					// �a�؃J�E���g�����Z����
					*pTraCnt += 1;

					break;						// �����o��
				}

				if (pPlayer->move.x >= 13.0f)
				{ // �ړ��ʂ����ȏ�̏ꍇ
					// �p�[�e�B�N���̐ݒ菈��
					SetParticle
					(
						D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + 50.0f, pPlayer->pos.z - PLAY_DEPTH),	// �ʒu
						D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),					// �F
						PARTICLETYPE_SPARK,									// �p�[�e�B�N���̎��
						10,													// ������
						2													// ����
					);
				}
			}							//��O�Ŏ~�߂��鏈��
			else if (pPosOld->z - fDepth >= pPlayer->oldPos.z + PLAY_DEPTH
				&& pPos->z - fDepth <= pPlayer->pos.z + PLAY_DEPTH)
			{//�O��̈ʒu���v���C���[�̈ʒu���������A���݂̈ʒu���v���C���[�̈ʒu������O�������ꍇ(���Ŏ~�߂��鏈��)
				switch (collObject)
				{
				case COLLOBJECTTYPE_POLICE:		// �x�@�̏ꍇ

					if (pPlayer->state != ACTIONSTATE_DAMAGE && pPlayer->state != ACTIONSTATE_UNRIVALED)
					{ // �_���[�W��Ԃł͂Ȃ����A���G��Ԃł͂Ȃ��ꍇ

						// �ʒu�����炷
						//pPlayer->pos.z = pPos->z - (PLAY_DEPTH + fDepth);

						// �v���C���[�̈ړ��ʂ��폜
						CollisionSlow(&pPlayer->move.x);

						// �ړ��ʂ��폜
						CollisionSlow(&pMove->x);

						if (state == TACKLESTATE_BOOST)
						{ // �^�b�N����
							// �v���C���[�̃_���[�W����
							HitPlayer
							(
								pPlayer,		// �v���C���[�̃A�h���X
								PLAY_DAMAGE		// �v���C���[�̃_���[�W��
							);
						}
					}

					break;						// �����o��

				case COLLOBJECTTYPE_CAR:		// �Ԃ̏ꍇ

					//�ʒu�����炷
					pPlayer->pos.z = pPos->z - (PLAY_DEPTH + fDepth);

					//�v���C���[�̈ړ��ʂ��폜
					CollisionSlow(&pPlayer->move.x);

					// �ړ��ʂ��폜
					pMove->x = 0.0f;

					// �a�؃J�E���g�����Z����
					*pTraCnt += 1;

					break;						// �����o��
				}

				if (pPlayer->move.x >= 13.0f)
				{ // �ړ��ʂ����ȏ�̏ꍇ
					// �p�[�e�B�N���̐ݒ菈��
					SetParticle
					(
						D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + 50.0f, pPlayer->pos.z + PLAY_DEPTH),	// �ʒu
						D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),					// �F
						PARTICLETYPE_SPARK,									// �p�[�e�B�N���̎��
						10,													// ������
						2													// ����
					);
				}
			}							//���Ŏ~�߂��鏈��
		}

		if (pPos->z - fDepth <= pPlayer->pos.z + PLAY_DEPTH
			&& pPos->z + fDepth >= pPlayer->pos.z - PLAY_DEPTH)
		{//����Z���̒��ɂ����ꍇ
			if (pPosOld->x + fWidth <= pPlayer->oldPos.x - PLAY_WIDTH
				&& pPos->x + fWidth >= pPlayer->pos.x - PLAY_WIDTH)
			{//�O��̈ʒu���u���b�N�̍��[��荶���A���݂̈ʒu���u���b�N�̍������E�������ꍇ(���̏���)
				switch (collObject)
				{
				case COLLOBJECTTYPE_POLICE:		// �x�@�̏ꍇ

					if (pPlayer->state != ACTIONSTATE_DAMAGE && pPlayer->state != ACTIONSTATE_UNRIVALED)
					{ // �_���[�W��Ԃł͂Ȃ����A���G��Ԃł͂Ȃ��ꍇ

						// �ʒu�����炷
						//pPlayer->pos.x = pPos->x + (PLAY_WIDTH + fWidth);

						// �v���C���[�̈ړ��ʂ��폜
						CollisionSlow(&pPlayer->move.x);

						// �ړ��ʂ��폜
						CollisionSlow(&pMove->x);

						if (state == TACKLESTATE_BOOST)
						{ // �^�b�N����
							// �v���C���[�̃_���[�W����
							HitPlayer
							(
								pPlayer,		// �v���C���[�̃A�h���X
								PLAY_DAMAGE		// �v���C���[�̃_���[�W��
							);
						}
					}

					break;						// �����o��

				case COLLOBJECTTYPE_CAR:		// �Ԃ̏ꍇ

					//�ʒu�����炷
					pPlayer->pos.x = pPos->x + (PLAY_WIDTH + fWidth);

					//�v���C���[�̈ړ��ʂ��폜
					CollisionSlow(&pPlayer->move.x);

					// �ړ��ʂ��폜
					pMove->x = 0.0f;

					// �a�؃J�E���g�����Z����
					*pTraCnt += 1;

					break;						// �����o��
				}

				if (pPlayer->move.x >= 13.0f)
				{ // �ړ��ʂ����ȏ�̏ꍇ
					// �p�[�e�B�N���̐ݒ菈��
					SetParticle
					(
						D3DXVECTOR3(pPlayer->pos.x - PLAY_WIDTH, pPlayer->pos.y + 50.0f, pPlayer->pos.z),	// �ʒu
						D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),					// �F
						PARTICLETYPE_SPARK,									// �p�[�e�B�N���̎��
						10,													// ������
						2													// ����
					);
				}
			}							//���[�̏���
			else if (pPosOld->x - fWidth >= pPlayer->oldPos.x + PLAY_WIDTH
				&& pPos->x - fWidth <= pPlayer->pos.x + PLAY_WIDTH)
			{//�O��̈ʒu���u���b�N�̉E�[���E���A���݂̈ʒu���u���b�N�̍������E�������ꍇ(�E�̏���)
				switch (collObject)
				{
				case COLLOBJECTTYPE_POLICE:		// �x�@�̏ꍇ

					if (pPlayer->state != ACTIONSTATE_DAMAGE && pPlayer->state != ACTIONSTATE_UNRIVALED)
					{ // �_���[�W��Ԃł͂Ȃ����A���G��Ԃł͂Ȃ��ꍇ

						// �ʒu�����炷
						//pPlayer->pos.x = pPos->x - (PLAY_WIDTH + fWidth);

						// �v���C���[�̈ړ��ʂ��폜
						CollisionSlow(&pPlayer->move.x);

						// �ړ��ʂ��폜
						CollisionSlow(&pMove->x);

						if (state == TACKLESTATE_BOOST)
						{ // �^�b�N����
							// �v���C���[�̃_���[�W����
							HitPlayer
							(
								pPlayer,		// �v���C���[�̃A�h���X
								PLAY_DAMAGE		// �v���C���[�̃_���[�W��
							);
						}
					}

					break;						// �����o��

				case COLLOBJECTTYPE_CAR:		// �Ԃ̏ꍇ

					//�ʒu�����炷
					pPlayer->pos.x = pPos->x - (PLAY_WIDTH + fWidth);

					//�v���C���[�̈ړ��ʂ��폜
					CollisionSlow(&pPlayer->move.x);

					// �ړ��ʂ��폜
					pMove->x = 0.0f;

					// �a�؃J�E���g�����Z����
					*pTraCnt += 1;

					break;						// �����o��
				}

				if (pPlayer->move.x >= 13.0f)
				{ // �ړ��ʂ����ȏ�̏ꍇ
					// �p�[�e�B�N���̐ݒ菈��
					SetParticle
					(
						D3DXVECTOR3(pPlayer->pos.x + PLAY_WIDTH, pPlayer->pos.y + 50.0f, pPlayer->pos.z),	// �ʒu
						D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),					// �F
						PARTICLETYPE_SPARK,									// �p�[�e�B�N���̎��
						10,													// ������
						2													// ����
					);
				}
			}							//�E�[�̏���
		}
	}
}

//============================================================
// �ԂƂ̓����蔻��
//============================================================
void CarBodyStopCar(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 rot, D3DXVECTOR3 *pMove, float fWidth, float fDepth, COLLOBJECTTYPE collObject, int *pTraCnt)
{
	Car *pCar = GetCarData();				// �Ԃ̏����擾����

	for (int nCnt = 0; nCnt < MAX_CAR; nCnt++, pCar++)
	{
		if (pCar->bUse == true && pCar->state != CARSTATE_TRAFFIC && GetBarrierState(pCar) == BARRIERSTATE_SET)
		{ // �Ԃ��g�p����Ă��邩�A�a�؏�ԈȊO�̏ꍇ
			if (pPos->x - fWidth <= pCar->pos.x + CAR_WIDTH
				&& pPos->x + fWidth >= pCar->pos.x - CAR_WIDTH)
			{ // �Ԃ�X���̒��ɂ����ꍇ
				if (pPosOld->z + fDepth < pCar->posOld.z - CAR_DEPTH
					&& pPos->z + fDepth > pCar->pos.z - CAR_DEPTH)
				{//�O��̈ʒu���u���b�N����O���A���݂̈ʒu���u���b�N���������������ꍇ(��O�Ŏ~�߂��鏈��)
					switch (collObject)
					{
					case COLLOBJECTTYPE_POLICE:		// �x�@�̏ꍇ

						//�ʒu�����炷
						pCar->pos.z = pPos->z + (CAR_DEPTH + fDepth);

						// �ړ��ʂ��폜
						CollisionSlow(&pMove->x);

						// �J�E���g�����Z����
						*pTraCnt += 1;

						break;						// �����o��

					case COLLOBJECTTYPE_CAR:		// �Ԃ̏ꍇ

						//�ʒu�����炷
						pCar->pos.z = pPos->z + (CAR_DEPTH + fDepth);

						// �ړ��ʂ��폜
						CollisionSlow(&pMove->x);

						// �J�E���g�����Z����
						*pTraCnt += 1;

						break;						// �����o��
					}
				}							//��O�Ŏ~�߂��鏈��
				else if (pPosOld->z - fDepth >= pCar->posOld.z + CAR_DEPTH
					&& pPos->z - fDepth <= pCar->pos.z + CAR_DEPTH)
				{//�O��̈ʒu���v���C���[�̈ʒu���������A���݂̈ʒu���v���C���[�̈ʒu������O�������ꍇ(���Ŏ~�߂��鏈��)
					switch (collObject)
					{
					case COLLOBJECTTYPE_POLICE:		// �x�@�̏ꍇ

						//�ʒu�����炷
						pCar->pos.z = pPos->z - (CAR_DEPTH + fDepth);

						// �ړ��ʂ��폜
						CollisionSlow(&pMove->x);

						// �J�E���g�����Z����
						*pTraCnt += 1;

						break;						// �����o��

					case COLLOBJECTTYPE_CAR:		// �Ԃ̏ꍇ

						//�ʒu�����炷
						pCar->pos.z = pPos->z - (CAR_DEPTH + fDepth);

						// �ړ��ʂ��폜
						CollisionSlow(&pMove->x);

						// �J�E���g�����Z����
						*pTraCnt += 1;

						break;						// �����o��
					}
				}							//���Ŏ~�߂��鏈��
			}

			if (pPos->z - fDepth <= pCar->pos.z + CAR_DEPTH
				&& pPos->z + fDepth >= pCar->pos.z - CAR_DEPTH)
			{//����Z���̒��ɂ����ꍇ
				if (pPosOld->x + fWidth <= pCar->posOld.x - CAR_WIDTH
					&& pPos->x + fWidth >= pCar->pos.x - CAR_WIDTH)
				{//�O��̈ʒu���u���b�N�̍��[��荶���A���݂̈ʒu���u���b�N�̍������E�������ꍇ(���̏���)
					switch (collObject)
					{
					case COLLOBJECTTYPE_POLICE:		// �x�@�̏ꍇ

						//�ʒu�����炷
						pCar->pos.x = pPos->x + (CAR_WIDTH + fWidth);

						// �ړ��ʂ��폜
						CollisionSlow(&pMove->x);

						// �J�E���g�����Z����
						*pTraCnt += 1;

						break;						// �����o��

					case COLLOBJECTTYPE_CAR:		// �Ԃ̏ꍇ

						//�ʒu�����炷
						pCar->pos.x = pPos->x + (CAR_WIDTH + fWidth);

						// �ړ��ʂ��폜
						CollisionSlow(&pMove->x);

						// �J�E���g�����Z����
						*pTraCnt += 1;

						break;						// �����o��
					}
				}							//���[�̏���
				else if (pPosOld->x - fWidth >= pCar->posOld.x + CAR_WIDTH
					&& pPos->x - fWidth <= pCar->pos.x + CAR_WIDTH)
				{//�O��̈ʒu���u���b�N�̉E�[���E���A���݂̈ʒu���u���b�N�̍������E�������ꍇ(�E�̏���)
					switch (collObject)
					{
					case COLLOBJECTTYPE_POLICE:		// �x�@�̏ꍇ

						//�ʒu�����炷
						pCar->pos.x = pPos->x - (CAR_WIDTH + fWidth);

						// �ړ��ʂ��폜
						CollisionSlow(&pMove->x);

						// �J�E���g�����Z����
						*pTraCnt += 1;

						break;						// �����o��

					case COLLOBJECTTYPE_CAR:		// �Ԃ̏ꍇ

						//�ʒu�����炷
						pCar->pos.x = pPos->x - (CAR_WIDTH + fWidth);

						// �ړ��ʂ��폜
						CollisionSlow(&pMove->x);

						// �J�E���g�����Z����
						*pTraCnt += 1;

						break;						// �����o��
					}
				}							//�E�[�̏���
			}
		}
	}
}

//============================================================
// �x�@�Ƃ̓����蔻��
//============================================================
void CarBodyStopPolice(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 rot, D3DXVECTOR3 *pMove, float fWidth, float fDepth, COLLOBJECTTYPE collObject, int *pTraCnt)
{
	Police *pPolice = GetPoliceData();			// �x�@�̏����擾����

	for (int nCnt = 0; nCnt < MAX_POLICE; nCnt++, pPolice++)
	{
		if (pPolice->bUse == true && pPolice->state != POLICESTATE_TRAFFIC)
		{ // �Ԃ��g�p����Ă��邩�A�a�؏�Ԃł͂Ȃ��ꍇ
			if (pPos->x - fWidth <= pPolice->pos.x + POLICAR_WIDTH
				&& pPos->x + fWidth >= pPolice->pos.x - POLICAR_WIDTH)
			{ // �Ԃ�X���̒��ɂ����ꍇ
				if (pPosOld->z + fDepth < pPolice->posOld.z - POLICAR_DEPTH
					&& pPos->z + fDepth > pPolice->pos.z - POLICAR_DEPTH)
				{//�O��̈ʒu���u���b�N����O���A���݂̈ʒu���u���b�N���������������ꍇ(��O�Ŏ~�߂��鏈��)
					switch (collObject)
					{
					case COLLOBJECTTYPE_POLICE:		// �x�@�̏ꍇ

						//�ʒu�����炷
						pPolice->pos.z = pPos->z + (POLICAR_DEPTH + fDepth);

						// �ړ��ʂ��폜
						CollisionSlow(&pMove->x);

						// �J�E���g�����Z����
						*pTraCnt += 1;

						break;						// �����o��

					case COLLOBJECTTYPE_CAR:		// �Ԃ̏ꍇ

						//�ʒu�����炷
						pPolice->pos.z = pPos->z + (POLICAR_DEPTH + fDepth);

						// �ړ��ʂ��폜
						CollisionSlow(&pMove->x);

						// �J�E���g�����Z����
						*pTraCnt += 1;

						break;						// �����o��
					}
				}							//��O�Ŏ~�߂��鏈��
				else if (pPosOld->z - fDepth >= pPolice->posOld.z + POLICAR_DEPTH
					&& pPos->z - fDepth <= pPolice->pos.z + POLICAR_DEPTH)
				{//�O��̈ʒu���v���C���[�̈ʒu���������A���݂̈ʒu���v���C���[�̈ʒu������O�������ꍇ(���Ŏ~�߂��鏈��)
					switch (collObject)
					{
					case COLLOBJECTTYPE_POLICE:		// �x�@�̏ꍇ

						//�ʒu�����炷
						pPolice->pos.z = pPos->z - (POLICAR_DEPTH + fDepth);

						// �ړ��ʂ��폜
						CollisionSlow(&pMove->x);

						// �J�E���g�����Z����
						*pTraCnt += 1;

						break;						// �����o��

					case COLLOBJECTTYPE_CAR:		// �Ԃ̏ꍇ

						//�ʒu�����炷
						pPolice->pos.z = pPos->z - (POLICAR_DEPTH + fDepth);

						// �ړ��ʂ��폜
						CollisionSlow(&pMove->x);

						// �J�E���g�����Z����
						*pTraCnt += 1;

						break;						// �����o��
					}
				}							//���Ŏ~�߂��鏈��
			}

			if (pPos->z - fDepth <= pPolice->pos.z + POLICAR_DEPTH
				&& pPos->z + fDepth >= pPolice->pos.z - POLICAR_DEPTH)
			{//����Z���̒��ɂ����ꍇ
				if (pPosOld->x + fWidth <= pPolice->posOld.x - POLICAR_WIDTH
					&& pPos->x + fWidth >= pPolice->pos.x - POLICAR_WIDTH)
				{//�O��̈ʒu���u���b�N�̍��[��荶���A���݂̈ʒu���u���b�N�̍������E�������ꍇ(���̏���)
					switch (collObject)
					{
					case COLLOBJECTTYPE_POLICE:		// �x�@�̏ꍇ

						//�ʒu�����炷
						pPolice->pos.x = pPos->x + (POLICAR_WIDTH + fWidth);

						// �ړ��ʂ��폜
						CollisionSlow(&pMove->x);

						// �J�E���g�����Z����
						*pTraCnt += 1;

						break;						// �����o��

					case COLLOBJECTTYPE_CAR:		// �Ԃ̏ꍇ

						//�ʒu�����炷
						pPolice->pos.x = pPos->x + (POLICAR_WIDTH + fWidth);

						// �ړ��ʂ��폜
						CollisionSlow(&pMove->x);

						// �J�E���g�����Z����
						*pTraCnt += 1;

						break;						// �����o��
					}
				}							//���[�̏���
				else if (pPosOld->x - fWidth >= pPolice->posOld.x + POLICAR_WIDTH
					&& pPos->x - fWidth <= pPolice->pos.x + POLICAR_WIDTH)
				{//�O��̈ʒu���u���b�N�̉E�[���E���A���݂̈ʒu���u���b�N�̍������E�������ꍇ(�E�̏���)
					switch (collObject)
					{
					case COLLOBJECTTYPE_POLICE:		// �x�@�̏ꍇ

						//�ʒu�����炷
						pPolice->pos.x = pPos->x - (POLICAR_WIDTH + fWidth);

						// �ړ��ʂ��폜
						CollisionSlow(&pMove->x);

						// �J�E���g�����Z����
						*pTraCnt += 1;

						break;						// �����o��

					case COLLOBJECTTYPE_CAR:		// �Ԃ̏ꍇ

						//�ʒu�����炷
						pPolice->pos.x = pPos->x - (POLICAR_WIDTH + fWidth);

						// �ړ��ʂ��폜
						CollisionSlow(&pMove->x);

						// �J�E���g�����Z����
						*pTraCnt += 1;

						break;						// �����o��
					}
				}							//�E�[�̏���
			}
		}
	}
}

//============================================================
// �Ԃ̏a�؉��P����
//============================================================
void CarTrafficImprove(Car *pCar)
{
	// �a�؃J�E���g�����Z����
	pCar->nTrafficCnt++;

	if (pCar->nTrafficCnt >= CAR_TRAFFIC_IMPROVE_CNT)
	{ // �a�؃J�E���g��������Ԃɓ������ꍇ
		// �J�E���g��0�ɂ���
		pCar->nTrafficCnt = 0;

		// �ʏ��Ԃɂ���
		pCar->state = CARSTATE_NORMAL;
	}
}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================
#endif