#pragma once
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
bool compareStrings(string s1, string s2)
{
	if (s1.size() != s2.size())
	{
		cout << "string size not equal" << endl << "s1 " << s1.size() << endl << s2.size();
		return false;
	}
	for (int i = 0; i < s1.size(); i++)
		if (s1[i] != s2[i])
		{
			cout << "at char "<<i <<"  "<< s1[i] << " != " << s2[i] << endl;
			return false;
		}
	return true;
}
bool compareFiles(string file1, string file2)
{
	ifstream F1,F2;
	F1.open(file1);
	F2.open(file2);
	if (!F1)
	{
		cout << "error opening the file" << endl;
	}
	if (!F1)
	{
		cout << "error opening the file" << endl;
	}
	string str1, str2;
	int c1, c2;
	c1 = 0; c2 = 0;
	string str;
	while (!F1.eof())
	{
		getline(F1, str);
		c1++;
	}
	while (!F2.eof())
	{
		getline(F2, str);
		c2++;
	}
	if (c1 != c2)
	{
		F1.close();
		F2.close();
		cout << "file size not equal" << endl;
		return false;
	}
	F1.close();
	F2.close();
	F1.open(file1);
	F2.open(file2);
	while (!F1.eof() && !F2.eof())
	{
		F1 >> str1;
		F2 >> str2;
		if (!compareStrings(str1, str2))
		{
			F1.close();
			F2.close();
			return false;
		}
	}

	F1.close();
	F2.close();
	return true;
}
