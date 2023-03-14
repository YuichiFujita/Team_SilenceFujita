//======================================================================================================================
//
//	�x�@�̏��� [Police.cpp]
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
 
#include "Car.h"
#include "Combo.h"
#include "gate.h"
#include "Police.h"
#include "shadow.h"
#include "sound.h"
#include "player.h"
#include "object.h"
#include "score.h"
#include "meshfield.h"

#ifdef _DEBUG	// �f�o�b�O����
#include "game.h"
#include "EditObject.h"
#endif

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define POLI_LIFE				(50)		// �x�@�̗̑�
#define POLI_GRAVITY			(0.75f)		// �v���C���[�ɂ�����d��
#define POLI_MOVE_FORWARD		(0.1f)		// �v���C���[�O�i���̈ړ���
#define POLI_MOVE_BACKWARD		(0.2f)		// �v���C���[��ގ��̈ړ���
#define POLI_MOVE_ROT			(0.012f)	// �v���C���[�̌����ύX��
#define REV_POLI_MOVE_ROT		(0.085f)	// �ړ��ʂɂ������ύX�ʂ̕␳�W��
#define SUB_POLI_MOVE_VALUE		(15.0f)		// �����ύX���̌������s����ړ���
#define SUB_POLI_MOVE			(0.05f)		// �����ύX���̌�����
#define MAX_POLI_FORWARD		(40.0f)		// �O�i���̍ō����x
#define MAX_POLI_FORWARD_PATROL (15.0f)		// �p�g���[�����̑O�i���̍ō����x
#define MAX_POLI_BACKWARD		(8.0f)		// ��ގ��̍ō����x
#define REV_POLI_MOVE_SUB		(0.04f)		// �ړ��ʂ̌����W��
#define POLI_BACK_ALPHA			(0.01f)		// �߂�Ƃ��̓����x�̕ω���
#define POLI_CHASE_ROT_MAGNI	(0.1f)		// �ǐՏ�Ԃ̎��̐���̔{��

#define POLICAR_TRAFFIC_CNT			(400)	// �a�؂��N�����Ƃ��ɉ��P����p�̃J�E���g
#define POLICAR_TRAFFIC_IMPROVE_CNT	(540)	// �a�؏�Ԃ̉����̃J�E���g
#define POLICAR_TRAFFIC_ALPHA		(0.5f)	// �a�؎��̓����x

#define POLICAR_CHASE_RANGE		(2000.0f)	// �ǐՏ�ԂɂȂ�͈�
#define POLICAR_PATBACK_RANGE	(4000.0f)	// �ǐՂ���߂�͈�
#define POLICAR_TACKLE_RANGE	(1000.0f)	// �^�b�N����ԂɂȂ�͈�
#define POLICAR_SPAWN_RANGE		(3000.0f)	// �X�|�[�����Ȃ��Q�[�g����͈̔�

#define ADDPOLICE_COUNT			(4)			// �x�@��������J�E���g

#define NONE_NEAREST			(-1)		// �Ŋ��̏����l

//**********************************************************************************************************************
//	�^�b�N���֌W�̃}�N����`
//**********************************************************************************************************************
#define POLICAR_TACKLE_ADD		(3.35f)		// �����Ă����ړ���
#define MAX_POLICAR_TACKLE_MOVE	(55.0f)		// �ǉ��ړ��ʂ̍ő吔
#define FINISH_POLICAR_TACKLE	(80)		// �^�b�N���I������܂ł̎���
#define STOP_POLICAR_CNT		(25)		// �~�܂��Ă���Ԃ̃J�E���g��
#define STOP_POLICAR_ROT		(0.05f)		// �Ԃ��~�܂��Ă���Ԃ̌����̑����W��
#define BOOST_POLICAR_ROT		(0.035f)		// �u�[�X�g���̌����̑����W��
#define POLICAR_ATTEN_STOP		(0.1f)		// �ǉ��ړ��ʂ̌����W��

//**********************************************************************************************************************
//	���ʊ֌W�̃}�N����`
//**********************************************************************************************************************
#define POLICE_SOUND_VOLUME_MAX	(1.0f)		// �ő�l�̉���
#define POLICE_SOUND_RADIUS		(3000.0f)	// ���a�����m���J�n���锻��

//**********************************************************************************************************************
//	�`���[�g���A���ł̃}�N����`
//**********************************************************************************************************************
#define POLICE_TUTORIAL_DEST	(D3DXVECTOR3(0.0f,0.0f,0.0f))		// �x�@�̃`���[�g���A���ł̖ړI�n
#define POLICE_TUTORIAL_MOVE	(1.0f)								// �x�@�̃`���[�g���A���ł̈ړ���
#define POLICE_TUTORIAL_ALPHA_MOVE	(0.05f)							// �x�@�̃`���[�g���A���ł̓����x�̈ړ���

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void PosPolice(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove);	// �v���C���[�̈ʒu�̍X�V����
void RevPolice(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos,D3DXVECTOR3 *move);				// �v���C���[�̕␳�̍X�V����
void PatrolPoliceAct(Police *pPolice);					// �x�@�̃p�g���[���s������
void ChasePoliceAct(Police *pPolice);					// �x�@�̒ǐՏ���
void PatrolBackAct(Police *pPolice);					// �p�g���[���ɖ߂鏈��
void CurvePolice(Police *pPolice);						// �x�@�̃J�[�u����
void DashPoliceAction(Police *pPolice);					// �x�@�̑��s����
void SetPolicePosRot(Police *pPolice);					// �x�@�̈ʒu�ƌ����̐ݒ菈��
void PolicePosRotCorrect(Police *pPolice);				// �x�@�̈ʒu�̕␳����
void PoliceTackle(Police *pPolice);						// �x�@�̃^�b�N������
void PoliceTrafficImprove(Police *pPolice);				// �x�@�̏a�؉��P����
void PoliceGatePos(Police *pPolice);					// �x�@�̏o���Q�[�g���菈��
void PoliceSpawn(Police *pPolice);						// �x�@�̏o������
void PoliceCurveCheck(Police *pPolice);					// �x�@�̋Ȃ���p�`�F�b�N����
void PoliceWaitCheck(Police *pPolice);					// �x�@�̑ҋ@��ԃ`�F�b�N����

void PoliceSoundVolume(D3DXVECTOR3 *pPos);				// �v���C���[�Ƃ̋����ŉ��ʂ𒲐�

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
Police g_aPolice[MAX_POLICE];	// �I�u�W�F�N�g�̏��
Reinforce g_AddPolice;			// �x�@�̑����̏��

// �`���[�g���A���p
TutorialPolice g_TutorialPolice;	// �x�@(�`���[�g���A����)

//======================================================================================================================
//	�x�@�̏���������
//======================================================================================================================
void InitPolice(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// �I�u�W�F�N�g�̏��̏�����
	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		// ��{����������
		g_aPolice[nCntPolice].pos		  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aPolice[nCntPolice].posOld	  = g_aPolice[nCntPolice].pos;		// �O��̈ʒu
		g_aPolice[nCntPolice].move		  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
		g_aPolice[nCntPolice].rot		  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aPolice[nCntPolice].rotDest	  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW�̌���
		g_aPolice[nCntPolice].state		  = POLICESTATE_PATROL;				// �x�@�̏��
		g_aPolice[nCntPolice].bombState   = BOMBSTATE_NONE;					// �{���̏��
		g_aPolice[nCntPolice].nLife		  = 0;								// �̗�
		g_aPolice[nCntPolice].nShadowID   = NONE_SHADOW;					// �e�̃C���f�b�N�X
		g_aPolice[nCntPolice].bJump		  = false;							// �W�����v���Ă��Ȃ�
		g_aPolice[nCntPolice].nTrafficCnt = 0;								// �a�؃J�E���g
		g_aPolice[nCntPolice].fAlpha	  = 1.0f;							// �����x
		g_aPolice[nCntPolice].nNumSpawnGate = NONE_NEAREST;					// �o�Ă���Q�[�g�̔ԍ�
		g_aPolice[nCntPolice].bUse		  = false;							// �g�p��

		// �Ȃ���p�֌W��������
		g_aPolice[nCntPolice].policeCurve.curveInfo.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �Ȃ���p�̈ʒu
		g_aPolice[nCntPolice].policeCurve.curveInfo.nCurveNumber = -1;						// �ԍ�
		g_aPolice[nCntPolice].policeCurve.curveInfo.curveAngle = CURVE_RIGHT;				// �E�ɋȂ���
		g_aPolice[nCntPolice].policeCurve.curveInfo.dashAngle = DASH_RIGHT;					// �E�ɐi��
		g_aPolice[nCntPolice].policeCurve.curveInfo.bDeadEnd = false;						// �s���~�܂肩�ǂ���
		g_aPolice[nCntPolice].policeCurve.actionState = CARACT_DASH;						// ���
		g_aPolice[nCntPolice].policeCurve.nSKipCnt = 0;										// �X�L�b�v�����
		g_aPolice[nCntPolice].policeCurve.rotDest = g_aPolice[nCntPolice].rot;				// �ڕW�̌���

		g_aPolice[nCntPolice].policeCurveCopy = g_aPolice[nCntPolice].policeCurve;			// �Ȃ���p�̏�����

		// ���f�����̏�����
		g_aPolice[nCntPolice].modelData.dwNumMat = 0;						// �}�e���A���̐�
		g_aPolice[nCntPolice].modelData.pTexture = NULL;					// �e�N�X�`���ւ̃|�C���^
		g_aPolice[nCntPolice].modelData.pMesh    = NULL;					// ���b�V�� (���_���) �ւ̃|�C���^
		g_aPolice[nCntPolice].modelData.pBuffMat = NULL;					// �}�e���A���ւ̃|�C���^
		g_aPolice[nCntPolice].modelData.dwNumMat = 0;						// �}�e���A���̐�
		g_aPolice[nCntPolice].modelData.vtxMin   = INIT_VTX_MIN;			// �ŏ��̒��_���W
		g_aPolice[nCntPolice].modelData.vtxMax   = INIT_VTX_MAX;			// �ő�̒��_���W
		g_aPolice[nCntPolice].modelData.size     = INIT_SIZE;				// �傫��
		g_aPolice[nCntPolice].modelData.fRadius  = 0.0f;					// ���a

		// �^�b�N���֌W�̏��̏�����
		g_aPolice[nCntPolice].tackle.nTackleCnt = 0;						// �^�b�N���̃J�E���g
		g_aPolice[nCntPolice].tackle.tacklemove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �^�b�N�����̒ǉ��ړ���
		g_aPolice[nCntPolice].tackle.tackleState = TACKLESTATE_CHARGE;		// �^�b�N���̏��

		// �A�C�R���̏��̏�����
		g_aPolice[nCntPolice].icon.nIconID = NONE_ICON;						// �A�C�R���̃C���f�b�N�X
		g_aPolice[nCntPolice].icon.state = ICONSTATE_NONE;					// �A�C�R���̏��

		// �`���[�g���A���̌x�@
		g_TutorialPolice.state = POLITUTOSTATE_DRIVE;						// ���
		g_TutorialPolice.fAlpha = 1.0f;									// �����x
	}

	// ���R���̏�����
	g_AddPolice.nBonus = 0;													// ���_����������
}

//======================================================================================================================
//	�x�@�̏I������
//======================================================================================================================
void UninitPolice(void)
{

}

