//===========================================
//
//���̃��C������[Arrow.cpp]
//Author ��������
//
//===========================================
#include "main.h"
#include "game.h"

#include "arrow.h"
#include "player.h"

//-------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------
#define ARROW_TEXTURE		"data/TEXTURE/PlayArrow.png"			// ���̃e�N�X�`��
#define ARROW_RADIUS		(D3DXVECTOR3(360.0f, 0.0f, 210.0f))		// ���̔��a
#define ARROW_SHIFT			(450.0f)								// ���炷��

//-------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// �O���[�o���ϐ�
//-------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureArrow = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffArrow = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

Arrow g_arrow;				// ���̏��

//==========================================
// ���̏���������
//==========================================
void InitArrow(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���n�̃e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		ARROW_TEXTURE,
		&g_pTextureArrow);

	// ���̏�����
	g_arrow.pos  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	g_arrow.rot	 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	g_arrow.bUse = false;								// �g�p��

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffArrow,
		NULL);

	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffArrow->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-ARROW_RADIUS.x, 0.0f, +ARROW_RADIUS.z);
	pVtx[1].pos = D3DXVECTOR3(+ARROW_RADIUS.x, 0.0f, +ARROW_RADIUS.z);
	pVtx[2].pos = D3DXVECTOR3(-ARROW_RADIUS.x, 0.0f, -ARROW_RADIUS.z);
	pVtx[3].pos = D3DXVECTOR3(+ARROW_RADIUS.x, 0.0f, -ARROW_RADIUS.z);

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffArrow->Unlock();
}

//========================================
// ���̏I������
//========================================
void UninitArrow(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureArrow != NULL)
	{
		g_pTextureArrow->Release();
		g_pTextureArrow = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffArrow != NULL)
	{
		g_pVtxBuffArrow->Release();
		g_pVtxBuffArrow = NULL;
	}
}

//========================================
// ���̍X�V����
//========================================
void UpdateArrow(void)
{
	Player *pPlayer = GetPlayer();

	if (g_arrow.bUse == true)
	{ // �g�p����Ă����ꍇ

		// ������ݒ肷��
		g_arrow.rot.y = pPlayer->rot.y;

		// �ʒu��ݒ肷��
		g_arrow.pos.x = pPlayer->pos.x + sinf(pPlayer->rot.y) * ARROW_SHIFT;
		g_arrow.pos.z = pPlayer->pos.z + cosf(pPlayer->rot.y) * ARROW_SHIFT;
	}
}

//=====================================
// ���̕`�揈��
//=====================================
void DrawArrow(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);					//Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						//Z�e�X�g�̗L��/�����ݒ�

	if (g_arrow.bUse == true)
	{ // ���b�V���t�B�[���h���g�p����Ă���ꍇ

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_arrow.mtx);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_arrow.rot.y, g_arrow.rot.x, g_arrow.rot.z);
		D3DXMatrixMultiply(&g_arrow.mtx, &g_arrow.mtx, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_arrow.pos.x, g_arrow.pos.y, g_arrow.pos.z);
		D3DXMatrixMultiply(&g_arrow.mtx, &g_arrow.mtx, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_arrow.mtx);

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffArrow, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureArrow);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			// �v���~�e�B�u�̎��
			0,												// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);												// �`�悷��v���~�e�B�u��
	}

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			//Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				//Z�e�X�g�̗L��/�����ݒ�
}

//======================================
// ���̐ݒ菈��
//======================================
void SetArrow(bool bUse)
{
	if (bUse == true)
	{ // �g�p����ꍇ

		// ����ݒ肷��
		g_arrow.pos = GetPlayer()->pos;		// �ʒu
		g_arrow.rot = GetPlayer()->rot;		// ����
		g_arrow.bUse = true;				// �g�p��
	}
	else
	{ // �g�p���Ȃ��ꍇ

		if (g_arrow.bUse == true)
		{ // �g�p���Ă���ꍇ

			// �g�p�󋵂�ݒ肷��
			g_arrow.bUse = false;
		}
	}
}