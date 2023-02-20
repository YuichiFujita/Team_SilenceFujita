//======================================================================================================================
//
//	�I�u�W�F�N�g���� [object.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "calculation.h"
#include "input.h"
#include "model.h"

#include "buildtimer.h"
#include "junk.h"
#include "object.h"
#include "particle.h"
#include "shadow.h"
#include "sound.h"

#ifdef _DEBUG	// �f�o�b�O����
#include "game.h"
#endif

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define OBJ_LIFE		(50)					// �I�u�W�F�N�g�̗̑�
#define DAMAGE_TIME_OBJ	(20)					// �_���[�W��Ԃ�ۂ���
#define UNR_TIME_OBJ	(DAMAGE_TIME_OBJ - 10)	// ���G��ԂɕύX���鎞��

#define OBJECT_GRAVITY	(-1.5f)					// �I�u�W�F�N�g�̏d��
#define SMASH_WIDTH		(10.0f)					// ������т̕�
#define SMASH_HEIGHT	(20.0f)					// ������т̍���
#define SMASH_DEPTH		(10.0f)					// ������т̉��s
#define SMASH_ANGLE		(0.3f)					// ������ԍ����̊p�x
#define SMASH_ANGLE_ADD	(0.3f)					// ������Ԋp�x�̒ǉ���
#define SMASH_ADD_ROT	(0.02f)					// ������т̉�]�̉��Z��
#define SMASH_DEATH_CNT	(40)					// ������񂾌�ɏ�����܂ł̃J�E���g
#define SMASH_ROTMOVE_X	(0.01f)					// ������Ԋp�x�̈ړ���(X��)
#define SMASH_ROTMOVE_Y	(0.2f)					// ������Ԋp�x�̈ړ���(Y��)
#define SMASH_ROTMOVE_Z	(0.01f)					// ������Ԋp�x�̈ړ���(Z��)

#define APPEAR_ADD_MAGNI	(0.05f)				// �o�����̉��Z���̔{��

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
Object    g_aObject[MAX_OBJECT];				// �I�u�W�F�N�g�̏��
Collision g_aCollision[MODEL_OBJ_MAX];			// �����蔻��̏��
float     g_aShadowRadius[MODEL_OBJ_MAX];		// �e�̔��a�̏��

//======================================================================================================================
//	�I�u�W�F�N�g�̏���������
//======================================================================================================================
void InitObject(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// �I�u�W�F�N�g�̏��̏�����
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		// ��{���̏�����
		g_aObject[nCntObject].pos            = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_aObject[nCntObject].rot            = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
		g_aObject[nCntObject].scale          = D3DXVECTOR3(1.0f, 1.0f, 1.0f);		// �g�嗦
		g_aObject[nCntObject].state          = ACTIONSTATE_NONE;					// ���
		g_aObject[nCntObject].nLife          = 0;									// �̗�
		g_aObject[nCntObject].nCollisionType = COLLISIONTYPE_NONE;					// �����蔻��̎��
		g_aObject[nCntObject].nShadowType    = SHADOWTYPE_NONE;						// �e�̎��
		g_aObject[nCntObject].nBreakType     = BREAKTYPE_NONE;						// �����̎��
		g_aObject[nCntObject].nType          = 0;									// �I�u�W�F�N�g�̎��
		g_aObject[nCntObject].nCounterState  = 0; 									// ��ԊǗ��J�E���^�[
		g_aObject[nCntObject].nShadowID      = NONE_SHADOW;							// �e�̃C���f�b�N�X
		g_aObject[nCntObject].bUse           = false;								// �g�p��

		// �����蔻����̏�����
		for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
		{ // �����蔻��̍ő吔���J��Ԃ�

			g_aObject[nCntObject].collInfo.vecPos[nCntColl] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu�x�N�g��
			g_aObject[nCntObject].collInfo.fWidth[nCntColl] = 0.0f;								// ����
			g_aObject[nCntObject].collInfo.fDepth[nCntColl] = 0.0f;								// ���s
		}

		//������ъ֌W�̏�����
		g_aObject[nCntObject].smash.nCounter = 0;									// �J�E���^�[
		g_aObject[nCntObject].smash.State = SMASHSTATE_NONE;						// ���
		g_aObject[nCntObject].smash.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
		g_aObject[nCntObject].smash.bJump = false;									// �W�����v�̏��
		g_aObject[nCntObject].smash.nSmashCount = 0;								// �J�E���g
		g_aObject[nCntObject].smash.rotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����̈ړ���

		// �W���b�W�̏��̏�����
		g_aObject[nCntObject].judge.col = JUDGE_WHITE;								// �s�J�s�J�̐F
		g_aObject[nCntObject].judge.state = JUDGESTATE_JUSTICE;						// �P��
		g_aObject[nCntObject].judge.ticatica = CHICASTATE_BLACKOUT;					// �`�J�`�J���
			
		// ������Ԃ�������
		g_aObject[nCntObject].collInfo.stateRot = ROTSTATE_0;

		// ���f�����̏�����
		g_aObject[nCntObject].modelData.dwNumMat = 0;								// �}�e���A���̐�
		g_aObject[nCntObject].modelData.pTexture = NULL;							// �e�N�X�`���ւ̃|�C���^
		g_aObject[nCntObject].modelData.pMesh    = NULL;							// ���b�V�� (���_���) �ւ̃|�C���^
		g_aObject[nCntObject].modelData.pBuffMat = NULL;							// �}�e���A���ւ̃|�C���^
		g_aObject[nCntObject].modelData.vtxMin   = INIT_VTX_MIN;					// �ŏ��̒��_���W
		g_aObject[nCntObject].modelData.vtxMax   = INIT_VTX_MAX;					// �ő�̒��_���W
		g_aObject[nCntObject].modelData.size     = INIT_SIZE;						// �傫��
		g_aObject[nCntObject].modelData.fRadius  = 0.0f;							// ���a

		// �}�e���A���̃R�s�[��������
		g_aObject[nCntObject].matCopy[MAX_MATERIAL] = {};

		// �o���֌W�̏�����
		g_aObject[nCntObject].appear.scaleCopy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �g�嗦
		g_aObject[nCntObject].appear.state = APPEARSTATE_NONE;						// �o�����
		g_aObject[nCntObject].appear.scaleAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �g�嗦�̉��Z��
		g_aObject[nCntObject].appear.fAlpha = 0.0f;									// �����x

#ifdef _DEBUG	// �f�o�b�O����
		// �G�f�B�b�g���̏��
		g_aObject[nCntObject].editState = OBJECTSTATE_NONE;
#endif
	}

	// �����蔻��̏��̏�����
	for (int nCntObject = 0; nCntObject < MODEL_OBJ_MAX; nCntObject++)
	{ // �I�u�W�F�N�g�̎�ނ̑������J��Ԃ�

		// �����蔻��̑�����������
		g_aCollision[nCntObject].nNumColl = 1;

		for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
		{ // �����蔻��̍ő吔���J��Ԃ�

			g_aCollision[nCntObject].vecPos[nCntColl] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu�x�N�g��
			g_aCollision[nCntObject].scale[nCntColl]  = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// �g�嗦
			g_aCollision[nCntObject].fWidth[nCntColl] = 0.0f;							// ����
			g_aCollision[nCntObject].fDepth[nCntColl] = 0.0f;							// ���s
		}
	}

	// �e�̔��a�̏��̏�����
	for (int nCntObject = 0; nCntObject < MODEL_OBJ_MAX; nCntObject++)
	{ // �I�u�W�F�N�g�̎�ނ̑������J��Ԃ�

		g_aShadowRadius[nCntObject] = FIRST_RADIUS;
	}

	//// �����蔻��̃Z�b�g�A�b�v
	//TxtSetCollision();

	//// �e�̔��a�̃Z�b�g�A�b�v
	//TxtSetShadow();
}

