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
using InventoryRef = TSharedPtr<Inventory>;

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
	InventoryRef _inventory = nullptr;
};

class Inventory : public RefCountable
{
public:
	Inventory(KnightRef knight) : _knight(knight)
	{

	}

	KnightRef _knight;
};

int main()
{
	// 1) 이미 만들어진 클래스 대상으로 사용 불가
	// 2) 순환 (Cycle) 문제 (표준 SharedPtr도 생기는 문제)

	KnightRef k1(new Knight());
	k1->ReleaseRef();
	
	k1->_inventory = new Inventory(k1);

	

}