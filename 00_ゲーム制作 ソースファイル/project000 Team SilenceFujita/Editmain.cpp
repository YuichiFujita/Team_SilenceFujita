#ifdef _DEBUG	// �f�o�b�O����
//========================================
//
//�G�f�B�b�g���C���̃��C������[Editmain.cpp]
//Author ��������
//
//========================================
#include "main.h"
#include "game.h"
#include "EditObject.h"
#include "EditCollision.h"
#include "EditBillboard.h"
#include "Editmain.h"
#include "SoundDJ.h"
#include "input.h"
#include "object.h"

//�}�N����`
#define SAVE_STAGE_TXT		"data\\TXT\\save_stage.txt"			// �X�e�[�W�ۑ��̊O���t�@�C���̑��΃p�X
#define SAVE_COLLISION_TXT	"data\\TXT\\save_collision.txt"		// �����蔻��ۑ��̊O���t�@�C���̑��΃p�X

//�v���g�^�C�v�錾
void DrawDebugEditObject(void);			//�G�f�B�b�g�I�u�W�F�N�g���[�h�̃f�o�b�O�\��
void DrawDebugEditBillboard(void);		//�G�f�B�b�g�r���{�[�h���[�h�̃f�o�b�O�\��
void DrawDebugControlObject(void);		//�G�f�B�b�g�I�u�W�F�N�g�������
void DrawDebugControlBillboard(void);	//�G�f�B�b�g�r���{�[�h�������

//�O���[�o���ϐ�
int g_EditStyle;						//�X�^�C��
int g_CollisionStyle;					//�����蔻��X�^�C��

//======================
//������
//======================
void InitEditmain()
{
	//�G�f�B�b�g�I�u�W�F�N�g�̏���������
	InitEditObject();

	//�G�f�B�b�g�����蔻��̏���������
	InitEditCollision();

	//�G�f�B�b�g�r���{�[�h�̏���������
	InitEditBillboard();

	//�G�f�B�b�g�I�u�W�F�N�g�X�^�C���ɂ���
	g_EditStyle = EDITSTYLE_OBJECT;

	//�����蔻��X�^�C��
	g_CollisionStyle = COLLISIONSTYLE_OBJECT;
}

//============================
//�I��
//============================
void UninitEditmain(void)
{
	//�G�f�B�b�g�I�u�W�F�N�g�̏I������
	UninitEditObject();

	//�G�f�B�b�g�����蔻��̏I������
	UninitEditCollision();

	//�G�f�B�b�g�r���{�[�h�̏I������
	UninitEditBillboard();
}

//==========================
//�X�V����
//==========================
void UpdateEditmain(void)
{
	//�X�^�C���ύX����
	StyleChange();

	//�����蔻��X�^�C���ύX����
	CollisionStyleChange();

	if (g_EditStyle == EDITSTYLE_OBJECT)
	{//�I�u�W�F�N�g�G�f�B�b�g���[�h�̏ꍇ
		//�G�f�B�b�g�I�u�W�F�N�g�̍X�V����
		UpdateEditObject();

		//�G�f�B�b�g�����蔻��̍X�V����
		UpdateEditCollision();
	}
	else if (g_EditStyle == EDITSTYLE_BILLBOARD)
	{//�r���{�[�h�G�f�B�b�g���[�h�̏ꍇ
		//�G�f�B�b�g�r���{�[�h�̍X�V����
		UpdateEditBillboard();
	}
}

//============================
//�Q�[����ʂ̕`�揈��
//============================
void DrawEditmain(void)
{
	if (g_EditStyle == EDITSTYLE_OBJECT)
	{//�I�u�W�F�N�g�G�f�B�b�g���[�h�̏ꍇ
		//�G�f�B�b�g�I�u�W�F�N�g�̕`�揈��
		DrawEditObject();

		//�G�f�B�b�g�����蔻��̕`�揈��
		DrawEditCollision();
	}
	else if (g_EditStyle == EDITSTYLE_BILLBOARD)
	{//�r���{�[�h�G�f�B�b�g���[�h�̏ꍇ
		//�G�f�B�b�g�r���{�[�h�̕`�揈��
		DrawEditBillboard();
	}
}