//======================================================================================================================
//	�I�u�W�F�N�g�̏I������
//======================================================================================================================
void UninitObject(void)
{

}

//======================================================================================================================
//	�I�u�W�F�N�g�̍X�V����
//======================================================================================================================
void UpdateObject(void)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aObject[nCntObject].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			switch (g_aObject[nCntObject].appear.state)
			{
			case APPEARSTATE_NONE:		// ��u�ŏo��

				// �o��������Ԃɂ���
				g_aObject[nCntObject].appear.state = APPEARSTATE_COMPLETE;

				break;					// �����o��

			case APPEARSTATE_SLOWLY:	// ���X�ɏo��

				// �g�嗦���グ�Ă���
				g_aObject[nCntObject].scale += g_aObject[nCntObject].appear.scaleAdd;

				// �����x���グ�Ă���
				g_aObject[nCntObject].appear.fAlpha += APPEAR_ADD_MAGNI;

				if (g_aObject[nCntObject].scale.x >= g_aObject[nCntObject].appear.scaleCopy.x)
				{ // �g�嗦���R�s�[�ȏ�ɂȂ����ꍇ

					// �g�嗦��␳����
					g_aObject[nCntObject].scale = g_aObject[nCntObject].appear.scaleCopy;

					// �o�������ɂ���
					g_aObject[nCntObject].appear.state = APPEARSTATE_COMPLETE;

					// �����x��ݒ肷��
					g_aObject[nCntObject].appear.fAlpha = 1.0f;
				}

				break;					// �����o��

			case APPEARSTATE_COMPLETE:	// �o������

				if (g_aObject[nCntObject].judge.state == JUDGESTATE_EVIL)
				{ // ���҂������ꍇ

					// �W���b�W�̍X�V����
					UpdateJudge(&g_aObject[nCntObject].judge);
				}

				switch (g_aObject[nCntObject].nBreakType)
				{ // �����̎�ނ��Ƃ̏���
				case BREAKTYPE_NONE:	// ���Ȃ����

					// ����

					// �����𔲂���
					break;

				case BREAKTYPE_ON:		// ������

					if (g_aObject[nCntObject].nCounterState > 0)
					{ // �J�E���^�[�� 0���傫���ꍇ

						// �J�E���^�[�����Z
						g_aObject[nCntObject].nCounterState--;

						if (g_aObject[nCntObject].nCounterState == UNR_TIME_OBJ)
						{ // �J�E���^�[�����l�̏ꍇ

							// ���G��Ԃɂ���
							g_aObject[nCntObject].state = ACTIONSTATE_UNRIVALED;
						}
						else if (g_aObject[nCntObject].nCounterState <= 0)
						{ // �J�E���^�[�� 0�ȉ��̏ꍇ

							// �ʏ��Ԃɂ���
							g_aObject[nCntObject].state = ACTIONSTATE_NORMAL;
						}
					}

					// �����𔲂���
					break;

				case BREAKTYPE_SMASH:		// ������Ԏ��

					switch (g_aObject[nCntObject].smash.State)
					{
					case SMASHSTATE_NONE:	// �������

						break;				// �����o��

					case SMASHSTATE_ON:		// �������ł�����

						// �d�͂�������
						g_aObject[nCntObject].smash.move.y += OBJECT_GRAVITY;

						// �ړ��ʂ𑫂�
						g_aObject[nCntObject].pos += g_aObject[nCntObject].smash.move;

						// �J�E���g�����Z����
						g_aObject[nCntObject].smash.nCounter++;

						g_aObject[nCntObject].rot.x += g_aObject[nCntObject].smash.rotMove.x + 0.02f;
						g_aObject[nCntObject].rot.y -= g_aObject[nCntObject].smash.rotMove.y + 0.02f;
						g_aObject[nCntObject].rot.z -= g_aObject[nCntObject].smash.rotMove.z + 0.02f;

						if (g_aObject[nCntObject].smash.nCounter >= SMASH_DEATH_CNT)
						{ // �J�E���g����莞�Ԍo������
							// �g�p���Ȃ�
							g_aObject[nCntObject].bUse = false;
						}

						// �����̐��K��(X��)
						if (g_aObject[nCntObject].rot.x >= D3DX_PI * 0.5f)
						{
							g_aObject[nCntObject].rot.x = D3DX_PI * 0.5f;

							g_aObject[nCntObject].smash.rotMove.z = 0.0f;
						}
						else if (g_aObject[nCntObject].rot.x <= (-D3DX_PI * 0.5f))
						{
							g_aObject[nCntObject].rot.x = -(D3DX_PI * 0.5f);

							g_aObject[nCntObject].smash.rotMove.z = 0.0f;
						}

						// �����̐��K��(Y��)
						if (g_aObject[nCntObject].rot.y >= D3DX_PI * 0.5f)
						{
							g_aObject[nCntObject].rot.y = D3DX_PI * 0.5f;

							g_aObject[nCntObject].smash.rotMove.y = 0.0f;
						}
						else if (g_aObject[nCntObject].rot.y <= (-D3DX_PI * 0.5f))
						{
							g_aObject[nCntObject].rot.y = -(D3DX_PI * 0.5f);

							g_aObject[nCntObject].smash.rotMove.y = 0.0f;
						}

						// �����̐��K��(Z��)
						if (g_aObject[nCntObject].rot.z >= D3DX_PI * 0.5f)
						{
							g_aObject[nCntObject].rot.z = D3DX_PI * 0.5f;

							g_aObject[nCntObject].smash.rotMove.x = 0.0f;
						}
						else if (g_aObject[nCntObject].rot.z <= (-D3DX_PI * 0.5f))
						{
							g_aObject[nCntObject].rot.z = -(D3DX_PI * 0.5f);

							g_aObject[nCntObject].smash.rotMove.x = 0.0f;
						}

						if (g_aObject[nCntObject].pos.y <= 0.0f)
						{ // �n�ʂ������ɍs������

							// �ړ��ʂ�0�ɂ���
							g_aObject[nCntObject].smash.move.x = 0.0f;
							g_aObject[nCntObject].smash.move.z = 0.0f;
						}

						break;				// �����o��(�������ł�����)
					}

					// �n�ʂɐڂ��Ă��邩
					LandObject(&g_aObject[nCntObject].pos, &g_aObject[nCntObject].smash.move, &g_aObject[nCntObject].smash.bJump);

					if (g_aObject[nCntObject].pos.y <= 0.0f)
					{ // �n�ʂ������ɍs������

						// �ʒu��0�ɂ���
						g_aObject[nCntObject].pos.y = 0.0f;
					}

					// �����𔲂���
					break;
				}

				break;					// �����o��(�o������)
			}
		}
	}
}

