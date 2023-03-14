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
#include "game.h"
#include "player.h"
#include "camera.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_ICON			(1500)		// �g�p����|���S���� (�A�C�R���̍ő吔)

#define PLAY_ICON_RADIUS	(350.0f)								// �v���C���[�̃A�C�R���̔��a
#define PLAY_ICON_COL		(D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f))		// �v���C���[�̃A�C�R���̐F

#define EVIL_ICON_RADIUS	(250.0f)								// �����z�̃A�C�R���̔��a
#define EVIL_ICON_COL		(D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f))		// �����z�̃A�C�R���̐F

#define POLICE_ICON_RADIUS	(250.0f)								// �x�@�̃A�C�R���̔��a
#define POLICE_ICON_COL		(D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f))		// �x�@�̃A�C�R���̐F

#define BARRIER_ICON_SET_RADIUS		(0.0f)							// �A�C�R���̐ݒ莞�̔��a
#define BARRIER_ICON_RADIUS	(300.0f)								// �o���A�̃A�C�R���̔��a
#define BARRIER_ICON_COL	(D3DXCOLOR(0.4f,1.0f,1.0f,1.0f))		// �o���A�̃A�C�R���̐F

#define GATE_ICON_RADIUS	(420.0f)								// �Q�[�g�A�C�R���̔��a
#define GATE_ICON_COL		(D3DXCOLOR(1.0f, 0.6f, 1.0f, 1.0f))		// �Q�[�g�̃A�C�R���̐F

#define ITEM_ICON_RADIUS	(120.0f)								// �A�C�e���̃A�C�R���̔��a
#define ITEM_ICON_COL		(D3DXCOLOR(0.4f, 1.0f, 1.0f, 1.0f))		// �A�C�e���̃A�C�R���̐F

#define OBJECT_ICON_COL		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))		// �I�u�W�F�N�g�̃A�C�R���̐F

#define ICON_CORRECT_RIGHT	(4000.0f)	// �A�C�R���̉E���̕␳�W��
#define ICON_CORRECT_LEFT	(4000.0f)	// �A�C�R���̍����̕␳�W��
#define ICON_CORRECT_FAR	(3800.0f)	// �A�C�R���̉����̕␳�W��
#define ICON_CORRECT_NEAR	(3800.0f)	// �A�C�R���̎�O���̕␳�W��

#define ICON_ALPHA_CHANGE	(0.005f)	// �A�C�R���̓����x�̕ω���
#define ICON_REVIVAL_CNT	(20)		// �������̃J�E���g
#define ICON_DAMAGE_COL		(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))		// �_���[�W���̐F
#define ICON_EMPHASIS_COL	(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))
#define ICON_UNRIVALED_CNT	(10)		// ���G��Ԃ̃J�E���g
#define ICON_SCALE_CHANGE	(50.0f);	// �g��k�����̔��a�̊g�嗦

