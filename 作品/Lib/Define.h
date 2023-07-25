#pragma once
#include "Literal.h"

#define FOR(name,loop)\
for(int name = 0; name < loop; name++)

#define E_FOR(enum_name)\
for(int index = (int)enum_name::BEGIN; index < (int)enum_name::NUM; index++)

//継承を利用しないシングルトン用マクロ
#define SINGLETON(ManagerName)\
friend class Allocator;\
public:\
static Pointer<ManagerName> CreateInstance() { return (!Instance_) ? Instance_ = MyNew(ManagerName, AllocType::Manager, 0) : Instance_; }\
static Pointer<ManagerName> GetInstance() { return (Instance_) ? Instance_ : CreateInstance(); }\
static void DeleteInstance() { if (Instance_) MyFree(Instance_) ; }\
private:\
static inline Pointer<ManagerName> Instance_

//範囲内か
#define CHECK_RANGE(Value, Min, Max) ((Value >= Min) && (Value <= Max))

//enumのオペレーター定義を簡略化するマクロ
#define ENUM_OQERATOR_MEMBER(enum_class_name, successor, operator_type)\
constexpr bool operator operator_type(enum_class_name a, successor b) {\
	return static_cast<successor>(a) operator_type b;\
}

//enumとenumの基底型との比較、ビット演算などの定義用マクロ
#define ENUM_OPERATORS(enum_class_name, successor)\
ENUM_OQERATOR_MEMBER(enum_class_name,successor, ==)\
ENUM_OQERATOR_MEMBER(enum_class_name,successor, !=)\
ENUM_OQERATOR_MEMBER(enum_class_name,successor, >)\
ENUM_OQERATOR_MEMBER(enum_class_name,successor, >=)\
ENUM_OQERATOR_MEMBER(enum_class_name,successor, <)\
ENUM_OQERATOR_MEMBER(enum_class_name,successor, <=)\
constexpr u64 operator<<(successor a, enum_class_name b)\
{\
	return static_cast<u64>(a) << static_cast<u64>(b);\
}