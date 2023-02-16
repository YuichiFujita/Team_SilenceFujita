//============================================================
//
//	�A�C�R������ [icon.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "icon.h"
#include "player.h"
#include "camera.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_ICON			(1024)		// �g�p����|���S���� (�A�C�R���̍ő吔)

#define PLAY_ICON_RADIUS	(130.0f)	// �v���C���[�̃A�C�R���̔��a
#define PLAY_ICON_COL		(D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f))		// �v���C���[�̃A�C�R���̐F

#define EVIL_ICON_RADIUS	(100.0f)	// �����z�̃A�C�R���̔��a
#define EVIL_ICON_COL		(D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f))		// �����z�̃A�C�R���̐F

#define POLICE_ICON_RADIUS	(100.0f)	// �x�@�̃A�C�R���̔��a
#define POLICE_ICON_COL		(D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f))		// �x�@�̃A�C�R���̐F

#define ICON_CORRECT_RIGHT	(4000.0f)	// �A�C�R���̉E���̕␳�W��
#define ICON_CORRECT_LEFT	(4000.0f)	// �A�C�R���̍����̕␳�W��
#define ICON_CORRECT_FAR	(3800.0f)	// �A�C�R���̉����̕␳�W��
#define ICON_CORRECT_NEAR	(3800.0f)	// �A�C�R���̎�O���̕␳�W��

//************************************************************
//	�\���̒�` (Icon)
//************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// �ʒu
	ICONTYPE	type;				// ���
	float		fRadius;			// ���a
	D3DXMATRIX  mtxWorld;			// ���[���h�}�g���b�N�X
	int        *pIconIDParent;		// �A�C�R���̐e�̃A�C�R���C���f�b�N�X
	bool       *pUseParent;			// �A�C�R���̐e�̎g�p��
	bool        bUse;				// �g�p��
} Icon;

//************************************************************
//	�O���[�o���ϐ�
//************************************************************
LPDIRECT3DTEXTURE9      g_pTextureIcon = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffIcon = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

Icon g_aIcon[MAX_ICON];		// �A�C�R���̏��

//=======================================================================================================
//	�A�C�R���̏���������
//=======================================================================================================
void InitIcon(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_3D *pVtx;							// ���_���ւ̃|�C���^
	
	// �e�N�X�`����ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\aaa.jpg", &g_pTextureIcon);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_3D) * 4 * MAX_ICON,		// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffIcon,
		NULL
	);

	// �A�C�R���̏��̏�����
	for (int nCntIcon = 0; nCntIcon < MAX_ICON; nCntIcon++)
	{ // �A�C�R���̍ő�\�������J��Ԃ�

		g_aIcon[nCntIcon].pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aIcon[nCntIcon].type			= ICONTYPE_PLAY;					// �A�C�R��
		g_aIcon[nCntIcon].pIconIDParent = NULL;								// �A�C�R���̐e�̃A�C�R���C���f�b�N�X
		g_aIcon[nCntIcon].pUseParent	= NULL;								// �A�C�R���̐e�̎g�p��
		g_aIcon[nCntIcon].bUse			= false;							// �g�p��
	}

	//--------------------------------------------------------
	//	���_���̏�����
	//--------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntIcon = 0; nCntIcon < MAX_ICON; nCntIcon++)
	{ // �A�C�R���̍ő�\�������J��Ԃ�

	  // ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffIcon->Unlock();
}

//=======================================================================================================
//	�A�C�R���̏I������
//=======================================================================================================
void UninitIcon(void)
{
	if (g_pTextureIcon != NULL)
	{ // �ϐ� (g_pTextureIcon) ��NULL�ł͂Ȃ��ꍇ

		g_pTextureIcon->Release();
		g_pTextureIcon = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffIcon != NULL)
	{ // �ϐ� (g_pVtxBuffIcon) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffIcon->Release();
		g_pVtxBuffIcon = NULL;
	}
}

