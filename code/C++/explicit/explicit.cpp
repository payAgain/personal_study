#include <iostream>

class Test{
public:
    int a;
    float c;
    Test(int num) : a(num), c(0) {}    
    Test(float fnum, int inum = 10) : a(inum), c(fnum) {} 
};

int main()
{
    // 隐式类型转换
    Test t1 = 10;
    Test t2 = 20.0f;
    std::cout << t1.a << ' ' << t1.c << std::endl;
    std::cout << t2.a << ' ' << t2.c << std::endl;

}