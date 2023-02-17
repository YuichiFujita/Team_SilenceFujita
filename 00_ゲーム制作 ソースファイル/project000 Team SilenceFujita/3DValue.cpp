//======================================================================================================================
//
//	3D���l���� [value.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "3DValue.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAX_3DVALUE		(9)			// �g�p����|���S���� (���̍ő吔)

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTexture3DValue[] =		// �e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\number000.png",	// ���� (�ʏ�) �̃e�N�X�`���̑��΃p�X
	"data\\TEXTURE\\number001.png",	// ���� (��) �̃e�N�X�`���̑��΃p�X
};

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTexture3DValue[VALUE3DTYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuff3DValue = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
D3DXMATRIX g_aMtx[MAX_3DVALUE];										// �}�g���b�N�X

//======================================================================================================================
//	3D���l�̏���������
//======================================================================================================================
void Init3DValue(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_3D         *pVtx;					// ���_���ւ̃|�C���^

	for (int nCnt3DValue = 0; nCnt3DValue < VALUE3DTYPE_MAX; nCnt3DValue++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

	  // �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, apTexture3DValue[nCnt3DValue], &g_apTexture3DValue[nCnt3DValue]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_3D) * 4 * MAX_3DVALUE,		// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuff3DValue,
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuff3DValue->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt3DValue = 0; nCnt3DValue < MAX_3DVALUE; nCnt3DValue++)
	{ // ���̍ő吔���J��Ԃ�

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// rhw �̐ݒ�
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
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuff3DValue->Unlock();
}

//======================================================================================================================
//	3D���l�̏I������
//======================================================================================================================
void Uninit3DValue(void)
{
	// �e�N�X�`���̔j��
	for (int nCnt3DValue = 0; nCnt3DValue < VALUE3DTYPE_MAX; nCnt3DValue++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTexture3DValue[nCnt3DValue] != NULL)
		{ // �ϐ� (g_apTexture3DValue) ��NULL�ł͂Ȃ��ꍇ

			g_apTexture3DValue[nCnt3DValue]->Release();
			g_apTexture3DValue[nCnt3DValue] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuff3DValue != NULL)
	{ // �ϐ� (g_pVtxBuff3DValue) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuff3DValue->Release();
		g_pVtxBuff3DValue = NULL;
	}
}

//======================================================================================================================
//	3D���l�̍X�V����
//======================================================================================================================
void Update3DValue(void)
{

}

//======================================================================================================================
//	3D���l�̕`�揈��
//======================================================================================================================
void Draw3DValue(D3DXVECTOR3 pos, int nDigit, int nType)
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

	for (int nCntDraw = 0; nCntDraw < nDigit; nCntDraw++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aMtx[nCntDraw]);

		//�r���[�}�g���b�N�X���擾
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//�|���S�����J�����Ɍ����Đ��ʂɌ�����
		D3DXMatrixInverse(&g_aMtx[nCntDraw], NULL, &mtxView);

		//�t�s������߂�
		g_aMtx[nCntDraw]._41 = 0.0f;
		g_aMtx[nCntDraw]._42 = 0.0f;
		g_aMtx[nCntDraw]._43 = 0.0f;

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&g_aMtx[nCntDraw], &g_aMtx[nCntDraw], &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aMtx[nCntDraw]);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0,
			g_pVtxBuff3DValue,									//���_�o�b�t�@�ւ̃|�C���^
			0,
			sizeof(VERTEX_3D));									//���_���\���̂̃T�C�Y

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTexture3DValue[nType]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//�v���~�e�B�u�̎��
			4 * nCntDraw,										//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);													//�`�悷��v���~�e�B�u��
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

//======================================================================================================================
//	3D���l�̐ݒ菈��
//======================================================================================================================
void Set3DValue(int n3DValue, int nMax3DValue, float fWidth, float fHeight, float fAlpha)
{
	// �ϐ���錾
	int nDigit = 1;			// �e�N�X�`�����W�v�Z�p

	// �ϐ��z���錾
	int aTexU[MAX_3DVALUE];	// �e���̐������i�[

	// �|�C���^��錾
	VERTEX_3D *pVtx;		// ���_���ւ̃|�C���^

	// �l�̕␳
	if (n3DValue > nMax3DValue)
	{ // �l���ő�l�𒴂����ꍇ

		// �l�ɍő�l����
		n3DValue = nMax3DValue;
	}
	else if (n3DValue < 0)
	{ // �l���ŏ��l����������ꍇ

		// �l�ɍŏ��l����
		n3DValue = 0;
	}

	for (int nCntDigit = 0; nCntDigit < MAX_3DVALUE; nCntDigit++)
	{ // ���̍ő吔���J��Ԃ�

		// �v�Z�p����o��
		nDigit *= 10;
	}

	for (int nCntTex = MAX_3DVALUE - 1; nCntTex >= 0; nCntTex--)
	{ // �����̍ő傩�猅���̍ŏ��܂ŌJ��Ԃ�

		// �e�N�X�`�����W��ݒ�
		aTexU[nCntTex] = (n3DValue % nDigit) / (nDigit / 10);

		// �v�Z�p����o��
		nDigit /= 10;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuff3DValue->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt3DValue = 0; nCnt3DValue < MAX_3DVALUE; nCnt3DValue++)
	{ // ���̍ő吔���J��Ԃ�

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-fWidth, +fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+fWidth, +fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+fWidth, -fHeight, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCnt3DValue] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCnt3DValue] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCnt3DValue] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCnt3DValue] * 0.1f + 0.1f, 1.0f);

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuff3DValue->Unlock();
}