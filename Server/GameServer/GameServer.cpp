#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <future>
#include <windows.h>
#include "ThreadManager.h"

#include "RefCounting.h"

class Wraight : public RefCountable
{
public:
	int _hp = 150;
	int _posX = 0;
	int _posY = 0;
};

class Missile : public RefCountable
{
public:
	void SetTarget(Wraight* target)
	{
		_target = target;
		target->AddRef();
	}

	bool Update()
	{
		if (_target == nullptr)
			return true;

		int posX = _target->_posX;
		int posY = _target->_posY;

		// TODO : 좇아간다

		if(_target->_hp == 0)
		{
			_target->ReleaseRef();
			_target = nullptr;
			return false;
		}
	}

	Wraight* _target = nullptr;
};



int main()
{
	Wraight* wraight = new Wraight();
	Missile* missile = new Missile();
	missile->SetTarget(wraight);

	// 레이스가 피격 당함
	wraight->_hp = 0;
	//delete wraight;
	wraight->ReleaseRef();
	wraight = nullptr;

	while (true)
	{
		if (missile->Update())
		{
			missile->ReleaseRef();
			missile = nullptr;
		}
	}

	missile->ReleaseRef();
	missile = nullptr;
	//delete missile;
}