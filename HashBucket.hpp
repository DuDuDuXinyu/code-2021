#pragma once

#include <iostream>
#include <vector>

#include "common.hpp"

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

template <class T, class DToI = DToIntDef<T>>
class HashBucket;

template <class T, class DToI = DToIntDef<T>>
struct HashBucketIterator
{
	typedef HashBucketNode<T> Node;
	typedef HashBucketIterator<T, DToI> Self;
public:
	HashBucketIterator(Node* node = nullptr, HashBucket<T,DToI>* ht = nullptr)
		:_node(node)
		,_ht(ht)
	{}

	T& operator* ()
	{
		return _node->data;
	}

	T* operator->()
	{
		return *(operator*());
	}

	Self& operator++()
	{
		Next();
		return *this;
	}

	Self operator++(int)
	{
		Self temp(*this);
		Next();
		return temp;
	}

	bool operator==(const Self& s)const
	{
		return _node == s._node;
	}

	bool operator!=(const Self& s)const
	{
		return _node != s._node;
	}

private:
	void Next()
	{
		if (_node->next)
			_node = _node->next;
		else
		{
			//_node为链表的最后一个节点
			//寻找下一个非空桶
			//需要知道_node所在的桶号
			size_t bucketNo = _ht->HashFunc(_ht->BucketCount(), _node->data) + 1;
			for (; bucketNo < _ht->BucketCount(); bucketNo++)
			{
				_node = _ht->_table[bucketNo];
				if (_node)
					return;
			}
			_node = nullptr;
		}
	}
private:
	Node* _node;
	HashBucket<T, DToI>* _ht;
};

template <class T, class DToI>
class HashBucket
{
	friend HashBucketIterator<T, DToI>;
	typedef HashBucketNode<T> Node;
public:
	typedef HashBucketIterator<T, DToI> iterator;
public:
	HashBucket(size_t capacity = 10)
		:_table(GetNextPrime(capacity))
		,_size(0)
	{}
	
	~HashBucket()
	{
		Clear();
	}

	iterator begin()
	{
		//第一个非空桶的首节点的位置
		for (size_t bucketNo = 0; bucketNo < BucketCount(); bucketNo++)
		{
			if (_table[bucketNo])
				return iterator(_table[bucketNo], this);
		}
		return end();
	}

	iterator end()
	{
		return iterator(nullptr, this);
	}

	bool InsertUnique(const T& data)
	{
		CheckCapacity();

		//通过哈希函数计算元素所在桶号
		size_t bucketNo = HashFunc(_table.capacity(), data);

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
		CheckCapacity();

		//通过哈希函数计算元素所在桶号
		size_t bucketNo = HashFunc(_table.capacity(), data);
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
		size_t bucketNo = HashFunc(_table.capacity(), data);

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
		size_t bucketNo = HashFunc(_table.capacity(), data);

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

	void Swap(HashBucket<T,DToI>& newHt)
	{
		_table.swap(newHt._table);
		swap(_size, newHt._size);
	}

	size_t BucketCount()const
	{
		return _table.capacity();
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
	void CheckCapacity()
	{
		//这个扩容方法不是很好，在扩容的时候节点存储了俩份
		/*
		if (_size == _table.capacity())
		{
			//将桶的个数翻倍
			HashBucket<T,DToI> newHt(_table.capacity() * 2);

			//将旧哈希桶的元素插入新的桶中
			for (size_t i = 0; i < _table.capacity(); i++)
			{
				Node* cur = _table[i];
				while (cur)
				{
					newHt.InsertEqual(cur->data);
					cur = cur->next;
				}
			}
			this->Swap(newHt);
		}*/

		//好的扩容方法：把旧桶中的节点搬移到新桶中
		if (_size == _table.capacity())
		{
			vector<Node*> newTable(GetNextPrime(_table.capacity() * 2));
			for (size_t i = 0; i < _table.capacity(); i++)
			{
				Node* cur = _table[i];
				while (cur)
				{
					//将旧桶的节点拆下来
					_table[i] = cur->next;

					//插入到新桶中
					size_t bucketNo = HashFunc(newTable.capacity(), cur->data);
					cur->next = newTable[bucketNo];
					newTable[bucketNo] = cur;

					//取下一个节点
					cur = _table[i];
				}
			}
			_table.swap(newTable);
		}
	}

	size_t HashFunc(size_t capacity, const T& data)
	{
		DToI dToInt;
		return dToInt(data) % capacity;
	}
private:
	vector<Node*> _table;
	size_t _size;
};

void Test1()
{
	int arr[] = { 19,14,23,1,68,20,84,27,55,11 };
	HashBucket<int> ht;

	for (auto& e : arr)
		ht.InsertUnique(e);

	ht.Print();

	ht.InsertUnique(10);
	ht.InsertUnique(79);

	ht.Print();

	auto it = ht.begin();
	while (it != ht.end())
	{
		cout << *it << " ";
		it++;
	}
	cout << endl;

	for (auto e : ht)
		cout << e << " ";
	cout << endl;
}

void Test2()
{
	HashBucket<string,Str2Int> ht;

	ht.InsertEqual("apple");
	ht.InsertEqual("orange");
	ht.InsertEqual("banana");

	ht.Print();
}