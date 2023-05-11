//===========================================
//
//�I�u�W�F�N�g�̃��C������[EditBillboard.cpp]
//Author ��������
//
//===========================================
#ifdef _DEBUG	// �f�o�b�O����

#include "main.h"
#include "EditBillboard.h"
#include "input.h"
#include "camera.h"
#include "Shadow.h"
#include "billboard.h"
#include "EditObject.h"
#include "Editmain.h"
#include "player.h"

// �}�N����`
#define INIT_RADIUS						(30.0f)		// ���a�̏����l
#define EDITBB_DELETE_RANGE				(30.0f)		// �r���{�[�h�����͈̔�
#define EDITBB_MOVE						(16.0f)		// �ʏ�̈ړ���
#define EDITBB_ADJUSTMENT_MOVE			(4.0f)		// �����p�̈ړ���
#define EDITBB_SCALING					(1.0f)		// �g��k����
#define EDITBB_COL_CHANGE_CNT			(5)			// �F���ς��J�E���g
#define EDITBB_COL_CONVERSION			(0.01f)		// �F�̕ω���
#define EDITBB_UPDOWN_MOVE				(6.0f)		// �㉺�ړ��̈ړ���
#define EDITBB_UPDOWN_ADJUSTMENT_MOVE	(2.0f)		// �����p�̏㉺�ړ��̈ړ���

// �v���g�^�C�v�錾
void TypeChangeEditBillboard(void);		// ��ޕύX����
void MoveEditBillboard(void);			// �ړ�����
void SetEditBillboard(D3DXVECTOR3 rot);	// �r���{�[�h�̐ݒ菈��
void DeleteEditBillboard(void);			// �r���{�[�h�̍폜����
void ScaleBillboardX(void);				// �r���{�[�h�̊g��k������(X��)
void ScaleBillboardY(void);				// �r���{�[�h�̊g��k������(Y��)
void ScaleBillboard(void);				// �r���{�[�h�̊g��k������
void ResetEditBillboard(void);			// �r���{�[�h�̏�񃊃Z�b�g����
void CustomBillboardColor(void);		// �r���{�[�h�̐F�̃G�f�B�b�g����
void CustomBillboardAnim(void);			// �r���{�[�h�̃A�j���[�V�����J�X�^������
void ReplayBillboard(void);				// �r���{�[�h�̃A�j���[�V�����Đ�����
void UpDownEditBillboard(void);			// �r���{�[�h�̏㉺�ړ�����
void CustomShadowBillboard(void);		// �e�̃J�X�^������
void PlayerPosSetEditBillboard(void);	// �r���{�[�h�̃v���C���[�ʒu�ړ�

// �O���[�o���ϐ�
EditBillboard g_EditBillboard;	// �r���{�[�h�̏��
int g_nStyleBillboard;			// �X�^�C���̕ϐ�

// �e�N�X�`���t�@�C����
const char *c_apTexturenameEditBillboard[BILLBOARD_MAX] =
{
	"data/TEXTURE/explosion000.png",	// ����
	"data/TEXTURE/bullet000.png",		// �e
};