//======================================================================================================================
//	�x�@�̍X�V����
//======================================================================================================================
void UpdatePolice(void)
{
	// �ϐ���錾
	float fPoliceRot = 0.0f;	// �e�̌����ݒ�p

	//���ʉ�BGM�̉��ʒ���
	if (GetSoundType(SOUND_TYPE_SUB_BGM) == true)
	{
		//�x�@�̌��ʉ��T�E���h�̉��ʂ�0�ɕύX
		SetSoundVolume(SOUND_LABEL_BGM_POLICE_000, 0.0f);
	}

	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aPolice[nCntPolice].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			if (g_aPolice[nCntPolice].state == POLICESTATE_SPAWN)
			{ // �o����Ԃ̏ꍇ

				if (g_aPolice[nCntPolice].bombState != BOMBSTATE_BAR_IN)
				{ // �o���A����Ԃł͂Ȃ��ꍇ

					// �x�@�̏o������
					PoliceSpawn(&g_aPolice[nCntPolice]);
				}

				if (GetBarrierState(&g_aPolice[nCntPolice]) == BARRIERSTATE_NONE ||
					GetBarrierState(&g_aPolice[nCntPolice]) == BARRIERSTATE_FLY ||
					GetBarrierState(&g_aPolice[nCntPolice]) == BARRIERSTATE_HOMING ||
					GetBarrierState(&g_aPolice[nCntPolice]) == BARRIERSTATE_LAND)
				{ // �o���A�Z�b�g��Ԃ���Ȃ������ꍇ

					//----------------------------------------------------
					//	�����蔻��
					//----------------------------------------------------
					// �I�u�W�F�N�g�Ƃ̓����蔻��
					CollisionObject
					( // ����
						&g_aPolice[nCntPolice].pos,			// ���݂̈ʒu
						&g_aPolice[nCntPolice].posOld,		// �O��̈ʒu
						&g_aPolice[nCntPolice].move,		// �ړ���
						POLICAR_WIDTH,						// ����
						POLICAR_DEPTH,						// ���s
						&g_aPolice[nCntPolice].nTrafficCnt,	// �a�؃J�E���g
						BOOSTSTATE_NONE,					// �u�[�X�g�̏��
						&g_aPolice[nCntPolice].state,		// �x�@�̏��
						&g_aPolice[nCntPolice].tackle.nTackleCnt,	// �^�b�N���J�E���g
						&g_aPolice[nCntPolice].tackle.tacklemove.x	// �^�b�N�����̈ړ���
					);

					// �Q�[�g�Ƃ̓����蔻��
					CollisionGate
					( // ����
						&g_aPolice[nCntPolice].pos,			// ���݂̈ʒu
						&g_aPolice[nCntPolice].posOld,		// �O��̈ʒu
						&g_aPolice[nCntPolice].move,		// �ړ���
						POLICAR_WIDTH,						// ����
						POLICAR_DEPTH						// ���s
					);

					// �ԓ��m�̓����蔻��
					CollisionCarBody
					( // ����
						&g_aPolice[nCntPolice].pos,
						&g_aPolice[nCntPolice].posOld,
						g_aPolice[nCntPolice].rot,
						&g_aPolice[nCntPolice].move,
						POLICAR_WIDTH,
						POLICAR_DEPTH,
						COLLOBJECTTYPE_POLICE,
						&g_aPolice[nCntPolice].nTrafficCnt,
						(g_aPolice[nCntPolice].tackle.tackleState)
					);
				}

				if (g_aPolice[nCntPolice].bombState != BOMBSTATE_BAR_IN)
				{ // �o���A����Ԃł͂Ȃ��ꍇ

					if (g_aPolice[nCntPolice].pos.y < 0.0f)
					{//Y���̈ʒu��0.0f�������ꍇ

						//�c�ւ̈ړ��ʂ�0.0f�ɂ���
						g_aPolice[nCntPolice].move.y = 0.0f;

						//�ʒu��0.0f�ɖ߂�
						g_aPolice[nCntPolice].pos.y = 0.0f;
					}

					// �v���C���[�̕␳�̍X�V����
					RevPolice(&g_aPolice[nCntPolice].rot, &g_aPolice[nCntPolice].pos, &g_aPolice[nCntPolice].move);
				}

				//----------------------------------------------------
				//	�A�C�R���̍X�V
				//----------------------------------------------------
				// �A�C�R���̈ʒu�ݒ�
				SetPositionIcon
				( // ����
					g_aPolice[nCntPolice].icon.nIconID,	// �A�C�R���̃C���f�b�N�X
					g_aPolice[nCntPolice].pos			// �ʒu
				);
			}
			else
			{ // ��L�ȊO
				if (g_aPolice[nCntPolice].bombState != BOMBSTATE_BAR_IN)
				{ // �o���A����Ԃł͂Ȃ��ꍇ

					// �O��ʒu�̍X�V
					g_aPolice[nCntPolice].posOld = g_aPolice[nCntPolice].pos;

					//// �v���C���[�̒��n�̍X�V����
					//LandObject(&g_aPolice[nCntPolice].pos, &g_aPolice[nCntPolice].move, &g_aPolice[nCntPolice].bJump);

					// �v���C���[�̈ʒu�̍X�V
					PosPolice(&g_aPolice[nCntPolice].move, &g_aPolice[nCntPolice].pos, &g_aPolice[nCntPolice].rot, g_aPolice[nCntPolice].bMove);

					//�v���C���[�Ƃ̋����ŉ��ʂ𒲐�
					PoliceSoundVolume(&g_aPolice[nCntPolice].pos);

					switch (g_aPolice[nCntPolice].state)
					{//��ԂŔ��f����
					case POLICESTATE_PATROL:		// �p�g���[�����

						// �x�@�̃p�g���[���s������
						PatrolPoliceAct(&g_aPolice[nCntPolice]);

						// �Ԃ̒�~����
						CollisionStopCar
						( // ����
							g_aPolice[nCntPolice].pos,		//�ʒu
							g_aPolice[nCntPolice].rot,		//����
							&g_aPolice[nCntPolice].move,	//�ړ���
							g_aPolice[nCntPolice].modelData.fRadius,	//���a
							COLLOBJECTTYPE_POLICE,			//�Ώۂ̃T�C�Y
							&g_aPolice[nCntPolice].nTrafficCnt
						);

						//�����x�����Z�b�g����
						g_aPolice[nCntPolice].fAlpha = 1.0f;

						break;						// �����o��

					case POLICESTATE_CHASE:			// �ǐՏ���

						// �x�@�̒ǐՏ���
						ChasePoliceAct(&g_aPolice[nCntPolice]);

						break;						// �����o��

					case POLICESTATE_PATBACK:		// �p�g���[���֖߂鏈��			

						// �����x�����Z����
						g_aPolice[nCntPolice].fAlpha -= POLI_BACK_ALPHA;

						if (g_aPolice[nCntPolice].fAlpha <= 0.0f)
						{ // �����x��0.0f�ȉ��ɂȂ����ꍇ

							// �����x��������
							g_aPolice[nCntPolice].fAlpha = 0.0f;

							// �p�g���[���ɖ߂鏈��
							PatrolBackAct(&g_aPolice[nCntPolice]);

							// �ŏ��̈ʒu�ɖ߂�����
							g_aPolice[nCntPolice].state = POLICESTATE_POSBACK;

							// �A�C�R���̏�Ԃ𕜊����ɂ���
							g_aPolice[nCntPolice].icon.state = ICONSTATE_REVIVAL;
						}

						break;						// �����o��

					case POLICESTATE_POSBACK:		// �ŏ��̍��W�ɖ߂鏈��

						// �����x�����Z����
						g_aPolice[nCntPolice].fAlpha += POLI_BACK_ALPHA;

						if (g_aPolice[nCntPolice].fAlpha >= 1.0f)
						{ // �����x��1.0f�ȉ��ɂȂ����ꍇ

							// �����x��������
							g_aPolice[nCntPolice].fAlpha = 1.0f;

							// �ŏ��̈ʒu�ɖ߂�����
							g_aPolice[nCntPolice].state = POLICESTATE_SPAWN;

							// �A�C�R���̏�Ԃ𖳂��ɂ���
							g_aPolice[nCntPolice].icon.state = ICONSTATE_NONE;
						}

						// �ŏ��̈ړ��ʂ����ɖ߂�
						g_aPolice[nCntPolice].move.x = 0.0f;

						break;						// �����o��

					case POLICESTATE_TACKLE:		// �^�b�N�����

						// �x�@�̃^�b�N������
						PoliceTackle(&g_aPolice[nCntPolice]);

						break;						// �����o��

					case POLICESTATE_TRAFFIC:		// �a�؏��

						// �x�@�ԗ��̏a�؉��P����
						PoliceTrafficImprove(&g_aPolice[nCntPolice]);

						break;						// �����o��

					case POLICESTATE_WAIT:			// �ҋ@���

						// �x�@�̑ҋ@��ԃ`�F�b�N����
						PoliceWaitCheck(&g_aPolice[nCntPolice]);

						break;						// �����o��
					}

				}

				if (GetBarrierState(&g_aPolice[nCntPolice]) == BARRIERSTATE_NONE ||
					GetBarrierState(&g_aPolice[nCntPolice]) == BARRIERSTATE_FLY ||
					GetBarrierState(&g_aPolice[nCntPolice]) == BARRIERSTATE_HOMING ||
					GetBarrierState(&g_aPolice[nCntPolice]) == BARRIERSTATE_LAND)
				{ // �o���A�Z�b�g��Ԃ���Ȃ������ꍇ

					if (g_aPolice[nCntPolice].state != POLICESTATE_TRAFFIC)
					{ // �a�؏�Ԃ���Ȃ��ꍇ

						//----------------------------------------------------
						//	�����蔻��
						//----------------------------------------------------
						// �I�u�W�F�N�g�Ƃ̓����蔻��
						CollisionObject
						( // ����
							&g_aPolice[nCntPolice].pos,			// ���݂̈ʒu
							&g_aPolice[nCntPolice].posOld,		// �O��̈ʒu
							&g_aPolice[nCntPolice].move,		// �ړ���
							POLICAR_WIDTH,						// ����
							POLICAR_DEPTH,						// ���s
							&g_aPolice[nCntPolice].nTrafficCnt,	// �a�؃J�E���g
							BOOSTSTATE_NONE,					// �u�[�X�g�̏��
							&g_aPolice[nCntPolice].state,		// �x�@�̏��
							&g_aPolice[nCntPolice].tackle.nTackleCnt,	// �^�b�N���J�E���g
							&g_aPolice[nCntPolice].tackle.tacklemove.x	// �^�b�N�����̈ړ���
						);

						// �Q�[�g�Ƃ̓����蔻��
						CollisionGate
						( // ����
							&g_aPolice[nCntPolice].pos,			// ���݂̈ʒu
							&g_aPolice[nCntPolice].posOld,		// �O��̈ʒu
							&g_aPolice[nCntPolice].move,		// �ړ���
							POLICAR_WIDTH,						// ����
							POLICAR_DEPTH						// ���s
						);
					}

					if (g_aPolice[nCntPolice].state != POLICESTATE_PATBACK && g_aPolice[nCntPolice].state != POLICESTATE_POSBACK && g_aPolice[nCntPolice].state != POLICESTATE_WAIT)
					{ // �p�g���[������߂鏈������Ȃ����A�����l�ɖ߂鎞�ȊO�̏ꍇ

						// �ԓ��m�̓����蔻��
						CollisionCarBody
						( // ����
							&g_aPolice[nCntPolice].pos,
							&g_aPolice[nCntPolice].posOld,
							g_aPolice[nCntPolice].rot,
							&g_aPolice[nCntPolice].move,
							POLICAR_WIDTH,
							POLICAR_DEPTH,
							COLLOBJECTTYPE_POLICE,
							&g_aPolice[nCntPolice].nTrafficCnt,
							(g_aPolice[nCntPolice].tackle.tackleState)
						);
					}

					//----------------------------------------------------
					//	�e�̍X�V
					//----------------------------------------------------
					// �x�@�̌�����ݒ�
					fPoliceRot = g_aPolice[nCntPolice].rot.y + D3DX_PI;
					RotNormalize(&fPoliceRot);	// �����𐳋K��

					// �e�̈ʒu�ݒ�
					SetPositionShadow
					( // ����
						g_aPolice[nCntPolice].nShadowID,													// �e�̃C���f�b�N�X
						g_aPolice[nCntPolice].pos,															// �ʒu
						D3DXVECTOR3(g_aPolice[nCntPolice].rot.x, fPoliceRot, g_aPolice[nCntPolice].rot.z),	// ����
						NONE_SCALE																			// �g�嗦
					);
				}

				if (g_aPolice[nCntPolice].nTrafficCnt >= POLICAR_TRAFFIC_CNT)
				{ // �a�؂Ɋ������܂ꂽ�ꍇ
					if (g_aPolice[nCntPolice].state != POLICESTATE_CHASE
						&& g_aPolice[nCntPolice].state != POLICESTATE_TACKLE)
					{ // �ǐՏ�Ԃ������́A�^�b�N����ԈȊO�̏ꍇ

						// �a�؏�Ԃɂ���
						g_aPolice[nCntPolice].state = POLICESTATE_TRAFFIC;
					}
					else
					{ // ��L�ȊO

						// �a�؃J�E���g��0�ɂ���
						g_aPolice[nCntPolice].nTrafficCnt = 0;
					}
				}

				if (g_aPolice[nCntPolice].bombState != BOMBSTATE_BAR_IN)
				{ // �o���A����Ԃł͂Ȃ��ꍇ

					if (g_aPolice[nCntPolice].pos.y < 0.0f)
					{//Y���̈ʒu��0.0f�������ꍇ

						//�c�ւ̈ړ��ʂ�0.0f�ɂ���
						g_aPolice[nCntPolice].move.y = 0.0f;

						//�ʒu��0.0f�ɖ߂�
						g_aPolice[nCntPolice].pos.y = 0.0f;
					}

					// �v���C���[�̕␳�̍X�V����
					RevPolice(&g_aPolice[nCntPolice].rot, &g_aPolice[nCntPolice].pos, &g_aPolice[nCntPolice].move);
				}

				//----------------------------------------------------
				//	�A�C�R���̍X�V
				//----------------------------------------------------
				// �A�C�R���̈ʒu�ݒ�
				SetPositionIcon
				( // ����
					g_aPolice[nCntPolice].icon.nIconID,	// �A�C�R���̃C���f�b�N�X
					g_aPolice[nCntPolice].pos			// �ʒu
				);
			}
		}
	}
}

