//===========================================
//
// �Č��z�^�C�}�[�̃��C������[buildtimer.cpp]
// Author ��������
//
//===========================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "buildtimer.h"
#include "object.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define BUILDTIMER_TEXTURE		"data/TEXTURE/BuildTimerUI.png"		// �Č��z�^�C�}�[�̃e�N�X�`��
#define BUILDTIMER_RADIUS_X		(200.0f)				// �Č��z�^�C�}�[�̔��a(X��)
#define BUILDTIMER_RADIUS_Y		(80.0f)					// �Č��z�^�C�}�[�̔��a(Y��)

#define MAX_VAL_BUILD			(2)						// �Č��z�^�C�}�[�̕\�����l��
#define MAX_BUILD_MIN			(60)					// �^�C�� (��) �̍ő�l
#define MAX_BUILD_SEC			(60)					// �^�C�� (�b) �̍ő�l

//**********************************************************************************************************************
//	�\���̐錾(Build)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXMATRIX mtx;			// ���[���h�}�g���b�N�X
	Object *pObject;		// �I�u�W�F�N�g�̃|�C���^
	int nCount;				// �J�E���g
	bool bUse;				// �g�p��
}Build;

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBuild = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBuild = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
Build g_aBuild[MAX_BUILDTIMER];					// �Č��z�^�C�}�[�̏��

//==========================================
// �Č��z�^�C�}�[�̏���������
//==========================================
void InitBuildtimer(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//���n�̃e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			BUILDTIMER_TEXTURE,
			&g_pTextureBuild);

	for (int nCntSet = 0; nCntSet < MAX_BUILDTIMER; nCntSet++)
	{
		// ���̏�����
		g_aBuild[nCntSet].pos	 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aBuild[nCntSet].nCount = 0;								// �J�E���^�[
		g_aBuild[nCntSet].pObject = NULL;							// �I�u�W�F�N�g�̃|�C���^
		g_aBuild[nCntSet].bUse	 = false;							// �g�p��
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BUILDTIMER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBuild,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBuild->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BUILDTIMER; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-BUILDTIMER_RADIUS_X, +BUILDTIMER_RADIUS_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+BUILDTIMER_RADIUS_X, +BUILDTIMER_RADIUS_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BUILDTIMER_RADIUS_X, -BUILDTIMER_RADIUS_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+BUILDTIMER_RADIUS_X, -BUILDTIMER_RADIUS_Y, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;						//���_�f�[�^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBuild->Unlock();
}

//========================================
// �Č��z�^�C�}�[�̏I������
//========================================
void UninitBuildtimer(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBuild != NULL)
	{
		g_pTextureBuild->Release();
		g_pTextureBuild = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBuild != NULL)
	{
		g_pVtxBuffBuild->Release();
		g_pVtxBuffBuild = NULL;
	}
}

//========================================
// �Č��z�^�C�}�[�̍X�V����
//========================================
void UpdateBuildtimer(void)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBuild->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBill = 0; nCntBill < MAX_BUILDTIMER; nCntBill++)
	{
		if (g_aBuild[nCntBill].bUse == true)
		{//�g�p����Ă����ꍇ

			// �J�E���g�����Z����
			g_aBuild[nCntBill].nCount--;

			if (g_aBuild[nCntBill].nCount <= 0)
			{ // �J�E���g��0�𒴂����ꍇ

				D3DXMATERIAL mat[MAX_MATERIAL];

				for (int nCnt = 0; nCnt < (int)g_aBuild[nCntBill].pObject->modelData.dwNumMat; nCnt++)
				{
					// �}�e���A�����R�s�[����
					mat[nCnt] = g_aBuild[nCntBill].pObject->matCopy[nCnt];
				}

				// �I�u�W�F�N�g�̐ݒ菈��
				SetObject
				(
					g_aBuild[nCntBill].pObject->pos,				// �ʒu
					g_aBuild[nCntBill].pObject->rot,				// ����
					g_aBuild[nCntBill].pObject->scale,				// �g�嗦
					&mat[0],										// �}�e���A���f�[�^
					g_aBuild[nCntBill].pObject->nType,				// ���
					g_aBuild[nCntBill].pObject->nBreakType,			// �����̎��
					g_aBuild[nCntBill].pObject->nShadowType,		// �e�̎��
					g_aBuild[nCntBill].pObject->nCollisionType,		// �����蔻��̎��
					g_aBuild[nCntBill].pObject->collInfo.stateRot	// �����蔻��̌����̏��
				);

				// �g�p���Ȃ�
				g_aBuild[nCntBill].bUse = false;
			}
		}
		pVtx += 4;					//���_�f�[�^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBuild->Unlock();
}

