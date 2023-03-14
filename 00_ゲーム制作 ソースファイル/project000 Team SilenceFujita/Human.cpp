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
#include "calculation.h"

#include "3Dnotation.h"
#include "Human.h"
#include "shadow.h"
#include "sound.h"
#include "player.h"
#include "Police.h"
#include "particle.h"
#include "curve.h"
#include "object.h"
#include "wind.h"
#include "Car.h"

#ifdef _DEBUG	// �f�o�b�O����
#include "game.h"
#endif

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define HUMAN_NORMAL_SETUP_TXT	"data\\MOTION\\humanNormal.txt"		// �l�Ԃ̃��[�V�����̃Z�b�g�A�b�v�p�̃e�L�X�g�t�@�C���̑��΃p�X (�ʏ�)
#define HUMAN_CIGARET_SETUP_TXT	"data\\MOTION\\humanCigaret.txt"	// �l�Ԃ̃��[�V�����̃Z�b�g�A�b�v�p�̃e�L�X�g�t�@�C���̑��΃p�X (�^�o�R)
#define HUMAN_PHONE_SETUP_TXT	"data\\MOTION\\humanPhone.txt"		// �l�Ԃ̃��[�V�����̃Z�b�g�A�b�v�p�̃e�L�X�g�t�@�C���̑��΃p�X (�X�}�z)

#define HUMAN_LIFE				(50)		// �l�̗̑�
#define HUMAN_GRAVITY			(1.0f)		// �l�ɂ�����d��
#define HUMAN_MOVE_FORWARD		(0.1f)		// �l�O�i���̈ړ���
#define HUMAN_MOVE_BACKWARD		(0.2f)		// �l��ގ��̈ړ���
#define HUMAN_MOVE_ROT			(0.012f)	// �l�̌����ύX��
#define REV_HUMAN_MOVE_ROT		(0.1f)		// �ړ��ʂɂ������ύX�ʂ̕␳�W��
#define SUB_HUMAN_MOVE_VALUE	(15.0f)		// �����ύX���̌������s����ړ���
#define SUB_HUMAN_MOVE			(0.05f)		// �����ύX���̌�����
#define MAX_HUMAN_BACKWARD		(8.0f)		// ��ގ��̍ō����x
#define REV_HUMAN_MOVE_SUB		(0.04f)		// �ړ��ʂ̌����W��
#define HUMAN_CURVE_ADD			(0.03f)		// �Ȃ���p�ł̌����̉��Z��
#define HUMAN_RANDAM_MOVE		(6)			// �l�Ԃ̈ړ��ʂ̃����_��
#define HUMAN_MOVE_LEAST		(4)			// �l�Ԃ̈ړ��ʂ̍Œ��
#define HUMAN_PASS_SHIFT		(40.0f)		// �����������̂��炷��
#define HUMAN_RADIUS			(30.0f)		// �l�Ԃ̕�
#define HUMAN_PASS_CORRECT		(0.06f)		// �l�Ԃ̂��炷�␳�{��
#define HUMAN_GROUND			(10.0f)		// �l�Ԃ̒n��
#define HUMAN_OVERLAP_COUNT		(180)		// �l�Ԃ̏d�Ȃ�J�E���g
#define HUMAN_STOP_COUNT		(240)		// �l�Ԃ̗����~�܂�J�E���g
#define HUMAN_CIGARETTE_POS		(5.0f)		// �l�Ԃ̃^�o�R�̉��̈ʒu

#define REACTION_HUMAN_RANGE	(170.0f)	// ���A�N�V��������l�Ԃ͈̔�
#define REACTION_CAR_RANGE		(50.0f)		// ���A�N�V��������Ԃ͈̔�

#define SHADOW_HUMAN_RADIUS		(45.0f)		// �l�Ԃ̉e�̔��a

#define RESURRECT_CNT			(300)		// �����܂ł̃J�E���g
#define HUMAN_SMOKING_CNT		(30)		// �����o��J�E���g

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTextureHumanUV[] =	// �l�Ԃ�UV�e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\human000.png",	// �l��1�̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\human001.png",	// �l��2�̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\human002.png",	// �l��3�̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\human003.png",	// �����^�o�R�̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\human004.png",	// �����X�}�z�̃e�N�X�`�����΃p�X
};

//**********************************************************************************************************************
//	�\���̒�`(Resurrect)
//**********************************************************************************************************************
typedef struct
{
	Human	humanDate;		// �l�Ԃ̏��
	int		nResurCount;	// �����̃J�E���g
	bool	bUse;			// �g�p��
}Resurrect;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void PosHuman(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove, float fMaxMove);	// �l�Ԃ̈ʒu�̍X�V����
void RevHuman(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos);													// �l�Ԃ̕␳�̍X�V����

void CurveHuman(Human *pHuman);				// �l�Ԃ̃J�[�u����
void StopHuman(Human *pHuman);				// �l�Ԃ̒�~����
void ReactionHuman(Human *pHuman);			// �l�Ԃ̃��A�N�V��������
void CollisionCarHuman(Human *pHuman);		// �l�ԂƎԂ̓����蔻��
void CurveRotHuman(Human *pHuman);			// �l�Ԃ̊p�x�X�V����
void WalkHuman(Human *pHuman);				// �l�Ԃ̕�������
void PassingHuman(Human *pHuman);			// �l�Ԃ̂���Ⴂ����
void SetResurrection(Human human);			// �������̐ݒ菈��
void ResurrectionHuman(Human human);		// �l�Ԃ̕�������
void CorrectCurveHuman(Human *human);		// �l�Ԃ̏����ʒu�̐ݒ菈��
bool OverlapHuman(Human *pHuman);			// �l�Ԃ̏d�Ȃ�h�~����

void InitMotionHuman(Human *pHuman, int nType, int nWalk);	// �l�ԃ��[�V�����̏���������
void UpdateMotionHuman(Human *pHuman);						// �l�ԃ��[�V�����̍X�V����
void SetMotionHuman(Human *pHuman, MOTIONTYPE type);		// �l�ԃ��[�V�����̐ݒ菈��

void TxtSetHuman(char *pTxt, HumanParts *pSetParts, MotionInfo *pSetMotion);	// �l�Ԃ̃Z�b�g�A�b�v����

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureHumanUV[HUMANTYPE_MAX] = {};	// �l�Ԃ�UV�e�N�X�`���̃|�C���^

Human     g_aHuman[MAX_HUMAN];		// �l�Ԃ̏��
Resurrect g_aResurrect[MAX_HUMAN];	// �l�Ԃ̕������

HumanParts g_aSetPartsNormal[MAX_PARTS];	// �ʏ�p�[�c�̃Z�b�g�A�b�v�p
HumanParts g_aSetPartsCigaret[MAX_PARTS];	// �^�o�R�p�[�c�̃Z�b�g�A�b�v�p
HumanParts g_aSetPartsPhone[MAX_PARTS];		// �X�}�z�p�[�c�̃Z�b�g�A�b�v�p

MotionInfo g_setMotionNormal;	// �ʏ탂�[�V�����̃Z�b�g�A�b�v�p
MotionInfo g_setMotionCigaret;	// �^�o�R���[�V�����̃Z�b�g�A�b�v�p
MotionInfo g_setMotionPhone;	// �X�}�z���[�V�����̃Z�b�g�A�b�v�p

//======================================================================================================================
//	�l�Ԃ̏���������
//======================================================================================================================
void InitHuman(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// �l�Ԃ�UV�e�N�X�`���̓ǂݍ���
	for (int nCntHuman = 0; nCntHuman < HUMANTYPE_MAX; nCntHuman++)
	{ // �l�Ԃ̎�ނ̑������J��Ԃ�

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, apTextureHumanUV[nCntHuman], &g_apTextureHumanUV[nCntHuman]);
	}

	// �l�Ԃ̃p�[�c�̏�����
	ZeroMemory(&g_aSetPartsNormal[0],  sizeof(HumanParts) * MAX_PARTS);
	ZeroMemory(&g_aSetPartsCigaret[0], sizeof(HumanParts) * MAX_PARTS);
	ZeroMemory(&g_aSetPartsPhone[0],   sizeof(HumanParts) * MAX_PARTS);

	// �l�Ԃ̃Z�b�g�A�b�v����
	TxtSetHuman(HUMAN_NORMAL_SETUP_TXT,  &g_aSetPartsNormal[0],  &g_setMotionNormal);	// �ʏ�
	TxtSetHuman(HUMAN_CIGARET_SETUP_TXT, &g_aSetPartsCigaret[0], &g_setMotionCigaret);	// �^�o�R
	TxtSetHuman(HUMAN_PHONE_SETUP_TXT,   &g_aSetPartsPhone[0],   &g_setMotionPhone);	// �X�}�z

	// �l�Ԃ̏��̏�����
	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++)
	{ // �l�Ԃ̍ő�\�������J��Ԃ�

		// ��{���̏�����
		g_aHuman[nCntHuman].pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aHuman[nCntHuman].posOld			= g_aHuman[nCntHuman].pos;			// �O��̈ʒu
		g_aHuman[nCntHuman].move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
		g_aHuman[nCntHuman].fMaxMove		= 0.0f;								// �ړ��ʂ̍ő吔
		g_aHuman[nCntHuman].rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aHuman[nCntHuman].nShadowID		= NONE_SHADOW;						// �e�̃C���f�b�N�X
		g_aHuman[nCntHuman].nNotaID			= NONE_3D_NOTATION;					// �����\���̃C���f�b�N�X
		g_aHuman[nCntHuman].nOverlapCounter = 0;								// �d�Ȃ�h�~�J�E���g
		g_aHuman[nCntHuman].nStopCount		= 0;								// ��~�J�E���g
		g_aHuman[nCntHuman].bMove			= false;							// �ړ����Ă��邩
		g_aHuman[nCntHuman].bRecur			= false;							// ������
		g_aHuman[nCntHuman].bUse			= false;							// �g�p��
		g_aHuman[nCntHuman].state			= HUMANSTATE_WALK;					// ���

		// �W���b�W�̏��̏�����
		g_aHuman[nCntHuman].judge.col      = JUDGE_WHITE;			// �s�J�s�J�̐F
		g_aHuman[nCntHuman].judge.state    = JUDGESTATE_JUSTICE;	// �P��
		g_aHuman[nCntHuman].judge.ticatica = CHICASTATE_BLACKOUT;	// �`�J�`�J���

		// �Ȃ���p�֌W�̏�����
		g_aHuman[nCntHuman].curveInfo.actionState  = HUMANACT_WALK;					// ���s���
		g_aHuman[nCntHuman].curveInfo.nRandamRoute = 0;								// �i�ރ��[�g�̎��
		g_aHuman[nCntHuman].curveInfo.rotDest      = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW�̈ʒu
		g_aHuman[nCntHuman].curveInfo.curveInfo.nCurveTime = 0;						// �Ȃ���p�̐�
		g_aHuman[nCntHuman].curveInfo.curveInfo.nNowCurve  = 0;						// ���݂̋Ȃ���p

		// �A�C�R���̏��̏�����
		g_aHuman[nCntHuman].icon.nIconID = NONE_ICON;		// �A�C�R���̃C���f�b�N�X
		g_aHuman[nCntHuman].icon.state   = ICONSTATE_NONE;	// �A�C�R���̏��

		// �ړ��̎��
		g_aHuman[nCntHuman].typeMove = MOVETYPE_STOP;

		for (int nCntCur = 0; nCntCur < MAX_HUMAN_CURVE; nCntCur++)
		{
			g_aHuman[nCntHuman].curveInfo.curveInfo.curvePoint[nCntCur] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �Ȃ���p�̈ʒu
			g_aHuman[nCntHuman].curveInfo.curveInfo.curveAngle[nCntCur] = CURVE_RIGHT;						// �E�ɋȂ���
			g_aHuman[nCntHuman].curveInfo.curveInfo.dashAngle[nCntCur]  = DASH_RIGHT;						// �E�Ɍ������đ����Ă���
		}

		// �����֌W�̏��̏�����
		g_aResurrect[nCntHuman].nResurCount = 0;			// �J�E���g
		g_aResurrect[nCntHuman].bUse = false;				// �g�p���Ă��Ȃ�
	}
}

