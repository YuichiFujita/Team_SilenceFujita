#ifdef _DEBUG	// �f�o�b�O����
//===================================
//
//�G�f�B�b�g�w�b�_�[[Edit.h]
//Author ��������
//
//===================================
#ifndef _EDITBILLBOARD_H_
#define _EDITBILLBOARD_H_

#include "main.h"
#include "game.h"
#include "billboard.h"

//�Z�b�g�r���{�[�h�̏��
typedef struct
{
	D3DXVECTOR3 pos;									//�ʒu
	D3DXVECTOR2 Radius;									//��
	D3DXCOLOR col;										//�F
	bool bUse;											//�r���{�[�h���g�p�\���ǂ���
	int nSetNumber;										//�r���{�[�h�̔ԍ�
	int nType;											//���
	D3DXMATRIX mtx;										//���[���h�}�g���b�N�X
	LPDIRECT3DTEXTURE9 Texture[BILLBOARD_MAX];			//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 VtxBuff;					//���_�o�b�t�@�ւ̃|�C���^
	int nColorCount;									//�F��ς���Ƃ��̃J�E���g
	bool bAnimReplay;									//�A�j���[�V�������v���C���邩�ǂ���
	BillboardAnim EditAnim;								//�A�j���[�V�����֌W
	bool bShadow;										//�e��t���邩�ǂ���
}EditBillboard;

//�v���g�^�C�v�錾
void InitEditBillboard(void);							//�r���{�[�h�̏���������
void UninitEditBillboard(void);							//�r���{�[�h�̏I������
void UpdateEditBillboard(void);							//�r���{�[�h�̍X�V����
void DrawEditBillboard(void);							//�r���{�[�h�̕`�揈��
EditBillboard *GetEditBillboard(void);					//�G�f�B�b�g�r���{�[�h�̎擾����

#endif

#endif