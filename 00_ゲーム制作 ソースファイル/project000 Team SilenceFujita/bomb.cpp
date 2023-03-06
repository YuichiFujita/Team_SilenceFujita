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
#include "input.h"
#include "sound.h"

#include "bomb.h"
#include "bonus.h"
#include "calculation.h"
#include "player.h"
#include "car.h"
#include "police.h"

#include "particle.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAX_BOMB		(128)		// ���e�͈͓̔��\���̍ő吔
#define MAX_BARINFO		(16)		// �o���A�̂܂Ƃ܂�̍ő吔

#define BOMB_VIEW_FAR	(4000.0f)				// �{�����˂̎���̋���
#define BOMB_VIEW_ANGLE	(D3DXToRadian(90))		// �{�����˂̎���p

#define BOMB_FRONT_PLUS	(100.0f)				// �v���C���[�̑O���ʒu�̌v�Z�p
#define BOMB_LEFT_PLUS	(BOMB_RADIUS + 100.0f)	// �Ԃ̈ʒu�֌W�̐ݒ�p�x�N�g���̌v�Z�p

#define BOMB_CAR_RADIUS	(80.0f)		// �Ԃ̌��m�T�C�Y�̔��a
#define BOMB_XZ_ADD		(90.0f)		// �{�����ˈʒu�� xz���Z��
#define BOMB_Y_ADD		(155.0f)	// �{�����ˈʒu�� y���Z��

#define SHOT_BAR_POS_XZ	(5.0f)		// �o���A�̔��ˈʒu�̒�����
#define SHOT_BAR_POS_Y	(15.0f)		// �o���A�̔��ˈʒu�̒�����
#define HORIZONTAL_MOVE	(60.0f)		// ���̈ړ���
#define VERTICAL_MOVE	(45.0f)		// �c�̈ړ���

#define BARRIER_GRAVITY	(0.5f)		// ���˂��ꂽ�o���A�ɂ�����d��
#define LAND_CNT		(5)			// ���e���̃J�E���g
#define REV_UP_POS		(450.0f)	// �㏸���̕␳�l
#define REV_DOWN_POS	(0.0f)		// ���~���̕␳�l
#define POS_Y_CHANGE	(1.0f)		// �ʒu�̕ύX��
#define MOVEPOS_ADD		(0.2f)		// �㏸�E���~���̉��Z��
#define DOWN_RADIUS		(240.0f)	// ���~���̉��̎Ԃ̊m�F���a
#define CARPOS_MUL		(0.9f)		// �Ԃ̈ʒu�̏�Z��
#define SET_CNT			(1)			// �������̃J�E���g (�X�R�A�����Z���I������猸�Z)
#define ENLARGE_SCALE	(9.0f)		// �g�嗦�̕␳�l
#define REDUCE_SCALE	(0.05f)		// �k�����̕␳�l
#define SCALE_CHANGE	(0.5f)		// �g�嗦�̕ύX��

#define BOMB_BONUS_CNT	(180)		// �{����Ԃ̎��{�[�i�X������J�E���g��
#define BOMB_BONUS_END	(5)			// �{����Ԃ̃{�[�i�X�擾�̑���

//**********************************************************************************************************************
//	�\���̒�` (Bomb)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	BOMBTYPE    type;		// ���
	void       *pCar;		// �ԃA�h���X
	bool        bUse;		// �g�p��
}Bomb;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void UpdateBombData(void);											// ���e�̏��̍X�V
void SetBomb(D3DXVECTOR3 pos, BOMBTYPE type, void *pCarAddress);	// ���e�̐ݒ�

void UpdateBarrierData(void);			// �o���A�̏��̍X�V
void HomingBarrier(int nCntBarrier);	// �o���A�̃z�[�~���O
bool DownBarrier(int nCntBarrier);		// �o���A�̉��~

void UpdateBarrierInfoData(void);		// �o���A�̂܂Ƃ܂�̏��̍X�V

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
Bomb        g_aBomb[MAX_BOMB];				// ���e�̏��
Barrier     g_aBarrier[MAX_BARRIER];		// �o���A�̏��
BarrierInfo g_aBarrierInfo[MAX_BARINFO];	// �o���A�̂܂Ƃ܂�̏��

