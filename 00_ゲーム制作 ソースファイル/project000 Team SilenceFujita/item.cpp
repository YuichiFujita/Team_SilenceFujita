//======================================================================================================================
//
//	�A�C�e������ [item.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "input.h"

#include "item.h"
#include "calculation.h"
#include "particle.h"
#include "player.h"
#include "shadow.h"
#include "sound.h"
//#include "tutorial.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define ITEM_SCALE				(D3DXVECTOR3(0.5f, 0.5f, 0.5f))		// �A�C�e���̊g�嗦
#define ITEM_HEAL				(5)		// �A�C�e���̉񕜗�
#define EFFECT_TIME_ITEM		(160)	// �p�[�e�B�N�����o���Ԋu
#define MOVE_ROT_ITEM			(0.03f)	// �A�C�e���̉�]��
#define ITEM_LOST_COUNT			(600)	// �A�C�e����������J�E���g��

#define ITEM_POS_DEST_UP		(100.0f)	// �A�C�e���̖ڕW�̈ʒu(�㏸���)
#define ITEM_POS_DEST_DOWN		(50.0f)		// �A�C�e���̖ڕW�̈ʒu(���~���)

#define ITEM_LOST_EFFECT_COL	(D3DXCOLOR(0.1f,0.1f,1.0f,1.0f))	// �A�C�e�������̃G�t�F�N�g�̐F
#define ITEM_GET_EFFECT_COL		(D3DXCOLOR(0.1f,0.1f,1.0f,1.0f))	// �A�C�e���擾�̃G�t�F�N�g�̐F
#define ITEM_MARK_EFFECT_COL	(D3DXCOLOR(0.1f,0.9f,1.0f,1.0f))	// �A�C�e���̖ڈ�̃G�t�F�N�g�̐F

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apModelItem[] =			// ���f���̑��΃p�X
{
	"data\\MODEL_ITEM\\heal000.x",	// �񕜃A�C�e���̃��f�����΃p�X
};

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void TutorialItem(void);			// �`���[�g���A���̃A�C�e���̍X�V����
void GameItem(void);				// �Q�[���̃A�C�e���̍X�V����

void CollisionPlayer(Item *pItem);	// �A�C�e���ƃv���C���[�̓����蔻��

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
Item  g_aItem[MAX_ITEM];			// �A�C�e���̏��

//======================================================================================================================
//	�A�C�e���̏���������
//======================================================================================================================
void InitItem(void)
{
	// �A�C�e���̏��̏�����
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{ // �A�C�e���̍ő�\�������J��Ԃ�

		g_aItem[nCntItem].pos            = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aItem[nCntItem].move			 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
		g_aItem[nCntItem].rot            = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aItem[nCntItem].nType          = 0;								// ���
		g_aItem[nCntItem].nCounterEffect = 0;								// �G�t�F�N�g�Ǘ��J�E���^�[
		g_aItem[nCntItem].nShadowID      = NONE_SHADOW;						// �e�̃C���f�b�N�X
		g_aItem[nCntItem].nLostCounter	 = 0;								// �����J�E���^�[
		g_aItem[nCntItem].bUse           = false;							// �g�p��

		// ��Ԋ֌W�̏�����
		g_aItem[nCntItem].stateInfo.fPosDest = 0.0f;						// �ڕW�̈ʒu
		g_aItem[nCntItem].stateInfo.state	 = ITEMSTATE_UP;				// ���
		g_aItem[nCntItem].stateInfo.nCounter = 0;							// �J�E���^�[

		// ���f�����̏�����
		g_aItem[nCntItem].modelData.dwNumMat = 0;							// �}�e���A���̐�
		g_aItem[nCntItem].modelData.pTexture = NULL;						// �e�N�X�`���ւ̃|�C���^
		g_aItem[nCntItem].modelData.pMesh	 = NULL;						// ���b�V�� (���_���) �ւ̃|�C���^
		g_aItem[nCntItem].modelData.pBuffMat = NULL;						// �}�e���A���ւ̃|�C���^
		g_aItem[nCntItem].modelData.dwNumMat = 0;							// �}�e���A���̐�
		g_aItem[nCntItem].modelData.vtxMin	 = INIT_VTX_MIN;				// �ŏ��̒��_���W
		g_aItem[nCntItem].modelData.vtxMax	 = INIT_VTX_MAX;				// �ő�̒��_���W
		g_aItem[nCntItem].modelData.size	 = INIT_SIZE;					// �傫��
		g_aItem[nCntItem].modelData.fRadius	 = 0.0f;						// ���a
	}
}

