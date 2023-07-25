#pragma once
#include <vcruntime_string.h>
#include <filesystem>
#include <atomic>
#include <functional>

#include <DxLib.h>

using std::atomic;
typedef std::filesystem::path filepath;
typedef int handle;

typedef __int8  i8;
typedef __int16 i16;
typedef __int32 i32;
typedef __int64 i64;

typedef unsigned __int8  u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;

typedef long double ldouble;

//struct DxHandle 
//{
//public:
//	enum class Type {
//		Null = -1,
//		Graph,
//		Model,
//		Sound,
//
//		Num
//	};
//
//	DxHandle() :handle(-1), type(Type::Null) {}
//	DxHandle(int h, Type t) :handle(h), type(t) {}
//	~DxHandle() { }
//
//
//	void LoadGraph(filepath path) { type = Type::Graph; handle = LoadFunc[(int)type](path.string().c_str()); }
//	void LoadSound(filepath path) { type = Type::Sound; handle = LoadFunc[(int)type](path.string().c_str()); }
//	void LoadModel(filepath path, int BufferNum = 3, int unionHandle = -1) { type = Type::Model; handle = LoadSoundMem(path.string().c_str(), BufferNum, unionHandle); }
//
//
//	int Delete() {
//		int result = -1; 
//		if (type > Type::Null && type < Type::Num && operator bool()) {
//			result = DeleteFunc[(int)type](handle); handle = -1; 
//		}
//		return result; }
//
//	int operator=(int h) { return handle = h; }
//
//	operator int() { return handle; }
//	operator bool() { return handle != -1; }
//private:
//	static const std::function<int(const char*)> LoadFunc[(int)Type::Num];
//	static const std::function<int(int)> DeleteFunc[(int)Type::Num];
//private:
//	int handle;
//	Type type;
//};
//
//const std::function<int(const char*)> DxHandle::LoadFunc[(int)Type::Num] = {
//	std::bind(&DxLib::LoadGraph, std::placeholders::_1, 0),
//	&MV1LoadModel,
//	std::bind(&DxLib::LoadSoundMem, std::placeholders::_1, 3, -1),
//};
//
//const std::function<int(int)> DxHandle::DeleteFunc[(int)Type::Num] = {
//	std::bind(&DeleteGraph, std::placeholders::_1, 0),
//	&MV1DeleteModel,
//	std::bind(&DeleteSoundMem, std::placeholders::_1, 0),
//};