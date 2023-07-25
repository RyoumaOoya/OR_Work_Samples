#pragma once
#include "Literal.h"

#define FOR(name,loop)\
for(int name = 0; name < loop; name++)

#define E_FOR(enum_name)\
for(int index = (int)enum_name::BEGIN; index < (int)enum_name::NUM; index++)

//�p���𗘗p���Ȃ��V���O���g���p�}�N��
#define SINGLETON(ManagerName)\
friend class Allocator;\
public:\
static Pointer<ManagerName> CreateInstance() { return (!Instance_) ? Instance_ = MyNew(ManagerName, AllocType::Manager, 0) : Instance_; }\
static Pointer<ManagerName> GetInstance() { return (Instance_) ? Instance_ : CreateInstance(); }\
static void DeleteInstance() { if (Instance_) MyFree(Instance_) ; }\
private:\
static inline Pointer<ManagerName> Instance_

//�͈͓���
#define CHECK_RANGE(Value, Min, Max) ((Value >= Min) && (Value <= Max))

//enum�̃I�y���[�^�[��`���ȗ�������}�N��
#define ENUM_OQERATOR_MEMBER(enum_class_name, successor, operator_type)\
constexpr bool operator operator_type(enum_class_name a, successor b) {\
	return static_cast<successor>(a) operator_type b;\
}

//enum��enum�̊��^�Ƃ̔�r�A�r�b�g���Z�Ȃǂ̒�`�p�}�N��
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