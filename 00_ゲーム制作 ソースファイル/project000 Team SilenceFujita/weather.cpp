//======================================================================================================================
//
//	�V�C���� [weather.cpp]
//	Author�F��������
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "weather.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define RAIN_TEXTURE		"data\\TEXTURE\\effect000.jpg"		// �J�̃e�N�X�`��

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureWeather = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWeather = NULL;		// ���_�o�b�t�@�ւ̃|�C���^

Rain g_aRain[MAX_RAIN];		// �G�t�F�N�g�̏��

//======================================================================================================================
//	�G�t�F�N�g�̏���������
//======================================================================================================================
void InitWeather(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_3D *pVtx;							// ���_���ւ̃|�C���^

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, RAIN_TEXTURE, &g_apTextureWeather);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_3D) * 4 * MAX_RAIN,		// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffWeather,
		NULL
	);

	// �J�̏��̏�����
	for (int nCntWeather = 0; nCntWeather < MAX_RAIN; nCntWeather++)
	{ // �J�̍ő�\�������J��Ԃ�

		g_aRain[nCntWeather].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_aRain[nCntWeather].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
		g_aRain[nCntWeather].fRadius = 0.0f;							// ���a
		g_aRain[nCntWeather].bUse = false;								// �g�p��

	}

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWeather->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_RAIN; nCntWeather++)
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
	g_pVtxBuffWeather->Unlock();
}

//======================================================================================================================
//	�G�t�F�N�g�̏I������
//======================================================================================================================
void UninitWeather(void)
{
	if (g_apTextureWeather != NULL)
	{ // �ϐ� (g_apTextureWeather) ��NULL�ł͂Ȃ��ꍇ

		g_apTextureWeather->Release();
		g_apTextureWeather = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffWeather != NULL)
	{ // �ϐ� (g_pVtxBuffWeather) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffWeather->Release();
		g_pVtxBuffWeather = NULL;
	}
}

//======================================================================================================================
//	�G�t�F�N�g�̍X�V����
//======================================================================================================================
void UpdateWeather(void)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

						// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWeather->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_RAIN; nCntWeather++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aRain[nCntWeather].bUse == true)
		{ // �G�t�F�N�g���g�p����Ă���ꍇ

			// �ړ��̍X�V
			g_aRain[nCntWeather].pos += g_aRain[nCntWeather].move;

			// ���a�̕␳
			if (g_aRain[nCntWeather].fRadius < 0.0f)
			{ // ���a�� 0.0f ��菬�����ꍇ

				// ���a�� 0.0f ����
				g_aRain[nCntWeather].fRadius = 0.0f;
			}

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aRain[nCntWeather].fRadius, +g_aRain[nCntWeather].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aRain[nCntWeather].fRadius, +g_aRain[nCntWeather].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aRain[nCntWeather].fRadius, -g_aRain[nCntWeather].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aRain[nCntWeather].fRadius, -g_aRain[nCntWeather].fRadius, 0.0f);
		}

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWeather->Unlock();
}

//======================================================================================================================
//	�G�t�F�N�g�̕`�揈��
//======================================================================================================================
void DrawWeather(void)
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

	for (int nCntWeather = 0; nCntWeather < MAX_RAIN; nCntWeather++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aRain[nCntWeather].bUse == true)
		{ // �G�t�F�N�g���g�p����Ă���ꍇ

		  // ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aRain[nCntWeather].mtxWorld);

			// �r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aRain[nCntWeather].mtxWorld, NULL, &mtxView);	// �t�s������߂�
			g_aRain[nCntWeather].mtxWorld._41 = 0.0f;
			g_aRain[nCntWeather].mtxWorld._42 = 0.0f;
			g_aRain[nCntWeather].mtxWorld._43 = 0.0f;

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aRain[nCntWeather].pos.x, g_aRain[nCntWeather].pos.y, g_aRain[nCntWeather].pos.z);
			D3DXMatrixMultiply(&g_aRain[nCntWeather].mtxWorld, &g_aRain[nCntWeather].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aRain[nCntWeather].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffWeather, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureWeather);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWeather * 4, 2);
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
//	�J�̐ݒ菈��
//======================================================================================================================
void SetRain(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRadius)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

						// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWeather->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_RAIN; nCntWeather++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aRain[nCntWeather].bUse == false)
		{ // �G�t�F�N�g���g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aRain[nCntWeather].pos = pos;			// �ʒu
			g_aRain[nCntWeather].move = move;		// �ړ���
			g_aRain[nCntWeather].fRadius = fRadius;	// ���a

			// �g�p���Ă����Ԃɂ���
			g_aRain[nCntWeather].bUse = true;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aRain[nCntWeather].fRadius, +g_aRain[nCntWeather].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aRain[nCntWeather].fRadius, +g_aRain[nCntWeather].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aRain[nCntWeather].fRadius, -g_aRain[nCntWeather].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aRain[nCntWeather].fRadius, -g_aRain[nCntWeather].fRadius, 0.0f);

			// �����𔲂���
			break;
		}

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWeather->Unlock();
}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================
////**********************************************************************************************************************
////	�G�t�F�N�g�̑����擾����
////**********************************************************************************************************************
//int GetNumWeather(void)
//{
//	// �ϐ���錾
//	int nNumWeather = 0;	// �G�t�F�N�g�̑����̊m�F�p
//
//	for (int nCntWeather = 0; nCntWeather < MAX_EFFECT; nCntWeather++)
//	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�
//
//		if (g_aWeather[nCntWeather].bUse == true)
//		{ // �G�t�F�N�g���g�p����Ă���ꍇ
//
//		  // �J�E���^�[�����Z
//			nNumWeather++;
//		}
//	}
//
//	// �ϐ��̒l��Ԃ�
//	return nNumWeather;	// �G�t�F�N�g�̑���
//}
#endif