//======================================================================================================================
//	�x�@�̕`�揈��
//======================================================================================================================
void DrawPolice(void)
{
	// �ϐ���錾
	float        policeRot;						// �x�@�̌����̌v�Z�p
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	Player           *pPlayer = GetPlayer();	// �v���C���[�̏��
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aPolice[nCntPolice].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aPolice[nCntPolice].mtxWorld);

			// �x�@�̌�����ݒ�
			policeRot = g_aPolice[nCntPolice].rot.y + D3DX_PI;
			RotNormalize(&policeRot);	// �����𐳋K��

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, policeRot, g_aPolice[nCntPolice].rot.x, g_aPolice[nCntPolice].rot.z);
			D3DXMatrixMultiply(&g_aPolice[nCntPolice].mtxWorld, &g_aPolice[nCntPolice].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aPolice[nCntPolice].pos.x, g_aPolice[nCntPolice].pos.y, g_aPolice[nCntPolice].pos.z);
			D3DXMatrixMultiply(&g_aPolice[nCntPolice].mtxWorld, &g_aPolice[nCntPolice].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aPolice[nCntPolice].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aPolice[nCntPolice].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aPolice[nCntPolice].modelData.dwNumMat; nCntMat++)
			{ // �}�e���A���̐����J��Ԃ�

#if 0
				if (pPlayer->bomb.state == ATTACKSTATE_BOMB)
				{ // �U����Ԃ��{���U����Ԃ̏ꍇ

					switch (g_aPolice[nCntPolice].bombState)
					{ // �{���̏��
					case BOMBSTATE_RANGE:	// �͈͓����

						// �͈͓����̃}�e���A���̐F��ݒ�
						g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Diffuse = BOMB_RANGE_COL;

						// �����𔲂���
						break;

					case BOMBSTATE_AIM:		// �_�����

						// �_�����̃}�e���A���̐F��ݒ�
						g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Diffuse = BOMB_AIM_COL;

						// �����𔲂���
						break;

					default:				// ��L�ȊO

						// �͈͊O���̃}�e���A���̐F��ݒ�
						g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Diffuse = BOMB_NONE_COL;

						// �����𔲂���
						break;
					}

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D);
				}
				else
				{ // �U����Ԃ�����ȊO�̏�Ԃ̏ꍇ

					switch (g_aPolice[nCntPolice].state)
					{ // ��Ԃ��Ƃ̏���
					case POLICESTATE_PATBACK:	// �p�g���[���ɖ߂��Ă�����

						// �}�e���A���̐F��������
						g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Diffuse.r = pMat[nCntMat].MatD3D.Diffuse.r;
						g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Diffuse.g = pMat[nCntMat].MatD3D.Diffuse.g;
						g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Diffuse.b = pMat[nCntMat].MatD3D.Diffuse.b;

						// �����x��������
						g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Diffuse.a  = g_aPolice[nCntPolice].fAlpha;
						g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Ambient.a  = g_aPolice[nCntPolice].fAlpha;
						g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Emissive.a = g_aPolice[nCntPolice].fAlpha;

						// �}�e���A���̐ݒ�
						pDevice->SetMaterial(&g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D);

						// �����𔲂���
						break;

					case POLICESTATE_POSBACK:	// �ŏ��̈ʒu�ɖ߂���

						// �}�e���A���̐F��������
						g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Diffuse.r = pMat[nCntMat].MatD3D.Diffuse.r;
						g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Diffuse.g = pMat[nCntMat].MatD3D.Diffuse.g;
						g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Diffuse.b = pMat[nCntMat].MatD3D.Diffuse.b;

						// �����x��������
						g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Diffuse.a  = g_aPolice[nCntPolice].fAlpha;
						g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Ambient.a  = g_aPolice[nCntPolice].fAlpha;
						g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Emissive.a = g_aPolice[nCntPolice].fAlpha;

						// �}�e���A���̐ݒ�
						pDevice->SetMaterial(&g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D);

						// �����𔲂���
						break;

					case POLICESTATE_TRAFFIC:	// �a�؎�

						// �}�e���A���f�[�^��������
						g_aPolice[nCntPolice].MatCopy[nCntMat] = pMat[nCntMat];

						// �Ԃ𔖂�����
						g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Diffuse.a = POLICAR_TRAFFIC_ALPHA;

						// �}�e���A���̐ݒ�
						pDevice->SetMaterial(&g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D);

						break;					// �����o��

					default:					// ��L�ȊO

						// �}�e���A���̐ݒ�
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

						// �����𔲂���
						break;
					}
				}
#else
				switch (g_aPolice[nCntPolice].state)
				{ // ��Ԃ��Ƃ̏���
				case POLICESTATE_PATBACK:	// �p�g���[���ɖ߂��Ă�����

					// �}�e���A���̐F��������
					g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Diffuse.r = pMat[nCntMat].MatD3D.Diffuse.r;
					g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Diffuse.g = pMat[nCntMat].MatD3D.Diffuse.g;
					g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Diffuse.b = pMat[nCntMat].MatD3D.Diffuse.b;

					// �����x��������
					g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Diffuse.a  = g_aPolice[nCntPolice].fAlpha;
					g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Ambient.a  = g_aPolice[nCntPolice].fAlpha;
					g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Emissive.a = g_aPolice[nCntPolice].fAlpha;

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D);

					// �����𔲂���
					break;

				case POLICESTATE_POSBACK:	// �ŏ��̈ʒu�ɖ߂���

					// �}�e���A���̐F��������
					g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Diffuse.r = pMat[nCntMat].MatD3D.Diffuse.r;
					g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Diffuse.g = pMat[nCntMat].MatD3D.Diffuse.g;
					g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Diffuse.b = pMat[nCntMat].MatD3D.Diffuse.b;

					// �����x��������
					g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Diffuse.a  = g_aPolice[nCntPolice].fAlpha;
					g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Ambient.a  = g_aPolice[nCntPolice].fAlpha;
					g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Emissive.a = g_aPolice[nCntPolice].fAlpha;

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D);

					// �����𔲂���
					break;

				case POLICESTATE_TRAFFIC:	// �a�؎�

					// �}�e���A���f�[�^��������
					g_aPolice[nCntPolice].MatCopy[nCntMat] = pMat[nCntMat];

					// �Ԃ𔖂�����
					g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Diffuse.a = POLICAR_TRAFFIC_ALPHA;

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D);

					break;					// �����o��

				default:					// ��L�ȊO

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					// �����𔲂���
					break;
				}

				if (g_aPolice[nCntPolice].bombState == BOMBSTATE_RANGE && GetPlayer()->bomb.state == ATTACKSTATE_NONE)
				{ // �͈͓���Ԃ��A�v���C���[���{�������Ă��Ԃ̏ꍇ

					// �͈͓����̃}�e���A���̐F��ݒ�
					g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Diffuse = POLICAR_BOMB_RANGE_DIF;
					g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D.Emissive = POLICAR_BOMB_RANGE_EMI;

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&g_aPolice[nCntPolice].MatCopy[nCntMat].MatD3D);
				}
#endif

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aPolice[nCntPolice].modelData.pTexture[nCntMat]);

				// ���f���̕`��
				g_aPolice[nCntPolice].modelData.pMesh->DrawSubset(nCntMat);
			}

			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	�x�@�̐ݒ菈��
//======================================================================================================================
void SetPolice(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aPolice[nCntPolice].bUse == false)
		{ // �x�@���g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aPolice[nCntPolice].pos		  = pos;							// ���݂̈ʒu
			g_aPolice[nCntPolice].posOld	  = g_aPolice[nCntPolice].pos;		// �O��̈ʒu

			// �ϐ��̐ݒ�
			g_aPolice[nCntPolice].rotDest	  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW�̌���
			g_aPolice[nCntPolice].move		  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
			g_aPolice[nCntPolice].state		  = POLICESTATE_SPAWN;				// ���
			g_aPolice[nCntPolice].bombState   = BOMBSTATE_NONE;					// �������Ă��Ȃ���Ԃɂ���
			g_aPolice[nCntPolice].nLife		  = POLI_LIFE;						// �̗�
			g_aPolice[nCntPolice].bJump		  = false;							// �W�����v���Ă��Ȃ�
			g_aPolice[nCntPolice].nTrafficCnt = 0;								// �a�؃J�E���g
			g_aPolice[nCntPolice].fAlpha	  = 1.0f;							// �����x
			g_aPolice[nCntPolice].nNumSpawnGate = NONE_NEAREST;					// �o�Ă���Q�[�g�̔ԍ�
			g_aPolice[nCntPolice].bMove		  = false;							// �ړ����Ă��Ȃ�
			g_aPolice[nCntPolice].bUse		  = true;							// �g�p��

			// �A�C�R���̏��̏�����
			g_aPolice[nCntPolice].icon.nIconID = NONE_ICON;						// �A�C�R���̃C���f�b�N�X
			g_aPolice[nCntPolice].icon.state   = ICONSTATE_NONE;				// �A�C�R���̏��

			// ���f������ݒ�
			g_aPolice[nCntPolice].modelData	   = GetModelData(MODELTYPE_CAR_POLICE);	// ���f�����

			// �e�̃C���f�b�N�X��ݒ�
			g_aPolice[nCntPolice].nShadowID    = SetModelShadow
			( // ����
				g_aPolice[nCntPolice].modelData,	// ���f�����
				&g_aPolice[nCntPolice].nShadowID,	// �e�̐e�̉e�C���f�b�N�X
				&g_aPolice[nCntPolice].bUse			// �e�̐e�̎g�p��
			);

			// �e�̈ʒu�ݒ�
			SetPositionShadow(g_aPolice[nCntPolice].nShadowID, g_aPolice[nCntPolice].pos, g_aPolice[nCntPolice].rot, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

			// �A�C�R���̃C���f�b�N�X��ݒ�
			g_aPolice[nCntPolice].icon.nIconID = SetIcon
			( // ����
				g_aPolice[nCntPolice].pos,
				ICONTYPE_POLICE,
				&g_aPolice[nCntPolice].icon.nIconID,
				&g_aPolice[nCntPolice].bUse,
				&g_aPolice[nCntPolice].icon.state
			);

			D3DXMATERIAL *pMat;					//�}�e���A���ւ̃|�C���^

			//�}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aPolice[nCntPolice].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aPolice[nCntPolice].modelData.dwNumMat; nCntMat++)
			{
				//�}�e���A���̏����擾����
				g_aPolice[nCntPolice].MatCopy[nCntMat] = pMat[nCntMat];
			}

			// �Ԃ̈ʒu�ƌ����̐ݒ菈��
			SetPolicePosRot(&g_aPolice[nCntPolice]);
			g_aPolice[nCntPolice].policeCurve.nSKipCnt = 0;										// �X�L�b�v����Ȃ���p�̉�
			g_aPolice[nCntPolice].policeCurve.rotDest = g_aPolice[nCntPolice].rot;				// �O��̌���
			g_aPolice[nCntPolice].policeCurve.actionState = CARACT_DASH;						// �����Ă�����

			// �^�b�N���֌W�̕ϐ��̏�����
			g_aPolice[nCntPolice].tackle.nTackleCnt = 0;			// �^�b�N���̃J�E���g
			g_aPolice[nCntPolice].tackle.tacklemove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �^�b�N�����̒ǉ��ړ���
			g_aPolice[nCntPolice].tackle.tackleState = TACKLESTATE_CHARGE;						// �^�b�N�����̏��

			// �x�@�̏o���Q�[�g���菈��
			PoliceGatePos(&g_aPolice[nCntPolice]);

			// �`���[�g���A���ł̌x�@�̏�����
			g_TutorialPolice.state = POLITUTOSTATE_DRIVE;				// ���
			g_TutorialPolice.fAlpha = 1.0f;								// �����x

			// �����𔲂���
			break;
		}
	}
}

