//============================================================
//
//	�����\������ [3Dnotation.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "3Dnotation.h"
#include "model.h"
#include "calculation.h"
#include "tutorial.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_3D_NOTATION		(128)		// �����\���̍ő吔
#define NOTA_MOVE_ROT		(0.02f)		// �����\���̉�]��

//************************************************************
//	�\���̒�` (3DNotation)
//************************************************************
typedef struct
{
	D3DXVECTOR3  pos;					// �ʒu
	D3DXVECTOR3  rot;					// ����
	D3DXMATRIX   mtxWorld;				// ���[���h�}�g���b�N�X
	NOTATIONTYPE type;					// ���
	Model        modelData;				// ���f�����
	int         *p3DNotationIDParent;	// �e�̋����\���C���f�b�N�X
	bool        *pUseParent;			// �e�̎g�p��
	bool         bUse;					// �g�p��
} Notation3D;

//************************************************************
//	�v���g�^�C�v�錾
//************************************************************
void MoveNotation(Notation3D *pNota);					// �����\���̓���̍X�V����
void FalseNotation(Notation3D *pNota, int nCntNota);	// �����\���̍폜�̍X�V����

//************************************************************
//	�O���[�o���ϐ�
//************************************************************
Notation3D g_a3DNotation[MAX_3D_NOTATION];		// �����\���̏��
float      g_fNotationRot;						// �����\���̌���

//============================================================
//	�����\���̏���������
//============================================================
void Init3DNotation(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// �����\���̌�����������
	g_fNotationRot = D3DX_PI;

	// �����\���̏��̏�����
	for (int nCntNota = 0; nCntNota < MAX_3D_NOTATION; nCntNota++)
	{ // �����\���̍ő�\�������J��Ԃ�

		// ��{����������
		g_a3DNotation[nCntNota].pos                 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_a3DNotation[nCntNota].rot                 = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);	// ����
		g_a3DNotation[nCntNota].type                = NOTATIONTYPE_BREAK;				// ���
		g_a3DNotation[nCntNota].p3DNotationIDParent = NULL;								// �e�̋����\���C���f�b�N�X
		g_a3DNotation[nCntNota].pUseParent          = NULL;								// �e�̎g�p��
		g_a3DNotation[nCntNota].bUse                = false;							// �g�p��

		// ���f�����̏�����
		g_a3DNotation[nCntNota].modelData.dwNumMat = 0;				// �}�e���A���̐�
		g_a3DNotation[nCntNota].modelData.pTexture = NULL;			// �e�N�X�`���ւ̃|�C���^
		g_a3DNotation[nCntNota].modelData.pMesh    = NULL;			// ���b�V�� (���_���) �ւ̃|�C���^
		g_a3DNotation[nCntNota].modelData.pBuffMat = NULL;			// �}�e���A���ւ̃|�C���^
		g_a3DNotation[nCntNota].modelData.dwNumMat = 0;				// �}�e���A���̐�
		g_a3DNotation[nCntNota].modelData.vtxMin   = INIT_VTX_MIN;	// �ŏ��̒��_���W
		g_a3DNotation[nCntNota].modelData.vtxMax   = INIT_VTX_MAX;	// �ő�̒��_���W
		g_a3DNotation[nCntNota].modelData.size     = INIT_SIZE;		// �傫��
		g_a3DNotation[nCntNota].modelData.fRadius  = 0.0f;			// ���a
	}
}

//============================================================
//	�����\���̏I������
//============================================================
void Uninit3DNotation(void)
{

}

//============================================================
//	�����\���̍X�V����
//============================================================
void Update3DNotation(void)
{
	// ���������Z
	g_fNotationRot += NOTA_MOVE_ROT;

	// �����𐳋K��
	RotNormalize(&g_fNotationRot);

	for (int nCntNota = 0; nCntNota < MAX_3D_NOTATION; nCntNota++)
	{ // �����\���̍ő�\�������J��Ԃ�

		if (g_a3DNotation[nCntNota].bUse == true)
		{ // �����\�����g�p����Ă���ꍇ

			// �����\���̓���̍X�V
			MoveNotation(&g_a3DNotation[nCntNota]);

			// �����\���̍폜�̍X�V
			FalseNotation(&g_a3DNotation[nCntNota], nCntNota);
		}
	}
}

