#ifdef _DEBUG	// �f�o�b�O����
//===========================================
//
//�I�u�W�F�N�g�̃��C������[EditObject.cpp]
//Author ��������
//
//===========================================
#include "main.h"
#include "model.h"
#include "EditObject.h"
#include "EditCollision.h"
#include "Editmain.h"
#include "input.h"
#include "camera.h"
#include "object.h"
#include "Shadow.h"
#include "EditBillboard.h"
#include "player.h"

//�}�N����`
#define EDITOBJ_SELECT_MATERIAL_ALPHA	(1.0f)		// �I�𒆂̃}�e���A���̓����x
#define EDITOBJ_NORMAL_MATERIAL_ALPHA	(0.5f)		// �I�����Ă��Ȃ��}�e���A���̓����x
#define EDITOBJ_DELETE_OBJECT_RANGE		(70.0f)		// �I�u�W�F�N�g�����͈̔�
#define EDITOBJ_ADJUSTMENT_MOVE			(4.0f)		// �����p�̈ړ���
#define EDITOBJ_MOVE					(16.0f)		// �ʏ�̈ړ���
#define EDITOBJ_ROT_MOVE				(0.02f)		// �����̈ړ���
#define EDITOBJ_SCALING					(0.02f)		// �g��k����
#define EDITOBJ_COL_CHANGE_CNT			(5)			// �F���ς��J�E���g
#define EDITOBJ_COL_CONVERSION			(0.01f)		// �}�e���A���̕ω���
#define EDITOBJ_UPDOWN_MOVE				(6.0f)		// �㉺�ړ��̈ړ���
#define EDITOBJ_UPDOWN_ADJUSTMENT_MOVE	(2.0f)		// �����p�̏㉺�ړ��̈ړ���
#define EDITOBJ_ADJUSTMENT_ROT_MOVE		(15)		// �����p�̌����̈ړ���

//�v���g�^�C�v�錾
void SaveCurrentEdit(void);			//�G�f�B�b�g�󋵂̈ꎞ�ۑ�����
void TypeChangeEdit(void);			//��ޕύX����
void MoveEdit(void);				//�ړ�����
void RotationEdit(void);			//��]����
void SetEdit(void);					//�I�u�W�F�N�g�̐ݒ菈��
void DeleteEditObject(void);		//�I�u�W�F�N�g�̏���
void ScaleObjectX(void);			//�I�u�W�F�N�g�̊g��k������(X��)
void ScaleObjectY(void);			//�I�u�W�F�N�g�̊g��k������(Y��)
void ScaleObjectZ(void);			//�I�u�W�F�N�g�̊g��k������(Z��)
void ScaleObject(void);				//�I�u�W�F�N�g�̊g��k������
void ResetEdit(void);				//�I�u�W�F�N�g�̏�񃊃Z�b�g����
void EditMaterialCustom(void);		//�}�e���A���̃G�f�B�b�g����
void BreakEdit(void);				//�I�u�W�F�N�g�̔j��G�f�B�b�g����
void ShadowEdit(void);				//�I�u�W�F�N�g�̉e�̃G�f�B�b�g����
void CollisionEdit(void);			//�I�u�W�F�N�g�̓����蔻��̃G�f�B�b�g����
void UpDownEditObject(void);		//�I�u�W�F�N�g�̏㉺�ړ�����
void RightAngleEditObject(void);	//�I�u�W�F�N�g�̒��p����
void CollisionRotationEdit(void);	//�����蔻��̉�]����
void PlayerPosSetEditObject(void);	//�I�u�W�F�N�g�̃v���C���[�ʒu�ړ�
void JudgeEditObject(void);			//�I�u�W�F�N�g�̑P������

//�j�󃂁[�h�̕\�L
const char *c_apBreakmodename[BREAKTYPE_MAX] =
{
	"�j��s��",
	"�ꌂ�j��",
	"�������",
};

//�e���[�h�̕\�L
const char *c_apShadowmodename[SHADOWTYPE_MAX] =
{
	"�e����",
	"�ۉe",
	"���A���e",
};

//�����蔻�胂�[�h�̕\�L
const char *c_apCollisionmodename[COLLISIONTYPE_MAX] =
{
	"�����蔻�薳��",
	"���f�����_�̓����蔻��",
	"�쐬�����ėp�̓����蔻��",
	//"�쐬�������ꂼ��̓����蔻��",
};

//�P����Ԃ̕\�L
const char *c_apJudgemodename[2] = 
{
	"�ǂ�����",
	"��������",
};