#if 0
//======================================================================================================================
//	�I�u�W�F�N�g�̃_���[�W����
//======================================================================================================================
void HitPolice(Police *pPolice, int nDamage)
{
	if (pPolice->state == ACTIONSTATE_NORMAL)
	{ // �I�u�W�F�N�g���ʏ��Ԃ̏ꍇ

	  // �����̃_���[�W����̗͂��猸�Z
		pPolice->nLife -= nDamage;

		if (pPolice->nLife > 0)
		{ // �̗͂��c���Ă���ꍇ

		  // �_���[�W��Ԃɂ���
			pPolice->state = ACTIONSTATE_DAMAGE;

			// �p�[�e�B�N���̐ݒ�
			SetParticle
			( // ����
				pPolice->pos,						// �ʒu
				D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f),	// �F
				PARTICLETYPE_DAMAGE,				// ���
				SPAWN_PARTICLE_DAMAGE,				// �G�t�F�N�g��
				2									// ����
			);

			// �J�E���^�[��ݒ�
			pPolice->nCounterState = DAMAGE_TIME_OBJ;

			// �T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_DMG);			// SE (�_���[�W)
		}
		else
		{ // �̗͂��s�����ꍇ

		  // �����̐ݒ�
			SetExplosion(pPolice->pos, SOUNDTYPE_BREAK);

			// �p�[�e�B�N���̐ݒ�
			SetParticle
			( // ����
				pPolice->pos,						// �ʒu
				D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f),	// �F
				PARTICLETYPE_EXPLOSION,				// ���
				SPAWN_PARTICLE_EXPLOSION,			// �G�t�F�N�g��
				2									// ����
			);

			// �A�C�e���̐ݒ�
			SetItem(pPolice->pos, ITEMTYPE_HEAL);

			// �g�p���Ă��Ȃ���Ԃɂ���
			pPolice->bUse = false;
		}
	}
}
#endif

//======================================================================================================================
//	�I�u�W�F�N�g�̎擾����
//======================================================================================================================
Police *GetPoliceData(void)
{
	// �I�u�W�F�N�g�̏��̐擪�A�h���X��Ԃ�
	return &g_aPolice[0];
}

//============================================================
//	�v���C���[�̈ʒu�̍X�V����
//============================================================
void PosPolice(D3DXVECTOR3 *move, D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, bool bMove)
{
	//--------------------------------------------------------
	//	�d�͂̍X�V
	//--------------------------------------------------------
	move->y -= POLI_GRAVITY;

	//--------------------------------------------------------
	//	�ړ��ʂ̕␳
	//--------------------------------------------------------
	if (move->x > MAX_POLI_FORWARD)
	{ // �v���C���[�̈ړ��� (x) �����l�ȏ�̏ꍇ

		// �v���C���[�̈ړ��� (x) ��␳
		move->x = MAX_POLI_FORWARD;
	}
	else if (move->x < -MAX_POLI_BACKWARD)
	{ // �v���C���[�̈ړ��� (x) �����l�ȉ��̏ꍇ

		// �v���C���[�̈ړ��� (x) ��␳
		move->x = -MAX_POLI_BACKWARD;
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
void RevPolice(D3DXVECTOR3 *rot, D3DXVECTOR3 *pos, D3DXVECTOR3 *move)
{
	// �����̐��K��
	RotNormalize(&rot->y);

	//--------------------------------------------------------
	//	�ړ��͈͂̕␳
	//--------------------------------------------------------
	if (pos->z > GetLimitStage().fNear - (30.0f * 2))
	{ // �͈͊O�̏ꍇ (��O)

		// ��O�Ɉʒu��␳
		pos->z = GetLimitStage().fNear - (30.0f * 2);

		// �ړ��ʂ��폜
		CollisionSlow(&move->x);
	}
	if (pos->z < GetLimitStage().fFar + (30.0f * 2))
	{ // �͈͊O�̏ꍇ (��)

		// ���Ɉʒu��␳
		pos->z = GetLimitStage().fFar + (30.0f * 2);

		// �ړ��ʂ��폜
		CollisionSlow(&move->x);
	}
	if (pos->x > GetLimitStage().fRight - (30.0f * 2))
	{ // �͈͊O�̏ꍇ (�E)

		// �E�Ɉʒu��␳
		pos->x = GetLimitStage().fRight - (30.0f * 2);

		// �ړ��ʂ��폜
		CollisionSlow(&move->x);
	}
	if (pos->x < GetLimitStage().fLeft + (30.0f * 2))
	{ // �͈͊O�̏ꍇ (��)

		// ���Ɉʒu��␳
		pos->x = GetLimitStage().fLeft + (30.0f * 2);

		// �ړ��ʂ��폜
		CollisionSlow(&move->x);
	}
}

//============================================================
//�x�@�̃p�g���[���s������
//============================================================
void PatrolPoliceAct(Police *pPolice)
{
	Player *pPlayer = GetPlayer();					//�v���C���[�̏����擾����

	// �ړ��ʂ��X�V
	pPolice->move.x += POLI_MOVE_FORWARD;

	float fDist;									//�ڕW�̋���

	// �ړ����Ă����Ԃɂ���
	pPolice->bMove = true;

	// �Ԃ̃J�[�u����
	CurvePolice(pPolice);

	if (pPolice->move.x > MAX_POLI_FORWARD_PATROL)
	{ // �v���C���[�̈ړ��� (x) �����l�ȏ�̏ꍇ

	  // �v���C���[�̈ړ��� (x) ��␳
		pPolice->move.x = MAX_POLI_FORWARD_PATROL;
	}

	if (pPlayer->bUse == true)
	{ // �v���C���[���g�p����Ă����ꍇ

		//�ړI�̋�����ݒ肷��
		fDist = fabsf(sqrtf((pPlayer->pos.x - pPolice->pos.x) * (pPlayer->pos.x - pPolice->pos.x) + (pPlayer->pos.z - pPolice->pos.z) * (pPlayer->pos.z - pPolice->pos.z)));

		if (fDist <= POLICAR_CHASE_RANGE)
		{ // �ړI�̋��������ȓ��ɓ�������

			// �ǐՏ�ԂɈڍs����
			pPolice->state = POLICESTATE_CHASE;
		}
	}
}

//============================================================
//�x�@�ԗ��̒T�m����
//============================================================
void PatrolCarSearch(Police *pPolice)
{
	Player *pPlayer = GetPlayer();					// �v���C���[�̏����擾����

	float fDist, fRotDest, fRotDiff;				// �ڕW�̋���,�p�x

	// �ړI�̋�����ݒ肷��
	fDist = fabsf(sqrtf((pPlayer->pos.x - pPolice->pos.x) * (pPlayer->pos.x - pPolice->pos.x) + (pPlayer->pos.z - pPolice->pos.z) * (pPlayer->pos.z - pPolice->pos.z)));

	// �ړI�̌�����ݒ肷��
	fRotDest = atan2f(pPlayer->pos.x - pPolice->pos.x, pPlayer->pos.z - pPolice->pos.z);

	if (fDist <= POLICAR_PATBACK_RANGE)
	{ // �ړI�̋��������ȓ��ɓ�������

		if (fDist <= POLICAR_TACKLE_RANGE && pPlayer->state != ACTIONSTATE_UNRIVALED)
		{ // �ړI�̋��������ȓ��ɓ��������A���G��ԈȊO�̏ꍇ

			// �^�b�N���̃J�E���g��0�ɂ���
			pPolice->tackle.nTackleCnt = 0;

			// �^�b�N����ԂɈڍs
			pPolice->state = POLICESTATE_TACKLE;

			// �`���[�W��Ԃɂ���
			pPolice->tackle.tackleState = TACKLESTATE_CHARGE;
		}
		else
		{ // �ړI�̋��������ȓ��ɓ���Ȃ������ꍇ

			// �ǐՏ�ԂɈڍs����
			pPolice->state = POLICESTATE_CHASE;

			// �����̍��������߂�
			fRotDiff = fRotDest - pPolice->rot.y;

			// �����̐��K��
			RotNormalize(&fRotDiff);

			// �p�x��␳����
			pPolice->rot.y += fRotDiff * POLI_CHASE_ROT_MAGNI;

			// �����̐��K��
			RotNormalize(&pPolice->rot.y);
		}
	}
	else
	{ // �͈͓��ɓ����Ă��Ȃ��ꍇ
		//����ɖ߂��Ԃɂ���
		pPolice->state = POLICESTATE_PATBACK;

		// �A�C�R���̏�Ԃ��������Ԃɂ���
		pPolice->icon.state = ICONSTATE_DISAPPEAR;
	}
}

//============================================================
//�x�@�̒ǐՏ���
//============================================================
void ChasePoliceAct(Police *pPolice)
{
	// �ړ��ʂ��X�V
	pPolice->move.x += POLI_MOVE_FORWARD;

	// �ړ����Ă����Ԃɂ���
	pPolice->bMove = true;

	//�x�@�ԗ��̒T�m����
	PatrolCarSearch(pPolice);

	if (pPolice->move.x >= MAX_POLI_FORWARD)
	{ // �ړ��ʂ����l�ȏ�̏ꍇ
		// �ړ��ʂ��X�V
		pPolice->move.x = MAX_POLI_FORWARD;
	}
}

//============================================================
// �p�g���[���ɖ߂鏈��
//============================================================
void PatrolBackAct(Police *pPolice)
{
	//�������Z�b�g����
	pPolice->posOld = pPolice->pos;						// �O��̈ʒu
	pPolice->move.x = 0.0f;								// �ړ���
	pPolice->policeCurve = pPolice->policeCurveCopy;	// �Ȃ���p�̏�����
	pPolice->tackle.nTackleCnt = 0;						// �^�b�N���J�E���g
	pPolice->tackle.tacklemove.x = 0.0f;				// �^�b�N�����̈ړ���
	pPolice->tackle.tackleState = TACKLESTATE_CHARGE;	// �^�b�N�����

	// �x�@�̏o���Q�[�g���菈��
	PoliceGatePos(pPolice);
}

//============================================================
// �Ԃ̃J�[�u����
//============================================================
void CurvePolice(Police *pPolice)
{
	switch (pPolice->policeCurve.actionState)
	{
	case CARACT_DASH:		// ���s���

		// �Ԃ̑��s����
		DashPoliceAction(pPolice);

		break;				// �����o��

	case CARACT_CURVE:		// �J�[�u���

		// �Ԃ̊p�x�X�V�E�␳����
		CurveInfoRotCar(&pPolice->policeCurve, &pPolice->rot, &pPolice->move, &pPolice->pos);

		break;				// �����o��
	}
}

//============================================================
// �Ԃ̑��s����
//============================================================
void DashPoliceAction(Police *pPolice)
{
	// �x�@�̋Ȃ���p�`�F�b�N����
	PoliceCurveCheck(pPolice);

	if (pPolice->policeCurve.nSKipCnt == 0)
	{ // �X�L�b�v�J�E���g��0�̏ꍇ
		switch (pPolice->policeCurve.curveInfo.dashAngle)
		{
		case DASH_RIGHT:			//�E�ɑ����Ă���

			//���킹��
			pPolice->pos.z = pPolice->policeCurve.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pPolice->policeCurve.curveInfo.curveAngle == CURVE_RIGHT)
			{ // �E�ɋȂ���ꍇ
				if (pPolice->pos.x >= pPolice->policeCurve.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
					// �ʒu��␳����
					pPolice->pos.x = pPolice->policeCurve.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pPolice->policeCurve.actionState = CARACT_CURVE;
				}
			}
			else if (pPolice->policeCurve.curveInfo.curveAngle == CURVE_LEFT)
			{ // ���ɋȂ���ꍇ
				if (pPolice->pos.x >= pPolice->policeCurve.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
					// �ʒu��␳����
					pPolice->pos.x = pPolice->policeCurve.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pPolice->policeCurve.actionState = CARACT_CURVE;
				}
			}

			break;					//�����o��

		case DASH_LEFT:				//���ɑ����Ă���

			//���킹��
			pPolice->pos.z = pPolice->policeCurve.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pPolice->policeCurve.curveInfo.curveAngle == CURVE_RIGHT)
			{ // �E�ɋȂ���ꍇ
				if (pPolice->pos.x <= pPolice->policeCurve.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
					// �ʒu��␳����
					pPolice->pos.x = pPolice->policeCurve.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pPolice->policeCurve.actionState = CARACT_CURVE;
				}
			}
			else if (pPolice->policeCurve.curveInfo.curveAngle == CURVE_LEFT)
			{ // ���ɋȂ���ꍇ
				if (pPolice->pos.x <= pPolice->policeCurve.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
					// �ʒu��␳����
					pPolice->pos.x = pPolice->policeCurve.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pPolice->policeCurve.actionState = CARACT_CURVE;
				}
			}

			break;					//�����o��

		case DASH_FAR:				//���ɑ����Ă���

									//���킹��
			pPolice->pos.x = pPolice->policeCurve.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pPolice->policeCurve.curveInfo.curveAngle == CURVE_RIGHT)
			{ // �E�ɋȂ���ꍇ
				if (pPolice->pos.z >= pPolice->policeCurve.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
				  // �ʒu��␳����
					pPolice->pos.z = pPolice->policeCurve.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pPolice->policeCurve.actionState = CARACT_CURVE;
				}
			}
			else if (pPolice->policeCurve.curveInfo.curveAngle == CURVE_LEFT)
			{ // ���ɋȂ���ꍇ
				if (pPolice->pos.z >= pPolice->policeCurve.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
				  // �ʒu��␳����
					pPolice->pos.z = pPolice->policeCurve.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pPolice->policeCurve.actionState = CARACT_CURVE;
				}
			}

			break;					//�����o��

		case DASH_NEAR:				//��O�ɑ����Ă���

									//���킹��
			pPolice->pos.x = pPolice->policeCurve.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pPolice->policeCurve.curveInfo.curveAngle == CURVE_RIGHT)
			{ // �E�ɋȂ���ꍇ
				if (pPolice->pos.z <= pPolice->policeCurve.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
				  // �ʒu��␳����
					pPolice->pos.z = pPolice->policeCurve.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pPolice->policeCurve.actionState = CARACT_CURVE;
				}
			}
			else if (pPolice->policeCurve.curveInfo.curveAngle == CURVE_LEFT)
			{ // ���ɋȂ���ꍇ
				if (pPolice->pos.z <= pPolice->policeCurve.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
				{ // �Ԃ̈ʒu���Ȃ���ʒu�ɒB�����ꍇ
				  // �ʒu��␳����
					pPolice->pos.z = pPolice->policeCurve.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

					// �J�[�u��Ԃɂ���
					pPolice->policeCurve.actionState = CARACT_CURVE;
				}
			}

			break;					//�����o��
		}
	}
}

