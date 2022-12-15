//===========================================
//
//�I�u�W�F�N�g�̃��C������[Object.cpp]
//Author ��������
//
//===========================================
#include "main.h"
#include "Object.h"
#include "Edit.h"
#include "input.h"
#include "camera.h"
#include "Shadow.h"
#include "billboard.h"

//�v���g�^�C�v�錾
void TypeChangeEdit(void);								//��ޕύX����
void StyleChangeEdit(void);								//�X�^�C���ύX����
void MoveEdit(float Camerarot);							//�ړ�����
void RotationEdit(void);								//��]����
void SetEdit(void);										//�I�u�W�F�N�g�̐ݒ菈��
void ScaleObjectX(void);								//�I�u�W�F�N�g�̊g��k������(X��)
void ScaleObjectY(void);								//�I�u�W�F�N�g�̊g��k������(Y��)
void ScaleObjectZ(void);								//�I�u�W�F�N�g�̊g��k������(Z��)
void ScaleObject(void);									//�I�u�W�F�N�g�̊g��k������
void ResetEdit(void);									//�I�u�W�F�N�g�̏�񃊃Z�b�g����

//�O���[�o���ϐ�
EditObject g_EditObject;								//�I�u�W�F�N�g�̏��
EditBillboard g_EditBillboard;							//�r���{�[�h�̏��
Doll g_DollEditObject[OBJECTTYPE_MAX];					//���f���̊�{���
int g_nStyle;											//�X�^�C���̎��

//X�t�@�C����
const char *c_apModelnameEditObject[OBJECTTYPE_MAX] =
{
	"data/MODEL/Tree.x",								//��
	"data/MODEL/Rock.x",								//��
	"data/MODEL/Forest.x",								//�X
	"data/MODEL/BigTree.x",								//���
	"data/MODEL/YellowTree.x",							//���F����
	"data/MODEL/Grass.x"								//��
};

//�e�N�X�`���t�@�C����
const char *c_apTexturenameEditBillboard[BILLBOARD_MAX] =
{
	"data/TEXTURE/explosion000.png",					//����
	"data/TEXTURE/bullet000.png"						//�e
};

//==========================================
//���f���̏���������
//==========================================
void InitEditObject(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	{//�G�f�B�b�g�I�u�W�F�N�g�̏�����
		//�G�f�B�b�g�I�u�W�F�N�g�̈ʒu������������
		g_EditObject.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�G�f�B�b�g�I�u�W�F�N�g�̌���������������
		g_EditObject.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�G�f�B�b�g�I�u�W�F�N�g�̊g�嗦������������
		g_EditObject.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		//�G�f�B�b�g�I�u�W�F�N�g�̔ԍ�������������
		g_EditObject.nSetNumber = 0;

		//�g�p���Ȃ�
		g_EditObject.bUse = false;

		//�G�f�B�b�g�I�u�W�F�N�g�̎�ނ�����������
		g_EditObject.nType = OBJECTTYPE_TREE;

		for (int nCntEdit = 0; nCntEdit < MAX_TEXTURE; nCntEdit++)
		{
			//�e�N�X�`��������������
			g_EditObject.pDollData.apTexture[nCntEdit] = NULL;
		}

		//�}�e���A���̐���0�ɂ���
		g_EditObject.pDollData.g_dwNumMat = 0;

		//�}�e���A���ւ̃|�C���^��NULL�ɂ���
		g_EditObject.pDollData.g_pBuffMat = NULL;

		//���b�V��(���_���)�ւ̃|�C���^��NULL�ɂ���
		g_EditObject.pDollData.g_pMesh = NULL;
	}

	//�X�^�C����ݒ肷��
	g_nStyle = EDITSTYLE_OBJECT;

	for (int nCntDoll = 0; nCntDoll < OBJECTTYPE_MAX; nCntDoll++)
	{//���f���̏�����
		for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
		{
			//�e�N�X�`��������������
			g_DollEditObject[nCntDoll].apTexture[nCnt] = NULL;
		}

		//�}�e���A���̐�������������
		g_DollEditObject[nCntDoll].g_dwNumMat = 0;

		//�}�e���A���ւ̃|�C���^������������
		g_DollEditObject[nCntDoll].g_pBuffMat = NULL;

		//���b�V��(���_���)�ւ̃|�C���^������������
		g_DollEditObject[nCntDoll].g_pMesh = NULL;
	}

	for (int nCntModel = 0; nCntModel < OBJECTTYPE_MAX; nCntModel++)
	{//���f���̓ǂݍ���
		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(c_apModelnameEditObject[nCntModel],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_DollEditObject[nCntModel].g_pBuffMat,
			NULL,
			&g_DollEditObject[nCntModel].g_dwNumMat,
			&g_DollEditObject[nCntModel].g_pMesh);

		D3DXMATERIAL *pMat;					//�}�e���A���ւ̃|�C���^

		//�}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_DollEditObject[nCntModel].g_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_DollEditObject[nCntModel].g_dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//�e�N�X�`���t�@�C���������݂���
			 //���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_DollEditObject[nCntModel].apTexture[nCntMat]);
			}
		}
	}
}

