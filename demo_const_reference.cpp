#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
	 int a = 5;
	const int& ref = a;
	// a = 4;
	printf("%d\n", a);
	printf("%p\n", &ref);
	int* p = &a;
	string t = "hello";
	string s = t.substr(1, 2);
	cout << s << endl;
	return 0;
}