//============================================================
// �Ԃ̈ʒu�ƌ����̐ݒ菈��
//============================================================
void SetPolicePosRot(Police *pPolice)
{
	float fCurveDist;			// �ŋ߂̋Ȃ���p�Ƃ̋���
	int nCurveNumber = 0;		// �ŋ߂̋Ȃ���p�̔ԍ�

	fCurveDist = fabsf(sqrtf((pPolice->pos.x - GetCurveInfo(0).pos.x) * (pPolice->pos.x - GetCurveInfo(0).pos.x) +
		(pPolice->pos.z - GetCurveInfo(0).pos.z) * (pPolice->pos.z - GetCurveInfo(0).pos.z)));

	for (int nCnt = 1; nCnt < MAX_CURVEPOINT; nCnt++)
	{ // �S�Ă̋Ȃ���p�ƎQ��

		float fCurvePoint;		// �Ȃ���p�̒l

		// �J�[�u�̋���
		fCurvePoint = fabsf(sqrtf((pPolice->pos.x - GetCurveInfo(nCnt).pos.x) * (pPolice->pos.x - GetCurveInfo(nCnt).pos.x) +
			(pPolice->pos.z - GetCurveInfo(nCnt).pos.z) * (pPolice->pos.z - GetCurveInfo(nCnt).pos.z)));

		if (fCurvePoint <= fCurveDist)
		{ // �����̋߂����X�V���ꂽ�ꍇ
			// �ŋߒl���X�V����
			fCurveDist = fCurvePoint;

			// �ԍ����X�V����
			nCurveNumber = nCnt;
		}
	}

	// �Ȃ���p�̏���������
	pPolice->policeCurve.curveInfo = GetCurveInfo(nCurveNumber);
	pPolice->policeCurveCopy = pPolice->policeCurve;			// �Ȃ���p�̏��̃R�s�[��ݒ�

	// �Ԃ̈ʒu�̕␳����
	PolicePosRotCorrect(pPolice);
}

//============================================================
// �Ԃ̈ʒu�̕␳����
//============================================================
void PolicePosRotCorrect(Police *pPolice)
{
	switch (pPolice->policeCurve.curveInfo.dashAngle)
	{
	case DASH_RIGHT:		// �E�Ɍ������đ���

		//���킹��
		pPolice->pos.z = pPolice->policeCurve.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

		// ������ς���
		pPolice->rot.y = D3DX_PI * 0.5f;

		break;				// �����o��

	case DASH_LEFT:			// ���Ɍ������đ���

		//���킹��
		pPolice->pos.z = pPolice->policeCurve.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

		// ������ς���
		pPolice->rot.y = -D3DX_PI * 0.5f;

		break;				// �����o��

	case DASH_FAR:			// ���Ɍ������đ���

		//���킹��
		pPolice->pos.x = pPolice->policeCurve.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

		// ������ς���
		pPolice->rot.y = 0.0f;

		break;				// �����o��

	case DASH_NEAR:			// ��O�Ɍ������đ���

		//���킹��
		pPolice->pos.x = pPolice->policeCurve.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

		switch (pPolice->policeCurve.curveInfo.curveAngle)
		{
		case CURVE_RIGHT:	// �E�ɋȂ���

			// ������ς���
			pPolice->rot.y = -D3DX_PI;

			break;			// �����o��

		case CURVE_LEFT:	// ���ɋȂ���

			// ������ς���
			pPolice->rot.y = D3DX_PI;

			break;			// �����o��
		}

		break;				// �����o��
	}
}

//============================================================
// �x�@�̃^�b�N������
//============================================================
void PoliceTackle(Police *pPolice)
{
	// �^�b�N���̃J�E���g�����Z����
	pPolice->tackle.nTackleCnt++;

	Player *pPlayer = GetPlayer();	// �v���C���[�̏��

	float fRotDest, fRotDiff;		// �����̕␳�p�ϐ�

	// �ړI�̌�����ݒ肷��
	fRotDest = atan2f(pPlayer->pos.x - pPolice->pos.x, pPlayer->pos.z - pPolice->pos.z);

	switch (pPolice->tackle.tackleState)
	{
	case TACKLESTATE_CHARGE:	// �`���[�W��

		// �����̍��������߂�
		fRotDiff = fRotDest - pPolice->rot.y;

		// �����̐��K��
		RotNormalize(&fRotDiff);

		// �p�x��␳����
		pPolice->rot.y += fRotDiff * STOP_POLICAR_ROT;

		// �����̐��K��
		RotNormalize(&pPolice->rot.y);

		// �ړ��ʂ����������Ă���
		pPolice->move.x *= POLICAR_ATTEN_STOP;

		if (pPolice->move.x <= 0.0f)
		{ // �ړ��ʂ�0.0f�ȉ��ɂȂ����ꍇ

			// �ړ��ʂ�0�ɐݒ肷��
			pPolice->move.x = 0.0f;
		}

		if (pPolice->tackle.nTackleCnt >= STOP_POLICAR_CNT)
		{ // �^�b�N���̃J�E���g�ɂȂ����ꍇ

			// �^�b�N����Ԃɂ���
			pPolice->tackle.tackleState = TACKLESTATE_BOOST;
		}

		break;					// �����o��

	case TACKLESTATE_BOOST:		// �u�[�X�g��

		// �ǉ��ړ��ʂ����Z���Ă���
		pPolice->tackle.tacklemove.x += POLICAR_TACKLE_ADD;

		// �ړ��ʂ����Z���Ă���
		pPolice->move.x += POLI_MOVE_FORWARD;

		// �����̍��������߂�
		fRotDiff = fRotDest - pPolice->rot.y;

		// �����̐��K��
		RotNormalize(&fRotDiff);

		// �p�x��␳����
		pPolice->rot.y += fRotDiff * BOOST_POLICAR_ROT;

		// �����̐��K��
		RotNormalize(&pPolice->rot.y);

		if (pPolice->tackle.tacklemove.x >= MAX_POLICAR_TACKLE_MOVE)
		{ // �ǉ����̈ړ��ʂ����𒴂����ꍇ

			// �ړ��ʂ�␳����
			pPolice->tackle.tacklemove.x = MAX_POLICAR_TACKLE_MOVE;
		}

		if (pPolice->tackle.nTackleCnt >= FINISH_POLICAR_TACKLE)
		{ // �^�b�N���̃J�E���g����萔�ȏ�ɂȂ����ꍇ

			// �^�b�N���̃J�E���g��0�ɂ���
			pPolice->tackle.nTackleCnt = 0;

			// �ړ��ʂ�0�ɂ���
			pPolice->tackle.tacklemove.x = 0.0f;

			//�x�@�ԗ��̒T�m����
			PatrolCarSearch(pPolice);
		}

		break;					// �����o��
	}

	//--------------------------------------------------------
	//	�ʒu�̍X�V
	//--------------------------------------------------------
	pPolice->pos.x += sinf(pPolice->rot.y) * pPolice->tackle.tacklemove.x;
	pPolice->pos.z += cosf(pPolice->rot.y) * pPolice->tackle.tacklemove.x;
}

//============================================================
// �x�@�̏a�؉��P����
//============================================================
void PoliceTrafficImprove(Police *pPolice)
{
	// �a�؃J�E���g�����Z����
	pPolice->nTrafficCnt++;

	if (pPolice->nTrafficCnt >= POLICAR_TRAFFIC_IMPROVE_CNT)
	{ // �a�؃J�E���g��������Ԃɓ������ꍇ

		// �J�E���g��0�ɂ���
		pPolice->nTrafficCnt = 0;

		// �p�g���[����Ԃɂ���
		pPolice->state = POLICESTATE_PATROL;
	}
}

