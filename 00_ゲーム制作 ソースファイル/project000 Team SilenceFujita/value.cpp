//======================================================================================================================
//
//	���l���� [value.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "value.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAX_VALUE		(9)			// �g�p����|���S���� (���̍ő吔)

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTextureValue[] =		// �e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\number000.tga",	// ���� (�ʏ�) �̃e�N�X�`���̑��΃p�X
	"data\\TEXTURE\\number001.tga",	// ���� (��) �̃e�N�X�`���̑��΃p�X
};

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureValue[VALUETYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffValue = NULL;					// ���_�o�b�t�@�ւ̃|�C���^

//======================================================================================================================
//	���l�̏���������
//======================================================================================================================
void InitValue(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D         *pVtx;					// ���_���ւ̃|�C���^

	for (int nCntValue = 0; nCntValue < VALUETYPE_MAX; nCntValue++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, apTextureValue[nCntValue], &g_apTextureValue[nCntValue]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_2D) * 4 * MAX_VALUE,		// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffValue,
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffValue->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntValue = 0; nCntValue < MAX_VALUE; nCntValue++)
	{ // ���̍ő吔���J��Ԃ�

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// rhw �̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

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
	g_pVtxBuffValue->Unlock();
}

//======================================================================================================================
//	���l�̏I������
//======================================================================================================================
void UninitValue(void)
{
	// �e�N�X�`���̔j��
	for (int nCntValue = 0; nCntValue < VALUETYPE_MAX; nCntValue++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTextureValue[nCntValue] != NULL)
		{ // �ϐ� (g_apTextureValue) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureValue[nCntValue]->Release();
			g_apTextureValue[nCntValue] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffValue != NULL)
	{ // �ϐ� (g_pVtxBuffValue) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffValue->Release();
		g_pVtxBuffValue = NULL;
	}
}

//======================================================================================================================
//	���l�̍X�V����
//======================================================================================================================
void UpdateValue(void)
{

}

//======================================================================================================================
//	���l�̕`�揈��
//======================================================================================================================
void DrawValue(int nDigit, int nType)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^ (�|�C���^�̐錾���� * �͂���Ȃ�)

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffValue, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntValue = 0; nCntValue < nDigit; nCntValue++)
	{ // �����̌������J��Ԃ�

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureValue[nType]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntValue * 4, 2);
	}
}

//======================================================================================================================
//	���l�̐ݒ菈��
//======================================================================================================================
void SetValue(D3DXVECTOR3 pos, int nValue, int nMaxValue, float fWidth, float fHeight, float fSpace, float fAlpha)
{
	// �ϐ���錾
	int nDigit = 1;			// �e�N�X�`�����W�v�Z�p

	// �ϐ��z���錾
	int aTexU[MAX_VALUE];	// �e���̐������i�[

	// �|�C���^��錾
	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// �l�̕␳
	if (nValue > nMaxValue)
	{ // �l���ő�l�𒴂����ꍇ

		// �l�ɍő�l����
		nValue = nMaxValue;
	}
	else if (nValue < 0)
	{ // �l���ŏ��l����������ꍇ

		// �l�ɍŏ��l����
		nValue = 0;
	}

	for (int nCntDigit = 0; nCntDigit < MAX_VALUE; nCntDigit++)
	{ // ���̍ő吔���J��Ԃ�

		// �v�Z�p����o��
		nDigit *= 10;
	}

	for (int nCntTex = MAX_VALUE - 1; nCntTex >= 0; nCntTex--)
	{ // �����̍ő傩�猅���̍ŏ��܂ŌJ��Ԃ�

		// �e�N�X�`�����W��ݒ�
		aTexU[nCntTex] = (nValue % nDigit) / (nDigit / 10);

		// �v�Z�p����o��
		nDigit /= 10;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffValue->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntValue = 0; nCntValue < MAX_VALUE; nCntValue++)
	{ // ���̍ő吔���J��Ԃ�

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3((pos.x - fWidth) - (nCntValue * fSpace), pos.y - fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3((pos.x + fWidth) - (nCntValue * fSpace), pos.y - fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3((pos.x - fWidth) - (nCntValue * fSpace), pos.y + fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((pos.x + fWidth) - (nCntValue * fSpace), pos.y + fHeight, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntValue] * 0.1f,        0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntValue] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntValue] * 0.1f,        1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntValue] * 0.1f + 0.1f, 1.0f);

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffValue->Unlock();
}