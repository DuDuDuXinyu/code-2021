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

template <class T>
class RBTree
{
	typedef RBTreeNode<T> Node;
public:
	RBTree()
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
	}

	bool Insert(const T& data)
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
			return true;
		}

		//找到待插入节点在红黑树中的位置
		Node* cur = root;
		Node* parent = head;
		while (cur)
		{
			parent = cur;
			if (data < cur->data)
				cur = cur->left;
			else if (data > cur->data)
				cur = cur->right;
			else
				return false;
		}

		//插入新节点
		cur = new Node(data);
		if (data < parent->data)
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

		return true;
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
};

void TestRBTree()
{
	int array[] = { 5, 3, 4, 1, 7, 8, 2, 6, 0, 9 };
	RBTree<int> t;
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
}