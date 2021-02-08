#pragma once

#include <iostream>
#include <vector>

using namespace std;

//哈希桶中管理的是一个个的列表
template <class T>
struct HashBucketNode
{
	HashBucketNode(const T& x = T())
		:next(nullptr)
		,data(x)
	{}
	HashBucketNode<T>* next;
	T data;
};

template <class T>
class HashBucket
{
	typedef HashBucketNode<T> Node;

public:
	HashBucket(size_t capacity = 10)
		:_table(capacity)
		,_size(0)
	{}
	
	~HashBucket()
	{
		Clear();
	}

	bool InsertUnique(const T& data)
	{
		//通过哈希函数计算元素所在桶号
		size_t bucketNo = HashFunc(data);

		//检测data是否在bucket桶中存在
		Node* cur = _table[bucketNo];
		while (cur)
		{
			if (cur->data == data)
				return false;

			cur = cur->next;
		}

		//插入新节点--头插
		cur = new Node(data);
		cur->next = _table[bucketNo];
		_table[bucketNo] = cur;
		_size++;

		return true;
	}

	bool InsertEqual(const T& data)
	{
		//通过哈希函数计算元素所在桶号
		size_t bucketNo = HashFunc(data);
		Node* cur = _table[bucketNo];

		//插入新节点--头插
		cur = new Node(data);
		cur->next = _table[bucketNo];
		_table[bucketNo] = cur;
		_size++;

		return true;
	}

	Node* Find(const T& data)
	{
		//通过哈希函数计算元素所在桶号
		size_t bucketNo = HashFunc(data);

		//查找data在bucket桶中的位置
		Node* cur = _table[bucketNo];
		while (cur)
		{
			if (cur->data == data)
				return cur;

			cur = cur->next;
		}
		return nullptr;
	}

	//删除值为data的元素
	//返回值为：删除节点的个数
	size_t Erase(const T& data)
	{
		//通过哈希函数计算元素所在桶号
		size_t bucketNo = HashFunc(data);

		Node* cur = _table[bucketNo];
		Node* prev = nullptr;	//保存cur的前一个节点
		size_t count = 0;
		while (cur)
		{
			if (cur->data == data)
			{
				//删除节点
				if (nullptr == prev)
				{
					_table[bucketNo] = cur->next;
					delete cur;
					_size--;
					cur = _table[bucketNo];
					count++;
				}
				else
				{
					prev->next = cur->next;
					delete cur;
					_size--;
					cur = prev->next;
					count++;
				}
			}
			else
			{
				prev = cur;
				cur = cur->next;
			}
		}
		return count;
	}

	size_t Size()const
	{
		return _size;
	}

	bool Empty()const
	{
		return 0 == _size;
	}

	//删除所有节点
	void Clear()
	{
		for (size_t bucketNo = 0; bucketNo < _table.capacity(); bucketNo++)
		{
			Node* cur = _table[bucketNo];

			while (cur)
			{
				_table[bucketNo] = cur->next;
				delete cur;
				cur = _table[bucketNo];
			}
		}

		_size = 0;
	}

	void Print()
	{
		for (size_t bucketNo = 0; bucketNo < _table.capacity(); bucketNo++)
		{
			cout << '[' << bucketNo << "]:";
			Node* cur = _table[bucketNo];
			while (cur)
			{
				cout << cur->data << "--->";
				cur = cur->next;
			}

			cout << "NULL" << endl;
		}
	}

private:
	size_t HashFunc(const T& data)
	{
		return data % _table.capacity();
	}
private:
	vector<Node*> _table;
	size_t _size;
};

void Test()
{
	int arr[] = { 19,14,23,1,68,20,84,27,55,11,10,79 };
	HashBucket<int> ht(7);

	for (auto& e : arr)
		ht.InsertUnique(e);

	ht.Print();
}