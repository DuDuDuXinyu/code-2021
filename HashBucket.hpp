#pragma once

#include <iostream>
#include <vector>

#include "common.hpp"

using namespace std;

//��ϣͰ�й������һ�������б�
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
			//_nodeΪ��������һ���ڵ�
			//Ѱ����һ���ǿ�Ͱ
			//��Ҫ֪��_node���ڵ�Ͱ��
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
		//��һ���ǿ�Ͱ���׽ڵ��λ��
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

		//ͨ����ϣ��������Ԫ������Ͱ��
		size_t bucketNo = HashFunc(_table.capacity(), data);

		//���data�Ƿ���bucketͰ�д���
		Node* cur = _table[bucketNo];
		while (cur)
		{
			if (cur->data == data)
				return false;

			cur = cur->next;
		}

		//�����½ڵ�--ͷ��
		cur = new Node(data);
		cur->next = _table[bucketNo];
		_table[bucketNo] = cur;
		_size++;

		return true;
	}

	bool InsertEqual(const T& data)
	{
		CheckCapacity();

		//ͨ����ϣ��������Ԫ������Ͱ��
		size_t bucketNo = HashFunc(_table.capacity(), data);
		Node* cur = _table[bucketNo];

		//�����½ڵ�--ͷ��
		cur = new Node(data);
		cur->next = _table[bucketNo];
		_table[bucketNo] = cur;
		_size++;

		return true;
	}

	Node* Find(const T& data)
	{
		//ͨ����ϣ��������Ԫ������Ͱ��
		size_t bucketNo = HashFunc(_table.capacity(), data);

		//����data��bucketͰ�е�λ��
		Node* cur = _table[bucketNo];
		while (cur)
		{
			if (cur->data == data)
				return cur;

			cur = cur->next;
		}
		return nullptr;
	}

	//ɾ��ֵΪdata��Ԫ��
	//����ֵΪ��ɾ���ڵ�ĸ���
	size_t Erase(const T& data)
	{
		//ͨ����ϣ��������Ԫ������Ͱ��
		size_t bucketNo = HashFunc(_table.capacity(), data);

		Node* cur = _table[bucketNo];
		Node* prev = nullptr;	//����cur��ǰһ���ڵ�
		size_t count = 0;
		while (cur)
		{
			if (cur->data == data)
			{
				//ɾ���ڵ�
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

	//ɾ�����нڵ�
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
		//������ݷ������Ǻܺã������ݵ�ʱ��ڵ�洢������
		/*
		if (_size == _table.capacity())
		{
			//��Ͱ�ĸ�������
			HashBucket<T,DToI> newHt(_table.capacity() * 2);

			//���ɹ�ϣͰ��Ԫ�ز����µ�Ͱ��
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

		//�õ����ݷ������Ѿ�Ͱ�еĽڵ���Ƶ���Ͱ��
		if (_size == _table.capacity())
		{
			vector<Node*> newTable(GetNextPrime(_table.capacity() * 2));
			for (size_t i = 0; i < _table.capacity(); i++)
			{
				Node* cur = _table[i];
				while (cur)
				{
					//����Ͱ�Ľڵ������
					_table[i] = cur->next;

					//���뵽��Ͱ��
					size_t bucketNo = HashFunc(newTable.capacity(), cur->data);
					cur->next = newTable[bucketNo];
					newTable[bucketNo] = cur;

					//ȡ��һ���ڵ�
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