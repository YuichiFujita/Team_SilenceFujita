//======================================================================================================================
//
//	�S�[���� [death.cpp]
//	Author�F��������
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "shadow.h"
#include "calculation.h"

#include "death.h"
#include "particle.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define DEATH_INIT_POS		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		// �S�[�̏����ʒu
#define DEATH_INIT_ROT		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		// �S�[�̏�������
#define DEATH_SMOKE_CNT		(30)								// �������o��J�E���g��
#define DEATH_SMOKE_COL		(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f))	// �����̐F
#define DEATH_ADD_ROT		(40)								// �����̒ǉ���

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
Death g_death;				// �S�[�̏��

//======================================================================================================================
//	�S�[�̏���������
//======================================================================================================================
void InitDeath(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	 // ��{���̏�����
	g_death.pos = DEATH_INIT_POS;				// �ʒu
	g_death.rot = DEATH_INIT_ROT;				// ����
	g_death.nShadowID = NONE_SHADOW;			// �e
	g_death.nSmokeCnt = 0;						// �����̃J�E���g
	g_death.bUse = false;						// �g�p��

	// ���f�����̏�����
	g_death.modelData.dwNumMat = 0;					// �}�e���A���̐�
	g_death.modelData.pTexture = NULL;				// �e�N�X�`���ւ̃|�C���^
	g_death.modelData.pMesh = NULL;					// ���b�V�� (���_���) �ւ̃|�C���^
	g_death.modelData.pBuffMat = NULL;				// �}�e���A���ւ̃|�C���^
	g_death.modelData.vtxMin = INIT_VTX_MIN;		// �ŏ��̒��_���W
	g_death.modelData.vtxMax = INIT_VTX_MAX;		// �ő�̒��_���W
	g_death.modelData.size = INIT_SIZE;				// �傫��
	g_death.modelData.fRadius = 0.0f;				// ���a
}

//======================================================================================================================
//	�S�[�̏I������
//======================================================================================================================
void UninitDeath(void)
{

}

//======================================================================================================================
//	�S�[�̍X�V����
//======================================================================================================================
void UpdateDeath(void)
{
	if (g_death.bUse == true)
	{ // �S�[���g�p����Ă���ꍇ

		// �����̃J�E���g�����Z����
		g_death.nSmokeCnt++;

		if (g_death.nSmokeCnt % DEATH_SMOKE_CNT == 0)
		{ // �����J�E���g����萔�ɒB�����ꍇ

			// �p�[�e�B�N���̐ݒ菈��
			SetParticle
			(
				g_death.pos,
				DEATH_SMOKE_COL,
				PARTICLETYPE_PLAY_SMOKE,
				SPAWN_PARTICLE_PLAY_SMOKE,
				3
			);
		}

		// �e�̈ʒu�ݒ菈��
		SetPositionShadow
		(
			g_death.nShadowID,
			g_death.pos,
			g_death.rot,
			NONE_SCALE
		);
	}
}

//======================================================================================================================
//	�S�[�̕`�揈��
//======================================================================================================================
void DrawDeath(void)
{
	// �ϐ���錾
	D3DXMATRIX   mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^

	if (g_death.bUse == true)
	{ // �S�[���g�p����Ă���ꍇ

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_death.mtxWorld);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_death.rot.y, g_death.rot.x, g_death.rot.z);
		D3DXMatrixMultiply(&g_death.mtxWorld, &g_death.mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_death.pos.x, g_death.pos.y, g_death.pos.z);
		D3DXMatrixMultiply(&g_death.mtxWorld, &g_death.mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_death.mtxWorld);

		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_death.modelData.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_death.modelData.dwNumMat; nCntMat++)
		{ // �}�e���A���̐����J��Ԃ�

			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// �ʏ�

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_death.modelData.pTexture[nCntMat]);

			// ���f���̕`��
			g_death.modelData.pMesh->DrawSubset(nCntMat);
		}

		// �ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//======================================================================================================================
//	�S�[�̐ݒ菈��
//======================================================================================================================
void SetDeath(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (g_death.bUse == false)
	{ // �S�[���g�p����Ă��Ȃ��ꍇ

		// �ϐ�����
		g_death.pos = pos;		// �ʒu
		g_death.rot = rot;		// ����
		g_death.rot.y -= D3DXToRadian(DEATH_ADD_ROT);		// �����������X����

		// �����̐��K��
		RotNormalize(&g_death.rot.y);

		g_death.nSmokeCnt = 0;	// �����J�E���g
		g_death.bUse = true;	// �g�p���Ă����Ԃɂ���

		// ���f������ݒ�
		g_death.modelData = GetModelData(MODELTYPE_PLAYER_DEATH);	// ���f�����

		// �e�̃C���f�b�N�X��ݒ�
		g_death.nShadowID = SetModelShadow
		( // ����
			g_death.modelData,			// ���f�����
			&g_death.nShadowID,			// �e�̃C���f�b�N�X
			&g_death.bUse				// �g�p��
		);

		// �e�̈ʒu�ݒ�
		SetPositionShadow(g_death.nShadowID, g_death.pos, g_death.rot, NONE_SCALE);
	}
}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================

#endif