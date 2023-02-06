//======================================================================================================================
//
//	���e���� [bomb.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "bomb.h"
#include "calculation.h"
#include "player.h"
#include "car.h"
#include "police.h"

#include "input.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAX_BOMB		(128)		// ���e�͈͓̔��\���̍ő吔

#define BOMB_PULS		(1200.0f)	// �v���C���[�ʒu����{�����m�̒��S�ʒu�ւ̂���
#define BOMB_RADIUS		(1000.0f)	// �{�����m�͈͂̔��a
#define BOMB_CAR_RADIUS	(80.0f)		// �Ԃ̌��m�T�C�Y�̔��a

#define BOMB_FRONT_PLUS	(100.0f)				// �v���C���[�̑O���ʒu�̌v�Z�p
#define BOMB_LEFT_PLUS	(BOMB_RADIUS + 100.0f)	// �Ԃ̈ʒu�֌W�̐ݒ�p�x�N�g���̌v�Z�p

//**********************************************************************************************************************
//	�񋓌^��` (BOMBTYPE)
//**********************************************************************************************************************
typedef enum
{
	BOMBTYPE_CAR = 0,		// ��
	BOMBTYPE_POLICE,		// �x�@
	BOMBTYPE_MAX,			// ���̗񋓌^�̑���
} BOMBTYPE;

//**********************************************************************************************************************
//	�\���̒�` (Bomb)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	BOMBTYPE    type;		// ���
	void       *pCar;		// �ԃA�h���X
	float       fOrder;		// ���苗��
	bool        bUse;		// �g�p��
}Bomb;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void SetBomb(D3DXVECTOR3 pos, BOMBTYPE type, void *pCar);	// �{���̐ݒ�

void ResetBomb(void);		// �{���̍Đݒ�
void CollisionBomb(void);	// �{�����m�͈͂̓����蔻��
void SortBomb(void);		// �{���̔z��̃\�[�g
void ChangeAim(void);		// �_���̕ύX
void CurrentAim(void);		// �I�𒆂̎ԗ�����
void SetAim(void);			// �_����Ԃ̐ݒ�

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
int   g_nCurrentMaxBomb;	// ���ݑ_�����̎Ԃ̑���
int   g_nID;				// ���ݑ_�����̎Ԃ�ID
void *g_pCurrentAim;		// ���ݑ_�����̎Ԃ̃A�h���X
Bomb  g_aBomb[MAX_BOMB];	// ���e�̏��

//======================================================================================================================
//	���e�̏���������
//======================================================================================================================
void InitBomb(void)
{
	// �O���[�o���ϐ���������
	g_nCurrentMaxBomb = 0;		// ���ݑ_�����̎Ԃ̑���
	g_nID             = 0;		// ���ݑ_�����̎Ԃ�ID
	g_pCurrentAim     = NULL;	// ���ݑ_�����̎Ԃ̃A�h���X

	// ���e�̏��̏�����
	for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
	{ // ���e�̍ő�\�������J��Ԃ�

		g_aBomb[nCntBomb].pos    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aBomb[nCntBomb].pCar   = NULL;							// �ԃA�h���X
		g_aBomb[nCntBomb].fOrder = 0.0f;							// ���苗��
		g_aBomb[nCntBomb].bUse   = false;							// �g�p��
	}
}

//======================================================================================================================
//	���e�̏I������
//======================================================================================================================
void UninitBomb(void)
{
	
}

//======================================================================================================================
//	���e�̍X�V����
//======================================================================================================================
void UpdateBomb(void)
{
	// �|�C���^��錾
	Player *pPlayer = GetPlayer();	// �v���C���[�̏��

	if (pPlayer->atkState == ATTACKSTATE_BOMB)
	{ // �v���C���[�̍U����Ԃ��{���̏ꍇ

		// �{���̍Đݒ�
		ResetBomb();

		// �{�����m�͈͂̓����蔻��
		CollisionBomb();

		// �{���̔z��̃\�[�g
		SortBomb();

		// �_���̕ύX
		ChangeAim();

		// �I�𒆂̎ԗ�����
		CurrentAim();

		// �_����Ԃ̐ݒ�
		SetAim();
	}
	else
	{ // �v���C���[�̍U����Ԃ�����ȊO�̏ꍇ

		// ���ݑ_�����̎Ԃ�ID��������
		g_nID = 0;

		// ���ݑ_�����̎Ԃ�������
		g_pCurrentAim = NULL;
	}
}

//======================================================================================================================
//	���e�̕`�揈��
//======================================================================================================================
void DrawBomb(void)
{
	
}

