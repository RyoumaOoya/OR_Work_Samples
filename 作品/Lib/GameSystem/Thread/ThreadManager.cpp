#include "ThreadManager.h"
#include "../../Math/MyMath.h"

namespace MyLib
{
	void ThreadManager::Init()
	{
		std::string threadName("サブスレッド");

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
			//関数ポインタが送られてくるまで待機する
			while (FuncQueue.size_approx() == 0)
			{
				Sleep(1);
			}

			++ActiveThreadNum;
			if (!FuncQueue.try_dequeue(func)) {
				--ActiveThreadNum;
				continue;
			}
			//nullptrが送られたらスレッドを終了する
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
		//各スレッドを閉じる
		for (int i = 0; i < ThreadNum; i++)
			PushFunc(nullptr);
		for (int i = 0; i < ThreadNum; i++)
			Threads[i].join();
		for (int i = 0; i < ThreadNum; i++)
			Threads[i].Close();

		delete[] Threads;
	}
}