//========================================
//�r���{�[�h�̏���������
//========================================
void InitEditBillboard(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	{//�G�f�B�b�g�r���{�[�h�̏�����
		//�ʒu������������
		g_EditBillboard.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�ԍ�������������
		g_EditBillboard.nSetNumber = 0;

		//��ނ�����������
		g_EditBillboard.nType = BILLBOARD_EXPL;

		//�g�p���Ȃ�
		g_EditBillboard.bUse = false;

		//��������������
		g_EditBillboard.Radius = D3DXVECTOR2(30.0f, 30.0f);

		//�o�b�t�@��NULL�ɂ���
		g_EditBillboard.VtxBuff = NULL;

		for (int nCntBill = 0; nCntBill < BILLBOARD_MAX; nCntBill++)
		{
			//�e�N�X�`��������������
			g_EditBillboard.Texture[nCntBill] = NULL;
		}
	}

	for (int nCntTexture = 0; nCntTexture < BILLBOARD_MAX; nCntTexture++)
	{//�e�N�X�`���̐ݒ�
		//���n�̃e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apTexturenameEditBillboard[nCntTexture],
			&g_EditBillboard.Texture[nCntTexture]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_EditBillboard.VtxBuff,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, +g_EditBillboard.Radius.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+g_EditBillboard.Radius.x, -g_EditBillboard.Radius.y, 0.0f);

	//�@���x�N�g���̐ݒ�
	pVtx[0].hor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].hor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].hor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].hor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

	//���_�o�b�t�@���A�����b�N����
	g_EditBillboard.VtxBuff->Unlock();
}