//======================================================================================================================
//	���e�̔��ˏ���
//======================================================================================================================
void ShotBomb(void)
{
	// �|�C���^��錾
	Player *pPlayer = GetPlayer();	// �v���C���[�̏��
	Car    *pCar;					// �Ԃ̏��
	Police *pPolice;				// �x�@�̏��

	if (g_pCurrentAim != NULL)
	{ // ���ݑ_���Ă���Ԃ̃A�h���X�� NULL�ł͂Ȃ��ꍇ

		if (g_aBomb[g_nID].bUse == true)
		{ // ���ݑ_�����̎Ԃ��g�p����Ă���ꍇ

			switch (g_aBomb[g_nID].type)
			{ // ���ݑ_�����̎Ԃ̎�ނ��Ƃ̏���
			case BOMBTYPE_CAR:		// ��

				// �_���Ă���Ԃ̃A�h���X����
				pCar = (Car*)g_aBomb[g_nID].pCar;



				// �����𔲂���
				break;

			case BOMBTYPE_POLICE:	// �x�@

				// �_���Ă���x�@�̃A�h���X����
				pPolice = (Police*)g_aBomb[g_nID].pCar;



				// �����𔲂���
				break;
			}

			// �U�����[�h��ʏ��ԂɕύX
			pPlayer->atkState = ATTACKSTATE_NORMAL;
		}
	}
}

//======================================================================================================================
//	�{���̐ݒ�
//======================================================================================================================
void SetBomb(D3DXVECTOR3 pos, BOMBTYPE type, void *pCar)
{
	// �ϐ���錾
	D3DXVECTOR3 playerPos;			// �v���C���[�̈ʒu
	D3DXVECTOR3 frontPos;			// �v���C���[�̑O���ʒu

	// �|�C���^��錾
	Player *pPlayer = GetPlayer();	// �v���C���[�̏��

	// �v���C���[�̈ʒu����
	playerPos = D3DXVECTOR3(pPlayer->pos.x, 0.0f, pPlayer->pos.z);

	// �v���C���[�̑O���ʒu��ݒ�
	frontPos.x = pPlayer->pos.x + sinf(pPlayer->rot.y) * BOMB_FRONT_PLUS;
	frontPos.y = 0.0f;
	frontPos.z = pPlayer->pos.z + cosf(pPlayer->rot.y) * BOMB_FRONT_PLUS;

	// �v���C���[�̈ʒu�A�O���ʒu�����ɂ��炷
	playerPos.x += sinf(pPlayer->rot.y - (D3DX_PI * 0.5f)) * BOMB_LEFT_PLUS;
	playerPos.z += cosf(pPlayer->rot.y - (D3DX_PI * 0.5f)) * BOMB_LEFT_PLUS;
	frontPos.x  += sinf(pPlayer->rot.y - (D3DX_PI * 0.5f)) * BOMB_LEFT_PLUS;
	frontPos.z  += cosf(pPlayer->rot.y - (D3DX_PI * 0.5f)) * BOMB_LEFT_PLUS;

	for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
	{ // ���e�̍ő�\�������J��Ԃ�

		if (g_aBomb[nCntBomb].bUse == false)
		{ // ���e���g�p����Ă��Ȃ��ꍇ

			// ������ݒ�
			g_aBomb[nCntBomb].pos  = pos;	// �ʒu
			g_aBomb[nCntBomb].type = type;	// ���
			g_aBomb[nCntBomb].pCar = pCar;	// �ԃA�h���X

			// ���苗�������߂�
			g_aBomb[nCntBomb].fOrder = LineOuterProduct(frontPos, playerPos, pos);

			// ���ݑ_���Ă���Ԃ̑��������Z
			g_nCurrentMaxBomb++;

			// �g�p���Ă����Ԃɂ���
			g_aBomb[nCntBomb].bUse = true;

			// �����𔲂���
			break;
		}
	}
}

//======================================================================================================================
//	�{���̍Đݒ�
//======================================================================================================================
void ResetBomb(void)
{
	// ���ݑ_���Ă���Ԃ̑�����������
	g_nCurrentMaxBomb = 0;

	// �{���̎g�p�󋵂�������
	for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
	{ // ���e�̍ő�\�������J��Ԃ�

		// ���苗����������
		g_aBomb[nCntBomb].fOrder = 0.0f;

		// �g�p���Ă��Ȃ���Ԃɂ���
		g_aBomb[nCntBomb].bUse = false;
	}
}

