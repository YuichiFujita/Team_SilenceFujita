//======================================================================================================================
//
//	�G�t�F�N�g���� [effect.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "effect.h"
#include "player.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAX_EFFECT		(4096)		// �g�p����|���S���� (�G�t�F�N�g�̍ő吔)

#define SPARK_MOVE_Y	(1.0f)		// �ΉԃG�t�F�N�g�̈ړ���(Y��)
#define SPARK_SUB_COL_G	(0.05f)		// �ΉԃG�t�F�N�g�̗΂̌����W��
#define SPARK_REV_COL_G	(0.5f)		// �ΉԃG�t�F�N�g�̗΂̍Œ���␳��

#define SPRAY_MOVE_Y	(1.3f)		// �����Ԃ��G�t�F�N�g�̈ړ���(Y��)

#define BREAK_OBJECT_MOVE_Y	(1.8f)	// �I�u�W�F�N�g�̔j�󎞂̉��̏d��

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTextureEffect[] =		// �e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\effect000.jpg",	// �G�t�F�N�g�̃e�N�X�`���̑��΃p�X
	"data\\TEXTURE\\effect002.tga",	// �G�t�F�N�g�̃e�N�X�`���̑��΃p�X(��)
};

//**********************************************************************************************************************
//	�񋓌^��` (TEXTURE_EFFECT)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_EFFECT_NORMAL = 0,		// �G�t�F�N�g (�ʏ�)
	TEXTURE_EFFECT_SMOKE,			// �G�t�F�N�g�i���j
	TEXTURE_EFFECT_MAX,				// ���̗񋓌^�̑���
} TEXTURE_EFFECT;

//**********************************************************************************************************************
//	�\���̒�` (Effect)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 move;				// �ړ���
	D3DXMATRIX  mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXCOLOR   col;				// �F
	int         nLife;				// ����
	float       fRadius;			// ���a
	float       fSub;				// ���Z�� (���a)
	float       fAlpha;				// 1F���Ƃ̔����Ȃ��
	EFFECTTYPE	effectType;			// �G�t�F�N�g�̎��
	bool        bUse;				// �g�p��
}Effect;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void EffectSparkUpdate(Effect *pEffect);			// �ΉԃG�t�F�N�g�̍X�V����
void EffectSprayUpdate(Effect *pEffect);			// �����Ԃ��G�t�F�N�g�̍X�V����
void EffectBreakObjectUpdate(Effect *pEffect);		// �I�u�W�F�N�g�̔j�󎞂̃G�t�F�N�g�̍X�V����

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureEffect[TEXTURE_EFFECT_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;					// ���_�o�b�t�@�ւ̃|�C���^

Effect g_aEffect[MAX_EFFECT];		// �G�t�F�N�g�̏��

//======================================================================================================================
//	�G�t�F�N�g�̏���������
//======================================================================================================================
void InitEffect(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_3D *pVtx;							// ���_���ւ̃|�C���^

	for (int nCntEffect = 0; nCntEffect < TEXTURE_EFFECT_MAX; nCntEffect++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, apTextureEffect[nCntEffect], &g_apTextureEffect[nCntEffect]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_3D) * 4 * MAX_EFFECT,		// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL
	);

	// �G�t�F�N�g�̏��̏�����
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		g_aEffect[nCntEffect].pos     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_aEffect[nCntEffect].move    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
		g_aEffect[nCntEffect].col     = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �F
		g_aEffect[nCntEffect].nLife   = 0;									// ����
		g_aEffect[nCntEffect].fRadius = 0.0f;								// ���a
		g_aEffect[nCntEffect].fSub    = 0.0f;								// ���Z�� (���a)
		g_aEffect[nCntEffect].fAlpha  = 0.0f;								// 1F���Ƃ̔����Ȃ��
		g_aEffect[nCntEffect].effectType = EFFECTTYPE_NONE;					// �G�t�F�N�g�̎��
		g_aEffect[nCntEffect].bUse    = false;								// �g�p��
	}

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
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
	g_pVtxBuffEffect->Unlock();
}

