//======================================================================================================================
//
//	�����@(�t���C�E�A�E�F�C)���� [wind.cpp]
//	Author�F��������
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "wind.h"
#include "player.h"
#include "calculation.h"

#include "camera.h"
#include "Combo.h"
#include "input.h"
#include "item.h"
#include "particle.h"
#include "object.h"
#include "Human.h"
#include "score.h"
#include "bonus.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAX_WIND			(1024)			// ���̍ő吔
#define HUMAN_FLY_POS_Y		(50.0f)			// ������Ԕ��肪���鍂��
#define WIND_WIDTH			(1000.0f)		// ���͈̔�(X��)
#define WIND_DEPTH			(150.0f)		// ���͈̔�(Z��)
#define FLYAWAY_WIDTH		(100.0f)		// ������ԕ�
#define FLYAWAY_HEIGHT		(25.0f)			// ������ԍ���
#define FLYAWAY_DEPTH		(100.0f)		// ������ԍ���

#define ITEM_WIND_COUNT		(5)								// �A�C�e����������J�E���g��

//**********************************************************************************************************************
//	�\���̒�` (Bomb)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 move;		// �ړ���
	D3DXMATRIX  mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXCOLOR   col;		// �F
	int         nLife;		// ����
	float       fRadius;	// ���a
	float       fAlpha;		// 1F���Ƃ̔����Ȃ��
	bool        bUse;		// �g�p��
}Wind;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void FlyAwayHuman(Human *pHuman, Player player);					// �l�Ԃ�������΂���鏈��

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureWind = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWind = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
Wind g_aWind[MAX_WIND];								// ���̍\����
WindInfo g_WindInfo;								// ���̏��̍\����

int		  g_nHumanItemCount;					// �A�C�e����������J�E���g

//======================================================================================================================
//	�����@�̏���������
//======================================================================================================================
void InitWind(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_3D *pVtx;							// ���_���ւ̃|�C���^

	for (int nCnt = 0; nCnt < MAX_WIND; nCnt++)
	{ // ���̏���
		g_aWind[nCnt].pos	  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_aWind[nCnt].move	  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
		g_aWind[nCnt].col	  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
		g_aWind[nCnt].fAlpha  = 0.0f;								// �����x
		g_aWind[nCnt].fRadius = 0.0f;								// ���a
		g_aWind[nCnt].nLife	  = 0;									// ����
		g_aWind[nCnt].bUse	  = false;								// �g�p��
	}

	// ���̏��̏�����
	g_WindInfo.nUseCounter = 0;										// ���̃J�E���^�[������������
	g_WindInfo.nOverHeatCounter = 0;								// �I�[�o�[�q�[�g�J�E���^�[������������
	g_WindInfo.state = WIND_USABLE;									// �g�p�\���

	// �A�C�e����������J�E���g������������
	g_nHumanItemCount = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/effect000.jpg",
		&g_pTextureWind);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_3D) * 4 * MAX_WIND,		// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffWind,
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWind->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWind = 0; nCntWind < MAX_WIND; nCntWind++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWind->Unlock();
}

//======================================================================================================================
//	�����@�̏I������
//======================================================================================================================
void UninitWind(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureWind != NULL)
	{ // �ϐ� (g_pTextureWind) ��NULL�ł͂Ȃ��ꍇ
		g_pTextureWind->Release();
		g_pTextureWind = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffWind != NULL)
	{ // �ϐ� (g_pVtxBuffWind) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffWind->Release();
		g_pVtxBuffWind = NULL;
	}
}

