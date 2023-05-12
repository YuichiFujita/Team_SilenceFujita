//=======================================
//
//�Z�b�g�}�[�N�̃��C������[tiremark.cpp]
//Author ��������
//
//=======================================
#include "main.h"
#include "tiremark.h"

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTireMark = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
TireMark g_aTireMark[MAX_TIREMARK];					// �Z�b�g�}�[�N�̏��

//==========================================
// �^�C�����̏���������
//==========================================
void InitTireMark(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�����̏��̏�����
	for (int nCntTireMark = 0; nCntTireMark < MAX_TIREMARK; nCntTireMark++)
	{

		// �ʒu������������
		g_aTireMark[nCntTireMark].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// ����������������
		g_aTireMark[nCntTireMark].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �F������������
		g_aTireMark[nCntTireMark].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �c�̍L��������������
		g_aTireMark[nCntTireMark].fDepth = 0.0f;

		// ���̍L��������������
		g_aTireMark[nCntTireMark].fWidth = 0.0f;

		// �����J�E���g������������
		g_aTireMark[nCntTireMark].nClearCount = 0;

		// �g�p���Ă��Ȃ�
		g_aTireMark[nCntTireMark].bUse = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_TIREMARK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTireMark,
		NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffTireMark->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTireMark = 0; nCntTireMark < MAX_TIREMARK; nCntTireMark++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_aTireMark[nCntTireMark].fWidth, 0.0f, +g_aTireMark[nCntTireMark].fDepth);
		pVtx[1].pos = D3DXVECTOR3(+g_aTireMark[nCntTireMark].fWidth, 0.0f, +g_aTireMark[nCntTireMark].fDepth);
		pVtx[2].pos = D3DXVECTOR3(-g_aTireMark[nCntTireMark].fWidth, 0.0f, -g_aTireMark[nCntTireMark].fDepth);
		pVtx[3].pos = D3DXVECTOR3(+g_aTireMark[nCntTireMark].fWidth, 0.0f, -g_aTireMark[nCntTireMark].fDepth);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_aTireMark[nCntTireMark].col;
		pVtx[1].col = g_aTireMark[nCntTireMark].col;
		pVtx[2].col = g_aTireMark[nCntTireMark].col;
		pVtx[3].col = g_aTireMark[nCntTireMark].col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTireMark->Unlock();
}

//========================================
//�o���l�̏I������
//========================================
void UninitTireMark(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTireMark != NULL)
	{
		g_pVtxBuffTireMark->Release();
		g_pVtxBuffTireMark = NULL;
	}
}

