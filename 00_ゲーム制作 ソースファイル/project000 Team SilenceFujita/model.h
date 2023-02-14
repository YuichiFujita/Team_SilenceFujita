//============================================================
//
//	���f���w�b�_�[ [model.h]
//	Author�F���c�E��
//
//============================================================
#ifndef _MODEL_H_	// ���̃}�N����`������Ă��Ȃ��ꍇ
#define _MODEL_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//************************************************************
//	�񋓌^��` (MODELTYPE)
//************************************************************
typedef enum
{
	// �I�u�W�F�N�g���f��
	MODELTYPE_OBJECT_TREE = 0,			// ��
	MODELTYPE_OBJECT_ROCK,				// ��
	MODELTYPE_OBJECT_FOREST,			// �X
	MODELTYPE_OBJECT_BIGTREE,			// ���
	MODELTYPE_OBJECT_YELLOWTREE,		// ���F����
	MODELTYPE_OBJECT_GRASS,				// ��
	MODELTYPE_OBJECT_HOUSE,				// ��
	MODELTYPE_OBJECT_CURRYSHOP,			// �J���[��
	MODELTYPE_OBJECT_BILL001,			// �r��ver1
	MODELTYPE_OBJECT_BILL002,			// �r��ver2
	MODELTYPE_OBJECT_ARCS,				// �X�[�p�[
	MODELTYPE_OBJECT_BAKERY,			// �p����
	MODELTYPE_OBJECT_CITYOFFECE,		// �s����
	MODELTYPE_OBJECT_GYMNASIUM,			// �̈��
	MODELTYPE_OBJECT_CASINO,			// �J�W�m
	MODELTYPE_OBJECT_SANDBOX,			// �����i����j
	MODELTYPE_OBJECT_TOILET,			// �����i�g�C���j
	MODELTYPE_OBJECT_WATER,				// �����i�����j
	MODELTYPE_OBJECT_BIN,				// �����i�S�~���j
	MODELTYPE_OBJECT_BENCH,				// �����i�x���`�j
	MODELTYPE_OBJECT_STREETLIGHT,		// �����i�X���j
	MODELTYPE_OBJECT_HORIZONTALBAR,		// �����i�S�_�j
	MODELTYPE_OBJECT_SLIDE,				// �����i�����j
	MODELTYPE_OBJECT_APART,				// �A�p�[�g
	MODELTYPE_OBJECT_GASSTAND,			// �K�\�����X�^���h
	MODELTYPE_OBJECT_CONVENI,			// �R���r�j
	MODELTYPE_OBJECT_JOYFAT,			// �W��
	MODELTYPE_OBJECT_TOWERCRANE,		// �^���[���[��
	MODELTYPE_OBJECT_NETCAFE,			// �l�b�g�J�t�F
	MODELTYPE_OBJECT_HOUSE002,			// �Ɓi2�j
	MODELTYPE_OBJECT_HOUSE003,			// �Ɓi3�j
	MODELTYPE_OBJECT_SCHOOL,			// �w�Z
	MODELTYPE_OBJECT_DENTIST,			// �����
	MODELTYPE_OBJECT_SIGNAL,			// �M��
	MODELTYPE_OBJECT_HOSPITAL,			// �a�@
	MODELTYPE_OBJECT_CONVENI_PARKING,	// �R���r�j�̒��ԏ�
	MODELTYPE_OBJECT_FUJICAMERA,		// �Ɠd�ʔ̓X�i�t�W�J�����j
	MODELTYPE_OBJECT_LOOF,				// ���X�X�i�����j
	MODELTYPE_OBJECT_OBARAFOOD,			// ���X�X�i�H���j
	MODELTYPE_OBJECT_OBAMART,			// ���X�X�i���z�X�j
	MODELTYPE_OBJECT_OBASTATION,		// ���X�X�i�Q�[���Z���^�[�j
	MODELTYPE_OBJECT_SWING,				// �����i�u�����R�j
	MODELTYPE_OBJECT_FUJIDORA,			// �t�W�h��
	MODELTYPE_OBJECT_FENCE,				// �����i�t�F���X�j
	MODELTYPE_OBJECT_OBARAKURABU,		// ���X�X�i�G�݁j
	MODELTYPE_OBJECT_OBARAMEAT,			// ���X�X�i�����X�j
	MODELTYPE_OBJECT_OBARAVEGETABLE,	// ���X�X�i�ʓX�j
	MODELTYPE_OBJECT_OBARAFLOUR,		// ���X�X�i�ԉ��j
	MODELTYPE_OBJECT_OBARAFUKUYA,		// ���X�X�i�����j
	MODELTYPE_OBJECT_OBARAKARAOKE,		// ���X�X�i�ėp�E�J���I�P�j
	MODELTYPE_OBJECT_OBARACLEANING,		// ���X�X�i�ėp�E�N���[�j���O�j
	MODELTYPE_OBJECT_OBARACLINIC,		// ���X�X�i�ėp�E����ҁj
	MODELTYPE_OBJECT_OBARARAMEN,		// ���X�X�i�ėp�E���[�����j
	MODELTYPE_OBJECT_OBARACOFFEE,		// ���X�X�i�ėp�E�i���j
	MODELTYPE_OBJECT_OBARAMOBILE,		// ���X�X�i�ėp�E�g�уV���b�v�j
	MODELTYPE_OBJECT_OBARABONE,			// ���X�X�i�ėp�E�����@�j
	MODELTYPE_OBJECT_OBARATENPURA,		// ���X�X�i�ėp�E�V�Ղ�j
	MODELTYPE_OBJECT_OBARAYAKKYOKU,		// ���X�X�i�ėp�E��ǁj
	MODELTYPE_OBJECT_OBARASHUTTER,		// ���X�X�i�ėp�E�V���b�^�[�j

	// �v���C���[���f��
	MODELTYPE_PLAYER_CAR,				// �v���C���[
	MODELTYPE_PLAYER_BARRIER,			// �o���A

	// �ԃ��f��
	MODELTYPE_CAR_POLICE,				// �p�g�J�[
	MODELTYPE_CAR_FARECAR,				// ���h��
	MODELTYPE_CAR_CAR001,				// ������
	MODELTYPE_CAR_YAKIIMOCAR,			// �ΏĂ�����
	MODELTYPE_CAR_ELECTIONCAR,			// �I���J�[

	// �Q�[�g���f��
	MODELTYPE_OBJECT_GATE000,			// �Q�[�g (����)
	MODELTYPE_OBJECT_GATE001,			// �Q�[�g (�S��)

	// ���ꂫ���f��
	MODELTYPE_OBJECT_JUNK,				// ���ꂫ

	// �G�f�B�b�g���f��
	MODELTYPE_EDIT_COLLISION,			// �����蔻��
	MODELTYPE_MAX,						// �S���f���̑���
} MODELTYPE;

