#include "Allocator.h"
#include "../../Constant/Constant.h"
#include "../Common/Common.h"
#include "../GameSystem.h"
#include "../../Render/Render.h"
#include "../../Render/BlendData/ScopedBlend.h"
//#include "../GameStstem/GameStstem.h"
#include <cassert>
//#include "../Message/Message.h"
#include "../../Math/MyMath.h"

namespace MyLib
{
	//インライン化しないとエラーになる
	template<class type, size_t len>
	constexpr inline size_t Sum(std::array<type, len> args) {
		size_t result = 0;
		for (const size_t num : args) {
			result += num;
		}
		return result;
	}

	namespace
	{
		constexpr size_t POINTER_NUM(16_KB);
		constexpr size_t FREE_SPACE_DATA_NUM(4_KB);
		constexpr size_t INITIAL_SECUREMENT(16_MB);
		static_assert(INITIAL_SECUREMENT > 0, "INITIAL_SECUREMENT error");
		CONSTANT_ARRAY(size_t, IAllocType::Num) ALLOC_SIZE = {
			//Library
			2_KB,
			//Prototype
			4_KB,
			//Manager
			2_KB,
			//Sound		実際のサウンドデータはDxLibで管理されている
			64_KB,
			//UI
			0.5_KB,
			//SceneData
			1_KB,
			//Camera
			128_B,
			//Stage
			512_B,
			//Player
			0.5_KB,
			//Bullet
			1_MB,
			//Enemy
			4_MB,
			//Item
			2_MB,
			//Collision
			8_MB,
			//Produce
			1_KB,
		};
		constexpr auto ALLOC_SIZE_SUM = Sum(ALLOC_SIZE);
		static_assert(ALLOC_SIZE_SUM <= INITIAL_SECUREMENT, "ALLOC_SIZE error");
	}

	void Allocator::Init()
	{
		//デバック時メモリリーク検知を開始する
#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
		//メモリ確保、初期化
		Pool = reinterpret_cast<char*>(malloc(INITIAL_SECUREMENT));
		if (Pool == nullptr) {
			abort();
		}
		memset(Pool, 0, INITIAL_SECUREMENT);

		constexpr size_t manager_size = sizeof(ManagementPointerList[IAllocType::Num]) +
			sizeof(ForManagementPointer[POINTER_NUM]) +
			sizeof(FreeSpaceData[FREE_SPACE_DATA_NUM]);
		char* manager_pool = (char*)malloc(manager_size);
		if (manager_pool == nullptr)
			abort();
		memset(manager_pool, 0, manager_size);

		{
			char* temp = manager_pool + sizeof(ManagementPointerList[IAllocType::Num]);
			PointerData.arr = PointerData.next = new(temp) ForManagementPointer[POINTER_NUM];
			PointerData.end = PointerData.arr + POINTER_NUM;
			temp = reinterpret_cast<char*>(PointerData.end);

			FSData.arr = FSData.next = new(temp) FreeSpaceData[FREE_SPACE_DATA_NUM]();
			FSData.end = FSData.arr + FREE_SPACE_DATA_NUM;
		}
		PointerList = new(manager_pool) ManagementPointerList[IAllocType::Num];

		char* current = reinterpret_cast<char*>(Pool);
		for (int i = 0; i < IAllocType::Num; i++) {
			auto& temp = PointerList[i];
			temp.fastPointer = temp.nextPointer = current;
			temp.endPointer = current += ALLOC_SIZE[i];
		}
	}

	void Allocator::Load()
	{
		if (CircleHandle == -1) {
			CircleHandle = LoadGraph("Data/Ui/Circle.png");
		}
	}