//======================================================================================================================
//	�l�Ԃ̏I������
//======================================================================================================================
void UninitHuman(void)
{
	// �l�Ԃ�UV�e�N�X�`���̓ǂݍ���
	for (int nCntHuman = 0; nCntHuman < HUMANTYPE_MAX; nCntHuman++)
	{ // �l�Ԃ̎�ނ̑������J��Ԃ�

		if (g_apTextureHumanUV[nCntHuman] != NULL)
		{ // �ϐ� (g_apTextureHumanUV) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureHumanUV[nCntHuman]->Release();
			g_apTextureHumanUV[nCntHuman] = NULL;
		}
	}
}

//======================================================================================================================
//	�l�Ԃ̍X�V����
//======================================================================================================================
void UpdateHuman(void)
{
	int nCnt = 0;		// �����ݒ�p
	int nTackleCnt = 0;	// �����ݒ�p
	float fMove = 0.0f;	// �����ݒ�p
	POLICESTATE policeState = POLICESTATE_CHASE;	// �x�@�̏��(�I�u�W�F�N�g�Ƃ̓����蔻��Ɏg�����ߖ��Ӗ�)

	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�
		if (g_aHuman[nCntHuman].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			if (g_aHuman[nCntHuman].nStopCount == 0)
			{ // ��~�J�E���g��0�̏ꍇ

				// �ړ�����
				g_aHuman[nCntHuman].bMove = true;
			}
			else
			{ // ��L�ȊO

				// �ړ����Ȃ�
				g_aHuman[nCntHuman].bMove = false;

				// ��~�J�E���g�����Z����
				g_aHuman[nCntHuman].nStopCount++;

				if (g_aHuman[nCntHuman].nStopCount >= HUMAN_STOP_COUNT)
				{ // ��~�J�E���g����萔�ɂȂ����ꍇ

					// ��~�J�E���g������������
					g_aHuman[nCntHuman].nStopCount = 0;
				}
			}

			// �O��ʒu�̍X�V
			g_aHuman[nCntHuman].posOld = g_aHuman[nCntHuman].pos;

			// �v���C���[�̈ʒu�̍X�V
			PosHuman
			(
				&g_aHuman[nCntHuman].move,			// �ړ���
				&g_aHuman[nCntHuman].pos,			// �ʒu
				&g_aHuman[nCntHuman].rot,			// ����
				g_aHuman[nCntHuman].bMove,			// �ړ����Ă��邩
				g_aHuman[nCntHuman].fMaxMove		// �ړ��ʂ̍ő吔
			);

			if (g_aHuman[nCntHuman].judge.state == JUDGESTATE_EVIL)
			{ // ���҂������ꍇ

				// �W���b�W�̍X�V����
				UpdateJudge(&g_aHuman[nCntHuman].judge);

				// �A�C�R���̈ʒu�ݒ菈��
				SetPositionIcon(g_aHuman[nCntHuman].icon.nIconID, g_aHuman[nCntHuman].pos);
			}

			//�l�Ԃ̃��A�N�V��������
			ReactionHuman(&g_aHuman[nCntHuman]);

			// ���̓����蔻��
			CollisionWind(&g_aHuman[nCntHuman]);

			switch (g_aHuman[nCntHuman].state)
			{
			case HUMANSTATE_WALK:		//�������

				if (g_aHuman[nCntHuman].typeMove == MOVETYPE_MOVE)
				{ // ������ނ̏ꍇ

					//�l�Ԃ̃J�[�u����
					CurveHuman(&g_aHuman[nCntHuman]);
				}

				break;					//�����o��

			case HUMANSTATE_STOP:		//�~�܂������

				if (g_aHuman[nCntHuman].typeMove == MOVETYPE_MOVE)
				{ // ������ނ̏ꍇ

					// �l�Ԃ̒�~����
					StopHuman(&g_aHuman[nCntHuman]);
				}

				break;					//�����o��

			case HUMANSTATE_FLY:		//������񂾏��

				// �p�[�e�B�N���̐ݒ菈��
				SetParticle
				(
					g_aHuman[nCntHuman].pos, 
					D3DXCOLOR(0.2f,0.2f,0.2f,1.0f),
					PARTICLETYPE_HUMAN_FLY, 
					5, 
					3
				);		

				// ��΂�
				g_aHuman[nCntHuman].pos.x += g_aHuman[nCntHuman].move.x;
				g_aHuman[nCntHuman].pos.z += g_aHuman[nCntHuman].move.z;

				if (g_aHuman[nCntHuman].pos.y <= HUMAN_GROUND)
				{ // �ʒu��0.0f�ȉ��ɂȂ����ꍇ

					if (g_aHuman[nCntHuman].bRecur == true)
					{ // ��������ꍇ

						// �������̐ݒ菈��
						SetResurrection(g_aHuman[nCntHuman]);
					}

					// �g�p���Ă��Ȃ�
					g_aHuman[nCntHuman].bUse = false;
				}

				break;					//�����o��
			}

			if (g_aHuman[nCntHuman].pos.y < HUMAN_GROUND)
			{ // Y���̈ʒu��0.0f�������ꍇ

				// �c�ւ̈ړ��ʂ�0.0f�ɂ���
				g_aHuman[nCntHuman].move.y = 0.0f;

				// �ʒu��0.0f�ɖ߂�
				g_aHuman[nCntHuman].pos.y = HUMAN_GROUND;
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
				HUMAN_DEPTH,					// ���s
				&nCnt,							// �a�؃J�E���g
				BOOSTSTATE_NONE,				// �u�[�X�g���
				&policeState,					// �x�@�̏��
				&nTackleCnt,					// �^�b�N���J�E���g
				&fMove							// �^�b�N�����̈ړ���
			);

			// �v���C���[�̕␳�̍X�V����
			RevHuman(&g_aHuman[nCntHuman].rot, &g_aHuman[nCntHuman].pos);

			// �e�̈ʒu�ݒ�
			SetPositionShadow
			( // ����
				g_aHuman[nCntHuman].nShadowID,		// �e�̃C���f�b�N�X
				D3DXVECTOR3(g_aHuman[nCntHuman].pos.x, HUMAN_GROUND, g_aHuman[nCntHuman].pos.z),			// �ʒu
				g_aHuman[nCntHuman].rot,			// ����
				NONE_SCALE							// �g�嗦
			);

			// �����\���̈ʒu�ݒ�
			SetPosition3DNotation
			( // ����
				g_aHuman[nCntHuman].nNotaID,		// �����\���C���f�b�N�X
				D3DXVECTOR3							// �ʒu
				( // ����
					g_aHuman[nCntHuman].pos.x,										// x
					g_aHuman[nCntHuman].pos.y + HUMAN_HEIGHT + NOTA_PLUS_POS_HUMAN,	// y
					g_aHuman[nCntHuman].pos.z										// z
				)
			);

			// ���[�V�����̍X�V
			UpdateMotionHuman(&g_aHuman[nCntHuman]);
		}

		if (g_aResurrect[nCntHuman].bUse == true)
		{ // �����֌W�̏����g�p���Ă����ꍇ

			// �����J�E���g�����Z����
			g_aResurrect[nCntHuman].nResurCount++;

			if (g_aResurrect[nCntHuman].nResurCount >= RESURRECT_CNT)
			{ // �����J�E���g����萔�ɂȂ����ꍇ

				// �g�p���Ȃ�
				g_aResurrect[nCntHuman].bUse = false;

				// �l�Ԃ̕�������
				ResurrectionHuman(g_aResurrect[nCntHuman].humanDate);
			}
		}
	}

	for (int nCntOverlap = 0; nCntOverlap < MAX_HUMAN; nCntOverlap++)
	{ // �l�Ԃ̏d�Ȃ�h�~����

		if (OverlapHuman(&g_aHuman[nCntOverlap]) == false)
		{ // �d�Ȃ����ꍇ

			// �d�Ȃ�h�~�J�E���^�[������������
			g_aHuman[nCntOverlap].nOverlapCounter = 0;
		}
	}
}

