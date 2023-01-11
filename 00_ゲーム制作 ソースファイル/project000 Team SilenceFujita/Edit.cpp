//===========================================
//
//�I�u�W�F�N�g�̃��C������[Object.cpp]
//Author ��������
//
//===========================================
#include "main.h"
#include "model.h"
#include "Edit.h"
#include "input.h"
#include "camera.h"
#include "object.h"
#include "Shadow.h"
#include "EditBillboard.h"
#include "SoundDJ.h"

//�v���g�^�C�v�錾
void TypeChangeEdit(void);								//��ޕύX����
void MoveEdit(float Camerarot);							//�ړ�����
void RotationEdit(void);								//��]����
void SetEdit(void);										//�I�u�W�F�N�g�̐ݒ菈��
void DeleteEditObject(void);							//�I�u�W�F�N�g�̏���
void ScaleObjectX(void);								//�I�u�W�F�N�g�̊g��k������(X��)
void ScaleObjectY(void);								//�I�u�W�F�N�g�̊g��k������(Y��)
void ScaleObjectZ(void);								//�I�u�W�F�N�g�̊g��k������(Z��)
void ScaleObject(void);									//�I�u�W�F�N�g�̊g��k������
void ResetEdit(void);									//�I�u�W�F�N�g�̏�񃊃Z�b�g����
void EditMaterialCustom(void);							//�}�e���A���̃G�f�B�b�g����

//�O���[�o���ϐ�
EditObject g_EditObject;								//�I�u�W�F�N�g�̏��
EditMaterial g_EditMaterial[MODELTYPE_MAX];				//�}�e���A���̏��
int g_nStyleObject;										//�X�^�C���̕ϐ�
int g_nSoundDJ;											//���ݗ���Ă���T�E���h

////X�t�@�C����
//const char *c_apModelnameEditObject[OBJECTTYPE_MAX] =
//{
//	"data/MODEL/Tree.x",								//��
//	"data/MODEL/Rock.x",								//��
//	"data/MODEL/Forest.x",								//�X
//	"data/MODEL/BigTree.x",								//���
//	"data/MODEL/YellowTree.x",							//���F����
//	"data/MODEL/Grass.x"								//��
//};

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
		g_EditObject.nSetNumber = -1;

		//�g�p���Ȃ�
		g_EditObject.bUse = false;

		//�G�f�B�b�g�I�u�W�F�N�g�̎�ނ�����������
		g_EditObject.nType = MODELTYPE_OBJECT_TREE;

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

		//�I�𒆂̃}�e���A���̔ԍ�������������
		g_EditObject.nCntMaterial = 0;

		//�F��ς���J�E���g������������
		g_EditObject.nColorCount = 0;
	}

	//�X�^�C����ݒ肷��
	g_nStyleObject = EDITSTYLE_OBJECT;

	//�T�E���h������������
	g_nSoundDJ = FUJITA_DJ_LABEL_ONE;

	for (int nCntDoll = 0; nCntDoll < MODELTYPE_OBJECT_MAX; nCntDoll++)
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

	for (int nCntModel = 0; nCntModel < MODELTYPE_OBJECT_MAX; nCntModel++)
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

			//�}�e���A�����R�s�[����
			g_DollEditObject[nCntModel].MatCopy[nCntMat] = pMat[nCntMat];
			g_EditMaterial[nCntModel].EditMaterial[nCntMat] = pMat[nCntMat];
		}
	}
}

//========================================
//���f���̏I������
//========================================
void UninitEditObject(void)
{

}