//========================================
//�r���{�[�h�̏���������
//========================================
void InitEditBillboard(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	{ // �G�f�B�b�g�r���{�[�h�̏�����

		// �X�^�C��������������
		g_nStyleBillboard = EDITSTYLE_BILLBOARD;

		// ��{����������
		g_EditBillboard.pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu������������
		g_EditBillboard.col		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F������������
		g_EditBillboard.Radius	= D3DXVECTOR2(INIT_RADIUS, INIT_RADIUS);	// ��������������

		g_EditBillboard.nSetNumber	= -1;				// �ԍ�������������
		g_EditBillboard.nType		= BILLBOARD_EXPL;	// ��ނ�����������
		g_EditBillboard.nColorCount = 0;				// �F�ς��J�E���^�[������������
		g_EditBillboard.bUse		= false;			// �g�p���Ȃ�
		g_EditBillboard.bAnimReplay	= false;			// �A�j���[�V�������Đ����Ȃ�
		g_EditBillboard.bShadow		= true;				// �e��t����

		// �A�j���[�V��������������
		g_EditBillboard.EditAnim.bAnim			= false;	// �A�j���[�V�������Ȃ�
		g_EditBillboard.EditAnim.nAnimCounter	= 1;		// �A�j���[�V�����J�E���^�[������������
		g_EditBillboard.EditAnim.nAnimPattern	= 1;		// �A�j���[�V�����p�^�[��������������
		g_EditBillboard.EditAnim.nReplayCount	= 0;		// �Đ��J�E���g������������
		g_EditBillboard.EditAnim.nReplayPattern	= 0;		// �Đ��p�^�[��������������
		g_EditBillboard.EditAnim.TexPattern		= 1.0f;		// �e�N�X�`���p�^�[��������������

		// �|���S������������
		g_EditBillboard.VtxBuff = NULL;	// �o�b�t�@��NULL�ɂ���

		for (int nCntBill = 0; nCntBill < BILLBOARD_MAX; nCntBill++)
		{
			// �e�N�X�`��������������
			g_EditBillboard.Texture[nCntBill] = NULL;

			// ���n�̃e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				c_apTexturenameEditBillboard[nCntBill],
				&g_EditBillboard.Texture[nCntBill]);
		}
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_EditBillboard.VtxBuff,
		NULL);

	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = g_EditBillboard.col;
	pVtx[1].col = g_EditBillboard.col;
	pVtx[2].col = g_EditBillboard.col;
	pVtx[3].col = g_EditBillboard.col;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_EditBillboard.VtxBuff->Unlock();
}

//========================================
//�r���{�[�h�̏I������
//========================================
void UninitEditBillboard(void)
{
	for (int nCntBill = 0; nCntBill < BILLBOARD_MAX; nCntBill++)
	{
		// �e�N�X�`���̔j��
		if (g_EditBillboard.Texture[nCntBill] != NULL)
		{
			g_EditBillboard.Texture[nCntBill]->Release();
			g_EditBillboard.Texture[nCntBill] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_EditBillboard.VtxBuff != NULL)
	{
		g_EditBillboard.VtxBuff->Release();
		g_EditBillboard.VtxBuff = NULL;
	}
}

//========================================
//�r���{�[�h�̍X�V����
//========================================
void UpdateEditBillboard(void)
{
	Billboard *pBillboard = GetBillboard();			//�r���{�[�h�̏����擾����
	Camera *pCamera = GetCamera(CAMERATYPE_MAIN);	//�J�����̏����擾����

	g_nStyleBillboard = GetStyle();					//�X�^�C�����擾����

	if (g_nStyleBillboard == EDITSTYLE_BILLBOARD)
	{//�r���{�[�h�ݒu���[�h�������ꍇ
		for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++, pBillboard++)
		{
			if (pBillboard->bUse == false && g_EditBillboard.bUse == false)
			{//�肪�������Ă��Ȃ��r���{�[�h�������ꍇ
				//�r���{�[�h��ݒ��Ԃɂ���
				g_EditBillboard.bUse = true;

				//�ԍ�������������
				g_EditBillboard.nSetNumber = nCntBillboard;

				break;							//�����o��
			}
		}
	}
	else
	{//�r���{�[�h�ݒu���[�h����Ȃ������ꍇ
		g_EditBillboard.bUse = false;
	}

	//��ޕύX����
	TypeChangeEditBillboard();

	//�ړ�����
	MoveEditBillboard();

	//�r���{�[�h�̍폜����
	DeleteEditBillboard();

	//�r���{�[�h�̐ݒ菈��
	SetEditBillboard(pCamera->rot);

	//�r���{�[�h�̊g��k������(X��)
	ScaleBillboardX();

	//�r���{�[�h�̊g��k������(Y��)
	ScaleBillboardY();

	//�r���{�[�h�̊g��k������
	ScaleBillboard();

	//�r���{�[�h�̐F�̃G�f�B�b�g����
	CustomBillboardColor();

	//�r���{�[�h�̃A�j���[�V�����J�X�^������
	CustomBillboardAnim();

	//�r���{�[�h�̃A�j���[�V�����Đ�����
	ReplayBillboard();

	//�r���{�[�h�̏�񃊃Z�b�g����
	ResetEditBillboard();

	//�e�̃J�X�^������
	CustomShadowBillboard();

	//�r���{�[�h�̏㉺�ړ�����
	UpDownEditBillboard();

	//�r���{�[�h�̃v���C���[�ʒu�ړ�
	PlayerPosSetEditBillboard();
}

