//========================================
//
//�G�f�B�b�g���C���̃��C������[Editmain.cpp]
//Author ��������
//
//========================================
#include "main.h"
#include "game.h"
#include "Edit.h"
#include "EditBillboard.h"
#include "Editmain.h"
#include "SoundDJ.h"
#include "input.h"

//�v���g�^�C�v�錾
void DrawDebugEditObject(void);			//�G�f�B�b�g�I�u�W�F�N�g���[�h�̃f�o�b�O�\��
void DrawDebugEditBillboard(void);		//�G�f�B�b�g�r���{�[�h���[�h�̃f�o�b�O�\��
void DrawDebugControlObject(void);		//�G�f�B�b�g�I�u�W�F�N�g�������
void DrawDebugControlBillboard(void);	//�G�f�B�b�g�r���{�[�h�������

//�O���[�o���ϐ�
int g_EditStyle;						//�X�^�C��

//======================
//������
//======================
void InitEditmain()
{
	//�G�f�B�b�g�I�u�W�F�N�g�̏���������
	InitEditObject();

	//�G�f�B�b�g�r���{�[�h�̏���������
	InitEditBillboard();

	//�G�f�B�b�g�I�u�W�F�N�g�X�^�C���ɂ���
	g_EditStyle = EDITSTYLE_OBJECT;
}

//============================
//�I��
//============================
void UninitEditmain(void)
{
	//�G�f�B�b�g�̏I������
	UninitEditObject();

	//�G�f�B�b�g�r���{�[�h�̏I������
	UninitEditBillboard();
}

//==========================
//�X�V����
//==========================
void UpdateEditmain(void)
{
	//�X�^�C���ύX����
	StyleChange();

	if (g_EditStyle == EDITSTYLE_OBJECT)
	{//�I�u�W�F�N�g�G�f�B�b�g���[�h�̏ꍇ
		//�G�f�B�b�g�̍X�V����
		UpdateEditObject();
	}
	else if (g_EditStyle == EDITSTYLE_BILLBOARD)
	{//�r���{�[�h�G�f�B�b�g���[�h�̏ꍇ
		//�G�f�B�b�g�r���{�[�h�̍X�V����
		UpdateEditBillboard();
	}
}

//============================
//�Q�[����ʂ̕`�揈��
//============================
void DrawEditmain(void)
{
	if (g_EditStyle == EDITSTYLE_OBJECT)
	{//�I�u�W�F�N�g�G�f�B�b�g���[�h�̏ꍇ
		//�G�f�B�b�g�I�u�W�F�N�g�̕`�揈��
		DrawEditObject();
	}
	else if (g_EditStyle == EDITSTYLE_BILLBOARD)
	{//�r���{�[�h�G�f�B�b�g���[�h�̏ꍇ
		//�G�f�B�b�g�r���{�[�h�̕`�揈��
		DrawEditBillboard();
	}
}

//=======================================
//�X�^�C���ύX����
//=======================================
void StyleChange(void)
{
	D3DXVECTOR3 *pEditBillpos = &GetEditBillboard()->pos;		//�G�f�B�b�g�r���{�[�h�̏����擾����
	D3DXVECTOR3 *pEditObjepos = &GetEditObject()->pos;			//�G�f�B�b�g�I�u�W�F�N�g�̏����擾����

	if (GetKeyboardTrigger(DIK_6) == true)
	{//6�L�[���������ꍇ
		//�X�^�C����I������
		g_EditStyle = (g_EditStyle + 1) % EDITSTYLE_MAX;

		if (g_EditStyle == EDITSTYLE_BILLBOARD)
		{//�r���{�[�h���[�h�������ꍇ
			//�I�u�W�F�N�g���������ʒu��������
			*pEditBillpos = *pEditObjepos;
		}
		else if (g_EditStyle == EDITSTYLE_OBJECT)
		{//�I�u�W�F�N�g���[�h�������ꍇ
			//�r���{�[�h���������ʒu��������
			*pEditObjepos = *pEditBillpos;
		}
	}
}

//=======================================
//�X�^�C���̎擾����
//=======================================
int GetStyle(void)
{
	//�G�f�B�b�g�X�^�C����Ԃ�
	return g_EditStyle;
}