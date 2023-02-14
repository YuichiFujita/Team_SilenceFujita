//==================================================================================
//
//	�ėp�֐����� [calculation.cpp]
//	Author�F���c�E�� & ��������
//
//==================================================================================
//**********************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************
#include "calculation.h"
#include "meshfield.h"

#include "billboard.h"
#include "camera.h"
#include "Car.h"
#include "effect.h"
#include "gate.h"
#include "Human.h"
#include "light.h"
#include "meshdome.h"
#include "meshcylinder.h"
#include "meshwall.h"
#include "particle.h"
#include "shadow.h"
#include "object.h"
#include "Police.h"
#include "weather.h"

#include "bonus.h"

//==================================================================================
//	�l���_�̈ʒu�̌v�Z����
//==================================================================================
//	�l�p�̒��_���W���v�Z����ۂɎg�p
//==================================================================================
void VecSizePos(D3DXVECTOR3 *sizePos, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight)
{
	// �ϐ���錾
	float fAngle, fLength;	// ���_���W�̌v�Z�p

	// �Ίp���̒����A�p�x�����߂�
	fAngle  = atan2f(fWidth, fHeight);
	fLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;
	
	// ���_���W�����߂�
	sizePos[0].x = pos.x + sinf(rot.y + (D3DX_PI + fAngle)) * fLength;
	sizePos[0].y = pos.y;
	sizePos[0].z = pos.z + cosf(rot.y + (D3DX_PI + fAngle)) * fLength;
	sizePos[1].x = pos.x + sinf(rot.y + (D3DX_PI - fAngle)) * fLength;
	sizePos[1].y = pos.y;
	sizePos[1].z = pos.z + cosf(rot.y + (D3DX_PI - fAngle)) * fLength;
	sizePos[2].x = pos.x + sinf(rot.y + fAngle) * fLength;
	sizePos[2].y = pos.y;
	sizePos[2].z = pos.z + cosf(rot.y + fAngle) * fLength;
	sizePos[3].x = pos.x + sinf(rot.y - fAngle) * fLength;
	sizePos[3].y = pos.y;
	sizePos[3].z = pos.z + cosf(rot.y - fAngle) * fLength;
}

//==================================================================================
//	�O�ς̍��E���f����
//==================================================================================
//	���E�����猩�āA���̋��E���̉E�ɂ��邩�A���ɂ��邩�̔���Ɏg�p
//==================================================================================
float LineOuterProduct(D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 pos)
{
	// �ϐ���錾
	D3DXVECTOR3 vecLine;	// ���E���x�N�g��
	D3DXVECTOR3 vecToPos;	// ���[�ƈʒu�̃x�N�g��

	// ���E���x�N�g�������߂�
	vecLine = posRight - posLeft;

	// ���[�ƈʒu�̃x�N�g�������߂�
	vecToPos = pos - posLeft;

	// �O�ς̌v�Z���ʂ�Ԃ�
	return (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);
}

//==================================================================================
//	�O�ς̓����蔻��
//==================================================================================
//	���E���ɂԂ����Ă����ꍇ�A�Ԃ������ʒu�����߂�ۂɎg�p
//==================================================================================
void CollisionOuterProduct(D3DXVECTOR3 *Targetpos, D3DXVECTOR3 *TargetposOld, D3DXVECTOR3 WallRpos, D3DXVECTOR3 WallLpos, bool *bUse, int *nShadow)
{
	// �ϐ���錾
	D3DXVECTOR3 vecMove, vecLine, vecTopos, posCross;	// �O�ς̕ϐ�
	float       fRate;									// �����̕ϐ�

	// �e�̋O�Ղ𑪂�
	vecMove.x = (Targetpos->x - TargetposOld->x);
	vecMove.y = (Targetpos->y - TargetposOld->y);
	vecMove.z = (Targetpos->z - TargetposOld->z);

	// �ǂ̋��E���𑪂�
	vecLine.x = (WallRpos.x - WallLpos.x);
	vecLine.y = (WallRpos.y - WallLpos.y);
	vecLine.z = (WallRpos.z - WallLpos.z);

	// �ǂ̎n�_����e�̈ʒu�̋����𑪂�
	vecTopos.x = (Targetpos->x - WallLpos.x);
	vecTopos.y = (Targetpos->y - WallLpos.y);
	vecTopos.z = (Targetpos->z - WallLpos.z);

	// ���������߂�
	fRate = ((vecTopos.z * vecMove.x) - (vecTopos.x * vecMove.z)) / ((vecLine.z * vecMove.x) - (vecLine.x * vecMove.z));

	// ��_�����߂�
	posCross.x = vecLine.x * fRate - WallLpos.x;
	posCross.y = vecLine.y * fRate - WallLpos.y;
	posCross.z = vecLine.z * fRate - WallLpos.z;

	if ((vecLine.z * vecTopos.x) - (vecLine.x * vecTopos.z) < 0.0f)
	{ // ���E���𒴂����ꍇ

		if (fRate >= 0.0f && fRate <= 1.0f)
		{ // ������0.0f�`1.0f�̊Ԃ�����(���E���𒴂���)�ꍇ

			// �ʒu��ݒ肷��
			Targetpos = &posCross;
		}
	}
}