//=====================================
//�r���{�[�h�̕`�揈��
//=====================================
void DrawEditBillboard(void)
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

	if (g_EditBillboard.bUse == true)
	{//�g�p���Ă����ꍇ
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_EditBillboard.mtx);

		//�r���[�}�g���b�N�X���擾
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//�|���S�����J�����Ɍ����Đ��ʂɌ�����
		D3DXMatrixInverse(&g_EditBillboard.mtx, NULL, &mtxView);

		//�t�s������߂�
		g_EditBillboard.mtx._41 = 0.0f;
		g_EditBillboard.mtx._42 = 0.0f;
		g_EditBillboard.mtx._43 = 0.0f;

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_EditBillboard.pos.x, g_EditBillboard.pos.y, g_EditBillboard.pos.z);
		D3DXMatrixMultiply(&g_EditBillboard.mtx, &g_EditBillboard.mtx, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_EditBillboard.mtx);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0,
			g_EditBillboard.VtxBuff,							//���_�o�b�t�@�ւ̃|�C���^
			0,
			sizeof(VERTEX_3D));									//���_���\���̂̃T�C�Y

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_EditBillboard.Texture[g_EditBillboard.nType]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//�v���~�e�B�u�̎��
			0,													//�`�悷��ŏ��̒��_�C���f�b�N�X
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

//=======================================
//�r���{�[�h�̏���
//=======================================
void DeleteEditBillboard(void)
{
	Billboard *Billboard = GetBillboard();		//�r���{�[�h�̏����擾����

	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++, Billboard++)
	{
		if (Billboard->bUse == true)
		{//�I�u�W�F�N�g���g�p����Ă����ꍇ
			if (g_EditBillboard.pos.x >= Billboard->pos.x - EDITBB_DELETE_RANGE &&
				g_EditBillboard.pos.x <= Billboard->pos.x + EDITBB_DELETE_RANGE &&
				g_EditBillboard.pos.z >= Billboard->pos.z - EDITBB_DELETE_RANGE &&
				g_EditBillboard.pos.z <= Billboard->pos.z + EDITBB_DELETE_RANGE &&
				g_EditBillboard.bUse == true)
			{//�͈͓��ɓ������ꍇ
				//�폜�Ώۏ�Ԃɂ���
				Billboard->State = BILLBOARDSTATE_DELETETARGET;

				if (GetKeyboardTrigger(DIK_9) == true)
				{//9�L�[���������ꍇ

					//�g�p���Ă��Ȃ�
					Billboard->bUse = false;
				}
			}
			else
			{//�͈͊O�������ꍇ
				//�g�p��Ԃɂ���
				Billboard->State = BILLBOARDSTATE_USE;
			}
		}
	}
}

//=======================================
//��ޕύX����
//=======================================
void TypeChangeEditBillboard(void)
{
	if (g_nStyleBillboard == EDITSTYLE_BILLBOARD)
	{//�r���{�[�h�G�f�B�b�g���[�h�������ꍇ
		if (GetKeyboardTrigger(DIK_1) == true)
		{//1�L�[���������ꍇ
			//�ݒ�I�u�W�F�N�g�̎�ނ�I������
			g_EditBillboard.nType = (g_EditBillboard.nType + 1) % BILLBOARD_MAX;
		}
	}
}