//============================================================
// �x�@�̏o������
//============================================================
void PoliceSpawn(Police *pPolice)
{
	float fLength;			// �x�@�Ƃ̋���
	float fNearestLen;		// �Ŋ��̓��Ƃ̋���
	int nNearest = NONE_NEAREST;	// �Ŋ��̓�

	// �ړ��ʂ��X�V
	pPolice->move.x += POLI_MOVE_FORWARD;

	if (pPolice->move.x > MAX_POLI_FORWARD_PATROL)
	{ // �v���C���[�̈ړ��� (x) �����l�ȏ�̏ꍇ

		// �v���C���[�̈ړ��� (x) ��␳
		pPolice->move.x = MAX_POLI_FORWARD_PATROL;
	}

	// �O��ʒu�̍X�V
	pPolice->posOld = pPolice->pos;

	// �v���C���[�̒��n�̍X�V����
	LandObject(&pPolice->pos, &pPolice->move, &pPolice->bJump);

	// �v���C���[�̈ʒu�̍X�V
	PosPolice(&pPolice->move, &pPolice->pos, &pPolice->rot, pPolice->bMove);

	//----------------------------------------------------
	//	�e�̍X�V
	//----------------------------------------------------
	// �e�̈ʒu�ݒ�
	SetPositionShadow
	( // ����
		pPolice->nShadowID,		// �e�̃C���f�b�N�X
		pPolice->pos,			// �ʒu
		pPolice->rot,			// ����
		NONE_SCALE				// �g�嗦
	);

	// �Ԃ̒�~����
	CollisionStopCar
	( // ����
		pPolice->pos,		// �ʒu
		pPolice->rot,		// ����
		&pPolice->move,		// �ړ���
		pPolice->modelData.fRadius,	// ���a
		COLLOBJECTTYPE_POLICE,		// �Ώۂ̃T�C�Y
		&pPolice->nTrafficCnt		// �a�؃J�E���g
	);

	for (int nCnt = 0; nCnt < MAX_CURVEPOINT; nCnt++)
	{
		switch (pPolice->policeCurve.curveInfo.dashAngle)
		{
		case DASH_RIGHT:		//�E�ɑ����Ă���ꍇ

			if (GetCurveInfo(nCnt).dashAngle == DASH_FAR)
			{ // �����������v�����ꍇ

				// �����𑪂�
				fLength = fabsf(GetCurveInfo(nCnt).pos.x - pPolice->pos.x);

				if (nNearest == NONE_NEAREST)
				{ // �Ŋ��̔ԍ���-1�̏ꍇ

					// �Ŋ��̋���������
					fNearestLen = fLength;

					// �Ŋ��̔ԍ�������
					nNearest = nCnt;
				}
				else
				{ // ��L�ȊO
					if (fLength <= fNearestLen)
					{ // �Ŋ��̋����̕��������ꍇ

						// ������������
						fNearestLen = fLength;

						// �ԍ���ۑ�����
						nNearest = nCnt;
					}
				}
			}

			break;				//�����o��

		case DASH_LEFT:			//���𑖂��Ă���ꍇ

			if (GetCurveInfo(nCnt).dashAngle == DASH_NEAR)
			{ // �����������v�����ꍇ

				// �����𑪂�
				fLength = fabsf(GetCurveInfo(nCnt).pos.x - pPolice->pos.x);

				if (nNearest == NONE_NEAREST)
				{ // �Ŋ��̔ԍ���-1�̏ꍇ

					// �Ŋ��̋���������
					fNearestLen = fLength;

					// �Ŋ��̔ԍ�������
					nNearest = nCnt;
				}
				else
				{ // ��L�ȊO
					if (fLength <= fNearestLen)
					{ // �Ŋ��̋����̕��������ꍇ

						// ������������
						fNearestLen = fLength;

						// �ԍ���ۑ�����
						nNearest = nCnt;
					}
				}
			}

			break;				//�����o��

		case DASH_FAR:			//���ɑ����Ă���ꍇ

			if (GetCurveInfo(nCnt).dashAngle == DASH_LEFT)
			{ // �����������v�����ꍇ

				// �����𑪂�
				fLength = fabsf(GetCurveInfo(nCnt).pos.z - pPolice->pos.z);

				if (nNearest == NONE_NEAREST)
				{ // �Ŋ��̔ԍ���-1�̏ꍇ

					// �Ŋ��̋���������
					fNearestLen = fLength;

					// �Ŋ��̔ԍ�������
					nNearest = nCnt;
				}
				else
				{ // ��L�ȊO
					if (fLength <= fNearestLen)
					{ // �Ŋ��̋����̕��������ꍇ

						// ������������
						fNearestLen = fLength;

						// �ԍ���ۑ�����
						nNearest = nCnt;
					}
				}
			}

			break;				//�����o��

		case DASH_NEAR:			//��O�ɑ����Ă���ꍇ

			if (GetCurveInfo(nCnt).dashAngle == DASH_RIGHT)
			{ // �����������v�����ꍇ

				// �����𑪂�
				fLength = fabsf(GetCurveInfo(nCnt).pos.z - pPolice->pos.z);

				if (nNearest == NONE_NEAREST)
				{ // �Ŋ��̔ԍ���-1�̏ꍇ

					// �Ŋ��̋���������
					fNearestLen = fLength;

					// �Ŋ��̔ԍ�������
					nNearest = nCnt;
				}
				else
				{ // ��L�ȊO
					if (fLength <= fNearestLen)
					{ // �Ŋ��̋����̕��������ꍇ

						// ������������
						fNearestLen = fLength;

						// �ԍ���ۑ�����
						nNearest = nCnt;
					}
				}
			}

			break;				//�����o��
		}
	}

	switch (pPolice->policeCurve.curveInfo.dashAngle)
	{
	case DASH_RIGHT:		//�E�ɑ����Ă���ꍇ

		if (pPolice->pos.x >= GetCurveInfo(nNearest).pos.x + (CAR_WIDTH * 2))
		{ // ���H���ɓ������ꍇ

			// �Ԃ̈ʒu�ƌ����̐ݒ菈��
			pPolice->pos.x = GetCurveInfo(nNearest).pos.x + (CAR_WIDTH * 2);		// �ʒu
			pPolice->rot.y = 0.0f;													// ����
			pPolice->policeCurve.curveInfo = GetCurveInfo(nNearest);				// �Ȃ���p�̏��
			pPolice->policeCurveCopy = pPolice->policeCurve;						// �Ȃ���p�̏��̃R�s�[
			pPolice->policeCurve.actionState = CARACT_DASH;							// �Ԃ̏��
			pPolice->policeCurve.curveInfo.dashAngle = DASH_FAR;					// �_�b�V������
			pPolice->policeCurve.nSKipCnt = rand() % CURVE_SKIP_RANDOM + CURVE_SKIP_LEAST;	// �X�L�b�v����Ȃ���p�̉�

			// �p�g���[����Ԃɂ���
			pPolice->state = POLICESTATE_PATROL;
		}

		break;				//�����o��

	case DASH_LEFT:			//���𑖂��Ă���ꍇ

		if (pPolice->pos.x <= GetCurveInfo(nNearest).pos.x - (CAR_WIDTH * 2))
		{ // ���H���ɓ������ꍇ

			// �Ԃ̈ʒu�ƌ����̐ݒ菈��
			pPolice->pos.x = GetCurveInfo(nNearest).pos.x - (CAR_WIDTH * 2);		// �ʒu
			pPolice->rot.y = D3DX_PI;												// ����
			pPolice->policeCurve.curveInfo = GetCurveInfo(nNearest);				// �Ȃ���p�̏��
			pPolice->policeCurveCopy = pPolice->policeCurve;						// �Ȃ���p�̏��̃R�s�[
			pPolice->policeCurve.actionState = CARACT_DASH;							// �Ԃ̏��
			pPolice->policeCurve.curveInfo.dashAngle = DASH_NEAR;					// �_�b�V������
			pPolice->policeCurve.nSKipCnt = rand() % CURVE_SKIP_RANDOM + CURVE_SKIP_LEAST;	// �X�L�b�v����Ȃ���p�̉�

			// �p�g���[����Ԃɂ���
			pPolice->state = POLICESTATE_PATROL;
		}

		break;				//�����o��

	case DASH_FAR:			//���ɑ����Ă���ꍇ

		if (pPolice->pos.z >= GetCurveInfo(nNearest).pos.z + (CAR_WIDTH * 2))
		{ // ���H���ɓ������ꍇ

			// �Ԃ̈ʒu�ƌ����̐ݒ菈��
			pPolice->pos.z = GetCurveInfo(nNearest).pos.z + (CAR_WIDTH * 2);		// �ʒu
			pPolice->rot.y = -D3DX_PI * 0.5f;										// ����
			pPolice->policeCurve.curveInfo = GetCurveInfo(nNearest);				// �Ȃ���p�̏��
			pPolice->policeCurveCopy = pPolice->policeCurve;						// �Ȃ���p�̏��̃R�s�[
			pPolice->policeCurve.actionState = CARACT_DASH;							// �Ԃ̏��
			pPolice->policeCurve.curveInfo.dashAngle = DASH_LEFT;					// �_�b�V������
			pPolice->policeCurve.nSKipCnt = rand() % CURVE_SKIP_RANDOM + CURVE_SKIP_LEAST;	// �X�L�b�v����Ȃ���p�̉�

			// �p�g���[����Ԃɂ���
			pPolice->state = POLICESTATE_PATROL;
		}

		break;				//�����o��

	case DASH_NEAR:			//��O�ɑ����Ă���ꍇ

		if (pPolice->pos.z <= GetCurveInfo(nNearest).pos.z - (CAR_WIDTH * 2))
		{ // ���H���ɓ������ꍇ

			// �Ԃ̈ʒu�ƌ����̐ݒ菈��
			pPolice->pos.z = GetCurveInfo(nNearest).pos.z - (CAR_WIDTH * 2);		// �ʒu
			pPolice->rot.y = D3DX_PI * 0.5f;										// ����
			pPolice->policeCurve.curveInfo = GetCurveInfo(nNearest);				// �Ȃ���p�̏��
			pPolice->policeCurveCopy = pPolice->policeCurve;						// �Ȃ���p�̏��̃R�s�[
			pPolice->policeCurve.actionState = CARACT_DASH;							// �Ԃ̏��
			pPolice->policeCurve.curveInfo.dashAngle = DASH_RIGHT;					// �_�b�V������
			pPolice->policeCurve.nSKipCnt = rand() % CURVE_SKIP_RANDOM + CURVE_SKIP_LEAST;	// �X�L�b�v����Ȃ���p�̉�

			// �p�g���[����Ԃɂ���
			pPolice->state = POLICESTATE_PATROL;
		}

		break;				//�����o��
	}
}