//======================================================================================================================
//	�I�u�W�F�N�g�̕`�揈��
//======================================================================================================================
void DrawObject(void)
{
	// �ϐ���錾
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL      blackMat;					// �}�e���A���f�[�^ (���{��)

#ifdef _DEBUG	// �f�o�b�O����
	D3DXMATERIAL deleteMat;						// �폜�}�e���A���f�[�^ (��)
#endif

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aObject[nCntObject].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aObject[nCntObject].mtxWorld);

			// �g�嗦�𔽉f
			D3DXMatrixScaling(&mtxScale, g_aObject[nCntObject].scale.x, g_aObject[nCntObject].scale.y, g_aObject[nCntObject].scale.z);
			D3DXMatrixMultiply(&g_aObject[nCntObject].mtxWorld, &g_aObject[nCntObject].mtxWorld, &mtxScale);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aObject[nCntObject].rot.y, g_aObject[nCntObject].rot.x, g_aObject[nCntObject].rot.z);
			D3DXMatrixMultiply(&g_aObject[nCntObject].mtxWorld, &g_aObject[nCntObject].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aObject[nCntObject].pos.x, g_aObject[nCntObject].pos.y, g_aObject[nCntObject].pos.z);
			D3DXMatrixMultiply(&g_aObject[nCntObject].mtxWorld, &g_aObject[nCntObject].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aObject[nCntObject].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = &g_aObject[nCntObject].matCopy[0];

			for (int nCntMat = 0; nCntMat < (int)g_aObject[nCntObject].modelData.dwNumMat; nCntMat++)
			{ // �}�e���A���̐����J��Ԃ�

				if (g_aObject[nCntObject].appear.state == APPEARSTATE_SLOWLY)
				{ // ���X�ɏo�����鎞

					// �����x��ݒ肷��
					pMat[nCntMat].MatD3D.Diffuse.a = g_aObject[nCntObject].appear.fAlpha;

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				}
				else
				{ // ��L�ȊO
					switch (g_aObject[nCntObject].state)
					{ // ��Ԃ��Ƃ̏���
					case ACTIONSTATE_DAMAGE:	// �_���[�W���

						// �ǂݍ��񂾃}�e���A������
						blackMat.MatD3D = pMat[nCntMat].MatD3D;

						// �g�U���E�����E���Ȕ�����Ԃɂ���
						blackMat.MatD3D.Diffuse = D3DXCOLOR(0.3f, 0.3f, 0.0f, 1.0f);
						blackMat.MatD3D.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.0f, 1.0f);
						blackMat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

						// �}�e���A���̐ݒ�
						pDevice->SetMaterial(&blackMat.MatD3D);			// ���{��

						// �����𔲂���
						break;

					default:					// ����ȊO�̏��

						// �}�e���A���̃R�s�[�ɑ������
						g_aObject[nCntObject].matCopy[nCntMat] = pMat[nCntMat];

						if (g_aObject[nCntObject].judge.state == JUDGESTATE_JUSTICE)
						{ // �ǂ��z�̏ꍇ
							// �}�e���A���̐ݒ�
							pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
						}
						else
						{ // �����z�̏ꍇ

							// ���Ȕ�����������
							g_aObject[nCntObject].matCopy[nCntMat].MatD3D.Emissive = g_aObject[nCntObject].judge.col;

							// �}�e���A���̐ݒ�
							pDevice->SetMaterial(&g_aObject[nCntObject].matCopy[nCntMat].MatD3D);
						}

						// �����𔲂���
						break;
					}
				}

#ifdef _DEBUG	// �f�o�b�O����
				if (GetGameMode() == GAMEMODE_EDIT)
				{ // �G�f�B�b�g���[�h�̏ꍇ

					switch (g_aObject[nCntObject].editState)
					{ // ��Ԃ��Ƃ̏���
					case OBJECTSTATE_DELETETARGET:	// �폜�Ώ�

						// �g�U���E�����E���Ȕ�����Ԃɂ���
						deleteMat.MatD3D.Diffuse  = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						deleteMat.MatD3D.Ambient  = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						deleteMat.MatD3D.Emissive = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						// �}�e���A���̐ݒ�
						pDevice->SetMaterial(&deleteMat.MatD3D);		// ��

						// �����𔲂���
						break;

					default:						// ����ȊO�̏��

						// �}�e���A���̐ݒ�
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// �ʏ�

						// �����𔲂���
						break;
					}
				}
#endif

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aObject[nCntObject].modelData.pTexture[nCntMat]);

				if (g_aObject[nCntObject].scale != NONE_SCALE)
				{ // �g�嗦���ύX����Ă���ꍇ

					// ���_�@���̎������K����L���ɂ���
					pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
				}

				// ���f���̕`��
				g_aObject[nCntObject].modelData.pMesh->DrawSubset(nCntMat);

				// ���_�@���̎������K���𖳌��ɂ���
				pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
			}

			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	�I�u�W�F�N�g�̐ݒ菈��
//======================================================================================================================
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXMATERIAL *pMat, int nType, int nBreakType, int nShadowType, int nCollisionType, ROTSTATE stateRot, APPEARSTATE appear)
{
	// �ϐ���錾
	float AverageScale;			// �g�嗦�̕��ϒl

	// �|�C���^��錾
	D3DXMATERIAL *pMatModel;	// �}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aObject[nCntObject].bUse == false)
		{ // �I�u�W�F�N�g���g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aObject[nCntObject].pos            = pos;					// �ʒu
			g_aObject[nCntObject].rot            = rot;					// ����
			g_aObject[nCntObject].scale          = scale;				// �g�嗦
			g_aObject[nCntObject].nType          = nType;				// �I�u�W�F�N�g�̎��
			g_aObject[nCntObject].nBreakType     = nBreakType;			// �����̎��
			g_aObject[nCntObject].nShadowType    = nShadowType;			// �e�̎��
			g_aObject[nCntObject].nCollisionType = nCollisionType;		// �����蔻��̎��

			// �o���֌W�̐ݒ�
			g_aObject[nCntObject].appear.scaleCopy = scale;							// �g�嗦�̃R�s�[
			g_aObject[nCntObject].appear.state	   = appear;						// �o�����
			g_aObject[nCntObject].appear.scaleAdd  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �g�嗦�̉��Z��
			g_aObject[nCntObject].appear.fAlpha	   = 1.0f;							// �����x

			if (g_aObject[nCntObject].appear.state == APPEARSTATE_SLOWLY)
			{ // ���X�ɏo�Ă����Ԃ������ꍇ

				// �g�嗦������������
				g_aObject[nCntObject].scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				// �g�嗦�̉��Z����ݒ肷��
				g_aObject[nCntObject].appear.scaleAdd.x = scale.x * APPEAR_ADD_MAGNI;
				g_aObject[nCntObject].appear.scaleAdd.y = scale.y * APPEAR_ADD_MAGNI;
				g_aObject[nCntObject].appear.scaleAdd.z = scale.z * APPEAR_ADD_MAGNI;

				// �����x��ݒ肷��
				g_aObject[nCntObject].appear.fAlpha = 0.0f;							
			}

			// �I�u�W�F�N�g�̏���������
			g_aObject[nCntObject].state         = ACTIONSTATE_NORMAL;	// ���
			g_aObject[nCntObject].nLife         = OBJ_LIFE;				// �̗�
			g_aObject[nCntObject].nCounterState = 0; 					// ��ԊǗ��J�E���^�[

			// �g�p���Ă����Ԃɂ���
			g_aObject[nCntObject].bUse = true;

			// �����蔻�����ݒ�
			g_aObject[nCntObject].collInfo.stateRot = stateRot;			// �������

			for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
			{ // �����蔻��̍ő吔���J��Ԃ�

				// �����Əc�����v�Z
				if (stateRot == ROTSTATE_0
				||  stateRot == ROTSTATE_180)
				{ // �p�x��0�x�A�܂���180�x�̏ꍇ
					g_aObject[nCntObject].collInfo.fWidth[nCntColl] = g_aCollision[nType].fWidth[nCntColl];
					g_aObject[nCntObject].collInfo.fDepth[nCntColl] = g_aCollision[nType].fDepth[nCntColl];
				}
				else
				{ // �p�x90�x�A�܂���270�x�̏ꍇ
					g_aObject[nCntObject].collInfo.fWidth[nCntColl] = g_aCollision[nType].fDepth[nCntColl];
					g_aObject[nCntObject].collInfo.fDepth[nCntColl] = g_aCollision[nType].fWidth[nCntColl];
				}
			}

			for (int nCntColl = 0; nCntColl < g_aCollision[g_aObject[nCntObject].nType].nNumColl; nCntColl++)
			{ // �����蔻��̑������J��Ԃ�

				// �ʒu�x�N�g����ݒ�
				g_aObject[nCntObject].collInfo.vecPos[nCntColl] = g_aCollision[nType].vecPos[nCntColl];

				for (int nCntRot = ROTSTATE_0; nCntRot < stateRot; nCntRot++)
				{ // �ݒ肳�ꂽ�����ɂȂ�܂ŌJ��Ԃ�

					// �ʒu�x�N�g����90�x��]
					g_aObject[nCntObject].collInfo.vecPos[nCntColl] = D3DXVECTOR3
					( // ����
						-g_aObject[nCntObject].collInfo.vecPos[nCntColl].z,
						 g_aObject[nCntObject].collInfo.vecPos[nCntColl].y,
						 g_aObject[nCntObject].collInfo.vecPos[nCntColl].x
					);
				}
			}

			// ������ъ֌W�̐ݒ�
			g_aObject[nCntObject].smash.nCounter = 0;									// �J�E���^�[
			g_aObject[nCntObject].smash.State = SMASHSTATE_NONE;						// ���
			g_aObject[nCntObject].smash.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
			g_aObject[nCntObject].smash.bJump = false;									// �W�����v�̏��
			g_aObject[nCntObject].smash.nSmashCount = 0;								// �J�E���g
			g_aObject[nCntObject].smash.rotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����̈ړ���

			// ���f������ݒ�
			g_aObject[nCntObject].modelData = GetModelData(nType + FROM_OBJECT);	// ���f�����

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMatModel = (D3DXMATERIAL*)g_aObject[nCntObject].modelData.pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aObject[nCntObject].modelData.dwNumMat; nCntMat++, pMatModel++)
			{ // �}�e���A���̐����J��Ԃ�

				// �}�e���A�����R�s�[����
				g_aObject[nCntObject].matCopy[nCntMat] = *pMatModel;
			}

			// �ő�l�𔽉f����
			g_aObject[nCntObject].modelData.vtxMax.x *= g_aObject[nCntObject].scale.x;
			g_aObject[nCntObject].modelData.vtxMax.y *= g_aObject[nCntObject].scale.y;
			g_aObject[nCntObject].modelData.vtxMax.z *= g_aObject[nCntObject].scale.z;

			// �ŏ��l�𔽉f����
			g_aObject[nCntObject].modelData.vtxMin.x *= g_aObject[nCntObject].scale.x;
			g_aObject[nCntObject].modelData.vtxMin.y *= g_aObject[nCntObject].scale.y;
			g_aObject[nCntObject].modelData.vtxMin.z *= g_aObject[nCntObject].scale.z;

			for (int nCntMat = 0; nCntMat < (int)g_aObject[nCntObject].modelData.dwNumMat; nCntMat++)
			{ // �}�e���A���̐����J��Ԃ�

				// �����̐F����
				g_aObject[nCntObject].matCopy[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Ambient;
				g_aObject[nCntObject].matCopy[nCntMat].MatD3D.Diffuse = pMat[nCntMat].MatD3D.Diffuse;
			}

			if (nShadowType == SHADOWTYPE_CIRCLE)
			{ // �ۉe�̏ꍇ

				// �g�嗦�̕��ς����߂�
				AverageScale = (g_aObject[nCntObject].scale.x + g_aObject[nCntObject].scale.z) * 0.5f;

				// �e�̃C���f�b�N�X��ݒ�
				g_aObject[nCntObject].nShadowID = SetCircleShadow
				( // ����
					0.5f,										// ���l
					g_aShadowRadius[nType] * AverageScale,		// ���a
					&g_aObject[nCntObject].nShadowID,			// �e�̐e�̉e�C���f�b�N�X
					&g_aObject[nCntObject].bUse					// �e�̐e�̎g�p��
				);
			}
			else if (nShadowType == SHADOWTYPE_MODEL)
			{ // ���f���e�̏ꍇ

				// �e�̃C���f�b�N�X��ݒ�
				g_aObject[nCntObject].nShadowID = SetModelShadow
				( // ����
					g_aObject[nCntObject].modelData,	// ���f�����
					&g_aObject[nCntObject].nShadowID,	// �e�̐e�̉e�C���f�b�N�X
					&g_aObject[nCntObject].bUse			// �e�̐e�̎g�p��
				);
			}
			else
			{ // �e�����̏ꍇ

				// �e�̃C���f�b�N�X��ݒ�
				g_aObject[nCntObject].nShadowID = NONE_SHADOW;	// �e��ݒ肵�Ȃ�
			}

			// �e�̈ʒu�ݒ�
			SetPositionShadow(g_aObject[nCntObject].nShadowID, g_aObject[nCntObject].pos, g_aObject[nCntObject].rot, g_aObject[nCntObject].scale);

			// �W���b�W�̏��̐ݒ�
			g_aObject[nCntObject].judge.col = JUDGE_WHITE;			// �s�J�s�J�̐F

			if (nCntObject % 2 == 0)
			{ // 2�̔{���������ꍇ

				g_aObject[nCntObject].judge.state = JUDGESTATE_EVIL;				// �P��
				g_aObject[nCntObject].judge.ticatica = CHICASTATE_BLACKOUT;			// �`�J�`�J���
			}
			else
			{ // ��L�ȊO

				g_aObject[nCntObject].judge.state = JUDGESTATE_JUSTICE;				// �P��
				g_aObject[nCntObject].judge.ticatica = CHICASTATE_BLACKOUT;			// �`�J�`�J���
			}

			// �����𔲂���
			break;
		}
	}
}