//=======================================
//�ړ�����
//=======================================
void MoveEditBillboard(void)
{
	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{//���V�t�g�L�[��������Ă����ꍇ
		//�����𔲂���
		return;
	}

	if (GetKeyboardPress(DIK_LCONTROL) == true)
	{//���R���g���[���L�[�������Ă����ꍇ
		if (g_nStyleBillboard == EDITSTYLE_BILLBOARD)
		{//�r���{�[�h�ݒu���[�h�������ꍇ
			if (GetKeyboardTrigger(DIK_W) == true)
			{//W�L�[���������ꍇ
				//�ʒu�����ɐi�߂�
				g_EditBillboard.pos.z += EDITBB_ADJUSTMENT_MOVE;
			}

			if (GetKeyboardTrigger(DIK_S) == true)
			{//S�L�[���������ꍇ
				//�ʒu����O�ɐi�߂�
				g_EditBillboard.pos.z -= EDITBB_ADJUSTMENT_MOVE;
			}

			if (GetKeyboardTrigger(DIK_A) == true)
			{//A�L�[���������ꍇ
				//�ʒu�����ɐi�߂�
				g_EditBillboard.pos.x -= EDITBB_ADJUSTMENT_MOVE;
			}

			if (GetKeyboardTrigger(DIK_D) == true)
			{//D�L�[���������ꍇ
				//�ʒu���E�ɐi�߂�
				g_EditBillboard.pos.x += EDITBB_ADJUSTMENT_MOVE;
			}
		}
	}
	else
	{//���R���g���[���L�[�������Ă��Ȃ��ꍇ
		if (g_nStyleBillboard == EDITSTYLE_BILLBOARD)
		{//�r���{�[�h�ݒu���[�h�������ꍇ
			if (GetKeyboardPress(DIK_W) == true)
			{//W�L�[���������ꍇ
				//�ʒu�����ɐi�߂�
				g_EditBillboard.pos.z += EDITBB_MOVE;
			}

			if (GetKeyboardPress(DIK_S) == true)
			{//S�L�[���������ꍇ
				//�ʒu����O�ɐi�߂�
				g_EditBillboard.pos.z -= EDITBB_MOVE;
			}

			if (GetKeyboardPress(DIK_A) == true)
			{//A�L�[���������ꍇ
				//�ʒu�����ɐi�߂�
				g_EditBillboard.pos.x -= EDITBB_MOVE;
			}

			if (GetKeyboardPress(DIK_D) == true)
			{//D�L�[���������ꍇ
				//�ʒu���E�ɐi�߂�
				g_EditBillboard.pos.x += EDITBB_MOVE;
			}
		}
	}
}

//=======================================
//�I�u�W�F�N�g�̐ݒ菈��
//=======================================
void SetEditBillboard(D3DXVECTOR3 rot)
{
	if (g_nStyleBillboard == EDITSTYLE_BILLBOARD)
	{//�r���{�[�h�ݒu���[�h�������ꍇ
		if (GetKeyboardTrigger(DIK_0) == true)
		{//0�L�[���������ꍇ
			//�r���{�[�h�̐ݒ菈��
			SetBillboard(rot, g_EditBillboard.pos, g_EditBillboard.nType, g_EditBillboard.Radius, g_EditBillboard.col
				, g_EditBillboard.EditAnim.nAnimCounter, g_EditBillboard.EditAnim.nAnimPattern, g_EditBillboard.EditAnim.bAnim, g_EditBillboard.bShadow);

			//�ԍ�������������
			g_EditBillboard.nSetNumber = -1;
		}
	}
}

