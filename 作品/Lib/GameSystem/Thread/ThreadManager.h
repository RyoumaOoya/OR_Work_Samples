#pragma once
#include "Thread.h"
#include "CriticalSection.h"
#include <atomic>
#include <mutex>
#include <array>
#include <string>
#include <map>
#include <condition_variable>
#include <queue>
#include "../GameSystem.h"
#include "../../Others/concurrentqueue.h"

namespace MyLib
{
	//�X���b�h�֘A�̊Ǘ��N���X
	class ThreadManager
	{
	public:
		typedef std::initializer_list<VoidFunc> FuncList;
		ThreadManager() = delete;
		~ThreadManager() = delete;
	public:
		static void Init();
		static void Fin();
		static _inline void PushFunc(const VoidFunc& func) { FuncQueue.enqueue(func); }
		static _inline void PushFunc(VoidFunc&& func) { FuncQueue.enqueue(func); }
		//�����֐��𕡐�����s���鏈��
		static _inline void PushFunc(const VoidFunc& func, size_t num) { for (size_t i = 0; i < num; i++) FuncQueue.enqueue(func); }
		static _inline void PushFunc(VoidFunc&& func, size_t num) { for (size_t i = 0; i < num; i++) FuncQueue.enqueue(func); }
		static _inline void PushFunc(FuncList&& fList) { for (const auto& func : fList) FuncQueue.enqueue(func);  }
		static _inline void PushFunc(std::queue<VoidFunc>& fList) {
			while (!fList.empty())
			{
				FuncQueue.enqueue(fList.front());
				fList.pop();
			}
		}
		/*�X���b�h�ɓ������Ă���^�X�N�̎c�萔���擾*/
		static _inline size_t GetRemainingTask() { return FuncQueue.size_approx() + ActiveThreadNum; }
		/*�^�X�N�����ׂďI��������*/
		static _inline bool isRemainingTask() { return GetRemainingTask() != 0; }
		/*�^�X�N�����ׂďI������܂ő҂�*/
		static _inline void WaitTask() {
			atomic_thread_fence(std::memory_order::acquire);
			while (GetRemainingTask() != 0)
				Sleep(1);
			atomic_thread_fence(std::memory_order::acquire);
		}

		//�o�b�N�O���E���h�p�֐�
		//�I�[�g�Z�[�u�Ȃǂ̕����t���[�����ׂ����������s���邽�߂̊֐��Q

		/*�o�b�N�O���E���h�œ���������ǉ�����
		���C���X���b�h�ȊO�ŌĂ΂ꂽ�Ƃ��̏����͖���`*/
		static _inline void PushBackGroundTask(const char* key, VoidFunc func) {
			MyLib::Thread thread;
			thread.Begin(func);
			thread.SetDescription(key);
			BackgroundTask.emplace(key, thread);
		}
		/*�o�b�N�O���E���h�̏������I���������Ԃ�
		�X���b�h�����݂��Ȃ��ꍇ��true��Ԃ�
		���C���X���b�h�ȊO�ŌĂ΂ꂽ�Ƃ��̏����͖���`*/
		static _inline bool isFinishTask(const char* key) {
			if (auto iter = BackgroundTask.find(key); iter != end(BackgroundTask)) {
				return !iter->second.isActive();
			}
			return true;
		}
		/*�o�b�N�O���E���h�œ����������폜����
		���C���X���b�h�ȊO�ŌĂ΂ꂽ�Ƃ��̏����͖���`*/
		static _inline void EraseBackGroundTask(const char* key) {
			if (auto iter = BackgroundTask.find(key); iter != end(BackgroundTask)) {
				auto& thread = iter->second;
				thread.join();
				thread.Close();
				BackgroundTask.erase(iter);
			}
		}

		//�X���b�h�����擾
		static _inline size_t GetThreadNum() { return ThreadNum; }
	private:
		static void ThreadFunc();
	private:
		static inline size_t									ThreadNum = 0;			//�X���b�h��(���s����CPU����ǂݎ���Č��߂�)
		static inline Thread*									Threads;				//�X�e�b�v��R���W�����p�̃X���b�h
		static inline moodycamel::ConcurrentQueue<VoidFunc>		FuncQueue;				//�X���b�h�Ɋ֐��|�C���^�𑗂邽�߂̃L���[
		static inline std::atomic<size_t>						ActiveThreadNum = 0;	//�A�N�e�B�u�ȃX���b�h�̐�
		static inline std::map<std::string, MyLib::Thread>		BackgroundTask;			//�o�b�N�O���E���h�Ŏ��s�����X���b�h�p�}�b�v
	};
}