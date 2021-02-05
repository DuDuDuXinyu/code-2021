
/*
���Ź�ʽ

#include <iostream>
using namespace std;
int main() {
	long long d[21] = { 0, 0, 1 };
	long long f[21] = { 0, 1, 2 };
	for (int i = 3; i < 21; ++i) {
		//���ŵĵ��ƹ�ʽ
		d[i] = (i - 1) * (d[i - 1] + d[i - 2]);
		//�׳˵ĵ��ƹ�ʽ
		f[i] = i * f[i - 1];
	}
	int n;
	while (cin >> n) {
		//��100.0���ѽ�������double��������λС��
		printf("%.2f%%\n", 100.0 * d[n] / f[n]);
	}
	return 0;
}

*/


//�����б�
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main()
{
	string s;
	while (getline(cin, s))
	{
		string ans;
		vector<string> ret;
		getline(cin, ans);

		int i = 0;

		while (i < s.size() && s[i] != '\n')
		{
			while (i < s.size() && s[i] == ' ')
				i++;
			if (s[i] == '\"')
			{
				i++;
				string temp;
				while (s[i] != '\"')
					temp += s[i++];
				i += 2;
				ret.push_back(temp);
			}
			else
			{
				string temp;
				temp += s[i++];
				while (i < s.size() && s[i] != ',')
					temp += s[i++];
				i++;
				ret.push_back(temp);
			}
		}
		/*for (auto e : ret)
			cout << e;
		cout << endl;*/
		int flag = 1;
		for (auto e : ret)
		{
			if (e.compare(ans) == 0)
			{
				cout << "Ignore" << endl;
				flag = 0;
				break;
			}
		}
		if (flag)
			cout << "Important!" << endl;
	}
	return 0;
}