//======================================================================================================================
//	�{�����m�͈͂̓����蔻��
//======================================================================================================================
void CollisionBomb(void)
{
	// �ϐ���錾
	D3DXVECTOR3 rangePos;	// �{���̌��m�͈͂̒��S
	float       fLength;	// ���m�͈͂ƌ��m�Ԃ̋���

	// �|�C���^��錾
	Player *pPlayer = GetPlayer();		// �v���C���[�̏��
	Police *pPolice = GetPoliceData();	// �x�@�̏��
	Car    *pCar    = GetCarData();		// �Ԃ̏��

	// �{���̌��m�͈͂̒��S��ݒ�
	rangePos.x = pPlayer->pos.x + sinf(pPlayer->rot.y) * BOMB_PULS;
	rangePos.y = pPlayer->pos.y + 0.0f;
	rangePos.z = pPlayer->pos.z + cosf(pPlayer->rot.y) * BOMB_PULS;

	// �Ԃ̌��m���m�F
	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++, pCar++)
	{ // �Ԃ̍ő�\�������J��Ԃ�

		if (pCar->bUse == true)
		{ // �Ԃ��g�p����Ă���ꍇ

			// ���m�͈͂ƌ��m�Ԃ̋��������߂�
			fLength = (rangePos.x - pCar->pos.x) * (rangePos.x - pCar->pos.x)
					+ (rangePos.z - pCar->pos.z) * (rangePos.z - pCar->pos.z);

			if (fLength < ((BOMB_RADIUS + BOMB_CAR_RADIUS) * (BOMB_RADIUS + BOMB_CAR_RADIUS)))
			{ // ���m�͈͓��̏ꍇ

				// �{���̏�Ԃ�͈͓���Ԃɂ���
				pCar->bombState = BOMBSTATE_RANGE;

				// �{���̐ݒ�
				SetBomb(pCar->pos, BOMBTYPE_CAR, pCar);
			}
			else
			{ // ���m�͈͊O�̏ꍇ

				// �{���̏�Ԃ��������Ȃ���Ԃɂ���
				pCar->bombState = BOMBSTATE_NONE;
			}
		}
	}

	// �x�@�̌��m���m�F
	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++, pPolice++)
	{ // �x�@�̍ő�\�������J��Ԃ�

		if (pPolice->bUse == true)
		{ // �x�@���g�p����Ă���ꍇ

			// ���m�͈͂ƌ��m�Ԃ̋��������߂�
			fLength = (rangePos.x - pPolice->pos.x) * (rangePos.x - pPolice->pos.x)
					+ (rangePos.z - pPolice->pos.z) * (rangePos.z - pPolice->pos.z);

			if (fLength < ((BOMB_RADIUS + BOMB_CAR_RADIUS) * (BOMB_RADIUS + BOMB_CAR_RADIUS)))
			{ // ���m�͈͓��̏ꍇ

				// �{���̏�Ԃ�͈͓���Ԃɂ���
				pPolice->bombState = BOMBSTATE_RANGE;

				// �{���̐ݒ�
				SetBomb(pPolice->pos, BOMBTYPE_POLICE, pPolice);
			}
			else
			{ // ���m�͈͊O�̏ꍇ

				// �{���̏�Ԃ��������Ȃ���Ԃɂ���
				pPolice->bombState = BOMBSTATE_NONE;
			}
		}
	}
}

//======================================================================================================================
//	�{���̔z��̃\�[�g
//======================================================================================================================
void SortBomb(void)
{
	// �ϐ���錾
	Bomb keepBomb;		// �\�[�g����ւ����̑���p
	int  nNowMaxID;		// �ő�l�������Ă���z��̗v�f�ԍ��̋L���p

	for (int nCntBomb = 0; nCntBomb < (g_nCurrentMaxBomb - 1); nCntBomb++)
	{ // ���ݑ_�����̎Ԃ̑��� -1�񕪌J��Ԃ�

		// ���݂̌J��Ԃ������� (�v�f1�Ƃ���)
		nNowMaxID = nCntBomb;

		for (int nCntSort = nCntBomb + 1; nCntSort < g_nCurrentMaxBomb; nCntSort++)
		{ // ���ݑ_�����̎Ԃ̑������J��Ԃ�

			if (g_aBomb[nNowMaxID].fOrder < g_aBomb[nCntSort].fOrder)
			{ // �ő�l�ɐݒ肳��Ă���l���A���݂̒l�̂ق����傫���ꍇ

				// ���݂̗v�f�ԍ����ő�l�ɐݒ�
				nNowMaxID = nCntSort;
			}
		}

		if (nCntBomb != nNowMaxID)
		{ // �ő�l�̗v�f�ԍ��ɕϓ����������ꍇ

			// �l�̓���ւ�
			keepBomb           = g_aBomb[nCntBomb];
			g_aBomb[nCntBomb]  = g_aBomb[nNowMaxID];
			g_aBomb[nNowMaxID] = keepBomb;
		}
	}
}