//======================================================================================================================
//	�l�Ԃ̕`�揈��
//======================================================================================================================
void DrawHuman(void)
{
	// �ϐ���錾
	float        humanRot;						// �l�Ԃ̌����̌v�Z�p
	D3DXMATRIX   mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p
	D3DXMATRIX   mtxRotParts, mtxTransParts;	// �K�w�\���̌v�Z�p�}�g���b�N�X
	D3DXMATRIX   mtxParent;						// �e�̃}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL	  evilMat;					// �����z�̃}�e���A���f�[�^

	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aHuman[nCntHuman].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aHuman[nCntHuman].mtxWorld);

			// �l�Ԃ̌�����ݒ�
			humanRot = g_aHuman[nCntHuman].rot.y + D3DX_PI;
			RotNormalize(&humanRot);	// �����𐳋K��

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, humanRot, g_aHuman[nCntHuman].rot.x, g_aHuman[nCntHuman].rot.z);
			D3DXMatrixMultiply(&g_aHuman[nCntHuman].mtxWorld, &g_aHuman[nCntHuman].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aHuman[nCntHuman].pos.x, g_aHuman[nCntHuman].pos.y, g_aHuman[nCntHuman].pos.z);
			D3DXMatrixMultiply(&g_aHuman[nCntHuman].mtxWorld, &g_aHuman[nCntHuman].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aHuman[nCntHuman].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			for (int nCntParts = 0; nCntParts < g_aHuman[nCntHuman].motion.nParts; nCntParts++)
			{ // �p�[�c�̑������J��Ԃ�

				// �p�[�c�̃��[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_aHuman[nCntHuman].aParts[nCntParts].mtxWorld);

				// �����𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRotParts, g_aHuman[nCntHuman].aParts[nCntParts].rot.y, g_aHuman[nCntHuman].aParts[nCntParts].rot.x, g_aHuman[nCntHuman].aParts[nCntParts].rot.z);
				D3DXMatrixMultiply(&g_aHuman[nCntHuman].aParts[nCntParts].mtxWorld, &g_aHuman[nCntHuman].aParts[nCntParts].mtxWorld, &mtxRotParts);

				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTransParts, g_aHuman[nCntHuman].aParts[nCntParts].pos.x, g_aHuman[nCntHuman].aParts[nCntParts].pos.y, g_aHuman[nCntHuman].aParts[nCntParts].pos.z);
				D3DXMatrixMultiply(&g_aHuman[nCntHuman].aParts[nCntParts].mtxWorld, &g_aHuman[nCntHuman].aParts[nCntParts].mtxWorld, &mtxTransParts);

				// �p�[�c�̐e�}�g���b�N�X��ݒ�
				if (g_aHuman[nCntHuman].aParts[nCntParts].nParentID == NONE_PARENT)
				{ // ���݂̃p�[�c�ɐe�����݂��Ȃ��ꍇ

					// �v���C���[�̃}�g���b�N�X��ݒ�
					mtxParent = g_aHuman[nCntHuman].mtxWorld;
				}
				else
				{ // ���݂̃p�[�c�ɐe�����݂���ꍇ

					// �e�̃}�g���b�N�X��ݒ�
					mtxParent = g_aHuman[nCntHuman].aParts[g_aHuman[nCntHuman].aParts[nCntParts].nParentID].mtxWorld;
				}

				// �p�[�c�̃��[���h�}�g���b�N�X�Ɛe�}�g���b�N�X���|�����킹��
				D3DXMatrixMultiply(&g_aHuman[nCntHuman].aParts[nCntParts].mtxWorld, &g_aHuman[nCntHuman].aParts[nCntParts].mtxWorld, &mtxParent);

				// �p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_aHuman[nCntHuman].aParts[nCntParts].mtxWorld);

				// �}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_aHuman[nCntHuman].aParts[nCntParts].modelData.pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aHuman[nCntHuman].aParts[nCntParts].modelData.dwNumMat; nCntMat++)
				{ // �}�e���A���̐����J��Ԃ�

					// �\���̗̂v�f���N���A
					ZeroMemory(&evilMat, sizeof(D3DXMATERIAL));

					// �}�e���A���̃R�s�[�ɑ������
					evilMat = pMat[nCntMat];

					if (g_aHuman[nCntHuman].judge.state == JUDGESTATE_JUSTICE)
					{ // �ǂ��z�̏ꍇ

						// �}�e���A���̐ݒ�
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
					}
					else
					{ // �����z�̏ꍇ

						// ���Ȕ�����������
						evilMat.MatD3D.Emissive = g_aHuman[nCntHuman].judge.col;

						// �}�e���A���̐ݒ�
						pDevice->SetMaterial(&evilMat.MatD3D);
					}

					if (nCntParts < MAX_PARTS - 1)
					{ // �̃p�[�c���f���̏ꍇ

						// �e�N�X�`���̐ݒ�
						pDevice->SetTexture(0, g_apTextureHumanUV[g_aHuman[nCntHuman].type]);
					}
					else
					{ // ������f���̏ꍇ

						// �e�N�X�`���̐ݒ�
						pDevice->SetTexture(0, g_aHuman[nCntHuman].aParts[nCntParts].modelData.pTexture[nCntMat]);
					}

					// ���f���̕`��
					g_aHuman[nCntHuman].aParts[nCntParts].modelData.pMesh->DrawSubset(nCntMat);
				}
			}

			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	�l�Ԃ̐ݒ菈��
//======================================================================================================================
void SetHuman(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int walk, bool bRecur, int type)
{
	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aHuman[nCntHuman].bUse == false)
		{ // �I�u�W�F�N�g���g�p����Ă��Ȃ��ꍇ

			// �ʒu��ݒ�
			g_aHuman[nCntHuman].pos				= D3DXVECTOR3(pos.x, HUMAN_GROUND, pos.z);	// ���݂̈ʒu
			g_aHuman[nCntHuman].posOld			= D3DXVECTOR3(pos.x, HUMAN_GROUND, pos.z);	// �O��̈ʒu

			// ��������
			g_aHuman[nCntHuman].typeMove		= (MOVETYPE)(walk);		// �ړ��̎��
			g_aHuman[nCntHuman].bRecur			= bRecur;				// ������
			g_aHuman[nCntHuman].type			= (HUMANTYPE)(type);	// ���
			g_aHuman[nCntHuman].rot				= rot;					// ����

			// ����������
			g_aHuman[nCntHuman].move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
			g_aHuman[nCntHuman].bMove			= false;							// �ړ����Ă��Ȃ�
			g_aHuman[nCntHuman].nStopCount		= 0;								// ��~�J�E���g
			g_aHuman[nCntHuman].nOverlapCounter = 0;								// �d�Ȃ�h�~�J�E���g
			g_aHuman[nCntHuman].state			= HUMANSTATE_WALK;					// �������
			g_aHuman[nCntHuman].nNotaID			= NONE_3D_NOTATION;					// �����\���̃C���f�b�N�X

			// �ړ��ʂ̍ő�l��ݒ�
			g_aHuman[nCntHuman].fMaxMove = (float)(rand() % HUMAN_RANDAM_MOVE + HUMAN_MOVE_LEAST);

			// �g�p���Ă����Ԃɂ���
			g_aHuman[nCntHuman].bUse = true;

			// �e�̃C���f�b�N�X��ݒ�
			g_aHuman[nCntHuman].nShadowID = SetCircleShadow
			( // ����
				0.5f,							// ���l
				SHADOW_HUMAN_RADIUS,			// ���a
				&g_aHuman[nCntHuman].nShadowID,	// �e�̐e�̉e�C���f�b�N�X
				&g_aHuman[nCntHuman].bUse		// �e�̐e�̎g�p��
			);

			// �e�̈ʒu�ݒ�
			SetPositionShadow
			(
				g_aHuman[nCntHuman].nShadowID, 
				g_aHuman[nCntHuman].pos,
				g_aHuman[nCntHuman].rot, 
				D3DXVECTOR3(1.0f, 1.0f, 1.0f)
			);

			switch (g_aHuman[nCntHuman].typeMove)
			{
			case MOVETYPE_MOVE:			// �ړ�����

				// �Ȃ���p���̐ݒu
				g_aHuman[nCntHuman].curveInfo.actionState = HUMANACT_WALK;							// ���
				g_aHuman[nCntHuman].curveInfo.nRandamRoute = rand() % MAX_HUMAN_ROUTE;				// ���[�g�̎��
				g_aHuman[nCntHuman].curveInfo.curveInfo = GetHumanRoute(g_aHuman[nCntHuman].curveInfo.nRandamRoute);	// ���[�g

				// �l�Ԃ̏����ʒu�̐ݒ菈��
				CorrectCurveHuman(&g_aHuman[nCntHuman]);

				g_aHuman[nCntHuman].curveInfo.rotDest = g_aHuman[nCntHuman].rot;					// �ڕW�̌���

				// �����𔲂���
				break;

			case MOVETYPE_STOP:			// ��~���

				// ����

				// �����𔲂���
				break;					
			}

			if (g_aHuman[nCntHuman].type == HUMANTYPE_CIGARETTE ||
				g_aHuman[nCntHuman].type == HUMANTYPE_SMARTPHONE)
			{ // �����z�������ꍇ

				g_aHuman[nCntHuman].judge.state = JUDGESTATE_EVIL;			// �P��
				g_aHuman[nCntHuman].judge.ticatica = CHICASTATE_BLACKOUT;	// �`�J�`�J���

				// �A�C�R���̏��̏�����
				g_aHuman[nCntHuman].icon.nIconID = NONE_ICON;				// �A�C�R���̃C���f�b�N�X
				g_aHuman[nCntHuman].icon.state = ICONSTATE_NONE;			// �A�C�R���̏��

				// �����\���̃C���f�b�N�X��ݒ�
				g_aHuman[nCntHuman].nNotaID = Set3DNotation
				( // ����
					NOTATIONTYPE_FLY,				// �����\���̎��
					&g_aHuman[nCntHuman].nNotaID,	// �e�̋����\���C���f�b�N�X
					&g_aHuman[nCntHuman].bUse		// �e�̎g�p��
				);

				// �����\���̈ʒu�ݒ�
				SetPosition3DNotation
				( // ����
					g_aHuman[nCntHuman].nNotaID,	// �����\���C���f�b�N�X
					D3DXVECTOR3						// �ʒu
					( // ����
						g_aHuman[nCntHuman].pos.x,										// x
						g_aHuman[nCntHuman].pos.y + HUMAN_HEIGHT + NOTA_PLUS_POS_HUMAN,	// y
						g_aHuman[nCntHuman].pos.z										// z
					)
				);
			}
			else
			{ // �ǂ��z�������ꍇ

				g_aHuman[nCntHuman].judge.state = JUDGESTATE_JUSTICE;		// �P��
				g_aHuman[nCntHuman].judge.ticatica = CHICASTATE_BLACKOUT;	// �`�J�`�J���
			}

			// �W���b�W�̏��̐ݒ�
			g_aHuman[nCntHuman].judge.col = JUDGE_WHITE;			// �s�J�s�J�̐F

			if (g_aHuman[nCntHuman].judge.state == JUDGESTATE_EVIL)
			{ // �����z�������ꍇ

				// �A�C�R���̐ݒ菈��
				g_aHuman[nCntHuman].icon.nIconID = SetIcon
				( // ����
					g_aHuman[nCntHuman].pos,
					ICONTYPE_EVIL_HUMAN,
					&g_aHuman[nCntHuman].icon.nIconID,
					&g_aHuman[nCntHuman].bUse,
					&g_aHuman[nCntHuman].icon.state
				);
			}

			// �l�ԃ��[�V�����̏���������
			InitMotionHuman(&g_aHuman[nCntHuman], type, walk);

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
void PosHuman(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove, float fMaxMove)
{
	//--------------------------------------------------------
	//	�d�͂̍X�V
	//--------------------------------------------------------
	move->y -= HUMAN_GRAVITY;

	//--------------------------------------------------------
	//	�ړ��ʂ̕␳
	//--------------------------------------------------------
	if (move->x > fMaxMove)
	{ // �v���C���[�̈ړ��� (x) �����l�ȏ�̏ꍇ

		// �v���C���[�̈ړ��� (x) ��␳
		move->x = fMaxMove;
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
	if      (rot->y > D3DX_PI) { rot->y -= D3DX_PI * 2; }
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
	if (pHuman->bMove == true)
	{ // �ړ�����ꍇ

		// �ړ��ʂ��X�V
		pHuman->move.x += HUMAN_MOVE_FORWARD;
	}

	switch (pHuman->curveInfo.actionState)
	{
	case HUMANACT_WALK:		// �������

		// �l�Ԃ̂���Ⴂ����
		PassingHuman(pHuman);

		break;				// �����o��

	case HUMANACT_CURVE:	// �J�[�u���

		// �l�Ԃ̊p�x�X�V�E�␳����
		CurveRotHuman(pHuman);

		break;				// �����o��
	}

	if (pHuman->move.x > pHuman->fMaxMove)
	{ // �v���C���[�̈ړ��� (x) �����l�ȏ�̏ꍇ

		// �v���C���[�̈ړ��� (x) ��␳
		pHuman->move.x = pHuman->fMaxMove;
	}
}

//============================================================
// �l�Ԃ̕�������
//============================================================
void WalkHuman(Human *pHuman)
{
	switch (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve])
	{
	case DASH_RIGHT:	// �E�ɐi��ł���ꍇ

		if (pHuman->pos.x >= pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x)
		{ // �ړI�n�ɒ�������

			// �J�[�u��ԂɂȂ�
			pHuman->curveInfo.actionState = HUMANACT_CURVE;
		}
		else
		{ // ���ɂ���ǂ��܂���������

			// ���̕ǂɔ��킹��
			pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;
		}

		break;			//�����o��

	case DASH_LEFT:		// ���ɐi��ł���ꍇ

		if (pHuman->pos.x <= pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x)
		{ // �ړI�n�ɒ�������

			// �J�[�u��ԂɂȂ�
			pHuman->curveInfo.actionState = HUMANACT_CURVE;
		}
		else
		{ // ���ɂ���ǂ��܂���������

			// ��O�̕ǂɔ��킹��
			pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;
		}

		break;			// �����o��

	case DASH_FAR:		// ���ɐi��ł���ꍇ

		if (pHuman->pos.z >= pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z)
		{ // �ړI�n�ɒ�������

			// �J�[�u��ԂɂȂ�
			pHuman->curveInfo.actionState = HUMANACT_CURVE;
		}
		else
		{ // ���ɂ���ǂ��܂���������

			// �E�̕ǂɔ��킹��
			pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;
		}

		break;			// �����o��

	case DASH_NEAR:		// ��O�ɐi��ł���ꍇ

		if (pHuman->pos.z <= pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z)
		{ // ���ɂ���ǂ��r�؂ꂽ��

			// �J�[�u��ԂɂȂ�
			pHuman->curveInfo.actionState = HUMANACT_CURVE;
		}
		else
		{ // ���ɂ���ǂ��܂���������

			// ���̕ǂɔ��킹��
			pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;
		}

		break;			// �����o��
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
	float fLength;	// �����̕ϐ�

	Player *pPlayer = GetPlayer();

	if (pHuman->state != HUMANSTATE_FLY)
	{ // ������я�Ԃ���Ȃ��ꍇ

		if (pPlayer->bUse == true)
		{ // �g�p���Ă���ꍇ

			// �����𑪂�
			fLength = (pPlayer->pos.x - pHuman->pos.x) * (pPlayer->pos.x - pHuman->pos.x)
					+ (pPlayer->pos.z - pHuman->pos.z) * (pPlayer->pos.z - pHuman->pos.z);

			if (fLength <= (pPlayer->modelData.fRadius + REACTION_CAR_RANGE) * REACTION_HUMAN_RANGE)
			{ // �v���C���[���߂��ɗ����ꍇ

				if (pHuman->state != HUMANSTATE_STOP)
				{ // �X�g�b�v��Ԃł͂Ȃ��ꍇ

					// �X�g�b�v��Ԃɂ���
					pHuman->state = HUMANSTATE_STOP;

					// �|���胂�[�V�����ɐݒ�
					SetMotionHuman(pHuman, MOTIONTYPE_SCARED);
				}
			}
			else
			{ // �v���C���[�ɋ߂��ɂ��Ȃ��ꍇ

				if (pHuman->state != HUMANSTATE_WALK)
				{ // ������Ԃł͂Ȃ��ꍇ

					// ������Ԃɂ���
					pHuman->state = HUMANSTATE_WALK;

					if (pHuman->typeMove == MOVETYPE_MOVE)
					{ // �ړ�����l�Ԃ̏ꍇ

						// �ړ����[�V�����ɐݒ�
						SetMotionHuman(pHuman, MOTIONTYPE_MOVE);
					}
					else
					{ // ��~����l�Ԃ̏ꍇ

						// ��~���[�V�����ɐݒ�
						SetMotionHuman(pHuman, MOTIONTYPE_STOP);
					}
				}
			}
		}
	}
}

//============================================================
// �l�ԂƎԂ̓����蔻��
//============================================================
void CollisionCarHuman(Human *pHuman)
{
	{ // �Ԃ̓����蔻��
		Car *pCar = GetCarData();					// �Ԃ̏����擾����

		for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
		{
			if (pCar[nCntCar].bUse == true)
			{ // �Ԃ��g�p����Ă����ꍇ
				if (pHuman->pos.x - HUMAN_WIDTH <= pCar[nCntCar].pos.x + pCar[nCntCar].modelData.vtxMax.x 
					&& pHuman->pos.x + HUMAN_WIDTH >= pCar[nCntCar].pos.x + pCar[nCntCar].modelData.vtxMin.x)
				{ // �Ԃ�X���̒��ɂ����ꍇ
					if (pHuman->posOld.z + HUMAN_DEPTH <= pCar[nCntCar].posOld.z + pCar[nCntCar].modelData.vtxMin.z
						&& pHuman->pos.z + HUMAN_DEPTH >= pCar[nCntCar].pos.z + pCar[nCntCar].modelData.vtxMin.z)
					{//�O��̈ʒu���u���b�N����O���A���݂̈ʒu���u���b�N���������������ꍇ(��O�Ŏ~�߂��鏈��)

						// �ړ��ʂ�0�ɂ���
						pCar[nCntCar].move.x = 0.0f;

					}							//��O�Ŏ~�߂��鏈��
					else if (pHuman->posOld.z - HUMAN_DEPTH >= pCar[nCntCar].posOld.z + pCar[nCntCar].modelData.vtxMax.z
						&& pHuman->pos.z - HUMAN_DEPTH <= pCar[nCntCar].pos.z + pCar[nCntCar].modelData.vtxMax.z)
					{//�O��̈ʒu�����̈ʒu���������A���݂̈ʒu�����̈ʒu������O�������ꍇ(���Ŏ~�߂��鏈��)
					
						// �ړ��ʂ�0�ɂ���
						pCar[nCntCar].move.x = 0.0f;

					}							//���Ŏ~�߂��鏈��
				}

				if (pHuman->pos.z - HUMAN_DEPTH <= pCar[nCntCar].pos.z + pCar[nCntCar].modelData.vtxMax.z
					&& pHuman->pos.z + HUMAN_DEPTH >= pCar[nCntCar].pos.z + pCar[nCntCar].modelData.vtxMin.z)
				{//����Z���̒��ɂ����ꍇ
					if (pHuman->posOld.x + HUMAN_WIDTH <= pCar[nCntCar].posOld.x + pCar[nCntCar].modelData.vtxMin.x
						&& pHuman->pos.x + HUMAN_WIDTH >= pCar[nCntCar].pos.x + pCar[nCntCar].modelData.vtxMin.x)
					{//�O��̈ʒu���u���b�N�̍��[��荶���A���݂̈ʒu���u���b�N�̍������E�������ꍇ(���̏���)
						
						//�ړ��ʂ�0�ɂ���
						pCar[nCntCar].move.x = 0.0f;

					}							//���[�̏���
					else if (pHuman->posOld.x - HUMAN_WIDTH >= pCar[nCntCar].posOld.x + pCar[nCntCar].modelData.vtxMax.x
						&& pHuman->pos.x - HUMAN_WIDTH <= pCar[nCntCar].pos.x + pCar[nCntCar].modelData.vtxMax.x)
					{//�O��̈ʒu���u���b�N�̉E�[���E���A���݂̈ʒu���u���b�N�̍������E�������ꍇ(�E�̏���)
						
						// �ړ��ʂ�0�ɂ���
						pCar[nCntCar].move.x = 0.0f;

					}							//�E�[�̏���
				}
			}
		}
	}

	{ // �x�@�̓����蔻��
		Police *pPolice = GetPoliceData();					// �x�@�̏����擾����

		for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
		{
			if (pPolice[nCntPolice].bUse == true)
			{ // �Ԃ��g�p����Ă����ꍇ
				if (pHuman->pos.x - HUMAN_WIDTH <= pPolice[nCntPolice].pos.x + pPolice[nCntPolice].modelData.vtxMax.x 
					&& pHuman->pos.x + HUMAN_WIDTH >= pPolice[nCntPolice].pos.x + pPolice[nCntPolice].modelData.vtxMin.x)
				{ // �Ԃ�X���̒��ɂ����ꍇ
					if (pHuman->posOld.z + HUMAN_DEPTH <= pPolice[nCntPolice].posOld.z + pPolice[nCntPolice].modelData.vtxMin.z
						&& pHuman->pos.z + HUMAN_DEPTH >= pPolice[nCntPolice].pos.z + pPolice[nCntPolice].modelData.vtxMin.z)
					{//�O��̈ʒu���u���b�N����O���A���݂̈ʒu���u���b�N���������������ꍇ(��O�Ŏ~�߂��鏈��)

						// �ړ��ʂ�0.0f�ɂ���
						pPolice[nCntPolice].move.x = 0.0f;

					}							//��O�Ŏ~�߂��鏈��
					else if (pHuman->posOld.z - HUMAN_DEPTH >= pPolice[nCntPolice].posOld.z + pPolice[nCntPolice].modelData.vtxMax.z
						&& pHuman->pos.z - HUMAN_DEPTH <= pPolice[nCntPolice].pos.z + pPolice[nCntPolice].modelData.vtxMax.z)
					{//�O��̈ʒu�����̈ʒu���������A���݂̈ʒu�����̈ʒu������O�������ꍇ(���Ŏ~�߂��鏈��)

						// �ړ��ʂ�0.0f�ɂ���
						pPolice[nCntPolice].move.x = 0.0f;

					}							//���Ŏ~�߂��鏈��
				}

				if (pHuman->pos.z - HUMAN_DEPTH <= pPolice[nCntPolice].pos.z + pPolice[nCntPolice].modelData.vtxMax.z
					&& pHuman->pos.z + HUMAN_DEPTH >= pPolice[nCntPolice].pos.z + pPolice[nCntPolice].modelData.vtxMin.z)
				{//����Z���̒��ɂ����ꍇ
					if (pHuman->posOld.x + HUMAN_WIDTH <= pPolice[nCntPolice].posOld.x + pPolice[nCntPolice].modelData.vtxMin.x
						&& pHuman->pos.x + HUMAN_WIDTH >= pPolice[nCntPolice].pos.x + pPolice[nCntPolice].modelData.vtxMin.x)
					{//�O��̈ʒu���u���b�N�̍��[��荶���A���݂̈ʒu���u���b�N�̍������E�������ꍇ(���̏���)
						
						// �ړ��ʂ�0.0f�ɂ���
						pPolice[nCntPolice].move.x = 0.0f;

					}							//���[�̏���
					else if (pHuman->posOld.x - HUMAN_WIDTH >= pPolice[nCntPolice].posOld.x + pPolice[nCntPolice].modelData.vtxMax.x
						&& pHuman->pos.x - HUMAN_WIDTH <= pPolice[nCntPolice].pos.x + pPolice[nCntPolice].modelData.vtxMax.x)
					{//�O��̈ʒu���u���b�N�̉E�[���E���A���݂̈ʒu���u���b�N�̍������E�������ꍇ(�E�̏���)
						
						// �ړ��ʂ�0.0f�ɂ���
						pPolice[nCntPolice].move.x = 0.0f;

					}							//�E�[�̏���
				}
			}
		}
	}
}

//============================================================
// �l�Ԃ̊p�x�X�V�E�␳����
//============================================================
void CurveRotHuman(Human *pHuman)
{
	switch (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve])
	{
	case CURVE_LEFT:	// ���ɋȂ���ꍇ

		// �ړ��ʂ�����
		pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

		// �������X�V
		pHuman->rot.y -= 0.05f * (pHuman->move.x * 0.1f);

		switch (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve])
		{
		case DASH_RIGHT:	// �E�ɐi��ł���ꍇ

			// �ʒu��␳����
			pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;

			// �ڕW�̌�����ݒ肷��
			pHuman->curveInfo.rotDest.y = 0.0f;

			break;			// �����o��

		case DASH_LEFT:		// ���ɐi��ł���ꍇ

			// �ʒu��␳����
			pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;

			// �ڕW�̌�����ݒ肷��
			pHuman->curveInfo.rotDest.y = -D3DX_PI;

			break;			// �����o��

		case DASH_FAR:		// ���ɐi��ł���ꍇ

			// �ʒu��␳����
			pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;

			// �ڕW�̌�����ݒ肷��
			pHuman->curveInfo.rotDest.y = -D3DX_PI * 0.5f;

			break;			// �����o��

		case DASH_NEAR:		// ��O�ɐi��ł���ꍇ

			// �ʒu��␳����
			pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;

			// �ڕW�̌�����ݒ肷��
			pHuman->curveInfo.rotDest.y = D3DX_PI * 0.5f;

			break;			// �����o��
		}

		// ������������
		pHuman->rot.y -= HUMAN_CURVE_ADD;

		if (pHuman->rot.y <= pHuman->curveInfo.rotDest.y)
		{ // �������ڕW�l�𒴂����ꍇ

			// ������␳����
			pHuman->rot.y = pHuman->curveInfo.rotDest.y;

			// ���s��Ԃɂ���
			pHuman->curveInfo.actionState = HUMANACT_WALK;

			// �x�@�̍s���ݒ肷��
			pHuman->curveInfo.curveInfo.nNowCurve = (pHuman->curveInfo.curveInfo.nNowCurve + 1) % pHuman->curveInfo.curveInfo.nCurveTime;

			if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_NEAR)
			{ // ���ɑ����Ă���ꍇ

				if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_LEFT)
				{ // ���̋Ȃ���p�����������ꍇ

					// �������}�C�i�X�ɂ���
					pHuman->rot.y = (float)D3DXToRadian(180);
				}
			}
		}

		break;			// �����o��

	default:

		// �ړ��ʂ�����
		pHuman->move.x += (0.0f - pHuman->move.x) * REV_HUMAN_MOVE_SUB;

		// �������X�V
		pHuman->rot.y += 0.05f * (pHuman->move.x * 0.1f);

		switch (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve])
		{
		case DASH_RIGHT:	// �E�ɐi��ł���ꍇ

			//�ʒu��␳����
			pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;

			// �ڕW�̌�����ݒ肷��
			pHuman->curveInfo.rotDest.y = D3DX_PI;

			break;			// �����o��

		case DASH_LEFT:		// ���ɐi��ł���ꍇ

			// �ʒu��␳����
			pHuman->pos.x = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x;

			// �ڕW�̌�����ݒ肷��
			pHuman->curveInfo.rotDest.y = 0.0f;

			break;			// �����o��

		case DASH_FAR:		// ���ɐi��ł���ꍇ

			// �ʒu��␳����
			pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;

			// �ڕW�̌�����ݒ肷��
			pHuman->curveInfo.rotDest.y = D3DX_PI * 0.5f;

			break;			// �����o��

		case DASH_NEAR:		// ��O�ɐi��ł���ꍇ

			// �ʒu��␳����
			pHuman->pos.z = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z;

			// �ڕW�̌�����ݒ肷��
			pHuman->curveInfo.rotDest.y = -D3DX_PI * 0.5f;

			break;			// �����o��
		}

		if (pHuman->rot.y >= pHuman->curveInfo.rotDest.y)
		{ // �������ڕW�l�𒴂����ꍇ

		  // ������␳����
			pHuman->rot.y = pHuman->curveInfo.rotDest.y;

			// ���s��Ԃɂ���
			pHuman->curveInfo.actionState = HUMANACT_WALK;

			// �x�@�̍s���ݒ肷��
			pHuman->curveInfo.curveInfo.nNowCurve = (pHuman->curveInfo.curveInfo.nNowCurve + 1) % pHuman->curveInfo.curveInfo.nCurveTime;

			if (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve] == DASH_NEAR)
			{ // ���ɑ����Ă���ꍇ

				if (pHuman->curveInfo.curveInfo.curveAngle[pHuman->curveInfo.curveInfo.nNowCurve] == CURVE_RIGHT)
				{ // ���̋Ȃ���p���}�C�i�X�����ɋȂ���ꍇ

				  // �������}�C�i�X�ɂ���
					pHuman->rot.y = (float)D3DXToRadian(-180);
				}
			}
		}

		break;			// �����o��
	}

	// �����̐��K��
	RotNormalize(&pHuman->rot.y);
}