//======================================================================================================================
//	�G�t�F�N�g�̏I������
//======================================================================================================================
void UninitEffect(void)
{
	// �e�N�X�`���̔j��
	for (int nCntEffect = 0; nCntEffect < TEXTURE_EFFECT_MAX; nCntEffect++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTextureEffect[nCntEffect] != NULL)
		{ // �ϐ� (g_apTextureEffect) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureEffect[nCntEffect]->Release();
			g_apTextureEffect[nCntEffect] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect != NULL)
	{ // �ϐ� (g_pVtxBuffEffect) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//======================================================================================================================
//	�G�t�F�N�g�̍X�V����
//======================================================================================================================
void UpdateEffect(void)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aEffect[nCntEffect].bUse == true)
		{ // �G�t�F�N�g���g�p����Ă���ꍇ

			switch (g_aEffect[nCntEffect].effectType)
			{
			case EFFECTTYPE_SPARK:		// �Ή�

				// �ΉԃG�t�F�N�g�̍X�V����
				EffectSparkUpdate(&g_aEffect[nCntEffect]);

				// �������甲����
				break;

			case EFFECTTYPE_RAINSPRAY:	// �J�̐����Ԃ�

				// �����Ԃ��G�t�F�N�g�̍X�V����
				EffectSprayUpdate(&g_aEffect[nCntEffect]);

				break;					// �����o��

			case EFFECTTYPE_PLAY_SMOKE:	// �v���C���[�̍���

				// �v���C���[�̈ړ��ʂ����Z����
				g_aEffect[nCntEffect].pos.x += (GetPlayer()->pos.x - GetPlayer()->oldPos.x);
				g_aEffect[nCntEffect].pos.z += (GetPlayer()->pos.z - GetPlayer()->oldPos.z);

				break;					// �����o��

			case EFFECTTYPE_BREAKOBJECT:	// �I�u�W�F�N�g�̔j�󎞂̉�

				// �I�u�W�F�N�g�̔j�󎞂̃G�t�F�N�g�̍X�V����
				EffectBreakObjectUpdate(&g_aEffect[nCntEffect]);

				break;					// �����o��

			default:					// ���̑�

				// ���ɖ���

				// �������甲����
				break;
			}

			// �ړ��̍X�V
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

			// �\���̗̂v�f�����Z����
			g_aEffect[nCntEffect].nLife--;									// ���������Z
			g_aEffect[nCntEffect].fRadius -= g_aEffect[nCntEffect].fSub;	// ���a�����Z
			g_aEffect[nCntEffect].col.a   -= g_aEffect[nCntEffect].fAlpha;	// ���l�����Z

			// ���a�̕␳
			if (g_aEffect[nCntEffect].fRadius < 0.0f)
			{ // ���a�� 0.0f ��菬�����ꍇ

				// ���a�� 0.0f ����
				g_aEffect[nCntEffect].fRadius = 0.0f;
			}

			// �����x�̕␳
			if (g_aEffect[nCntEffect].col.a < 0.0f)
			{ // ���l�� 0.0f ��菬�����ꍇ

				// ���l�� 0.0f ����
				g_aEffect[nCntEffect].col.a = 0.0f;
			}

			if (g_aEffect[nCntEffect].nLife <= 0)
			{ // �������}�����ꍇ

				// �g�p���Ă��Ȃ���Ԃɂ���
				g_aEffect[nCntEffect].bUse = false;
			}

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;
		}

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//======================================================================================================================
//	�G�t�F�N�g�̕`�揈��
//======================================================================================================================
void DrawEffect(void)
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

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aEffect[nCntEffect].bUse == true)
		{ // �G�t�F�N�g���g�p����Ă���ꍇ

			if (g_aEffect[nCntEffect].effectType != EFFECTTYPE_PLAY_SMOKE
			 && g_aEffect[nCntEffect].effectType != EFFECTTYPE_BREAKOBJECT)
			{ // �G�t�F�N�g�̃^�C�v�����n�ȊO�������ꍇ

				// ���u�����f�B���O�����Z�����ɐݒ�
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);

			// �r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aEffect[nCntEffect].mtxWorld, NULL, &mtxView);	// �t�s������߂�
			g_aEffect[nCntEffect].mtxWorld._41 = 0.0f;
			g_aEffect[nCntEffect].mtxWorld._42 = 0.0f;
			g_aEffect[nCntEffect].mtxWorld._43 = 0.0f;

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			switch (g_aEffect[nCntEffect].effectType)
			{
			case EFFECTTYPE_PLAY_SMOKE:		// �v���C���[�̍���

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_apTextureEffect[TEXTURE_EFFECT_SMOKE]);

				break;				// �����o��

			case EFFECTTYPE_BREAKOBJECT:	// �I�u�W�F�N�g�̔j��

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_apTextureEffect[TEXTURE_EFFECT_SMOKE]);

				break;				//�����o��

			default:

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_apTextureEffect[TEXTURE_EFFECT_NORMAL]);

				break;
			}

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}

		// ���u�����f�B���O�����ɖ߂�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//======================================================================================================================
