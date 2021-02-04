#pragma once
#include <iostream>
using namespace std;

enum Color{RED, BLACK};

//红黑树节点的定义
template <class T>
struct RBTreeNode
{
	RBTreeNode(const T& x = T(), Color c = RED)
		:left(nullptr)
		, right(nullptr)
		, parent(nullptr)
		, data(x)
		, color(c)
	{}

	RBTreeNode<T>* left;
	RBTreeNode<T>* right;
	RBTreeNode<T>* parent;
	T data;
	Color color;
};

template<class T>
struct RBTreeIterator
{
	typedef RBTreeNode<T> Node;
	typedef RBTreeIterator<T> Self;
public:
	RBTreeIterator(Node* n = nullptr)
		:node(n)
	{}

	T& operator*()
	{
		return node->data;
	}

	T* operator->()
	{
		return &(operator*());
	}

	Self& operator++()	//前置++
	{
		Increment();
		return *this;
	}

	Self operator++(int)	//后置++
	{
		Self temp(*this);
		Increment();
		return temp;
	}

	Self& operator--()	//前置--
	{
		Decrement();
		return *this;
	}

	Self operator--(int)	//后置--
	{
		Self temp(*this);
		Decrement();
		return temp;
	}

	//找到当前迭代器的下一个位置
	void Increment()
	{
		if (node->right)
		{
			//node右子树存在，在右子树中找
			node = node->right;
			while (node->left)
				node = node->left;
		}
		else
		{
			//不存在，找node的双亲
			Node* parent = node->parent;
			while (node == parent->right)
			{
				node = parent;
				parent = node->parent;
			}
			
			//当node是parent的左孩子的时候，循环结束
			if (node->right != parent)
				node = parent;
		}
	}

	//找到当前迭代器的上一个位置
	void Decrement()
	{
		if (node->parent->parent == node && node->color == RED)
		{
			//node在end的位置
			node = node->right;
		}
		else if (node->left)
		{
			node = node->left;
			while (node->right)
				node = node->right;
		}
		else
		{
			Node* parent = node->parent;
			while (node == parent->left)
			{
				node = parent;
				parent = node->parent;
			}
			node = parent;
		}
	}

	bool operator !=(const Self& s)const
	{
		return node != s.node;
	}
	bool operator ==(const Self& s)const
	{
		return node == s.node;
	}

private:
	Node* node;
};

template <class T, class KOFD>
class RBTree
{
	typedef RBTreeNode<T> Node;
public:
	typedef RBTreeIterator<T> iterator;
public:
	RBTree()
		:_size(0)
	{
		head = new Node();
		head->left = head;
		head->right = head;
		head->parent = nullptr;
	}

	~RBTree()
	{
		Destroy(GetRoot());
		delete head;
		head = nullptr;
		_size = 0;
	}

	iterator begin()
	{
		return iterator(head->left);
	}

	iterator end()
	{
		return iterator(head);
	}

	size_t size()const
	{
		return _size;
	}

	bool empty()const
	{
		return nullptr == head->parent;
	}

	pair<iterator,bool> Insert(const T& data)
	{
		//按照二叉搜索树的规则插入新节点
		Node*& root = GetRoot();

		//空树
		if (nullptr == root)
		{
			root = new Node(data, BLACK);
			root->parent = head;
			head->left = root;
			head->right = root;
			_size = 1;
			return make_pair(iterator(root), true);
		}

		//找到待插入节点在红黑树中的位置
		Node* cur = root;
		Node* parent = head;
		KOFD key;
		while (cur)
		{
			parent = cur;
			//注意使用data中的key来进行比较
			if (key(data) < key(cur->data))
				cur = cur->left;
			else if (key(data) > key(cur->data))
				cur = cur->right;
			else
				return make_pair(iterator(cur), false);
		}

		//插入新节点
		Node* newNode = new Node(data);
		cur = newNode;
		if (key(data) < key(parent->data))
			parent->left = cur;
		else
			parent->right = cur;

		cur->parent = parent;

		//检测新插入节点与其双亲节点的颜色是否都为红色
		while (parent != head && RED == parent->color)
		{
			Node* grandFather = parent->parent;
			if (parent == grandFather->left)
			{
				Node* uncle = grandFather->right;
				if (uncle && RED == uncle->color)
				{
					//情况一：叔叔节点存在且为红
					parent->color = BLACK;
					uncle->color = BLACK;
					grandFather->color = RED;
					cur = grandFather;
					parent = cur->parent;
				}
				else
				{
					//叔叔节点不存在-->情况二
					//或者叔叔节点存在且为黑色-->情况三
					if (cur == parent->right)
					{
						//情况三：
						RotateLeft(parent);
						swap(parent, cur);
					}
					//情况二：
					grandFather->color = RED;
					parent->color = BLACK;
					RotateRight(grandFather);
				}
			}
			else
			{
				Node* uncle = grandFather->left;
				if (uncle && RED == uncle->color)
				{
					//情况一：叔叔节点存在且为红
					parent->color = BLACK;
					uncle->color = BLACK;
					grandFather->color = RED;
					cur = grandFather;
					parent = cur->parent;
				}
				else
				{
					//叔叔节点不存在-->情况二
					//或者叔叔节点存在且为黑色-->情况三
					if (cur == parent->left)
					{
						//情况三：
						RotateRight(parent);
						swap(parent, cur);
					}
					//情况二：
					grandFather->color = RED;
					parent->color = BLACK;
					RotateLeft(grandFather);
				}
			}
		}

		//新节点插入后，红色树中的最大或者最小节点发送变化
		root->color = BLACK;
		head->left = LeftMost();
		head->right = RightMost();
		_size++;

		return make_pair(iterator(newNode), true);
	}