//=======================================
//�r���{�[�h�̊g��k������(X��)
//=======================================
void ScaleBillboardX(void)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nStyleBillboard == EDITSTYLE_BILLBOARD)
	{//�r���{�[�h�ݒu���[�h�������ꍇ
		if (GetKeyboardPress(DIK_U) == true)
		{//U�L�[���������ꍇ
			//X�����g�傷��
			g_EditBillboard.Radius.x += EDITBB_SCALING;
		}
		else if (GetKeyboardPress(DIK_J) == true)
		{//J�L�[���������ꍇ
			//X�����k������
			g_EditBillboard.Radius.x -= EDITBB_SCALING;
		}

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_EditBillboard.VtxBuff->Unlock();
}

//=======================================
//�r���{�[�h�̊g��k������(Y��)
//=======================================
void ScaleBillboardY(void)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nStyleBillboard == EDITSTYLE_BILLBOARD)
	{//�r���{�[�h�ݒu���[�h�������ꍇ
		if (GetKeyboardPress(DIK_I) == true)
		{//I�L�[���������ꍇ
			//Y�����g�傷��
			g_EditBillboard.Radius.y += EDITBB_SCALING;
		}
		else if (GetKeyboardPress(DIK_K) == true)
		{//K�L�[���������ꍇ
			//Y�����k������
			g_EditBillboard.Radius.y -= EDITBB_SCALING;
		}

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_EditBillboard.VtxBuff->Unlock();
}

//=======================================
//�r���{�[�h�̊g��k������
//=======================================
void ScaleBillboard(void)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nStyleBillboard == EDITSTYLE_BILLBOARD)
	{//�r���{�[�h�ݒu���[�h�������ꍇ
		if (GetKeyboardPress(DIK_4) == true)
		{//4�L�[���������ꍇ
			//�g�傷��
			g_EditBillboard.Radius.x += EDITBB_SCALING;
			g_EditBillboard.Radius.y += EDITBB_SCALING;
		}
		else if (GetKeyboardPress(DIK_5) == true)
		{//5�L�[���������ꍇ
			//�k������
			g_EditBillboard.Radius.x -= EDITBB_SCALING;
			g_EditBillboard.Radius.y -= EDITBB_SCALING;
		}

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_EditBillboard.VtxBuff->Unlock();
}