//========================================
//�o���l�̍X�V����
//========================================
void UpdateTireMark(void)
{
	VERTEX_3D * pVtx;				//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTireMark->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTireMark = 0; nCntTireMark < MAX_TIREMARK; nCntTireMark++)
	{
		if (g_aTireMark[nCntTireMark].bUse == true)
		{//�g�p���Ă����ꍇ
			// �����J�E���g�����Z����
			g_aTireMark[nCntTireMark].nClearCount++;

			if (g_aTireMark[nCntTireMark].nClearCount >= 50)
			{ // �����J�E���g��30�𒴂����ꍇ
				// �����x��i�X�����Ă���
				g_aTireMark[nCntTireMark].col.a -= 0.001f;
			}

			if (g_aTireMark[nCntTireMark].col.a <= 0.0f)
			{ // �����x��0.0f���z�����ꍇ
				// �����x��0.0f�ɂ���
				g_aTireMark[nCntTireMark].col.a = 0.0f;

				//�g�p���Ȃ�
				g_aTireMark[nCntTireMark].bUse = false;
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aTireMark[nCntTireMark].fWidth, 0.0f, +g_aTireMark[nCntTireMark].fDepth);
			pVtx[1].pos = D3DXVECTOR3(+g_aTireMark[nCntTireMark].fWidth, 0.0f, +g_aTireMark[nCntTireMark].fDepth);
			pVtx[2].pos = D3DXVECTOR3(-g_aTireMark[nCntTireMark].fWidth, 0.0f, -g_aTireMark[nCntTireMark].fDepth);
			pVtx[3].pos = D3DXVECTOR3(+g_aTireMark[nCntTireMark].fWidth, 0.0f, -g_aTireMark[nCntTireMark].fDepth);

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aTireMark[nCntTireMark].col;
			pVtx[1].col = g_aTireMark[nCntTireMark].col;
			pVtx[2].col = g_aTireMark[nCntTireMark].col;
			pVtx[3].col = g_aTireMark[nCntTireMark].col;
		}

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTireMark->Unlock();
}

//=====================================
//�o���l�̕`�揈��
//=====================================
void DrawTireMark(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	D3DXMATRIX mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;						// �r���[�}�g���b�N�X�擾�p

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//���C�e�B���O��OFF�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCntTireMark = 0; nCntTireMark < MAX_TIREMARK; nCntTireMark++)
	{
		if (g_aTireMark[nCntTireMark].bUse == true)
		{//�Z�b�g�}�[�N���g�p����Ă����ꍇ
		 //���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aTireMark[nCntTireMark].mtx);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aTireMark[nCntTireMark].rot.y, g_aTireMark[nCntTireMark].rot.x, g_aTireMark[nCntTireMark].rot.z);
			D3DXMatrixMultiply(&g_aTireMark[nCntTireMark].mtx, &g_aTireMark[nCntTireMark].mtx, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aTireMark[nCntTireMark].pos.x, g_aTireMark[nCntTireMark].pos.y, g_aTireMark[nCntTireMark].pos.z);
			D3DXMatrixMultiply(&g_aTireMark[nCntTireMark].mtx, &g_aTireMark[nCntTireMark].mtx, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aTireMark[nCntTireMark].mtx);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0,
				g_pVtxBuffTireMark,									//���_�o�b�t�@�ւ̃|�C���^
				0,
				sizeof(VERTEX_3D));									//���_���\���̂̃T�C�Y

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//�v���~�e�B�u�̎��
				nCntTireMark * 4,									//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);													//�`�悷��v���~�e�B�u��
		}
	}

	//���C�e�B���O��ON�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//========================================
//�o���l�̐ݒ菈��
//========================================
void SetTireMark(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	VERTEX_3D * pVtx;					//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTireMark->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTireMark = 0; nCntTireMark < MAX_TIREMARK; nCntTireMark++)
	{
		if (g_aTireMark[nCntTireMark].bUse == false)
		{//�e���g�p����Ă��Ȃ�

			// ����ݒ肷��
			g_aTireMark[nCntTireMark].pos = pos;
			g_aTireMark[nCntTireMark].rot = rot;
			g_aTireMark[nCntTireMark].fDepth = 8.0f;
			g_aTireMark[nCntTireMark].fWidth = 12.0f;
			g_aTireMark[nCntTireMark].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			g_aTireMark[nCntTireMark].nClearCount = 0;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aTireMark[nCntTireMark].fWidth, 0.0f, +g_aTireMark[nCntTireMark].fDepth);
			pVtx[1].pos = D3DXVECTOR3(+g_aTireMark[nCntTireMark].fWidth, 0.0f, +g_aTireMark[nCntTireMark].fDepth);
			pVtx[2].pos = D3DXVECTOR3(-g_aTireMark[nCntTireMark].fWidth, 0.0f, -g_aTireMark[nCntTireMark].fDepth);
			pVtx[3].pos = D3DXVECTOR3(+g_aTireMark[nCntTireMark].fWidth, 0.0f, -g_aTireMark[nCntTireMark].fDepth);

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aTireMark[nCntTireMark].col;
			pVtx[1].col = g_aTireMark[nCntTireMark].col;
			pVtx[2].col = g_aTireMark[nCntTireMark].col;
			pVtx[3].col = g_aTireMark[nCntTireMark].col;

			//�g�p���Ă����Ԃɂ���
			g_aTireMark[nCntTireMark].bUse = true;				//�g�p���Ă����Ԃɂ���

			break;
		}
		pVtx += 4;//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTireMark->Unlock();
}

//========================================
//�Z�b�g�}�[�N�̎擾����
//========================================
TireMark *GetTireMark(void)
{
	//�Z�b�g�}�[�N�̏����擾����
	return &g_aTireMark[0];
}