//=====================================
// �Č��z�^�C�}�[�̕`�揈��
//=====================================
void DrawBuildtimer(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	D3DXMATRIX mtxTrans;			//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;				//�r���[�}�g���b�N�X�擾�p

	//���C�e�B���O��OFF�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);					//�A���t�@�e�X�g�̗L��/�����ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);				//�A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 75);							//�A���t�@�e�X�g�̎Q�ƒl�ݒ�

	for (int nCntDraw = 0; nCntDraw < MAX_BUILDTIMER; nCntDraw++)
	{
		if (g_aBuild[nCntDraw].bUse == true)
		{//�g�p���Ă����ꍇ

			// ���Ԃ�ݒ肷��
			int nMin = g_aBuild[nCntDraw].nCount / 3600;			// �^�C�}�[ (��)
			int nSec = (g_aBuild[nCntDraw].nCount % 3600) / 60;		// �^�C�}�[ (�b)

			// �ϐ��z���錾
			D3DXVECTOR3 aPosValue[MAX_VAL_BUILD] =					// �^�C�}�[�̈ʒu�p
			{ // �����l
				D3DXVECTOR3(g_aBuild[nCntDraw].pos.x - 50.0f, g_aBuild[nCntDraw].pos.y, 0.0f),	// �^�C�}�[ (��) �̈ʒu
				D3DXVECTOR3(g_aBuild[nCntDraw].pos.x + 20.0f, g_aBuild[nCntDraw].pos.y, 0.0f)	// �^�C�}�[ (�b) �̈ʒu
			};

			int aValue[MAX_VAL_BUILD] = { nMin,    nSec };			// �^�C�}�[�\���p
			int aMaxValue[MAX_VAL_BUILD] = { MAX_BUILD_MIN, MAX_BUILD_SEC };	// �^�C�}�[�\���̒l�̐����p

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBuild[nCntDraw].mtx);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����Ɍ����Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aBuild[nCntDraw].mtx, NULL, &mtxView);

			//�t�s������߂�
			g_aBuild[nCntDraw].mtx._41 = 0.0f;
			g_aBuild[nCntDraw].mtx._42 = 0.0f;
			g_aBuild[nCntDraw].mtx._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aBuild[nCntDraw].pos.x, g_aBuild[nCntDraw].pos.y, g_aBuild[nCntDraw].pos.z);
			D3DXMatrixMultiply(&g_aBuild[nCntDraw].mtx, &g_aBuild[nCntDraw].mtx, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBuild[nCntDraw].mtx);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0,
				g_pVtxBuffBuild,									//���_�o�b�t�@�ւ̃|�C���^
				0,
				sizeof(VERTEX_3D));									//���_���\���̂̃T�C�Y

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBuild);

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

	//���C�e�B���O��ON�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//======================================
// �Č��z�^�C�}�[�̐ݒ菈��
//======================================
void SetBuildtimer(D3DXVECTOR3 pos, int nCount, Object *pObject)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBuild->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBill = 0; nCntBill < MAX_BUILDTIMER; nCntBill++)
	{
		if (g_aBuild[nCntBill].bUse == false)
		{//�g�p����Ă��Ȃ������ꍇ
			
			// ���̐ݒ�
			g_aBuild[nCntBill].pos		= pos;			// �ʒu
			g_aBuild[nCntBill].pObject	= pObject;		// �I�u�W�F�N�g�̃A�h���X
			g_aBuild[nCntBill].nCount	= nCount;		// �J�E���g

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-BUILDTIMER_RADIUS_X, +BUILDTIMER_RADIUS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+BUILDTIMER_RADIUS_X, +BUILDTIMER_RADIUS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-BUILDTIMER_RADIUS_X, -BUILDTIMER_RADIUS_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+BUILDTIMER_RADIUS_X, -BUILDTIMER_RADIUS_Y, 0.0f);

			//�g�p����
			g_aBuild[nCntBill].bUse = true;

			break;									//�����o��
		}
		pVtx += 4;									//���_�f�[�^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBuild->Unlock();
}