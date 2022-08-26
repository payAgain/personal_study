#include <iostream>
using namespace std;

// 虚函数在哪？
class A{
public:
	int x;
	virtual void b() {}
};

int main()
{
	A* p = new A;
	cout << p << endl;
	cout << &p->x << endl;
	return 0;
}