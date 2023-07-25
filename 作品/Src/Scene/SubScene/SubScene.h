#pragma once

class SubScene
{
public:
	friend class SceneManager;
	enum class State{
		MainStop,	//���C���V�[�����~�߂邩

	};
public:
	SubScene();
	virtual ~SubScene();

	//������
	virtual void Init() = 0;

	//���[�h
	virtual void Load() = 0;

	//���[�h����
	virtual void OnLoadEnd() = 0;

	//�X�e�b�v����
	virtual bool Step();

	//�X�V����
	virtual void UpDate() = 0;

	//�`��
	virtual void Draw() = 0;

	//�㏈��
	virtual void Fin() = 0;

public:
	inline bool isState(int state) const { return (State_ & (0x01ULL << state)) != 0; }

protected:
	inline void AddState(int state) { State_ |= 0x01ULL << state; }
	void Delete();

private:
	unsigned __int64 State_;

	// vfptr(8byte)
};

/*
	SubScene �������C���[�W

	State_	8byte
	vfptr	8byte
*/

namespace Size {
	constexpr size_t SUB_SCENE_BASE = sizeof(SubScene);
}