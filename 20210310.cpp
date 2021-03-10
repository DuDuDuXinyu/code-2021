/*#include <iostream>
#include <string>
#include <vector>
#include <algorithm>  
using namespace std;

int main()
{
	int num;
	while (cin >> num)
	{
		getchar();
		int max = 0;
		vector<char> v;
		while (num--)
		{
			string s;
			getline(cin, s);
			char name = s[0];
			if (find(v.begin(), v.end(), name) == v.end())
				v.push_back(name);
			else
				v.erase(find(v.begin(), v.end(), name));
			max = v.size() > max ? v.size() : max;
		}
		cout << max << endl;
	}

	return 0;
}
*/

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;

int main()
{
	int num;
	while (cin >> num)
	{
		getchar();
		vector<string> v;
		vector<string> ret;
		while (num--)
		{
			string s;
			getline(cin, s);
			v.push_back(s);
		}
		sort(v.begin(), v.end());

		for (int i = 0; i < v.size() - 1; i++)
		{
			if (v[i + 1].find(v[i]) == v[i + 1].npos || v[i + 1][v[i].size()] != '/')
				cout << "mkdir -p " << v[i] << endl;
		}
		cout << "mkdir -p " << v[v.size() - 1] << endl << endl;

	}

	return 0;
}