//========================================
//���f���̍X�V����
//========================================
void UpdateEditObject(void)
{
	Object *pObject = GetObje();					//�I�u�W�F�N�g���擾����
	D3DXVECTOR3 Camerarot = GetCamera().rot;		//�J�����̏����擾����
	g_nStyleObject = GetStyle();					//�X�^�C�����擾����

	if (g_nStyleObject == EDITSTYLE_OBJECT)
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
	}
	else
	{//�I�u�W�F�N�g�ݒu���[�h����Ȃ������ꍇ
		//�g�p���Ȃ�
		g_EditObject.bUse = false;
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

	//�I�u�W�F�N�g�̏���
	DeleteEditObject();

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

	//�}�e���A���̃G�f�B�b�g����
	EditMaterialCustom();

	if (GetKeyboardTrigger(DIK_F5) == true)
	{//F11�L�[���������ꍇ
		//�T�E���h�̒�~
		StopSoundDJ();

		//�T�E���h�𗬂�
		PlaySound(g_nSoundDJ, true);
	}

	if (GetKeyboardTrigger(DIK_F4) == true)
	{//F10�L�[���������ꍇ
		//�T�E���h��ς���
		g_nSoundDJ = (g_nSoundDJ + 1) % SOUND_DJ_LABEL_MAX;
	}
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
	D3DXMATERIAL *pEditMat;						//�G�f�B�b�g�}�e���A���̃f�[�^

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
			pEditMat = &g_EditMaterial[g_EditObject.nType].EditMaterial[nCntMat];

			if (nCntMat == g_EditObject.nCntMaterial)
			{//�I�𒆂̃}�e���A���������ꍇ
				//�����x��ݒ肷��
				pEditMat->MatD3D.Ambient.a = 1.0f;
				pEditMat->MatD3D.Diffuse.a = 1.0f;
			}
			else
			{//�I������Ă��Ȃ��}�e���A���������ꍇ
				//�����x��ݒ肷��
				pEditMat->MatD3D.Ambient.a = 0.5f;
				pEditMat->MatD3D.Diffuse.a = 0.5f;
			}

			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&g_EditMaterial[g_EditObject.nType].EditMaterial[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_EditObject.pModelData.pTexture[nCntMat]);

			//���f��(�p�[�c)�̕`��
			g_EditObject.pModelData.pMesh->DrawSubset(nCntMat);
		}
		//�ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//=======================================
//�G�f�B�b�g�I�u�W�F�N�g�̎擾����
//=======================================
EditObject *GetEditObject(void)
{
	//�G�f�B�b�g�I�u�W�F�N�g�̏���Ԃ�
	return &g_EditObject;
}

//=======================================
//�I�u�W�F�N�g�̏���
//=======================================
void DeleteEditObject(void)
{
	Object *pObject = GetObje();				//�I�u�W�F�N�g�̏����擾����

	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++, pObject++)
	{
		if (pObject->bUse == true)
		{//�I�u�W�F�N�g���g�p����Ă����ꍇ
			if (g_EditObject.pos.x >= pObject->pos.x - 50.0f &&
				g_EditObject.pos.x <= pObject->pos.x + 50.0f &&
				g_EditObject.pos.z >= pObject->pos.z - 50.0f &&
				g_EditObject.pos.z <= pObject->pos.z + 50.0f &&
				g_EditObject.bUse == true)
			{//�͈͓��ɓ������ꍇ
				//�폜�Ώۏ�Ԃɂ���
				pObject->State = OBJECTSTATE_DELETETARGET;

				if (GetKeyboardTrigger(DIK_9) == true)
				{//9�L�[���������ꍇ

					//�g�p���Ă��Ȃ�
					pObject->bUse = false;
				}
			}
			else
			{//�͈͊O�������ꍇ
				//�g�p��Ԃɂ���
				pObject->State = OBJECTSTATE_USE;
			}
		}
	}
}

//=======================================
//��ޕύX����
//=======================================
void TypeChangeEdit(void)
{
	if (g_nStyleObject == EDITSTYLE_OBJECT)
	{//�I�u�W�F�N�g�ݒu���[�h�������ꍇ
		if (GetKeyboardTrigger(DIK_1) == true)
		{//1�L�[���������ꍇ
			//�ݒ�I�u�W�F�N�g�̎�ނ�I������
			g_EditObject.nType = (g_EditObject.nType + 1) % OBJECTTYPE_MAX;

			//�ݒ肷��
			g_EditObject.pDollData = g_DollEditObject[g_EditObject.nType];
		}
	}
}

//=======================================
//�ړ�����
//=======================================
void MoveEdit(float Camerarot)
{
	if (g_nStyleObject == EDITSTYLE_OBJECT)
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
	if (g_nStyleObject == EDITSTYLE_OBJECT)
	{//�I�u�W�F�N�g�ݒu���[�h�������ꍇ
		if (GetKeyboardTrigger(DIK_0) == true)
		{//0�L�[���������ꍇ
			for (int nCount = 0; nCount < MAX_MATERIAL; nCount++)
			{//�}�e���A���̓����x��1.0f�ɂ���
				//�����x��1.0f�ɂ���
				g_EditMaterial[g_EditObject.nType].EditMaterial[nCount].MatD3D.Ambient.a = 1.0f;
				g_EditMaterial[g_EditObject.nType].EditMaterial[nCount].MatD3D.Diffuse.a = 1.0f;
			}

			//�I�u�W�F�N�g�̐ݒ菈��
			SetObject(g_EditObject.pos, g_EditObject.rot, g_EditObject.nType, g_EditObject.scale, &g_EditMaterial[g_EditObject.nType].EditMaterial[0]);

			//�G�f�B�b�g�I�u�W�F�N�g�̔ԍ�������������
			g_EditObject.nSetNumber = -1;
		}
	}
}

