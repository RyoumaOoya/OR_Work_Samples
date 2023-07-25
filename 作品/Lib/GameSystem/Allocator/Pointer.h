#pragma once
#include <xtr1common>
#include <type_traits>
#include <compare>
#include "../../typedef.h"

namespace MyLib
{
	template<class T>
	class RefPointer;

	//渡す2重ポインタを隠ぺいするためのクラス
	template<class T>
	class Pointer
	{
		friend class Allocator;
		friend class Pointer;
		template<class T>
		friend class RefPointer;
		template<class T>
		friend class OffSetPtr;
	private:
		typedef T Element;
	public:
#ifdef REFERENCE_COUNTER
		Pointer() :p(nullptr), type(AllocType::NullPtr), use_num(nullptr) {}
		Pointer(Element** ptr, AllocType t, int* use_ptr) :p(ptr), type(t), use_num(use_ptr) {
			*use_num = *use_num + 1;
		}
		Pointer(AllocType t) :p(nullptr), type(t), use_num(nullptr) {}
		Pointer(const Pointer& ptr) {
			p = ptr.p;
			type = ptr.type;
			use_num = ptr.use_num;
			if (use_num)
				*use_num = *use_num + 1;
		}
		~Pointer() {
			if (use_num)
				*use_num = *use_num - 1;
		}
#else
		Pointer() :p(nullptr), type(-1) {}
		Pointer(Element** ptr, i64 t) :p(ptr), type(t) {}
		Pointer(i64 t) :p(nullptr), type(t) {}
		Pointer(const Pointer& ptr) {
			p = ptr.p;
			type = ptr.type;
		}
		~Pointer() {}
#endif // REFERENCE_COUNTER
	public:
		_inline Element* operator->() { return (*p); }
		_inline Element& operator*() { return **p; }
		template<class U, std::enable_if_t<std::is_member_function_pointer<U>::value, std::nullptr_t> = nullptr>
		auto operator->*(U u) const noexcept
		{
			return [u, this](auto... args) { return ((*p)->*u)(std::forward<decltype(args)>(args)...); };
		}
		_inline const Element* const operator->() const { return (*p); }
		_inline const Element& operator*() const { return **p; }
		_inline Pointer<Element> operator=(const Pointer<Element>& ptr) noexcept {
			memcpy(this, &ptr, sizeof(*this));
#ifdef REFERENCE_COUNTER
			if (use_num)
				*use_num = *use_num + 1;
#endif // REFERENCE_COUNTER
			return *this;
		}
		template<class T>
		_inline bool operator==(const Pointer<T>& ptr)const { return (p == reinterpret_cast<Element**>(ptr.p)); }
		operator bool() const { return (p != nullptr) ? *p != nullptr : false; }
		_inline Element* get() { return *p; }
		_inline const Element* get() const { return *p; }
		template<class CAST_TYPE>
		explicit(std::is_base_of_v<Element, CAST_TYPE>) operator Pointer<CAST_TYPE>() const noexcept {
#ifdef REFERENCE_COUNTER
			* use_num = *use_num + 1;
			return Pointer<CAST_TYPE>((CAST_TYPE**)p, type);
#else
			return Pointer<CAST_TYPE>(reinterpret_cast<CAST_TYPE**>(p), type);
#endif // REFERENCE_COUNTER
		}
		template<class CAST_TYPE>
		explicit(std::is_base_of_v<Element, CAST_TYPE>) operator RefPointer<CAST_TYPE>() const noexcept {
#ifdef REFERENCE_COUNTER
			* use_num = *use_num + 1;
			return RefPointer<CAST_TYPE>((CAST_TYPE**)p, use_num);
#else
			return RefPointer<CAST_TYPE>(reinterpret_cast<CAST_TYPE**>(p), type);
#endif // REFERENCE_COUNTER
		}
	private:
		_inline explicit operator void** () noexcept { return reinterpret_cast<void**>(p); }
	private:
		Element** p;
	private:
		i64 type;
#ifdef REFERENCE_COUNTER
		int* use_num;
#endif
	};