//==================================================================================
//	���f���̒��n�̍X�V����
//==================================================================================
//	�n�ʂ̏�ɗ������b�V���t�B�[���h�̏�ɂ��邩�̔���Ɏg�p
//==================================================================================
void LandObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, bool *pbJump)
{
	// �ϐ���錾
	float fLandPosY = CollisionMeshField(*pPos);		// ���n�_

	if (pPos->y < fLandPosY)
	{ // ���n�_�ɓ������Ă���ꍇ

		// �W�����v���Ă��Ȃ���Ԃɂ���
		*pbJump = false;

		// �ʒu��␳
		pPos->y = fLandPosY;

		// �ړ��ʂ�������
		pMove->y = 0.0f;
	}
	else
	{ // ���n�_�ɓ������Ă��Ȃ��ꍇ

		// �W�����v���Ă����Ԃɂ���
		*pbJump = true;
	}
}

//==================================================================================
//	�����̐��K��
//==================================================================================
//	����������̐��K���Ɏg�p
//==================================================================================
void RotNormalize(float *rot)
{
	if      (*rot >  D3DX_PI) { *rot -= D3DX_PI * 2; }
	else if (*rot < -D3DX_PI) { *rot += D3DX_PI * 2; }
}

//==================================================================================
//	�t�@�C�������[�h����S�̏���
//==================================================================================
//	�t�@�C�������[�h����ۂɎg�p�B�����œǂݍ��ނ��̂��R���g���[�����邱�Ƃ���
//==================================================================================
void LoadFileChunk(bool bCurve, bool bHumanCurve, bool bStage, bool bCollision, bool bShadow, bool bObject, bool bAI)
{
	// �J�[�u�̏��̏���������
	InitCurveInfo();

	if (bCurve == true)
	{ // �J�[�u��ǂݍ��ޏꍇ

		// �J�[�u�e�L�X�g�̃��[�h����
		LoadCurveTxt();
	}

	if (bHumanCurve == true)
	{ // �l�Ԃ̋Ȃ���p��ǂݍ��ޏꍇ

		// �l�Ԃ̃��[�g�̃��[�h����
		LoadHumanCurveTxt();

		// �Ȃ���p�̐ݒ菈��
		SetCurvePoint();
	}

	if (bStage == true)
	{ // �X�e�[�W��ǂݍ��ޏꍇ

		// �X�e�[�W�̃Z�b�g�A�b�v
		TxtSetStage();
	}

	if (bCollision == true)
	{ // �����蔻���ǂݍ��ޏꍇ

		// �����蔻��̃Z�b�g�A�b�v
		TxtSetCollision();
	}

	if (bShadow == true)
	{ // �e��ǂݍ��ޏꍇ

		// �e�̔��a�̃Z�b�g�A�b�v
		TxtSetShadow();
	}

	if (bObject == true)
	{ // �I�u�W�F�N�g��ǂݍ��ޏꍇ

		// �I�u�W�F�N�g�̃Z�b�g�A�b�v
		TxtSetObject();
	}

	if (bAI == true)
	{ // AI��ǂݍ��ޏꍇ

		// AI�n�̃Z�b�g�A�b�v
		TxtSetAI();
	}
}

//==================================================================================
//	���\�������̑S�̏���
//==================================================================================
void InitAllAroundChunk(void)
{
	// �V�C�̏���������
	InitWeather();

	// �e�̏�����
	InitShadow();

	// �x�@�̏�����
	InitPolice();

	// �I�u�W�F�N�g�̏�����
	InitObject();

	// �Ԃ̏�����
	InitCar();

	// �l�Ԃ̏�����
	InitHuman();

	// �Q�[�g�̏�����
	InitGate();

	// �J�����̏�����
	InitCamera();

	// ���C�g�̏�����
	InitLight();

	// ���b�V���h�[���̏�����
	InitMeshDome();

	// ���b�V���V�����_�[�̏�����
	InitMeshCylinder();

	// ���b�V���t�B�[���h�̏�����
	InitMeshField();

	// ���b�V���E�H�[���̏�����
	InitMeshWall();

	// �r���{�[�h�̏�����
	InitBillboard();

	// �G�t�F�N�g�̏�����
	InitEffect();

	// �p�[�e�B�N���̏�����
	InitParticle();
}

