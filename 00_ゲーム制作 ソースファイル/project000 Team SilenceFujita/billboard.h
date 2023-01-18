//===================================
//
//�r���{�[�h�w�b�_�[[billboard.h]
//Author ��������
//
//===================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"

//�}�N����`
#define MAX_BILLBOARD			(256)				//�r���{�[�h�̍ő吔

//�r���{�[�h�A�j���[�V�����֌W
typedef struct
{
	bool bAnim;											//�A�j���[�V�������邩�ǂ���
	int nAnimCounter;									//�A�j���[�V�����J�E���^�[
	int nReplayCount;									//�Đ��J�E���g
	int nAnimPattern;									//�A�j���[�V�����p�^�[��
	int nReplayPattern;									//�Đ��p�^�[��
	float TexPattern;									//�e�N�X�`���̃p�^�[��
}BillboardAnim;

//�r���{�[�h�̍\����
typedef struct
{
	D3DXVECTOR3 pos;								//�ʒu
	D3DXVECTOR3 rot;								//����(���˂̕����ݒ�ɕK�v)
	D3DXCOLOR col;									//�F
	D3DXCOLOR colCopy;								//�F�̃R�s�[
	D3DXMATRIX mtx;									//���[���h�}�g���b�N�X
	D3DXVECTOR2 Radius;								//�傫��
	int State;										//���
	int Type;										//���
	int nLife;										//����
	int nAnimCounter;								//�A�j���J�E���^�[
	int nAnimPattern;								//�A�j���p�^�[��No.
	int nShadow;									//�e�̃C���f�b�N�X(�e�̔ԍ�)
	bool bShadow;									//�e�̎g�p��
	bool bUse;										//�g�p���Ă��邩�ǂ���
	BillboardAnim Anim;								//�A�j���[�V�����֌W
}Billboard;

//�r���{�[�h�̎��
typedef enum
{
	BILLBOARD_EXPL = 0,								//����
	BILLBOARD_BULLET,								//�e
	BILLBOARD_MAX
}Billtype;

//�I�u�W�F�N�g�̏��
typedef enum
{
	BILLBOARDSTATE_NONE = 0,							//�I�u�W�F�N�g�s�g�p���
	BILLBOARDSTATE_USE,									//�I�u�W�F�N�g�g�p��
	BILLBOARDSTATE_DELETETARGET,						//�I�u�W�F�N�g�폜���蒆
	BILLBOARDSTATE_MAX
}BILLBOARDSTATE;

//�v���g�^�C�v�錾
void InitBillboard(void);							//�r���{�[�h�̏���������
void UninitBillboard(void);							//�r���{�[�h�̏I������
void UpdateBillboard(void);							//�r���{�[�h�̍X�V����
void DrawBillboard(void);							//�r���{�[�h�̕`�揈��
void SetBillboard(D3DXVECTOR3 rot, D3DXVECTOR3 pos, int nType, D3DXVECTOR2 Radius, D3DXCOLOR col, int AnimCnt, int AnimPtrn, bool bAnim, bool bShadow);	//�r���{�[�h�̐ݒ菈��
Billboard *GetBillboard(void);						//�r���{�[�h�̎擾����

#endif