//************************************************************
//	�O���[�o���ϐ�
//************************************************************
LPDIRECT3DTEXTURE9      g_pTextureIcon[ICONTYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffIcon = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

Icon g_aIcon[MAX_ICON];		// �A�C�R���̏��

// �e�N�X�`���t�@�C����
const char *c_apTextureFileIcon[ICONTYPE_MAX] =
{
	"data\\TEXTURE\\icon006.png",
	"data\\TEXTURE\\icon006.png",
	"data\\TEXTURE\\icon001.png",
	"data\\TEXTURE\\icon002.png",
	"data\\TEXTURE\\icon000.png",
	"data\\TEXTURE\\icon004.png",
	"data\\TEXTURE\\icon004.png",
	"data\\TEXTURE\\icon005.png",
	"data\\TEXTURE\\icon005.png",
	"data\\TEXTURE\\icon003.png",
};

//=======================================================================================================
//	�A�C�R���̏���������
//=======================================================================================================
void InitIcon(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_3D *pVtx;							// ���_���ւ̃|�C���^
	
	for (int nCntTex = 0; nCntTex < ICONTYPE_MAX; nCntTex++)
	{
		// �e�N�X�`����ǂݍ���
		D3DXCreateTextureFromFile(pDevice, c_apTextureFileIcon[nCntTex], &g_pTextureIcon[nCntTex]);
	}

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

		g_aIcon[nCntIcon].pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_aIcon[nCntIcon].type			= ICONTYPE_PLAY;						// �A�C�R��
		g_aIcon[nCntIcon].col			= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
		g_aIcon[nCntIcon].colCopy		= g_aIcon[nCntIcon].col;				// �F�̃R�s�[
		g_aIcon[nCntIcon].nCounter		= 0;									// �J�E���^�[
		g_aIcon[nCntIcon].radius		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���a
		g_aIcon[nCntIcon].alpha			= ICONALPHA_NONE;						// �����x�̏��
		g_aIcon[nCntIcon].pIconIDParent = NULL;									// �A�C�R���̐e�̃A�C�R���C���f�b�N�X
		g_aIcon[nCntIcon].pUseParent	= NULL;									// �A�C�R���̐e�̎g�p��
		g_aIcon[nCntIcon].bUse			= false;								// �g�p��
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
		pVtx[0].col = g_aIcon[nCntIcon].col;
		pVtx[1].col = g_aIcon[nCntIcon].col;
		pVtx[2].col = g_aIcon[nCntIcon].col;
		pVtx[3].col = g_aIcon[nCntIcon].col;

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
	for (int nCntTex = 0; nCntTex < ICONTYPE_MAX; nCntTex++)
	{
		if (g_pTextureIcon[nCntTex] != NULL)
		{ // �ϐ� (g_pTextureIcon) ��NULL�ł͂Ȃ��ꍇ

			g_pTextureIcon[nCntTex]->Release();
			g_pTextureIcon[nCntTex] = NULL;
		}
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

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_3D *pVtx;							// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntIcon = 0; nCntIcon < MAX_ICON; nCntIcon++)
	{ // �A�C�R���̍ő�\�������J��Ԃ�

		if (g_aIcon[nCntIcon].bUse == true && g_aIcon[nCntIcon].pUseParent != NULL)
		{ // �A�C�R�����g�p����Ă��銎�A�A�C�R���̐e�̎g�p�󋵃A�h���X���ݒ肳��Ă���ꍇ

			if (*g_aIcon[nCntIcon].pUseParent == false)
			{ // �A�C�R���̐e���g�p����Ă��Ȃ��ꍇ

				// �A�C�R�����g�p���Ă��Ȃ���Ԃɂ���
				g_aIcon[nCntIcon].bUse = false;
			}
			else
			{ // �A�C�R���̐e���g�p����Ă���ꍇ

				if (g_aIcon[nCntIcon].pIconIDParent != NULL)
				{ // �A�C�R���̐e�̃A�C�R���C���f�b�N�X�A�h���X���ݒ肳��Ă���ꍇ

					if (*g_aIcon[nCntIcon].pIconIDParent != nCntIcon)
					{ // �A�C�R���̐e�̃A�C�R���C���f�b�N�X�ƁA���݂̃A�C�R���C���f�b�N�X�̒l���Ⴄ�ꍇ

						// �A�C�R�����g�p���Ă��Ȃ���Ԃɂ���
						g_aIcon[nCntIcon].bUse = false;

						// [��] �g�p���Ȃ���ԂɂȂ����e�����t���[�����œ����C���f�b�N�X���g����
						//      �܂��g�p���ꂽ�ꍇ�ɃA�C�R������̐e�ɓ��������Ă��܂����߁A���̖h�~�B
					}
				}

				switch (*g_aIcon[nCntIcon].pState)
				{
				case ICONSTATE_NONE:		// �����Ȃ�

					// �J�E���^�[������������
					g_aIcon[nCntIcon].nCounter = 0;

					// �����x��ݒ肷��
					g_aIcon[nCntIcon].col.a = 1.0f;

					// �����x�̈ʂ�ݒ肷��
					g_aIcon[nCntIcon].alpha = ICONALPHA_NONE;

					// �F��ݒ肷��
					g_aIcon[nCntIcon].col = g_aIcon[nCntIcon].colCopy;

					break;					// �����o��

				case ICONSTATE_DISAPPEAR:	// ������r��

					// �J�E���^�[������������
					g_aIcon[nCntIcon].nCounter = 0;

					// �����x��������
					g_aIcon[nCntIcon].col.a -= ICON_ALPHA_CHANGE;

					if (g_aIcon[nCntIcon].col.a <= 0.0f)
					{ //�����x��1.0f�𒴂����ꍇ

						// �����x��1.0f�ɂ���
						g_aIcon[nCntIcon].col.a = 0.0f;
					}

					break;					// �����o��

				case ICONSTATE_REVIVAL:		// ������

					// �J�E���^�[�����Z����
					g_aIcon[nCntIcon].nCounter++;

					if (g_aIcon[nCntIcon].nCounter % ICON_REVIVAL_CNT == 0)
					{ // ��萔���Ƃ�

						// �����x�̏�Ԃ�ݒ�
						g_aIcon[nCntIcon].alpha = (ICONALPHA)((g_aIcon[nCntIcon].alpha + 1) % ICONALPHA_MAX);
					}

					switch (g_aIcon[nCntIcon].alpha)
					{
					case ICONALPHA_NONE:	// �s����

						// �����x��ݒ肷��
						g_aIcon[nCntIcon].col.a = 1.0f;

						break;				// �����o��

					case ICONALPHA_CLEAR:	// ����

						// �����x��ݒ肷��
						g_aIcon[nCntIcon].col.a = 0.0f;

						break;				// �����o��
					}

					break;					// �����o��

				case ICONSTATE_DAMAGE:		// �_���[�W��

					// �J�E���^�[������������
					g_aIcon[nCntIcon].nCounter = 0;

					// �F��Ԃ�����
					g_aIcon[nCntIcon].col = ICON_DAMAGE_COL;

					break;					// �����o��

				case ICONSTATE_UNRIVALED:	// ���G���

					// �F��ݒ肷��
					g_aIcon[nCntIcon].col = g_aIcon[nCntIcon].colCopy;

					// �J�E���^�[�����Z����
					g_aIcon[nCntIcon].nCounter++;

					if (g_aIcon[nCntIcon].nCounter % ICON_UNRIVALED_CNT == 0)
					{ // ��萔���Ƃ�

						// �����x�̏�Ԃ�ݒ�
						g_aIcon[nCntIcon].alpha = (ICONALPHA)((g_aIcon[nCntIcon].alpha + 1) % ICONALPHA_MAX);
					}

					switch (g_aIcon[nCntIcon].alpha)
					{
					case ICONALPHA_NONE:	// �s����

						// �����x��ݒ肷��
						g_aIcon[nCntIcon].col.a = 1.0f;

						break;				// �����o��

					case ICONALPHA_CLEAR:	// ����

						// �����x��ݒ肷��
						g_aIcon[nCntIcon].col.a = 0.0f;

						break;				// �����o��
					}

					break;					// �����o��

				case ICONSTATE_FLYAWAY:		// �l�����ł�����

					// �������ɂ���
					g_aIcon[nCntIcon].col.a = 0.4f;

					break;					// �����o��

				case ICONSTATE_ENLARGE:		// �g����

					if (g_aIcon[nCntIcon].radius.x < BARRIER_ICON_RADIUS)
					{ // ���̔��a�����������ꍇ

						// �A�C�R���̔��a�ɉ��Z����
						g_aIcon[nCntIcon].radius.x += ICON_SCALE_CHANGE;
						g_aIcon[nCntIcon].radius.z += ICON_SCALE_CHANGE;
					}
					else
					{ // ���̔��a�������ꍇ

						// �A�C�R���̔��a�ɉ��Z����
						g_aIcon[nCntIcon].radius.x = BARRIER_ICON_RADIUS;
						g_aIcon[nCntIcon].radius.z = BARRIER_ICON_RADIUS;
					}

					break;					// �����o��

				case ICONSTATE_REDUCE:		// �k�����

					if (g_aIcon[nCntIcon].radius.x >= 0.0f)
					{ // ���̔��a�����������ꍇ

						// �A�C�R���̔��a�ɉ��Z����
						g_aIcon[nCntIcon].radius.x -= ICON_SCALE_CHANGE;
						g_aIcon[nCntIcon].radius.z -= ICON_SCALE_CHANGE;
					}
					else
					{ // ���̔��a�������ꍇ

						// �A�C�R���̔��a�ɉ��Z����
						g_aIcon[nCntIcon].radius.x = 0.0f;
						g_aIcon[nCntIcon].radius.z = 0.0f;
					}

					break;					// �����o��

				case ICONSTATE_EMPHASIS:	// �������

					// �F��ݒ肷��
					g_aIcon[nCntIcon].col = ICON_EMPHASIS_COL;

					break;					// �����o��
				}

				if (g_aIcon[nCntIcon].type != ICONTYPE_EVIL_OBJECT && g_aIcon[nCntIcon].type != ICONTYPE_OBJECT)
				{ // �o���A�E�����n�̃A�C�R���ȊO�̏ꍇ

					if (cameraPos.x + ICON_CORRECT_RIGHT <= g_aIcon[nCntIcon].pos.x + g_aIcon[nCntIcon].radius.x)
					{ // �E�������O���ɃA�C�R��������ꍇ
						// �A�C�R����␳����
						g_aIcon[nCntIcon].pos.x = cameraPos.x + ICON_CORRECT_RIGHT - g_aIcon[nCntIcon].radius.x;
					}

					if (cameraPos.x - ICON_CORRECT_LEFT >= g_aIcon[nCntIcon].pos.x - g_aIcon[nCntIcon].radius.x)
					{ // ���������O���ɃA�C�R��������ꍇ
						// �A�C�R����␳����
						g_aIcon[nCntIcon].pos.x = cameraPos.x - ICON_CORRECT_LEFT + g_aIcon[nCntIcon].radius.x;
					}

					if (cameraPos.z + ICON_CORRECT_FAR <= g_aIcon[nCntIcon].pos.z + g_aIcon[nCntIcon].radius.z)
					{ // ���������O���ɃA�C�R��������ꍇ
						// �A�C�R����␳����
						g_aIcon[nCntIcon].pos.z = cameraPos.z + ICON_CORRECT_FAR - g_aIcon[nCntIcon].radius.z;
					}

					if (cameraPos.z - ICON_CORRECT_NEAR >= g_aIcon[nCntIcon].pos.z - g_aIcon[nCntIcon].radius.z)
					{ // ��O�������O���ɃA�C�R��������ꍇ
						// �A�C�R����␳����
						g_aIcon[nCntIcon].pos.z = cameraPos.z - ICON_CORRECT_NEAR + g_aIcon[nCntIcon].radius.z;
					}
				}
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aIcon[nCntIcon].radius.x, +0.0f, +g_aIcon[nCntIcon].radius.z);
			pVtx[1].pos = D3DXVECTOR3(+g_aIcon[nCntIcon].radius.x, +0.0f, +g_aIcon[nCntIcon].radius.z);
			pVtx[2].pos = D3DXVECTOR3(-g_aIcon[nCntIcon].radius.x, +0.0f, -g_aIcon[nCntIcon].radius.z);
			pVtx[3].pos = D3DXVECTOR3(+g_aIcon[nCntIcon].radius.x, +0.0f, -g_aIcon[nCntIcon].radius.z);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aIcon[nCntIcon].col;
			pVtx[1].col = g_aIcon[nCntIcon].col;
			pVtx[2].col = g_aIcon[nCntIcon].col;
			pVtx[3].col = g_aIcon[nCntIcon].col;
		}

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffIcon->Unlock();
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

	for (int nCntType = 0; nCntType < ICONTYPE_MAX; nCntType++)
	{
		for (int nCntIcon = 0; nCntIcon < MAX_ICON; nCntIcon++)
		{ // �A�C�R���̍ő�\�������J��Ԃ�

			if (g_aIcon[nCntIcon].bUse == true && g_aIcon[nCntIcon].type == nCntType)
			{ // �A�C�R�����g�p����Ă��邩�A�^�C�v����v���Ă����ꍇ

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
				pDevice->SetTexture(0, g_pTextureIcon[g_aIcon[nCntIcon].type]);

				// �|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntIcon * 4, 2);
			}
		}
	}

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			//Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				//Z�e�X�g�̗L��/�����ݒ�

	//���C�e�B���O��ON�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//===============================
//�A�C�R���̐ݒ菈��
//===============================
int SetIcon(D3DXVECTOR3 pos, ICONTYPE type, int *pIconID, bool *pUse, ICONSTATE *pState)
{
	//�A�C�R���̔ԍ�������������
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

			// ���̐ݒ�
			g_aIcon[nCntIcon].pos.x = pos.x;			//�ʒu
			g_aIcon[nCntIcon].pos.z = pos.z;
			g_aIcon[nCntIcon].nCounter = 0;				// �J�E���^�[
			g_aIcon[nCntIcon].alpha = ICONALPHA_NONE;	// �����x�̏��

			// ��ނ�ݒ肷��
			g_aIcon[nCntIcon].type = type;

			// �����̃A�h���X��ݒ肷��
			g_aIcon[nCntIcon].pIconIDParent = pIconID;		// �A�C�R���̃C���f�b�N�X
			g_aIcon[nCntIcon].pUseParent = pUse;			// �e�̎g�p��
			g_aIcon[nCntIcon].pState = pState;				// ���

			switch (g_aIcon[nCntIcon].type)
			{
			case ICONTYPE_PLAY:		// �v���C���[

				// ���a��ݒ�
				g_aIcon[nCntIcon].radius = D3DXVECTOR3(PLAY_ICON_RADIUS, 0.0f, PLAY_ICON_RADIUS);

				// �F��ݒ�
				g_aIcon[nCntIcon].col = PLAY_ICON_COL;

				break;				// �����o��

			case ICONTYPE_EVIL_HUMAN:		// �����l

				// ���a��ݒ�
				g_aIcon[nCntIcon].radius = D3DXVECTOR3(EVIL_ICON_RADIUS, 0.0f, EVIL_ICON_RADIUS);

				// �F��ݒ�
				g_aIcon[nCntIcon].col = EVIL_ICON_COL;

				break;				// �����o��

			case ICONTYPE_EVIL_CAR:		// ������

				// ���a��ݒ�
				g_aIcon[nCntIcon].radius = D3DXVECTOR3(EVIL_ICON_RADIUS, 0.0f, EVIL_ICON_RADIUS);

				// �F��ݒ�
				g_aIcon[nCntIcon].col = EVIL_ICON_COL;

				break;				// �����o��

			case ICONTYPE_POLICE:	// �x�@

				// ���a��ݒ�
				g_aIcon[nCntIcon].radius = D3DXVECTOR3(POLICE_ICON_RADIUS, 0.0f, POLICE_ICON_RADIUS);

				// �F��ݒ�
				g_aIcon[nCntIcon].col = POLICE_ICON_COL;

				break;				// �����o��

			case ICONTYPE_BARRIER:	// �o���A

				// ���a��ݒ�
				g_aIcon[nCntIcon].radius = D3DXVECTOR3(BARRIER_ICON_SET_RADIUS, 0.0f, BARRIER_ICON_SET_RADIUS);

				// �F��ݒ�
				g_aIcon[nCntIcon].col = BARRIER_ICON_COL;

				break;				// �����o��

			case ICONTYPE_GATE_VERT:	// ���ʁE�������Q�[�g

				// ���a��ݒ�
				g_aIcon[nCntIcon].radius = D3DXVECTOR3(GATE_ICON_RADIUS, 0.0f, GATE_ICON_RADIUS);

				// �F��ݒ�
				g_aIcon[nCntIcon].col = GATE_ICON_COL;

				break;				//�����o��

			case ICONTYPE_GATE_HORIZ:	// ���E�����Q�[�g

				// ���a��ݒ�
				g_aIcon[nCntIcon].radius = D3DXVECTOR3(GATE_ICON_RADIUS, 0.0f, GATE_ICON_RADIUS);

				// �F��ݒ�
				g_aIcon[nCntIcon].col = GATE_ICON_COL;

				break;				// �����o��

			case ICONTYPE_ITEM:		// �A�C�e��

				// ���a��ݒ�
				g_aIcon[nCntIcon].radius = D3DXVECTOR3(ITEM_ICON_RADIUS, 0.0f, ITEM_ICON_RADIUS);

				// �F��ݒ�
				g_aIcon[nCntIcon].col = ITEM_ICON_COL;

				break;				// �����o��
			}

			// �F�̃R�s�[��ݒ肷��
			g_aIcon[nCntIcon].colCopy = g_aIcon[nCntIcon].col;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aIcon[nCntIcon].radius.x, +0.0f, +g_aIcon[nCntIcon].radius.z);
			pVtx[1].pos = D3DXVECTOR3(+g_aIcon[nCntIcon].radius.x, +0.0f, +g_aIcon[nCntIcon].radius.z);
			pVtx[2].pos = D3DXVECTOR3(-g_aIcon[nCntIcon].radius.x, +0.0f, -g_aIcon[nCntIcon].radius.z);
			pVtx[3].pos = D3DXVECTOR3(+g_aIcon[nCntIcon].radius.x, +0.0f, -g_aIcon[nCntIcon].radius.z);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aIcon[nCntIcon].col;
			pVtx[1].col = g_aIcon[nCntIcon].col;
			pVtx[2].col = g_aIcon[nCntIcon].col;
			pVtx[3].col = g_aIcon[nCntIcon].col;

			//�g�p����
			g_aIcon[nCntIcon].bUse = true;

			//�A�C�R���̔ԍ����R�s�[����
			nIdxIcon = nCntIcon;

			break;				//�����o��
		}
		pVtx += 4;				//���_�f�[�^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffIcon->Unlock();

	//�A�C�R���̔ԍ���Ԃ�
	return nIdxIcon;
}

//=======================================================================================================
// �I�u�W�F�N�g�̃A�C�R���̐ݒ菈��
//=======================================================================================================
int SetIconObject(D3DXVECTOR3 radius, D3DXVECTOR3 pos, JUDGESTATE type, int *pIconID, bool *pUse, ICONSTATE *pState, ROTSTATE rot)
{
	//�A�C�R���̔ԍ�������������
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

			// ���̐ݒ�
			g_aIcon[nCntIcon].pos.x = pos.x;			// �ʒu(X��)
			g_aIcon[nCntIcon].pos.z = pos.z;			// �ʒu(Z��)
			g_aIcon[nCntIcon].pIconIDParent = pIconID;	// �A�C�R���̃C���f�b�N�X
			g_aIcon[nCntIcon].pUseParent = pUse;		// �e�̎g�p��
			g_aIcon[nCntIcon].pState = pState;			// ���

			g_aIcon[nCntIcon].nCounter = 0;				// �J�E���^�[
			g_aIcon[nCntIcon].alpha = ICONALPHA_NONE;	// �����x�̏��

			// �����Əc�����v�Z
			if (rot == ROTSTATE_0
				|| rot == ROTSTATE_180)
			{ // �p�x��0�x�A�܂���180�x�̏ꍇ

				//���a��ݒ肷��
				g_aIcon[nCntIcon].radius.x = radius.x;
				g_aIcon[nCntIcon].radius.z = radius.z;
			}
			else
			{ // �p�x90�x�A�܂���270�x�̏ꍇ

				//���a��ݒ肷��
				g_aIcon[nCntIcon].radius.x = radius.z;
				g_aIcon[nCntIcon].radius.z = radius.x;
			}

			switch (type)
			{
			case JUDGESTATE_JUSTICE:		// �ǂ��I�u�W�F�N�g�̏ꍇ

				// �A�C�R���̐F��ݒ�
				g_aIcon[nCntIcon].col = OBJECT_ICON_COL;

				// ��ނ�ݒ�
				g_aIcon[nCntIcon].type = ICONTYPE_OBJECT;

				break;

			case JUDGESTATE_EVIL:			// �����I�u�W�F�N�g�̏ꍇ

				// �A�C�R���̐F��ݒ�
				g_aIcon[nCntIcon].col = EVIL_ICON_COL;

				// ��ނ�ݒ�
				g_aIcon[nCntIcon].type = ICONTYPE_EVIL_OBJECT;

				break;
			}

			// �F�̃R�s�[��ݒ肷��
			g_aIcon[nCntIcon].colCopy = g_aIcon[nCntIcon].col;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aIcon[nCntIcon].radius.x, +0.0f, +g_aIcon[nCntIcon].radius.z);
			pVtx[1].pos = D3DXVECTOR3(+g_aIcon[nCntIcon].radius.x, +0.0f, +g_aIcon[nCntIcon].radius.z);
			pVtx[2].pos = D3DXVECTOR3(-g_aIcon[nCntIcon].radius.x, +0.0f, -g_aIcon[nCntIcon].radius.z);
			pVtx[3].pos = D3DXVECTOR3(+g_aIcon[nCntIcon].radius.x, +0.0f, -g_aIcon[nCntIcon].radius.z);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aIcon[nCntIcon].col;
			pVtx[1].col = g_aIcon[nCntIcon].col;
			pVtx[2].col = g_aIcon[nCntIcon].col;
			pVtx[3].col = g_aIcon[nCntIcon].col;

			//�g�p����
			g_aIcon[nCntIcon].bUse = true;

			//�A�C�R���̔ԍ����R�s�[����
			nIdxIcon = nCntIcon;

			break;				//�����o��
		}
		pVtx += 4;				//���_�f�[�^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffIcon->Unlock();

	//�A�C�R���̔ԍ���Ԃ�
	return nIdxIcon;
}

//=======================================================================================================
//	�A�C�R���̈ʒu�̐ݒ菈��
//=======================================================================================================
void SetPositionIcon(int nIconID, D3DXVECTOR3 pos)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	if (nIconID > NONE_ICON)
	{ // �g�p�ł���ID�̏ꍇ

		// �����̃A�C�R���̈ʒu��ݒ�
		g_aIcon[nIconID].pos = pos;
	}
}