//=======================================================================================================
//	�A�C�R���̍X�V����
//=======================================================================================================
void UpdateIcon(void)
{
	D3DXVECTOR3 cameraPos = GetCamera(CAMERATYPE_MAP)->posV;	// �J�����̎��_���擾

	for (int nCntIcon = 0; nCntIcon < MAX_ICON; nCntIcon++)
	{ // �A�C�R���̍ő�\�������J��Ԃ�

		if (g_aIcon[nCntIcon].bUse == true && g_aIcon[nCntIcon].pUseParent != NULL)
		{ // �e���g�p����Ă��銎�A�e�̐e�̎g�p�󋵃A�h���X���ݒ肳��Ă���ꍇ

			if (*g_aIcon[nCntIcon].pUseParent == false)
			{ // �e�̐e���g�p����Ă��Ȃ��ꍇ

				// �e���g�p���Ă��Ȃ���Ԃɂ���
				g_aIcon[nCntIcon].bUse = false;
			}
			else
			{ // �e�̐e���g�p����Ă���ꍇ

				if (g_aIcon[nCntIcon].pIconIDParent != NULL)
				{ // �e�̐e�̉e�C���f�b�N�X�A�h���X���ݒ肳��Ă���ꍇ

					if (*g_aIcon[nCntIcon].pIconIDParent != nCntIcon)
					{ // �e�̐e�̉e�C���f�b�N�X�ƁA���݂̉e�C���f�b�N�X�̒l���Ⴄ�ꍇ

						// �e���g�p���Ă��Ȃ���Ԃɂ���
						g_aIcon[nCntIcon].bUse = false;

						// [��] �g�p���Ȃ���ԂɂȂ����e�����t���[�����œ����C���f�b�N�X���g����
						//      �܂��g�p���ꂽ�ꍇ�ɉe����̐e�ɓ��������Ă��܂����߁A���̖h�~�B
					}
				}

				if (cameraPos.x + ICON_CORRECT_RIGHT <= g_aIcon[nCntIcon].pos.x + g_aIcon[nCntIcon].fRadius)
				{ // �E�������O���ɃA�C�R��������ꍇ
					// �A�C�R����␳����
					g_aIcon[nCntIcon].pos.x = cameraPos.x + ICON_CORRECT_RIGHT - g_aIcon[nCntIcon].fRadius;
				}

				if (cameraPos.x - ICON_CORRECT_LEFT >= g_aIcon[nCntIcon].pos.x - g_aIcon[nCntIcon].fRadius)
				{ // ���������O���ɃA�C�R��������ꍇ
					// �A�C�R����␳����
					g_aIcon[nCntIcon].pos.x = cameraPos.x - ICON_CORRECT_LEFT + g_aIcon[nCntIcon].fRadius;
				}

				if (cameraPos.z + ICON_CORRECT_FAR <= g_aIcon[nCntIcon].pos.z + g_aIcon[nCntIcon].fRadius)
				{ // ���������O���ɃA�C�R��������ꍇ
					// �A�C�R����␳����
					g_aIcon[nCntIcon].pos.z = cameraPos.z + ICON_CORRECT_FAR - g_aIcon[nCntIcon].fRadius;
				}

				if (cameraPos.z - ICON_CORRECT_NEAR >= g_aIcon[nCntIcon].pos.z - g_aIcon[nCntIcon].fRadius)
				{ // ��O�������O���ɃA�C�R��������ꍇ
					// �A�C�R����␳����
					g_aIcon[nCntIcon].pos.z = cameraPos.z - ICON_CORRECT_NEAR + g_aIcon[nCntIcon].fRadius;
				}
			}
		}
	}
}

//=======================================================================================================
//	�A�C�R���̕`�揈��
//=======================================================================================================
void DrawIcon(void)
{
	D3DXMATRIX mtxTrans;			//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;				//�r���[�}�g���b�N�X�擾�p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	//���C�e�B���O��OFF�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);					//Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						//Z�e�X�g�̗L��/�����ݒ�

	for (int nCntIcon = 0; nCntIcon < MAX_ICON; nCntIcon++)
	{ // �A�C�R���̍ő�\�������J��Ԃ�

		if (g_aIcon[nCntIcon].bUse == true)
		{ // �A�C�R�����g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aIcon[nCntIcon].mtxWorld);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aIcon[nCntIcon].pos.x, g_aIcon[nCntIcon].pos.y, g_aIcon[nCntIcon].pos.z);
			D3DXMatrixMultiply(&g_aIcon[nCntIcon].mtxWorld, &g_aIcon[nCntIcon].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aIcon[nCntIcon].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffIcon, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureIcon);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntIcon * 4, 2);
		}
	}

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			//Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				//Z�e�X�g�̗L��/�����ݒ�

	//���C�e�B���O��ON�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//===============================
