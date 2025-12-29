#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"

void Lock::WriteLock()
{
	// 아무도 소유 및 공유하고 있지 않을 때, 경합해서 소유권을 얻는다.

}

void Lock::WriteUnlock()
{

}

void Lock::ReadLock()
{

}

void Lock::ReadUnlock()
{

}