	void Allocator::Draw()
	{
#ifndef n
#ifndef _DEBUG1
		const int DrawStart_Y = (GameSystem::GetWindowSize().y >> 1) - (operator""_ROW(IAllocType::Num) >> 1);
		static CONSTANT_ARRAY(Color, IAllocType::Num) MEM_COLOR = {
			//Library
			color::PapayaWhip,
			//Prototype
			color::Blue,
			//Manager
			color::Azuki,
			//Sound
			color::DeepSkyBlue,
			//UI
			color::Cyan,
			//SceneData
			color::Green,
			//Camera
			color::Black,
			//Stage
			color::Lime,
			//Player
			color::SkyBlue,
			//Bullet
			color::Brown,
			//Enemy
			color::Red,
			//Item
			color::BlueViolet,
			//Collision
			color::DarkOrange,
			//Product
			Constant::Color(92, 41,  0),
		};
		static const char* AllocName[IAllocType::Num] = { 
			"ライブラリ", "プロトタイプ", "マネージャー", "サウンド", "UI", "シーンデータ", "カメラ", "ステージ", "プレイヤー", "バレット", "エネミー", "アイテム", "コリジョン", "生産"
		};

		//使用済みメモリ容量を取得
		static auto get_size = [](const ManagementPointerList& list_str) {
			const auto& list = list_str.list;
			size_t result = 0;
			for (const auto ptr : list) {
				result += ptr->size;
			}
			return result;
		};

		const ManagementPointerList* pointer_list = PointerList;
		std::array<size_t, IAllocType::Num> arr;
		for (int i = 0; i < IAllocType::Num; i++, pointer_list++) {
			arr[i] = get_size(*pointer_list);
		}
		{
			constexpr Vector2D CirclePos(180, 350);
			ScopedBlend blend(DX_BLENDMODE_ALPHA, 256 >> 2);

			auto DrawCircleGauge = [](const Vector2D& pos, float begin, float end, Color color) {
				SetDrawBright(color.r, color.g, color.b);
				DxLib::DrawCircleGauge(static_cast<int>(CirclePos.x), static_cast<int>(CirclePos.y), end, CircleHandle, begin);
			};
			const auto total_size = Sum(arr);
			pointer_list = PointerList;
			float current_angle = 0.f, end_angle = 0.f;

			DrawBox(0, DrawStart_Y, 460, DrawStart_Y + operator""_ROW(IAllocType::Num), Constant::Color(255, 255, 255), TRUE);

			blend.SetBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(256 * 0.75));
			for (int i = 0; i < IAllocType::Num; i++, pointer_list++) {
				if (arr[i] == 0)
					continue;
				end_angle = (static_cast<float>(arr[i]) / total_size * 100) + current_angle;

				DrawCircleGauge(CirclePos, current_angle, end_angle, MEM_COLOR[i]);

				current_angle = end_angle;
			}

			SetDrawBright(255, 255, 255);
		}
		
