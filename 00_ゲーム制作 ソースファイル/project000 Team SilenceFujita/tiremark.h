//===================================
//
//�^�C�����w�b�_�[[tiremark.h]
//Author ��������
//
//===================================
#ifndef _TIREMARK_H_
#define _TIREMARK_H_

#include "main.h"

//�}�N����`
#define MAX_TIREMARK			(1024)	// �Z�b�g�}�[�N�̍ő吔

//�^�C�����̍\����
typedef struct
{
	D3DXVECTOR3 pos;					// �ʒu
	D3DXVECTOR3 rot;					// ����
	D3DXMATRIX	mtx;					// ���[���h�}�g���b�N�X
	D3DXCOLOR	col;					// �F
	int			nClearCount;			// ������J�E���g
	float		fWidth;					// ���̍L��
	float		fDepth;					// �c�̍L��
	bool		bUse;					// �g�p���Ă��邩�ǂ���
}TireMark;

//�v���g�^�C�v�錾
void InitTireMark(void);				// �^�C�����̏���������
void UninitTireMark(void);				// �^�C�����̏I������
void UpdateTireMark(void);				// �^�C�����̍X�V����
void DrawTireMark(void);				// �^�C�����̕`�揈��
void SetTireMark(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// �^�C�����̐ݒ菈��
TireMark *GetTireMark(void);			// �^�C�����̎擾����

#endif