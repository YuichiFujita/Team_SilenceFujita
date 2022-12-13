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

//�v���g�^�C�v�錾
void TypeChangeEdit(void);								//��ޕύX����
void MoveEdit(float Camerarot);							//�ړ�����
void RotationEdit(void);								//��]����
void SetEdit(void);										//�I�u�W�F�N�g�̐ݒ菈��
void ScaleObjectX(void);								//�I�u�W�F�N�g�̊g��k������(X��)
void ScaleObjectY(void);								//�I�u�W�F�N�g�̊g��k������(Y��)
void ScaleObjectZ(void);								//�I�u�W�F�N�g�̊g��k������(Z��)
void ScaleObject(void);									//�I�u�W�F�N�g�̊g��k������

//�O���[�o���ϐ�
EditObject g_EditObject;								//�I�u�W�F�N�g�̏��
Doll g_DollEditObject[OBJECTTYPE_MAX];					//���f���̊�{���

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

		//�G�f�B�b�g�I�u�W�F�N�g��ݒ肵�Ȃ�
		g_EditObject.bSet = false;

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
}

//========================================
//���f���̍X�V����
//========================================
void UpdateEditObject(void)
{
	Object *pObject = GetObje();					//�I�u�W�F�N�g�̏����擾����

	D3DXVECTOR3 Camerarot = GetCamera().rot;		//�J�����̏����擾����

	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++, pObject++)
	{
		if (pObject->bUse == false && g_EditObject.bSet == false)
		{//�肪�������Ă��Ȃ��I�u�W�F�N�g�������ꍇ
			//�I�u�W�F�N�g��ݒ��Ԃɂ���
			g_EditObject.bSet = true;

			//�I�u�W�F�N�g�̔ԍ���ۑ�����
			g_EditObject.nSetNumber = nCnt;

			//���f������ǂݍ���
			g_EditObject.pDollData = g_DollEditObject[g_EditObject.nType];

			break;							//�����o��
		}
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

	if (g_EditObject.bSet == true)
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

//=======================================
//�G�f�B�b�g�I�u�W�F�N�g�̎擾����
//=======================================
EditObject GetEdit(void)
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
	if (GetKeyboardTrigger(DIK_1) == true)
	{//1�L�[���������ꍇ
		//�ݒ�I�u�W�F�N�g�̎�ނ�I������
		g_EditObject.nType = (g_EditObject.nType + 1) % OBJECTTYPE_MAX;

		//�ݒ肷��
		g_EditObject.pDollData = g_DollEditObject[g_EditObject.nType];
	}
}

//=======================================
//�ړ�����
//=======================================
void MoveEdit(float Camerarot)
{
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
	if (GetKeyboardTrigger(DIK_0) == true)
	{//0�L�[���������ꍇ
		//�I�u�W�F�N�g�̐ݒ菈��
		SetObject(g_EditObject.pos, g_EditObject.rot, g_EditObject.nType, g_EditObject.scale);
	}
}

//=======================================
//�I�u�W�F�N�g�̊g��k������(X��)
//=======================================
void ScaleObjectX(void)
{
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

//=======================================
//�I�u�W�F�N�g�̊g��k������(Y��)
//=======================================
void ScaleObjectY(void)
{
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