//======================================================================================================================
// �x�@�̋Ȃ���p�`�F�b�N����
//======================================================================================================================
void PoliceCurveCheck(Police *pPolice)
{
	for (int nCnt = 0; nCnt < MAX_CURVEPOINT; nCnt++)
	{
		switch (pPolice->policeCurve.curveInfo.dashAngle)
		{
		case DASH_RIGHT:		//�E�ɑ����Ă���ꍇ

			//���킹��
			pPolice->pos.z = pPolice->policeCurve.curveInfo.pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pPolice->pos.z == GetCurveInfo(nCnt).pos.z - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
			{ // ����Z����𑖂��Ă���ꍇ
				if (pPolice->pos.x >= GetCurveInfo(nCnt).pos.x - (CAR_WIDTH * 2) &&
					pPolice->posOld.x <= GetCurveInfo(nCnt).pos.x - (CAR_WIDTH * 2))
				{ // �ʒu����v�����ꍇ
					if (GetCurveInfo(nCnt).dashAngle == DASH_RIGHT)
					{ // �E�ɑ���ꍇ�̂�
						// �X�L�b�v�J�E���g�����Z����
						pPolice->policeCurve.nSKipCnt--;

						if (pPolice->policeCurve.nSKipCnt == 0 || GetCurveInfo(nCnt).bDeadEnd == true)
						{ // �X�L�b�v�񐔂�0�ɂȂ����܂��́A�s���~�܂肾�����ꍇ
							// �X�L�b�v�񐔂�0�ɂ���
							pPolice->policeCurve.nSKipCnt = 0;

							// �Ȃ���p�̏����X�V����
							pPolice->policeCurve.curveInfo = GetCurveInfo(nCnt);
						}
					}
				}
			}

			break;				//�����o��

		case DASH_LEFT:			//���𑖂��Ă���ꍇ

			//���킹��
			pPolice->pos.z = pPolice->policeCurve.curveInfo.pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pPolice->pos.z == GetCurveInfo(nCnt).pos.z + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
			{ // ����Z����𑖂��Ă���ꍇ
				if (pPolice->pos.x <= GetCurveInfo(nCnt).pos.x + (CAR_WIDTH * 2) &&
					pPolice->posOld.x >= GetCurveInfo(nCnt).pos.x + (CAR_WIDTH * 2))
				{ // �ʒu����v�����ꍇ
					if (GetCurveInfo(nCnt).dashAngle == DASH_LEFT)
					{ // ���ɑ���ꍇ�̂�
						// �X�L�b�v�J�E���g�����Z����
						pPolice->policeCurve.nSKipCnt--;

						if (pPolice->policeCurve.nSKipCnt == 0 || GetCurveInfo(nCnt).bDeadEnd == true)
						{ // �X�L�b�v�񐔂�0�ɂȂ����ꍇ
							// �X�L�b�v�񐔂�0�ɂ���
							pPolice->policeCurve.nSKipCnt = 0;

							// �Ȃ���p�̏����X�V����
							pPolice->policeCurve.curveInfo = GetCurveInfo(nCnt);
						}
					}
				}
			}

			break;				//�����o��

		case DASH_FAR:			//���ɑ����Ă���ꍇ

			//���킹��
			pPolice->pos.x = pPolice->policeCurve.curveInfo.pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pPolice->pos.x == GetCurveInfo(nCnt).pos.x + (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
			{ // ����Z����𑖂��Ă���ꍇ
				if (pPolice->pos.z >= GetCurveInfo(nCnt).pos.z - (CAR_WIDTH * 2) &&
					pPolice->posOld.z <= GetCurveInfo(nCnt).pos.z - (CAR_WIDTH * 2))
				{ // �ʒu����v�����ꍇ
					if (GetCurveInfo(nCnt).dashAngle == DASH_FAR)
					{ // ���ɑ���ꍇ�̂�
						// �X�L�b�v�J�E���g�����Z����
						pPolice->policeCurve.nSKipCnt--;

						if (pPolice->policeCurve.nSKipCnt == 0 || GetCurveInfo(nCnt).bDeadEnd == true)
						{ // �X�L�b�v�񐔂�0�ɂȂ����ꍇ
							// �X�L�b�v�񐔂�0�ɂ���
							pPolice->policeCurve.nSKipCnt = 0;

							// �Ȃ���p�̏����X�V����
							pPolice->policeCurve.curveInfo = GetCurveInfo(nCnt);
						}
					}
				}
			}

			break;				//�����o��

		case DASH_NEAR:			//���ɑ����Ă���ꍇ

			//���킹��
			pPolice->pos.x = pPolice->policeCurve.curveInfo.pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2));

			if (pPolice->pos.x == GetCurveInfo(nCnt).pos.x - (SHIFT_CAR_CURVE + (CAR_WIDTH * 2)))
			{ // ����Z����𑖂��Ă���ꍇ
				if (pPolice->pos.z <= GetCurveInfo(nCnt).pos.z + (CAR_WIDTH * 2) &&
					pPolice->posOld.z >= GetCurveInfo(nCnt).pos.z + (CAR_WIDTH * 2))
				{ // �ʒu����v�����ꍇ
					if (GetCurveInfo(nCnt).dashAngle == DASH_NEAR)
					{ // ��O�ɑ���ꍇ�̂�
						// �X�L�b�v�J�E���g�����Z����
						pPolice->policeCurve.nSKipCnt--;

						if (pPolice->policeCurve.nSKipCnt == 0 || GetCurveInfo(nCnt).bDeadEnd == true)
						{ // �X�L�b�v�񐔂�0�ɂȂ����ꍇ
							// �X�L�b�v�񐔂�0�ɂ���
							pPolice->policeCurve.nSKipCnt = 0;

							// �Ȃ���p�̏����X�V����
							pPolice->policeCurve.curveInfo = GetCurveInfo(nCnt);

							if (pPolice->policeCurve.curveInfo.curveAngle == CURVE_LEFT)
							{ // �Ȃ���������������������ꍇ
								// �p�x��␳����
								pPolice->rot.y = D3DX_PI;
							}
							else
							{ // �Ȃ���������E�����������ꍇ
								// �p�x��␳����
								pPolice->rot.y = -D3DX_PI;
							}
						}
					}
				}
			}

			break;				//�����o��
		}
	}
}

//======================================================================================================================
// �x�@�̒ǉ�����
//======================================================================================================================
void AddPolice(void)
{
	while (g_AddPolice.nBonus >= ADDPOLICE_COUNT)
	{ // �J�E���g����萔�ȏ�Ȃ�Ύ��s

		// �{�[�i�X�̉񐔂����Z����
		g_AddPolice.nBonus -= ADDPOLICE_COUNT;

		D3DXVECTOR3 pos;		// �ʒu�̕ϐ�

		// �x�@�̐ݒ菈��
		pos.x = (float)(rand() % 18000 - 9000);
		pos.y = 0.0f;
		pos.z = (float)(rand() % 18000 - 9000);

		// �x�@�̐ݒ菈��
		SetPolice(pos);

		if (g_AddPolice.nBonus <= 0)
		{ // �{�[�i�X�̉񐔂�0�����������

			// �{�[�i�X�̉񐔂�␳����
			g_AddPolice.nBonus = 0;
		}
	}
}

//======================================================================================================================
// �������̎擾����
//======================================================================================================================
Reinforce *GetReinforce(void)
{
	// �����̏���Ԃ�
	return &g_AddPolice;
}

//======================================================================================================================
// �x�@�̏o���Q�[�g���菈��
//======================================================================================================================
void PoliceGatePos(Police *pPolice)
{
	int nGateNum = GetGateNum();		// �Q�[�g�̐����擾����
	int nSpawnGateNum;					// �Q�[�g�̔ԍ�
	Gate *pGate = GetGateData();		// �Q�[�g�̏��
	Player *pPlayer = GetPlayer();		// �v���C���[�̏��
	float fPlayRange;					// �v���C���[�ƃQ�[�g�͈̔�

	// �o�Ă���Q�[�g�̔ԍ��������_���ŏo��
	nSpawnGateNum = rand() % nGateNum;

	// �v���C���[�͈̔͂��Z�o����
	fPlayRange = sqrtf((pPlayer->pos.x - pGate[nSpawnGateNum].pos.x) * (pPlayer->pos.x - pGate[nSpawnGateNum].pos.x) + (pPlayer->pos.z - pGate[nSpawnGateNum].pos.z) * (pPlayer->pos.z - pGate[nSpawnGateNum].pos.z));

	if (fPlayRange >= POLICAR_SPAWN_RANGE)
	{ // ���͈̔͊O�������ꍇ

		switch (pGate[nSpawnGateNum].collInfo->stateRot)
		{
		case ROTSTATE_0:		// 0�x

			// ��O�ɑ���
			pPolice->policeCurve.curveInfo.dashAngle = DASH_NEAR;

			// ������ݒ肷��
			pPolice->rot.y = D3DX_PI;

			break;

		case ROTSTATE_180:		// 180�x

			// ���ɑ���
			pPolice->policeCurve.curveInfo.dashAngle = DASH_FAR;

			// ������ݒ肷��
			pPolice->rot.y = 0.0f;

			break;

		case ROTSTATE_90:		// 90�x

			// ���ɑ���
			pPolice->policeCurve.curveInfo.dashAngle = DASH_LEFT;

			// ������ݒ肷��
			pPolice->rot.y = -D3DX_PI * 0.5f;

			break;

		case ROTSTATE_270:		// 270�x

			// �E�ɑ���
			pPolice->policeCurve.curveInfo.dashAngle = DASH_RIGHT;

			// ������ݒ肷��
			pPolice->rot.y = D3DX_PI * 0.5f;

			break;

		default:				// ��L�ȊO

			break;
		}

		// �����̐��K��
		RotNormalize(&pPolice->rot.y);

		// �o�Ă���Q�[�g�̔ԍ���ۑ�����
		pPolice->nNumSpawnGate = nSpawnGateNum;

		// �Q�[�g�̈ʒu
		pPolice->pos.x = pGate[nSpawnGateNum].pos.x;
		pPolice->pos.z = pGate[nSpawnGateNum].pos.z;
	}
	else
	{ // �͈͓��������ꍇ
		while (fPlayRange <= POLICAR_SPAWN_RANGE)
		{
			// �Q�[�g�̐������炷
			nSpawnGateNum = (nSpawnGateNum + 1) % nGateNum;

			// �v���C���[�͈̔͂��Z�o����
			fPlayRange = sqrtf((pPlayer->pos.x - pGate[nSpawnGateNum].pos.x) * (pPlayer->pos.x - pGate[nSpawnGateNum].pos.x) + (pPlayer->pos.z - pGate[nSpawnGateNum].pos.z) * (pPlayer->pos.z - pGate[nSpawnGateNum].pos.z));
		}

		switch (pGate[nSpawnGateNum].collInfo->stateRot)
		{
		case ROTSTATE_0:		// 0�x

			// ��O�ɑ���
			pPolice->policeCurve.curveInfo.dashAngle = DASH_NEAR;

			// ������ݒ肷��
			pPolice->rot.y = D3DX_PI;

			break;

		case ROTSTATE_180:		// 180�x

			// ���ɑ���
			pPolice->policeCurve.curveInfo.dashAngle = DASH_FAR;

			// ������ݒ肷��
			pPolice->rot.y = 0.0f;

			break;

		case ROTSTATE_90:		// 90�x

			// ���ɑ���
			pPolice->policeCurve.curveInfo.dashAngle = DASH_LEFT;

			// ������ݒ肷��
			pPolice->rot.y = -D3DX_PI * 0.5f;

			break;

		case ROTSTATE_270:		// 270�x

			// �E�ɑ���
			pPolice->policeCurve.curveInfo.dashAngle = DASH_RIGHT;

			// ������ݒ肷��
			pPolice->rot.y = D3DX_PI * 0.5f;

			break;

		default:				// ��L�ȊO

			break;
		}

		// �����̐��K��
		RotNormalize(&pPolice->rot.y);

		// �o�Ă���Q�[�g�̔ԍ���ۑ�����
		pPolice->nNumSpawnGate = nSpawnGateNum;

		// �Q�[�g�̈ʒu
		pPolice->pos.x = pGate[nSpawnGateNum].pos.x;
		pPolice->pos.z = pGate[nSpawnGateNum].pos.z;
	}

	// �x�@�̑ҋ@��ԃ`�F�b�N����
	PoliceWaitCheck(pPolice);
}

//======================================================================================================================
// �x�@�̑ҋ@��ԃ`�F�b�N����
//======================================================================================================================
void PoliceWaitCheck(Police *pPolice)
{
	Police *CheckPoli = GetPoliceData();		// �m�F�p�̌x�@
	Gate *pGate = GetGateData();				// �Q�[�g�̏��
	bool bWait = false;							// �`�F�b�N�����̕ϐ�

	for (int nCnt = 0; nCnt < MAX_POLICE; nCnt++, CheckPoli++)
	{
		if (CheckPoli->bUse == true)
		{ // �g�p���Ă����ꍇ
			if (CheckPoli->nNumSpawnGate == pPolice->nNumSpawnGate
			 && CheckPoli != pPolice)
			{ // �Q�[�g�̈ʒu���ꏏ�������ꍇ
				if (CheckPoli->state == POLICESTATE_POSBACK
				 || CheckPoli->state == POLICESTATE_SPAWN)
				{ // ���̌x�@�̏�Ԃ��o����Ԃ܂��́A�ŏ��̈ʒu�ɖ߂��Ԃ������ꍇ

					// ��Ԃ�ҋ@��Ԃɂ���
					pPolice->state = POLICESTATE_WAIT;

					// �ҋ@���
					bWait = true;

					// �Q�[�g�̈ʒu
					pPolice->pos.x = pGate[pPolice->nNumSpawnGate].pos.x;
					pPolice->pos.z = pGate[pPolice->nNumSpawnGate].pos.z;

					// �����o��
					break;
				}
			}
		}
	}

	if (bWait == false)
	{ // �`�F�b�N�����������ꍇ

		// �ŏ��̍��W�ɖ߂��Ԃɂ���
		pPolice->state = POLICESTATE_POSBACK;

		// �Q�[�g�̈ʒu
		pPolice->pos.x = pGate[pPolice->nNumSpawnGate].pos.x;
		pPolice->pos.z = pGate[pPolice->nNumSpawnGate].pos.z;
	}
}