//======================================================================================================================
//	�A�C�e���̏I������
//======================================================================================================================
void UninitItem(void)
{

}

//======================================================================================================================
//	�A�C�e���̍X�V����
//======================================================================================================================
void UpdateItem(void)
{
	switch (GetMode())
	{ // ���[�h���Ƃ̍X�V
	case MODE_TUTORIAL:	// �`���[�g���A�����

		// �`���[�g���A���̃A�C�e���̍X�V
		TutorialItem();

		// �����𔲂���
		break;

	case MODE_GAME:		// �Q�[�����

		// �Q�[���̃A�C�e���̍X�V
		GameItem();

		// �����𔲂���
		break;
	}
}

//======================================================================================================================
//	�A�C�e���̕`�揈��
//======================================================================================================================
void DrawItem(void)
{
	// �ϐ���錾
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{ // �A�C�e���̍ő�\�������J��Ԃ�

		if (g_aItem[nCntItem].bUse == true)
		{ // �A�C�e�����g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aItem[nCntItem].mtxWorld);

			// �g�嗦�𔽉f
			D3DXMatrixScaling(&mtxScale, ITEM_SCALE.x, ITEM_SCALE.y, ITEM_SCALE.z);
			D3DXMatrixMultiply(&g_aItem[nCntItem].mtxWorld, &g_aItem[nCntItem].mtxWorld, &mtxScale);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aItem[nCntItem].rot.y, g_aItem[nCntItem].rot.x, g_aItem[nCntItem].rot.z);
			D3DXMatrixMultiply(&g_aItem[nCntItem].mtxWorld, &g_aItem[nCntItem].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y, g_aItem[nCntItem].pos.z);
			D3DXMatrixMultiply(&g_aItem[nCntItem].mtxWorld, &g_aItem[nCntItem].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aItem[nCntItem].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aItem[nCntItem].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aItem[nCntItem].modelData.dwNumMat; nCntMat++)
			{ // �}�e���A���̐����J��Ԃ�

				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aItem[nCntItem].modelData.pTexture[nCntMat]);

				// ���f���̕`��
				g_aItem[nCntItem].modelData.pMesh->DrawSubset(nCntMat);
			}

			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	�A�C�e���̐ݒ菈��
//======================================================================================================================
void SetItem(D3DXVECTOR3 pos, int nType)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{ // �A�C�e���̍ő�\�������J��Ԃ�

		if (g_aItem[nCntItem].bUse == false)
		{ // �A�C�e�����g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aItem[nCntItem].pos	 = D3DXVECTOR3(pos.x, pos.y + 50.0f, pos.z);		// �ʒu
			g_aItem[nCntItem].nType  = nType;	// ���

			// �A�C�e���̏���������
			g_aItem[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
			g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
			g_aItem[nCntItem].nCounterEffect = 0;					// �G�t�F�N�g�Ǘ��J�E���^�[
			g_aItem[nCntItem].nLostCounter = 0;						// �����J�E���^�[

			// �g�p���Ă����Ԃɂ���
			g_aItem[nCntItem].bUse = true;

			// ���f������ݒ�
			g_aItem[nCntItem].modelData = GetModelData(MODELTYPE_PLAYER_BARRIER);

			// �e�̃C���f�b�N�X��ݒ�
			g_aItem[nCntItem].nShadowID = SetCircleShadow
			( // ����
				0.5f,										// ���l
				g_aItem[nCntItem].modelData.fRadius,		// ���a
				&g_aItem[nCntItem].nShadowID,				// �e�̐e�̉e�C���f�b�N�X
				&g_aItem[nCntItem].bUse						// �e�̐e�̎g�p��
			);

			// �e�̈ʒu�ݒ�
			SetPositionShadow
			(
				g_aItem[nCntItem].nShadowID,		// �e�̃C���f�b�N�X
				g_aItem[nCntItem].pos,				// �ʒu
				g_aItem[nCntItem].rot,				// ����
				NONE_SCALE							// �g�嗦
			);

			// ��Ԋ֌W�̐ݒ�
			g_aItem[nCntItem].stateInfo.fPosDest = ITEM_POS_DEST_UP;		// �ڕW�̈ʒu
			g_aItem[nCntItem].stateInfo.state	 = ITEMSTATE_UP;			// ���
			g_aItem[nCntItem].stateInfo.nCounter = 0;						// �J�E���^�[

			// �����𔲂���
			break;
		}
	}
}

//======================================================================================================================
//	�A�C�e���ƃv���C���[�̓����蔻��
//======================================================================================================================
void CollisionPlayer(Item *pItem)
{
	// �ϐ���錾
	float fLength = 0.0f;				// �A�C�e���ƃv���C���[�Ƃ̊Ԃ̋���

	// �|�C���^��錾
	Player *pPlayer = GetPlayer();	// �v���C���[�̏��

	if (pPlayer->bUse == true)
	{ // �v���C���[���g�p����Ă���ꍇ

		// �A�C�e���ƃv���C���[�Ƃ̊Ԃ̋��������߂�
		fLength = (pItem->pos.x - pPlayer->pos.x) * (pItem->pos.x - pPlayer->pos.x)
				+ (pItem->pos.z - pPlayer->pos.z) * (pItem->pos.z - pPlayer->pos.z);

		if (fLength < ((pItem->modelData.fRadius + PLAY_WIDTH) * (pItem->modelData.fRadius + PLAY_WIDTH)))
		{ // �A�C�e�����v���C���[�ɓ������Ă���ꍇ

			// �p�[�e�B�N���̐ݒ�
			SetParticle
			( // ����
				pItem->pos,							// �ʒu
				ITEM_GET_EFFECT_COL,				// �F
				PARTICLETYPE_EXPLOSION,				// ���
				SPAWN_PARTICLE_EXPLOSION,			// �G�t�F�N�g��
				1									// ����
			);

			// �o���A�̉񕜔���
			HealBarrier(pPlayer, ITEM_HEAL);	

			// �g�p���Ă��Ȃ���Ԃɂ���
			pItem->bUse = false;
		}
	}
}

//======================================================================================================================
//	�`���[�g���A���̃A�C�e���̍X�V����
//======================================================================================================================
void TutorialItem(void)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{ // �A�C�e���̍ő�\�������J��Ԃ�

		if (g_aItem[nCntItem].bUse == true)
		{ // �A�C�e�����g�p����Ă���ꍇ

		  //--------------------------------------------------------------------------------------------------------------
		  //	�����̍X�V
		  //--------------------------------------------------------------------------------------------------------------
		  // �����̍X�V
			g_aItem[nCntItem].rot.y += MOVE_ROT_ITEM;

			// �����̐��K��
			RotNormalize(&g_aItem[nCntItem].rot.y);

			switch (g_aItem[nCntItem].stateInfo.state)
			{
			case ITEMSTATE_UP:		// �㏸���

									// �ړ��ʂ�ݒ肷��
				g_aItem[nCntItem].move.y = 1.0f;

				// �ړ��ʂ����Z����
				g_aItem[nCntItem].pos.y += g_aItem[nCntItem].move.y;

				if (g_aItem[nCntItem].pos.y >= g_aItem[nCntItem].stateInfo.fPosDest)
				{ // �ʒu���ڕW�Ɉ��ʋ߂Â����ꍇ

				  // �ʒu��␳����
					g_aItem[nCntItem].pos.y = g_aItem[nCntItem].stateInfo.fPosDest;

					// �ڕW�̈ʒu��ݒ肷��
					g_aItem[nCntItem].stateInfo.fPosDest = ITEM_POS_DEST_DOWN;

					// ��Ԃ�ݒ肷��
					g_aItem[nCntItem].stateInfo.state = (ITEMSTATE)((g_aItem[nCntItem].stateInfo.state + 1) % ITEMSTATE_MAX);
				}

				break;

			case ITEMSTATE_DOWN:	// ���~���

									// �ړ��ʂ�ݒ肷��
				g_aItem[nCntItem].move.y = -1.0f;

				// �ړ��ʂ����Z����
				g_aItem[nCntItem].pos.y += g_aItem[nCntItem].move.y;

				if (g_aItem[nCntItem].pos.y <= g_aItem[nCntItem].stateInfo.fPosDest)
				{ // �ʒu���ڕW�Ɉ��ʋ߂Â����ꍇ

				  // �ʒu��␳����
					g_aItem[nCntItem].pos.y = g_aItem[nCntItem].stateInfo.fPosDest;

					// �ڕW�̈ʒu��ݒ肷��
					g_aItem[nCntItem].stateInfo.fPosDest = ITEM_POS_DEST_UP;

					// ��Ԃ�ݒ肷��
					g_aItem[nCntItem].stateInfo.state = (ITEMSTATE)((g_aItem[nCntItem].stateInfo.state + 1) % ITEMSTATE_MAX);
				}

				break;
			}

			//--------------------------------------------------------------------------------------------------------------
			//	�p�[�e�B�N���̍X�V
			//--------------------------------------------------------------------------------------------------------------
			if (g_aItem[nCntItem].nCounterEffect < EFFECT_TIME_ITEM)
			{ // �J�E���^�[�����l��菬�����ꍇ

			  // �J�E���^�[�����Z
				g_aItem[nCntItem].nCounterEffect++;
			}
			else
			{ // �J�E���^�[�����l�ȏ�̏ꍇ

			  // �J�E���^�[��������
				g_aItem[nCntItem].nCounterEffect = 0;

				// �p�[�e�B�N���̐ݒ�
				SetParticle
				( // ����
					g_aItem[nCntItem].pos,				// �ʒu
					ITEM_MARK_EFFECT_COL,				// �F
					PARTICLETYPE_DAMAGE,				// ���
					SPAWN_PARTICLE_DAMAGE,				// �G�t�F�N�g��
					1									// ����
				);
			}

			// �����J�E���g�����Z����
			g_aItem[nCntItem].nLostCounter++;

			if (g_aItem[nCntItem].nLostCounter % ITEM_LOST_COUNT == 0)
			{ // �����J�E���g����萔�ɂȂ����ꍇ

				// �p�[�e�B�N���̐ݒ菈��
				SetParticle
				(
					g_aItem[nCntItem].pos,
					ITEM_LOST_EFFECT_COL,
					PARTICLETYPE_ITEM_LOST,
					SPAWN_PARTICLE_ITEM_LOST,
					3
				);

				// �����J�E���g������������
				g_aItem[nCntItem].nLostCounter = 0;

				// �g�p���Ȃ�
				g_aItem[nCntItem].bUse = false;
			}

			//------------------------------------------------------------------------------------------------------
			//	�����蔻��
			//------------------------------------------------------------------------------------------------------
			// �A�C�e���ƃv���C���[�̓����蔻��
			CollisionPlayer(&g_aItem[nCntItem]);

			//------------------------------------------------------------------------------------------------------
			//	�e�̍X�V
			//------------------------------------------------------------------------------------------------------
			// �e�̈ʒu�ݒ�
			SetPositionShadow
			(
				g_aItem[nCntItem].nShadowID,		// �e�̃C���f�b�N�X
				D3DXVECTOR3(g_aItem[nCntItem].pos.x, 0.0f, g_aItem[nCntItem].pos.z),				// �ʒu
				g_aItem[nCntItem].rot,				// ����
				ITEM_SCALE							// �g�嗦
			);
		}
	}
}