//======================================================================================================================
//	�_���̕ύX
//======================================================================================================================
void ChangeAim(void)
{
	if (g_nCurrentMaxBomb > 0)
	{ // �_����Ԃ� 0��葽���ꍇ

		if (GetKeyboardTrigger(DIK_RIGHT) == true || GetJoyKeyTrigger(JOYKEY_R1, 0)/* || GetJoyStickPressRX(0) > 0*/)
		{ // �E�̎Ԃ֕ύX����{�����s��ꂽ�ꍇ

			// �I�����Ă���Ԃ̃C���f�b�N�X��ύX
			g_nID = (g_nID + 1) % g_nCurrentMaxBomb;

			// ���ݑ_���Ă���Ԃ̃A�h���X��ύX
			g_pCurrentAim = g_aBomb[g_nID].pCar;
		}
		else if (GetKeyboardTrigger(DIK_LEFT) == true || GetJoyKeyTrigger(JOYKEY_L1, 0)/* || GetJoyStickPressRX(0) < 0*/)
		{ // ���̎Ԃ֕ύX����{�����s��ꂽ�ꍇ

			// �I�����Ă���Ԃ̃C���f�b�N�X��ύX
			g_nID = (g_nID + (g_nCurrentMaxBomb - 1)) % g_nCurrentMaxBomb;

			// ���ݑ_���Ă���Ԃ̃A�h���X��ύX
			g_pCurrentAim = g_aBomb[g_nID].pCar;
		}

		if (g_pCurrentAim == NULL)
		{ // ���ݑ_���Ă���Ԃ̃A�h���X�� NULL�̏ꍇ

			// ��ԍ��ɂ���Ԃ̃A�h���X��ݒ�
			g_pCurrentAim = g_aBomb[0].pCar;
		}
	}
}

//======================================================================================================================
//	�I�𒆂̎ԗ�����
//======================================================================================================================
void CurrentAim(void)
{
	// �ϐ���錾
	int nAim = -1;

	for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
	{ // ���e�̍ő�\�������J��Ԃ�

		if (g_aBomb[nCntBomb].bUse == true)
		{ // ���e���g�p����Ă���ꍇ

			if (g_pCurrentAim == g_aBomb[nCntBomb].pCar)
			{ // �A�h���X�������ꍇ (�I�𒆂̎�)

				// ���ݑ_�����̎Ԃ�ID��ۑ�
				nAim = nCntBomb;

				// �����𔲂���
				break;
			}
		}
	}

	if (nAim != -1)
	{ // ���ݑ_�����̎ԂƓ����A�h���X�̎Ԃ����݂����ꍇ

		// ���ݑ_�����̎Ԃ�ID���Đݒ�
		g_nID = nAim;
	}
	else
	{ // ���ݑ_�����̎ԂƓ����A�h���X�̎Ԃ����݂��Ȃ������ꍇ

		// ���ݑ_�����̎Ԃ�ID��������
		g_nID = 0;

		// ���ݑ_�����̎Ԃ�������
		g_pCurrentAim = NULL;
	}
}

//======================================================================================================================
//	�_����Ԃ̐ݒ�
//======================================================================================================================
void SetAim(void)
{
	// �|�C���^��錾
	Car    *pCar;		// �Ԃ̏��
	Police *pPolice;	// �x�@�̏��

	if (g_aBomb[g_nID].bUse == true)
	{ // ���ݑ_�����̎Ԃ��g�p����Ă���ꍇ

		switch (g_aBomb[g_nID].type)
		{ // ���ݑ_�����̎Ԃ̎�ނ��Ƃ̏���
		case BOMBTYPE_CAR:		// ��

			// �_���Ă���Ԃ̃A�h���X����
			pCar = (Car*)g_aBomb[g_nID].pCar;

			// �_���Ă����Ԃɂ���
			pCar->bombState = BOMBSTATE_AIM;

			// �����𔲂���
			break;

		case BOMBTYPE_POLICE:	// �x�@

			// �_���Ă���x�@�̃A�h���X����
			pPolice = (Police*)g_aBomb[g_nID].pCar;

			// �_���Ă����Ԃɂ���
			pPolice->bombState = BOMBSTATE_AIM;

			// �����𔲂���
			break;
		}
	}
}

#ifdef _DEBUG	// �f�o�b�O����
#endif