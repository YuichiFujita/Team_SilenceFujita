#ifdef _DEBUG	// �f�o�b�O����
//===========================================
//
//�I�u�W�F�N�g�̃��C������[EditCollision.cpp]
//Author ��������
//
//===========================================
#include "EditCollision.h"
#include "EditObject.h"
#include "Editmain.h"
#include "input.h"
#include "object.h"

//�v���g�^�C�v�錾
void CollisionMoveEdit(void);		// �ړ�����
void CollisionRotationEdit(void);	// ��]����
void CollisionScaleObjectX(void);	// �I�u�W�F�N�g�̊g��k������(X��)
void CollisionScaleObjectY(void);	// �I�u�W�F�N�g�̊g��k������(Y��)
void CollisionScaleObjectZ(void);	// �I�u�W�F�N�g�̊g��k������(Z��)
void CollisionScaleObject(void);	// �I�u�W�F�N�g�̊g��k������
void CollisionResetEdit(void);		// �I�u�W�F�N�g�̏�񃊃Z�b�g����
void CollisionUpDownEdit(void);		// �I�u�W�F�N�g�̏㉺�ړ�����

//�O���[�o���ϐ�
EditCollision g_EditCollision;		// �G�f�B�b�g�����蔻��̏��

//==========================================
//���f���̏���������
//==========================================
void InitEditCollision(void)
{
	//�|�C���^��錾
	Collision *pCollision = GetCollision();

	// ��{���̏�����
	g_EditCollision.pos       = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
	g_EditCollision.rot       = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ����
	g_EditCollision.stateRot  = ROTSTATE_0;								// �������
	g_EditCollision.modelData = GetModelData(MODELTYPE_EDIT_COLLISION);	// ���f�����
	g_EditCollision.nType     = GetEditObject()->nType;					// �I�u�W�F�N�g�̎��

	// �G�f�B�b�g�����蔻��̏���������
	g_EditCollision.collision = *pCollision;
}

//========================================
//���f���̏I������
//========================================
void UninitEditCollision(void)
{

}

//========================================
//���f���̍X�V����
//========================================
void UpdateEditCollision(void)
{
	if (GetStyle() == EDITSTYLE_OBJECT)
	{ // �I�u�W�F�N�g�ݒu���[�h�������ꍇ

		if (GetEditObject()->Collisiontype.Collisiontype == COLLISIONTYPE_CREATE
		/*||  GetEditObject()->Collisiontype.Collisiontype == COLLISIONTYPE_ONLY*/)
		{//�����蔻��̎�ނ��쐬�֘A�̎�ނ̏ꍇ

			if (GetCollisionStyle() == COLLISIONSTYLE_COLLISION)
			{ // �����蔻��X�^�C���������蔻��ύX��Ԃ̏ꍇ

				//��]����
				CollisionRotationEdit();

				//�ړ�����
				CollisionMoveEdit();

				//�I�u�W�F�N�g�̊g��k������(X��)
				CollisionScaleObjectX();

				//�I�u�W�F�N�g�̊g��k������(Y��)
				CollisionScaleObjectY();

				//�I�u�W�F�N�g�̊g��k������(Z��)
				CollisionScaleObjectZ();

				//�I�u�W�F�N�g�̊g��k������
				CollisionScaleObject();

				//�I�u�W�F�N�g�̏�񃊃Z�b�g����
				CollisionResetEdit();

				//�I�u�W�F�N�g�̏㉺�ړ�����
				CollisionUpDownEdit();
			}
		}
	}
}