//=======================================
//�I�u�W�F�N�g�̏�񃊃Z�b�g����
//=======================================
void ResetEditBillboard(void)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nStyleBillboard == EDITSTYLE_BILLBOARD)
	{//�r���{�[�h�ݒu���[�h�������ꍇ
		if (GetKeyboardTrigger(DIK_3) == true)
		{//3�L�[���������ꍇ
			//�g�嗦������������
			g_EditBillboard.Radius = D3DXVECTOR2(INIT_RADIUS, INIT_RADIUS);

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_EditBillboard.VtxBuff->Unlock();
}

//=======================================
//�G�f�B�b�g�r���{�[�h�̎擾����
//=======================================
EditBillboard *GetEditBillboard(void)
{
	//�G�f�B�b�g�r���{�[�h�̏���Ԃ�
	return &g_EditBillboard;
}

//=======================================
//�r���{�[�h�̐F�̃G�f�B�b�g����
//=======================================
void CustomBillboardColor(void)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nStyleBillboard == EDITSTYLE_BILLBOARD)
	{//�r���{�[�h�ݒu��Ԃ������ꍇ
		if (GetKeyboardPress(DIK_LSHIFT) == true)
		{//���V�t�g�L�[���������ꍇ
			if (GetKeyboardPress(DIK_R) == true)
			{//R�L�[���������ꍇ
				//�F�J�E���g�����Z����
				g_EditBillboard.nColorCount++;

				if (g_EditBillboard.nColorCount % EDITBB_COL_CHANGE_CNT == 0)
				{//��莞�Ԍo�߂�����
					//�ԐF����������
					g_EditBillboard.col.r += EDITBB_COL_CONVERSION;

					if (g_EditBillboard.col.r > 1.0f)
					{//�ԐF��1.0f�𒴂�����
						//�ԐF��0.0f�ɂ���
						g_EditBillboard.col.r = 0.0f;
					}
				}
			}
			else if (GetKeyboardPress(DIK_F) == true)
			{//F�L�[���������ꍇ
				//�F�J�E���g�����Z����
				g_EditBillboard.nColorCount++;

				if (g_EditBillboard.nColorCount % EDITBB_COL_CHANGE_CNT == 0)
				{//��莞�Ԍo�߂�����
					//�ԐF����������
					g_EditBillboard.col.r -= EDITBB_COL_CONVERSION;

					if (g_EditBillboard.col.r < 0.0f)
					{//�ԐF��0.0f�𒴂�����
						//�ԐF��1.0f�ɂ���
						g_EditBillboard.col.r = 1.0f;
					}
				}
			}

			if (GetKeyboardPress(DIK_T) == true)
			{//T�L�[���������ꍇ
				//�F�J�E���g�����Z����
				g_EditBillboard.nColorCount++;

				if (g_EditBillboard.nColorCount % EDITBB_COL_CHANGE_CNT == 0)
				{//��莞�Ԍo�߂�����
					//�ΐF����������
					g_EditBillboard.col.g += EDITBB_COL_CONVERSION;

					if (g_EditBillboard.col.g > 1.0f)
					{//�ΐF��1.0f�𒴂�����
						//�ΐF��0.0f�ɂ���
						g_EditBillboard.col.g = 0.0f;
					}
				}
			}
			else if (GetKeyboardPress(DIK_G) == true)
			{//G�L�[���������ꍇ
				//�F�J�E���g�����Z����
				g_EditBillboard.nColorCount++;

				if (g_EditBillboard.nColorCount % EDITBB_COL_CHANGE_CNT == 0)
				{//��莞�Ԍo�߂�����
					//�ΐF����������
					g_EditBillboard.col.g -= EDITBB_COL_CONVERSION;

					if (g_EditBillboard.col.g < 0.0f)
					{//�ΐF��0.0f�𒴂�����
						//�ΐF��1.0f�ɂ���
						g_EditBillboard.col.g = 1.0f;
					}
				}
			}

			if (GetKeyboardPress(DIK_Y) == true)
			{//Y�L�[���������ꍇ
				//�F�J�E���g�����Z����
				g_EditBillboard.nColorCount++;

				if (g_EditBillboard.nColorCount % EDITBB_COL_CHANGE_CNT == 0)
				{//��莞�Ԍo�߂�����
					//�F����������
					g_EditBillboard.col.b += EDITBB_COL_CONVERSION;

					if (g_EditBillboard.col.b > 1.0f)
					{//�F��1.0f�𒴂�����
						//�F��0.0f�ɂ���
						g_EditBillboard.col.b = 0.0f;
					}
				}
			}
			else if (GetKeyboardPress(DIK_H) == true)
			{//G�L�[���������ꍇ
				//�F�J�E���g�����Z����
				g_EditBillboard.nColorCount++;

				if (g_EditBillboard.nColorCount % EDITBB_COL_CHANGE_CNT == 0)
				{//��莞�Ԍo�߂�����
					//�F����������
					g_EditBillboard.col.b -= EDITBB_COL_CONVERSION;

					if (g_EditBillboard.col.b < 0.0f)
					{//�F��0.0f�𒴂�����
						//�F��1.0f�ɂ���
						g_EditBillboard.col.b = 1.0f;
					}
				}
			}

			if (GetKeyboardTrigger(DIK_V) == true)
			{//V�L�[���������ꍇ
				//�ԐF��1.0f�ɂ���
				g_EditBillboard.col.r = 1.0f;
			}

			if (GetKeyboardTrigger(DIK_B) == true)
			{//B�L�[���������ꍇ
				//�ΐF��1.0f�ɂ���
				g_EditBillboard.col.g = 1.0f;
			}

			if (GetKeyboardTrigger(DIK_N) == true)
			{//N�L�[���������ꍇ
				//�F��1.0f�ɂ���
				g_EditBillboard.col.b = 1.0f;
			}
		}

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_EditBillboard.col;
		pVtx[1].col = g_EditBillboard.col;
		pVtx[2].col = g_EditBillboard.col;
		pVtx[3].col = g_EditBillboard.col;
	}

	//���_�o�b�t�@���A�����b�N����
	g_EditBillboard.VtxBuff->Unlock();
}

