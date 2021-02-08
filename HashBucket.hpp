#pragma once

#include <iostream>
#include <vector>

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
		//ͨ����ϣ��������Ԫ������Ͱ��
		size_t bucketNo = HashFunc(data);

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
		//ͨ����ϣ��������Ԫ������Ͱ��
		size_t bucketNo = HashFunc(data);
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
		size_t bucketNo = HashFunc(data);

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
		size_t bucketNo = HashFunc(data);

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