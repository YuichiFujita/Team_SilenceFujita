//===========================================
//
//�r���{�[�h�̃��C������[Billboard.cpp]
//Author ��������
//
//===========================================
#include"main.h"
#include "game.h"
#include "billboard.h"
#include "input.h"
#include "shadow.h"
#include "meshfield.h"
#include "Editmain.h"

//�v���g�^�C�v�錾
void CollisionWall(Billboard *Billboard);		//�ǂ̓����蔻��
void PlayBillboard(BillboardAnim *Anim);		//�r���{�[�h�̍Đ�����

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBillboard[BILLBOARD_MAX] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Billboard g_aBillboard[MAX_BILLBOARD];				//�r���{�[�h�̏��

//�e�N�X�`���t�@�C����
const char *c_apFilenameBillboard[BILLBOARD_MAX] = 
{
	"data/TEXTURE/explosion000.png",				//�����̃e�N�X�`��
	"data/TEXTURE/bullet000.png",					//�e�̃e�N�X�`��
};

//==========================================
//�r���{�[�h�̏���������
//==========================================
void InitBillboard(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntTexture = 0; nCntTexture < BILLBOARD_MAX; nCntTexture++)
	{//�e�N�X�`���̐ݒ�
		//���n�̃e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameBillboard[nCntTexture],
			&g_pTextureBillboard[nCntTexture]);
	}
	
	for (int nCntSet = 0; nCntSet < MAX_BILLBOARD; nCntSet++)
	{
		//�ʒu�̏�����
		g_aBillboard[nCntSet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�����̏�����
		g_aBillboard[nCntSet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//���a������������
		g_aBillboard[nCntSet].Radius = D3DXVECTOR2(30.0f, 30.0f);

		//�e�̔ԍ�������������
		g_aBillboard[nCntSet].nShadow = -1;

		//�A�j���J�E���^�[������������
		g_aBillboard[nCntSet].nAnimCounter = 0;

		//�A�j���p�^�[��No.������������
		g_aBillboard[nCntSet].nAnimPattern = 0;

		//�g�p���Ă��Ȃ�
		g_aBillboard[nCntSet].bUse = false;

		//�F������������
		g_aBillboard[nCntSet].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�F�̃R�s�[������������
		g_aBillboard[nCntSet].colCopy = g_aBillboard[nCntSet].col;

		//��Ԃ�����������
		g_aBillboard[nCntSet].State = BILLBOARDSTATE_NONE;

		//�A�j���[�V�������Ȃ�
		g_aBillboard[nCntSet].Anim.bAnim = false;

		//�A�j���J�E���g������������
		g_aBillboard[nCntSet].Anim.nAnimCounter = 1;

		//�A�j���p�^�[��������������
		g_aBillboard[nCntSet].Anim.nAnimPattern = 1;

		//�Đ��J�E���g������������
		g_aBillboard[nCntSet].Anim.nReplayCount = 0;

		//�Đ��p�^�[��������������
		g_aBillboard[nCntSet].Anim.nReplayPattern = 0;

		//�e�N�X�`���̃p�^�[��������������
		g_aBillboard[nCntSet].Anim.TexPattern = 1.0f;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_aBillboard[nCnt].Radius.x, +g_aBillboard[nCnt].Radius.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+g_aBillboard[nCnt].Radius.x, +g_aBillboard[nCnt].Radius.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aBillboard[nCnt].Radius.x, -g_aBillboard[nCnt].Radius.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+g_aBillboard[nCnt].Radius.x, -g_aBillboard[nCnt].Radius.y, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_aBillboard[nCnt].col;
		pVtx[1].col = g_aBillboard[nCnt].col;
		pVtx[2].col = g_aBillboard[nCnt].col;
		pVtx[3].col = g_aBillboard[nCnt].col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;						//���_�f�[�^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBillboard->Unlock();
}

//========================================
//�r���{�[�h�̏I������
//========================================
void UninitBillboard(void)
{
	for (int nCntBill = 0; nCntBill < BILLBOARD_MAX; nCntBill++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureBillboard[nCntBill] != NULL)
		{
			g_pTextureBillboard[nCntBill]->Release();
			g_pTextureBillboard[nCntBill] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//========================================
//�r���{�[�h�̍X�V����
//========================================
void UpdateBillboard(void)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		if (g_aBillboard[nCntBill].bUse == true)
		{//�g�p����Ă����ꍇ
			switch (g_aBillboard[nCntBill].State)
			{
			case BILLBOARDSTATE_DELETETARGET:			//�폜�Ώۂ̏ꍇ
				//�ԐF�ɕς���
				g_aBillboard[nCntBill].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				break;									//�����o��

			default:									//��L�ȊO
				//���̐F�ɕς���
				g_aBillboard[nCntBill].col = g_aBillboard[nCntBill].colCopy;

				break;									//�����o��
			}

			if (g_aBillboard[nCntBill].Anim.bAnim == true)
			{//�A�j���[�V��������ꍇ
				//�r���{�[�h�̍Đ�����
				PlayBillboard(&g_aBillboard[nCntBill].Anim);

				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2((g_aBillboard[nCntBill].Anim.TexPattern * g_aBillboard[nCntBill].Anim.nReplayPattern), 0.0f);
				pVtx[1].tex = D3DXVECTOR2((g_aBillboard[nCntBill].Anim.TexPattern * g_aBillboard[nCntBill].Anim.nReplayPattern) + g_aBillboard[nCntBill].Anim.TexPattern, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((g_aBillboard[nCntBill].Anim.TexPattern * g_aBillboard[nCntBill].Anim.nReplayPattern), 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_aBillboard[nCntBill].Anim.TexPattern * g_aBillboard[nCntBill].Anim.nReplayPattern) + g_aBillboard[nCntBill].Anim.TexPattern, 1.0f);
			}
			else
			{//�A�j���[�V�������Ȃ��ꍇ
				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aBillboard[nCntBill].col;
			pVtx[1].col = g_aBillboard[nCntBill].col;
			pVtx[2].col = g_aBillboard[nCntBill].col;
			pVtx[3].col = g_aBillboard[nCntBill].col;

			//�e�̈ʒu�̐ݒ菈��
			SetPositionShadow(g_aBillboard[nCntBill].nShadow, g_aBillboard[nCntBill].pos, NONE_ROT, NONE_SCALE);
		}
		pVtx += 4;					//���_�f�[�^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBillboard->Unlock();
}

//=====================================
//�r���{�[�h�̕`�揈��
//=====================================
void DrawBillboard(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	D3DXMATRIX mtxTrans;			//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;				//�r���[�}�g���b�N�X�擾�p

	//���C�e�B���O��OFF�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);					//Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						//Z�e�X�g�̗L��/�����ݒ�

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);					//�A���t�@�e�X�g�̗L��/�����ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);				//�A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 75);							//�A���t�@�e�X�g�̎Q�ƒl�ݒ�

	for (int nCntDraw = 0; nCntDraw < MAX_BILLBOARD; nCntDraw++)
	{
		if (g_aBillboard[nCntDraw].bUse == true)
		{//�g�p���Ă����ꍇ
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBillboard[nCntDraw].mtx);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����Ɍ����Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aBillboard[nCntDraw].mtx, NULL, &mtxView);

			//�t�s������߂�
			g_aBillboard[nCntDraw].mtx._41 = 0.0f;
			g_aBillboard[nCntDraw].mtx._42 = 0.0f;
			g_aBillboard[nCntDraw].mtx._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aBillboard[nCntDraw].pos.x, g_aBillboard[nCntDraw].pos.y, g_aBillboard[nCntDraw].pos.z);
			D3DXMatrixMultiply(&g_aBillboard[nCntDraw].mtx, &g_aBillboard[nCntDraw].mtx, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCntDraw].mtx);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0,
				g_pVtxBuffBillboard,								//���_�o�b�t�@�ւ̃|�C���^
				0,
				sizeof(VERTEX_3D));									//���_���\���̂̃T�C�Y

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBillboard[g_aBillboard[nCntDraw].Type]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//�v���~�e�B�u�̎��
				4 * nCntDraw,										//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);													//�`�悷��v���~�e�B�u��
		}
	}

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			//�A���t�@�e�X�g�̗L��/�����ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		//�A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						//�A���t�@�e�X�g�̎Q�ƒl�ݒ�

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			//Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				//Z�e�X�g�̗L��/�����ݒ�

	//���C�e�B���O��ON�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//======================================
