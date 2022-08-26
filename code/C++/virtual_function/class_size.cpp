#include <iostream>

// #pragma pack(1) 对齐规则对clss同样生效

/*
首先，类大小的计算遵循结构体的对齐原则
类的大小与普通数据成员有关，与成员函数和静态成员无关。即普通成员函数，静态成员函数，静态数据成员，静态常量数据成员均对类的大小无影响
虚函数对类的大小有影响，是因为虚函数表指针带来的影响
虚继承对类的大小有影响，是因为虚基表指针带来的影响
空类的大小是一个特殊情况,空类的大小为1
*/

class Empty {};
class EmptyArray {
    int a[0];
};
class A {
    int a;
    char b; // C++也存在内存对齐
    double c;
    void func() {} // 不占内存空间
};
class VA {
    // 多个虚函数只占用8个字节 也就是64位指针的大小 虚表指针
    virtual void a_func_0() {};
    virtual void a_func_1() {};
};
class VB {
    virtual void b_func_0() {};
    virtual void b_func_1() {};
};
// 单继承继承一个续表指针
class SingleC : VA {
};
// 多继承继承所有的虚表指针
class MultipleC : VA, VB {
};
// 继承者的虚函数表使用的第一个继承的父类
class MultipleC_newV : VA, VB {
    virtual void func_local() {}
};
int main()
{
    std::cout << sizeof (Empty) << std::endl;
    std::cout << sizeof (EmptyArray) << '\n';
    std::cout << sizeof (A) << '\n';
    std::cout << sizeof (VA) << '\n';
    std::cout << sizeof (SingleC) << '\n';
    std::cout << sizeof (MultipleC) << '\n';
    std::cout << sizeof (MultipleC_newV) << '\n';
}