//************************************************************
//	�}�N����`
//************************************************************
#define INIT_VTX_MIN	(D3DXVECTOR3( 9999.0f,  9999.0f,  9999.0f))		// ���f���̍ŏ��̒��_���W�̏����l
#define INIT_VTX_MAX	(D3DXVECTOR3(-9999.0f, -9999.0f, -9999.0f))		// ���f���̍ő�̒��_���W�̏����l
#define INIT_SIZE		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))					// ���f���T�C�Y�̏����l

#define FROM_OBJECT		(0)								// �I�u�W�F�N�g�̃��f���񋓂̊J�n�n�_
#define MODEL_OBJ_MAX	(int(MODELTYPE_PLAYER_CAR))		// �I�u�W�F�N�g�̃��f���̑���
#define FROM_PLAYER		(int(MODELTYPE_PLAYER_CAR))		// �v���C���[�̃��f���񋓂̊J�n�n�_
#define MODEL_PLAY_MAX	(2)								// �v���C���[�̃��f���̑���
#define FROM_CAR		(int(MODELTYPE_CAR_POLICE))		// �Ԃ̃��f���񋓂̊J�n�n�_
#define MODEL_CAR_MAX	(5)								// �Ԃ̃��f���̑���
#define FROM_GATE		(int(MODELTYPE_OBJECT_GATE000))	// �Q�[�g�̃��f���񋓂̊J�n�n�_
#define MODEL_GATE_MAX	(2)								// �Q�[�g�̃��f���̑���
#define FROM_EDIT		(int(MODELTYPE_EDIT_COLLISION))	// �G�f�B�b�g�̃��f���񋓂̊J�n�n�_
#define MODEL_EDIT_MAX	(1)								// �G�f�B�b�g�̃��f���̑���

//************************************************************
//	�\���̒�` (Model)
//************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9 *pTexture;	// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH         pMesh;		// ���b�V�� (���_���) �ւ̃|�C���^
	LPD3DXBUFFER       pBuffMat;	// �}�e���A���ւ̃|�C���^
	DWORD              dwNumMat;	// �}�e���A���̐�
	D3DXVECTOR3        vtxMin;		// �ŏ��̒��_���W
	D3DXVECTOR3        vtxMax;		// �ő�̒��_���W
	D3DXVECTOR3        size;		// �傫��
	float              fRadius;		// ���a
}Model;

//************************************************************
//	�v���g�^�C�v�錾
//************************************************************
HRESULT InitModel(void);			// ���f���̏���������
void UninitModel(void);				// ���f���̏I������
Model GetModelData(int nID);		// ���f�����̎擾����

#endif