		pointer_list = PointerList;
		int current_y = DrawStart_Y;
		for (int i = 0; i < IAllocType::Num; i++, pointer_list++) {
			const auto used_size = arr[i];
			const size_t max_size = pointer_list->endPointer - pointer_list->fastPointer;
			DrawFormatString(0, current_y, Constant::Color(0, 0, 0), "%s", AllocName[i]);
			if (max_size >= 1_GB) {
				DrawFormatString(15 * 8, current_y, color::Black, "使用率:%.1f%%(%.1fGB / %.1fGB)",
					/*使用率*/(static_cast<double>(used_size) / (max_size)) * 100,
					/*使用中メモリサイズ*/(static_cast<float>(used_size)) / 1_GB, /*最大サイズ*/static_cast<float>(max_size) / 1._GB);
			}
			else if (max_size >= 1_KB) {
				DrawFormatString(15 * 8, current_y, color::Black, "使用率:%.1f%%(%.1fKB / %.1fKB)",
					/*使用率*/(static_cast<double>(used_size) / (max_size)) * 100,
					/*使用中メモリサイズ*/(static_cast<float>(used_size)) / 1_KB, /*最大サイズ*/static_cast<float>(max_size) / 1._KB);
			}
			else {
				DrawFormatString(15 * 8, current_y, color::Black, "使用率:%.1f%%(%dB / %dB)",
					/*使用率*/(static_cast<double>(used_size) / (max_size)) * 100,
					/*使用中メモリサイズ*/used_size, /*最大サイズ*/max_size);
			}
			DrawFormatString(350, current_y, Constant::Color(0, 0, 0), "断片化領域:%dB", (pointer_list->nextPointer - pointer_list->fastPointer) - used_size);
			current_y += 1_ROW;
		}
		DrawFormatString(350, current_y, Constant::Color(0, 0, 0), "空き領域管理領域:%dB", FSData.next - FSData.arr);
#endif // _DEBUG
#endif // _DX_LIB_INC
	}

	void Allocator::GarbageCollection()
	{
		//使用済みメモリ容量を取得
		auto get_size = [](ManagementPointerList& list_str) {
			auto& list = list_str.list;
			size_t result = 0;
			for (auto ptr : list) {
				result += ptr->size;
			}
			return result;
		};
		//Message::Push(Message::TextData(4.f, color::Black, "メモリ再配置"));

		for (int i = 0; i < IAllocType::Num; i++) {
			auto& temp = PointerList[i];
			//整列させる
			temp.list.sort();
			for (auto itr = temp.list.begin(); itr != temp.list.end(); ++itr) {
#ifdef REFERENCE_COUNTER
				if ((*itr)->use == 0) {
					(*itr)->~ForManagementPointer();
					temp.list.erase(itr);
				}
#endif // REFERENCE_COUNTER
			}
			if (temp.nextPointer - temp.fastPointer == get_size(temp))
				continue;
			//メモリの空き空間を埋める
			char* write_ptr = temp.fastPointer;
			for (auto ptr : temp.list) {
				if (write_ptr != ptr->pointer) {
					memcpy(write_ptr, ptr->pointer, ptr->size);
					ptr->pointer = write_ptr;
				}
				write_ptr += ptr->size;
			}
			temp.freeSpaceList.clear();
			temp.nextPointer = write_ptr;
		}
	}

	void Allocator::Reset()
	{
		char* current = reinterpret_cast<char*>(Pool);
		for (int i = 0; i < IAllocType::Num; i++) {
			auto& temp = PointerList[i];
			for (const auto& ptr : temp.list) {
				ptr->~ForManagementPointer();
			}
			temp.list.clear();
			temp.fastPointer = temp.nextPointer = current;
			current += ALLOC_SIZE[i];
		}

		PointerData.next = PointerData.arr;
		//memset(iterator_arr, 0, ITERATOR_NUM * sizeof(GeneralIterator));
		memset(PointerData.arr, 0, sizeof(ForManagementPointer[POINTER_NUM]));
	}

	void Allocator::Fin()
	{
		if (CircleHandle != -1) {
			DeleteGraph(CircleHandle);
			CircleHandle = -1;
		}
		auto list = PointerList;
		for (int i = 0; i < IAllocType::Num; i++, list++) {
			for (auto ptr : list->list) {
#if MOMORY_LEAK_DETECT
				LogFileAdd("メモリリークが検出されました\n");
				LogFileAdd((std::string("\tファイル:") + ptr->fileName + '\n').c_str());
				LogFileAdd((std::string("\t行:") + std::to_string(ptr->lineNumber) + '\n').c_str());
#endif // MOMORY_LEAK_DETECT
				ptr->~ForManagementPointer();
			}
			list->~ManagementPointerList();
		}
		auto fs = FSData.arr;
		for (int i = 0; i < FREE_SPACE_DATA_NUM; i++, fs++) {
			fs->~FreeSpaceData();
		}

		free(PointerList);
		free(Pool);
	}

	void Allocator::Clear(AllocType type)
	{
		auto& t_list = PointerList[static_cast<int>(type)];
		auto& list = t_list.list;
		for (auto t_itr = list.begin(); t_itr != list.end();) {
			(*t_itr)->~ForManagementPointer();
			t_itr = list.erase(t_itr);
		}
		t_list.freeSpaceList.clear();
		t_list.nextPointer = t_list.fastPointer;
	}

	void Allocator::Clear(MemTag type)
	{
		for (int i = 0; i < IAllocType::Num; i++) {
			auto& t_list = PointerList[i].list;
			for (auto t_itr = t_list.begin(); t_itr != t_list.end(); ++t_itr) {
				if (((*t_itr)->tag & static_cast<int>(type)) != 0)
					Delete(t_itr, static_cast<AllocType>(i));
			}
			FreeSpaceDataNormalize();
		}
	}

	void Allocator::Delete(std::list<ForManagementPointer*>::iterator itr, AllocType type)
	{
		auto& t_list = PointerList[static_cast<int>(type)];
		//pを検索
		for (auto t_itr = t_list.list.begin(); t_itr != t_list.list.end(); ++t_itr) {
			if (t_itr != itr) continue;
			auto ptr = (*itr);

			if (t_list.nextPointer == ptr->pointer + ptr->size)
				t_list.nextPointer = ptr->pointer;
			else {
				auto free_space_data = FreeSpaceDataAllocate();
				free_space_data->ptr = ptr->pointer;
				free_space_data->size = ptr->size;
				t_list.freeSpaceList.push_back(free_space_data);
			}

			ptr->~ForManagementPointer();
			t_list.list.erase(t_itr);
			break;
		}
	}

	Allocator::ForManagementPointer* Allocator::ManagementPointerAllocate(const ForManagementPointer& p)
	{
		if (PointerData.next == PointerData.end)
			throw;

		ScopedCS l_cs(&PointerData.cs);
		ForManagementPointer* result = new(PointerData.next) ForManagementPointer(p);
		//未使用の管理用変数を検索
		for (; PointerData.next != PointerData.end; PointerData.next++)
			if ((PointerData.next->tag & TAG_SIGN_BIT) == 0)
				break;

		return result;
	}

	Allocator::ForManagementPointer* Allocator::ManagementPointerAllocate(size_t s, AllocType Type, int Tag, void(*d)(void*))
	{
		if (PointerData.next == PointerData.end)
			throw;

		ScopedCS l_cs(&PointerData.cs);
		ForManagementPointer* result = new(PointerData.next) ForManagementPointer(s, Type, Tag, d);
		//未使用の管理用変数を検索
		for (; PointerData.next != PointerData.end; PointerData.next++)
			if ((PointerData.next->tag & TAG_SIGN_BIT) == 0)
				break;

		return result;
	}

	Allocator::FreeSpaceData* Allocator::FreeSpaceDataAllocate()
	{
		if (FSData.next == FSData.end)
			throw;

		ScopedCS l_cs(&FSData.cs);
		FreeSpaceData* result = new(FSData.next) FreeSpaceData(true);
		//未使用の管理用変数を検索
		for (; FSData.next != FSData.end; FSData.next++)
			if (!FSData.next->use)
				break;

		return result;
	}

	void Allocator::FreeSpaceDataNormalize()
	{
		ScopedCS l_cs(&FSData.cs);
		for (int i = 0; i < IAllocType::Num; i++) {
			FreeSpaceDataNormalize(i);
		}
	}

	void Allocator::FreeSpaceDataNormalize(__int64 type)
	{
		ScopedCS l_cs(&FSData.cs);
		auto& temp_list = PointerList[type].freeSpaceList;
		if (temp_list.empty())
			return;
		temp_list.sort();

		for (auto temp = temp_list.begin(), next = ++temp_list.begin(); next != temp_list.end();) {
			FreeSpaceData* temp_element = (*temp), * next_element = (*next);
			if (temp_element->ptr + temp_element->size == next_element->ptr) {
				temp_element->size += next_element->size;
				next_element->~FreeSpaceData();
				next = temp_list.erase(next);
			}
			else {
				++temp;
				++next;
			}
		}
		if (temp_list.empty())
			return;
		auto& ptr_list = PointerList[type];
		for (auto itr = --temp_list.end(); itr != temp_list.end();) {
			auto ptr = *itr;
			if (ptr->ptr + ptr->size != ptr_list.nextPointer)
				break;

			ptr_list.nextPointer = ptr->ptr;
			ptr->~FreeSpaceData();
			itr = temp_list.erase(itr);
		}
	}

	Allocator::ForManagementPointer::~ForManagementPointer()
	{
		ScopedCS l_cs(&PointerData.cs);
		if ((tag & TAG_SIGN_BIT) == 0)
			return;

		destructor(pointer);
		pointer = nullptr;
		tag = 0;
#ifdef REFERENCE_COUNTER
		use = 0;
#endif
		//管理ポインタの空いている場所の先頭が自身より後ろにあったら先頭を示す変数に自身を代入
		PointerData.next = (PointerData.next > this) ? this : PointerData.next;
	}

	auto Allocator::ForManagementPointer::operator<=>(const void* b) const
	{
		if (pointer != b) {
			return pointer <=> b;
		}
		return std::strong_ordering::equivalent;
	}

	auto Allocator::ForManagementPointer::operator<=>(const ForManagementPointer& b) const
	{
		if (pointer != b.pointer) {
			return pointer <=> b.pointer;
		}
		return std::strong_ordering::equivalent;
	}

	Allocator::FreeSpaceData::~FreeSpaceData()
	{
		ScopedCS l_cs(&FSData.cs);
		ptr = nullptr;
		size = 0;
		use = false;

		//管理ポインタの空いている場所の先頭が自身より後ろにあったら先頭を示す変数に自身を代入
		if (FSData.next > this)
			FSData.next = this;
	}
	auto Allocator::FreeSpaceData::operator<=>(const FreeSpaceData& data) const
	{
		if (ptr != data.ptr) {
			return ptr <=> data.ptr;
		}
		return std::strong_ordering::equivalent;
	}
}