#include <fmt/core.h>
#include <vector>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <ctype.h>
using namespace std;
bool isNumeric(const std::string& str)
{
	for (char c : str)
	{
		if (!std::isdigit(c))
		{
			return false;
		}
	}
	return true;
}
int main()
{
	ifstream file;
	file.open("config.txt");
	if (!file.is_open())
	{
		file.close();
		ofstream fout("config.txt");
		if (fout)
		{
			fout.close();
			file.open("config.txt");
		}
		else
		{
			fmt::print("打开文本错误!\n");
			system("pause");
			exit(1);
		}
	}
	vector<string> v;
	int num = 0;
	while (!file.eof())
	{
		string temp;
		getline(file, temp);
		if (temp.find("//") != string::npos)
			continue;
		v.push_back(temp);
		++num;
	}
	if (num <= 1)
	{
		fmt::print("错误!\nconfig.txt文件为空或只有一个组ID!\n");
		system("pause");
		exit(1);
	}
	fmt::print("========================================\n共有{}个组ID被加载:\n========================================\n", num);
	for (size_t i = 0; i < num; i++)
	{
		fmt::print("{}.{}\n", i + 1, v[i]);
	}
	fmt::print("========================================\n");
	for (size_t i = 0; i < num; i++)
	{
		if (!isNumeric(v[i]))
		{
			fmt::print("警告:您输入的组ID可能有误!\n");
			break;
		}
	}
	unsigned int m = 500;
	fmt::print("请输入切换频率(ms):");
	cin >> m;
	while (cin.fail())
	{
		cin.clear();
		cin.ignore(10, '\n');
		fmt::print("请输入切换频率(ms):");
		cin >> m;
	}
	COPYDATASTRUCT m_cData;
	m_cData.dwData = 0;
	while (true)
	{
		static int now = 0;
		HWND m_hEngine = FindWindowA("Valve001", 0);
		while (!m_hEngine)
		{
			static int retry = 0;
			cout << "没有找到CSGO窗口! [" << ++retry << "]" << endl;
			Sleep(5000);
		}
		m_cData.cbData = strlen(("cl_clanid " + v[now]).c_str()) + 1;
		m_cData.lpData = (void*)("cl_clanid " + v[now]).c_str();
		SendMessageA(m_hEngine, WM_COPYDATA, 0, (LPARAM)&m_cData);
		fmt::print("正在显示第{}个组名\n", now + 1);
		std::this_thread::sleep_for(std::chrono::milliseconds(m));
		now <= num - 2 ? ++now : now = 0;
	}
	system("pause");
	return 0;
}