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
	//スレッド関連の管理クラス
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
		//同じ関数を複数回実行する処理
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
		/*スレッドに投げられているタスクの残り数を取得*/
		static _inline size_t GetRemainingTask() { return FuncQueue.size_approx() + ActiveThreadNum; }
		/*タスクがすべて終了したか*/
		static _inline bool isRemainingTask() { return GetRemainingTask() != 0; }
		/*タスクがすべて終了するまで待つ*/
		static _inline void WaitTask() {
			atomic_thread_fence(std::memory_order::acquire);
			while (GetRemainingTask() != 0)
				Sleep(1);
			atomic_thread_fence(std::memory_order::acquire);
		}

		//バックグラウンド用関数
		//オートセーブなどの複数フレームを跨ぐ処理を実行するための関数群

		/*バックグラウンドで動く処理を追加する
		メインスレッド以外で呼ばれたときの処理は未定義*/
		static _inline void PushBackGroundTask(const char* key, VoidFunc func) {
			MyLib::Thread thread;
			thread.Begin(func);
			thread.SetDescription(key);
			BackgroundTask.emplace(key, thread);
		}
		/*バックグラウンドの処理が終わったかを返す
		スレッドが存在しない場合はtrueを返す
		メインスレッド以外で呼ばれたときの処理は未定義*/
		static _inline bool isFinishTask(const char* key) {
			if (auto iter = BackgroundTask.find(key); iter != end(BackgroundTask)) {
				return !iter->second.isActive();
			}
			return true;
		}
		/*バックグラウンドで動く処理を削除する
		メインスレッド以外で呼ばれたときの処理は未定義*/
		static _inline void EraseBackGroundTask(const char* key) {
			if (auto iter = BackgroundTask.find(key); iter != end(BackgroundTask)) {
				auto& thread = iter->second;
				thread.join();
				thread.Close();
				BackgroundTask.erase(iter);
			}
		}

		//スレッド数を取得
		static _inline size_t GetThreadNum() { return ThreadNum; }
	private:
		static void ThreadFunc();
	private:
		static inline size_t									ThreadNum = 0;			//スレッド数(実行時にCPU情報を読み取って決める)
		static inline Thread*									Threads;				//ステップやコリジョン用のスレッド
		static inline moodycamel::ConcurrentQueue<VoidFunc>		FuncQueue;				//スレッドに関数ポインタを送るためのキュー
		static inline std::atomic<size_t>						ActiveThreadNum = 0;	//アクティブなスレッドの数
		static inline std::map<std::string, MyLib::Thread>		BackgroundTask;			//バックグラウンドで実行されるスレッド用マップ
	};
}