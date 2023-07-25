#include "ThreadManager.h"
#include "../../Math/MyMath.h"

namespace MyLib
{
	void ThreadManager::Init()
	{
		std::string threadName("�T�u�X���b�h");

		ThreadNum = MAX((GameSystem::GetSystemInfo().dwNumberOfProcessors), 1ul);

		Threads = new Thread[ThreadNum];

		std::stringstream stream;
		stream << std::hex;
		for (int i = 0; i < ThreadNum; i++) {
			stream.str(std::string());
			stream << i;
			Threads[i].Begin(&ThreadFunc);
			Threads[i].SetIdealProcessor(i);
			Threads[i].SetDescription((threadName + stream.str()).c_str());
		}
	}

	void ThreadManager::ThreadFunc()
	{
		VoidFunc func = nullptr;
		while (true)
		{
			//�֐��|�C���^�������Ă���܂őҋ@����
			while (FuncQueue.size_approx() == 0)
			{
				Sleep(1);
			}

			++ActiveThreadNum;
			if (!FuncQueue.try_dequeue(func)) {
				--ActiveThreadNum;
				continue;
			}
			//nullptr������ꂽ��X���b�h���I������
			if (func == nullptr) {
				--ActiveThreadNum;
				break;
			}

			func();
			--ActiveThreadNum;
		}
	}

	void ThreadManager::Fin()
	{
		//�e�X���b�h�����
		for (int i = 0; i < ThreadNum; i++)
			PushFunc(nullptr);
		for (int i = 0; i < ThreadNum; i++)
			Threads[i].join();
		for (int i = 0; i < ThreadNum; i++)
			Threads[i].Close();

		delete[] Threads;
	}
}