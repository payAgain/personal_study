#include <iostream>
#include <string>
#include <vector>
class A;
/*
    踩坑了很久：
        class C 必须在A之前定义！
        class A必须在 C之前声明！
        不能在定义中，使用不完全类型！
*/
class C {
public:
    void visitA(const A &a);
};
class A {
    std::string name;
    int age;
public: 
    // 重载左移运算符 这里必须要使用引用
    // 如果不使用引用，在传递值的时候会调用复制构造函数
    // ostream是不允许进行复制
    friend std::ostream& operator<<(std::ostream &cout, A a);
    friend bool operator < (const A& a, const A& b);

    // 友元类
    friend class B;
    // 友元成员函数
    friend void C::visitA(const A &a);
    //构造函数
    A(std::string name, int age) : name(name), age(age){}
};
class B {
public:
    void visitA(const A &a);
};
std::ostream& operator<<(std::ostream &out, A a) {
    out << "Name: " << a.name << "  age: " << a.age << std::endl;
    return out;
}

bool operator < (const A& a, const A& b) {
    return a.age < b.age;
}

void B::visitA(const A &a) {
        std::cout << "当前为class B访问" << std::endl;
        std::cout << a;
}

void C::visitA(const A &a) {
        std::cout << "当前为class C访问" << std::endl;
        std::cout << a;
}

int main()
{
    std::vector<A> a;
    a.push_back(A("张三", 19));
    a.push_back(A("李四", 17));
    std::sort(a.begin(), a.end());
    for (auto &item : a) {
        std::cout << item;
    }
    // 友元类访问
    B().visitA(a[0]);
    C().visitA(a[0]);
}