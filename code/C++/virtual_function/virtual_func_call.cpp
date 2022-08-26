#include <iostream>
#include <stdio.h>
using namespace std;

class Base1 {
public:
	virtual void A() { cout << "Base1 A()" << endl; }
	virtual void B() { cout << "Base1 B()" << endl; }
	virtual void C() { cout << "Base1 C()" << endl; }
};

class Base2 {
public:
	virtual void D() { cout << "Base2 D()" << endl; }
	virtual void E() { cout << "Base2 E()" << endl; }
};

class Base3 {
	virtual void F() { cout << "Base3 F()" << endl; }
	virtual void G() { cout << "Base3 G()" << endl; }
};

class Derive : public Base1, public Base2, public Base3{
public:
	virtual void A() { cout << "Derive A()" << endl; }           // 覆盖Base1::A()
	virtual void D() { cout << "Derive D()" << endl; }           // 覆盖Base2::D()
	virtual void F() { cout << "Derive F()" << endl; }			// 覆盖Base3::F()
	virtual void MyA() { cout << "Derive MyA()" << endl; }
};

int main()
{
	typedef void (*Func)();
	Derive d;
	Base1 &b1 = d;
	Base2 &b2 = d;
	cout << "Derive对象所占的内存大小为：" << sizeof(d) << endl;
	
	cout << "\n---------第一个虚函数表-------------" << endl;
	long* tmp1 = (long *)&d;              // 获取第一个虚函数表的指针
	long* vptr1 = (long*)(*tmp1);         // 获取虚函数表

	Func x1 = (Func)vptr1[0];
	Func x2 = (Func)vptr1[1];
	Func x3 = (Func)vptr1[2];
	Func x4 = (Func)vptr1[3];
	Func x5 = (Func)vptr1[4];
	Func x6 = (Func)vptr1[5];
	x1();x2();x3();x4();x5();x6();

	cout << "\n---------第二个虚函数表-------------" << endl;
	long* tmp2 = tmp1 + 1;               // 获取第二个虚函数表指针 相当于跳过8个字节
	long* vptr2 = (long*)(*tmp2);

	Func y1 = (Func)vptr2[0];
	Func y2 = (Func)vptr2[1];
	y1(); y2();

	cout << "\n---------第三个虚函数表-------------" << endl;
	long* tmp3 = tmp1 + 2;               // 获取第三个虚函数表指针 相当于跳过8个字节
	long* vptr3 = (long*)(*tmp3);

	Func z1 = (Func)vptr3[0];
	Func z2 = (Func)vptr3[1];
	z1(); z2();
	return 0;
}