//======================================================================================================================
//	�Q�[���̃A�C�e���̍X�V����
//======================================================================================================================
void GameItem(void)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{ // �A�C�e���̍ő�\�������J��Ԃ�

		if (g_aItem[nCntItem].bUse == true)
		{ // �A�C�e�����g�p����Ă���ꍇ

			//--------------------------------------------------------------------------------------------------------------
			//	�����̍X�V
			//--------------------------------------------------------------------------------------------------------------
			// �����̍X�V
			g_aItem[nCntItem].rot.y += MOVE_ROT_ITEM;

			// �����̐��K��
			RotNormalize(&g_aItem[nCntItem].rot.y);

			switch (g_aItem[nCntItem].stateInfo.state)
			{
			case ITEMSTATE_UP:		// �㏸���

				// �ړ��ʂ�ݒ肷��
				g_aItem[nCntItem].move.y = 1.0f;

				// �ړ��ʂ����Z����
				g_aItem[nCntItem].pos.y += g_aItem[nCntItem].move.y;

				if (g_aItem[nCntItem].pos.y >= g_aItem[nCntItem].stateInfo.fPosDest)
				{ // �ʒu���ڕW�Ɉ��ʋ߂Â����ꍇ

					// �ʒu��␳����
					g_aItem[nCntItem].pos.y = g_aItem[nCntItem].stateInfo.fPosDest;

					// �ڕW�̈ʒu��ݒ肷��
					g_aItem[nCntItem].stateInfo.fPosDest = ITEM_POS_DEST_DOWN;

					// ��Ԃ�ݒ肷��
					g_aItem[nCntItem].stateInfo.state = (ITEMSTATE)((g_aItem[nCntItem].stateInfo.state + 1) % ITEMSTATE_MAX);
				}

				break;

			case ITEMSTATE_DOWN:	// ���~���

				// �ړ��ʂ�ݒ肷��
				g_aItem[nCntItem].move.y = -1.0f;

				// �ړ��ʂ����Z����
				g_aItem[nCntItem].pos.y += g_aItem[nCntItem].move.y;

				if (g_aItem[nCntItem].pos.y <= g_aItem[nCntItem].stateInfo.fPosDest)
				{ // �ʒu���ڕW�Ɉ��ʋ߂Â����ꍇ

					// �ʒu��␳����
					g_aItem[nCntItem].pos.y = g_aItem[nCntItem].stateInfo.fPosDest;

					// �ڕW�̈ʒu��ݒ肷��
					g_aItem[nCntItem].stateInfo.fPosDest = ITEM_POS_DEST_UP;

					// ��Ԃ�ݒ肷��
					g_aItem[nCntItem].stateInfo.state = (ITEMSTATE)((g_aItem[nCntItem].stateInfo.state + 1) % ITEMSTATE_MAX);
				}

				break;
			}

			//--------------------------------------------------------------------------------------------------------------
			//	�p�[�e�B�N���̍X�V
			//--------------------------------------------------------------------------------------------------------------
			if (g_aItem[nCntItem].nCounterEffect < EFFECT_TIME_ITEM)
			{ // �J�E���^�[�����l��菬�����ꍇ

				// �J�E���^�[�����Z
				g_aItem[nCntItem].nCounterEffect++;
			}
			else
			{ // �J�E���^�[�����l�ȏ�̏ꍇ

				// �J�E���^�[��������
				g_aItem[nCntItem].nCounterEffect = 0;

				// �p�[�e�B�N���̐ݒ�
				SetParticle
				( // ����
					g_aItem[nCntItem].pos,				// �ʒu
					ITEM_MARK_EFFECT_COL,				// �F
					PARTICLETYPE_DAMAGE,				// ���
					SPAWN_PARTICLE_DAMAGE,				// �G�t�F�N�g��
					1									// ����
				);
			}

			// �����J�E���g�����Z����
			g_aItem[nCntItem].nLostCounter++;

			if (g_aItem[nCntItem].nLostCounter % ITEM_LOST_COUNT == 0)
			{ // �����J�E���g����萔�ɂȂ����ꍇ

				// �p�[�e�B�N���̐ݒ菈��
				SetParticle
				(
					g_aItem[nCntItem].pos,
					ITEM_LOST_EFFECT_COL,
					PARTICLETYPE_ITEM_LOST,
					SPAWN_PARTICLE_ITEM_LOST,
					3
				);

				// �����J�E���g������������
				g_aItem[nCntItem].nLostCounter = 0;

				// �g�p���Ȃ�
				g_aItem[nCntItem].bUse = false;
			}

			//------------------------------------------------------------------------------------------------------
			//	�����蔻��
			//------------------------------------------------------------------------------------------------------
			// �A�C�e���ƃv���C���[�̓����蔻��
			CollisionPlayer(&g_aItem[nCntItem]);

			//------------------------------------------------------------------------------------------------------
			//	�e�̍X�V
			//------------------------------------------------------------------------------------------------------
			// �e�̈ʒu�ݒ�
			SetPositionShadow
			(
				g_aItem[nCntItem].nShadowID,		// �e�̃C���f�b�N�X
				D3DXVECTOR3(g_aItem[nCntItem].pos.x, 0.0f, g_aItem[nCntItem].pos.z),				// �ʒu
				g_aItem[nCntItem].rot,				// ����
				ITEM_SCALE							// �g�嗦
			);
		}
	}
}

//======================================================================================================================
//	�A�C�e���̎擾����
//======================================================================================================================
Item *GetItem(void)
{
	// �A�C�e���̏��̐擪�A�h���X��Ԃ�
	return &g_aItem[0];
}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================
//**********************************************************************************************************************
//	�A�C�e���̑����擾����
//**********************************************************************************************************************
int GetNumItem(void)
{
	// �ϐ���錾
	int nNumItem = 0;	// �A�C�e���̑����̊m�F�p

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{ // �A�C�e���̍ő�\�������J��Ԃ�

		if (g_aItem[nCntItem].bUse == true)
		{ // �A�C�e�����g�p����Ă���ꍇ

			// �J�E���^�[�����Z
			nNumItem++;
		}
	}

	// �ϐ��̒l��Ԃ�
	return nNumItem;	// �A�C�e���̑���
}
#endif