//======================================================================================================================
//	���e�̏���������
//======================================================================================================================
void InitBomb(void)
{
	// ���e�̏��̏�����
	for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
	{ // ���e�̍ő�\�������J��Ԃ�

		g_aBomb[nCntBomb].pos  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aBomb[nCntBomb].type = BOMBTYPE_CAR;					// ���
		g_aBomb[nCntBomb].pCar = NULL;							// �ԃA�h���X
		g_aBomb[nCntBomb].bUse = false;							// �g�p��
	}

	// �o���A�̏��̏�����
	for (int nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++)
	{ // �o���A�̍ő�\�������J��Ԃ�

		g_aBarrier[nCntBarrier].pos       = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
		g_aBarrier[nCntBarrier].pCarPos   = NULL;									// �Ԃ̈ʒu
		g_aBarrier[nCntBarrier].move      = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
		g_aBarrier[nCntBarrier].rot       = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ����
		g_aBarrier[nCntBarrier].scale     = D3DXVECTOR3(1.0f, 1.0f, 1.0f);			// �g�嗦
		g_aBarrier[nCntBarrier].state     = BARRIERSTATE_NONE;						// ���
		g_aBarrier[nCntBarrier].type      = BOMBTYPE_CAR;							// ���
		g_aBarrier[nCntBarrier].modelData = GetModelData(MODELTYPE_PLAYER_BARRIER);	// ���f�����
		g_aBarrier[nCntBarrier].pCar      = NULL;									// �ԃA�h���X
		g_aBarrier[nCntBarrier].nCounter  = 0;										// �o���A�J�E���^�[
		g_aBarrier[nCntBarrier].bUse      = false;									// �g�p��

		// �A�C�R���̏��̏�����
		g_aBarrier[nCntBarrier].icon.nIconID = NONE_ICON;		// �A�C�R���̃C���f�b�N�X
		g_aBarrier[nCntBarrier].icon.state   = ICONSTATE_NONE;	// �A�C�R���̏��
	}

	// �o���A�̂܂Ƃ܂�̏��̏�����
	for (int nCntBarInfo = 0; nCntBarInfo < MAX_BARINFO; nCntBarInfo++)
	{ // �o���A�̂܂Ƃ܂�̍ő�\�������J��Ԃ�

		// ��{����������
		g_aBarrierInfo[nCntBarInfo].state         = BARSCOSTATE_NONE;	// ���
		g_aBarrierInfo[nCntBarInfo].nNumBarrier   = 0;					// �g�����o���A�̑���
		g_aBarrierInfo[nCntBarInfo].nNumAddScore  = 0;					// �X�R�A���Z��
		g_aBarrierInfo[nCntBarInfo].nCounterScore = 0;					// �X�R�A���Z�J�E���^�[
		g_aBarrierInfo[nCntBarInfo].bUse          = false;				// �g�p��

		for (int nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++)
		{ // �o���A�̍ő�\�������J��Ԃ�

			// �o���A�̏���������
			g_aBarrierInfo[nCntBarInfo].pBarrier[nCntBarrier] = NULL;
		}
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
	// ���e�̏��̍X�V
	UpdateBombData();

	// �o���A�̏��̍X�V
	UpdateBarrierData();

	// �o���A�̂܂Ƃ܂�̏��̍X�V
	UpdateBarrierInfoData();
}

//======================================================================================================================
//	���e�̕`�揈��
//======================================================================================================================
void DrawBomb(void)
{
	// �ϐ���錾
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^

	// ���_�@���̎������K����L���ɂ���
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	for (int nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++)
	{ // �o���A�̍ő�\�������J��Ԃ�

		if (g_aBarrier[nCntBarrier].bUse == true)
		{ // �o���A���g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBarrier[nCntBarrier].mtxWorld);

			// �g�嗦�𔽉f
			D3DXMatrixScaling(&mtxScale, g_aBarrier[nCntBarrier].scale.x, g_aBarrier[nCntBarrier].scale.y, g_aBarrier[nCntBarrier].scale.z);
			D3DXMatrixMultiply(&g_aBarrier[nCntBarrier].mtxWorld, &g_aBarrier[nCntBarrier].mtxWorld, &mtxScale);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBarrier[nCntBarrier].rot.y, g_aBarrier[nCntBarrier].rot.x, g_aBarrier[nCntBarrier].rot.z);
			D3DXMatrixMultiply(&g_aBarrier[nCntBarrier].mtxWorld, &g_aBarrier[nCntBarrier].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aBarrier[nCntBarrier].pos.x, g_aBarrier[nCntBarrier].pos.y, g_aBarrier[nCntBarrier].pos.z);
			D3DXMatrixMultiply(&g_aBarrier[nCntBarrier].mtxWorld, &g_aBarrier[nCntBarrier].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBarrier[nCntBarrier].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aBarrier[nCntBarrier].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aBarrier[nCntBarrier].modelData.dwNumMat; nCntMat++)
			{ // �}�e���A���̐����J��Ԃ�

				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aBarrier[nCntBarrier].modelData.pTexture[nCntMat]);

				// ���f���̕`��
				g_aBarrier[nCntBarrier].modelData.pMesh->DrawSubset(nCntMat);
			}

			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}

	// ���_�@���̎������K���𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//======================================================================================================================
//	�o���A�̔��ˏ���
//======================================================================================================================
void ShotBarrier(void)
{
	// �ϐ���錾
	int nInfoID    = NONE_ID;		// �o���A�̂܂Ƃ܂�̃C���f�b�N�X
	int nBarrierID = 0;				// �o���A�̏��̃C���f�b�N�X

	// �|�C���^��錾
	Player *pPlayer = GetPlayer();	// �v���C���[�̏��
	Car    *pCar;					// �Ԃ̏��
	Police *pPolice;				// �x�@�̏��

	//------------------------------------------------------------------------------------------------------------------
	//	�o���A�̂܂Ƃ܂�̐ݒ�
	//------------------------------------------------------------------------------------------------------------------
	for (int nCntBarInfo = 0; nCntBarInfo < MAX_BARINFO; nCntBarInfo++)
	{ // �o���A�̂܂Ƃ܂�̍ő�\�������J��Ԃ�

		if (g_aBarrierInfo[nCntBarInfo].bUse == false)
		{ // �o���A�̂܂Ƃ܂肪�g�p����Ă��Ȃ��ꍇ

			// ��{����������
			g_aBarrierInfo[nCntBarInfo].state         = BARSCOSTATE_NONE;	// ���
			g_aBarrierInfo[nCntBarInfo].nNumBarrier   = 0;					// �g�����o���A�̑���
			g_aBarrierInfo[nCntBarInfo].nNumAddScore  = 0;					// �X�R�A���Z��
			g_aBarrierInfo[nCntBarInfo].nCounterScore = 0;					// �X�R�A���Z�J�E���^�[

			for (int nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++)
			{ // �o���A�̍ő�\�������J��Ԃ�

				// �o���A�̏���������
				g_aBarrierInfo[nCntBarInfo].pBarrier[nCntBarrier] = NULL;
			}

			// �g�p���Ă����Ԃɂ���
			g_aBarrierInfo[nCntBarInfo].bUse = true;

			// �C���f�b�N�X��ݒ�
			nInfoID = nCntBarInfo;

			// �����𔲂���
			break;
		}
	}

	//------------------------------------------------------------------------------------------------------------------
	//	�o���A�̐ݒ�
	//------------------------------------------------------------------------------------------------------------------
	for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
	{ // ���e�̍ő�\�������J��Ԃ�

		if (g_aBomb[nCntBomb].bUse == true)
		{ // ���e���g�p����Ă���ꍇ

			for (int nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++)
			{ // �o���A�̍ő�\�������J��Ԃ�

				if (g_aBarrier[nCntBarrier].bUse == false)
				{ // �o���A���g�p����Ă��Ȃ��ꍇ

					// ���ˈʒu��ݒ�
					g_aBarrier[nCntBarrier].pos.x = pPlayer->pos.x + sinf(pPlayer->rot.y) * BOMB_XZ_ADD;	// x
					g_aBarrier[nCntBarrier].pos.y = pPlayer->pos.y + BOMB_Y_ADD;							// y
					g_aBarrier[nCntBarrier].pos.z = pPlayer->pos.z + cosf(pPlayer->rot.y) * BOMB_XZ_ADD;	// z

					// �ړ��ʂ�ݒ�
					g_aBarrier[nCntBarrier].move.x = sinf(pPlayer->rot.y) * (SHOT_BAR_POS_XZ + pPlayer->move.x + pPlayer->boost.plusMove.x);	// x
					g_aBarrier[nCntBarrier].move.y = SHOT_BAR_POS_Y;																			// y
					g_aBarrier[nCntBarrier].move.z = cosf(pPlayer->rot.y) * (SHOT_BAR_POS_XZ + pPlayer->move.x + pPlayer->boost.plusMove.x);	// z

					// �{���̏�����
					g_aBarrier[nCntBarrier].type = g_aBomb[nCntBomb].type;	// ���
					g_aBarrier[nCntBarrier].pCar = g_aBomb[nCntBomb].pCar;	// �ԃA�h���X

					// �\���̂̏���������
					g_aBarrier[nCntBarrier].scale    = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// �g�嗦
					g_aBarrier[nCntBarrier].nCounter = 0;								// �o���A�J�E���^�[

					// ���ˏ�Ԃɂ���
					g_aBarrier[nCntBarrier].state = BARRIERSTATE_FLY;

					switch (g_aBarrier[nCntBarrier].type)
					{ // ��ނ��Ƃ̏���	
					case BOMBTYPE_CAR:		// ��

						// �_���Ă���Ԃ̃A�h���X����
						pCar = (Car*)g_aBarrier[nCntBarrier].pCar;

						// �Ԃ̈ʒu�A�h���X��ݒ�
						g_aBarrier[nCntBarrier].pCarPos = &pCar->pos;

						// �Ԃ̈ړ��ʂ�������
						pCar->move.x = 0.0f;

						// �o���A�ڋߏ�Ԃɂ���
						pCar->bombState = BOMBSTATE_BAR_NEAR;

						// �����𔲂���
						break;

					case BOMBTYPE_POLICE:	// �x�@

						// �_���Ă���x�@�̃A�h���X����
						pPolice = (Police*)g_aBarrier[nCntBarrier].pCar;

						// �x�@�̈ʒu�A�h���X��ݒ�
						g_aBarrier[nCntBarrier].pCarPos = &pPolice->pos;

						// �x�@�̈ړ��ʂ�������
						pPolice->move.x = 0.0f;

						// �o���A�ڋߏ�Ԃɂ���
						pPolice->bombState = BOMBSTATE_BAR_NEAR;

						// �����𔲂���
						break;
					}

					// �U�����[�h��ҋ@��ԂɕύX
					pPlayer->bomb.state = ATTACKSTATE_WAIT;

					// �J�E���^�[��������
					pPlayer->bomb.nCounterState = 0;

					if (nInfoID != NONE_ID)
					{ // �o���A�̂܂Ƃ܂�̃C���f�b�N�X���������ݒ肳��Ă���ꍇ

						// �o���A�̏���ݒ�
						g_aBarrierInfo[nInfoID].pBarrier[nBarrierID] = &g_aBarrier[nCntBarrier];

						// �o���A�̃C���f�b�N�X�����Z
						nBarrierID++;

						// �g�����o���A�̑��������Z
						g_aBarrierInfo[nInfoID].nNumBarrier++;
					}

					// SE�̍Đ�
					if (GetSoundType(SOUND_TYPE_SE) == true)
					{
						// ��C�̃T�E���h�iSE�j�̍Đ�
						PlaySound(SOUND_LABEL_SE_ABILITY_BOMB_000);
					}

					// �g�p���Ă����Ԃɂ���
					g_aBarrier[nCntBarrier].bUse = true;

					// �����𔲂���
					break;
				}
			}
		}
	}
}

//======================================================================================================================
//	�o���A�̎擾����
//======================================================================================================================
Barrier *GetBarrierData(void)
{
	// �o���A�̏��̐擪�A�h���X��Ԃ�
	return &g_aBarrier[0];
}

//======================================================================================================================
//	�o���A�̏�Ԃ̎擾����
//======================================================================================================================
BARRIERSTATE GetBarrierState(void *pCar)
{
	for (int nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++)
	{ // �o���A�̍ő�\�������J��Ԃ�

		if (g_aBarrier[nCntBarrier].bUse == true)
		{ // �o���A���g�p����Ă���ꍇ

			if (g_aBarrier[nCntBarrier].pCar == pCar)
			{ // �����̎ԃA�h���X�ƃo���A�̎ԃA�h���X�������ꍇ

				// ���݂̎Ԃ̃o���A��Ԃ�Ԃ�
				return g_aBarrier[nCntBarrier].state;
			}
		}
	}

	// �����A�h���X�̎Ԃ��Ȃ������ꍇ
	return BARRIERSTATE_NONE;			// �������Ȃ���Ԃ�Ԃ�
}

//======================================================================================================================
//	���e�̏��̍X�V
//======================================================================================================================
void UpdateBombData(void)
{
	// �|�C���^��錾
	Player *pPlayer = GetPlayer();		// �v���C���[�̏��
	Car    *pCar    = GetCarData();		// �Ԃ̏��
	Police *pPolice = GetPoliceData();	// �x�@�̏��
	
	// ���e�̏����Đݒ�
	for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
	{ // ���e�̍ő�\�������J��Ԃ�

		// ���e���g�p���Ă��Ȃ���Ԃɂ���
		g_aBomb[nCntBomb].bUse = false;
	}

	// �Ԃ̌��m���m�F
	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++, pCar++)
	{ // �Ԃ̍ő�\�������J��Ԃ�

		if (pCar->bUse == true
		&&  pCar->bombState != BOMBSTATE_BAR_NEAR
		&&  pCar->bombState != BOMBSTATE_BAR_IN)
		{ // �Ԃ��g�p����Ă��銎�A�o���A�֘A�̏�Ԃł͂Ȃ��ꍇ

			// ���e�̐ݒ�
			SetBomb(pCar->pos, BOMBTYPE_CAR, pCar);
		}
	}

	// �x�@�̌��m���m�F
	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++, pPolice++)
	{ // �x�@�̍ő�\�������J��Ԃ�

		if (pPolice->bUse == true
		&&  pPolice->bombState != BOMBSTATE_BAR_NEAR
		&&  pPolice->bombState != BOMBSTATE_BAR_IN)
		{ // �x�@���g�p����Ă��銎�A�o���A�֘A�̏�Ԃł͂Ȃ��ꍇ

			// ���e�̐ݒ�
			SetBomb(pPolice->pos, BOMBTYPE_POLICE, pPolice);
		}
	}
}

