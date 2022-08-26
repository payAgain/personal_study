#include <iostream>

// 定义对齐为4
// #pragma pack(1)


typedef struct A {
    char c[0];
} AA;

int main()
{
    AA a[10];
    std::cout << sizeof(AA) << ' ' << alignof(AA) << std::endl;
    std::cout << sizeof(a) << ' ' << alignof(AA) << std::endl;
}