//========================================
//���f���̏I������
//========================================
void UninitEditObject(void)
{
	for (int nCntDoll = 0; nCntDoll < OBJECTTYPE_MAX; nCntDoll++)
	{
		for (int nCntMat = 0; nCntMat < MAX_TEXTURE; nCntMat++)
		{
			//�e�N�X�`���̔j��
			if (g_DollEditObject[nCntDoll].apTexture[nCntMat] != NULL)
			{
				g_DollEditObject[nCntDoll].apTexture[nCntMat]->Release();
				g_DollEditObject[nCntDoll].apTexture[nCntMat] = NULL;
			}
		}

		//���b�V���̔j��
		if (g_DollEditObject[nCntDoll].g_pMesh != NULL)
		{
			g_DollEditObject[nCntDoll].g_pMesh->Release();
			g_DollEditObject[nCntDoll].g_pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_DollEditObject[nCntDoll].g_pBuffMat != NULL)
		{
			g_DollEditObject[nCntDoll].g_pBuffMat->Release();
			g_DollEditObject[nCntDoll].g_pBuffMat = NULL;
		}
	}

	for (int nCntBill = 0; nCntBill < BILLBOARD_MAX; nCntBill++)
	{
		//�e�N�X�`���̔j��
		if (g_EditBillboard.Texture[nCntBill] != NULL)
		{
			g_EditBillboard.Texture[nCntBill]->Release();
			g_EditBillboard.Texture[nCntBill] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_EditBillboard.VtxBuff != NULL)
	{
		g_EditBillboard.VtxBuff->Release();
		g_EditBillboard.VtxBuff = NULL;
	}
}

//========================================
//���f���̍X�V����
//========================================
void UpdateEditObject(void)
{
	Object *pObject = GetObje();					//�I�u�W�F�N�g�̏����擾����
	Billboard *pBillboard = GetBillboard();			//�r���{�[�h�̏����擾����

	D3DXVECTOR3 Camerarot = GetCamera().rot;		//�J�����̏����擾����

	//�X�^�C���ύX����
	StyleChangeEdit();

	if (g_nStyle == EDITSTYLE_OBJECT)
	{//�I�u�W�F�N�g�ݒu���[�h�������ꍇ
		for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pObject++)
		{
			if (pObject->bUse == false && g_EditObject.bUse == false)
			{//�肪�������Ă��Ȃ��I�u�W�F�N�g�������ꍇ
				//�I�u�W�F�N�g��ݒ��Ԃɂ���
				g_EditObject.bUse = true;

				//�I�u�W�F�N�g�̔ԍ���ۑ�����
				g_EditObject.nSetNumber = nCntObject;

				//���f������ǂݍ���
				g_EditObject.pDollData = g_DollEditObject[g_EditObject.nType];

				break;							//�����o��
			}
		}

		//�ʒu��ݒ肷��
		g_EditBillboard.pos = g_EditObject.pos;
	}
	else if (g_nStyle == EDITSTYLE_BILLBOARD)
	{//�r���{�[�h�ݒu���[�h�������ꍇ
		for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++, pBillboard++)
		{
			if (pBillboard->bUse == false && g_EditBillboard.bUse == false)
			{//�肪�������Ă��Ȃ��r���{�[�h�������ꍇ
				//�r���{�[�h��ݒ��Ԃɂ���
				g_EditBillboard.bUse = true;

				//�r���{�[�h�̔ԍ���ۑ�����
				g_EditBillboard.nSetNumber = nCntBillboard;

				break;							//�����o��
			}
		}

		//�ʒu��ݒ肷��
		g_EditObject.pos = g_EditBillboard.pos;
	}

	//��ޕύX����
	TypeChangeEdit();

	//�ړ�����
	MoveEdit(Camerarot.y);

	//��]����
	RotationEdit();

	if (g_EditObject.rot.y > D3DX_PI)
	{//3.14f���傫���Ȃ����ꍇ
		//-3.14f�ɕ␳����
		g_EditObject.rot.y = -D3DX_PI;
	}
	else if (g_EditObject.rot.y < -D3DX_PI)
	{//-3.14f��菬�����Ȃ����ꍇ
		//3.14f�ɕ␳����
		g_EditObject.rot.y = D3DX_PI;
	}

	//�I�u�W�F�N�g�̐ݒ菈��
	SetEdit();										

	//�I�u�W�F�N�g�̊g��k������(X��)
	ScaleObjectX();								

	//�I�u�W�F�N�g�̊g��k������(Y��)
	ScaleObjectY();

	//�I�u�W�F�N�g�̊g��k������(Z��)
	ScaleObjectZ();

	//�I�u�W�F�N�g�̊g��k������
	ScaleObject();

	//�I�u�W�F�N�g�̏�񃊃Z�b�g����
	ResetEdit();
}

//=====================================
//���f���̕`�揈��
//=====================================
void DrawEditObject(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;		//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^

	if (g_EditObject.bUse == true)
	{//�ݒ蒆���g�p���Ă���ꍇ
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_EditObject.mtx);

		//�g�嗦�𔽉f
		D3DXMatrixScaling(&mtxScale, g_EditObject.scale.x, g_EditObject.scale.y, g_EditObject.scale.z);
		D3DXMatrixMultiply(&g_EditObject.mtx, &g_EditObject.mtx, &mtxScale);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_EditObject.rot.y, g_EditObject.rot.x, g_EditObject.rot.z);
		D3DXMatrixMultiply(&g_EditObject.mtx, &g_EditObject.mtx, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_EditObject.pos.x, g_EditObject.pos.y, g_EditObject.pos.z);
		D3DXMatrixMultiply(&g_EditObject.mtx, &g_EditObject.mtx, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_EditObject.mtx);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_EditObject.pDollData.g_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_EditObject.pDollData.g_dwNumMat; nCntMat++)
		{
			//�����x��ݒ肷��
			pMat[nCntMat].MatD3D.Ambient.a = 0.5f;
			pMat[nCntMat].MatD3D.Diffuse.a = 0.5f;

			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_EditObject.pDollData.apTexture[nCntMat]);

			//���f��(�p�[�c)�̕`��
			g_EditObject.pDollData.g_pMesh->DrawSubset(nCntMat);
		}
		//�ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
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
}

//=======================================
//�G�f�B�b�g�I�u�W�F�N�g�̎擾����
//=======================================
EditObject GetEditObject(void)
{
	//�G�f�B�b�g�I�u�W�F�N�g�̏���Ԃ�
	return g_EditObject;
}

