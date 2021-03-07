//#include <iostream>
//#include <string>
//using namespace std;
//
//int main()
//{
//	string s1, s2;
//	while (getline(cin, s1) && getline(cin, s2))
//	{
//		string s;
//		for (auto e : s1)
//		{
//			if (s2.find(e) == s2.npos)
//				s.push_back(e);
//		}
//		cout << s << endl;
//	}
//
//	return 0;
//}


//#include <iostream>
//#include <vector>
//using namespace std;
//
//int main()
//{
//	int num;
//	while (cin >> num)
//	{
//		vector<int> v;
//		while (num--)
//		{
//			int x;
//			cin >> x;
//			v.push_back(x);
//		}
//
//		//遍历寻找
//		int count = 0;
//		int max = 0;
//		for (auto e : v)
//		{
//			count += e;
//			max = count > max ? count : max;
//			if (count < 0)
//				count = 0;
//		}
//		cout << max << endl;
//	}
//
//	return 0;
//}



//24点游戏算法----穷举
#include <iostream>
using namespace std;

bool Calculate(int x, int y, int z, int w) //运算表达式的所有情况
{
	if (x + y + z + w == 24) return true;
	else if (x + y + z - w == 24) return true;
	else if ((x + y) * (z + w) == 24) return true;
	else if ((x - y) * (z + w) == 24) return true;
	else if ((x - y) * (z - w) == 24) return true;
	else if ((x + y + z) * w == 24)return true;
	else if ((x - y - z) * w == 24)return true;
	else if ((x + y - z) * w == 24) return true;
	else if (w != 0 &&(x * y * z) / w == 24) return true;
	else if ((x * y) * (z + w) == 24) return true;
	else if ((x * y) * (z - w) == 24) return true;
	else if ((x * y) * z - w == 24) return true;
	else if ((x * y) * z + w == 24) return true;
	else if (x * y * z * w == 24) return true;
	else if (w != 0 && (x + y) + (z / w) == 24) return true;
	else if (w != 0 && (x + y) * (z / w) == 24) return true;
	else if ((x * y) + z + w == 24) return true;
	else if ((x * y) + z - w == 24) return true;
	else if (w != 0 && (x * y) - (z / w) == 24) return true;
	else if (w != 0 && (x * y) + (z / w) == 24) return true;
	else if ((x * y) - z - w == 24) return true;
	else if ((x * y) + (z * w) == 24) return true;
	else if ((x * y) - (z * w) == 24) return true;
	else if ((z * w) != 0 && (x * y) / (z * w) == 24) return true;
	else if ((z - w) != 0 && (x * y) / (z - w) == 24) return true;
	else if ((z + w) != 0 && (x * y) / (z + w) == 24) return true;
	else return false;
}

bool twenty_four(int a, int b, int c, int d){ // a .b.c.d 的所有排列组合情况
	return Calculate(a, b, c, d) || Calculate(a, b, d, c) || Calculate(a, c, d, b) ||
		Calculate(a, c, b, d) || Calculate(a, d, b, c) || Calculate(a, d, c, b) ||
		Calculate(b, a, c, d) || Calculate(b, a, d, c) || Calculate(b, c, a, d) ||
		Calculate(b, c, d, a) || Calculate(b, d, c, a) || Calculate(b, d, a, c) ||
		Calculate(c, a, b, d) || Calculate(c, a, d, b) || Calculate(c, b, d, a) ||
		Calculate(c, b, a, d) || Calculate(c, d, a, b) || Calculate(c, d, b, a) ||
		Calculate(d, a, b, c) || Calculate(d, a, c, b) || Calculate(d, b, c, a) ||
		Calculate(d, b, a, c) || Calculate(d, c, a, b) || Calculate(d, c, b, a);
}


int main()
{
	int a, b, c, d;
	while (cin >> a >> b >> c >> d)
	{
		if (twenty_four(a, b, c, d))
			cout << "true" << endl;
		else
			cout << "false" << endl;
	}

	return 0;
}