//======================================================================================================================
//	���e�̐ݒ�
//======================================================================================================================
void SetBomb(D3DXVECTOR3 pos, BOMBTYPE type, void *pCarAddress)
{
	// �|�C���^��錾
	Player *pPlayer = GetPlayer();		// �v���C���[�̏��
	Car    *pCar    = GetCarData();		// �Ԃ̏��
	Police *pPolice = GetPoliceData();	// �x�@�̏��

	if (CollisionSector(pPlayer->pos, pos, pPlayer->rot.y, BOMB_VIEW_FAR, BOMB_VIEW_ANGLE) == true)
	{ // �v���C���[�̎��E���̏ꍇ

		for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
		{ // ���e�̍ő�\�������J��Ԃ�

			if (g_aBomb[nCntBomb].bUse == false)
			{ // ���e���g�p����Ă��Ȃ��ꍇ

				// ������ݒ�
				g_aBomb[nCntBomb].pos  = pos;			// �ʒu
				g_aBomb[nCntBomb].type = type;			// ���
				g_aBomb[nCntBomb].pCar = pCarAddress;	// �ԃA�h���X

				// �g�p���Ă����Ԃɂ���
				g_aBomb[nCntBomb].bUse = true;

				// �����𔲂���
				break;
			}
		}

		switch (type)
		{ // ��ނ��Ƃ̏���	
		case BOMBTYPE_CAR:		// ��

			// �Ԃ̃A�h���X����
			pCar = (Car*)pCarAddress;

			// �͈͓���Ԃɂ���
			pCar->bombState = BOMBSTATE_RANGE;

			// �����𔲂���
			break;

		case BOMBTYPE_POLICE:	// �x�@

			// �x�@�̃A�h���X����
			pPolice = (Police*)pCarAddress;

			// �͈͓���Ԃɂ���
			pPolice->bombState = BOMBSTATE_RANGE;

			// �����𔲂���
			break;
		}
	}
	else
	{ // �v���C���[�̎��E�O�̏ꍇ

		switch (type)
		{ // ��ނ��Ƃ̏���	
		case BOMBTYPE_CAR:		// ��

			// �Ԃ̃A�h���X����
			pCar = (Car*)pCarAddress;

			// �͈͊O��Ԃɂ���
			pCar->bombState = BOMBSTATE_NONE;

			// �����𔲂���
			break;

		case BOMBTYPE_POLICE:	// �x�@

			// �x�@�̃A�h���X����
			pPolice = (Police*)pCarAddress;

			// �͈͊O��Ԃɂ���
			pPolice->bombState = BOMBSTATE_NONE;

			// �����𔲂���
			break;
		}
	}
}

