//======================================================================================================================
//
//	���C�g���� [light.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "game.h"
#include "light.h"
#include "weather.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAX_LIGHT		(3)			// �g�p���郉�C�g��

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
D3DLIGHT9 g_aLight[MAX_LIGHT];		// ���C�g�̏��

//======================================================================================================================
//	���C�g�̏���������
//======================================================================================================================
void InitLight(void)
{
	MODE mode = GetMode();			// ���[�h
	RESULTSTATE ResultState = GetResultState();	// �Q�[���̏�Ԃ��擾����

	// �ϐ��z���錾
	D3DXCOLOR   aDiffuse[MAX_LIGHT];			// �ݒ�p�g�U���J���[
	D3DXVECTOR3 aVecDir[MAX_LIGHT];				// �ݒ�p�����x�N�g��
	WEATHERTYPE weather = GetWeather();			// �V�C���擾����

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	switch (mode)
	{ // ���[�h���Ƃ̏���
	case MODE_TUTORIAL:	// �`���[�g���A��
		
		// �g�U���J���[��ݒ�
		aDiffuse[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		aDiffuse[1] = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
		aDiffuse[2] = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

		// �����𔲂���
		break;

	case MODE_GAME:		// �Q�[��

		switch (weather)
		{ // �V�󂲂Ƃ̏���
		case WEATHERTYPE_SUNNY:		// ����

			// �g�U���J���[��ݒ�
			aDiffuse[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			aDiffuse[1] = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
			aDiffuse[2] = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

			// �����𔲂���
			break;

		case WEATHERTYPE_RAIN:		// �J

			// �g�U���J���[��ݒ�
			aDiffuse[0] = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
			aDiffuse[1] = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
			aDiffuse[2] = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

			// �����𔲂���
			break;

		case WEATHERTYPE_SNOW:		// ��

			// �g�U���J���[��ݒ�
			aDiffuse[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			aDiffuse[1] = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
			aDiffuse[2] = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

			// �����𔲂���
			break;

		case WEATHERTYPE_THUNDER:	// ���J

			// �g�U���J���[��ݒ�
			aDiffuse[0] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			aDiffuse[1] = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
			aDiffuse[2] = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);

			// �����𔲂���
			break;
		}

		// �����𔲂���
		break;

	case MODE_RESULT:	// ���U���g

		if (ResultState == RESULTSTATE_CLEAR)
		{ // �Q�[���N���A��Ԃ̏ꍇ
	
			// �g�U���J���[��ݒ�
			aDiffuse[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			aDiffuse[1] = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
			aDiffuse[2] = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
		}
		else if (ResultState == RESULTSTATE_TIMEOVER
			 ||  ResultState == RESULTSTATE_LIFEOVER)
		{ // �Q�[���I�[�o�[��Ԃ̏ꍇ
	
			// �g�U���J���[��ݒ�
			aDiffuse[0] = D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f);
			aDiffuse[1] = D3DXCOLOR(0.7f, 0.2f, 0.2f, 1.0f);
			aDiffuse[2] = D3DXCOLOR(0.3f, 0.1f, 0.1f, 1.0f);
		}

		// �����𔲂���
		break;

	case MODE_RANKING:	// �����L���O
		
		// �g�U���J���[��ݒ�
		aDiffuse[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		aDiffuse[1] = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
		aDiffuse[2] = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

		// �����𔲂���
		break;
	}

	// �����x�N�g����ݒ�
	aVecDir[0] = D3DXVECTOR3( 0.34f, -0.87f,  0.35f);
	aVecDir[1] = D3DXVECTOR3(-0.59f, -0.10f, -0.80f);
	aVecDir[2] = D3DXVECTOR3(-0.22f, -0.74f,  0.63f);

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{ // �g�p���郉�C�g�����J��Ԃ�

		// �\���̗̂v�f���N���A
		ZeroMemory(&g_aLight[nCntLight], sizeof(D3DLIGHT9));

		// ���C�g�̎�ނ�ݒ�
		g_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		// ���C�g�̊g�U����ݒ�
		g_aLight[nCntLight].Diffuse = aDiffuse[nCntLight];

		// ���C�g�̕�����ݒ�
		D3DXVec3Normalize(&aVecDir[nCntLight], &aVecDir[nCntLight]);	// �x�N�g���𐳋K��
		g_aLight[nCntLight].Direction = aVecDir[nCntLight];

		// ���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &g_aLight[nCntLight]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//======================================================================================================================
//	���C�g�̏I������
//======================================================================================================================
void UninitLight(void)
{

}

//======================================================================================================================
//	���C�g�̍X�V����
//======================================================================================================================
void UpdateLight(void)
{

}