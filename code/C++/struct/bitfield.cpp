#include <iostream>


typedef struct A {
    int a : 4;
    char d : 4;
    int b : 25;
} AA;

int main()
{
    std::cout << sizeof (AA) << std::endl;
}