//======================================================================================================================
//	�o���A�̏��̍X�V
//======================================================================================================================
void UpdateBarrierData(void)
{
	// �|�C���^��錾
	Car    *pCar;		// �Ԃ̏��
	Police *pPolice;	// �x�@�̏��

	for (int nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++)
	{ // �o���A�̍ő�\�������J��Ԃ�

		if (g_aBarrier[nCntBarrier].bUse == true)
		{ // �o���A���g�p����Ă���ꍇ

			switch (g_aBarrier[nCntBarrier].state)
			{ // ��Ԃ��Ƃ̏���
			case BARRIERSTATE_NONE:		// �������Ȃ����

				// ����

				// �����𔲂���
				break;

			case BARRIERSTATE_FLY:		// ���ˏ��

				// �ʒu���ړ�
				g_aBarrier[nCntBarrier].pos += g_aBarrier[nCntBarrier].move;

				// �d�͂�^����
				g_aBarrier[nCntBarrier].move.y -= BARRIER_GRAVITY;

				if (g_aBarrier[nCntBarrier].move.y <= 0.0f)
				{ // y�ړ��ʂ� 0.0f�ȉ��̏ꍇ (���~)

					// �ړ��ʂ�������
					g_aBarrier[nCntBarrier].move = D3DXVECTOR3(0.0f, 0.0f, 0.0);

					// �ǔ���Ԃɂ���
					g_aBarrier[nCntBarrier].state = BARRIERSTATE_HOMING;
				}

				// �����𔲂���
				break;

			case BARRIERSTATE_HOMING:	// �ǔ����

				// �o���A�̃z�[�~���O
				HomingBarrier(nCntBarrier);

				// �p�[�e�B�N���̐ݒ�
				SetParticle
				( // ����
					g_aBarrier[nCntBarrier].pos,		// �ʒu
					D3DXCOLOR(0.0f, 0.4f, 0.8f, 1.0f),	// �F
					PARTICLETYPE_DAMAGE,				// ���
					SPAWN_PARTICLE_DAMAGE,				// �G�t�F�N�g��
					2									// ����
				);

				// �����𔲂���
				break;

			case BARRIERSTATE_LAND:		// ���e���

				if (g_aBarrier[nCntBarrier].nCounter > 0)
				{ // �J�E���^�[�� 0���傫���ꍇ

					// �J�E���^�[�����Z
					g_aBarrier[nCntBarrier].nCounter--;
				}
				else
				{ // �J�E���^�[�� 0�ȉ��̏ꍇ

					// �g���Ԃɂ���
					g_aBarrier[nCntBarrier].state = BARRIERSTATE_ENLARGE;

					// �A�C�R���̐ݒ菈��
					g_aBarrier[nCntBarrier].icon.nIconID = SetIcon
					(
						g_aBarrier[nCntBarrier].pos,
						ICONTYPE_BARRIER,
						&g_aBarrier[nCntBarrier].icon.nIconID,
						&g_aBarrier[nCntBarrier].bUse,
						&g_aBarrier[nCntBarrier].icon.state
					);
				}

				// �����𔲂���
				break;

			case BARRIERSTATE_ENLARGE:	// �g����

				if (g_aBarrier[nCntBarrier].scale.x < ENLARGE_SCALE)
				{ // �g�嗦�����l��菬�����ꍇ

					// �g�嗦�����Z
					g_aBarrier[nCntBarrier].scale.x += SCALE_CHANGE;
					g_aBarrier[nCntBarrier].scale.y += SCALE_CHANGE;
					g_aBarrier[nCntBarrier].scale.z += SCALE_CHANGE;

					// �A�C�R���̏�Ԃ��g���Ԃɂ���
					g_aBarrier[nCntBarrier].icon.state = ICONSTATE_ENLARGE;
				}
				else
				{ // �g�嗦�����l�ȏ�̏ꍇ

					// �g�嗦��␳
					g_aBarrier[nCntBarrier].scale.x = ENLARGE_SCALE;
					g_aBarrier[nCntBarrier].scale.y = ENLARGE_SCALE;
					g_aBarrier[nCntBarrier].scale.z = ENLARGE_SCALE;

					// �㏸��Ԃɂ���
					g_aBarrier[nCntBarrier].state = BARRIERSTATE_UP;

					// �A�C�R���̏�Ԃ�ʏ��Ԃɂ���
					g_aBarrier[nCntBarrier].icon.state = ICONSTATE_NONE;
				}

				// �����𔲂���
				break;

			case BARRIERSTATE_UP:		// �㏸���

				if (g_aBarrier[nCntBarrier].pos.y < REV_UP_POS)
				{ // �o���A�̈ʒu�������W��艺�̏ꍇ

					// �ړ��ʂ����Z
					g_aBarrier[nCntBarrier].move.y += MOVEPOS_ADD;

					// �ʒu���㏸
					g_aBarrier[nCntBarrier].pos.y      +=  g_aBarrier[nCntBarrier].move.y + POS_Y_CHANGE;				// �o���A�̈ʒu
					g_aBarrier[nCntBarrier].pCarPos->y += (g_aBarrier[nCntBarrier].move.y * CARPOS_MUL) + POS_Y_CHANGE;	// �Ԃ̈ʒu
				}
				else
				{ // �o���A�̈ʒu�������W�ȏ�̏ꍇ

					// �ʒu��␳
					g_aBarrier[nCntBarrier].pos.y      = REV_UP_POS;				// �o���A�̈ʒu
					g_aBarrier[nCntBarrier].pCarPos->y = REV_UP_POS * CARPOS_MUL;	// �Ԃ̈ʒu

					// �ړ��ʂ�������
					g_aBarrier[nCntBarrier].move = D3DXVECTOR3(0.0f, 0.0f, 0.0);

					// �J�E���^�[��ݒ�
					g_aBarrier[nCntBarrier].nCounter = SET_CNT;

					// ������Ԃɂ���
					g_aBarrier[nCntBarrier].state = BARRIERSTATE_SET;
				}

				// �����𔲂���
				break;

			case BARRIERSTATE_SET:		// �������

				if (g_aBarrier[nCntBarrier].nCounter <= 0)
				{ // �J�E���^�[�� 0�ȉ��̏ꍇ

					if (GetMode() != MODE_TUTORIAL)
					{ // ���݂̃��[�h���`���[�g���A���ł͂Ȃ��ꍇ

						if (DownBarrier(nCntBarrier) == true)
						{ // ���~�\�������ꍇ

							// ���~��Ԃɂ���
							g_aBarrier[nCntBarrier].state = BARRIERSTATE_DOWN;
						}
					}
				}

				// �����𔲂���
				break;

			case BARRIERSTATE_DOWN:		// ���~���

				if (g_aBarrier[nCntBarrier].pos.y > REV_DOWN_POS)
				{ // �o���A�̈ʒu�������W����̏ꍇ

					// �ړ��ʂ����Z
					g_aBarrier[nCntBarrier].move.y += MOVEPOS_ADD;

					// �ʒu�����~
					g_aBarrier[nCntBarrier].pos.y      -=  g_aBarrier[nCntBarrier].move.y + POS_Y_CHANGE;				// �o���A�̈ʒu
					g_aBarrier[nCntBarrier].pCarPos->y -= (g_aBarrier[nCntBarrier].move.y * CARPOS_MUL) + POS_Y_CHANGE;	// �Ԃ̈ʒu
				}
				else
				{ // �o���A�̈ʒu�������W�ȉ��̏ꍇ

					// �ʒu��␳
					g_aBarrier[nCntBarrier].pos.y      = REV_DOWN_POS;	// �o���A�̈ʒu
					g_aBarrier[nCntBarrier].pCarPos->y = REV_DOWN_POS;	// �Ԃ̈ʒu

					// �ړ��ʂ�������
					g_aBarrier[nCntBarrier].move = D3DXVECTOR3(0.0f, 0.0f, 0.0);

					// �k����Ԃɂ���
					g_aBarrier[nCntBarrier].state = BARRIERSTATE_REDUCE;
				}

				// �����𔲂���
				break;

			case BARRIERSTATE_REDUCE:	// �k�����

				if (g_aBarrier[nCntBarrier].scale.x > REDUCE_SCALE)
				{ // �g�嗦�����l���傫���ꍇ

					// �g�嗦�����Z
					g_aBarrier[nCntBarrier].scale.x -= SCALE_CHANGE;
					g_aBarrier[nCntBarrier].scale.y -= SCALE_CHANGE;
					g_aBarrier[nCntBarrier].scale.z -= SCALE_CHANGE;

					// �A�C�R���̏�Ԃ��k����Ԃɂ���
					g_aBarrier[nCntBarrier].icon.state = ICONSTATE_REDUCE;
				}
				else
				{ // �g�嗦�����l�ȉ��̏ꍇ

					// �g�嗦��␳
					g_aBarrier[nCntBarrier].scale.x = REDUCE_SCALE;
					g_aBarrier[nCntBarrier].scale.y = REDUCE_SCALE;
					g_aBarrier[nCntBarrier].scale.z = REDUCE_SCALE;

					switch (g_aBarrier[nCntBarrier].type)
					{ // �o���A���̎Ԃ̎�ނ��Ƃ̏���	
					case BOMBTYPE_CAR:		// ��

						// �o���A���̎Ԃ̃A�h���X����
						pCar = (Car*)g_aBarrier[nCntBarrier].pCar;

						// �Ԃ̏�Ԃ�ʏ��Ԃɂ���
						pCar->state = CARSTATE_NORMAL;

						// �a�؃J�E���g��������
						pCar->nTrafficCnt = 0;

						// �{���̏�Ԃ��������Ȃ���Ԃɂ���
						pCar->bombState = BOMBSTATE_NONE;

						// �����𔲂���
						break;

					case BOMBTYPE_POLICE:	// �x�@

						// �o���A���̌x�@�̃A�h���X����
						pPolice = (Police*)g_aBarrier[nCntBarrier].pCar;

						// �x�@�̏�Ԃ��p�g���[����Ԃɂ���
						pPolice->state = POLICESTATE_PATROL;

						// �a�؃J�E���g��������
						pPolice->nTrafficCnt = 0;

						// �{���̏�Ԃ��������Ȃ���Ԃɂ���
						pPolice->bombState = BOMBSTATE_NONE;

						// �p�g���[���̒T�m����
						PatrolCarSearch(pPolice);

						// �����𔲂���
						break;
					}

					// �o���A�̏�Ԃ��������Ȃ���Ԃɂ���
					g_aBarrier[nCntBarrier].state = BARRIERSTATE_NONE;

					// �g�p���Ă��Ȃ���Ԃɂ���
					g_aBarrier[nCntBarrier].bUse = false;
				}

				// �����𔲂���
				break;
			}

			// �A�C�R���̈ʒu�ݒ菈��
			SetPositionIcon
			(
				g_aBarrier[nCntBarrier].icon.nIconID, 
				g_aBarrier[nCntBarrier].pos
			);
		}
	}
}