//=======================================
//�X�^�C���ύX����
//=======================================
void StyleChange(void)
{
	D3DXVECTOR3 *pEditBillpos = &GetEditBillboard()->pos;		//�G�f�B�b�g�r���{�[�h�̏����擾����
	D3DXVECTOR3 *pEditObjepos = &GetEditObject()->pos;			//�G�f�B�b�g�I�u�W�F�N�g�̏����擾����

	if (GetKeyboardTrigger(DIK_6) == true)
	{//6�L�[���������ꍇ
		//�X�^�C����I������
		g_EditStyle = (g_EditStyle + 1) % EDITSTYLE_MAX;

		if (g_EditStyle == EDITSTYLE_BILLBOARD)
		{//�r���{�[�h���[�h�������ꍇ
			//�I�u�W�F�N�g���������ʒu��������
			*pEditBillpos = *pEditObjepos;
		}
		else if (g_EditStyle == EDITSTYLE_OBJECT)
		{//�I�u�W�F�N�g���[�h�������ꍇ
			//�r���{�[�h���������ʒu��������
			*pEditObjepos = *pEditBillpos;
		}
	}
}

//=======================================
//�����蔻��X�^�C���ύX����
//=======================================
void CollisionStyleChange(void)
{
	if (GetEditObject()->Collisiontype.Collisiontype == COLLISIONTYPE_CREATE
	/*||  GetEditObject()->Collisiontype.Collisiontype == COLLISIONTYPE_ONLY*/)
	{//�����蔻��̎�ނ��쐬�֘A�̎�ނ̏ꍇ
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//Enter�L�[���������ꍇ
			//�X�^�C����I������
			g_CollisionStyle = (g_CollisionStyle + 1) % EDITSTYLE_MAX;
		}
	}
}

//=======================================
//�X�^�C���̎擾����
//=======================================
int GetStyle(void)
{
	//�G�f�B�b�g�X�^�C����Ԃ�
	return g_EditStyle;
}

//=======================================
//�����蔻��X�^�C���̎擾����
//=======================================
int GetCollisionStyle(void)
{
	//�����蔻��X�^�C����Ԃ�
	return g_CollisionStyle;
}