//============================================================
// �v���C���[�Ƃ̋����ŉ��ʂ𒲐�
//============================================================
void PoliceSoundVolume(D3DXVECTOR3 *pPos)
{
	Player *pPlayer = GetPlayer();				// �v���C���[�̏����擾����

	//���ʉ�BGM�̉��ʒ���
	if (GetSoundType(SOUND_TYPE_SUB_BGM) == true)
	{//���ʉ�BGM���g�p��Ԃ̂Ƃ�
		if (pPlayer->bUse == true)
		{//�v���C���[���g�p��Ԃ̂Ƃ�

			//�����𑪂�ϐ�
			float fLength = ((pPlayer->pos.x - pPos->x) * (pPlayer->pos.x - pPos->x)
							+ (pPlayer->pos.z - pPos->z) * (pPlayer->pos.z - pPos->z));

			//���ʉ�BGM�̍Đ�
			if (fLength < (POLICE_SOUND_RADIUS * POLICE_SOUND_RADIUS))
			{//���͈͓��̏ꍇ

			 //���ʐݒ�p�̕ϐ�
				float fVolume = POLICE_SOUND_VOLUME_MAX - (fLength / (POLICE_SOUND_RADIUS * POLICE_SOUND_RADIUS));

				if (fVolume >= GetSoundVolume(SOUND_LABEL_BGM_POLICE_000))
				{//���݂̉��ʂ��傫���ꍇ

					//���ʂ𒲐��i�x�@�j
					SetSoundVolume(SOUND_LABEL_BGM_POLICE_000, fVolume);


				}

			}
		}
	}
}

//============================================================
// �x�@�̃`���[�g���A������
//============================================================
void UpdateTutorialPolice(void)
{
	if (g_aPolice[0].bUse == true)
	{ // �x�@���g�p���Ă����ꍇ

		switch (g_TutorialPolice.state)
		{
		case POLITUTOSTATE_DRIVE:		// ���s��

			// �O��̈ʒu��������
			g_aPolice[0].posOld = g_aPolice[0].pos;

			// �ړ��󋵂�ݒ肷��
			g_aPolice[0].bMove = true;

			// �ړ��ʂ�ݒ肷��
			g_aPolice[0].move.x = POLICE_TUTORIAL_MOVE;

			// �v���C���[�̈ʒu�̍X�V
			PosPolice(&g_aPolice[0].move, &g_aPolice[0].pos, &g_aPolice[0].rot, g_aPolice[0].bMove);

			//�v���C���[�Ƃ̋����ŉ��ʂ𒲐�
			PoliceSoundVolume(&g_aPolice[0].pos);

			if (g_aPolice[0].pos.y < 0.0f)
			{//Y���̈ʒu��0.0f�������ꍇ

				//�c�ւ̈ړ��ʂ�0.0f�ɂ���
				g_aPolice[0].move.y = 0.0f;

				//�ʒu��0.0f�ɖ߂�
				g_aPolice[0].pos.y = 0.0f;
			}

			if (g_aPolice[0].move.x > 0.0f)
			{ // �ړ��ʂ����̐��������ꍇ

				if (g_aPolice[0].pos.x >= POLICE_TUTORIAL_DEST.x)
				{ // �ʒu���ړI�n�𒴂����ꍇ

					// ������Ԃɂ���
					g_TutorialPolice.state = POLITUTOSTATE_DISAPPEAR;

					// �ʒu��␳����
					g_aPolice[0].pos.x = POLICE_TUTORIAL_DEST.x;

					// �ړ����Ȃ�
					g_aPolice[0].bMove = false;

					// �ړ��ʂ�␳����
					g_aPolice[0].move.x = 0.0f;
				}
			}
			else
			{ // �ړ��ʂ����̐��������ꍇ

				if (g_aPolice[0].pos.x <= POLICE_TUTORIAL_DEST.x)
				{ // �ʒu���ړI�n�𒴂����ꍇ

					// ������Ԃɂ���
					g_TutorialPolice.state = POLITUTOSTATE_DISAPPEAR;

					// �ʒu��␳����
					g_aPolice[0].pos.x = POLICE_TUTORIAL_DEST.x;

					// �ړ����Ȃ�
					g_aPolice[0].bMove = false;

					// �ړ��ʂ�␳����
					g_aPolice[0].move.x = 0.0f;
				}
			}

			break;

		case POLITUTOSTATE_DISAPPEAR:	// ������

			// �����x��������
			g_TutorialPolice.fAlpha -= POLICE_TUTORIAL_ALPHA_MOVE;

			if (g_TutorialPolice.fAlpha <= 0.0f)
			{ // �����x�̓����x��0.0f�ȉ��ɂȂ����ꍇ

				// �����x��␳����
				g_TutorialPolice.fAlpha = 0.0f;

				// �x�@������
				g_aPolice[0].bUse = false;
			}

			break;
		}
	}
}

//============================================================
// �x�@�̃`���[�g���A���ł̕`�揈��
//============================================================
void DrawTutorialPolice(void)
{
	// �ϐ���錾
	float        policeRot;						// �x�@�̌����̌v�Z�p
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	Player           *pPlayer = GetPlayer();	// �v���C���[�̏��
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^

	if (g_aPolice[0].bUse == true)
	{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aPolice[0].mtxWorld);

		// �x�@�̌�����ݒ�
		policeRot = g_aPolice[0].rot.y + D3DX_PI;
		RotNormalize(&policeRot);	// �����𐳋K��

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, policeRot, g_aPolice[0].rot.x, g_aPolice[0].rot.z);
		D3DXMatrixMultiply(&g_aPolice[0].mtxWorld, &g_aPolice[0].mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aPolice[0].pos.x, g_aPolice[0].pos.y, g_aPolice[0].pos.z);
		D3DXMatrixMultiply(&g_aPolice[0].mtxWorld, &g_aPolice[0].mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aPolice[0].mtxWorld);

		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aPolice[0].modelData.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aPolice[0].modelData.dwNumMat; nCntMat++)
		{ // �}�e���A���̐����J��Ԃ�

			switch (g_TutorialPolice.state)
			{
			case POLITUTOSTATE_DRIVE:			// ���s��

				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				break;

			case POLITUTOSTATE_DISAPPEAR:		// ������

				// �F���R�s�[����
				g_aPolice[0].MatCopy[nCntMat] = pMat[nCntMat];

				//�@�����x��ݒ肷��
				g_aPolice[0].MatCopy[nCntMat].MatD3D.Diffuse.a = g_TutorialPolice.fAlpha;

				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&g_aPolice[0].MatCopy[nCntMat].MatD3D);

				break;
			}

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aPolice[0].modelData.pTexture[nCntMat]);

			// ���f���̕`��
			g_aPolice[0].modelData.pMesh->DrawSubset(nCntMat);
		}

		// �ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//============================================================
// �x�@�̃`���[�g���A���ł̐ݒ菈��
//============================================================
void SetTutorialPolice(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (g_aPolice[0].bUse == false)
	{ // �x�@���g�p����Ă��Ȃ��ꍇ

		// ��������
		g_aPolice[0].pos		  = pos;				// ���݂̈ʒu
		g_aPolice[0].posOld	  = g_aPolice[0].pos;		// �O��̈ʒu

		// �ϐ��̐ݒ�
		g_aPolice[0].rotDest	   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ڕW�̌���
		g_aPolice[0].move		   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
		g_aPolice[0].state		   = POLICESTATE_SPAWN;					// ���
		g_aPolice[0].bombState     = BOMBSTATE_NONE;					// �������Ă��Ȃ���Ԃɂ���
		g_aPolice[0].nLife		   = POLI_LIFE;							// �̗�
		g_aPolice[0].bJump		   = false;								// �W�����v���Ă��Ȃ�
		g_aPolice[0].nTrafficCnt   = 0;									// �a�؃J�E���g
		g_aPolice[0].fAlpha		   = 1.0f;								// �����x
		g_aPolice[0].nNumSpawnGate = NONE_NEAREST;						// �o�Ă���Q�[�g�̔ԍ�
		g_aPolice[0].bMove		   = false;								// �ړ����Ă��Ȃ�
		g_aPolice[0].bUse		   = true;								// �g�p��

		// �A�C�R���̏��̏�����
		g_aPolice[0].icon.nIconID  = NONE_ICON;						// �A�C�R���̃C���f�b�N�X
		g_aPolice[0].icon.state    = ICONSTATE_NONE;				// �A�C�R���̏��

		// ���f������ݒ�
		g_aPolice[0].modelData	   = GetModelData(MODELTYPE_CAR_POLICE);	// ���f�����

		// �e�̃C���f�b�N�X��ݒ�
		g_aPolice[0].nShadowID     = SetModelShadow
		( // ����
			g_aPolice[0].modelData,	// ���f�����
			&g_aPolice[0].nShadowID,	// �e�̐e�̉e�C���f�b�N�X
			&g_aPolice[0].bUse			// �e�̐e�̎g�p��
		);

		// �e�̈ʒu�ݒ�
		SetPositionShadow(g_aPolice[0].nShadowID, g_aPolice[0].pos, g_aPolice[0].rot, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

		// �A�C�R���̃C���f�b�N�X��ݒ�
		g_aPolice[0].icon.nIconID = SetIcon
		( // ����
			g_aPolice[0].pos,
			ICONTYPE_POLICE,
			&g_aPolice[0].icon.nIconID,
			&g_aPolice[0].bUse,
			&g_aPolice[0].icon.state
		);

		D3DXMATERIAL *pMat;					//�}�e���A���ւ̃|�C���^

		//�}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aPolice[0].modelData.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aPolice[0].modelData.dwNumMat; nCntMat++)
		{
			//�}�e���A���̏����擾����
			g_aPolice[0].MatCopy[nCntMat] = pMat[nCntMat];
		}

		// �Ԃ̈ʒu�ƌ����̐ݒ菈��
		g_aPolice[0].policeCurve.curveInfo = GetCurveInfo(0);					// �Ȃ���p�̏��
		g_aPolice[0].policeCurve.nSKipCnt = 0;									// �X�L�b�v����Ȃ���p�̉�
		g_aPolice[0].policeCurve.rotDest = g_aPolice[0].rot;					// �O��̌���
		g_aPolice[0].policeCurve.actionState = CARACT_DASH;						// �����Ă�����

		// �^�b�N���֌W�̕ϐ��̏�����
		g_aPolice[0].tackle.nTackleCnt = 0;			// �^�b�N���̃J�E���g
		g_aPolice[0].tackle.tacklemove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �^�b�N�����̒ǉ��ړ���
		g_aPolice[0].tackle.tackleState = TACKLESTATE_CHARGE;					// �^�b�N�����̏��

		if (g_aPolice[0].pos.x < POLICE_TUTORIAL_DEST.x)
		{ // �ړI�n�����ɂ������ꍇ

			// ������ݒ肷��
			g_aPolice[0].rot.y = -D3DX_PI * 0.5f;
		}
		else
		{ // �ړI�n���E�ɂ������ꍇ

			// ������ݒ肷��
			g_aPolice[0].rot.y = D3DX_PI * 0.5f;
		}

		// �`���[�g���A���ł̌x�@�̏�����
		g_TutorialPolice.state = POLITUTOSTATE_DRIVE;				// ���
		g_TutorialPolice.fAlpha = 1.0f;								// �����x
	}
}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================
//**********************************************************************************************************************
//	�I�u�W�F�N�g�̑����擾����
//**********************************************************************************************************************
int GetNumPolice(void)
{
	// �ϐ���錾
	int nNumPolice = 0;	// �I�u�W�F�N�g�̑����̊m�F�p

	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aPolice[nCntPolice].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

		  // �J�E���^�[�����Z
			nNumPolice++;
		}
	}

	// �ϐ��̒l��Ԃ�
	return nNumPolice;	// �I�u�W�F�N�g�̑���
}
#endif