//======================================================================================================================
//	�o���A�̃z�[�~���O
//======================================================================================================================
void HomingBarrier(int nCntBarrier)
{
	// �ϐ���錾
	D3DXVECTOR3 destPos;	// �z�[�~���O�ڕW�ʒu
	D3DXVECTOR2 destRot;	// �z�[�~���O�v�Z�p

	// �|�C���^��錾
	Car    *pCar;			// �Ԃ̏��
	Police *pPolice;		// �x�@�̏��

	// �ڕW�ʒu��ݒ�
	destPos = *g_aBarrier[nCntBarrier].pCarPos;

	// �ڕW�̈ړ����������߂�
	destRot.x = atan2f(destPos.x - g_aBarrier[nCntBarrier].pos.x, destPos.z - g_aBarrier[nCntBarrier].pos.z);
	destRot.y = atan2f(destPos.x - g_aBarrier[nCntBarrier].pos.x, destPos.y - g_aBarrier[nCntBarrier].pos.y);

	// �ړ��ʂ�ݒ�
	g_aBarrier[nCntBarrier].move.x = HORIZONTAL_MOVE * sinf(destRot.x);	// x
	g_aBarrier[nCntBarrier].move.y = VERTICAL_MOVE   * cosf(destRot.y);	// y
	g_aBarrier[nCntBarrier].move.z = HORIZONTAL_MOVE * cosf(destRot.x);	// z

	// �ʒu���ړ�
	g_aBarrier[nCntBarrier].pos += g_aBarrier[nCntBarrier].move;

	if (g_aBarrier[nCntBarrier].pos.y <= 0.0f)
	{ // y���W�� 0.0f�ȉ��̏ꍇ (���e)

		switch (g_aBarrier[nCntBarrier].type)
		{ // ��ނ��Ƃ̏���
		case BOMBTYPE_CAR:		// ��

			// �_���Ă���Ԃ̃A�h���X����
			pCar = (Car*)g_aBarrier[nCntBarrier].pCar;

			// �o���A����Ԃɂ���
			pCar->bombState = BOMBSTATE_BAR_IN;

			// �����𔲂���
			break;

		case BOMBTYPE_POLICE:	// �x�@

			// �_���Ă���x�@�̃A�h���X����
			pPolice = (Police*)g_aBarrier[nCntBarrier].pCar;

			// �o���A����Ԃɂ���
			pPolice->bombState = BOMBSTATE_BAR_IN;

			// �����𔲂���
			break;
		}

		// ���W��␳
		g_aBarrier[nCntBarrier].pos.x = destPos.x;
		g_aBarrier[nCntBarrier].pos.y = 0.0f;
		g_aBarrier[nCntBarrier].pos.z = destPos.z;

		// �ړ��ʂ�������
		g_aBarrier[nCntBarrier].move = D3DXVECTOR3(0.0f, 0.0f, 0.0);

		// �J�E���^�[��ݒ�
		g_aBarrier[nCntBarrier].nCounter = LAND_CNT;

		// ���e��Ԃɂ���
		g_aBarrier[nCntBarrier].state = BARRIERSTATE_LAND;
	}
}

