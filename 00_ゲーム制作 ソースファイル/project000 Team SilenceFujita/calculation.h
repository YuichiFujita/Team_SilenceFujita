//=============================================================================
//
//	�ėp�֐��w�b�_�[ [calculation.h]
//	Author�F���c�E�� & ��������
//
//=============================================================================
#ifndef _CALCULATION_H_		// ���̃}�N����`������Ă��Ȃ��ꍇ
#define _CALCULATION_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

//*****************************************************************************
//	�C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//	�v���g�^�C�v�錾
//*****************************************************************************
void  VecSizePos(D3DXVECTOR3 *sizePos, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);	// �l���_�̈ʒu�̌v�Z����
float LineOuterProduct(D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 pos);						// �O�ς̍��E���f����
void  LandObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, bool *pbJump);									// ���f���̒��n�̍X�V����

void RotNormalize(float *rot);			// �����̐��K��

// ���\�V���[�Y
void LoadFileChunk(bool bCurve, bool bHumanCurve, bool bStage, bool bCollision, bool bShadow, bool bObject, bool bAI);	// �t�@�C�������[�h����S�̏���

void InitAllAroundChunk(void);			// ���\�������̑S�̏���
void UninitAllAroundChunk(void);		// ���\�I���̑S�̏���
void DrawAllAroundChunk(void);			// ���\�`��̑S�̏���

void InitResultChunk(void);				// ���U���g�̏������S�̏���
void UninitResultChunk(void);			// ���U���g�̏I���S�̏���
void DrawResultChunk(void);				// ���U���g�̕`��S�̏���

#endif