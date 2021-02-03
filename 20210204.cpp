
/*收件人列表
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main()
{
	int n;
	while (cin >> n)
	{
		vector<string> v;
		getchar();
		while (n--)
		{
			string buf;
			getline(cin, buf);
			if (buf.find(" ") != buf.npos || buf.find(",") != buf.npos)
			{
				buf.insert(buf.begin(), '\"');
				buf += "\"";
			}
			v.push_back(buf);
		}
		int i = 0;
		for (auto& e : v)
		{
			
			if (i == v.size() - 1)
				break;
			cout << e << ", ";
			i++;
		}
		cout << v[v.size() - 1] << endl;
	}
	return 0;
}*/

//养兔子
#include <iostream>
#include <vector>
using namespace std;

int main()
{
	vector<long> v{ 1,1,2 };
	for (int i = 3; i <= 90; i++)
		v.push_back(v[i - 1] + v[i - 2]);
	int n = 0;
	while (cin >> n)
		cout << v[n] << endl;

	return 0;
}