//======================================================================================================================
//	�o���A�̉��~
//======================================================================================================================
bool DownBarrier(int nCntBarrier)
{
	// �ϐ���錾
	float fLength;	// ���m�͈͂ƌ��m�Ԃ̋���

	// �|�C���^��錾
	Player *pPlayer = GetPlayer();		// �v���C���[�̏��
	Police *pPolice = GetPoliceData();	// �x�@�̏��
	Car    *pCar    = GetCarData();		// �Ԃ̏��

	// �v���C���[�̉�����
	if (pPlayer->bUse == true)
	{ // �v���C���[���g�p����Ă���ꍇ

		// ���m�͈͂ƌ��m�Ԃ̋��������߂�
		fLength = (g_aBarrier[nCntBarrier].pos.x - pPlayer->pos.x) * (g_aBarrier[nCntBarrier].pos.x - pPlayer->pos.x)
				+ (g_aBarrier[nCntBarrier].pos.z - pPlayer->pos.z) * (g_aBarrier[nCntBarrier].pos.z - pPlayer->pos.z);

		if (fLength < ((DOWN_RADIUS + BOMB_CAR_RADIUS) * (DOWN_RADIUS + BOMB_CAR_RADIUS)))
		{ // ���m�͈͓��̏ꍇ

			// �v���C���[�����ɂ����Ԃ�Ԃ�
			return false;
		}
	}

	// �Ԃ̉�����
	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++, pCar++)
	{ // �Ԃ̍ő�\�������J��Ԃ�

		if (pCar->bUse == true								// �Ԃ��g�p����Ă���
		&&  g_aBarrier[nCntBarrier].pCar != pCar			// �ԃA�h���X�������ł͂Ȃ�
		&&  GetBarrierState(pCar) != BARRIERSTATE_SET)		// �Ԃ̃o���A��Ԃ�������Ԃł͂Ȃ�
		{ // ��L�𖞂����Ă���ꍇ

			// ���m�͈͂ƌ��m�Ԃ̋��������߂�
			fLength = (g_aBarrier[nCntBarrier].pos.x - pCar->pos.x) * (g_aBarrier[nCntBarrier].pos.x - pCar->pos.x)
					+ (g_aBarrier[nCntBarrier].pos.z - pCar->pos.z) * (g_aBarrier[nCntBarrier].pos.z - pCar->pos.z);

			if (fLength < ((DOWN_RADIUS + BOMB_CAR_RADIUS) * (DOWN_RADIUS + BOMB_CAR_RADIUS)))
			{ // ���m�͈͓��̏ꍇ

				// �Ԃ����ɂ����Ԃ�Ԃ�
				return false;
			}
		}
	}

	// �x�@�̉�����
	for (int nCntPolice = 0; nCntPolice < MAX_POLICE; nCntPolice++, pPolice++)
	{ // �x�@�̍ő�\�������J��Ԃ�

		if (pPolice->bUse == true							// �x�@���g�p����Ă���
		&&  g_aBarrier[nCntBarrier].pCar != pCar			// �ԃA�h���X�������ł͂Ȃ�
		&&  GetBarrierState(pPolice) != BARRIERSTATE_SET)	// �x�@�̃o���A��Ԃ�������Ԃł͂Ȃ�
		{ // ��L�𖞂����Ă���ꍇ

			// ���m�͈͂ƌ��m�Ԃ̋��������߂�
			fLength = (g_aBarrier[nCntBarrier].pos.x - pPolice->pos.x) * (g_aBarrier[nCntBarrier].pos.x - pPolice->pos.x)
					+ (g_aBarrier[nCntBarrier].pos.z - pPolice->pos.z) * (g_aBarrier[nCntBarrier].pos.z - pPolice->pos.z);

			if (fLength < ((DOWN_RADIUS + BOMB_CAR_RADIUS) * (DOWN_RADIUS + BOMB_CAR_RADIUS)))
			{ // ���m�͈͓��̏ꍇ

				// �Ԃ����ɂ����Ԃ�Ԃ�
				return false;
			}
		}
	}

	// �Ԃ����ɂȂ���Ԃ�Ԃ�
	return true;
}

