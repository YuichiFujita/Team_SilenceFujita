//======================================================================================================================
//
//	�r���{�[�h���� [billboard.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "billboard.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAX_BILLBOARD		(4)		// �g�p����|���S���� (�r���{�[�h�̍ő吔)

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTextureBillboard[] =	// �e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\billboard000.png",	// �r���{�[�h�̃e�N�X�`���̑��΃p�X
};

//**********************************************************************************************************************
//	�񋓌^��` (TEXTURE_BILLBOARD)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_BILLBOARD_NORMAL = 0,	// �r���{�[�h (�ʏ�)
	TEXTURE_BILLBOARD_MAX,			// ���̗񋓌^�̑���
} TEXTURE_BILLBOARD;

//**********************************************************************************************************************
//	�\���̒�` (Billboard)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// �ʒu
	D3DXMATRIX  mtxWorld;			// ���[���h�}�g���b�N�X
	bool        bUse;				// �g�p��
} Billboard;

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureBillboard[TEXTURE_BILLBOARD_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;							// ���_�o�b�t�@�ւ̃|�C���^

Billboard g_aBillboard[MAX_BILLBOARD];			// �r���{�[�h�̏��

//======================================================================================================================
//	�r���{�[�h�̏���������
//======================================================================================================================
void InitBillboard(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_3D *pVtx;							// ���_���ւ̃|�C���^

	// �e�N�X�`���̓ǂݍ���
	for (int nCntBillboard = 0; nCntBillboard < TEXTURE_BILLBOARD_MAX; nCntBillboard++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		D3DXCreateTextureFromFile(pDevice, apTextureBillboard[nCntBillboard], &g_apTextureBillboard[nCntBillboard]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,	// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL
	);

	// �r���{�[�h�̏��̏�����
	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{ // �r���{�[�h�̍ő�\�������J��Ԃ�

		g_aBillboard[nCntBillboard].pos  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_aBillboard[nCntBillboard].bUse = false;								// �g�p��
	}

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{ // �r���{�[�h�̍ő�\�������J��Ԃ�

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-25.0f,  25.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3( 25.0f,  25.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-25.0f, -25.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3( 25.0f, -25.0f, 0.0f);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBillboard->Unlock();
}

//======================================================================================================================
//	�r���{�[�h�̏I������
//======================================================================================================================
void UninitBillboard(void)
{
	// �e�N�X�`���̔j��
	for (int nCntBillboard = 0; nCntBillboard < TEXTURE_BILLBOARD_MAX; nCntBillboard++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTextureBillboard[nCntBillboard] != NULL)
		{ // �ϐ� (g_apTextureBillboard) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureBillboard[nCntBillboard]->Release();
			g_apTextureBillboard[nCntBillboard] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBillboard != NULL)
	{ // �ϐ� (g_pVtxBuffBillboard) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//======================================================================================================================
//	�r���{�[�h�̍X�V����
//======================================================================================================================
void UpdateBillboard(void)
{

}

//======================================================================================================================
//	�r���{�[�h�̕`�揈��
//======================================================================================================================
void DrawBillboard(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxTrans;						// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;							// �r���[�}�g���b�N�X�擾�p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	// ���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// ���e�X�g�̗L�� / �����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 160);				// ���e�X�g�̎Q�ƒl�ݒ�

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{ // �r���{�[�h�̍ő�\�������J��Ԃ�

		if (g_aBillboard[nCntBillboard].bUse == true)
		{ // �r���{�[�h���g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBillboard[nCntBillboard].mtxWorld);

			// �r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aBillboard[nCntBillboard].mtxWorld, NULL, &mtxView);	// �t�s������߂�
			g_aBillboard[nCntBillboard].mtxWorld._41 = 0.0f;
			g_aBillboard[nCntBillboard].mtxWorld._42 = 0.0f;
			g_aBillboard[nCntBillboard].mtxWorld._43 = 0.0f;

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aBillboard[nCntBillboard].pos.x, g_aBillboard[nCntBillboard].pos.y, g_aBillboard[nCntBillboard].pos.z);
			D3DXMatrixMultiply(&g_aBillboard[nCntBillboard].mtxWorld, &g_aBillboard[nCntBillboard].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCntBillboard].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureBillboard[TEXTURE_BILLBOARD_NORMAL]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBillboard * 4, 2);
		}
	}

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	// ���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g�̗L�� / �����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// ���e�X�g�̎Q�ƒl�ݒ�
}

//======================================================================================================================
//	�r���{�[�h�̐ݒ菈��
//======================================================================================================================
void SetBillboard(D3DXVECTOR3 pos)
{
	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{ // �r���{�[�h�̍ő�\�������J��Ԃ�

		if (g_aBillboard[nCntBillboard].bUse == false)
		{ // �r���{�[�h���g�p����Ă��Ȃ��ꍇ

			// �����̈ʒu����
			g_aBillboard[nCntBillboard].pos = pos;

			// �g�p���Ă����Ԃɂ���
			g_aBillboard[nCntBillboard].bUse = true;

			// �����𔲂���
			break;
		}
	}
}