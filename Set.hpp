#pragma once

#include "RBTree.hpp"

namespace du
{
	template<class K>

	class set
	{
		typedef K ValueType;
		struct KOFD
		{
			const K& operator()(const ValueType& data)
			{
				return data;
			}
		};
		typedef RBTree<ValueType, KOFD> RBT;
		typedef typename RBT::iterator iterator;
	public:
		set()
			:_t()
		{}

		//iterator
		iterator begin()
		{
			return _t.begin();
		}

		iterator end()
		{
			return _t.end();
		}

		//capacity
		size_t size()const
		{
			return _t.size();
		}

		bool empty()const
		{
			return _t.empty();
		}

		//modify
		pair<iterator, bool> insert(const ValueType& data)
		{
			return _t.Insert(data);
		}

		void swap(set<K>& m)
		{
			_t.Swap(m._t);
		}

		void clear()
		{
			_t.Clear();
		}

		iterator find(const K& key)
		{
			return _t.Find(key);
		}

	private:
		RBT _t;
	};
}


#include <string>
#include<iostream>
using namespace std;

void TestSet()
{
	du::set<string> s;
	s.insert("orage");
	s.insert("apple");
	s.insert("banana");
	cout << s.size() << endl;

	for (auto e : s)
		cout << e << " ";
	cout << endl;

	auto ret = s.insert("apple");
	if (ret.second)
		cout << "apple is not in set" << endl;
	else
		cout << "apple is in set" << endl;

	s.clear();
	cout << s.size() << endl;
}