//======================================================================================================================
//	�����@�̍X�V����
//======================================================================================================================
void UpdateWind(void)
{
	Player *pPlayer = GetPlayer();				// �v���C���[�̏����擾����
	VERTEX_3D *pVtx;							// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWind->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWind = 0; nCntWind < MAX_WIND; nCntWind++)
	{
		if (g_aWind[nCntWind].bUse == true)
		{ // �g�p�����ꍇ
			//�ړ�������
			g_aWind[nCntWind].pos += g_aWind[nCntWind].move;
			g_aWind[nCntWind].pos.x += (sinf(pPlayer->rot.y) * pPlayer->move.x);
			g_aWind[nCntWind].pos.z += (cosf(pPlayer->rot.y) * pPlayer->move.x);

			//���������Z������
			g_aWind[nCntWind].nLife--;

			//�����ɂ��Ă���
			g_aWind[nCntWind].col.a -= g_aWind[nCntWind].fAlpha;

			if (g_aWind[nCntWind].nLife <= 0)
			{ // ������0�ȉ��ɂȂ����ꍇ
				// �g�p���Ȃ�
				g_aWind[nCntWind].bUse = false;
			}

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aWind[nCntWind].fRadius, +g_aWind[nCntWind].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aWind[nCntWind].fRadius, +g_aWind[nCntWind].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aWind[nCntWind].fRadius, -g_aWind[nCntWind].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aWind[nCntWind].fRadius, -g_aWind[nCntWind].fRadius, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aWind[nCntWind].col;
			pVtx[1].col = g_aWind[nCntWind].col;
			pVtx[2].col = g_aWind[nCntWind].col;
			pVtx[3].col = g_aWind[nCntWind].col;
		}

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWind->Unlock();
}

//======================================================================================================================
//	�����@�̕`�揈��
//======================================================================================================================
void DrawWind(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxTrans;						// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;							// �r���[�}�g���b�N�X�擾�p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntWind = 0; nCntWind < MAX_WIND; nCntWind++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aWind[nCntWind].bUse == true)
		{ // �G�t�F�N�g���g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aWind[nCntWind].mtxWorld);

			// �r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aWind[nCntWind].mtxWorld, NULL, &mtxView);	// �t�s������߂�
			g_aWind[nCntWind].mtxWorld._41 = 0.0f;
			g_aWind[nCntWind].mtxWorld._42 = 0.0f;
			g_aWind[nCntWind].mtxWorld._43 = 0.0f;

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aWind[nCntWind].pos.x, g_aWind[nCntWind].pos.y, g_aWind[nCntWind].pos.z);
			D3DXMatrixMultiply(&g_aWind[nCntWind].mtxWorld, &g_aWind[nCntWind].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aWind[nCntWind].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffWind, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureWind);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWind * 4, 2);
		}
	}

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//======================================================================================================================
//	�����@�̐ݒ菈��
//======================================================================================================================
void SetWind(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	VERTEX_3D *pVtx;							// ���_���ւ̃|�C���^

	float fMoveMagni;							// �ړ��ʂ̔{��
	float fRadius;								// ���a
	int	  nLife;								// ����

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWind->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWind = 0; nCntWind < MAX_WIND; nCntWind++)
	{
		if (g_aWind[nCntWind].bUse == false)
		{ // �����g�p���Ă��Ȃ��ꍇ

			// �ʒu��ݒu����
			g_aWind[nCntWind].pos = pos;

			// �ړ��ʂ̔{����ݒ肷��
			fMoveMagni = (float)(rand() % 100) / 5;

			// �ړ��ʂ�ݒ肷��
			g_aWind[nCntWind].move.x = sinf(rot.y) * fMoveMagni;
			g_aWind[nCntWind].move.y = 0.0f;
			g_aWind[nCntWind].move.z = cosf(rot.y) * fMoveMagni;

			// �F��ݒ肷��
			g_aWind[nCntWind].col = D3DXCOLOR(0.7f, 1.0f, 0.7f, 1.0f);

			//�������Z�o����
			nLife = (rand() % 10) + 6;

			// ������ݒ肷��
			g_aWind[nCntWind].nLife = nLife;

			// �����x��ݒ肷��
			g_aWind[nCntWind].fAlpha = 1.0f / nLife;

			//���a���Z�o����
			fRadius = (float)((rand() % 470) / 10);

			// ���a��ݒ肷��
			g_aWind[nCntWind].fRadius = fRadius;

			// �g�p����
			g_aWind[nCntWind].bUse = true;

			// �����o��
			break;
		}

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWind->Unlock();
}

//======================================================================================================================
//	���̓����蔻��
//======================================================================================================================
void CollisionWind(Human *pHuman)
{
	Player *pPlayer = GetPlayer();			//�v���C���[�̏����擾����

	// �ϐ��z���錾
	D3DXVECTOR3 vecPos[4];	// ���_�ʒu ([��] 0�F�E��@1�F����@2�F�����@3�F�E��)

	if (pHuman->pos.y <= HUMAN_FLY_POS_Y)
	{ // 50.0f�ȉ��ɂ���ꍇ

		// �l���_�̈ʒu�̌v�Z
		VecSizePos
		( // ����
			&vecPos[0],
			pPlayer->pos,	// ��΍��W
			pPlayer->rot,	// ����
			WIND_WIDTH,		// ����
			WIND_DEPTH		// �c��
		);

		if (LineOuterProduct(vecPos[0], vecPos[1], pHuman->pos) < 0
		&&  LineOuterProduct(vecPos[1], vecPos[2], pHuman->pos) < 0
		&&  LineOuterProduct(vecPos[2], vecPos[3], pHuman->pos) < 0
		&&  LineOuterProduct(vecPos[3], vecPos[0], pHuman->pos) < 0)
		{ // �l�ӂ̓����ɂ���ꍇ (�������Ă���ꍇ)

			if (pPlayer->wind.bUseWind == true && pHuman->state != HUMANSTATE_FLY && pHuman->judge.state == JUDGESTATE_EVIL)
			{ // �����g�p���Ă��邩�A�l���������ł��Ȃ������A�����z�������ꍇ

				// �������ł����Ԃɂ���
				pHuman->state = HUMANSTATE_FLY;

				// �l�Ԃ�������΂���鏈��
				FlyAwayHuman(pHuman, *pPlayer);

				// �A�C�e����������J�E���g������������
				g_nHumanItemCount++;

				if (g_nHumanItemCount % ITEM_WIND_COUNT == 0)
				{ // ��萔�ȏ�ɂȂ�����

					// �A�C�e���̐ݒ菈��
					SetItem(pHuman->pos, ITEMTYPE_HEAL_BARRIER);
				}

				// �{�[�i�X�̐ݒ菈��
				SetBonus(SCORE_HUMAN);

				// �J�����̏�Ԃ�ς���
				*GetCameraState() = CAMERASTATE_GOODJOB;
			}
		}
	}
}

//============================================================
//	�l�Ԃ�������΂���鏈��
//============================================================
void FlyAwayHuman(Human *pHuman, Player player)
{
	float FlyAngle = atan2f(pHuman->pos.x - player.pos.x, pHuman->pos.z - player.pos.z);

	// �ړ��ʂ�ݒ肷��
	pHuman->move.x = sinf(FlyAngle) * FLYAWAY_WIDTH;
	pHuman->move.y = FLYAWAY_HEIGHT;
	pHuman->move.z = cosf(FlyAngle) * FLYAWAY_DEPTH;

	// ��΂�
	pHuman->pos += pHuman->move;

	// �l�����ł����Ԃɂ���
	pHuman->icon.state = ICONSTATE_FLYAWAY;
}

//============================================================
//	���̏��̎擾����
//============================================================
WindInfo *GetWindInfo(void)
{
	//���̏���Ԃ�
	return &g_WindInfo;
}

#ifdef _DEBUG	// �f�o�b�O����
#endif