#if 1
//======================================================================================================================
//	�I�u�W�F�N�g�̃_���[�W����
//======================================================================================================================
void HitObject(Object *pObject, int nDamage)
{
	if (pObject->state == ACTIONSTATE_NORMAL && pObject->appear.state == APPEARSTATE_COMPLETE && pObject->bUse == true)
	{ // �I�u�W�F�N�g���ʏ��Ԃ��A�o��������Ԃ̏ꍇ

		// �����̃_���[�W����̗͂��猸�Z
		pObject->nLife -= nDamage;

		if (pObject->nLife > 0)
		{ // �̗͂��c���Ă���ꍇ

			// �_���[�W��Ԃɂ���
			pObject->state = ACTIONSTATE_DAMAGE;

			// �p�[�e�B�N���̐ݒ�
			SetParticle
			( // ����
				pObject->pos,						// �ʒu
				D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f),	// �F
				PARTICLETYPE_DAMAGE,				// ���
				SPAWN_PARTICLE_DAMAGE,				// �G�t�F�N�g��
				2									// ����
			);

			// �J�E���^�[��ݒ�
			pObject->nCounterState = DAMAGE_TIME_OBJ;

			//// �T�E���h�̍Đ�
			//PlaySound(SOUND_LABEL_SE_DMG);			// SE (�_���[�W)
		}
		else
		{ // �̗͂��s�����ꍇ

			//// �����̐ݒ�
			//SetExplosion(pObject->pos, SOUNDTYPE_BREAK);

			// �p�[�e�B�N���̐ݒ�
			SetParticle
			( // ����
				pObject->pos,						// �ʒu
				D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f),	// �F
				PARTICLETYPE_EXPLOSION,				// ���
				SPAWN_PARTICLE_EXPLOSION,			// �G�t�F�N�g��
				2									// ����
			);

			// �Č��z�^�C�}�[�̐ݒ菈��
			SetBuildtimer(pObject->pos, 600, *pObject);

			// �T�C�Y�ɉ����Ă��ꂫ�𐶂ݏo��
			for (int nCntColl = 0; nCntColl < g_aCollision[pObject->nType].nNumColl; nCntColl++)
			{
				// �ʒu
				D3DXVECTOR3 pos = pObject->pos + g_aCollision[pObject->nType].vecPos[nCntColl];

				// ���ꂫ�̐ݒ菈��
				SetJunk
				(
					D3DXVECTOR3(pos.x - g_aCollision[pObject->nType].fWidth[nCntColl], pos.y + (pObject->modelData.vtxMax.y * 0.5f), pos.z - g_aCollision[pObject->nType].fDepth[nCntColl]),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					(SCALETYPE)((nCntColl + 1) % SCALETYPE_MAX),
					g_aObject[pObject->nType].matCopy[nCntColl].MatD3D
				);

				SetJunk
				(
					D3DXVECTOR3(pos.x + g_aCollision[pObject->nType].fWidth[nCntColl], pos.y + (pObject->modelData.vtxMax.y * 0.5f), pos.z - g_aCollision[pObject->nType].fDepth[nCntColl]),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					(SCALETYPE)((nCntColl + 1) % SCALETYPE_MAX),
					g_aObject[pObject->nType].matCopy[nCntColl].MatD3D
				);

				SetJunk
				(
					D3DXVECTOR3(pos.x - g_aCollision[pObject->nType].fWidth[nCntColl], pos.y + (pObject->modelData.vtxMax.y * 0.5f), pos.z + g_aCollision[pObject->nType].fDepth[nCntColl]),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					(SCALETYPE)((nCntColl + 1) % SCALETYPE_MAX),
					g_aObject[pObject->nType].matCopy[nCntColl].MatD3D
				);

				SetJunk
				(
					D3DXVECTOR3(pos.x + g_aCollision[pObject->nType].fWidth[nCntColl], pos.y + (pObject->modelData.vtxMax.y * 0.5f), pos.z + g_aCollision[pObject->nType].fDepth[nCntColl]),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					(SCALETYPE)((nCntColl + 1) % SCALETYPE_MAX),
					g_aObject[pObject->nType].matCopy[nCntColl].MatD3D
				);
			}

			//// �A�C�e���̐ݒ�
			//SetItem(pObject->pos, ITEMTYPE_HEAL);

			// �g�p���Ă��Ȃ���Ԃɂ���
			pObject->bUse = false;
		}
	}
}
#endif

