#pragma once

enum Color{RED, BLACK};

//红黑树节点的定义
template <class T>
struct RBTreeNode
{
	RBTreeNode(const T& x, Color c = RED)
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
		if (RED == parent->color)
		{

		}

		//新节点插入后，红色树中的最大或者最小节点发送变化
		root->color = BLACK;
		head->left = LeftMost();
		head->right = RightMost();

		return true;
	}

private:
	Node* GetRoot()
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
private:
	Node* head;	//指向红黑树中的头结点
};