//=======================================
//�r���{�[�h�̃A�j���[�V�����J�X�^������
//=======================================
void CustomBillboardAnim(void)
{
	if (GetKeyboardTrigger(DIK_7) == true)
	{//7�L�[���������ꍇ
		//�A�j���[�V�����̗L����؂�ւ���
		g_EditBillboard.EditAnim.bAnim = g_EditBillboard.EditAnim.bAnim ? false : true;
	}

	if (GetKeyboardTrigger(DIK_8) == true)
	{//8�L�[���������ꍇ
		//�A�j���[�V�����̍Đ���؂�ւ���
		g_EditBillboard.bAnimReplay = g_EditBillboard.bAnimReplay ? false : true;
	}

	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{//��SHIFT�L�[�������Ă����ꍇ
		if (GetKeyboardTrigger(DIK_UP) == true)
		{//��L�[���������ꍇ
			//�p�^�[�������Z����
			g_EditBillboard.EditAnim.nAnimPattern++;
		}
		else if (GetKeyboardTrigger(DIK_DOWN) == true)
		{//���L�[���������ꍇ
			//�p�^�[�������Z����
			g_EditBillboard.EditAnim.nAnimPattern--;
		}

		if (g_EditBillboard.EditAnim.nAnimPattern < 1)
		{//�A�j���[�V�����p�^�[����1�����ɂȂ����ꍇ
			//�A�j���[�V�����p�^�[����1�ɂ���
			g_EditBillboard.EditAnim.nAnimPattern = 1;
		}

		if (GetKeyboardTrigger(DIK_RIGHT) == true)
		{//�E�L�[���������ꍇ
			//�A�j���J�E���^�[�����Z����
			g_EditBillboard.EditAnim.nAnimCounter++;
		}
		else if (GetKeyboardTrigger(DIK_LEFT) == true)
		{//�E�L�[���������ꍇ
			//�A�j���J�E���^�[�����Z����
			g_EditBillboard.EditAnim.nAnimCounter--;
		}

		if (g_EditBillboard.EditAnim.nAnimCounter < 1)
		{//�J�E���^�[��0�����ɂȂ����ꍇ
			//�J�E���^�[��0�ɂ���
			g_EditBillboard.EditAnim.nAnimCounter = 1;
		}
	}
}