	void Swap(RBTree<T, KOFD>& t)
	{
		swap(head, t.head);
		swap(_size, t._size);
	}

	void Clear()
	{
		Destroy(GetRoot());
		_size = 0;
	}

	iterator Find(const T& data)
	{
		Node* cur = GetRoot();
		KOFD key;
		while (cur)
		{
			if (key(data) == key(cur->data))
				return iterator(cur);
			else if (key(data) < key(cur->data))
				cur = cur->left;
			else
				cur = cur->right;
		}
		return end();
	}

	void InOrder()
	{
		InOrder(GetRoot());
	}

	bool IsValidRBTree()
	{
		Node* root = GetRoot();
		if (nullptr == root)
			return true;

		if (BLACK != root->color)
		{
			cout << "违反了性质二：根节点的颜色不是黑色的" << endl;
			return false;
		}

		size_t blackCount = 0;
		Node* cur = root;
		while (cur)
		{
			if (BLACK == cur->color)
				blackCount++;
			cur = cur->left;
		}
		size_t pathBlackCount = 0;
		return _IsValidRBTree(root, pathBlackCount, blackCount);
	}

private:
	bool _IsValidRBTree(Node* root, size_t pathBlackCount, const size_t blackCount)
	{
		if (nullptr == root)
			return true;

		if (BLACK == root->color)
			pathBlackCount++;

		Node* parent = root->parent;
		//head != parent说明root不是根节点，parent为有效节点
		if (head != parent && RED == parent->color && RED == root->color)
		{
			cout << "违反了性质三：存在连在一起的红色节点" << endl;
			return false;
		}

		if (nullptr == root->left && nullptr == root->right)
		{
			if (pathBlackCount != blackCount)
			{
				cout << "违反了性质四：路径中黑色节点的个数不一样" << endl;
				return false;
			}
		}
		return _IsValidRBTree(root->left, pathBlackCount, blackCount)&&
			_IsValidRBTree(root->right, pathBlackCount, blackCount);
	}

	void RotateLeft(Node* parent)
	{
		Node* subR = parent->right;
		Node* subRL = subR->left;
		
		parent->right = subRL;
		if (subRL)
			subRL->parent = parent;

		subR->left = parent;

		Node* pparent = parent->parent;
		parent->parent = subR;
		subR->parent = pparent;
		//需要处理旋转之前parent双亲的情况
		if (head == pparent)
		{
			//旋转之前parent是根节点
			head->parent = subR;
		}
		else
		{
			//旋转之前有双亲
			if (parent == pparent->left)
				pparent->left = subR;
			else
				pparent->right = subR;
		}
	}

	void RotateRight(Node* parent)
	{
		Node* subL = parent->left;
		Node* subLR = subL->right;

		parent->left = subLR;
		if (subLR)
			subLR->parent = parent;
		subL->right = parent;

		Node* pparent = parent->parent;
		parent->parent = subL;
		subL->parent = pparent;

		if (head == pparent)
			head->parent = subL;
		else
		{
			if (parent == pparent->left)
				pparent->left = subL;
			else
				pparent->right = subL;
		}
	}

	Node*& GetRoot()
	{
		return head->parent;
	}

	Node* LeftMost()
	{
		Node* root = GetRoot();
		if (nullptr == root)
			return head;

		Node* cur = root;
		while (cur->left)
			cur = cur->left;

		return cur;
	}

	Node* RightMost()
	{
		Node* root = GetRoot();
		if (nullptr == root)
			return head;

		Node* cur = root;
		while (cur->right)
			cur = cur->right;

		return cur;
	}

	void InOrder(Node* root)
	{
		if (root)
		{
			InOrder(root->left);
			cout << root->data << " ";
			InOrder(root->right);
		}
	}

	void Destroy(Node*& root)
	{
		if (root)
		{
			Destroy(root->left);
			Destroy(root->right);
			delete root;
			root = nullptr;
		}
	}

private:
	Node* head;	//指向红黑树中的头结点
	size_t _size;
};

struct KOFD
{
	int operator()(int data)
	{
		return data;
	}
};

void TestRBTree()
{
	int array[] = { 5, 3, 4, 1, 7, 8, 2, 6, 0, 9 };
	RBTree<int, KOFD> t;
	for (auto e : array)
		t.Insert(e);

	// 验证红黑树正确性：
	// 1. 是否为二叉树搜索树-->检测中序遍历是否为有序序列
	t.InOrder();

	// 2. 验证红黑树性质--
	if (t.IsValidRBTree())
	{
		cout << "t is a RBTree!!!" << endl;
	}
	else
	{
		cout << "t is not a RBTree!!!" << endl;
	}

	auto it = t.begin();
	while (it != t.end())
	{
		cout << *it << " ";
		it++;
	}
	cout << endl;

	for (auto e : t)
		cout << e << " ";
	cout << endl;
}