//============================================================
// �l�Ԃ̂���Ⴂ����
//============================================================
void PassingHuman(Human *pHuman)
{
	Human *pPassHuman = GetHumanData();		// �l�Ԃ̏��
	float fLength;							// ����
	float posDest;							// �ڕW�̈ʒu
	float posDiff;							// �ʒu�̍���

	for (int nCnt = 0; nCnt < MAX_HUMAN; nCnt++, pPassHuman++)
	{
		if (pPassHuman->bUse == true)
		{ // �l�Ԃ��g�p����Ă����ꍇ
			if (pPassHuman != pHuman)
			{ // ��r����l�Ԃ��������g����Ȃ��ꍇ

				//�����𑪂�
				fLength = (pPassHuman->pos.x - pHuman->pos.x) * (pPassHuman->pos.x - pHuman->pos.x)
					+ (pPassHuman->pos.z - pHuman->pos.z) * (pPassHuman->pos.z - pHuman->pos.z);

				if (fLength <= (HUMAN_RADIUS * HUMAN_RADIUS))
				{ // �I�u�W�F�N�g���������Ă���
					if (pHuman->move.x >= pPassHuman->move.x)
					{ // �ړ��ʂ������ꍇ
						switch (pHuman->curveInfo.curveInfo.dashAngle[pHuman->curveInfo.curveInfo.nNowCurve])
						{
						case DASH_RIGHT:		// �E

							// �ڕW�̈ʒu��ݒ�
							posDest = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z - HUMAN_PASS_SHIFT;

							// �ʒu�̍��������߂�
							posDiff = posDest - pHuman->pos.z;

							// �������炷
							pHuman->pos.z += posDiff * HUMAN_PASS_CORRECT;

							break;				// �����o��

						case DASH_LEFT:			// ��

							// �ڕW�̈ʒu��ݒ�
							posDest = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].z + HUMAN_PASS_SHIFT;

							// �ʒu�̍��������߂�
							posDiff = posDest - pHuman->pos.z;

							// �������炷
							pHuman->pos.z += posDiff * HUMAN_PASS_CORRECT;

							break;				// �����o��

						case DASH_FAR:			// ��

							// �ڕW�̈ʒu��ݒ�
							posDest = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x + HUMAN_PASS_SHIFT;

							// �ʒu�̍��������߂�
							posDiff = posDest - pHuman->pos.x;

							// �������炷
							pHuman->pos.x += posDiff * HUMAN_PASS_CORRECT;

							break;				// �����o��

						case DASH_NEAR:			// ��O

							// �ڕW�̈ʒu��ݒ�
							posDest = pHuman->curveInfo.curveInfo.curvePoint[pHuman->curveInfo.curveInfo.nNowCurve].x - HUMAN_PASS_SHIFT;

							// �ʒu�̍��������߂�
							posDiff = posDest - pHuman->pos.x;

							// �������炷
							pHuman->pos.x += posDiff * HUMAN_PASS_CORRECT;

							break;				// �����o��
						}
					}
				}
				else
				{ // �I�u�W�F�N�g���������Ă���

					// �l�Ԃ̕�������
					WalkHuman(pHuman);
				}
			}
			else
			{ // ��L�ȊO

				// �l�Ԃ̕�������
				WalkHuman(pHuman);
			}
		}
	}
}

