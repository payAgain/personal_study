#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char __executable_start[];
extern char _etext[];
extern char _edata[];
extern char _end[];

int main()
{
    printf("Executable Start %X\n",__executable_start);
    printf("Text Section End %X\n",_etext);
    printf("Data Section End %X\n", _edata);
    printf("Program End %X\n", _end);
    // 不能够尝试去访问内存
    // unsigned int data_end = (unsigned int)_edata-8*8;
    // int *p = (int *)data_end;
    // printf("%X\n", data_end);
    // printf("%d\n", *p);
    
    // 测试输出
    // unsigned int p=0x3f3f3f3f;
    // char *t=(char *)&p;
    // unsigned int* x=&p;
    // printf("%X  %X  %X\n", t, &p, x);
    return 0;
}