//�O���[�o���ϐ�
EditObject  g_EditObject;								//�I�u�W�F�N�g�̏��
D3DXVECTOR3 g_aRotObject[MODEL_OBJ_MAX];				//�I�u�W�F�N�g�̌����̏��
D3DXVECTOR3 g_aScaleObject[MODEL_OBJ_MAX];				//�I�u�W�F�N�g�̊g�嗦�̏��
int g_nStyleObject;										//�X�^�C���̕ϐ�

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

		//���f���̊�{���
		g_EditObject.modelData = GetModelData(g_EditObject.nType + FROM_OBJECT);

		//�I�𒆂̃}�e���A���̔ԍ�������������
		g_EditObject.nCntMaterial = 0;

		//�F��ς���J�E���g������������
		g_EditObject.nColorCount = 0;

		//���Ȃ�
		g_EditObject.Break.Breaktype = BREAKTYPE_NONE;

		//�e����
		g_EditObject.Shadowtype.Shadowtype = SHADOWTYPE_NONE;

		//�����蔻�肠��
		g_EditObject.Collisiontype.Collisiontype = COLLISIONTYPE_MODEL;

		//�ǂ�����
		g_EditObject.Judge.Judgetype = JUDGESTATE_JUSTICE;
	}

	for (int nCntBreak = 0; nCntBreak < BREAKTYPE_MAX; nCntBreak++)
	{
		//�����̃f�o�b�O�\�L��ݒ�
		g_EditObject.Break.pBreakMode[nCntBreak] = (char*)c_apBreakmodename[nCntBreak];
	}

	for (int nCntShadow = 0; nCntShadow < SHADOWTYPE_MAX; nCntShadow++)
	{
		//�e�̃f�o�b�O�\�L��ݒ�
		g_EditObject.Shadowtype.pShadowMode[nCntShadow] = (char*)c_apShadowmodename[nCntShadow];
	}

	for (int nCntCollision = 0; nCntCollision < COLLISIONTYPE_MAX; nCntCollision++)
	{
		//�����蔻��̃f�o�b�O�\�L��ݒ�
		g_EditObject.Collisiontype.pCollisionMode[nCntCollision] = (char*)c_apCollisionmodename[nCntCollision];
	}

	for (int nCntJudge = 0; nCntJudge < JUDGESTATE_MAX; nCntJudge++)
	{
		//�P���̃f�o�b�O���C��ݒ�
		g_EditObject.Judge.pJudgeMode[nCntJudge] = (char*)c_apJudgemodename[nCntJudge];
	}

	//�����蔻��̌����ϐ�
	g_EditObject.CollInfo.rot      = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
	g_EditObject.CollInfo.stateRot = ROTSTATE_0;					//�������

	//�X�^�C����ݒ肷��
	g_nStyleObject = EDITSTYLE_OBJECT;

	//�J�X�^���p�̃}�e���A�����
	for (int nCntModel = 0; nCntModel < MODEL_OBJ_MAX; nCntModel++)
	{ // ���f���̏�����
		//�}�e���A���̐F���擾����
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)GetModelData(nCntModel + FROM_OBJECT).pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)GetModelData(nCntModel + FROM_OBJECT).dwNumMat; nCntMat++)
		{
			//�}�e���A�����R�s�[����
			g_EditObject.MatCopy[nCntModel][nCntMat] = pMat[nCntMat];
			g_EditObject.EditMaterial[nCntModel][nCntMat] = pMat[nCntMat];
		}
	}

	// �I�u�W�F�N�g�̊g�嗦�̏��̏�����
	for (int nCntObject = 0; nCntObject < MODEL_OBJ_MAX; nCntObject++)
	{ // �I�u�W�F�N�g�̎�ނ̑������J��Ԃ�

		g_aRotObject[nCntObject]   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�I�u�W�F�N�g�̌����̏��
		g_aScaleObject[nCntObject] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);		//�I�u�W�F�N�g�̊g�嗦���
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
	Object *pObject = GetObjectData();				//�I�u�W�F�N�g���擾����

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
				g_EditObject.modelData = GetModelData(g_EditObject.nType + FROM_OBJECT);

				break;							//�����o��
			}
		}

		//�G�f�B�b�g�󋵂̈ꎞ�ۑ�����
		SaveCurrentEdit();
	}
	else
	{//�I�u�W�F�N�g�ݒu���[�h����Ȃ������ꍇ
		//�g�p���Ȃ�
		g_EditObject.bUse = false;
	}

	if (GetCollisionStyle() == COLLISIONSTYLE_OBJECT)
	{ // �����蔻��X�^�C�����I�u�W�F�N�g�ύX��Ԃ̏ꍇ

		//��ޕύX����
		TypeChangeEdit();

		//�ړ�����
		MoveEdit();

		//��]����
		RotationEdit();

		//�I�u�W�F�N�g�̒��p����
		RightAngleEditObject();

		if (g_EditObject.rot.y > D3DX_PI)
		{//3.14f���傫���Ȃ����ꍇ
			//-3.14f�ɕ␳����
			g_EditObject.rot.y -= D3DX_PI * 2;
		}
		else if (g_EditObject.rot.y < -D3DX_PI)
		{//-3.14f��菬�����Ȃ����ꍇ
			//3.14f�ɕ␳����
			g_EditObject.rot.y += D3DX_PI * 2;
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

		//�I�u�W�F�N�g�̔j��G�f�B�b�g����
		BreakEdit();

		//�I�u�W�F�N�g�̉e�̃G�f�B�b�g����
		ShadowEdit();

		//�I�u�W�F�N�g�̓����蔻��̃G�f�B�b�g����
		CollisionEdit();

		//�I�u�W�F�N�g�̏㉺�ړ�����
		UpDownEditObject();

		//��]����
		CollisionRotationEdit();

		//�I�u�W�F�N�g�̃v���C���[�ʒu�ړ�
		PlayerPosSetEditObject();

		//�I�u�W�F�N�g�̑P������
		JudgeEditObject();			
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

		for (int nCntMat = 0; nCntMat < (int)g_EditObject.modelData.dwNumMat; nCntMat++)
		{
			pEditMat = &g_EditObject.EditMaterial[g_EditObject.nType][nCntMat];

			if (nCntMat == g_EditObject.nCntMaterial)
			{//�I�𒆂̃}�e���A���������ꍇ
				//�����x��ݒ肷��
				pEditMat->MatD3D.Ambient.a = EDITOBJ_SELECT_MATERIAL_ALPHA;
				pEditMat->MatD3D.Diffuse.a = EDITOBJ_SELECT_MATERIAL_ALPHA;
			}
			else
			{//�I������Ă��Ȃ��}�e���A���������ꍇ
				//�����x��ݒ肷��
				pEditMat->MatD3D.Ambient.a = EDITOBJ_NORMAL_MATERIAL_ALPHA;
				pEditMat->MatD3D.Diffuse.a = EDITOBJ_NORMAL_MATERIAL_ALPHA;
			}

			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&g_EditObject.EditMaterial[g_EditObject.nType][nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_EditObject.modelData.pTexture[nCntMat]);

			//���f��(�p�[�c)�̕`��
			g_EditObject.modelData.pMesh->DrawSubset(nCntMat);
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
	Object *pObject = GetObjectData();				//�I�u�W�F�N�g�̏����擾����

	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++, pObject++)
	{
		if (pObject->bUse == true)
		{//�I�u�W�F�N�g���g�p����Ă����ꍇ
			if (g_EditObject.pos.x >= pObject->pos.x - EDITOBJ_DELETE_OBJECT_RANGE &&
				g_EditObject.pos.x <= pObject->pos.x + EDITOBJ_DELETE_OBJECT_RANGE &&
				g_EditObject.pos.z >= pObject->pos.z - EDITOBJ_DELETE_OBJECT_RANGE &&
				g_EditObject.pos.z <= pObject->pos.z + EDITOBJ_DELETE_OBJECT_RANGE &&
				g_EditObject.bUse == true)
			{//�͈͓��ɓ������ꍇ
				//�폜�Ώۏ�Ԃɂ���
				pObject->editState = OBJECTSTATE_DELETETARGET;

				if (GetKeyboardTrigger(DIK_9) == true)
				{//9�L�[���������ꍇ

					//�g�p���Ă��Ȃ�
					pObject->bUse = false;
				}
			}
			else
			{//�͈͊O�������ꍇ
				//�g�p��Ԃɂ���
				pObject->editState = OBJECTSTATE_USE;
			}
		}
	}
}

//=======================================
//�G�f�B�b�g�󋵂̈ꎞ�ۑ�����
//=======================================
void SaveCurrentEdit(void)
{
	//�|�C���^��錾
	Collision     *pCollision     = GetCollision();
	EditCollision *pEditCollision = GetEditCollision();

	for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
	{ // �����蔻��̍ő吔���J��Ԃ�

		// �����Əc�����v�Z
		pEditCollision->pCollision->fWidth[nCntColl] = (GetModelData(MODELTYPE_EDIT_COLLISION).size.x * pEditCollision->pCollision->scale[nCntColl].x) * 0.5f;
		pEditCollision->pCollision->fDepth[nCntColl] = (GetModelData(MODELTYPE_EDIT_COLLISION).size.z * pEditCollision->pCollision->scale[nCntColl].z) * 0.5f;
		
		// �����Əc������
		pCollision[g_EditObject.nType].fWidth[nCntColl] = pEditCollision->pCollision->fWidth[nCntColl];
		pCollision[g_EditObject.nType].fDepth[nCntColl] = pEditCollision->pCollision->fDepth[nCntColl];
	}

	//���݂̃��f���������Z�[�u
	g_aRotObject[g_EditObject.nType] = g_EditObject.rot;

	//���݂̃��f���g�嗦���Z�[�u
	g_aScaleObject[g_EditObject.nType] = g_EditObject.scale;
}

//=======================================
//��ޕύX����
//=======================================
void TypeChangeEdit(void)
{
	//�|�C���^��錾
	Collision     *pCollision     = GetCollision();
	EditCollision *pEditCollision = GetEditCollision();

	if (g_nStyleObject == EDITSTYLE_OBJECT)
	{//�I�u�W�F�N�g�ݒu���[�h�������ꍇ
		if (GetKeyboardTrigger(DIK_1) == true &&
			GetKeyboardPress(DIK_LSHIFT) == true)
		{
			//�ݒ�I�u�W�F�N�g�̎�ނ�I������
			g_EditObject.nType = (g_EditObject.nType + (MODEL_OBJ_MAX - 1)) % MODEL_OBJ_MAX;

			//�ݒ肷��
			g_EditObject.modelData = GetModelData(g_EditObject.nType + FROM_OBJECT);

			//���݂̌�����������
			g_EditObject.CollInfo.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_EditObject.CollInfo.stateRot = ROTSTATE_0;

			//���݂̓����蔻������[�h
			pEditCollision->pCollision = &pCollision[g_EditObject.nType];

			// �ʒu���̏�����
			for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
			{ // �����蔻��̍ő吔���J��Ԃ�

				pEditCollision->pos[nCntColl] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �ʒu
				pEditCollision->vecPos[nCntColl] = pEditCollision->pCollision->vecPos[nCntColl];	// �ʒu�x�N�g��
			}

			//���݂̃��f�����������[�h
			g_EditObject.rot = g_aRotObject[g_EditObject.nType];

			//���݂̃��f���g�嗦�����[�h
			g_EditObject.scale = g_aScaleObject[g_EditObject.nType];
		}
		else if (GetKeyboardTrigger(DIK_1) == true)
		{//1�L�[���������ꍇ
			//�ݒ�I�u�W�F�N�g�̎�ނ�I������
			g_EditObject.nType = (g_EditObject.nType + 1) % MODEL_OBJ_MAX;

			//�ݒ肷��
			g_EditObject.modelData = GetModelData(g_EditObject.nType + FROM_OBJECT);

			//���݂̌�����������
			g_EditObject.CollInfo.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_EditObject.CollInfo.stateRot = ROTSTATE_0;

			//���݂̓����蔻������[�h
			pEditCollision->pCollision = &pCollision[g_EditObject.nType];

			// �ʒu���̏�����
			for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
			{ // �����蔻��̍ő吔���J��Ԃ�

				pEditCollision->pos[nCntColl] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �ʒu
				pEditCollision->vecPos[nCntColl] = pEditCollision->pCollision->vecPos[nCntColl];	// �ʒu�x�N�g��
			}

			//���݂̃��f�����������[�h
			g_EditObject.rot = g_aRotObject[g_EditObject.nType];

			//���݂̃��f���g�嗦�����[�h
			g_EditObject.scale = g_aScaleObject[g_EditObject.nType];
		}
	}
}

//=======================================
//�ړ�����
//=======================================
void MoveEdit(void)
{
	//�|�C���^��錾
	EditCollision *pEditCollision = GetEditCollision();

	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{//���V�t�g�L�[��������Ă����ꍇ
		//�����𔲂���
		return;
	}

	if (GetKeyboardPress(DIK_LCONTROL) == true)
	{//���R���g���[���L�[�������Ă����ꍇ
		if (g_nStyleObject == EDITSTYLE_OBJECT)
		{//�I�u�W�F�N�g�ݒu���[�h�������ꍇ
			if (GetKeyboardTrigger(DIK_W) == true)
			{//W�L�[���������ꍇ
				//�ʒu�����ɐi�߂�
				g_EditObject.pos.z += EDITOBJ_ADJUSTMENT_MOVE;
			}

			if (GetKeyboardTrigger(DIK_S) == true)
			{//S�L�[���������ꍇ
				//�ʒu����O�ɐi�߂�
				g_EditObject.pos.z -= EDITOBJ_ADJUSTMENT_MOVE;
			}

			if (GetKeyboardTrigger(DIK_A) == true)
			{//A�L�[���������ꍇ
				//�ʒu�����ɐi�߂�
				g_EditObject.pos.x -= EDITOBJ_ADJUSTMENT_MOVE;
			}

			if (GetKeyboardTrigger(DIK_D) == true)
			{//D�L�[���������ꍇ
				//�ʒu���E�ɐi�߂�
				g_EditObject.pos.x += EDITOBJ_ADJUSTMENT_MOVE;
			}
		}
	}
	else
	{//���R���g���[���������Ă��Ȃ��ꍇ
		if (g_nStyleObject == EDITSTYLE_OBJECT)
		{//�I�u�W�F�N�g�ݒu���[�h�������ꍇ
			if (GetKeyboardPress(DIK_W) == true)
			{//W�L�[���������ꍇ
			 //�ʒu�����ɐi�߂�
				g_EditObject.pos.z += EDITOBJ_MOVE;
			}

			if (GetKeyboardPress(DIK_S) == true)
			{//S�L�[���������ꍇ
			 //�ʒu����O�ɐi�߂�
				g_EditObject.pos.z -= EDITOBJ_MOVE;
			}

			if (GetKeyboardPress(DIK_A) == true)
			{//A�L�[���������ꍇ
			 //�ʒu�����ɐi�߂�
				g_EditObject.pos.x -= EDITOBJ_MOVE;
			}

			if (GetKeyboardPress(DIK_D) == true)
			{//D�L�[���������ꍇ
			 //�ʒu���E�ɐi�߂�
				g_EditObject.pos.x += EDITOBJ_MOVE;
			}
		}
	}

	for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
	{ // �����蔻��̍ő吔���J��Ԃ�

		//�����蔻��̈ʒu�𔽉f (�x�N�g���̋t����)
		pEditCollision->pos[nCntColl] = g_EditObject.pos - pEditCollision->vecPos[nCntColl];
	}
}

//=======================================
//��]����
//=======================================
void RotationEdit(void)
{
	if (GetKeyboardPress(DIK_LCONTROL) == true)
	{//��CTRL�L�[�������Ă���ꍇ
		//�����𔲂���
		return;
	}

	if (GetKeyboardPress(DIK_Q) == true)
	{//Q�L�[���������ꍇ
		//������ς���
		g_EditObject.rot.y += EDITOBJ_ROT_MOVE;
	}
	else if (GetKeyboardPress(DIK_E) == true)
	{//E�L�[���������ꍇ
		//������ς���
		g_EditObject.rot.y -= EDITOBJ_ROT_MOVE;
	}
}

//=======================================
//�I�u�W�F�N�g�̐ݒ菈��
//=======================================
void SetEdit(void)
{
	// �ϐ���錾
	ROTSTATE stateRot;

	// �|�C���^��錾
	EditCollision *pEditCollision = GetEditCollision();

	if (g_nStyleObject == EDITSTYLE_OBJECT)
	{//�I�u�W�F�N�g�ݒu���[�h�������ꍇ
		if (GetKeyboardTrigger(DIK_0) == true)
		{//0�L�[���������ꍇ
			for (int nCount = 0; nCount < MAX_MATERIAL; nCount++)
			{
				// �����x�����ɖ߂�
				g_EditObject.EditMaterial[g_EditObject.nType][nCount].MatD3D.Ambient.a = g_EditObject.MatCopy[g_EditObject.nType][nCount].MatD3D.Ambient.a;
				g_EditObject.EditMaterial[g_EditObject.nType][nCount].MatD3D.Diffuse.a = g_EditObject.MatCopy[g_EditObject.nType][nCount].MatD3D.Diffuse.a;
			}

			//������������Ԃ�ݒ�
			stateRot = (g_EditObject.Collisiontype.Collisiontype == COLLISIONTYPE_CREATE) ? g_EditObject.CollInfo.stateRot : ROTSTATE_0;

			//�I�u�W�F�N�g�̐ݒ菈��
			SetObject
			(
				g_EditObject.pos,									// �ʒu
				g_EditObject.rot,									// ����
				g_EditObject.scale,									// �g�嗦
				&g_EditObject.EditMaterial[g_EditObject.nType][0],	// �}�e���A���̃f�[�^
				g_EditObject.nType,									// ���
				g_EditObject.Break.Breaktype,						// �j��̎��
				g_EditObject.Shadowtype.Shadowtype,					// �e�̎��
				g_EditObject.Collisiontype.Collisiontype,			// �����蔻��̎��
				stateRot,											// �������
				APPEARSTATE_COMPLETE,								// �o�����@
				g_EditObject.Judge.Judgetype						// �P���̎��
			);

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
	//�|�C���^��錾
	EditCollision *pEditCollision = GetEditCollision();

	if (g_nStyleObject == EDITSTYLE_OBJECT)
	{//�I�u�W�F�N�g�ݒu���[�h�������ꍇ
		if (GetKeyboardPress(DIK_LCONTROL) == true)
		{//���R���g���[���L�[�������Ă����ꍇ
			if (GetKeyboardTrigger(DIK_U) == true)
			{//U�L�[���������ꍇ
				//X�����g�傷��
				g_EditObject.scale.x += EDITOBJ_SCALING;
			}
			else if (GetKeyboardTrigger(DIK_J) == true)
			{//J�L�[���������ꍇ
				//X�����k������
				g_EditObject.scale.x -= EDITOBJ_SCALING;
			}
		}
		else
		{//���R���g���[���L�[�������Ă��Ȃ��ꍇ
			if (GetKeyboardPress(DIK_U) == true)
			{//U�L�[���������ꍇ
				//X�����g�傷��
				g_EditObject.scale.x += EDITOBJ_SCALING;
			}
			else if (GetKeyboardPress(DIK_J) == true)
			{//J�L�[���������ꍇ
				//X�����k������
				g_EditObject.scale.x -= EDITOBJ_SCALING;
			}
		}
	}
}

//=======================================
//�I�u�W�F�N�g�̊g��k������(Y��)
//=======================================
void ScaleObjectY(void)
{
	//�|�C���^��錾
	EditCollision *pEditCollision = GetEditCollision();

	if (g_nStyleObject == EDITSTYLE_OBJECT)
	{//�I�u�W�F�N�g�ݒu���[�h�������ꍇ
		if (GetKeyboardPress(DIK_LCONTROL) == true)
		{//���R���g���[���L�[�������Ă����ꍇ
			if (GetKeyboardTrigger(DIK_I) == true)
			{//I�L�[���������ꍇ
				//Y�����g�傷��
				g_EditObject.scale.y += EDITOBJ_SCALING;
			}
			else if (GetKeyboardTrigger(DIK_K) == true)
			{//K�L�[���������ꍇ
				//Y�����k������
				g_EditObject.scale.y -= EDITOBJ_SCALING;
			}
		}
		else
		{//���R���g���[���L�[�������Ă��Ȃ��ꍇ
			if (GetKeyboardPress(DIK_I) == true)
			{//I�L�[���������ꍇ
				//Y�����g�傷��
				g_EditObject.scale.y += EDITOBJ_SCALING;
			}
			else if (GetKeyboardPress(DIK_K) == true)
			{//K�L�[���������ꍇ
				//Y�����k������
				g_EditObject.scale.y -= EDITOBJ_SCALING;
			}
		}
	}
}

//=======================================
//�I�u�W�F�N�g�̊g��k������(Z��)
//=======================================
void ScaleObjectZ(void)
{
	//�|�C���^��錾
	EditCollision *pEditCollision = GetEditCollision();

	if (g_nStyleObject == EDITSTYLE_OBJECT)
	{//�I�u�W�F�N�g�ݒu���[�h�������ꍇ
		if (GetKeyboardPress(DIK_LCONTROL) == true)
		{//���R���g���[���L�[�������Ă����ꍇ
			if (GetKeyboardTrigger(DIK_O) == true)
			{//O�L�[���������ꍇ
				//Z�����g�傷��
				g_EditObject.scale.z += EDITOBJ_SCALING;
			}
			else if (GetKeyboardTrigger(DIK_L) == true)
			{//L�L�[���������ꍇ
				//Z�����k������
				g_EditObject.scale.z -= EDITOBJ_SCALING;
			}
		}
		else
		{//���R���g���[���L�[�������Ă��Ȃ��ꍇ
			if (GetKeyboardPress(DIK_O) == true)
			{//O�L�[���������ꍇ
				//Z�����g�傷��
				g_EditObject.scale.z += EDITOBJ_SCALING;
			}
			else if (GetKeyboardPress(DIK_L) == true)
			{//L�L�[���������ꍇ
				//Z�����k������
				g_EditObject.scale.z -= EDITOBJ_SCALING;
			}
		}
	}
}

//=======================================
//�I�u�W�F�N�g�̊g��k������
//=======================================
void ScaleObject(void)
{
	//�|�C���^��錾
	EditCollision *pEditCollision = GetEditCollision();

	if (g_nStyleObject == EDITSTYLE_OBJECT)
	{//�I�u�W�F�N�g�ݒu���[�h�������ꍇ
		if (GetKeyboardPress(DIK_LCONTROL) == true)
		{//���R���g���[���L�[�������Ă����ꍇ
			if (GetKeyboardTrigger(DIK_4) == true)
			{//4�L�[���������ꍇ
				//�g�傷��
				g_EditObject.scale.x += EDITOBJ_SCALING;
				g_EditObject.scale.y += EDITOBJ_SCALING;
				g_EditObject.scale.z += EDITOBJ_SCALING;
			}
			else if (GetKeyboardTrigger(DIK_5) == true)
			{//5�L�[���������ꍇ
				//�k������
				g_EditObject.scale.x -= EDITOBJ_SCALING;
				g_EditObject.scale.y -= EDITOBJ_SCALING;
				g_EditObject.scale.z -= EDITOBJ_SCALING;
			}
		}
		else
		{//���R���g���[���L�[�������Ă��Ȃ��ꍇ
			if (GetKeyboardPress(DIK_4) == true)
			{//4�L�[���������ꍇ
				//�g�傷��
				g_EditObject.scale.x += EDITOBJ_SCALING;
				g_EditObject.scale.y += EDITOBJ_SCALING;
				g_EditObject.scale.z += EDITOBJ_SCALING;
			}
			else if (GetKeyboardPress(DIK_5) == true)
			{//5�L�[���������ꍇ
				//�k������
				g_EditObject.scale.x -= EDITOBJ_SCALING;
				g_EditObject.scale.y -= EDITOBJ_SCALING;
				g_EditObject.scale.z -= EDITOBJ_SCALING;
			}
		}
	}
}

//=======================================
//�I�u�W�F�N�g�̏�񃊃Z�b�g����
//=======================================
void ResetEdit(void)
{
	if (g_nStyleObject == EDITSTYLE_OBJECT)
	{//�I�u�W�F�N�g�ݒu���[�h�������ꍇ

		if (GetKeyboardTrigger(DIK_2) == true)
		{//2�L�[���������ꍇ

			//�p�x������������
			g_EditObject.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
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
	D3DMATERIAL9 *pMatEdit;		//�}�e���A���f�[�^�ւ̃|�C���^

	if (g_EditObject.bUse == true)
	{//�ݒ蒆���g�p���Ă���ꍇ

		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{//SPACE�L�[���������ꍇ

			//�}�e���A����ύX����
			g_EditObject.nCntMaterial = (g_EditObject.nCntMaterial + 1) % (int)(g_EditObject.modelData.dwNumMat);
		}

		for (int nCntMat = 0; nCntMat < (int)g_EditObject.modelData.dwNumMat; nCntMat++)
		{
			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMatEdit = &g_EditObject.EditMaterial[g_EditObject.nType][nCntMat].MatD3D;

			if (nCntMat == g_EditObject.nCntMaterial)
			{//�I�𒆂̃}�e���A���������ꍇ

				if (GetKeyboardPress(DIK_LSHIFT) == true)
				{//���V�t�g�L�[���������ꍇ

					if (GetKeyboardPress(DIK_R) == true)
					{//R�L�[���������ꍇ

						//�J�E���g�����Z����
						g_EditObject.nColorCount++;

						if (g_EditObject.nColorCount % EDITOBJ_COL_CHANGE_CNT == 0)
						{//20F���Ƃ�

							//R�l�𑝂₷
							pMatEdit->Ambient.r += EDITOBJ_COL_CONVERSION;

							if (pMatEdit->Ambient.r > 1.0f)
							{//R�l�����E�𒴂�����

								//R�l��0�ɖ߂�
								pMatEdit->Ambient.r = 0.0f;
							}

							pMatEdit->Diffuse.r += EDITOBJ_COL_CONVERSION;

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

						if (g_EditObject.nColorCount % EDITOBJ_COL_CHANGE_CNT == 0)
						{//20F���Ƃ�

							//R�l�𑝂₷
							pMatEdit->Ambient.r -= EDITOBJ_COL_CONVERSION;

							if (pMatEdit->Ambient.r < 0.0f)
							{//R�l�����E�𒴂�����

								//R�l��0�ɖ߂�
								pMatEdit->Ambient.r = 1.0f;
							}

							pMatEdit->Diffuse.r -= EDITOBJ_COL_CONVERSION;

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

						if (g_EditObject.nColorCount % EDITOBJ_COL_CHANGE_CNT == 0)
						{//20F���Ƃ�

							//G�l�𑝂₷
							pMatEdit->Ambient.g += EDITOBJ_COL_CONVERSION;

							if (pMatEdit->Ambient.g > 1.0f)
							{//G�l�����E�𒴂�����

								//G�l��0�ɖ߂�
								pMatEdit->Ambient.g = 0.0f;
							}

							//G�l�𑝂₷
							pMatEdit->Diffuse.g += EDITOBJ_COL_CONVERSION;

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

						if (g_EditObject.nColorCount % EDITOBJ_COL_CHANGE_CNT == 0)
						{//20F���Ƃ�

							//R�l�𑝂₷
							pMatEdit->Ambient.g -= EDITOBJ_COL_CONVERSION;

							if (pMatEdit->Ambient.g < 0.0f)
							{//R�l�����E�𒴂�����

								//R�l��0�ɖ߂�
								pMatEdit->Ambient.g = 1.0f;
							}

							//G�l�𑝂₷
							pMatEdit->Diffuse.g -= EDITOBJ_COL_CONVERSION;

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

						if (g_EditObject.nColorCount % EDITOBJ_COL_CHANGE_CNT == 0)
						{//20F���Ƃ�

							//B�l�𑝂₷
							pMatEdit->Ambient.b += EDITOBJ_COL_CONVERSION;

							if (pMatEdit->Ambient.b > 1.0f)
							{//B�l�����E�𒴂�����

								//B�l��0�ɖ߂�
								pMatEdit->Ambient.b = 0.0f;
							}

							//B�l�𑝂₷
							pMatEdit->Diffuse.b += EDITOBJ_COL_CONVERSION;

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

						if (g_EditObject.nColorCount % EDITOBJ_COL_CHANGE_CNT == 0)
						{//20F���Ƃ�

							//R�l�𑝂₷
							pMatEdit->Ambient.b -= EDITOBJ_COL_CONVERSION;

							if (pMatEdit->Ambient.b < 0.0f)
							{//R�l�����E�𒴂�����

								//R�l��0�ɖ߂�
								pMatEdit->Ambient.b = 1.0f;
							}

							pMatEdit->Diffuse.b -= EDITOBJ_COL_CONVERSION;

							if (pMatEdit->Diffuse.b < 0.0f)
							{//R�l�����E�𒴂�����

								//R�l��0�ɖ߂�
								pMatEdit->Diffuse.b = 1.0f;
							}
						}
					}

					//�A�h���X������
					D3DMATERIAL9 *pMatOriginal = &g_EditObject.MatCopy[g_EditObject.nType][nCntMat].MatD3D;

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
//�I�u�W�F�N�g�̔j��\����
//=======================================
void BreakEdit(void)
{
	if (GetKeyboardTrigger(DIK_7) == true)
	{//7�L�[���������ꍇ
		//�j��̕ϐ���؂�ւ���
		g_EditObject.Break.Breaktype = (g_EditObject.Break.Breaktype + 1) % BREAKTYPE_MAX;
	}
}

//=======================================
//�I�u�W�F�N�g�̉e�̃G�f�B�b�g����
//=======================================
void ShadowEdit(void)
{
	if (GetKeyboardTrigger(DIK_8) == true)
	{//8�L�[���������ꍇ
		//�e�̎�ނ�؂�ւ���
		g_EditObject.Shadowtype.Shadowtype = (g_EditObject.Shadowtype.Shadowtype + 1) % SHADOWTYPE_MAX;
	}
}

//=======================================
//�I�u�W�F�N�g�̓����蔻��̃G�f�B�b�g����
//=======================================
void CollisionEdit(void)
{
	if (GetKeyboardTrigger(DIK_BACKSPACE) == true)
	{//DIK_BACKSPACE�L�[���������ꍇ
		//�����蔻��̎�ނ�؂�ւ���
		g_EditObject.Collisiontype.Collisiontype = (g_EditObject.Collisiontype.Collisiontype + 1) % COLLISIONTYPE_MAX;
	}
}

//=======================================
//�I�u�W�F�N�g�̏㉺�ړ�����
//=======================================
void UpDownEditObject(void)
{
	//�|�C���^��錾
	EditCollision *pEditCollision = GetEditCollision();

	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{//��SHIFT�L�[�������Ă���ꍇ
		if (GetKeyboardPress(DIK_LCONTROL) == true)
		{//���R���g���[���L�[�������Ă����ꍇ
			if (GetKeyboardTrigger(DIK_W) == true)
			{//W�L�[�������Ă���ꍇ
				//�ʒu�����ɐi�߂�
				g_EditObject.pos.y += EDITOBJ_UPDOWN_ADJUSTMENT_MOVE;
			}

			if (GetKeyboardTrigger(DIK_S) == true)
			{//S�L�[�������Ă���ꍇ
				//�ʒu����O�ɐi�߂�
				g_EditObject.pos.y -= EDITOBJ_UPDOWN_ADJUSTMENT_MOVE;
			}
		}
		else
		{//���R���g���[���L�[�������Ă��Ȃ��ꍇ
			if (GetKeyboardPress(DIK_W) == true)
			{//W�L�[�������Ă���ꍇ
			 //�ʒu�����ɐi�߂�
				g_EditObject.pos.y += EDITOBJ_UPDOWN_MOVE;
			}

			if (GetKeyboardPress(DIK_S) == true)
			{//S�L�[�������Ă���ꍇ
			 //�ʒu����O�ɐi�߂�
				g_EditObject.pos.y -= EDITOBJ_UPDOWN_MOVE;
			}
		}

		if (GetKeyboardTrigger(DIK_A) == true || GetKeyboardTrigger(DIK_D) == true)
		{//A�L�[��D�L�[���������ꍇ
			//�n�ʂɖ߂�
			g_EditObject.pos.y = 0.0f;
		}
	}

	for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
	{ // �����蔻��̍ő吔���J��Ԃ�

		//�����蔻��̈ʒu�𔽉f (�x�N�g���̋t����)
		pEditCollision->pos[nCntColl] = g_EditObject.pos - pEditCollision->vecPos[nCntColl];
	}
}

//=======================================
//�I�u�W�F�N�g�̒��p����
//=======================================
void RightAngleEditObject(void)
{
	if (GetKeyboardPress(DIK_LCONTROL) == true)
	{//��CTRL�L�[�������Ă���ꍇ
		if (GetKeyboardTrigger(DIK_Q) == true)
		{//Q�L�[���������ꍇ
			//������ς���
			g_EditObject.rot.y += D3DXToRadian(EDITOBJ_ADJUSTMENT_ROT_MOVE);
		}
		else if (GetKeyboardTrigger(DIK_E) == true)
		{//E�L�[���������ꍇ
			//������ς���
			g_EditObject.rot.y -= D3DXToRadian(EDITOBJ_ADJUSTMENT_ROT_MOVE);
		}
	}
}

//=======================================
//�����蔻���]����
//=======================================
void CollisionRotationEdit(void)
{
	// �|�C���^��錾
	EditCollision *pEditCollision = GetEditCollision();

	if (GetKeyboardTrigger(DIK_C) == true)
	{//C�L�[���������ꍇ
		//������ύX
		g_EditObject.CollInfo.rot.y += D3DX_PI * 0.5f;

		//������Ԃ�ύX
		g_EditObject.CollInfo.stateRot = (ROTSTATE)((g_EditObject.CollInfo.stateRot + 1) % ROTSTATE_MAX);

		for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
		{ // �����蔻��̍ő吔���J��Ԃ�

			//�ʒu�x�N�g����90�x��]
			pEditCollision->vecPos[nCntColl] = D3DXVECTOR3(-pEditCollision->vecPos[nCntColl].z, pEditCollision->vecPos[nCntColl].y, pEditCollision->vecPos[nCntColl].x);

			//�ʒu�𔽉f (�x�N�g���̕���)
			pEditCollision->pos[nCntColl] = g_EditObject.pos - pEditCollision->vecPos[nCntColl];
		}
	}
	else if (GetKeyboardTrigger(DIK_Z) == true)
	{//Z�L�[���������ꍇ
		//������ύX
		g_EditObject.CollInfo.rot.y -= D3DX_PI * 0.5f;

		//������Ԃ�ύX
		g_EditObject.CollInfo.stateRot = (ROTSTATE)((g_EditObject.CollInfo.stateRot + (ROTSTATE_MAX - 1)) % ROTSTATE_MAX);

		for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
		{ // �����蔻��̍ő吔���J��Ԃ�

			//�ʒu�x�N�g����90�x��]
			pEditCollision->vecPos[nCntColl] = D3DXVECTOR3(pEditCollision->vecPos[nCntColl].z, pEditCollision->vecPos[nCntColl].y, -pEditCollision->vecPos[nCntColl].x);

			//�ʒu�𔽉f (�x�N�g���̕���)
			pEditCollision->pos[nCntColl] = g_EditObject.pos - pEditCollision->vecPos[nCntColl];
		}
	}

	if (g_EditObject.CollInfo.rot.y > D3DX_PI)
	{//3.14f���傫���Ȃ����ꍇ
		//-3.14f�ɕ␳����
		g_EditObject.CollInfo.rot.y -= D3DX_PI * 2;
	}
	else if (g_EditObject.CollInfo.rot.y < -D3DX_PI)
	{//-3.14f��菬�����Ȃ����ꍇ
		//3.14f�ɕ␳����
		g_EditObject.CollInfo.rot.y += D3DX_PI * 2;
	}
}

//=======================================
//�I�u�W�F�N�g�̃v���C���[�ʒu�ړ�
//=======================================
void PlayerPosSetEditObject(void)
{
	// �|�C���^��錾
	Player *pPlayer = GetPlayer();	// �v���C���[�̏��

	if (GetKeyboardTrigger(DIK_LMENU) == true)
	{ // ���� ALT�L�[�������ꂽ�ꍇ

		// �v���C���[�̈ʒu�Ɉʒu���ړ�
		g_EditObject.pos = pPlayer->pos;
	}
}

//=======================================
//�I�u�W�F�N�g�̑P������
//=======================================
void JudgeEditObject(void)
{
	if (GetKeyboardTrigger(DIK_RCONTROL) == true)
	{//�E�R���g���[���L�[���������ꍇ

		//�P���̎�ނ�؂�ւ���
		g_EditObject.Judge.Judgetype = (g_EditObject.Judge.Judgetype + 1) % JUDGESTATE_MAX;
	}
}

#endif