//======================================================================================================================
//	�I�u�W�F�N�g�Ƃ̓����蔻��
//======================================================================================================================
void CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pOldPos, D3DXVECTOR3 *pMove, float fWidth, float fDepth, int *pTraCnt, BOOSTSTATE state, POLICESTATE *pPolice)
{
	// �ϐ���錾
	D3DXVECTOR3 collPos;	// �����蔻��̒��S���W

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aObject[nCntObject].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			switch (g_aObject[nCntObject].nCollisionType)
			{ // �����蔻��̎�ނ��Ƃ̏���
			case COLLISIONTYPE_NONE:	// �����蔻�薳��

				// ����

				// �����𔲂���
				break;

			case COLLISIONTYPE_MODEL:	// ���f�����_�̓����蔻��

				// �O��̓����蔻��
				if (pPos->x + fWidth > g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMin.x
				&&  pPos->x - fWidth < g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMax.x)
				{ // �u���b�N�̍��E�͈͓̔��̏ꍇ

					if (pPos->z    + fDepth >  g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMin.z
					&&  pOldPos->z + fDepth <= g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMin.z)
					{ // �O����̓����蔻��

						// �ʒu��␳
						pPos->z = g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMin.z - fDepth - 0.01f;

						if (pMove->x >= 13.0f)
						{ // �ړ��ʂ����ȏ�̏ꍇ
							// �p�[�e�B�N���̐ݒ菈��
							SetParticle
							(
								D3DXVECTOR3(pPos->x, pPos->y + 50.0f, g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMin.z),	// �ʒu
								D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),					// �F
								PARTICLETYPE_SPARK,									// �p�[�e�B�N���̎��
								10,													// ������
								2													// ����
							);
						}

						if (state == BOOSTSTATE_UP)
						{ // �u�[�X�g���̏ꍇ

							if (g_aObject[nCntObject].nBreakType == BREAKTYPE_ON)
							{ // �����ނ̃I�u�W�F�N�g�������ꍇ

								// �I�u�W�F�N�g�̃_���[�W����
								HitObject(&g_aObject[nCntObject], OBJ_LIFE);
							}
							else
							{ // ��L�ȊO�̏ꍇ

								// �ړ��ʂ��폜
								pMove->x *= 0.95f;
							}
						}
						else
						{ // ��L�ȊO�̏ꍇ

							// �ړ��ʂ��폜
							pMove->x *= 0.95f;
						}

						if (*pPolice == POLICESTATE_TACKLE)
						{ // �^�b�N�����������ꍇ

							// �ǐՏ�Ԃɂ���
							*pPolice = POLICESTATE_CHASE;
						}

						// �a�؃J�E���g�����Z����
						*pTraCnt += 1;
					}
					else if (pPos->z    - fDepth <  g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMax.z
					     &&  pOldPos->z - fDepth >= g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMax.z)
					{ // ��납��̓����蔻��

						// �ʒu��␳
						pPos->z = g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMax.z + fDepth + 0.01f;

						if (pMove->x >= 13.0f)
						{ // �ړ��ʂ����ȏ�̏ꍇ
							// �p�[�e�B�N���̐ݒ菈��
							SetParticle
							(
								D3DXVECTOR3(pPos->x, pPos->y + 50.0f, g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMax.z),	// �ʒu
								D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),					// �F
								PARTICLETYPE_SPARK,									// �p�[�e�B�N���̎��
								10,													// ������
								2													// ����
							);
						}

						if (state == BOOSTSTATE_UP)
						{ // �u�[�X�g���̏ꍇ

							if (g_aObject[nCntObject].nBreakType == BREAKTYPE_ON)
							{ // �����ނ̃I�u�W�F�N�g�������ꍇ

								// �I�u�W�F�N�g�̃_���[�W����
								HitObject(&g_aObject[nCntObject], OBJ_LIFE);
							}
							else
							{ // ��L�ȊO�̏ꍇ

								// �ړ��ʂ��폜
								pMove->x *= 0.95f;
							}
						}
						else
						{ // ��L�ȊO�̏ꍇ

							// �ړ��ʂ��폜
							pMove->x *= 0.95f;
						}

						if (*pPolice == POLICESTATE_TACKLE)
						{ // �^�b�N�����������ꍇ

							// �ǐՏ�Ԃɂ���
							*pPolice = POLICESTATE_CHASE;
						}

						// �a�؃J�E���g�����Z����
						*pTraCnt += 1;
					}
				}

				// ���E�̓����蔻��
				if (pPos->z + fDepth > g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMin.z
				&&  pPos->z - fDepth < g_aObject[nCntObject].pos.z + g_aObject[nCntObject].modelData.vtxMax.z)
				{ // �u���b�N�̑O��͈͓̔��̏ꍇ

					if (pPos->x    + fWidth >  g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMin.x
					&&  pOldPos->x + fWidth <= g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMin.x)
					{ // ������̓����蔻��

						// �ʒu��␳
						pPos->x = g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMin.x - fWidth - 0.01f;

						if (pMove->x >= 13.0f)
						{ // �ړ��ʂ����ȏ�̏ꍇ
							// �p�[�e�B�N���̐ݒ菈��
							SetParticle
							(
								D3DXVECTOR3(g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMin.x, pPos->y + 50.0f, pPos->z),	// �ʒu
								D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),					// �F
								PARTICLETYPE_SPARK,									// �p�[�e�B�N���̎��
								10,													// ������
								2													// ����
							);
						}

						if (state == BOOSTSTATE_UP)
						{ // �u�[�X�g���̏ꍇ

							if (g_aObject[nCntObject].nBreakType == BREAKTYPE_ON)
							{ // �����ނ̃I�u�W�F�N�g�������ꍇ

								// �I�u�W�F�N�g�̃_���[�W����
								HitObject(&g_aObject[nCntObject], OBJ_LIFE);
							}
							else
							{ // ��L�ȊO�̏ꍇ

								// �ړ��ʂ��폜
								pMove->x *= 0.95f;
							}
						}
						else
						{ // ��L�ȊO�̏ꍇ

							// �ړ��ʂ��폜
							pMove->x *= 0.95f;
						}

						if (*pPolice == POLICESTATE_TACKLE)
						{ // �^�b�N�����������ꍇ

							// �ǐՏ�Ԃɂ���
							*pPolice = POLICESTATE_CHASE;
						}

						// �a�؃J�E���g�����Z����
						*pTraCnt += 1;
					}
					else if (pPos->x    - fWidth <  g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMax.x
					     &&  pOldPos->x - fWidth >= g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMax.x)
					{ // �E����̓����蔻��
						
						// �ʒu��␳
						pPos->x = g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMax.x + fWidth + 0.01f;

						if (pMove->x >= 13.0f)
						{ // �ړ��ʂ����ȏ�̏ꍇ
							// �p�[�e�B�N���̐ݒ菈��
							SetParticle
							(
								D3DXVECTOR3(g_aObject[nCntObject].pos.x + g_aObject[nCntObject].modelData.vtxMax.x, pPos->y + 50.0f, pPos->z),	// �ʒu
								D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),					// �F
								PARTICLETYPE_SPARK,									// �p�[�e�B�N���̎��
								10,													// ������
								2													// ����
							);
						}

						if (state == BOOSTSTATE_UP)
						{ // �u�[�X�g���̏ꍇ

							if (g_aObject[nCntObject].nBreakType == BREAKTYPE_ON)
							{ // �����ނ̃I�u�W�F�N�g�������ꍇ

								// �I�u�W�F�N�g�̃_���[�W����
								HitObject(&g_aObject[nCntObject], OBJ_LIFE);
							}
							else
							{ // ��L�ȊO�̏ꍇ

								// �ړ��ʂ��폜
								pMove->x *= 0.95f;
							}
						}
						else
						{ // ��L�ȊO�̏ꍇ

							// �ړ��ʂ��폜
							pMove->x *= 0.95f;
						}

						if (*pPolice == POLICESTATE_TACKLE)
						{ // �^�b�N�����������ꍇ

							// �ǐՏ�Ԃɂ���
							*pPolice = POLICESTATE_CHASE;
						}

						// �a�؃J�E���g�����Z����
						*pTraCnt += 1;
					}
				}

				// �����𔲂���
				break;

			case COLLISIONTYPE_CREATE:	// �쐬���������蔻�� (�ėp)

				for (int nCntColl = 0; nCntColl < g_aCollision[g_aObject[nCntObject].nType].nNumColl; nCntColl++)
				{ // �����蔻�萔���J��Ԃ�

					// �����蔻��̒��S���W��ݒ�
					collPos = g_aObject[nCntObject].pos - g_aObject[nCntObject].collInfo.vecPos[nCntColl];

					// �O��̓����蔻��
					if (pPos->x + fWidth > collPos.x - g_aObject[nCntObject].collInfo.fWidth[nCntColl]
					&&  pPos->x - fWidth < collPos.x + g_aObject[nCntObject].collInfo.fWidth[nCntColl])
					{ // �u���b�N�̍��E�͈͓̔��̏ꍇ

						if (pPos->z    + fDepth >  collPos.z - g_aObject[nCntObject].collInfo.fDepth[nCntColl]
						&&  pOldPos->z + fDepth <= collPos.z - g_aObject[nCntObject].collInfo.fDepth[nCntColl])
						{ // �O����̓����蔻��

							// �ʒu��␳
							pPos->z = collPos.z - g_aObject[nCntObject].collInfo.fDepth[nCntColl] - fDepth - 0.01f;

							if (pMove->x >= 13.0f)
							{ // �ړ��ʂ����ȏ�̏ꍇ
								// �p�[�e�B�N���̐ݒ菈��
								SetParticle
								(
									D3DXVECTOR3(pPos->x, pPos->y + 50.0f, collPos.z - g_aObject[nCntObject].collInfo.fDepth[nCntColl]),	// �ʒu
									D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),					// �F
									PARTICLETYPE_SPARK,									// �p�[�e�B�N���̎��
									10,													// ������
									2													// ����
								);
							}

							if (state == BOOSTSTATE_UP)
							{ // �u�[�X�g���̏ꍇ

								if (g_aObject[nCntObject].nBreakType == BREAKTYPE_ON)
								{ // �����ނ̃I�u�W�F�N�g�������ꍇ

									// �I�u�W�F�N�g�̃_���[�W����
									HitObject(&g_aObject[nCntObject], OBJ_LIFE);
								}
								else
								{ // ��L�ȊO�̏ꍇ

									// �ړ��ʂ��폜
									pMove->x *= 0.95f;
								}
							}
							else
							{ // ��L�ȊO�̏ꍇ

								// �ړ��ʂ��폜
								pMove->x *= 0.95f;
							}

							if (*pPolice == POLICESTATE_TACKLE)
							{ // �^�b�N�����������ꍇ

								// �ǐՏ�Ԃɂ���
								*pPolice = POLICESTATE_CHASE;
							}

							// �a�؃J�E���g�����Z����
							*pTraCnt += 1;
						}
						else if (pPos->z    - fDepth <  collPos.z + g_aObject[nCntObject].collInfo.fDepth[nCntColl]
						     &&  pOldPos->z - fDepth >= collPos.z + g_aObject[nCntObject].collInfo.fDepth[nCntColl])
						{ // ��납��̓����蔻��

							// �ʒu��␳
							pPos->z = collPos.z + g_aObject[nCntObject].collInfo.fDepth[nCntColl] + fDepth + 0.01f;

							if (pMove->x >= 13.0f)
							{ // �ړ��ʂ����ȏ�̏ꍇ
							  // �p�[�e�B�N���̐ݒ菈��
								SetParticle
								(
									D3DXVECTOR3(pPos->x, pPos->y + 50.0f, collPos.z + g_aObject[nCntObject].collInfo.fDepth[nCntColl]),	// �ʒu
									D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),					// �F
									PARTICLETYPE_SPARK,									// �p�[�e�B�N���̎��
									10,													// ������
									2													// ����
								);
							}

							if (state == BOOSTSTATE_UP)
							{ // �u�[�X�g���̏ꍇ

								if (g_aObject[nCntObject].nBreakType == BREAKTYPE_ON)
								{ // �����ނ̃I�u�W�F�N�g�������ꍇ

									// �I�u�W�F�N�g�̃_���[�W����
									HitObject(&g_aObject[nCntObject], OBJ_LIFE);
								}
								else
								{ // ��L�ȊO�̏ꍇ

									// �ړ��ʂ��폜
									pMove->x *= 0.95f;
								}
							}
							else
							{ // ��L�ȊO�̏ꍇ

								// �ړ��ʂ��폜
								pMove->x *= 0.95f;
							}

							if (*pPolice == POLICESTATE_TACKLE)
							{ // �^�b�N�����������ꍇ

								// �ǐՏ�Ԃɂ���
								*pPolice = POLICESTATE_CHASE;
							}

							// �a�؃J�E���g�����Z����
							*pTraCnt += 1;
						}
					}

					// ���E�̓����蔻��
					if (pPos->z + fDepth > collPos.z - g_aObject[nCntObject].collInfo.fDepth[nCntColl]
					&&  pPos->z - fDepth < collPos.z + g_aObject[nCntObject].collInfo.fDepth[nCntColl])
					{ // �u���b�N�̑O��͈͓̔��̏ꍇ

						if (pPos->x    + fWidth >  collPos.x - g_aObject[nCntObject].collInfo.fWidth[nCntColl]
						&&  pOldPos->x + fWidth <= collPos.x - g_aObject[nCntObject].collInfo.fWidth[nCntColl])
						{ // ������̓����蔻��

							// �ʒu��␳
							pPos->x = collPos.x - g_aObject[nCntObject].collInfo.fWidth[nCntColl] - fWidth - 0.01f;

							if (pMove->x >= 13.0f)
							{ // �ړ��ʂ����ȏ�̏ꍇ
								// �p�[�e�B�N���̐ݒ菈��
								SetParticle
								(
									D3DXVECTOR3(collPos.x - g_aObject[nCntObject].collInfo.fWidth[nCntColl], pPos->y + 50.0f, pPos->z),	// �ʒu
									D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),					// �F
									PARTICLETYPE_SPARK,									// �p�[�e�B�N���̎��
									10,													// ������
									2													// ����
								);
							}

							if (state == BOOSTSTATE_UP)
							{ // �u�[�X�g���̏ꍇ

								if (g_aObject[nCntObject].nBreakType == BREAKTYPE_ON)
								{ // �����ނ̃I�u�W�F�N�g�������ꍇ

									// �I�u�W�F�N�g�̃_���[�W����
									HitObject(&g_aObject[nCntObject], OBJ_LIFE);
								}
								else
								{ // ��L�ȊO�̏ꍇ

									// �ړ��ʂ��폜
									pMove->x *= 0.95f;
								}
							}
							else
							{ // ��L�ȊO�̏ꍇ

								// �ړ��ʂ��폜
								pMove->x *= 0.95f;
							}

							if (*pPolice == POLICESTATE_TACKLE)
							{ // �^�b�N�����������ꍇ

								// �ǐՏ�Ԃɂ���
								*pPolice = POLICESTATE_CHASE;
							}

							// �a�؃J�E���g�����Z����
							*pTraCnt += 1;
						}
						else if (pPos->x    - fWidth <  collPos.x + g_aObject[nCntObject].collInfo.fWidth[nCntColl]
						     &&  pOldPos->x - fWidth >= collPos.x + g_aObject[nCntObject].collInfo.fWidth[nCntColl])
						{ // �E����̓����蔻��
							
							// �ʒu��␳
							pPos->x = collPos.x + g_aObject[nCntObject].collInfo.fWidth[nCntColl] + fWidth + 0.01f;

							if (pMove->x >= 13.0f)
							{ // �ړ��ʂ����ȏ�̏ꍇ
								// �p�[�e�B�N���̐ݒ菈��
								SetParticle
								(
									D3DXVECTOR3(collPos.x + g_aObject[nCntObject].collInfo.fWidth[nCntColl], pPos->y + 50.0f, pPos->z),	// �ʒu
									D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f),					// �F
									PARTICLETYPE_SPARK,									// �p�[�e�B�N���̎��
									10,													// ������
									2													// ����
								);
							}

							if (state == BOOSTSTATE_UP)
							{ // �u�[�X�g���̏ꍇ

								if (g_aObject[nCntObject].nBreakType == BREAKTYPE_ON)
								{ // �����ނ̃I�u�W�F�N�g�������ꍇ

									// �I�u�W�F�N�g�̃_���[�W����
									HitObject(&g_aObject[nCntObject], OBJ_LIFE);
								}
								else
								{ // ��L�ȊO�̏ꍇ

									// �ړ��ʂ��폜
									pMove->x *= 0.95f;
								}
							}
							else
							{ // ��L�ȊO�̏ꍇ

								// �ړ��ʂ��폜
								pMove->x *= 0.95f;
							}

							if (*pPolice == POLICESTATE_TACKLE)
							{ // �^�b�N�����������ꍇ

								// �ǐՏ�Ԃɂ���
								*pPolice = POLICESTATE_CHASE;
							}

							// �a�؃J�E���g�����Z����
							*pTraCnt += 1;
						}
					}
				}

				// �����𔲂���
				break;
			}
		}
	}
}