//�r���{�[�h�̐ݒ菈��
//======================================
void SetBillboard(D3DXVECTOR3 rot, D3DXVECTOR3 pos, int nType, D3DXVECTOR2 Radius, D3DXCOLOR col, int AnimCnt, int AnimPtrn, bool bAnim, bool bShadow)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		if (g_aBillboard[nCntBill].bUse == false)
		{//�g�p����Ă��Ȃ������ꍇ
			//�ʒu��ݒ肷��
			g_aBillboard[nCntBill].pos = pos;

			//������ݒ肷��
			g_aBillboard[nCntBill].rot = rot;

			//�傫����ݒ肷��
			g_aBillboard[nCntBill].Radius = Radius;

			//�F��ݒ肷��
			g_aBillboard[nCntBill].col = col;

			//�F�̃R�s�[��ݒ肷��
			g_aBillboard[nCntBill].colCopy = g_aBillboard[nCntBill].col;

			//�A�j���[�V�������邩��ݒ肷��
			g_aBillboard[nCntBill].Anim.bAnim = bAnim;

			//�A�j���[�V�����J�E���g��ݒ肷��
			g_aBillboard[nCntBill].Anim.nAnimCounter = AnimCnt;

			//�A�j���[�V�����p�^�[����ݒ肷��
			g_aBillboard[nCntBill].Anim.nAnimPattern = AnimPtrn;

			//�Đ��J�E���g��ݒ肷��
			g_aBillboard[nCntBill].Anim.nReplayCount = 0;

			//�Đ��p�^�[����ݒ肷��
			g_aBillboard[nCntBill].Anim.nReplayPattern = 0;

			//�e�N�X�`���̃p�^�[����ݒ肷��
			g_aBillboard[nCntBill].Anim.TexPattern = (float)(1.0f / g_aBillboard[nCntBill].Anim.nAnimPattern);

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aBillboard[nCntBill].Radius.x, +g_aBillboard[nCntBill].Radius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aBillboard[nCntBill].Radius.x, +g_aBillboard[nCntBill].Radius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aBillboard[nCntBill].Radius.x, -g_aBillboard[nCntBill].Radius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aBillboard[nCntBill].Radius.x, -g_aBillboard[nCntBill].Radius.y, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aBillboard[nCntBill].col;
			pVtx[1].col = g_aBillboard[nCntBill].col;
			pVtx[2].col = g_aBillboard[nCntBill].col;
			pVtx[3].col = g_aBillboard[nCntBill].col;

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2((g_aBillboard[nCntBill].Anim.TexPattern * g_aBillboard[nCntBill].Anim.nReplayPattern), 0.0f);
			pVtx[1].tex = D3DXVECTOR2((g_aBillboard[nCntBill].Anim.TexPattern * g_aBillboard[nCntBill].Anim.nReplayPattern) + g_aBillboard[nCntBill].Anim.TexPattern, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((g_aBillboard[nCntBill].Anim.TexPattern * g_aBillboard[nCntBill].Anim.nReplayPattern), 1.0f);
			pVtx[3].tex = D3DXVECTOR2((g_aBillboard[nCntBill].Anim.TexPattern * g_aBillboard[nCntBill].Anim.nReplayPattern) + g_aBillboard[nCntBill].Anim.TexPattern, 1.0f);

			//��ނ�ݒ肷��
			g_aBillboard[nCntBill].Type = nType;

			//�A�j���J�E���^�[��ݒ肷��
			g_aBillboard[nCntBill].nAnimCounter = 0;

			//�A�j���p�^�[��No.��ݒ肷��
			g_aBillboard[nCntBill].nAnimPattern = 0;

			//�r���{�[�h�̏�Ԃ�ݒ肷��
			g_aBillboard[nCntBill].State = BILLBOARDSTATE_USE;

			if (bShadow == true)
			{//�e������
				//�e��ݒ肷��
				g_aBillboard[nCntBill].nShadow = SetCircleShadow(0.5f, 30.0f, &g_aBillboard[nCntBill].nShadow, &g_aBillboard[nCntBill].bUse);
			}
			else
			{//�e�����Ȃ�
				//�e��-1�ɐݒ肷��
				g_aBillboard[nCntBill].nShadow = -1;
			}

			//�g�p����
			g_aBillboard[nCntBill].bUse = true;

			break;									//�����o��
		}
		pVtx += 4;									//���_�f�[�^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBillboard->Unlock();
}

