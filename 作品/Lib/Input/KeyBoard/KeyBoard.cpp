#include "KeyBoard.h"
#include "../../Define.h"
#include "DxLib.h"
#include <xmmintrin.h>
#include <immintrin.h>
#include "../../GameSystem/Thread/CriticalSection.h"

namespace MyLib
{
	namespace
	{
		CriticalSection KeyCs;	//競合対策（もっと軽量の物を使ってもいい）
		std::atomic<bool> update = false;
	}

	CKeyBoardInput::CKeyBoardInput()
	{
		memset(currentBuf_, 0, sizeof(currentBuf_));
		memset(preBuf_, 0, sizeof(preBuf_));
	}

	CKeyBoardInput::~CKeyBoardInput()
	{

	}

	bool CKeyBoardInput::Input(_In_range_(0, 255)u32 keyCode, _In_range_(INPUT_TYPE::Push, INPUT_TYPE::Down)INPUT_TYPE type) const
	{
		switch (type)
		{
		case INPUT_TYPE::Push:	 return (preBuf_[keyCode] == 0 && currentBuf_[keyCode] == 1);
		case INPUT_TYPE::Keep:	 return (preBuf_[keyCode] == 1 && currentBuf_[keyCode] == 1);
		case INPUT_TYPE::Release: return (preBuf_[keyCode] == 1 && currentBuf_[keyCode] == 0);
		case INPUT_TYPE::Down:	 return (currentBuf_[keyCode] == 1);
		default:				 return false;
		}
	}

	INPUT_TYPE CKeyBoardInput::Input(_In_range_(0, 255)u32 keyCode) const
	{
		return  ((preBuf_[keyCode]) != 0 && (currentBuf_[keyCode]) != 0) ? INPUT_TYPE::Keep :
			((preBuf_[keyCode]) == 0) ? INPUT_TYPE::Push :
			((currentBuf_[keyCode]) == 0) ? INPUT_TYPE::Release :
			INPUT_TYPE::Null;
	}

	i32 MyLib::CKeyBoardInput::AnythingInput(INPUT_TYPE type)
	{
		FOR(key_code, KEY_BUF_LEN) {
			if (Input(key_code, type))
				return key_code;
		}

		return -1;
	}

	void CKeyBoardInput::Init()
	{
		memset(currentBuf_, 0, sizeof(currentBuf_));
		memset(preBuf_, 0, sizeof(preBuf_));
		memset(mainKeyBuf_, 0, sizeof(mainKeyBuf_));
	}

	void CKeyBoardInput::Step()
	{
		//前フレームのキー情報変数に記録しておく
		memcpy(preBuf_, currentBuf_, sizeof(char[KEY_BUF_LEN]));
		{
			ScopedCS cs(&KeyCs);
			memcpy(currentBuf_, mainKeyBuf_, sizeof(char[KEY_BUF_LEN]));
			update = true;
		}
	}

	void CKeyBoardInput::MainStep()
	{
		//現在のキー情報を取得
		char buf[KEY_BUF_LEN] = { 0 };
		GetHitKeyStateAll(buf);

		__m256* main_buf = reinterpret_cast<__m256*>(mainKeyBuf_);
		__m256* buf_128 = reinterpret_cast<__m256*>(buf);
		{
			ScopedCS cs(&KeyCs);
			if (update) {
				memset(mainKeyBuf_, 0, sizeof(mainKeyBuf_));
				update = false;
			}
			for (int i = 0; i < KEY_BUF_LEN; i += sizeof(__m256), main_buf++, buf_128++) {
				*main_buf = _mm256_or_ps(*main_buf, *buf_128);
			}
		}
	}

	void CKeyBoardInput::AlwaysStep()
	{
	}

	void CKeyBoardInput::Fin()
	{

	}
}