//==================================================================================
//	���\�I���̑S�̏���
//==================================================================================
void UninitAllAroundChunk(void)
{
	// �V�C�̏���������
	UninitWeather();

	// �e�̏�����
	UninitShadow();

	// �x�@�̏�����
	UninitPolice();

	// �I�u�W�F�N�g�̏�����
	UninitObject();

	// �Ԃ̏�����
	UninitCar();

	// �l�Ԃ̏�����
	UninitHuman();

	// �Q�[�g�̏�����
	UninitGate();

	// �J�����̏�����
	UninitCamera();

	// ���C�g�̏�����
	UninitLight();

	// ���b�V���h�[���̏�����
	UninitMeshDome();

	// ���b�V���V�����_�[�̏�����
	UninitMeshCylinder();

	// ���b�V���t�B�[���h�̏�����
	UninitMeshField();

	// ���b�V���E�H�[���̏�����
	UninitMeshWall();

	// �r���{�[�h�̏�����
	UninitBillboard();

	// �G�t�F�N�g�̏�����
	UninitEffect();

	// �p�[�e�B�N���̏�����
	UninitParticle();
}

//==================================================================================
//	���\�`��̑S�̏���
//==================================================================================
void DrawAllAroundChunk(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	//------------------------------------------------------------------------------------------------------------------
	//	���C���J�����̕`��
	//------------------------------------------------------------------------------------------------------------------
	// �J�����̐ݒ�
	SetCamera(CAMERATYPE_MAIN);

	// ���b�V���h�[���̕`��
	DrawMeshDome();

	// ���̕`�揈��
	DrawThunder();

	// ���b�V���V�����_�[�̕`��
	DrawMeshCylinder();

	// ���b�V���t�B�[���h�̕`��
	DrawMeshField(false);

	// ���b�V���E�H�[���̕`��
	DrawMeshWall();

	// �e�̕`��
	DrawShadow();

	// �I�u�W�F�N�g�̕`��
	DrawObject();

	// �x�@�̕`��
	DrawPolice();

	// �Ԃ̕`�揈��
	DrawCar();

	// �l�Ԃ̕`��
	DrawHuman();

	// �Q�[�g�̕`��
	DrawGate();

	// �r���{�[�h�̕`��
	DrawBillboard();

	// ���e�̕`��
	DrawBomb();

	// �G�t�F�N�g�̕`��
	DrawEffect();

	// �p�[�e�B�N���̕`��
	DrawParticle();

	// �V�C�̕`�揈��
	DrawWeather();
}

//==================================================================================
//	���U���g�̏������S�̏���
//==================================================================================
void InitResultChunk(void)
{
	// ���\�������̑S�̏���
	InitAllAroundChunk();
}

//==================================================================================
//	���U���g�̏I���S�̏���
//==================================================================================
void UninitResultChunk(void)
{
	// ���\�I���̑S�̏���
	UninitAllAroundChunk();
}

//==================================================================================
//	���U���g�̕`��S�̏���
//==================================================================================
void DrawResultChunk(void)
{
	// ���\�`��̑S�̏���
	DrawAllAroundChunk();
}

//==================================================================================
//�@�S�ẴA�b�v�f�[�g���I����Ă��邩�ǂ����̔��f����
//==================================================================================
//�@���I���̃A�b�v�f�[�g���Ȃ����𔻒f���鏈��
//==================================================================================
bool UpdateAllClear(void)
{
	bool bAllClear = true;			// �Ԃ�l�p�̕ϐ�(�S�ẴA�b�v�f�[�g���I�����Ă���)
	
	Bonus *pBonus = GetBonus();		// �{�[�i�X�̏��

	for (int nCnt = 0; nCnt < MAX_BONUS; nCnt++, pBonus++)
	{
		if (pBonus->bUse == true)
		{ // �g�p����Ă���ꍇ

			// �S�Ă̍X�V���������Ă��Ȃ�
			bAllClear = false;

			// �{�[�i�X�̍X�V
			UpdateBonus();

			break;		// �����o��
		}
	}

	//Gate *pGate = GetGateData();	// �Q�[�g�̏��

	//for (int nCnt = 0; nCnt < MAX_GATE; nCnt++, pGate++)
	//{
	//	if (pGate->bUse == true)
	//	{ // �g�p���Ă���ꍇ
	//		if (pGate->state != GATESTATE_STOP)
	//		{ // ��~��Ԃ���Ȃ��ꍇ

	//			// �S�Ă̍X�V���������Ă��Ȃ�
	//			bAllClear = false;

	//			// �Q�[�g�̍X�V
	//			UpdateGate();

	//			break;	// �����o��
	//		}
	//	}
	//}

	return bAllClear;		// �S�ẴA�b�v�f�[�g�̕Ԃ�l
}