//================================
//�ǂ̓����蔻��
//================================
void CollisionWall(Billboard *Billboard)
{
	if (Billboard->pos.x >= 300.0f ||
		Billboard->pos.x <= -300.0f ||
		Billboard->pos.z >= 300.0f ||
		Billboard->pos.z <= -300.0f)
	{//�e���X�e�[�W�̊O�ɏo���ꍇ
		//�����̃r���{�[�h��ݒ�
		SetBillboard(Billboard->rot, Billboard->pos, BILLBOARD_EXPL, D3DXVECTOR2(10.0f, 10.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 4, 8, true, false);

		//�g�p���Ă��Ȃ�
		Billboard->bUse = false;
	}
}

//================================
//�r���{�[�h�̎擾����
//================================
Billboard *GetBillboard(void)
{
	//�r���{�[�h�̏���Ԃ�
	return &g_aBillboard[0];
}

//================================
//�r���{�[�h�̍Đ�����
//================================
void PlayBillboard(BillboardAnim *Anim)
{
	//�Đ��J�E���^�[�����Z����
	Anim->nReplayCount += 1;

	if (Anim->nReplayCount % Anim->nAnimCounter == 0)
	{//�Đ��J�E���g���ݒ肳��Ă����J�E���g�ɓ͂����ꍇ
		//�p�^�[�������Z����
		Anim->nReplayPattern = (Anim->nReplayPattern + 1) % Anim->nAnimPattern;
	}
}