//=======================================
//	�X�e�[�W�̕ۑ�����
//=======================================
void TxtSaveStage(void)
{
	// �|�C���^��錾
	FILE      *pFile;							// �t�@�C���|�C���^
	Object    *pObject    = GetObjectData();	// �I�u�W�F�N�g�̏��|�C���^
	Billboard *pBillboard = GetBillboard();		// �r���{�[�h�̏��|�C���^

	// �t�@�C���������o�������ŊJ��
	pFile = fopen(SAVE_STAGE_TXT, "w");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ
		
		// ���o��
		fprintf(pFile, "#===========================================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#	�G�f�B�b�g�̕ۑ��X�e�[�W [save_stage.txt]\n");
		fprintf(pFile, "#	Author�F���� ���� & ���c �E�� & you\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#===========================================================\n");
		fprintf(pFile, "<>****<> �������牺���R�s�[�� [stage.txt]�ɒ���t�� <>****<>\n");
		fprintf(pFile, "\n");

		// �I�u�W�F�N�g�̏����o��
		fprintf(pFile, "#===========================================================\n");
		fprintf(pFile, "#	�I�u�W�F�N�g�̐ݒ�\n");
		fprintf(pFile, "#===========================================================\n");
		fprintf(pFile, "#-----------------------------------------------------------\n");
		fprintf(pFile, "#	 BREAKTYPE�F0  ���Ȃ�\n");
		fprintf(pFile, "#	          �F1  ����\n");
		fprintf(pFile, "#-----------------------------------------------------------\n");
		fprintf(pFile, "#	SHADOWTYPE�F0  �e����\n");
		fprintf(pFile, "#	          �F1  �ۉe\n");
		fprintf(pFile, "#	          �F2  ���f���e\n");
		fprintf(pFile, "#-----------------------------------------------------------\n");
		fprintf(pFile, "#	  COLLTYPE�F0  ������Ȃ�\n");
		fprintf(pFile, "#	          �F1  ������\n");
		fprintf(pFile, "#-----------------------------------------------------------\n");

		// �I�u�W�F�N�g�̐ݒ�̊J�n�n�_���e�L�X�g�ɏ����o��
		fprintf(pFile, "SETSTAGE_OBJECT\n\n");

		for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pObject++)
		{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

			if (pObject->bUse == true)
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

				// �I�u�W�F�N�g�̏��̊J�n�n�_�e�L�X�g�ɏ����o��
				fprintf(pFile, "	SET_OBJECT\n");

				// ��{���̏����o��
				fprintf(pFile, "		POS        = %.1f %.1f %.1f\n", pObject->pos.x,   pObject->pos.y,   pObject->pos.z);	// �ʒu
				fprintf(pFile, "		ROT        = %.1f %.1f %.1f\n", pObject->rot.x,   pObject->rot.y,   pObject->rot.z);	// ����
				fprintf(pFile, "		SCALE      = %.1f %.1f %.1f\n", pObject->scale.x, pObject->scale.y, pObject->scale.z);	// �g�嗦
				fprintf(pFile, "		TYPE       = %d\n",   pObject->nType);			// �I�u�W�F�N�g�̎��
				fprintf(pFile, "		BREAKTYPE  = %d\n",   pObject->nBreakType);		// �����̎��
				fprintf(pFile, "		SHADOWTYPE = %d\n",   pObject->nShadowType);	// �e�̎��
				fprintf(pFile, "		COLLTYPE   = %d\n\n", pObject->nCollisionType);	// �����蔻��̎��

				// �ʒu�x�N�g���̏����o��
				if (pObject->nCollisionType == COLLISIONTYPE_CREATE)
				{ // �����蔻��̎�ނ��쐬�̏ꍇ

					// �ʒu�x�N�g���������o��
					fprintf(pFile, "		VECPOS     = %.1f %.1f %.1f\n", pObject->collInfo.vecPos.x, pObject->collInfo.vecPos.y, pObject->collInfo.vecPos.z);
				}
				else
				{ //�@����ȊO�̎�ނ̏ꍇ

					// 0.0f�������o��
					fprintf(pFile, "		VECPOS     = 0.0f 0.0f 0.0f\n");
				}

				// �}�e���A�����̏����o��
				fprintf(pFile, "		NUMMAT     = %d\n", pObject->modelData.dwNumMat);

				// �}�e���A�����̏����o��
				for (int nCntMat = 0; nCntMat < (int)pObject->modelData.dwNumMat; nCntMat++)
				{ // �}�e���A���̐����J��Ԃ�

					// �g�U���̏����o��
					fprintf(pFile, "		%02d_MAT DIF = %.2f %.2f %.2f %.2f\n", nCntMat,
							pObject->matCopy[nCntMat].MatD3D.Diffuse.r, pObject->matCopy[nCntMat].MatD3D.Diffuse.g,
							pObject->matCopy[nCntMat].MatD3D.Diffuse.b, pObject->matCopy[nCntMat].MatD3D.Diffuse.a);

					// �����̏����o��
					fprintf(pFile, "		%02d_MAT AMB = %.2f %.2f %.2f %.2f\n", nCntMat,
							pObject->matCopy[nCntMat].MatD3D.Ambient.r, pObject->matCopy[nCntMat].MatD3D.Ambient.g,
							pObject->matCopy[nCntMat].MatD3D.Ambient.b, pObject->matCopy[nCntMat].MatD3D.Ambient.a);
				}

				// �I�u�W�F�N�g�̏��̏I���n�_�e�L�X�g�ɏ����o��
				fprintf(pFile, "	END_SET_OBJECT\n\n");
			}
		}

		// �I�u�W�F�N�g�̐ݒ�̏I���n�_���e�L�X�g�ɏ����o��
		fprintf(pFile, "END_SETSTAGE_OBJECT\n\n");

		// �r���{�[�h�̏����o��
		fprintf(pFile, "#===========================================================\n");
		fprintf(pFile, "#	�r���{�[�h�̐ݒ�\n");
		fprintf(pFile, "#===========================================================\n");
		fprintf(pFile, "#-----------------------------------------------------------\n");
		fprintf(pFile, "#	 ANIMATION�F0  �A�j���[�V���� OFF\n");
		fprintf(pFile, "#	          �F1  �A�j���[�V���� ON\n");
		fprintf(pFile, "#-----------------------------------------------------------\n");
		fprintf(pFile, "#	SHADOWTYPE�F0  �ۉe\n");
		fprintf(pFile, "#	          �F1  �e����\n");
		fprintf(pFile, "#-----------------------------------------------------------\n");

		// �r���{�[�h�̐ݒ�̊J�n�n�_���e�L�X�g�ɏ����o��
		fprintf(pFile, "SETSTAGE_BILLBOARD\n\n");

		for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++, pBillboard++)
		{ // �r���{�[�h�̍ő�\�������J��Ԃ�

			if (pBillboard->bUse == true)
			{ // �r���{�[�h���g�p����Ă���ꍇ

				// �r���{�[�h�̏��̊J�n�n�_�e�L�X�g�ɏ����o��
				fprintf(pFile, "	SET_BILLBOARD\n");

				// ��{���̏����o��
				fprintf(pFile, "		POS        = %.1f %.1f %.1f\n",      pBillboard->pos.x, pBillboard->pos.y, pBillboard->pos.z);						// �ʒu
				fprintf(pFile, "		ROT        = %.1f %.1f %.1f\n",      pBillboard->rot.x, pBillboard->rot.y, pBillboard->rot.z);						// ����
				fprintf(pFile, "		COL        = %.1f %.1f %.1f %.1f\n", pBillboard->col.r, pBillboard->col.g, pBillboard->col.b, pBillboard->col.a);	// �F
				fprintf(pFile, "		RADIUS     = %.1f %.1f\n",           pBillboard->Radius.x, pBillboard->Radius.y);	// ���a
				fprintf(pFile, "		TYPE       = %d\n",                  pBillboard->Type);								// ���
				fprintf(pFile, "		SHADOWTYPE = %d\n\n",                pBillboard->bShadow);							// �e�̎��

				// �A�j���[�V�������̏����o��
				fprintf(pFile, "		ANIMATION  = %d\n", pBillboard->Anim.bAnim);		// �A�j���[�V������ ON / OFF
				fprintf(pFile, "		ANIMCNT    = %d\n", pBillboard->Anim.nAnimCounter);	// �Đ��J�E���g
				fprintf(pFile, "		ANIMPAT    = %d\n", pBillboard->Anim.nAnimPattern);	// �Đ��p�^�[��

				// �r���{�[�h�̏��̏I���n�_�e�L�X�g�ɏ����o��
				fprintf(pFile, "	END_SET_BILLBOARD\n\n");
			}
		}

		// �r���{�[�h�̐ݒ�̏I���n�_���e�L�X�g�ɏ����o��
		fprintf(pFile, "END_SETSTAGE_BILLBOARD\n\n");

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�X�e�[�W�ۑ��t�@�C���̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}