//=======================================
//�r���{�[�h�̃A�j���[�V�����Đ�����
//=======================================
void ReplayBillboard(void)
{
	//�p�^�[������ݒ肷��
	g_EditBillboard.EditAnim.TexPattern = (float)(1.0f / g_EditBillboard.EditAnim.nAnimPattern);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (g_EditBillboard.EditAnim.bAnim == true)
	{//�A�j���[�V��������ꍇ
		if (g_EditBillboard.bAnimReplay == true)
		{//�Đ���ON�ɂȂ��Ă���ꍇ
			//�Đ��J�E���^�[�����Z����
			g_EditBillboard.EditAnim.nReplayCount++;

			if (g_EditBillboard.EditAnim.nReplayCount % g_EditBillboard.EditAnim.nAnimCounter == 0)
			{//�Đ��J�E���g���ݒ肳��Ă����J�E���g�ɓ͂����ꍇ
				//�p�^�[�������Z����
				g_EditBillboard.EditAnim.nReplayPattern = (g_EditBillboard.EditAnim.nReplayPattern + 1) % g_EditBillboard.EditAnim.nAnimPattern;
			}
		}
		else
		{//�Đ���OFF�ɂȂ��Ă���
			//�Đ��J�E���g�����Z�b�g����
			g_EditBillboard.EditAnim.nReplayCount = 0;

			//�Đ��p�^�[�������Z�b�g����
			g_EditBillboard.EditAnim.nReplayPattern = 0;
		}

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(g_EditBillboard.EditAnim.TexPattern * g_EditBillboard.EditAnim.nReplayPattern, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_EditBillboard.EditAnim.TexPattern * g_EditBillboard.EditAnim.nReplayPattern + (g_EditBillboard.EditAnim.TexPattern), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_EditBillboard.EditAnim.TexPattern * g_EditBillboard.EditAnim.nReplayPattern, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_EditBillboard.EditAnim.TexPattern * g_EditBillboard.EditAnim.nReplayPattern + (g_EditBillboard.EditAnim.TexPattern), 1.0f);
	}
	else
	{//�A�j���[�V�������Ă��Ȃ��ꍇ
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_EditBillboard.VtxBuff->Unlock();
}

//=======================================
//�r���{�[�h�̏㉺�ړ�����
//=======================================
void UpDownEditBillboard(void)
{
	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{//��SHIFT�L�[�������Ă���ꍇ
		if (GetKeyboardPress(DIK_LCONTROL) == true)
		{//���R���g���[���L�[�������Ă����ꍇ
			if (GetKeyboardTrigger(DIK_W) == true)
			{//W�L�[�������Ă���ꍇ
				//�ʒu�����ɐi�߂�
				g_EditBillboard.pos.y += EDITBB_UPDOWN_ADJUSTMENT_MOVE;
			}

			if (GetKeyboardTrigger(DIK_S) == true)
			{//S�L�[�������Ă���ꍇ
				//�ʒu����O�ɐi�߂�
				g_EditBillboard.pos.y -= EDITBB_UPDOWN_ADJUSTMENT_MOVE;
			}
		}
		else
		{//���R���g���[���L�[�������Ă��Ȃ��ꍇ
			if (GetKeyboardPress(DIK_W) == true)
			{//W�L�[�������Ă���ꍇ
				//�ʒu�����ɐi�߂�
				g_EditBillboard.pos.y += EDITBB_UPDOWN_MOVE;
			}

			if (GetKeyboardPress(DIK_S) == true)
			{//S�L�[�������Ă���ꍇ
				//�ʒu����O�ɐi�߂�
				g_EditBillboard.pos.y -= EDITBB_UPDOWN_MOVE;
			}
		}
	}

	if (GetKeyboardTrigger(DIK_A) == true || GetKeyboardTrigger(DIK_D) == true)
	{//A�L�[��D�L�[���������ꍇ
		//�n�ʂɖ߂�
		g_EditBillboard.pos.y = 0.0f;
	}
}

//=======================================
//�e�̃J�X�^������
//=======================================
void CustomShadowBillboard(void)
{
	if (GetKeyboardTrigger(DIK_2) == true)
	{//2�L�[���������ꍇ
		//�e��؂�ւ���
		g_EditBillboard.bShadow = g_EditBillboard.bShadow ? false : true;
	}
}

//=======================================
//�r���{�[�h�̃v���C���[�ʒu�ړ�
//=======================================
void PlayerPosSetEditBillboard(void)
{
	// �|�C���^��錾
	Player *pPlayer = GetPlayer();	// �v���C���[�̏��

	if (GetKeyboardTrigger(DIK_LMENU) == true)
	{ // ���� ALT�L�[�������ꂽ�ꍇ

		// �v���C���[�̈ʒu�Ɉʒu���ړ�
		g_EditBillboard.pos = pPlayer->pos;
	}
}

#endif