//=====================================
//���f���̕`�揈��
//=====================================
void DrawEditCollision(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;		// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							// �}�e���A���f�[�^�ւ̃|�C���^

	if (GetEditObject()->Collisiontype.Collisiontype == COLLISIONTYPE_CREATE
	/*||  GetEditObject()->Collisiontype.Collisiontype == COLLISIONTYPE_ONLY*/)
	{//�����蔻��̎�ނ��쐬�֘A�̎�ނ̏ꍇ

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_EditCollision.mtxWorld);

		//�g�嗦�𔽉f
		D3DXMatrixScaling(&mtxScale, g_EditCollision.collision.scale.x, g_EditCollision.collision.scale.y, g_EditCollision.collision.scale.z);
		D3DXMatrixMultiply(&g_EditCollision.mtxWorld, &g_EditCollision.mtxWorld, &mtxScale);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_EditCollision.rot.y, g_EditCollision.rot.x, g_EditCollision.rot.z);
		D3DXMatrixMultiply(&g_EditCollision.mtxWorld, &g_EditCollision.mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_EditCollision.pos.x, g_EditCollision.pos.y, g_EditCollision.pos.z);
		D3DXMatrixMultiply(&g_EditCollision.mtxWorld, &g_EditCollision.mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_EditCollision.mtxWorld);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_EditCollision.modelData.pBuffMat->GetBufferPointer();;

		for (int nCntMat = 0; nCntMat < (int)g_EditCollision.modelData.dwNumMat; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// �ʏ�

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_EditCollision.modelData.pTexture[nCntMat]);

			//���f��(�p�[�c)�̕`��
			g_EditCollision.modelData.pMesh->DrawSubset(nCntMat);
		}
		//�ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//=======================================
//�G�f�B�b�g�I�u�W�F�N�g�̎擾����
//=======================================
EditCollision *GetEditCollision(void)
{
	//�G�f�B�b�g�I�u�W�F�N�g�̏���Ԃ�
	return &g_EditCollision;
}

//=======================================
//�ړ�����
//=======================================
void CollisionMoveEdit(void)
{
	// �|�C���^��錾
	EditObject *pEditObject = GetEditObject();

	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{//���V�t�g�L�[��������Ă����ꍇ

		//�����𔲂���
		return;
	}

	if (GetKeyboardPress(DIK_LCONTROL) == true)
	{//���R���g���[���L�[�������Ă����ꍇ

		if (GetKeyboardTrigger(DIK_W) == true)
		{//W�L�[���������ꍇ
			//�ʒu�����ɐi�߂�
			g_EditCollision.pos.z += 4.0f;
		}

		if (GetKeyboardTrigger(DIK_S) == true)
		{//S�L�[���������ꍇ
			//�ʒu����O�ɐi�߂�
			g_EditCollision.pos.z -= 4.0f;
		}

		if (GetKeyboardTrigger(DIK_A) == true)
		{//A�L�[���������ꍇ
			//�ʒu�����ɐi�߂�
			g_EditCollision.pos.x -= 4.0f;
		}

		if (GetKeyboardTrigger(DIK_D) == true)
		{//D�L�[���������ꍇ
			//�ʒu���E�ɐi�߂�
			g_EditCollision.pos.x += 4.0f;
		}
	}
	else
	{//���R���g���[���������Ă��Ȃ��ꍇ

		if (GetKeyboardPress(DIK_W) == true)
		{//W�L�[���������ꍇ
		 //�ʒu�����ɐi�߂�
			g_EditCollision.pos.z += 16.0f;
		}

		if (GetKeyboardPress(DIK_S) == true)
		{//S�L�[���������ꍇ
		 //�ʒu����O�ɐi�߂�
			g_EditCollision.pos.z -= 16.0f;
		}

		if (GetKeyboardPress(DIK_A) == true)
		{//A�L�[���������ꍇ
		 //�ʒu�����ɐi�߂�
			g_EditCollision.pos.x -= 16.0f;
		}

		if (GetKeyboardPress(DIK_D) == true)
		{//D�L�[���������ꍇ
		 //�ʒu���E�ɐi�߂�
			g_EditCollision.pos.x += 16.0f;
		}
	}

	//�I�u�W�F�N�g���猩�������蔻��̐�΍��W�x�N�g����ݒ�
	g_EditCollision.collision.vecPos = pEditObject->pos - g_EditCollision.pos;
}

