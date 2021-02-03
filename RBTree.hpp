#pragma once
#include <iostream>
using namespace std;

enum Color{RED, BLACK};

//������ڵ�Ķ���
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
		//���ն����������Ĺ�������½ڵ�
		Node*& root = GetRoot();

		//����
		if (nullptr == root)
		{
			root = new Node(data, BLACK);
			root->parent = head;
			head->left = root;
			head->right = root;
			return true;
		}

		//�ҵ�������ڵ��ں�����е�λ��
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

		//�����½ڵ�
		cur = new Node(data);
		if (data < parent->data)
			parent->left = cur;
		else
			parent->right = cur;

		cur->parent = parent;

		//����²���ڵ�����˫�׽ڵ����ɫ�Ƿ�Ϊ��ɫ
		while (parent != head && RED == parent->color)
		{
			Node* grandFather = parent->parent;
			if (parent == grandFather->left)
			{
				Node* uncle = grandFather->right;
				if (uncle && RED == uncle->color)
				{
					//���һ������ڵ������Ϊ��
					parent->color = BLACK;
					uncle->color = BLACK;
					grandFather->color = RED;
					cur = grandFather;
					parent = cur->parent;
				}
				else
				{
					//����ڵ㲻����-->�����
					//��������ڵ������Ϊ��ɫ-->�����
					if (cur == parent->right)
					{
						//�������
						RotateLeft(parent);
						swap(parent, cur);
					}
					//�������
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
					//���һ������ڵ������Ϊ��
					parent->color = BLACK;
					uncle->color = BLACK;
					grandFather->color = RED;
					cur = grandFather;
					parent = cur->parent;
				}
				else
				{
					//����ڵ㲻����-->�����
					//��������ڵ������Ϊ��ɫ-->�����
					if (cur == parent->left)
					{
						//�������
						RotateRight(parent);
						swap(parent, cur);
					}
					//�������
					grandFather->color = RED;
					parent->color = BLACK;
					RotateLeft(grandFather);
				}
			}
		}

		//�½ڵ����󣬺�ɫ���е���������С�ڵ㷢�ͱ仯
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
			cout << "Υ�������ʶ������ڵ����ɫ���Ǻ�ɫ��" << endl;
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
		//head != parent˵��root���Ǹ��ڵ㣬parentΪ��Ч�ڵ�
		if (head != parent && RED == parent->color && RED == root->color)
		{
			cout << "Υ��������������������һ��ĺ�ɫ�ڵ�" << endl;
			return false;
		}

		if (nullptr == root->left && nullptr == root->right)
		{
			if (pathBlackCount != blackCount)
			{
				cout << "Υ���������ģ�·���к�ɫ�ڵ�ĸ�����һ��" << endl;
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
		//��Ҫ������ת֮ǰparent˫�׵����
		if (head == pparent)
		{
			//��ת֮ǰparent�Ǹ��ڵ�
			head->parent = subR;
		}
		else
		{
			//��ת֮ǰ��˫��
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
	Node* head;	//ָ�������е�ͷ���
};

void TestRBTree()
{
	int array[] = { 5, 3, 4, 1, 7, 8, 2, 6, 0, 9 };
	RBTree<int> t;
	for (auto e : array)
		t.Insert(e);

	// ��֤�������ȷ�ԣ�
	// 1. �Ƿ�Ϊ������������-->�����������Ƿ�Ϊ��������
	t.InOrder();

	// 2. ��֤���������--
	if (t.IsValidRBTree())
	{
		cout << "t is a RBTree!!!" << endl;
	}
	else
	{
		cout << "t is not a RBTree!!!" << endl;
	}
}