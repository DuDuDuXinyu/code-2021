#pragma once

#include "RBTree.hpp"

namespace du
{
	template<class K, class V>

	class map
	{
		typedef pair<K, V> ValueType;
		struct KOFD
		{
			const K& operator()(const ValueType& data)
			{
				return data.first;
			}
		};

		typedef RBTree<ValueType,KOFD> RBT;
	public:
		typedef typename RBT::iterator iterator;
	public:
		map()
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

		void swap(map<K, V>& m)
		{
			_t.Swap(m._t);
		}

		void clear()
		{
			_t.Clear();
		}

		iterator find(const K& key)
		{
			return _t.Find(make_pair(key, V()));
		}

		V& operator[](const K& key)
		{
			return (*(_t.Insert(make_pair(key, V()))).first).second;
		}

	private:
		RBT _t;
	};
}	

#include <string>
#include<iostream>
using namespace std;

void TestMap()
{
	du::map < string, string> m;
	m.insert(make_pair("orange", "³È×Ó"));
	m.insert(make_pair("banana", "Ïã½¶"));
	m.insert(make_pair("apple", "Æ»¹û"));

	cout << m.size() << endl;
	cout << m["apple"] << endl;

	m["peach"] = "ÌÒ×Ó";
	auto it = m.begin();
	while (it != m.end())
	{
		cout << "[" << it->first << "," << (*it).second << "]" << endl;
		it++;
	}
	cout << endl;

	it = m.find("grape");
	if (it != m.end())
		cout << "grape is in map" << endl;
	else
		cout << "grape is not in map" << endl;

	m.clear();
	cout << m.size() << endl;
}