	//メンバ型を参照するためのポインタクラス
	template<class T>
	class OffSetPtr
	{
	private:
		typedef T Element;
	public:
		OffSetPtr() :p(nullptr), offset(0) {}
		template<class U>
		OffSetPtr(const Pointer<U> ptr, const Element* var) { 
			p = reinterpret_cast<Element**>(ptr.p); 
			offset = static_cast<i64>(reinterpret_cast<const char*>(var) - reinterpret_cast<const char*>(*ptr.p));
			if (offset >= sizeof(U))
				throw;
		}
		~OffSetPtr() {}
	public:
		_inline Element* operator->() { return get_offset_ptr(); }
		_inline Element& operator*() { return *get_offset_ptr(); }
		template<class U, std::enable_if_t<std::is_member_function_pointer<U>::value, std::nullptr_t> = nullptr>
		auto operator->*(const U& u) const noexcept
		{
			return [u, this](auto... args) { return ((get_offset_ptr())->*u)(std::forward<decltype(args)>(args)...); };
		}
		template<class U, std::enable_if_t<std::is_member_function_pointer<U>::value, std::nullptr_t> = nullptr>
		auto operator->*(U&& u) const noexcept
		{
			return [u, this](auto... args) { return ((get_offset_ptr())->*u)(std::forward<decltype(args)>(args)...); };
		}
		_inline const Element* const operator->() const { return get_offset_ptr(); }
		_inline const Element& operator*() const { return *get_offset_ptr(); }
		template<class T>
		_inline bool operator==(const OffSetPtr<T>& ptr)const { return (get_offset_ptr() == reinterpret_cast<Element*>(ptr.get_offset_ptr())); }
		operator bool() const { return (p != nullptr) ? *p != nullptr : false; }
		_inline explicit operator Element* () { return get_offset_ptr(); }
		_inline explicit operator const Element* () const { return get_offset_ptr(); }
		_inline Element* get() { return get_offset_ptr(); }
		_inline const Element* get() const { return get_offset_ptr(); }
		auto operator<=>(const OffSetPtr<Element>& b)const {
			if (get_offset_ptr() != b.get_offset_ptr()) {
				return get_offset_ptr() <=> b.get_offset_ptr();
			}
			return std::strong_ordering::equivalent;
		}

		template<class CAST_TYPE>
		explicit operator OffSetPtr<CAST_TYPE>() const noexcept {
			return OffSetPtr<CAST_TYPE>(reinterpret_cast<CAST_TYPE**>(p), offset);
		}
	private:
		_inline Element* get_offset_ptr() const { return reinterpret_cast<Element*>(reinterpret_cast<char*>(*p) + offset); }
	private:
		Element** p;
		i64 offset;
	};

	//参照用ポインタ(未使用)
	template<class T>
	class RefPointer
	{
	private:
		typedef T Element;
	public:
#ifdef REFERENCE_COUNTER
		RefPointer() :p(nullptr), use_num(nullptr) {};
		RefPointer(const Pointer<Element>& ptr) {
			p = ptr.p;
			use_num = ptr.use_num;
			if (use_num)
				*use_num = *use_num + 1;
		}
		~RefPointer() {
			if (use_num)
				*use_num = *use_num - 1;
		}
#else
		RefPointer() :p(nullptr) {};
		RefPointer(const Pointer<Element>& ptr) { p = ptr.p; }
		~RefPointer() {}
#endif // REFERENCE_COUNTER
	public:
		_inline Element* operator->() {
			return (*p);
		}
		_inline Element& operator*() {
			return **p;
		}
		_inline const Element* operator->() const {
			return (*p);
		}
		_inline const Element& operator*() const {
			return **p;
		}
		_inline RefPointer<Element> operator=(const RefPointer<Element>& ptr) {
			memcpy(this, &ptr, sizeof(*this));
#ifdef REFERENCE_COUNTER
			if (use_num)
				*use_num = *use_num + 1;
#endif // REFERENCE_COUNTER
			return *this;
		}
		_inline RefPointer<Element> operator=(const Pointer<Element>& ptr) {
			p = ptr.p;
#ifdef REFERENCE_COUNTER
			use_num = ptr.use_num;
			if (use_num)
				*use_num = *use_num + 1;
#endif // REFERENCE_COUNTER
			return *this;
		}
		template<class T>
		_inline bool operator==(const RefPointer<T> ptr)const {
			return (p == (Element**)ptr.p);
		}
		template<class T>
		_inline bool operator==(const Pointer<T> ptr)const {
			return (p == (Element**)ptr.p);
		}
		operator bool() const {
#ifdef REFERENCE_COUNTER
			return (p != nullptr) ? *p != nullptr && *use_num != 0 : false;
#else
			return (p != nullptr) ? *p != nullptr : false;
#endif
		}
		_inline explicit operator Element* () const {
			return *p;
		}
		Element* get() const {
			return *p;
		}
		template<class T>
		explicit operator RefPointer<T>() const {
#ifdef REFERENCE_COUNTER
			* use_num = *use_num + 1;
			return RefPointer<T>((T**)p, use_num);
#else
			return RefPointer<T>(reinterpret_cast<T**>(p));
#endif
		}
	private:
		Element** p;
#ifdef REFERENCE_COUNTER
		int* use_num;
#endif
	};
}

using namespace MyLib;

namespace Size {
	class dummy;
	constexpr size_t POINTER = sizeof(MyLib::Pointer<dummy>);
}