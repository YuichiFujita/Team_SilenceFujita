//======================================================================================================================
//
//	�G�f�B�b�g�p�[�e�B�N������ [EditParticle.cpp]
//	Author�F��������
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "Editmain.h"
#include "EditParticle.h"
#include "EditEffect.h"
#include "input.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************

//**********************************************************************************************************************
//�v���g�^�C�v�錾
//**********************************************************************************************************************
void EditParticleUpdate(void);			// �p�[�e�B�N���̍X�V
void CustomRandomParticle(void);		// �p�[�e�B�N���̃����_���̃J�X�^��
void CustomRandomSpeed(void);			// �p�[�e�B�N���̑��x�̃J�X�^��

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
EditParticle g_EditParticle;			// �p�[�e�B�N���̏��
int g_nEditStyleParticle;				// �G�f�B�b�g�X�^�C��(�p�[�e�B�N��)

//======================================================================================================================
//	�G�f�B�b�g�p�[�e�B�N���̏���������
//======================================================================================================================
void InitEditParticle(void)
{
	g_EditParticle.pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
	g_EditParticle.col			= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F
	g_EditParticle.Radius		= D3DXVECTOR3(3.0f, 3.0f, 0.0f);			// ���a
	g_EditParticle.nRandom		= 6.28f;									// �����_���͈̔�
	g_EditParticle.nSpawn		= 4;										// �G�t�F�N�g��
	g_EditParticle.nCountPress	= 0;										// �J�E���g�v���X
	g_EditParticle.fAngle		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �p�x
	g_EditParticle.bUse			= false;									// �g�p��
	g_EditParticle.move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
	g_EditParticle.Speed		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);			// ���x
	g_EditParticle.SpeedRandomX = 1;										// �ړ��ʂ̃����_��(X��)
	g_EditParticle.SpeedRandomY = 1;										// �ړ��ʂ̃����_��(Y��)
	g_EditParticle.SpeedRandomZ = 1;										// �ړ��ʂ̃����_��(Z��)
}

//======================================================================================================================
//	�G�f�B�b�g�p�[�e�B�N���̏I������
//======================================================================================================================
void UninitEditParticle(void)
{

}

//======================================================================================================================
//	�G�f�B�b�g�p�[�e�B�N���̍X�V����
//======================================================================================================================
void UpdateEditParticle(void)
{
	g_nEditStyleParticle = GetStyle();			//�X�^�C�����擾����
	
	if (g_nEditStyleParticle == EDITSTYLE_PARTICLE)
	{//�p�[�e�B�N���G�f�B�b�g���[�h�������ꍇ
		// �g�p���Ă���
		g_EditParticle.bUse = true;
	}
	else
	{//�p�[�e�B�N���G�f�B�b�g���[�h����Ȃ������ꍇ
		// �g�p���Ă��Ȃ�
		g_EditParticle.bUse = false;
	}

	if (g_EditParticle.bUse == true)
	{ // �G�f�B�b�g�p�[�e�B�N�����g�p����Ă���ꍇ
		// �p�[�e�B�N���̃����_���̃J�X�^��
		CustomRandomParticle();

		// �p�[�e�B�N���̑��x�̃J�X�^��
		CustomRandomSpeed();

		// �p�[�e�B�N���̍X�V
		EditParticleUpdate();
	}
}

//======================================================================================================================
//	�G�f�B�b�g�p�[�e�B�N���̕`�揈��
//======================================================================================================================
void DrawEditParticle(void)
{

}

//======================================================================================================================
// �p�[�e�B�N���̎擾����
//======================================================================================================================
EditParticle *GetEditParticle(void)
{
	//�p�[�e�B�N���̏���Ԃ�
	return &g_EditParticle;
}