//======================================================================================================================
//	�l�ԃ��[�V�����̏���������
//======================================================================================================================
void InitMotionHuman(Human *pHuman, int nType, int nWalk)
{
	//------------------------------------------------------------------------------------------------------------------
	//	�p�[�c���̐ݒ�
	//------------------------------------------------------------------------------------------------------------------
	switch (nType)
	{ // ��ނ��Ƃ̏���
	case HUMANTYPE_001:			// �ʏ�1
	case HUMANTYPE_002:			// �ʏ�2
	case HUMANTYPE_003:			// �ʏ�3

		// �p�[�c�̑�����ݒ�
		pHuman->motion.nParts = g_setMotionNormal.nParts;

		// �����𔲂���
		break;

	case HUMANTYPE_CIGARETTE:	// �^�o�R

		// �p�[�c�̑�����ݒ�
		pHuman->motion.nParts = g_setMotionCigaret.nParts;

		// �����𔲂���
		break;

	case HUMANTYPE_SMARTPHONE:	// �X�}�z

		// �p�[�c�̑�����ݒ�
		pHuman->motion.nParts = g_setMotionPhone.nParts;

		// �����𔲂���
		break;

	default:	// ��L�ȊO

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�l�Ԃ̃p�[�c���~�X���Ă邼�I", "�x���I", MB_ICONWARNING);

		// �����𔲂���
		break;
	}

	//------------------------------------------------------------------------------------------------------------------
	//	���[�V�������̐ݒ�
	//------------------------------------------------------------------------------------------------------------------
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{ // ���[�V�����̍ő吔���J��Ԃ�

		switch (nType)
		{ // ��ނ��Ƃ̏���
		case HUMANTYPE_001:			// �ʏ�1
		case HUMANTYPE_002:			// �ʏ�2
		case HUMANTYPE_003:			// �ʏ�3

			// ���[�V�����̏���������
			pHuman->motion.aMotion[nCntMotion] = g_setMotionNormal.aMotion[nCntMotion];

			// �����𔲂���
			break;

		case HUMANTYPE_CIGARETTE:	// �^�o�R

			// ���[�V�����̏���������
			pHuman->motion.aMotion[nCntMotion] = g_setMotionCigaret.aMotion[nCntMotion];

			// �����𔲂���
			break;

		case HUMANTYPE_SMARTPHONE:	// �X�}�z

			// ���[�V�����̏���������
			pHuman->motion.aMotion[nCntMotion] = g_setMotionPhone.aMotion[nCntMotion];

			// �����𔲂���
			break;

		default:	// ��L�ȊO

			// �G���[���b�Z�[�W�{�b�N�X
			MessageBox(NULL, "�l�Ԃ̎�ރ~�X���Ă邼�I", "�x���I", MB_ICONWARNING);

			// �����𔲂���
			break;
		}
	}

	//------------------------------------------------------------------------------------------------------------------
	//	�p�[�c���̐ݒ�
	//------------------------------------------------------------------------------------------------------------------
	for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
	{ // �p�[�c�̑������J��Ԃ�

		switch (nType)
		{ // ��ނ��Ƃ̏���
		case HUMANTYPE_001:			// �ʏ�1
		case HUMANTYPE_002:			// �ʏ�2
		case HUMANTYPE_003:			// �ʏ�3

			// �p�[�c�̊�{����������
			pHuman->aParts[nCntParts] = g_aSetPartsNormal[nCntParts];

			// �����𔲂���
			break;

		case HUMANTYPE_CIGARETTE:	// �^�o�R

			// �p�[�c�̊�{����������
			pHuman->aParts[nCntParts] = g_aSetPartsCigaret[nCntParts];

			// �����𔲂���
			break;

		case HUMANTYPE_SMARTPHONE:	// �X�}�z

			// �p�[�c�̊�{����������
			pHuman->aParts[nCntParts] = g_aSetPartsPhone[nCntParts];

			// �����𔲂���
			break;

		default:	// ��L�ȊO

			// �G���[���b�Z�[�W�{�b�N�X
			MessageBox(NULL, "�l�Ԃ̃p�[�c���~�X���Ă邼�I", "�x���I", MB_ICONWARNING);

			// �����𔲂���
			break;
		}

		if (nCntParts < MAX_PARTS - 1)
		{ // �̃p�[�c���f���̏ꍇ

			// �p�[�c�̃��f������������
			pHuman->aParts[nCntParts].modelData = GetModelData(nCntParts + FROM_HUMAN);
		}
		else
		{ // ������f���̏ꍇ

			switch (nType)
			{ // ��ނ��Ƃ̏���
			case HUMANTYPE_001:			// �ʏ�1
			case HUMANTYPE_002:			// �ʏ�2
			case HUMANTYPE_003:			// �ʏ�3
		
				// �\���̗̂v�f��������
				ZeroMemory(&pHuman->aParts[MAX_PARTS - 1].modelData, sizeof(Model));
		
				// �����𔲂���
				break;
		
			case HUMANTYPE_CIGARETTE:	// �^�o�R
		
				// �p�[�c�̃��f�������^�o�R�ɐݒ�
				pHuman->aParts[MAX_PARTS - 1].modelData = GetModelData(MODELTYPE_ITEM_CIGARET);
		
				// �����𔲂���
				break;
		
			case HUMANTYPE_SMARTPHONE:	// �X�}�z
		
				// �p�[�c�̃��f�������X�}�z�ɐݒ�
				pHuman->aParts[MAX_PARTS - 1].modelData = GetModelData(MODELTYPE_ITEM_PHONE);
		
				// �����𔲂���
				break;
		
			default:	// ��L�ȊO
		
				// �G���[���b�Z�[�W�{�b�N�X
				MessageBox(NULL, "�l�Ԃ̃��f�����~�X���Ă邼�I", "�x���I", MB_ICONWARNING);
		
				// �����𔲂���
				break;
			}
		}
	}

	//------------------------------------------------------------------------------------------------------------------
	//	���[�V�����̐ݒ�
	//------------------------------------------------------------------------------------------------------------------
	switch (nWalk)
	{ // ��ނ��Ƃ̏���
	case MOVETYPE_STOP:	// ��~

		// ���[�V�����̐ݒ�
		SetMotionHuman(pHuman, MOTIONTYPE_STOP);

		// �����𔲂���
		break;

	case MOVETYPE_MOVE:	// �ړ�

		// ���[�V�����̐ݒ�
		SetMotionHuman(pHuman, MOTIONTYPE_MOVE);

		// �����𔲂���
		break;

	default:	// ��L�ȊO

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�l�Ԃ̃��[�V�������~�X���Ă邼�I", "�x���I", MB_ICONWARNING);

		// �����𔲂���
		break;
	}
}

//======================================================================================================================
//	���[�V�����̍X�V����
//======================================================================================================================
void UpdateMotionHuman(Human *pHuman)
{
	// �ϐ���錾
	D3DXVECTOR3 diffPos;		// ���̃|�[�Y�܂ł̍��� (�ʒu)
	D3DXVECTOR3 diffRot;		// ���̃|�[�Y�܂ł̍��� (����)
	D3DXVECTOR3 currentPos;		// ���݂̃t���[���̈ʒu
	D3DXVECTOR3 currentRot;		// ���݂̃t���[���̌���

	MOTIONTYPE  type  = pHuman->motion.type;	// ���[�V�����̎��
	int         nPose = pHuman->motion.nPose;	// ���[�V�����̃|�[�Y�ԍ�

	// �p�[�c�̈ʒu���X�V
	for (int nCntParts = 0; nCntParts < pHuman->motion.nParts; nCntParts++)
	{ // �p�[�c�̑������J��Ԃ�

		// �ʒu�̍��������߂�
		diffPos = pHuman->motion.aMotion[type].aKey[(nPose + 1) % pHuman->motion.aMotion[type].nNumKey].aPos[nCntParts] - pHuman->motion.aMotion[type].aKey[nPose].aPos[nCntParts];

		// �����̍��������߂�
		diffRot = pHuman->motion.aMotion[type].aKey[(nPose + 1) % pHuman->motion.aMotion[type].nNumKey].aRot[nCntParts] - pHuman->motion.aMotion[type].aKey[nPose].aRot[nCntParts];

		// ���������̐��K��
		RotNormalize(&diffRot.x);
		RotNormalize(&diffRot.y);
		RotNormalize(&diffRot.z);

		// ���݂̃t���[���̈ʒu��ݒ�
		currentPos = pHuman->motion.aMotion[type].aKey[nPose].aPos[nCntParts] + diffPos * ((float)pHuman->motion.nCounter / (float)pHuman->motion.aMotion[type].aKey[nPose].nFrame);

		// ���݂̃t���[���̌�����ݒ�
		currentRot = pHuman->motion.aMotion[type].aKey[nPose].aRot[nCntParts] + diffRot * ((float)pHuman->motion.nCounter / (float)pHuman->motion.aMotion[type].aKey[nPose].nFrame);

		// ���������̐��K��
		RotNormalize(&currentRot.x);
		RotNormalize(&currentRot.y);
		RotNormalize(&currentRot.z);

		// ���݂̃p�[�c�̈ʒu���X�V
		pHuman->aParts[nCntParts].pos = pHuman->aParts[nCntParts].initPos + currentPos;

		// ���݂̃p�[�c�̌������X�V
		pHuman->aParts[nCntParts].rot = pHuman->aParts[nCntParts].initRot + currentRot;

		// �����̐��K��
		RotNormalize(&pHuman->aParts[nCntParts].rot.x);
		RotNormalize(&pHuman->aParts[nCntParts].rot.y);
		RotNormalize(&pHuman->aParts[nCntParts].rot.z);
	}

	// ���[�V�����̑J�ڂ̍X�V
	if (pHuman->motion.nCounter >= pHuman->motion.aMotion[type].aKey[nPose].nFrame)
	{ // ���݂̃��[�V�����J�E���^�[�����݂̃|�[�Y�̍Đ��t���[�����𒴂��Ă���ꍇ

		// ���̃|�[�Y�Ɉڍs
		if (pHuman->motion.aMotion[type].bLoop == true)
		{ // ���[�V���������[�v����ݒ�̏ꍇ

			// ���[�V�����J�E���^�[��������
			pHuman->motion.nCounter = 0;

			// �|�[�Y�J�E���g�����Z (�����ɒB�����ꍇ 0�ɖ߂�)
			pHuman->motion.nPose = (pHuman->motion.nPose + 1) % pHuman->motion.aMotion[type].nNumKey;
		}
		else
		{ // ���[�V���������[�v���Ȃ��ݒ�̏ꍇ
#if 0
			if (pHuman->motion.nPose < pHuman->motion.aMotion[type].nNumKey - SUB_MOTION_STOP)
			{ // ���݂̃|�[�Y���ŏI�̃|�[�Y�ł͂Ȃ��ꍇ

				// ���[�V�����J�E���^�[��������
				pHuman->motion.nCounter = 0;

				// �|�[�Y�J�E���g�����Z
				pHuman->motion.nPose++;
			}
			else
			{ // ���݂̃|�[�Y���ŏI�̃|�[�Y�̏ꍇ

				switch (type)
				{ // ���[�V�����̎�ނ��Ƃ̏���
				case MOTIONTYPE_ATTACK:		// �U�����

					// �ҋ@���[�V�����Ɉڍs
					SetMotionHuman(pHuman, MOTIONTYPE_NONE);

					// �����𔲂���
					break;

				case MOTIONTYPE_JUMP:		// �W�����v���

					// ����

					// �����𔲂���
					break;

				case MOTIONTYPE_LANDING:	// ���n���

					// ����

					// �����𔲂���
					break;
				}
			}
#endif
		}
	}
	else
	{ // ���݂̃��[�V�����J�E���^�[�����݂̃|�[�Y�̍Đ��t���[�����𒴂��Ă��Ȃ��ꍇ

		// ���[�V�����J�E���^�[�����Z
		pHuman->motion.nCounter++;
	}
}

//======================================================================================================================
//	���[�V�����̐ݒ菈��
//======================================================================================================================
void SetMotionHuman(Human *pHuman, MOTIONTYPE type)
{
	// ��������
	pHuman->motion.type = type;		// ���[�V�����̎��

	// ���[�V��������������
	pHuman->motion.nPose    = 0;	// ���[�V�����̃|�[�Y�ԍ�
	pHuman->motion.nCounter = 0;	// ���[�V�����̃J�E���^�[
}

//======================================================================================================================
// �������̐ݒ菈��
//======================================================================================================================
void SetResurrection(Human human)
{
	for (int nCnt = 0; nCnt < MAX_HUMAN; nCnt++)
	{
		if (g_aResurrect[nCnt].bUse == false)
		{ // �����̏����g�p���Ă��Ȃ��ꍇ
			
			// �����֌W�̏��̐ݒ�
			g_aResurrect[nCnt].humanDate = human;		// �l�Ԃ̃f�[�^��������
			g_aResurrect[nCnt].nResurCount = 0;			// �����J�E���g
			g_aResurrect[nCnt].bUse = true;				// �g�p��

			// �������甲����
			break;
		}
	}
}

//======================================================================================================================
// �l�Ԃ̕�������
//======================================================================================================================
void ResurrectionHuman(Human human)
{
	for (int nCnt = 0; nCnt < MAX_HUMAN; nCnt++)
	{
		if (g_aHuman[nCnt].bUse == false)
		{ // �l�Ԃ��g�p���Ă��Ȃ��ꍇ

			// �����̈ʒu����
			g_aHuman[nCnt].pos    = human.pos;		// ���݂̈ʒu
			g_aHuman[nCnt].posOld = human.pos;		// �O��̈ʒu	
			g_aHuman[nCnt].type   = human.type;		// ���

			// ����������
			g_aHuman[nCnt].move    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
			g_aHuman[nCnt].bMove   = false;							// �ړ����Ă��Ȃ�
			g_aHuman[nCnt].state   = HUMANSTATE_WALK;				// �������
			g_aHuman[nCnt].nNotaID = NONE_3D_NOTATION;				// �����\���̃C���f�b�N�X

			// �ړ��ʂ̍ő�l��ݒ�
			g_aHuman[nCnt].fMaxMove = (float)(rand() % HUMAN_RANDAM_MOVE + HUMAN_MOVE_LEAST);

			// �g�p���Ă����Ԃɂ���
			g_aHuman[nCnt].bUse = true;

			// �e�̃C���f�b�N�X��ݒ�
			g_aHuman[nCnt].nShadowID = SetCircleShadow
			( // ����
				0.5f,							// ���l
				SHADOW_HUMAN_RADIUS,			// ���a
				&g_aHuman[nCnt].nShadowID,		// �e�̐e�̉e�C���f�b�N�X
				&g_aHuman[nCnt].bUse			// �e�̐e�̎g�p��
			);

			// �e�̈ʒu�ݒ�
			SetPositionShadow
			(
				g_aHuman[nCnt].nShadowID,
				g_aHuman[nCnt].pos,
				g_aHuman[nCnt].rot,
				D3DXVECTOR3(1.0f, 1.0f, 1.0f)
			);

			// ��������
			g_aHuman[nCnt].bRecur = true;

			// �ړ����@��ݒ肷��
			g_aHuman[nCnt].typeMove = MOVETYPE_MOVE;

			switch (g_aHuman[nCnt].typeMove)
			{
			case MOVETYPE_MOVE:			// �ړ�����

				// �Ȃ���p���̐ݒu
				g_aHuman[nCnt].curveInfo.actionState = HUMANACT_WALK;							// ���
				g_aHuman[nCnt].curveInfo.nRandamRoute = rand() % MAX_HUMAN_ROUTE;				// ���[�g�̎��
				g_aHuman[nCnt].curveInfo.curveInfo = GetHumanRoute(g_aHuman[nCnt].curveInfo.nRandamRoute);	// ���[�g

				// �l�Ԃ̏����ʒu�̐ݒ菈��
				CorrectCurveHuman(&g_aHuman[nCnt]);

				g_aHuman[nCnt].curveInfo.rotDest = g_aHuman[nCnt].rot;					// �ڕW�̌���

				// �����𔲂���
				break;

			case MOVETYPE_STOP:			// ��~���

				// ����

				// �����𔲂���
				break;
			}

			// �W���b�W�̏��̐ݒ�
			g_aHuman[nCnt].judge.col = JUDGE_WHITE;			// �s�J�s�J�̐F

			g_aHuman[nCnt].judge.state    = human.judge.state;		// �P��
			g_aHuman[nCnt].judge.ticatica = CHICASTATE_BLACKOUT;	// �`�J�`�J���

			if (g_aHuman[nCnt].judge.state == JUDGESTATE_EVIL)
			{ // ���҂̏ꍇ

				// �A�C�R���̏��̏�����
				g_aHuman[nCnt].icon.nIconID = NONE_ICON;			// �A�C�R���̃C���f�b�N�X
				g_aHuman[nCnt].icon.state   = ICONSTATE_NONE;		// �A�C�R���̏��

				// �A�C�R���̐ݒ菈��
				g_aHuman[nCnt].icon.nIconID = SetIcon
				( // ����
					g_aHuman[nCnt].pos,
					ICONTYPE_EVIL_HUMAN,
					&g_aHuman[nCnt].icon.nIconID,
					&g_aHuman[nCnt].bUse,
					&g_aHuman[nCnt].icon.state
				);

				// �����\���̃C���f�b�N�X��ݒ�
				g_aHuman[nCnt].nNotaID = Set3DNotation
				( // ����
					NOTATIONTYPE_FLY,			// �����\���̎��
					&g_aHuman[nCnt].nNotaID,	// �e�̋����\���C���f�b�N�X
					&g_aHuman[nCnt].bUse		// �e�̎g�p��
				);

				// �����\���̈ʒu�ݒ�
				SetPosition3DNotation
				( // ����
					g_aHuman[nCnt].nNotaID,		// �����\���C���f�b�N�X
					D3DXVECTOR3					// �ʒu
					( // ����
						g_aHuman[nCnt].pos.x,										// x
						g_aHuman[nCnt].pos.y + HUMAN_HEIGHT + NOTA_PLUS_POS_HUMAN,	// y
						g_aHuman[nCnt].pos.z										// z
					)
				);
			}

			// �l�ԃ��[�V�����̏���������
			InitMotionHuman(&g_aHuman[nCnt], g_aHuman[nCnt].type, g_aHuman[nCnt].typeMove);

			// �l�Ԃ̏d�Ȃ�h�~����
			OverlapHuman(&g_aHuman[nCnt]);

			// �����𔲂���
			break;
		}
	}
}

//======================================================================================================================
// �l�Ԃ̏����ʒu�̐ݒ菈��
//======================================================================================================================
void CorrectCurveHuman(Human *human)
{
	int nPosNumber;		// �ŏ��ɖڎw���ԍ�
	int fPosRange;		// �ʒu�͈̔�
	int fPosShift;		// ���炷�ʒu

	// �ŏ��ɖڎw���ԍ��������_���ŏo��
	nPosNumber = rand() % human->curveInfo.curveInfo.nCurveTime;

	// �����ʒu��ݒ肷��
	human->curveInfo.curveInfo.nNowCurve = nPosNumber;

	switch (human->curveInfo.curveInfo.dashAngle[nPosNumber])
	{
	case DASH_RIGHT:	// �E�Ɍ������đ����Ă���

		// �ʒu��␳����
		human->pos.z = human->curveInfo.curveInfo.curvePoint[nPosNumber].z;

		// �ʒu�͈̔͂��Z�o����
		fPosRange = (int)(fabsf(human->curveInfo.curveInfo.curvePoint[nPosNumber].x - human->curveInfo.curveInfo.curvePoint[(nPosNumber + (human->curveInfo.curveInfo.nCurveTime - 1)) % human->curveInfo.curveInfo.nCurveTime].x));

		// ���炷�ʒu��ݒ肷��
		fPosShift = rand() % fPosRange;

		// �ʒu��ݒ肷��
		human->pos.x = human->curveInfo.curveInfo.curvePoint[(nPosNumber + (human->curveInfo.curveInfo.nCurveTime - 1)) % human->curveInfo.curveInfo.nCurveTime].x + fPosShift;

		// ���������̐ݒ�
		human->rot.y = D3DXToRadian(90);

		break;			// �����o��

	case DASH_LEFT:		// ���Ɍ������đ����Ă���

		// �ʒu��␳����
		human->pos.z = human->curveInfo.curveInfo.curvePoint[nPosNumber].z;

		// �ʒu�͈̔͂��Z�o����
		fPosRange = (int)(fabsf(human->curveInfo.curveInfo.curvePoint[nPosNumber].x - human->curveInfo.curveInfo.curvePoint[(nPosNumber + (human->curveInfo.curveInfo.nCurveTime - 1)) % human->curveInfo.curveInfo.nCurveTime].x));

		// ���炷�ʒu��ݒ肷��
		fPosShift = rand() % fPosRange;

		// �ʒu��ݒ肷��
		human->pos.x = human->curveInfo.curveInfo.curvePoint[(nPosNumber + (human->curveInfo.curveInfo.nCurveTime - 1)) % human->curveInfo.curveInfo.nCurveTime].x - fPosShift;

		// ���������̐ݒ�
		human->rot.y = D3DXToRadian(-90);

		break;			// �����o��

	case DASH_FAR:		// ���Ɍ������đ����Ă���

		// �ʒu��␳����
		human->pos.x = human->curveInfo.curveInfo.curvePoint[nPosNumber].x;

		// �ʒu�͈̔͂��Z�o����
		fPosRange = (int)(fabsf(human->curveInfo.curveInfo.curvePoint[nPosNumber].z - human->curveInfo.curveInfo.curvePoint[(nPosNumber + (human->curveInfo.curveInfo.nCurveTime - 1)) % human->curveInfo.curveInfo.nCurveTime].z));

		// ���炷�ʒu��ݒ肷��
		fPosShift = rand() % fPosRange;

		// �ʒu��ݒ肷��
		human->pos.z = human->curveInfo.curveInfo.curvePoint[(nPosNumber + (human->curveInfo.curveInfo.nCurveTime - 1)) % human->curveInfo.curveInfo.nCurveTime].z + fPosShift;

		// ���������̐ݒ�
		human->rot.y = D3DXToRadian(0);

		break;			// �����o��

	case DASH_NEAR:		// ��O�Ɍ������đ����Ă���

		// �ʒu��␳����
		human->pos.x = human->curveInfo.curveInfo.curvePoint[nPosNumber].x;

		// �ʒu�͈̔͂��Z�o����
		fPosRange = (int)(fabsf(human->curveInfo.curveInfo.curvePoint[nPosNumber].z - human->curveInfo.curveInfo.curvePoint[(nPosNumber + (human->curveInfo.curveInfo.nCurveTime - 1)) % human->curveInfo.curveInfo.nCurveTime].z));

		// ���炷�ʒu��ݒ肷��
		fPosShift = rand() % fPosRange;

		// �ʒu��ݒ肷��
		human->pos.z = human->curveInfo.curveInfo.curvePoint[(nPosNumber + (human->curveInfo.curveInfo.nCurveTime - 1)) % human->curveInfo.curveInfo.nCurveTime].z - fPosShift;

		switch (human->curveInfo.curveInfo.curveAngle[nPosNumber])
		{
		case CURVE_RIGHT:		// �E�ɋȂ���

			// ���������̐ݒ�
			human->rot.y = D3DXToRadian(-180);

			break;				// �����o��

		case CURVE_LEFT:		// ���ɋȂ���

			// ���������̐ݒ�
			human->rot.y = D3DXToRadian(180);

			break;				// �����o��
		}

		break;			// �����o��
	}
}

//======================================================================================================================
//	�l�Ԃ̃Z�b�g�A�b�v����
//======================================================================================================================
void TxtSetHuman(char *pTxt, HumanParts *pSetParts, MotionInfo *pSetMotion)
{
	// �ϐ���錾
	int nID        = 0;			// �C���f�b�N�X�̑���p
	int nNowMotion = 0;			// ���݂̃��[�V�����ԍ�
	int nNowPose   = 0;			// ���݂̃|�[�Y�ԍ�
	int nNowKey    = 0;			// ���݂̃L�[�ԍ�
	int nLoop      = 0;			// ���[�v�� ON / OFF �̕ϊ��p
	int nEnd       = 0;			// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;				// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(pTxt, "r");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			//----------------------------------------------------------------------------------------------------------
			//	�L�����N�^�[�̐ݒ�
			//----------------------------------------------------------------------------------------------------------
			if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{ // �ǂݍ��񂾕����� CHARACTERSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_CHARACTERSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NUM_PARTS") == 0)
					{ // �ǂݍ��񂾕����� NUM_PARTS �̏ꍇ
						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &pSetMotion->nParts);	// �p�[�c����ǂݍ���
					}
					else if (strcmp(&aString[0], "PARTSSET") == 0)
					{ // �ǂݍ��񂾕����� PARTSSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_PARTSSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "INDEX") == 0)
							{ // �ǂݍ��񂾕����� INDEX �̏ꍇ
								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nID);			// ���f�� (�p�[�c) �̃C���f�b�N�X��ǂݍ���
							}
							else if (strcmp(&aString[0], "PARENT") == 0)
							{ // �ǂݍ��񂾕����� PARENT �̏ꍇ
								fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &pSetParts[nID].nParentID);	// ���f�� (�p�[�c) �̐e�̃C���f�b�N�X��ǂݍ���
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // �ǂݍ��񂾕����� POS �̏ꍇ
								fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &pSetParts[nID].initPos.x);	// X���W��ǂݍ���
								fscanf(pFile, "%f", &pSetParts[nID].initPos.y);	// Y���W��ǂݍ���
								fscanf(pFile, "%f", &pSetParts[nID].initPos.z);	// Z���W��ǂݍ���

								// ���݂̈ʒu�ɓǂݍ��񂾒l��ݒ�
								pSetParts[nID].pos = pSetParts[nID].initPos;
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // �ǂݍ��񂾕����� ROT �̏ꍇ
								fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &pSetParts[nID].initRot.x);	// X������ǂݍ���
								fscanf(pFile, "%f", &pSetParts[nID].initRot.y);	// Y������ǂݍ���
								fscanf(pFile, "%f", &pSetParts[nID].initRot.z);	// Z������ǂݍ���

								// ���݂̌����ɓǂݍ��񂾒l��ݒ�
								pSetParts[nID].rot = pSetParts[nID].initRot;
							}
						} while (strcmp(&aString[0], "END_PARTSSET") != 0);		// �ǂݍ��񂾕����� END_PARTSSET �ł͂Ȃ��ꍇ���[�v
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);			// �ǂݍ��񂾕����� END_CHARACTERSET �ł͂Ȃ��ꍇ���[�v
			}
			//----------------------------------------------------------------------------------------------------------
			//	���[�V�����̐ݒ�
			//----------------------------------------------------------------------------------------------------------
			else if (strcmp(&aString[0], "MOTIONSET") == 0)
			{ // �ǂݍ��񂾕����� MOTIONSET �̏ꍇ

				// ���݂̃|�[�Y�ԍ���������
				nNowPose = 0;

				do
				{ // �ǂݍ��񂾕����� END_MOTIONSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "LOOP") == 0)
					{ // �ǂݍ��񂾕����� LOOP �̏ꍇ
						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nLoop);		// ���[�v�� ON / OFF ��ǂݍ���

						// �ǂݍ��񂾒l�� 0�Ȃ� true�A0�ȊO�Ȃ� false����
						pSetMotion->aMotion[nNowMotion].bLoop = (nLoop == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "NUM_KEY") == 0)
					{ // �ǂݍ��񂾕����� NUM_KEY �̏ꍇ
						fscanf(pFile, "%s", &aString[0]);								// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &pSetMotion->aMotion[nNowMotion].nNumKey);	// �L�[�̑�����ǂݍ���
					}
					else if (strcmp(&aString[0], "KEYSET") == 0)
					{ // �ǂݍ��񂾕����� KEYSET �̏ꍇ

						// ���݂̃L�[�ԍ���������
						nNowKey = 0;

						do
						{ // �ǂݍ��񂾕����� END_KEYSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "FRAME") == 0)
							{ // �ǂݍ��񂾕����� FRAME �̏ꍇ
								fscanf(pFile, "%s", &aString[0]);												// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &pSetMotion->aMotion[nNowMotion].aKey[nNowPose].nFrame);	// �L�[���؂�ւ��܂ł̃t���[������ǂݍ���
							}
							else if (strcmp(&aString[0], "KEY") == 0)
							{ // �ǂݍ��񂾕����� KEY �̏ꍇ

								do
								{ // �ǂݍ��񂾕����� END_KEY �ł͂Ȃ��ꍇ���[�v

									// �t�@�C�����當�����ǂݍ���
									fscanf(pFile, "%s", &aString[0]);
									if (strcmp(&aString[0], "POS") == 0)
									{ // �ǂݍ��񂾕����� POS �̏ꍇ
										fscanf(pFile, "%s", &aString[0]);														// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &pSetMotion->aMotion[nNowMotion].aKey[nNowPose].aPos[nNowKey].x);	// �ʒu (x) ��ǂݍ���
										fscanf(pFile, "%f", &pSetMotion->aMotion[nNowMotion].aKey[nNowPose].aPos[nNowKey].y);	// �ʒu (y) ��ǂݍ���
										fscanf(pFile, "%f", &pSetMotion->aMotion[nNowMotion].aKey[nNowPose].aPos[nNowKey].z);	// �ʒu (z) ��ǂݍ���
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // �ǂݍ��񂾕����� ROT �̏ꍇ
										fscanf(pFile, "%s", &aString[0]);														// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &pSetMotion->aMotion[nNowMotion].aKey[nNowPose].aRot[nNowKey].x);	// ���� (x) ��ǂݍ���
										fscanf(pFile, "%f", &pSetMotion->aMotion[nNowMotion].aKey[nNowPose].aRot[nNowKey].y);	// ���� (y) ��ǂݍ���
										fscanf(pFile, "%f", &pSetMotion->aMotion[nNowMotion].aKey[nNowPose].aRot[nNowKey].z);	// ���� (z) ��ǂݍ���
									}
								} while (strcmp(&aString[0], "END_KEY") != 0);	// �ǂݍ��񂾕����� END_KEY �ł͂Ȃ��ꍇ���[�v

								// ���݂̃L�[�ԍ������Z
								nNowKey++;
							}
						} while (strcmp(&aString[0], "END_KEYSET") != 0);		// �ǂݍ��񂾕����� END_KEYSET �ł͂Ȃ��ꍇ���[�v

						// ���݂̃|�[�Y�ԍ������Z
						nNowPose++;
					}
				} while (strcmp(&aString[0], "END_MOTIONSET") != 0);			// �ǂݍ��񂾕����� END_MOTIONSET �ł͂Ȃ��ꍇ���[�v

				// ���݂̃��[�V�����ԍ������Z
				nNowMotion++;
			}
		} while (nEnd != EOF);													// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�l�Ԃ̃Z�b�g�A�b�v�t�@�C���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}