//======================================================================================================================
//	�o���A�̂܂Ƃ܂�̏��̍X�V
//======================================================================================================================
void UpdateBarrierInfoData(void)
{
	// �ϐ���錾
	int  nNumEvil   = 0;		// �����z�̑���
	bool bNextState = true;		// ��ԑJ�ڗp

	// �|�C���^��錾
	Car *pCar = GetCarData();	// �Ԃ̏��

	for (int nCntBarInfo = 0; nCntBarInfo < MAX_BARINFO; nCntBarInfo++)
	{ // �o���A�̂܂Ƃ܂�̍ő�\�������J��Ԃ�

		if (g_aBarrierInfo[nCntBarInfo].bUse == true)
		{ // �o���A�̂܂Ƃ܂肪�g�p����Ă��Ȃ��ꍇ

			switch (g_aBarrierInfo[nCntBarInfo].state)
			{ // ��Ԃ��Ƃ̏���
			case BARSCOSTATE_NONE:		// �������Ȃ����

				for (int nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++)
				{ // �o���A�̍ő�\�������J��Ԃ�

					if (g_aBarrierInfo[nCntBarInfo].pBarrier[nCntBarrier] != NULL)
					{ // �o���A�̏�񂪐ݒ肳��Ă���ꍇ

						if (g_aBarrierInfo[nCntBarInfo].pBarrier[nCntBarrier]->state != BARRIERSTATE_SET)
						{ // ������Ԃł͂Ȃ��ꍇ

							// ��ԑJ�ڂł��Ȃ���Ԃɂ���
							bNextState = false;

							// �����𔲂���
							break;
						}
					}
				}

				if (bNextState)
				{ // ��ԑJ�ڂ��\�������ꍇ

					// �X�R�A�̉��Z��Ԃɂ���
					g_aBarrierInfo[nCntBarInfo].state = BARSCOSTATE_ADDSCORE;
				}

				// �����𔲂���
				break;

			case BARSCOSTATE_ADDSCORE:	// �X�R�A�̉��Z���

				// �J�E���^�[�����Z
				g_aBarrierInfo[nCntBarInfo].nCounterScore++;

				if (g_aBarrierInfo[nCntBarInfo].nCounterScore >= BOMB_BONUS_CNT)
				{ // �J�E���^�[�����l�ȏ�̏ꍇ

					for (int nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++)
					{ // �o���A�̍ő�\�������J��Ԃ�

						if (g_aBarrierInfo[nCntBarInfo].pBarrier[nCntBarrier] != NULL)
						{ // �o���A�̏�񂪐ݒ肳��Ă���ꍇ

							if (g_aBarrierInfo[nCntBarInfo].pBarrier[nCntBarrier]->type == BOMBTYPE_CAR)
							{ // �ʏ�Ԃ̏ꍇ

								// �Ԃ̏���ݒ�
								pCar = (Car*)g_aBarrier[nCntBarrier].pCar;

								if (pCar->judge.state == JUDGESTATE_EVIL)
								{ // �����z�̏ꍇ

									// �����z�����Z
									nNumEvil++;
								}
							}
						}
					}

					if (nNumEvil > 0)
					{ // �����z�������ꍇ

						// �{�[�i�X�̐ݒ菈��
						SetBonus(SCORE_CAR * nNumEvil);
					}

					// �J�E���^�[��������
					g_aBarrierInfo[nCntBarInfo].nCounterScore = 0;

					// �X�R�A���Z�񐔂����Z
					g_aBarrierInfo[nCntBarInfo].nNumAddScore++;

					if (g_aBarrierInfo[nCntBarInfo].nNumAddScore >= BOMB_BONUS_END)
					{ // �X�R�A�̉��Z�����l�ȏ�s�����ꍇ

						for (int nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++)
						{ // �o���A�̍ő�\�������J��Ԃ�

							if (g_aBarrierInfo[nCntBarInfo].pBarrier[nCntBarrier] != NULL)
							{ // �o���A�̏�񂪐ݒ肳��Ă���ꍇ

								// ��ԊǗ��J�E���^�[�����Z (�o���A�̏�Ԃ����~��ԂɈڍs)
								g_aBarrierInfo[nCntBarInfo].pBarrier[nCntBarrier]->nCounter--;
							}
						}

						// �g�p���Ă��Ȃ���Ԃɂ���
						g_aBarrierInfo[nCntBarInfo].bUse = false;
					}
				}

				// �����𔲂���
				break;
			}
		}
	}
}

#ifdef _DEBUG	// �f�o�b�O����
#endif