//=======================================
//�I�u�W�F�N�g�̏���
//=======================================
void DeleteEditObject(Object *pObject)
{
	if (pObject->bUse == true)
	{//�I�u�W�F�N�g���g�p����Ă����ꍇ
		if (g_EditObject.pos.x >= pObject->pos.x - 50.0f &&
			g_EditObject.pos.x <= pObject->pos.x + 50.0f &&
			g_EditObject.pos.z >= pObject->pos.z - 50.0f &&
			g_EditObject.pos.z <= pObject->pos.z + 50.0f)
		{//�͈͓��ɓ������ꍇ
			//�폜�Ώۏ�Ԃɂ���
			pObject->State = OBJECTSTATE_DELETETARGET;

			if (GetKeyboardTrigger(DIK_9) == true)
			{//9�L�[���������ꍇ
				//�g�p���Ă��Ȃ�
				pObject->bUse = false;

				//�e�̃��Z�b�g����
				ResetShadow(&pObject->nShadow);
			}
		}
		else
		{//�͈͊O�������ꍇ
			//�g�p��Ԃɂ���
			pObject->State = OBJECTSTATE_USE;
		}
	}
}

//=======================================
//��ޕύX����
//=======================================
void TypeChangeEdit(void)
{
	if (g_nStyle == EDITSTYLE_OBJECT)
	{//�I�u�W�F�N�g�ݒu���[�h�������ꍇ
		if (GetKeyboardTrigger(DIK_1) == true)
		{//1�L�[���������ꍇ
			//�ݒ�I�u�W�F�N�g�̎�ނ�I������
			g_EditObject.nType = (g_EditObject.nType + 1) % OBJECTTYPE_MAX;

			//�ݒ肷��
			g_EditObject.pDollData = g_DollEditObject[g_EditObject.nType];
		}
	}
	else if (g_nStyle == EDITSTYLE_BILLBOARD)
	{//�r���{�[�h�ݒu���[�h�������ꍇ
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
void MoveEdit(float Camerarot)
{
	if (g_nStyle == EDITSTYLE_OBJECT)
	{//�I�u�W�F�N�g�ݒu���[�h�������ꍇ
		if (GetKeyboardPress(DIK_W) == true)
		{//W�L�[���������ꍇ
		 //�ʒu�����ɐi�߂�
			g_EditObject.pos.x += sinf(Camerarot) * 5.0f;
			g_EditObject.pos.z += cosf(Camerarot) * 5.0f;
		}

		if (GetKeyboardPress(DIK_S) == true)
		{//S�L�[���������ꍇ
		 //�ʒu����O�ɐi�߂�
			g_EditObject.pos.x += -sinf(Camerarot) * 5.0f;
			g_EditObject.pos.z += -cosf(Camerarot) * 5.0f;
		}

		if (GetKeyboardPress(DIK_A) == true)
		{//A�L�[���������ꍇ
		 //�ʒu�����ɐi�߂�
			g_EditObject.pos.x += sinf(-D3DX_PI * 0.5f - Camerarot) * 5.0f;
			g_EditObject.pos.z += -cosf(-D3DX_PI * 0.5f - Camerarot) * 5.0f;
		}

		if (GetKeyboardPress(DIK_D) == true)
		{//D�L�[���������ꍇ
		 //�ʒu���E�ɐi�߂�
			g_EditObject.pos.x += sinf(D3DX_PI * 0.5f - Camerarot) * 5.0f;
			g_EditObject.pos.z += -cosf(D3DX_PI * 0.5f - Camerarot) * 5.0f;
		}
	}

	else if (g_nStyle == EDITSTYLE_BILLBOARD)
	{//�r���{�[�h�ݒu���[�h�������ꍇ
		if (GetKeyboardPress(DIK_W) == true)
		{//W�L�[���������ꍇ
			//�ʒu�����ɐi�߂�
			g_EditBillboard.pos.x += sinf(Camerarot) * 5.0f;
			g_EditBillboard.pos.z += cosf(Camerarot) * 5.0f;
		}

		if (GetKeyboardPress(DIK_S) == true)
		{//S�L�[���������ꍇ
			//�ʒu����O�ɐi�߂�
			g_EditBillboard.pos.x += -sinf(Camerarot) * 5.0f;
			g_EditBillboard.pos.z += -cosf(Camerarot) * 5.0f;
		}

		if (GetKeyboardPress(DIK_A) == true)
		{//A�L�[���������ꍇ
			//�ʒu�����ɐi�߂�
			g_EditBillboard.pos.x += sinf(-D3DX_PI * 0.5f - Camerarot) * 5.0f;
			g_EditBillboard.pos.z += -cosf(-D3DX_PI * 0.5f - Camerarot) * 5.0f;
		}

		if (GetKeyboardPress(DIK_D) == true)
		{//D�L�[���������ꍇ
			//�ʒu���E�ɐi�߂�
			g_EditBillboard.pos.x += sinf(D3DX_PI * 0.5f - Camerarot) * 5.0f;
			g_EditBillboard.pos.z += -cosf(D3DX_PI * 0.5f - Camerarot) * 5.0f;
		}
	}
}

//=======================================
//��]����
//=======================================
void RotationEdit(void)
{
	if (GetKeyboardPress(DIK_Q) == true)
	{//2�L�[���������ꍇ
		//������ς���
		g_EditObject.rot.y += 0.02f;
	}
	else if (GetKeyboardPress(DIK_E) == true)
	{//3�L�[���������ꍇ
		//������ς���
		g_EditObject.rot.y -= 0.02f;
	}
}

//=======================================
//�I�u�W�F�N�g�̐ݒ菈��
//=======================================
void SetEdit(void)
{
	if (g_nStyle == EDITSTYLE_OBJECT)
	{//�I�u�W�F�N�g�ݒu���[�h�������ꍇ
		if (GetKeyboardTrigger(DIK_0) == true)
		{//0�L�[���������ꍇ
			//�I�u�W�F�N�g�̐ݒ菈��
			SetObject(g_EditObject.pos, g_EditObject.rot, g_EditObject.nType, g_EditObject.scale);
		}
	}
	else if (g_nStyle == EDITSTYLE_BILLBOARD)
	{//�r���{�[�h�ݒu���[�h�������ꍇ
		if (GetKeyboardTrigger(DIK_0) == true)
		{//0�L�[���������ꍇ
			//�r���{�[�h�̐ݒ菈��
			SetBillboard(D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_EditBillboard.pos, g_EditBillboard.nType, g_EditBillboard.Radius);
		}
	}
}

//=======================================
//�I�u�W�F�N�g�̊g��k������(X��)
//=======================================
void ScaleObjectX(void)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nStyle == EDITSTYLE_OBJECT)
	{//�I�u�W�F�N�g�ݒu���[�h�������ꍇ
		if (GetKeyboardPress(DIK_U) == true)
		{//U�L�[���������ꍇ
			//X�����g�傷��
			g_EditObject.scale.x += 0.02f;
		}
		else if (GetKeyboardPress(DIK_J) == true)
		{//J�L�[���������ꍇ
			//X�����k������
			g_EditObject.scale.x -= 0.02f;
		}
	}
	if (g_nStyle == EDITSTYLE_BILLBOARD)
	{//�r���{�[�h�ݒu���[�h�������ꍇ
		if (GetKeyboardPress(DIK_U) == true)
		{//U�L�[���������ꍇ
			//X�����g�傷��
			g_EditBillboard.Radius.x += 1.0f;
		}
		else if (GetKeyboardPress(DIK_J) == true)
		{//J�L�[���������ꍇ
			//X�����k������
			g_EditBillboard.Radius.x -= 1.0f;
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
//�I�u�W�F�N�g�̊g��k������(Y��)
//=======================================
void ScaleObjectY(void)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nStyle == EDITSTYLE_OBJECT)
	{//�I�u�W�F�N�g�ݒu���[�h�������ꍇ
		if (GetKeyboardPress(DIK_I) == true)
		{//I�L�[���������ꍇ
			//Y�����g�傷��
			g_EditObject.scale.y += 0.02f;
		}
		else if (GetKeyboardPress(DIK_K) == true)
		{//K�L�[���������ꍇ
			//Y�����k������
			g_EditObject.scale.y -= 0.02f;
		}
	}
	if (g_nStyle == EDITSTYLE_BILLBOARD)
	{//�r���{�[�h�ݒu���[�h�������ꍇ
		if (GetKeyboardPress(DIK_I) == true)
		{//I�L�[���������ꍇ
			//Y�����g�傷��
			g_EditBillboard.Radius.y += 1.0f;
		}
		else if (GetKeyboardPress(DIK_K) == true)
		{//K�L�[���������ꍇ
			//Y�����k������
			g_EditBillboard.Radius.y -= 1.0f;
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
//�I�u�W�F�N�g�̊g��k������(Z��)
//=======================================
void ScaleObjectZ(void)
{
	if (GetKeyboardPress(DIK_O) == true)
	{//O�L�[���������ꍇ
		//Z�����g�傷��
		g_EditObject.scale.z += 0.02f;
	}
	else if (GetKeyboardPress(DIK_L) == true)
	{//L�L�[���������ꍇ
		//Z�����k������
		g_EditObject.scale.z -= 0.02f;
	}
}

//=======================================
//�I�u�W�F�N�g�̊g��k������
//=======================================
void ScaleObject(void)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nStyle == EDITSTYLE_OBJECT)
	{//�I�u�W�F�N�g�ݒu���[�h�������ꍇ
		if (GetKeyboardPress(DIK_4) == true)
		{//4�L�[���������ꍇ
			//�g�傷��
			g_EditObject.scale.x += 0.02f;
			g_EditObject.scale.y += 0.02f;
			g_EditObject.scale.z += 0.02f;
		}
		else if (GetKeyboardPress(DIK_5) == true)
		{//5�L�[���������ꍇ
			//�k������
			g_EditObject.scale.x -= 0.02f;
			g_EditObject.scale.y -= 0.02f;
			g_EditObject.scale.z -= 0.02f;
		}
	}
	else if (g_nStyle == EDITSTYLE_BILLBOARD)
	{//�r���{�[�h�ݒu���[�h�������ꍇ
		if (GetKeyboardPress(DIK_4) == true)
		{//4�L�[���������ꍇ
			//�g�傷��
			g_EditBillboard.Radius.x += 1.0f;
			g_EditBillboard.Radius.y += 1.0f;
		}
		else if (GetKeyboardPress(DIK_5) == true)
		{//5�L�[���������ꍇ
			//�k������
			g_EditBillboard.Radius.x -= 1.0f;
			g_EditBillboard.Radius.y -= 1.0f;
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
//�X�^�C���ύX����
//=======================================
void StyleChangeEdit(void)
{
	if (GetKeyboardTrigger(DIK_6) == true)
	{//6�L�[���������ꍇ
		//�X�^�C����I������
		g_nStyle = (g_nStyle + 1) % EDITSTYLE_MAX;

		if (g_nStyle == EDITSTYLE_OBJECT)
		{//�I�u�W�F�N�g�ݒu���[�h�������ꍇ
			//�I�u�W�F�N�g���g�p����
			g_EditObject.bUse = true;

			//�r���{�[�h���g�p���Ȃ�
			g_EditBillboard.bUse = false;
		}
		else if (g_nStyle == EDITSTYLE_BILLBOARD)
		{//�r���{�[�h�ݒu���[�h�������ꍇ
			//�r���{�[�h���g�p����
			g_EditBillboard.bUse = true;

			//�I�u�W�F�N�g���g�p���Ȃ�
			g_EditObject.bUse = false;
		}
	}
}

//=======================================
//�I�u�W�F�N�g�̏�񃊃Z�b�g����
//=======================================
void ResetEdit(void)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_EditBillboard.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (GetKeyboardTrigger(DIK_2) == true)
	{//2�L�[���������ꍇ
		//�p�x������������
		g_EditObject.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	if (g_nStyle == EDITSTYLE_OBJECT)
	{//�I�u�W�F�N�g�ݒu���[�h�������ꍇ
		if (GetKeyboardTrigger(DIK_3) == true)
		{//3�L�[���������ꍇ
			//�g�嗦������������
			g_EditObject.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		}
	}
	else if (g_nStyle == EDITSTYLE_BILLBOARD)
	{//�r���{�[�h�ݒu���[�h�������ꍇ
		if (GetKeyboardTrigger(DIK_3) == true)
		{//3�L�[���������ꍇ
			//�g�嗦������������
			g_EditBillboard.Radius = D3DXVECTOR2(30.0f, 30.0f);

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
EditBillboard GetEditBillboard(void)
{
	//�G�f�B�b�g�r���{�[�h�̏���Ԃ�
	return g_EditBillboard;
}

//=======================================
//�G�f�B�b�g�X�^�C���̎擾����
//=======================================
int GetStyle(void)
{
	//�G�f�B�b�g�X�^�C����Ԃ�
	return g_nStyle;
}