//=======================================
//�I�u�W�F�N�g�̊g��k������(X��)
//=======================================
void ScaleObjectX(void)
{
	if (g_nStyleObject == EDITSTYLE_OBJECT)
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
}

//=======================================
//�I�u�W�F�N�g�̊g��k������(Y��)
//=======================================
void ScaleObjectY(void)
{
	if (g_nStyleObject == EDITSTYLE_OBJECT)
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
}

//=======================================
//�I�u�W�F�N�g�̊g��k������(Z��)
//=======================================
void ScaleObjectZ(void)
{
	if (g_nStyleObject == EDITSTYLE_OBJECT)
	{//�I�u�W�F�N�g�ݒu���[�h�������ꍇ
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
}

//=======================================
//�I�u�W�F�N�g�̊g��k������
//=======================================
void ScaleObject(void)
{
	if (g_nStyleObject == EDITSTYLE_OBJECT)
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
}

//=======================================
//�I�u�W�F�N�g�̏�񃊃Z�b�g����
//=======================================
void ResetEdit(void)
{
	if (GetKeyboardTrigger(DIK_2) == true)
	{//2�L�[���������ꍇ
		//�p�x������������
		g_EditObject.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	if (g_nStyleObject == EDITSTYLE_OBJECT)
	{//�I�u�W�F�N�g�ݒu���[�h�������ꍇ
		if (GetKeyboardTrigger(DIK_3) == true)
		{//3�L�[���������ꍇ
			//�g�嗦������������
			g_EditObject.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		}
	}
}

//=======================================
//�}�e���A���̃G�f�B�b�g����
//=======================================
void EditMaterialCustom(void)
{
	D3DMATERIAL9 *pMatEdit;						//�}�e���A���f�[�^�ւ̃|�C���^

	if (g_EditObject.bUse == true)
	{//�ݒ蒆���g�p���Ă���ꍇ
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{//SPACE�L�[���������ꍇ
			//�}�e���A����ύX����
			g_EditObject.nCntMaterial = (g_EditObject.nCntMaterial + 1) % (int)(g_EditObject.pDollData.g_dwNumMat);
		}

		for (int nCntMat = 0; nCntMat < (int)g_EditObject.pDollData.g_dwNumMat; nCntMat++)
		{
			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMatEdit = &g_EditMaterial[g_EditObject.nType].EditMaterial[nCntMat].MatD3D;

			if (nCntMat == g_EditObject.nCntMaterial)
			{//�I�𒆂̃}�e���A���������ꍇ
				if (GetKeyboardPress(DIK_LSHIFT) == true)
				{//���V�t�g�L�[���������ꍇ
					if (GetKeyboardPress(DIK_R) == true)
					{//R�L�[���������ꍇ
						//�J�E���g�����Z����
						g_EditObject.nColorCount++;

						if (g_EditObject.nColorCount % 5 == 0)
						{//20F���Ƃ�
							//R�l�𑝂₷
							pMatEdit->Ambient.r += 0.01f;

							if (pMatEdit->Ambient.r > 1.0f)
							{//R�l�����E�𒴂�����
								//R�l��0�ɖ߂�
								pMatEdit->Ambient.r = 0.0f;
							}

							pMatEdit->Diffuse.r += 0.01f;

							if (pMatEdit->Diffuse.r > 1.0f)
							{//R�l�����E�𒴂�����
								//R�l��0�ɖ߂�
								pMatEdit->Diffuse.r = 0.0f;
							}
						}
					}
					else if (GetKeyboardPress(DIK_F) == true)
					{//F�L�[���������ꍇ
					 //�J�E���g�����Z����
						g_EditObject.nColorCount++;

						if (g_EditObject.nColorCount % 5 == 0)
						{//20F���Ƃ�
							//R�l�𑝂₷
							pMatEdit->Ambient.r -= 0.01f;

							if (pMatEdit->Ambient.r < 0.0f)
							{//R�l�����E�𒴂�����
								//R�l��0�ɖ߂�
								pMatEdit->Ambient.r = 1.0f;
							}

							pMatEdit->Diffuse.r -= 0.01f;

							if (pMatEdit->Diffuse.r < 0.0f)
							{//R�l�����E�𒴂�����
								//R�l��0�ɖ߂�
								pMatEdit->Diffuse.r = 1.0f;
							}
						}
					}

					if (GetKeyboardPress(DIK_T) == true)
					{//T�L�[���������ꍇ
						//�J�E���g�����Z����
						g_EditObject.nColorCount++;

						if (g_EditObject.nColorCount % 5 == 0)
						{//20F���Ƃ�
							//G�l�𑝂₷
							pMatEdit->Ambient.g += 0.01f;

							if (pMatEdit->Ambient.g > 1.0f)
							{//G�l�����E�𒴂�����
								//G�l��0�ɖ߂�
								pMatEdit->Ambient.g = 0.0f;
							}

							//G�l�𑝂₷
							pMatEdit->Diffuse.g += 0.01f;

							if (pMatEdit->Diffuse.g > 1.0f)
							{//G�l�����E�𒴂�����
								//G�l��0�ɖ߂�
								pMatEdit->Diffuse.g = 0.0f;
							}
						}
					}
					else if (GetKeyboardPress(DIK_G) == true)
					{//G�L�[���������ꍇ
						//�J�E���g�����Z����
						g_EditObject.nColorCount++;

						if (g_EditObject.nColorCount % 5 == 0)
						{//20F���Ƃ�
							//R�l�𑝂₷
							pMatEdit->Ambient.g -= 0.01f;

							if (pMatEdit->Ambient.g < 0.0f)
							{//R�l�����E�𒴂�����
								//R�l��0�ɖ߂�
								pMatEdit->Ambient.g = 1.0f;
							}

							//G�l�𑝂₷
							pMatEdit->Diffuse.g -= 0.01f;

							if (pMatEdit->Diffuse.g < 0.0f)
							{//R�l�����E�𒴂�����
								//R�l��0�ɖ߂�
								pMatEdit->Diffuse.g = 1.0f;
							}
						}
					}

					if (GetKeyboardPress(DIK_Y) == true)
					{//Y�L�[���������ꍇ
						//�J�E���g�����Z����
						g_EditObject.nColorCount++;

						if (g_EditObject.nColorCount % 5 == 0)
						{//20F���Ƃ�
							//B�l�𑝂₷
							pMatEdit->Ambient.b += 0.01f;

							if (pMatEdit->Ambient.b > 1.0f)
							{//B�l�����E�𒴂�����
								//B�l��0�ɖ߂�
								pMatEdit->Ambient.b = 0.0f;
							}

							//B�l�𑝂₷
							pMatEdit->Diffuse.b += 0.01f;

							if (pMatEdit->Diffuse.b > 1.0f)
							{//B�l�����E�𒴂�����
								//B�l��0�ɖ߂�
								pMatEdit->Diffuse.b = 0.0f;
							}
						}
					}
					else if (GetKeyboardPress(DIK_H) == true)
					{//H�L�[���������ꍇ
						//�J�E���g�����Z����
						g_EditObject.nColorCount++;

						if (g_EditObject.nColorCount % 5 == 0)
						{//20F���Ƃ�
							//R�l�𑝂₷
							pMatEdit->Ambient.b -= 0.01f;

							if (pMatEdit->Ambient.b < 0.0f)
							{//R�l�����E�𒴂�����
								//R�l��0�ɖ߂�
								pMatEdit->Ambient.b = 1.0f;
							}

							pMatEdit->Diffuse.b -= 0.01f;

							if (pMatEdit->Diffuse.b < 0.0f)
							{//R�l�����E�𒴂�����
								//R�l��0�ɖ߂�
								pMatEdit->Diffuse.b = 1.0f;
							}
						}
					}

					//�A�h���X������
					D3DMATERIAL9 *pMatOriginal = &g_DollEditObject[g_EditObject.nType].MatCopy[nCntMat].MatD3D;

					if (GetKeyboardPress(DIK_V) == true)
					{//V�L�[���������ꍇ
						//R�l�̃��Z�b�g
						pMatEdit->Ambient.r = pMatOriginal->Ambient.r;
						pMatEdit->Diffuse.r = pMatOriginal->Diffuse.r;
					}

					if (GetKeyboardPress(DIK_B) == true)
					{//B�L�[���������ꍇ
						//G�l�̃��Z�b�g
						pMatEdit->Ambient.g = pMatOriginal->Ambient.g;
						pMatEdit->Diffuse.g = pMatOriginal->Diffuse.g;
					}

					if (GetKeyboardPress(DIK_N) == true)
					{//N�L�[���������ꍇ
						//B�l�̃��Z�b�g
						pMatEdit->Ambient.b = pMatOriginal->Ambient.b;
						pMatEdit->Diffuse.b = pMatOriginal->Diffuse.b;
					}
				}
			}
		}
	}
}

//=======================================
//�}�e���A���̏��擾����
//=======================================
EditMaterial GetCustomMaterial(void)
{
	//�}�e���A���̏���Ԃ�
	return g_EditMaterial[g_EditObject.nType];
}