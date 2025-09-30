#pragma once
#include <mutex>

template<typename T>
class LockQueue
{
public:
	LockQueue() {}
	LockQueue(const LockQueue&) = delete;
	LockQueue& operator = (const LockQueue&) = delete;
	
	void Push(T value)
	{
		lock_guard<mutex> lock(_mutex);
		_queue.push(std::move(value));
		_condVar.notify_one();
	}

	bool TryPop(T& value)
	{
		lock_guard<mutex> lock(_mutex);

		if (_queue.empty())
			return false;


		value = std::move(_queue.front());
		_queue.pop();

		return true;
	}

	void WaitPop(T& value)
	{
		unique_lock<mutex> lock(_mutex);
		_condVar.wait(lock, [this] {return _queue.empty() == false; });
		value = std::move(_queue.front());
		_queue.pop();
	}

private:
	queue<T> _queue;
	mutex _mutex;
	condition_variable _condVar;
};

//
//template<typename T>
//class LockFreeQueue
//{
//	struct Node
//	{
//		shared_ptr<T> data;
//		Node* next = nullptr;
//	};
//
//public:
//	LockFreeQueue() : _head(new Node), _tail(_head)
//	{
//
//	}
//
//	LockFreeQueue(const LockFreeQueue&) = delete;
//	LockFreeQueue operator=(const LockFreeQueue&) = delete;
//
//	void Push(const T& value)
//	{
//		shared_ptr<T> newData = make_shared<T>(value);
//
//		Node* dummy = new Node();
//
//		Node* oldTail = _tail;
//		oldTail->data.swap(newData);
//		oldTail->next = dummy;
//
//		_tail = dummy;
//	}
//
//	shared_ptr<T> TryPop()
//	{
//		if(oldHead == nullptr)
//			return shared_ptr<T>();
//
//		shared_ptr<T> res(oldHead->data);
//		delete oldHead;
//		return res;
//	}
//	
//
//private:
//	Node* PopHead()
//	{
//		Node* oldHead = _head;
//		if (oldHead == _tail)
//			return nullptr;
//
//		_head = oldHead->next;
//		return oldHead;
//	}
//
//
//
//private:
//	Node* _head = nullptr;
//	Node* _tail = nullptr;
//};
// 단일 스레드에서는 문제 없음.
// but 멀티스레드에선 문제 많음. 경합이 붙는 부분이 많음
// 스택에서는 푸쉬에서 데이터를 만든 다음 경합을 통해서 데이터를
// 밀어넣지만 큐에서는 공용으로 갖고 있는 더미노드를 건드리려 할 것이고
// 팝을 할 때는 공용으로 사용하고 있는 첫번째 값을 사용하기 때문에
// 양쪽에 다 경합이 붙는 상황이다.
// 결: Push에서 tail을 건드리는 순간이 위험하고
// 팝하는 경우도 마찬가지로 head를 사용해서 하는 부분 자체가
// 멀티스레드 환경에서 경합이 붙어 여러명이 한다면 정상적으로 작동하지 않음



template<typename T>
class LockFreeQueue
{
	struct Node
	{
		shared_ptr<T> data;
		Node* next = nullptr;
	};

public:
	LockFreeQueue() : _head(new Node), _tail(_head)
	{

	}

	LockFreeQueue(const LockFreeQueue&) = delete;
	LockFreeQueue operator=(const LockFreeQueue&) = delete;

	void Push(const T& value)	// 스택과 다르게 Q에서는 푸쉬할때, 팝할때 둘다 동일하게 더미노드에 경합이 생김
	{
		shared_ptr<T> newData = make_shared<T>(value);

		Node* dummy = new Node();

		Node* oldTail = _tail;	// Tail에서 경합이 붙는 부분
		oldTail->data.swap(newData);
		oldTail->next = dummy;

		_tail = dummy;
	}

	shared_ptr<T> TryPop()
	{
		Node* oldHead = PopHead();
		if (oldHead == nullptr)
			return shared_ptr<T>();

		shared_ptr<T> res(oldHead->data);
		delete oldHead;
		return res;
	}


private:
	Node* PopHead()
	{
		Node* oldHead = _head;		//멀티스레드 환경에서 정상적으로 동작할 수 없음
		if (oldHead == _tail)
			return nullptr;

		_head = oldHead->next;
		return oldHead;
	}



private:
	Node* _head = nullptr;
	Node* _tail = nullptr;
};