//=======================================
//��]����
//=======================================
void CollisionRotationEdit(void)
{
	// �|�C���^��錾
	EditObject *pEditObject = GetEditObject();

	if (GetKeyboardTrigger(DIK_Q) == true)
	{//Q�L�[���������ꍇ
		//������ύX
		g_EditCollision.rot.y += D3DX_PI * 0.5f;

		//������Ԃ�ύX
		g_EditCollision.stateRot = (ROTSTATE)((g_EditCollision.stateRot + 1) % ROTSTATE_MAX);

		//�ʒu�x�N�g����90�x��]
		g_EditCollision.collision.vecPos = D3DXVECTOR3(-g_EditCollision.collision.vecPos.z, g_EditCollision.collision.vecPos.y, g_EditCollision.collision.vecPos.x);

		//�ʒu�𔽉f (�x�N�g���̕���)
		g_EditCollision.pos = pEditObject->pos - g_EditCollision.collision.vecPos;
	}
	else if (GetKeyboardTrigger(DIK_E) == true)
	{//E�L�[���������ꍇ
		//������ύX
		g_EditCollision.rot.y -= D3DX_PI * 0.5f;

		//������Ԃ�ύX
		g_EditCollision.stateRot = (ROTSTATE)((g_EditCollision.stateRot + (ROTSTATE_MAX - 1)) % ROTSTATE_MAX);

		//�ʒu�x�N�g����90�x��]
		g_EditCollision.collision.vecPos = D3DXVECTOR3(g_EditCollision.collision.vecPos.z, g_EditCollision.collision.vecPos.y, -g_EditCollision.collision.vecPos.x);

		//�ʒu�𔽉f (�x�N�g���̕���)
		g_EditCollision.pos = pEditObject->pos - g_EditCollision.collision.vecPos;
	}

	if (g_EditCollision.rot.y > D3DX_PI)
	{//3.14f���傫���Ȃ����ꍇ
		//-3.14f�ɕ␳����
		g_EditCollision.rot.y -= D3DX_PI * 2;
	}
	else if (g_EditCollision.rot.y < -D3DX_PI)
	{//-3.14f��菬�����Ȃ����ꍇ
		//3.14f�ɕ␳����
		g_EditCollision.rot.y += D3DX_PI * 2;
	}
}

//=======================================
//�I�u�W�F�N�g�̊g��k������(X��)
//=======================================
void CollisionScaleObjectX(void)
{
	// �ϐ���錾
	float fScale = 0.0f;

	if (GetKeyboardPress(DIK_U) == true)
	{//U�L�[���������ꍇ
		//�g��ʂ�ۑ�����
		fScale += 0.02f;
	}
	else if (GetKeyboardPress(DIK_J) == true)
	{//J�L�[���������ꍇ
		//�k���ʂ�ۑ�����
		fScale -= 0.02f;
	}

	// �����Əc�����v�Z
	if (g_EditCollision.stateRot == ROTSTATE_0
	||  g_EditCollision.stateRot == ROTSTATE_180)
	{ // �p�x��0�x�A�܂���180�x�̏ꍇ
		//X�����g�k����
		g_EditCollision.collision.scale.x += fScale;
	}
	else
	{ // �p�x90�x�A�܂���270�x�̏ꍇ
		//Z�����g�k����
		g_EditCollision.collision.scale.z += fScale;
	}
}

//=======================================
//�I�u�W�F�N�g�̊g��k������(Y��)
//=======================================
void CollisionScaleObjectY(void)
{
	if (GetKeyboardPress(DIK_I) == true)
	{//I�L�[���������ꍇ
		//Y�����g�傷��
		g_EditCollision.collision.scale.y += 0.02f;
	}
	else if (GetKeyboardPress(DIK_K) == true)
	{//K�L�[���������ꍇ
		//Y�����k������
		g_EditCollision.collision.scale.y -= 0.02f;
	}
}

//=======================================
//�I�u�W�F�N�g�̊g��k������(Z��)
//=======================================
void CollisionScaleObjectZ(void)
{
	// �ϐ���錾
	float fScale = 0.0f;

	if (GetKeyboardPress(DIK_O) == true)
	{//O�L�[���������ꍇ
		//�g��ʂ�ۑ�����
		fScale += 0.02f;
	}
	else if (GetKeyboardPress(DIK_L) == true)
	{//L�L�[���������ꍇ
		//�k���ʂ�ۑ�����
		fScale -= 0.02f;
	}

	// �����Əc�����v�Z
	if (g_EditCollision.stateRot == ROTSTATE_0
	||  g_EditCollision.stateRot == ROTSTATE_180)
	{ // �p�x��0�x�A�܂���180�x�̏ꍇ
		//Z�����g�k����
		g_EditCollision.collision.scale.z += fScale;
	}
	else
	{ // �p�x90�x�A�܂���270�x�̏ꍇ
		//X�����g�k����
		g_EditCollision.collision.scale.x += fScale;
	}
}