//�e�̐ݒ菈��
//===============================
int SetIcon(D3DXVECTOR3 pos, ICONTYPE type, int *pIconID, bool *pUse)
{
	//�e�̔ԍ�������������
	int nIdxIcon = NONE_ICON;

	D3DXVECTOR3 playerPos = GetPlayer()->pos;		// �v���C���[�̈ʒu���擾

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntIcon = 0; nCntIcon < MAX_ICON; nCntIcon++)
	{
		if (g_aIcon[nCntIcon].bUse == false)
		{//�g�p���Ă��Ȃ������ꍇ

			//�ʒu��ݒ肷��
			g_aIcon[nCntIcon].pos.x = pos.x;
			g_aIcon[nCntIcon].pos.z = pos.z;

			// ��ނ�ݒ肷��
			g_aIcon[nCntIcon].type = type;

			// �����̃A�h���X��ݒ肷��
			g_aIcon[nCntIcon].pIconIDParent = pIconID;
			g_aIcon[nCntIcon].pUseParent = pUse;

			switch (g_aIcon[nCntIcon].type)
			{
			case ICONTYPE_PLAY:		// �v���C���[

				// ���a��ݒ�
				g_aIcon[nCntIcon].fRadius = PLAY_ICON_RADIUS;		

				// ���_�J���[�̐ݒ�
				pVtx[0].col = PLAY_ICON_COL;
				pVtx[1].col = PLAY_ICON_COL;
				pVtx[2].col = PLAY_ICON_COL;
				pVtx[3].col = PLAY_ICON_COL;

				break;				// �����o��

			case ICONTYPE_EVIL:		// �����z

				// ���a��ݒ�
				g_aIcon[nCntIcon].fRadius = EVIL_ICON_RADIUS;

				// ���_�J���[�̐ݒ�
				pVtx[0].col = EVIL_ICON_COL;
				pVtx[1].col = EVIL_ICON_COL;
				pVtx[2].col = EVIL_ICON_COL;
				pVtx[3].col = EVIL_ICON_COL;

				break;				// �����o��

			case ICONTYPE_POLICE:	// �x�@

				// ���a��ݒ�
				g_aIcon[nCntIcon].fRadius = POLICE_ICON_RADIUS;

				// ���_�J���[�̐ݒ�
				pVtx[0].col = POLICE_ICON_COL;
				pVtx[1].col = POLICE_ICON_COL;
				pVtx[2].col = POLICE_ICON_COL;
				pVtx[3].col = POLICE_ICON_COL;

				break;				// �����o��
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aIcon[nCntIcon].fRadius, +0.0f, +g_aIcon[nCntIcon].fRadius);
			pVtx[1].pos = D3DXVECTOR3(+g_aIcon[nCntIcon].fRadius, +0.0f, +g_aIcon[nCntIcon].fRadius);
			pVtx[2].pos = D3DXVECTOR3(-g_aIcon[nCntIcon].fRadius, +0.0f, -g_aIcon[nCntIcon].fRadius);
			pVtx[3].pos = D3DXVECTOR3(+g_aIcon[nCntIcon].fRadius, +0.0f, -g_aIcon[nCntIcon].fRadius);

			//�g�p����
			g_aIcon[nCntIcon].bUse = true;

			//�e�̔ԍ����R�s�[����
			nIdxIcon = nCntIcon;

			break;				//�����o��
		}
		pVtx += 4;				//���_�f�[�^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffIcon->Unlock();

	//�e�̔ԍ���Ԃ�
	return nIdxIcon;
}

//=======================================================================================================
//	�A�C�R���̈ʒu�̐ݒ菈��
//=======================================================================================================
void SetPositionIcon(int nIconID, D3DXVECTOR3 pos)
{
	// �ϐ���錾
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	if (nIconID > NONE_ICON)
	{ // �g�p�ł���ID�̏ꍇ

		// �����̃A�C�R���̈ʒu��ݒ�
		g_aIcon[nIconID].pos = pos;
	}
}
