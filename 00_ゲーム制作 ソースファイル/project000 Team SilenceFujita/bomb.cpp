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
#include "Police.h"

#include "input.h"
#include "particle.h"
#include "object.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAX_BOMB		(128)		// ���e�͈͓̔��\���̍ő吔

#define BOMB_PULS		(400.0f)	// �v���C���[�ʒu����{�����m�̒��S�ʒu�ւ̂���
#define BOMB_RADIUS		(400.0f)	// �{�����m�͈͂̔��a
#define BOMB_CAR_RADIUS	(80.0f)		// �Ԃ̌��m�T�C�Y�̔��a

#define BOMB_FRONT_PLUS	(100.0f)				// �v���C���[�̑O���ʒu�̌v�Z�p
#define BOMB_LEFT_PLUS	(BOMB_RADIUS + 100.0f)	// �Ԃ̈ʒu�֌W�̐ݒ�p�x�N�g���̌v�Z�p

//**********************************************************************************************************************
//	�񋓌^��` (CARTYPE)
//**********************************************************************************************************************
typedef enum
{
	CARTYPE_NORMAL = 0,		// �ʏ��
	CARTYPE_POLICE,			// �x�@��
	CARTYPE_MAX,			// ���̗񋓌^�̑���
} CARTYPE;

//**********************************************************************************************************************
//	�\���̒�` (Bomb)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	BOMBSTATE   state;		// ���
	CARTYPE     type;		// ���
	void       *pCar;		// �ԃA�h���X
	float       fOrder;		// ���苗��
	bool        bUse;		// �g�p��
}Bomb;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void SetBomb(D3DXVECTOR3 pos, CARTYPE type, void *pCar);	// �{���̐ݒ�

void ResetBomb(void);		// �{���̍Đݒ�
void CollisionBomb(void);	// �{�����m�͈͂̓����蔻��
void SortBomb(void);		// �{���̔z��̃\�[�g
int  CurrentAim(void);		// �I�𒆂̎ԗ�

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
int     g_nCurrentMaxBomb;	// ���ݑ_�����̎Ԃ̑���
CARTYPE g_nType;			// ���ݑ_�����̎Ԃ̎��
int     g_nID;				// ���ݑ_�����̎Ԃ�ID
void   *g_pCurrentAim;		// ���ݑ_�����̎Ԃ̃A�h���X
Bomb    g_aBomb[MAX_BOMB];	// ���e�̏��

//======================================================================================================================
//	���e�̏���������
//======================================================================================================================
void InitBomb(void)
{
	// �O���[�o���ϐ���������
	g_nCurrentMaxBomb = 0;			// ���ݑ_�����̎Ԃ̑���
	g_nType       = CARTYPE_NORMAL;	// ���ݑ_�����̎Ԃ̎��
	g_nID         = 0;				// ���ݑ_�����̎Ԃ�ID
	g_pCurrentAim = NULL;			// ���ݑ_�����̎Ԃ̃A�h���X

	// ���e�̏��̏�����
	for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
	{ // ���e�̍ő�\�������J��Ԃ�

		g_aBomb[nCntBomb].pos    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aBomb[nCntBomb].state  = BOMBSTATE_NONE;					// ���
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
	// �ϐ���錾
	int nAim = -1;

	// �{���̍Đݒ�
	ResetBomb();

	// �{�����m�͈͂̓����蔻��
	CollisionBomb();

	// �{���̔z��̃\�[�g
	SortBomb();

	if (g_nCurrentMaxBomb > 0)
	{

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{

			g_pCurrentAim = g_aBomb[g_nID].pCar;
			g_nID = (g_nID + 1) % g_nCurrentMaxBomb;
		}
	}

	// �I�𒆂̎ԗ�
	nAim = CurrentAim();

	if (nAim == -1)
	{
		g_pCurrentAim = NULL;
	}

	if (g_aBomb[nAim].bUse == true)
	{ // 

		// �p�[�e�B�N���̐ݒ�
		SetParticle
		( // ����
			g_aBomb[nAim].pos,					// �ʒu
			D3DXCOLOR(0.8f, 0.0f, 0.3f, 1.0f),	// �F
			PARTICLETYPE_DAMAGE,				// ���
			SPAWN_PARTICLE_DAMAGE,				// �G�t�F�N�g��
			2									// ����
		);
	}
}

//======================================================================================================================
//	���e�̕`�揈��
//======================================================================================================================
void DrawBomb(void)
{
	
}

//======================================================================================================================
//	�{���̐ݒ�
//======================================================================================================================
void SetBomb(D3DXVECTOR3 pos, CARTYPE type, void *pCar)
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
	Object *pObject = GetObjectData();	// �I�u�W�F�N�g�̏��

	// �{���̌��m�͈͂̒��S��ݒ�
	rangePos.x = pPlayer->pos.x + sinf(pPlayer->rot.y) * BOMB_PULS;
	rangePos.y = pPlayer->pos.y + 0.0f;
	rangePos.z = pPlayer->pos.z + cosf(pPlayer->rot.y) * BOMB_PULS;

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

				// �{���̐ݒ�
				SetBomb(pPolice->pos, CARTYPE_POLICE, pPolice);
			}
		}
	}

	// �I�u�W�F�N�g�̌��m���m�F
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (pObject->bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// ���m�͈͂ƌ��m�Ԃ̋��������߂�
			fLength = (rangePos.x - pObject->pos.x) * (rangePos.x - pObject->pos.x)
					+ (rangePos.z - pObject->pos.z) * (rangePos.z - pObject->pos.z);

			if (fLength < ((BOMB_RADIUS + BOMB_CAR_RADIUS) * (BOMB_RADIUS + BOMB_CAR_RADIUS)))
			{ // ���m�͈͓��̏ꍇ

				// �{���̐ݒ�
				SetBomb(pObject->pos, CARTYPE_NORMAL, pObject);
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

		// ������ID�ύX�H
	}
}

//======================================================================================================================
//	�I�𒆂̎ԗ�
//======================================================================================================================
int CurrentAim(void)
{
	for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
	{ // ���e�̍ő�\�������J��Ԃ�

		if (g_aBomb[nCntBomb].bUse == true)
		{ // ���e���g�p����Ă���ꍇ

			if (g_pCurrentAim == g_aBomb[nCntBomb].pCar)
			{ // �A�h���X�������ꍇ (�I�𒆂̎�)

				// �I�𒆂̃C���f�b�N�X��Ԃ�
				return nCntBomb;
			}
		}
	}

	// �I�𒆂̃C���f�b�N�X���Ȃ��ꍇ
	return -1;	// -1��Ԃ�
}

#ifdef _DEBUG	// �f�o�b�O����
#endif