//=======================================
//�I�u�W�F�N�g�̊g��k������
//=======================================
void CollisionScaleObject(void)
{
	if (GetKeyboardPress(DIK_4) == true)
	{//4�L�[���������ꍇ
		//�g�傷��
		g_EditCollision.collision.scale.x += 0.02f;
		g_EditCollision.collision.scale.y += 0.02f;
		g_EditCollision.collision.scale.z += 0.02f;
	}
	else if (GetKeyboardPress(DIK_5) == true)
	{//5�L�[���������ꍇ
		//�k������
		g_EditCollision.collision.scale.x -= 0.02f;
		g_EditCollision.collision.scale.y -= 0.02f;
		g_EditCollision.collision.scale.z -= 0.02f;
	}
}

//=======================================
//�I�u�W�F�N�g�̏�񃊃Z�b�g����
//=======================================
void CollisionResetEdit(void)
{
	// �|�C���^��錾
	EditObject *pEditObject = GetEditObject();

	if (GetKeyboardTrigger(DIK_1) == true)
	{//1�L�[���������ꍇ
		//�ʒu���I�u�W�F�N�g�ɍ��킹��
		g_EditCollision.pos = pEditObject->pos;
	}
	if (GetKeyboardTrigger(DIK_2) == true)
	{//2�L�[���������ꍇ
		//�p�x������������
		g_EditCollision.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		for (int nCntRot = g_EditCollision.stateRot; nCntRot > ROTSTATE_0; nCntRot--)
		{ // ������0�x�ɂȂ�܂ŌJ��Ԃ�

			//������Ԃ�ύX
			g_EditCollision.stateRot = (ROTSTATE)((g_EditCollision.stateRot + (ROTSTATE_MAX - 1)) % ROTSTATE_MAX);

			//�ʒu�x�N�g����90�x��]
			g_EditCollision.collision.vecPos = D3DXVECTOR3(g_EditCollision.collision.vecPos.z, g_EditCollision.collision.vecPos.y, -g_EditCollision.collision.vecPos.x);
		}

		//�ʒu�𔽉f (�x�N�g���̕���)
		g_EditCollision.pos = pEditObject->pos - g_EditCollision.collision.vecPos;
	}
	if (GetKeyboardTrigger(DIK_3) == true)
	{//3�L�[���������ꍇ
		//�g�嗦������������
		g_EditCollision.collision.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}
}

//=======================================
//�I�u�W�F�N�g�̏㉺�ړ�����
//=======================================
void CollisionUpDownEdit(void)
{
	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{//��SHIFT�L�[�������Ă���ꍇ
		if (GetKeyboardPress(DIK_LCONTROL) == true)
		{//���R���g���[���L�[�������Ă����ꍇ
			if (GetKeyboardTrigger(DIK_W) == true)
			{//W�L�[�������Ă���ꍇ
				//�ʒu�����ɐi�߂�
				g_EditCollision.pos.y += 2.0f;
			}

			if (GetKeyboardTrigger(DIK_S) == true)
			{//S�L�[�������Ă���ꍇ
				//�ʒu����O�ɐi�߂�
				g_EditCollision.pos.y -= 2.0f;
			}
		}
		else
		{//���R���g���[���L�[�������Ă��Ȃ��ꍇ
			if (GetKeyboardPress(DIK_W) == true)
			{//W�L�[�������Ă���ꍇ
			 //�ʒu�����ɐi�߂�
				g_EditCollision.pos.y += 6.0f;
			}

			if (GetKeyboardPress(DIK_S) == true)
			{//S�L�[�������Ă���ꍇ
			 //�ʒu����O�ɐi�߂�
				g_EditCollision.pos.y -= 6.0f;
			}
		}

		if (GetKeyboardTrigger(DIK_A) == true || GetKeyboardTrigger(DIK_D) == true)
		{//A�L�[��D�L�[���������ꍇ
			//�n�ʂɖ߂�
			g_EditCollision.pos.y = 0.0f;
		}
	}
}
#endif