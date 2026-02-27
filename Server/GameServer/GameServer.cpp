#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <future>
#include <windows.h>
#include "ThreadManager.h"

#include "RefCounting.h"

using KnightRef = TSharedPtr<Knight>;

class Knight : public RefCountable
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

	void SetTarget(KnightRef target)
	{
		_target = target;
	}

	KnightRef _target = nullptr;
};


int main()
{
	// 1) 이미 만들어진 클래스 대상으로 사용 불가
	// 2) 순환 (Cycle) 문제 (표준 SharedPtr도 생기는 문제)

	KnightRef k1(new Knight());
	k1->ReleaseRef();
	KnightRef k2(new Knight());
	k2->ReleaseRef();

	k1->SetTarget(k2);
	k2->SetTarget(k1);

	k1->SetTarget(nullptr);
	k2->SetTarget(nullptr);
	
	k1 = nullptr;
	k2 = nullptr;

	

}