//===========================================
//
// �Č��z�^�C�}�[�̃��C������[buildtimer.cpp]
// Author ��������
//
//===========================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "buildtimer.h"
#include "object.h"
#include "player.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define BUILDTIMER_TEXTURE		"data/TEXTURE/BuildTimerUI.png"		// �Č��z�^�C�}�[�̃e�N�X�`��
#define BUILDTIMER_RADIUS_X		(200.0f)				// �Č��z�^�C�}�[�̔��a(X��)
#define BUILDTIMER_RADIUS_Y		(80.0f)					// �Č��z�^�C�}�[�̔��a(Y��)
#define BUILDTIMER_POS_Y_SHIFT	(100.0f)				// �Č��z�^�C�}�[�̂��炷����(Y��)

#define MAX_VAL_BUILD			(2)						// �Č��z�^�C�}�[�̕\�����l��
#define MAX_BUILD_MIN			(60)					// �^�C�� (��) �̍ő�l
#define MAX_BUILD_SEC			(60)					// �^�C�� (�b) �̍ő�l
#define BUILD_WAIT_TIME			(40)					// �ҋ@��Ԃ̃J�E���g��
#define BUILD_APPEAR_MAGNI		(0.05f)					// �o�����̊g�嗦

//**********************************************************************************************************************
//	�\���̐錾(Build)
//**********************************************************************************************************************
typedef struct
{
	Object		object;		// �I�u�W�F�N�g�̃|�C���^
	BUILDSTATE	state;		// ���
	int			nCount;		// �J�E���g
	bool		bUse;		// �g�p��
}Build;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void CollisionReBuild(Build *pBuild);			// �Č��ݎ��̓����蔻��

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
Build g_aBuild[MAX_BUILDTIMER];					// �Č��z�^�C�}�[�̏��

//==========================================
// �Č��z�^�C�}�[�̏���������
//==========================================
void InitBuildtimer(void)
{
	for (int nCntSet = 0; nCntSet < MAX_BUILDTIMER; nCntSet++)
	{
		// ���̏�����
		g_aBuild[nCntSet].state = BUILDSTATE_COUNT;					// ���
		g_aBuild[nCntSet].nCount = 0;								// �J�E���^�[
		g_aBuild[nCntSet].bUse	 = false;							// �g�p��
	}
}

//========================================
// �Č��z�^�C�}�[�̏I������
//========================================
void UninitBuildtimer(void)
{

}

//========================================
// �Č��z�^�C�}�[�̍X�V����
//========================================
void UpdateBuildtimer(void)
{
	for (int nCntBill = 0; nCntBill < MAX_BUILDTIMER; nCntBill++)
	{
		if (g_aBuild[nCntBill].bUse == true)
		{//�g�p����Ă����ꍇ

			switch (g_aBuild[nCntBill].state)
			{
			case BUILDSTATE_COUNT:		// �J�E���g���

				// �J�E���g�����Z����
				g_aBuild[nCntBill].nCount--;

				if (g_aBuild[nCntBill].nCount <= 0)
				{ // �J�E���g��0�𒴂����ꍇ

					// �J�E���g��0�ɂ���
					g_aBuild[nCntBill].nCount = 0;

					// �Č��ݎ��̓����蔻��
					CollisionReBuild(&g_aBuild[nCntBill]);
				}

				break;					// �����o��

			case BUILDSTATE_DISAPPEAR:	// ���ŏ��

				// �I�u�W�F�N�g�̐ݒ菈��
				SetObject
				(
					g_aBuild[nCntBill].object.pos,					// �ʒu
					g_aBuild[nCntBill].object.rot,					// ����
					g_aBuild[nCntBill].object.scale,				// �g�嗦
					&g_aBuild[nCntBill].object.matCopy[0],			// �}�e���A���f�[�^
					g_aBuild[nCntBill].object.nType,				// ���
					g_aBuild[nCntBill].object.nBreakType,			// �����̎��
					g_aBuild[nCntBill].object.nShadowType,			// �e�̎��
					g_aBuild[nCntBill].object.nCollisionType,		// �����蔻��̎��
					g_aBuild[nCntBill].object.collInfo.stateRot,	// �����蔻��̌����̏��
					APPEARSTATE_SLOWLY,								// ���X�ɏo��
					g_aBuild[nCntBill].object.judge.state			// �P���̏��
				);

				// �g�p���Ȃ�
				g_aBuild[nCntBill].bUse = false;

				break;					// �����o��

			case BUILDSTATE_WAIT:		// �ҋ@���

				// �Č��ݎ��̓����蔻��
				CollisionReBuild(&g_aBuild[nCntBill]);

				break;
			}
		}
	}
}

//=====================================
// �Č��z�^�C�}�[�̕`�揈��
//=====================================
void DrawBuildtimer(void)
{
	
}

//======================================
// �Č��z�^�C�}�[�̐ݒ菈��
//======================================
void SetBuildtimer(D3DXVECTOR3 pos, int nCount, Object object)
{
	for (int nCntBill = 0; nCntBill < MAX_BUILDTIMER; nCntBill++)
	{
		if (g_aBuild[nCntBill].bUse == false)
		{//�g�p����Ă��Ȃ������ꍇ

			// ���̐ݒ�
			g_aBuild[nCntBill].object = object;					// �I�u�W�F�N�g�̏��
			g_aBuild[nCntBill].nCount = nCount;					// �J�E���g
			g_aBuild[nCntBill].state = BUILDSTATE_COUNT;		// ���

			//�g�p����
			g_aBuild[nCntBill].bUse = true;

			break;									//�����o��
		}
	}
}

//======================================
// �Č��ݎ��̓����蔻��
//======================================
void CollisionReBuild(Build *pBuild)
{
	Player *pPlayer = GetPlayer();

	float fLength;										// �����̕ϐ�

	// �����𑪂�
	fLength = (pBuild->object.pos.x - pPlayer->pos.x) * (pBuild->object.pos.x - pPlayer->pos.x)
		+ (pBuild->object.pos.z - pPlayer->pos.z) * (pBuild->object.pos.z - pPlayer->pos.z);

	if (fLength <= (pPlayer->modelData.fRadius * pPlayer->modelData.fRadius) + ((pBuild->object.modelData.fRadius * pBuild->object.scale.x) * (pBuild->object.modelData.fRadius * pBuild->object.scale.x)))
	{ // �͈͓��Ƀv���C���[������ꍇ

		// �ҋ@��Ԃɂ���
		pBuild->state = BUILDSTATE_WAIT;
	}
	else
	{ // ��L�ȊO

		// �Č��z��Ԃɂ���
		pBuild->state = BUILDSTATE_DISAPPEAR;
	}
}

//======================================
// �Č��z�^�C�}�[�̑����擾����
//======================================
int GetNumBuildTimer(void)
{
	int nNumBuild = 0;

	for (int nCnt = 0; nCnt < MAX_BUILDTIMER; nCnt++)
	{
		if (g_aBuild[nCnt].bUse == true)
		{ // �g�p���Ă���ꍇ

			// �Č��z�̑��������Z����
			nNumBuild++;
		}
	}

	// �Č��z�^�C�}�[�̑�����Ԃ�
	return nNumBuild;
}