//======================================================================================================================
// �l�Ԃ̏d�Ȃ�h�~����
//======================================================================================================================
bool OverlapHuman(Human *pHuman)
{
	bool bOverlap = false;			// �d�Ȃ������ǂ���

	for (int nCnt = 0; nCnt < MAX_HUMAN; nCnt++)
	{
		if (g_aHuman[nCnt].bUse == true && &g_aHuman[nCnt] != pHuman)
		{ // �l�Ԃ��g���Ă����ꍇ

			if (g_aHuman[nCnt].pos == pHuman->pos && g_aHuman[nCnt].fMaxMove == g_aHuman[nCnt].fMaxMove && g_aHuman[nCnt].bMove == true)
			{ // �ʒu����v���A�ړ��ʂ̍ő吔����v���Ă����ꍇ

				// �l�Ԃ̏d�Ȃ�h�~�J�E���^�[�����Z����
				pHuman->nOverlapCounter++;

				// �d�Ȃ�h�~�����ʂ���
				bOverlap = true;

				if (pHuman->nOverlapCounter >= HUMAN_OVERLAP_COUNT)
				{ // �d�Ȃ�h�~�̃J�E���^�[����萔�𒴂����ꍇ

					// �ړ��ʂ�0,0f�ɂ���
					pHuman->move.x = 0.0f;

					// �ړ������Ȃ�
					pHuman->bMove = false;

					// ��~�J�E���g�����Z����
					pHuman->nStopCount++;
				}
			}
		}
	}

	// �d�Ȃ������ǂ�����Ԃ�
	return bOverlap;
}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================
#endif