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