//============================================================
//	�����\���̕`�揈��
//============================================================
void Draw3DNotation(void)
{
	// �ϐ���錾
	D3DXMATRIX   mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p
	bool         bDrawExit = false;				// Exit�\���̗L��

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^

	switch (GetMode())
	{ // ���[�h���Ƃ̏���
	case MODE_TUTORIAL:	// �`���[�g���A��

		if (GetLessonState() >= LESSON_07)
		{ // ���b�X��7�ɒ��풆�A�܂��̓N���A���Ă���ꍇ

			// Exit��\����Ԃɂ���
			bDrawExit = true;
		}

		// �����𔲂���
		break;

	case MODE_GAME:		// �Q�[��

		// Exit��\����Ԃɂ���
		bDrawExit = true;

		// �����𔲂���
		break;
	}

	for (int nCntNota = 0; nCntNota < MAX_3D_NOTATION; nCntNota++)
	{ // �����\���̍ő�\�������J��Ԃ�

		if (g_a3DNotation[nCntNota].bUse == true)
		{ // �����\�����g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_a3DNotation[nCntNota].mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_a3DNotation[nCntNota].rot.y, g_a3DNotation[nCntNota].rot.x, g_a3DNotation[nCntNota].rot.z);
			D3DXMatrixMultiply(&g_a3DNotation[nCntNota].mtxWorld, &g_a3DNotation[nCntNota].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_a3DNotation[nCntNota].pos.x, g_a3DNotation[nCntNota].pos.y, g_a3DNotation[nCntNota].pos.z);
			D3DXMatrixMultiply(&g_a3DNotation[nCntNota].mtxWorld, &g_a3DNotation[nCntNota].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_a3DNotation[nCntNota].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_a3DNotation[nCntNota].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_a3DNotation[nCntNota].modelData.dwNumMat; nCntMat++)
			{ // �}�e���A���̐����J��Ԃ�

				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_a3DNotation[nCntNota].modelData.pTexture[nCntMat]);

				switch (g_a3DNotation[nCntNota].type)
				{ // ��ނ��Ƃ̏���
				case NOTATIONTYPE_EXIT:	// �E�o

					if (bDrawExit)
					{ // �E�o���\����Ԃ̏ꍇ

						// ���f���̕`��
						g_a3DNotation[nCntNota].modelData.pMesh->DrawSubset(nCntMat);
					}

					// �����𔲂���
					break;

				default:				// ��L�ȊO

					// ���f���̕`��
					g_a3DNotation[nCntNota].modelData.pMesh->DrawSubset(nCntMat);

					// �����𔲂���
					break;
				}
			}

			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//============================================================
//	�����\���̐ݒ菈��
//============================================================
int Set3DNotation(NOTATIONTYPE type, int *p3DNotationID, bool *pUse)
{
	for (int nCntNota = 0; nCntNota < MAX_3D_NOTATION; nCntNota++)
	{ // �����\���̍ő�\�������J��Ԃ�

		if (g_a3DNotation[nCntNota].bUse == false)
		{ // �����\�����g�p����Ă��Ȃ��ꍇ

			// ��ނ�ݒ�
			g_a3DNotation[nCntNota].type = type;

			// �����̃A�h���X����
			g_a3DNotation[nCntNota].p3DNotationIDParent = p3DNotationID;	// �e�̋����\���C���f�b�N�X
			g_a3DNotation[nCntNota].pUseParent          = pUse;				// �e�̎g�p��

			// ����������
			g_a3DNotation[nCntNota].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
			g_a3DNotation[nCntNota].rot = D3DXVECTOR3(0.0f, g_fNotationRot, 0.0f);	// ����

			switch (type)
			{ // �����̎�ނ��Ƃ̏���
			case NOTATIONTYPE_BREAK:	// �j��

				// ���f���̏���ݒ�
				g_a3DNotation[nCntNota].modelData = GetModelData(MODELTYPE_FONT_BREAK);

				// �����𔲂���
				break;

			case NOTATIONTYPE_SHOT:		// �ˏo

				// ���f���̏���ݒ�
				g_a3DNotation[nCntNota].modelData = GetModelData(MODELTYPE_FONT_SHOT);

				// �����𔲂���
				break;

			case NOTATIONTYPE_FLY:		// ����

				// ���f���̏���ݒ�
				g_a3DNotation[nCntNota].modelData = GetModelData(MODELTYPE_FONT_FLY);

				// �����𔲂���
				break;

			case NOTATIONTYPE_EXIT:		// �E�o

				// ���f���̏���ݒ�
				g_a3DNotation[nCntNota].modelData = GetModelData(MODELTYPE_FONT_EXIT);

				// �����𔲂���
				break;
			}

			// �g�p���Ă����Ԃɂ���
			g_a3DNotation[nCntNota].bUse = true;

			// �g�p�ł��鋭���\����ID��Ԃ�
			return nCntNota;
		}
	}

	// �����\���̑���������Ȃ��ꍇ -1��Ԃ�
	return -1;
}

//============================================================
//	�����\���̈ʒu�̐ݒ菈��
//============================================================
void SetPosition3DNotation(int n3DNotationID, D3DXVECTOR3 pos)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	if (n3DNotationID > NONE_3D_NOTATION)
	{ // �g�p�ł���ID�̏ꍇ

		// �����̋����\���̈ʒu��ݒ�
		g_a3DNotation[n3DNotationID].pos = pos;
	}
}

//============================================================
//	�����\���̓���̍X�V����
//============================================================
void MoveNotation(Notation3D *pNota)
{
	// ��������
	pNota->rot.y = g_fNotationRot;
}

//============================================================
//	�����\���̍폜�̍X�V����
//============================================================
void FalseNotation(Notation3D *pNota, int nCntNota)
{
	if (pNota->pUseParent != NULL)
	{ // �e�̎g�p�󋵃A�h���X���ݒ肳��Ă���ꍇ

		if (*pNota->pUseParent == false)
		{ // �e���g�p����Ă��Ȃ��ꍇ

			// �����\�����g�p���Ă��Ȃ���Ԃɂ���
			pNota->bUse = false;
		}
		else
		{ // �e���g�p����Ă���ꍇ

			if (pNota->p3DNotationIDParent != NULL)
			{ // �e�̋����\���C���f�b�N�X�A�h���X���ݒ肳��Ă���ꍇ

				if (*pNota->p3DNotationIDParent != nCntNota)
				{ // �e�̋����\���C���f�b�N�X�ƁA���݂̋����\���C���f�b�N�X�̒l���Ⴄ�ꍇ

					// �����\�����g�p���Ă��Ȃ���Ԃɂ���
					pNota->bUse = false;

					// [��] �g�p���Ȃ���ԂɂȂ����e�����t���[�����œ����C���f�b�N�X���g����
					//      �܂��g�p���ꂽ�ꍇ�ɋ����\������̐e�ɓ��������Ă��܂����߁A���̖h�~�B
				}
			}
		}
	}
}