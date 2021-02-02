#include <iostream>

using namespace std;

bool One(int x)
{
	if (x == 2 || x == 3 || x == 5 || x == 7 || x == 11)
		return true;
	return false;
}

int main()
{
	int begin_year, begin_month, begin_day;
	int end_year, end_month, end_day;
	int month_day[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
	int leap_month_day[] = { 0, 31,29,31,30,31,30,31,31,30,31,30,31 };
	while (cin >> begin_year >> begin_month >> begin_day >> end_year >> end_month >> end_day)
	{
		int money = 0;
		if (begin_year < end_year)
		{
			if ((begin_year - 2000) % 4 == 0)
			{
				//闰年
				//算所在月的日子算完
				if (One(begin_month))
					money += leap_month_day[begin_month] - begin_day + 1;
				else
					money += 2 * (leap_month_day[begin_month] - begin_day + 1);
				//再算间隔年的日子
				begin_month++;
				while (begin_month <= 12)
				{
					if (One(begin_month))
						money += leap_month_day[begin_month];
					else
						money += 2 * leap_month_day[begin_month];
					begin_month++;
				}
				//间隔几年的日子
				begin_year++;
				while (begin_year < end_year)
				{
					if ((begin_year - 2000) % 4 == 0)
					{
						for (int i = 1; i <= 12; i++)
						{
							if (One(i))
								money += leap_month_day[i];
							else
								money += 2 * leap_month_day[i];
						}
					}
					else
					{
						for (int i = 1; i <= 12; i++)
						{
							if (One(i))
								money += month_day[i];
							else
								money += 2 * month_day[i];
						}
					}
					begin_year++;
				}
				//最后一年的日子
				if ((begin_year - 2000) % 4 == 0)
				{
					int new_month = 1;
					while (new_month < end_month)
					{
						if (One(begin_month))
							money += leap_month_day[begin_month];
						else
							money += 2 * leap_month_day[begin_month];
						begin_month++;
					}
					if (One(end_month))
						money += end_day;
					else
						money += 2 * end_day;
				}
				else
				{
					int new_month = 1;
					while (new_month < end_month)
					{
						if (One(begin_month))
							money += month_day[begin_month];
						else
							money += 2 * month_day[begin_month];
						begin_month++;
					}
					if (One(end_month))
						money += end_day;
					else
						money += 2 * end_day;
				}
			}
			else
			{
				//不是闰年
				//算所在月的日子算完
				if (One(begin_month))
					money += month_day[begin_month] - begin_day + 1;
				else
					money += 2 * (month_day[begin_month] - begin_day + 1);
				//再算间隔年的日子
				begin_month++;
				while (begin_month <= 12)
				{
					if (One(begin_month))
						money += month_day[begin_month];
					else
						money += 2 * month_day[begin_month];
					begin_month++;
				}
				//间隔几年的日子
				begin_year++;
				while (begin_year < end_year)
				{
					if ((begin_year - 2000) % 4 == 0)
					{
						for (int i = 1; i <= 12; i++)
						{
							if (One(i))
								money += leap_month_day[i];
							else
								money += 2 * leap_month_day[i];
						}
					}
					else
					{
						for (int i = 1; i <= 12; i++)
						{
							if (One(i))
								money += month_day[i];
							else
								money += 2 * month_day[i];
						}
					}
					begin_year++;
				}
				//最后一年的日子
				if ((begin_year - 2000) % 4 == 0)
				{
					int new_month = 1;
					while (new_month < end_month)
					{
						if (One(begin_month))
							money += leap_month_day[begin_month];
						else
							money += 2 * leap_month_day[begin_month];
						begin_month++;
					}
					if (One(end_month))
						money += end_day;
					else
						money += 2 * end_day;
				}
				else
				{
					int new_month = 1;
					while (new_month < end_month)
					{
						if (One(begin_month))
							money += month_day[begin_month];
						else
							money += 2 * month_day[begin_month];
						begin_month++;
					}
					if (One(end_month))
						money += end_day;
					else
						money += 2 * end_day;
				}
			}
		}
		else
		{
			//在同一年
			if (begin_month < end_month)
			{
				if ((begin_year - 2000) % 4 == 0)
				{
					//闰年
					//先把所在月的日子算完
					if (One(begin_month))
						money += leap_month_day[begin_month] - begin_day + 1;
					else
						money += 2 * (leap_month_day[begin_month] - begin_day + 1);
					//在算间隔月的日子
					begin_month++;
					while (begin_month < end_month)
					{
						if (One(begin_month))
							money += leap_month_day[begin_month];
						else
							money += 2 * leap_month_day[begin_month];
						begin_month++;
					}
					//计算最后的几天
					if (One(begin_month))
						money += end_day;
					else
						money += 2 * end_day;
				}
				else
				{
					//不是闰年
					//先把所在月的日子算完
					if (One(begin_month))
						money += month_day[begin_month] - begin_day + 1;
					else
						money += 2 * (month_day[begin_month] - begin_day + 1);
					//在算间隔月的日子
					begin_month++;
					while (begin_month < end_month)
					{
						if (One(begin_month))
							money += month_day[begin_month];
						else
							money += 2 * month_day[begin_month];
						begin_month++;
					}
					//计算最后的几天
					if (One(begin_month))
						money += end_day;
					else
						money += 2 * end_day;
				}
			}
			else     //在同一个月
			{
				if (One(end_month))
					money += (end_day - begin_day + 1);
				else
					money += 2 * (end_day - begin_day + 1);
			}
		}
		cout << money << endl;
	}

	return 0;
}