//======================================================================================================================
// ������ԃI�u�W�F�N�g�Ƃ̓����蔻��
//======================================================================================================================
void SmashCollision(D3DXVECTOR3 pos, float fRadius)
{
	float fLength;			// ����
	float fAngle;			// ����

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (g_aObject[nCntObject].bUse == true && g_aObject[nCntObject].nBreakType == BREAKTYPE_SMASH && g_aObject[nCntObject].smash.State != SMASHSTATE_ON)
		{ // �g�p���Ă��Ȃ����A������΂���ނ��A������΂����ł͂Ȃ��ꍇ�������ꍇ
			
			//�ڕW�̌�����ݒ肷��
			fAngle = atan2f(pos.x - g_aObject[nCntObject].pos.x, pos.z - g_aObject[nCntObject].pos.z);

			// �����̐��K��
			RotNormalize(&fAngle);

			//�����𑪂�
			fLength = (g_aObject[nCntObject].pos.x - pos.x) * (g_aObject[nCntObject].pos.x - pos.x)
				+ (g_aObject[nCntObject].pos.z - pos.z) * (g_aObject[nCntObject].pos.z - pos.z);
			
			if (fLength <= ((g_aObject[nCntObject].modelData.fRadius + fRadius) * (g_aObject[nCntObject].modelData.fRadius + fRadius)))
			{ // �I�u�W�F�N�g���������Ă���
				// ������΂���Ԃɂ���
				g_aObject[nCntObject].smash.State = SMASHSTATE_ON;

				// �����̈ړ��ʂ�ݒ肷��
				g_aObject[nCntObject].smash.rotMove.x = SMASH_ROTMOVE_X;
				g_aObject[nCntObject].smash.rotMove.y = SMASH_ROTMOVE_Y;
				g_aObject[nCntObject].smash.rotMove.z = SMASH_ROTMOVE_Z;

				if (fAngle <= 0.0f)
				{ // �p�x���E���������ꍇ
					// �p�x��ς���
					fAngle += (D3DX_PI * SMASH_ANGLE_ADD);

					// �����̐��K��
					RotNormalize(&fAngle);

					//�I�u�W�F�N�g�𐁂���΂�
					g_aObject[nCntObject].smash.move.x = -sinf(fAngle) * SMASH_WIDTH;
					g_aObject[nCntObject].smash.move.y = cosf(D3DX_PI * SMASH_ANGLE) * SMASH_HEIGHT;
					g_aObject[nCntObject].smash.move.z = -cosf(fAngle) * SMASH_DEPTH;
				}
				else
				{ // ��L�ȊO
					// �p�x��ς���
					fAngle -= (D3DX_PI * SMASH_ANGLE_ADD);

					// �����̐��K��
					RotNormalize(&fAngle);

					//�I�u�W�F�N�g�𐁂���΂�
					g_aObject[nCntObject].smash.move.x = -sinf(fAngle) * SMASH_WIDTH;
					g_aObject[nCntObject].smash.move.y = cosf(D3DX_PI * SMASH_ANGLE) * SMASH_HEIGHT;
					g_aObject[nCntObject].smash.move.z = -cosf(fAngle) * SMASH_DEPTH;
				}
			}
		}
	}
}

//======================================================================================================================
//	�I�u�W�F�N�g�̎擾����
//======================================================================================================================
Object *GetObjectData(void)
{
	// �I�u�W�F�N�g�̏��̐擪�A�h���X��Ԃ�
	return &g_aObject[0];
}

//======================================================================================================================
//	�����蔻��̎擾����
//======================================================================================================================
Collision *GetCollision(void)
{
	// �����蔻��̏��̐擪�A�h���X��Ԃ�
	return &g_aCollision[0];
}

//======================================================================================================================
//	�e�̔��a�̎擾����
//======================================================================================================================
float *GetShadowRadius(void)
{
	// �e�̔��a�̏��̐擪�A�h���X��Ԃ�
	return &g_aShadowRadius[0];
}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================
//**********************************************************************************************************************
//	�I�u�W�F�N�g�̑����擾����
//**********************************************************************************************************************
int GetNumObject(void)
{
	// �ϐ���錾
	int nNumObject = 0;	// �I�u�W�F�N�g�̑����̊m�F�p

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aObject[nCntObject].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// �J�E���^�[�����Z
			nNumObject++;
		}
	}

	// �ϐ��̒l��Ԃ�
	return nNumObject;	// �I�u�W�F�N�g�̑���
}
#endif