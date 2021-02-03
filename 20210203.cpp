
/*¼ô»¨²¼Ìõ
#include <iostream>
#include <string>
using namespace std;

int main()
{
	string s1, s2;
	while (cin >> s1 >> s2)
	{
		int count = 0;
		size_t pos = 0;
		while ((pos = s1.find(s2, pos)) != s1.npos)
		{
			count++;
			pos += s2.size();
		}
		cout << count << endl;
	}

	return 0;
}
*/


#include <iostream>
#include <vector>
using namespace std;

int main()
{
	vector<long> v{ 1,1 };
	for (int i = 2; i < 80; i++)
		v.push_back(v[i - 1] + v[i - 2]);

	int begin, end;
	while (cin >> begin >> end)
	{
		long count = 0;
		begin--;
		while (begin < end)
			count += v[begin++];
		cout << count << endl;
	}

	return 0;
}