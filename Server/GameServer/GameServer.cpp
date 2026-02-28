#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <future>
#include <windows.h>
#include "ThreadManager.h"

#include "RefCounting.h"


class Knight
{
public:
	Knight()
	{
		cout << "Knight()" << endl;
	}

	~Knight()
	{
		cout << "~Knight()" << endl;
	}

};


int main()
{
	// 1) 이미 만들어진 클래스 대상으로 사용 불가
	// 2) 순환 (Cycle) 문제 (표준 SharedPtr도 생기는 문제)



	// shared_ptr
	// weak_ptr
	
	// [Knight | RefCountingBlock(uses, weak)]

	// [T*][RefCountBlock*]

	// weak_ptr는 Knight 수명 자체에는 영향을 주지 않는 반쪽자리 스마트 포인터다
	// 객체가 사라졌는지 알기위한 RefCountingBlock만 참조한다.

	// RefCountBlock(useCount(shared), weakCount)
	shared_ptr<Knight> spr = make_shared<Knight>();
	weak_ptr<Knight> wpr = spr;

	bool expired = wpr.expired();
	shared_ptr<Knight> spr2 = wpr.lock();
	if (spr2 != nullptr)
	{

	}
}