//	�G�t�F�N�g�̐ݒ菈��
//======================================================================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fRadius, float fSub,EFFECTTYPE effectType)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aEffect[nCntEffect].bUse == false)
		{ // �G�t�F�N�g���g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aEffect[nCntEffect].pos     = pos;		// �ʒu
			g_aEffect[nCntEffect].move    = move;		// �ړ���
			g_aEffect[nCntEffect].col     = col;		// �F
			g_aEffect[nCntEffect].nLife   = nLife;		// ����
			g_aEffect[nCntEffect].fRadius = fRadius;	// ���a
			g_aEffect[nCntEffect].fSub    = fSub;		// ���Z�� (���a)
			g_aEffect[nCntEffect].effectType = effectType;		// �G�t�F�N�g�̎��

			// 1F ���Ƃ̔����Ȃ�ʂ�ݒ�
			g_aEffect[nCntEffect].fAlpha = 1.0f / nLife;

			// �g�p���Ă����Ԃɂ���
			g_aEffect[nCntEffect].bUse = true;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			// �����𔲂���
			break;
		}

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//======================================================================================================================
//	�G�t�F�N�g�̍폜
//======================================================================================================================
void AllFalseEffect(void)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		// �g�p���Ă��Ȃ���Ԃɂ���
		g_aEffect[nCntEffect].bUse = false;
	}
}

//======================================================================================================================
// �ΉԃG�t�F�N�g�̍X�V����
//======================================================================================================================
void EffectSparkUpdate(Effect *pEffect)
{
	// ���Ɉړ��ʂ𑫂��Ă���
	pEffect->move.y -= SPARK_MOVE_Y;

	// �ΉԂ�Ԃ����Ă���
	pEffect->col.g -= SPARK_SUB_COL_G;

	if (pEffect->col.g <= SPARK_REV_COL_G)
	{ // ���̒l�ȉ��ɂȂ����ꍇ
		// G�l��␳����
		pEffect->col.g = SPARK_REV_COL_G;
	}
}

//======================================================================================================================
// �����Ԃ��G�t�F�N�g�̍X�V����
//======================================================================================================================
void EffectSprayUpdate(Effect *pEffect)
{
	// ���Ɉړ��ʂ𑫂��Ă���
	pEffect->move.y -= SPRAY_MOVE_Y;
}

//======================================================================================================================
// �I�u�W�F�N�g�̔j�󎞂̃G�t�F�N�g�̍X�V����
//======================================================================================================================
void EffectBreakObjectUpdate(Effect *pEffect)
{
	// ���Ɉړ��ʂ𑫂��Ă���
	pEffect->move.y -= BREAK_OBJECT_MOVE_Y;

	if (pEffect->pos.y <= 0.0f)
	{ // �G�t�F�N�g���n�ʂ𒴂����ꍇ

		// �G�t�F�N�g�̈ʒu��␳����
		pEffect->pos.y = 0.0f;

		// �g�p���Ȃ�
		pEffect->bUse = false;
	}
}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================
//**********************************************************************************************************************
//	�G�t�F�N�g�̑����擾����
//**********************************************************************************************************************
int GetNumEffect(void)
{
	// �ϐ���錾
	int nNumEffect = 0;	// �G�t�F�N�g�̑����̊m�F�p

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aEffect[nCntEffect].bUse == true)
		{ // �G�t�F�N�g���g�p����Ă���ꍇ

			// �J�E���^�[�����Z
			nNumEffect++;
		}
	}

	// �ϐ��̒l��Ԃ�
	return nNumEffect;	// �G�t�F�N�g�̑���
}
#endif