//======================================================================================================================
// �G�f�B�b�g�p�[�e�B�N���̍X�V
//======================================================================================================================
void EditParticleUpdate(void)
{
	int nRandomMinus;			// �����_���̈�����

	float AngleDiv;				//�p�x�̊��鐔

	if (g_EditParticle.nRandom <= 7)
	{//�����_���̒l��10�ȉ��������ꍇ
		//���鐔��1.0f�ɐݒ肷��
		AngleDiv = 1.0f;
	}
	else if (g_EditParticle.nRandom <= 63)
	{//�����_���̒l��100�ȉ��������ꍇ
		//���鐔��10.0f�ɐݒ肷��
		AngleDiv = 10.0f;
	}
	else if (g_EditParticle.nRandom <= 629)
	{//�����_���̒l��100�ȉ��������ꍇ
		//���鐔��100.0f�ɐݒ肷��
		AngleDiv = 100.0f;
	}

	nRandomMinus = (int)(g_EditParticle.nRandom / 2);

	for (int nCntAppear = 0; nCntAppear < g_EditParticle.nSpawn; nCntAppear++)
	{ // �p�[�e�B�N���� 1F�Ő��������G�t�F�N�g�����J��Ԃ�
		//�p�x��ݒ肷��
		g_EditParticle.fAngle.x = (float)(rand() % (int)(g_EditParticle.nRandom * 100) - 314) / 100.0f;
		g_EditParticle.fAngle.y = (float)(rand() % (int)(g_EditParticle.nRandom * 100) - 314) / 100.0f;
		g_EditParticle.fAngle.z = (float)(rand() % (int)(g_EditParticle.nRandom * 100) - 314) / 100.0f;

		//�ړ��ʂ�ݒ肷��
		g_EditParticle.Speed.x = (float)(rand() % g_EditParticle.SpeedRandomX) / 10.0f;
		g_EditParticle.Speed.y = (float)(rand() % g_EditParticle.SpeedRandomY) / 10.0f;
		g_EditParticle.Speed.z = (float)(rand() % g_EditParticle.SpeedRandomZ) / 10.0f;

		//�ړ��ʂ�ݒ肷��
		g_EditParticle.move.x = sinf(g_EditParticle.fAngle.x) * g_EditParticle.Speed.x;
		g_EditParticle.move.y = cosf(g_EditParticle.fAngle.y) * g_EditParticle.Speed.y;
		g_EditParticle.move.z = cosf(g_EditParticle.fAngle.z) * g_EditParticle.Speed.z;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&g_EditParticle.move, &g_EditParticle.move);

		// �G�t�F�N�g�̐ݒ菈��
		SetEditEffect(g_EditParticle.pos,
			g_EditParticle.move,
			g_EditParticle.col,
			40,
			g_EditParticle.Radius);
	}
}
//======================================================================================================================
// �p�[�e�B�N���̃����_���̃J�X�^��
//======================================================================================================================
void CustomRandomParticle(void)
{
	if (GetKeyboardPress(DIK_1) == true)
	{// 1�L�[���������ꍇ
		// �J�E���g�v���X�����Z����
		g_EditParticle.nCountPress++;

		if (g_EditParticle.nCountPress % 20 == 0)
		{// �J�E���g�����̐��ɂȂ����ꍇ
			// �����_���͈̔͂𑝂₷
			g_EditParticle.nRandom += 0.04f;

			if (g_EditParticle.nRandom > 6.29)
			{// �����_����629��蒴�߂����ꍇ
			 // �����_����629�ɌŒ肷��
				g_EditParticle.nRandom = 6.29f;
			}
		}
	}
	else if (GetKeyboardPress(DIK_2) == true)
	{// 2�L�[���������ꍇ
		// �J�E���g�v���X�����Z����
		g_EditParticle.nCountPress++;

		if (g_EditParticle.nCountPress % 20 == 0)
		{// �J�E���g�����̐��ɂȂ����ꍇ
			// �����_���͈̔͂����炷
			g_EditParticle.nRandom -= 0.04f;

			if (g_EditParticle.nRandom < 0.10f)
			{// �����_����1�����ɂȂ����ꍇ
			 // �����_����1�ɌŒ肷��
				g_EditParticle.nRandom = 0.10f;
			}
		}
	}
}

//======================================================================================================================
// �p�[�e�B�N���̑��x�̃J�X�^��
//======================================================================================================================
void CustomRandomSpeed(void)
{
	if (GetKeyboardPress(DIK_3) == true)
	{// 3�L�[���������ꍇ
		// �J�E���g�v���X�����Z����
		g_EditParticle.nCountPress++;

		if (g_EditParticle.nCountPress % 20 == 0)
		{// �J�E���g�����̐��ɂȂ����ꍇ
			// ���x�̃����_��(X��)�����Z����
			g_EditParticle.SpeedRandomX++;

			if (g_EditParticle.SpeedRandomX > 100)
			{// �����_����100��蒴�߂����ꍇ
				// �����_����100�ɌŒ肷��
				g_EditParticle.SpeedRandomX = 100;
			}
		}
	}
	else if (GetKeyboardPress(DIK_4) == true)
	{// 4�L�[���������ꍇ
		// �J�E���g�v���X�����Z����
		g_EditParticle.nCountPress++;

		if (g_EditParticle.nCountPress % 20 == 0)
		{// �J�E���g�����̐��ɂȂ����ꍇ
			// ���x�̃����_��(X��)�����Z����
			g_EditParticle.SpeedRandomY++;

			if (g_EditParticle.SpeedRandomY > 100)
			{// �����_����100��蒴�߂����ꍇ
				// �����_����100�ɌŒ肷��
				g_EditParticle.SpeedRandomY = 100;
			}
		}
	}
	else if (GetKeyboardPress(DIK_5) == true)
	{// 5�L�[���������ꍇ
		// �J�E���g�v���X�����Z����
		g_EditParticle.nCountPress++;

		if (g_EditParticle.nCountPress % 20 == 0)
		{// �J�E���g�����̐��ɂȂ����ꍇ
			// ���x�̃����_��(X��)�����Z����
			g_EditParticle.SpeedRandomZ++;

			if (g_EditParticle.SpeedRandomZ > 100)
			{// �����_����100��蒴�߂����ꍇ
				// �����_����100�ɌŒ肷��
				g_EditParticle.SpeedRandomZ = 100;
			}
		}
	}
}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================
#endif