//=======================================
//	�����蔻��̕ۑ�����
//=======================================
void TxtSaveCollision(void)
{
	// �|�C���^��錾
	FILE      *pFile;						// �t�@�C���|�C���^
	Collision *pCollision = GetCollision();	// �����蔻��̏��|�C���^

	// �t�@�C���������o�������ŊJ��
	pFile = fopen(SAVE_COLLISION_TXT, "w");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ
		
		// ���o��
		fprintf(pFile, "#===========================================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#	�����蔻��̕ۑ� [save_collision.txt]\n");
		fprintf(pFile, "#	Author�F���c �E�� & you\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#===========================================================\n");
		fprintf(pFile, "<>**<> �������牺���R�s�[�� [collision.txt]�ɒ���t�� <>**<>\n");
		fprintf(pFile, "\n");

		// �����蔻��̐ݒ�̊J�n�n�_���e�L�X�g�ɏ����o��
		fprintf(pFile, "SETCOLL_OBJECT\n\n");

		for (int nCntCollision = 0; nCntCollision < MODEL_OBJ_MAX; nCntCollision++, pCollision++)
		{ // �I�u�W�F�N�g�̎�ނ̑������J��Ԃ�

			// �����蔻��̏��̊J�n�n�_�e�L�X�g�ɏ����o��
			fprintf(pFile, "	SET_COLLISION\n");

			fprintf(pFile, "		TYPE   = %d\n", nCntCollision);																	// ���
			fprintf(pFile, "		VECPOS = %.1f %.1f %.1f\n", pCollision->vecPos.x, pCollision->vecPos.y, pCollision->vecPos.z);	// �ʒu�x�N�g��
			fprintf(pFile, "		SCALE  = %.1f %.1f %.1f\n", pCollision->scale.x, pCollision->scale.y, pCollision->scale.z);		// �g�嗦
			fprintf(pFile, "		WIDTH  = %.1f\n", pCollision->fWidth);	// ����
			fprintf(pFile, "		DEPTH  = %.1f\n", pCollision->fDepth);	// ���s

			// �����蔻��̏��̏I���n�_�e�L�X�g�ɏ����o��
			fprintf(pFile, "	END_SET_COLLISION\n\n");
		}

		// �����蔻��̐ݒ�̏I���n�_���e�L�X�g�ɏ����o��
		fprintf(pFile, "END_SETCOLL_OBJECT\n\n");

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�����蔻��ۑ��t�@�C���̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
#endif